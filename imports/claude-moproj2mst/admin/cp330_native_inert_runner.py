"""CP330 explicit native-capable CP316 inert WORKER orchestration.

Import and construction make no calls. The public live factory remains HARD
HOLD pending independent review. Explicit run() accepts CP325 BoundExports;
there is no fake-marker authorization. Never run it as an integration test.

PeekNamedPipe is only a queued-byte snapshot, not proof about future bytes.
CP327 pins a file path/identity, not mapped process bytes. CreateProcessW is
synchronous: deadline checks cannot turn it into a hard wall-clock bound.
"""
from __future__ import annotations

import ctypes as C
from dataclasses import dataclass, field
import secrets

import cp313_worker_binary_protocol as wire
import cp321_native_exports as abi
import cp325_real_bindings as binding
import cp326_native_pipe_security as security
import cp327_native_image_pin as image
import cp328_native_pipes as pipes
import cp329_native_worker_creator as creator
import cp420_runner_progress as progress

NATIVE_ENTRY = "HARD HOLD"
Hold = abi.Hold


@dataclass
class Receipt:
    phase: str = "new"
    deadline: int | None = None
    resume_outcome: str = "not-attempted"
    init_outcome: str = "not-attempted"
    result_outcome: str = "not-attempted"
    peek_outcome: str = "not-attempted"
    ack_outcome: str = "not-attempted"
    errors: list = field(default_factory=list)


class RunFailure(Hold):
    def __init__(self, error, owner):
        self.owner, self.receipt = owner, owner.receipt
        super().__init__(str(error))


