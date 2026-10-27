"""CP316 typed preparation/overlapped ownership model; native entry HARD HOLD.

This module contains NO Win32 binding and proves no native I/O or inheritance.
An injected fake kernel implements submit/poll/wait/cancel/close_event. poll is
nonblocking, wait is bounded by milliseconds, and only an explicit terminal
completion permits freeing an operation's event, buffer and OVERLAPPED token.
CancelIoEx (including ERROR_NOT_FOUND) is never a completion receipt.
"""
from __future__ import annotations

from dataclasses import dataclass
import math
import time
from typing import Callable, Protocol
from uuid import uuid4

NATIVE_ENTRY = "HARD HOLD"
FILE_FLAG_OVERLAPPED = 0x40000000
PIPE_ACCESS_INBOUND = 1
PIPE_ACCESS_OUTBOUND = 2
FILE_FLAG_FIRST_PIPE_INSTANCE = 0x00080000
PIPE_REJECT_REMOTE_CLIENTS = 8
FRAME_SIZES = (48, 80)
MAX_PENDING_WAITS = 256  # bound a signalled-but-incomplete adapter spin as well


class Hold(RuntimeError):
    pass


@dataclass(frozen=True)
class PipeSpec:
    name: str
    server_access: int
    client_access: str
    server_inheritable: bool = False
    client_inheritable: bool = True
    pipe_mode: int = PIPE_REJECT_REMOTE_CLIENTS  # byte type/read mode, wait mode
    max_instances: int = 1


@dataclass(frozen=True)
class Preparation:
    deadline: float
    to_worker: PipeSpec
    from_worker: PipeSpec
    qualification: str = "specification only; native creation/security/connection unbound"


def prepare(*, budget_ms: int = 5000,
            clock: Callable[[], float] = time.monotonic) -> Preparation:
    """Allocate the SINGLE deadline before names, creation or connection work.

    A future binding must apply a current-user private DACL (random names alone
    are not access control), create both server ends with OVERLAPPED and first
    instance flags, connect before use, and provide exactly the two client ends
    through STARTUPINFOEX HANDLE_LIST. No process creation is implemented here.
    """
    if type(budget_ms) is not int or not 0 < budget_ms <= 5000:
        raise ValueError("budget_ms must be in [1, 5000]")
    start = clock()
    if not math.isfinite(start):
        raise ValueError("finite monotonic clock required")
    deadline = start + budget_ms / 1000
    base = "\\\\.\\pipe\\cp316-" + uuid4().hex
    flags = FILE_FLAG_OVERLAPPED | FILE_FLAG_FIRST_PIPE_INSTANCE
    return Preparation(deadline,
                       PipeSpec(base + "-in", flags | PIPE_ACCESS_OUTBOUND, "read"),
                       PipeSpec(base + "-out", flags | PIPE_ACCESS_INBOUND, "write"))


@dataclass(frozen=True)
class Completion:
    state: str  # pending, complete, error (error is terminal)
    count: int = 0
    error: int | None = None


@dataclass(eq=False)
class Operation:
    handle: object
    direction: str
    buffer: bytearray
    event: object
    overlapped: object
    terminal: bool = False
    cancel_requested: bool = False


class FakeKernel(Protocol):
    cp316_fake_kernel: bool
    def create_event(self) -> object: ...
    def create_overlapped(self, event: object) -> object: ...
    def submit(self, operation: Operation) -> Completion: ...
    def poll(self, operation: Operation) -> Completion: ...
    def wait(self, event: object, remaining_ms: int) -> None: ...
    def cancel(self, operation: Operation) -> None: ...
    def close_event(self, event: object) -> None: ...


