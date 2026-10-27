"""CP332 exact-handle handoff prerequisite. Import and live factory remain inert.

Explicit methods accept CP325 exports and CP328 overlapped channels. An ACK
authorizes only the offered suspended WORKER; it is not trial acceptance or
mapped-image attestation. CP330 must validate the full ACK before ResumeThread.
Synchronous exports can overrun deadlines. Retain failed owners until close()
returns True, including pipe operations with uncertain cancellation outcomes.
"""
from __future__ import annotations

import ctypes as C
from dataclasses import dataclass
import hashlib
import struct
from types import MappingProxyType

import cp321_native_exports as abi
import cp325_real_bindings as binding
import cp327_native_image_pin as image
import cp329_native_worker_creator as creator

NATIVE_ENTRY = "HARD HOLD"
Hold = abi.Hold
MAGIC, BOOTSTRAP, OFFER, ACK, IMAGE_ID = 0x32333343, 1, 2, 3, 316
BOOT = struct.Struct("<IIQ32s")
FRAME = struct.Struct("<IIQQIIQ32sQ")
INVALID = C.c_void_p(-1).value


class _PID_LIST_8(C.Structure):
    """Fixed diagnostic buffer for JobObjectBasicProcessIdList (Win64)."""
    _fields_ = [("assigned", abi.DWORD), ("listed", abi.DWORD),
                ("ids", abi.ULONG_PTR * 8)]


def _uint(value, bits, positive=True):
    if type(value) is not int or not (int(positive) <= value < 2**bits):
        raise Hold("invalid fixed unsigned field")
    return value


def _nonce(value):
    if type(value) is not bytes or len(value) != 32 or not any(value):
        raise Hold("nonzero 32-byte nonce required")
    return value


def encode_bootstrap(deadline, nonce):
    return BOOT.pack(MAGIC, BOOTSTRAP, _uint(deadline, 64), _nonce(nonce))


def decode_bootstrap(raw):
    if type(raw) is not bytes or len(raw) != BOOT.size:
        raise Hold("exact BOOTSTRAP frame required")
    magic, kind, deadline, nonce = BOOT.unpack(raw)
    if (magic, kind) != (MAGIC, BOOTSTRAP):
        raise Hold("wrong BOOTSTRAP magic/order")
    encode_bootstrap(deadline, nonce)
    return deadline, nonce


@dataclass(frozen=True)
class Offer:
    process: int
    job: int
    pid: int
    creation_time: int
    nonce: bytes
    deadline: int


def _encode(offer, kind):
    if not isinstance(offer, Offer):
        raise TypeError("Offer required")
    for handle in (offer.process, offer.job):
        _uint(handle, 64)
        if handle >= INVALID - 15:
            raise Hold("pseudo/invalid remote token")
    if offer.process == offer.job:
        raise Hold("distinct remote tokens required")
    return FRAME.pack(MAGIC, kind, offer.process, offer.job,
        _uint(offer.pid, 32), IMAGE_ID, _uint(offer.creation_time, 64),
        _nonce(offer.nonce), _uint(offer.deadline, 64))


def encode_offer(offer):
    return _encode(offer, OFFER)


def encode_ack(offer):
    return _encode(offer, ACK)


def decode_offer(raw):
    if type(raw) is not bytes or len(raw) != FRAME.size:
        raise Hold("exact OFFER frame required")
    magic, kind, process, job, pid, ident, stamp, nonce, deadline = FRAME.unpack(raw)
    if (magic, kind, ident) != (MAGIC, OFFER, IMAGE_ID):
        raise Hold("wrong OFFER magic/order/image")
    offer = Offer(process, job, pid, stamp, nonce, deadline)
    encode_offer(offer)
    return offer


def validate_ack(raw, offer):
    if type(raw) is not bytes or raw != encode_ack(offer):
        raise Hold("ACK must bind every OFFER field and nonce")
    return True


