"""Injected CP372 tests. No DLL, child process, original or GUI is started."""
import builtins
import ctypes as C
import io
import json
import os
from pathlib import Path
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp372_startup_only_probe as m
from test_cp331_outer_watchdog import Harness, out


def temporary_directory():
    return tempfile.TemporaryDirectory(dir=os.environ.get("CP372_TEST_TEMP") or None)


class Exports(Harness):
    def __init__(self):
        super().__init__()
        self.event_state = 0
        self.child_exit = 0
    def dispatch(self, name, *args):
        if name == "GetHandleInformation" and args[0] == 17:
            self.calls.append((name, args))
            out(args[1], m.abi.DWORD, 1)
            return 1
        if name == "CreateEventW":
            self.calls.append((name, args))
            self.event_args = args
            return 17
        if name == "WaitForSingleObject" and args[0] == 17:
            self.calls.append((name, args))
            return self.event_state
        if name == "QueryFullProcessImageNameW":
            self.calls.append((name, args))
            args[2].value = m.IMAGE
            out(args[3], m.abi.DWORD, len(m.IMAGE))
            return 1
        if name == "WaitForSingleObject" and args[0] == 31 and args[1] > 0:
            self.dead = True
        if name == "GetExitCodeProcess":
            self.exit_code = self.child_exit
        return super().dispatch(name, *args)


class Security:
    def __init__(self, exports):
        self.exports = exports
    def prepare(self):
        return self
    def security_attributes(self, *, inheritable):
        return m.abi.SECURITY_ATTRIBUTES(C.sizeof(m.abi.SECURITY_ATTRIBUTES), 123, inheritable)
    def cleanup(self):
        return True


class Pipe:
    failure = None
    cleanup_ok = True
    def __init__(self, exports, descriptor, *, deadline):
        self.exports, self.deadline = exports, deadline
        self.handles = {10, 11, 15, 13}
        self.operations = []
    def prepare(self, name):
        assert name.startswith(r"\\.\pipe\cp372-")
        return SimpleNamespace(parent_write=10, client_read=11, parent_read=15, client_write=13)
    def _close(self, handle):
        self.handles.remove(handle)
    def transfer(self, handle, direction, size):
        assert (handle, direction, size) == (15, "read", 48)
        if self.failure:
            raise self.failure
        return m.MARKER
    def cleanup(self):
        if self.cleanup_ok:
            self.handles.clear()
        return self.cleanup_ok


