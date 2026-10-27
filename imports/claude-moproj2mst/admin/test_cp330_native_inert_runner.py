"""Injected CP325 BoundExports only: no native library, process or worker run."""
import ctypes as C
import importlib
from pathlib import Path
import sys
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp330_native_inert_runner as m
from test_cp329_native_worker_creator import Harness as CreatorHarness, out


class Harness(CreatorHarness):
    def __init__(self):
        super().__init__()
        self.resume = 1
        self.available = self.exit_code = 0
        self.exited = False
        self.frames = []
        self.bad_nonce = self.nonzero_child = self.short_read = False
        self.short_write = False
        self.expire_peek = self.regress_peek = False
        self.pending_read = False
        self.pipes.operations, self.pipes.handles = [], set()
        self.pipes.receipt = m.pipes.ConnectedEndpoints(10, 11, 12, 13)
        self.pipes.transfer = self.transfer
        self.runner = m.NativeInertRunner(self.exports, timeout_ms=900)

    def dispatch(self, name, *args):
        if name in ("ResumeThread", "PeekNamedPipe", "GetExitCodeProcess", "WaitForSingleObject"):
            self.calls.append(name)
            if name == self.fail:
                self.error = 5
                return 0xFFFFFFFF if name == "WaitForSingleObject" else 0
            if name == "ResumeThread":
                self.assert_thread = args == (32,)
                return self.resume
            if name == "PeekNamedPipe":
                assert args[0:4] == (12, None, 0, None) and args[5] is None
                out(args[4], m.abi.DWORD, self.available)
                if self.expire_peek:
                    self.now = 1000
                if self.regress_peek:
                    self.now = 99
            elif name == "GetExitCodeProcess":
                assert args[0] == 31
                out(args[1], m.abi.DWORD, self.exit_code)
            elif name == "WaitForSingleObject":
                assert args[0] == 31 and 0 <= args[1] <= 900
                if args[1]:
                    self.exited = self.wait == 0
                return self.wait
            return 1
        if name == "QueryInformationJobObject" and self.exited:
            saved = self.terminated
            self.terminated = True
            try:
                return super().dispatch(name, *args)
            finally:
                self.terminated = saved
        return super().dispatch(name, *args)

    def transfer(self, handle, kind, payload):
        self.calls.append("transfer." + kind)
        if kind == "write":
            assert handle == 10
            self.frames.append(payload)
            return len(payload) - int(self.short_write)
        assert (handle, payload) == (12, 80)
        init = m.wire.decode_init(self.frames[0])
        result = m.wire.WorkerTelemetry(int(self.nonzero_child), 0, 0, 0, 0, 0, 0, 0, 0,
                                       b"x" * 32 if self.bad_nonce else init.nonce)
        if self.pending_read:
            from types import SimpleNamespace
            self.pipes.operations.append(SimpleNamespace(terminal=False, kind="read"))
        raw = m.wire.encode_result(result)
        return raw[:-1] if self.short_read else raw

    def run(self):
        def pipe_factory(exports, security, *, deadline):
            assert exports is self.exports and security is self.security
            self.pipes.deadline = deadline
            return self.pipes
        with patch.object(m.security, "NativePrivatePipeSecurity", return_value=self.security), \
             patch.object(m.pipes, "NativePipeOwner", side_effect=pipe_factory), \
             patch.object(m.image, "NativeImagePin", return_value=self.pin):
            return self.runner.run()


