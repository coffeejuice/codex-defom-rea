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
        self.pipes.operations, self.pipes.handles = [], {10, 11, 12, 13}
        self.pin.snapshot = m.image.Snapshot("a" * 64,
            m.image.Identity(1, b"i" * 16, 104960, 2, 3, m.image.WORKER_IMAGE))
        self.pin.post_create_verified = True
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
    def external(self, h, callback, *, deadline=700):
        nonce = bytes(range(32))
        h.runner = m.NativeInertRunner(h.exports, timeout_ms=900,
            deadline=deadline, nonce=nonce, before_resume=callback)
        return nonce

    def test_external_occurrence_preserves_deadline_nonce_and_callback_order(self):
        h, seen = Harness(), []
        def callback(worker, nonce):
            self.assertIs(worker, h.runner.worker)
            self.assertEqual(worker.receipt.phase, "prepared-suspended")
            self.assertEqual(worker.deadline, 700)
            self.assertNotIn("ResumeThread", h.calls)
            self.assertEqual(h.frames, [])
            h.calls.append("before_resume")
            seen.append((worker, nonce))
        nonce = self.external(h, callback)
        with patch.object(m.secrets, "token_bytes", side_effect=AssertionError("no new nonce")), \
             patch.object(m.secrets, "token_hex", return_value="a" * 64):
            r = h.run()
        self.assertEqual(r.deadline, 700)
        self.assertEqual(h.command, m.creator.WORKER_IMAGE + " 700")
        self.assertEqual(len(seen), 1)
        self.assertEqual(seen[0][1], nonce)
        self.assertEqual(m.wire.decode_init(h.frames[0]).nonce, nonce)
        self.assertEqual(m.wire.decode_ack(h.frames[1]).nonce, nonce)
        self.assertLess(h.calls.index("before_resume"), h.calls.index("ResumeThread"))
        self.assertEqual(h.calls.count("ResumeThread"), 1)
        with self.assertRaises(m.Hold):
            h.runner.run()
        self.assertEqual(len(seen), 1)
        self.assertEqual(h.calls.count("ResumeThread"), 1)

    def test_external_worker_progress_markers_follow_exact_milestones_best_effort(self):
        h = Harness()
        nonce = self.external(h, lambda *_: None)
        marks = []

        def record_progress(got_nonce, deadline, stage, *, remaining=None):
            if stage == "worker_ack":
                self.assertEqual(remaining, deadline - h.runner._last_tick)
            marks.append((got_nonce, deadline, stage, remaining))
            return True

        with patch.object(m.progress, "write_progress", side_effect=record_progress), \
             patch.object(m.secrets, "token_hex", return_value="a" * 64):
            r = h.run()

        self.assertEqual(r.phase, "closed-success")
        self.assertEqual([(n, d, s) for n, d, s, _ in marks],
                         [(nonce, 700, "worker_resumed"), (nonce, 700, "result"),
                          (nonce, 700, "worker_ack"), (nonce, 700, "worker_closed")])
        self.assertTrue(all(remaining is None for _, _, stage, remaining in marks
                            if stage != "worker_ack"))
        self.assertTrue(0 < marks[2][3] <= 5000)

    def test_external_progress_storage_failures_do_not_change_acceptance(self):
        h = Harness()
        nonce = self.external(h, lambda *_: None)
        calls = []

        def failed_progress(got_nonce, deadline, stage, *, remaining=None):
            calls.append((got_nonce, deadline, stage, remaining))
            raise OSError("progress storage unavailable")

        with patch.object(m.progress, "write_progress", side_effect=failed_progress), \
             patch.object(m.secrets, "token_hex", return_value="a" * 64):
            r = h.run()

        self.assertEqual(r.phase, "closed-success")
        self.assertEqual(r.result_outcome, "80-byte-zero-child-result")
        self.assertEqual(r.ack_outcome, "exact-worker-exit-zero-observed")
        self.assertEqual([stage for _, _, stage, _ in calls],
                         ["worker_resumed", "result", "worker_ack", "worker_closed"])
        self.assertTrue(all(got_nonce == nonce and deadline == 700
                            for got_nonce, deadline, _, _ in calls))

    def test_non_external_run_does_not_write_worker_progress(self):
        h = Harness()
        with patch.object(m.progress, "write_progress",
                          side_effect=AssertionError("unexpected progress write")):
            self.assertEqual(h.run().phase, "closed-success")

    def test_external_expiry_rejects_before_preparation_without_reallocation(self):
        for deadline in (100, 99, 1001):
            h = Harness()
            self.external(h, lambda *_: self.fail("callback must not run"), deadline=deadline)
            with patch.object(m.secrets, "token_bytes", side_effect=AssertionError("no nonce")), \
                 patch.object(m.secrets, "token_hex", side_effect=AssertionError("no pipe name")):
                with self.assertRaises(m.RunFailure):
                    h.run()
            self.assertEqual(h.runner.receipt.deadline, deadline)
            self.assertIsNone(h.runner.worker)
            self.assertEqual(h.calls, ["GetTickCount64"])

    def test_external_expiry_after_callback_terminates_without_resume(self):
        h = Harness()
        self.external(h, lambda *_: setattr(h, "now", 700))
        with self.assertRaises(m.RunFailure):
            h.run()
        self.assertNotIn("ResumeThread", h.calls)
        self.assertIn("TerminateJobObject", h.calls)
        self.assertEqual(h.frames, [])

    def test_external_callback_failure_preserves_owner_and_retries_cleanup_only(self):
        h, seen = Harness(), []
        h.pipes.ready = False
        def callback(worker, nonce):
            seen.append(worker)
            raise RuntimeError("handoff uncertain")
        self.external(h, callback)
        with self.assertRaises(m.RunFailure) as caught:
            h.run()
        self.assertIs(caught.exception.owner, h.runner)
        self.assertIs(h.runner.worker, seen[0])
        self.assertEqual(h.runner.worker.receipt.phase, "hold")
        self.assertTrue(h.runner.worker.receipt.local)
        self.assertNotIn("ResumeThread", h.calls)
        self.assertIn("TerminateJobObject", h.calls)
        h.pipes.ready = True
        h.runner.close()
        self.assertEqual(h.runner.worker.receipt.phase, "closed")
        self.assertEqual(len(seen), 1)
        self.assertEqual(h.frames, [])

    def test_external_custody_mutations_hold_original_snapshot_without_resume(self):
        mutations = {
            "thread": lambda w: w.receipt.local.__setitem__("worker_thread", 999),
            "process": lambda w: w.receipt.local.__setitem__("worker_process", 999),
            "local-object": lambda w: setattr(w.receipt, "local", dict(w.receipt.local)),
            "pid": lambda w: setattr(w.receipt, "pid", 99),
            "remote": lambda w: setattr(w.receipt, "remote_child_job", m.creator.RemoteHandle(99, 31)),
            "phase": lambda w: setattr(w.receipt, "phase", "closed"),
            "evidence": lambda w: setattr(w.receipt, "worker_waited", True),
            "exports": lambda w: setattr(w, "exports", object()),
            "receipt": lambda w: setattr(w, "receipt", m.creator.Receipt()),
        }
        for name, mutate in mutations.items():
            with self.subTest(name=name):
                h, original = Harness(), []
                def callback(worker, nonce):
                    original.append(worker)
                    mutate(worker)
                self.external(h, callback)
                with self.assertRaises(m.RunFailure):
                    h.run()
                self.assertTrue(h.runner._custody_uncertain)
                self.assertIs(h.runner._prepared_custody[0], original[0])
                self.assertEqual(dict(h.runner._prepared_custody[3])["worker_thread"], 32)
                self.assertNotIn("ResumeThread", h.calls)
                self.assertNotIn("TerminateJobObject", h.calls)
                self.assertEqual(h.frames, [])

    def test_external_options_are_all_required_and_validated_inertly(self):
        valid = dict(deadline=700, nonce=b"n" * 32, before_resume=lambda *_: None)
        for name, bad in (("deadline", True), ("deadline", 0), ("nonce", b"short"),
                          ("nonce", bytearray(32)), ("before_resume", 1)):
            h = Harness()
            values = dict(valid, **{name: bad})
            with self.assertRaises(ValueError):
                m.NativeInertRunner(h.exports, **values)
            self.assertEqual(h.calls, [])
        for name in valid:
            h = Harness()
            values = {key: value for key, value in valid.items() if key != name}
            with self.assertRaises(ValueError):
                m.NativeInertRunner(h.exports, **values)
            self.assertEqual(h.calls, [])

    def test_external_runner_references_and_occurrence_cannot_change(self):
        mutations = {
            "security": lambda r: setattr(r, "security", object()),
            "pipe": lambda r: setattr(r, "pipe_owner", object()),
            "image": lambda r: setattr(r, "image_pin", object()),
            "receipt": lambda r: setattr(r, "receipt", m.Receipt(deadline=700)),
            "external": lambda r: setattr(r, "_external", False),
            "supplied-deadline": lambda r: setattr(r, "_supplied_deadline", 701),
            "nonce": lambda r: setattr(r, "_supplied_nonce", b"z" * 32),
            "callback": lambda r: setattr(r, "_before_resume", lambda *_: None),
            "timeout": lambda r: setattr(r, "timeout_ms", 5000),
            "used": lambda r: setattr(r, "_used", False),
            "custody-snapshot": lambda r: setattr(r, "_prepared_custody", None),
            "pipe-exports": lambda r: setattr(r.pipe_owner, "exports", object()),
            "pipe-security": lambda r: setattr(r.pipe_owner, "security", object()),
            "pipe-deadline": lambda r: setattr(r.pipe_owner, "deadline", 701),
            "pipe-endpoints": lambda r: setattr(r.pipe_owner, "receipt", m.pipes.ConnectedEndpoints(91, 92, 93, 94)),
            "security-exports": lambda r: setattr(r.security, "exports", object()),
            "image-exports": lambda r: setattr(r.image_pin, "exports", object()),
        }
        for name, mutate in mutations.items():
            with self.subTest(name=name):
                h = Harness()
                self.external(h, lambda *_: mutate(h.runner))
                original_receipt = h.runner.receipt
                with self.assertRaises(m.RunFailure):
                    h.run()
                self.assertIs(h.runner.receipt, original_receipt)
                self.assertTrue(h.runner._custody_uncertain)
                self.assertIs(h.runner._prepared_custody[-1][0], original_receipt)
                self.assertNotIn("ResumeThread", h.calls)
                self.assertEqual(h.frames, [])
                with self.assertRaises(m.Hold):
                    h.runner.run()
                self.assertNotIn("ResumeThread", h.calls)

    def test_external_deadline_spoof_cannot_extend_original_occurrence(self):
        class Spoof:
            def __eq__(self, other):
                return True
            def __sub__(self, other):
                return 1
        for target in ("receipt-worker", "private", "supplied"):
            h = Harness()
            def callback(worker, nonce):
                spoof = Spoof()
                if target == "receipt-worker":
                    worker._deadline = h.runner.receipt.deadline = spoof
                elif target == "private":
                    h.runner._deadline = spoof
                else:
                    h.runner._supplied_deadline = spoof
                h.now = 1001
            self.external(h, callback)
            with self.assertRaises(m.RunFailure):
                h.run()
            self.assertNotIn("ResumeThread", h.calls)
            self.assertTrue(h.runner._custody_uncertain)
            self.assertEqual(h.frames, [])

    def test_external_subordinate_ownership_and_frozen_identity_cannot_change(self):
        mutations = {
            "clear-handles": lambda w: w.pipe_owner.handles.clear(),
            "replace-handle-set": lambda w: setattr(w.pipe_owner, "handles", set(w.pipe_owner.handles)),
            "replace-operations": lambda w: setattr(w.pipe_owner, "operations", []),
            "remote-value": lambda w: object.__setattr__(w.receipt.remote_child_job, "value", 999),
            "remote-owner": lambda w: object.__setattr__(w.receipt.remote_child_job, "owner_process", 999),
            "image-handle": lambda w: setattr(w.image_pin, "handle", 999),
            "image-reopen": lambda w: setattr(w.image_pin, "reopened_handle", 999),
            "image-hash": lambda w: object.__setattr__(w.image_pin.snapshot, "sha256", "b" * 64),
            "image-file-id": lambda w: object.__setattr__(w.image_pin.snapshot.identity, "file_id", b"z" * 16),
            "image-verified": lambda w: setattr(w.image_pin, "post_create_verified", False),
            "endpoint-handle": lambda w: object.__setattr__(w.pipe_owner.receipt, "parent_write", 999),
        }
        for name, mutate in mutations.items():
            with self.subTest(name=name):
                h = Harness()
                self.external(h, lambda worker, nonce: mutate(worker))
                with self.assertRaises(m.RunFailure):
                    h.run()
                self.assertTrue(h.runner._custody_uncertain)
                subordinate = h.runner._prepared_custody[-2]
                self.assertEqual(subordinate[1], frozenset({10, 11, 12, 13}))
                self.assertEqual(subordinate[5], (44, 31))
                self.assertEqual(subordinate[6], 50)
                self.assertEqual(subordinate[10][0], "a" * 64)
                self.assertNotIn("ResumeThread", h.calls)
                self.assertNotIn("CloseHandle", h.calls)
                self.assertNotIn("pipes.cleanup", h.calls)
                self.assertEqual(h.frames, [])

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
