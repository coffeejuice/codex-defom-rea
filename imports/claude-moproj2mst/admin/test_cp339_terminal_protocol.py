"""Inert CP339 frame and runner-receipt tests."""
from __future__ import annotations

import dataclasses
from types import SimpleNamespace
import unittest

import cp339_terminal_protocol as terminal


NONCE = bytes(range(1, 33))
DEADLINE = 1234567


def successful_receipts():
    run = SimpleNamespace(
        deadline=DEADLINE, phase="closed-success",
        resume_outcome="resumed-once-previous-count-one",
        init_outcome="48-bytes-written",
        result_outcome="80-byte-zero-child-result",
        peek_outcome="zero-queued-byte-snapshot",
        ack_outcome="exact-worker-exit-zero-observed", errors=[])
    worker = SimpleNamespace(phase="closed", worker_waited=True,
                             jobs_empty=True, worker_terminated=False, errors=[])
    return run, worker


class TerminalProtocolTests(unittest.TestCase):
    def test_exact_roundtrip(self):
        value = terminal.TerminalSuccess(DEADLINE, NONCE)
        raw = terminal.encode_terminal(value)
        self.assertEqual(len(raw), 80)
        self.assertEqual(terminal.decode_terminal(raw, deadline=DEADLINE, nonce=NONCE), value)
        self.assertEqual(raw, terminal.encode_terminal(value))

    def test_bad_lengths_and_types(self):
        raw = terminal.encode_terminal(terminal.TerminalSuccess(DEADLINE, NONCE))
        for candidate in (raw[:-1], raw + b"x", bytearray(raw), memoryview(raw), None):
            with self.subTest(candidate=type(candidate).__name__, length=len(candidate) if candidate is not None else 0):
                with self.assertRaises(terminal.TerminalError):
                    terminal.decode_terminal(candidate, deadline=DEADLINE, nonce=NONCE)

    def test_all_bytes_bound_to_checksum_or_occurrence(self):
        raw = terminal.encode_terminal(terminal.TerminalSuccess(DEADLINE, NONCE))
        for index in range(80):
            mutated = bytearray(raw)
            mutated[index] ^= 1
            with self.subTest(index=index), self.assertRaises(terminal.TerminalError):
                terminal.decode_terminal(bytes(mutated), deadline=DEADLINE, nonce=NONCE)

    def test_wrong_occurrence_rejected(self):
        raw = terminal.encode_terminal(terminal.TerminalSuccess(DEADLINE, NONCE))
        with self.assertRaises(terminal.TerminalError):
            terminal.decode_terminal(raw, deadline=DEADLINE + 1, nonce=NONCE)
        with self.assertRaises(terminal.TerminalError):
            terminal.decode_terminal(raw, deadline=DEADLINE, nonce=b"z" * 32)

    def test_unknown_flags_even_with_recomputed_digest_rejected(self):
        import hashlib
        raw = bytearray(terminal.encode_terminal(terminal.TerminalSuccess(DEADLINE, NONCE)))
        raw[16] ^= 0x40
        raw[56:] = hashlib.sha256(raw[:56]).digest()[:24]
        with self.assertRaises(terminal.TerminalError):
            terminal.decode_terminal(bytes(raw), deadline=DEADLINE, nonce=NONCE)

    def test_reserved_even_with_recomputed_digest_rejected(self):
        import hashlib
        raw = bytearray(terminal.encode_terminal(terminal.TerminalSuccess(DEADLINE, NONCE)))
        raw[20] = 1
        raw[56:] = hashlib.sha256(raw[:56]).digest()[:24]
        with self.assertRaises(terminal.TerminalError):
            terminal.decode_terminal(bytes(raw), deadline=DEADLINE, nonce=NONCE)

    def test_source_receipts_require_every_checkpoint(self):
        run, worker = successful_receipts()
        value = terminal.from_runner(run, worker, deadline=DEADLINE, nonce=NONCE)
        self.assertEqual(value, terminal.TerminalSuccess(DEADLINE, NONCE))
        for name in ("phase", "resume_outcome", "init_outcome", "result_outcome", "peek_outcome", "ack_outcome", "deadline"):
            original = getattr(run, name)
            setattr(run, name, "not-confirmed")
            with self.subTest(name=name), self.assertRaises(terminal.TerminalError):
                terminal.from_runner(run, worker, deadline=DEADLINE, nonce=NONCE)
            setattr(run, name, original)
        for name in ("phase", "worker_waited", "jobs_empty", "worker_terminated"):
            original = getattr(worker, name)
            setattr(worker, name, not original if type(original) is bool else False)
            with self.subTest(name=name), self.assertRaises(terminal.TerminalError):
                terminal.from_runner(run, worker, deadline=DEADLINE, nonce=NONCE)
            setattr(worker, name, original)
        for receipt in (run, worker):
            receipt.errors.append("unresolved")
            with self.assertRaises(terminal.TerminalError):
                terminal.from_runner(run, worker, deadline=DEADLINE, nonce=NONCE)
            receipt.errors.clear()

    def test_strict_field_types_and_inert_import(self):
        self.assertEqual(terminal.NATIVE_ENTRY, "HARD HOLD")
        for deadline in (0, -1, 2**64, True, 1.0):
            with self.assertRaises(terminal.TerminalError):
                terminal.TerminalSuccess(deadline, NONCE)
        for nonce in (b"", b"\0" * 32, bytearray(NONCE), bytes(range(31))):
            with self.assertRaises(terminal.TerminalError):
                terminal.TerminalSuccess(DEADLINE, nonce)
        with self.assertRaises(dataclasses.FrozenInstanceError):
            terminal.TerminalSuccess(DEADLINE, NONCE).deadline = 1


if __name__ == "__main__":
    unittest.main()
