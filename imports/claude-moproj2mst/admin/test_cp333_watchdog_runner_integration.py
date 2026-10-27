"""Injected CP333 contract tests; no DLLs, native processes or GUI calls."""
import ctypes as C
from dataclasses import replace
from pathlib import Path
from types import SimpleNamespace as NS
import sys
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp333_watchdog_runner_integration as m


class Harness:
    def __init__(self):
        self.events, self.now, self.on_tick = [], 100, None
        self.peek_available, self.peek_ok = 0, True
        self.exports = m.binding.BoundExports({name: lambda *a, n=name: self.call(n, *a)
            for name in m.abi.EXPORTS}, {}, lambda: 5)
        self.frames, self.partial, self.after = [], False, None
        self.pipe = NS(exports=self.exports, deadline=500,
            receipt=NS(parent_write=9, client_read=11, parent_read=10, client_write=12),
            transfer=self.transfer)
        self.owner = m.RunnerHandoff(self.exports, pipe_owner=self.pipe, deadline=500)
        self.worker = NS(exports=self.exports, deadline=500,
            receipt=NS(phase="prepared-suspended", pid=73, tid=74,
                remote_child_job=m.handoff.creator.RemoteHandle(76, 71),
                local={"worker_process": 71, "worker_job": 72, "worker_thread": 75, "child_job": 77}))
        self.nonce = b"x" * 32
        self.offer = m.handoff.Offer(71, 72, 73, 123456, self.nonce, 500)
        self.frames.append(m.handoff.encode_bootstrap(500, self.nonce))

    def call(self, name, *args):
        self.events.append(name)
        if name == "GetTickCount64":
            if self.on_tick:
                self.on_tick()
            return self.now
        if name == "GetProcessTimes":
            self.assert_process = args[0]
            C.cast(args[1], C.POINTER(m.abi.FILETIME)).contents.dwLowDateTime = 123456
            return 1
        if name == "PeekNamedPipe":
            C.cast(args[4], C.POINTER(m.abi.DWORD)).contents.value = self.peek_available
            return int(self.peek_ok)
        raise AssertionError("unexpected native API " + name)

    def transfer(self, handle, kind, payload):
        self.events.append((handle, kind, payload))
        if self.after:
            self.after(kind)
        if kind == "read":
            return self.frames.pop(0)
        return len(payload) - int(self.partial)


