"""CP313 suspended WORKER preparation, with a deliberately fake-only entry.

The supervisor retains the sole WORKER job handle.  Only the two worker pipe
ends are inherited.  No ResumeThread or child-creation operation is exposed.
The injected API marker is a test seam, not an authorization boundary.
"""
from __future__ import annotations

import ctypes as C
from ctypes import wintypes as W
from dataclasses import dataclass, field
import hashlib
from pathlib import Path, PureWindowsPath
import subprocess


WORKER_IMAGE = r"C:\PROBLEM\s312\worker.exe"
WORKER_SHA256 = "8691e408108d788e7c97cbcefdb2d19dec82972ab2404c17d4da05abab4e1797"
WORKER_BYTES = 105472
WORKER_CWD = r"C:\PROBLEM\s312"
JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE = 0x2000
JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK = 0x1000
JobObjectExtendedLimitInformation = 9
JobObjectBasicAccountingInformation = 1
PROC_THREAD_ATTRIBUTE_HANDLE_LIST = 0x00020002
PROC_THREAD_ATTRIBUTE_JOB_LIST = 0x0002000D
CREATE_SUSPENDED = 4
EXTENDED_STARTUPINFO_PRESENT = 0x80000
CREATE_NO_WINDOW = 0x8000000
CREATE_FLAGS = CREATE_SUSPENDED | EXTENDED_STARTUPINFO_PRESENT | CREATE_NO_WINDOW
STARTF_USESTDHANDLES = 0x100
HANDLE_FLAG_INHERIT = 1
DUPLICATE_SAME_ACCESS = 2
WAIT_OBJECT_0 = 0
WAIT_TIMEOUT = 0x102


class SECURITY_ATTRIBUTES(C.Structure):
    _fields_ = [("nLength", W.DWORD), ("lpSecurityDescriptor", C.c_void_p),
                ("bInheritHandle", W.BOOL)]


class STARTUPINFOW(C.Structure):
    _fields_ = [("cb", W.DWORD), ("lpReserved", W.LPWSTR),
                ("lpDesktop", W.LPWSTR), ("lpTitle", W.LPWSTR),
                ("dwX", W.DWORD), ("dwY", W.DWORD),
                ("dwXSize", W.DWORD), ("dwYSize", W.DWORD),
                ("dwXCountChars", W.DWORD), ("dwYCountChars", W.DWORD),
                ("dwFillAttribute", W.DWORD), ("dwFlags", W.DWORD),
                ("wShowWindow", W.WORD), ("cbReserved2", W.WORD),
                ("lpReserved2", C.POINTER(W.BYTE)), ("hStdInput", W.HANDLE),
                ("hStdOutput", W.HANDLE), ("hStdError", W.HANDLE)]


class STARTUPINFOEXW(C.Structure):
    _fields_ = [("StartupInfo", STARTUPINFOW), ("lpAttributeList", C.c_void_p)]


class PROCESS_INFORMATION(C.Structure):
    _fields_ = [("hProcess", W.HANDLE), ("hThread", W.HANDLE),
                ("dwProcessId", W.DWORD), ("dwThreadId", W.DWORD)]


class BASIC_LIMIT(C.Structure):
    _fields_ = [("PerProcessUserTimeLimit", C.c_longlong),
                ("PerJobUserTimeLimit", C.c_longlong), ("LimitFlags", W.DWORD),
                ("MinimumWorkingSetSize", C.c_size_t),
                ("MaximumWorkingSetSize", C.c_size_t),
                ("ActiveProcessLimit", W.DWORD), ("Affinity", C.c_size_t),
                ("PriorityClass", W.DWORD), ("SchedulingClass", W.DWORD)]


class IO_COUNTERS(C.Structure):
    _fields_ = [(name, C.c_ulonglong) for name in (
        "ReadOperationCount", "WriteOperationCount", "OtherOperationCount",
        "ReadTransferCount", "WriteTransferCount", "OtherTransferCount")]


class EXTENDED_LIMIT(C.Structure):
    _fields_ = [("BasicLimitInformation", BASIC_LIMIT), ("IoInfo", IO_COUNTERS),
                ("ProcessMemoryLimit", C.c_size_t), ("JobMemoryLimit", C.c_size_t),
                ("PeakProcessMemoryUsed", C.c_size_t),
                ("PeakJobMemoryUsed", C.c_size_t)]


