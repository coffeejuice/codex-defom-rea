"""CP304 protocol tests; all process and job operations are fake calls."""
import ctypes as C
from ctypes import wintypes as W
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp303_job_creation as creation
import cp304_job_supervisor as m


IMAGE = creation.FixedCloneImage(
    "C:\\PROBLEM\\m289_0123456789abcdef\\fixture.exe",
    "C:\\PROBLEM\\m289_0123456789abcdef", creation.CP302_SHA256)


class FakeApi:
    cp303_fake_api = True
    cp304_fake_api = True

    def __init__(self, fail=(), active=0):
        self.calls = []
        self.fail = set(fail)
        self.active = active

    def last_error(self):
        return 1234

    def CreateJobObjectW(self, security, name):
        self.calls.append(("create-job",))
        return 71

    def GetHandleInformation(self, handle, flags):
        self.calls.append(("job-flags", handle))
        C.cast(flags, C.POINTER(W.DWORD))[0] = 0
        return True

    def SetInformationJobObject(self, handle, info_class, limits, size):
        self.calls.append(("set-job", handle))
        return True

    def GetCurrentProcess(self):
        return 99

    def DuplicateHandle(self, source, handle, target, output, access, inherit, options):
        self.calls.append(("duplicate", source, handle, target, inherit, options))
        if "duplicate-child" in self.fail and handle == 202:
            return False
        C.cast(output, C.POINTER(W.HANDLE))[0] = 81 if handle == 71 else 303
        return True

    def GetProcessId(self, handle):
        self.calls.append(("pid", handle))
        return 123 if "pid-mismatch" not in self.fail else 124

    def GetProcessTimes(self, handle):
        self.calls.append(("times", handle))
        return 456 if "time-mismatch" not in self.fail else 457

    def QueryFullProcessImageNameW(self, handle):
        self.calls.append(("image", handle))
        return IMAGE.executable if "image-mismatch" not in self.fail else r"C:\PROBLEM\other.exe"

    def TerminateJobObject(self, handle, exit_code):
        self.calls.append(("terminate-job", handle))
        return "terminate-job" not in self.fail

    def TerminateProcess(self, handle, exit_code):
        self.calls.append(("terminate-worker", handle))
        return "terminate-worker" not in self.fail

    def WaitForSingleObject(self, handle, ms):
        self.calls.append(("wait", handle, ms))
        if handle == 303 and "child-timeout" in self.fail:
            return m.WAIT_TIMEOUT
        if handle == 42 and ms == 0:
            return m.WAIT_TIMEOUT
        if handle == 42 and "worker-timeout" in self.fail:
            return m.WAIT_TIMEOUT
        return m.WAIT_OBJECT_0

    def QueryInformationJobObject(self, handle):
        self.calls.append(("query-active", handle))
        return self.active

    def CloseHandle(self, handle):
        self.calls.append(("close", handle))
        return not ((handle == 303 and "close-child" in self.fail) or
                    (handle == 42 and "close-worker" in self.fail) or
                    (handle == 71 and "close-job" in self.fail))


OFFER = m.ChildOffer("occurrence-1", 202, 123, 124, 456, IMAGE.executable)


def started(api):
    supervisor = m.FakeJobSupervisor(api, IMAGE)
    supervisor.stage_job()
    supervisor.bind_worker(42)
    supervisor.start_worker()
    return supervisor


