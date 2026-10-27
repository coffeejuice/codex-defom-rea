"""CP302 fake WinAPI checks; no process is created or attached."""
import ctypes
import hashlib
import importlib.util
from pathlib import Path
import threading
import unittest
from unittest import mock


SPEC = importlib.util.spec_from_file_location(
    "cp214_owned_suspended_process", Path(__file__).with_name("cp214_owned_suspended_process.py"))
owned = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(owned)


class FakeKernel:
    def __init__(self, *, waits=(), terminate=True, close=(), query=True,
                 exit_code=True):
        self.waits = list(waits)
        self.terminate_ok = terminate
        self.close_outcomes = list(close)
        self.query_ok = query
        self.exit_code_ok = exit_code
        self.calls = []

    def CreateProcessW(self, _exe, _command, _a, _b, _inherit, _flags,
                       _env, _cwd, _si, pi):
        self.calls.append("create")
        pi._obj.hProcess = 101
        pi._obj.hThread = 202
        pi._obj.dwProcessId = 303
        pi._obj.dwThreadId = 404
        return 1

    def GetProcessTimes(self, _handle, *times):
        self.calls.append("times")
        times[0]._obj.dwLowDateTime = 77
        return 1

    def QueryFullProcessImageNameW(self, _handle, _flags, _buf, _size):
        self.calls.append("query")
        return int(self.query_ok)

    def WaitForSingleObject(self, handle, timeout):
        self.calls.append(("wait", handle, timeout))
        return self.waits.pop(0) if self.waits else 0xffffffff

    def GetExitCodeProcess(self, _handle, code):
        self.calls.append("exit_code")
        if not self.exit_code_ok:
            return 0
        code._obj.value = 0xe201
        return 1

    def TerminateProcess(self, handle, code):
        self.calls.append(("terminate", handle, code))
        return int(self.terminate_ok)

    def CloseHandle(self, handle):
        self.calls.append(("close", handle))
        return int(self.close_outcomes.pop(0)) if self.close_outcomes else 1


def make_child(fake):
    child = owned.OwnedProcess.__new__(owned.OwnedProcess)
    child.k = fake
    child.lock = threading.RLock()
    child.pi = owned.PROCESS_INFORMATION()
    child.pi.hProcess = 101
    child.pi.hThread = 202
    child.resumed = False
    child.termination_attempts = []
    child.identity = None
    child._exit_verified = False
    child._last_exit_receipt = None
    return child


