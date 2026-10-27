"""CP303 fake-only Job Object creation seam. No real child entry is enabled.

The supervisor owns the unnamed kill-on-close job. A noninheritable duplicate is
placed in its already-owned worker, where JOB_LIST assigns a fixed synthetic
clone during CreateProcessW. A creator lost inside CreateProcessW leaves the
child outcome unknown; the supervisor must retain its job and investigate.
"""
from __future__ import annotations

import ctypes as C
from ctypes import wintypes as W
from dataclasses import dataclass
from pathlib import PureWindowsPath
import re
import subprocess
import sys


JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE = 0x00002000
JobObjectExtendedLimitInformation = 9
PROC_THREAD_ATTRIBUTE_JOB_LIST = 0x0002000D
CREATE_SUSPENDED = 0x00000004
CREATE_NO_WINDOW = 0x08000000
EXTENDED_STARTUPINFO_PRESENT = 0x00080000
DUPLICATE_SAME_ACCESS = 0x00000002
HANDLE_FLAG_INHERIT = 0x00000001
CP302_SHA256 = "66eadac3f5dd6ee0d0b856a53760dc9270ecfd170808c62dff01b99a583f6a28"


class IO_COUNTERS(C.Structure):
    _fields_ = [(name, C.c_ulonglong) for name in (
        "ReadOperationCount", "WriteOperationCount", "OtherOperationCount",
        "ReadTransferCount", "WriteTransferCount", "OtherTransferCount")]


class JOBOBJECT_BASIC_LIMIT_INFORMATION(C.Structure):
    _fields_ = [("PerProcessUserTimeLimit", C.c_longlong),
                ("PerJobUserTimeLimit", C.c_longlong),
                ("LimitFlags", W.DWORD), ("MinimumWorkingSetSize", C.c_size_t),
                ("MaximumWorkingSetSize", C.c_size_t),
                ("ActiveProcessLimit", W.DWORD),
                ("Affinity", C.c_size_t), ("PriorityClass", W.DWORD),
                ("SchedulingClass", W.DWORD)]


class JOBOBJECT_EXTENDED_LIMIT_INFORMATION(C.Structure):
    _fields_ = [("BasicLimitInformation", JOBOBJECT_BASIC_LIMIT_INFORMATION),
                ("IoInfo", IO_COUNTERS), ("ProcessMemoryLimit", C.c_size_t),
                ("JobMemoryLimit", C.c_size_t),
                ("PeakProcessMemoryUsed", C.c_size_t),
                ("PeakJobMemoryUsed", C.c_size_t)]


class STARTUPINFOW(C.Structure):
    _fields_ = [("cb", W.DWORD), ("lpReserved", W.LPWSTR),
                ("lpDesktop", W.LPWSTR), ("lpTitle", W.LPWSTR),
                ("dwX", W.DWORD), ("dwY", W.DWORD),
                ("dwXSize", W.DWORD), ("dwYSize", W.DWORD),
                ("dwXCountChars", W.DWORD), ("dwYCountChars", W.DWORD),
                ("dwFillAttribute", W.DWORD), ("dwFlags", W.DWORD),
                ("wShowWindow", W.WORD), ("cbReserved2", W.WORD),
                ("lpReserved2", C.POINTER(W.BYTE)),
                ("hStdInput", W.HANDLE), ("hStdOutput", W.HANDLE),
                ("hStdError", W.HANDLE)]


class STARTUPINFOEXW(C.Structure):
    _fields_ = [("StartupInfo", STARTUPINFOW),
                ("lpAttributeList", C.c_void_p)]


class PROCESS_INFORMATION(C.Structure):
    _fields_ = [("hProcess", W.HANDLE), ("hThread", W.HANDLE),
                ("dwProcessId", W.DWORD), ("dwThreadId", W.DWORD)]


class ApiFailure(RuntimeError):
    def __init__(self, operation, code):
        self.operation, self.code = operation, code
        super().__init__(f"{operation} failed; Win32 error {code}")


