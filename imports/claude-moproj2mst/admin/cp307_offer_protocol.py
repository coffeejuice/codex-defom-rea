"""CP307 fake-only occurrence-bound child offer handshake.

This module models a trusted *in-memory* harness binding. It does not create a
process, socket, GUI session, or authentication channel. ``worker_identity`` is
sideband harness evidence, independent of the worker's OFFER fields; a caller-
supplied identity echo inside a frame never authenticates a real child.
The locally constructed receipt models a worker acceptance claim within this
trusted harness; it is not independent proof that an external worker accepted
the ACK.

No adapter to an original, real child, or GUI entry point is provided. Those
paths remain on hard HOLD until separately established evidence and controls
permit them. A missing, duplicate, reordered, or mismatched frame leaves the
outcome uncertain and never authorizes resume.
"""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum, auto


@dataclass(frozen=True)
class Occurrence:
    run_id: str
    occurrence_id: str
    child_id: str


@dataclass(frozen=True)
class Offer:
    occurrence: Occurrence
    offer_id: str
    claimed_child_id: str


@dataclass(frozen=True)
class Ack:
    offer: Offer
    supervisor_id: str


@dataclass(frozen=True)
class Receipt:
    ack: Ack


class Phase(Enum):
    READY = auto()
    OFFER_SENT = auto()
    ACK_SENT = auto()
    RECEIPT_SENT = auto()
    VERIFIED = auto()
    HOLD = auto()


class ProtocolError(RuntimeError):
    """A fail-closed transition; frame delivery may have happened."""


class FakeOfferProtocol:
    """A single-use, bounded handshake over a trusted in-memory binding.

    The harness supplies ``worker_identity`` separately from any OFFER payload.
    That identity is a modeling assumption, not proof of operating-system child
    identity. The worker knows the expected supervisor identity in advance and
    compares the complete ACK against the exact value it expects.
    """

    def __init__(
        self,
        expected: Occurrence,
        *,
        worker_identity: str,
        supervisor_identity: str,
    ) -> None:
        if not all((expected.run_id, expected.occurrence_id, expected.child_id,
                    worker_identity, supervisor_identity)):
            raise ValueError("all occurrence and binding identifiers must be nonempty")
        self.expected = expected
        self._worker_identity = worker_identity
        self._supervisor_identity = supervisor_identity
        self.phase = Phase.READY
        self.hold_reason: str | None = None
        self._offer: Offer | None = None
        self._ack: Ack | None = None
        self._worker_expected_ack: Ack | None = None

    @property
    def can_resume(self) -> bool:
        """Permission for this fake harness only; no real child resume API exists."""
        return self.phase is Phase.VERIFIED

    def external_entry(self, kind: str) -> None:
        """Keep real child, original executable, and GUI paths on hard HOLD."""
        if kind not in {"real", "original", "gui"}:
            raise ValueError("unknown external entry kind")
        self._hold(f"{kind} entry hard HOLD; fake channel cannot authorize it")

    def _hold(self, reason: str) -> None:
        self.phase = Phase.HOLD
        self.hold_reason = reason
        raise ProtocolError(reason)

    def _expect(self, phase: Phase, action: str) -> None:
        if self.phase is not phase:
            self._hold(f"{action} in {self.phase.name}; outcome uncertain")

    def worker_send_offer(self, offer_id: str) -> Offer:
        self._expect(Phase.READY, "offer")
        if not offer_id:
            self._hold("empty offer id; outcome uncertain")
        offer = Offer(self.expected, offer_id, self._worker_identity)
        self._offer = offer
        self._worker_expected_ack = Ack(offer, self._supervisor_identity)
        self.phase = Phase.OFFER_SENT
        return offer

    def supervisor_receive_offer(self, frame: Offer) -> Ack:
        self._expect(Phase.OFFER_SENT, "offer delivery")
        if (
            not isinstance(frame, Offer)
            or frame != self._offer
            or frame.occurrence != self.expected
            or frame.claimed_child_id != self.expected.child_id
            or self._worker_identity != self.expected.child_id
        ):
            self._hold("offer or independent harness child identity mismatch; outcome uncertain")
        ack = Ack(frame, self._supervisor_identity)
        self._ack = ack
        self.phase = Phase.ACK_SENT
        return ack

    def worker_receive_ack(self, frame: Ack) -> Receipt:
        """Model exact worker-side comparison and its local receipt claim."""
        self._expect(Phase.ACK_SENT, "ACK delivery")
        # Compare the complete frame, not a subset or a caller-provided echo.
        if not isinstance(frame, Ack) or frame != self._worker_expected_ack:
            self._hold("ACK mismatch; outcome uncertain")
        receipt = Receipt(frame)
        self.phase = Phase.RECEIPT_SENT
        return receipt

    def supervisor_receive_receipt(self, frame: Receipt) -> None:
        self._expect(Phase.RECEIPT_SENT, "receipt delivery")
        if not isinstance(frame, Receipt) or frame != Receipt(self._ack):
            self._hold("receipt mismatch; outcome uncertain")
        self.phase = Phase.VERIFIED

    def timeout(self, waiting_for: str) -> None:
        """Record an absent delivery without guessing whether it was lost."""
        if not waiting_for:
            raise ValueError("waiting_for must be nonempty")
        self._hold(f"timeout waiting for {waiting_for}; delivery outcome uncertain")

