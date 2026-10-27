"""CP329 injected BoundExports tests: no DLL loading or native invocation."""
import ctypes as C
from pathlib import Path
import sys
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp329_native_worker_creator as m
from cp325_real_bindings import BoundExports


def out(pointer, kind, value):
    C.cast(pointer, C.POINTER(kind)).contents.value = value


class Harness:
    def __init__(self):
        self.calls, self.attributes, self.closed = [], {}, []
        self.now, self.error, self.next_job = 100, 122, 20
        self.created = self.terminated = False
        self.wait, self.child_count = 0, 0
        self.bad_pid = self.bad_path = self.bad_membership = False
        self.create_false = self.create_raise = self.create_no_outputs = False
        self.duplicate_false = self.duplicate_raise = False
        self.close_fail = self.fail = None
        self.bad_return_size = self.bad_job_flags = False
        self.attr_init_raise = False
        functions = {name: (lambda *args, name=name: self.dispatch(name, *args))
                     for name in m.abi.EXPORTS}
        self.exports = BoundExports(functions, {}, lambda: self.error)
        self.security = Security(self)
        self.pipes = Pipes(self)
        self.pin = Pin(self)
        self.owner = m.NativeWorkerCreator(self.exports, security=self.security,
            pipe_owner=self.pipes, image_pin=self.pin, deadline=1000)

    def dispatch(self, name, *args):
        self.calls.append(name)
        if name == self.fail:
            self.error = 5
            return 0
        if name == "GetTickCount64":
            return self.now
        if name == "GetHandleInformation":
            out(args[1], m.abi.DWORD, int(args[0] in (11, 13)))
        elif name == "CreateJobObjectW":
            self.next_job += 1
            return self.next_job
        elif name == "QueryInformationJobObject":
            handle, kind, pointer, size, returned = args
            out(returned, m.abi.DWORD, size - int(self.bad_return_size))
            if kind == m.JOB_LIMITS:
                obj = C.cast(pointer, C.POINTER(m.abi.EXTENDED_LIMIT)).contents
                obj.BasicLimitInformation.LimitFlags = m.KILL_ON_CLOSE | (0x1000 if self.bad_job_flags else 0)
            else:
                obj = C.cast(pointer, C.POINTER(m.abi.BASIC_ACCOUNTING)).contents
                obj.ActiveProcesses = (int(self.created and not self.terminated)
                                       if handle == 21 else self.child_count)
        elif name == "InitializeProcThreadAttributeList":
            if args[0] is None:
                out(args[3], m.abi.SIZE_T, 256)
                self.error = 122
                return 0
            if self.attr_init_raise:
                raise RuntimeError("init output uncertain")
        elif name == "UpdateProcThreadAttribute":
            self.attributes[args[2]] = tuple(C.cast(args[3], C.POINTER(m.abi.HANDLE))[i]
                                           for i in range(args[4] // C.sizeof(m.abi.HANDLE)))
        elif name == "CreateProcessW":
            self.command, self.creation = args[1].value, args
            self.created = True
            if not self.create_no_outputs:
                info = C.cast(args[9], C.POINTER(m.abi.PROCESS_INFORMATION)).contents
                info.hProcess, info.hThread, info.dwProcessId, info.dwThreadId = 31, 32, 33, 34
            if self.create_raise:
                raise RuntimeError("create uncertain")
            if self.create_false:
                self.error = 5
                return 0
        elif name == "GetProcessId":
            return 99 if self.bad_pid else 33
        elif name == "QueryFullProcessImageNameW":
            path = r"C:\wrong.exe" if self.bad_path else m.WORKER_IMAGE
            args[2].value = path
            out(args[3], m.abi.DWORD, len(path))
        elif name == "IsProcessInJob":
            member = args[1] == 21
            out(args[2], m.abi.BOOL, int(not member if self.bad_membership else member))
        elif name == "GetCurrentProcess":
            return m.INVALID_HANDLE
        elif name == "DuplicateHandle":
            self.dup_args = args
            out(args[3], m.abi.HANDLE, 44)
            if self.duplicate_raise:
                raise RuntimeError("duplicate uncertain")
            if self.duplicate_false:
                self.error = 5
                return 0
        elif name == "TerminateJobObject":
            self.terminated = True
        elif name == "WaitForSingleObject":
            assert args == (31, 0)
            return self.wait
        elif name == "CloseHandle":
            if args[0] == self.close_fail:
                self.error = 5
                return 0
            self.closed.append(args[0])
        elif name == "ResumeThread":
            raise AssertionError("forbidden resume")
        return 1


class Security:
    def __init__(self, h):
        self.h, self.exports, self.started = h, h.exports, False

    def prepare(self):
        self.h.calls.append("security.prepare")
        self.started = True

    def cleanup(self):
        self.h.calls.append("security.cleanup")
        return True


class Pipes:
    def __init__(self, h):
        self.h, self.exports, self.security = h, h.exports, h.security
        self.deadline, self.started, self.ready = 1000, False, True

    def prepare(self, name):
        self.started = True
        self.h.calls.append("pipes.prepare")
        return m.pipes.ConnectedEndpoints(10, 11, 12, 13)

    def cleanup(self):
        self.h.calls.append("pipes.cleanup")
        return self.ready


class Pin:
    def __init__(self, h):
        self.h, self.exports, self._used = h, h.exports, False
        self.handle = self.reopened_handle = None
        self.fail_verify = self.fail_close = False

    def prepare(self):
        self.h.calls.append("pin.prepare")
        self._used, self.handle = True, 50

    def verify_after_create(self):
        self.h.calls.append("pin.verify")
        if self.fail_verify:
            raise m.Hold("image changed")

    def close(self):
        self.h.calls.append("pin.close")
        if self.fail_close:
            raise m.Hold("pin close failed")
        self.handle = None


class CreatorTests(unittest.TestCase):
    def completion_harness(self, *, status=0, exit_code=0, worker_count=0):
        h = Harness()
        self.prepare(h)
        h.pipes.operations = []
        dispatch = h.dispatch

        def complete_dispatch(name, *args):
            if name == "WaitForSingleObject":
                h.calls.append(name)
                self.assertEqual(args, (31, 900))
                return status
            if name == "GetExitCodeProcess":
                h.calls.append(name)
                self.assertEqual(args[0], 31)
                out(args[1], m.abi.DWORD, exit_code)
                return 1
            if name == "QueryInformationJobObject" and args[1] == m.JOB_ACCOUNTING:
                result = dispatch(name, *args)
                obj = C.cast(args[2], C.POINTER(m.abi.BASIC_ACCOUNTING)).contents
                if args[0] == 21:
                    obj.ActiveProcesses = worker_count
                return result
            return dispatch(name, *args)

        h.dispatch = complete_dispatch
        return h

    def test_success_completion_owns_exit_and_cleanup_evidence(self):
        h = self.completion_harness()
        self.assertEqual(h.owner.complete_success().phase, "closed")
        self.assertTrue(h.owner.receipt.worker_waited)
        self.assertTrue(h.owner.receipt.jobs_empty)
        self.assertFalse(h.owner.receipt.worker_terminated)
        self.assertIsNone(h.owner.receipt.remote_child_job)
        self.assertNotIn("TerminateJobObject", h.calls)
        self.assertEqual(h.owner.receipt.local, {})

    def test_completion_refuses_timeout_nonzero_exit_jobs_or_pending_io(self):
        for mode in ("timeout", "bad-status", "exit", "worker-job", "child-job", "pending"):
            with self.subTest(mode=mode):
                h = self.completion_harness(status=258 if mode == "timeout" else
                    (True if mode == "bad-status" else 0),
                    exit_code=8 if mode == "exit" else 0,
                    worker_count=int(mode == "worker-job"))
                h.child_count = int(mode == "child-job")
                if mode == "pending":
                    from types import SimpleNamespace
                    h.pipes.operations.append(SimpleNamespace(terminal=False))
                with self.assertRaises(m.Hold):
                    h.owner.complete_success()
                self.assertFalse(h.owner.receipt.worker_waited)
                self.assertFalse(h.owner.receipt.jobs_empty)
                self.assertEqual(h.closed, [])

    def test_completion_rejects_deadline_expiry_and_cleanup_uncertainty(self):
        h = self.completion_harness()
        h.now = 1000
        with self.assertRaises(m.Hold):
            h.owner.complete_success()
        self.assertNotIn("WaitForSingleObject", h.calls)
        h = self.completion_harness()
        h.pipes.ready = False
        self.assertEqual(h.owner.complete_success().phase, "hold")
        self.assertTrue(h.owner.receipt.worker_waited)
        self.assertTrue(h.owner.receipt.local)
        self.assertNotIn("TerminateJobObject", h.calls)
        h.pipes.ready = True
        self.assertEqual(h.owner.close().phase, "closed")

    def prepare(self, h):
        return h.owner.prepare(r"\\.\pipe\cp329-injected")

    def test_inert_constructor_and_factory_hold(self):
        h = Harness()
        self.assertEqual(h.calls, [])
        with self.assertRaises(m.Hold):
            m.create_live_worker()
        self.assertEqual(h.calls, [])

    def test_suspended_fixed_identity_and_exact_attributes(self):
        h = Harness()
        r = self.prepare(h)
        self.assertEqual(r.phase, "prepared-suspended")
        self.assertEqual(h.command, m.WORKER_IMAGE + " 1000")
        self.assertEqual(h.attributes, {m.JOB_LIST: (21,), m.HANDLE_LIST: (11, 13)})
        self.assertEqual(h.creation[0], m.WORKER_IMAGE)
        self.assertEqual(h.creation[5], m.CREATE_FLAGS)
        self.assertEqual(h.creation[7], m.WORKER_CWD)
        self.assertEqual(h.dup_args[1:3], (22, 31))
        self.assertIs(h.dup_args[5], False)
        self.assertEqual(r.remote_child_job, m.RemoteHandle(44, 31))
        self.assertLess(h.calls.index("GetTickCount64"), h.calls.index("security.prepare"))
        self.assertLess(h.calls.index("pin.verify"), h.calls.index("DuplicateHandle"))
        self.assertNotIn("ResumeThread", h.calls)
        with self.assertRaises(m.Hold):
            self.prepare(h)
        self.assertEqual(h.owner.close().phase, "closed")
        self.assertNotIn(44, h.closed)

    def test_deadline_expired_before_preparation(self):
        h = Harness()
        h.now = 1000
        with self.assertRaises(m.PreparationFailure):
            self.prepare(h)
        self.assertNotIn("security.prepare", h.calls)
        self.assertNotIn("CreateProcessW", h.calls)

    def test_deadline_over_5000ms_rejected_before_preparation(self):
        h = Harness()
        h.pipes.deadline = 5101
        h.owner = m.NativeWorkerCreator(h.exports, security=h.security,
            pipe_owner=h.pipes, image_pin=h.pin, deadline=5101)
        with self.assertRaises(m.PreparationFailure):
            self.prepare(h)
        self.assertNotIn("security.prepare", h.calls)
        self.assertNotIn("pipes.prepare", h.calls)
        self.assertNotIn("pin.prepare", h.calls)
        self.assertNotIn("CreateProcessW", h.calls)

    def test_deadline_not_renewed_after_slow_security(self):
        h = Harness()
        original = h.security.prepare
        def slow():
            original()
            h.now = 1001
        h.security.prepare = slow
        with self.assertRaises(m.PreparationFailure):
            self.prepare(h)
        self.assertNotIn("pipes.prepare", h.calls)

    def test_deadline_property_cannot_be_reset_and_pipe_change_rejected(self):
        h = Harness()
        with self.assertRaises(AttributeError):
            h.owner.deadline = 2000
        h.pipes.deadline = 2000
        with self.assertRaises(m.PreparationFailure):
            self.prepare(h)
        self.assertNotIn("security.prepare", h.calls)

    def test_clock_regression_and_expiry_during_image_prepare(self):
        for now in (99, 1000):
            with self.subTest(now=now):
                h = Harness()
                original = h.pin.prepare
                def slow():
                    original()
                    h.now = now
                h.pin.prepare = slow
                with self.assertRaises(m.PreparationFailure):
                    self.prepare(h)
                self.assertNotIn("CreateProcessW", h.calls)

    def test_identity_and_containment_failures_never_duplicate(self):
        for flag in ("bad_pid", "bad_path", "bad_membership", "bad_return_size", "bad_job_flags"):
            with self.subTest(flag=flag):
                h = Harness()
                setattr(h, flag, True)
                with self.assertRaises(m.PreparationFailure):
                    self.prepare(h)
                self.assertNotIn("DuplicateHandle", h.calls)
                self.assertNotIn("ResumeThread", h.calls)

    def test_image_verify_failure_terminates_before_pending_pipe_cleanup(self):
        h = Harness()
        h.pin.fail_verify, h.pipes.ready = True, False
        with self.assertRaises(m.PreparationFailure) as caught:
            self.prepare(h)
        self.assertIs(caught.exception.owner, h.owner)
        self.assertEqual(h.owner.receipt.phase, "hold")
        self.assertLess(h.calls.index("TerminateJobObject"), h.calls.index("pipes.cleanup"))
        self.assertEqual(set(h.owner.receipt.local), {"worker_job", "child_job", "worker_process", "worker_thread"})
        self.assertEqual(h.closed, [])
        self.assertEqual(h.pin.handle, 50)
        h.pipes.ready = True
        self.assertEqual(h.owner.close().phase, "closed")

    def test_exact_wait_and_both_counts_gate_cleanup(self):
        for status, count in ((258, 0), (0xffffffff, 0), (True, 0), (0, 1)):
            with self.subTest(status=status, child_count=count):
                h = Harness()
                self.prepare(h)
                h.wait, h.child_count = status, count
                r = h.owner.close()
                self.assertEqual(r.phase, "hold")
                self.assertEqual(h.closed, [])
                self.assertNotIn("pipes.cleanup", h.calls)
                self.assertIsNotNone(r.remote_child_job)
                h.wait, h.child_count = 0, 0
                self.assertEqual(h.owner.close().phase, "closed")

    def test_create_and_duplicate_partial_outputs_retained(self):
        for flag in ("create_false", "create_raise", "duplicate_false", "duplicate_raise"):
            with self.subTest(flag=flag):
                h = Harness()
                setattr(h, flag, True)
                h.wait = 258
                with self.assertRaises(m.PreparationFailure):
                    self.prepare(h)
                r = h.owner.receipt
                self.assertEqual(r.local["worker_process"], 31)
                self.assertEqual(r.local["worker_thread"], 32)
                self.assertEqual(r.pid, 33)
                if flag.startswith("duplicate"):
                    self.assertEqual(r.remote_child_job, m.RemoteHandle(44, 31))
                self.assertEqual(h.closed, [])

    def test_unknown_creation_without_wait_handle_stays_held(self):
        h = Harness()
        h.create_raise = h.create_no_outputs = True
        with self.assertRaises(m.PreparationFailure):
            self.prepare(h)
        self.assertTrue(h.terminated)
        self.assertEqual(h.owner.close().phase, "hold")
        self.assertEqual(h.closed, [])
        self.assertNotIn("pipes.cleanup", h.calls)

    def test_close_failure_retains_exact_handle_and_pin(self):
        h = Harness()
        self.prepare(h)
        h.close_fail = 21
        self.assertEqual(h.owner.close().phase, "hold")
        self.assertEqual(h.owner.receipt.local["worker_job"], 21)
        self.assertEqual(h.pin.handle, 50)
        h.close_fail = None
        self.assertEqual(h.owner.close().phase, "closed")
        self.assertEqual(h.closed.count(21), 1)

    def test_terminate_failure_precedes_and_prevents_pipe_cleanup(self):
        h = Harness()
        self.prepare(h)
        h.fail = "TerminateJobObject"
        self.assertEqual(h.owner.close().phase, "hold")
        self.assertNotIn("pipes.cleanup", h.calls)
        self.assertEqual(h.closed, [])
        h.fail = None
        self.assertEqual(h.owner.close().phase, "closed")

    def test_pin_close_failure_is_retryable_after_process_death(self):
        h = Harness()
        self.prepare(h)
        h.pin.fail_close = True
        self.assertEqual(h.owner.close().phase, "hold")
        self.assertEqual(h.pin.handle, 50)
        self.assertFalse(h.owner.receipt.local)
        h.pin.fail_close = False
        self.assertEqual(h.owner.close().phase, "closed")

    def test_last_error_captured_before_failure_cleanup(self):
        h = Harness()
        h.create_false = True
        with self.assertRaises(m.PreparationFailure) as caught:
            self.prepare(h)
        self.assertIsInstance(caught.exception.__cause__, m.ApiFailure)
        self.assertEqual(caught.exception.__cause__.code, 5)

    def test_uncertain_attribute_initialization_retains_storage(self):
        h = Harness()
        h.attr_init_raise = True
        with self.assertRaises(m.PreparationFailure):
            self.prepare(h)
        self.assertEqual(h.owner.receipt.phase, "hold")
        self.assertIsNotNone(h.owner._attributes)
        self.assertEqual(h.closed, [])

    def test_no_signature_mutation_or_legacy_fake_marker(self):
        h = Harness()
        fn = h.exports.fn("CreateProcessW")
        self.prepare(h)
        self.assertFalse(hasattr(fn, "argtypes"))
        self.assertFalse(hasattr(fn, "restype"))


if __name__ == "__main__":
    unittest.main()
