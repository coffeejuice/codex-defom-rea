import hashlib
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import cp418_runner_marker as marker


class MarkerTests(unittest.TestCase):
    def test_fixed_path_derivation_and_identity_validation(self):
        nonce, deadline = b"a" * 32, 123
        digest = hashlib.sha256(marker._DOMAIN + nonce + deadline.to_bytes(8, "big")).hexdigest()
        expected = Path(marker.__file__).resolve().parent.parent / "analysis" / f"cp418-runner-{digest}.txt"
        self.assertEqual(marker.marker_path(nonce, deadline), expected)
        self.assertEqual(expected.parent.name, "analysis")
        for bad_nonce, bad_deadline in ((b"x", 1), (nonce, 0), (nonce, True), (nonce, 2**64)):
            with self.subTest(bad_nonce=bad_nonce, bad_deadline=bad_deadline), self.assertRaises(ValueError):
                marker.marker_path(bad_nonce, bad_deadline)

    def test_exclusive_create_and_ordered_append(self):
        nonce, deadline = b"b" * 32, 456
        with tempfile.TemporaryDirectory() as scratch:
            path = Path(scratch) / "fixed-test-marker.txt"
            with patch.object(marker, "marker_path", return_value=path):
                self.assertEqual(marker.read_stage(nonce, deadline), "absent")
                self.assertTrue(marker.write_stage(nonce, deadline, "entered"))
                self.assertEqual(path.read_bytes(), b"entered\n")
                self.assertFalse(marker.write_stage(nonce, deadline, "entered"))
                self.assertTrue(marker.write_stage(nonce, deadline, "returned"))
                self.assertEqual(path.read_bytes(), b"entered\nreturned\n")
                self.assertFalse(marker.write_stage(nonce, deadline, "returned"))
                self.assertEqual(marker.read_stage(nonce, deadline), "returned")

    def test_bounded_malformed_and_unavailable(self):
        nonce, deadline = b"c" * 32, 789
        with tempfile.TemporaryDirectory() as scratch:
            path = Path(scratch) / "fixed-test-marker.txt"
            path.write_bytes(b"x" * 128)
            with patch.object(marker, "marker_path", return_value=path):
                self.assertEqual(marker.read_stage(nonce, deadline), "malformed")
            with patch.object(Path, "open", side_effect=PermissionError):
                self.assertEqual(marker.read_stage(nonce, deadline), "unavailable")

    def test_rejects_stage_outside_fixed_api(self):
        self.assertFalse(marker.write_stage(b"d" * 32, 100, "../arbitrary"))


if __name__ == "__main__":
    unittest.main()