class RunnerTests(unittest.TestCase):
    def test_import_constructor_and_live_entry_inert(self):
        with patch.object(m.binding, "load_native_exports", side_effect=AssertionError("no load")):
            importlib.reload(m)
            h = Harness()
            self.assertEqual(h.calls, [])
            with self.assertRaises(m.Hold):
                m.create_live_runner()
            self.assertEqual(h.calls, [])

    def test_exact_inert_transcript_and_success_cleanup(self):
        h = Harness()
        r = h.run()
        self.assertEqual(r.phase, "closed-success")
        self.assertEqual(r.deadline, 1000)
        self.assertEqual(h.calls[0], "GetTickCount64")
        self.assertTrue(h.assert_thread)
        self.assertEqual(h.calls.count("ResumeThread"), 1)
        init, ack = m.wire.decode_init(h.frames[0]), m.wire.decode_ack(h.frames[1])
        self.assertEqual((init.child_job, ack.child_process_handle), (44, 0))
        self.assertEqual(init.nonce, ack.nonce)
        self.assertEqual(len(init.nonce), 32)
        self.assertLess(h.calls.index("PeekNamedPipe"), h.calls.index("WaitForSingleObject"))
        self.assertNotIn("TerminateJobObject", h.calls)
        self.assertEqual(h.runner.worker.receipt.phase, "closed")
        self.assertFalse(h.runner.worker.receipt.local)
        with self.assertRaises(m.Hold):
            h.runner.run()

    def test_random_pipe_names_and_nonces(self):
        names, nonces = [], []
        for _ in range(2):
            h = Harness()
            original = h.pipes.prepare
            h.pipes.prepare = lambda name, original=original: (names.append(name), original(name))[1]
            h.run()
            nonces.append(m.wire.decode_init(h.frames[0]).nonce)
        self.assertNotEqual(names[0], names[1])
        self.assertTrue(all(n.startswith(r"\\.\pipe\cp330-") for n in names))
        self.assertNotEqual(nonces[0], nonces[1])

    def test_resume_invalid_or_uncertain_never_retried(self):
        for previous in (0, 2, 0xFFFFFFFF, True):
            with self.subTest(previous=previous):
                h = Harness()
                h.resume = previous
                with self.assertRaises(m.RunFailure) as caught:
                    h.run()
                self.assertIs(caught.exception.owner, h.runner)
                self.assertEqual(h.calls.count("ResumeThread"), 1)
                self.assertEqual(h.frames, [])
                self.assertIn("TerminateJobObject", h.calls)

    def test_bad_result_and_peek_never_ack(self):
        for flag in ("bad_nonce", "nonzero_child", "short_read", "short_write",
                     "available", "pending_read", "expire_peek", "regress_peek"):
            with self.subTest(flag=flag):
                h = Harness()
                setattr(h, flag, True)
                with self.assertRaises(m.RunFailure):
                    h.run()
                self.assertEqual(len(h.frames), 1)
                self.assertIn("TerminateJobObject", h.calls)
                self.assertLess(h.calls.index("TerminateJobObject"), h.calls.index("pipes.cleanup"))

    def test_peek_failure_captures_error_and_retains_failed_cleanup(self):
        h = Harness()
        h.fail = "PeekNamedPipe"
        h.pipes.ready = False
        with self.assertRaises(m.RunFailure) as caught:
            h.run()
        self.assertEqual(caught.exception.__cause__.code, 5)
        self.assertEqual(h.runner.worker.receipt.phase, "hold")
        self.assertTrue(h.runner.worker.receipt.local)
        h.pipes.ready = True
        h.runner.close()
        self.assertEqual(h.runner.worker.receipt.phase, "closed")

    def test_exit_nonzero_or_unsignaled_not_success(self):
        for field, value in (("exit_code", 7), ("wait", 258), ("child_count", 1)):
            with self.subTest(field=field):
                h = Harness()
                setattr(h, field, value)
                with self.assertRaises(m.RunFailure):
                    h.run()
                self.assertEqual(h.runner.receipt.phase, "hold")
                self.assertNotEqual(h.runner.receipt.ack_outcome, "exact-worker-exit-zero-observed")

    def test_non_bound_exports_and_invalid_timeout_rejected_inertly(self):
        for timeout in (0, 5001, True, 1.5):
            h = Harness()
            with self.assertRaises(ValueError):
                m.NativeInertRunner(h.exports, timeout_ms=timeout)
            self.assertEqual(h.calls, [])
        with self.assertRaises(TypeError):
            m.NativeInertRunner(object())


if __name__ == "__main__":
    unittest.main()
