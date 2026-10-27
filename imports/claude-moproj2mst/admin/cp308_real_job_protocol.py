"""CP308 fake-only supervisor half of a Windows x64 containment protocol.

This module composes *after* separate creation-bound WORKER and CHILD JOB_LIST
operations. It does not perform those operations, prove their creation history,
or launch/resume anything. Both jobs and the worker's creation-returned handles
remain supervisor-owned. The only accepted child is a CP302 fixture.exe clone
under C:\\PROBLEM\\m289_<16 hex>. The CP302 digest is a preflight pin, not a
measurement of mapped process bytes.

An injected API and transport are trusted in-memory test doubles, not a security
boundary. A production adapter, real child run, original run and GUI run remain
on hard HOLD. In particular, a creator lost inside CreateProcessW leaves child
existence unknown even with creation-time JOB_LIST; absence of an OFFER is not
evidence that no child was created.
"""
from __future__ import annotations

import ctypes as C
from ctypes import wintypes as W
from dataclasses import dataclass, field
from pathlib import PureWindowsPath
import re


CP302_SHA256 = "66eadac3f5dd6ee0d0b856a53760dc9270ecfd170808c62dff01b99a583f6a28"
DUPLICATE_SAME_ACCESS = 2
HANDLE_FLAG_INHERIT = 1
JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE = 0x2000
WAIT_OBJECT_0 = 0


def _handle(value: object) -> int | None:
    value = getattr(value, "value", value)
    return value if type(value) is int and value > 0 else None


def _fixed_image(path: str, digest: str) -> str:
    if digest != CP302_SHA256 or not isinstance(path, str):
        raise ValueError("fixed CP302 digest and clone image required")
    image = PureWindowsPath(path)
    if (not image.is_absolute() or image.name.casefold() != "fixture.exe"
            or image.parent.parent != PureWindowsPath("C:\\PROBLEM")
            or re.fullmatch(r"m289_[0-9a-f]{16}", image.parent.name) is None
            or len(str(image)) >= 100):
        raise ValueError("fixed CP302 clone under C:\\PROBLEM required")
    return str(image)


def _image_key(path: str) -> str:
    return str(PureWindowsPath(path)).casefold()


@dataclass(frozen=True)
class Occurrence:
    run: str
    child: str


@dataclass(frozen=True)
class Offer:
    occurrence: Occurrence
    offer_id: str
    worker_local_process_handle: int
    pid: int
    creation_filetime: int
    image: str


@dataclass(frozen=True)
class Ack:
    occurrence: Occurrence
    offer_id: str
    pid: int
    creation_filetime: int
    image: str


@dataclass(frozen=True)
class WorkerReceipt:
    ack: Ack
    worker_id: str


@dataclass
class Report:
    phase: str = "new"
    create_outcome: str = "not-observed"
    ack_delivery: str = "not-attempted"
    worker_receipt: str = "not-received"
    child_wait: str = "not-attempted"
    worker_wait: str = "not-attempted"
    job_active: dict[str, int | None] = field(default_factory=dict)
    close: dict[str, str] = field(default_factory=dict)
    retained: dict[str, int] = field(default_factory=dict)
    errors: list[str] = field(default_factory=list)


class ProtocolHold(RuntimeError):
    """A failed or ambiguous transition must not authorize child resume."""