class OverlappedModel:
    """Fake-only lifecycle model using an already prepared absolute deadline.

    Retain this owner after HOLD. retained operations must be polled to terminal
    before handles can close. No destructor closes events or pipe handles.
    Exceptions after submission conservatively retain ownership for later poll.
    """
    def __init__(self, kernel: FakeKernel, preparation: Preparation,
                 *, clock: Callable[[], float] = time.monotonic):
        if getattr(kernel, "cp316_fake_kernel", False) is not True:
            raise TypeError("injected fake kernel required; native binding missing")
        self.kernel, self.preparation, self.clock = kernel, preparation, clock
        self.retained: list[Operation] = []
        self.held = False

    def _budget(self) -> int:
        remaining = self.preparation.deadline - self.clock()
        if not math.isfinite(remaining) or remaining < .001:
            raise Hold("single pre-creation deadline exhausted")
        return int(remaining * 1000)

    @staticmethod
    def _validate(result: Completion):
        if not isinstance(result, Completion) or result.state not in ("pending", "complete", "error"):
            raise Hold("invalid completion; ownership retained")

    def _terminal(self, op: Operation, result: Completion):
        self._validate(result)
        if result.state == "pending":
            return False
        op.terminal = True
        # Keep retained if close itself fails; no double-close retry is inferred.
        self.kernel.close_event(op.event)
        self.retained.remove(op)
        return True

    def _cancel(self, op: Operation):
        if op.terminal or op.cancel_requested:
            return
        op.cancel_requested = True
        try:
            self.kernel.cancel(op)
        except Exception:
            pass  # outcome uncertain, including ERROR_NOT_FOUND race
        try:
            self._terminal(op, self.kernel.poll(op))
        except Exception:
            pass  # preserve exact resources until a later terminal receipt

    def reap(self) -> int:
        """One nonblocking pass, even after deadline. No implicit cleanup wait."""
        for op in tuple(self.retained):
            if not op.terminal:
                self._terminal(op, self.kernel.poll(op))
        return len(self.retained)

    def require_releasable(self):
        if self.retained:
            raise Hold("pipe closure forbidden while operation resources retained")

    def transfer(self, handle: object, direction: str,
                 payload: bytes | int) -> bytes:
        if self.held:
            raise Hold("owner held after uncertain transfer")
        if direction == "write" and type(payload) is bytes:
            size = len(payload)
        elif direction == "read" and type(payload) is int:
            size = payload
        else:
            raise ValueError("write bytes or read integer required")
        if size not in FRAME_SIZES:
            raise ValueError("exact 48/80-byte frame required")
        output = bytearray()
        offset = 0
        op = None
        try:
            while offset < size:  # at most size iterations: zero progress fails
                self._budget()
                event = self.kernel.create_event()
                try:
                    overlapped = self.kernel.create_overlapped(event)
                except Exception:
                    self.kernel.close_event(event)
                    raise
                buffer = bytearray(payload[offset:] if direction == "write" else size - offset)
                op = Operation(handle, direction, buffer, event, overlapped)
                self.retained.append(op)  # own BEFORE submit may throw
                self._budget()
                result = self.kernel.submit(op)
                waits = 0
                while True:
                    self._validate(result)
                    if self._terminal(op, result):
                        break
                    if waits >= MAX_PENDING_WAITS:
                        raise Hold("pending completion wait count exhausted")
                    budget = self._budget()
                    self.kernel.wait(op.event, budget)
                    waits += 1
                    self._budget()
                    result = self.kernel.poll(op)
                self._budget()  # late success remains HOLD, never resubmitted
                if result.state == "error":
                    raise Hold(f"terminal I/O error {result.error}")
                if type(result.count) is not int or not 0 < result.count <= len(buffer):
                    raise Hold("zero/invalid completion count")
                if direction == "read":
                    output.extend(buffer[:result.count])
                offset += result.count
            return bytes(output)
        except BaseException:
            self.held = True
            if op is not None and op in self.retained:
                self._cancel(op)
            raise


def create_native_pipe_adapter(*_args, **_kwargs):
    raise Hold("native Win32 ABI, private DACL, endpoint creation/connection and HANDLE_LIST binding missing; HARD HOLD")
