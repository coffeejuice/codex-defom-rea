"""Synthetic launcher/hash checks. Does not invoke CTest or read real fixtures."""
import contextlib
import importlib.util
import io
import json
import os
from pathlib import Path
import stat as stat_module
import subprocess
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch

PROJECT = Path(__file__).resolve().parents[3]
ADMIN = PROJECT / "admin"
sys.path.insert(0, str(ADMIN))
import fixture_hash_fast as fast

spec = importlib.util.spec_from_file_location("converter_runner", ADMIN / "run_converter_tests.py")
runner = importlib.util.module_from_spec(spec)
spec.loader.exec_module(runner)


class SnapshotTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(dir=Path(__file__).resolve().parent)
        self.addCleanup(self.temp.cleanup)
        self.base = Path(self.temp.name).resolve()
        self.evidence = self.base / "evidence"
        self.evidence.mkdir()
        self.tree = self.evidence / "tree"
        (self.tree / "nested").mkdir(parents=True)
        (self.tree / "empty").write_bytes(b"")
        self.file = self.tree / "nested/data"
        self.file.write_bytes(bytes(range(256)) * 8193)
        (self.tree / "small").write_bytes(b"content")
        self.roots = {self.tree, self.file.parent, self.file, self.evidence / "missing"}
        self.evidence_patch = patch.object(runner, "EVIDENCE", self.evidence)
        self.evidence_patch.start()
        self.addCleanup(self.evidence_patch.stop)

    def test_mapping_and_serialized_order(self):
        expected = runner.snapshot(self.roots)
        for path_engine in ("exact", "parent-cache"):
            for workers in (1, 2, 4, 8):
                with self.subTest(workers=workers, path_engine=path_engine):
                    actual = runner.fixture_snapshot(self.roots, "threaded", workers, path_engine)
                    self.assertEqual(json.dumps(expected), json.dumps(actual))

    def test_default_uses_original_serial(self):
        with patch.object(runner, "snapshot", return_value={"serial": []}) as serial:
            self.assertEqual(runner.fixture_snapshot(self.roots), {"serial": []})
            serial.assert_called_once_with(self.roots)
        with patch.object(runner, "snapshot", return_value={}) as serial, \
             patch.object(fast, "snapshot") as threaded:
            runner.fixture_snapshot(self.roots, "serial", 8, "parent-cache")
            serial.assert_called_once_with(self.roots)
            threaded.assert_not_called()

    def variants(self):
        return (("serial", "exact"), ("threaded", "exact"), ("threaded", "parent-cache"))

    def test_same_size_and_mtime_mutation_is_detected(self):
        for engine, path_engine in self.variants():
            with self.subTest(engine=engine, path_engine=path_engine):
                before = runner.fixture_snapshot(self.roots, engine, path_engine=path_engine)
                stat = self.file.stat()
                original = self.file.read_bytes()
                self.file.write_bytes(bytes([original[0] ^ 255]) + original[1:])
                os.utime(self.file, ns=(stat.st_atime_ns, stat.st_mtime_ns))
                after = runner.fixture_snapshot(self.roots, engine, path_engine=path_engine)
                key = str(self.file.relative_to(self.evidence))
                self.assertEqual(before[key][:2], after[key][:2])
                self.assertNotEqual(before[key][2], after[key][2])

    def test_missing_roots_match_legacy_empty_result(self):
        roots = {self.evidence / "absent"}
        self.assertEqual(runner.snapshot(roots), {})
        self.assertEqual(runner.fixture_snapshot(roots, "threaded"), {})
        self.assertEqual(runner.fixture_snapshot(roots, "threaded", path_engine="parent-cache"), {})

    def test_outside_file_rejected(self):
        outside = self.base / "outside"
        outside.write_bytes(b"not a fixture")
        for engine, path_engine in self.variants():
            with self.subTest(engine=engine, path_engine=path_engine), self.assertRaises(ValueError):
                runner.fixture_snapshot({outside}, engine, path_engine=path_engine)

    def test_read_failure_propagates_without_partial_snapshot(self):
        original_open = Path.open
        def fail(path, *args, **kwargs):
            if path == self.file:
                raise PermissionError("synthetic read denial")
            return original_open(path, *args, **kwargs)
        for engine, path_engine in self.variants():
            with self.subTest(engine=engine, path_engine=path_engine), patch.object(Path, "open", fail):
                with self.assertRaisesRegex(PermissionError, "synthetic read denial"):
                    runner.fixture_snapshot(self.roots, engine, path_engine=path_engine)

    def test_stat_failure_propagates(self):
        # Fail only the post-enumeration stat; is_file also calls stat.
        original_stat = Path.stat
        def fail(path, *args, **kwargs):
            if path == self.file and not kwargs:
                raise FileNotFoundError("synthetic vanished file")
            return original_stat(path, *args, **kwargs)
        for engine, path_engine in self.variants():
            with self.subTest(engine=engine, path_engine=path_engine), patch.object(Path, "stat", fail):
                with self.assertRaisesRegex(FileNotFoundError, "synthetic vanished file"):
                    runner.fixture_snapshot(self.roots, engine, path_engine=path_engine)

    def test_invalid_workers_and_engine(self):
        for workers in (0, 33, True, 1.5):
            with self.subTest(workers=workers), self.assertRaises(ValueError):
                fast.snapshot(self.roots, evidence=self.evidence, workers=workers)
        with self.assertRaises(ValueError):
            runner.fixture_snapshot(self.roots, "unknown")
        with self.assertRaises(ValueError):
            runner.fixture_snapshot(self.roots, "threaded", path_engine="unknown")

    def junction(self, target, name):
        if os.name != "nt":
            self.skipTest("Windows junction test")
        import _winapi
        link = self.evidence / name
        try:
            _winapi.CreateJunction(str(target), str(link))
        except OSError as exc:
            self.skipTest(f"Cannot create disposable junction: {exc}")
        # Remove the junction itself before TemporaryDirectory cleanup.
        self.addCleanup(os.rmdir, link)
        return link

    def test_internal_junction_mapping_and_order(self):
        link = self.junction(self.tree, "junction-inside")
        roots = {link, self.tree, self.file}
        expected = runner.snapshot(roots)
        for path_engine in ("exact", "parent-cache"):
            with self.subTest(path_engine=path_engine):
                actual = runner.fixture_snapshot(roots, "threaded", 2, path_engine)
                self.assertEqual(json.dumps(expected), json.dumps(actual))

    def test_outside_junction_is_rejected(self):
        target = self.base / "outside-dir"
        target.mkdir()
        (target / "file").write_bytes(b"outside")
        link = self.junction(target, "junction-outside")
        for engine, path_engine in self.variants():
            with self.subTest(engine=engine, path_engine=path_engine), self.assertRaises(ValueError):
                runner.fixture_snapshot({link}, engine, 2, path_engine)

    def test_file_symlink_resolves_and_outside_is_rejected(self):
        outside = self.base / "outside"
        outside.write_bytes(b"outside")
        inside_link = self.tree / "alias"
        outside_link = self.tree / "escape"
        try:
            inside_link.symlink_to(self.file)
            outside_link.symlink_to(outside)
        except OSError as exc:
            self.skipTest(f"File symlink privilege unavailable: {exc}")
        expected = runner.snapshot({inside_link, self.file})
        for path_engine in ("exact", "parent-cache"):
            with self.subTest(path_engine=path_engine):
                self.assertEqual(json.dumps(expected), json.dumps(runner.fixture_snapshot(
                    {inside_link, self.file}, "threaded", 2, path_engine)))
                with self.assertRaises(ValueError):
                    runner.fixture_snapshot({outside_link}, "threaded", 2, path_engine)
                # Also exercise reparse fallback after recursive enumeration.
                with self.assertRaises(ValueError):
                    runner.fixture_snapshot({self.tree}, "threaded", 2, path_engine)

    def test_reparse_and_uncertain_metadata_use_full_resolution(self):
        original_lstat = Path.lstat
        for attributes in (stat_module.FILE_ATTRIBUTE_REPARSE_POINT, None):
            def metadata(path, *args, **kwargs):
                if path == self.file:
                    values = {"st_mode": original_lstat(path).st_mode}
                    if attributes is not None:
                        values["st_file_attributes"] = attributes
                    return SimpleNamespace(**values)
                return original_lstat(path, *args, **kwargs)
            with self.subTest(attributes=attributes), patch.object(Path, "lstat", metadata), \
                 patch.object(fast, "_child", wraps=fast._child) as full:
                actual = runner.fixture_snapshot({self.tree}, "threaded", 2, "parent-cache")
                self.assertIn(str(self.file.relative_to(self.evidence)), actual)
                self.assertIn(self.file, [call.args[0] for call in full.call_args_list])

    def test_parent_cache_lstat_and_parent_resolution_failures_propagate(self):
        original_lstat, original_resolve = Path.lstat, Path.resolve
        def fail_lstat(path, *args, **kwargs):
            if path == self.file:
                raise PermissionError("synthetic lstat denial")
            return original_lstat(path, *args, **kwargs)
        with patch.object(Path, "lstat", fail_lstat):
            with self.assertRaisesRegex(PermissionError, "synthetic lstat denial"):
                runner.fixture_snapshot({self.tree}, "threaded", 2, "parent-cache")
        def fail_resolve(path, *args, **kwargs):
            if path == self.file.parent:
                raise PermissionError("synthetic parent denial")
            return original_resolve(path, *args, **kwargs)
        with patch.object(Path, "resolve", fail_resolve):
            with self.assertRaisesRegex(PermissionError, "synthetic parent denial"):
                runner.fixture_snapshot({self.tree}, "threaded", 2, "parent-cache")

    def test_cli_validation_happens_before_run_creation(self):
        for options in (("--hash-workers", "0"), ("--hash-workers", "33"),
                        ("--hash-workers", "1.5"), ("--hash-engine", "cached"),
                        ("--hash-path-engine", "cached")):
            timing = runner.LaunchTiming()
            with self.subTest(options=options), patch.object(sys, "argv", ["runner", *options]), \
                 contextlib.redirect_stderr(io.StringIO()), self.assertRaises(SystemExit) as exc:
                runner.main(timing)
            self.assertEqual(exc.exception.code, 2)
            self.assertIsNone(timing.run)

    def test_launcher_receipts_and_failed_ctest_still_hash_after(self):
        for engine, path_engine, code, mutate in (
                ("serial", "exact", 0, False), ("serial", "parent-cache", 0, False),
                ("threaded", "exact", 0, False), ("threaded", "parent-cache", 0, False),
                ("threaded", "exact", 7, False), ("threaded", "parent-cache", 7, False),
                ("threaded", "exact", 0, True), ("threaded", "parent-cache", 0, True)):
            with self.subTest(engine=engine, path_engine=path_engine, code=code, mutate=mutate):
                root = self.base / f"project-{engine}-{path_engine}-{code}-{mutate}"
                (root / "build/baseline").mkdir(parents=True)
                (root / "analysis").mkdir()
                scratch = root / "build/scratch"
                settings = {name: str(self.tree) for name in runner.FIXTURES}
                settings.update({name: str(scratch) for name in runner.OUTPUTS})
                settings.update(M2M_TEST_SCRATCH=str(scratch), M2M_FOLDER_SWEEP="1")
                inventory = {"tests": [{"name": "synthetic", "properties": [
                    {"name": "ENVIRONMENT", "value": [f"{k}={v}" for k, v in settings.items()]},
                    {"name": "WORKING_DIRECTORY", "value": str(scratch)}]}]}
                def fake_run(command, **kwargs):
                    if "--show-only=json-v1" in command:
                        return subprocess.CompletedProcess(command, 0, json.dumps(inventory))
                    junit = Path(command[command.index("--output-junit") + 1])
                    failure = '<failure message="synthetic"/>' if code else ""
                    junit.write_text(f'<testsuite><testcase name="synthetic" time="0.1">{failure}'
                                     '<system-out>SKIP historical gap</system-out></testcase></testsuite>')
                    if mutate:
                        stat = self.file.stat()
                        data = self.file.read_bytes()
                        self.file.write_bytes(bytes([data[0] ^ 255]) + data[1:])
                        os.utime(self.file, ns=(stat.st_atime_ns, stat.st_mtime_ns))
                    return subprocess.CompletedProcess(command, code)
                timing = runner.LaunchTiming()
                with patch.object(runner, "ROOT", root), patch.object(runner.subprocess, "run", fake_run), \
                     patch.object(sys, "argv", ["runner", "--build", str(root / "build/baseline"),
                                                "--hash-engine", engine,
                                                "--hash-path-engine", path_engine]), contextlib.redirect_stdout(io.StringIO()):
                    actual_code = runner.main(timing)
                timing.write(return_code=actual_code)
                receipt = json.loads((timing.run / "receipt.json").read_text())
                self.assertEqual(actual_code, 1 if mutate else code)
                self.assertEqual(receipt["fixture_hash"]["engine"], engine)
                self.assertEqual(receipt["fixture_hash"]["workers"], 1 if engine == "serial" else 4)
                self.assertEqual(receipt["fixture_hash"]["path_engine"], path_engine if engine == "threaded" else "exact")
                self.assertEqual(receipt["fixture_hash"]["requested_path_engine"], path_engine)
                self.assertFalse(receipt["fixture_hash"]["cache"])
                self.assertEqual(bool(receipt["fixture_changes"]), mutate)
                self.assertTrue((timing.run / "fixtures-after.json").is_file())
                self.assertEqual(receipt["tests"][0]["status"], "failed" if code else "passed-with-logged-gaps")
                self.assertEqual(receipt, json.loads((root / "analysis/test-relocation-results.json").read_text()))
                phases = json.loads((timing.run / "timing.json").read_text())["phase_seconds"]
                self.assertEqual(set(phases), {"inventory", "fixture_before_sha", "ctest",
                                               "fixture_after_sha", "junit_parsing_report"})


if __name__ == "__main__":
    unittest.main()
