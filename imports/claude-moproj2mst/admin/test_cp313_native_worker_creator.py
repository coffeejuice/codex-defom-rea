"""CP313 pure fake ABI, ordering, containment, and cleanup tests."""
import ctypes as C
from ctypes import wintypes as W
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp313_native_worker_creator as m


class Fn:
    def __init__(self, callback):
        self.callback = callback
        self.argtypes = self.restype = None

    def __call__(self, *args):
        return self.callback(*args)


class FakeApi:
    cp313_fake_api = True

    def __init__(self):
        self.calls = []
        self.next_handle = 100
        self.flags = {}
        self.job_flags = {}
        self.fail = None
        self.fail_create_with_partial = False
        self.pid = 31301
        self.image = m.WORKER_IMAGE
        self.membership = {"worker": True, "child": False}
        self.attr = []
        self.close_fail = None
        self.wait_status = m.WAIT_OBJECT_0
        self.wait_raise = False
        self.active_count = {"worker": 0, "child": 0}
        self.accounting_raise = False
        for name in m.SIGNATURES:
            setattr(self, name, Fn(lambda *args, name=name: self._call(name, *args)))

    def _new(self, inherited=False):
        self.next_handle += 1
        self.flags[self.next_handle] = m.HANDLE_FLAG_INHERIT if inherited else 0
        return self.next_handle

    @staticmethod
    def put(pointer, typ, value):
        C.cast(pointer, C.POINTER(typ))[0] = value

    def last_error(self):
        self.calls.append(("last_error", ()))
        return 87

    def _call(self, name, *args):
        self.calls.append((name, args))
        if name == "CreateJobObjectW":
            h = self._new()
            self.job_flags[h] = 0
            return h
        if name == "SetInformationJobObject":
            self.job_flags[args[0]] = C.cast(args[2], C.POINTER(m.EXTENDED_LIMIT))[0].BasicLimitInformation.LimitFlags
        elif name == "QueryInformationJobObject":
            if args[1] == m.JobObjectBasicAccountingInformation:
                if self.accounting_raise:
                    raise RuntimeError("fake accounting interrupted")
                jobs = list(self.job_flags)
                label = "worker" if args[0] == jobs[0] else "child"
                C.cast(args[2], C.POINTER(m.BASIC_ACCOUNTING))[0].ActiveProcesses = self.active_count[label]
                self.put(args[4], W.DWORD, C.sizeof(m.BASIC_ACCOUNTING))
            else:
                C.cast(args[2], C.POINTER(m.EXTENDED_LIMIT))[0].BasicLimitInformation.LimitFlags = self.job_flags[args[0]]
                self.put(args[4], W.DWORD, C.sizeof(m.EXTENDED_LIMIT))
        elif name == "GetHandleInformation":
            self.put(args[1], W.DWORD, self.flags[args[0]])
        elif name == "CreatePipe":
            self.put(args[0], W.HANDLE, self._new(True))
            self.put(args[1], W.HANDLE, self._new(True))
        elif name == "SetHandleInformation":
            self.flags[args[0]] = (self.flags[args[0]] & ~args[1]) | (args[2] & args[1])
        elif name == "InitializeProcThreadAttributeList":
            self.put(args[3], C.c_size_t, 128)
            if args[0] is None:
                return 0
        elif name == "UpdateProcThreadAttribute":
            count = args[4] // C.sizeof(W.HANDLE)
            values = tuple(C.cast(args[3], C.POINTER(W.HANDLE * count))[0])
            self.attr.append((args[2], values))
        elif name == "CreateProcessW":
            startup = C.cast(args[8], C.POINTER(m.STARTUPINFOEXW))[0]
            self.startup = startup
            self.create_args = args
            info = C.cast(args[9], C.POINTER(m.PROCESS_INFORMATION))
            if self.fail != name or self.fail_create_with_partial:
                self.process, self.thread = self._new(), self._new()
                info[0].hProcess, info[0].hThread = self.process, self.thread
                info[0].dwProcessId, info[0].dwThreadId = self.pid, 31302
        elif name == "GetProcessId":
            return self.pid
        elif name == "QueryFullProcessImageNameW":
            C.memmove(args[2], C.create_unicode_buffer(self.image),
                      (len(self.image) + 1) * C.sizeof(C.c_wchar))
            self.put(args[3], W.DWORD, len(self.image))
        elif name == "IsProcessInJob":
            jobs = list(self.job_flags)
            self.put(args[2], W.BOOL, int(self.membership["worker" if args[1] == jobs[0] else "child"]))
        elif name == "GetCurrentProcess":
            return -1
        elif name == "DuplicateHandle":
            self.put(args[3], W.HANDLE, 909)
        elif name == "WaitForSingleObject":
            if self.wait_raise:
                raise RuntimeError("fake interrupted wait")
            return self.wait_status
        elif name == "CloseHandle":
            if self.close_fail == args[0]:
                return 0
        if self.fail == name:
            return 0
        return 1


