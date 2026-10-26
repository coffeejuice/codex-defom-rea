"""Unit tests for source and executable run binding in the CTest launcher."""

import tempfile
import unittest
from pathlib import Path

from run_converter_tests import binary_paths, binding_is_stable, execution_binding, gap_events


class ExecutionBindingTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.source = self.root / "source"
        self.source.mkdir()
        self.source_file = self.source / "unit.cpp"
        self.source_file.write_text("int value = 1;\\n", encoding="utf-8")
        self.binary = self.root / "test.exe"
        self.binary.write_bytes(b"first executable")

    def test_identical_source_and_binaries_are_stable(self):
        before = execution_binding(self.source, [self.binary])
        after = execution_binding(self.source, [self.binary])
        self.assertTrue(binding_is_stable(before, after))
        self.assertEqual(before["source_tree"]["files"], 1)
        self.assertEqual(before["binaries"][str(self.binary)]["bytes"], 16)

    def test_source_edit_is_detected(self):
        before = execution_binding(self.source, [self.binary])
        self.source_file.write_text("int value = 2;\\n", encoding="utf-8")
        after = execution_binding(self.source, [self.binary])
        self.assertFalse(binding_is_stable(before, after))

    def test_binary_rebuild_is_detected(self):
        before = execution_binding(self.source, [self.binary])
        self.binary.write_bytes(b"replacement executable")
        after = execution_binding(self.source, [self.binary])
        self.assertFalse(binding_is_stable(before, after))

    def test_public_abi_artifacts_are_bound_alongside_test_executable(self):
        build = self.root / "build"
        output = build / "bin"
        output.mkdir(parents=True)
        for name in ("moproj2mst.exe", "moproj2mst.dll", "deformdb.dll"):
            (output / name).write_bytes(name.encode())
        paths = binary_paths([{"command": [str(self.binary)]}], build)
        self.assertEqual({path.name for path in paths}, {
            "test.exe", "moproj2mst.exe", "moproj2mst.dll", "deformdb.dll"
        })


class GapEventTests(unittest.TestCase):
    def test_empty_active_cogging_summary_is_not_a_gap(self):
        self.assertEqual(gap_events("  refused (active cogging operation):  \r\n"), [])

    def test_nonempty_active_cogging_refusal_remains_a_gap(self):
        line = "  refused (active cogging operation): B11_CM T_GRP"
        self.assertEqual(gap_events(line + "\n"), [line])

    def test_other_gap_lines_remain_detected(self):
        output = "SKIP optional fixture unset\nNOTE target not run\n2 cases skipped\n"
        self.assertEqual(gap_events(output), output.splitlines())

    def test_cp125_mdt_write_log_only_reports_real_skip(self):
        log = (Path(__file__).resolve().parents[1] / "build/test-runs/1792922590847127500/"
               "m2m.mdt.mdt_write.log")
        if not log.exists():
            self.skipTest("retained CP125 mdt_write log is unavailable")
        self.assertEqual(gap_events(log.read_text(encoding="utf-8")), [
            "SKIP optional imported PACK/pack.dat header check: M2M_INSTALL_FIXTURE unset"
        ])


if __name__ == "__main__":
    unittest.main()
