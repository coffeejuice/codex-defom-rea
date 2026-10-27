"""Static contract checks for the CP372 outer monitor; never launches native code."""
from pathlib import Path
import re
import unittest


SCRIPT = Path(__file__).with_name("run_cp372_monitored.ps1").read_text(encoding="utf-8")


class MonitorStructureTests(unittest.TestCase):
    def test_occurrence_and_fixed_paths(self):
        self.assertIn("ValidatePattern('^[0-9a-f]{32}$')", SCRIPT)
        self.assertIn("cp372-receipt-' + $OccurrenceId", SCRIPT)
        self.assertIn("cp372-child-' + $OccurrenceId", SCRIPT)
        self.assertIn("cp372-monitor-' + $OccurrenceId", SCRIPT)
        self.assertIn("[IO.File]::Exists($childReceipt) -or [IO.File]::Exists($childOutput)", SCRIPT)
        self.assertIn("[IO.File]::Exists($monitor)", SCRIPT)
        self.assertIn("[IO.FileMode]::CreateNew", SCRIPT)

    def test_fixed_python_probe_and_source_binding(self):
        self.assertIn("C:\\Program Files\\Python312\\python.exe", SCRIPT)
        for fixed in ("'-I'", "'-B'", "'-c'", "cp350_native_trial as pre",
                      "pre._preflight()", "cp372_startup_only_probe as probe",
                      "probe._run_once(bind.load_native_exports(),occurrence_id=sys.argv[1])",
                      "except probe.ProbeFailure as failure", "after=pre._preflight()",
                      "equal=before==after"):
            self.assertIn(fixed, SCRIPT)
        self.assertNotRegex(SCRIPT, r"(?i)WORKER|DEF_PRE_64|gui|original")

    def test_exact_controller_timeout_and_drain_contract(self):
        self.assertIn("$process.WaitForExit(15000)", SCRIPT)
        self.assertIn("$process.Kill()", SCRIPT)
        self.assertIn("$process.WaitForExit(5000)", SCRIPT)
        self.assertIn("$process.StandardOutput.ReadToEndAsync()", SCRIPT)
        self.assertIn("$process.StandardError.ReadToEndAsync()", SCRIPT)
        self.assertIn("$item[1].Wait(1000)", SCRIPT)
        self.assertIn("$process.Dispose()", SCRIPT)

    def test_acceptance_requires_all_independent_receipts(self):
        self.assertIn("$child.probe_passed -eq $true", SCRIPT)
        self.assertIn("$child.cleanup -eq 'closed'", SCRIPT)
        self.assertIn("$child.schema -eq 'cp372-parent-v1'", SCRIPT)
        self.assertIn("$child.occurrence_id -ceq $OccurrenceId", SCRIPT)
        self.assertIn("$child.image -ceq $pythonImage", SCRIPT)
        self.assertIn("$receiptPathMatches", SCRIPT)
        self.assertIn("$childPathMatches", SCRIPT)
        self.assertIn("$controller.probe_passed -eq $true", SCRIPT)
        self.assertIn("$receipt.source_manifests_equal", SCRIPT)
        self.assertIn("$receipt.probe_passed = $true", SCRIPT)
        self.assertIn("$stream.Flush($true)", SCRIPT)
        self.assertIn("if (-not $receipt.probe_passed) { exit 1 }", SCRIPT)


if __name__ == "__main__":
    unittest.main()
