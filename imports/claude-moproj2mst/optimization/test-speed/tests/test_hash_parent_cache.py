"""Opt-in canonical-directory cache checks; all files are disposable."""
import json
import os
from pathlib import Path
import subprocess
import stat
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parents[3] / "admin"))
import fixture_hash_fast as fast


class ParentCacheTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(dir=Path(__file__).parent)
        self.addCleanup(self.temp.cleanup)
        self.base = Path(self.temp.name).resolve()
        self.evidence = self.base / "evidence"
        self.tree = self.evidence / "tree"
        self.tree.mkdir(parents=True)
        (self.tree / "small").write_bytes(b"data")
        (self.tree / "empty").write_bytes(b"")
        (self.tree / "large").write_bytes(b"abc" * 400000)

    def cached(self, roots, workers=2):
        return fast.snapshot(roots, evidence=self.evidence, workers=workers, path_engine="parent-cache")

    def test_nested_missing_and_direct_file_roots(self):
        roots = {self.tree, self.tree / "small", self.evidence / "missing"}
        expected = fast.snapshot(roots, evidence=self.evidence)
        for workers in (1, 2, 4):
            self.assertEqual(json.dumps(self.cached(roots, workers)), json.dumps(expected))
        self.assertEqual(self.cached({self.evidence / "missing"}), {})

    def test_unknown_windows_attributes_fall_back_to_exact(self):
        if os.name != "nt":
            self.skipTest("Windows metadata classification")
        original = Path.lstat
        class Metadata:
            def __init__(self, source):
                self.st_mode = source.st_mode
        with patch.object(Path, "lstat", lambda p: Metadata(original(p))), patch.object(fast, "_child", wraps=fast._child) as checked:
            self.cached({self.tree})
            self.assertEqual(checked.call_count, 3)

    def test_actual_file_symlink_outside_is_rejected(self):
        outside = self.base / "outside"
        outside.write_bytes(b"outside")
        try:
            (self.tree / "link").symlink_to(outside)
        except OSError as exc:
            self.skipTest(str(exc))
        with self.assertRaises(ValueError):
            self.cached({self.tree})

    def test_junction_parent_inside_and_outside(self):
        if os.name != "nt":
            self.skipTest("Windows junctions")
        for outside in (False, True):
            target = (self.base if outside else self.evidence) / ("outside-dir" if outside else "inside-dir")
            target.mkdir()
            (target / "sample").write_bytes(b"target")
            link = self.evidence / ("outside-junction" if outside else "inside-junction")
            result = subprocess.run(["cmd", "/c", "mklink", "/J", str(link), str(target)], capture_output=True, text=True)
            if result.returncode:
                self.skipTest("Junction creation unavailable: " + result.stderr)
            try:
                if outside:
                    for engine in ("exact", "parent-cache"):
                        with self.assertRaises(ValueError):
                            fast.snapshot({link}, evidence=self.evidence, path_engine=engine)
                else:
                    self.assertEqual(json.dumps(self.cached({link})), json.dumps(fast.snapshot({link}, evidence=self.evidence)))
            finally:
                os.rmdir(link)  # Remove only the owned junction itself, never its target.

    def test_bad_engine_rejected(self):
        with self.assertRaises(ValueError):
            fast.snapshot({self.tree}, evidence=self.evidence, path_engine="unknown")

    def test_reparse_flag_forces_full_resolution(self):
        original = Path.lstat
        class Metadata:
            def __init__(self, source):
                self.st_mode = source.st_mode
                self.st_file_attributes = stat.FILE_ATTRIBUTE_REPARSE_POINT
        with patch.object(Path, "lstat", lambda p: Metadata(original(p))), patch.object(fast, "_child", wraps=fast._child) as checked:
            self.cached({self.tree})
            self.assertEqual(checked.call_count, 3)

    def test_same_size_mtime_changes_still_hashed(self):
        before = self.cached({self.tree})
        path = self.tree / "small"
        prior = path.stat()
        path.write_bytes(b"edit")
        os.utime(path, ns=(prior.st_atime_ns, prior.st_mtime_ns))
        after = self.cached({self.tree})
        key = str(path.relative_to(self.evidence))
        self.assertEqual(before[key][:2], after[key][:2])
        self.assertNotEqual(before[key][2], after[key][2])


if __name__ == "__main__":
    unittest.main()
