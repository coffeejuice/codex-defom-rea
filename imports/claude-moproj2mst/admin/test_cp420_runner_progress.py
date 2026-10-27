"""Filesystem contract tests for CP420 occurrence progress records."""
import os
import json
import unittest
from pathlib import Path
import tempfile
from unittest.mock import patch

import cp420_runner_progress as progress


class ProgressTests(unittest.TestCase):
    def test_cp432_failure_receipt_is_separate_bounded_and_one_use(self):
        nonce, deadline = b"q" * 32, 900
        self.assertRegex(progress.failure_path(nonce, deadline).name,
                         r"^cp432-failure-[0-9a-f]{64}\.json$")
        self.assertNotEqual(progress.failure_path(nonce, deadline),
                            progress.path(nonce, deadline))
        with tempfile.TemporaryDirectory(dir=os.environ.get("CP420_TEST_TEMP") or None) as directory:
            target = Path(directory) / "failure.json"
            snapshot = {"runner": {"ack_memory_stage": "returned"},
                        "worker": {"wait_outcome": "returned"}}
            with patch.object(progress, "failure_path", return_value=target), \
                 patch.object(progress.os, "fsync") as fsync:
                self.assertTrue(progress.write_failure_completion(nonce, deadline, snapshot))
                self.assertEqual(json.loads(target.read_text(encoding="ascii")),
                                 {"schema": 1, "snapshot": snapshot})
                self.assertFalse(progress.write_failure_completion(nonce, deadline, snapshot))
                self.assertFalse(progress.write_failure_completion(
                    nonce, deadline, {"runner": {"x": "a" * 1200}, "worker": {}}))
                fsync.assert_not_called()

    def test_deterministic_identity_and_full_order(self):
        nonce, deadline = b"n" * 32, 100
        derived = progress.path(nonce, deadline)
        self.assertEqual(derived.parent.name, "analysis")
        self.assertRegex(derived.name, r"^cp429-runner-[0-9a-f]{64}\.txt$")
        with tempfile.TemporaryDirectory(dir=os.environ.get("CP420_TEST_TEMP") or None) as directory:
            target = Path(directory) / "cp420-test-record.txt"
            with patch.object(progress, "path", return_value=target):
                self.assertEqual(progress.path(nonce, deadline), target)
                self.assertEqual(progress.read_progress(nonce, deadline), "absent")
                for stage in progress._STAGES:
                    if stage == "worker_ack":
                        self.assertTrue(progress.write_progress(
                            nonce, deadline, stage, remaining=1234))
                    else:
                        self.assertTrue(progress.write_progress(nonce, deadline, stage))
                    self.assertEqual(progress.read_progress(nonce, deadline), stage)
                self.assertEqual(progress.read_ack_remaining(nonce, deadline), 1234)
                self.assertIn(b"worker_ack:1234\n", target.read_bytes())
                self.assertFalse(progress.write_progress(nonce, deadline, "bootstrap"))

    def test_post_ack_stages_flush_without_fsync(self):
        nonce, deadline = b"z" * 32, 103
        with tempfile.TemporaryDirectory(dir=os.environ.get("CP420_TEST_TEMP") or None) as directory:
            target = Path(directory) / "cp429-test-record.txt"
            with patch.object(progress, "path", return_value=target), \
                 patch.object(progress.os, "fsync") as fsync:
                for index, stage in enumerate(progress._STAGES):
                    kwargs = {"remaining": 1234} if stage == "worker_ack" else {}
                    self.assertTrue(progress.write_progress(nonce, deadline, stage, **kwargs))
                    self.assertEqual(progress.read_progress(nonce, deadline), stage)
                    self.assertEqual(fsync.call_count, min(index + 1, 4))

    def test_requires_exact_prefix_and_classifies_bad_states(self):
        nonce, deadline = b"x" * 32, 101
        with tempfile.TemporaryDirectory(dir=os.environ.get("CP420_TEST_TEMP") or None) as directory:
            target = Path(directory) / "cp420-test-bad-record.txt"
            with patch.object(progress, "path", return_value=target):
                self.assertFalse(progress.write_progress(nonce, deadline, "parent_ack"))
                target.write_bytes(b"bootstrap\nworker_resumed\n")
                self.assertEqual(progress.read_progress(nonce, deadline), "malformed")
                self.assertFalse(progress.write_progress(nonce, deadline, "result"))
                target.write_bytes(b"z" * 1000)
                self.assertEqual(progress.read_progress(nonce, deadline), "malformed")
                self.assertIsNone(progress.read_ack_remaining(nonce, deadline))

    def test_ack_remaining_requires_canonical_bounded_integer(self):
        nonce, deadline = b"y" * 32, 102
        with tempfile.TemporaryDirectory(dir=os.environ.get("CP420_TEST_TEMP") or None) as directory:
            target = Path(directory) / "cp420-test-ack.txt"
            with patch.object(progress, "path", return_value=target):
                for stage in progress._STAGES[:4]:
                    self.assertTrue(progress.write_progress(nonce, deadline, stage))
                for remaining in (None, 0, True, 5001, -1, 1.5):
                    self.assertFalse(progress.write_progress(
                        nonce, deadline, "worker_ack", remaining=remaining))
                self.assertTrue(progress.write_progress(
                    nonce, deadline, "worker_ack", remaining=5000))
                self.assertEqual(progress.read_ack_remaining(nonce, deadline), 5000)
                target.write_bytes(b"bootstrap\nparent_ack\nworker_resumed\nresult\nworker_ack:0500\n")
                self.assertEqual(progress.read_progress(nonce, deadline), "malformed")

    def test_invalid_identity_and_unavailable(self):
        for nonce, deadline in ((b"short", 10), (b"n" * 32, 0), (b"n" * 32, True)):
            with self.assertRaises(ValueError):
                progress.path(nonce, deadline)
        with patch.object(progress, "path", side_effect=PermissionError):
            self.assertEqual(progress.read_progress(b"n" * 32, 100), "unavailable")


if __name__ == "__main__":
    unittest.main()
