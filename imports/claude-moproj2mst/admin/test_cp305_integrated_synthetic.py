"""CP305 integration tests with in-memory fake operations only."""
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp303_job_creation as creation
import cp304_job_supervisor as protocol
import cp305_integrated_synthetic as m


IMAGE = creation.FixedCloneImage(
    r"C:\PROBLEM\m289_0123456789abcdef\fixture.exe",
    r"C:\PROBLEM\m289_0123456789abcdef", creation.CP302_SHA256)
TX = m.FixedTransaction(IMAGE, "cp305-0123456789abcdef")


class FakeApi:
    cp303_fake_api = True
    cp304_fake_api = True

    def __init__(self, fail=()):
        self.calls = []
        self.fail = set(fail)

    def last_error(self): return 1234
    def CreateJobObjectW(self, *_): self.calls.append("job-create"); return 71
    def GetHandleInformation(self, _h, flags):
        import ctypes as C
        from ctypes import wintypes as W
        self.calls.append("job-flags"); C.cast(flags, C.POINTER(W.DWORD))[0] = 0; return True
    def SetInformationJobObject(self, *_): self.calls.append("job-config"); return True
    def GetCurrentProcess(self): return 99
    def DuplicateHandle(self, source, handle, target, out, *_):
        import ctypes as C
        from ctypes import wintypes as W
        self.calls.append(("duplicate", source, handle))
        if "duplicate" in self.fail or (handle == 202 and "child-duplicate" in self.fail): return False
        C.cast(out, C.POINTER(W.HANDLE))[0] = 81 if handle == 71 else 303
        return True
    def GetProcessId(self, _): self.calls.append("pid"); return 123
    def GetProcessTimes(self, handle): self.calls.append(("time", handle)); return 456
    def QueryFullProcessImageNameW(self, _): self.calls.append("image"); return IMAGE.executable
    def InitializeProcThreadAttributeList(self, attr, _count, _flags, size):
        import ctypes as C
        self.calls.append("attr-init")
        if attr is None:
            C.cast(size, C.POINTER(C.c_size_t))[0] = 64
        return True
    def UpdateProcThreadAttribute(self, _list, _flags, key, value, _size, *_):
        self.calls.append(("attr-job", key))
        return True
    def DeleteProcThreadAttributeList(self, _): self.calls.append("attr-delete")
    def CreateProcessW(self, exe, command, _process_attr, _thread_attr, inherit,
                       flags, _env, cwd, _startup, out):
        import ctypes as C
        self.calls.append(("create", exe, command.value, inherit, flags, cwd))
        pi = C.cast(out, C.POINTER(creation.PROCESS_INFORMATION)).contents
        pi.hProcess, pi.hThread, pi.dwProcessId, pi.dwThreadId = 202, 203, 123, 124
        if "create-exception" in self.fail:
            raise OSError("creator interrupted")
        return "create-false" not in self.fail
    def TerminateJobObject(self, *_): self.calls.append("kill-job"); return "kill-job" not in self.fail
    def TerminateProcess(self, *_): self.calls.append("kill-worker"); return True
    def WaitForSingleObject(self, handle, ms):
        self.calls.append(("wait", handle, ms))
        if handle == 202 and "local-timeout" in self.fail:
            return protocol.WAIT_TIMEOUT
        return protocol.WAIT_TIMEOUT if handle == 42 and ms == 0 else protocol.WAIT_OBJECT_0
    def QueryInformationJobObject(self, _): self.calls.append("active"); return 0
    def CloseHandle(self, h):
        self.calls.append(("close", h))
        return h not in self.fail


def run(api, *, fail_worker=False):
    tx = m.FakeTransaction(api, TX)
    def worker(_):
        api.calls.append("worker-create")
        if fail_worker: raise OSError("worker allocation failed")
        return 42
    tx.run(worker_factory=worker,
           send_ack=lambda ack: api.calls.append("ack-send") or True,
           ack_receipt_factory=lambda ack: ack)
    return tx


