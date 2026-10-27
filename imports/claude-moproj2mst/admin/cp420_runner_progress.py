"""Bounded best-effort progress record for one CP335 runner occurrence."""
from __future__ import annotations

import hashlib
import os
from pathlib import Path

_DOMAIN = b"codex-deform-rea:cp420-runner-progress:v2\0"
_STAGES = ("bootstrap", "parent_ack", "worker_resumed", "result",
           "worker_ack", "worker_closed", "terminal_entered", "terminal_returned")
_RECORDS = tuple(("\n".join(_STAGES[:i]) + "\n").encode("ascii")
                 for i in range(1, len(_STAGES) + 1))
_LIMIT = 160


def _identity(nonce: bytes, deadline: int) -> None:
    if type(nonce) is not bytes or len(nonce) != 32:
        raise ValueError("exact 32-byte nonce required")
    if type(deadline) is not int or not 0 < deadline < 2**64:
        raise ValueError("positive uint64 deadline required")


def path(nonce: bytes, deadline: int) -> Path:
    _identity(nonce, deadline)
    digest = hashlib.sha256(_DOMAIN + nonce + deadline.to_bytes(8, "big")).hexdigest()
    return Path(__file__).resolve().parent.parent / "analysis" / f"cp420-runner-{digest}.txt"


def write_progress(nonce: bytes, deadline: int, stage: str) -> bool:
    _identity(nonce, deadline)
    if type(stage) is not str or stage not in _STAGES:
        return False
    target = path(nonce, deadline)
    index = _STAGES.index(stage)
    try:
        if index == 0:
            with target.open("xb") as stream:
                stream.write(_RECORDS[0]); stream.flush(); os.fsync(stream.fileno())
            return True
        with target.open("rb") as stream:
            current = stream.read(_LIMIT + 1)
        if current != _RECORDS[index - 1]:
            return False
        with target.open("ab") as stream:
            stream.write((stage + "\n").encode("ascii"))
            stream.flush(); os.fsync(stream.fileno())
        return True
    except BaseException:
        return False


def read_progress(nonce: bytes, deadline: int) -> str:
    _identity(nonce, deadline)
    try:
        with path(nonce, deadline).open("rb") as stream:
            current = stream.read(_LIMIT + 1)
        if current in _RECORDS:
            return _STAGES[_RECORDS.index(current)]
        return "malformed"
    except FileNotFoundError:
        return "absent"
    except BaseException:
        return "unavailable"
