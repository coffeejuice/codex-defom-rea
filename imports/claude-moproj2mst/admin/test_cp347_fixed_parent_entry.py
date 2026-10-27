"""CP347 injected-only tests; no native process or GUI execution."""
import hashlib
import importlib
import runpy
from types import SimpleNamespace
import unittest
from unittest.mock import Mock, patch

import cp347_fixed_parent_entry as entry
import cp334_runner_client_pipe as client

NONCE = b"p" * 32
DEADLINE = 4242


def accepted_receipt():
    return SimpleNamespace(deadline=DEADLINE, phase="closed-success",
        terminal_outcome="80-byte-occurrence-validated-zero-queued-snapshot",
        worker_outcome="exact-worker-observed-and-duplicates-released",
        runner_outcome="exact-runner-exit-zero-job-empty-observed",
        cleanup_outcome="terminal-parent-owners-released", errors=[])


class Sink:
    def __init__(self):
        self.events = []
        self.raw = None
        self.short = self.fail_write = self.fail_flush = False
        self.on_write = None

    def write(self, raw):
        self.events.append("write")
        self.raw = raw
        if self.on_write:
            self.on_write()
        if self.fail_write:
            raise RuntimeError("write entered, completion unknown")
        return len(raw) - 1 if self.short else len(raw)

    def flush(self):
        self.events.append("flush")
        if self.fail_flush:
            raise RuntimeError("flush entered, completion unknown")


class SimpleClient:
    def __init__(self, exports, deadline, events):
        self.exports, self.deadline, self.events = exports, deadline, events
        self.receipt = client.ClientEndpoints(11, 12)
        self.handles, self.operations = {11, 12}, []
        self.started, self.held = True, False
        self.raw = None
        self.short = self.cleanup_fail = self.cleanup_false_success = False
        self.on_cleanup = None

    def transfer(self, handle, kind, payload):
        assert handle == 12 and kind == "write" and type(payload) is bytes
        self.events.append("overlapped-write")
        self.raw = payload
        return len(payload) - int(self.short)

    def cleanup(self):
        self.events.append("client-cleanup")
        if self.on_cleanup:
            self.on_cleanup()
        if self.cleanup_fail:
            return False
        if self.cleanup_false_success:
            return True
        self.handles.clear()
        self.receipt = None
        self.held = True
        return True


class StubState:
    def __init__(self):
        self.events = []
        self.exports = object()
        self.fail_ack = self.fail_complete = False
        self.cleanup_ok = True
        state = self

        class Integration:
            def __init__(self, exports):
                assert exports is state.exports
                state.events.append("construct")
                self.exports = exports
                self.receipt = accepted_receipt()
                self.nonce = NONCE
                self._accepted_success = False
                self._verified_success = False
                state.owner = self

            def acknowledge(self):
                state.events.append("acknowledge")
                if state.fail_ack:
                    raise RuntimeError("ACK uncertain")

            def complete(self):
                state.events.append("complete")
                if state.fail_complete:
                    raise RuntimeError("terminal failure")
                self._accepted_success = self._verified_success = True
                self._completion_owners = (state.exports, None, None, None,
                    None, self.receipt, self.nonce, self.receipt.deadline, ())
                return self.receipt

            def close(self):
                state.events.append("close")
                return state.cleanup_ok

        self.Integration = Integration

    def load(self):
        self.events.append("load")
        return self.exports


