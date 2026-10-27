"""Inert CP312 v2 binary bridge for the pinned, single CHILD-job worker.

This module does not start a process, inspect a job, or authorize a resume.
RESULT is worker telemetry. Its PID, handle and membership fields require
independent supervisor-side identity and job checks before any operational use.
Even a matching ACK is only a byte-level echo, not an authenticated receipt.
Native execution remains HARD HOLD.
"""

from dataclasses import dataclass
import struct
from typing import Protocol


MAGIC = 0x32313343
VERSION = 2
NONCE_SIZE = 32
INIT = struct.Struct("<IIQ32s")
RESULT = struct.Struct("<10IQ32s")
ACK = struct.Struct("<IIQ32s")
NATIVE_ENTRY = "HARD HOLD"


class ProtocolError(ValueError):
    """A complete frame violates the CP312 v2 contract."""


class UncertainTransfer(RuntimeError):
    """A partial, timed out, or broken transfer has an unknown worker outcome."""


class BinaryTransport(Protocol):
    def read(self, size: int) -> bytes: ...
    def write(self, data: bytes) -> int: ...


def _uint(value: int, bits: int, name: str, *, positive: bool = False) -> int:
    if type(value) is not int or value < (1 if positive else 0) or value >= 1 << bits:
        raise ProtocolError(f"{name} must be a {'positive ' if positive else ''}uint{bits}")
    return value


def _bool(value: int, name: str) -> int:
    if type(value) is not int or value not in (0, 1):
        raise ProtocolError(f"{name} must be 0 or 1")
    return value


def _nonce(value: bytes) -> bytes:
    if type(value) is not bytes or len(value) != NONCE_SIZE:
        raise ProtocolError("nonce must be exactly 32 raw bytes")
    return value


def _header(magic: int, version: int) -> None:
    if magic != MAGIC or version != VERSION:
        raise ProtocolError("wrong CP312 magic or protocol version")


def _frame(data: bytes, size: int) -> bytes:
    if type(data) is not bytes or len(data) != size:
        raise ProtocolError(f"frame must be exactly {size} bytes")
    return data


@dataclass(frozen=True)
class Init:
    child_job: int  # Worker-local, noninheritable handle for the sole CHILD job.
    nonce: bytes

    def __post_init__(self) -> None:
        _uint(self.child_job, 64, "child_job", positive=True)
        _nonce(self.nonce)


@dataclass(frozen=True)
class WorkerTelemetry:
    create_called: int
    create_ok: int
    create_error: int
    pid: int
    tid: int
    child_query_ok: int
    child_query_error: int
    child_member: int
    child_process_handle: int  # Worker-local; never an ownership proof.
    nonce: bytes

    def __post_init__(self) -> None:
        for name in ("create_called", "create_ok", "child_query_ok", "child_member"):
            _bool(getattr(self, name), name)
        for name in ("create_error", "pid", "tid", "child_query_error"):
            _uint(getattr(self, name), 32, name)
        _uint(self.child_process_handle, 64, "child_process_handle")
        _nonce(self.nonce)
        if not self.create_called:
            if any((self.create_ok, self.create_error, self.pid, self.tid,
                    self.child_query_ok, self.child_query_error, self.child_member,
                    self.child_process_handle)):
                raise ProtocolError("uncalled CreateProcessW requires zeroed telemetry")
        elif not self.create_ok:
            if any((self.pid, self.tid, self.child_query_ok, self.child_query_error,
                    self.child_member, self.child_process_handle)):
                raise ProtocolError("failed CreateProcessW cannot report a child")
        elif not (self.pid and self.tid and self.child_process_handle):
            raise ProtocolError("successful CreateProcessW requires PID, TID and handle")
        if not self.child_query_ok and self.child_member:
            raise ProtocolError("failed or uncalled child query cannot report membership")
        # Both error fields are immediate raw GetLastError values, including on success.


@dataclass(frozen=True)
class Ack:
    child_process_handle: int
    nonce: bytes

    def __post_init__(self) -> None:
        _uint(self.child_process_handle, 64, "child_process_handle")
        _nonce(self.nonce)


def encode_init(value: Init) -> bytes:
    if type(value) is not Init:
        raise ProtocolError("expected Init")
    return INIT.pack(MAGIC, VERSION, value.child_job, value.nonce)


def decode_init(data: bytes) -> Init:
    magic, version, handle, nonce = INIT.unpack(_frame(data, INIT.size))
    _header(magic, version)
    return Init(handle, nonce)


def encode_result(value: WorkerTelemetry) -> bytes:
    if type(value) is not WorkerTelemetry:
        raise ProtocolError("expected WorkerTelemetry")
    return RESULT.pack(MAGIC, VERSION, value.create_called, value.create_ok,
                       value.create_error, value.pid, value.tid, value.child_query_ok,
                       value.child_query_error, value.child_member,
                       value.child_process_handle, value.nonce)


def decode_result(data: bytes) -> WorkerTelemetry:
    fields = RESULT.unpack(_frame(data, RESULT.size))
    _header(fields[0], fields[1])
    return WorkerTelemetry(*fields[2:])


def encode_ack(value: Ack) -> bytes:
    if type(value) is not Ack:
        raise ProtocolError("expected Ack")
    return ACK.pack(MAGIC, VERSION, value.child_process_handle, value.nonce)


def decode_ack(data: bytes) -> Ack:
    magic, version, handle, nonce = ACK.unpack(_frame(data, ACK.size))
    _header(magic, version)
    return Ack(handle, nonce)


def result_matches_init(result: WorkerTelemetry, init: Init) -> bool:
    """Match the worker's nonce echo; this is not independent identity proof."""
    return result.nonce == init.nonce


def ack_matches(ack: Ack, result: WorkerTelemetry) -> bool:
    """Match the worker's byte-level echo; this grants no resume authority."""
    return (ack.child_process_handle == result.child_process_handle
            and ack.nonce == result.nonce)


def read_exact(transport: BinaryTransport, size: int) -> bytes:
    """Read one known-size frame; interruption leaves the outcome uncertain.

    This consumes exactly one frame. Trailing stream bytes remain unknown until
    the caller explicitly checks the transport end or reads another frame.
    """
    if size not in (INIT.size, RESULT.size, ACK.size):
        raise ProtocolError("unsupported frame size")
    parts = []
    remaining = size
    try:
        while remaining:
            part = transport.read(remaining)
            if type(part) is not bytes or not part or len(part) > remaining:
                raise UncertainTransfer("EOF or invalid read during frame")
            parts.append(part)
            remaining -= len(part)
    except (TimeoutError, OSError) as exc:
        raise UncertainTransfer("read interrupted; worker outcome unknown") from exc
    return b"".join(parts)


def write_exact(transport: BinaryTransport, data: bytes) -> None:
    """Write one complete frame; a partial write has uncertain peer state."""
    _frame(data, len(data))
    if len(data) not in (INIT.size, RESULT.size, ACK.size):
        raise ProtocolError("unsupported frame size")
    offset = 0
    try:
        while offset < len(data):
            written = transport.write(data[offset:])
            if type(written) is not int or not 0 < written <= len(data) - offset:
                raise UncertainTransfer("zero or invalid write during frame")
            offset += written
    except (TimeoutError, OSError) as exc:
        raise UncertainTransfer("write interrupted; peer state unknown") from exc
