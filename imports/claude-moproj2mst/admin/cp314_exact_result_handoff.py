"""Fake-only CP312 v2 RESULT handoff. Native execution remains HARD HOLD.

The supplied API must be an in-memory fake marked ``cp314_fake_api = True``.
Its methods follow CP310 NativeJobApi's typed LocalHandle/RemoteHandle
contract. If duplication is interrupted after publishing an output handle, it
must raise DuplicationInterrupted with that LocalHandle. The transport must be
marked ``cp314_bounded_fake_transport = True`` and implement
read(size, timeout_ms) and write(bytes, timeout_ms), each honoring its deadline.
No API or transport in this module starts or resumes a process.

All handles in a creator Receipt remain owned by the supervisor. A duplicated
child handle becomes supervisor-owned as soon as it is returned or exposed by
an interruption. HOLD never closes jobs, worker, pipes, or child handles.
"""
from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import PureWindowsPath
import math
import time

import cp310_native_job_adapter as observed
import cp313_native_worker_creator as creator
import cp313_worker_binary_protocol as wire


CHILD_IMAGE = r"C:\PROBLEM\s302\fixture.exe"
HANDLE_FLAG_INHERIT = 1
MAX_IO_MS = 5000
NATIVE_ENTRY = "HARD HOLD"


class Hold(RuntimeError):
    """Handoff stopped with a receipt that retains conservative ownership."""

    def __init__(self, reason: str, receipt: HandoffReceipt):
        self.receipt = receipt
        super().__init__(reason)


class DuplicationInterrupted(RuntimeError):
    """Fake DuplicateHandle interruption, including any published local output."""

    def __init__(self, handle: observed.LocalHandle | None = None):
        if handle is not None and not isinstance(handle, observed.LocalHandle):
            raise TypeError("partial duplicate must be a supervisor-local handle")
        self.handle = handle
        super().__init__("DuplicateHandle outcome unknown")


@dataclass
class HandoffReceipt:
    phase: str = "new"
    result: wire.WorkerTelemetry | None = None
    child_process: observed.LocalHandle | None = None
    child_pid: int | None = None
    child_creation_filetime: int | None = None
    child_image: str | None = None
    worker_member: bool | None = None
    child_member: bool | None = None
    ack_outcome: str = "not-attempted"
    errors: list[str] = field(default_factory=list)
    identity_verified: bool = False
    uncertain: bool = True


def create_live_handoff(*_args, **_kwargs):
    raise RuntimeError("CP314 native RESULT handoff HARD HOLD")


