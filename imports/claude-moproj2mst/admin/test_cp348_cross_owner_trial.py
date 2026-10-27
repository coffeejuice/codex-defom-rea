"""Shared injected parent/runner converter-control occurrence.

Real CP333/332 parent and CP335/334/333/330 runner owners exchange binary
frames over bounded in-memory queues. CP331, CP329, Win32 exports, transport
operations and the WORKER are injected; no DLL, native process or GUI runs.
"""
from pathlib import Path
from queue import Queue
from threading import Thread
from types import SimpleNamespace
import sys
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp332_exact_worker_handoff as parent_wire
import cp333_watchdog_runner_integration as runner_wire
import cp334_runner_client_pipe as client
import cp330_native_inert_runner as inert
import cp335_fixed_runner_entry as entry
import cp339_terminal_protocol as terminal
from test_cp332_exact_worker_handoff import Harness


class SharedChannel:
    def __init__(self, exports, *, corrupt_ack=False):
        self.to_runner, self.to_parent = Queue(), Queue()
        self.events = []
        self.corrupt_ack = corrupt_ack
        self.parent = ParentEnd(self, exports)
        self.runner = RunnerEnd(self, exports)


class ParentEnd:
    def __init__(self, channel, exports):
        self.channel, self.exports, self.deadline = channel, exports, 500
        self.receipt = SimpleNamespace(parent_write=11, client_read=13,
                                       parent_read=12, client_write=14)
        self.operations = []
        self.held = False

    def transfer(self, handle, kind, payload):
        self.channel.events.append(("parent", handle, kind, payload))
        if kind == "write":
            assert handle == 11 and type(payload) is bytes
            frame = payload
            if self.channel.corrupt_ack and len(frame) == parent_wire.FRAME.size:
                frame = frame[:-1] + bytes([frame[-1] ^ 1])
            self.channel.to_runner.put(frame)
            return len(payload)
        assert (handle, kind, payload) == (12, "read", parent_wire.FRAME.size)
        return self.channel.to_parent.get(timeout=2)

    def _hold(self):
        self.held = True

    def cleanup(self):
        return True


class RunnerEnd:
    def __init__(self, channel, exports):
        self.channel, self.exports, self.deadline = channel, exports, 500
        self.receipt = client.ClientEndpoints(13, 14)

    def transfer(self, handle, kind, payload):
        self.channel.events.append(("runner", handle, kind, payload))
        if kind == "read":
            assert handle == 13 and payload in (parent_wire.BOOT.size,
                                                 parent_wire.FRAME.size)
            return self.channel.to_runner.get(timeout=2)
        assert handle == 14 and type(payload) is bytes
        self.channel.to_parent.put(payload)
        return len(payload)


