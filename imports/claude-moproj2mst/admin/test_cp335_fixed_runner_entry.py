"""Injected stubs only; no native, process, GUI, or original application run."""
import importlib
import runpy
import ctypes as C
from types import SimpleNamespace
import unittest
from unittest.mock import patch

import cp335_fixed_runner_entry as entry
import cp339_terminal_protocol as terminal
import cp332_exact_worker_handoff as handoff_wire

ARGV = ["--cp334-client", "11", "12", "100"]


class Stubs:
    def __init__(self):
        self.events = []
        self.exports = object()
        self.cleanup_ok = True
        self.fail_bootstrap = self.fail_run = self.skip_ack = False
        self.fail_terminal = False
        self.short_terminal = False
        self.marker_failure = False
        self.bootstrap_deadline = 100
        self.pipe = SimpleNamespace(cleanup=self.cleanup, transfer=self.transfer,
                                    receipt=SimpleNamespace(client_read=11, client_write=12))
        self.terminal_frame = None
        state = self

        class Handoff:
            def __init__(self, exports, *, pipe_owner, deadline):
                assert exports is state.exports and pipe_owner is state.pipe
                assert deadline == 100
                self.phase = "new"
                state.handoff = self

            def receive_bootstrap(self):
                state.events.append("bootstrap")
                if state.fail_bootstrap:
                    raise RuntimeError("bootstrap failure")
                self.nonce = b"n" * 32
                return state.bootstrap_deadline, b"n" * 32

            def before_resume(self, worker, nonce):
                assert nonce == b"n" * 32
                state.events.append("offer-ack")
                self.phase = "authorized"

        class Runner:
            def __init__(self, exports, *, deadline, nonce, before_resume):
                assert exports is state.exports and deadline == 100
                assert nonce == b"n" * 32
                state.events.append("runner-construct")
                self.before_resume, self.nonce = before_resume, nonce
                self.receipt = SimpleNamespace(phase="new", deadline=deadline,
                    resume_outcome="not-attempted", init_outcome="not-attempted",
                    result_outcome="not-attempted", peek_outcome="not-attempted",
                    ack_outcome="not-attempted", errors=[])
                self.worker = SimpleNamespace(receipt=SimpleNamespace(
                    phase="prepared-suspended", worker_waited=False, jobs_empty=False,
                    worker_terminated=False, errors=[]))

            def run(self):
                if not state.skip_ack:
                    self.before_resume(self.worker, self.nonce)
                state.events.append("resume")
                if state.fail_run:
                    raise RuntimeError("uncertain worker")
                self.receipt.phase = "closed-success"
                self.worker.receipt.phase = "closed"
                self.receipt.resume_outcome = "resumed-once-previous-count-one"
                self.receipt.init_outcome = "48-bytes-written"
                self.receipt.result_outcome = "80-byte-zero-child-result"
                self.receipt.peek_outcome = "zero-queued-byte-snapshot"
                self.receipt.ack_outcome = "exact-worker-exit-zero-observed"
                self.worker.receipt.worker_waited = True
                self.worker.receipt.jobs_empty = True
                return self.receipt

            def close(self):
                state.events.append("runner-close")

        self.Handoff, self.Runner = Handoff, Runner

    def cleanup(self):
        self.events.append("client-cleanup")
        return self.cleanup_ok

    def transfer(self, handle, kind, payload):
        self.events.append("terminal-write")
        assert handle == 12 and kind == "write" and type(payload) is bytes
        self.terminal_frame = payload
        if self.fail_terminal:
            raise RuntimeError("terminal write outcome unknown")
        return len(payload) - 1 if self.short_terminal else len(payload)

    def load(self):
        self.events.append("load")
        return self.exports

    def mark(self, nonce, deadline, stage):
        self.events.append("progress-" + stage)
        if self.marker_failure:
            raise OSError("diagnostic unavailable")
        return True

    def adopt(self, exports, argv):
        assert exports is self.exports and tuple(argv) == tuple(ARGV)
        self.events.append("adopt-two")
        return self.pipe

    def patches(self):
        return (patch.object(entry.binding, "load_native_exports", self.load),
                patch.object(entry.progress, "write_progress", self.mark),
                patch.object(entry.client, "from_bootstrap_argv", self.adopt),
                patch.object(entry.integration, "RunnerHandoff", self.Handoff),
                patch.object(entry.runner, "NativeInertRunner", self.Runner))


