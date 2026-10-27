"""CP304 fake-only ownership protocol for one suspended CP302 clone.

This is a review seam, not a launcher. No PID is reopened and there is no
built-in route to a real child or ResumeThread. The fake markers are conventions,
not a security boundary: an arbitrary supplied API or ACK callback can execute
real operations of its own. Only reviewed in-memory fakes may be passed here.
"""
from __future__ import annotations

import ctypes as C
from ctypes import wintypes as W
from dataclasses import dataclass, field
from pathlib import PureWindowsPath

import cp303_job_creation as creation


WAIT_OBJECT_0 = 0
WAIT_TIMEOUT = 0x102
WAIT_FAILED = 0xFFFFFFFF
DUPLICATE_SAME_ACCESS = creation.DUPLICATE_SAME_ACCESS


def _fake_only(api):
    if isinstance(api, creation.Kernel32Api) or getattr(api, "cp304_fake_api", False) is not True:
        raise RuntimeError("CP304 real job supervisor HOLD")


def _positive_int(value):
    return type(value) is int and value > 0


@dataclass(frozen=True)
class ChildOffer:
    occurrence: str
    worker_handle: int
    pid: int
    tid: int
    creation_filetime: int
    image: str


@dataclass(frozen=True)
class ChildAck:
    occurrence: str
    pid: int
    tid: int
    creation_filetime: int
    image: str


@dataclass
class CleanupReport:
    """One cleanup attempt. Retained values are unclosed, not proven live OS objects."""
    job_termination: str = "not-attempted"
    child_wait: str = "no-exact-child-handle"
    worker_termination: str = "not-attempted"
    worker_wait: str = "no-exact-worker-handle"
    active_processes: int | None = None
    active_processes_zero: bool = False
    child_identity: str = "unknown"
    handle_closes: dict[str, str] = field(default_factory=dict)
    retained_handles: dict[str, int] = field(default_factory=dict)
    errors: list[str] = field(default_factory=list)