class ExactResultHandoff:
    """Consume one exact RESULT and write ACK only after independent checks.

    The caller captures occurrence_started and occurrence_deadline using the
    same monotonic clock before entering CP313 WORKER creation. This module
    refuses a fresh per-handoff budget: creator time consumes the same bound.
    """

    def __init__(self, prepared: creator.Receipt, init: wire.Init, transport,
                 api, *, occurrence_started: float, occurrence_deadline: float,
                 clock=time.monotonic):
        if type(prepared) is not creator.Receipt or type(init) is not wire.Init:
            raise TypeError("CP313 creator Receipt and CP312 Init required")
        if getattr(transport, "cp314_bounded_fake_transport", False) is not True:
            raise TypeError("bounded fake transport required")
        if getattr(api, "cp314_fake_api", False) is not True:
            raise TypeError("fake CP310-compatible API required")
        if (not callable(clock) or type(occurrence_started) not in (int, float) or
                type(occurrence_deadline) not in (int, float) or
                not math.isfinite(occurrence_started) or
                not math.isfinite(occurrence_deadline) or
                not 0 < occurrence_deadline - occurrence_started <= MAX_IO_MS / 1000):
            raise ValueError("pre-creation monotonic deadline within 5000 ms required")
        for method in ("read", "write"):
            if not callable(getattr(transport, method, None)):
                raise TypeError(f"transport.{method} required")
        for method in ("GetCurrentProcess", "DuplicateHandle", "GetProcessId",
                       "GetProcessTimes", "QueryFullProcessImageNameW",
                       "GetHandleInformation", "IsProcessInJob"):
            if not callable(getattr(api, method, None)):
                raise TypeError(f"API.{method} required")
        self.prepared, self.init, self.transport, self.api = prepared, init, transport, api
        self.clock, self.deadline = clock, occurrence_deadline
        self.receipt = HandoffReceipt()
        self._used = False

    def _hold(self, reason: str):
        self.receipt.phase = "hold"
        self.receipt.errors.append(reason)
        raise Hold(reason, self.receipt)

    def _deadline_ms(self, deadline: float) -> int:
        remaining = int((deadline - self.clock()) * 1000)
        if remaining <= 0:
            self._hold("bounded transport deadline expired")
        return remaining

    def _read_result(self, deadline: float) -> bytes:
        chunks = []
        remaining = wire.RESULT.size
        for _ in range(wire.RESULT.size):
            if remaining == 0:
                break
            part = self.transport.read(remaining, self._deadline_ms(deadline))
            if type(part) is not bytes or not 0 < len(part) <= remaining:
                self._hold("missing, partial, or invalid RESULT")
            chunks.append(part)
            remaining -= len(part)
        if remaining:
            self._hold("partial RESULT exceeded read-call bound")
        return b"".join(chunks)

    def _write_ack(self, frame: bytes, deadline: float) -> None:
        offset = 0
        self.receipt.ack_outcome = "unknown: ACK write entered"
        for _ in range(wire.ACK.size):
            if offset == len(frame):
                break
            count = self.transport.write(frame[offset:], self._deadline_ms(deadline))
            if type(count) is not int or not 0 < count <= len(frame) - offset:
                self._hold("partial or invalid ACK write; worker acceptance unknown")
            offset += count
        if offset != len(frame):
            self._hold("partial ACK exceeded write-call bound; worker acceptance unknown")
        self.receipt.ack_outcome = "written-unconfirmed"

    def _creator_handles(self):
        p = self.prepared
        if (p.phase != "prepared-suspended" or
                p.create_outcome != "returned-success" or
                p.duplicate_outcome != "returned-success" or
                p.errors or p.worker_terminated or p.worker_waited or
                not p.pid or not p.tid):
            self._hold("WORKER preparation is not complete and suspended")
        names = ("worker_process", "worker_thread", "worker_job", "child_job",
                 "input_write", "output_read")
        if any(type(p.local.get(name)) is not int or p.local[name] <= 0 for name in names):
            self._hold("creation-owned WORKER, jobs, and supervisor pipe ends required")
        if len({p.local[name] for name in names}) != len(names):
            self._hold("creation-owned local handles alias")
        if "input_read" in p.local or "output_write" in p.local:
            self._hold("WORKER pipe ends remain supervisor-owned after preparation")
        remote = p.remote_child_job
        if (not isinstance(remote, creator.RemoteHandle) or
                remote.owner_process != p.local["worker_process"] or
                remote.value != self.init.child_job):
            self._hold("INIT CHILD job is not CP313's worker-local duplicate")
        return (observed.LocalHandle(p.local[name]) for name in
                ("worker_process", "worker_job", "child_job"))

    def run(self) -> HandoffReceipt:
        if self._used:
            raise RuntimeError("RESULT handoff is one-use")
        self._used = True
        r = self.receipt
        try:
            worker, worker_job, child_job = self._creator_handles()
            r.phase = "await-result"
            frame = self._read_result(self.deadline)
            r.result = wire.decode_result(frame)
            result = r.result
            if not wire.result_matches_init(result, self.init):
                self._hold("RESULT nonce does not bind to INIT")
            if not (result.create_called == result.create_ok == result.child_query_ok ==
                    result.child_member == 1):
                self._hold("RESULT does not report a successfully queried child")
            r.phase = "duplicate-child"
            remote_child = observed.RemoteHandle(result.child_process_handle, worker.value)
            current = self.api.GetCurrentProcess()
            try:
                duplicated = self.api.DuplicateHandle(worker, remote_child, current)
            except DuplicationInterrupted as exc:
                if exc.handle is not None:
                    r.child_process = exc.handle
                raise
            if not isinstance(duplicated, observed.LocalHandle):
                self._hold("duplicate did not return supervisor-local child handle")
            r.child_process = duplicated  # Ownership starts before any query.
            if duplicated.value in self.prepared.local.values():
                self._hold("duplicated child aliases an owned local handle")
            r.phase = "verify-child"
            r.child_pid = self.api.GetProcessId(duplicated)
            if r.child_pid != result.pid:
                self._hold("independent child PID differs from RESULT")
            r.child_creation_filetime = self.api.GetProcessTimes(duplicated)
            if type(r.child_creation_filetime) is not int or r.child_creation_filetime <= 0:
                self._hold("child creation FILETIME is zero or invalid")
            r.child_image = self.api.QueryFullProcessImageNameW(duplicated)
            if (type(r.child_image) is not str or
                    str(PureWindowsPath(r.child_image)).casefold() != CHILD_IMAGE.casefold()):
                self._hold("independent child image differs from fixed s302 image")
            if self.api.GetHandleInformation(duplicated) & HANDLE_FLAG_INHERIT:
                self._hold("supervisor child handle is inheritable")
            r.worker_member = self.api.IsProcessInJob(duplicated, worker_job)
            if r.worker_member is not True:
                self._hold("child is not in exact WORKER job")
            r.child_member = self.api.IsProcessInJob(duplicated, child_job)
            if r.child_member is not True:
                self._hold("child is not in exact CHILD job")
            r.identity_verified = True
            r.phase = "ack"
            ack = wire.encode_ack(wire.Ack(result.child_process_handle, self.init.nonce))
            if len(ack) != wire.ACK.size:
                self._hold("ACK length is not 48 bytes")
            self._write_ack(ack, self.deadline)
            r.phase = "verified-ack-written"
            # There is no worker receipt. Even a complete ACK write leaves
            # acceptance and worker cleanup unconfirmed.
            return r
        except Hold:
            raise
        except BaseException as exc:
            self._hold(f"{type(exc).__name__}: {exc}")
