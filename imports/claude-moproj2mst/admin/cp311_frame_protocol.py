"""CP311 strict in-memory framed JSON protocol (no OS or process operations).

This module validates byte framing and an occurrence-bound OFFER/ACK/RECEIPT
sequence. It supplies no authenticated worker identity or resume authority:
without external pipe and identity binding, real entry remains HARD HOLD.
"""
from __future__ import annotations

from dataclasses import dataclass
from enum import Enum, auto
import json
import re
import struct

MAX_FRAME = 4096
MAX_TOTAL = 16384
NONCE_BITS = 256
_HEX_NONCE = re.compile(r"[0-9a-f]{64}\Z")
_DECIMAL = re.compile(r"(?:0|[1-9][0-9]*)\Z")


class FrameError(ValueError):
    pass


class ProtocolError(RuntimeError):
    pass


def _unique_object(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            raise FrameError("duplicate JSON key")
        result[key] = value
    return result


def _finite_float(text: str) -> float:
    value = float(text)
    if not __import__("math").isfinite(value):
        raise FrameError("non-finite JSON number")
    return value


def _reject_surrogates(value) -> None:
    if isinstance(value, str):
        if any(0xD800 <= ord(ch) <= 0xDFFF for ch in value):
            raise FrameError("unpaired surrogate in JSON string")
    elif isinstance(value, list):
        for item in value:
            _reject_surrogates(item)
    elif isinstance(value, dict):
        for key, item in value.items():
            _reject_surrogates(key)
            _reject_surrogates(item)


def encode_frame(value: dict) -> bytes:
    if type(value) is not dict:
        raise FrameError("frame payload must be an object")
    try:
        payload = json.dumps(value, ensure_ascii=False, separators=(",", ":"),
                             allow_nan=False).encode("utf-8", errors="strict")
    except (TypeError, ValueError, UnicodeError) as exc:
        raise FrameError("payload is not strict UTF-8 JSON") from exc
    if not payload or len(payload) > MAX_FRAME:
        raise FrameError("frame length outside bounds")
    return struct.pack("<I", len(payload)) + payload


def decode_frames(data: bytes) -> list[dict]:
    """Decode exactly concatenated frames; reject incomplete or extra bytes."""
    if type(data) is not bytes:
        raise FrameError("input must be bytes")
    if len(data) > MAX_TOTAL:
        raise FrameError("total byte limit exceeded")
    frames, pos = [], 0
    while pos < len(data):
        if len(data) - pos < 4:
            raise FrameError("truncated length prefix")
        size = struct.unpack_from("<I", data, pos)[0]
        pos += 4
        if size == 0 or size > MAX_FRAME:
            raise FrameError("frame length outside bounds")
        end = pos + size
        if end > len(data):
            raise FrameError("truncated frame payload")
        raw = data[pos:end]
        pos = end
        try:
            value = json.loads(raw.decode("utf-8", errors="strict"),
                               object_pairs_hook=_unique_object,
                               parse_float=_finite_float,
                               parse_constant=lambda _x: (_ for _ in ()).throw(FrameError("non-finite JSON number")))
        except FrameError:
            raise
        except (UnicodeError, json.JSONDecodeError, RecursionError) as exc:
            raise FrameError("invalid UTF-8 JSON") from exc
        if type(value) is not dict:
            raise FrameError("frame payload must be an object")
        _reject_surrogates(value)
        frames.append(value)
    if not frames:
        raise FrameError("empty stream")
    return frames


def handle_value(text: object, pointer_bits: int = struct.calcsize("P") * 8) -> int:
    """Parse canonical decimal handle text bounded to the pointer-width range."""
    if type(text) is not str or not _DECIMAL.fullmatch(text):
        raise FrameError("handle must be canonical decimal text")
    if type(pointer_bits) is not int or pointer_bits not in (32, 64):
        raise FrameError("unsupported pointer width")
    maximum = (1 << pointer_bits) - 1
    maximum_text = str(maximum)
    if len(text) > len(maximum_text) or (len(text) == len(maximum_text) and text > maximum_text):
        raise FrameError("handle outside pointer-width range")
    value = int(text)
    if value == 0:
        raise FrameError("handle outside pointer-width range")
    return value


def _nonce(value: object) -> str:
    if type(value) is not str or not _HEX_NONCE.fullmatch(value):
        raise ProtocolError("occurrence nonce must be exactly 256-bit lowercase hex")
    return value


class Phase(Enum):
    READY = auto()
    OFFERED = auto()
    ACKED = auto()
    RECEIPTED = auto()
    COMPLETE = auto()
    HOLD = auto()


@dataclass(frozen=True)
class Message:
    kind: str
    occurrence: str
    handle: str

    def wire(self) -> dict:
        return {"kind": self.kind, "occurrence": self.occurrence, "handle": self.handle}


class FrameProtocol:
    """Single-use state machine. Matching frames are not worker authentication."""
    def __init__(self, occurrence: str, handle: str, pointer_bits: int = struct.calcsize("P") * 8):
        self.occurrence = _nonce(occurrence)
        handle_value(handle, pointer_bits)
        self.handle = handle
        self.pointer_bits = pointer_bits
        self.phase = Phase.READY
        self._offer = Message("OFFER", self.occurrence, handle)
        self._ack = Message("ACK", self.occurrence, handle)
        self._receipt = Message("RECEIPT", self.occurrence, handle)

    @property
    def can_resume(self) -> bool:
        return False

    def external_entry(self, kind: str) -> None:
        self.phase = Phase.HOLD
        raise ProtocolError(f"{kind} entry HARD HOLD: no external pipe/identity binding")

    def _accept(self, obj: object, expected: Message, phase: Phase, next_phase: Phase):
        if self.phase is not phase:
            self.phase = Phase.HOLD
            raise ProtocolError("out-of-order frame; outcome uncertain")
        if type(obj) is not dict or set(obj) != {"kind", "occurrence", "handle"}:
            self.phase = Phase.HOLD
            raise ProtocolError("extra or missing frame field")
        try:
            handle_value(obj["handle"], self.pointer_bits)
            actual = Message(obj["kind"], _nonce(obj["occurrence"]), obj["handle"])
        except (ProtocolError, FrameError, TypeError):
            self.phase = Phase.HOLD
            raise ProtocolError("malformed frame; outcome uncertain") from None
        if actual != expected:
            self.phase = Phase.HOLD
            raise ProtocolError("frame mismatch; outcome uncertain")
        self.phase = next_phase
        return expected.wire()

    def _decode_one(self, data: bytes) -> dict:
        try:
            frames = decode_frames(data)
            if len(frames) != 1:
                raise FrameError("expected exactly one protocol frame")
            return frames[0]
        except FrameError:
            self.phase = Phase.HOLD
            raise

    def offer(self) -> bytes:
        if self.phase is not Phase.READY:
            self.phase = Phase.HOLD
            raise ProtocolError("OFFER out of order")
        self.phase = Phase.OFFERED
        return encode_frame(self._offer.wire())

    def accept_offer(self, data: bytes) -> bytes:
        frame = self._decode_one(data)
        self._accept(frame, self._offer, Phase.OFFERED, Phase.ACKED)
        return encode_frame(self._ack.wire())

    def accept_ack(self, data: bytes) -> bytes:
        frame = self._decode_one(data)
        self._accept(frame, self._ack, Phase.ACKED, Phase.RECEIPTED)
        return encode_frame(self._receipt.wire())

    def accept_receipt(self, data: bytes) -> None:
        frame = self._decode_one(data)
        self._accept(frame, self._receipt, Phase.RECEIPTED, Phase.COMPLETE)

    def timeout(self) -> None:
        self.phase = Phase.HOLD
        raise ProtocolError("timeout; delivery outcome uncertain")