class JobSupervisorTests(unittest.TestCase):
    def test_order_identity_ack_receipt_and_cleanup(self):
        api = FakeApi()
        supervisor = m.FakeJobSupervisor(api, IMAGE)
        with self.assertRaises(RuntimeError):
            supervisor.bind_worker(42)
        supervisor.stage_job()
        with self.assertRaises(RuntimeError):
            supervisor.start_worker()
        self.assertEqual(supervisor.bind_worker(42), 81)
        supervisor.start_worker()
        sent = []
        ack = supervisor.accept_offer(OFFER, lambda message: sent.append(message) or True)
        self.assertEqual(sent, [ack])
        self.assertEqual(ack.occurrence, OFFER.occurrence)
        self.assertEqual(supervisor.ack_transmission, "sent")
        self.assertEqual(supervisor.ack_receipt, "not-received")
        supervisor.record_ack_receipt(ack)
        self.assertEqual(supervisor.ack_receipt, "matching-unverified-claim")
        report = supervisor.cleanup(wait_ms=37)
        self.assertEqual(report.job_termination, "succeeded")
        self.assertEqual(report.child_wait, "exited")
        self.assertEqual(report.worker_wait, "exited")
        self.assertTrue(report.active_processes_zero)
        self.assertEqual(report.handle_closes,
                         {"child": "succeeded", "worker": "succeeded", "job": "succeeded"})
        self.assertEqual(report.errors, [])
        names = [call[0] for call in api.calls]
        self.assertLess(names.index("set-job"), names.index("duplicate"))
        self.assertEqual(api.calls[3], ("duplicate", 99, 71, 42, False, 2))
        self.assertEqual(api.calls[4], ("duplicate", 42, 202, 99, False, 2))
        self.assertLess(names.index("image"), names.index("terminate-job"))
        self.assertEqual(names[-3:], ["close", "close", "close"])
        retry = supervisor.cleanup()
        self.assertEqual(retry.retained_handles, {})
        self.assertEqual(len([c for c in api.calls if c[0] == "close"]), 3)

    def test_refuses_mismatched_identity_without_ack_and_closes_exact_duplicate(self):
        for failure in ("pid-mismatch", "time-mismatch", "image-mismatch"):
            with self.subTest(failure=failure):
                api = FakeApi(fail={failure})
                supervisor = started(api)
                sent = []
                with self.assertRaises(RuntimeError):
                    supervisor.accept_offer(OFFER, lambda ack: sent.append(ack) or True)
                self.assertEqual(sent, [])
                report = supervisor.cleanup()
                self.assertEqual(report.child_identity, "unknown even if job accounting reaches zero")
                self.assertEqual(report.handle_closes["child"], "succeeded")

    def test_rejects_bad_offer_without_pid_reopen_or_ack(self):
        api = FakeApi()
        supervisor = started(api)
        with self.assertRaises(ValueError):
            supervisor.accept_offer(m.ChildOffer("", 202, 123, 124, 456, IMAGE.executable),
                                    lambda ack: True)
        self.assertFalse(any(call[0] == "pid" for call in api.calls))
        self.assertEqual(supervisor.cleanup().child_identity,
                         "unknown even if job accounting reaches zero")

    def test_duplicate_failure_has_no_local_child_and_no_ack(self):
        api = FakeApi(fail={"duplicate-child"})
        supervisor = started(api)
        with self.assertRaises(creation.ApiFailure):
            supervisor.accept_offer(OFFER, lambda ack: True)
        report = supervisor.cleanup()
        self.assertEqual(report.child_wait, "no-exact-child-handle")
        self.assertNotIn("child", report.handle_closes)
        self.assertEqual(report.child_identity, "unknown even if job accounting reaches zero")

    def test_worker_job_duplicate_failure_still_cleans_owned_worker(self):
        class NoJobDuplicate(FakeApi):
            def DuplicateHandle(self, source, handle, target, output, access, inherit, options):
                if handle == 71:
                    self.calls.append(("duplicate-job-failed",))
                    return False
                return super().DuplicateHandle(source, handle, target, output,
                                               access, inherit, options)
        api = NoJobDuplicate()
        supervisor = m.FakeJobSupervisor(api, IMAGE)
        supervisor.stage_job()
        with self.assertRaises(creation.ApiFailure):
            supervisor.bind_worker(42)
        with self.assertRaises(RuntimeError):
            supervisor.start_worker()
        report = supervisor.cleanup()
        self.assertEqual(report.handle_closes,
                         {"worker": "succeeded", "job": "succeeded"})
        self.assertEqual(report.child_identity, "unknown")

    def test_start_worker_has_no_callback_or_builtin_child_call(self):
        supervisor = m.FakeJobSupervisor(FakeApi(), IMAGE)
        supervisor.stage_job()
        supervisor.bind_worker(42)
        with self.assertRaises(TypeError):
            supervisor.start_worker(lambda job: None)
        supervisor.start_worker()
        report = supervisor.cleanup()
        self.assertEqual(report.child_identity, "unknown even if job accounting reaches zero")

    def test_interrupted_create_and_zero_accounting_do_not_identify_child(self):
        api = FakeApi(active=0)
        supervisor = started(api)
        supervisor.note_create_interrupted()
        report = supervisor.cleanup()
        self.assertTrue(report.active_processes_zero)
        self.assertEqual(report.child_identity, "unknown even if job accounting reaches zero")
        self.assertEqual(report.child_wait, "no-exact-child-handle")
        self.assertEqual(report.worker_termination, "succeeded")
        self.assertNotIn("child", report.handle_closes)

    def test_ack_send_outcome_and_receipt_are_distinct(self):
        api = FakeApi()
        supervisor = started(api)
        def uncertain(_ack):
            raise OSError("link broke after possible send")
        with self.assertRaises(OSError):
            supervisor.accept_offer(OFFER, uncertain)
        self.assertEqual(supervisor.ack_transmission, "attempted; outcome unknown")
        with self.assertRaises(RuntimeError):
            supervisor.record_ack_receipt(supervisor.ack)
        supervisor.cleanup()

    def test_mismatched_receipt_is_refused(self):
        supervisor = started(FakeApi())
        ack = supervisor.accept_offer(OFFER, lambda message: True)
        with self.assertRaises(RuntimeError):
            supervisor.record_ack_receipt(m.ChildAck("different", ack.pid, ack.tid,
                                                    ack.creation_filetime, ack.image))
        self.assertEqual(supervisor.ack_receipt, "not-received")
        supervisor.cleanup()

    def test_cleanup_reports_timeouts_failures_and_handle_close_uncertainty(self):
        api = FakeApi(fail={"terminate-job", "child-timeout", "worker-timeout",
                            "terminate-worker", "close-child"}, active=1)
        supervisor = started(api)
        supervisor.accept_offer(OFFER, lambda ack: True)
        report = supervisor.cleanup(wait_ms=17)
        self.assertEqual(report.job_termination, "failed or unknown")
        self.assertEqual(report.child_wait, "timeout")
        self.assertEqual(report.worker_termination, "failed or unknown")
        self.assertEqual(report.worker_wait, "timeout")
        self.assertFalse(report.active_processes_zero)
        self.assertEqual(report.handle_closes["child"], "deferred; exact handle retained")
        self.assertEqual(report.retained_handles, {"child": 303, "worker": 42, "job": 71})
        self.assertTrue(report.errors)
        self.assertFalse(any(c[0] == "close" for c in api.calls))
        api.fail = {"close-child"}
        api.active = 0
        retry = supervisor.cleanup(wait_ms=17)
        self.assertEqual(retry.child_wait, "exited")
        self.assertEqual(retry.worker_wait, "exited")
        self.assertEqual(retry.handle_closes["child"], "failed or unknown")
        self.assertEqual(retry.retained_handles, {"child": 303, "job": 71})
        self.assertEqual(api.calls[-2:], [("close", 303), ("close", 42)])
        api.fail.clear()
        final = supervisor.cleanup(wait_ms=17)
        self.assertEqual(final.retained_handles, {})
        self.assertEqual(final.handle_closes, {"child": "succeeded", "job": "succeeded"})
        self.assertEqual(api.calls[-2:], [("close", 303), ("close", 71)])

    def test_unknown_accounting_and_failed_worker_job_close_are_retryable(self):
        api = FakeApi(fail={"close-worker", "close-job"}, active=None)
        supervisor = started(api)
        first = supervisor.cleanup()
        self.assertIsNone(first.active_processes)
        self.assertFalse(first.active_processes_zero)
        self.assertEqual(first.retained_handles, {"worker": 42, "job": 71})
        self.assertEqual(first.handle_closes["worker"], "failed or unknown")
        self.assertEqual(first.handle_closes["job"], "deferred; exact handle retained")
        api.active = 0
        second = supervisor.cleanup()
        self.assertEqual(second.retained_handles, {"worker": 42, "job": 71})
        self.assertEqual(second.handle_closes["worker"], "failed or unknown")
        api.fail.remove("close-worker")
        third = supervisor.cleanup()
        self.assertEqual(third.retained_handles, {"job": 71})
        self.assertEqual(third.handle_closes["job"], "failed or unknown")
        api.fail.clear()
        fourth = supervisor.cleanup()
        self.assertEqual(fourth.retained_handles, {})
        self.assertEqual(fourth.handle_closes["job"], "succeeded")

    def test_real_entry_and_unmarked_api_hold(self):
        with self.assertRaises(RuntimeError):
            m.FakeJobSupervisor(object(), IMAGE)
        with self.assertRaises(RuntimeError):
            m.run_real_job_supervisor()


if __name__ == "__main__":
    unittest.main()
