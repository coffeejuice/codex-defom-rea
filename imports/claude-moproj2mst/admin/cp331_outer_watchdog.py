"""CP331 outer containment prerequisite; public launch remains HARD HOLD.

Explicit methods are native-capable with CP325 BoundExports. Import is inert.
No child IPC or exact WORKER-handle acknowledgement exists here, so this is not
an approved CP330 trial launcher. A zero runner exit is never trial acceptance.

The unnamed outer job contains only the creation-returned disposable Python
runner and descendants. Creation-time JOB_LIST has no assignment/breakaway
fallback. Host job membership is recorded, but membership alone cannot prove
nested-job compatibility: host restrictions may reject CreateProcessW. No host
job is modified. Synchronous Win32 calls can overrun the absolute deadline;
this owner supplies bounded waits, not an OS hard real-time guarantee. Image
path/PID/timestamp checks establish handle identity, not mapped-byte attestation.
"""
from __future__ import annotations

import ctypes as C
from dataclasses import dataclass, field
from pathlib import Path
import subprocess
import sys

import cp321_native_exports as abi
import cp325_real_bindings as binding
import cp329_native_worker_creator as creator

NATIVE_ENTRY = "HARD HOLD"
Hold = abi.Hold
RUNNER_IMAGE = str(Path(sys.executable).resolve())
RUNNER_SCRIPT = str(Path(__file__).resolve().with_name("cp330_native_inert_runner.py"))
FIXED_ENTRY_MODULE = "cp335_fixed_runner_entry"
FIXED_ENTRY_TAG = "--cp334-client"
RUNNER_CWD = str(Path(__file__).resolve().parent)
KILL_ON_CLOSE, INHERIT = creator.KILL_ON_CLOSE, creator.INHERIT
JOB_LIMITS, JOB_ACCOUNTING = creator.JOB_LIMITS, creator.JOB_ACCOUNTING
HANDLE_LIST, JOB_LIST = creator.HANDLE_LIST, creator.JOB_LIST
CREATE_FLAGS, INVALID_HANDLE = creator.CREATE_FLAGS, creator.INVALID_HANDLE
RUNNER_CREATE_FLAGS = 4 | 0x80000 | 0x00000008  # suspended, extended startup, detached console


@dataclass
class Receipt:
    phase: str = "new"
    deadline: int | None = None
    host_in_job: bool | None = None
    local: dict = field(default_factory=dict)
    pid: int | None = None
    tid: int | None = None
    creation_time: int | None = None
    create_outcome: str = "not-attempted"
    resume_outcome: str = "not-attempted"
    handoff_state: str = "unclaimed"
    handoff_nonce: bytes | None = None
    handoff_ack_digest: bytes | None = None
    job_terminated: bool = False
    process_terminated: bool = False
    runner_waited: bool = False
    job_empty: bool = False
    errors: list = field(default_factory=list)


class WatchdogFailure(Hold):
    def __init__(self, error, owner):
        self.owner, self.receipt = owner, owner.receipt
        super().__init__(str(error))


