"""Bounded best-effort CP418 terminal-write marker for one runner occurrence."""
from __future__ import annotations

import hashlib
from pathlib import Path

_DOMAIN = b"codex-deform-rea:cp418-runner-marker:v1\0"
_MAX_SIZE = 32
_RECORDS = {b"entered\n", b"entered\nreturned\n"}


def _identity(nonce: bytes, deadline: int) -> None:
    if type(nonce) is not bytes or len(nonce) != 32:
        raise ValueError("exact 32-byte nonce required")
    if type(deadline) is not int or not 0 < deadline < 2**64:
        raise ValueError("positive uint64 deadline required")


def marker_path(nonce: bytes, deadline: int) -> Path:
    """Return the sole analysis-directory path derived from occurrence data."""
    _identity(nonce, deadline)
    digest = hashlib.sha256(_DOMAIN + nonce + deadline.to_bytes(8, "big")).hexdigest()
    return Path(__file__).resolve().parent.parent / "analysis" / f"cp418-runner-{digest}.txt"


def write_stage(nonce: bytes, deadline: int, stage: str) -> bool:
    """Create entered exclusively, or append returned after exact verification."""
    _identity(nonce, deadline)
    if type(stage) is not str or stage not in ("entered", "returned"):
        return False
    path = marker_path(nonce, deadline)
    try:
        if stage == "entered":
            with path.open("xb") as stream:
                stream.write(b"entered\n")
                stream.flush()
                import os
                os.fsync(stream.fileno())
            return True
        with path.open("rb") as stream:
            current = stream.read(_MAX_SIZE + 1)
        if current != b"entered\n":
            return False
        with path.open("ab") as stream:
            stream.write(b"returned\n")
            stream.flush()
            import os
            os.fsync(stream.fileno())
        return True
    except BaseException:
        return False


def read_stage(nonce: bytes, deadline: int) -> str:
    """Read only the bounded fixed marker and classify its exact contents."""
    _identity(nonce, deadline)
    path = marker_path(nonce, deadline)
    try:
        with path.open("rb") as stream:
            data = stream.read(_MAX_SIZE + 1)
        if data == b"entered\n":
            return "entered"
        if data == b"entered\nreturned\n":
            return "returned"
        return "malformed"
    except FileNotFoundError:
        return "absent"
    except BaseException:
        return "unavailable"
