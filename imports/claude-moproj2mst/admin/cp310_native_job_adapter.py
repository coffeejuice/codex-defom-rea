"""CP310 typed Win32 job observations and cleanup, deliberately unwired.

Only caller-supplied, exact handles are accepted.  A handle value is meaningful
in one process handle table; RemoteHandle cannot be queried or closed here.
This module has no process/job creation or worker/child execution entry point.
"""
from __future__ import annotations

import ctypes as C
from ctypes import wintypes as W
from dataclasses import dataclass
import sys


JOB_OBJECT_BASIC_ACCOUNTING_INFORMATION = 1
JOB_OBJECT_EXTENDED_LIMIT_INFORMATION = 9
DUPLICATE_SAME_ACCESS = 2
WAIT_OBJECT_0 = 0
WAIT_TIMEOUT = 0x102
WAIT_FAILED = 0xFFFFFFFF
ERROR_INSUFFICIENT_BUFFER = 122
MAX_HANDLE_VALUE = (1 << (C.sizeof(C.c_void_p) * 8)) - 1


class FILETIME(C.Structure):
    _fields_ = [("dwLowDateTime", W.DWORD), ("dwHighDateTime", W.DWORD)]


class BASIC_ACCOUNTING(C.Structure):
    _fields_ = [("TotalUserTime", C.c_longlong), ("TotalKernelTime", C.c_longlong),
                ("ThisPeriodTotalUserTime", C.c_longlong),
                ("ThisPeriodTotalKernelTime", C.c_longlong),
                ("TotalPageFaultCount", W.DWORD), ("TotalProcesses", W.DWORD),
                ("ActiveProcesses", W.DWORD), ("TotalTerminatedProcesses", W.DWORD)]


class IO_COUNTERS(C.Structure):
    _fields_ = [(name, C.c_ulonglong) for name in (
        "ReadOperationCount", "WriteOperationCount", "OtherOperationCount",
        "ReadTransferCount", "WriteTransferCount", "OtherTransferCount")]


class BASIC_LIMIT(C.Structure):
    _fields_ = [("PerProcessUserTimeLimit", C.c_longlong),
                ("PerJobUserTimeLimit", C.c_longlong), ("LimitFlags", W.DWORD),
                ("MinimumWorkingSetSize", C.c_size_t),
                ("MaximumWorkingSetSize", C.c_size_t),
                ("ActiveProcessLimit", W.DWORD), ("Affinity", C.c_size_t),
                ("PriorityClass", W.DWORD), ("SchedulingClass", W.DWORD)]


class EXTENDED_LIMIT(C.Structure):
    _fields_ = [("BasicLimitInformation", BASIC_LIMIT), ("IoInfo", IO_COUNTERS),
                ("ProcessMemoryLimit", C.c_size_t), ("JobMemoryLimit", C.c_size_t),
                ("PeakProcessMemoryUsed", C.c_size_t),
                ("PeakJobMemoryUsed", C.c_size_t)]


# HANDLE and pointers use pointer width; DWORD/BOOL remain 32 bit on Win64.
SIGNATURES = {
    "GetCurrentProcess": ([], W.HANDLE),
    "GetHandleInformation": ([W.HANDLE, C.POINTER(W.DWORD)], W.BOOL),
    "QueryInformationJobObject": ([W.HANDLE, W.INT, C.c_void_p, W.DWORD,
                                   C.POINTER(W.DWORD)], W.BOOL),
    "IsProcessInJob": ([W.HANDLE, W.HANDLE, C.POINTER(W.BOOL)], W.BOOL),
    "GetProcessId": ([W.HANDLE], W.DWORD),
    "GetProcessTimes": ([W.HANDLE, C.POINTER(FILETIME), C.POINTER(FILETIME),
                         C.POINTER(FILETIME), C.POINTER(FILETIME)], W.BOOL),
    "QueryFullProcessImageNameW": ([W.HANDLE, W.DWORD, W.LPWSTR,
                                    C.POINTER(W.DWORD)], W.BOOL),
    "DuplicateHandle": ([W.HANDLE, W.HANDLE, W.HANDLE, C.POINTER(W.HANDLE),
                         W.DWORD, W.BOOL, W.DWORD], W.BOOL),
    "TerminateJobObject": ([W.HANDLE, W.UINT], W.BOOL),
    "WaitForSingleObject": ([W.HANDLE, W.DWORD], W.DWORD),
    "CloseHandle": ([W.HANDLE], W.BOOL),
}


