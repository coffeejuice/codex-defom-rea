"""Canonical-parent optimization compatibility checks on disposable fixtures."""
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parents[3] / "admin"))
import fixture_hash_fast as fast
import run_converter_tests as serial


class ParentHoistTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(dir=Path(__file__).parent)
        self.addCleanup(self.temp.cleanup)
        self.base = Path(self.temp.name).resolve()
        self.evidence = self.base / "evidence"
        self.nested = self.evidence / "tree/nested"
        self.nested.mkdir(parents=True)
        (self.nested / "a").write_bytes(b"abc")
        (self.evidence / "tree/b").write_bytes(b"def")

    def test_nested_duplicate_and_missing_roots_keep_mapping_order(self):
        roots = {self.nested.parent, self.nested, self.evidence / "missing"}
        with patch.object(serial, "EVIDENCE", self.evidence):
            expected = json.dumps(serial.snapshot(roots))
        for workers in (1, 2, 4):
            self.assertEqual(json.dumps(fast.snapshot(roots, evidence=self.evidence, workers=workers)), expected)

    def test_missing_only_roots_return_empty(self):
        for workers in (1, 2, 4):
            self.assertEqual(fast.snapshot({self.evidence / "missing"}, evidence=self.evidence, workers=workers), {})

    def test_outside_symlink_is_rejected(self):
        outside = self.base / "outside"
        outside.write_bytes(b"outside")
        link = self.nested / "escape"
        try:
            link.symlink_to(outside)
        except OSError as exc:
            self.skipTest(f"OS does not permit creating symbolic links: {exc}")
        for workers in (1, 2, 4):
            with self.assertRaises(ValueError):
                fast.snapshot({self.nested}, evidence=self.evidence, workers=workers)
        with patch.object(serial, "EVIDENCE", self.evidence), self.assertRaises(ValueError):
            serial.snapshot({self.nested})

    def test_parent_resolved_once_each_file_still_resolved(self):
        original = Path.resolve
        calls = []
        def counted(path, *args, **kwargs):
            calls.append(path)
            return original(path, *args, **kwargs)
        with patch.object(Path, "resolve", counted):
            fast.snapshot({self.nested.parent}, evidence=self.evidence, workers=2)
        self.assertEqual(calls.count(self.evidence), 1)
        self.assertEqual(set(calls), {self.evidence, self.nested / "a", self.nested.parent / "b"})

    def test_resolved_outside_target_rejected_without_symlink_privilege(self):
        original = Path.resolve
        escaped = self.nested / "a"
        def redirected(path, *args, **kwargs):
            return self.base / "outside" if path == escaped else original(path, *args, **kwargs)
        for workers in (1, 2, 4):
            with patch.object(Path, "resolve", redirected), self.assertRaises(ValueError):
                fast.snapshot({self.nested}, evidence=self.evidence, workers=workers)


if __name__ == "__main__":
    unittest.main()
