"""Injected CP332 tests only; no DLL loading, process creation or GUI calls."""
import ctypes as C
from dataclasses import replace
from pathlib import Path
from types import SimpleNamespace
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp332_exact_worker_handoff as m
import cp331_outer_watchdog as outer


def out(pointer, kind, value):
    C.cast(pointer, C.POINTER(kind)).contents.value = value


class Pipe:
    def __init__(self, harness):
        self.h = harness
        self.exports, self.deadline = harness.exports, 500
        self.receipt = SimpleNamespace(parent_read=12, parent_write=11,
                                       client_read=13, client_write=14)
        self.frames, self.held, self.cleanup_ready = [], False, True
        self.operations = []

    def transfer(self, handle, kind, payload):
        self.frames.append((handle, kind, payload))
        if kind == "read":
            return self.h.raw
        if self.h.partial_write:
            return len(payload) - 1
        if self.h.raise_ack and len(payload) == 80:
            self.operations.append("uncertain-overlapped-buffer")
            raise RuntimeError("ACK outcome unknown")
        return len(payload)

    def _hold(self):
        self.held = True

    def cleanup(self):
        if self.cleanup_ready:
            self.operations.clear()
        return self.cleanup_ready


class Harness:
    def __init__(self):
        self.calls, self.closed = [], []
        self.now, self.error, self.dead = 100, 5, False
        self.bad = self.fail = self.raise_at = None
        self.partial_write = self.raise_ack = False
        self.duplicate_count = 0
        self.watchdog_last_tick, self.claim, self.ack = None, None, None
        self.output_override = None
        self.outer_flags = m.creator.KILL_ON_CLOSE
        self.success_exit_code = 0
        self.success_wait_status = None
        self.success_active = None
        self.exports = m.binding.BoundExports({name: lambda *a, n=name: self.dispatch(n, *a)
            for name in m.abi.EXPORTS}, {}, lambda: self.error)
        self.watchdog = SimpleNamespace(exports=self.exports, deadline=500,
            receipt=SimpleNamespace(local={"runner_process": 31, "outer_job": 21},
                deadline=500, phase="running-unacknowledged"), close=self.close_watchdog,
            _tick=self.watchdog_tick, claim_handoff=self.claim_handoff,
            record_handoff_ack=self.record_handoff_ack)
        self.pipe = Pipe(self)
        self.offer = m.Offer(71, 72, 73, 123456, b"x" * 32, 500)
        self.raw = m.encode_offer(self.offer)
        self.owner = m.ExactWorkerHandoff(self.exports, watchdog=self.watchdog,
            pipe_owner=self.pipe, nonce=self.offer.nonce)

    def close_watchdog(self):
        self.watchdog.receipt.phase = "closed"
        self.watchdog.receipt.local.clear()
        return self.watchdog.receipt

    def watchdog_tick(self):
        now = self.dispatch("GetTickCount64")
        if self.watchdog_last_tick is not None and now < self.watchdog_last_tick:
            raise m.Hold("shared watchdog clock regressed")
        self.watchdog_last_tick = now
        return now

    def claim_handoff(self, nonce):
        if self.claim is not None:
            raise m.Hold("watchdog occurrence already claimed")
        self.watchdog_tick()
        self.claim = nonce

    def record_handoff_ack(self, nonce, offer_digest):
        if self.claim != nonce or self.ack is not None:
            raise m.Hold("watchdog ACK claim mismatch or replay")
        self.watchdog_tick()
        self.ack = (nonce, offer_digest)

    def dispatch(self, name, *args):
        self.calls.append((name, args))
        if name == self.fail:
            return 0
        if name == "GetTickCount64":
            return self.now
        if name == "GetCurrentProcess":
            return m.INVALID
        if name == "DuplicateHandle":
            self.duplicate_count += 1
            assert args[0] == 31 and args[1] in (71, 72)
            assert args[2] == m.INVALID and args[4:] == (0, False, 2)
            value = self.output_override if self.output_override is not None else (81 if args[1] == 71 else 82)
            out(args[3], m.abi.HANDLE, value)
            if self.raise_at == "DuplicateHandle":
                raise RuntimeError("duplicate raised after output")
            if self.bad == "duplicate-false":
                return 0
        elif name == "GetHandleInformation":
            out(args[1], m.abi.DWORD, int(self.bad == "inherit"))
        elif name == "GetProcessId":
            assert args == (81,)
            return 99 if self.bad == "pid" else 73
        elif name == "GetProcessTimes":
            stamp = C.cast(args[1], C.POINTER(m.abi.FILETIME)).contents
            stamp.dwLowDateTime = 123457 if self.bad == "stamp" else 123456
        elif name == "QueryFullProcessImageNameW":
            args[2].value = r"C:\wrong.exe" if self.bad == "image" else m.image.WORKER_IMAGE
            out(args[3], m.abi.DWORD, len(args[2].value) - int(self.bad == "image-size"))
        elif name == "IsProcessInJob":
            assert args[0] == 81 and args[1] in (21, 82)
            out(args[2], m.abi.BOOL, int(self.bad != "member"))
        elif name == "QueryInformationJobObject":
            job, kind, pointer, size, returned = args
            out(returned, m.abi.DWORD, size - int(self.bad == "query-size"))
            if kind == m.creator.JOB_LIMITS:
                obj = C.cast(pointer, C.POINTER(m.abi.EXTENDED_LIMIT)).contents
                obj.BasicLimitInformation.LimitFlags = 0 if self.bad == "limits" else m.creator.KILL_ON_CLOSE
                if job == 21:
                    obj.BasicLimitInformation.LimitFlags = self.outer_flags
            else:
                obj = C.cast(pointer, C.POINTER(m.abi.BASIC_ACCOUNTING)).contents
                obj.ActiveProcesses = 0 if self.dead else (2 if job == 21 else 1)
                if job == 82 and self.success_active is not None:
                    obj.ActiveProcesses = self.success_active
                if self.bad == "accounting":
                    obj.ActiveProcesses += 1
        elif name == "WaitForSingleObject":
            if args[0] == 81 and self.success_wait_status is not None:
                return self.success_wait_status
            return 0 if self.dead or self.bad == "wait" else 258
        elif name == "GetExitCodeProcess":
            assert args[0] == 81
            out(args[1], m.abi.DWORD, self.success_exit_code)
        elif name == "PeekNamedPipe":
            out(args[4], m.abi.DWORD, int(self.bad == "trailing"))
        elif name == "TerminateJobObject":
            assert args[0] == 21
            self.dead = True
        elif name == "CloseHandle":
            self.closed.append(args[0])
        if name == self.raise_at:
            raise RuntimeError("injected exception")
        return 1

    def run(self):
        self.owner.bootstrap()
        return self.owner.accept_offer_and_ack()


