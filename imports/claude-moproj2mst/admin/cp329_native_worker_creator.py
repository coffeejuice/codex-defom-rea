"""CP329 native-capable suspended CP316 creator; public live entry HARD HOLD.

Import/construction are inert. Explicit prepare uses CP325 bound exports and
retains every owner through unresolved cleanup. No worker is resumed here.
The caller allocates the sole GetTickCount64 deadline before preparing any
security, pipe or image owner; this module never allocates or renews it.
"""
from __future__ import annotations

import ctypes as C
from dataclasses import dataclass, field
import subprocess

import cp321_native_exports as abi
import cp327_native_image_pin as image
import cp328_native_pipes as pipes

NATIVE_ENTRY = "HARD HOLD"
Hold = abi.Hold
WORKER_IMAGE, WORKER_CWD = image.WORKER_IMAGE, r"C:\PROBLEM\s316"
KILL_ON_CLOSE, INHERIT = 0x2000, 1
JOB_LIMITS, JOB_ACCOUNTING = 9, 1
HANDLE_LIST, JOB_LIST = 0x20002, 0x2000D
CREATE_FLAGS = 4 | 0x80000 | 0x08000000  # suspended, extended startup, no window
INVALID_HANDLE = C.c_void_p(-1).value


class ApiFailure(Hold):
    def __init__(self, name, code):
        self.api, self.code = name, code
        super().__init__(f"{name} failed ({code})")


@dataclass(frozen=True)
class RemoteHandle:
    value: int
    owner_process: int


@dataclass
class Receipt:
    phase: str = "new"
    local: dict = field(default_factory=dict)
    remote_child_job: RemoteHandle | None = None
    pid: int | None = None
    tid: int | None = None
    create_outcome: str = "not-attempted"
    duplicate_outcome: str = "not-attempted"
    worker_terminated: bool = False
    worker_waited: bool = False
    jobs_empty: bool = False
    # Diagnostic-only success-completion milestones. Cleanup never changes
    # these fields, and no acceptance check consumes them.
    completion_wait_outcome: str = "not-attempted"
    completion_wait_remaining: int | None = None
    completion_wait_status: int | None = None
    completion_post_wait_budget: str = "not-attempted"
    completion_post_wait_remaining: int | None = None
    completion_post_wait_stage: str = "not-attempted"
    errors: list = field(default_factory=list)


class PreparationFailure(Hold):
    def __init__(self, error, owner):
        self.owner, self.receipt = owner, owner.receipt
        super().__init__(str(error))


