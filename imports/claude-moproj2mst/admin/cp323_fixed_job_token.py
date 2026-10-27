"""CP323 fixed suspended CP316 WORKER/empty CHILD token, injected fakes only.

CP320 retains file identity, not mapped-image attestation. CP318 supplies the
borrowed fake endpoint contract; CP321 ABI and CP322 native endpoint binding
remain HARD HOLD. No resume, protocol send, DLL load or native factory exists.
The sole deadline is EndpointPair.deadline_tick_ms in GetTickCount64 units;
the legacy CP316 floating-point deadline is neither consulted nor renewed.
"""
from __future__ import annotations

import ctypes as C
import subprocess

import cp313_native_worker_creator as base
import cp318_fixed_worker_creator as fixed
import cp320_retained_image_pin as image

Hold = base.Hold
PreparationFailure = base.PreparationFailure
EndpointPair = fixed.EndpointPair
WORKER_IMAGE, WORKER_CWD = fixed.WORKER_IMAGE, fixed.WORKER_CWD
NATIVE_ENTRY = "HARD HOLD"


def create_live_worker(*_args, **_kwargs):
    raise Hold("CP323 native HARD HOLD: CP321 ABI and CP322 endpoints unresolved")


class FakeWorkerCreator(base.FakeWorkerCreator):
    """One-use owner. Retain after HOLD and retry close until its receipt closes.

    The remote CHILD handle belongs to the exact creation-returned suspended
    process. It is evidence for a future INIT frame, never a local CloseHandle
    argument. Borrowed pipe endpoints remain owned by their endpoint owner.
    """
    def __init__(self, functions, *, last_error, image_pin, endpoints, tick_ms):
        if getattr(functions, "cp323_fake_api", False) is not True:
            raise Hold("CP323 injected fake API required")
        if not isinstance(image_pin, image.FakeRetainedImagePin):
            raise Hold("CP320 retained image owner required")
        # A fake marker never grants permission to call real DLL exports.
        if any(isinstance(getattr(functions, name), C._CFuncPtr)
               for name in base.SIGNATURES) or any(
                   isinstance(fn, C._CFuncPtr) for fn in image_pin._fn.values()):
            raise Hold("native DLL exports forbidden at injected fake seam")
        if image_pin._used or image_pin.handle is not None or image_pin.reopened_handle is not None:
            raise Hold("fresh unused CP320 retained image owner required")
        if not callable(tick_ms):
            raise Hold("injected GetTickCount64 clock required")
        super().__init__(functions, last_error=last_error)
        self.image_pin, self.endpoints, self.tick_ms = image_pin, endpoints, tick_ms
        self.image_path = WORKER_IMAGE
        self._last_tick = None
        self._deadline = None

    def _own(self, name, value):
        raw = getattr(value, "value", value)
        if raw in (-1, C.c_void_p(-1).value):
            raise Hold(f"{name} invalid handle sentinel")
        return super()._own(name, value)

    def _budget(self):
        now = self.tick_ms()
        deadline = self.endpoints.deadline_tick_ms
        if (type(now) is not int or not 0 <= now < 2**64 or
                type(deadline) is not int or not 0 < deadline < 2**64 or
                (self._last_tick is not None and now < self._last_tick) or
                (self._deadline is not None and deadline != self._deadline) or
                not 0 < deadline - now <= 5000):
            raise Hold("single absolute GetTickCount64 deadline invalid or exhausted")
        self._last_tick, self._deadline = now, deadline

    _endpoints = fixed.FakeWorkerCreator._endpoints

    def _job(self, name):
        job = self._own(name, self._fn["CreateJobObjectW"](None, None))
        if self._flags(job) & base.HANDLE_FLAG_INHERIT:
            raise Hold(f"{name} must be noninheritable")
        limits = base.EXTENDED_LIMIT()
        limits.BasicLimitInformation.LimitFlags = base.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
        self._ok("SetInformationJobObject", self._fn["SetInformationJobObject"](
            job, base.JobObjectExtendedLimitInformation, C.byref(limits), C.sizeof(limits)))
        if self._job_limits(job) != base.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE:
            raise Hold(f"{name} sole kill-on-close limit required")
        if self._active_count(job) != 0:
            raise Hold(f"{name} must initially be empty")
        return job

    def prepare(self):
        if self._used:
            raise Hold("creator is one-use")
        self._used = True
        r = self.receipt
        try:
            self._endpoints()
            self.image_pin.prepare()
            self._budget()
            job, child = self._job("worker_job"), self._job("child_job")
            e = self.endpoints
            attributes = self._attribute_list(job, e.worker_read, e.worker_write)
            try:
                address, buffer, jobs, handles = next(attributes)
                startup = base.STARTUPINFOEXW()
                startup.StartupInfo.cb = C.sizeof(startup)
                startup.StartupInfo.dwFlags = base.STARTF_USESTDHANDLES
                startup.StartupInfo.hStdInput = e.worker_read
                startup.StartupInfo.hStdOutput = startup.StartupInfo.hStdError = e.worker_write
                startup.lpAttributeList = address.value
                command = C.create_unicode_buffer(subprocess.list2cmdline(
                    [WORKER_IMAGE, str(self._deadline)]))
                info = base.PROCESS_INFORMATION()
                self._budget()
                r.create_outcome = "unknown: CreateProcessW entered"
                try:
                    success = self._fn["CreateProcessW"](
                        WORKER_IMAGE, command, None, None, True, base.CREATE_FLAGS,
                        None, WORKER_CWD, C.byref(startup.StartupInfo), C.byref(info))
                finally:
                    # Capture both outputs independently, including exceptions.
                    capture_errors = []
                    for name, value in (("worker_process", info.hProcess),
                                        ("worker_thread", info.hThread)):
                        if value:
                            try:
                                self._own(name, value)
                            except BaseException as error:
                                capture_errors.append(str(error))
                    r.pid, r.tid = int(info.dwProcessId) or None, int(info.dwThreadId) or None
                    if capture_errors:
                        raise Hold("; ".join(capture_errors))
                self._ok("CreateProcessW", success)
                r.create_outcome = "returned-success"
                self.image_pin.verify_after_create()  # still CREATE_SUSPENDED
            finally:
                attributes.close()
            self._budget()
            if not all(n in r.local for n in ("worker_process", "worker_thread")) or not r.pid or not r.tid:
                raise Hold("incomplete creation-returned identity")
            process = r.local["worker_process"]
            if self._fn["GetProcessId"](process) != r.pid:
                raise Hold("creation-returned PID mismatch")
            path = C.create_unicode_buffer(32768)
            length = base.W.DWORD(len(path))
            self._ok("QueryFullProcessImageNameW", self._fn["QueryFullProcessImageNameW"](
                process, 0, path, C.byref(length)))
            if not 0 < length.value < len(path) or path[:length.value].casefold() != WORKER_IMAGE.casefold():
                raise Hold("creation-returned image path mismatch")
            for target, expected in ((job, True), (child, False)):
                member = base.W.BOOL()
                self._ok("IsProcessInJob", self._fn["IsProcessInJob"](process, target, C.byref(member)))
                if bool(member.value) != expected:
                    raise Hold("exact WORKER/CHILD membership mismatch")
            for target in (job, child):
                if self._flags(target) & base.HANDLE_FLAG_INHERIT or self._job_limits(target) != base.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE:
                    raise Hold("job containment changed")
            if self._active_count(child) != 0:
                raise Hold("CHILD token job must remain empty")
            self._budget()
            duplicate = base.W.HANDLE()
            current = self._fn["GetCurrentProcess"]()
            r.duplicate_outcome = "unknown: DuplicateHandle entered"
            try:
                success = self._fn["DuplicateHandle"](
                    current, child, process, C.byref(duplicate), 0, False, base.DUPLICATE_SAME_ACCESS)
            finally:
                if duplicate.value:
                    r.remote_child_job = base.RemoteHandle(duplicate.value, process)
            self._ok("DuplicateHandle", success)
            if not duplicate.value or duplicate.value == C.c_void_p(-1).value:
                raise Hold("positive worker-local CHILD token required")
            r.duplicate_outcome = "returned-success"
            self._budget()
            r.phase = "prepared-suspended"
            return r
        except BaseException as error:
            r.errors.append(str(error))
            r.phase = "hold"
            if r.create_outcome == "not-attempted":
                self.close()
            raise PreparationFailure(str(error), r) from error

    def close(self):
        r = self.receipt
        if r.phase == "closed":
            return r
        if r.create_outcome != "not-attempted" and not (r.worker_waited and r.jobs_empty):
            try:
                # Termination must not depend on pending pipe I/O being releasable.
                if not r.worker_terminated:
                    self._ok("TerminateJobObject", self._fn["TerminateJobObject"](
                        r.local["worker_job"], 0xE323))
                    r.worker_terminated = True
                if "worker_process" not in r.local:
                    raise Hold("unknown creation outcome lacks process wait handle")
                if not r.worker_waited:
                    status = self._fn["WaitForSingleObject"](r.local["worker_process"], 0)
                    r.worker_waited = type(status) is int and status == base.WAIT_OBJECT_0
                if not r.worker_waited:
                    raise Hold("worker wait unresolved")
                r.jobs_empty = (self._active_count(r.local["worker_job"]) == 0 and
                                self._active_count(r.local["child_job"]) == 0)
                if not r.jobs_empty:
                    raise Hold("WORKER/CHILD emptiness unresolved")
                r.remote_child_job = None  # vanished process table, never close locally
            except BaseException as error:
                r.errors.append(str(error))
                r.phase = "hold"
                return r
        try:
            self.endpoints.owner.require_releasable()
        except BaseException as error:
            # No attempted creation and invalid endpoints own no pipe handles.
            if r.create_outcome != "not-attempted" or r.local or self.image_pin.handle is not None:
                r.errors.append(str(error))
                r.phase = "hold"
                return r
        for name in tuple(r.local):
            try:
                self._close_one(name)
            except BaseException as error:
                r.errors.append(str(error))
        if not r.local:
            try:
                self.image_pin.close()
            except BaseException as error:
                r.errors.append(str(error))
        r.phase = "closed" if not r.local and self.image_pin.handle is None and self.image_pin.reopened_handle is None else "hold"
        return r
