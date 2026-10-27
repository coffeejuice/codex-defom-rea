"""CP324 inert CP312 WORKER protocol coordinator. Native entry is HARD HOLD.

Only injected exact-frame transport, creator, and exit observer are accepted.
No process, GUI, resume, or native operation is implemented here.
"""
from __future__ import annotations

import math
import secrets
import time

import cp313_worker_binary_protocol as wire

NATIVE_ENTRY = "HARD HOLD"


class Hold(RuntimeError):
    """Protocol stopped; worker outcome may be uncertain."""


class ExactFrameTransport:
    """Fake-only exact-frame contract; cannot prove future absence of bytes.

    assert_no_queued_bytes must nonblockingly reject any already queued byte.
    A native binding remains HARD HOLD until fixed-worker transport implements
    this and independently meets its security and lifecycle requirements.
    """
    def read_exact(self, size: int, deadline: float) -> bytes:  # pragma: no cover - protocol shape
        raise NotImplementedError

    def write_exact(self, frame: bytes, deadline: float) -> None:  # pragma: no cover
        raise NotImplementedError

    def assert_no_queued_bytes(self, deadline: float) -> None:  # pragma: no cover
        raise NotImplementedError


def coordinate(*, deadline: float, creator, transport, wait_worker_exit,
               clock=time.monotonic, nonce_factory=secrets.token_bytes):
    """Run inert handshake under a deadline allocated before creator is called.

    Creator returns the positive worker-local CHILD job handle to place in INIT.
    ACK transmission is unconfirmed until wait_worker_exit(deadline) reports the
    exact WORKER exit. There is intentionally no resume authority.
    """
    if type(deadline) not in (int, float) or not math.isfinite(deadline):
        raise ValueError("finite absolute deadline required")
    if not callable(creator) or not callable(wait_worker_exit) or not callable(clock):
        raise TypeError("injected creator and exact worker-exit observer required")
    if getattr(transport, "cp324_exact_frame_transport", False) is not True:
        raise TypeError("marked exact-frame transport required")
    for method in ("read_exact", "write_exact", "assert_no_queued_bytes"):
        if not callable(getattr(transport, method, None)):
            raise TypeError("exact-frame transport methods required")

    def budget():
        now = clock()
        if not math.isfinite(now) or now >= deadline:
            raise Hold("single pre-creator absolute deadline expired")

    budget()
    nonce = nonce_factory(32)
    if type(nonce) is not bytes or len(nonce) != 32:
        raise Hold("fresh 32-byte nonce required")
    child_job = creator(deadline)
    budget()
    try:
        init = wire.Init(child_job, nonce)
        transport.write_exact(wire.encode_init(init), deadline)
        budget()
        raw = transport.read_exact(wire.RESULT.size, deadline)
        if type(raw) is not bytes or len(raw) != wire.RESULT.size:
            raise Hold("RESULT must be exactly 80 bytes")
        result = wire.decode_result(raw)
        if result.nonce != nonce:
            raise Hold("RESULT nonce mismatch")
        if any((result.create_called, result.create_ok, result.create_error,
                result.pid, result.tid, result.child_query_ok,
                result.child_query_error, result.child_member,
                result.child_process_handle)):
            raise Hold("WORKER RESULT must contain zero child telemetry")
        # Detect queued duplicates/trailing bytes before ACK. This only checks
        # currently queued input; it cannot prove no later byte will arrive.
        transport.assert_no_queued_bytes(deadline)
        budget()
        ack = wire.encode_ack(wire.Ack(0, nonce))
        if len(ack) != wire.ACK.size:
            raise Hold("ACK must be exactly 48 bytes")
        transport.write_exact(ack, deadline)
        # A complete write alone is not receipt or worker termination proof.
        budget()
        exited = wait_worker_exit(deadline)
        budget()
        if exited is not True:
            raise Hold("exact WORKER exit was not observed after ACK")
        return {"init": init, "result": result, "ack": ack,
                "ack_outcome": "worker-exit-observed", "resume_authorized": False}
    except Hold:
        raise
    except BaseException as exc:
        raise Hold(f"protocol outcome uncertain: {type(exc).__name__}: {exc}") from exc