class RunnerTests(unittest.TestCase):
    def test_complete_binding_before_authorization(self):
        h = Harness()
        self.assertEqual(h.owner.receive_bootstrap(), (500, h.nonce))
        h.frames.append(m.handoff.encode_ack(h.offer))
        self.assertEqual(h.owner.before_resume(h.worker, h.nonce), h.offer)
        self.assertEqual(h.owner.phase, "authorized")
        transfers = [e for e in h.events if isinstance(e, tuple)]
        self.assertEqual(transfers, [(11, "read", 48),
            (12, "write", m.handoff.encode_offer(h.offer)), (11, "read", 80)])
        self.assertNotIn("ResumeThread", h.events)
        with self.assertRaises(m.Hold):
            h.owner.before_resume(h.worker, h.nonce)

    def test_cp334_two_client_endpoints_drive_exact_handoff(self):
        h = Harness()
        import cp334_runner_client_pipe as client
        h.pipe.receipt = client.ClientEndpoints(11, 12)
        h.owner = m.RunnerHandoff(h.exports, pipe_owner=h.pipe, deadline=500)
        self.assertEqual(h.owner.receive_bootstrap(), (500, h.nonce))
        h.frames.append(m.handoff.encode_ack(h.offer))
        self.assertEqual(h.owner.before_resume(h.worker, h.nonce), h.offer)
        transfers = [e for e in h.events if isinstance(e, tuple)]
        self.assertEqual(transfers, [(11, "read", 48),
            (12, "write", m.handoff.encode_offer(h.offer)), (11, "read", 80)])
        self.assertNotIn("ResumeThread", h.events)

    def test_cp334_endpoint_custody_checked_after_transfer_and_clock(self):
        h = Harness()
        import cp334_runner_client_pipe as client
        h.pipe.receipt = client.ClientEndpoints(11, 12)
        h.owner = m.RunnerHandoff(h.exports, pipe_owner=h.pipe, deadline=500)
        h.after = lambda kind: setattr(h.pipe, "receipt", client.ClientEndpoints(11, 13)) if kind == "read" else None
        with self.assertRaises(m.Hold):
            h.owner.receive_bootstrap()
        self.assertEqual(h.owner.phase, "hold")

    def test_bootstrap_wrong_deadline_and_replay(self):
        h = Harness()
        h.frames[0] = m.handoff.encode_bootstrap(501, h.nonce)
        with self.assertRaises(m.Hold):
            h.owner.receive_bootstrap()
        self.assertEqual(h.owner.phase, "hold")
        with self.assertRaises(m.Hold):
            h.owner.receive_bootstrap()

    def test_each_ack_field_is_bound(self):
        for changes in ({"process": 81}, {"job": 82}, {"pid": 74},
                        {"creation_time": 123457}, {"nonce": b"z" * 32}, {"deadline": 501}):
            with self.subTest(changes=changes):
                h = Harness()
                h.owner.receive_bootstrap()
                h.frames.append(m.handoff.encode_ack(replace(h.offer, **changes)))
                with self.assertRaises(m.Hold):
                    h.owner.before_resume(h.worker, h.nonce)
                self.assertEqual(h.owner.phase, "hold")

    def test_bad_occurrence_cannot_offer(self):
        for field, value in (("deadline", 501), ("exports", object()), ("phase", "running")):
            with self.subTest(field=field):
                h = Harness()
                h.owner.receive_bootstrap()
                setattr(h.worker.receipt if field == "phase" else h.worker, field, value)
                with self.assertRaises(m.Hold):
                    h.owner.before_resume(h.worker, h.nonce)
                self.assertFalse(any(isinstance(e, tuple) and e[1] == "write" for e in h.events))

    def test_expiry_after_ack_prevents_authorization(self):
        h = Harness()
        h.owner.receive_bootstrap()
        h.frames.append(m.handoff.encode_ack(h.offer))
        h.after = lambda kind: setattr(h, "now", 500) if kind == "read" else None
        with self.assertRaises(m.Hold):
            h.owner.before_resume(h.worker, h.nonce)
        self.assertEqual(h.owner.phase, "hold")

    def test_clock_regression_rejected(self):
        h = Harness()
        h.owner.receive_bootstrap()
        h.now = 99
        with self.assertRaises(m.Hold):
            h.owner.before_resume(h.worker, h.nonce)

    def test_short_offer_write_cannot_read_ack(self):
        h = Harness()
        h.owner.receive_bootstrap()
        h.partial = True
        with self.assertRaises(m.Hold):
            h.owner.before_resume(h.worker, h.nonce)
        self.assertEqual(sum(isinstance(e, tuple) and e[1] == "read" for e in h.events), 1)

    def test_changed_handle_custody_during_ack_rejected(self):
        h = Harness()
        h.owner.receive_bootstrap()
        h.frames.append(m.handoff.encode_ack(h.offer))
        h.after = lambda kind: h.worker.receipt.local.update(worker_process=99) if kind == "read" else None
        with self.assertRaises(m.Hold):
            h.owner.before_resume(h.worker, h.nonce)
        self.assertEqual(h.owner.phase, "hold")

    def test_channel_and_occurrence_mutations_rejected_at_each_boundary(self):
        def mutate(h, name):
            if name == "receipt":
                h.pipe.receipt = NS(**vars(h.pipe.receipt))
            elif name == "deadline":
                h.owner.deadline = h.pipe.deadline = h.worker.deadline = 501
                h.frames[:] = [m.handoff.encode_ack(replace(h.offer, deadline=501))]
            elif name == "nonce":
                h.owner.nonce = b"z" * 32
            elif name == "exports":
                h.owner.exports = h.pipe.exports = object()
            elif name == "owner":
                h.owner.pipe_owner = NS(**vars(h.pipe))
            else:
                setattr(h.pipe.receipt, name, 99)
        names = ("receipt", "deadline", "nonce", "exports", "owner", "parent_write",
                 "client_read", "parent_read", "client_write")
        for boundary in ("before-offer", "during-offer", "during-ack"):
            for name in names:
                with self.subTest(boundary=boundary, mutation=name):
                    h = Harness()
                    h.owner.receive_bootstrap()
                    h.frames.append(m.handoff.encode_ack(h.offer))
                    if boundary == "before-offer":
                        mutate(h, name)
                    else:
                        h.after = lambda kind, h=h, name=name: mutate(h, name) if kind == (
                            "write" if boundary == "during-offer" else "read") else None
                    with self.assertRaises(m.Hold):
                        h.owner.before_resume(h.worker, h.nonce)
                    self.assertEqual(h.owner.phase, "hold")
                    transfers = [event for event in h.events if isinstance(event, tuple)]
                    self.assertTrue(all(event[0] in (11, 12) for event in transfers))

    def test_bootstrap_cannot_change_frozen_channel(self):
        h = Harness()
        h.after = lambda kind: setattr(h.pipe.receipt, "client_read", 99)
        with self.assertRaises(m.Hold):
            h.owner.receive_bootstrap()
        self.assertEqual(h.owner.phase, "hold")

    def test_full_worker_custody_after_ack_and_final_clock(self):
        def mutate(h, field):
            r = h.worker.receipt
            if field == "receipt":
                h.worker.receipt = NS(**vars(r))
            elif field == "local":
                r.local = dict(r.local)
            elif field in ("pid", "tid"):
                setattr(r, field, 99)
            elif field == "remote":
                r.remote_child_job = m.handoff.creator.RemoteHandle(99, 71)
            elif field == "remote-owner":
                object.__setattr__(r.remote_child_job, "owner_process", 99)
            elif field == "offer":
                h.owner.offer = replace(h.owner.offer, pid=99)
            elif field == "offer-field":
                object.__setattr__(h.owner.offer, "pid", 99)
            else:
                r.local[field] = 99
        for boundary in ("ack", "final-clock"):
            for field in ("receipt", "local", "pid", "tid", "remote", "remote-owner",
                          "worker_process", "worker_thread", "worker_job", "child_job",
                          "offer", "offer-field"):
                with self.subTest(boundary=boundary, field=field):
                    h = Harness()
                    h.owner.receive_bootstrap()
                    h.frames.append(m.handoff.encode_ack(h.offer))
                    after_ack_ticks = []
                    def after(kind):
                        if kind == "read":
                            if boundary == "ack":
                                mutate(h, field)
                            else:
                                def tick():
                                    after_ack_ticks.append(1)
                                    if len(after_ack_ticks) == 2:
                                        mutate(h, field)
                                h.on_tick = tick
                    h.after = after
                    with self.assertRaises(m.Hold):
                        h.owner.before_resume(h.worker, h.nonce)
                    self.assertEqual(h.owner.phase, "hold")
                    if boundary == "final-clock":
                        self.assertEqual(len(after_ack_ticks), 2)

    def test_constructor_inert_and_shared_deadline(self):
        h = Harness()
        self.assertEqual(h.events, [])
        with self.assertRaises(m.Hold):
            m.RunnerHandoff(h.exports, pipe_owner=h.pipe, deadline=501)
        with self.assertRaises(TypeError):
            m.RunnerHandoff(object(), pipe_owner=h.pipe, deadline=500)