class BASIC_ACCOUNTING(C.Structure):
    _fields_ = [("TotalUserTime", C.c_longlong),
                ("TotalKernelTime", C.c_longlong),
                ("ThisPeriodTotalUserTime", C.c_longlong),
                ("ThisPeriodTotalKernelTime", C.c_longlong),
                ("TotalPageFaultCount", W.DWORD), ("TotalProcesses", W.DWORD),
                ("ActiveProcesses", W.DWORD),
                ("TotalTerminatedProcesses", W.DWORD)]


SIGNATURES = {
    "CreateJobObjectW": ([C.c_void_p, W.LPCWSTR], W.HANDLE),
    "SetInformationJobObject": ([W.HANDLE, W.INT, C.c_void_p, W.DWORD], W.BOOL),
    "QueryInformationJobObject": ([W.HANDLE, W.INT, C.c_void_p, W.DWORD, C.POINTER(W.DWORD)], W.BOOL),
    "CreatePipe": ([C.POINTER(W.HANDLE), C.POINTER(W.HANDLE), C.POINTER(SECURITY_ATTRIBUTES), W.DWORD], W.BOOL),
    "SetHandleInformation": ([W.HANDLE, W.DWORD, W.DWORD], W.BOOL),
    "GetHandleInformation": ([W.HANDLE, C.POINTER(W.DWORD)], W.BOOL),
    "InitializeProcThreadAttributeList": ([C.c_void_p, W.DWORD, W.DWORD, C.POINTER(C.c_size_t)], W.BOOL),
    "UpdateProcThreadAttribute": ([C.c_void_p, W.DWORD, C.c_size_t, C.c_void_p, C.c_size_t, C.c_void_p, C.c_void_p], W.BOOL),
    "DeleteProcThreadAttributeList": ([C.c_void_p], None),
    "CreateProcessW": ([W.LPCWSTR, W.LPWSTR, C.c_void_p, C.c_void_p, W.BOOL,
                        W.DWORD, C.c_void_p, W.LPCWSTR, C.POINTER(STARTUPINFOW),
                        C.POINTER(PROCESS_INFORMATION)], W.BOOL),
    "GetProcessId": ([W.HANDLE], W.DWORD),
    "QueryFullProcessImageNameW": ([W.HANDLE, W.DWORD, W.LPWSTR, C.POINTER(W.DWORD)], W.BOOL),
    "IsProcessInJob": ([W.HANDLE, W.HANDLE, C.POINTER(W.BOOL)], W.BOOL),
    "GetCurrentProcess": ([], W.HANDLE),
    "DuplicateHandle": ([W.HANDLE, W.HANDLE, W.HANDLE, C.POINTER(W.HANDLE), W.DWORD, W.BOOL, W.DWORD], W.BOOL),
    "TerminateJobObject": ([W.HANDLE, W.UINT], W.BOOL),
    "WaitForSingleObject": ([W.HANDLE, W.DWORD], W.DWORD),
    "CloseHandle": ([W.HANDLE], W.BOOL),
}


class Hold(RuntimeError):
    pass


class ApiFailure(Hold):
    def __init__(self, operation: str, code: int):
        self.operation, self.code = operation, code
        super().__init__(f"{operation} failed; Win32 error {code}")


@dataclass(frozen=True)
class RemoteHandle:
    value: int
    owner_process: int


@dataclass
class Receipt:
    phase: str = "new"
    local: dict[str, int] = field(default_factory=dict)
    remote_child_job: RemoteHandle | None = None
    pid: int | None = None
    tid: int | None = None
    create_outcome: str = "not-attempted"
    duplicate_outcome: str = "not-attempted"
    errors: list[str] = field(default_factory=list)
    closed: list[str] = field(default_factory=list)
    worker_terminated: bool = False
    worker_waited: bool = False
    jobs_empty: bool = False


class PreparationFailure(Hold):
    def __init__(self, reason: str, receipt: Receipt):
        self.receipt = receipt
        super().__init__(reason)


def create_live_worker(*_args, **_kwargs):
    raise Hold("CP313 native WORKER entry HARD HOLD")