class CrossOwnerHandoffTests(unittest.TestCase):
    def drive(self, *, corrupt_ack=False):
        h = Harness()
        channel = SharedChannel(h.exports, corrupt_ack=corrupt_ack)
        original_dispatch = h.dispatch
        def dispatch(name, *args):
            if name == "DuplicateHandle":
                channel.events.append(("kernel", name, args[1]))
            return original_dispatch(name, *args)
        h.dispatch = dispatch
        h.pipe = channel.parent
        h.owner = parent_wire.ExactWorkerHandoff(h.exports, watchdog=h.watchdog,
                                                 pipe_owner=channel.parent,
                                                 nonce=h.offer.nonce)
        runner = runner_wire.RunnerHandoff(h.exports, pipe_owner=channel.runner,
                                           deadline=500)
        worker = SimpleNamespace(exports=h.exports, deadline=500,
            receipt=SimpleNamespace(phase="prepared-suspended", pid=73, tid=74,
                local={"worker_process": 71, "worker_job": 72,
                       "worker_thread": 75, "child_job": 77},
                remote_child_job=parent_wire.creator.RemoteHandle(76, 71)))
        outcomes = {}

        def parent_side():
            try:
                h.owner.bootstrap()
                outcomes["parent"] = h.owner.accept_offer_and_ack()
            except BaseException as error:
                outcomes["parent_error"] = error

        def runner_side():
            try:
                outcomes["bootstrap"] = runner.receive_bootstrap()
                outcomes["runner"] = runner.before_resume(worker, h.offer.nonce)
            except BaseException as error:
                outcomes["runner_error"] = error

        threads = [Thread(target=parent_side, daemon=True),
                   Thread(target=runner_side, daemon=True)]
        for thread in threads:
            thread.start()
        for thread in threads:
            thread.join(timeout=3)
        self.assertFalse(any(thread.is_alive() for thread in threads),
                         "injected channel exchange must finish within bounds")
        return h, channel, runner, outcomes

    def test_exact_shared_bootstrap_offer_ack_precedes_any_resume(self):
        h, channel, runner, outcomes = self.drive()
        self.assertEqual(outcomes.get("bootstrap"), (500, h.offer.nonce))
        self.assertEqual(outcomes.get("parent"), h.offer)
        self.assertEqual(outcomes.get("runner"), h.offer)
        self.assertNotIn("parent_error", outcomes)
        self.assertNotIn("runner_error", outcomes)
        self.assertEqual(h.owner.phase, "acknowledged")
        self.assertEqual(runner.phase, "authorized")
        self.assertEqual(h.owner.local, {"worker_process": 81, "worker_job": 82})
        self.assertEqual(h.ack, (h.offer.nonce,
                                 parent_wire.hashlib.sha256(
                                     parent_wire.encode_offer(h.offer)).digest()))
        parent_frames = [event for event in channel.events if event[0] == "parent"]
        runner_frames = [event for event in channel.events if event[0] == "runner"]
        self.assertEqual([(handle, kind) for _, handle, kind, _ in parent_frames],
                         [(11, "write"), (12, "read"), (11, "write")])
        self.assertEqual([(handle, kind) for _, handle, kind, _ in runner_frames],
                         [(13, "read"), (14, "write"), (13, "read")])
        self.assertEqual(parent_frames[0][3],
                         parent_wire.encode_bootstrap(500, h.offer.nonce))
        self.assertEqual(runner_frames[1][3], parent_wire.encode_offer(h.offer))
        self.assertEqual(parent_frames[2][3], parent_wire.encode_ack(h.offer))
        ack_index = channel.events.index(parent_frames[2])
        duplications = [index for index, event in enumerate(channel.events)
                        if event[0:2] == ("kernel", "DuplicateHandle")]
        self.assertEqual(len(duplications), 2)
        self.assertTrue(all(index < ack_index for index in duplications))
        names = [name for name, _ in h.calls]
        self.assertEqual(names.count("DuplicateHandle"), 2)
        self.assertNotIn("OpenProcess", names)
        self.assertNotIn("ResumeThread", names)

    def test_corrupted_ack_denies_runner_authorization(self):
        h, channel, runner, outcomes = self.drive(corrupt_ack=True)
        self.assertEqual(outcomes.get("parent"), h.offer)
        self.assertIsInstance(outcomes.get("runner_error"), runner_wire.Hold)
        self.assertEqual(runner.phase, "hold")
        self.assertNotIn("runner", outcomes)
        self.assertFalse(any(name == "ResumeThread" for name, _ in h.calls))
        self.assertEqual(channel.to_runner.qsize(), 0)

    def _full_path(self, *, corrupt_ack=False):
        """Compose real CP332/333/334/335/330 framing on one injected occurrence.

        The CP329 creator and WORKER pipe are fixed injected owners; no OS
        process, DLL or native pipe is touched. Parent CP331 remains a fake.
        """
        h = Harness()
        channel = SharedChannel(h.exports, corrupt_ack=corrupt_ack)
        original_dispatch = h.dispatch
        def dispatch(name, *args):
            if name == "GetHandleInformation" and args[0] in (13, 14):
                parent_wire.C.cast(args[1], parent_wire.C.POINTER(
                    parent_wire.abi.DWORD)).contents.value = 1
                return 1
            if name == "GetNamedPipeInfo" and args[0] in (13, 14):
                parent_wire.C.cast(args[1], parent_wire.C.POINTER(
                    parent_wire.abi.DWORD)).contents.value = (
                    client.PIPE_REJECT_REMOTE_CLIENTS)
                return 1
            if name in ("DuplicateHandle", "ResumeThread"):
                channel.events.append(("kernel", name, args[1] if name ==
                                       "DuplicateHandle" else args[0]))
            return original_dispatch(name, *args)
        h.dispatch = dispatch
        h.pipe = channel.parent
        h.watchdog.start_occurrence = lambda: 500
        h.watchdog._budget = lambda: 400
        h.watchdog.prepare_suspended = lambda **kwargs: None
        h.watchdog.resume_once = lambda: None
        def observe_runner_success():
            h.watchdog.receipt.phase = "runner-closed-success"
            h.watchdog.receipt.runner_waited = True
            h.watchdog.receipt.job_empty = True
            h.watchdog._runner_success_observed = True
            return h.watchdog.receipt
        h.watchdog.observe_runner_success = observe_runner_success
        channel.parent.prepare = lambda name: channel.parent.receipt

        class ParentSecurity:
            def __init__(self):
                self.exports = h.exports
                self.started = self.prepared = False
            def prepare(self):
                if self.started:
                    raise AssertionError("parent descriptor prepared twice")
                self.started = self.prepared = True
            def cleanup(self):
                return True
        parent_security = ParentSecurity()

        class WorkerSecurity:
            exports, started = h.exports, False
            def prepare(self):
                self.started = True
            def cleanup(self):
                return True

        security = WorkerSecurity()
        class WorkerPipe:
            def __init__(self):
                self.exports, self.deadline, self.security = h.exports, 500, security
                self.receipt = inert.pipes.ConnectedEndpoints(90, 91, 92, 93)
                self.handles = {90, 91, 92, 93}
                self.operations = []
                self.started = False
                self.frames = []
            def transfer(self, handle, kind, payload):
                if kind == "write":
                    assert handle == 90 and len(payload) == 48
                    self.frames.append(payload)
                    return len(payload)
                assert (handle, kind, payload) == (92, "read", 80)
                nonce = inert.wire.decode_init(self.frames[0]).nonce
                return inert.wire.encode_result(inert.wire.WorkerTelemetry(
                    0, 0, 0, 0, 0, 0, 0, 0, 0, nonce))
            def cleanup(self):
                self.handles.clear()
                return True

        worker_pipe = WorkerPipe()
        class WorkerPin:
            exports, reopened_handle = h.exports, None
            def __init__(self):
                self.handle = None
                self._used = False
                self.post_create_verified = True
                self.snapshot = inert.image.Snapshot("a" * 64,
                    inert.image.Identity(1, b"i" * 16, 104960, 2, 3,
                                         inert.image.WORKER_IMAGE))
            def prepare(self):
                self._used, self.handle = True, 50
            def close(self):
                self.handle = None

        pin = WorkerPin()
        class Worker:
            def __init__(self, exports, *, security, pipe_owner, image_pin, deadline):
                assert exports is h.exports and security is worker_security
                assert pipe_owner is worker_pipe and image_pin is pin
                self.exports, self.security = exports, security
                self.pipe_owner, self.image_pin = pipe_owner, image_pin
                self.deadline = deadline
                self.receipt = inert.creator.Receipt()
            def prepare(self, name):
                self.security.prepare()
                self.pipe_owner.started = True
                self.image_pin.prepare()
                self.receipt = inert.creator.Receipt(
                    phase="prepared-suspended",
                    local={"worker_process": 71, "worker_job": 72,
                           "worker_thread": 75, "child_job": 77},
                    remote_child_job=inert.creator.RemoteHandle(76, 71),
                    pid=73, tid=74, create_outcome="created-suspended",
                    duplicate_outcome="child-token-duplicated")
                return self.receipt
            def complete_success(self):
                self.pipe_owner.cleanup()
                self.image_pin.close()
                self.security.cleanup()
                self.receipt.worker_waited = self.receipt.jobs_empty = True
                self.receipt.phase = "closed"
                return self.receipt
            def close(self):
                return self.receipt

        worker_security = security
        outcomes = {}
        real_adopt = entry.client.from_bootstrap_argv
        def adopt(exports, argv):
            owner = real_adopt(exports, argv)
            owner.transfer = channel.runner.transfer
            original_cleanup = owner.cleanup
            eof_sent = [False]
            def cleanup():
                if corrupt_ack and not eof_sent[0]:
                    # Model terminal EOF after the failed runner closes its
                    # inherited channel; never synthesize a success frame.
                    channel.to_parent.put(b"")
                    eof_sent[0] = True
                return original_cleanup()
            owner.cleanup = cleanup
            return owner
        def parent_side():
            try:
                parent = runner_wire.WatchdogIntegration(h.exports)
                outcomes["parent_owner"] = parent
                outcomes["offer"] = parent.acknowledge()
                h.owner = parent.handoff
                h.success_wait_status, h.success_active = 0, 0
                outcomes["parent_receipt"] = parent.complete()
            except BaseException as error:
                outcomes["parent_error"] = error
        def child_side():
            try:
                outcomes["entry"] = entry.FixedRunnerEntry(
                    ["--cp334-client", "13", "14", "500"]).run()
            except BaseException as error:
                outcomes["child_error"] = error

        def security_factory(exports):
            assert exports is h.exports
            return parent_security if __import__("threading").current_thread().name == "cp348-parent" else security
        def pipe_factory(exports, descriptor, *, deadline):
            assert exports is h.exports and deadline == 500
            return channel.parent if descriptor is parent_security else worker_pipe

        with patch.object(entry.binding, "load_native_exports", return_value=h.exports), \
             patch.object(entry.client, "from_bootstrap_argv", side_effect=adopt), \
             patch.object(runner_wire.outer, "OuterWatchdog", return_value=h.watchdog), \
             patch.object(inert.security, "NativePrivatePipeSecurity", side_effect=security_factory), \
             patch.object(inert.pipes, "NativePipeOwner", side_effect=pipe_factory), \
             patch.object(inert.image, "NativeImagePin", return_value=pin), \
             patch.object(inert.creator, "NativeWorkerCreator", side_effect=Worker), \
             patch.object(runner_wire.secrets, "token_bytes", return_value=h.offer.nonce):
            threads = [Thread(target=parent_side, name="cp348-parent", daemon=True),
                       Thread(target=child_side, name="cp348-child", daemon=True)]
            for thread in threads:
                thread.start()
            for thread in threads:
                thread.join(timeout=3)
            self.assertFalse(any(thread.is_alive() for thread in threads))
        return h, channel, outcomes, worker_pipe

    def test_fixed_entry_runs_actual_inert_transcript_after_bound_ack(self):
        h, channel, outcomes, worker_pipe = self._full_path()
        self.assertNotIn("parent_error", outcomes, outcomes)
        self.assertNotIn("child_error", outcomes, outcomes)
        self.assertEqual(outcomes["offer"], h.offer)
        self.assertEqual(outcomes["parent_receipt"].phase, "closed-success")
        self.assertEqual(outcomes["parent_receipt"].terminal_outcome,
                         "80-byte-occurrence-validated-zero-queued-snapshot")
        self.assertEqual(outcomes["parent_receipt"].worker_outcome,
                         "exact-worker-observed-and-duplicates-released")
        self.assertEqual(outcomes["parent_receipt"].runner_outcome,
                         "exact-runner-exit-zero-job-empty-observed")
        self.assertEqual(outcomes["entry"].phase, "closed-success")
        self.assertEqual(h.owner.phase, "worker-success-released")
        self.assertEqual(len(worker_pipe.frames), 2)
        self.assertEqual(inert.wire.decode_init(worker_pipe.frames[0]).nonce,
                         h.offer.nonce)
        self.assertEqual(inert.wire.decode_ack(worker_pipe.frames[1]).nonce,
                         h.offer.nonce)
        ack_index = next(i for i, event in enumerate(channel.events)
                         if event[0:3] == ("parent", 11, "write") and
                         len(event[3]) == parent_wire.FRAME.size)
        resume_index = next(i for i, event in enumerate(channel.events)
                            if event[0:2] == ("kernel", "ResumeThread"))
        self.assertLess(ack_index, resume_index)
        self.assertEqual(sum(event[0:2] == ("kernel", "ResumeThread")
                             for event in channel.events), 1)

    def test_corrupted_full_path_ack_blocks_resume_and_terminal_acceptance(self):
        h, channel, outcomes, worker_pipe = self._full_path(corrupt_ack=True)
        self.assertEqual(outcomes.get("offer"), h.offer)
        self.assertIn("child_error", outcomes)
        self.assertIn("parent_error", outcomes)
        self.assertNotIn("entry", outcomes)
        self.assertNotIn("parent_receipt", outcomes)
        self.assertFalse(any(event[0:2] == ("kernel", "ResumeThread")
                             for event in channel.events))
        self.assertEqual(worker_pipe.frames, [])
        runner_writes = [event[3] for event in channel.events
                         if event[0] == "runner" and event[2] == "write"]
        self.assertEqual(runner_writes, [parent_wire.encode_offer(h.offer)])
        self.assertNotEqual(outcomes["parent_owner"].receipt.phase, "closed-success")


if __name__ == "__main__":
    unittest.main()