class Tests(unittest.TestCase):
    def run_injected(self, harness=None):
        harness = harness or Exports()
        with temporary_directory() as directory, \
                mock.patch.object(m, "ANALYSIS", Path(directory)), \
                mock.patch.object(m.security, "NativePrivatePipeSecurity", Security), \
                mock.patch.object(m.pipes, "NativePipeOwner", Pipe):
            owner = m._Probe(harness.exports)
            try:
                result = owner.run_once()
            except m.ProbeFailure:
                result = owner.result
            durable = json.loads(owner.receipt_path.read_text())
            return owner, result, durable, harness

    def test_public_hold_and_fixed_bootstrap(self):
        self.assertEqual(m.NATIVE_ENTRY, "HARD HOLD")
        with self.assertRaises(m.Hold):
            m.launch_probe()
        code = m._bootstrap(17, Path("fixed-file.json"))
        compile(code, "<CP372>", "exec")
        self.assertEqual(code.splitlines()[0], "import ctypes as C")
        self.assertLess(code.index("if not s(17)"), code.index("import os,sys,json"))
        self.assertNotIn("sys.path", code)
        self.assertIn("'xb'", code)
        self.assertIn("os.fsync", code)
        with self.assertRaises(ValueError):
            m._Probe(Exports().exports, occurrence_id="not-an-occurrence")
        fixed = m._Probe(Exports().exports, occurrence_id="f" * 32)
        self.assertEqual(fixed.receipt_path.name, "cp372-receipt-" + "f" * 32 + ".json")
        self.assertEqual(fixed.result["occurrence_id"], "f" * 32)

    def test_fixed_creation_three_handles_and_stdio_parity(self):
        owner, result, durable, h = self.run_injected()
        self.assertEqual(h.creation[0], m.IMAGE)
        self.assertEqual(h.creation[7], m.CWD)
        self.assertEqual(h.attributes[m.creator.HANDLE_LIST], (11, 13, 17))
        self.assertEqual(h.attributes[m.creator.JOB_LIST], (21,))
        startup = C.cast(h.creation[8], C.POINTER(m.abi.STARTUPINFOEXW)).contents
        self.assertEqual((startup.StartupInfo.hStdInput, startup.StartupInfo.hStdOutput,
                          startup.StartupInfo.hStdError), (11, 13, 13))
        self.assertEqual(h.creation[4], True)
        self.assertEqual(h.creation[5], m.creator.CREATE_FLAGS)
        self.assertEqual(result["event"], "signaled")
        self.assertEqual(result["stdout"], "exact-marker")
        self.assertEqual(result["exit"], {"code": 0, "exact_handle_wait": True})
        self.assertEqual(result["file"]["status"], "absent")
        self.assertEqual(result["cleanup"], "closed")
        self.assertFalse(result["probe_passed"])
        self.assertEqual(durable, result)
        self.assertEqual(owner.deadline, 5100)
        self.assertEqual(h.closed[-4:], [32, 31, 21, 17])

    def test_read_failure_still_observes_exact_nonzero_exit(self):
        h = Exports()
        h.child_exit = 120
        with mock.patch.object(Pipe, "failure", RuntimeError("read 109")):
            _, result, durable, _ = self.run_injected(h)
        self.assertEqual(result["stdout"], "read-failed-or-partial")
        self.assertEqual(result["exit"]["code"], 120)
        self.assertFalse(result["probe_passed"])
        self.assertEqual(durable["event"], "signaled")
        self.assertIn("read 109", result["errors"])

    def test_probe_passed_requires_all_independent_observations(self):
        child = {"status": "valid", "data": {"stdout": "written-flushed"}}
        with mock.patch.object(m, "_file_observation", return_value=child):
            _, result, durable, _ = self.run_injected()
        self.assertTrue(result["probe_passed"])
        self.assertTrue(durable["probe_passed"])
        self.assertEqual(result["file"], child)

    def test_receipt_failure_clears_probe_passed(self):
        child = {"status": "valid", "data": {"stdout": "written-flushed"}}
        for mode in ("oversized", "write-failed"):
            with self.subTest(mode=mode), temporary_directory() as directory, \
                    mock.patch.object(m, "ANALYSIS", Path(directory)), \
                    mock.patch.object(m.security, "NativePrivatePipeSecurity", Security), \
                    mock.patch.object(m.pipes, "NativePipeOwner", Pipe), \
                    mock.patch.object(m, "_file_observation", return_value=child):
                owner = m._Probe(Exports().exports)
                if mode == "oversized":
                    patcher = mock.patch.object(m.json, "dumps", return_value="x" * 16385)
                else:
                    patcher = mock.patch.object(Path, "open", side_effect=OSError("injected receipt write"))
                with patcher, self.assertRaises(m.ProbeFailure):
                    owner.run_once()
                self.assertFalse(owner.result["probe_passed"])
                self.assertEqual(owner.result["cleanup"], "closed")
                self.assertIn("receipt_write", owner.result)

    def test_absent_event_does_not_claim_startup_failure(self):
        h = Exports()
        h.event_state = 258
        _, result, _, _ = self.run_injected(h)
        self.assertEqual(result["event"], "timeout")
        self.assertEqual(result["stdout"], "unobserved")
        self.assertFalse(result["probe_passed"])
        self.assertNotIn("startup-failure", str(result))

    def test_uncertain_creation_retains_exact_outputs_and_cleans(self):
        h = Exports()
        h.create_raise = True
        _, result, durable, h = self.run_injected(h)
        self.assertEqual(result["owner"]["create_outcome"], "unknown: CreateProcessW entered")
        self.assertTrue(result["owner"]["runner_waited"])
        self.assertEqual(result["cleanup"], "closed")
        self.assertEqual(durable["exit"], "unobserved")
        self.assertIn(31, h.closed)

    def test_unknown_creation_without_process_retains_job_and_event(self):
        h = Exports()
        h.create_raise = h.no_outputs = True
        owner, result, _, h = self.run_injected(h)
        self.assertEqual(result["cleanup"], "unresolved")
        self.assertEqual(owner.event, 17)
        self.assertEqual(owner._owned_handles["outer_job"], 21)
        self.assertNotIn(21, h.closed)
        self.assertNotIn(17, h.closed)

    def test_pipe_cleanup_uncertain_is_not_closed(self):
        with mock.patch.object(Pipe, "cleanup_ok", False):
            owner, result, durable, _ = self.run_injected()
        self.assertEqual(result["cleanup"], "unresolved")
        self.assertEqual(durable["cleanup"], "unresolved")
        self.assertTrue(owner.pipe_owner.handles)
        owner.close()
        self.assertEqual(owner.result["cleanup"], "closed")

    def test_bounded_file_schema_and_hash(self):
        with temporary_directory() as directory:
            path = Path(directory) / "file.json"
            self.assertEqual(m._file_observation(path), {"status": "absent"})
            raw = b'{"schema":"cp372-child-v1","event":"signaled","stdout":"failed","error_type":"OSError"}'
            path.write_bytes(raw)
            result = m._file_observation(path)
            self.assertEqual(result["status"], "valid")
            self.assertEqual(result["sha256"], m.hashlib.sha256(raw).hexdigest())
            path.write_bytes(b"x" * (m.LIMIT + 10))
            self.assertEqual(m._file_observation(path)["status"], "oversized")
            path.write_bytes(b'{"schema":"other"}')
            self.assertEqual(m._file_observation(path)["status"], "malformed")

    def test_child_script_event_then_stdout_failure_still_writes_file(self):
        stages = []
        class BadStdout:
            def write(self, _):
                stages.append("stdout")
                raise OSError("injected")
        fake_ctypes = SimpleNamespace(
            WinDLL=lambda *a, **k: object(), c_int=object(), c_void_p=object(),
            WINFUNCTYPE=lambda *a, **k: lambda entry: lambda event: stages.append("event") or 1)
        original_import = builtins.__import__
        def injected_import(name, *args, **kwargs):
            if name == "ctypes":
                return fake_ctypes
            if name == "sys":
                return SimpleNamespace(stdout=SimpleNamespace(buffer=BadStdout()))
            return original_import(name, *args, **kwargs)
        with temporary_directory() as directory:
            path = Path(directory) / "exclusive.json"
            scope = {"__builtins__": dict(vars(builtins), __import__=injected_import)}
            exec(m._bootstrap(17, path), scope)
            self.assertEqual(stages, ["event", "stdout"])
            self.assertEqual(m._file_observation(path)["data"]["stdout"], "failed")
            before = path.read_bytes()
            with self.assertRaises(FileExistsError):
                exec(m._bootstrap(17, path), scope)
            self.assertEqual(path.read_bytes(), before)


if __name__ == "__main__":
    unittest.main()