class OuterTests(unittest.TestCase):
    def setup_contract(self, fail=None):
        h = Harness()
        events = h.events
        def event(name, result=None):
            def called(*args, **kwargs):
                events.append(name)
                if name == fail:
                    raise m.Hold(name)
                return result
            return called
        watchdog = NS(start_occurrence=event("start", 500),
            _budget=event("budget", 400),
            prepare_suspended=event("prepare-runner"), resume_once=event("resume-runner"),
            close=event("close-watchdog", NS(phase="closed")))
        pipe = NS(prepare=event("prepare-pipes", NS(client_read=11, client_write=12)),
            cleanup=event("cleanup-pipes", True))
        control = NS(bootstrap=event("bootstrap"), accept_offer_and_ack=event("ack", h.offer),
            _channel_handles=(9, 11, 10, 12), close=event("close-handoff", True))
        def pipe_factory(exports, security, *, deadline):
            self.assertEqual(deadline, 500)
            events.append("construct-pipes")
            return pipe
        def control_factory(exports, **kwargs):
            self.assertIs(kwargs["watchdog"], watchdog)
            self.assertIs(kwargs["pipe_owner"], pipe)
            self.assertEqual(kwargs["nonce"], h.nonce)
            events.append("construct-handoff")
            return control
        return h, watchdog, pipe_factory, control_factory

    def test_start_precedes_all_ipc_and_exact_ack(self):
        h, watchdog, pf, cf = self.setup_contract()
        descriptor = NS(prepare=lambda: h.events.append("prepare-security"),
            cleanup=lambda: True)
        with patch.object(m.outer, "OuterWatchdog", return_value=watchdog), \
             patch.object(m.security, "NativePrivatePipeSecurity",
                 side_effect=lambda e: h.events.append("security") or descriptor), \
             patch.object(m.pipes, "NativePipeOwner", side_effect=pf), \
             patch.object(m.handoff, "ExactWorkerHandoff", side_effect=cf), \
             patch.object(m.secrets, "token_bytes", return_value=h.nonce):
            owner = m.WatchdogIntegration(h.exports)
            self.assertEqual(h.events, [])
            self.assertEqual(owner.acknowledge(), h.offer)
            self.assertEqual(h.events, ["start", "security", "budget", "prepare-security",
                "budget", "construct-pipes", "prepare-pipes",
                "prepare-runner", "construct-handoff", "resume-runner", "bootstrap", "ack"])
            self.assertEqual(owner.receipt.phase, "worker-authorized-trial-unaccepted")
            with self.assertRaises(m.Hold):
                owner.acknowledge()

    def test_acknowledge_emits_ordered_diagnostic_boundaries(self):
        h, watchdog, pipe_factory, control_factory = self.setup_contract()
        descriptor = NS(prepare=lambda: None, cleanup=lambda: True)
        stages = []
        with patch.object(m.outer, "OuterWatchdog", return_value=watchdog), \
             patch.object(m.security, "NativePrivatePipeSecurity",
                          return_value=descriptor), \
             patch.object(m.pipes, "NativePipeOwner", side_effect=pipe_factory), \
             patch.object(m.handoff, "ExactWorkerHandoff",
                          side_effect=control_factory), \
             patch.object(m.secrets, "token_bytes", return_value=h.nonce):
            owner = m.WatchdogIntegration(h.exports, stage=stages.append)
            self.assertEqual(owner.acknowledge(), h.offer)
        self.assertEqual(stages, [
            "cp333_start", "cp333_occurrence_started",
            "cp333_before_security", "cp333_after_security",
            "cp333_before_pipes", "cp333_after_pipes",
            "cp333_before_worker_prepare", "cp333_after_worker_prepare",
            "cp333_before_resume", "cp333_after_resume",
            "cp333_before_bootstrap", "cp333_after_bootstrap",
            "cp333_before_offer_ack", "cp333_after_offer_ack",
        ])

    def test_ack_failure_marker_precedes_cleanup_and_preserves_primary_error(self):
        h, watchdog, pipe_factory, control_factory = self.setup_contract()
        descriptor = NS(prepare=lambda: None, cleanup=lambda: True)
        stages = []
        def failing_control_factory(*args, **kwargs):
            control = control_factory(*args, **kwargs)
            def fail_offer():
                h.events.append("offer-failure")
                raise m.Hold("OFFER rejected (109)")
            control.accept_offer_and_ack = fail_offer
            return control
        with patch.object(m.outer, "OuterWatchdog", return_value=watchdog), \
             patch.object(m.security, "NativePrivatePipeSecurity",
                          return_value=descriptor), \
             patch.object(m.pipes, "NativePipeOwner", side_effect=pipe_factory), \
             patch.object(m.handoff, "ExactWorkerHandoff",
                          side_effect=failing_control_factory), \
             patch.object(m.secrets, "token_bytes", return_value=h.nonce):
            owner = m.WatchdogIntegration(h.exports, stage=stages.append)
            with self.assertRaises(m.IntegrationFailure) as caught:
                owner.acknowledge()
        self.assertIsInstance(caught.exception.__cause__, m.Hold)
        self.assertEqual(str(caught.exception.__cause__), "OFFER rejected (109)")
        self.assertEqual(stages[-1], "cp333_error:Hold:OFFER rejected (109)")
        self.assertLess(h.events.index("offer-failure"),
                        h.events.index("close-handoff"))

    def test_cp326_prepared_descriptor_is_required_by_parent_pipe_path(self):
        h, watchdog, pipe_factory, control_factory = self.setup_contract()
        real_security = m.security.NativePrivatePipeSecurity
        unprepared = real_security(h.exports)
        with self.assertRaisesRegex(m.Hold, "successful prepare required"):
            unprepared.security_attributes(inheritable=False)

        native_check = h.call
        descriptor_calls = {"InitializeAcl", "AddAccessAllowedAceEx",
            "InitializeSecurityDescriptor", "SetSecurityDescriptorDacl",
            "IsValidSecurityDescriptor"}
        def check_call(name, *args):
            if name in descriptor_calls:
                h.events.append(name)
                return 1
            return native_check(name, *args)
        h.call = check_call

        def checking_pipe_factory(exports, descriptor, *, deadline):
            pipe = pipe_factory(exports, descriptor, deadline=deadline)
            original_prepare = pipe.prepare
            def prepare(name):
                # Use the actual CP326 precondition at CP328's call boundary.
                attrs = descriptor.security_attributes(inheritable=False)
                self.assertIs(attrs._descriptor_owner, descriptor)
                return original_prepare(name)
            pipe.prepare = prepare
            return pipe

        with patch.object(m.outer, "OuterWatchdog", return_value=watchdog), \
             patch.object(real_security, "_copy_user",
                 lambda descriptor: setattr(descriptor, "_sid", C.create_string_buffer(8))), \
             patch.object(m.pipes, "NativePipeOwner", side_effect=checking_pipe_factory), \
             patch.object(m.handoff, "ExactWorkerHandoff", side_effect=control_factory), \
             patch.object(m.secrets, "token_bytes", return_value=h.nonce):
            owner = m.WatchdogIntegration(h.exports)
            self.assertEqual(owner.acknowledge(), h.offer)
            self.assertIsInstance(owner.security, real_security)
            self.assertTrue(owner.security.started)
            self.assertTrue(owner.security.prepared)
            self.assertEqual(h.events.count("InitializeAcl"), 1)
            self.assertEqual(h.events.count("budget"), 2)

    def test_security_prepare_failure_retains_owner_and_prevents_pipes(self):
        h, watchdog, pipe_factory, control_factory = self.setup_contract()
        def prepare():
            h.events.append("prepare-security")
            raise m.Hold("descriptor preparation failed")
        descriptor = NS(prepare=prepare,
            cleanup=lambda: h.events.append("cleanup-security") or True)
        with patch.object(m.outer, "OuterWatchdog", return_value=watchdog), \
             patch.object(m.security, "NativePrivatePipeSecurity", return_value=descriptor), \
             patch.object(m.pipes, "NativePipeOwner", side_effect=pipe_factory), \
             patch.object(m.handoff, "ExactWorkerHandoff", side_effect=control_factory), \
             patch.object(m.secrets, "token_bytes", return_value=h.nonce):
            owner = m.WatchdogIntegration(h.exports)
            with self.assertRaisesRegex(m.IntegrationFailure, "descriptor preparation failed"):
                owner.acknowledge()
            self.assertIs(owner.security, descriptor)
            self.assertIsNone(owner.pipe_owner)
            self.assertEqual(owner.receipt.phase, "closed-aborted")
            self.assertEqual(h.events, ["start", "budget", "prepare-security",
                "close-watchdog", "cleanup-security"])

    def test_security_prepare_uses_original_deadline_before_pipes(self):
        h, watchdog, pipe_factory, control_factory = self.setup_contract()
        def budget():
            h.events.append("budget")
            if h.now >= 500:
                raise m.Hold("original deadline expired")
            return 500 - h.now
        watchdog._budget = budget
        def prepare():
            h.events.append("prepare-security")
            h.now = 500
        descriptor = NS(prepare=prepare,
            cleanup=lambda: h.events.append("cleanup-security") or True)
        with patch.object(m.outer, "OuterWatchdog", return_value=watchdog), \
             patch.object(m.security, "NativePrivatePipeSecurity", return_value=descriptor), \
             patch.object(m.pipes, "NativePipeOwner", side_effect=pipe_factory), \
             patch.object(m.handoff, "ExactWorkerHandoff", side_effect=control_factory), \
             patch.object(m.secrets, "token_bytes", return_value=h.nonce):
            owner = m.WatchdogIntegration(h.exports)
            with self.assertRaisesRegex(m.IntegrationFailure, "original deadline expired"):
                owner.acknowledge()
            self.assertIs(owner.security, descriptor)
            self.assertIsNone(owner.pipe_owner)
            self.assertEqual(h.events, ["start", "budget", "prepare-security",
                "budget", "close-watchdog", "cleanup-security"])

    def test_failure_closes_retained_owner(self):
        for fail in ("prepare-pipes", "prepare-runner", "resume-runner", "bootstrap", "ack"):
            with self.subTest(fail=fail):
                h, watchdog, pf, cf = self.setup_contract(fail)
                with patch.object(m.outer, "OuterWatchdog", return_value=watchdog), \
                     patch.object(m.security, "NativePrivatePipeSecurity"), \
                     patch.object(m.pipes, "NativePipeOwner", side_effect=pf), \
                     patch.object(m.handoff, "ExactWorkerHandoff", side_effect=cf), \
                     patch.object(m.secrets, "token_bytes", return_value=h.nonce):
                    owner = m.WatchdogIntegration(h.exports)
                    with self.assertRaises(m.Hold):
                        owner.acknowledge()
                    self.assertEqual(owner.receipt.phase, "closed-aborted")
                    self.assertIn("close-handoff" if owner.handoff else "close-watchdog", h.events)
                    self.assertIsNotNone(owner.pipe_owner)

    def test_pending_cleanup_retains_integration(self):
        h = Harness()
        owner = m.WatchdogIntegration(h.exports)
        owner.handoff = NS(close=lambda: False)
        self.assertFalse(owner.close())
        self.assertIsNotNone(owner.handoff)

    def test_security_token_cleanup_is_required(self):
        h = Harness()
        owner = m.WatchdogIntegration(h.exports)
        owner.handoff = NS(close=lambda: True)
        owner.security = NS(cleanup=lambda: False)
        self.assertFalse(owner.close())
        self.assertNotEqual(owner.receipt.phase, "closed-aborted")
        owner.security.cleanup = lambda: True
        self.assertTrue(owner.close())
        self.assertEqual(owner.receipt.phase, "closed-aborted")

    def test_public_launch_unconditionally_held(self):
        with self.assertRaises(m.Hold):
            m.launch_trial()
        self.assertEqual(m.NATIVE_ENTRY, "HARD HOLD")


