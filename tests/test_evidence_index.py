"""Index invalidation, bounded misses, and record identity preservation."""
import hashlib
import importlib.util
import contextlib
import io
import json
import os
from pathlib import Path
import tempfile
import subprocess
import sys
import unittest
from unittest import mock

SPEC = importlib.util.spec_from_file_location("evidence_index", Path(__file__).resolve().parents[1] / "tools/evidence_index.py")
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)


class EvidenceIndexTests(unittest.TestCase):
    def setUp(self):
        scratch = Path(__file__).resolve().parents[1] / "build/evidence-slice-tests"
        scratch.mkdir(parents=True, exist_ok=True)
        self.temp = tempfile.TemporaryDirectory(dir=scratch)
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.source = self.root / "ignored.json"
        self.record = {"evidence_id": "fixture", "operation": "procedure_pseudo_code",
                       "parameters": {"procedure": "0x0010"},
                       "subject": {"digest": {"sha256": "a" * 64}},
                       "raw_result": {"value": "exact\r\nΩ"}}
        self.source.write_text(json.dumps({"evidence_bundle": {"records": [self.record, {"parameters": None}]}}), encoding="utf-8")
        self.index_path = self.root / "index.json"
        self.rebuild()

    def rebuild(self):
        self.index = MODULE.build([self.source])
        raw = MODULE.encode(self.index)
        self.index_path.write_bytes(raw)
        self.digest = hashlib.sha256(raw).hexdigest()

    def query(self, proc="0x10", target="a" * 64, operation=None):
        return MODULE.query(self.index_path, self.digest, proc, target, operation)

    def test_identity_and_scope(self):
        result = self.query("0X0010")
        match = result["matches"][0]
        self.assertEqual(match["record_pointer"], "/evidence_bundle/records/0")
        self.assertEqual(match["evidence_id"], "fixture")
        self.assertEqual(match["source_sha256"], hashlib.sha256(self.source.read_bytes()).hexdigest())
        self.assertEqual(match["body_pointers"], ["/raw_result/value"])
        self.assertEqual(result["scope"][0]["records_skipped"], 1)

    def test_address_target_operation_misses_are_bounded(self):
        for args in [("0x11",), ("0x10", "b" * 64), ("0x10", "a" * 64, "analyze_function")]:
            with self.subTest(args=args):
                self.assertEqual(self.query(*args)["status"], "not_found_in_indexed_scope")

    def test_same_size_same_mtime_edit_invalidates_even_miss(self):
        stat = self.source.stat()
        self.source.write_bytes(self.source.read_bytes().replace(b"fixture", b"changed"))
        os.utime(self.source, ns=(stat.st_atime_ns, stat.st_mtime_ns))
        with self.assertRaisesRegex(ValueError, "Stale source"):
            self.query("0xffff")

    def test_missing_source_and_tampered_index_fail(self):
        self.source.unlink()
        with self.assertRaises(OSError):
            self.query()
        self.index_path.write_bytes(self.index_path.read_bytes().replace(b"fixture", b"changed"))
        with self.assertRaisesRegex(ValueError, "Index SHA-256 mismatch"):
            self.query()

    def test_supported_containers_duplicates_and_body_locations(self):
        for doc, expected in [(self.record, ""), ({"records": [self.record, self.record]}, "/records/0")]:
            self.source.write_text(json.dumps(doc), encoding="utf-8")
            self.rebuild()
            self.assertEqual(self.query()["matches"][0]["record_pointer"], expected)
        self.assertEqual(len(self.query()["matches"]), 2)
        row = {"normalized_result": {"pseudocode": "n"}, "raw_result": {"value": {"pseudocode": "r"}}}
        self.assertEqual(MODULE.body_pointers(row), ["/normalized_result/pseudocode", "/raw_result/value/pseudocode"])

    def test_unknown_container_and_invalid_query_fail(self):
        self.source.write_text('{"unrecognized": []}', encoding="utf-8")
        with self.assertRaisesRegex(ValueError, "Unsupported"):
            MODULE.build([self.source])
        for proc in ["10", "-0x10", "0xzz"]:
            with self.assertRaises(ValueError):
                self.query(proc)

    def run_build_cli(self, output):
        return subprocess.run([sys.executable, "-B", str(Path(MODULE.__file__)),
                               "build", str(self.source), "--output", str(output)],
                              capture_output=True, check=False)

    def assert_rejected_unchanged(self, output):
        source_before, output_before = self.source.read_bytes(), output.read_bytes()
        run = self.run_build_cli(output)
        self.assertEqual(run.returncode, 2, run.stderr)
        self.assertEqual(run.stdout, b"")
        self.assertIn(b"already exists", run.stderr)
        self.assertEqual(self.source.read_bytes(), source_before)
        self.assertEqual(output.read_bytes(), output_before)

    def test_cli_rejects_hard_link_output_without_changing_source(self):
        alias = self.root / "hard-link.json"
        os.link(self.source, alias)
        self.assertTrue(self.source.samefile(alias))
        self.assertNotEqual(self.source.resolve(), alias.resolve())
        self.assert_rejected_unchanged(alias)

    def test_cli_rejects_source_as_output(self):
        self.assert_rejected_unchanged(self.source)

    def test_cli_rejects_existing_independent_cache(self):
        self.assert_rejected_unchanged(self.index_path)

    def test_cli_creates_fresh_output_and_valid_receipt(self):
        output = self.root / "fresh-index.json"
        source_before = self.source.read_bytes()
        run = self.run_build_cli(output)
        self.assertEqual(run.returncode, 0, run.stderr)
        self.assertEqual(run.stderr, b"")
        receipt = json.loads(run.stdout)
        self.assertEqual(output.read_bytes(), MODULE.encode(MODULE.build([self.source])))
        self.assertEqual(receipt["index_sha256"], hashlib.sha256(output.read_bytes()).hexdigest())
        self.assertEqual(self.source.read_bytes(), source_before)
        self.assertEqual(MODULE.query(output, receipt["index_sha256"], "0x10", "a" * 64)["status"], "matches")

    def test_exclusive_creation_rejects_output_appearing_after_preflight(self):
        original_build = MODULE.build
        for kind in ("hard_link", "independent_file"):
            with self.subTest(kind=kind):
                output = self.root / (kind + "-race.json")
                source_before = self.source.read_bytes()

                def create_after_check(paths):
                    result = original_build(paths)
                    self.assertFalse(os.path.lexists(output))
                    if kind == "hard_link":
                        os.link(self.source, output)
                    else:
                        output.write_bytes(b"concurrent writer's bytes")
                    return result

                stdout, stderr = io.StringIO(), io.StringIO()
                with mock.patch.object(MODULE, "build", side_effect=create_after_check), \
                        mock.patch.object(sys, "argv", ["evidence_index.py", "build", str(self.source), "--output", str(output)]), \
                        contextlib.redirect_stdout(stdout), contextlib.redirect_stderr(stderr):
                    status = MODULE.main()
                self.assertEqual(status, 2)
                self.assertEqual(stdout.getvalue(), "")
                self.assertIn("error", json.loads(stderr.getvalue()))
                self.assertEqual(self.source.read_bytes(), source_before)
                self.assertEqual(output.read_bytes(), source_before if kind == "hard_link" else b"concurrent writer's bytes")


if __name__ == "__main__":
    unittest.main()