class ExactWorkerHandoff:
    """Watchdog-side one-use owner; never opens a process by PID.

    Watchdog remains the sole owner of creation-returned runner/outer handles;
    this object strongly retains it and freezes the exact tokens. All duplicate
    outputs are captured in finally before clock/error/verification operations.
    The channel must already be CP328-connected with the same original deadline.
    """
    def __init__(self, exports, *, watchdog, pipe_owner, nonce):
        if not isinstance(exports, binding.BoundExports):
            raise TypeError("CP325 BoundExports required")
        if watchdog.exports is not exports or pipe_owner.exports is not exports:
            raise Hold("same retained BoundExports required")
        if not all(callable(getattr(watchdog, name, None)) for name in
                   ("_tick", "claim_handoff", "record_handoff_ack")):
            raise Hold("CP331 shared clock and occurrence handoff registry required")
        self.exports, self.watchdog, self.pipe_owner = exports, watchdog, pipe_owner
        self._bound_exports, self._watchdog_owner = exports, watchdog
        self.nonce = _nonce(nonce)
        self._occurrence_nonce = self.nonce
        self.deadline = watchdog.deadline
        _uint(self.deadline, 64)
        if pipe_owner.deadline != self.deadline or pipe_owner.receipt is None:
            raise Hold("connected channel with original outer deadline required")
        self._channel_owner = pipe_owner
        self._channel_receipt = pipe_owner.receipt
        self._channel_fields = ("parent_write", "client_read", "parent_read", "client_write")
        self._channel_handles = tuple(getattr(self._channel_receipt, name)
                                      for name in self._channel_fields)
        if (any(type(value) is not int or not 0 < value < INVALID - 15
                for value in self._channel_handles) or len(set(self._channel_handles)) != 4):
            raise Hold("four distinct valid connected endpoint handles required")
        self._parent_write, _, self._parent_read, _ = self._channel_handles
        self.runner = watchdog.receipt.local["runner_process"]
        self.outer = watchdog.receipt.local["outer_job"]
        self._runner_handle, self._outer_handle = self.runner, self.outer
        self._reserved = set(watchdog.receipt.local.values())
        self._reserved.update(self._channel_handles)
        self.local, self.uncertain = {}, []
        self._public_local = self.local
        self._owned_outputs = MappingProxyType({})  # frozen authoritative ownership snapshot
        self.phase, self.offer, self._last_tick = "new", None, None
        self._validated_offer, self._offer_frame = None, None
        self._ack_recorded = False
        self._success_observation_entered = False
        self._worker_success_observed = False
        self._worker_success_released = False
        self._verified_outputs = MappingProxyType({})
        self._diagnostic_snapshot = None
        self._diagnostic_snapshot_uncertain = False
        self.errors, self.termination_confirmed = [], False

    def _call(self, name, *args):
        return self._bound_exports.fn(name)(*args)

    def _ok(self, name, *args):
        result = self._call(name, *args)
        if not result:
            raise creator.ApiFailure(name, int(self.exports.last_error()))
        return result

    def _custody(self, *, allow_held_channel=False):
        if (self.exports is not self._bound_exports or
                self.watchdog is not self._watchdog_owner or
                self._watchdog_owner.exports is not self._bound_exports or
                self.runner != self._runner_handle or self.outer != self._outer_handle or
                self.local is not self._public_local or self.local != self._owned_outputs or
                self.nonce != self._occurrence_nonce or
                self.pipe_owner is not self._channel_owner or
                self._channel_owner.exports is not self._bound_exports or
                (self._channel_owner.receipt is not self._channel_receipt and
                 not (allow_held_channel and
                      getattr(self._channel_owner, "held", None) is True and
                      self._channel_owner.receipt is None)) or
                tuple(getattr(self._channel_receipt, name) for name in self._channel_fields)
                    != self._channel_handles or
                type(self._parent_write) is not int or
                self._parent_write != self._channel_handles[0] or
                type(self._parent_read) is not int or
                self._parent_read != self._channel_handles[2] or
                (self._validated_offer is not None and
                 (self.offer is not self._validated_offer or
                  encode_offer(self._validated_offer) != self._offer_frame))):
            raise Hold("changed exact owner/channel/OFFER custody")

    def _budget(self):
        self._custody()
        now = self._watchdog_owner._tick()  # shared history from runner creation onward
        self._custody()  # injected/native clock boundaries cannot mutate custody
        if (type(now) is not int or not 0 <= now < 2**64 or
                (self._last_tick is not None and now < self._last_tick) or
                not 0 < self.deadline - now <= 5000 or
                self.watchdog.deadline != self.deadline or
                self.watchdog.receipt.deadline != self.deadline or
                self.pipe_owner is not self._channel_owner or
                self.pipe_owner.exports is not self.exports or
                self.pipe_owner.receipt is not self._channel_receipt or
                tuple(getattr(self._channel_receipt, name) for name in self._channel_fields)
                    != self._channel_handles or
                self.pipe_owner.deadline != self.deadline or
                self.watchdog.receipt.local.get("runner_process") != self.runner or
                self.watchdog.receipt.local.get("outer_job") != self.outer):
            raise Hold("expired/changed original deadline or exact custody")
        self._last_tick = now
        if self.watchdog.receipt.phase != "running-unacknowledged":
            raise Hold("exact running unacknowledged runner required")

    def _transfer(self, kind, payload):
        self._budget()
        endpoint = self._parent_write if kind == "write" else self._parent_read
        result = self._channel_owner.transfer(endpoint, kind, payload)
        self._budget()
        if kind == "write" and (type(result) is not int or result != len(payload)):
            raise Hold("exact frame write required")
        return result

    def _fail(self, error):
        self.phase = "hold"
        self.errors.append(str(error))
        if not self.termination_confirmed:
            try:
                self._ok("TerminateJobObject", self._outer_handle, 0xE332)
                self.termination_confirmed = True
            except BaseException as cleanup_error:
                self.errors.append(str(cleanup_error))
        # CP328 retains OVERLAPPED storage until terminal completion evidence.
        self._channel_owner._hold()

    def bootstrap(self):
        try:
            if self.phase != "new":
                raise Hold("BOOTSTRAP is one-use")
            self.phase = "bootstrap-entered"
            self.watchdog.claim_handoff(self.nonce)  # irreversible before any channel write
            self._transfer("write", encode_bootstrap(self.deadline, self.nonce))
            self.phase = "offered-required"
        except BaseException as error:
            self._fail(error)
            raise

    def _duplicate(self, name, remote):
        self._budget()
        target = abi.HANDLE()
        current = self._call("GetCurrentProcess")
        try:
            ok = self._call("DuplicateHandle", self._runner_handle, remote, current,
                            C.byref(target), 0, False, 2)  # SAME_ACCESS; never CLOSE_SOURCE
        finally:
            value = target.value
            if value:
                if value >= INVALID - 15 or value in self._reserved or value in self._owned_outputs.values():
                    self.uncertain.append((name, value))
                else:
                    self._owned_outputs = MappingProxyType(
                        {**self._owned_outputs, name: value})  # retain before publishing diagnostics
                    self.local[name] = value
        # Capture ownership before even the injected last-error hook can raise.
        # Adoption above performs no export call, preserving ctypes' error copy.
        error = None if ok else int(self.exports.last_error())
        if self.uncertain or not ok or name not in self._owned_outputs:
            raise Hold("uncertain/failed DuplicateHandle output" + (f" ({error})" if not ok else ""))
        self._budget()

    def _flags(self, handle):
        flags = abi.DWORD()
        self._ok("GetHandleInformation", handle, C.byref(flags))
        return flags.value

    _query = creator.NativeWorkerCreator._query
    _active = creator.NativeWorkerCreator._active
    _job_verified = creator.NativeWorkerCreator._job_verified

    def _outer_census(self, expected_count):
        """One failure-only snapshot; a race or query fault is inconclusive."""
        try:
            result, returned = _PID_LIST_8(), abi.DWORD()
            ok = self._call("QueryInformationJobObject", self.outer, 3,
                            C.byref(result), C.sizeof(result), C.byref(returned))
            if (not ok or (returned.value != 0 and
                           not 8 <= returned.value <= C.sizeof(result)) or
                    result.assigned != expected_count or
                    result.listed != result.assigned or result.listed > 8):
                return "unavailable", None
            if returned.value and returned.value < 8 + result.listed * C.sizeof(abi.ULONG_PTR):
                return "unavailable", None
            values = tuple(int(result.ids[i]) for i in range(result.listed))
            if any(not 0 < value < 2**32 for value in values) or len(set(values)) != len(values):
                return "invalid", None
            return ",".join(str(value) for value in values), values
        except BaseException:
            return "unavailable", None

    def _scan_diagnostic_snapshot(self, handle, ids, extra_pid):
        """Read names only for census PIDs; never open a PID-derived handle."""
        target, found = set(ids), {}
        entry = abi.PROCESSENTRY32W()
        entry.dwSize = C.sizeof(entry)
        if not self._call("Process32FirstW", handle, C.byref(entry)):
            return "unavailable"
        for _ in range(1024):
            pid = int(entry.th32ProcessID)
            if pid in target:
                if pid in found:
                    return "invalid"
                found[pid] = (str(entry.szExeFile), int(entry.th32ParentProcessID))
            if len(found) == len(target):
                name, parent = found[extra_pid]
                if not name or not 0 <= parent < 2**32:
                    return "invalid"
                name = "".join(ch if 33 <= ord(ch) <= 126 else "?"
                               for ch in name[:24])
                return f"{extra_pid}:{name}:{parent}"
            if not self._call("Process32NextW", handle, C.byref(entry)):
                return "unavailable"
        return "unavailable"

    def _extra_process_snapshot(self, ids, runner_pid, worker_pid):
        """Failure-only Toolhelp enrichment with retryable snapshot custody."""
        if (ids is None or len(ids) != 3 or runner_pid not in ids or
                worker_pid not in ids or runner_pid == worker_pid or
                self._diagnostic_snapshot is not None):
            return "unavailable"
        extra = set(ids) - {runner_pid, worker_pid}
        if len(extra) != 1:
            return "unavailable"
        try:
            handle = self._call("CreateToolhelp32Snapshot", 2, 0)
        except BaseException:
            return "unavailable"
        if type(handle) is not int or handle in (0, INVALID):
            return "unavailable"
        if handle in self._reserved or handle in self._owned_outputs.values():
            self.uncertain.append(("diagnostic_snapshot", handle))
            return "unavailable"
        self._diagnostic_snapshot = handle
        try:
            result = self._scan_diagnostic_snapshot(handle, ids, extra.pop())
        except BaseException:
            result = "unavailable"
        try:
            if self._call("CloseHandle", handle):
                self._diagnostic_snapshot = None
        except BaseException:
            self._diagnostic_snapshot_uncertain = True
        if self._diagnostic_snapshot is not None:
            return "close-uncertain"
        return result

    def _verify(self, offer):
        self._budget()
        process, job = self._owned_outputs["worker_process"], self._owned_outputs["worker_job"]
        if self._flags(process) & 1 or self._flags(job) & 1:
            raise Hold("duplicated handles must be noninheritable")
        if self._call("GetProcessId", process) != offer.pid:
            raise Hold("exact WORKER PID mismatch")
        times = [abi.FILETIME() for _ in range(4)]
        self._ok("GetProcessTimes", process, *(C.byref(t) for t in times))
        stamp = times[0].dwLowDateTime | times[0].dwHighDateTime << 32
        if stamp != offer.creation_time:
            raise Hold("exact WORKER creation FILETIME mismatch")
        path, size = C.create_unicode_buffer(32768), abi.DWORD(32768)
        self._ok("QueryFullProcessImageNameW", process, 0, path, C.byref(size))
        if (not 0 < size.value < len(path) or size.value != len(path.value) or
                path.value.casefold() != image.WORKER_IMAGE.casefold()):
            raise Hold("fixed CP316 image path required")
        for container in (self.outer, job):
            member = abi.BOOL()
            self._ok("IsProcessInJob", process, container, C.byref(member))
            if member.value != 1:
                raise Hold("exact WORKER membership required in both jobs")
        self._job_verified(job)
        self._job_verified(self.outer)
        worker_active = self._active(job)
        if worker_active != 1:
            raise Hold("sole WORKER and runner-plus-WORKER accounting required "
                       f"(worker={worker_active}; outer=not-queried)")
        outer_active = self._active(self.outer)
        if outer_active != 2:
            members, ids = self._outer_census(outer_active)
            runner_pid = self.watchdog.receipt.pid
            extra = self._extra_process_snapshot(ids, runner_pid, offer.pid)
            raise Hold(f"job accounting w={worker_active} o={outer_active} x={extra}; "
                       f"ids={members}; r={runner_pid}; wp={offer.pid}")
        if self._call("WaitForSingleObject", process, 0) != 258:
            raise Hold("exact WORKER WAIT_TIMEOUT required")
        if self._call("WaitForSingleObject", self.runner, 0) != 258:
            raise Hold("exact runner WAIT_TIMEOUT required")
        self._budget()

    def accept_offer_and_ack(self):
        try:
            if self.phase != "offered-required":
                raise Hold("OFFER requires completed BOOTSTRAP; no replay")
            self.phase = "offer-entered"
            offer = decode_offer(self._transfer("read", FRAME.size))
            if (offer.nonce, offer.deadline) != (self._occurrence_nonce, self.deadline):
                raise Hold("OFFER nonce/deadline mismatch")
            offer_frame, ack_frame = encode_offer(offer), encode_ack(offer)
            offer_digest = hashlib.sha256(offer_frame).digest()
            self.offer = self._validated_offer = offer
            self._offer_frame = offer_frame
            self._duplicate("worker_process", offer.process)
            self._duplicate("worker_job", offer.job)
            self._verify(offer)
            if any(not op.terminal and op.kind == "read" for op in self._channel_owner.operations):
                raise Hold("no pending read allowed before trailing-byte snapshot")
            available = abi.DWORD()
            self._ok("PeekNamedPipe", self._parent_read,
                     None, 0, None, C.byref(available), None)
            if available.value:
                raise Hold("queued duplicate/trailing OFFER bytes")
            self._budget()
            self.phase = "ack-entered-outcome-uncertain"
            self._transfer("write", ack_frame)
            self._budget()
            self._watchdog_owner.record_handoff_ack(self._occurrence_nonce, offer_digest)
            self._budget()  # registry's own clock can mutate custody or exhaust budget
            self._ack_recorded = True
            self.phase = "acknowledged"
            return offer
        except BaseException as error:
            self._fail(error)
            raise

    def diagnostic_worker_wait(self):
        """Failure-only zero-time snapshot of the retained exact WORKER handle.

        This reports process state after ACK without spending the expired
        success deadline, changing ownership, or affecting the ACK outcome.
        """
        try:
            if (self.phase != "acknowledged" or not self._ack_recorded or
                    self.uncertain or
                    set(self._owned_outputs) != {"worker_process", "worker_job"}):
                return "unavailable"
            self._custody(allow_held_channel=True)
            process = self._owned_outputs["worker_process"]
            status = self._call("WaitForSingleObject", process, 0)
            if type(status) is not int:
                return "unavailable"
            if status == 258:
                return "active"
            if status == 0:
                code = abi.DWORD()
                try:
                    self._ok("GetExitCodeProcess", process, C.byref(code))
                except BaseException:
                    return "signaled"
                return f"exit_{int(code.value) & 0xFFFFFFFF}"
            return "unavailable"
        except BaseException:
            return "unavailable"

    def observe_worker_success(self):
        """Independently observe the exact duplicated WORKER's clean exit.

        This is a prerequisite for parent acceptance, not acceptance itself.
        Duplicated handles remain owned until release_worker_success() or the
        existing abort close() completes. A failed observation aborts the outer
        containment and cannot be retried as a new trial.
        """
        try:
            if (self.phase != "acknowledged" or not self._ack_recorded or
                    self._success_observation_entered or self.uncertain):
                raise Hold("one exact acknowledged handoff required")
            if set(self._owned_outputs) != {"worker_process", "worker_job"}:
                raise Hold("both exact duplicated WORKER handles required")
            self._success_observation_entered = True
            self.phase = "worker-success-observing"
            self._budget()
            process = self._owned_outputs["worker_process"]
            remaining = self.deadline - self._last_tick
            status = self._call("WaitForSingleObject", process, remaining)
            self._budget()
            if type(status) is not int or status != 0:
                raise Hold("exact bounded WORKER WAIT_OBJECT_0 required")
            code = abi.DWORD()
            self._ok("GetExitCodeProcess", process, C.byref(code))
            self._budget()
            if code.value != 0:
                raise Hold("exact WORKER exit code zero required")
            if self._active(self._owned_outputs["worker_job"]) != 0:
                raise Hold("duplicated WORKER job must be empty")
            self._budget()
            if any(not op.terminal for op in self._channel_owner.operations):
                raise Hold("parent channel operations need terminal evidence")
            self._custody()
            self._verified_outputs = MappingProxyType(dict(self._owned_outputs))
            self._worker_success_observed = True
            self.phase = "worker-success-observed"
            return True
        except BaseException as error:
            self._fail(error)
            raise

    def release_worker_success(self):
        """Retryably release only verified duplicates; leave parent owners live.

        The caller still owns the outer watchdog, pipe, and security descriptor
        and must separately verify runner exit and their terminal cleanup.
        """
        if self._worker_success_released:
            if (self.phase == "worker-success-released" and not self._owned_outputs
                    and not self._public_local):
                return True
            raise Hold("released WORKER ownership receipt changed")
        if (self.phase != "worker-success-observed" or not self._worker_success_observed
                or self.uncertain or not self._verified_outputs):
            raise Hold("verified exact WORKER exit required before release")
        try:
            self._custody()
            if any(self._verified_outputs.get(name) != handle
                   for name, handle in self._owned_outputs.items()):
                raise Hold("verified duplicate ownership changed")
            if any(not op.terminal for op in self._channel_owner.operations):
                return False
            for name, handle in tuple(self._owned_outputs.items()):
                self._custody()
                self._ok("CloseHandle", handle)
                self._owned_outputs = MappingProxyType(
                    {key: value for key, value in self._owned_outputs.items() if key != name})
                if self._public_local.get(name) == handle:
                    del self._public_local[name]
                self._custody()  # a close callback must not alter the next target
            self._worker_success_released = True
            self.phase = "worker-success-released"
            return True
        except BaseException as error:
            self.errors.append(str(error))
            return False

    def close(self):
        """Abort containment and retry cleanup; never discard uncertain outputs."""
        if self.phase == "closed":
            return True
        self._fail(Hold("explicit handoff cleanup"))
        if not self.termination_confirmed or self.uncertain:
            return False
        if self._diagnostic_snapshot_uncertain:
            return False
        try:
            if self._diagnostic_snapshot is not None:
                self._ok("CloseHandle", self._diagnostic_snapshot)
                self._diagnostic_snapshot = None
            if "worker_process" in self._owned_outputs:
                if self._call("WaitForSingleObject", self._owned_outputs["worker_process"], 0) != 0:
                    return False
            if "worker_job" in self._owned_outputs and self._active(self._owned_outputs["worker_job"]) != 0:
                return False
            if not self._channel_owner.cleanup():
                return False
            if self._watchdog_owner.close().phase != "closed":
                return False
            for name, handle in tuple(self._owned_outputs.items()):
                self._ok("CloseHandle", handle)
                self._owned_outputs = MappingProxyType(
                    {key: value for key, value in self._owned_outputs.items() if key != name})
                if self._public_local.get(name) == handle:
                    del self._public_local[name]
            self.phase = "closed"
            return True
        except BaseException as error:
            self.errors.append(str(error))
            return False


def create_live_handoff(*_args, **_kwargs):
    raise Hold("CP332 public live factory HARD HOLD pending independent integration review")