class FakeRealJobProtocol:
    """One occurrence, two jobs, one exact child-handle offer.

    ``bind_created`` takes ownership of supervisor-local handles returned by
    separately reviewed creation code. Its flags and membership checks establish
    present state only. The caller must separately establish x64 ABI, kill-on-close
    limits, creation-time JOB_LIST, suspended creation, disabled inheritance,
    protected clone provenance, and worker transport binding before calling it.
    The fake API implements GetHandleInformation, GetJobLimitFlags,
    IsProcessInJob, DuplicateHandle,
    GetProcessId, GetProcessTimes, QueryFullProcessImageNameW, TerminateJobObject,
    WaitForSingleObject, QueryInformationJobObject and CloseHandle. The transport
    implements send_ack only; it never manufactures the worker receipt. Several
    fake API methods deliberately return a value directly instead of using the
    Win32 BOOL-plus-output-pointer ABI. This class is not a native Win32 adapter.
    """

    def __init__(self, api: object, transport: object, *, occurrence: Occurrence,
                 worker_id: str, fixed_clone_image: str, fixture_sha256: str):
        if (getattr(api, "cp308_fake_api", False) is not True
                or getattr(transport, "cp308_fake_transport", False) is not True):
            raise RuntimeError("CP308 real worker/child/original/GUI HARD HOLD")
        if C.sizeof(C.c_void_p) != 8:
            raise RuntimeError("CP308 requires a native x64 process ABI")
        if (not isinstance(occurrence, Occurrence) or not occurrence.run
                or not occurrence.child or not isinstance(worker_id, str)
                or not worker_id):
            raise ValueError("one complete occurrence and worker binding required")
        self.expected_image = _fixed_image(fixed_clone_image, fixture_sha256)
        self.api, self.transport = api, transport
        self.occurrence, self.worker_id = occurrence, worker_id
        self.worker_job: int | None = None
        self.child_job: int | None = None
        self.worker_process: int | None = None
        self.worker_thread: int | None = None
        self.child_process: int | None = None
        self.offer: Offer | None = None
        self.ack: Ack | None = None
        self.report = Report()
        self._worker_exited = False
        self._child_exited = False
        self._job_terminated = {"child_job": False, "worker_job": False}
        self._job_zero = {"child_job": False, "worker_job": False}

    @property
    def fake_receipt_claim_matched(self) -> bool:
        """Caller-supplied in-memory claim only; no real resume authority."""
        return self.report.phase == "fake-receipt-claim-matched"

    def _retain(self) -> None:
        self.report.retained = {name: value for name, value in (
            ("worker_job", self.worker_job), ("child_job", self.child_job),
            ("worker_process", self.worker_process),
            ("worker_thread", self.worker_thread),
            ("child_process", self.child_process)) if value is not None}

    def _hold(self, reason: str) -> None:
        self.report.phase = "hold"
        self.report.errors.append(reason)
        self._retain()
        raise ProtocolHold(reason)

    def bind_created(self, *, worker_job: int, child_job: int,
                     worker_process: int, worker_thread: int) -> None:
        if self.report.phase != "new":
            self._hold("created handles may be bound once")
        values = (worker_job, child_job, worker_process, worker_thread)
        if any(_handle(value) is None for value in values) or len(set(values)) != 4:
            raise ValueError("four distinct creation-owned supervisor handles required")
        self.worker_job, self.child_job, self.worker_process, self.worker_thread = values
        self._retain()  # Ownership precedes queries, including an API exception.
        try:
            for name, handle in (("worker_job", worker_job), ("child_job", child_job)):
                if self.api.GetHandleInformation(handle) & HANDLE_FLAG_INHERIT:
                    self._hold(f"{name} is inheritable")
                if not self.api.GetJobLimitFlags(handle) & JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE:
                    self._hold(f"{name} lacks kill-on-close limit")
            if not self.api.IsProcessInJob(worker_process, worker_job):
                self._hold("creation-owned WORKER is not in WORKER job")
            if self.api.IsProcessInJob(worker_process, child_job):
                self._hold("WORKER unexpectedly belongs to CHILD job")
        except ProtocolHold:
            raise
        except BaseException as error:
            self._hold(f"worker job gate unknown: {type(error).__name__}: {error}")
        self.report.phase = "worker-gate-checked"

    def note_creator_interrupted(self) -> None:
        if self.report.phase not in ("worker-gate-checked", "hold") or self.offer:
            self._hold("creator interruption out of order")
        self.report.create_outcome = "unknown: interrupted inside CreateProcessW"
        self._hold("child existence and assignment unknown after creator interruption")

    def accept_offer(self, offer: Offer) -> Ack:
        if self.report.phase != "worker-gate-checked" or self.offer is not None:
            self._hold("offer missing, repeated or out of order")
        if (not isinstance(offer, Offer) or offer.occurrence != self.occurrence
                or not offer.offer_id or _handle(offer.worker_local_process_handle) is None
                or _handle(offer.pid) is None or _handle(offer.creation_filetime) is None
                or not isinstance(offer.image, str)
                or _image_key(offer.image) != _image_key(self.expected_image)):
            self._hold("invalid occurrence or fixed CP302 child offer")
        self.offer = offer  # Duplicate failure leaves this occurrence spent.
        self.report.create_outcome = "offered; identity unverified"
        duplicated = W.HANDLE()
        try:
            ok = self.api.DuplicateHandle(
                self.worker_process, offer.worker_local_process_handle,
                self.api.GetCurrentProcess(), C.byref(duplicated), 0, False,
                DUPLICATE_SAME_ACCESS)
        except BaseException as error:
            self.child_process = _handle(duplicated)
            self._hold(f"DuplicateHandle outcome unknown: {type(error).__name__}: {error}")
        self.child_process = _handle(duplicated)
        self._retain()
        if not ok or self.child_process is None:
            self._hold("DuplicateHandle child failed or returned no exact handle")
        try:
            if self.api.GetHandleInformation(self.child_process) & HANDLE_FLAG_INHERIT:
                self._hold("duplicated child handle is inheritable")
            pid = self.api.GetProcessId(self.child_process)
            filetime = self.api.GetProcessTimes(self.child_process)
            image = self.api.QueryFullProcessImageNameW(self.child_process)
            member = self.api.IsProcessInJob(self.child_process, self.child_job)
            in_worker_job = self.api.IsProcessInJob(self.child_process, self.worker_job)
        except ProtocolHold:
            raise
        except BaseException as error:
            self._hold(f"exact child identity or membership unknown: {type(error).__name__}: {error}")
        if (pid != offer.pid or filetime != offer.creation_filetime
                or not isinstance(image, str)
                or _image_key(image) != _image_key(offer.image)
                or _image_key(image) != _image_key(self.expected_image)
                or member is not True or in_worker_job is True):
            self._hold("duplicated child identity or CHILD job membership mismatch")
        self.report.create_outcome = "exact offered child identity and membership checked"
        ack = Ack(self.occurrence, offer.offer_id, pid, filetime, image)
        self.ack = ack
        self.report.ack_delivery = "unknown: send attempted"
        try:
            self.transport.send_ack(ack)
        except BaseException as error:
            self._hold(f"ACK delivery unknown: {type(error).__name__}: {error}")
        self.report.ack_delivery = "fake transport returned; worker acceptance unverified"
        self.report.phase = "ack-sent"
        return ack

    def receive_worker_receipt(self, receipt: WorkerReceipt) -> None:
        """Compare a caller-supplied claim, not an authenticated worker message."""
        if self.report.phase != "ack-sent":
            self._hold("worker receipt missing, repeated or out of order")
        if (not isinstance(receipt, WorkerReceipt) or receipt.worker_id != self.worker_id
                or receipt.ack != self.ack):
            self._hold("worker receipt does not match exact ACK and binding")
        self.report.worker_receipt = "matching caller-supplied fake worker claim"
        self.report.phase = "fake-receipt-claim-matched"

    def cleanup(self, *, wait_ms: int = 2000) -> Report:
        if type(wait_ms) is not int or not 0 <= wait_ms <= 5000:
            raise ValueError("bounded wait [0, 5000] required")
        self.report.phase = "cleanup"
        for name in ("child_job", "worker_job"):
            handle = getattr(self, name)
            if handle is None:
                continue
            try:
                confirmed = self.api.TerminateJobObject(handle, 0xE308) is True
                self._job_terminated[name] |= confirmed
                if not confirmed:
                    self.report.errors.append(f"{name} termination unconfirmed")
            except BaseException as error:
                self.report.errors.append(f"{name} termination unknown: {error}")
        exited = {}
        for name in ("child_process", "worker_process"):
            handle = getattr(self, name)
            if handle is None:
                continue
            try:
                exited[name] = self.api.WaitForSingleObject(handle, wait_ms) == WAIT_OBJECT_0
            except BaseException as error:
                exited[name] = False
                self.report.errors.append(f"{name} wait unknown: {error}")
            setattr(self.report, "child_wait" if name == "child_process" else "worker_wait",
                    "exited" if exited[name] else "not-confirmed")
            if name == "child_process":
                self._child_exited |= exited[name]
            else:
                self._worker_exited |= exited[name]
        for name in ("child_job", "worker_job"):
            handle = getattr(self, name)
            if handle is None:
                continue
            try:
                count = self.api.QueryInformationJobObject(handle)
                self.report.job_active[name] = count if type(count) is int and count >= 0 else None
                self._job_zero[name] = count == 0 and type(count) is int
            except BaseException as error:
                self.report.job_active[name] = None
                self._job_zero[name] = False
                self.report.errors.append(f"{name} active count unknown: {error}")
        # ActiveProcesses == 0 is only a momentary count. A live worker may
        # still hold its CHILD job duplicate and create a child later. A zero
        # count also cannot substitute for an exact offered-child exit receipt.
        # Neither job can be released until both termination calls, exact waits
        # for every retained process, and both job-zero queries confirm.
        jobs_safe = (all(self._job_terminated.values())
                     and self._worker_exited
                     and (self.child_process is None or self._child_exited)
                     and all(self._job_zero.values()))
        closable = {
            "child_process": self._child_exited,
            "worker_thread": self._worker_exited,
            "worker_process": self._worker_exited,
            "child_job": jobs_safe,
            "worker_job": jobs_safe,
        }
        for name in ("child_process", "worker_thread", "worker_process",
                     "child_job", "worker_job"):
            handle = getattr(self, name)
            if handle is None:
                continue
            if not closable[name]:
                self.report.close[name] = "deferred; exact handle retained"
                continue
            try:
                ok = self.api.CloseHandle(handle)
            except BaseException as error:
                ok = False
                self.report.errors.append(f"CloseHandle {name} unknown: {error}")
            self.report.close[name] = "confirmed" if ok else "failed or unknown"
            if ok:
                setattr(self, name, None)
        self._retain()
        self.report.phase = "cleanup-confirmed" if not self.report.retained else "cleanup-uncertain"
        return self.report


def run_real_fixed_child(*_args: object, **_kwargs: object) -> None:
    raise RuntimeError("CP308 real child/original/GUI HARD HOLD")
