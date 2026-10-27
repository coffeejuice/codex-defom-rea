"""CP323 pure fake integration tests: no native executable, DLL, GUI or I/O."""
import ctypes as C
from dataclasses import replace
import hashlib
from pathlib import Path
import sys
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp323_fixed_job_token as m
from test_cp313_native_worker_creator import FakeApi
from test_cp318_fixed_worker_creator import Endpoints
from test_cp320_retained_image_pin import FakeApi as ImageApi, DATA


class Tests(unittest.TestCase):
    def setUp(self):
        self.digest = patch.object(m.image, "WORKER_SHA256", hashlib.sha256(DATA).hexdigest())
        self.digest.start()
        self.addCleanup(self.digest.stop)
        self.api = FakeApi()
        self.api.cp323_fake_api = True
        self.api.image = m.WORKER_IMAGE
        self.images = ImageApi()
        self.pin = m.image.FakeRetainedImagePin(self.images, last_error=lambda: 5)
        self.owner = Endpoints()
        self.owner.retained = False
        ends = tuple(self.api._new(v) for v in (True, True, False, False))
        self.endpoints = m.EndpointPair(m.fixed.pipes.prepare(clock=lambda: 10),
                                       *ends, self.owner, 15000)
        self.tick = 10000
        self.creator = m.FakeWorkerCreator(self.api, last_error=self.api.last_error,
            image_pin=self.pin, endpoints=self.endpoints, tick_ms=lambda: self.tick)

    def names(self):
        return [n for n, _ in self.api.calls]

    def failure(self):
        with self.assertRaises(m.PreparationFailure):
            self.creator.prepare()
        self.assertNotIn("ResumeThread", self.names())
        self.assertNotIn("CreatePipe", self.names())
        return self.creator.receipt

    def test_exact_suspended_containment_and_token(self):
        r = self.creator.prepare()
        self.assertEqual(r.phase, "prepared-suspended")
        self.assertEqual(self.api.attr, [
            (m.base.PROC_THREAD_ATTRIBUTE_JOB_LIST, (r.local["worker_job"],)),
            (m.base.PROC_THREAD_ATTRIBUTE_HANDLE_LIST,
             (self.endpoints.worker_read, self.endpoints.worker_write))])
        self.assertEqual(len(self.api.job_flags), 2)
        self.assertEqual(set(self.api.job_flags.values()), {m.base.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE})
        self.assertEqual(self.api.create_args[1].value, m.WORKER_IMAGE + " 15000")
        self.assertEqual(self.api.create_args[5], m.base.CREATE_FLAGS)
        self.assertEqual(self.api.create_args[7], m.WORKER_CWD)
        self.assertTrue(self.pin.post_create_verified)
        duplicate = [a for n, a in self.api.calls if n == "DuplicateHandle"]
        self.assertEqual(len(duplicate), 1)
        a = duplicate[0]
        self.assertEqual((a[1], a[2], a[4], a[5], a[6]),
            (r.local["child_job"], r.local["worker_process"], 0, False, m.base.DUPLICATE_SAME_ACCESS))
        self.assertEqual(r.remote_child_job, m.base.RemoteHandle(909, r.local["worker_process"]))
        for name in ("worker_job", "child_job"):
            self.assertEqual(self.api.flags[r.local[name]], 0)
        self.assertNotIn("ResumeThread", self.names())
        with self.assertRaises(m.Hold):
            self.creator.prepare()

    def test_image_pin_order_and_postcreate_failure(self):
        create = self.api.CreateProcessW.callback
        def callback(*args):
            self.assertIsNotNone(self.pin.snapshot)
            self.assertFalse(self.pin.post_create_verified)
            result = create(*args)
            self.images.reopen_id = 99
            return result
        self.api.CreateProcessW.callback = callback
        r = self.failure()
        self.assertEqual(r.create_outcome, "returned-success")
        self.assertNotIn("DuplicateHandle", self.names())
        self.assertIsNotNone(self.pin.handle)
        self.assertEqual(self.creator.close().phase, "closed")

    def test_live_and_unmarked_hold(self):
        with self.assertRaisesRegex(m.Hold, "CP321.*CP322"):
            m.create_live_worker()
        self.api.cp323_fake_api = False
        with self.assertRaises(m.Hold):
            m.FakeWorkerCreator(self.api, last_error=self.api.last_error,
                image_pin=self.pin, endpoints=self.endpoints, tick_ms=lambda: self.tick)
        self.assertFalse(self.api.calls)

    def test_fake_marked_native_export_is_rejected(self):
        # This synthetic callback is a ctypes export-shaped pointer, never called.
        self.api.CreateJobObjectW = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_void_p)(lambda *_: 1)
        with self.assertRaisesRegex(m.Hold, "DLL exports"):
            m.FakeWorkerCreator(self.api, last_error=self.api.last_error,
                image_pin=self.pin, endpoints=self.endpoints, tick_ms=lambda: self.tick)

    def test_tick_deadline_only_and_exhaustion(self):
        self.creator.endpoints = replace(self.endpoints,
            preparation=replace(self.endpoints.preparation, deadline=float("nan")))
        self.creator.prepare()  # no unrelated clock conversion or allowance
        self.assertEqual(self.creator._deadline, 15000)
        self.assertEqual(self.creator.close().phase, "closed")

    def test_expired_and_bad_tick_types_fail_before_image(self):
        for tick in (15000, 9999, True, -1, 2**64):
            with self.subTest(tick=tick):
                self.setUp()
                self.tick = tick
                self.failure()
                self.assertFalse(self.images.calls)

    def test_deadline_never_renewed_or_clock_reversed(self):
        create = self.api.CreateProcessW.callback
        def callback(*args):
            result = create(*args)
            self.creator.endpoints = replace(self.endpoints, deadline_tick_ms=16000)
            return result
        self.api.CreateProcessW.callback = callback
        self.failure()
        self.assertNotIn("DuplicateHandle", self.names())

    def test_create_partial_return_and_interruption_preserved(self):
        self.api.fail, self.api.fail_create_with_partial = "CreateProcessW", True
        r = self.failure()
        self.assertIn("worker_process", r.local)
        self.assertIn("worker_thread", r.local)
        self.assertEqual(r.create_outcome, "unknown: CreateProcessW entered")
        self.assertEqual(self.creator.close().phase, "closed")

    def test_interruption_only_thread_output(self):
        def callback(*args):
            info = C.cast(args[9], C.POINTER(m.base.PROCESS_INFORMATION))[0]
            info.hThread = self.api._new()
            raise RuntimeError("partial thread")
        self.api.CreateProcessW.callback = callback
        r = self.failure()
        self.assertIn("worker_thread", r.local)
        self.assertNotIn("worker_process", r.local)
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertTrue(r.worker_terminated)
        self.assertIsNotNone(self.pin.handle)
        self.assertFalse(r.closed)

    def test_duplicate_partial_failure_retains_remote_not_local(self):
        self.api.fail = "DuplicateHandle"
        r = self.failure()
        self.assertEqual(r.duplicate_outcome, "unknown: DuplicateHandle entered")
        self.assertEqual(r.remote_child_job.value, 909)
        self.assertFalse(r.closed)
        self.assertEqual(self.creator.close().phase, "closed")
        self.assertIsNone(r.remote_child_job)
        self.assertNotIn(909, [a[0] for n, a in self.api.calls if n == "CloseHandle"])

    def test_duplicate_interrupted_after_output(self):
        duplicate = self.api.DuplicateHandle.callback
        def callback(*args):
            duplicate(*args)
            raise RuntimeError("partial duplicate")
        self.api.DuplicateHandle.callback = callback
        r = self.failure()
        self.assertEqual(r.remote_child_job.owner_process, r.local["worker_process"])

    def test_zero_duplicate_is_not_positive_token(self):
        self.api.DuplicateHandle.callback = lambda *_: 1
        r = self.failure()
        self.assertIsNone(r.remote_child_job)

    def test_membership_child_activity_and_pid_fail_closed(self):
        self.api.membership["child"] = True
        self.failure()
        self.assertNotIn("DuplicateHandle", self.names())

    def test_child_initially_nonempty_fails_before_create(self):
        self.api.active_count["child"] = 1
        self.failure()
        self.assertNotIn("CreateProcessW", self.names())
        self.assertEqual(self.creator.receipt.phase, "closed")

    def test_cleanup_terminates_before_pending_io_gate(self):
        r = self.creator.prepare()
        self.owner.retained = True
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertTrue(r.worker_terminated and r.worker_waited and r.jobs_empty)
        self.assertEqual(self.names().count("TerminateJobObject"), 1)
        self.assertFalse(r.closed)
        self.assertIsNotNone(self.pin.handle)
        self.owner.retained = False
        self.assertEqual(self.creator.close().phase, "closed")
        self.assertEqual(self.names().count("TerminateJobObject"), 1)
        closed = [a[0] for n, a in self.api.calls if n == "CloseHandle"]
        self.assertTrue(set(closed).isdisjoint((self.endpoints.worker_read,
            self.endpoints.worker_write, self.endpoints.supervisor_write, self.endpoints.supervisor_read)))

    def test_wait_jobs_and_close_retries_retain_until_proven(self):
        r = self.creator.prepare()
        self.api.wait_status = m.base.WAIT_TIMEOUT
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertIsNotNone(r.remote_child_job)
        self.api.wait_status = m.base.WAIT_OBJECT_0
        self.api.active_count["child"] = 1
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertFalse(r.closed)
        self.api.active_count["child"] = 0
        self.api.close_fail = r.local["worker_process"]
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertNotIn("worker_job", r.local)
        self.api.close_fail = None
        self.assertEqual(self.creator.close().phase, "closed")
        before = (len(self.api.calls), len(self.images.calls))
        self.assertEqual(self.creator.close().phase, "closed")
        self.assertEqual((len(self.api.calls), len(self.images.calls)), before)

    def test_pin_close_failure_retains_then_exact_retry(self):
        self.creator.prepare()
        self.images.close_fail = True
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertIsNotNone(self.pin.handle)
        self.images.close_fail = False
        count = len(self.api.calls)
        self.assertEqual(self.creator.close().phase, "closed")
        self.assertEqual(len(self.api.calls), count)

    def test_invalid_process_output_still_retains_valid_thread(self):
        def callback(*args):
            info = C.cast(args[9], C.POINTER(m.base.PROCESS_INFORMATION))[0]
            info.hProcess = C.c_void_p(-1).value
            info.hThread = self.api._new()
            info.dwProcessId, info.dwThreadId = 42, 43
            return 1
        self.api.CreateProcessW.callback = callback
        r = self.failure()
        self.assertIn("worker_thread", r.local)
        self.assertNotIn("worker_process", r.local)
        self.assertEqual((r.pid, r.tid), (42, 43))
        self.assertEqual(self.creator.close().phase, "hold")

    def test_backward_tick_after_create_fails_before_duplicate(self):
        create = self.api.CreateProcessW.callback
        def callback(*args):
            value = create(*args)
            self.tick = 9999
            return value
        self.api.CreateProcessW.callback = callback
        self.failure()
        self.assertNotIn("DuplicateHandle", self.names())

    def test_missing_create_outputs_retain_job_and_pin_after_termination(self):
        self.api.fail = "CreateProcessW"
        self.failure()
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertTrue(self.creator.receipt.worker_terminated)
        self.assertFalse(self.creator.receipt.closed)
        self.assertIsNotNone(self.pin.handle)

    def test_pid_and_path_failures_never_duplicate(self):
        self.api.GetProcessId.callback = lambda *_: 0
        self.failure()
        self.assertNotIn("DuplicateHandle", self.names())

    def test_wrong_image_path_never_duplicates(self):
        self.api.image = r"C:\other.exe"
        self.failure()
        self.assertNotIn("DuplicateHandle", self.names())

    def test_failed_endpoint_inheritance_and_image_prepare_fail_before_create(self):
        self.api.flags[self.endpoints.supervisor_read] = m.base.HANDLE_FLAG_INHERIT
        self.failure()
        self.assertFalse(self.images.calls)
        self.assertNotIn("CreateProcessW", self.names())

    def test_image_prepare_failure_closes_partial_pin_without_creating(self):
        self.images.data = b"X" * len(DATA)
        self.failure()
        self.assertIsNone(self.pin.handle)
        self.assertNotIn("CreateProcessW", self.names())

    def test_child_changes_to_nonempty_after_create(self):
        create = self.api.CreateProcessW.callback
        def callback(*args):
            value = create(*args)
            self.api.active_count["child"] = 1
            return value
        self.api.CreateProcessW.callback = callback
        self.failure()
        self.assertNotIn("DuplicateHandle", self.names())
        self.assertEqual(self.creator.close().phase, "hold")

    def test_accounting_exception_retries_after_wait_without_retermination(self):
        r = self.creator.prepare()
        self.api.accounting_raise = True
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertTrue(r.worker_waited)
        self.assertFalse(r.closed)
        self.api.accounting_raise = False
        self.assertEqual(self.creator.close().phase, "closed")
        self.assertEqual(self.names().count("TerminateJobObject"), 1)
        self.assertEqual(self.names().count("WaitForSingleObject"), 1)
        self.owner.retained = True
        self.assertEqual(self.creator.close().phase, "closed")

    def test_termination_failure_cannot_close_or_wait(self):
        r = self.creator.prepare()
        self.api.fail = "TerminateJobObject"
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertFalse(r.worker_terminated)
        self.assertNotIn("WaitForSingleObject", self.names())
        self.assertFalse(r.closed)
        self.api.fail = None
        self.assertEqual(self.creator.close().phase, "closed")


if __name__ == "__main__":
    unittest.main()
