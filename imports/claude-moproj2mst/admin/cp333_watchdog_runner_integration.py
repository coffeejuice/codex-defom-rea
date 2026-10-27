"""CP333 integration contract. Public launch stays HARD HOLD.

Explicit owners use injected CP325 exports. This module does not load DLLs,
open processes by PID, select an image/command, or infer trial success from ACK.
CP330/331 entry seams must be integrated and independently reviewed before a
contained native trial. Retain the integration after any uncertain failure.
"""
from __future__ import annotations

import ctypes as C
from dataclasses import dataclass, field
import secrets

import cp321_native_exports as abi
import cp325_real_bindings as binding
import cp326_native_pipe_security as security
import cp328_native_pipes as pipes
import cp331_outer_watchdog as outer
import cp332_exact_worker_handoff as handoff
import cp339_terminal_protocol as terminal

NATIVE_ENTRY = "HARD HOLD"
Hold = abi.Hold


@dataclass
class Receipt:
    phase: str = "new"
    deadline: int | None = None
    terminal_outcome: str = "not-attempted"
    worker_outcome: str = "not-attempted"
    runner_outcome: str = "not-attempted"
    cleanup_outcome: str = "not-attempted"
    errors: list = field(default_factory=list)


class IntegrationFailure(Hold):
    def __init__(self, error, owner):
        self.owner, self.receipt = owner, owner.receipt
        super().__init__(str(error))


