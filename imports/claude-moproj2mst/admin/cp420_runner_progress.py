"""Bounded best-effort progress record for one CP335 runner occurrence."""
from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path
import re

_DOMAIN = b"codex-deform-rea:cp420-runner-progress:v4\0"
_FAILURE_DOMAIN = b"codex-deform-rea:cp438-completion-failure:v4\0"
_STAGES = ("bootstrap", "parent_ack", "worker_resumed", "result",
           "worker_ack", "worker_closed", "terminal_entered", "terminal_returned")
_LIMIT = 160


def _identity(nonce: bytes, deadline: int) -> None:
    if type(nonce) is not bytes or len(nonce) != 32:
        raise ValueError("exact 32-byte nonce required")
    if type(deadline) is not int or not 0 < deadline < 2**64:
        raise ValueError("positive uint64 deadline required")


def path(nonce: bytes, deadline: int) -> Path:
    _identity(nonce, deadline)
    digest = hashlib.sha256(_DOMAIN + nonce + deadline.to_bytes(8, "big")).hexdigest()
    return Path(__file__).resolve().parent.parent / "analysis" / f"cp429-runner-{digest}.txt"


def failure_path(nonce: bytes, deadline: int) -> Path:
    """Separate one-use CP438 diagnostic; preserve historical records."""
    _identity(nonce, deadline)
    digest = hashlib.sha256(_FAILURE_DOMAIN + nonce + deadline.to_bytes(8, "big")).hexdigest()
    return Path(__file__).resolve().parent.parent / "analysis" / f"cp438-failure-{digest}.json"


def write_failure_completion(nonce: bytes, deadline: int, snapshot: dict) -> bool:
    """Best-effort, failure-only persistence; never called on the ACK path."""
    _identity(nonce, deadline)
    if (type(snapshot) is not dict or set(snapshot) != {"runner", "worker", "failure"} or
            any(type(snapshot[name]) is not dict for name in ("runner", "worker", "failure"))):
        return False
    try:
        payload = (json.dumps({"schema": 4, "snapshot": snapshot},
                              sort_keys=True, separators=(",", ":"),
                              ensure_ascii=True) + "\n").encode("ascii")
        if len(payload) > 1024:
            return False
        with failure_path(nonce, deadline).open("xb") as stream:
            stream.write(payload)
            stream.flush()
        return True
    except BaseException:
        return False


def _parse(current: bytes):
    """Return exact last-stage index and ACK remaining ms, or None."""
    if (not current or len(current) > _LIMIT or not current.endswith(b"\n")):
        return None
    lines = current.splitlines()
    if len(lines) > len(_STAGES):
        return None
    remaining = None
    for index, line in enumerate(lines):
        if index == 4:
            match = re.fullmatch(rb"worker_ack:([1-9][0-9]{0,3})", line)
            if not match:
                return None
            remaining = int(match.group(1))
            if remaining > 5000:
                return None
        elif line != _STAGES[index].encode("ascii"):
            return None
    return len(lines) - 1, remaining


def write_progress(nonce: bytes, deadline: int, stage: str,
                   remaining: int | None = None) -> bool:
    _identity(nonce, deadline)
    if type(stage) is not str or stage not in _STAGES:
        return False
    target = path(nonce, deadline)
    index = _STAGES.index(stage)
    if index == 4:
        if type(remaining) is not int or not 0 < remaining <= 5000:
            return False
        record = f"worker_ack:{remaining}\n".encode("ascii")
    else:
        if remaining is not None:
            return False
        record = (stage + "\n").encode("ascii")
    try:
        if index == 0:
            with target.open("xb") as stream:
                stream.write(record); stream.flush(); os.fsync(stream.fileno())
            return True
        with target.open("rb") as stream:
            current = stream.read(_LIMIT + 1)
        parsed = _parse(current)
        if parsed is None or parsed[0] != index - 1:
            return False
        with target.open("ab") as stream:
            stream.write(record)
            stream.flush()
            # Post-ACK evidence is best effort: avoid the extra synchronous
            # durability call on the completion/terminal critical path.
            if index < 4:
                os.fsync(stream.fileno())
        return True
    except BaseException:
        return False


def read_progress(nonce: bytes, deadline: int) -> str:
    _identity(nonce, deadline)
    try:
        with path(nonce, deadline).open("rb") as stream:
            current = stream.read(_LIMIT + 1)
        parsed = _parse(current)
        return _STAGES[parsed[0]] if parsed is not None else "malformed"
    except FileNotFoundError:
        return "absent"
    except BaseException:
        return "unavailable"


def read_ack_remaining(nonce: bytes, deadline: int) -> int | None:
    """Return the existing post-ACK clock sample, if its record is complete."""
    _identity(nonce, deadline)
    try:
        with path(nonce, deadline).open("rb") as stream:
            parsed = _parse(stream.read(_LIMIT + 1))
        return parsed[1] if parsed is not None and parsed[0] >= 4 else None
    except BaseException:
        return None