class NativeWorkerCreator:
    """One-use owner, explicitly usable with real or injected BoundExports.

    Supply fresh, unprepared CP326/327/328 owners sharing the same exports.
    After any failure retain this object and retry close until phase=closed.
    A remote CHILD handle is worker-local and is never closed in the parent.
    Image-pin verification establishes retained-file identity, not attestation
    of the process's mapped bytes; CP327's qualifications still apply.
    """
    def __init__(self, exports, *, security, pipe_owner, image_pin, deadline):
        if type(deadline) is not int or not 0 < deadline < 2**64:
            raise ValueError("caller-allocated absolute GetTickCount64 deadline required")
        if not callable(getattr(exports, "fn", None)) or not callable(exports.last_error):
            raise TypeError("CP325 BoundExports fn/last_error required")
        if any(getattr(owner, "exports", None) is not exports
               for owner in (security, pipe_owner, image_pin)):
            raise Hold("owners must retain the same BoundExports")
        if pipe_owner.security is not security or pipe_owner.deadline != deadline:
            raise Hold("pipe owner must share security and the original deadline")
        if (security.started or pipe_owner.started or image_pin._used or
                image_pin.handle is not None or image_pin.reopened_handle is not None):
            raise Hold("fresh unprepared owners required")
        self.exports, self.security = exports, security
        self.pipe_owner, self.image_pin, self._deadline = pipe_owner, image_pin, deadline
        self.receipt, self._used = Receipt(), False
        self._attributes, self._last_tick = None, None
        self._endpoint_handles = set()

    @property
    def deadline(self):
        return self._deadline

    def _call(self, name, *args):
        return self.exports.fn(name)(*args)

    def _ok(self, name, *args):
        value = self._call(name, *args)
        if not value:
            raise ApiFailure(name, int(self.exports.last_error()))
        return value

    def _budget(self):
        now = self._call("GetTickCount64")
        if (type(now) is not int or not 0 <= now < 2**64 or
                not 0 < self.deadline - now <= 5000 or
                (self._last_tick is not None and now < self._last_tick) or
                self.pipe_owner.deadline != self.deadline):
            raise Hold("original absolute deadline expired, changed or exceeds 5000ms")
        self._last_tick = now

    def _own(self, name, value):
        value = getattr(value, "value", value)
        if (type(value) is not int or value <= 0 or value == INVALID_HANDLE or
                value in self.receipt.local.values() or value in self._endpoint_handles):
            raise Hold(f"invalid or aliased {name} handle")
        self.receipt.local[name] = value
        return value

    def _flags(self, handle):
        flags = abi.DWORD()
        self._ok("GetHandleInformation", handle, C.byref(flags))
        return flags.value

    def _query(self, job, kind, structure):
        info, size = structure(), abi.DWORD()
        self._ok("QueryInformationJobObject", job, kind, C.byref(info),
                 C.sizeof(info), C.byref(size))
        if size.value != C.sizeof(info):
            raise Hold("exact returned job information size required")
        return info

    def _active(self, job):
        return self._query(job, JOB_ACCOUNTING, abi.BASIC_ACCOUNTING).ActiveProcesses

    def _job_verified(self, job):
        if self._flags(job) & INHERIT:
            raise Hold("jobs must be noninheritable")
        limits = self._query(job, JOB_LIMITS, abi.EXTENDED_LIMIT)
        if limits.BasicLimitInformation.LimitFlags != KILL_ON_CLOSE:
            raise Hold("sole kill-on-close job limit required")

    def _job(self, name):
        self._budget()
        handle = self._call("CreateJobObjectW", None, None)
        if getattr(handle, "value", handle) in (None, 0, -1, INVALID_HANDLE):
            raise ApiFailure("CreateJobObjectW", int(self.exports.last_error()))
        job = self._own(name, handle)
        limits = abi.EXTENDED_LIMIT()
        limits.BasicLimitInformation.LimitFlags = KILL_ON_CLOSE
        self._ok("SetInformationJobObject", job, JOB_LIMITS, C.byref(limits), C.sizeof(limits))
        self._job_verified(job)
        if self._active(job) != 0:
            raise Hold("new job must be empty")
        return job

    def _attribute_list(self, job, endpoints):
        size = abi.SIZE_T()
        ok = self._call("InitializeProcThreadAttributeList", None, 2, 0, C.byref(size))
        error = int(self.exports.last_error())
        if ok or error != 122 or not 0 < size.value <= 1048576:
            raise Hold("expected bounded attribute-list size probe")
        buffer = C.create_string_buffer(size.value)
        address = C.cast(buffer, abi.PVOID)
        jobs = (abi.HANDLE * 1)(job)
        handles = (abi.HANDLE * 2)(endpoints.client_read, endpoints.client_write)
        # Retain storage before initialization; uncertain initialization also
        # retains the storage instead of freeing memory still referenced by OS.
        self._attributes = [address, buffer, jobs, handles, None]
        ok = self._call("InitializeProcThreadAttributeList", address, 2, 0, C.byref(size))
        if not ok:
            error = int(self.exports.last_error())
            self._attributes[4] = False
            raise ApiFailure("InitializeProcThreadAttributeList", error)
        self._attributes[4] = True
        for kind, values in ((JOB_LIST, jobs), (HANDLE_LIST, handles)):
            self._budget()
            self._ok("UpdateProcThreadAttribute", address, 0, kind,
                     C.cast(values, abi.PVOID), C.sizeof(values), None, None)
        return address

    def _delete_attributes(self):
        if self._attributes is not None:
            if self._attributes[4] is None:
                raise Hold("uncertain attribute-list initialization/deletion retains storage")
            if self._attributes[4]:
                self._attributes[4] = None
                self._call("DeleteProcThreadAttributeList", self._attributes[0])
            self._attributes = None

    def _verify_process(self, job, child):
        r = self.receipt
        if not all(name in r.local for name in ("worker_process", "worker_thread")) or not r.pid or not r.tid:
            raise Hold("complete creation-returned identity required")
        process = r.local["worker_process"]
        if self._call("GetProcessId", process) != r.pid:
            raise Hold("creation-returned PID mismatch")
        if any(self._flags(r.local[n]) & INHERIT for n in ("worker_process", "worker_thread")):
            raise Hold("process/thread must be noninheritable")
        path, count = C.create_unicode_buffer(32768), abi.DWORD(32768)
        self._ok("QueryFullProcessImageNameW", process, 0, path, C.byref(count))
        if not 0 < count.value < len(path) or path.value.casefold() != WORKER_IMAGE.casefold() or count.value != len(path.value):
            raise Hold("fixed creation-returned image path mismatch")
        for target, expected in ((job, True), (child, False)):
            member = abi.BOOL()
            self._ok("IsProcessInJob", process, target, C.byref(member))
            if member.value not in (0, 1) or bool(member.value) != expected:
                raise Hold("exact WORKER/CHILD membership mismatch")
            self._job_verified(target)
        if self._active(job) != 1 or self._active(child) != 0:
            raise Hold("sole WORKER and empty CHILD required")
        self._budget()
        self.image_pin.verify_after_create()  # still suspended, never resumed

    def prepare(self, base_name):
        if self._used:
            raise Hold("creator is one-use")
        self._used = True
        r = self.receipt
        try:
            self._budget()  # before security, pipe or image preparation
            self.security.prepare()
            self._budget()
            endpoints = self.pipe_owner.prepare(base_name)
            if not isinstance(endpoints, pipes.ConnectedEndpoints):
                raise Hold("CP328 typed connected endpoints required")
            flat = (endpoints.parent_write, endpoints.client_read,
                    endpoints.parent_read, endpoints.client_write)
            if (any(type(h) is not int or h <= 0 or h == INVALID_HANDLE for h in flat)
                    or len(set(flat)) != 4):
                raise Hold("distinct valid pipe endpoints required")
            self._endpoint_handles = set(flat)
            for index, handle in enumerate(flat):
                if bool(self._flags(handle) & INHERIT) != bool(index % 2):
                    raise Hold("only the two client pipe handles may inherit")
            self._budget()
            self.image_pin.prepare()
            job, child = self._job("worker_job"), self._job("child_job")
            if set(flat) & set(r.local.values()):
                raise Hold("job/pipe handle alias")
            address = self._attribute_list(job, endpoints)
            startup = abi.STARTUPINFOEXW()
            startup.StartupInfo.cb = C.sizeof(startup)
            startup.StartupInfo.dwFlags = 0x100
            startup.StartupInfo.hStdInput = endpoints.client_read
            startup.StartupInfo.hStdOutput = startup.StartupInfo.hStdError = endpoints.client_write
            startup.lpAttributeList = address.value
            command = C.create_unicode_buffer(subprocess.list2cmdline([WORKER_IMAGE, str(self.deadline)]))
            info = abi.PROCESS_INFORMATION()
            self._budget()
            r.create_outcome = "unknown: CreateProcessW entered"
            try:
                success = self._call("CreateProcessW", WORKER_IMAGE, command, None, None,
                    True, CREATE_FLAGS, None, WORKER_CWD, C.byref(startup.StartupInfo), C.byref(info))
                error = None if success else int(self.exports.last_error())
            finally:
                # Capture independently even when the call raises after outputs.
                capture_errors = []
                for name, value in (("worker_process", info.hProcess), ("worker_thread", info.hThread)):
                    if value:
                        try:
                            self._own(name, value)
                        except BaseException as exc:
                            capture_errors.append(str(exc))
                r.pid, r.tid = int(info.dwProcessId) or None, int(info.dwThreadId) or None
                if capture_errors:
                    raise Hold("; ".join(capture_errors))
            if not success:
                raise ApiFailure("CreateProcessW", error)
            r.create_outcome = "returned-success"
            self._budget()
            self._verify_process(job, child)
            self._delete_attributes()
            duplicate = abi.HANDLE()
            current = self._call("GetCurrentProcess")
            self._budget()
            r.duplicate_outcome = "unknown: DuplicateHandle entered"
            try:
                success = self._call("DuplicateHandle", current, child, r.local["worker_process"],
                                     C.byref(duplicate), 0, False, 2)
                error = None if success else int(self.exports.last_error())
            finally:
                if duplicate.value:
                    r.remote_child_job = RemoteHandle(duplicate.value, r.local["worker_process"])
            if not success:
                raise ApiFailure("DuplicateHandle", error)
            if duplicate.value in (None, 0, INVALID_HANDLE):
                raise Hold("valid worker-local CHILD token required")
            r.duplicate_outcome = "returned-success"
            self._budget()
            r.phase = "prepared-suspended"
            return r
        except BaseException as error:
            r.errors.append(str(error))
            r.phase = "hold"
            self.close()
            raise PreparationFailure(error, self) from error

    def complete_success(self):
        """Observe a clean exact WORKER exit, then release terminal owners.

        This method owns the evidence flags; callers cannot provide an exit or
        job-count assertion. It never resumes a thread or allocates a deadline.
        On any error the caller must retain this owner and invoke close().
        """
        r = self.receipt
        if r.phase != "prepared-suspended" or r.create_outcome != "returned-success":
            raise Hold("success completion requires the exact prepared WORKER owner")
        self._budget()
        process = r.local["worker_process"]
        remaining = self.deadline - self._last_tick
        r.completion_wait_remaining = remaining
        r.completion_wait_outcome = "entered"
        status = self._call("WaitForSingleObject", process, remaining)
        r.completion_wait_outcome = "returned"
        r.completion_wait_status = (status if type(status) is int and
                                    0 <= status <= 0xFFFFFFFF else None)
        r.completion_post_wait_budget = "entered"
        try:
            self._budget()  # unchanged clock query and acceptance boundary
        except BaseException:
            r.completion_post_wait_budget = "rejected"
            raise
        r.completion_post_wait_budget = "passed"
        r.completion_post_wait_remaining = self.deadline - self._last_tick
        r.completion_post_wait_stage = "wait-status-check"
        if type(status) is not int or status != 0:
            raise Hold("exact bounded worker WAIT_OBJECT_0 required")
        code = abi.DWORD()
        r.completion_post_wait_stage = "exit-query-entered"
        self._ok("GetExitCodeProcess", process, C.byref(code))
        r.completion_post_wait_stage = "exit-query-returned"
        r.completion_post_wait_stage = "exit-budget-entered"
        self._budget()
        r.completion_post_wait_stage = "exit-budget-passed"
        if code.value != 0:
            raise Hold("exact WORKER exit code zero required")
        r.completion_post_wait_stage = "worker-accounting-entered"
        worker_count = self._active(r.local["worker_job"])
        r.completion_post_wait_stage = "worker-accounting-returned"
        r.completion_post_wait_stage = "worker-budget-entered"
        self._budget()
        r.completion_post_wait_stage = "worker-budget-passed"
        r.completion_post_wait_stage = "child-accounting-entered"
        child_count = self._active(r.local["child_job"])
        r.completion_post_wait_stage = "child-accounting-returned"
        r.completion_post_wait_stage = "child-budget-entered"
        self._budget()
        r.completion_post_wait_stage = "child-budget-passed"
        r.completion_post_wait_stage = "job-count-check"
        if worker_count != 0 or child_count != 0:
            raise Hold("both WORKER and CHILD jobs must be empty")
        r.completion_post_wait_stage = "pending-pipe-check"
        if any(not op.terminal for op in self.pipe_owner.operations):
            raise Hold("all pipe I/O must have terminal evidence")
        r.worker_waited, r.jobs_empty = True, True
        r.remote_child_job = None  # exact worker death releases remote ownership
        r.completion_post_wait_stage = "success-flags-set"
        r.completion_post_wait_stage = "close-entered"
        completed = self.close()
        r.completion_post_wait_stage = "close-returned"
        return completed

    def close(self):
        """Nonblocking retryable cleanup; failed waits retain all handles.

        Terminate WORKER before consulting pending pipe I/O. Exact WAIT_OBJECT_0
        and both zero active counts precede releasing either containment job.
        Unknown creation without a process wait handle remains held indefinitely.
        """
        r = self.receipt
        if r.phase == "closed":
            return r
        try:
            if r.create_outcome != "not-attempted" and not (r.worker_waited and r.jobs_empty):
                if not r.worker_terminated:
                    self._ok("TerminateJobObject", r.local["worker_job"], 0xE329)
                    r.worker_terminated = True
                if "worker_process" not in r.local:
                    raise Hold("unknown creation outcome lacks exact process wait handle")
                if not r.worker_waited:
                    status = self._call("WaitForSingleObject", r.local["worker_process"], 0)
                    if type(status) is not int or status != 0:
                        raise Hold("exact worker WAIT_OBJECT_0 required")
                    r.worker_waited = True
                r.jobs_empty = self._active(r.local["worker_job"]) == 0 and self._active(r.local["child_job"]) == 0
                if not r.jobs_empty:
                    raise Hold("WORKER/CHILD active counts unresolved")
                r.remote_child_job = None
            self._delete_attributes()
            if not self.pipe_owner.cleanup():
                raise Hold("pending pipe ownership retained")
            # Keep image pin and security until pipe/process/job cleanup finishes.
            for name, handle in tuple(r.local.items()):
                self._ok("CloseHandle", handle)
                del r.local[name]
            self.image_pin.close()
            if not self.security.cleanup():
                raise Hold("security cleanup unresolved")
            r.phase = "closed"
        except BaseException as error:
            r.errors.append(str(error))
            r.phase = "hold"
        return r


def create_live_worker(*_args, **_kwargs):
    raise Hold("CP329 live worker factory HARD HOLD pending independent integration review")