class WireTests(unittest.TestCase):
    def test_sizes_roundtrip_and_full_ack_binding(self):
        h = Harness()
        self.assertEqual(len(m.encode_bootstrap(500, h.offer.nonce)), 48)
        self.assertEqual(m.decode_bootstrap(m.encode_bootstrap(500, h.offer.nonce)), (500, h.offer.nonce))
        self.assertEqual(len(h.raw), 80)
        self.assertEqual(m.decode_offer(h.raw), h.offer)
        self.assertTrue(m.validate_ack(m.encode_ack(h.offer), h.offer))
        for field, value in (("process", 91), ("job", 92), ("pid", 93),
                ("creation_time", 1), ("nonce", b"y" * 32), ("deadline", 499)):
            with self.subTest(field=field), self.assertRaises(m.Hold):
                m.validate_ack(m.encode_ack(replace(h.offer, **{field: value})), h.offer)

    def test_bad_frames(self):
        h = Harness()
        for raw in (b"", h.raw[:-1], h.raw + b"x", m.encode_ack(h.offer),
                    b"xxxx" + h.raw[4:]):
            with self.subTest(raw=raw[:8]), self.assertRaises(m.Hold):
                m.decode_offer(raw)
        with self.assertRaises(m.Hold):
            m.decode_bootstrap(h.raw[:48])
        for offer in (replace(h.offer, process=m.INVALID), replace(h.offer, job=71),
                      replace(h.offer, pid=True), replace(h.offer, nonce=b"\0" * 32)):
            with self.assertRaises(m.Hold):
                m.encode_offer(offer)