class ExactHandleCleanupTests(unittest.TestCase):
    def setUp(self):
        self.winerror = mock.patch.object(owned.C, "WinError", side_effect=lambda code: OSError(code), create=True)
        self.last_error = mock.patch.object(owned.C, "get_last_error", return_value=5, create=True)
        self.winerror.start()
        self.last_error.start()
        self.addCleanup(self.winerror.stop)
        self.addCleanup(self.last_error.stop)

    def test_failed_terminate_and_no_race_exit_retains_both_handles(self):
        fake = FakeKernel(waits=[258, 258], terminate=False)
        child = make_child(fake)
        with self.assertRaises(OSError):
            child.close()
        self.assertEqual((child.pi.hProcess, child.pi.hThread), (101, 202))
        self.assertFalse(any(call[0] == "close" for call in fake.calls if isinstance(call, tuple)))
        self.assertEqual([call for call in fake.calls if isinstance(call, tuple) and call[0] == "wait"],
                         [("wait", 101, 0), ("wait", 101, 5000)])
        fake.terminate_ok = True
        fake.waits = [258, 0, 0]
        receipt = child.close()
        self.assertTrue(receipt["exit_verified"])
        self.assertEqual((child.pi.hProcess, child.pi.hThread), (None, None))

    def test_successful_terminate_but_timeout_or_wait_failure_retains_handles(self):
        for wait_result in (258, 0xffffffff):
            with self.subTest(wait_result=wait_result):
                fake = FakeKernel(waits=[258, wait_result])
                child = make_child(fake)
                with self.assertRaisesRegex(RuntimeError, "termination not verified"):
                    child.close()
                self.assertEqual((child.pi.hProcess, child.pi.hThread), (101, 202))
                self.assertFalse(any(isinstance(call, tuple) and call[0] == "close" for call in fake.calls))

    def test_status_exit_code_failure_keeps_exact_handles_for_retry(self):
        fake = FakeKernel(waits=[0], exit_code=False)
        child = make_child(fake)
        with self.assertRaises(OSError):
            child.close()
        self.assertEqual((child.pi.hProcess, child.pi.hThread), (101, 202))
        self.assertFalse(any(isinstance(call, tuple) and call[0] == "close" for call in fake.calls))
        fake.exit_code_ok = True
        fake.waits = [0, 0, 0]
        self.assertTrue(child.close()["exit_verified"])
        self.assertEqual((child.pi.hProcess, child.pi.hThread), (None, None))

    def test_failed_terminate_accepts_exit_on_exact_handle_and_closes(self):
        fake = FakeKernel(waits=[258, 0, 0, 0], terminate=False)
        child = make_child(fake)
        receipt = child.close()
        self.assertTrue(receipt["exit_verified"])
        self.assertEqual(receipt["termination_attempts"], [{
            "source": "final_cleanup", "requested_exit_code": 0xe201,
            "api_succeeded": False, "winerror": 5,
            "post_failure_wait_result": 0,
            "exit_during_termination_race": True,
            "wait_result": 0, "exit_code": 0xe201, "exit_verified": True}])
        self.assertEqual((child.pi.hProcess, child.pi.hThread), (None, None))
        self.assertEqual([call for call in fake.calls if isinstance(call, tuple) and call[0] == "wait"],
                         [("wait", 101, 0), ("wait", 101, 5000),
                          ("wait", 101, 5000), ("wait", 101, 0)])

    def test_each_close_failure_retains_only_its_exact_handle_then_retries(self):
        for close_outcomes, expected in [([False, True], (None, 202)),
                                         ([True, False], (101, None)),
                                         ([False, False], (101, 202))]:
            with self.subTest(close_outcomes=close_outcomes):
                fake = FakeKernel(waits=[258, 0, 0, 0, 0, 0], close=close_outcomes)
                child = make_child(fake)
                with self.assertRaisesRegex(RuntimeError, "retained for retry"):
                    child.close()
                self.assertEqual((child.pi.hProcess, child.pi.hThread), expected)
                receipt = child.close()
                self.assertTrue(receipt["exit_verified"])
                self.assertEqual((child.pi.hProcess, child.pi.hThread), (None, None))
                closes = [call for call in fake.calls if isinstance(call, tuple) and call[0] == "close"]
                self.assertEqual(len(closes), 2 + sum(bool(h) for h in expected))
                self.assertEqual(child.close(), receipt)
                self.assertEqual(len([call for call in fake.calls
                                      if isinstance(call, tuple) and call[0] == "close"]), len(closes))

    def test_natural_exit_closes_without_termination(self):
        fake = FakeKernel(waits=[0, 0, 0])
        child = make_child(fake)
        receipt = child.close()
        self.assertEqual(receipt["state"], "exited")
        self.assertFalse(receipt["termination_requested"])
        self.assertEqual(receipt["termination_attempts"], [])
        self.assertEqual((child.pi.hProcess, child.pi.hThread), (None, None))

    def test_constructor_identity_failure_preserves_original_cleanup_error_and_owner(self):
        exe = Path(__file__).resolve()  # Existing bytes; fake API never executes it.
        digest = hashlib.sha256(exe.read_bytes()).hexdigest()
        fake = FakeKernel(query=False, waits=[258, 258], terminate=False)
        with mock.patch.object(owned, "kernel", return_value=fake):
            with self.assertRaises(OSError) as caught:
                owned.OwnedProcess(str(exe), str(exe.parent), [], digest)
        error = caught.exception
        self.assertIsInstance(error.cleanup_error, OSError)
        child = error.owned_process
        self.assertEqual((child.pi.hProcess, child.pi.hThread), (101, 202))
        self.assertEqual(fake.calls[:3], ["create", "times", "query"])
        self.assertFalse(any(isinstance(call, tuple) and call[0] == "close" for call in fake.calls))
        fake.terminate_ok = True
        fake.waits = [258, 0, 0]
        self.assertTrue(child.close()["exit_verified"])
        self.assertEqual((child.pi.hProcess, child.pi.hThread), (None, None))

    def test_constructor_handle_close_failure_retains_failed_handle(self):
        exe = Path(__file__).resolve()
        digest = hashlib.sha256(exe.read_bytes()).hexdigest()
        fake = FakeKernel(query=False, waits=[258, 0, 0], close=[False, True])
        with mock.patch.object(owned, "kernel", return_value=fake):
            with self.assertRaises(OSError) as caught:
                owned.OwnedProcess(str(exe), str(exe.parent), [], digest)
        error = caught.exception
        self.assertIn("retained for retry", str(error.cleanup_error))
        child = error.owned_process
        self.assertTrue(child._exit_verified)
        self.assertEqual((child.pi.hProcess, child.pi.hThread), (None, 202))
        self.assertTrue(child.close()["exit_verified"])
        self.assertEqual((child.pi.hProcess, child.pi.hThread), (None, None))

    def test_main_failure_receipt_does_not_claim_owner_survives_exit(self):
        class FakeOwner:
            verified = False
            handles = (101, 202)

            def __init__(self, *_args):
                self.lock = threading.RLock()
                self.pi = owned.PROCESS_INFORMATION()
                self.pi.hProcess, self.pi.hThread = self.handles
                self._exit_verified = self.verified
                self.identity = {"pid": 303}

            def status(self):
                if not self._exit_verified:
                    raise OSError("fake status failure before exit verification")
                return {"state": "exited", "exit_verified": True, "wait_result": 0,
                        "exit_code": 0}

            def close(self):
                raise RuntimeError("fake cleanup failure")

        argv = ["fake", "--exe", "unused", "--cwd", "unused", "--sha256", "unused",
                "--max-seconds", "1"]
        scenarios = [
            (False, (101, 202), "uncertain_child_may_remain_live"),
            (True, (101, 202), "verified_exited_handles_nonzero_at_error"),
            (True, (None, None), "verified_exited_handles_closed"),
        ]
        for verified, handles, certainty in scenarios:
            with self.subTest(verified=verified, handles=handles):
                records = []
                FakeOwner.verified, FakeOwner.handles = verified, handles
                with (mock.patch.object(owned, "OwnedProcess", FakeOwner),
                      mock.patch.object(owned, "emit", side_effect=lambda event, **fields:
                                        records.append({"event": event, **fields})),
                      mock.patch.object(owned.sys, "argv", argv)):
                    self.assertEqual(owned.main(), 1)
                failure = records[-1]
                self.assertEqual(failure["event"], "cleanup_error")
                self.assertIs(failure["exit_verified"], verified)
                self.assertEqual(failure["handle_fields_nonzero_at_error"],
                                 {"hProcess": bool(handles[0]), "hThread": bool(handles[1])})
                self.assertIs(failure["owner_retention_after_helper_exit"], False)
                self.assertEqual(failure["cleanup_certainty"], certainty)
                self.assertNotIn("handles_open_at_error", failure)


if __name__ == "__main__":
    unittest.main()