class CreatorTests(unittest.TestCase):
    def setUp(self):
        self.fake = FakeApi()
        self.creator = m.FakeWorkerCreator(self.fake, last_error=self.fake.last_error)

    def test_x64_layout_and_signatures(self):
        self.assertEqual(C.sizeof(C.c_void_p), 8)
        self.assertEqual(C.sizeof(m.PROCESS_INFORMATION), 24)
        self.assertEqual(C.sizeof(m.STARTUPINFOEXW), 112)
        self.assertEqual(C.sizeof(m.EXTENDED_LIMIT), 144)
        self.assertEqual(C.sizeof(m.BASIC_ACCOUNTING), 48)
        self.assertEqual(m.PROCESS_INFORMATION.dwProcessId.offset, 16)
        for name, (args, result) in m.SIGNATURES.items():
            fn = getattr(self.fake, name)
            self.assertEqual(fn.argtypes, args, name)
            self.assertIs(fn.restype, result, name)

    def test_live_entry_and_unmarked_api_hold(self):
        with self.assertRaises(m.Hold):
            m.create_live_worker()
        self.fake.cp313_fake_api = False
        with self.assertRaises(m.Hold):
            m.FakeWorkerCreator(self.fake, last_error=self.fake.last_error)
        self.assertEqual(self.fake.calls, [])

    def test_preparation_order_pipe_inheritance_and_suspended_identity(self):
        r = self.creator.prepare()
        self.assertEqual(r.phase, "prepared-suspended")
        names = [name for name, _ in self.fake.calls]
        self.assertLess(names.index("CreateJobObjectW"), names.index("CreatePipe"))
        self.assertLess(names.index("SetHandleInformation"), names.index("CreateProcessW"))
        self.assertLess(names.index("CreateProcessW"), names.index("GetProcessId"))
        self.assertLess(names.index("IsProcessInJob"), names.index("DuplicateHandle"))
        self.assertNotIn("ResumeThread", names)
        self.assertEqual([key for key, _ in self.fake.attr],
                         [m.PROC_THREAD_ATTRIBUTE_JOB_LIST, m.PROC_THREAD_ATTRIBUTE_HANDLE_LIST])
        worker_job = r.local["worker_job"]
        child_job = r.local["child_job"]
        self.assertEqual(self.fake.attr[0][1], (worker_job,))
        self.assertEqual(self.fake.attr[1][1],
                         (self.fake.startup.StartupInfo.hStdInput,
                          self.fake.startup.StartupInfo.hStdOutput))
        self.assertEqual(self.fake.startup.StartupInfo.hStdError,
                         self.fake.startup.StartupInfo.hStdOutput)
        self.assertEqual(self.fake.startup.StartupInfo.dwFlags, m.STARTF_USESTDHANDLES)
        self.assertEqual(self.fake.startup.StartupInfo.cb, C.sizeof(m.STARTUPINFOEXW))
        self.assertEqual(self.fake.create_args[0], m.WORKER_IMAGE)
        self.assertTrue(self.fake.create_args[4])
        self.assertEqual(self.fake.create_args[5], m.CREATE_FLAGS)
        self.assertEqual(self.fake.create_args[7], m.WORKER_CWD)
        self.assertEqual(r.remote_child_job, m.RemoteHandle(909, r.local["worker_process"]))
        self.assertEqual(r.duplicate_outcome, "returned-success")
        self.assertEqual(set(r.closed), {"input_read", "output_write"})
        self.assertEqual(set(r.local), {"worker_job", "child_job", "input_write",
                                        "output_read", "worker_process", "worker_thread"})
        self.assertEqual(self.fake.flags[worker_job], 0)
        duplicate = [args for name, args in self.fake.calls if name == "DuplicateHandle"][0]
        self.assertEqual((duplicate[1], duplicate[2], duplicate[4], duplicate[5], duplicate[6]),
                         (child_job, r.local["worker_process"], 0, False, m.DUPLICATE_SAME_ACCESS))
        self.assertEqual(len([name for name, _ in self.fake.calls if name == "CreateProcessW"]), 1)
        self.assertEqual(self.creator.close().phase, "closed")
        self.assertTrue(r.worker_terminated and r.worker_waited)
        self.assertTrue(r.jobs_empty)
        self.assertNotIn(909, [args[0] for name, args in self.fake.calls if name == "CloseHandle"])
        self.assertEqual(r.local, {})

    def test_precreation_failure_closes_every_local_handle(self):
        self.fake.fail = "SetHandleInformation"
        with self.assertRaises(m.PreparationFailure) as caught:
            self.creator.prepare()
        r = caught.exception.receipt
        self.assertEqual(r.create_outcome, "not-attempted")
        self.assertEqual(r.local, {})
        self.assertNotIn("CreateProcessW", [name for name, _ in self.fake.calls])

    def test_create_failure_retains_job_when_existence_unproved(self):
        self.fake.fail = "CreateProcessW"
        with self.assertRaises(m.PreparationFailure) as caught:
            self.creator.prepare()
        r = caught.exception.receipt
        self.assertEqual(r.create_outcome, "unknown: CreateProcessW entered")
        self.assertIn("worker_job", r.local)
        self.creator.close()
        self.assertIn("worker_job", r.local)
        self.assertIn("child_job", r.local)
        self.assertIn("input_write", r.local)
        self.assertIn("output_read", r.local)
        self.assertTrue(r.worker_terminated)
        self.assertFalse(r.worker_waited)

    def test_partial_create_failure_can_wait_and_close_exact_handles(self):
        self.fake.fail = "CreateProcessW"
        self.fake.fail_create_with_partial = True
        with self.assertRaises(m.PreparationFailure) as caught:
            self.creator.prepare()
        r = caught.exception.receipt
        self.assertIn("worker_process", r.local)
        self.assertIn("worker_thread", r.local)
        self.creator.close()
        self.assertTrue(r.worker_waited)
        self.assertEqual(r.local, {})

    def test_identity_or_membership_failure_holds_before_duplicate(self):
        for mutation in ("pid", "image", "member", "child_member"):
            with self.subTest(mutation=mutation):
                fake = FakeApi()
                if mutation == "pid":
                    original = fake._call
                    def wrong_pid(name, *args):
                        if name == "GetProcessId":
                            fake.calls.append((name, args))
                            return fake.pid + 1
                        return original(name, *args)
                    fake.GetProcessId.callback = lambda *args: wrong_pid("GetProcessId", *args)
                elif mutation == "image":
                    fake.image = r"C:\PROBLEM\s312\other.exe"
                elif mutation == "member":
                    fake.membership["worker"] = False
                else:
                    fake.membership["child"] = True
                creator = m.FakeWorkerCreator(fake, last_error=fake.last_error)
                with self.assertRaises(m.PreparationFailure):
                    creator.prepare()
                self.assertNotIn("DuplicateHandle", [name for name, _ in fake.calls])
                creator.close()
                self.assertEqual(creator.receipt.local, {})

    def test_duplicate_failure_retains_worker_until_explicit_abort(self):
        self.fake.fail = "DuplicateHandle"
        with self.assertRaises(m.PreparationFailure) as caught:
            self.creator.prepare()
        r = caught.exception.receipt
        self.assertEqual(r.phase, "hold")
        self.assertIn("worker_job", r.local)
        self.assertEqual(r.duplicate_outcome, "unknown: DuplicateHandle entered")
        self.assertEqual(r.remote_child_job, m.RemoteHandle(909, r.local["worker_process"]))
        self.creator.close()
        self.assertEqual(r.local, {})

    def test_wait_timeout_keeps_sole_worker_job(self):
        r = self.creator.prepare()
        self.fake.wait_status = m.WAIT_TIMEOUT
        self.creator.close()
        self.assertEqual(r.phase, "hold")
        self.assertIn("worker_process", r.local)
        self.assertIn("worker_thread", r.local)
        self.assertIn("worker_job", r.local)
        self.assertIn("child_job", r.local)
        self.assertIsNotNone(r.remote_child_job)

    def test_bool_false_wait_cannot_release_either_job(self):
        r = self.creator.prepare()
        self.fake.wait_status = False  # bool is an int subclass, but not a Win32 DWORD.
        self.creator.close()
        self.assertFalse(r.worker_waited)
        self.assertEqual(r.phase, "hold")
        self.assertIn("worker_process", r.local)
        self.assertIn("worker_job", r.local)
        self.assertIn("child_job", r.local)
        self.assertIsNotNone(r.remote_child_job)

    def test_interrupted_wait_retains_exact_handles_for_retry(self):
        r = self.creator.prepare()
        self.fake.wait_raise = True
        self.creator.close()
        self.assertEqual(r.phase, "hold")
        self.assertIn("worker_process", r.local)
        self.assertIn("worker_job", r.local)
        self.assertIn("child_job", r.local)
        self.assertTrue(any("worker wait interrupted" in error for error in r.errors))
        self.fake.wait_raise = False
        self.creator.close()
        self.assertTrue(r.worker_waited and r.jobs_empty)
        self.assertEqual(r.local, {})

    def test_active_child_job_retains_containment_until_resolved(self):
        r = self.creator.prepare()
        self.fake.active_count["child"] = 1
        self.creator.close()
        self.assertTrue(r.worker_waited)
        self.assertFalse(r.jobs_empty)
        self.assertIn("worker_process", r.local)
        self.assertIn("worker_job", r.local)
        self.assertIn("child_job", r.local)
        self.fake.active_count["child"] = 0
        self.creator.close()
        self.assertTrue(r.jobs_empty)
        self.assertEqual(r.local, {})

    def test_failed_termination_retains_every_handle_then_retries(self):
        r = self.creator.prepare()
        before = dict(r.local)
        self.fake.fail = "TerminateJobObject"
        self.creator.close()
        self.assertEqual(r.phase, "hold")
        self.assertFalse(r.worker_terminated)
        self.assertEqual(r.local, before)
        self.assertNotIn("WaitForSingleObject", [name for name, _ in self.fake.calls])
        self.fake.fail = None
        self.creator.close()
        self.assertTrue(r.worker_terminated and r.worker_waited and r.jobs_empty)
        self.assertEqual(r.local, {})

    def test_interrupted_accounting_retains_jobs_and_process_then_retries(self):
        r = self.creator.prepare()
        self.fake.accounting_raise = True
        self.creator.close()
        self.assertTrue(r.worker_waited)
        self.assertFalse(r.jobs_empty)
        self.assertEqual(r.phase, "hold")
        self.assertIn("worker_process", r.local)
        self.assertIn("worker_job", r.local)
        self.assertIn("child_job", r.local)
        self.assertTrue(any("job accounting unresolved" in error for error in r.errors))
        self.fake.accounting_raise = False
        self.creator.close()
        self.assertTrue(r.jobs_empty)
        self.assertEqual(r.local, {})

    def test_failed_close_retains_exact_handle_then_retries(self):
        r = self.creator.prepare()
        retained_process = r.local["worker_process"]
        self.fake.close_fail = retained_process
        self.creator.close()
        self.assertTrue(r.worker_waited and r.jobs_empty)
        self.assertEqual(r.phase, "hold")
        self.assertEqual(r.local, {"worker_process": retained_process})
        self.assertTrue(any("CloseHandle" in error for error in r.errors))
        self.fake.close_fail = None
        self.creator.close()
        self.assertEqual(r.local, {})
        self.assertEqual(r.phase, "closed")


if __name__ == "__main__":
    unittest.main()
