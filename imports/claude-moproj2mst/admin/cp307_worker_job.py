"""CP307 fake-only creation-bound WORKER job seam.

The supervisor owns this unnamed kill-on-close job and the creation-returned
worker handles.  It is distinct from CP303/304's CHILD job.  A suspended fake
worker is assigned through STARTUPINFOEX JOB_LIST at creation; only an explicit
membership check opens the gate that would precede child creation.  This file
has no live CreateProcessW entry and never creates a child.
Fake markers and injected methods are conventions, not a security boundary;
only reviewed in-memory fakes may be supplied.
"""
from __future__ import annotations

import ctypes as C
from ctypes import wintypes as W
from dataclasses import dataclass, field

import cp303_job_creation as abi


FAKE_WORKER_IMAGE = r"C:\PROBLEM\cp307_fake_worker.exe"
FAKE_WORKER_CWD = r"C:\PROBLEM"
WAIT_OBJECT_0 = 0


def _fake_only(api):
    if isinstance(api, abi.Kernel32Api) or getattr(api, "cp307_fake_api", False) is not True:
        raise RuntimeError("CP307 live WORKER CreateProcessW HOLD")
    launch = getattr(api, "CreateProcessW", None)
    if (not callable(launch) or launch is abi.Kernel32Api.CreateProcessW or
            isinstance(getattr(launch, "__self__", None), abi.Kernel32Api) or
            getattr(launch, "__func__", None) is abi.Kernel32Api.CreateProcessW):
        raise RuntimeError("CP307 bound live CreateProcessW HOLD")


def _handle(value):
    value = getattr(value, "value", value)
    return value if type(value) is int and value > 0 else None


@dataclass
class WorkerJobReport:
    phase: str = "new"
    worker_assignment: str = "not-attempted"
    worker_gate: str = "closed"
    create_outcome: str = "not-attempted"
    retained_handles: dict[str, int] = field(default_factory=dict)
    close_results: dict[str, str] = field(default_factory=dict)
    errors: list[str] = field(default_factory=list)


