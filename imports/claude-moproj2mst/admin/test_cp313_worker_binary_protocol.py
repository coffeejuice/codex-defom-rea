"""Static, in-memory tests for the inert CP312 v2 Python bridge."""

import importlib.util
from pathlib import Path
import struct
import unittest


SPEC = importlib.util.spec_from_file_location(
    "cp313_worker_binary_protocol", Path(__file__).with_name("cp313_worker_binary_protocol.py"))
bridge = importlib.util.module_from_spec(SPEC)
import sys
sys.modules[SPEC.name] = bridge
SPEC.loader.exec_module(bridge)

NONCE = bytes(range(32))


class MemoryTransport:
    def __init__(self, incoming=b"", chunk=7, fail_at=None, failure=None):
        self.incoming = incoming
        self.outgoing = bytearray()
        self.chunk = chunk
        self.fail_at = fail_at
        self.failure = failure
        self.calls = 0

    def _failure(self):
        self.calls += 1
        if self.calls == self.fail_at:
            raise self.failure

    def read(self, size):
        self._failure()
        take = min(size, self.chunk)
        result, self.incoming = self.incoming[:take], self.incoming[take:]
        return result

    def write(self, data):
        self._failure()
        part = data[:self.chunk]
        self.outgoing.extend(part)
        return len(part)


class ProtocolTests(unittest.TestCase):
    def setUp(self):
        self.result = bridge.WorkerTelemetry(1, 1, 1234, 222, 333, 1, 5678, 1,
                                             0xABCDEF0123456789, NONCE)

    def test_exact_layout_round_trips_and_raw_errors(self):
        init = bridge.Init(0xFEDCBA9876543210, NONCE)
        ack = bridge.Ack(self.result.child_process_handle, NONCE)
        self.assertEqual((bridge.INIT.size, bridge.RESULT.size, bridge.ACK.size), (48, 80, 48))
        self.assertEqual(bridge.encode_init(init), struct.pack("<IIQ32s", bridge.MAGIC, 2,
                                                               init.child_job, NONCE))
        self.assertEqual(bridge.decode_init(bridge.encode_init(init)), init)
        self.assertEqual(bridge.decode_result(bridge.encode_result(self.result)), self.result)
        self.assertEqual(bridge.decode_ack(bridge.encode_ack(ack)), ack)
        self.assertEqual(self.result.create_error, 1234)
        self.assertEqual(self.result.child_query_error, 5678)
        self.assertTrue(bridge.result_matches_init(self.result, init))
        self.assertFalse(bridge.result_matches_init(
            self.result, bridge.Init(init.child_job, b"z" * 32)))
        self.assertTrue(bridge.ack_matches(ack, self.result))
        self.assertFalse(bridge.ack_matches(bridge.Ack(0, NONCE), self.result))
        self.assertFalse(bridge.ack_matches(bridge.Ack(ack.child_process_handle, b"z" * 32),
                                            self.result))
        self.assertEqual(bridge.NATIVE_ENTRY, "HARD HOLD")

    def test_strict_frame_sizes_magic_version_and_nonce(self):
        for decoder, valid in ((bridge.decode_init, bridge.encode_init(bridge.Init(1, NONCE))),
                               (bridge.decode_result, bridge.encode_result(self.result)),
                               (bridge.decode_ack, bridge.encode_ack(bridge.Ack(1, NONCE)))):
            for bad in (valid[:-1], valid + b"x", b"\0\0\0\0" + valid[4:],
                        valid[:4] + struct.pack("<I", 1) + valid[8:]):
                with self.assertRaises(bridge.ProtocolError):
                    decoder(bad)
        for nonce in (b"x" * 31, b"x" * 33, "x" * 32, bytearray(32)):
            with self.assertRaises(bridge.ProtocolError):
                bridge.Init(1, nonce)

    def test_handle_bool_and_identity_consistency(self):
        for handle in (0, -1, 1 << 64, True):
            with self.assertRaises(bridge.ProtocolError):
                bridge.Init(handle, NONCE)
        for handle in (-1, 1 << 64, True):
            with self.assertRaises(bridge.ProtocolError):
                bridge.Ack(handle, NONCE)
        for handle in (-1, 1 << 64, True):
            with self.assertRaises(bridge.ProtocolError):
                bridge.WorkerTelemetry(1, 1, 0, 4, 5, 0, 0, 0, handle, NONCE)
        raw = bytearray(bridge.encode_result(self.result))
        for offset, value in ((8, 2), (12, 2), (28, 2), (36, 2)):
            changed = raw.copy()
            struct.pack_into("<I", changed, offset, value)
            with self.assertRaises(bridge.ProtocolError):
                bridge.decode_result(bytes(changed))
        for result in (
            bridge.WorkerTelemetry(1, 0, 5, 0, 0, 0, 0, 0, 0, NONCE),
            bridge.WorkerTelemetry(0, 0, 0, 0, 0, 0, 0, 0, 0, NONCE),
            bridge.WorkerTelemetry(1, 1, 9, 10, 11, 0, 12, 0, 13, NONCE),
        ):
            self.assertEqual(bridge.decode_result(bridge.encode_result(result)), result)
        for fields in ((1, 0, 5, 4, 0, 0, 0, 0, 0, NONCE),
                       (1, 1, 0, 4, 0, 0, 0, 0, 1, NONCE),
                       (0, 0, 1, 0, 0, 0, 0, 0, 0, NONCE),
                       (1, 1, 0, 4, 5, 0, 0, 1, 1, NONCE),
                       (1, 0, 5, 0, 0, 1, 0, 0, 0, NONCE),
                       (1, 1, 0, 4, 5, 0, 0, 0, 0, NONCE)):
            with self.assertRaises(bridge.ProtocolError):
                bridge.WorkerTelemetry(*fields)

    def test_bounded_fragmented_io_and_uncertain_outcomes(self):
        frame = bridge.encode_result(self.result)
        reader = MemoryTransport(frame, chunk=3)
        self.assertEqual(bridge.read_exact(reader, 80), frame)
        self.assertEqual(reader.incoming, b"")
        trailing = MemoryTransport(frame + b"trailing")
        self.assertEqual(bridge.read_exact(trailing, 80), frame)
        self.assertEqual(trailing.incoming, b"trailing")
        writer = MemoryTransport(chunk=3)
        bridge.write_exact(writer, frame)
        self.assertEqual(bytes(writer.outgoing), frame)
        for incoming in (b"", frame[:17], frame[:-1]):
            with self.assertRaises(bridge.UncertainTransfer):
                bridge.read_exact(MemoryTransport(incoming), 80)
        for failure in (TimeoutError(), OSError("broken pipe")):
            with self.assertRaises(bridge.UncertainTransfer):
                bridge.read_exact(MemoryTransport(frame, fail_at=2, failure=failure), 80)
            with self.assertRaises(bridge.UncertainTransfer):
                bridge.write_exact(MemoryTransport(fail_at=2, failure=failure), frame)
        with self.assertRaises(bridge.UncertainTransfer):
            bridge.write_exact(MemoryTransport(chunk=0), frame)
        class OversizedRead:
            def read(self, size):
                return b"x" * (size + 1)

        with self.assertRaises(bridge.UncertainTransfer):
            bridge.read_exact(OversizedRead(), 80)
        with self.assertRaises(bridge.ProtocolError):
            bridge.read_exact(MemoryTransport(), 81)
        with self.assertRaises(bridge.ProtocolError):
            bridge.write_exact(MemoryTransport(), frame + b"x")


if __name__ == "__main__":
    unittest.main()
