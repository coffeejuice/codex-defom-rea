"""CP315 conservative bounded fake-only transport for CP312 v2 frames.

No Win32 API is invoked here. Native pipe access remains an explicit HOLD:
PeekNamedPipe/ReadFile/WriteFile cannot be made deadline-safe by wrapping a
synchronous call with a clock check. Every public transfer creates one absolute
deadline before I/O and checks it before and after every injected adapter call.
An adapter must itself honor the supplied remaining budget; a late return is
uncertain and never retried.
"""
from __future__ import annotations

from dataclasses import dataclass, field
import time
from typing import Callable

import cp313_worker_binary_protocol as wire

NATIVE_ENTRY = "HARD HOLD"
MAX_BUDGET_MS = 5000
FRAME_SIZES = (wire.INIT.size, wire.RESULT.size, wire.ACK.size)


class Hold(RuntimeError):
    """Transfer stopped with the stream/peer outcome uncertain."""

    def __init__(self, reason: str, receipt: "TransferReceipt"):
        self.receipt = receipt
        super().__init__(reason)


@dataclass
class TransferReceipt:
    direction: str
    frame_size: int
    phase: str = "created"
    transferred: int = 0
    deadline: float | None = None
    calls: int = 0
    events: list[str] = field(default_factory=list)
    uncertain: bool = True


class BoundedPipeTransport:
    """Deadline adapter. `pipe` is an injected fake and must be marked fake-only.

    Fake contract: peek(remaining_ms)->readable bytes, read(size, remaining_ms)
    -> bytes, write(data, remaining_ms)->count. Write is called directly:
    PeekNamedPipe reports readable bytes, never writable capacity. Calls must not block past the
    supplied budget. EINTR is HOLD (no retry because elapsed time and partial
    effects are uncertain); EOF, exceptions, and late results are also HOLD.
    """
    cp315_bounded_fake_transport = True

    def __init__(self, pipe, *, clock: Callable[[], float] = time.monotonic,
                 budget_ms: int = 5000):
        if getattr(pipe, "cp315_fake_pipe", False) is not True:
            raise TypeError("CP315 accepts injected fake pipes only")
        if not callable(clock) or type(budget_ms) is not int or not 0 < budget_ms <= MAX_BUDGET_MS:
            raise ValueError("budget must be an integer in [1, 5000] ms")
        for name in ("peek", "read", "write"):
            if not callable(getattr(pipe, name, None)):
                raise TypeError(f"fake pipe.{name} required")
        self.pipe, self.clock, self.budget_ms = pipe, clock, budget_ms
        self.receipts: list[TransferReceipt] = []

    def _begin(self, direction: str, size: int) -> tuple[TransferReceipt, float]:
        # Deadline is established before the first adapter call.
        started = self.clock()
        deadline = started + self.budget_ms / 1000.0
        receipt = TransferReceipt(direction, size, deadline=deadline)
        self.receipts.append(receipt)
        return receipt, deadline

    def _hold(self, r: TransferReceipt, detail: str):
        r.phase = "hold"
        r.events.append(detail)
        raise Hold(detail, r)

    def _remaining_ms(self, r: TransferReceipt, deadline: float) -> int:
        remaining = deadline - self.clock()
        if remaining <= 0:
            self._hold(r, "absolute deadline expired before blocking operation")
        # Round down, so an adapter never receives a budget beyond deadline.
        ms = int(remaining * 1000)
        if ms <= 0:
            self._hold(r, "less than one millisecond remains")
        return ms

    def _late(self, r: TransferReceipt, deadline: float):
        if self.clock() >= deadline:
            self._hold(r, "adapter returned at or after absolute deadline")

    def write_frame(self, data: bytes) -> TransferReceipt:
        if type(data) is not bytes or len(data) not in FRAME_SIZES:
            raise wire.ProtocolError("write requires exact 48-byte INIT/ACK or 80-byte RESULT frame")
        r, deadline = self._begin("write", len(data))
        offset = 0
        try:
            while offset < len(data):
                budget = self._remaining_ms(r, deadline)
                r.phase = "write"
                r.calls += 1
                written = self.pipe.write(data[offset:], budget)
                self._late(r, deadline)
                if type(written) is not int or written <= 0 or written > len(data) - offset:
                    self._hold(r, "WriteFile fake returned EOF/invalid/zero count")
                offset += written
                r.transferred = offset
            r.phase, r.uncertain = "complete", False
            return r
        except Hold:
            raise
        except BaseException as exc:
            self._hold(r, f"{type(exc).__name__}: {exc}; write outcome uncertain")

    def read_frame(self, size: int) -> tuple[bytes, TransferReceipt]:
        if type(size) is not int or size not in FRAME_SIZES:
            raise wire.ProtocolError("read requires exact 48-byte INIT/ACK or 80-byte RESULT frame")
        r, deadline = self._begin("read", size)
        data = bytearray()
        try:
            while len(data) < size:
                budget = self._remaining_ms(r, deadline)
                r.phase = "peek"
                r.calls += 1
                available = self.pipe.peek(budget)
                self._late(r, deadline)
                if type(available) is not int or available <= 0:
                    self._hold(r, "PeekNamedPipe fake reported EOF/no bytes")
                count = min(size - len(data), available)
                budget = self._remaining_ms(r, deadline)
                r.phase = "read"
                r.calls += 1
                part = self.pipe.read(count, budget)
                self._late(r, deadline)
                if type(part) is not bytes or not part or len(part) > count:
                    self._hold(r, "ReadFile fake returned EOF or invalid bytes")
                data.extend(part)
                r.transferred = len(data)
            r.phase, r.uncertain = "complete", False
            return bytes(data), r
        except Hold:
            raise
        except BaseException as exc:
            # Includes EINTR; no retry after a potentially partial operation.
            self._hold(r, f"{type(exc).__name__}: {exc}; read outcome uncertain")


def create_native_pipe_transport(*_args, **_kwargs):
    raise Hold("native synchronous Win32 pipe transport HARD HOLD", TransferReceipt("native", 0))
