"""CP318 injected fake tests. No file open, process, DLL, build or GUI."""
import ctypes as C
from dataclasses import replace
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp318_fixed_worker_creator as m
from test_cp313_native_worker_creator import FakeApi


class Images:
    cp318_fake_image_api = True

    def __init__(self):
        self.pin = m.FakeBuildReceipt(m.WORKER_SHA256, m.WORKER_BYTES, (2, 16))
        self.calls = []
        self.changed = False
        self.close_fail = False

    def open_locked(self, path, **shares):
        self.calls.append(("open", path, shares))
        return 9001

    def snapshot(self, handle):
        self.calls.append(("snapshot", handle))
        return ("b" * 64 if self.changed else self.pin.sha256,
                self.pin.size, self.pin.identity)

    def close(self, handle):
        self.calls.append(("close", handle))
        if self.close_fail:
            raise RuntimeError("close uncertain")


class Endpoints:
    cp318_fake_endpoint_owner = True
    retained = False

    def require_releasable(self):
        if self.retained:
            raise m.Hold("pending pipe operation retains ownership")


class CreatorTests(unittest.TestCase):
    def setUp(self):
        self.api = FakeApi()
        self.api.cp318_fake_api = True
        self.api.image = m.WORKER_IMAGE
        self.images = Images()
        self.owner = Endpoints()
        handles = tuple(self.api._new(v) for v in (True, True, False, False))
        self.endpoints = m.EndpointPair(m.pipes.prepare(clock=lambda: 10),
                                       *handles, self.owner, 15000)
        self.clock = 10
        self.ticks = 10000
        self.creator = self.make()

    def make(self, **kwargs):
        args = dict(last_error=self.api.last_error, image_api=self.images,
                    build_receipt=self.images.pin, endpoints=self.endpoints,
                    clock=lambda: self.clock, tick_ms=lambda: self.ticks)
        args.update(kwargs)
        return m.FakeWorkerCreator(self.api, **args)

    def test_fixed_path_and_no_live_entry(self):
        self.assertEqual(m.WORKER_SHA256, "19886a0821d47611cc82dc44476ae222328e9c169b8e8ba9183cf92c3b655985")
        with self.assertRaises(m.Hold):
            m.create_live_worker()
        for path in (r"C:\PROBLEM\s312\worker.exe", r"C:\other.exe"):
            with self.assertRaises(m.Hold):
                self.make(build_receipt=replace(self.images.pin, image=path))
        with self.assertRaises(TypeError):
            self.make(image_path=r"C:\other.exe")

    def test_reviewed_build_hash_size_receipt_are_exact(self):
        for pin in (replace(self.images.pin, sha256="a" * 64),
                    replace(self.images.pin, size=m.WORKER_BYTES + 1),
                    replace(self.images.pin, receipt="other.json"),
                    replace(self.images.pin, receipt_sha256="b" * 64)):
            with self.assertRaises(m.Hold):
                self.make(build_receipt=pin)

    def test_exact_attributes_command_suspended_and_retained_image(self):
        r = self.creator.prepare()
        self.assertEqual(r.phase, "prepared-suspended")
        self.assertEqual(self.api.attr, [
            (m.base.PROC_THREAD_ATTRIBUTE_JOB_LIST, (r.local["worker_job"],)),
            (m.base.PROC_THREAD_ATTRIBUTE_HANDLE_LIST,
             (self.endpoints.worker_read, self.endpoints.worker_write))])
        args = self.api.create_args
        self.assertEqual(args[0], m.WORKER_IMAGE)
        self.assertEqual(args[1].value, m.WORKER_IMAGE + " 15000")
        self.assertEqual(args[5], m.base.CREATE_FLAGS)
        self.assertTrue(args[5] & m.base.CREATE_SUSPENDED)
        self.assertEqual(args[7], m.WORKER_CWD)
        self.assertEqual(len(self.api.job_flags), 1)
        names = [name for name, _ in self.api.calls]
        self.assertNotIn("CreatePipe", names)
        self.assertNotIn("DuplicateHandle", names)
        self.assertEqual(self.images.calls[0],
                         ("open", m.WORKER_IMAGE, {"share_write": False, "share_delete": False}))
        self.assertEqual(self.creator.image_handle, 9001)
        self.assertNotIn(("close", 9001), self.images.calls)

    def test_deadline_is_precreate_and_never_renewed(self):
        self.clock, self.ticks = 15, 15000
        with self.assertRaises(m.PreparationFailure):
            self.creator.prepare()
        self.assertFalse(self.images.calls)
        self.assertNotIn("CreateProcessW", [n for n, _ in self.api.calls])
        with self.assertRaises(m.Hold):
            self.creator.prepare()

    def test_retain_hash_identity_through_creation(self):
        create = self.api.CreateProcessW.callback

        def change(*args):
            value = create(*args)
            self.images.changed = True
            return value

        self.api.CreateProcessW.callback = change
        with self.assertRaises(m.PreparationFailure):
            self.creator.prepare()
        self.assertEqual(self.creator.image_handle, 9001)
        self.assertIn("worker_process", self.creator.receipt.local)

    def test_deadline_clock_binding_cannot_add_new_allowance(self):
        self.ticks = 12000  # still individually in budget, but not same deadline
        with self.assertRaises(m.PreparationFailure):
            self.creator.prepare()
        self.assertFalse(self.images.calls)

    def test_worker_process_image_mismatch_is_hold(self):
        self.api.image = r"C:\PROBLEM\s312\worker.exe"
        with self.assertRaises(m.PreparationFailure):
            self.creator.prepare()
        self.assertFalse(self.creator.receipt.closed)

    def test_create_interruption_keeps_partial_outputs(self):
        create = self.api.CreateProcessW.callback

        def interrupted(*args):
            create(*args)
            C.cast(args[9], C.POINTER(m.base.PROCESS_INFORMATION))[0].hThread = None
            raise RuntimeError("interrupted after process handle output")

        self.api.CreateProcessW.callback = interrupted
        with self.assertRaises(m.PreparationFailure):
            self.creator.prepare()
        self.assertIn("worker_process", self.creator.receipt.local)
        self.assertNotIn("worker_thread", self.creator.receipt.local)
        self.assertEqual(self.creator.image_handle, 9001)

    def test_membership_failure_holds_without_early_close(self):
        self.api.membership["worker"] = False
        with self.assertRaises(m.PreparationFailure):
            self.creator.prepare()
        self.assertIn("worker_job", self.creator.receipt.local)
        self.assertFalse(self.creator.receipt.closed)

    def test_partial_creation_output_retained_on_error(self):
        self.api.fail = "CreateProcessW"
        self.api.fail_create_with_partial = True
        with self.assertRaises(m.PreparationFailure):
            self.creator.prepare()
        r = self.creator.receipt
        self.assertEqual(r.create_outcome, "unknown: CreateProcessW entered")
        self.assertIn("worker_process", r.local)
        self.assertEqual(self.creator.image_handle, 9001)
        self.assertEqual(self.creator.close().phase, "closed")

    def test_unknown_create_without_process_retains_everything(self):
        self.api.fail = "CreateProcessW"
        with self.assertRaises(m.PreparationFailure):
            self.creator.prepare()
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertIn("worker_job", self.creator.receipt.local)
        self.assertEqual(self.creator.image_handle, 9001)

    def test_wait_accounting_and_pending_io_retain_cleanup(self):
        self.creator.prepare()
        self.api.wait_status = m.base.WAIT_TIMEOUT
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertFalse(self.creator.receipt.closed)
        self.api.wait_status = m.base.WAIT_OBJECT_0
        self.api.active_count["worker"] = 1
        self.assertEqual(self.creator.close().phase, "hold")
        self.api.active_count["worker"] = 0
        self.owner.retained = True
        self.assertEqual(self.creator.close().phase, "hold")
        self.owner.retained = False
        self.assertEqual(self.creator.close().phase, "closed")
        closed = [args[0] for name, args in self.api.calls if name == "CloseHandle"]
        self.assertTrue(set(closed).isdisjoint((self.endpoints.worker_read,
            self.endpoints.worker_write, self.endpoints.supervisor_read,
            self.endpoints.supervisor_write)))

    def test_bad_endpoint_type_and_inheritance_hold(self):
        self.creator.endpoints = object()
        with self.assertRaises(m.PreparationFailure):
            self.creator.prepare()
        creator = self.make()
        self.api.flags[self.endpoints.supervisor_read] = m.base.HANDLE_FLAG_INHERIT
        with self.assertRaises(m.PreparationFailure):
            creator.prepare()

    def test_close_failure_keeps_image_owned(self):
        self.creator.prepare()
        self.api.close_fail = self.creator.receipt.local["worker_process"]
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertEqual(self.creator.image_handle, 9001)
        self.api.close_fail = None
        self.images.close_fail = True
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertEqual(self.creator.image_handle, 9001)

    def test_cleanup_retry_after_job_closed_and_process_close_failure(self):
        self.creator.prepare()
        process = self.creator.receipt.local["worker_process"]
        self.api.close_fail = process
        self.assertEqual(self.creator.close().phase, "hold")
        self.assertNotIn("worker_job", self.creator.receipt.local)
        self.assertTrue(self.creator.receipt.worker_waited)
        self.assertTrue(self.creator.receipt.jobs_empty)
        self.api.close_fail = None
        self.assertEqual(self.creator.close().phase, "closed")
        calls = len(self.api.calls)
        image_calls = len(self.images.calls)
        self.assertEqual(self.creator.close().phase, "closed")
        self.assertEqual(len(self.api.calls), calls)
        self.assertEqual(len(self.images.calls), image_calls)


if __name__ == "__main__":
    unittest.main()