class FakeJobSupervisor:
    """Strict one-offer protocol with exact-handle cleanup receipts.

    The worker process handle is creation-owned and transferred to this object
    by ``bind_worker``.  The worker itself is outside the child job.  Its
    worker-local job duplicate is never closed from the supervisor process.
    The API and ACK callback must be trusted in-memory fakes; marker attributes
    alone cannot prevent arbitrary wrapper code from performing real operations.
    """

    def __init__(self, api, image: creation.FixedCloneImage):
        _fake_only(api)
        expected_exe, _ = creation._validate_supplied_fixed_image_contract(image)
        self.api = api
        self.expected_image = expected_exe
        self.job: creation.SupervisorJob | None = None
        self.worker_handle: int | None = None
        self.worker_job_handle: int | None = None
        self.child_handle: int | None = None
        self.offer: ChildOffer | None = None
        self.ack: ChildAck | None = None
        self.ack_transmission = "not-attempted"
        self.ack_receipt = "not-received"
        self.worker_started = False
        self.create_outcome = "not-attempted"
        self.cleanup_report: CleanupReport | None = None
        self._job_terminated = False
        self._child_exited = False
        self._worker_terminated = False
        self._worker_exited = False
        self._active_zero = False

    def stage_job(self):
        if self.job is not None or self.worker_handle is not None:
            raise RuntimeError("job must be staged once before worker creation")
        self.job = creation.stage_supervisor_job_for_fake_test(self.api)
        return self.job

    def bind_worker(self, worker_process_handle: int):
        if self.job is None or self.worker_handle is not None or self.cleanup_report is not None:
            raise RuntimeError("stage job before binding one owned worker")
        if not _positive_int(worker_process_handle):
            raise ValueError("exact creation-owned worker handle required")
        # Take ownership before duplication so even a duplicate failure can be
        # cleaned up through the exact handle supplied by worker creation.
        self.worker_handle = worker_process_handle
        self.worker_job_handle = creation.duplicate_job_to_worker_for_fake_test(
            self.api, self.job, worker_process_handle)
        return self.worker_job_handle

    def start_worker(self):
        if self.worker_job_handle is None or self.worker_started or self.cleanup_report is not None:
            raise RuntimeError("worker must receive exact job duplicate before start")
        # Only records an external fake worker gate. Never invokes a launcher.
        self.worker_started = True

    def note_create_interrupted(self):
        if not self.worker_started or self.offer is not None:
            raise RuntimeError("interruption belongs to started worker before offer")
        self.create_outcome = "unknown: interrupted CreateProcessW; child identity unknown"

    def accept_offer(self, offer: ChildOffer, send_ack):
        if not self.worker_started or self.worker_handle is None or self.job is None:
            raise RuntimeError("worker not started under staged job protocol")
        if self.offer is not None or self.child_handle is not None or self.cleanup_report is not None:
            raise RuntimeError("only one child offer is permitted")
        if (not isinstance(offer, ChildOffer) or
                not isinstance(offer.occurrence, str) or not offer.occurrence or
                not all(_positive_int(value) for value in (
                    offer.worker_handle, offer.pid, offer.tid, offer.creation_filetime)) or
                not isinstance(offer.image, str) or not offer.image):
            raise ValueError("complete exact child offer required")
        self.offer = offer
        self.create_outcome = "offered; identity unverified"
        local = W.HANDLE()
        ok = self.api.DuplicateHandle(self.worker_handle, offer.worker_handle,
                                      self.api.GetCurrentProcess(), C.byref(local),
                                      0, False, DUPLICATE_SAME_ACCESS)
        if not ok or not local.value:
            raise creation.ApiFailure("DuplicateHandle child into supervisor", self.api.last_error())
        self.child_handle = int(local.value)
        # These three facts are independently read through the newly duplicated
        # exact handle.  The offered PID is never used to obtain a new handle.
        pid = self.api.GetProcessId(self.child_handle)
        filetime = self.api.GetProcessTimes(self.child_handle)
        image = self.api.QueryFullProcessImageNameW(self.child_handle)
        if not _positive_int(pid) or not _positive_int(filetime) or not image:
            raise creation.ApiFailure("query duplicated child identity", self.api.last_error())
        image_key = str(PureWindowsPath(image)).casefold()
        offered_key = str(PureWindowsPath(offer.image)).casefold()
        fixed_key = str(PureWindowsPath(self.expected_image)).casefold()
        if (pid != offer.pid or filetime != offer.creation_filetime or
                image_key != offered_key or image_key != fixed_key):
            raise RuntimeError("duplicated child identity disagrees with offer or fixed CP302 clone")
        self.create_outcome = "verified exact offered child"
        self.ack = ChildAck(offer.occurrence, pid, offer.tid, filetime, image)
        self.ack_transmission = "attempted; outcome unknown"
        sent = send_ack(self.ack)
        self.ack_transmission = "sent" if sent is True else "not-confirmed"
        return self.ack

    def record_ack_receipt(self, receipt: ChildAck):
        """Record an untrusted matching claim, not authenticated worker acceptance."""
        if self.ack is None or self.ack_transmission != "sent":
            raise RuntimeError("no confirmed ACK transmission")
        if receipt != self.ack:
            raise RuntimeError("worker ACK receipt mismatches offered occurrence or identity")
        self.ack_receipt = "matching-unverified-claim"

    def cleanup(self, *, wait_ms: int = 2000):
        if type(wait_ms) is not int or not 0 <= wait_ms <= 5000:
            raise ValueError("bounded wait in [0, 5000] milliseconds required")
        report = CleanupReport()
        self.cleanup_report = report
        if self.offer is not None and self.create_outcome == "verified exact offered child":
            report.child_identity = "verified exact offered child"
        elif self.worker_started:
            report.child_identity = "unknown even if job accounting reaches zero"

        def attempt(label, action):
            try:
                return action()
            except BaseException as error:
                report.errors.append(f"{label}: {type(error).__name__}: {error}")
                return None

        if self.job is not None and not self._job_terminated:
            result = attempt("TerminateJobObject", lambda: self.api.TerminateJobObject(self.job.handle, 0xE304))
            self._job_terminated = result is True
            if not self._job_terminated:
                report.errors.append("TerminateJobObject did not confirm termination")
        report.job_termination = "succeeded" if self._job_terminated else "failed or unknown"
        if self.child_handle is not None:
            if not self._child_exited:
                result = attempt("child wait", lambda: self.api.WaitForSingleObject(self.child_handle, wait_ms))
                self._child_exited = result == WAIT_OBJECT_0
                report.child_wait = _wait_status(result)
            else:
                report.child_wait = "exited"
            if not self._child_exited:
                report.errors.append(f"exact child wait {report.child_wait}")
        if self.worker_handle is not None:
            if not self._worker_exited:
                result = attempt("worker wait before stop", lambda: self.api.WaitForSingleObject(self.worker_handle, 0))
                if result == WAIT_OBJECT_0:
                    self._worker_exited = True
                    report.worker_termination = "already exited"
                elif not self._worker_terminated:
                    stopped = attempt("TerminateProcess worker", lambda: self.api.TerminateProcess(self.worker_handle, 0xE304))
                    self._worker_terminated = stopped is True
                    if not self._worker_terminated:
                        report.errors.append("TerminateProcess worker did not confirm termination")
                if not self._worker_exited:
                    result = attempt("worker wait", lambda: self.api.WaitForSingleObject(self.worker_handle, wait_ms))
                    self._worker_exited = result == WAIT_OBJECT_0
                    report.worker_wait = _wait_status(result)
                else:
                    report.worker_wait = "exited"
            else:
                report.worker_wait = "exited"
            if report.worker_termination == "not-attempted" and self._worker_terminated:
                report.worker_termination = "succeeded"
            elif report.worker_termination == "not-attempted" and not self._worker_exited:
                report.worker_termination = "failed or unknown"
            if not self._worker_exited:
                report.errors.append(f"exact worker wait {report.worker_wait}")
        if self.job is not None:
            active = attempt("QueryInformationJobObject", lambda: self.api.QueryInformationJobObject(self.job.handle))
            if type(active) is int and active >= 0:
                report.active_processes = active
                self._active_zero = active == 0
            else:
                self._active_zero = False
            report.active_processes_zero = self._active_zero
            if not report.active_processes_zero:
                report.errors.append("job active process count not verified zero")
        else:
            report.active_processes_zero = self._active_zero
            report.active_processes = 0 if self._active_zero else None
        for label in ("child", "worker", "job"):
            handle = (self.child_handle if label == "child" else
                      self.worker_handle if label == "worker" else
                      self.job.handle if self.job else None)
            safe_to_close = (self._child_exited if label == "child" else
                             self._worker_exited if label == "worker" else
                             self._job_terminated and self._active_zero and
                             self.child_handle is None and self.worker_handle is None)
            if handle is not None and safe_to_close:
                closed = attempt(f"CloseHandle {label}", lambda h=handle: self.api.CloseHandle(h))
                report.handle_closes[label] = "succeeded" if closed is True else "failed or unknown"
                if closed is True:
                    if label == "child":
                        self.child_handle = None
                    elif label == "worker":
                        self.worker_handle = None
                    else:
                        self.job = None
                else:
                    report.errors.append(f"CloseHandle {label} did not confirm closure")
            elif handle is not None:
                report.handle_closes[label] = "deferred; exact handle retained"
        report.retained_handles = {
            label: handle for label, handle in (
                ("child", self.child_handle), ("worker", self.worker_handle),
                ("job", self.job.handle if self.job else None)) if handle is not None
        }
        return report


def _wait_status(result):
    return {WAIT_OBJECT_0: "exited", WAIT_TIMEOUT: "timeout", WAIT_FAILED: "failed"}.get(
        result, "unknown")


def run_real_job_supervisor(*_args, **_kwargs):
    raise RuntimeError("CP304 real job supervisor HOLD")