class CreateOutcomeUnknown(ApiFailure):
    """The creator may have died or failed while Windows was creating a child."""

    def __init__(self, code, process_information=None):
        self.process_information = process_information
        super().__init__("CreateProcessW; child existence/job assignment unknown", code)


class Kernel32Api:
    """Typed Windows ABI; CreateProcessW stays disabled even through a wrapper."""

    def __init__(self):
        if sys.platform != "win32":
            raise RuntimeError("Windows only")
        dll = C.WinDLL("kernel32", use_last_error=True)
        specs = {
            "CreateJobObjectW": ([C.c_void_p, W.LPCWSTR], W.HANDLE),
            "SetInformationJobObject": ([W.HANDLE, W.INT, C.c_void_p, W.DWORD], W.BOOL),
            "GetHandleInformation": ([W.HANDLE, C.POINTER(W.DWORD)], W.BOOL),
            "DuplicateHandle": ([W.HANDLE, W.HANDLE, W.HANDLE, C.POINTER(W.HANDLE),
                                 W.DWORD, W.BOOL, W.DWORD], W.BOOL),
            "GetCurrentProcess": ([], W.HANDLE),
            "CloseHandle": ([W.HANDLE], W.BOOL),
            "InitializeProcThreadAttributeList": ([C.c_void_p, W.DWORD, W.DWORD,
                                                    C.POINTER(C.c_size_t)], W.BOOL),
            "UpdateProcThreadAttribute": ([C.c_void_p, W.DWORD, C.c_size_t,
                                           C.c_void_p, C.c_size_t, C.c_void_p,
                                           C.c_void_p], W.BOOL),
            "DeleteProcThreadAttributeList": ([C.c_void_p], None),
        }
        for name, (args, result) in specs.items():
            fn = getattr(dll, name)
            fn.argtypes, fn.restype = args, result
            setattr(self, name, fn)

    @staticmethod
    def last_error():
        return C.get_last_error()

    @staticmethod
    def CreateProcessW(*_args, **_kwargs):
        # A fake-marked forwarding wrapper must not recover a raw launch method.
        raise RuntimeError("CP303 Kernel32Api.CreateProcessW real invocation HOLD")


def _fake_only(api):
    if isinstance(api, Kernel32Api) or getattr(api, "cp303_fake_api", False) is not True:
        raise RuntimeError("CP303 real Job Object and child creation HOLD")


def _checked(api, operation, result):
    if not result:
        raise ApiFailure(operation, api.last_error())
    return result


@dataclass(frozen=True)
class SupervisorJob:
    """Exact supervisor-local handle. Its owner must retain it through cleanup."""

    handle: int


def stage_supervisor_job_for_fake_test(api):
    """Prepare an unnamed, noninheritable job before worker creation."""
    _fake_only(api)
    handle = api.CreateJobObjectW(None, None)
    _checked(api, "CreateJobObjectW", handle)
    try:
        flags = W.DWORD()
        _checked(api, "GetHandleInformation", api.GetHandleInformation(handle, C.byref(flags)))
        if flags.value & HANDLE_FLAG_INHERIT:
            raise RuntimeError("supervisor job handle is inheritable")
        limits = JOBOBJECT_EXTENDED_LIMIT_INFORMATION()
        limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
        _checked(api, "SetInformationJobObject", api.SetInformationJobObject(
            handle, JobObjectExtendedLimitInformation, C.byref(limits), C.sizeof(limits)))
        return SupervisorJob(int(handle))
    except BaseException:
        # Before a worker exists, the supervisor's exact handle is the sole owner.
        # A failed close is reported without pretending that containment was set.
        if not api.CloseHandle(handle):
            raise ApiFailure("CloseHandle after job setup failure", api.last_error())
        raise