class NativeInertRunner:
    """One-use, explicitly invoked owner; retain it after any uncertain failure.

    All subordinate owners and bound DLLs remain strongly held. close() retries
    CP329 cleanup, which terminates WORKER before testing pipe readiness.
    """
    def __init__(self, exports, *, timeout_ms=5000, deadline=None, nonce=None, before_resume=None):
        if not isinstance(exports, binding.BoundExports):
            raise TypeError("CP325 BoundExports required")
        if type(timeout_ms) is not int or not 0 < timeout_ms <= 5000:
            raise ValueError("timeout must be an integer in 1..5000ms")
        external = deadline is not None or nonce is not None or before_resume is not None
        if external and (type(deadline) is not int or not 0 < deadline < 2**64 or
                         type(nonce) is not bytes or len(nonce) != 32 or
                         not callable(before_resume)):
            raise ValueError("external deadline, 32-byte nonce and pre-resume callback required")
        self._external = external
        self._supplied_deadline, self._supplied_nonce = deadline, nonce
        self._before_resume = before_resume
        self.exports, self.timeout_ms = exports, timeout_ms
        self.receipt, self.worker = Receipt(), None
        self.security = self.pipe_owner = self.image_pin = None
        self._used, self._last_tick = False, None
        self._deadline = None
        self._prepared_custody = None
        self._custody_uncertain = False

    def _capture_custody(self, worker, receipt):
        # Retain both the exact mutable owners and immutable evidence before
        # calling external code. A changed owner cannot safely authorize cleanup.
        self._prepared_custody = (worker, receipt, receipt.local,
            tuple(sorted(receipt.local.items())), receipt.remote_child_job,
            receipt.pid, receipt.tid, worker.exports, worker.security,
            worker.pipe_owner, worker.image_pin, worker.deadline,
            (receipt.create_outcome, receipt.duplicate_outcome,
             receipt.worker_terminated, receipt.worker_waited, receipt.jobs_empty),
            self._subordinate_state(worker),
            (self.receipt, self.security, self.pipe_owner, self.image_pin,
             self._external, self._supplied_deadline, self._supplied_nonce,
             self._before_resume, self.timeout_ms, self._used, self.pipe_owner.receipt))
        return self._prepared_custody

    @staticmethod
    def _subordinate_state(worker):
        pipe, pin, remote = worker.pipe_owner, worker.image_pin, worker.receipt.remote_child_job
        endpoints = pipe.receipt
        endpoint_values = tuple(getattr(endpoints, name) for name in
            ("parent_write", "client_read", "parent_read", "client_write"))
        if (type(pipe.handles) is not set or any(type(h) is not int for h in pipe.handles) or
                any(type(h) is not int for h in endpoint_values) or
                type(remote.value) is not int or type(remote.owner_process) is not int or
                type(pin.handle) is not int or
                (pin.reopened_handle is not None and type(pin.reopened_handle) is not int)):
            raise Hold("exact integer subordinate handle ownership required")
        snapshot = pin.snapshot
        identity = snapshot.identity
        image_values = (snapshot.sha256, identity.volume, identity.file_id,
            identity.size, identity.legacy_volume, identity.legacy_index, identity.final_path)
        if (type(snapshot) is not image.Snapshot or type(identity) is not image.Identity or
                type(snapshot.sha256) is not str or type(identity.file_id) is not bytes or
                type(identity.final_path) is not str or
                any(type(value) is not int for value in
                    (identity.volume, identity.size, identity.legacy_volume, identity.legacy_index)) or
                type(pin._used) is not bool or type(pin.post_create_verified) is not bool):
            raise Hold("exact immutable image identity evidence required")
        return (pipe.handles, frozenset(pipe.handles), pipe.operations,
            tuple(pipe.operations), endpoint_values, (remote.value, remote.owner_process),
            pin.handle, pin.reopened_handle, snapshot, identity, image_values,
            pin._used, pin.post_create_verified)

    @staticmethod
    def _same_subordinates(worker, frozen):
        current = NativeInertRunner._subordinate_state(worker)
        # Compare identity separately: equal replacements can still discard the
        # exact cleanup owner. Immutable values preserve mutated-frozen evidence.
        return (current[0] is frozen[0] and current[2] is frozen[2] and
            current[8] is frozen[8] and current[9] is frozen[9] and
            all(current[index] == frozen[index] for index in
                (1, 3, 4, 5, 6, 7, 10, 11, 12)))

    def _custody_changed(self, custody):
        worker, receipt, local, tokens, remote, pid, tid, exports, sec, pipe, pin, deadline, state, subordinate, runner = custody
        own_receipt, own_sec, own_pipe, own_pin, external, supplied_deadline, supplied_nonce, callback, timeout, used, endpoints = runner
        return (self._prepared_custody is not custody or self.worker is not worker or
                self.receipt is not own_receipt or self.security is not own_sec or
                self.pipe_owner is not own_pipe or self.image_pin is not own_pin or
                own_sec is not sec or own_pipe is not pipe or own_pin is not pin or
                sec.exports is not exports or pipe.exports is not exports or pin.exports is not exports or
                pipe.security is not sec or type(pipe.deadline) is not int or pipe.deadline != deadline or
                pipe.receipt is not endpoints or
                self._external is not external or self._used is not used or
                type(self._supplied_deadline) is not int or self._supplied_deadline != supplied_deadline or
                type(self._supplied_nonce) is not bytes or self._supplied_nonce != supplied_nonce or
                self._before_resume is not callback or type(self.timeout_ms) is not int or self.timeout_ms != timeout or
                worker.receipt is not receipt or
                receipt.phase != "prepared-suspended" or receipt.local is not local or
                tuple(sorted(local.items())) != tokens or
                any(type(value) is not int for value in local.values()) or
                receipt.remote_child_job is not remote or
                type(receipt.pid) is not int or receipt.pid != pid or
                type(receipt.tid) is not int or receipt.tid != tid or
                worker.exports is not exports or self.exports is not exports or
                worker.security is not sec or worker.pipe_owner is not pipe or
                worker.image_pin is not pin or type(worker.deadline) is not int or worker.deadline != deadline or
                not self._same_subordinates(worker, subordinate) or
                type(self._deadline) is not int or self._deadline != deadline or
                type(self.receipt.deadline) is not int or self.receipt.deadline != deadline or
                (receipt.create_outcome, receipt.duplicate_outcome,
                 receipt.worker_terminated, receipt.worker_waited, receipt.jobs_empty) != state)

    def _verify_custody(self, custody):
        try:
            changed = self._custody_changed(custody)
        except BaseException as error:
            self._prepared_custody = custody
            self._custody_uncertain = True
            raise Hold("prepared WORKER custody cannot be verified") from error
        if changed:
            self._prepared_custody = custody
            self._custody_uncertain = True
            raise Hold("prepared WORKER custody changed after pre-resume callback")

    def _call(self, name, *args):
        return self.exports.fn(name)(*args)

    def _tick(self):
        now = self._call("GetTickCount64")
        if (type(now) is not int or not 0 <= now < 2**64 or
                (self._last_tick is not None and now < self._last_tick)):
            raise Hold("invalid or regressing GetTickCount64")
        self._last_tick = now
        return now

    def _budget(self):
        deadline, timeout = self._deadline, self.timeout_ms
        def unchanged():
            return (type(deadline) is int and type(timeout) is int and
                type(self._deadline) is int and self._deadline == deadline and
                type(self.timeout_ms) is int and self.timeout_ms == timeout and
                type(self.receipt.deadline) is int and self.receipt.deadline == deadline and
                (self.worker is None or (type(self.worker.deadline) is int and self.worker.deadline == deadline)))
        if not unchanged():
            raise Hold("original exact integer deadline or timeout changed")
        now = self._tick()
        if not unchanged():
            raise Hold("original exact integer deadline or timeout changed during clock query")
        remaining = deadline - now
        if not 0 < remaining <= timeout:
            raise Hold("original absolute deadline expired or changed")
        if self.worker is not None and self.worker.deadline != self.receipt.deadline:
            raise Hold("creator deadline changed")
        return remaining

    def _write(self, frame):
        self._budget()
        count = self.pipe_owner.transfer(self.pipe_owner.receipt.parent_write, "write", frame)
        self._budget()
        if type(count) is not int or count != len(frame):
            raise Hold("exact frame write required")

    def _peek_empty(self):
        self._budget()
        if any(not op.terminal and op.kind == "read" for op in self.pipe_owner.operations):
            raise Hold("no pending read allowed before PeekNamedPipe")
        available = abi.DWORD()
        ok = self._call("PeekNamedPipe", self.pipe_owner.receipt.parent_read,
                        None, 0, None, C.byref(available), None)
        error = None if ok else int(self.exports.last_error())
        self._budget()
        if not ok:
            raise creator.ApiFailure("PeekNamedPipe", error)
        if available.value != 0:
            raise Hold("queued duplicate/trailing RESULT bytes")
        self.receipt.peek_outcome = "zero-queued-byte-snapshot"

    def _progress(self, nonce, stage):
        """Best-effort CP420 evidence; progress storage never gates the run."""
        if not self._external:
            return
        try:
            progress.write_progress(nonce, self._supplied_deadline, stage)
        except BaseException:
            pass

    def run(self):
        if self._used or self._custody_uncertain:
            raise Hold("runner is one-use")
        self._used = True
        r = self.receipt
        try:
            if self._external:
                self._deadline = r.deadline = self._supplied_deadline
                self._budget()
            else:
                start = self._tick()  # sole allocation, before any owner preparation
                self._deadline = r.deadline = start + self.timeout_ms
                if r.deadline >= 2**64:
                    raise Hold("deadline overflow")
            self.security = security.NativePrivatePipeSecurity(self.exports)
            self.pipe_owner = pipes.NativePipeOwner(self.exports, self.security, deadline=r.deadline)
            self.image_pin = image.NativeImagePin(self.exports)
            self.worker = creator.NativeWorkerCreator(self.exports, security=self.security,
                pipe_owner=self.pipe_owner, image_pin=self.image_pin, deadline=r.deadline)
            pipe_name = r"\\.\pipe\cp330-" + secrets.token_hex(32)
            nonce = self._supplied_nonce if self._external else secrets.token_bytes(32)
            wire.Init(1, nonce)  # validate entropy output before preparation
            self._budget()
            prepared = self.worker.prepare(pipe_name)
            self._budget()
            if prepared is not self.worker.receipt or prepared.phase != "prepared-suspended":
                raise Hold("CP329 prepared-suspended receipt required")
            remote = prepared.remote_child_job
            if (not isinstance(remote, creator.RemoteHandle) or
                    remote.owner_process != prepared.local.get("worker_process")):
                raise Hold("exact WORKER-owned remote CHILD token required")
            init = wire.encode_init(wire.Init(remote.value, nonce))
            self._budget()
            if self._external:
                custody = self._capture_custody(self.worker, prepared)
                try:
                    self._before_resume(self.worker, nonce)
                finally:
                    self._verify_custody(custody)
                self._budget()
                self._verify_custody(custody)  # the clock call is also an external boundary
            r.resume_outcome = "unknown: ResumeThread entered"
            previous = self._call("ResumeThread", prepared.local["worker_thread"])
            if type(previous) is not int or previous != 1:
                raise Hold("ResumeThread previous suspend count must be exactly one")
            r.resume_outcome = "resumed-once-previous-count-one"
            self._budget()
            self._progress(nonce, "worker_resumed")
            r.init_outcome = "unknown: write entered"
            self._write(init)
            r.init_outcome = "48-bytes-written"
            r.result_outcome = "unknown: read entered"
            self._budget()
            raw = self.pipe_owner.transfer(self.pipe_owner.receipt.parent_read, "read", 80)
            self._budget()
            result = wire.decode_result(raw)
            if result.nonce != nonce or any(getattr(result, name) for name in (
                    "create_called", "create_ok", "create_error", "pid", "tid",
                    "child_query_ok", "child_query_error", "child_member", "child_process_handle")):
                raise Hold("RESULT must echo nonce with all child fields zero")
            r.result_outcome = "80-byte-zero-child-result"
            self._progress(nonce, "result")
            self._peek_empty()
            r.ack_outcome = "unknown: write entered"
            self._write(wire.encode_ack(wire.Ack(0, nonce)))
            r.ack_outcome = "48-bytes-written-exit-unconfirmed"
            self._progress(nonce, "worker_ack")
            self._budget()
            completed = self.worker.complete_success()
            self._budget()
            if completed.phase != "closed":
                raise Hold("successful exit with complete terminal owner cleanup required")
            r.ack_outcome = "exact-worker-exit-zero-observed"
            r.phase = "closed-success"
            self._progress(nonce, "worker_closed")
            return r
        except BaseException as error:
            self.receipt = r  # retain the original occurrence receipt on callback replacement
            r.errors.append(f"{type(error).__name__}: {error}")
            r.phase = "hold"
            self.close()
            raise RunFailure(error, self) from error

    def close(self):
        if self._custody_uncertain:
            # The retained snapshot is evidence, not permission to close handles
            # whose ownership an external callback may have changed or released.
            return self.receipt
        if self.worker is not None:
            self.worker.close()
        return self.receipt


def create_live_runner(*_args, **_kwargs):
    # Independent review must explicitly replace this hold before any live run.
    # The reviewed eventual entry will obtain exports with binding.load_native_exports().
    raise Hold("CP330 live runner HARD HOLD pending independent integration review")


if __name__ == "__main__":
    create_live_runner()