class ParentEntryTests(unittest.TestCase):
    def setUp(self):
        self.state = StubState()
        self.sink = Sink()
        loader = patch.object(entry.binding, "load_native_exports", self.state.load)
        owner = patch.object(entry.integration, "WatchdogIntegration", self.state.Integration)
        self.owner_patcher = owner
        loader.start(); owner.start()
        self.addCleanup(loader.stop)
        self.addCleanup(owner.stop)

    def test_import_and_construction_inert_public_hold(self):
        importlib.reload(entry)
        parent = entry.FixedParentEntry(self.sink)
        self.assertEqual(parent.receipt.phase, "new")
        self.assertEqual(self.state.events, [])
        with self.assertRaises(entry.Hold):
            entry.launch_trial()
        with self.assertRaises(entry.Hold):
            runpy.run_module("cp347_fixed_parent_entry", run_name="__main__")
        self.assertEqual(self.state.events, [])

    def test_exact_claim_and_order(self):
        parent = entry.FixedParentEntry(self.sink)
        result = parent.run()
        self.assertIs(result, parent.receipt)
        self.assertEqual(result.phase, "closed-local-success-controller-unverified")
        self.assertEqual(result.claim_outcome, "80-byte-claim-written-and-flushed")
        self.assertEqual(self.state.events, ["load", "construct", "acknowledge", "complete"])
        self.assertEqual(self.sink.events, ["write", "flush"])
        self.assertEqual(len(self.sink.raw), 80)
        self.assertEqual(entry.decode_parent(self.sink.raw, deadline=DEADLINE, nonce=NONCE),
                         entry.ParentSuccess(DEADLINE, NONCE))
        self.assertTrue(parent.close())
        self.assertEqual(self.state.events[-1], "close")
        with self.assertRaises(entry.ParentEntryFailure):
            parent.run()
        self.assertEqual(self.sink.events, ["write", "flush"])

    def test_fixed_frame_rejects_corruption_and_other_occurrence(self):
        raw = entry.encode_parent(entry.ParentSuccess(DEADLINE, NONCE))
        self.assertEqual(len(raw), entry.FRAME.size)
        self.assertNotEqual(entry.MAGIC, 0x39333343)
        for bad in (raw[:-1], raw + b"x", bytearray(raw),
                    raw[:16] + b"\0\0\0\0" + raw[20:],
                    raw[:56] + b"x" + raw[57:]):
            with self.subTest(bad=bad), self.assertRaises(entry.ParentClaimError):
                entry.decode_parent(bad)
        with self.assertRaises(entry.ParentClaimError):
            entry.decode_parent(raw, deadline=DEADLINE + 1)
        with self.assertRaises(entry.ParentClaimError):
            entry.decode_parent(raw, nonce=b"q" * 32)
        with self.assertRaises(entry.ParentClaimError):
            entry.encode_parent(SimpleNamespace(deadline=DEADLINE, nonce=NONCE))
        self.assertEqual(hashlib.sha256(raw[:56]).digest()[:24], raw[56:])

    def test_no_claim_before_complete_or_when_receipt_incomplete(self):
        self.state.fail_ack = True
        parent = entry.FixedParentEntry(self.sink)
        with self.assertRaises(entry.ParentEntryFailure) as caught:
            parent.run()
        self.assertIs(caught.exception.owner, parent)
        self.assertEqual(self.sink.events, [])
        self.assertIn("close", self.state.events)
        self.state.fail_ack = False
        self.state.fail_complete = True
        parent2 = entry.FixedParentEntry(Sink())
        with self.assertRaises(entry.ParentEntryFailure):
            parent2.run()
        self.assertIsNone(parent2._claim)
        self.state.fail_complete = False
        owner = self.state.owner
        owner._accepted_success = owner._verified_success = True
        owner._completion_owners = (self.state.exports, None, None, None,
            None, owner.receipt, owner.nonce, owner.receipt.deadline, ())
        for name in ("terminal_outcome", "worker_outcome", "runner_outcome",
                     "cleanup_outcome", "phase"):
            saved = getattr(owner.receipt, name)
            setattr(owner.receipt, name, "not-observed")
            with self.subTest(name=name), self.assertRaises(entry.ParentClaimError):
                entry.from_integration(owner)
            setattr(owner.receipt, name, saved)

    def test_short_and_uncertain_write_never_replay(self):
        for mode in ("short", "fail_write", "fail_flush"):
            with self.subTest(mode=mode):
                sink = Sink()
                setattr(sink, mode, True)
                parent = entry.FixedParentEntry(sink)
                with self.assertRaises(entry.ParentEntryFailure) as caught:
                    parent.run()
                self.assertIs(caught.exception.owner, parent)
                self.assertEqual(parent.receipt.phase, "hold")
                self.assertEqual(parent.receipt.claim_outcome,
                                 "unknown: flush entered" if mode == "fail_flush"
                                 else "unknown: 80-byte write entered")
                self.assertEqual(sink.events.count("write"), 1)
                parent.close()
                self.assertEqual(sink.events.count("write"), 1)
                with self.assertRaises(entry.ParentEntryFailure):
                    parent.run()

    def test_mutation_during_write_blocks_local_success(self):
        parent = entry.FixedParentEntry(self.sink)
        def mutate():
            parent.integration.receipt.worker_outcome = "forged"
        self.sink.on_write = mutate
        with self.assertRaises(entry.ParentEntryFailure) as caught:
            parent.run()
        self.assertIs(caught.exception.owner, parent)
        self.assertEqual(parent.receipt.phase, "hold")
        self.assertEqual(self.sink.events, ["write", "flush"])

    def test_owner_replacement_during_write_retains_original_cleanup(self):
        parent = entry.FixedParentEntry(self.sink)
        original_receipt = parent.receipt
        def mutate():
            parent.integration = object()
            parent._owner = object()
            parent.receipt = entry.Receipt(phase="forged")
        self.sink.on_write = mutate
        with self.assertRaises(entry.ParentEntryFailure):
            parent.run()
        self.assertIs(parent._owner, self.state.owner)
        self.assertIs(parent.receipt, original_receipt)
        self.assertEqual(self.state.events[-1], "close")
        self.assertEqual(self.sink.events.count("write"), 1)

    def test_forged_public_receipt_does_not_create_claim(self):
        parent = entry.FixedParentEntry(self.sink)
        def mutate():
            parent.integration.receipt = accepted_receipt()
        self.sink.on_write = mutate
        with self.assertRaises(entry.ParentEntryFailure):
            parent.run()
        self.assertEqual(parent.receipt.phase, "hold")
        self.assertEqual(self.sink.events.count("write"), 1)

    def test_sink_swap_during_write_never_flushes_replacement_or_accepts(self):
        parent = entry.FixedParentEntry(self.sink)
        replacement = Sink()
        def mutate():
            parent.sink = parent._sink = replacement
        self.sink.on_write = mutate
        with self.assertRaises(entry.ParentEntryFailure):
            parent.run()
        self.assertEqual(self.sink.events, ["write"])
        self.assertEqual(replacement.events, [])
        self.assertIs(parent.sink, self.sink)
        self.assertIs(parent._sink, self.sink)
        self.assertEqual(parent.receipt.phase, "hold")
        with self.assertRaises(entry.ParentEntryFailure):
            parent.run()
        self.assertEqual(self.sink.events, ["write"])

    def test_used_flag_reset_inside_write_cannot_replay_occurrence(self):
        parent = entry.FixedParentEntry(self.sink)
        def mutate():
            parent._used = False
        self.sink.on_write = mutate
        with self.assertRaises(entry.ParentEntryFailure):
            parent.run()
        self.assertIs(parent._used, True)
        self.assertEqual(self.state.events.count("acknowledge"), 1)
        self.assertEqual(self.sink.events.count("write"), 1)
        with self.assertRaises(entry.ParentEntryFailure):
            parent.run()
        self.assertEqual(self.state.events.count("acknowledge"), 1)
        self.assertEqual(self.sink.events.count("write"), 1)

    def test_sink_required_without_loader_call(self):
        with self.assertRaises(TypeError):
            entry.FixedParentEntry(object())
        self.assertEqual(self.state.events, [])

    def test_actual_cp333_acknowledge_freezes_nine_field_occurrence(self):
        # Exercise CP333's production tuple construction, not this file's stub.
        self.owner_patcher.stop()
        import test_cp333_watchdog_runner_integration as cp333_tests
        h, watchdog, pipe_factory, control_factory = (
            cp333_tests.OuterTests().setup_contract())
        descriptor = Mock()
        descriptor.prepare.return_value = None
        with patch.object(entry.integration.outer, "OuterWatchdog", return_value=watchdog), \
             patch.object(entry.integration.security, "NativePrivatePipeSecurity",
                          return_value=descriptor), \
             patch.object(entry.integration.pipes, "NativePipeOwner",
                          side_effect=pipe_factory), \
             patch.object(entry.integration.handoff, "ExactWorkerHandoff",
                          side_effect=control_factory), \
             patch.object(entry.integration.secrets, "token_bytes", return_value=h.nonce):
            actual = entry.integration.WatchdogIntegration(h.exports)
            actual.acknowledge()
        descriptor.prepare.assert_called_once_with()
        self.assertEqual(len(actual._completion_owners), 9)
        self.assertIs(actual._completion_owners[5], actual.receipt)
        actual._accepted_success = actual._verified_success = True
        actual.receipt.phase = "closed-success"
        actual.receipt.terminal_outcome = (
            "80-byte-occurrence-validated-zero-queued-snapshot")
        actual.receipt.worker_outcome = (
            "exact-worker-observed-and-duplicates-released")
        actual.receipt.runner_outcome = (
            "exact-runner-exit-zero-job-empty-observed")
        actual.receipt.cleanup_outcome = "terminal-parent-owners-released"
        self.assertEqual(entry.from_integration(actual),
                         entry.ParentSuccess(500, h.nonce))

    def test_fixed_client_argv_strict_before_load(self):
        valid = ["--cp334-client", "11", "12", "4000"]
        self.assertEqual(entry.parse_client_argv(valid), (11, 12, 4000))
        owner = entry.FixedParentEntry.from_client_argv(valid)
        self.assertEqual(owner.receipt.phase, "new")
        self.assertEqual(self.state.events, [])
        invalid = [[], valid + ["path"], valid[1:],
                   ["--other", "11", "12", "4000"],
                   [valid[0], "11", "11", "4000"],
                   [valid[0], "0", "12", "4000"]]
        for idx in (1, 2, 3):
            for value in ("01", "-1", "+1", "1.0", " 1", "1 ", "١", "",
                          str(2**64), object()):
                bad = list(valid); bad[idx] = value; invalid.append(bad)
        for bad in invalid:
            with self.subTest(argv=bad), self.assertRaises(entry.Hold):
                entry.FixedParentEntry.from_client_argv(bad)
        self.assertEqual(self.state.events, [])

    def test_distinct_channel_and_inner_deadlines_overlapped_claim(self):
        channel_deadline = 4000  # CP349 channel; CP333 inner deadline is 4242.
        argv = [client.ARGV_TAG, "11", "12", str(channel_deadline)]
        events = []
        owner = SimpleClient(self.state.exports, channel_deadline, events)
        def adopt(exports, args):
            self.assertIs(exports, self.state.exports)
            self.assertEqual(tuple(args), tuple(argv))
            events.append("adopt")
            return owner
        with patch.object(entry.client, "from_bootstrap_argv", side_effect=adopt):
            parent = entry.FixedParentEntry.from_client_argv(argv)
            result = parent.run()
        self.assertEqual(result.phase, "closed-local-success-controller-unverified")
        self.assertEqual(result.client_cleanup_outcome, "terminal-client-owners-released")
        self.assertEqual(events, ["adopt", "overlapped-write", "client-cleanup"])
        self.assertEqual(owner.handles, set())
        self.assertEqual(entry.decode_parent(owner.raw).deadline, DEADLINE)
        self.assertNotEqual(channel_deadline, DEADLINE)

    def test_client_adoption_failure_retains_owner(self):
        argv = [client.ARGV_TAG, "11", "12", "4000"]
        events = []
        owner = SimpleClient(self.state.exports, 4000, events)
        failure = RuntimeError("adoption entered, outcome unknown")
        failure.owner = owner
        with patch.object(entry.client, "from_bootstrap_argv", side_effect=failure):
            parent = entry.FixedParentEntry.from_client_argv(argv)
            with self.assertRaises(entry.ParentEntryFailure) as caught:
                parent.run()
        self.assertIs(caught.exception.owner, parent)
        self.assertIs(parent._client_owner, owner)
        self.assertEqual(events, ["client-cleanup"])
        self.assertNotIn("acknowledge", self.state.events)

    def test_client_claim_short_or_cleanup_uncertain_never_local_success(self):
        argv = [client.ARGV_TAG, "11", "12", "4000"]
        for mode in ("short", "cleanup_fail"):
            with self.subTest(mode=mode):
                events = []
                owner = SimpleClient(self.state.exports, 4000, events)
                setattr(owner, mode, True)
                with patch.object(entry.client, "from_bootstrap_argv", return_value=owner):
                    parent = entry.FixedParentEntry.from_client_argv(argv)
                    with self.assertRaises(entry.ParentEntryFailure):
                        parent.run()
                self.assertEqual(parent.receipt.phase, "hold")
                self.assertEqual(events.count("overlapped-write"), 1)

    def test_client_cleanup_mutation_or_false_success_cannot_accept(self):
        argv = [client.ARGV_TAG, "11", "12", "4000"]
        for mode in ("mutate_cp333", "false_success"):
            with self.subTest(mode=mode):
                events = []
                owner = SimpleClient(self.state.exports, 4000, events)
                if mode == "mutate_cp333":
                    owner.on_cleanup = lambda: setattr(
                        self.state.owner.receipt, "worker_outcome", "forged-after-claim")
                else:
                    owner.cleanup_false_success = True
                with patch.object(entry.client, "from_bootstrap_argv", return_value=owner):
                    parent = entry.FixedParentEntry.from_client_argv(argv)
                    with self.assertRaises(entry.ParentEntryFailure):
                        parent.run()
                self.assertEqual(parent.receipt.phase, "hold")
                self.assertEqual(events.count("overlapped-write"), 1)
                self.assertNotEqual(parent.receipt.phase,
                                    "closed-local-success-controller-unverified")
                with self.assertRaises(entry.ParentEntryFailure):
                    parent.run()
                self.assertEqual(events.count("overlapped-write"), 1)


if __name__ == "__main__":
    unittest.main()
