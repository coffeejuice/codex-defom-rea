"""CP303 ABI and failure checks with a fake WinAPI; never creates a process."""
import ctypes as C
from ctypes import wintypes as W
from pathlib import Path
import subprocess
import sys
import unittest


sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp303_job_creation as m


class FakeApi:
    cp303_fake_api = True

    def __init__(self, fail=None, create_exception=False):
        self.calls = []
        self.fail = fail
        self.create_exception = create_exception

    def last_error(self):
        return 1234

    def CreateJobObjectW(self, security, name):
        self.calls.append(("create-job", security, name))
        return None if self.fail == "create-job" else 71

    def GetHandleInformation(self, handle, flags):
        self.calls.append(("get-handle-flags", handle))
        C.cast(flags, C.POINTER(W.DWORD))[0] = m.HANDLE_FLAG_INHERIT if self.fail == "inheritable" else 0
        return self.fail != "get-handle-flags"

    def SetInformationJobObject(self, handle, info_class, limits, size):
        structure = C.cast(limits, C.POINTER(m.JOBOBJECT_EXTENDED_LIMIT_INFORMATION)).contents
        self.calls.append(("set-job", handle, info_class, size,
                           structure.BasicLimitInformation.LimitFlags))
        return self.fail != "set-job"

    def CloseHandle(self, handle):
        self.calls.append(("close", handle))
        return self.fail != "close"

    def GetCurrentProcess(self):
        self.calls.append(("current-process",))
        return 99

    def DuplicateHandle(self, source, job, worker, output, access, inherit, options):
        self.calls.append(("duplicate", source, job, worker, access, inherit, options))
        if self.fail == "duplicate":
            return False
        C.cast(output, C.POINTER(W.HANDLE))[0] = 81
        return True

    def InitializeProcThreadAttributeList(self, pointer, count, flags, size):
        self.calls.append(("initialize", bool(pointer), count, flags))
        if not pointer:
            C.cast(size, C.POINTER(C.c_size_t))[0] = 128 if self.fail != "size" else 0
            return False  # documented sizing call intentionally fails
        return self.fail != "initialize"

    def UpdateProcThreadAttribute(self, pointer, flags, key, value, length, previous, returned):
        job = C.cast(value, C.POINTER(W.HANDLE))[0]
        self.calls.append(("update", flags, key, job, length, previous, returned))
        return self.fail != "update"

    def CreateProcessW(self, exe, command, process_attributes, thread_attributes,
                       inherit, flags, environment, cwd, startup, process):
        si = C.cast(startup, C.POINTER(m.STARTUPINFOEXW)).contents
        self.calls.append(("create-process", exe, command.value, process_attributes,
                           thread_attributes, inherit, flags, environment, cwd,
                           si.StartupInfo.cb, bool(si.lpAttributeList)))
        if self.create_exception:
            raise RuntimeError("creator interrupted inside CreateProcessW")
        if self.fail == "create-process":
            return False
        pi = C.cast(process, C.POINTER(m.PROCESS_INFORMATION))
        pi.contents.hProcess = 101
        pi.contents.hThread = 102
        pi.contents.dwProcessId = 103
        pi.contents.dwThreadId = 104
        return True

    def DeleteProcThreadAttributeList(self, pointer):
        self.calls.append(("delete", bool(pointer)))


IMAGE = m.FixedCloneImage("C:\\PROBLEM\\m289_0123456789abcdef\\fixture.exe",
                          "C:\\PROBLEM\\m289_0123456789abcdef", m.CP302_SHA256)


