import json
import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp311_frame_protocol as p


NONCE = "a" * 64
HANDLE = "18446744073709551615"


def raw(obj):
    b = json.dumps(obj, separators=(",", ":")).encode()
    return struct.pack("<I", len(b)) + b


class FrameTests(unittest.TestCase):
    def test_round_trip_concatenated(self):
        values = [{"x": 1}, {"y": "λ"}]
        encoded = p.encode_frame(values[0]) + p.encode_frame(values[1])
        self.assertEqual(p.decode_frames(encoded), values)

    def test_duplicate_keys_and_invalid_json(self):
        for payload in (b'{"a":1,"a":2}', b'{"outer":{"x":1,"x":2}}',
                        b'{bad}', b'\xff', b'{"s":"\\ud800"}', b'{"n":1e999}'):
            with self.subTest(payload=payload), self.assertRaises(p.FrameError):
                p.decode_frames(struct.pack("<I", len(payload)) + payload)

    def test_lengths_truncation_and_total(self):
        for data in (b"\x01", struct.pack("<I", 5) + b"{}",
                     struct.pack("<I", 0), struct.pack("<I", p.MAX_FRAME + 1)):
            with self.subTest(data=data), self.assertRaises(p.FrameError):
                p.decode_frames(data)
        with self.assertRaises(p.FrameError):
            p.decode_frames(b" " * (p.MAX_TOTAL + 1))

    def test_extra_trailing_bytes_are_not_ignored(self):
        with self.assertRaises(p.FrameError):
            p.decode_frames(p.encode_frame({"ok": True}) + b"x")
        protocol = p.FrameProtocol(NONCE, "1")
        offer = protocol.offer()
        with self.assertRaises(p.FrameError):
            protocol.accept_offer(offer + offer)
        self.assertEqual(protocol.phase, p.Phase.HOLD)

    def test_pointer_width_decimal(self):
        self.assertEqual(p.handle_value("4294967295", 32), 4294967295)
        for value in ("0", "01", "-1", "4294967296", 1, "1e3"):
            with self.subTest(value=value), self.assertRaises(p.FrameError):
                p.handle_value(value, 32)
        with self.assertRaises(p.FrameError):
            p.handle_value("9" * 5000, 64)

    def test_nonce_fixed_256_bits(self):
        for nonce in ("a" * 62, "A" * 64, "g" * 64, 3):
            with self.subTest(nonce=nonce), self.assertRaises(p.ProtocolError):
                p.FrameProtocol(nonce, "1")

    def test_ordered_handshake_and_no_resume_authority(self):
        protocol = p.FrameProtocol(NONCE, HANDLE)
        offer = protocol.offer()
        ack = protocol.accept_offer(offer)
        receipt = protocol.accept_ack(ack)
        protocol.accept_receipt(receipt)
        self.assertEqual(protocol.phase, p.Phase.COMPLETE)
        self.assertFalse(protocol.can_resume)

    def test_state_rejects_reordered_or_extra_fields(self):
        protocol = p.FrameProtocol(NONCE, "1")
        with self.assertRaises(p.ProtocolError):
            protocol.accept_ack(raw({"kind": "ACK", "occurrence": NONCE, "handle": "1"}))
        self.assertEqual(protocol.phase, p.Phase.HOLD)
        protocol = p.FrameProtocol(NONCE, "1")
        offer = protocol.offer()
        decoded = p.decode_frames(offer)[0]
        decoded["unexpected"] = 1
        with self.assertRaises(p.ProtocolError):
            protocol.accept_offer(raw(decoded))

    def test_external_entry_hard_hold(self):
        protocol = p.FrameProtocol(NONCE, "1")
        with self.assertRaisesRegex(p.ProtocolError, "HARD HOLD"):
            protocol.external_entry("real")
        self.assertEqual(protocol.phase, p.Phase.HOLD)
        self.assertFalse(protocol.can_resume)

    def test_decode_failure_is_sticky_hold_at_each_receive_stage(self):
        def at_offer(protocol):
            protocol.offer()
            return protocol.accept_offer, raw({"kind": "OFFER", "occurrence": NONCE, "handle": "1"})

        def at_ack(protocol):
            offer = protocol.offer()
            ack = protocol.accept_offer(offer)
            return protocol.accept_ack, ack

        def at_receipt(protocol):
            offer = protocol.offer()
            ack = protocol.accept_offer(offer)
            receipt = protocol.accept_ack(ack)
            return protocol.accept_receipt, receipt

        for prepare in (at_offer, at_ack, at_receipt):
            protocol = p.FrameProtocol(NONCE, "1")
            receiver, valid = prepare(protocol)
            with self.subTest(phase=protocol.phase):
                with self.assertRaises(p.FrameError):
                    receiver(b"\x08\x00")
                self.assertEqual(protocol.phase, p.Phase.HOLD)
                with self.assertRaises(p.ProtocolError):
                    receiver(valid)
                self.assertEqual(protocol.phase, p.Phase.HOLD)

    def test_nonfinite_and_surrogate_decode_failures_are_sticky(self):
        for payload in (b'{"kind":"OFFER","occurrence":"' + NONCE.encode() +
                        b'","handle":"1","nested":["\\udfff"]}',
                        b'{"kind":"OFFER","occurrence":"' + NONCE.encode() +
                        b'","handle":"1","x":1e999}'):
            protocol = p.FrameProtocol(NONCE, "1")
            protocol.offer()
            frame = struct.pack("<I", len(payload)) + payload
            with self.assertRaises(p.FrameError):
                protocol.accept_offer(frame)
            self.assertEqual(protocol.phase, p.Phase.HOLD)


if __name__ == "__main__":
    unittest.main()
