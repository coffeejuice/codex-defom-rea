"""Pure fake ABI checks for CP310; no kernel call or process creation."""
import ctypes as C
from ctypes import wintypes as W
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp310_native_job_adapter as m


class Fn:
    def __init__(self, callback):
        self.callback = callback
        self.argtypes = self.restype = None

    def __call__(self, *args):
        return self.callback(*args)


class FakeTable:
    def __init__(self):
        self.calls = []
        self.error = 5
        self.fail = None
        self.wait_status = m.WAIT_OBJECT_0
        self.image_attempts = 0
        self.output_read = False
        self.job_return_length = None
        self.in_job = True
        self.image_output_length = None
        self.duplicate_output = 70
        names = m.SIGNATURES
        for name in names:
            setattr(self, name, Fn(lambda *args, name=name: self._call(name, *args)))

    @staticmethod
    def put(pointer, typ, value):
        C.cast(pointer, C.POINTER(typ))[0] = value

    def _call(self, name, *args):
        self.calls.append((name, args))
        if name == "GetCurrentProcess":
            return -1
        if name == "GetProcessId":
            return 0 if self.fail == name else 1234
        if name == "WaitForSingleObject":
            return m.WAIT_FAILED if self.fail == name else self.wait_status
        if name == "QueryFullProcessImageNameW":
            self.image_attempts += 1
            if self.fail == name:
                self.put(args[3], W.DWORD, 999)
                return 0
            if self.fail == "image-cap":
                self.error = m.ERROR_INSUFFICIENT_BUFFER
                return 0
            image = r"C:\PROBLEM\fixture.exe"
            if self.fail == "image-grow" and self.image_attempts == 1:
                self.error = m.ERROR_INSUFFICIENT_BUFFER
                self.put(args[3], W.DWORD, 999)
                return 0
            C.memmove(args[2], C.create_unicode_buffer(image),
                      (len(image) + 1) * C.sizeof(C.c_wchar))
            self.put(args[3], W.DWORD, (len(image) if self.image_output_length is None
                                        else self.image_output_length))
            return 1
        if name == "GetHandleInformation":
            self.put(args[1], W.DWORD, 0xA5)
        elif name == "QueryInformationJobObject":
            if args[1] == m.JOB_OBJECT_BASIC_ACCOUNTING_INFORMATION:
                obj = C.cast(args[2], C.POINTER(m.BASIC_ACCOUNTING))
                obj[0].ActiveProcesses = 0
                length = C.sizeof(m.BASIC_ACCOUNTING)
            else:
                obj = C.cast(args[2], C.POINTER(m.EXTENDED_LIMIT))
                obj[0].BasicLimitInformation.LimitFlags = 0x2000
                length = C.sizeof(m.EXTENDED_LIMIT)
            self.put(args[4], W.DWORD, (length if self.job_return_length is None
                                        else self.job_return_length))
        elif name == "IsProcessInJob":
            self.put(args[2], W.BOOL, int(self.in_job))
        elif name == "GetProcessTimes":
            self.put(args[1], m.FILETIME, m.FILETIME(0x89ABCDEF, 0x12345678))
        elif name == "DuplicateHandle":
            self.put(args[3], W.HANDLE, W.HANDLE(self.duplicate_output))
        if self.fail == name:
            return 0
        return 1

    def last_error(self):
        self.calls.append(("last_error", ()))
        return self.error