class JobCreationTests(unittest.TestCase):
    def test_windows_structure_shape(self):
        self.assertEqual(m.STARTUPINFOEXW.lpAttributeList.offset, C.sizeof(m.STARTUPINFOW))
        self.assertEqual(m.JOBOBJECT_EXTENDED_LIMIT_INFORMATION.IoInfo.offset,
                         C.sizeof(m.JOBOBJECT_BASIC_LIMIT_INFORMATION))
        self.assertEqual(m.PROCESS_INFORMATION.hThread.offset, C.sizeof(W.HANDLE))
        self.assertEqual(m.PROC_THREAD_ATTRIBUTE_JOB_LIST, 0x0002000D)

    def test_supervisor_job_noninherit_and_kill_limit_before_worker_duplicate(self):
        api = FakeApi()
        job = m.stage_supervisor_job_for_fake_test(api)
        worker_value = m.duplicate_job_to_worker_for_fake_test(api, job, 42)
        self.assertEqual((job.handle, worker_value), (71, 81))
        self.assertEqual(api.calls, [
            ("create-job", None, None), ("get-handle-flags", 71),
            ("set-job", 71, 9, C.sizeof(m.JOBOBJECT_EXTENDED_LIMIT_INFORMATION), 0x2000),
            ("current-process",), ("duplicate", 99, 71, 42, 0, False, 2)])

    def test_job_setup_failures_close_exact_handle(self):
        for failure in ("get-handle-flags", "inheritable", "set-job"):
            with self.subTest(failure=failure):
                api = FakeApi(fail=failure)
                with self.assertRaises((m.ApiFailure, RuntimeError)):
                    m.stage_supervisor_job_for_fake_test(api)
                self.assertEqual(api.calls[-1], ("close", 71))
        api = FakeApi(fail="create-job")
        with self.assertRaises(m.ApiFailure):
            m.stage_supervisor_job_for_fake_test(api)
        self.assertEqual(len(api.calls), 1)

    def test_worker_duplicate_failure_is_not_receipt(self):
        with self.assertRaises(m.ApiFailure):
            m.duplicate_job_to_worker_for_fake_test(FakeApi(fail="duplicate"),
                                                    m.SupervisorJob(71), 42)

    def test_exact_creation_order_and_flags(self):
        api = FakeApi()
        pi = m.worker_create_suspended_for_fake_test(api, IMAGE, 81)
        self.assertEqual((pi.hProcess, pi.hThread, pi.dwProcessId, pi.dwThreadId),
                         (101, 102, 103, 104))
        self.assertEqual([call[0] for call in api.calls],
                         ["initialize", "initialize", "update", "create-process", "delete"])
        update, create = api.calls[2:4]
        self.assertEqual(update, ("update", 0, m.PROC_THREAD_ATTRIBUTE_JOB_LIST,
                                  81, C.sizeof(W.HANDLE), None, None))
        self.assertEqual(create[1:9],
                         (IMAGE.executable, subprocess.list2cmdline([IMAGE.executable]), None, None,
                          False, m.CREATE_SUSPENDED | m.EXTENDED_STARTUPINFO_PRESENT |
                          m.CREATE_NO_WINDOW, None, IMAGE.cwd))
        self.assertEqual(create[9:], (C.sizeof(m.STARTUPINFOEXW), True))
        self.assertNotIn("ResumeThread", [call[0] for call in api.calls])
        self.assertNotIn("AssignProcessToJobObject", [call[0] for call in api.calls])

    def test_attribute_setup_failure_never_calls_create(self):
        for failure, names in (
            ("size", ["initialize"]),
            ("initialize", ["initialize", "initialize"]),
            ("update", ["initialize", "initialize", "update", "delete"]),
        ):
            with self.subTest(failure=failure):
                api = FakeApi(fail=failure)
                with self.assertRaises(m.ApiFailure):
                    m.worker_create_suspended_for_fake_test(api, IMAGE, 81)
                self.assertEqual([c[0] for c in api.calls], names)

    def test_create_failure_and_interruption_are_uncertain_and_delete_list(self):
        for api in (FakeApi(fail="create-process"), FakeApi(create_exception=True)):
            with self.subTest(api=api):
                with self.assertRaises(m.CreateOutcomeUnknown) as caught:
                    m.worker_create_suspended_for_fake_test(api, IMAGE, 81)
                self.assertIsNotNone(caught.exception.process_information)
                self.assertEqual(caught.exception.code, 1234)
                self.assertEqual(api.calls[-1], ("delete", True))

    def test_fixed_image_and_real_entry_hold(self):
        for bad in (m.FixedCloneImage(IMAGE.executable, IMAGE.cwd, "0" * 64),
                    m.FixedCloneImage("C:\\PROBLEM\\m289_0123456789abcdef\\other.exe",
                                      IMAGE.cwd, m.CP302_SHA256)):
            with self.assertRaises(ValueError):
                m.worker_create_suspended_for_fake_test(FakeApi(), bad, 81)
        with self.assertRaises(RuntimeError):
            m.create_real_child()
        with self.assertRaises(RuntimeError):
            m.stage_supervisor_job_for_fake_test(object())

    def test_fake_marked_wrapper_cannot_forward_to_real_create(self):
        class ForwardingApi(FakeApi):
            def CreateProcessW(self, *args):
                self.calls.append(("attempt-forward-to-real",))
                return m.Kernel32Api.CreateProcessW(*args)

        api = ForwardingApi()
        with self.assertRaises(m.CreateOutcomeUnknown) as caught:
            m.worker_create_suspended_for_fake_test(api, IMAGE, 81)
        self.assertIsInstance(caught.exception.__cause__, RuntimeError)
        self.assertIn("real invocation HOLD", str(caught.exception.__cause__))
        self.assertEqual(api.calls[-2:], [("attempt-forward-to-real",), ("delete", True)])
        with self.assertRaisesRegex(RuntimeError, "real invocation HOLD"):
            m.Kernel32Api.CreateProcessW()


if __name__ == "__main__":
    unittest.main()
