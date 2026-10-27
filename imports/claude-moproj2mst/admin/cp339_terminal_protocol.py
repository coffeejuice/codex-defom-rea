"""Fixed 80-byte terminal claim for one contained CP316 inert occurrence.

The runner emits this only after CP330 and CP329 report complete success.
It is a claim over a private channel, not proof of the watchdog's exact process
waits, exit codes, job accounting, pipe drain, or cleanup. Those observations
must be made independently before accepting a trial. Import is inert.
"""
from __future__ import annotations

from dataclasses import dataclass
import hashlib
import hmac
import struct

NATIVE_ENTRY = "HARD HOLD"
MAGIC = 0x39333343  # CP339, distinct from CP332 BOOTSTRAP/OFFER/ACK.
VERSION = 1
FLAG_ZERO_CHILD_RESULT = 1 << 0
FLAG_WORKER_ACK = 1 << 1
FLAG_WORKER_EXIT_ZERO = 1 << 2
FLAG_WORKER_JOB_EMPTY = 1 << 3
FLAG_CHILD_JOB_EMPTY = 1 << 4
FLAG_TERMINAL_IO = 1 << 5
FLAGS = (FLAG_ZERO_CHILD_RESULT | FLAG_WORKER_ACK | FLAG_WORKER_EXIT_ZERO |
         FLAG_WORKER_JOB_EMPTY | FLAG_CHILD_JOB_EMPTY | FLAG_TERMINAL_IO)
FRAME = struct.Struct("<IIQII32s24s")
_BODY = struct.Struct("<IIQII32s")


class TerminalError(ValueError):
    pass


def _deadline(value):
    if type(value) is not int or not 0 < value < 2**64:
        raise TerminalError("positive uint64 absolute deadline required")
    return value


def _nonce(value):
    if type(value) is not bytes or len(value) != 32 or not any(value):
        raise TerminalError("nonzero exact 32-byte nonce required")
    return value


@dataclass(frozen=True)
class TerminalSuccess:
    deadline: int
    nonce: bytes

    def __post_init__(self):
        _deadline(self.deadline)
        _nonce(self.nonce)


def encode_terminal(value):
    if type(value) is not TerminalSuccess:
        raise TerminalError("TerminalSuccess required")
    body = _BODY.pack(MAGIC, VERSION, value.deadline, FLAGS, 0, value.nonce)
    return body + hashlib.sha256(body).digest()[:24]


def decode_terminal(raw, *, deadline, nonce):
    """Validate exact frame and occurrence; never grant trial acceptance."""
    expected_deadline, expected_nonce = _deadline(deadline), _nonce(nonce)
    if type(raw) is not bytes or len(raw) != FRAME.size:
        raise TerminalError("exact 80-byte terminal frame required")
    magic, version, got_deadline, flags, reserved, got_nonce, digest = FRAME.unpack(raw)
    if (magic, version, flags, reserved) != (MAGIC, VERSION, FLAGS, 0):
        raise TerminalError("terminal version, flags or reserved field invalid")
    if got_deadline != expected_deadline or got_nonce != expected_nonce:
        raise TerminalError("terminal occurrence mismatch")
    if not hmac.compare_digest(digest, hashlib.sha256(raw[:_BODY.size]).digest()[:24]):
        raise TerminalError("terminal frame checksum mismatch")
    return TerminalSuccess(got_deadline, got_nonce)


def from_runner(run_receipt, worker_receipt, *, deadline, nonce):
    """Build a terminal claim only from all CP330/CP329 success checkpoints."""
    value = TerminalSuccess(deadline, nonce)
    if (run_receipt is None or worker_receipt is None or
            type(getattr(run_receipt, "deadline", None)) is not int or
            run_receipt.deadline != deadline or
            getattr(run_receipt, "phase", None) != "closed-success" or
            getattr(run_receipt, "resume_outcome", None) != "resumed-once-previous-count-one" or
            getattr(run_receipt, "init_outcome", None) != "48-bytes-written" or
            getattr(run_receipt, "result_outcome", None) != "80-byte-zero-child-result" or
            getattr(run_receipt, "peek_outcome", None) != "zero-queued-byte-snapshot" or
            getattr(run_receipt, "ack_outcome", None) != "exact-worker-exit-zero-observed" or
            getattr(run_receipt, "errors", None) != [] or
            getattr(worker_receipt, "phase", None) != "closed" or
            getattr(worker_receipt, "worker_waited", None) is not True or
            getattr(worker_receipt, "jobs_empty", None) is not True or
            getattr(worker_receipt, "worker_terminated", None) is not False or
            getattr(worker_receipt, "errors", None) != []):
        raise TerminalError("complete CP330 transcript and CP329 cleanup required")
    return value