class WatchdogIntegration:
    """Prepare fixed private IPC only after CP331 allocates the occurrence.

    acknowledge() ends at exact suspended WORKER authorization. ACK is not an
    inert RESULT/exit receipt. close() aborts containment and retains owners
    through unresolved cancellation/waits; it never labels abort as success.
    """
    def __init__(self, exports, *, timeout_ms=5000):
        if not isinstance(exports, binding.BoundExports):
            raise TypeError("CP325 BoundExports required")
        self.exports = exports
        self.watchdog = outer.OuterWatchdog(exports, timeout_ms=timeout_ms)
        self.receipt = Receipt()
        self.security = self.pipe_owner = self.handoff = None
        self.nonce = None
        self._completion_owners = None
        self._completion_entered = False
        self._verified_success = False
        self._accepted_success = False

    def _completion_custody(self):
        frozen = self._completion_owners
        if frozen is None:
            raise Hold("exact acknowledged occurrence required")
        exports, watchdog, pipe_owner, handoff_owner, descriptor, receipt, nonce, deadline, endpoints = frozen
        if (self.exports is not exports or self.watchdog is not watchdog or
                self.pipe_owner is not pipe_owner or self.handoff is not handoff_owner or
                self.security is not descriptor or self.receipt is not receipt or
                type(self.nonce) is not bytes or
                self.nonce != nonce or type(self.receipt.deadline) is not int or
                self.receipt.deadline != deadline or
                watchdog.exports is not exports or pipe_owner.exports is not exports or
                handoff_owner.exports is not exports or
                type(watchdog.deadline) is not int or watchdog.deadline != deadline or
                type(pipe_owner.deadline) is not int or pipe_owner.deadline != deadline or
                type(handoff_owner.deadline) is not int or handoff_owner.deadline != deadline or
                type(handoff_owner._channel_handles) is not tuple or
                handoff_owner._channel_handles != endpoints or
                type(handoff_owner._parent_read) is not int or
                handoff_owner._parent_read != endpoints[2] or
                type(handoff_owner._parent_write) is not int or
                handoff_owner._parent_write != endpoints[0]):
            raise Hold("exact parent occurrence ownership changed")

    def _terminal_queue_empty(self):
        """Reject bytes currently queued after the one terminal frame.

        This is a snapshot, not proof that a writer cannot enqueue later.
        Exact runner exit and terminal pipe cleanup are still required.
        """
        self._completion_custody()
        if any(not op.terminal and op.kind == "read" for op in self.pipe_owner.operations):
            raise Hold("no pending read permitted before terminal queue snapshot")
        self.handoff._budget()
        available = abi.DWORD()
        ok = self.exports.fn("PeekNamedPipe")(
            self._completion_owners[8][2],
            None, 0, None, C.byref(available), None)
        error = None if ok else int(self.exports.last_error())
        self.handoff._budget()
        self._completion_custody()
        if not ok:
            raise Hold(f"terminal queue snapshot failed: {error}")
        if available.value != 0:
            raise Hold("queued duplicate/trailing terminal bytes")

    def _completion_budget(self):
        """Keep process release and parent cleanup inside the sole deadline."""
        self._completion_custody()
        self._completion_owners[1]._budget()
        self._completion_custody()

    def acknowledge(self):
        if self.receipt.phase != "new":
            raise Hold("integration is one-use")
        self.receipt.phase = "starting"
        try:
            self.receipt.deadline = self.watchdog.start_occurrence()
            self.nonce = secrets.token_bytes(32)
            handoff.encode_bootstrap(self.receipt.deadline, self.nonce)
            self.security = security.NativePrivatePipeSecurity(self.exports)
            self.pipe_owner = pipes.NativePipeOwner(self.exports, self.security,
                deadline=self.receipt.deadline)
            endpoints = self.pipe_owner.prepare(r"\\.\pipe\cp333-" + secrets.token_hex(32))
            self.watchdog.prepare_suspended(child_read=endpoints.client_read,
                child_write=endpoints.client_write)
            self.handoff = handoff.ExactWorkerHandoff(self.exports,
                watchdog=self.watchdog, pipe_owner=self.pipe_owner, nonce=self.nonce)
            self.watchdog.resume_once()
            self.handoff.bootstrap()
            offer = self.handoff.accept_offer_and_ack()
            self._completion_owners = (self.exports, self.watchdog, self.pipe_owner,
                self.handoff, self.security, self.receipt, self.nonce, self.receipt.deadline,
                tuple(self.handoff._channel_handles))
            self.receipt.phase = "worker-authorized-trial-unaccepted"
            return offer
        except BaseException as error:
            self.receipt.errors.append(f"{type(error).__name__}: {error}")
            self.receipt.phase = "hold"
            self.close()
            raise IntegrationFailure(error, self) from error

    def complete(self):
        """Accept only a terminal claim plus independent exact process evidence.

        ACK alone authorizes one WORKER resume. It never proves RESULT, exit,
        empty jobs or terminal cleanup. This method is one-use even after EOF,
        timeout or an uncertain API return; close() can retry cleanup only.
        """
        if self._completion_entered or self.receipt.phase != "worker-authorized-trial-unaccepted":
            raise Hold("one acknowledged unfinished trial required")
        self._completion_entered = True
        r = self.receipt
        try:
            self._completion_custody()
            if self.handoff.phase != "acknowledged":
                raise Hold("exact WORKER ACK required before terminal receipt")
            r.phase = "terminal-read-entered"
            r.terminal_outcome = "unknown: read entered"
            # CP332 owns the parent endpoint and shares CP331's original clock.
            # Its transfer checks exact channel/handle custody on both sides.
            raw = self.handoff._transfer("read", terminal.FRAME.size)
            self._completion_custody()
            terminal.decode_terminal(raw, deadline=r.deadline, nonce=self.nonce)
            r.terminal_outcome = "unknown: terminal queue snapshot entered"
            self._terminal_queue_empty()
            r.terminal_outcome = "80-byte-occurrence-validated-zero-queued-snapshot"

            r.worker_outcome = "unknown: exact WORKER observation entered"
            if self.handoff.observe_worker_success() is not True:
                raise Hold("exact WORKER success observation required")
            self._completion_custody()
            r.worker_outcome = "exact-worker-exit-zero-job-empty-observed"
            if self.handoff.release_worker_success() is not True:
                raise Hold("verified WORKER duplicate release unresolved")
            self._completion_custody()
            r.worker_outcome = "exact-worker-observed-and-duplicates-released"

            r.runner_outcome = "unknown: exact RUNNER observation entered"
            runner_receipt = self.watchdog.observe_runner_success()
            if (getattr(self._completion_owners[3], "_worker_success_released", False) is True and
                    getattr(self._completion_owners[1], "_runner_success_observed", False) is True):
                # CP331 has independently proved the exact runner/job, even
                # if a subsequent public custody check fails after its handle
                # releases. From here cleanup must never abort a closed job.
                self._verified_success = True
            self._completion_custody()
            if (runner_receipt is not self.watchdog.receipt or
                    self._verified_success is not True or
                    runner_receipt.phase != "runner-closed-success" or
                    runner_receipt.runner_waited is not True or
                    runner_receipt.job_empty is not True):
                raise Hold("exact RUNNER exit-zero and empty outer job required")
            r.runner_outcome = "exact-runner-exit-zero-job-empty-observed"
            self._verified_success = True
            self._completion_budget()

            r.cleanup_outcome = "unknown: parent channel cleanup entered"
            if self.pipe_owner.cleanup() is not True:
                raise Hold("terminal parent pipe cleanup unresolved")
            self._completion_budget()
            if self.security.cleanup() is not True:
                raise Hold("terminal private descriptor cleanup unresolved")
            self._completion_budget()
            r.cleanup_outcome = "terminal-parent-owners-released"
            r.phase = "closed-success"
            self._accepted_success = True
            return r
        except BaseException as error:
            # The original receipt is part of the frozen occurrence owner.
            # Preserve its diagnostics if an injected callback replaces the
            # public attribute while a native call is in progress.
            if self._completion_owners is not None:
                self.receipt = self._completion_owners[5]
                watchdog_owner = self._completion_owners[1]
                handoff_owner = self._completion_owners[3]
                if (getattr(handoff_owner, "_worker_success_released", False) is True and
                        getattr(watchdog_owner, "_runner_success_observed", False) is True):
                    # CP331's private proof may have completed even if its
                    # post-proof handle release or this parent's custody check
                    # raised. Both processes/jobs are then proved dead; retry
                    # release only, never TerminateJobObject on a closed handle.
                    self._verified_success = True
            r.errors.append(f"{type(error).__name__}: {error}")
            r.phase = "hold"
            self.close()
            raise IntegrationFailure(error, self) from error

    def close(self):
        """Retry containment first, then pipe cleanup; preserve failed owners."""
        if self._completion_owners is not None:
            self.receipt = self._completion_owners[5]
        if self._accepted_success:
            return True
        if self._completion_owners is None:
            watchdog, pipe_owner, handoff_owner, descriptor = (
                self.watchdog, self.pipe_owner, self.handoff, self.security)
        else:
            _, watchdog, pipe_owner, handoff_owner, descriptor, _, _, _, _ = self._completion_owners
        if self._verified_success:
            # Both exact processes and jobs were independently observed. An
            # earlier cleanup failure remains unaccepted; retry release only.
            try:
                runner_receipt = watchdog.observe_runner_success()
                if (runner_receipt is not watchdog.receipt or
                        getattr(watchdog, "_runner_success_released", False) is not True):
                    return False
                if pipe_owner.cleanup() is not True:
                    return False
                if descriptor.cleanup() is not True:
                    return False
                self.receipt.phase = "closed-unaccepted-after-cleanup-error"
                return True
            except BaseException as error:
                self.receipt.errors.append(f"cleanup: {type(error).__name__}: {error}")
                return False
        try:
            if handoff_owner is not None:
                ready = handoff_owner.close()
            else:
                ready = watchdog.close().phase == "closed"
                if ready and pipe_owner is not None:
                    ready = pipe_owner.cleanup()
            if ready and descriptor is not None:
                ready = descriptor.cleanup()
            if ready:
                self.receipt.phase = "closed-aborted"
            return ready
        except BaseException as error:
            self.receipt.errors.append(str(error))
            return False