class EntryTests(unittest.TestCase):
    def run_stubs(self, state):
        from contextlib import ExitStack
        stack = ExitStack()
        self.addCleanup(stack.close)
        for context in state.patches():
            stack.enter_context(context)
        return entry.FixedRunnerEntry(ARGV)

    def test_canonical_grammar(self):
        self.assertEqual(entry.parse_argv(ARGV), (11, 12, 100))
        self.assertEqual(entry.parse_argv([ARGV[0], "1", "2", str(2**64 - 1)]),
                         (1, 2, 2**64 - 1))
        invalid = [None, " ", [], ARGV + ["path"], ARGV[1:],
                   ["--other", "11", "12", "100"]]
        for index in (1, 2, 3):
            for value in ("0", "-1", "+1", "01", " 1", "1 ", "1.0", "1e2",
                          "١", "１", "1\x00", str(2**64), "9" * 200, 1, True):
                bad = list(ARGV)
                bad[index] = value
                invalid.append(bad)
        invalid.extend(([ARGV[0], "11", "11", "100"],
                        [ARGV[0], str(2**64 - 16), "12", "100"]))
        for argv in invalid:
            with self.subTest(argv=argv), self.assertRaises(entry.Hold):
                entry.parse_argv(argv)

    def test_invalid_before_binding_and_owner_bearing(self):
        with patch.object(entry.binding, "load_native_exports") as load:
            owner = entry.FixedRunnerEntry(ARGV + ["arbitrary.py"])
            with self.assertRaises(entry.EntryFailure) as caught:
                owner.run()
            self.assertIs(caught.exception.owner, owner)
            load.assert_not_called()

    def test_missing_hook_before_binding(self):
        class OldRunner:
            def __init__(self, exports, *, timeout_ms=5000):
                pass
        with patch.object(entry.runner, "NativeInertRunner", OldRunner), \
                patch.object(entry.binding, "load_native_exports") as load:
            with self.assertRaisesRegex(entry.EntryFailure, "hooks required"):
                entry.FixedRunnerEntry(ARGV).run()
            load.assert_not_called()

    def test_order_one_deadline_one_use(self):
        state = Stubs()
        owner = self.run_stubs(state)
        self.assertEqual(state.events, [])
        self.assertIs(owner.run(), owner.receipt)
        self.assertEqual(owner.receipt.phase, "closed-success")
        self.assertEqual(owner.receipt.deadline, 100)
        self.assertEqual(owner.receipt.terminal_outcome,
                         "80-byte-claim-written-parent-unconfirmed")
        self.assertEqual(terminal.decode_terminal(state.terminal_frame,
                         deadline=100, nonce=b"n" * 32),
                         terminal.TerminalSuccess(100, b"n" * 32))
        self.assertEqual(state.events, ["load", "adopt-two", "bootstrap", "progress-bootstrap",
                         "runner-construct", "offer-ack", "progress-parent_ack", "resume",
                         "progress-terminal_entered", "terminal-write",
                         "progress-terminal_returned", "runner-close", "client-cleanup"])
        before = list(state.events)
        with self.assertRaises(entry.EntryFailure):
            owner.run()
        self.assertEqual(state.events, before)

    def test_adoption_failure_retains_owner(self):
        state = Stubs()
        owner = self.run_stubs(state)
        failure = RuntimeError("query failed")
        failure.owner = state.pipe
        with patch.object(entry.client, "from_bootstrap_argv", side_effect=failure):
            with self.assertRaises(entry.EntryFailure) as caught:
                owner.run()
        self.assertIs(caught.exception.owner.pipe_owner, state.pipe)
        self.assertEqual(state.events, ["load", "client-cleanup"])

    def test_bootstrap_failure_or_changed_deadline_prevents_worker(self):
        for mode in ("failure", "changed"):
            state = Stubs()
            state.fail_bootstrap = mode == "failure"
            state.bootstrap_deadline = 101
            owner = self.run_stubs(state)
            with self.assertRaises(entry.EntryFailure):
                owner.run()
            self.assertNotIn("runner-construct", state.events)
            self.assertIn("client-cleanup", state.events)

    def test_cleanup_failure_is_retryable_without_protocol_replay(self):
        state = Stubs()
        state.cleanup_ok = False
        owner = self.run_stubs(state)
        with self.assertRaises(entry.EntryFailure) as caught:
            owner.run()
        self.assertIs(caught.exception.owner, owner)
        self.assertEqual(owner.receipt.phase, "hold")
        state.cleanup_ok = True
        self.assertTrue(owner.close())
        for event in ("load", "adopt-two", "bootstrap", "offer-ack", "resume"):
            self.assertEqual(state.events.count(event), 1)

    def test_worker_failure_retains_uncertain_owner(self):
        state = Stubs()
        state.fail_run = True
        owner = self.run_stubs(state)
        with self.assertRaises(entry.EntryFailure):
            owner.run()
        self.assertIsNotNone(owner.runner)
        self.assertFalse(owner.close())
        self.assertEqual(state.events.count("resume"), 1)

    def test_terminal_failure_retains_unknown_write_and_never_replays(self):
        for mode in ("exception", "short"):
            state = Stubs()
            state.fail_terminal = mode == "exception"
            state.short_terminal = mode == "short"
            owner = self.run_stubs(state)
            with self.subTest(mode=mode), self.assertRaises(entry.EntryFailure) as caught:
                owner.run()
            self.assertIs(caught.exception.owner, owner)
            self.assertEqual(owner.receipt.phase, "hold")
            self.assertEqual(owner.receipt.terminal_outcome,
                             "unknown: terminal write entered")
            self.assertEqual(state.events.count("terminal-write"), 1)
            self.assertNotIn("progress-terminal_returned", state.events)
            self.assertEqual(terminal.decode_terminal(state.terminal_frame,
                             deadline=100, nonce=b"n" * 32),
                             terminal.TerminalSuccess(100, b"n" * 32))
            self.assertTrue(owner.close())
            self.assertEqual(state.events.count("terminal-write"), 1)

    def test_marker_failure_never_gates_success(self):
        state = Stubs()
        state.marker_failure = True
        owner = self.run_stubs(state)
        self.assertIs(owner.run(), owner.receipt)
        self.assertEqual(owner.receipt.phase, "closed-success")
        self.assertEqual(state.events.count("terminal-write"), 1)
        self.assertLess(state.events.index("progress-terminal_entered"), state.events.index("terminal-write"))
        self.assertGreater(state.events.index("progress-terminal_returned"), state.events.index("terminal-write"))

    def test_real_adopted_client_and_handoff_send_terminal_after_ack(self):
        """Compose actual CP334/CP333 owners with injected Win32 exports."""
        nonce = b"n" * 32
        offer = handoff_wire.Offer(71, 72, 73, 123456, nonce, 100)
        reads = [handoff_wire.encode_bootstrap(100, nonce),
                 handoff_wire.encode_ack(offer)]
        writes, calls = [], []

        def get_handle_information(handle, flags):
            flags._obj.value = 1
            return True

        def get_named_pipe_info(handle, flags, unused_out, unused_in, unused_count):
            flags._obj.value = entry.client.PIPE_REJECT_REMOTE_CLIENTS
            return True

        def get_process_times(handle, created, *unused):
            self.assertEqual(handle, 71)
            C.cast(created, C.POINTER(entry.abi.FILETIME)).contents.dwLowDateTime = 123456
            return True

        exports = entry.binding.BoundExports({
            "GetTickCount64": lambda: 1,
            "GetHandleInformation": get_handle_information,
            "GetNamedPipeInfo": get_named_pipe_info,
            "GetProcessTimes": get_process_times,
            "CloseHandle": lambda handle: calls.append(("close", handle)) or True,
        }, {}, lambda: 0)

        real_adopt = entry.client.from_bootstrap_argv

        def adopt(bound, argv):
            owner = real_adopt(bound, argv)

            def transfer(handle, kind, payload):
                calls.append((kind, handle))
                if kind == "read":
                    self.assertEqual(handle, 11)
                    raw = reads.pop(0)
                    self.assertEqual(len(raw), payload)
                    return raw
                self.assertEqual((handle, kind), (12, "write"))
                writes.append(payload)
                return len(payload)

            owner.transfer = transfer
            return owner

        class Runner:
            def __init__(self, bound, *, deadline, nonce, before_resume):
                self.receipt = SimpleNamespace(phase="new", deadline=deadline, errors=[],
                    resume_outcome="not-attempted", init_outcome="not-attempted",
                    result_outcome="not-attempted", peek_outcome="not-attempted",
                    ack_outcome="not-attempted")
                self.worker = SimpleNamespace(exports=bound, deadline=deadline,
                    receipt=SimpleNamespace(phase="prepared-suspended", pid=73, tid=74,
                        local={"worker_process": 71, "worker_job": 72,
                               "worker_thread": 75, "child_job": 77},
                        remote_child_job=handoff_wire.creator.RemoteHandle(76, 71),
                        worker_waited=False, jobs_empty=False,
                        worker_terminated=False, errors=[]))
                self.before_resume, self.nonce = before_resume, nonce

            def run(self):
                self.before_resume(self.worker, self.nonce)
                calls.append(("resume", 75))
                self.receipt.phase = "closed-success"
                self.receipt.resume_outcome = "resumed-once-previous-count-one"
                self.receipt.init_outcome = "48-bytes-written"
                self.receipt.result_outcome = "80-byte-zero-child-result"
                self.receipt.peek_outcome = "zero-queued-byte-snapshot"
                self.receipt.ack_outcome = "exact-worker-exit-zero-observed"
                self.worker.receipt.phase = "closed"
                self.worker.receipt.worker_waited = True
                self.worker.receipt.jobs_empty = True
                return self.receipt

            def close(self):
                calls.append(("runner-close", 0))

        with patch.object(entry.binding, "load_native_exports", return_value=exports), \
                patch.object(entry.client, "from_bootstrap_argv", adopt), \
                patch.object(entry.progress, "write_progress", return_value=True), \
                patch.object(entry.runner, "NativeInertRunner", Runner):
            owner = entry.FixedRunnerEntry(ARGV)
            self.assertEqual(owner.run().phase, "closed-success")
        self.assertEqual(reads, [])
        self.assertEqual(writes[0], handoff_wire.encode_offer(offer))
        self.assertEqual(terminal.decode_terminal(writes[1], deadline=100, nonce=nonce),
                         terminal.TerminalSuccess(100, nonce))
        self.assertEqual([event for event in calls if event[0] == "write"],
                         [("write", 12), ("write", 12)])
        self.assertLess(calls.index(("resume", 75)), calls.index(("close", 11)))
        self.assertEqual({event for event in calls if event[0] == "close"},
                         {("close", 11), ("close", 12)})

    def test_missing_authorization_cannot_report_success(self):
        state = Stubs()
        state.skip_ack = True
        owner = self.run_stubs(state)
        with self.assertRaisesRegex(entry.EntryFailure, "authorized exact"):
            owner.run()

    def test_import_and_public_main_are_inert(self):
        with patch.object(entry.binding, "load_native_exports") as load, \
                patch.object(entry.client, "from_bootstrap_argv") as adopt:
            importlib.reload(entry)
            self.assertEqual(entry.NATIVE_ENTRY, "HARD HOLD")
            with self.assertRaisesRegex(entry.Hold, "HARD HOLD"):
                runpy.run_path(entry.__file__, run_name="__main__")
            load.assert_not_called()
            adopt.assert_not_called()


if __name__ == "__main__":
    unittest.main()