class HandoffTests(unittest.TestCase):
    def test_success_pull_handles_verify_then_full_ack(self):
        h = Harness()
        self.assertEqual(h.run(), h.offer)
        self.assertEqual(h.owner.local, {"worker_process": 81, "worker_job": 82})
        self.assertEqual(h.pipe.frames[-1][2], m.encode_ack(h.offer))
        self.assertEqual(h.owner.phase, "acknowledged")
        self.assertEqual(h.claim, h.offer.nonce)
        self.assertEqual(h.ack, (h.offer.nonce, m.hashlib.sha256(h.raw).digest()))
        self.assertFalse(any(name in ("OpenProcess", "ResumeThread") for name, _ in h.calls))
        self.assertEqual([name for name, _ in h.calls].count("DuplicateHandle"), 2)

    def test_all_independent_rejections_terminate_without_ack(self):
        for bad in ("pid", "stamp", "inherit", "image", "image-size", "member",
                    "limits", "accounting", "query-size", "wait", "trailing"):
            with self.subTest(bad=bad):
                h = Harness()
                h.bad = bad
                with self.assertRaises(m.Hold):
                    h.run()
                self.assertEqual(h.owner.phase, "hold")
                self.assertTrue(h.dead)
                self.assertEqual(len([f for f in h.pipe.frames if f[1] == "write"]), 1)

    def test_nonce_deadline_order_replay(self):
        for offer in (replace(Harness().offer, nonce=b"y" * 32),
                      replace(Harness().offer, deadline=499)):
            h = Harness()
            h.raw = m.encode_offer(offer)
            with self.assertRaises(m.Hold):
                h.run()
            self.assertEqual(h.duplicate_count, 0)
        h = Harness()
        with self.assertRaises(m.Hold):
            h.owner.accept_offer_and_ack()
        self.assertTrue(h.dead)
        h = Harness()
        h.run()
        with self.assertRaises(m.Hold):
            h.owner.accept_offer_and_ack()
        self.assertTrue(h.dead)

    def test_cross_owner_replay_consumes_single_watchdog_occurrence(self):
        for first_succeeds in (False, True):
            with self.subTest(first_succeeds=first_succeeds):
                h = Harness()
                other = m.ExactWorkerHandoff(h.exports, watchdog=h.watchdog,
                    pipe_owner=h.pipe, nonce=h.offer.nonce)
                if first_succeeds:
                    h.run()
                else:
                    h.partial_write = True
                    with self.assertRaises(m.Hold):
                        h.owner.bootstrap()
                    self.assertIsNone(h.ack)
                frames = len(h.pipe.frames)
                with self.assertRaisesRegex(m.Hold, "already claimed"):
                    other.bootstrap()
                self.assertEqual(len(h.pipe.frames), frames)
                self.assertTrue(h.dead)

    def test_shared_clock_regression_from_watchdog_rejected_before_bootstrap(self):
        h = Harness()
        h.watchdog_last_tick = 400  # runner creation/resume already observed 400
        h.now = 100
        with self.assertRaisesRegex(m.Hold, "shared watchdog clock regressed"):
            h.owner.bootstrap()
        self.assertEqual(h.pipe.frames, [])
        self.assertTrue(h.dead)
        self.assertIsNone(h.claim)

    def test_channel_mutation_after_bootstrap_blocks_offer_and_ack(self):
        for mutation in ("receipt", "parent_read", "parent_write", "client_read", "client_write", "owner"):
            with self.subTest(mutation=mutation):
                h = Harness()
                h.owner.bootstrap()
                original = h.pipe
                if mutation == "receipt":
                    h.pipe.receipt = SimpleNamespace(**vars(h.pipe.receipt))
                elif mutation == "owner":
                    h.owner.pipe_owner = Pipe(h)
                else:
                    setattr(h.pipe.receipt, mutation, 99)
                with self.assertRaises(m.Hold):
                    h.owner.accept_offer_and_ack()
                self.assertEqual(len(original.frames), 1)
                self.assertEqual(h.duplicate_count, 0)
                self.assertTrue(original.held)
                self.assertTrue(h.dead)

    def test_channel_mutation_during_transfer_rejected_before_offer_use(self):
        h = Harness()
        h.owner.bootstrap()
        transfer = h.pipe.transfer

        def mutate_after_read(handle, kind, payload):
            result = transfer(handle, kind, payload)
            h.pipe.receipt.client_write = 99
            return result

        h.pipe.transfer = mutate_after_read
        with self.assertRaises(m.Hold):
            h.owner.accept_offer_and_ack()
        self.assertEqual(h.duplicate_count, 0)
        self.assertEqual(len([f for f in h.pipe.frames if f[1] == "write"]), 1)
        self.assertTrue(h.dead)

    def test_private_endpoint_redirect_after_ack_is_rejected(self):
        for field in ("_parent_read", "_parent_write"):
            for value in (999, True):
                with self.subTest(field=field, value=value):
                    h = Harness()
                    h.run()
                    original_pipe = h.pipe
                    frame_count = len(original_pipe.frames)
                    setattr(h.owner, field, value)
                    h.dead = True
                    with self.assertRaises(m.Hold):
                        h.owner.observe_worker_success()
                    self.assertEqual(len(original_pipe.frames), frame_count)
                    self.assertTrue(h.owner.termination_confirmed)
                    self.assertTrue(original_pipe.held)
                    self.assertEqual(h.owner._owned_outputs,
                        {"worker_process": 81, "worker_job": 82})
                    self.assertEqual(h.closed, [])

    def test_offer_or_owner_mutation_during_ack_write_holds_without_record(self):
        for mutation in ("offer", "watchdog", "outer", "channel"):
            with self.subTest(mutation=mutation):
                h = Harness()
                transfer = h.pipe.transfer

                def mutate_during_ack(handle, kind, payload):
                    result = transfer(handle, kind, payload)
                    if kind == "write" and len(payload) == 80:
                        if mutation == "offer":
                            h.owner.offer = replace(h.offer, pid=999)
                        elif mutation == "watchdog":
                            h.owner.watchdog = SimpleNamespace()
                        elif mutation == "outer":
                            h.owner.outer = 999
                        else:
                            h.pipe.receipt.parent_read = 99
                    return result

                h.pipe.transfer = mutate_during_ack
                with self.assertRaises(m.Hold):
                    h.run()
                self.assertEqual(h.pipe.frames[-1][2], m.encode_ack(h.offer))
                self.assertIsNone(h.ack)
                self.assertEqual(h.owner.phase, "hold")
                self.assertTrue(h.dead)

    def test_duplicate_custody_mutation_retains_private_outputs_and_blocks_ack_record(self):
        for boundary in ("ack-write", "registry-clock"):
            for mutation in ("replace-process", "clear", "replace-mapping"):
                with self.subTest(boundary=boundary, mutation=mutation):
                    h = Harness()

                    def corrupt():
                        if mutation == "replace-process":
                            h.owner.local["worker_process"] = 999
                        elif mutation == "clear":
                            h.owner.local.clear()
                        else:
                            h.owner.local = dict(h.owner.local)

                    if boundary == "ack-write":
                        transfer = h.pipe.transfer

                        def mutate_during_ack(handle, kind, payload):
                            result = transfer(handle, kind, payload)
                            if kind == "write" and len(payload) == 80:
                                corrupt()
                            return result

                        h.pipe.transfer = mutate_during_ack
                    else:
                        record = h.watchdog.record_handoff_ack

                        def mutate_during_registry(nonce, digest):
                            h.watchdog._tick()
                            corrupt()
                            record(nonce, digest)

                        h.watchdog.record_handoff_ack = mutate_during_registry
                    with self.assertRaises(m.Hold):
                        h.run()
                    self.assertEqual(h.owner._owned_outputs,
                        {"worker_process": 81, "worker_job": 82})
                    if boundary == "ack-write":
                        self.assertIsNone(h.ack)
                    self.assertTrue(h.dead)
                    self.assertEqual(h.owner.phase, "hold")
                    h.pipe.cleanup_ready = False
                    self.assertFalse(h.owner.close())
                    self.assertEqual(h.closed, [])
                    h.pipe.cleanup_ready = True
                    self.assertTrue(h.owner.close())
                    self.assertEqual(h.closed, [81, 82])
                    self.assertEqual(h.owner._owned_outputs, {})

    def test_registry_clock_mutation_detected_after_record_and_digest_is_exact(self):
        for mutation in ("offer", "channel", "deadline"):
            with self.subTest(mutation=mutation):
                h = Harness()
                watchdog = outer.OuterWatchdog(h.exports)
                watchdog._deadline = watchdog.receipt.deadline = 500
                watchdog.receipt.phase = "running-unacknowledged"
                watchdog.receipt.local = {"runner_process": 31, "outer_job": 21}
                h.watchdog = watchdog
                h.owner = m.ExactWorkerHandoff(h.exports, watchdog=watchdog,
                    pipe_owner=h.pipe, nonce=h.offer.nonce)
                record = h.watchdog.record_handoff_ack

                def mutate_from_registry(nonce, digest):
                    original_tick = h.watchdog._tick

                    def registry_tick():
                        now = original_tick()
                        if mutation == "offer":
                            h.owner.offer = replace(h.offer, process=999)
                        elif mutation == "channel":
                            h.pipe.receipt.client_read = 99
                        else:
                            h.now = 500
                        return now

                    h.watchdog._tick = registry_tick
                    record(nonce, digest)

                h.watchdog.record_handoff_ack = mutate_from_registry
                with self.assertRaises(m.Hold):
                    h.run()
                self.assertEqual(h.pipe.frames[-1][2], m.encode_ack(h.offer))
                self.assertEqual(watchdog.receipt.handoff_ack_digest,
                    m.hashlib.sha256(m.encode_offer(h.offer)).digest())
                self.assertEqual(h.owner.phase, "hold")
                self.assertTrue(h.dead)

    def test_real_cp331_shared_occurrence_and_clock_contract(self):
        for prior in (100, 400):
            with self.subTest(prior_tick=prior):
                h = Harness()
                watchdog = outer.OuterWatchdog(h.exports)
                watchdog._deadline = watchdog.receipt.deadline = 500
                watchdog.receipt.phase = "running-unacknowledged"
                watchdog.receipt.local = {"runner_process": 31, "outer_job": 21}
                watchdog._last_tick = prior
                first = m.ExactWorkerHandoff(h.exports, watchdog=watchdog,
                    pipe_owner=h.pipe, nonce=h.offer.nonce)
                second = m.ExactWorkerHandoff(h.exports, watchdog=watchdog,
                    pipe_owner=h.pipe, nonce=h.offer.nonce)
                if prior == 400:
                    with self.assertRaisesRegex(m.Hold, "regressing clock"):
                        first.bootstrap()
                    self.assertEqual(h.pipe.frames, [])
                else:
                    first.bootstrap()
                    first.accept_offer_and_ack()
                    self.assertEqual(watchdog.receipt.handoff_state, "acknowledged")
                    count = len(h.pipe.frames)
                    with self.assertRaisesRegex(m.Hold, "already claimed"):
                        second.bootstrap()
                    self.assertEqual(len(h.pipe.frames), count)
                self.assertTrue(h.dead)

    def test_partial_duplicate_outputs_owned_before_failure(self):
        for mode in ("raise", "false"):
            h = Harness()
            if mode == "raise":
                h.raise_at = "DuplicateHandle"
            else:
                h.bad = "duplicate-false"
            with self.assertRaises((m.Hold, RuntimeError)):
                h.run()
            self.assertEqual(h.owner.local, {"worker_process": 81})
            self.assertTrue(h.dead)
            self.assertNotIn(81, h.closed)
            self.assertTrue(h.owner.close())
            self.assertIn(81, h.closed)

    def test_output_adopted_before_last_error_hook_even_when_hook_raises(self):
        h = Harness()
        h.bad = "duplicate-false"

        def error_hook():
            self.assertEqual(h.owner.local, {"worker_process": 81})
            raise RuntimeError("last-error hook failed after ownership check")

        h.exports.last_error = error_hook
        with self.assertRaisesRegex(RuntimeError, "last-error hook failed"):
            h.run()
        self.assertEqual(h.owner.local, {"worker_process": 81})
        self.assertTrue(h.dead)
        self.assertEqual(len([f for f in h.pipe.frames if f[1] == "write"]), 1)

    def test_outer_policy_revalidated_before_ack(self):
        for flags in (0, 0x800, 0x1000, m.creator.KILL_ON_CLOSE | 0x800,
                      m.creator.KILL_ON_CLOSE | 0x1000, m.creator.KILL_ON_CLOSE | 8):
            with self.subTest(flags=flags):
                h = Harness()
                h.outer_flags = flags
                with self.assertRaisesRegex(m.Hold, "sole kill-on-close"):
                    h.run()
                self.assertTrue(h.dead)
                self.assertEqual(len([f for f in h.pipe.frames if f[1] == "write"]), 1)
                self.assertFalse(any(name == "ResumeThread" for name, _ in h.calls))

    def test_alias_output_retained_and_blocks_cleanup(self):
        for value in (11, 12, 21, 31, m.INVALID, m.INVALID - 1):
            h = Harness()
            h.output_override = value
            with self.assertRaises(m.Hold):
                h.run()
            self.assertEqual(h.owner.uncertain, [("worker_process", value)])
            self.assertFalse(h.owner.close())
            self.assertEqual(h.closed, [])

    def test_deadline_regression_and_custody_changes(self):
        for mutate in (lambda h: setattr(h, "now", 500),
                       lambda h: setattr(h, "now", 99),
                       lambda h: setattr(h.pipe, "deadline", 501),
                       lambda h: h.watchdog.receipt.local.update(runner_process=99)):
            h = Harness()
            h.owner.bootstrap()
            mutate(h)
            with self.assertRaises(m.Hold):
                h.owner.accept_offer_and_ack()
            self.assertTrue(h.dead)
            self.assertEqual(h.duplicate_count, 0)

    def test_partial_bootstrap_and_uncertain_ack(self):
        h = Harness()
        h.partial_write = True
        with self.assertRaises(m.Hold):
            h.run()
        self.assertTrue(h.dead)
        h = Harness()
        h.raise_ack = True
        with self.assertRaises(RuntimeError):
            h.run()
        self.assertIsNone(h.ack)
        self.assertEqual(h.claim, h.offer.nonce)
        self.assertEqual(h.pipe.operations, ["uncertain-overlapped-buffer"])
        h.pipe.cleanup_ready = False
        self.assertFalse(h.owner.close())
        self.assertEqual(h.owner.local, {"worker_process": 81, "worker_job": 82})
        h.pipe.cleanup_ready = True
        self.assertTrue(h.owner.close())
        self.assertEqual(h.pipe.operations, [])

    def test_cleanup_failures_retry_and_live_factory_hold(self):
        h = Harness()
        h.run()
        h.fail = "TerminateJobObject"
        self.assertFalse(h.owner.close())
        self.assertEqual(h.closed, [])
        h.fail = "CloseHandle"
        self.assertFalse(h.owner.close())
        self.assertEqual(len(h.owner.local), 2)
        h.fail = None
        kills = len([name for name, _ in h.calls if name == "TerminateJobObject"])
        self.assertTrue(h.owner.close())
        self.assertEqual(len([name for name, _ in h.calls if name == "TerminateJobObject"]), kills)
        self.assertTrue(h.owner.close())
        with self.assertRaises(m.Hold):
            m.create_live_handoff()

    def test_success_observes_exact_duplicate_then_releases_without_abort(self):
        h = Harness()
        h.run()
        h.dead = True  # injected exact WORKER exit and zero active job count
        self.assertTrue(h.owner.observe_worker_success())
        self.assertEqual(h.owner.phase, "worker-success-observed")
        self.assertEqual(h.owner.local, {"worker_process": 81, "worker_job": 82})
        self.assertEqual(h.closed, [])
        self.assertFalse(h.owner.termination_confirmed)
        self.assertTrue(h.owner.release_worker_success())
        self.assertEqual(h.closed, [81, 82])
        self.assertEqual(h.owner.local, {})
        self.assertEqual(h.owner.phase, "worker-success-released")
        self.assertTrue(h.owner.release_worker_success())
        self.assertFalse(any(name == "TerminateJobObject" for name, _ in h.calls))
        self.assertFalse(any(name in ("OpenProcess", "ResumeThread") for name, _ in h.calls))

    def test_success_requires_wait_zero_exit_zero_empty_job_and_deadline(self):
        for fault in ("wait", "exit", "active", "deadline", "pending"):
            with self.subTest(fault=fault):
                h = Harness()
                h.run()
                h.dead = True
                if fault == "wait":
                    h.success_wait_status = 258
                elif fault == "exit":
                    h.success_exit_code = 3
                elif fault == "active":
                    h.success_active = 1
                elif fault == "deadline":
                    h.now = 500
                else:
                    h.pipe.operations.append(SimpleNamespace(terminal=False))
                with self.assertRaises(m.Hold):
                    h.owner.observe_worker_success()
                self.assertEqual(h.owner.phase, "hold")
                self.assertTrue(h.owner.termination_confirmed)
                self.assertEqual(h.owner._owned_outputs,
                    {"worker_process": 81, "worker_job": 82})
                self.assertEqual(h.closed, [])

    def test_success_release_retries_remaining_exact_duplicates(self):
        h = Harness()
        h.run()
        h.dead = True
        h.owner.observe_worker_success()
        h.fail = "CloseHandle"
        self.assertFalse(h.owner.release_worker_success())
        self.assertEqual(h.owner._owned_outputs,
            {"worker_process": 81, "worker_job": 82})
        self.assertEqual(h.closed, [])
        h.fail = None
        self.assertTrue(h.owner.release_worker_success())
        self.assertEqual(h.closed, [81, 82])
        self.assertFalse(any(name == "TerminateJobObject" for name, _ in h.calls))

    def test_public_phase_cannot_forge_success_release(self):
        h = Harness()
        h.run()
        h.owner.phase = "worker-success-released"
        with self.assertRaises(m.Hold):
            h.owner.release_worker_success()
        self.assertEqual(h.owner._owned_outputs,
            {"worker_process": 81, "worker_job": 82})
        self.assertEqual(h.closed, [])

    def test_close_callback_mutation_stops_before_second_duplicate(self):
        h = Harness()
        h.run()
        h.dead = True
        h.owner.observe_worker_success()
        original = h.dispatch

        def mutate_after_first_close(name, *args):
            result = original(name, *args)
            if name == "CloseHandle" and args[0] == 81:
                h.owner.local["worker_job"] = 999
            return result

        h.dispatch = mutate_after_first_close
        self.assertFalse(h.owner.release_worker_success())
        self.assertEqual(h.closed, [81])
        self.assertEqual(h.owner._owned_outputs, {"worker_job": 82})
        self.assertFalse(h.owner._worker_success_released)
        self.assertEqual(h.owner.phase, "worker-success-observed")
        self.assertFalse(any(name == "TerminateJobObject" for name, _ in h.calls))

    def test_success_observation_cannot_replay_or_skip_ack(self):
        h = Harness()
        with self.assertRaises(m.Hold):
            h.owner.observe_worker_success()
        self.assertTrue(h.dead)
        h = Harness()
        h.run()
        h.dead = True
        h.owner.observe_worker_success()
        with self.assertRaises(m.Hold):
            h.owner.observe_worker_success()
        self.assertTrue(h.owner.termination_confirmed)


if __name__ == "__main__":
    unittest.main()