class RunnerHandoff:
    """CP330 pre-ResumeThread callback, using an adopted CP328 client channel.

    The initial deadline is supplied by CP331's fixed entry contract. BOOTSTRAP
    must echo it; no local timeout allocation or renewal is permitted. Adoption
    and CP330 entry wiring are deliberately still held prerequisites.
    """
    def __init__(self, exports, *, pipe_owner, deadline):
        if not isinstance(exports, binding.BoundExports):
            raise TypeError("CP325 BoundExports required")
        handoff._uint(deadline, 64)
        if pipe_owner.exports is not exports or pipe_owner.deadline != deadline:
            raise Hold("same exports and original deadline required")
        if pipe_owner.receipt is None:
            raise Hold("connected inherited client channel required")
        self._exports, self._pipe_owner, self._deadline = exports, pipe_owner, deadline
        self._channel_receipt = pipe_owner.receipt
        # CP334 owns the two inherited client endpoints. Keep the older
        # injected CP328 four-endpoint seam for focused contract tests.
        if (hasattr(self._channel_receipt, "client_read") and
                hasattr(self._channel_receipt, "client_write") and
                not hasattr(self._channel_receipt, "parent_write")):
            self._endpoint_names = ("client_read", "client_write")
            self._read_index, self._write_index = 0, 1
        else:
            self._endpoint_names = ("parent_write", "client_read", "parent_read", "client_write")
            self._read_index, self._write_index = 1, 3
        self._endpoints = tuple(getattr(self._channel_receipt, name) for name in self._endpoint_names)
        if (any(type(value) is not int or not 0 < value < handoff.INVALID - 15
                for value in self._endpoints) or len(set(self._endpoints)) != len(self._endpoints)):
            raise Hold("distinct retained channel endpoints required")
        self.exports, self.pipe_owner, self.deadline = exports, pipe_owner, deadline
        self._nonce = None
        self.nonce = self.offer = None
        self.phase, self._last_tick = "new", None

    def _custody(self):
        endpoints = tuple(getattr(self._channel_receipt, name, None)
                          for name in self._endpoint_names)
        if (self.exports is not self._exports or self.pipe_owner is not self._pipe_owner
                or type(self.deadline) is not int or self.deadline != self._deadline
                or type(self.nonce) is not type(self._nonce) or self.nonce != self._nonce
                or self._pipe_owner.exports is not self._exports
                or type(self._pipe_owner.deadline) is not int
                or self._pipe_owner.deadline != self._deadline
                or self._pipe_owner.receipt is not self._channel_receipt
                or any(type(value) is not int for value in endpoints)
                or endpoints != self._endpoints):
            raise Hold("original occurrence or channel custody changed")

    def _budget(self):
        self._custody()
        now = self._exports.fn("GetTickCount64")()
        self._custody()
        if (type(now) is not int or not 0 <= now < 2**64 or
                (self._last_tick is not None and now < self._last_tick) or
                not 0 < self._deadline - now <= 5000):
            raise Hold("original runner deadline expired or changed")
        self._last_tick = now

    def _transfer(self, kind, payload):
        self._budget()
        endpoint = self._endpoints[self._write_index if kind == "write" else self._read_index]
        result = self._pipe_owner.transfer(endpoint, kind, payload)
        self._budget()
        if kind == "write" and (type(result) is not int or result != len(payload)):
            raise Hold("exact frame write required")
        return result

    def receive_bootstrap(self):
        if self.phase != "new":
            raise Hold("BOOTSTRAP is one-use")
        self.phase = "bootstrap-entered"
        try:
            deadline, nonce = handoff.decode_bootstrap(self._transfer("read", handoff.BOOT.size))
            if deadline != self._deadline:
                raise Hold("BOOTSTRAP changed original deadline")
            self._nonce = self.nonce = nonce
            self._budget()
            self.phase = "prepared-required"
            return deadline, nonce
        except BaseException:
            self.phase = "hold"
            raise

    def before_resume(self, worker, nonce):
        """Return only after exact full ACK; CP330 must then recheck budget."""
        if self.phase != "prepared-required":
            raise Hold("completed BOOTSTRAP and one suspended WORKER required")
        self.phase = "offer-entered"
        try:
            self._budget()
            if (worker.exports is not self._exports or worker.deadline != self._deadline
                    or nonce != self._nonce or worker.receipt.phase != "prepared-suspended"):
                raise Hold("same occurrence and prepared suspended WORKER required")
            receipt, local = worker.receipt, worker.receipt.local
            tokens = tuple(sorted(local.items()))
            pid, tid = receipt.pid, receipt.tid
            process = local["worker_process"]
            for name in ("worker_process", "worker_thread", "worker_job", "child_job"):
                handoff._uint(local[name], 64)
            handoff._uint(pid, 32)
            handoff._uint(tid, 32)
            remote = receipt.remote_child_job
            if (not isinstance(remote, handoff.creator.RemoteHandle)
                    or remote.owner_process != process):
                raise Hold("exact WORKER-owned remote CHILD token required")
            remote_fields = (remote.value, remote.owner_process)
            handoff._uint(remote.value, 64)

            def verify_worker(offer=None, encoded=None):
                if (worker.exports is not self._exports or type(worker.deadline) is not int
                        or worker.deadline != self._deadline
                        or worker.receipt is not receipt or receipt.phase != "prepared-suspended"
                        or receipt.local is not local or tuple(sorted(local.items())) != tokens
                        or any(type(value) is not int for value in local.values())
                        or type(receipt.pid) is not int or receipt.pid != pid
                        or type(receipt.tid) is not int or receipt.tid != tid
                        or receipt.remote_child_job is not remote
                        or type(remote.value) is not int or type(remote.owner_process) is not int
                        or (remote.value, remote.owner_process) != remote_fields
                        or (offer is not None and (self.offer is not offer
                            or handoff.encode_offer(offer) != encoded))):
                    raise Hold("exact suspended WORKER custody changed")

            self._budget()
            verify_worker()
            times = [abi.FILETIME() for _ in range(4)]
            if not self._exports.fn("GetProcessTimes")(process, *(C.byref(t) for t in times)):
                raise Hold("WORKER creation timestamp query failed")
            stamp = times[0].dwLowDateTime | times[0].dwHighDateTime << 32
            self._budget()
            verify_worker()
            offer = self.offer = handoff.Offer(process, local["worker_job"],
                pid, stamp, self._nonce, self._deadline)
            encoded = handoff.encode_offer(offer)
            self._transfer("write", encoded)
            handoff.validate_ack(self._transfer("read", handoff.FRAME.size), offer)
            verify_worker(offer, encoded)
            self._budget()
            verify_worker(offer, encoded)  # final clock is also an injected/native boundary
            self.phase = "authorized"
            return offer
        except BaseException:
            self.phase = "hold"
            raise


def launch_trial(*_args, **_kwargs):
    raise Hold("CP333 live launch HARD HOLD pending entry wiring and independent end-to-end review")


if __name__ == "__main__":
    launch_trial()