class OuterWatchdog:
    """One-use owner. Retain this object until close() reports closed.

    Call start_occurrence() before preparing caller-owned private IPC.
    Caller-owned inherited pipe handles must remain alive until creation returns.
    This prerequisite restricts inheritance to exactly those two handles; it does
    not construct IPC or infer child acknowledgement from arbitrary bytes.
    """
    def __init__(self, exports, *, timeout_ms=5000, cleanup_wait_ms=100):
        if not isinstance(exports, binding.BoundExports):
            raise TypeError("CP325 BoundExports required")
        if type(timeout_ms) is not int or not 0 < timeout_ms <= 5000:
            raise ValueError("timeout must be 1..5000ms")
        if type(cleanup_wait_ms) is not int or not 0 <= cleanup_wait_ms <= 1000:
            raise ValueError("cleanup wait must be 0..1000ms")
        self.exports, self.timeout_ms = exports, timeout_ms
        self.cleanup_wait_ms = cleanup_wait_ms
        self.receipt, self._used = Receipt(), False
        self._attributes, self._last_tick = None, None
        self._endpoint_handles = set()
        self._deadline = None
        self._ownership_uncertain = []
        self._handoff_nonce = self._handoff_ack_digest = None
        # Creation-returned handles remain authoritative if a diagnostic
        # receipt is changed by a callback during a later native call.
        self._owned_handles = {}
        self._receipt_identity = self.receipt
        self._local_receipt = self.receipt.local
        self._runner_success_observed = False
        self._runner_success_released = False
        self._abort_closed = False
        self._create_attempted = False
        self._no_child_proven = False
        self._abort_job_terminated = False
        self._abort_process_terminated = False
        self._abort_runner_waited = False
        self._abort_job_empty = False

    @property
    def deadline(self):
        return self._deadline

    _call = creator.NativeWorkerCreator._call
    _ok = creator.NativeWorkerCreator._ok
    def _own(self, name, value):
        try:
            owned = creator.NativeWorkerCreator._own(self, name, value)
            self._owned_handles[name] = owned
            return owned
        except BaseException:
            # Never silently discard anomalous nonzero creation outputs. These
            # are not safe termination targets, and block releasing containment.
            self._ownership_uncertain.append((name, getattr(value, "value", value)))
            raise

    _flags = creator.NativeWorkerCreator._flags
    _query = creator.NativeWorkerCreator._query
    _active = creator.NativeWorkerCreator._active
    _job_verified = creator.NativeWorkerCreator._job_verified
    _delete_attributes = creator.NativeWorkerCreator._delete_attributes

    def _tick(self):
        now = self._call("GetTickCount64")
        if (type(now) is not int or not 0 <= now < 2**64 or
                (self._last_tick is not None and now < self._last_tick)):
            raise Hold("invalid or regressing clock")
        self._last_tick = now
        return now

    def start_occurrence(self):
        """Allocate once, BEFORE caller prepares private IPC/security owners.

        Construct IPC owners with this read-only deadline, then prepare them and
        pass their two child endpoints to prepare_suspended(). No later step
        renews the occurrence or excludes IPC preparation time from its budget.
        """
        if self._deadline is not None or self._used or self.receipt.phase != "new":
            raise Hold("outer occurrence can only start once")
        # Mark one-use before reading the clock, including an uncertain read.
        self.receipt.phase = "starting-occurrence"
        try:
            deadline = self._tick() + self.timeout_ms
            if deadline >= 2**64:
                raise Hold("deadline overflow")
            self._deadline = deadline
            self.receipt.deadline = deadline
            self.receipt.phase = "occurrence-started"
            return deadline
        except BaseException as error:
            self.receipt.errors.append(str(error))
            self.receipt.phase = "hold"
            raise WatchdogFailure(error, self) from error

    def _budget(self):
        if type(self.deadline) is not int:
            raise Hold("start_occurrence required before IPC preparation")
        left = self.deadline - self._tick()
        if self.receipt.deadline != self.deadline or not 0 < left <= self.timeout_ms:
            raise Hold("original outer deadline expired or changed")
        return left

    def claim_handoff(self, occurrence_nonce):
        """Consume this occurrence once across every CP332 owner.

        Claim before the first bootstrap transfer. A failed or uncertain handoff
        never resets this claim, even when a different object retries it.
        """
        if type(occurrence_nonce) is not bytes or len(occurrence_nonce) != 32:
            raise Hold("exact 32-byte occurrence nonce required")
        if self._handoff_nonce is not None:
            raise Hold("outer occurrence handoff is already claimed")
        if self.receipt.phase != "running-unacknowledged":
            raise Hold("exact running runner required before handoff claim")
        self._budget()
        self._handoff_nonce = occurrence_nonce
        self.receipt.handoff_nonce = occurrence_nonce
        self.receipt.handoff_state = "claimed"

    def record_handoff_ack(self, occurrence_nonce, offer_digest):
        """Record a completed exact ACK write; this never grants another claim.

        CP332 supplies SHA-256 of its complete encoded OFFER after its bounded
        ACK transfer succeeds. A claimed occurrence stays consumed if transfer
        fails or its outcome is uncertain. This record is not trial success.
        """
        if (type(occurrence_nonce) is not bytes or len(occurrence_nonce) != 32 or
                occurrence_nonce != self._handoff_nonce):
            raise Hold("ACK must match the claimed outer occurrence")
        if type(offer_digest) is not bytes or len(offer_digest) != 32:
            raise Hold("exact 32-byte OFFER digest required")
        if self._handoff_ack_digest is not None:
            raise Hold("outer occurrence ACK is already recorded")
        if self.receipt.phase != "running-unacknowledged":
            raise Hold("exact running runner required before ACK record")
        self._budget()
        self._handoff_ack_digest = offer_digest
        self.receipt.handoff_ack_digest = offer_digest
        self.receipt.handoff_state = "acknowledged"

    def _stamp(self):
        times = [abi.FILETIME() for _ in range(4)]
        self._ok("GetProcessTimes", self.receipt.local["runner_process"],
                 *(C.byref(t) for t in times))
        created = (times[0].dwHighDateTime << 32) | times[0].dwLowDateTime
        # Exit FILETIME is undefined until termination; never inspect it to
        # decide whether a process is alive. Only the exact wait handle proves
        # the still-suspended creation-returned process has not terminated.
        status = self._call("WaitForSingleObject", self.receipt.local["runner_process"], 0)
        if not created or type(status) is not int or status != 258:
            raise Hold("positive creation time and exact runner WAIT_TIMEOUT required")
        return created

    def _verify(self):
        r = self.receipt
        self._budget()
        if not r.pid or not r.tid or not all(n in r.local for n in ("runner_process", "runner_thread")):
            raise Hold("complete creation-returned runner identity required")
        process = r.local["runner_process"]
        if self._call("GetProcessId", process) != r.pid:
            raise Hold("exact runner PID mismatch")
        if any(self._flags(r.local[n]) & INHERIT for n in r.local):
            raise Hold("owned job/process/thread must be noninheritable")
        path, count = C.create_unicode_buffer(32768), abi.DWORD(32768)
        self._ok("QueryFullProcessImageNameW", process, 0, path, C.byref(count))
        if (not 0 < count.value < len(path) or count.value != len(path.value) or
                path.value.casefold() != RUNNER_IMAGE.casefold()):
            raise Hold("exact disposable Python image mismatch")
        member = abi.BOOL()
        self._ok("IsProcessInJob", process, r.local["outer_job"], C.byref(member))
        if member.value != 1:
            raise Hold("runner is not in exact outer job")
        self._job_verified(r.local["outer_job"])
        if self._active(r.local["outer_job"]) != 1:
            raise Hold("sole suspended runner required")
        stamp = self._stamp()
        if r.creation_time is None:
            r.creation_time = stamp
        elif stamp != r.creation_time:
            raise Hold("exact runner creation timestamp changed")
        self._budget()

    def _attribute_list(self, job, handles):
        size = abi.SIZE_T()
        ok = self._call("InitializeProcThreadAttributeList", None, 2, 0, C.byref(size))
        error = int(self.exports.last_error())
        if ok or error != 122 or not 0 < size.value <= 1048576:
            raise Hold("expected bounded attribute-list probe")
        storage = C.create_string_buffer(size.value)
        address = C.cast(storage, abi.PVOID)
        jobs = (abi.HANDLE * 1)(job)
        inherited = (abi.HANDLE * 2)(*handles)
        self._attributes = [address, storage, jobs, inherited, None]
        ok = self._call("InitializeProcThreadAttributeList", address, 2, 0, C.byref(size))
        if not ok:
            error = int(self.exports.last_error())
            self._attributes[4] = False
            raise creator.ApiFailure("InitializeProcThreadAttributeList", error)
        self._attributes[4] = True
        for kind, values in ((JOB_LIST, jobs), (HANDLE_LIST, inherited)):
            self._budget()
            self._ok("UpdateProcThreadAttribute", address, 0, kind,
                     C.cast(values, abi.PVOID), C.sizeof(values), None, None)
        return address

    def prepare_suspended(self, *, child_read, child_write):
        if self._used:
            raise Hold("watchdog is one-use")
        self._used = True
        r = self.receipt
        try:
            handles = (child_read, child_write)
            if (any(type(h) is not int or h <= 0 or h == INVALID_HANDLE for h in handles)
                    or child_read == child_write):
                raise Hold("two distinct valid caller-owned IPC handles required")
            self._endpoint_handles = set(handles)
            if r.phase != "occurrence-started":
                raise Hold("start_occurrence required before private IPC preparation")
            self._budget()  # includes all elapsed caller IPC preparation time
            current, member = self._call("GetCurrentProcess"), abi.BOOL()
            self._ok("IsProcessInJob", current, None, C.byref(member))
            if member.value not in (0, 1):
                raise Hold("invalid host job membership")
            r.host_in_job = bool(member.value)
            for handle in handles:
                if not self._flags(handle) & INHERIT:
                    raise Hold("explicit child endpoints must be inheritable")
            job = creator.NativeWorkerCreator._job(self, "outer_job")
            address = self._attribute_list(job, handles)
            startup = abi.STARTUPINFOEXW()
            startup.StartupInfo.cb = C.sizeof(startup)
            startup.StartupInfo.dwFlags = 0x100
            startup.StartupInfo.hStdInput = child_read
            startup.StartupInfo.hStdOutput = startup.StartupInfo.hStdError = child_write
            startup.lpAttributeList = address.value
            # Isolated Python excludes the script directory from sys.path.
            # Explicitly admit only this fixed reviewed admin directory so the
            # existing CP330 module can import its sibling prerequisites.
            # Keep the contained entry fixed and isolated.  The module receives
            # only CP334's reviewed two-handle/deadline grammar; no path or
            # executable is accepted from the caller.  Its public __main__ is
            # still HARD HOLD; this invokes only its explicit fixed entry API.
            bootstrap = ("import sys;sys.path.insert(0," + repr(RUNNER_CWD) +
                         ");import " + FIXED_ENTRY_MODULE +
                         " as e;e.FixedRunnerEntry(sys.argv[1:]).run()")
            command = C.create_unicode_buffer(subprocess.list2cmdline(
                [RUNNER_IMAGE, "-I", "-B", "-c", bootstrap, FIXED_ENTRY_TAG,
                 str(child_read), str(child_write), str(self.deadline)]))
            info = abi.PROCESS_INFORMATION()
            self._budget()
            self._create_attempted = True
            r.create_outcome = "unknown: CreateProcessW entered"
            try:
                ok = self._call("CreateProcessW", RUNNER_IMAGE, command, None, None,
                    True, RUNNER_CREATE_FLAGS, None, RUNNER_CWD,
                    C.byref(startup.StartupInfo), C.byref(info))
                error = None if ok else int(self.exports.last_error())
            finally:
                capture_errors = []
                for name, value in (("runner_process", info.hProcess), ("runner_thread", info.hThread)):
                    if value:
                        try:
                            self._own(name, value)
                        except BaseException as exc:
                            capture_errors.append(str(exc))
                r.pid, r.tid = int(info.dwProcessId) or None, int(info.dwThreadId) or None
                if capture_errors:
                    raise Hold("; ".join(capture_errors))
            if not ok:
                # A conventional failure with entirely zero outputs establishes no
                # child; partial outputs or an exception remain uncertain.
                if not any((info.hProcess, info.hThread, info.dwProcessId, info.dwThreadId)):
                    self._no_child_proven = True
                    r.create_outcome = "returned-failure-no-child"
                raise creator.ApiFailure("CreateProcessW", error)
            r.create_outcome = "returned-success"
            self._verify()
            self._delete_attributes()
            r.phase = "prepared-suspended"
            return r
        except BaseException as error:
            r.errors.append(str(error))
            r.phase = "hold"
            self.close()
            raise WatchdogFailure(error, self) from error

    def resume_once(self):
        r = self.receipt
        try:
            if r.phase != "prepared-suspended" or r.resume_outcome != "not-attempted":
                raise Hold("one exact suspended preparation required")
            self._verify()  # recheck stamp/PID/image/job immediately before resume
            r.resume_outcome = "unknown: ResumeThread entered"
            previous = self._call("ResumeThread", r.local["runner_thread"])
            if type(previous) is not int or previous != 1:
                raise Hold("exactly one previous suspension required")
            r.resume_outcome = "resumed-once"
            self._budget()
            r.phase = "running-unacknowledged"
            return r
        except BaseException as error:
            r.errors.append(str(error))
            r.phase = "hold"
            self.close()
            raise WatchdogFailure(error, self) from error

    def observe_runner(self):
        """Bounded exact wait; runner exit never substitutes for WORKER IPC ack."""
        try:
            if self.receipt.phase != "running-unacknowledged":
                raise Hold("running exact runner required")
            left = self._budget()
            status = self._call("WaitForSingleObject", self.receipt.local["runner_process"], left)
            if type(status) is not int or status != 0:
                raise Hold("runner timeout or exact wait failure")
            raise Hold("runner EOF/exit: child IPC and exact WORKER acknowledgement unavailable")
        except BaseException as error:
            self.receipt.errors.append(str(error))
            self.receipt.phase = "hold"
            self.close()
            raise WatchdogFailure(error, self) from error

    def _success_custody(self):
        if (self.receipt is not self._receipt_identity or
                self.receipt.local is not self._local_receipt or
                self.receipt.local.keys() != self._owned_handles.keys() or
                any(type(value) is not int or value != self._owned_handles[name]
                    for name, value in self.receipt.local.items())):
            raise Hold("creation-returned runner handle custody changed")

    def _finish_runner_success(self):
        """Retry only handle release after runner exit and empty job were proven."""
        if not self._runner_success_observed or self._runner_success_released:
            raise Hold("private exact runner success proof required")
        self._success_custody()
        self._delete_attributes()
        for name in ("runner_thread", "runner_process", "outer_job"):
            if name in self._owned_handles:
                handle = self._owned_handles[name]
                self._ok("CloseHandle", handle)
                # CloseHandle returned success; only now relinquish ownership.
                del self._owned_handles[name]
                if (type(self.receipt.local.get(name)) is int and
                        self.receipt.local[name] == handle):
                    del self.receipt.local[name]
                self._success_custody()
        self._runner_success_released = True
        self.receipt.phase = "runner-closed-success"
        return self.receipt

    def observe_runner_success(self):
        """Prove exact RUNNER exit/outer-job emptiness, not trial acceptance.

        The caller must separately verify the duplicated WORKER and terminal
        protocol before it treats the occurrence as successful. A failed
        observation aborts containment; a failed handle close after the proof
        retains the private handles for a release-only retry.
        """
        r = self.receipt
        try:
            if self._runner_success_released:
                return r
            if self._runner_success_observed:
                return self._finish_runner_success()
            if (r.phase != "running-unacknowledged" or
                    self._handoff_nonce is None or self._handoff_ack_digest is None):
                raise Hold("acknowledged running runner required")
            self._success_custody()
            remaining = self._budget()
            self._success_custody()
            process = self._owned_handles["runner_process"]
            job = self._owned_handles["outer_job"]
            status = self._call("WaitForSingleObject", process, remaining)
            self._success_custody()
            if type(status) is not int or status != 0:
                raise Hold("exact runner success wait unresolved")
            self._budget()
            self._success_custody()
            code = abi.DWORD()
            self._ok("GetExitCodeProcess", process, C.byref(code))
            self._success_custody()
            if code.value != 0:
                raise Hold("exact runner exit code is nonzero")
            self._budget()
            self._success_custody()
            if self._active(job) != 0:
                raise Hold("outer job is not empty after runner exit")
            self._budget()
            self._success_custody()
            r.runner_waited = r.job_empty = True
            self._runner_success_observed = True
            r.phase = "runner-success-observed"
            return self._finish_runner_success()
        except BaseException as error:
            if self.receipt is not r:
                self.receipt = r  # retain the creation-owned receipt on tampering
            r.errors.append(str(error))
            if not self._runner_success_observed:
                r.phase = "hold"
                self.close()
            raise WatchdogFailure(error, self) from error

    def close(self):
        """Retryable bounded cleanup. Unknown owners are never silently dropped."""
        if self.receipt is not self._receipt_identity:
            self.receipt = self._receipt_identity
            self.receipt.errors.append("public runner receipt identity changed")
        r = self.receipt
        if self._abort_closed or self._runner_success_released:
            return r
        try:
            attempted = self._create_attempted and not self._no_child_proven
            if attempted:
                # Independent exact process fallback even when job termination fails.
                if not self._abort_job_terminated and "outer_job" in self._owned_handles:
                    try:
                        self._ok("TerminateJobObject", self._owned_handles["outer_job"], 0xE331)
                        self._abort_job_terminated = True
                        r.job_terminated = True
                    except BaseException as error:
                        r.errors.append(str(error))
                if ("runner_process" not in self._owned_handles and
                        not self._abort_runner_waited and not self._runner_success_observed):
                    raise Hold("uncertain creation: no exact process wait handle; owners retained")
                if not self._abort_runner_waited and not self._runner_success_observed:
                    process = self._owned_handles["runner_process"]
                    status = self._call("WaitForSingleObject", process, 0)
                    if status != 0:
                        try:
                            self._ok("TerminateProcess", process, 0xE331)
                            self._abort_process_terminated = True
                            r.process_terminated = True
                        except BaseException as error:
                            r.errors.append(str(error))
                        status = self._call("WaitForSingleObject", process, self.cleanup_wait_ms)
                    if type(status) is not int or status != 0:
                        raise Hold("exact runner termination wait unresolved")
                    self._abort_runner_waited = True
                    r.runner_waited = True
                if "outer_job" in self._owned_handles:
                    self._abort_job_empty = self._active(self._owned_handles["outer_job"]) == 0
                    r.job_empty = self._abort_job_empty
                if not self._abort_job_empty and not self._runner_success_observed:
                    raise Hold("outer job active-process accounting unresolved")
            if self._ownership_uncertain:
                raise Hold("anomalous creation handle ownership retained")
            self._delete_attributes()
            # Close job last; any failed close preserves remaining handles.
            for name in ("runner_thread", "runner_process", "outer_job"):
                if name in self._owned_handles:
                    handle = self._owned_handles[name]
                    self._ok("CloseHandle", handle)
                    del self._owned_handles[name]
                    if type(r.local.get(name)) is int and r.local[name] == handle:
                        del r.local[name]
            self._success_custody()
            self._abort_closed = True
            r.phase = "closed"
        except BaseException as error:
            r.errors.append(str(error))
            r.phase = "hold"
        return r


def launch_trial(*_args, **_kwargs):
    raise Hold("CP331 launch HARD HOLD: child IPC, exact WORKER handle acknowledgement and independent review required")


if __name__ == "__main__":
    launch_trial()
