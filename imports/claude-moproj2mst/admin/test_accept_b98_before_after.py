"""Receipt binding regressions using inert files; no converter is launched."""

from contextlib import redirect_stderr
import io
import json
import os
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

import accept_b98_before_after as acceptance


class FrozenReceiptBindingTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="b98-receipt-", dir=os.environ.get("M2M_TEST_SCRATCH"))
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name).resolve()
        root_patch = patch.object(acceptance, "ROOT", self.root)
        root_patch.start()
        self.addCleanup(root_patch.stop)
        run_patch = patch.object(acceptance.subprocess, "run", side_effect=AssertionError("converter must not launch"))
        run_patch.start()
        self.addCleanup(run_patch.stop)
        self.folder = self.root / "build" / "cp534-verified"
        self.folder.mkdir(parents=True)
        self.hashes = {}
        for name in acceptance.CP88_ARTIFACTS:
            path = self.folder / name
            path.write_bytes(("inert fixture " + name).encode("ascii"))
            self.hashes[name] = acceptance.file_sha256(path)
        self.source_hash = "a" * 64
        self.receipt = self.folder / "receipt.json"
        self.receipt_data = {
            "source_tree_sha256": self.source_hash,
            "artifacts": [
                {"path": str(self.folder / name), "sha256": value}
                for name, value in self.hashes.items()
            ],
        }
        self.write_receipt()

    def write_receipt(self):
        self.receipt.write_text(json.dumps(self.receipt_data), encoding="utf-8")

    def validate(self, **overrides):
        arguments = {
            "cli_arg": str(self.folder / "moproj2mst.exe"),
            "expected_artifacts": self.hashes,
            "candidate_version": "CP534",
            "expected_source_sha256": self.source_hash,
            "freeze_receipt_arg": None,
            "current_freeze_receipt_arg": str(self.receipt),
        }
        arguments.update(overrides)
        return acceptance.validate_cli(**arguments)

    def test_current_layout_binds_checkpoint_source_and_all_artifacts(self):
        cli, hashes, receipt, receipt_hash = self.validate()
        self.assertEqual(cli, self.folder / "moproj2mst.exe")
        self.assertEqual(hashes, self.hashes)
        self.assertEqual(receipt, self.receipt)
        self.assertEqual(receipt_hash, acceptance.file_sha256(self.receipt))

    def test_current_layout_rejects_other_checkpoint_label(self):
        with self.assertRaisesRegex(RuntimeError, "does not match candidate version CP533"):
            self.validate(candidate_version="CP533")

    def test_current_layout_requires_checkpoint_label(self):
        with self.assertRaisesRegex(RuntimeError, "CP<number>"):
            self.validate(candidate_version="current")

    def test_current_layout_requires_explicit_current_option(self):
        with self.assertRaisesRegex(RuntimeError, "does not match candidate version"):
            self.validate(freeze_receipt_arg=str(self.receipt), current_freeze_receipt_arg=None)

    def test_current_layout_rejects_receipt_outside_checkpoint_directory(self):
        foreign = self.root / "receipt.json"
        foreign.write_bytes(self.receipt.read_bytes())
        with self.assertRaisesRegex(RuntimeError, "does not match candidate version"):
            self.validate(current_freeze_receipt_arg=str(foreign))

    def test_source_hash_mismatch_is_rejected(self):
        with self.assertRaisesRegex(RuntimeError, "source-tree hash"):
            self.validate(expected_source_sha256="b" * 64)

    def test_changed_artifact_bytes_are_rejected(self):
        (self.folder / "moproj2mst.dll").write_bytes(b"changed")
        with self.assertRaisesRegex(RuntimeError, "identity mismatch for moproj2mst.dll"):
            self.validate()

    def test_receipt_artifact_hash_mismatch_is_rejected(self):
        self.receipt_data["artifacts"][0]["sha256"] = "c" * 64
        self.write_receipt()
        with self.assertRaisesRegex(RuntimeError, "Explicit hash does not match"):
            self.validate()

    def test_receipt_artifact_path_mismatch_is_rejected(self):
        self.receipt_data["artifacts"][0]["path"] = str(self.root / "moproj2mst.exe")
        self.write_receipt()
        with self.assertRaisesRegex(RuntimeError, "not the artifact bound"):
            self.validate()

    def test_missing_receipt_artifact_is_rejected(self):
        self.receipt_data["artifacts"].pop()
        self.write_receipt()
        with self.assertRaisesRegex(RuntimeError, "lacks required artifact deformdb.dll"):
            self.validate()

    def test_legacy_checkpoint_layout_remains_supported(self):
        legacy = self.root / "build" / "wave3-checkpoint162-verified"
        self.folder.rename(legacy)
        self.folder = legacy
        self.receipt = legacy / "receipt.json"
        for entry in self.receipt_data["artifacts"]:
            entry["path"] = str(legacy / Path(entry["path"]).name)
        self.write_receipt()
        _, _, receipt, _ = self.validate(
            candidate_version="CP162", freeze_receipt_arg=str(self.receipt),
            current_freeze_receipt_arg=None,
        )
        self.assertEqual(receipt, self.receipt)

    def test_both_receipt_options_are_rejected_by_api(self):
        with self.assertRaisesRegex(RuntimeError, "only one frozen receipt layout"):
            self.validate(freeze_receipt_arg=str(self.receipt))

    def test_both_receipt_options_are_rejected_by_cli(self):
        with patch("sys.argv", ["accept_b98_before_after.py", "--freeze-receipt", "old", "--current-freeze-receipt", "new"]):
            with redirect_stderr(io.StringIO()), self.assertRaises(SystemExit) as result:
                acceptance.main()
        self.assertEqual(result.exception.code, 2)

    def test_cli_forwards_current_receipt_and_matching_label_before_execution(self):
        argv = [
            "accept_b98_before_after.py", "--candidate-version", "CP534",
            "--current-freeze-receipt", str(self.receipt),
            "--cli", str(self.folder / "moproj2mst.exe"),
            "--source-tree-sha256", self.source_hash,
            "--expected-cli-sha256", self.hashes["moproj2mst.exe"],
            "--expected-moproj2mst-dll-sha256", self.hashes["moproj2mst.dll"],
            "--expected-deformdb-sha256", self.hashes["deformdb.dll"],
        ]
        with patch("sys.argv", argv), patch.object(acceptance, "validate_inputs"):
            with patch.object(acceptance, "validate_cli", side_effect=RuntimeError("binding boundary")) as validate:
                with self.assertRaisesRegex(RuntimeError, "binding boundary"):
                    acceptance.main()
        self.assertEqual(validate.call_args.kwargs["candidate_version"], "CP534")
        self.assertEqual(validate.call_args.kwargs["current_freeze_receipt_arg"], str(self.receipt))
        self.assertIsNone(validate.call_args.kwargs["freeze_receipt_arg"])


if __name__ == "__main__":
    unittest.main()
