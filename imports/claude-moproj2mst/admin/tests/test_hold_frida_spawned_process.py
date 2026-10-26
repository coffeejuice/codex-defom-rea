"""Harmless suspended Python children only; no Frida or original app launch.

This qualifies handle cleanup, not Frida pause/ownership integration. The spawn
token is deliberately simulated. Existing creator retains an independent handle
so exit is verified without trusting the guardian receipt or re-opening a PID.
"""
import ctypes as C
from ctypes import wintypes as W
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import time
import unittest

ADMIN = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ADMIN))
import hold_frida_spawned_process as guard
import owned_suspended_process as creator


@unittest.skipUnless(sys.platform == "win32", "Windows handle APIs")
class GuardianTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.exe = str(Path(sys.executable).resolve())
        cls.digest = hashlib.sha256(Path(cls.exe).read_bytes()).hexdigest()

    def setUp(self):
        self.child = creator.OwnedProcess(self.exe, str(ADMIN),
                                         ["-c", "import time; time.sleep(60)"], self.digest)
        self.created = self.child.identity["creation_filetime"]
        self.pid = self.child.identity["pid"]

    def tearDown(self):
        self.child.close()

    def adopt(self, **changes):
        args = dict(pid=self.pid, image=self.exe, sha256=self.digest,
                    after=self.created - 10000, before=self.created + 10000,
                    adopt_ms=3000, token="simulated_test_spawn_token_1234567890",
                    controller_pid=os.getpid())
        args.update(changes)
        return guard.HeldSpawn(**args)

    def launch(self, seconds=5, **changes):
        values = {"pid": self.pid, "expected-image": self.exe,
                  "expected-sha256": self.digest,
                  "spawn-token": "simulated_test_spawn_token_1234567890",
                  "controller-pid": os.getpid(),
                  "created-after-filetime": self.created - 10000,
                  "created-before-filetime": self.created + 10000,
                  "adopt-within-ms": 3000, "max-seconds": seconds}
        values.update(changes)
        argv = [sys.executable, str(ADMIN / "hold_frida_spawned_process.py")]
        for key, value in values.items():
            argv.extend(["--" + key, str(value)])
        return subprocess.Popen(argv, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE, text=True)

    def verify_receipt(self, proc, commands):
        stdout, stderr = proc.communicate(commands, timeout=10)
        rows = [json.loads(x) for x in stdout.splitlines()]
        self.assertEqual(proc.returncode, 0, (rows, stderr))
        self.assertEqual(rows[0]["event"], "ready")
        self.assertEqual(rows[-1]["event"], "closed")
        self.assertTrue(rows[-1]["exit_verified"])
        self.assertTrue(rows[-1]["handle_closed"])
        self.assertEqual(rows[-1]["identity"]["creation_filetime"], self.created)
        self.assertFalse(rows[-1]["identity"]["pause_verified"])
        self.assertTrue(self.child.status()["exit_verified"])
        self.assertEqual(self.child.status()["exit_code"], 0xe202)
        return rows

    def test_exact_handle_status_terminate_close(self):
        held = self.adopt()
        try:
            self.assertFalse(held.status()["exit_verified"])
            result = held.terminate("test")
            self.assertTrue(result["exit_verified"])
            self.assertEqual(result["exit_code"], 0xe202)
            self.assertEqual(len(result["termination_attempts"]), 1)
            self.assertTrue(held.close()["handle_closed"])
            self.assertTrue(self.child.status()["exit_verified"])
        finally:
            if held.handle:
                held.close()

    def test_bad_hash_does_not_kill(self):
        with self.assertRaises(ValueError):
            self.adopt(sha256="0" * 64)
        self.assertFalse(self.child.status()["exit_verified"])

    def test_exit_code_259_is_not_mistaken_for_live_process(self):
        held = self.adopt()
        try:
            creator.checked(self.child.k.TerminateProcess(self.child.pi.hProcess, 259))
            self.assertEqual(self.child.k.WaitForSingleObject(self.child.pi.hProcess, 5000), 0)
            result = held.close()
            self.assertTrue(result["exit_verified"])
            self.assertEqual(result["exit_code"], 259)
            self.assertEqual(result["termination_attempts"], [])
        finally:
            if held.handle:
                held.close()

    def test_wide_window_and_short_token_rejected(self):
        for changes in ({"before": self.created + 40000000}, {"token": "short"},
                        {"pid": os.getpid()}):
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                self.adopt(**changes)
        self.assertFalse(self.child.status()["exit_verified"])

    def test_mismatched_handle_image_does_not_kill(self):
        alternate = Path(self.exe).with_name("pythonw.exe")
        if not alternate.exists():
            self.skipTest("pythonw.exe unavailable")
        digest = hashlib.sha256(alternate.read_bytes()).hexdigest()
        with self.assertRaisesRegex(ValueError, "identity mismatch"):
            self.adopt(image=str(alternate), sha256=digest)
        self.assertFalse(self.child.status()["exit_verified"])

    def test_creation_mismatch_does_not_kill(self):
        with self.assertRaises(ValueError):
            self.adopt(after=self.created + 1)
        self.assertFalse(self.child.status()["exit_verified"])

    def test_expired_adoption_does_not_kill(self):
        time.sleep(.03)
        with self.assertRaises(ValueError):
            self.adopt(after=self.created, before=self.created, adopt_ms=1)
        self.assertFalse(self.child.status()["exit_verified"])

    def test_already_exited_rejected(self):
        self.child.terminate("test_setup")
        with self.assertRaises(ValueError):
            self.adopt()

    def test_jsonl_status_then_terminate(self):
        rows = self.verify_receipt(self.launch(), '{"command":"status"}\n{"command":"terminate"}\n')
        self.assertEqual([r["event"] for r in rows], ["ready", "status", "terminated", "closed"])

    def test_eof_cleanup(self):
        rows = self.verify_receipt(self.launch(), "")
        self.assertEqual(rows[1]["event"], "eof")

    def test_deadline_with_open_stdin(self):
        proc = self.launch(seconds=.5)
        try:
            ready = json.loads(proc.stdout.readline())
            self.assertEqual(ready["event"], "ready")
            proc.wait(timeout=8)
            tail, stderr = proc.communicate(timeout=2)
            rows = [json.loads(x) for x in tail.splitlines()]
            self.assertEqual(proc.returncode, 0, (rows, stderr))
            self.assertEqual(rows[-1]["event"], "closed")
            self.assertTrue(rows[-1]["handle_closed"])
            self.assertTrue(self.child.status()["exit_verified"])
        finally:
            if proc.poll() is None:
                proc.kill()
                proc.communicate()

    def test_malformed_command_cleanup(self):
        proc = self.launch()
        stdout, stderr = proc.communicate('{"command":"resume"}\n', timeout=10)
        rows = [json.loads(x) for x in stdout.splitlines()]
        self.assertEqual(proc.returncode, 1, stderr)
        self.assertEqual(rows[-1]["event"], "closed")
        self.assertTrue(rows[-1]["exit_verified"])
        self.assertTrue(self.child.status()["exit_verified"])

    def test_wrong_controller_rejected_without_kill(self):
        proc = self.launch(**{"controller-pid": os.getpid() + 100000})
        stdout, stderr = proc.communicate("", timeout=10)
        self.assertEqual(proc.returncode, 1, stderr)
        row = json.loads(stdout)
        self.assertFalse(row["adopted"])
        self.assertFalse(self.child.status()["exit_verified"])


if __name__ == "__main__":
    unittest.main()