def duplicate_job_to_worker_for_fake_test(api, job: SupervisorJob, worker_process_handle: int):
    """Copy the exact job into the already-owned worker, without inheritance."""
    _fake_only(api)
    if not isinstance(job, SupervisorJob) or job.handle <= 0 or worker_process_handle <= 0:
        raise ValueError("exact supervisor job and owned worker process handle required")
    target = W.HANDLE()
    _checked(api, "DuplicateHandle", api.DuplicateHandle(
        api.GetCurrentProcess(), job.handle, worker_process_handle, C.byref(target),
        0, False, DUPLICATE_SAME_ACCESS))
    if not target.value:
        raise RuntimeError("DuplicateHandle returned a null worker job handle")
    # The returned value is valid in the worker only. The supervisor must not
    # CloseHandle it in its own process or treat it as proof of worker receipt.
    return int(target.value)


@dataclass(frozen=True)
class FixedCloneImage:
    executable: str
    cwd: str
    sha256: str


def _validate_supplied_fixed_image_contract(image):
    """Check caller-supplied CP302 clone fields, not disk or loaded-image bytes."""
    if not isinstance(image, FixedCloneImage) or image.sha256 != CP302_SHA256:
        raise ValueError("reviewed CP302 fixture identity required")
    exe, cwd = PureWindowsPath(image.executable), PureWindowsPath(image.cwd)
    if (not exe.is_absolute() or not cwd.is_absolute() or
            str(exe.parent).casefold() != str(cwd).casefold() or
            cwd.parent != PureWindowsPath("C:\\PROBLEM") or
            re.fullmatch(r"m289_[0-9a-f]{16}", cwd.name) is None or
            exe.name.casefold() != "fixture.exe" or len(str(exe)) >= 100):
        raise ValueError("caller must supply a fixed CP302 clone image under C:\\PROBLEM")
    return str(exe), str(cwd)


def worker_create_suspended_for_fake_test(api, image: FixedCloneImage, worker_job_handle: int):
    """Prepare JOB_LIST then create exactly one suspended clone; never resume it.

    The result's exact process/thread handles belong to the worker. If the
    creator is interrupted inside CreateProcessW, the outcome is unknown even
    though the supervisor still owns its job. No post-create assignment exists.
    """
    _fake_only(api)
    exe, cwd = _validate_supplied_fixed_image_contract(image)
    if not isinstance(worker_job_handle, int) or worker_job_handle <= 0:
        raise ValueError("worker-local job handle required")
    size = C.c_size_t()
    api.InitializeProcThreadAttributeList(None, 1, 0, C.byref(size))
    if size.value == 0:
        raise ApiFailure("InitializeProcThreadAttributeList size", api.last_error())
    buffer = C.create_string_buffer(size.value)
    attribute_list = C.cast(buffer, C.c_void_p)
    _checked(api, "InitializeProcThreadAttributeList", api.InitializeProcThreadAttributeList(
        attribute_list, 1, 0, C.byref(size)))
    try:
        job_array = (W.HANDLE * 1)(worker_job_handle)
        _checked(api, "UpdateProcThreadAttribute", api.UpdateProcThreadAttribute(
            attribute_list, 0, PROC_THREAD_ATTRIBUTE_JOB_LIST,
            C.cast(job_array, C.c_void_p), C.sizeof(job_array), None, None))
        startup = STARTUPINFOEXW()
        startup.StartupInfo.cb = C.sizeof(STARTUPINFOEXW)
        startup.lpAttributeList = attribute_list.value
        process = PROCESS_INFORMATION()
        command = C.create_unicode_buffer(subprocess.list2cmdline([exe]))
        flags = CREATE_SUSPENDED | EXTENDED_STARTUPINFO_PRESENT | CREATE_NO_WINDOW
        try:
            success = api.CreateProcessW(exe, command, None, None, False,
                                         flags, None, cwd, C.byref(startup),
                                         C.byref(process))
        except BaseException as error:
            unknown = CreateOutcomeUnknown(api.last_error(), process)
            unknown.__cause__ = error
            raise unknown
        if not success:
            raise CreateOutcomeUnknown(api.last_error(), process)
        return process
    finally:
        api.DeleteProcThreadAttributeList(attribute_list)


def create_real_child(*_args, **_kwargs):
    raise RuntimeError("CP303 real synthetic child HOLD pending independent review")