class FakeWorkerJob:
    """One-use worker container; uncertain handles stay owned for retry/review."""

    def __init__(self, api):
        _fake_only(api)
        self.api = api
        self.job = None
        self.process = None
        self.thread = None
        self._created = False
        self._worker_exited = False
        self._job_zero = False
        self.report = WorkerJobReport()

    def _retain(self):
        self.report.retained_handles = {
            name: value for name, value in (
                ("worker_job", self.job), ("worker_process", self.process),
                ("worker_thread", self.thread)) if value is not None}

    def _close(self, name, value):
        try:
            ok = self.api.CloseHandle(value)
        except BaseException as error:
            self.report.errors.append(f"CloseHandle {name}: {type(error).__name__}: {error}")
            ok = None
        self.report.close_results[name] = "confirmed" if ok else "failed or unknown"
        if not ok:
            self.report.errors.append(f"CloseHandle {name} did not confirm closure")
        return bool(ok)

    def stage_job(self):
        if self.report.phase != "new":
            raise RuntimeError("worker job must be staged exactly once")
        self.report.phase = "staging"
        handle = self.api.CreateJobObjectW(None, None)
        if not _handle(handle):
            raise abi.ApiFailure("CreateJobObjectW WORKER", self.api.last_error())
        self.job = _handle(handle)
        self._retain()
        try:
            flags = W.DWORD()
            if not self.api.GetHandleInformation(self.job, C.byref(flags)):
                raise abi.ApiFailure("GetHandleInformation WORKER", self.api.last_error())
            if flags.value & abi.HANDLE_FLAG_INHERIT:
                raise RuntimeError("WORKER job handle is inheritable")
            limits = abi.JOBOBJECT_EXTENDED_LIMIT_INFORMATION()
            limits.BasicLimitInformation.LimitFlags = abi.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
            if not self.api.SetInformationJobObject(
                    self.job, abi.JobObjectExtendedLimitInformation,
                    C.byref(limits), C.sizeof(limits)):
                raise abi.ApiFailure("SetInformationJobObject WORKER", self.api.last_error())
            self.report.phase = "job-staged"
            return self.job
        except BaseException:
            if self._close("worker_job", self.job):
                self.job = None
            self._retain()
            self.report.phase = "setup-failed"
            raise

    def create_suspended_worker(self):
        """Only the fixed fake image, with creation-time WORKER job assignment."""
        if self.report.phase != "job-staged" or self.job is None:
            raise RuntimeError("stage WORKER job before worker creation")
        self.report.phase = "creating"
        size = C.c_size_t()
        self.api.InitializeProcThreadAttributeList(None, 1, 0, C.byref(size))
        if not size.value:
            self.report.phase = "create-failed"
            raise abi.ApiFailure("InitializeProcThreadAttributeList size WORKER", self.api.last_error())
        buffer = C.create_string_buffer(size.value)
        attributes = C.cast(buffer, C.c_void_p)
        initialized = False
        try:
            if not self.api.InitializeProcThreadAttributeList(attributes, 1, 0, C.byref(size)):
                raise abi.ApiFailure("InitializeProcThreadAttributeList WORKER", self.api.last_error())
            initialized = True
            job_array = (W.HANDLE * 1)(self.job)
            if not self.api.UpdateProcThreadAttribute(
                    attributes, 0, abi.PROC_THREAD_ATTRIBUTE_JOB_LIST,
                    C.cast(job_array, C.c_void_p), C.sizeof(job_array), None, None):
                raise abi.ApiFailure("UpdateProcThreadAttribute WORKER JOB_LIST", self.api.last_error())
            startup = abi.STARTUPINFOEXW()
            startup.StartupInfo.cb = C.sizeof(abi.STARTUPINFOEXW)
            startup.lpAttributeList = attributes.value
            pi = abi.PROCESS_INFORMATION()
            command = C.create_unicode_buffer('"' + FAKE_WORKER_IMAGE + '"')
            flags = abi.CREATE_SUSPENDED | abi.EXTENDED_STARTUPINFO_PRESENT | abi.CREATE_NO_WINDOW
            self.report.create_outcome = "unknown: inside CreateProcessW"
            try:
                ok = self.api.CreateProcessW(
                    FAKE_WORKER_IMAGE, command, None, None, False, flags,
                    None, FAKE_WORKER_CWD, C.byref(startup), C.byref(pi))
            finally:
                # Partial PROCESS_INFORMATION values are ownership obligations
                # even when CreateProcessW fails or raises.
                self.process = _handle(pi.hProcess)
                self.thread = _handle(pi.hThread)
                self._retain()
            if not ok:
                raise abi.CreateOutcomeUnknown(self.api.last_error(), pi)
            if self.process is None or self.thread is None or not pi.dwProcessId or not pi.dwThreadId:
                raise RuntimeError("WORKER CreateProcessW returned incomplete PROCESS_INFORMATION")
            self._created = True
            self.report.create_outcome = "created suspended; assignment awaiting verification"
            self.report.phase = "worker-suspended"
            return pi
        except BaseException:
            self.report.phase = "create-failed-or-uncertain"
            raise
        finally:
            if initialized:
                self.api.DeleteProcThreadAttributeList(attributes)

    def verify_worker_gate(self):
        """Require exact-handle membership before any caller may attempt a child."""
        if self.report.phase != "worker-suspended" or not self._created:
            raise RuntimeError("complete suspended WORKER required")
        membership = W.BOOL()
        if not self.api.IsProcessInJob(self.process, self.job, C.byref(membership)):
            raise abi.ApiFailure("IsProcessInJob WORKER", self.api.last_error())
        if not membership.value:
            self.report.worker_assignment = "not confirmed"
            raise RuntimeError("WORKER is not in its creation-bound job")
        self.report.worker_assignment = "confirmed by exact process and job handles"
        self.report.worker_gate = "open for controlled worker resume"
        self.report.phase = "gate-open"
        return True

    def cleanup(self, *, wait_ms=2000):
        if type(wait_ms) is not int or not 0 <= wait_ms <= 5000:
            raise ValueError("bounded wait in [0, 5000] milliseconds required")
        self.report.worker_gate = "closed"
        if self.job is not None and self.process is not None and not self._worker_exited:
            try:
                stopped = self.api.TerminateJobObject(self.job, 0xE307)
                if not stopped:
                    self.report.errors.append("TerminateJobObject WORKER did not confirm termination")
                self._worker_exited = self.api.WaitForSingleObject(self.process, wait_ms) == WAIT_OBJECT_0
                if not self._worker_exited:
                    self.report.errors.append("exact WORKER wait did not confirm exit")
            except BaseException as error:
                self.report.errors.append(f"WORKER termination/wait: {type(error).__name__}: {error}")
        if self.job is not None:
            try:
                self._job_zero = self.api.QueryInformationJobObject(self.job) == 0
                if not self._job_zero:
                    self.report.errors.append("WORKER job active-process count not confirmed zero")
            except BaseException as error:
                self.report.errors.append(f"WORKER job query: {type(error).__name__}: {error}")
        if self._worker_exited:
            for name in ("thread", "process"):
                value = getattr(self, name)
                if value is not None and self._close("worker_" + name, value):
                    setattr(self, name, None)
        if self.job is not None and self._job_zero and self._worker_exited:
            if self._close("worker_job", self.job):
                self.job = None
        elif self.job is not None and self.process is None and self.report.create_outcome == "not-attempted":
            if self._close("worker_job", self.job):
                self.job = None
        self._retain()
        self.report.phase = "cleaned" if not self.report.retained_handles else "cleanup-uncertain"
        return self.report


def create_real_worker(*_args, **_kwargs):
    raise RuntimeError("CP307 live WORKER CreateProcessW HOLD")
