"""CP331 inert injected BoundExports tests; never load DLLs or start a process."""
import ctypes as C
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp331_outer_watchdog as m
from cp325_real_bindings import BoundExports


def out(pointer, kind, value):
    C.cast(pointer, C.POINTER(kind)).contents.value = value


class Harness:
    def __init__(self):
        self.calls, self.closed, self.attributes = [], [], {}
        self.now, self.error = 100, 122
        self.created = self.dead = False
        self.host_job = 1
        self.bad = None
        self.wait = None
        self.exit_stamp = 0
        self.stamp = 123456
        self.exit_code = 0
        self.create_raise = self.create_false = self.no_outputs = False
        self.fail = self.raise_at = None
        self.exports = BoundExports({name: (lambda *a, n=name: self.dispatch(n, *a))
            for name in m.abi.EXPORTS}, {}, lambda: self.error)
        self.owner = m.OuterWatchdog(self.exports)

    def dispatch(self, name, *args):
        self.calls.append((name, args))
        if name == self.raise_at:
            raise RuntimeError("uncertain native outcome")
        if name == self.fail:
            self.error = 5
            return 0
        if name == "GetTickCount64":
            return self.now
        if name == "GetCurrentProcess":
            return m.INVALID_HANDLE
        if name == "GetHandleInformation":
            out(args[1], m.abi.DWORD, int(args[0] in (11, 13)))
        elif name == "CreateJobObjectW":
            assert args == (None, None)
            return 21
        elif name == "QueryInformationJobObject":
            _, kind, pointer, size, returned = args
            out(returned, m.abi.DWORD, size - int(self.bad == "size"))
            if kind == m.JOB_LIMITS:
                obj = C.cast(pointer, C.POINTER(m.abi.EXTENDED_LIMIT)).contents
                obj.BasicLimitInformation.LimitFlags = m.KILL_ON_CLOSE | (0x1000 if self.bad == "limits" else 0)
            else:
                obj = C.cast(pointer, C.POINTER(m.abi.BASIC_ACCOUNTING)).contents
                obj.ActiveProcesses = (2 if self.bad == "accounting" else
                    int(self.created and not self.dead))
        elif name == "InitializeProcThreadAttributeList":
            if args[0] is None:
                out(args[3], m.abi.SIZE_T, 256)
                self.error = 122
                return 0
        elif name == "UpdateProcThreadAttribute":
            self.attributes[args[2]] = tuple(C.cast(args[3], C.POINTER(m.abi.HANDLE))[i]
                for i in range(args[4] // C.sizeof(m.abi.HANDLE)))
        elif name == "CreateProcessW":
            self.created = True
            self.creation = args
            if not self.no_outputs:
                info = C.cast(args[9], C.POINTER(m.abi.PROCESS_INFORMATION)).contents
                info.hProcess, info.hThread, info.dwProcessId, info.dwThreadId = 31, 32, 33, 34
            if self.create_raise:
                raise RuntimeError("unknown CreateProcessW outcome")
            if self.create_false:
                self.created = not self.no_outputs
                self.error = 5
                return 0
        elif name == "GetProcessId":
            assert args == (31,)
            return 99 if self.bad == "pid" else 33
        elif name == "QueryFullProcessImageNameW":
            path = r"C:\wrong.exe" if self.bad == "image" else m.RUNNER_IMAGE
            args[2].value = path
            out(args[3], m.abi.DWORD, len(path))
        elif name == "IsProcessInJob":
            if args[1] is None:
                out(args[2], m.abi.BOOL, self.host_job)
            else:
                assert args[:2] == (31, 21)
                out(args[2], m.abi.BOOL, int(self.bad != "membership"))
        elif name == "GetProcessTimes":
            assert args[0] == 31
            obj = C.cast(args[1], C.POINTER(m.abi.FILETIME)).contents
            obj.dwLowDateTime = self.stamp
            C.cast(args[2], C.POINTER(m.abi.FILETIME)).contents.dwLowDateTime = self.exit_stamp
        elif name == "GetExitCodeProcess":
            assert args[0] == 31
            out(args[1], m.abi.DWORD, self.exit_code)
        elif name == "ResumeThread":
            assert args == (32,)
            return 2 if self.bad == "suspend" else 1
        elif name in ("TerminateJobObject", "TerminateProcess"):
            assert args[0] == (21 if name == "TerminateJobObject" else 31)
            self.dead = True
        elif name == "WaitForSingleObject":
            assert args[0] == 31
            assert 0 <= args[1] <= 5000
            return (258 if self.created and not self.dead else 0) if self.wait is None else self.wait
        elif name == "CloseHandle":
            self.closed.append(args[0])
        return 1

    def prepare(self):
        if self.owner.deadline is None:
            self.owner.start_occurrence()
        return self.owner.prepare_suspended(child_read=11, child_write=13)

    def names(self):
        return [n for n, _ in self.calls]


class WatchdogTests(unittest.TestCase):
    @staticmethod
    def acknowledged_runner():
        h = Harness()
        h.prepare()
        h.owner.resume_once()
        nonce = b"n" * 32
        h.owner.claim_handoff(nonce)
        h.owner.record_handoff_ack(nonce, b"d" * 32)
        return h

    def test_runner_success_observation_uses_exact_wait_exit_and_empty_job(self):
        h = self.acknowledged_runner()
        h.dead = True
        result = h.owner.observe_runner_success()
        self.assertIs(result, h.owner.receipt)
        self.assertEqual(result.phase, "runner-closed-success")
        self.assertTrue(result.runner_waited and result.job_empty)
        self.assertEqual(h.closed, [32, 31, 21])
        self.assertEqual(h.owner._owned_handles, {})
        self.assertNotIn("TerminateJobObject", h.names())
        self.assertNotIn("TerminateProcess", h.names())
        self.assertEqual(h.names().count("GetExitCodeProcess"), 1)
        self.assertEqual(h.owner.deadline, 5100)
        self.assertIs(h.owner.close(), result)
        self.assertEqual(result.phase, "runner-closed-success")

    def test_runner_success_requires_ack_and_independent_exit_evidence(self):
        h = Harness()
        h.prepare()
        h.owner.resume_once()
        h.dead = True
        with self.assertRaises(m.WatchdogFailure):
            h.owner.observe_runner_success()
        self.assertNotIn("GetExitCodeProcess", h.names())
        self.assertNotEqual(h.owner.receipt.phase, "runner-closed-success")
        for fault in ("wait", "exit", "active", "deadline"):
            with self.subTest(fault=fault):
                h = self.acknowledged_runner()
                h.dead = True
                if fault == "wait":
                    h.wait = 258
                elif fault == "exit":
                    h.exit_code = 7
                elif fault == "active":
                    h.bad = "accounting"
                else:
                    h.now = 5100
                with self.assertRaises(m.WatchdogFailure):
                    h.owner.observe_runner_success()
                self.assertNotEqual(h.owner.receipt.phase, "runner-closed-success")

    def test_public_success_phase_cannot_skip_private_ack_or_exit_proof(self):
        for acknowledged in (False, True):
            with self.subTest(acknowledged=acknowledged):
                h = Harness()
                h.prepare()
                h.owner.resume_once()
                if acknowledged:
                    nonce = b"n" * 32
                    h.owner.claim_handoff(nonce)
                    h.owner.record_handoff_ack(nonce, b"d" * 32)
                h.owner.receipt.phase = "runner-success-observed"
                with self.assertRaises(m.WatchdogFailure):
                    h.owner.observe_runner_success()
                self.assertNotIn("GetExitCodeProcess", h.names())
                self.assertNotEqual(h.owner.receipt.phase, "runner-closed-success")
                self.assertIn("TerminateJobObject", h.names())
        h = Harness()
        h.prepare()
        h.owner.receipt.phase = "closed"
        h.owner.close()
        self.assertIn("TerminateJobObject", h.names())

    def test_success_cleanup_failure_retries_release_without_reobservation(self):
        h = self.acknowledged_runner()
        h.dead = True
        h.fail = "CloseHandle"
        with self.assertRaises(m.WatchdogFailure):
            h.owner.observe_runner_success()
        self.assertEqual(h.owner.receipt.phase, "runner-success-observed")
        self.assertEqual(h.owner._owned_handles,
            {"outer_job": 21, "runner_process": 31, "runner_thread": 32})
        self.assertEqual(h.names().count("GetExitCodeProcess"), 1)
        h.fail = None
        self.assertEqual(h.owner.observe_runner_success().phase, "runner-closed-success")
        self.assertEqual(h.names().count("GetExitCodeProcess"), 1)
        self.assertNotIn("TerminateJobObject", h.names())

    def test_success_query_receipt_mutation_uses_private_exact_cleanup(self):
        class EqualitySpoof:
            def __eq__(self, _other):
                return True
        for value in (999, EqualitySpoof(), True):
            with self.subTest(value_type=type(value)):
                h = self.acknowledged_runner()
                h.dead = True
                original = h.dispatch
                def dispatch(name, *args):
                    result = original(name, *args)
                    if name == "GetExitCodeProcess":
                        h.owner.receipt.local["runner_process"] = value
                    return result
                h.dispatch = dispatch
                with self.assertRaises(m.WatchdogFailure):
                    h.owner.observe_runner_success()
                self.assertEqual(h.owner.receipt.phase, "hold")
                self.assertNotIn(999, h.closed)
                self.assertEqual(h.closed, [32, 31, 21])

    def test_replaced_public_receipt_cannot_lose_creation_owned_evidence(self):
        h = self.acknowledged_runner()
        h.dead = True
        original_receipt = h.owner.receipt
        original = h.dispatch
        def dispatch(name, *args):
            result = original(name, *args)
            if name == "GetExitCodeProcess":
                h.owner.receipt = m.Receipt(phase="running-unacknowledged",
                    deadline=original_receipt.deadline, local=original_receipt.local)
            return result
        h.dispatch = dispatch
        with self.assertRaises(m.WatchdogFailure):
            h.owner.observe_runner_success()
        self.assertIs(h.owner.receipt, original_receipt)
        self.assertEqual(original_receipt.phase, "closed")
        self.assertEqual(h.closed, [32, 31, 21])

    def test_public_cleanup_flags_cannot_bypass_failed_success_wait(self):
        for field_set in ("create", "wait-and-accounting"):
            with self.subTest(field_set=field_set):
                h = self.acknowledged_runner()
                original = h.dispatch
                def dispatch(name, *args):
                    result = original(name, *args)
                    if name == "WaitForSingleObject" and args[1] > 1000:
                        if field_set == "create":
                            h.owner.receipt.create_outcome = "not-attempted"
                        else:
                            h.owner.receipt.runner_waited = True
                            h.owner.receipt.job_empty = True
                    return result
                h.dispatch = dispatch
                with self.assertRaises(m.WatchdogFailure):
                    h.owner.observe_runner_success()
                self.assertIn("TerminateJobObject", h.names())
                waits = [a for n, a in h.calls if n == "WaitForSingleObject"]
                self.assertGreaterEqual(len(waits), 4)  # two stamps, failed success, abort wait
                self.assertEqual(h.closed, [32, 31, 21])
                self.assertNotEqual(h.owner.receipt.phase, "runner-closed-success")

    def test_construction_and_public_entry_inert(self):
        h = Harness()
        self.assertEqual(h.calls, [])
        with self.assertRaisesRegex(m.Hold, "HARD HOLD"):
            m.launch_trial(exports=h.exports)
        self.assertEqual(h.calls, [])
        with self.assertRaises(TypeError):
            m.OuterWatchdog(object())

    def test_start_required_and_deadline_read_only(self):
        h = Harness()
        with self.assertRaisesRegex(m.WatchdogFailure, "start_occurrence"):
            h.owner.prepare_suspended(child_read=11, child_write=13)
        self.assertNotIn("CreateProcessW", h.names())
        h = Harness()
        self.assertEqual(h.owner.start_occurrence(), 5100)
        self.assertEqual(h.names(), ["GetTickCount64"])
        with self.assertRaises(AttributeError):
            h.owner.deadline = 6000
        with self.assertRaisesRegex(m.Hold, "once"):
            h.owner.start_occurrence()
        self.assertEqual(h.owner.deadline, 5100)

    def test_elapsed_pipe_preparation_consumes_same_deadline(self):
        h = Harness()
        deadline = h.owner.start_occurrence()
        # Stand-in for caller CP328 preparation under that same deadline.
        h.now = 4000
        h.prepare()
        self.assertEqual(h.owner.deadline, deadline)
        self.assertEqual(h.owner._budget(), 1100)
        h.owner.resume_once()
        h.wait = 258
        with self.assertRaises(m.WatchdogFailure):
            h.owner.observe_runner()
        waits = [a for n, a in h.calls if n == "WaitForSingleObject"]
        self.assertIn((31, 1100), waits)
        self.assertEqual(h.owner.receipt.deadline, 5100)

    def test_pipe_preparation_exhaustion_prevents_create_without_renewal(self):
        h = Harness()
        h.owner.start_occurrence()
        h.now = 5100
        with self.assertRaises(m.WatchdogFailure):
            h.prepare()
        self.assertNotIn("CreateProcessW", h.names())
        self.assertEqual(h.owner.deadline, 5100)
        with self.assertRaisesRegex(m.Hold, "once"):
            h.owner.start_occurrence()

    def test_suspended_creation_attributes_identity_and_deadline(self):
        h = Harness()
        r = h.prepare()
        self.assertEqual(r.phase, "prepared-suspended")
        self.assertEqual(r.deadline, 5100)
        self.assertEqual(r.creation_time, 123456)
        self.assertTrue(r.host_in_job)
        self.assertEqual(h.attributes, {m.JOB_LIST: (21,), m.HANDLE_LIST: (11, 13)})
        self.assertEqual(h.creation[0], m.RUNNER_IMAGE)
        self.assertEqual(h.creation[4:6], (True, m.CREATE_FLAGS))
        expected_bootstrap = ("import sys;sys.path.insert(0," + repr(m.RUNNER_CWD) +
            ");import cp335_fixed_runner_entry as e;"
            "e.FixedRunnerEntry(sys.argv[1:]).run()")
        self.assertEqual(h.creation[1].value, m.subprocess.list2cmdline([
            m.RUNNER_IMAGE, "-I", "-c", expected_bootstrap,
            "--cp334-client", "11", "13", "5100"]))
        self.assertIn("-I", h.creation[1].value)
        self.assertIn("--cp334-client", h.creation[1].value)
        self.assertIn("FixedRunnerEntry(sys.argv[1:]).run()", h.creation[1].value)
        self.assertNotIn("cp330_native_inert_runner.py", h.creation[1].value)
        self.assertNotIn("ResumeThread", h.names())
        self.assertLess(h.names().index("GetTickCount64"), h.names().index("CreateProcessW"))
        h.owner.close()
        self.assertEqual(h.closed, [32, 31, 21])
        self.assertNotIn(11, h.closed)
        self.assertNotIn(13, h.closed)

    def test_resume_once_checks_creation_timestamp_twice(self):
        h = Harness()
        h.prepare()
        h.owner.resume_once()
        self.assertEqual(h.names().count("GetProcessTimes"), 2)
        self.assertEqual(h.names().count("ResumeThread"), 1)
        with self.assertRaises(m.WatchdogFailure):
            h.owner.resume_once()
        self.assertEqual(h.names().count("ResumeThread"), 1)

    def test_identity_rejections_prevent_resume(self):
        for bad in ("pid", "image", "membership", "limits", "size", "accounting"):
            with self.subTest(bad=bad):
                h = Harness()
                h.bad = bad
                with self.assertRaises(m.WatchdogFailure):
                    h.prepare()
                self.assertNotIn("ResumeThread", h.names())

    def test_undefined_exit_filetime_does_not_reject_live_runner(self):
        h = Harness()
        h.exit_stamp = 987654321  # undefined for a live process
        h.prepare()
        h.owner.resume_once()
        self.assertEqual(h.owner.receipt.resume_outcome, "resumed-once")
        waits = [args for name, args in h.calls if name == "WaitForSingleObject"]
        self.assertEqual(waits, [(31, 0), (31, 0)])
        h.owner.close()

    def test_already_exited_runner_cannot_resume(self):
        h = Harness()
        h.wait = 0
        with self.assertRaises(m.WatchdogFailure):
            h.prepare()
        self.assertNotIn("ResumeThread", h.names())

    def test_timestamp_change_and_zero_rejected(self):
        h = Harness()
        h.prepare()
        h.stamp += 1
        with self.assertRaises(m.WatchdogFailure):
            h.owner.resume_once()
        self.assertNotIn("ResumeThread", h.names())
        h = Harness()
        h.stamp = 0
        with self.assertRaises(m.WatchdogFailure):
            h.prepare()

    def test_expired_deadline_before_resume_terminates(self):
        h = Harness()
        h.prepare()
        h.now = 5100
        with self.assertRaises(m.WatchdogFailure):
            h.owner.resume_once()
        self.assertNotIn("ResumeThread", h.names())
        self.assertIn("TerminateJobObject", h.names())
        self.assertEqual(h.owner.receipt.deadline, 5100)

    def test_timeout_and_eof_both_fail_closed(self):
        for status in (0, 258, 0xFFFFFFFF):
            with self.subTest(status=status):
                h = Harness()
                h.prepare()
                h.owner.resume_once()
                h.wait = status
                with self.assertRaises(m.WatchdogFailure):
                    h.owner.observe_runner()
                self.assertIn("TerminateJobObject", h.names())
                if status:
                    self.assertEqual(h.closed, [])
                    self.assertEqual(h.owner.receipt.phase, "hold")
                    h.wait = 0
                    self.assertEqual(h.owner.close().phase, "closed")
                self.assertNotIn("success", h.owner.receipt.phase)

    def test_uncertain_create_keeps_exact_outputs_and_cleans(self):
        h = Harness()
        h.create_raise = True
        with self.assertRaises(m.WatchdogFailure) as caught:
            h.prepare()
        self.assertIs(caught.exception.owner, h.owner)
        self.assertIn("unknown", h.owner.receipt.create_outcome)
        self.assertEqual(h.owner.receipt.pid, 33)
        self.assertEqual(h.closed, [32, 31, 21])

    def test_unknown_create_without_handle_retains_outer(self):
        h = Harness()
        h.create_raise = h.no_outputs = True
        with self.assertRaises(m.WatchdogFailure):
            h.prepare()
        self.assertEqual(h.owner.receipt.phase, "hold")
        self.assertEqual(h.owner.receipt.local, {"outer_job": 21})
        self.assertEqual(h.closed, [])
        self.assertEqual(h.owner.close().phase, "hold")

    def test_known_failed_nested_creation_no_fallback(self):
        h = Harness()
        h.create_false = h.no_outputs = True
        with self.assertRaises(m.WatchdogFailure):
            h.prepare()
        self.assertTrue(h.owner.receipt.host_in_job)
        self.assertEqual(h.owner.receipt.create_outcome, "returned-failure-no-child")
        self.assertEqual(h.names().count("CreateProcessW"), 1)
        self.assertNotIn("AssignProcessToJobObject", h.names())
        self.assertEqual(h.closed, [21])

    def test_job_kill_failure_uses_exact_process_fallback(self):
        h = Harness()
        h.prepare()
        h.fail = "TerminateJobObject"
        h.wait = 258
        h.owner.close()
        self.assertIn("TerminateProcess", h.names())
        self.assertEqual(h.owner.receipt.phase, "hold")
        self.assertEqual(h.closed, [])
        h.wait = 0
        self.assertEqual(h.owner.close().phase, "closed")

    def test_job_accounting_and_failed_close_retain_owners(self):
        h = Harness()
        h.prepare()
        h.bad = "accounting"
        self.assertEqual(h.owner.close().phase, "hold")
        self.assertEqual(h.closed, [])
        h.bad = None
        h.fail = "CloseHandle"
        self.assertEqual(h.owner.close().phase, "hold")
        self.assertEqual(len(h.owner.receipt.local), 3)
        h.fail = None
        self.assertEqual(h.owner.close().phase, "closed")
        self.assertEqual(h.closed, [32, 31, 21])

    def test_handoff_claim_is_occurrence_global_and_irreversible(self):
        h = Harness()
        h.prepare()
        h.owner.resume_once()
        nonce = b"n" * 32
        h.owner.claim_handoff(nonce)
        self.assertEqual(h.owner.receipt.handoff_state, "claimed")
        for replay in (nonce, b"x" * 32):
            with self.assertRaisesRegex(m.Hold, "already claimed"):
                h.owner.claim_handoff(replay)
        # Receipt mutation cannot reset the authoritative private claim.
        h.owner.receipt.handoff_state = "unclaimed"
        h.owner.receipt.handoff_nonce = None
        with self.assertRaisesRegex(m.Hold, "already claimed"):
            h.owner.claim_handoff(nonce)
        h.owner.close()
        with self.assertRaises(m.Hold):
            h.owner.claim_handoff(nonce)

    def test_handoff_ack_matches_claim_once_without_phase_change(self):
        h = Harness()
        h.prepare()
        h.owner.resume_once()
        nonce, digest = b"n" * 32, b"d" * 32
        with self.assertRaises(m.Hold):
            h.owner.record_handoff_ack(nonce, digest)
        h.owner.claim_handoff(nonce)
        with self.assertRaises(m.Hold):
            h.owner.record_handoff_ack(b"x" * 32, digest)
        with self.assertRaises(m.Hold):
            h.owner.record_handoff_ack(nonce, b"short")
        h.owner.record_handoff_ack(nonce, digest)
        self.assertEqual(h.owner.receipt.handoff_state, "acknowledged")
        self.assertEqual(h.owner.receipt.handoff_ack_digest, digest)
        self.assertEqual(h.owner.receipt.phase, "running-unacknowledged")
        with self.assertRaises(m.Hold):
            h.owner.record_handoff_ack(nonce, digest)
        with self.assertRaises(m.WatchdogFailure):
            h.owner.observe_runner()

    def test_handoff_claim_and_ack_share_monotonic_clock(self):
        h = Harness()
        h.prepare()
        h.owner.resume_once()
        h.now = 400
        h.owner._tick()  # an intervening owner's shared clock observation
        h.now = 399
        with self.assertRaisesRegex(m.Hold, "regressing"):
            h.owner.claim_handoff(b"n" * 32)
        h.now = 401
        h.owner.claim_handoff(b"n" * 32)
        h.now = 400
        with self.assertRaisesRegex(m.Hold, "regressing"):
            h.owner.record_handoff_ack(b"n" * 32, b"d" * 32)
        self.assertEqual(h.owner.receipt.handoff_state, "claimed")
        h.owner.close()

    def test_handoff_claim_requires_live_phase_nonce_and_budget(self):
        h = Harness()
        h.prepare()
        with self.assertRaises(m.Hold):
            h.owner.claim_handoff(b"n" * 32)
        h.owner.resume_once()
        for nonce in (b"short", bytearray(32), "n" * 32):
            with self.assertRaises(m.Hold):
                h.owner.claim_handoff(nonce)
        h.now = 5100
        with self.assertRaisesRegex(m.Hold, "expired"):
            h.owner.claim_handoff(b"n" * 32)
        self.assertEqual(h.owner.receipt.handoff_state, "unclaimed")
        h.owner.close()

    def test_bad_resume_result_never_retried(self):
        h = Harness()
        h.prepare()
        h.bad = "suspend"
        with self.assertRaises(m.WatchdogFailure):
            h.owner.resume_once()
        with self.assertRaises(m.WatchdogFailure):
            h.owner.resume_once()
        self.assertEqual(h.names().count("ResumeThread"), 1)

    def test_inheritance_rejected_before_create(self):
        h = Harness()
        h.owner.start_occurrence()
        with self.assertRaises(m.WatchdogFailure):
            h.owner.prepare_suspended(child_read=12, child_write=13)
        self.assertNotIn("CreateProcessW", h.names())

    def test_clock_regression_prevents_resume(self):
        h = Harness()
        h.prepare()
        h.now = 99
        with self.assertRaises(m.WatchdogFailure):
            h.owner.resume_once()
        self.assertNotIn("ResumeThread", h.names())

    def test_create_overruns_original_deadline_before_resume(self):
        h = Harness()
        original = h.dispatch
        def dispatch(name, *args):
            result = original(name, *args)
            if name == "CreateProcessW":
                h.now = 6000
            return result
        h.dispatch = dispatch
        with self.assertRaises(m.WatchdogFailure):
            h.prepare()
        self.assertNotIn("ResumeThread", h.names())
        self.assertEqual(h.owner.deadline, 5100)
        self.assertIn("TerminateJobObject", h.names())

    def test_partial_failed_creation_keeps_exact_handle_cleanup(self):
        h = Harness()
        h.create_false = True
        with self.assertRaises(m.WatchdogFailure):
            h.prepare()
        self.assertIn("unknown", h.owner.receipt.create_outcome)
        self.assertEqual(h.closed, [32, 31, 21])

    def test_aliased_output_retains_owners(self):
        h = Harness()
        original = h.dispatch
        def dispatch(name, *args):
            result = original(name, *args)
            if name == "CreateProcessW":
                info = C.cast(args[9], C.POINTER(m.abi.PROCESS_INFORMATION)).contents
                info.hThread = info.hProcess
            return result
        h.dispatch = dispatch
        with self.assertRaises(m.WatchdogFailure):
            h.prepare()
        self.assertEqual(h.owner.receipt.phase, "hold")
        self.assertEqual(h.closed, [])
        self.assertEqual(h.owner._ownership_uncertain, [("runner_thread", 31)])

    def test_resume_exception_retains_unknown_outcome(self):
        h = Harness()
        h.prepare()
        h.raise_at = "ResumeThread"
        with self.assertRaises(m.WatchdogFailure):
            h.owner.resume_once()
        self.assertIn("unknown", h.owner.receipt.resume_outcome)
        self.assertEqual(h.names().count("ResumeThread"), 1)
        self.assertEqual(h.closed, [32, 31, 21])

    def test_uncertain_attribute_init_retains_storage(self):
        h = Harness()
        original = h.dispatch
        def dispatch(name, *args):
            if name == "InitializeProcThreadAttributeList" and args[0] is not None:
                raise RuntimeError("uncertain initialization")
            return original(name, *args)
        h.dispatch = dispatch
        with self.assertRaises(m.WatchdogFailure):
            h.prepare()
        self.assertEqual(h.owner.receipt.phase, "hold")
        self.assertIsNotNone(h.owner._attributes)
        self.assertEqual(h.closed, [])


if __name__ == "__main__":
    unittest.main()
