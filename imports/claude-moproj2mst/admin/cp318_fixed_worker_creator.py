"""CP318 fixed CP316 WORKER bridge; injected fakes only, native HARD HOLD.

Reuses CP313's handle, attribute-list and job checks, never its CP312 prepare
path. CP316 has no connected-endpoint receipt yet: EndpointPair below is a
fake bridge contract, not evidence of native DACL/connection/overlapped mode.
No DLL, image read by pathname, process resume, or child job is exposed.
"""
from __future__ import annotations

import ctypes as C
from dataclasses import dataclass
import math
import subprocess

import cp313_native_worker_creator as base
import cp316_native_pipe_adapter as pipes

WORKER_IMAGE = r"C:\PROBLEM\s316\worker.exe"
WORKER_CWD = r"C:\PROBLEM\s316"
WORKER_SHA256 = "19886a0821d47611cc82dc44476ae222328e9c169b8e8ba9183cf92c3b655985"
WORKER_BYTES = 104960
BUILD_RECEIPT = "analysis/cp316-worker-fixture-build.json"
BUILD_RECEIPT_SHA256 = "7dbf6936c3261e5faa83b1d29e3fd69e54963ab312670c8b1d9a058d6728e309"
NATIVE_ENTRY = "HARD HOLD"
Hold = base.Hold
PreparationFailure = base.PreparationFailure


@dataclass(frozen=True)
class FakeBuildReceipt:
    """Test image identity bound to the static reviewed build; no live approval."""
    sha256: str
    size: int
    identity: object
    image: str = WORKER_IMAGE
    receipt: str = BUILD_RECEIPT
    receipt_sha256: str = BUILD_RECEIPT_SHA256


@dataclass(frozen=True)
class EndpointPair:
    """Borrowed fake endpoints; the CP316 owner retains and closes all four.

    deadline_tick_ms is a clock-domain representation of preparation.deadline,
    allocated by the fake caller before endpoint preparation, never a fresh
    per-process allowance. _budget rejects inconsistent clock-domain bindings.
    """
    preparation: pipes.Preparation
    worker_read: int
    worker_write: int
    supervisor_write: int
    supervisor_read: int
    owner: object
    deadline_tick_ms: int
    qualification: str = "fake-only; native connected endpoint binding missing"


def create_live_worker(*_args, **_kwargs):
    raise Hold("CP318 native HARD HOLD: retained image "
               "identity binding and typed connected CP316 endpoints missing")