class FakeWorkerCreator:
    """One-use fake WinAPI sequence. The caller owns the returned suspended state."""

    def __init__(self, functions, *, last_error, image_path: str = WORKER_IMAGE):
        if C.sizeof(C.c_void_p) != 8:
            raise Hold("native x64 ABI required")
        if getattr(functions, "cp313_fake_api", False) is not True or not callable(last_error):
            raise Hold("CP313 fake-only injected API required")
        if str(PureWindowsPath(image_path)).casefold() != WORKER_IMAGE.casefold():
            raise Hold("fixed CP312 WORKER image required")
        self._fn = {}
        for name, (args, result) in SIGNATURES.items():
            fn = functions[name] if isinstance(functions, dict) else getattr(functions, name)
            if not callable(fn):
                raise TypeError(f"{name} must be callable")
            try:
                fn.argtypes, fn.restype = args, result
            except AttributeError:
                pass
            self._fn[name] = fn
        self.last_error = last_error
        self.image_path = image_path
        self.receipt = Receipt()
        self._used = False

    def _ok(self, name, result):
        if not result:
            raise ApiFailure(name, int(self.last_error()))

    def _own(self, name, value):
        value = getattr(value, "value", value)
        if type(value) is not int or value <= 0 or value in self.receipt.local.values():
            raise Hold(f"{name} is invalid or aliases an owned local handle")
        self.receipt.local[name] = value
        return value

    def _flags(self, handle):
        flags = W.DWORD()
        self._ok("GetHandleInformation", self._fn["GetHandleInformation"](handle, C.byref(flags)))
        return flags.value

    def _job_limits(self, job):
        info, returned = EXTENDED_LIMIT(), W.DWORD()
        self._ok("QueryInformationJobObject", self._fn["QueryInformationJobObject"](
            job, JobObjectExtendedLimitInformation, C.byref(info), C.sizeof(info), C.byref(returned)))
        if returned.value not in (0, C.sizeof(info)):
            raise Hold("unexpected job information length")
        return info.BasicLimitInformation.LimitFlags

    def _active_count(self, job):
        info, returned = BASIC_ACCOUNTING(), W.DWORD()
        self._ok("QueryInformationJobObject", self._fn["QueryInformationJobObject"](
            job, JobObjectBasicAccountingInformation, C.byref(info), C.sizeof(info),
            C.byref(returned)))
        if returned.value not in (0, C.sizeof(info)):
            raise Hold("unexpected job accounting length")
        return int(info.ActiveProcesses)

    def _pipe(self, stem, security):
        read, write = W.HANDLE(), W.HANDLE()
        try:
            result = self._fn["CreatePipe"](C.byref(read), C.byref(write), C.byref(security), 0)
        except BaseException:
            # A fake interruption may have written either output pointer.
            for suffix, item in (("read", read), ("write", write)):
                if item.value:
                    self._own(f"{stem}_{suffix}", item)
            raise
        for suffix, item in (("read", read), ("write", write)):
            if item.value:
                self._own(f"{stem}_{suffix}", item)
        self._ok("CreatePipe", result)
        if not (read.value and write.value):
            raise Hold("CreatePipe returned incomplete handles")

    def _attribute_list(self, job, worker_stdin, worker_stdout):
        size = C.c_size_t()
        self._fn["InitializeProcThreadAttributeList"](None, 2, 0, C.byref(size))
        if not 0 < size.value <= 1 << 20:
            raise Hold("attribute-list size unavailable or unbounded")
        buffer = C.create_string_buffer(size.value)
        address = C.cast(buffer, C.c_void_p)
        self._ok("InitializeProcThreadAttributeList", self._fn["InitializeProcThreadAttributeList"](
            address, 2, 0, C.byref(size)))
        jobs = (W.HANDLE * 1)(job)
        handles = (W.HANDLE * 2)(worker_stdin, worker_stdout)
        try:
            self._ok("UpdateProcThreadAttribute JOB_LIST", self._fn["UpdateProcThreadAttribute"](
                address, 0, PROC_THREAD_ATTRIBUTE_JOB_LIST, C.cast(jobs, C.c_void_p),
                C.sizeof(jobs), None, None))
            self._ok("UpdateProcThreadAttribute HANDLE_LIST", self._fn["UpdateProcThreadAttribute"](
                address, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST, C.cast(handles, C.c_void_p),
                C.sizeof(handles), None, None))
            yield address, buffer, jobs, handles
        finally:
            self._fn["DeleteProcThreadAttributeList"](address)

    def prepare(self) -> Receipt:
        if self._used:
            raise Hold("creator is one-use")
        self._used = True
        r = self.receipt
        try:
            image = Path(self.image_path)
            if image.stat().st_size != WORKER_BYTES or hashlib.sha256(image.read_bytes()).hexdigest() != WORKER_SHA256:
                raise Hold("reviewed CP312 WORKER bytes do not match")
            r.phase = "staging"
            job = self._own("worker_job", self._fn["CreateJobObjectW"](None, None))
            if self._flags(job) & HANDLE_FLAG_INHERIT:
                raise Hold("WORKER job handle is inheritable")
            limits = EXTENDED_LIMIT()
            limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
            self._ok("SetInformationJobObject", self._fn["SetInformationJobObject"](
                job, JobObjectExtendedLimitInformation, C.byref(limits), C.sizeof(limits)))
            if self._job_limits(job) != JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE:
                raise Hold("WORKER job limits differ from sole kill-on-close limit")
            child_job = self._own("child_job", self._fn["CreateJobObjectW"](None, None))
            if self._flags(child_job) & HANDLE_FLAG_INHERIT:
                raise Hold("CHILD job handle is inheritable")
            self._ok("SetInformationJobObject", self._fn["SetInformationJobObject"](
                child_job, JobObjectExtendedLimitInformation, C.byref(limits), C.sizeof(limits)))
            if self._job_limits(child_job) != JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE:
                raise Hold("CHILD job limits differ from sole kill-on-close limit")
            security = SECURITY_ATTRIBUTES(C.sizeof(SECURITY_ATTRIBUTES), None, True)
            self._pipe("input", security)   # supervisor writes; worker reads
            self._pipe("output", security)  # worker writes; supervisor reads
            for name in ("input_write", "output_read"):
                self._ok("SetHandleInformation", self._fn["SetHandleInformation"](
                    r.local[name], HANDLE_FLAG_INHERIT, 0))
            for name in ("input_write", "output_read", "worker_job", "child_job"):
                if self._flags(r.local[name]) & HANDLE_FLAG_INHERIT:
                    raise Hold(f"{name} remains inheritable")
            for name in ("input_read", "output_write"):
                if not self._flags(r.local[name]) & HANDLE_FLAG_INHERIT:
                    raise Hold(f"{name} is not inheritable")
            attributes = self._attribute_list(job, r.local["input_read"], r.local["output_write"])
            address, buffer, jobs, handles = next(attributes)
            try:
                startup = STARTUPINFOEXW()
                startup.StartupInfo.cb = C.sizeof(startup)
                startup.StartupInfo.dwFlags = STARTF_USESTDHANDLES
                startup.StartupInfo.hStdInput = r.local["input_read"]
                startup.StartupInfo.hStdOutput = r.local["output_write"]
                startup.StartupInfo.hStdError = r.local["output_write"]
                startup.lpAttributeList = address.value
                info = PROCESS_INFORMATION()
                command = C.create_unicode_buffer(subprocess.list2cmdline([self.image_path]))
                r.phase = "creating-worker"
                r.create_outcome = "unknown: CreateProcessW entered"
                try:
                    success = self._fn["CreateProcessW"](
                        self.image_path, command, None, None, True, CREATE_FLAGS,
                        None, WORKER_CWD, C.byref(startup.StartupInfo), C.byref(info))
                finally:
                    for name, value in (("worker_process", info.hProcess), ("worker_thread", info.hThread)):
                        if value:
                            self._own(name, value)
                    if info.dwProcessId:
                        r.pid = int(info.dwProcessId)
                    if info.dwThreadId:
                        r.tid = int(info.dwThreadId)
                self._ok("CreateProcessW", success)
                r.create_outcome = "returned-success"
            finally:
                try:
                    next(attributes)
                except StopIteration:
                    pass
            if not all(name in r.local for name in ("worker_process", "worker_thread")) or not r.pid or not r.tid:
                raise Hold("incomplete creation-returned worker identity")
            if self._fn["GetProcessId"](r.local["worker_process"]) != r.pid:
                raise Hold("creation-returned worker PID mismatch")
            image_buffer = C.create_unicode_buffer(32768)
            length = W.DWORD(len(image_buffer))
            self._ok("QueryFullProcessImageNameW", self._fn["QueryFullProcessImageNameW"](
                r.local["worker_process"], 0, image_buffer, C.byref(length)))
            if not 0 < length.value < len(image_buffer) or image_buffer[:length.value].casefold() != self.image_path.casefold():
                raise Hold("creation-returned worker image mismatch")
            member = W.BOOL()
            self._ok("IsProcessInJob", self._fn["IsProcessInJob"](
                r.local["worker_process"], job, C.byref(member)))
            if not member.value:
                raise Hold("worker is not in exact WORKER job")
            self._ok("IsProcessInJob", self._fn["IsProcessInJob"](
                r.local["worker_process"], child_job, C.byref(member)))
            if member.value:
                raise Hold("worker unexpectedly joined CHILD job")
            if "worker_job" not in r.local or self._flags(job) & HANDLE_FLAG_INHERIT:
                raise Hold("sole supervisor WORKER job handle not retained")
            current = self._fn["GetCurrentProcess"]()
            current = getattr(current, "value", current)
            duplicate = W.HANDLE()
            r.duplicate_outcome = "unknown: DuplicateHandle entered"
            try:
                duplicated = self._fn["DuplicateHandle"](
                    current, child_job, r.local["worker_process"], C.byref(duplicate),
                    0, False, DUPLICATE_SAME_ACCESS)
            finally:
                if duplicate.value:
                    r.remote_child_job = RemoteHandle(duplicate.value, r.local["worker_process"])
            self._ok("DuplicateHandle", duplicated)
            if not duplicate.value:
                raise Hold("worker-local CHILD job duplicate absent")
            r.duplicate_outcome = "returned-success"
            for name in ("input_read", "output_write"):
                self._close_one(name)
            r.phase = "prepared-suspended"
            return r
        except BaseException as error:
            r.errors.append(str(error))
            r.phase = "hold"
            if r.create_outcome == "not-attempted":
                self.close()
            raise PreparationFailure(str(error), r) from error

    def _close_one(self, name):
        value = self.receipt.local.get(name)
        if value is None:
            return
        self._ok("CloseHandle", self._fn["CloseHandle"](value))
        del self.receipt.local[name]
        self.receipt.closed.append(name)

    def close(self) -> Receipt:
        """Abort owned state; retain every handle until worker and jobs resolve."""
        r = self.receipt
        if r.create_outcome != "not-attempted" and "worker_job" in r.local:
            if not r.worker_terminated:
                try:
                    self._ok("TerminateJobObject", self._fn["TerminateJobObject"](
                        r.local["worker_job"], 0xE313))
                    r.worker_terminated = True
                except BaseException as error:
                    r.errors.append(str(error))
            if "worker_process" in r.local and r.worker_terminated and not r.worker_waited:
                try:
                    status = self._fn["WaitForSingleObject"](r.local["worker_process"], 2000)
                    r.worker_waited = type(status) is int and status == WAIT_OBJECT_0
                    if not r.worker_waited:
                        r.errors.append(f"worker wait status {status!r}")
                except BaseException as error:
                    r.errors.append(f"worker wait interrupted: {error}")
            if r.worker_waited and not r.jobs_empty:
                try:
                    worker_active = self._active_count(r.local["worker_job"])
                    child_active = self._active_count(r.local["child_job"])
                    r.jobs_empty = worker_active == child_active == 0
                    if not r.jobs_empty:
                        r.errors.append(f"jobs still active: WORKER={worker_active}, CHILD={child_active}")
                except BaseException as error:
                    r.errors.append(f"job accounting unresolved: {error}")
            if not (r.worker_waited and r.jobs_empty):
                r.phase = "hold"
                return r
        for name in list(r.local):
            try:
                self._close_one(name)
            except BaseException as error:
                r.errors.append(str(error))
        if r.remote_child_job is not None and r.worker_waited and r.jobs_empty:
            r.remote_child_job = None  # Worker table disappeared; never CloseHandle remotely here.
        r.phase = "closed" if not r.local else "hold"
        return r