class NativeAdapterTests(unittest.TestCase):
    def setUp(self):
        self.fake = FakeTable()
        self.api = m.NativeJobApi(self.fake, last_error=self.fake.last_error)
        self.job = m.LocalHandle(20)
        self.process = m.LocalHandle(30)

    def test_win64_layout_and_exact_function_signatures(self):
        self.assertEqual(C.sizeof(C.c_void_p), 8)
        self.assertEqual(C.sizeof(W.BOOL), 4)
        self.assertEqual(C.sizeof(W.DWORD), 4)
        self.assertEqual(C.sizeof(m.FILETIME), 8)
        self.assertEqual(C.sizeof(m.BASIC_ACCOUNTING), 48)
        self.assertEqual(m.BASIC_ACCOUNTING.ActiveProcesses.offset, 40)
        self.assertEqual(C.sizeof(m.BASIC_LIMIT), 64)
        self.assertEqual(C.sizeof(m.EXTENDED_LIMIT), 144)
        self.assertEqual(m.EXTENDED_LIMIT.IoInfo.offset, 64)
        for name, (args, result) in m.SIGNATURES.items():
            fn = getattr(self.fake, name)
            self.assertEqual(fn.argtypes, args, name)
            self.assertIs(fn.restype, result, name)

    def test_bool_outputs_and_zero_are_normalized(self):
        self.assertEqual(self.api.GetHandleInformation(self.job), 0xA5)
        self.assertEqual(self.api.QueryInformationJobObject(self.job), 0)
        self.assertEqual(self.api.GetJobLimitFlags(self.job), 0x2000)
        self.assertIs(self.api.IsProcessInJob(self.process, self.job), True)
        self.assertEqual(self.api.GetProcessId(self.process), 1234)
        self.assertEqual(self.api.GetProcessTimes(self.process), 0x1234567889ABCDEF)
        self.assertEqual(self.api.QueryFullProcessImageNameW(self.process),
                         r"C:\PROBLEM\fixture.exe")
        self.assertIs(self.api.TerminateJobObject(self.job, 0), True)
        self.assertIs(self.api.CloseHandle(self.job), True)
        self.assertEqual(self.api.WaitForSingleObject(self.process, 0), 0)
        self.fake.wait_status = m.WAIT_TIMEOUT
        self.assertEqual(self.api.WaitForSingleObject(self.process, 1), 0x102)
        self.assertNotEqual(self.api.WaitForSingleObject(self.process, 1), 0)
        query_classes = [args[1] for name, args in self.fake.calls
                         if name == "QueryInformationJobObject"]
        self.assertEqual(query_classes, [1, 9])

    def test_failed_bool_never_accepts_poisoned_output_and_reads_last_error(self):
        cases = [
            ("GetHandleInformation", lambda: self.api.GetHandleInformation(self.job)),
            ("QueryInformationJobObject", lambda: self.api.QueryInformationJobObject(self.job)),
            ("IsProcessInJob", lambda: self.api.IsProcessInJob(self.process, self.job)),
            ("GetProcessId", lambda: self.api.GetProcessId(self.process)),
            ("GetProcessTimes", lambda: self.api.GetProcessTimes(self.process)),
            ("QueryFullProcessImageNameW", lambda: self.api.QueryFullProcessImageNameW(self.process)),
            ("DuplicateHandle", lambda: self.api.DuplicateHandle(
                self.process, m.RemoteHandle(40, 30), -1)),
            ("TerminateJobObject", lambda: self.api.TerminateJobObject(self.job, 0)),
            ("CloseHandle", lambda: self.api.CloseHandle(self.job)),
            ("WaitForSingleObject", lambda: self.api.WaitForSingleObject(self.process, 0)),
        ]
        for name, invoke in cases:
            with self.subTest(name=name):
                self.fake.calls.clear()
                self.fake.fail = name
                with self.assertRaises(m.ApiFailure) as raised:
                    invoke()
                self.assertEqual(raised.exception.operation, name)
                self.assertEqual(raised.exception.code, self.fake.error)
                self.assertEqual(self.fake.calls[-1][0], "last_error")
        self.fake.fail = "image-grow"
        self.fake.image_attempts = 0
        self.assertEqual(self.api.QueryFullProcessImageNameW(self.process),
                         r"C:\PROBLEM\fixture.exe")
        self.assertEqual(self.fake.image_attempts, 2)
        image_calls = [args for name, args in self.fake.calls
                       if name == "QueryFullProcessImageNameW"][-2:]
        self.assertEqual([len(args[2]) for args in image_calls], [260, 520])
        self.assertEqual([C.cast(args[3], C.POINTER(W.DWORD))[0]
                          for args in image_calls], [999, len(r"C:\PROBLEM\fixture.exe")])

    def test_duplicate_handle_locality_and_no_remote_close(self):
        incoming = self.api.DuplicateHandle(
            self.process, m.RemoteHandle(40, 30), -1)
        self.assertEqual(incoming, m.LocalHandle(70))
        outgoing = self.api.DuplicateHandle(-1, self.job, self.process)
        self.assertEqual(outgoing, m.RemoteHandle(70, 30))
        for method in (self.api.GetHandleInformation, self.api.QueryInformationJobObject,
                       self.api.GetJobLimitFlags, self.api.GetProcessId,
                       self.api.GetProcessTimes, self.api.QueryFullProcessImageNameW,
                       self.api.CloseHandle):
            with self.subTest(method=method.__name__):
                with self.assertRaises(TypeError):
                    method(outgoing)
        with self.assertRaises(TypeError):
            self.api.DuplicateHandle(self.process, m.RemoteHandle(40, 31), -1)
        with self.assertRaises(TypeError):
            self.api.DuplicateHandle(-1, m.RemoteHandle(40, 30), self.process)
        with self.assertRaises(ValueError):
            self.api.DuplicateHandle(-1, self.job, self.process, inherit=True)
        self.fake.GetCurrentProcess.callback = lambda: (1 << 64) - 1
        self.assertEqual(self.api.GetCurrentProcess(), -1)
        self.assertEqual(self.api.DuplicateHandle(self.process,
                                                  m.RemoteHandle(40, 30), -1),
                         m.LocalHandle(70))
        self.assertEqual([name for name, _ in self.fake.calls if name == "CloseHandle"], [])

    def test_job_query_accepts_optional_zero_length_but_rejects_partial_or_oversize(self):
        for method, size in ((self.api.QueryInformationJobObject, C.sizeof(m.BASIC_ACCOUNTING)),
                             (self.api.GetJobLimitFlags, C.sizeof(m.EXTENDED_LIMIT))):
            self.fake.job_return_length = 0
            method(self.job)
            for invalid in (1, size - 1, size + 1, 0xFFFFFFFF):
                with self.subTest(method=method.__name__, length=invalid):
                    self.fake.job_return_length = invalid
                    with self.assertRaisesRegex(RuntimeError, "unexpected length"):
                        method(self.job)

    def test_successful_false_membership_and_null_duplicate_output(self):
        self.fake.in_job = False
        self.assertIs(self.api.IsProcessInJob(self.process, self.job), False)
        self.fake.duplicate_output = None
        with self.assertRaisesRegex(RuntimeError, "without a valid output handle"):
            self.api.DuplicateHandle(self.process, m.RemoteHandle(40, 30), -1)

    def test_image_name_success_length_bounds_and_growth_cap(self):
        for invalid in (0, 260, 261, 0xFFFFFFFF):
            with self.subTest(length=invalid):
                self.fake.image_output_length = invalid
                with self.assertRaisesRegex(RuntimeError, "invalid output length"):
                    self.api.QueryFullProcessImageNameW(self.process)
        self.fake.image_output_length = None
        self.fake.fail = "image-cap"
        self.fake.image_attempts = 0
        with self.assertRaises(m.ApiFailure) as raised:
            self.api.QueryFullProcessImageNameW(self.process)
        self.assertEqual(raised.exception.code, m.ERROR_INSUFFICIENT_BUFFER)
        image_calls = [args for name, args in self.fake.calls
                       if name == "QueryFullProcessImageNameW"][-self.fake.image_attempts:]
        self.assertEqual([len(args[2]) for args in image_calls],
                         [260, 520, 1040, 2080, 4160, 8320, 16640, 32768])

    def test_ambiguous_values_and_live_creation_hold(self):
        with self.assertRaises(ValueError):
            m.LocalHandle(True)
        with self.assertRaises(ValueError):
            m.LocalHandle(0)
        with self.assertRaises(ValueError):
            m.LocalHandle(m.MAX_HANDLE_VALUE)
        with self.assertRaises(ValueError):
            m.LocalHandle(m.MAX_HANDLE_VALUE + 1)
        for value, owner in ((m.MAX_HANDLE_VALUE, 30), (m.MAX_HANDLE_VALUE + 1, 30),
                             (40, m.MAX_HANDLE_VALUE), (40, m.MAX_HANDLE_VALUE + 1)):
            with self.subTest(value=value, owner=owner), self.assertRaises(ValueError):
                m.RemoteHandle(value, owner)
        with self.assertRaises(ValueError):
            self.api.WaitForSingleObject(self.process, True)
        with self.assertRaises(ValueError):
            self.api.TerminateJobObject(self.job, True)
        with self.assertRaises(RuntimeError):
            m.create_live_job_or_process()
        self.assertFalse(hasattr(self.api, "CreateProcessW"))
        self.assertFalse(hasattr(self.api, "CreateJobObjectW"))


if __name__ == "__main__":
    unittest.main()