class FakeWorkerCreator(base.FakeWorkerCreator):
    """One-use suspended creator. Caller retains this owner after any HOLD.

    image_api is fake-only and must open_locked the FIXED image with write and
    delete sharing denied. snapshot(handle) hashes and identifies that same
    open handle. The handle stays owned until process/job cleanup is proven.
    A future native implementation needs independently reviewed Win32 binding.
    """
    def __init__(self, functions, *, last_error, image_api, build_receipt,
                 endpoints: EndpointPair, clock, tick_ms):
        if getattr(functions, "cp318_fake_api", False) is not True:
            raise Hold("CP318 injected fake API required")
        if getattr(image_api, "cp318_fake_image_api", False) is not True:
            raise Hold("injected retained-image fake required")
        if not isinstance(build_receipt, FakeBuildReceipt):
            raise Hold("explicit fake build receipt required; native integration remains unreviewed")
        if (build_receipt.image != WORKER_IMAGE or
                build_receipt.sha256 != WORKER_SHA256 or
                type(build_receipt.size) is not int or build_receipt.size != WORKER_BYTES or
                build_receipt.receipt != BUILD_RECEIPT or
                build_receipt.receipt_sha256 != BUILD_RECEIPT_SHA256 or
                build_receipt.identity is None):
            raise Hold("fixed CP316 fake image pin required")
        # CP313 constructor binds signatures only; its CP312 prepare is unused.
        super().__init__(functions, last_error=last_error)
        self.image_path = WORKER_IMAGE
        self.image_api, self.pin = image_api, build_receipt
        self.endpoints, self.clock, self.tick_ms = endpoints, clock, tick_ms
        self.image_handle = None

    def _budget(self):
        remaining = self.endpoints.preparation.deadline - self.clock()
        now_tick = self.tick_ms()
        if type(now_tick) is not int or now_tick < 0:
            raise Hold("unsigned GetTickCount64 clock required")
        ticks = self.endpoints.deadline_tick_ms - now_tick
        if (not math.isfinite(remaining) or not 0 < remaining <= 5 or
                not 0 < ticks <= 5000 or abs(remaining * 1000 - ticks) > 2):
            raise Hold("single pre-create deadline exhausted or clock binding invalid")

    def _endpoints(self):
        e = self.endpoints
        if not isinstance(e, EndpointPair) or not isinstance(e.preparation, pipes.Preparation):
            raise Hold("CP316 typed fake endpoint bridge required; native integration HOLD")
        if type(e.deadline_tick_ms) is not int or not 0 < e.deadline_tick_ms < 2**64:
            raise Hold("pre-create absolute GetTickCount64 deadline required")
        handles = (e.worker_read, e.worker_write, e.supervisor_write, e.supervisor_read)
        if any(type(h) is not int or h <= 0 for h in handles) or len(set(handles)) != 4:
            raise Hold("four distinct typed endpoint handles required")
        if (e.preparation.to_worker.client_access != "read" or
                e.preparation.from_worker.client_access != "write" or
                not e.preparation.to_worker.client_inheritable or
                not e.preparation.from_worker.client_inheritable):
            raise Hold("CP316 endpoint directions/inheritance mismatch")
        if getattr(e.owner, "cp318_fake_endpoint_owner", False) is not True:
            raise Hold("connected native CP316 endpoint owner unavailable")
        e.owner.require_releasable()
        for h, inherited in zip(handles, (True, True, False, False)):
            if bool(self._flags(h) & base.HANDLE_FLAG_INHERIT) != inherited:
                raise Hold("endpoint inheritance mismatch")
        self._budget()

    def _image_snapshot(self):
        actual = self.image_api.snapshot(self.image_handle)
        if actual != (self.pin.sha256, self.pin.size, self.pin.identity):
            raise Hold("retained CP316 image handle hash/size/identity mismatch")

    def prepare(self):
        if self._used:
            raise Hold("creator is one-use")
        self._used = True
        r = self.receipt
        try:
            self._endpoints()
            self.image_handle = self.image_api.open_locked(
                WORKER_IMAGE, share_write=False, share_delete=False)
            if self.image_handle in (None, 0, -1, C.c_void_p(-1).value):
                raise Hold("retained image handle missing")
            self._image_snapshot()
            self._budget()
            job = self._own("worker_job", self._fn["CreateJobObjectW"](None, None))
            if self._flags(job) & base.HANDLE_FLAG_INHERIT:
                raise Hold("WORKER job must remain supervisor-only")
            limits = base.EXTENDED_LIMIT()
            limits.BasicLimitInformation.LimitFlags = base.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
            self._ok("SetInformationJobObject", self._fn["SetInformationJobObject"](
                job, base.JobObjectExtendedLimitInformation, C.byref(limits), C.sizeof(limits)))
            if self._job_limits(job) != base.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE:
                raise Hold("sole WORKER kill-on-close limits required")
            e = self.endpoints
            attributes = self._attribute_list(job, e.worker_read, e.worker_write)
            address, buffer, jobs, handles = next(attributes)
            try:
                startup = base.STARTUPINFOEXW()
                startup.StartupInfo.cb = C.sizeof(startup)
                startup.StartupInfo.dwFlags = base.STARTF_USESTDHANDLES
                startup.StartupInfo.hStdInput = e.worker_read
                startup.StartupInfo.hStdOutput = startup.StartupInfo.hStdError = e.worker_write
                startup.lpAttributeList = address.value
                command = C.create_unicode_buffer(subprocess.list2cmdline(
                    [WORKER_IMAGE, str(e.deadline_tick_ms)]))
                info = base.PROCESS_INFORMATION()
                self._image_snapshot()
                self._budget()
                r.create_outcome = "unknown: CreateProcessW entered"
                try:
                    success = self._fn["CreateProcessW"](
                        WORKER_IMAGE, command, None, None, True, base.CREATE_FLAGS,
                        None, WORKER_CWD, C.byref(startup.StartupInfo), C.byref(info))
                finally:
                    for name, value in (("worker_process", info.hProcess), ("worker_thread", info.hThread)):
                        if value:
                            self._own(name, value)
                    r.pid, r.tid = int(info.dwProcessId) or None, int(info.dwThreadId) or None
                self._ok("CreateProcessW", success)
                r.create_outcome = "returned-success"
            finally:
                attributes.close()
            self._image_snapshot()
            self._budget()
            if not all(n in r.local for n in ("worker_process", "worker_thread")) or not r.pid or not r.tid:
                raise Hold("incomplete creation-returned worker identity")
            if self._fn["GetProcessId"](r.local["worker_process"]) != r.pid:
                raise Hold("creation-returned PID mismatch")
            image_buffer = C.create_unicode_buffer(32768)
            length = base.W.DWORD(len(image_buffer))
            self._ok("QueryFullProcessImageNameW", self._fn["QueryFullProcessImageNameW"](
                r.local["worker_process"], 0, image_buffer, C.byref(length)))
            if (not 0 < length.value < len(image_buffer) or
                    image_buffer[:length.value].casefold() != WORKER_IMAGE.casefold()):
                raise Hold("creation-returned image path mismatch")
            member = base.W.BOOL()
            self._ok("IsProcessInJob", self._fn["IsProcessInJob"](
                r.local["worker_process"], job, C.byref(member)))
            if not member.value:
                raise Hold("worker absent from exact WORKER job")
            if self._flags(job) & base.HANDLE_FLAG_INHERIT:
                raise Hold("WORKER job inheritance changed")
            r.phase = "prepared-suspended"
            return r
        except BaseException as error:
            r.errors.append(str(error))
            r.phase = "hold"
            if r.create_outcome == "not-attempted":
                self.close()
            raise PreparationFailure(str(error), r) from error

    def close(self):
        """No pipe closure: borrowed endpoints stay with their adapter owner."""
        r = self.receipt
        if r.create_outcome != "not-attempted" and not (r.worker_waited and r.jobs_empty):
            try:
                self.endpoints.owner.require_releasable()
                if not r.worker_terminated:
                    self._ok("TerminateJobObject", self._fn["TerminateJobObject"](
                        r.local["worker_job"], 0xE318))
                    r.worker_terminated = True
                if "worker_process" not in r.local:
                    raise Hold("unknown creation outcome lacks process wait handle")
                status = self._fn["WaitForSingleObject"](r.local["worker_process"], 0)
                r.worker_waited = type(status) is int and status == base.WAIT_OBJECT_0
                r.jobs_empty = self._active_count(r.local["worker_job"]) == 0
                if not r.worker_waited or not r.jobs_empty:
                    raise Hold("worker wait/job-empty proof unresolved")
            except BaseException as error:
                r.errors.append(str(error))
                r.phase = "hold"
                return r
        for name in tuple(r.local):
            try:
                self._close_one(name)
            except BaseException as error:
                r.errors.append(str(error))
        if not r.local and self.image_handle is not None:
            try:
                self.image_api.close(self.image_handle)
                self.image_handle = None
            except BaseException as error:
                r.errors.append(str(error))
        r.phase = "closed" if not r.local and self.image_handle is None else "hold"
        return r