class ApiFailure(RuntimeError):
    def __init__(self, operation: str, code: int):
        self.operation, self.code = operation, code
        super().__init__(f"{operation} failed; Win32 error {code}")


@dataclass(frozen=True)
class LocalHandle:
    value: int

    def __post_init__(self):
        if type(self.value) is not int or not 0 < self.value < MAX_HANDLE_VALUE:
            raise ValueError("closable pointer-width current-process handle required")


@dataclass(frozen=True)
class RemoteHandle:
    value: int
    owner_process_handle: int

    def __post_init__(self):
        if (type(self.value) is not int or not 0 < self.value < MAX_HANDLE_VALUE or
                type(self.owner_process_handle) is not int or
                not 0 < self.owner_process_handle < MAX_HANDLE_VALUE):
            raise ValueError("closable pointer-width remote and owner handles required")


def _local(handle: LocalHandle) -> int:
    if not isinstance(handle, LocalHandle):
        raise TypeError("a current-process LocalHandle is required")
    return handle.value


class NativeJobApi:
    """Read/cleanup adapter; no live creation method exists.

    Inject a function table and last_error callback for ABI-level fake tests.
    A successful DuplicateHandle returns a handle tagged for the target table.
    The caller still owns all provenance, access-right and cleanup decisions.
    """

    def __init__(self, functions=None, *, last_error=None):
        if C.sizeof(C.c_void_p) != 8:
            raise RuntimeError("CP310 requires native x64 Python")
        if functions is None:
            if sys.platform != "win32":
                raise RuntimeError("CP310 native API requires Windows")
            functions = C.WinDLL("kernel32", use_last_error=True)
            last_error = C.get_last_error
        elif last_error is None:
            raise ValueError("injected functions require a last_error callback")
        self._fn = {}
        self._last_error = last_error
        for name, (args, result) in SIGNATURES.items():
            fn = functions[name] if isinstance(functions, dict) else getattr(functions, name)
            if not callable(fn):
                raise TypeError(f"{name} must be callable")
            # Python bound-method fakes cannot carry these attributes; their
            # signatures are still checked by tests against SIGNATURES.
            try:
                fn.argtypes, fn.restype = args, result
            except AttributeError:
                pass
            self._fn[name] = fn

    def _ok(self, operation, result):
        if not result:
            # Capture last error before inspecting any output pointer.
            raise ApiFailure(operation, int(self._last_error()))
        return True

    def GetCurrentProcess(self):
        value = self._fn["GetCurrentProcess"]()
        value = getattr(value, "value", value)
        if type(value) is not int or value == 0:
            raise ApiFailure("GetCurrentProcess", int(self._last_error()))
        # ctypes may surface the pointer-sized -1 pseudo handle unsigned.
        if value == (1 << (C.sizeof(C.c_void_p) * 8)) - 1:
            value = -1
        # Win32's pseudo handle is not an ordinary closable LocalHandle.
        return value

    def GetHandleInformation(self, handle: LocalHandle) -> int:
        flags = W.DWORD()
        self._ok("GetHandleInformation", self._fn["GetHandleInformation"](
            _local(handle), C.byref(flags)))
        return int(flags.value)

    def _query_job(self, handle: LocalHandle, info_class: int, output):
        returned = W.DWORD()
        self._ok("QueryInformationJobObject", self._fn["QueryInformationJobObject"](
            _local(handle), info_class, C.byref(output), C.sizeof(output),
            C.byref(returned)))
        # Some Windows versions leave optional return length zero on success.
        if returned.value not in (0, C.sizeof(output)):
            raise RuntimeError("QueryInformationJobObject returned unexpected length")
        return output

    def QueryInformationJobObject(self, handle: LocalHandle) -> int:
        return int(self._query_job(handle, JOB_OBJECT_BASIC_ACCOUNTING_INFORMATION,
                                   BASIC_ACCOUNTING()).ActiveProcesses)

    def GetJobLimitFlags(self, handle: LocalHandle) -> int:
        return int(self._query_job(handle, JOB_OBJECT_EXTENDED_LIMIT_INFORMATION,
                                   EXTENDED_LIMIT()).BasicLimitInformation.LimitFlags)

    def IsProcessInJob(self, process: LocalHandle, job: LocalHandle) -> bool:
        result = W.BOOL()
        self._ok("IsProcessInJob", self._fn["IsProcessInJob"](
            _local(process), _local(job), C.byref(result)))
        return bool(result.value)

    def GetProcessId(self, process: LocalHandle) -> int:
        pid = self._fn["GetProcessId"](_local(process))
        if pid == 0:
            raise ApiFailure("GetProcessId", int(self._last_error()))
        return int(pid)

    def GetProcessTimes(self, process: LocalHandle) -> int:
        created, exited, kernel, user = (FILETIME() for _ in range(4))
        self._ok("GetProcessTimes", self._fn["GetProcessTimes"](
            _local(process), C.byref(created), C.byref(exited),
            C.byref(kernel), C.byref(user)))
        return (int(created.dwHighDateTime) << 32) | int(created.dwLowDateTime)

    def QueryFullProcessImageNameW(self, process: LocalHandle) -> str:
        capacity = 260
        while capacity <= 32768:
            buffer = C.create_unicode_buffer(capacity)
            length = W.DWORD(capacity)
            ok = self._fn["QueryFullProcessImageNameW"](
                _local(process), 0, buffer, C.byref(length))
            if ok:
                if not 0 < length.value < capacity:
                    raise RuntimeError("QueryFullProcessImageNameW invalid output length")
                return buffer[:length.value]
            code = int(self._last_error())
            if code != ERROR_INSUFFICIENT_BUFFER or capacity == 32768:
                raise ApiFailure("QueryFullProcessImageNameW", code)
            capacity = min(capacity * 2, 32768)
        raise AssertionError("unreachable image buffer bound")

    def DuplicateHandle(self, source_process: LocalHandle | int,
                        source_handle: LocalHandle | RemoteHandle,
                        target_process: LocalHandle | int, *,
                        access: int = 0, inherit: bool = False,
                        options: int = DUPLICATE_SAME_ACCESS):
        current = self.GetCurrentProcess()
        source_is_current = source_process == current
        target_is_current = target_process == current
        if source_is_current:
            source_value = _local(source_handle)
        else:
            source_process_value = _local(source_process)
            if (not isinstance(source_handle, RemoteHandle) or
                    source_handle.owner_process_handle != source_process_value):
                raise TypeError("source remote handle is not tagged to source process")
            source_value = source_handle.value
        if not target_is_current:
            target_process_value = _local(target_process)
        else:
            target_process_value = current
        if (type(access) is not int or access != 0 or inherit is not False or
                type(options) is not int or options != DUPLICATE_SAME_ACCESS):
            raise ValueError("only noninheritable SAME_ACCESS duplication is allowed")
        output = W.HANDLE()
        self._ok("DuplicateHandle", self._fn["DuplicateHandle"](
            current if source_is_current else source_process_value,
            source_value, target_process_value, C.byref(output),
            access, False, options))
        value = output.value
        if type(value) is not int or value <= 0:
            raise RuntimeError("DuplicateHandle succeeded without a valid output handle")
        return (LocalHandle(value) if target_is_current else
                RemoteHandle(value, target_process_value))

    def TerminateJobObject(self, job: LocalHandle, exit_code: int) -> bool:
        if type(exit_code) is not int or not 0 <= exit_code <= 0xFFFFFFFF:
            raise ValueError("DWORD exit code required")
        return self._ok("TerminateJobObject", self._fn["TerminateJobObject"](
            _local(job), exit_code))

    def WaitForSingleObject(self, handle: LocalHandle, wait_ms: int) -> int:
        if type(wait_ms) is not int or not 0 <= wait_ms <= 5000:
            raise ValueError("bounded wait [0, 5000] required")
        status = int(self._fn["WaitForSingleObject"](_local(handle), wait_ms))
        if status == WAIT_FAILED:
            raise ApiFailure("WaitForSingleObject", int(self._last_error()))
        if status not in (WAIT_OBJECT_0, WAIT_TIMEOUT):
            raise RuntimeError(f"WaitForSingleObject unexpected status {status}")
        return status

    def CloseHandle(self, handle: LocalHandle) -> bool:
        return self._ok("CloseHandle", self._fn["CloseHandle"](_local(handle)))


def create_live_job_or_process(*_args, **_kwargs):
    raise RuntimeError("CP310 live job/process creation HARD HOLD")
