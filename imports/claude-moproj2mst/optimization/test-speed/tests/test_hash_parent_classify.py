"""Classification reuse inherits the accepted engine's disposable-fixture checks."""
from pathlib import Path
import unittest
from unittest.mock import patch
import test_hash_parent_cache as accepted
import fixture_hash_classify_candidate as fast


class ParentClassifyTests(accepted.ParentCacheTests):
    def setUp(self):
        super().setUp()
        swap = patch.object(accepted, "fast", fast)
        swap.start()
        self.addCleanup(swap.stop)

    def cached(self, roots, workers=2):
        return fast.snapshot(roots, evidence=self.evidence, workers=workers,
                             path_engine="parent-cache-classify")

    def test_lstat_denial_on_visible_file_aborts(self):
        original = Path.lstat
        target = self.tree / "small"
        def denied(path, *args, **kwargs):
            if path == target:
                raise PermissionError("classification denied")
            return original(path, *args, **kwargs)
        for engine in ("parent-cache", "parent-cache-classify"):
            with patch.object(Path, "lstat", denied), self.assertRaisesRegex(PermissionError, "classification denied"):
                fast.snapshot({self.tree}, evidence=self.evidence, path_engine=engine)

    def test_missing_entry_filter_matches_accepted(self):
        original_lstat, original_is_file = Path.lstat, Path.is_file
        target = self.tree / "small"
        def vanished(path, *args, **kwargs):
            if path == target:
                raise FileNotFoundError("vanished")
            return original_lstat(path, *args, **kwargs)
        def filtered(path):
            return False if path == target else original_is_file(path)
        with patch.object(Path, "lstat", vanished), patch.object(Path, "is_file", filtered):
            expected = fast.snapshot({self.tree}, evidence=self.evidence, path_engine="parent-cache")
            self.assertEqual(self.cached({self.tree}), expected)

    def test_is_file_error_on_uncertain_entry_aborts(self):
        original = Path.is_file
        def denied(path):
            if path == self.tree / "small":
                raise PermissionError("classification fallback denied")
            return original(path)
        # A symlink-mode proxy forces the same is_file fallback without link privilege.
        original_lstat = Path.lstat
        class LinkMetadata:
            st_mode = 0o120777
        with patch.object(Path, "lstat", lambda p: LinkMetadata() if p == self.tree / "small" else original_lstat(p)), patch.object(Path, "is_file", denied):
            with self.assertRaisesRegex(PermissionError, "classification fallback denied"):
                self.cached({self.tree})


if __name__ == "__main__":
    unittest.main()