class CompletionTests(unittest.TestCase):
    def make_owner(self, *, raw=None, fail=None):
        import cp339_terminal_protocol as terminal
        h = Harness()
        events = h.events
        if raw is None:
            raw = terminal.encode_terminal(terminal.TerminalSuccess(500, h.nonce))

        owner = m.WatchdogIntegration(h.exports)
        owner.receipt.phase, owner.receipt.deadline, owner.nonce = (
            "worker-authorized-trial-unaccepted", 500, h.nonce)

        def step(name, result):
            events.append(name)
            if name == fail:
                raise m.Hold(name)
            return result

        runner_receipt = NS(phase="running-unacknowledged", runner_waited=False,
            job_empty=False)
        def runner_observe():
            step("observe-runner", None)
            runner_receipt.phase = "runner-closed-success"
            runner_receipt.runner_waited = runner_receipt.job_empty = True
            watchdog._runner_success_observed = watchdog._runner_success_released = True
            return runner_receipt

        def watchdog_budget():
            if not 0 < 500 - h.now <= 500:
                raise m.Hold("original deadline expired during completion")
            return 500 - h.now

        watchdog = NS(exports=h.exports, deadline=500, receipt=runner_receipt,
            _runner_success_observed=False, _runner_success_released=False,
            _budget=watchdog_budget,
            observe_runner_success=runner_observe,
            close=lambda: step("abort-watchdog", NS(phase="closed")))
        pipe = NS(exports=h.exports, deadline=500, operations=[],
            cleanup=lambda: step("cleanup-pipes", fail != "cleanup-pipes"))
        control = NS(exports=h.exports, deadline=500, phase="acknowledged",
            _channel_handles=(9, 11, 10, 12), _parent_read=10, _parent_write=9,
            _worker_success_released=False, _budget=lambda: None,
            _transfer=lambda kind, size: step("read-terminal", raw),
            observe_worker_success=lambda: step("observe-worker", True),
            release_worker_success=lambda: release_worker(),
            close=lambda: step("abort-handoff", True))
        def release_worker():
            step("release-worker", True)
            control._worker_success_released = True
            return True
        descriptor = NS(cleanup=lambda: step("cleanup-security", fail != "cleanup-security"))
        owner.watchdog, owner.pipe_owner, owner.handoff, owner.security = (
            watchdog, pipe, control, descriptor)
        owner._completion_owners = (h.exports, watchdog, pipe, control,
            descriptor, owner.receipt, h.nonce, 500, control._channel_handles)
        return owner, h, terminal

    def test_terminal_then_exact_worker_and_runner_then_cleanup(self):
        owner, h, _ = self.make_owner()
        self.assertIs(owner.complete(), owner.receipt)
        self.assertEqual(owner.receipt.phase, "closed-success")
        self.assertEqual(h.events, ["read-terminal", "PeekNamedPipe",
            "observe-worker", "release-worker",
            "observe-runner", "cleanup-pipes", "cleanup-security"])
        self.assertEqual(owner.receipt.terminal_outcome,
            "80-byte-occurrence-validated-zero-queued-snapshot")
        self.assertEqual(owner.receipt.worker_outcome,
            "exact-worker-observed-and-duplicates-released")
        self.assertEqual(owner.receipt.runner_outcome,
            "exact-runner-exit-zero-job-empty-observed")
        self.assertTrue(owner.close())
        self.assertEqual(h.events[-1], "cleanup-security")
        with self.assertRaises(m.Hold):
            owner.complete()

    def test_ack_or_forged_public_phase_never_accepts(self):
        h = Harness()
        owner = m.WatchdogIntegration(h.exports)
        with self.assertRaises(m.Hold):
            owner.complete()
        owner.receipt.phase = "worker-authorized-trial-unaccepted"
        with self.assertRaises(m.IntegrationFailure):
            owner.complete()
        self.assertNotEqual(owner.receipt.phase, "closed-success")

        forged, events, _ = self.make_owner()
        forged.receipt.phase = "closed-success"
        self.assertTrue(forged.close())
        self.assertIn("abort-handoff", events.events)
        self.assertEqual(forged.receipt.phase, "closed-aborted")

    def test_bad_terminal_claim_blocks_every_success_observation(self):
        import cp339_terminal_protocol as terminal
        for raw in (b"", terminal.encode_terminal(
                terminal.TerminalSuccess(500, b"z" * 32)),
                terminal.encode_terminal(terminal.TerminalSuccess(501, b"x" * 32))):
            with self.subTest(raw=raw[:4]):
                owner, h, _ = self.make_owner(raw=raw)
                with self.assertRaises(m.IntegrationFailure):
                    owner.complete()
                self.assertEqual(h.events, ["read-terminal", "abort-handoff",
                    "cleanup-security"])
                self.assertNotEqual(owner.receipt.phase, "closed-success")

    def test_each_independent_failure_aborts_without_acceptance(self):
        for fail in ("read-terminal", "observe-worker", "release-worker",
                     "observe-runner"):
            with self.subTest(fail=fail):
                owner, h, _ = self.make_owner(fail=fail)
                with self.assertRaises(m.IntegrationFailure):
                    owner.complete()
                self.assertIn("abort-handoff", h.events)
                self.assertNotIn("cleanup-pipes", h.events)
                self.assertNotEqual(owner.receipt.phase, "closed-success")
                with self.assertRaises(m.Hold):
                    owner.complete()  # no terminal-read replay

    def test_terminal_queue_snapshot_rejects_trailing_bytes_and_failure(self):
        for available, ok in ((1, True), (0, False)):
            with self.subTest(available=available, ok=ok):
                owner, h, _ = self.make_owner()
                h.peek_available, h.peek_ok = available, ok
                with self.assertRaises(m.IntegrationFailure):
                    owner.complete()
                self.assertEqual(h.events[:2], ["read-terminal", "PeekNamedPipe"])
                self.assertNotIn("observe-worker", h.events)
                self.assertIn("abort-handoff", h.events)

    def test_cleanup_failure_after_exact_observation_remains_unaccepted(self):
        for fail in ("cleanup-pipes", "cleanup-security"):
            with self.subTest(fail=fail):
                owner, h, _ = self.make_owner(fail=fail)
                with self.assertRaises(m.IntegrationFailure):
                    owner.complete()
                self.assertNotIn("abort-handoff", h.events)
                self.assertEqual(owner.receipt.phase, "hold")
                self.assertNotEqual(owner.receipt.phase, "closed-success")

    def test_mutated_public_channel_retains_original_abort_owner(self):
        owner, h, _ = self.make_owner()
        original = owner.handoff
        def mutate(kind, size):
            h.events.append("read-terminal")
            owner.handoff = NS(close=lambda: h.events.append("wrong-abort"))
            return m.terminal.encode_terminal(m.terminal.TerminalSuccess(500, h.nonce))
        original._transfer = mutate
        with self.assertRaises(m.IntegrationFailure):
            owner.complete()
        self.assertIn("abort-handoff", h.events)
        self.assertNotIn("wrong-abort", h.events)
        self.assertNotIn("observe-worker", h.events)

    def test_mutated_public_receipt_cannot_replace_success_owner(self):
        owner, h, _ = self.make_owner()
        original = owner.receipt
        control = owner.handoff
        raw = m.terminal.encode_terminal(m.terminal.TerminalSuccess(500, h.nonce))
        def mutate(kind, size):
            h.events.append("read-terminal")
            owner.receipt = m.Receipt(phase="worker-authorized-trial-unaccepted",
                deadline=500)
            return raw
        control._transfer = mutate
        with self.assertRaises(m.IntegrationFailure):
            owner.complete()
        self.assertIs(owner.receipt, original)
        self.assertIn("abort-handoff", h.events)
        self.assertNotIn("observe-worker", h.events)

    def test_post_runner_release_custody_failure_uses_cleanup_only(self):
        owner, h, _ = self.make_owner()
        original_observe = owner.watchdog.observe_runner_success
        def mutate_after_release():
            result = original_observe()
            owner.pipe_owner = object()
            return result
        owner.watchdog.observe_runner_success = mutate_after_release
        with self.assertRaises(m.IntegrationFailure):
            owner.complete()
        self.assertNotIn("abort-handoff", h.events)
        self.assertNotIn("abort-watchdog", h.events)
        self.assertEqual(owner.receipt.phase, "closed-unaccepted-after-cleanup-error")
        self.assertEqual(h.events.count("observe-runner"), 2)
        self.assertEqual(h.events[-2:], ["cleanup-pipes", "cleanup-security"])

    def test_partial_runner_handle_release_is_retry_only(self):
        owner, h, _ = self.make_owner()
        attempts = []
        def partial_then_retry():
            attempts.append(1)
            h.events.append("observe-runner")
            owner.watchdog._runner_success_observed = True
            if len(attempts) == 1:
                raise m.Hold("post-proof close failed")
            owner.watchdog._runner_success_released = True
            receipt = owner.watchdog.receipt
            receipt.phase = "runner-closed-success"
            receipt.runner_waited = receipt.job_empty = True
            return receipt
        owner.watchdog.observe_runner_success = partial_then_retry
        with self.assertRaises(m.IntegrationFailure):
            owner.complete()
        self.assertEqual(len(attempts), 2)
        self.assertNotIn("abort-handoff", h.events)
        self.assertEqual(owner.receipt.phase, "closed-unaccepted-after-cleanup-error")

    def test_original_deadline_expiry_after_proofs_or_cleanup_rejects(self):
        for boundary in ("runner", "pipe", "security"):
            with self.subTest(boundary=boundary):
                owner, h, _ = self.make_owner()
                if boundary == "runner":
                    original = owner.watchdog.observe_runner_success
                    def expire_runner():
                        receipt = original()
                        h.now = 500
                        return receipt
                    owner.watchdog.observe_runner_success = expire_runner
                elif boundary == "pipe":
                    original = owner.pipe_owner.cleanup
                    def expire_pipe():
                        result = original()
                        h.now = 500
                        return result
                    owner.pipe_owner.cleanup = expire_pipe
                else:
                    original = owner.security.cleanup
                    def expire_security():
                        result = original()
                        h.now = 500
                        return result
                    owner.security.cleanup = expire_security
                with self.assertRaises(m.IntegrationFailure):
                    owner.complete()
                self.assertNotEqual(owner.receipt.phase, "closed-success")
                self.assertNotIn("abort-handoff", h.events)
                self.assertNotIn("abort-watchdog", h.events)


if __name__ == "__main__":
    unittest.main()
