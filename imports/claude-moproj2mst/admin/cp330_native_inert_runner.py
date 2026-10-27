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
    def __init__(self, exports, *, timeout_ms=5000):
        if not isinstance(exports, binding.BoundExports):
            raise TypeError("CP325 BoundExports required")
        if type(timeout_ms) is not int or not 0 < timeout_ms <= 5000:
            raise ValueError("timeout must be an integer in 1..5000ms")
        self.exports, self.timeout_ms = exports, timeout_ms
        self.receipt, self.worker = Receipt(), None
        self.security = self.pipe_owner = self.image_pin = None
        self._used, self._last_tick = False, None

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
        remaining = self.receipt.deadline - self._tick()
        if not 0 < remaining <= self.timeout_ms:
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

    def run(self):
        if self._used:
            raise Hold("runner is one-use")
        self._used = True
        r = self.receipt
        try:
            start = self._tick()  # sole allocation, before any owner preparation
            r.deadline = start + self.timeout_ms
            if r.deadline >= 2**64:
                raise Hold("deadline overflow")
            self.security = security.NativePrivatePipeSecurity(self.exports)
            self.pipe_owner = pipes.NativePipeOwner(self.exports, self.security, deadline=r.deadline)
            self.image_pin = image.NativeImagePin(self.exports)
            self.worker = creator.NativeWorkerCreator(self.exports, security=self.security,
                pipe_owner=self.pipe_owner, image_pin=self.image_pin, deadline=r.deadline)
            pipe_name = r"\\.\pipe\cp330-" + secrets.token_hex(32)
            nonce = secrets.token_bytes(32)
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
            r.resume_outcome = "unknown: ResumeThread entered"
            previous = self._call("ResumeThread", prepared.local["worker_thread"])
            if type(previous) is not int or previous != 1:
                raise Hold("ResumeThread previous suspend count must be exactly one")
            r.resume_outcome = "resumed-once-previous-count-one"
            self._budget()
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
            self._peek_empty()
            r.ack_outcome = "unknown: write entered"
            self._write(wire.encode_ack(wire.Ack(0, nonce)))
            r.ack_outcome = "48-bytes-written-exit-unconfirmed"
            self._budget()
            completed = self.worker.complete_success()
            self._budget()
            if completed.phase != "closed":
                raise Hold("successful exit with complete terminal owner cleanup required")
            r.ack_outcome = "exact-worker-exit-zero-observed"
            r.phase = "closed-success"
            return r
        except BaseException as error:
            r.errors.append(f"{type(error).__name__}: {error}")
            r.phase = "hold"
            self.close()
            raise RunFailure(error, self) from error

    def close(self):
        if self.worker is not None:
            self.worker.close()
        return self.receipt


def create_live_runner(*_args, **_kwargs):
    # Independent review must explicitly replace this hold before any live run.
    # The reviewed eventual entry will obtain exports with binding.load_native_exports().
    raise Hold("CP330 live runner HARD HOLD pending independent integration review")


if __name__ == "__main__":
    create_live_runner()
