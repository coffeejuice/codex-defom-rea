"""Bounded fake-channel ordering and failure tests for CP307."""

import unittest

from cp307_offer_protocol import (
    Ack,
    FakeOfferProtocol,
    Occurrence,
    Offer,
    Phase,
    ProtocolError,
    Receipt,
)


EXPECTED = Occurrence("run-1", "occurrence-7", "child-3")


def channel(*, worker_identity: str = "child-3") -> FakeOfferProtocol:
    return FakeOfferProtocol(
        EXPECTED, worker_identity=worker_identity, supervisor_identity="supervisor-2"
    )


class OfferProtocolTests(unittest.TestCase):
    def test_resume_only_after_separate_receipt_verification(self) -> None:
        protocol = channel()
        self.assertFalse(protocol.can_resume)
        offer = protocol.worker_send_offer("offer-11")
        self.assertFalse(protocol.can_resume)
        ack = protocol.supervisor_receive_offer(offer)
        self.assertFalse(protocol.can_resume)
        receipt = protocol.worker_receive_ack(ack)
        self.assertFalse(protocol.can_resume)
        protocol.supervisor_receive_receipt(receipt)
        self.assertEqual(protocol.phase, Phase.VERIFIED)
        self.assertTrue(protocol.can_resume)

    def test_claimed_identity_cannot_override_binding(self) -> None:
        protocol = channel(worker_identity="other-child")
        offer = protocol.worker_send_offer("offer-11")
        echoed_expected_identity = Offer(offer.occurrence, offer.offer_id, "child-3")
        with self.assertRaises(ProtocolError):
            protocol.supervisor_receive_offer(echoed_expected_identity)
        self.assertEqual(protocol.phase, Phase.HOLD)
        self.assertFalse(protocol.can_resume)

        protocol = channel(worker_identity="other-child")
        honest_offer = protocol.worker_send_offer("offer-11")
        with self.assertRaises(ProtocolError):
            protocol.supervisor_receive_offer(honest_offer)
        self.assertFalse(protocol.can_resume)

    def test_wrong_occurrence_and_child_hold(self) -> None:
        for frame in (
            Offer(Occurrence("run-2", "occurrence-7", "child-3"), "offer-11", "child-3"),
            Offer(Occurrence("run-1", "occurrence-8", "child-3"), "offer-11", "child-3"),
            Offer(Occurrence("run-1", "occurrence-7", "child-4"), "offer-11", "child-3"),
        ):
            with self.subTest(frame=frame):
                protocol = channel()
                protocol.worker_send_offer("offer-11")
                with self.assertRaises(ProtocolError):
                    protocol.supervisor_receive_offer(frame)
                self.assertFalse(protocol.can_resume)

    def test_exact_ack_comparison(self) -> None:
        for wrong_ack in (
            Ack(Offer(EXPECTED, "other-offer", "child-3"), "supervisor-2"),
            Ack(Offer(EXPECTED, "offer-11", "child-3"), "other-supervisor"),
        ):
            with self.subTest(ack=wrong_ack):
                protocol = channel()
                offer = protocol.worker_send_offer("offer-11")
                protocol.supervisor_receive_offer(offer)
                with self.assertRaises(ProtocolError):
                    protocol.worker_receive_ack(wrong_ack)
                self.assertFalse(protocol.can_resume)

    def test_receipt_mismatch_holds(self) -> None:
        protocol = channel()
        offer = protocol.worker_send_offer("offer-11")
        ack = protocol.supervisor_receive_offer(offer)
        protocol.worker_receive_ack(ack)
        with self.assertRaises(ProtocolError):
            protocol.supervisor_receive_receipt(Receipt(Ack(offer, "other-supervisor")))
        self.assertFalse(protocol.can_resume)

    def test_duplicate_and_reordered_frames_hold(self) -> None:
        protocol = channel()
        offer = protocol.worker_send_offer("offer-11")
        with self.assertRaises(ProtocolError):
            protocol.worker_receive_ack(Ack(offer, "supervisor-2"))
        self.assertFalse(protocol.can_resume)

        protocol = channel()
        offer = protocol.worker_send_offer("offer-11")
        protocol.supervisor_receive_offer(offer)
        with self.assertRaises(ProtocolError):
            protocol.supervisor_receive_offer(offer)
        self.assertFalse(protocol.can_resume)

        protocol = channel()
        offer = protocol.worker_send_offer("offer-11")
        ack = protocol.supervisor_receive_offer(offer)
        protocol.worker_receive_ack(ack)
        with self.assertRaises(ProtocolError):
            protocol.worker_receive_ack(ack)
        self.assertFalse(protocol.can_resume)

        protocol = channel()
        offer = protocol.worker_send_offer("offer-11")
        ack = protocol.supervisor_receive_offer(offer)
        receipt = protocol.worker_receive_ack(ack)
        protocol.supervisor_receive_receipt(receipt)
        with self.assertRaises(ProtocolError):
            protocol.supervisor_receive_receipt(receipt)
        self.assertFalse(protocol.can_resume)

    def test_timeout_retains_uncertain_outcome(self) -> None:
        for phase in (Phase.READY, Phase.OFFER_SENT, Phase.ACK_SENT, Phase.RECEIPT_SENT):
            with self.subTest(phase=phase):
                protocol = channel()
                if phase is not Phase.READY:
                    offer = protocol.worker_send_offer("offer-11")
                if phase in (Phase.ACK_SENT, Phase.RECEIPT_SENT):
                    ack = protocol.supervisor_receive_offer(offer)
                if phase is Phase.RECEIPT_SENT:
                    protocol.worker_receive_ack(ack)
                self.assertEqual(protocol.phase, phase)
                with self.assertRaisesRegex(ProtocolError, "outcome uncertain"):
                    protocol.timeout("next frame")
                self.assertIn("timeout", protocol.hold_reason)
                self.assertFalse(protocol.can_resume)

    def test_empty_identifiers_fail_closed(self) -> None:
        with self.assertRaises(ValueError):
            FakeOfferProtocol(EXPECTED, worker_identity="", supervisor_identity="supervisor-2")
        protocol = channel()
        with self.assertRaises(ProtocolError):
            protocol.worker_send_offer("")
        self.assertFalse(protocol.can_resume)

    def test_external_entry_is_hard_hold_even_after_fake_verification(self) -> None:
        for kind in ("real", "original", "gui"):
            with self.subTest(kind=kind):
                protocol = channel()
                offer = protocol.worker_send_offer("offer-11")
                ack = protocol.supervisor_receive_offer(offer)
                receipt = protocol.worker_receive_ack(ack)
                protocol.supervisor_receive_receipt(receipt)
                with self.assertRaisesRegex(ProtocolError, "hard HOLD"):
                    protocol.external_entry(kind)
                self.assertFalse(protocol.can_resume)


if __name__ == "__main__":
    unittest.main()
