"""Windows integration tests: only owned no-window Python children in scratch."""
import hashlib
import ctypes
from ctypes import wintypes
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import time
import unittest
import importlib.util

ROOT = Path(__file__).resolve().parents[2]
HELPER = ROOT / "admin" / "owned_suspended_process.py"
SPEC = importlib.util.spec_from_file_location("owned_suspended_process", HELPER)
OWNED = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(OWNED)


@unittest.skipUnless(sys.platform == "win32", "Win32 lifecycle integration")
class OwnedLauncherTests(unittest.TestCase):
    def setUp(self):
        scratch = ROOT / "scratch"
        scratch.mkdir(exist_ok=True)
        self.temp = tempfile.TemporaryDirectory(prefix="resume201-owned-", dir=scratch)
        self.cwd = Path(self.temp.name)
        self.digest = hashlib.sha256(Path(sys.executable).read_bytes()).hexdigest()
        self.controllers = []

    def tearDown(self):
        for proc in self.controllers:
            if proc.poll() is None:
                proc.stdin.close()  # The controller owns exact-child EOF cleanup.
                proc.wait(timeout=8)
            if proc.stdin and not proc.stdin.closed:
                proc.stdin.close()
            proc.stdout.close()
            proc.stderr.close()
        self.temp.cleanup()

    def launch(self, code="import time; time.sleep(10)", seconds=3, digest=None, delay_after_created=0):
        controller = [sys.executable, str(HELPER)]
        if delay_after_created:
            # Delay only the controller's main thread after the initial receipt.
            # Its real watchdog must terminate the still-suspended owned child.
            wrapper = (
                "import importlib.util,time; "
                f"s=importlib.util.spec_from_file_location('owned', {str(HELPER)!r}); "
                "m=importlib.util.module_from_spec(s); s.loader.exec_module(m); "
                "original=m.emit; "
                f"m.emit=lambda event,**kw: (original(event,**kw),time.sleep({delay_after_created}) if event=='created' else None); "
                "raise SystemExit(m.main())"
            )
            controller = [sys.executable, "-c", wrapper]
        proc = subprocess.Popen([*controller, "--exe", sys.executable,
                                 "--cwd", str(self.cwd), "--sha256", digest or self.digest,
                                 "--max-seconds", str(seconds), "--", "-I", "-S", "-c", code],
                                stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE, text=True,
                                creationflags=subprocess.CREATE_NO_WINDOW)
        self.controllers.append(proc)
        return proc, json.loads(proc.stdout.readline())

    def finish(self, proc, command=None):
        stdout, stderr = proc.communicate(command, timeout=9)
        self.assertEqual(stderr, "")
        return [json.loads(line) for line in stdout.splitlines()]

    def assert_cleanup(self, records):
        cleanup = records[-1]
        self.assertEqual(cleanup["event"], "cleanup")
        self.assertTrue(cleanup["exit_verified"])
        self.assertTrue(cleanup["handles_closed"])
        self.assertEqual(cleanup["wait_result"], 0)

    def test_suspended_explicit_cwd_resume_terminate(self):
        code = "from pathlib import Path; import time; Path('cwd.txt').write_text(str(Path.cwd())); time.sleep(10)"
        proc, created = self.launch(code)
        self.assertEqual(created["state"], "suspended")
        self.assertGreater(created["identity"]["creation_filetime"], 0)
        self.assertEqual(created["identity"]["sha256"], self.digest)
        time.sleep(.1)
        self.assertFalse((self.cwd / "cwd.txt").exists())
        proc.stdin.write('{"command":"resume"}\n')
        proc.stdin.flush()
        resumed = json.loads(proc.stdout.readline())
        self.assertEqual(resumed["previous_suspend_count"], 1)
        until = time.monotonic() + 1
        while not (self.cwd / "cwd.txt").exists() and time.monotonic() < until:
            time.sleep(.01)
        self.assertEqual(Path((self.cwd / "cwd.txt").read_text()).resolve(), self.cwd.resolve())
        records = self.finish(proc, '{"command":"status"}\n{"command":"terminate"}\n')
        self.assertEqual(proc.returncode, 0)
        self.assert_cleanup(records)

    def test_eof_terminates_suspended_process(self):
        proc, created = self.launch()
        self.assertEqual(created["state"], "suspended")
        records = self.finish(proc, "")
        self.assertEqual(records[0]["reason"], "eof")
        self.assert_cleanup(records)

    def test_malformed_input_cleans_up(self):
        proc, _ = self.launch()
        records = self.finish(proc, '{"command":"attach","pid":1}\n')
        self.assertEqual(proc.returncode, 1)
        self.assertEqual(records[0]["event"], "error")
        self.assert_cleanup(records)

    def test_deadline_while_stdin_open(self):
        proc, _ = self.launch(seconds=.5)
        proc.wait(timeout=8)
        records = [json.loads(line) for line in proc.stdout.read().splitlines()]
        self.assertEqual(records[0]["reason"], "deadline")
        self.assert_cleanup(records)

    def test_watchdog_provenance_and_independent_owned_exit(self):
        proc, created = self.launch(seconds=.5, delay_after_created=1.0)
        k = ctypes.WinDLL("kernel32", use_last_error=True)
        k.OpenProcess.argtypes = [wintypes.DWORD, wintypes.BOOL, wintypes.DWORD]
        k.OpenProcess.restype = wintypes.HANDLE
        k.WaitForSingleObject.argtypes = [wintypes.HANDLE, wintypes.DWORD]
        k.WaitForSingleObject.restype = wintypes.DWORD
        k.GetProcessTimes.argtypes = [wintypes.HANDLE] + [ctypes.POINTER(wintypes.FILETIME)] * 4
        k.GetProcessTimes.restype = wintypes.BOOL
        k.GetExitCodeProcess.argtypes = [wintypes.HANDLE, ctypes.POINTER(wintypes.DWORD)]
        k.GetExitCodeProcess.restype = wintypes.BOOL
        k.CloseHandle.argtypes = [wintypes.HANDLE]
        k.CloseHandle.restype = wintypes.BOOL
        pid = created["identity"]["pid"]
        handle = k.OpenProcess(0x100000 | 0x1000, False, pid)
        self.assertTrue(handle)
        try:
            times = [wintypes.FILETIME() for _ in range(4)]
            self.assertTrue(k.GetProcessTimes(handle, *[ctypes.byref(t) for t in times]))
            self.assertEqual((times[0].dwHighDateTime << 32) | times[0].dwLowDateTime,
                             created["identity"]["creation_filetime"])
            # The controller main thread is still in the deliberate delay.
            self.assertEqual(k.WaitForSingleObject(handle, 900), 0)
            exit_code = wintypes.DWORD()
            self.assertTrue(k.GetExitCodeProcess(handle, ctypes.byref(exit_code)))
            self.assertEqual(exit_code.value, 0xe201)
        finally:
            self.assertTrue(k.CloseHandle(handle))
        proc.wait(timeout=8)
        records = [json.loads(line) for line in proc.stdout.read().splitlines()]
        self.assert_cleanup(records)
        self.assertEqual(records[-1]["termination_requested"], True)
        self.assertEqual(records[-1]["exit_code"], 0xe201)
        self.assertEqual(records[-1]["termination_attempts"], [{
            "source": "watchdog_deadline", "requested_exit_code": 0xe201,
            "api_succeeded": True, "winerror": None, "wait_result": 0,
            "exit_code": 0xe201, "exit_verified": True}])
        # After all held handles close, no object for this PID remains.
        # A reused PID would be retained as an explicit test failure, never killed.
        reopened = k.OpenProcess(0x1000, False, pid)
        if reopened:
            k.CloseHandle(reopened)
        self.assertFalse(reopened, "owned PID still openable or reused; absence not established")
        self.assertEqual(ctypes.get_last_error(), 87)

    def test_natural_exit(self):
        proc, _ = self.launch("raise SystemExit(7)")
        proc.stdin.write('{"command":"resume"}\n')
        proc.stdin.flush()
        proc.wait(timeout=8)
        records = [json.loads(line) for line in proc.stdout.read().splitlines()]
        self.assertEqual(records[-2]["reason"], "target_exited")
        self.assertEqual(records[-1]["exit_code"], 7)
        self.assertFalse(records[-1]["termination_requested"])
        self.assertEqual(records[-1]["termination_attempts"], [])
        self.assert_cleanup(records)

    def test_hash_mismatch_rejected_before_creation(self):
        proc, first = self.launch(digest="0" * 64)
        records = self.finish(proc)
        self.assertEqual(first["event"], "error")
        self.assertEqual(records, [])
        self.assertEqual(proc.returncode, 1)

    def test_second_resume_is_error_and_cleanup(self):
        proc, _ = self.launch()
        records = self.finish(proc, '{"command":"resume"}\n{"command":"resume"}\n')
        self.assertEqual(proc.returncode, 1)
        self.assertEqual(records[-2]["event"], "error")
        self.assert_cleanup(records)

    def test_failed_terminate_accepts_exit_proven_by_same_held_handle(self):
        class RacingKernel:
            def __init__(self):
                self.waits = []

            def WaitForSingleObject(self, handle, timeout):
                self.waits.append(timeout)
                return 0 if len(self.waits) > 1 else 258

            def GetExitCodeProcess(self, handle, code):
                code._obj.value = 0
                return 1

            def TerminateProcess(self, handle, code):
                ctypes.set_last_error(5)
                return 0

        child = OWNED.OwnedProcess.__new__(OWNED.OwnedProcess)
        child.k = RacingKernel()
        child.lock = __import__("threading").RLock()
        child.pi = OWNED.PROCESS_INFORMATION()
        child.pi.hProcess = 123
        child.resumed = True
        child.termination_attempts = []
        receipt = child.terminate("race_test")
        self.assertTrue(receipt["exit_verified"])
        self.assertEqual(child.termination_attempts, [{
            "source": "race_test", "requested_exit_code": 0xe201,
            "api_succeeded": False, "winerror": 5,
            "post_failure_wait_result": 0,
            "exit_during_termination_race": True,
            "wait_result": 0, "exit_code": 0, "exit_verified": True}])
        self.assertEqual(child.k.waits, [0, 5000, 5000, 0])


if __name__ == "__main__":
    unittest.main()