class IntegratedTests(unittest.TestCase):
    def test_happy_path_cp303_fields_and_automatic_cleanup(self):
        api = FakeApi(); tx = run(api)
        self.assertLess(api.calls.index("job-config"), api.calls.index("worker-create"))
        self.assertLess(api.calls.index("worker-create"), next(i for i, c in enumerate(api.calls) if isinstance(c, tuple) and c[0] == "create"))
        self.assertLess(api.calls.index("image"), api.calls.index("ack-send"))
        self.assertEqual(tx.supervisor.offer, protocol.ChildOffer(TX.occurrence, 202, 123, 124, 456, IMAGE.executable))
        self.assertEqual(tx.report["status"], "acknowledged-unverified-receipt")
        receipt = tx.report["cleanup"]
        self.assertEqual(receipt.retained_handles, {})
        self.assertEqual(receipt.handle_closes, {"child": "succeeded", "worker": "succeeded", "job": "succeeded"})
        self.assertEqual(tx.report["local_handle_closes"], {"thread": "succeeded", "process": "succeeded"})
        self.assertEqual(tx.report["retained_local_handles"], {})
        self.assertEqual(tx.cleanup().retained_handles, {})

    def test_worker_failure_cleans_job_without_starting_or_offer(self):
        api = FakeApi(); tx = m.FakeTransaction(api, TX)
        with self.assertRaises(OSError):
            tx.run(worker_factory=lambda _: (_ for _ in ()).throw(OSError("no worker")),
                   send_ack=lambda _: self.fail("ACK must not be sent"),
                   ack_receipt_factory=lambda _: None)
        self.assertEqual(tx.report["status"], "failed-or-uncertain")
        self.assertFalse(any(isinstance(c, tuple) and c[0] == "create" for c in api.calls))
        self.assertEqual(tx.report["cleanup"].retained_handles, {})

    def test_ack_exception_keeps_child_owned_until_cleanup(self):
        api = FakeApi(); tx = m.FakeTransaction(api, TX)
        with self.assertRaises(OSError):
            tx.run(worker_factory=lambda _: 42,
                   send_ack=lambda _: (_ for _ in ()).throw(OSError("possible send")),
                   ack_receipt_factory=lambda ack: ack)
        self.assertEqual(tx.supervisor.ack_transmission, "attempted; outcome unknown")
        report = tx.report["cleanup"]
        self.assertEqual(report.retained_handles, {})
        self.assertEqual(tx.report["retained_local_handles"], {})
        self.assertEqual(tx.cleanup().retained_handles, {})

    def test_interrupted_create_keeps_outcome_unknown_and_closes_partial_pi(self):
        api = FakeApi({"create-exception"}); tx = m.FakeTransaction(api, TX)
        with self.assertRaises(creation.CreateOutcomeUnknown):
            tx.run(worker_factory=lambda _: 42, send_ack=lambda _: self.fail("ACK"),
                   ack_receipt_factory=lambda _: None)
        self.assertIn("unknown", tx.supervisor.create_outcome)
        self.assertIn(("close", 202), api.calls)
        self.assertIn(("close", 203), api.calls)
        self.assertEqual(tx.report["cleanup"].retained_handles, {})

    def test_failed_create_return_keeps_outcome_unknown(self):
        api = FakeApi({"create-false"}); tx = m.FakeTransaction(api, TX)
        with self.assertRaises(creation.CreateOutcomeUnknown):
            tx.run(worker_factory=lambda _: 42, send_ack=lambda _: self.fail("ACK"),
                   ack_receipt_factory=lambda _: None)
        self.assertIn("unknown", tx.supervisor.create_outcome)
        self.assertEqual(tx.report["retained_local_handles"], {})
        self.assertEqual(tx.report["cleanup"].retained_handles, {})

    def test_offer_duplication_failure_still_cleans_all_returned_handles(self):
        api = FakeApi({"child-duplicate"}); tx = m.FakeTransaction(api, TX)
        with self.assertRaises(creation.ApiFailure):
            tx.run(worker_factory=lambda _: 42, send_ack=lambda _: self.fail("ACK"),
                   ack_receipt_factory=lambda _: None)
        self.assertEqual(tx.supervisor.offer.worker_handle, 202)
        self.assertEqual(tx.report["retained_local_handles"], {})
        self.assertEqual(tx.report["cleanup"].retained_handles, {})

    def test_failed_local_close_is_retained_and_retryable(self):
        api = FakeApi({203}); tx = run(api)
        self.assertEqual(tx.report["status"], "acknowledged-cleanup-uncertain")
        self.assertEqual(tx.report["retained_local_handles"], {"thread": 203})
        api.fail.remove(203)
        tx.cleanup()
        self.assertEqual(tx.report["retained_local_handles"], {})

    def test_unknown_create_timeout_retains_exact_local_process_handle(self):
        api = FakeApi({"create-exception", "local-timeout"})
        tx = m.FakeTransaction(api, TX)
        with self.assertRaises(creation.CreateOutcomeUnknown):
            tx.run(worker_factory=lambda _: 42, send_ack=lambda _: self.fail("ACK"),
                   ack_receipt_factory=lambda _: None)
        self.assertEqual(tx.report["retained_local_handles"], {"process": 202})
        self.assertNotIn(("close", 202), api.calls)
        self.assertIn(("close", 203), api.calls)
        api.fail.remove("local-timeout")
        tx.cleanup()
        self.assertEqual(tx.report["retained_local_handles"], {})

    def test_invalid_wait_is_rejected_before_any_owned_state(self):
        api = FakeApi(); tx = m.FakeTransaction(api, TX)
        with self.assertRaisesRegex(ValueError, "bounded wait"):
            tx.run(worker_factory=lambda _: self.fail("worker must not start"),
                   send_ack=lambda _: self.fail("ACK must not send"),
                   ack_receipt_factory=lambda _: self.fail("receipt must not arrive"),
                   wait_ms=6000)
        self.assertEqual(tx.phase, "new")
        self.assertEqual(tx.report["status"], "not-started")
        self.assertIsNone(tx.supervisor.job)
        self.assertIsNone(tx.supervisor.worker_handle)
        self.assertEqual(api.calls, [])

    def test_fixed_field_contract_and_real_entry_hold(self):
        with self.assertRaises(ValueError):
            m.FakeTransaction(FakeApi(), m.FixedTransaction(
                creation.FixedCloneImage(IMAGE.executable, IMAGE.cwd, "0" * 64), TX.occurrence))
        with self.assertRaises(RuntimeError): m.run_integrated_fake_transaction()


if __name__ == "__main__": unittest.main()
