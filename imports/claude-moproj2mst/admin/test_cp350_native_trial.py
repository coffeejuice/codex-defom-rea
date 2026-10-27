"""Injected CP350 receipt tests; no native export or process is called."""
from pathlib import Path
from tempfile import TemporaryDirectory
from types import SimpleNamespace
from unittest import mock
import json
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp350_native_trial as m


class FakeTop:
    def __init__(self, *, fail=False, on_run=None):
        self.fail, self.on_run = fail, on_run
        self.receipt = SimpleNamespace(
            phase="new", deadline=100, pid=12, tid=13,
            create_outcome="returned-success", resume_outcome="resumed-once",
            runner_waited=True, job_empty=True, job_terminated=False,
            process_terminated=False, errors=[])
        self.claim = SimpleNamespace(deadline=99, nonce=b"a" * 32)
        self._accepted = False
        self._timer_fired = False
        self._owned_handles = {}
        self.pipe_owner = SimpleNamespace(handles=set())
        self.runs = self.closes = 0

    def run(self):
        self.runs += 1
        if self.on_run:
            self.on_run()
        if self.fail:
            raise m.Hold("injected uncertain OS call")
        self._accepted = True
        self.receipt.phase = "closed-success"
        return self.receipt

    def close(self):
        self.closes += 1
        self.receipt.phase = "closed"
        return self.receipt


class TrialTests(unittest.TestCase):
    def setUp(self):
        self.temp = TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        root = Path(self.temp.name)
        admin = root / "admin"
        analysis = root / "analysis"
        admin.mkdir()
        analysis.mkdir()
        self.source = admin / "cp349_top_controller.py"
        self.source.write_text("fixed source\n")
        worker_source = admin / "fixtures" / "cp316" / "worker.c"
        worker_source.parent.mkdir(parents=True)
        worker_source.write_text("int main(void) { return 0; }\n")
        worker = root / "worker.exe"
        worker.write_bytes(b"WORKER")
        build = analysis / "cp316-worker-fixture-build.json"
        build.write_text(json.dumps({"source_sha256": m._digest(worker_source),
                                     "pe": {"sha256": m._digest(worker),
                                            "bytes": 6}}))
        python = root / "python.exe"
        python.write_bytes(b"PYTHON")
        self.path = analysis / ("cp350-inert-trial-" + "a" * 32 + ".json")
        self.patches = [
            mock.patch.object(m, "ADMIN_DIR", admin),
            mock.patch.object(m, "ANALYSIS_DIR", analysis),
            mock.patch.object(m, "WORKER_IMAGE", worker),
            mock.patch.object(m, "WORKER_BYTES", 6),
            mock.patch.object(m, "WORKER_SHA256", m._digest(worker)),
            mock.patch.object(m, "WORKER_SOURCE", worker_source),
            mock.patch.object(m, "WORKER_SOURCE_SHA256", m._digest(worker_source)),
            mock.patch.object(m, "WORKER_BUILD_RECEIPT", build),
            mock.patch.object(m, "WORKER_BUILD_SHA256", m._digest(build)),
            mock.patch.object(m, "PYTHON_IMAGE", python),
            mock.patch.object(m, "_used", False),
            mock.patch.object(m, "_load_exports", return_value=object()),
        ]
        for patcher in self.patches:
            patcher.start()
            self.addCleanup(patcher.stop)

    def test_import_and_public_cli_are_inert(self):
        self.assertEqual(m.NATIVE_ENTRY, "HARD HOLD")
        with self.assertRaises(m.Hold):
            m.launch_trial()
        self.assertFalse(m._used)

    def test_fixed_success_report_and_one_use(self):
        owner = FakeTop()
        with mock.patch.object(m, "_make_controller", return_value=owner):
            trial = m.run_once(self.path)
        self.assertTrue(trial.accepted)
        self.assertIs(trial.owner, owner)
        self.assertEqual(owner.runs, 1)
        self.assertEqual(owner.closes, 0)
        disk = json.loads(self.path.read_text())
        self.assertEqual(disk["phase"], "closed-success")
        self.assertEqual(disk["owner"]["claim_nonce_hex"], (b"a" * 32).hex())
        self.assertEqual(disk["before"], disk["after"])
        with self.assertRaises(m.Hold):
            m.run_once(self.path.with_name("cp350-inert-trial-" + "b" * 32 + ".json"))

    def test_failure_retains_exact_owner_and_retries_only_cleanup(self):
        owner = FakeTop(fail=True)
        with mock.patch.object(m, "_make_controller", return_value=owner):
            trial = m.run_once(self.path)
        self.assertFalse(trial.accepted)
        self.assertIs(trial.owner, owner)
        self.assertEqual((owner.runs, owner.closes), (1, 1))
        self.assertEqual(trial.report["cleanup"], "closed")
        self.assertIn("injected uncertain OS call", trial.report["error"])

    def test_failure_report_preserves_bounded_child_diagnostic(self):
        owner = FakeTop(fail=True)
        diagnostic = {"path": "analysis/cp349-child-failure-test.json",
                      "status": "valid", "bytes": 42, "sha256": "a" * 64,
                      "data": {"stage": "fixed-parent-import-or-run",
                               "type": "InjectedError", "message": "failure",
                               "traceback": []}}
        owner.receipt.local = {"child_diagnostic": diagnostic}
        with mock.patch.object(m, "_make_controller", return_value=owner):
            trial = m.run_once(self.path)
        self.assertFalse(trial.accepted)
        self.assertEqual(trial.report["owner"]["child_diagnostic"], diagnostic)
        disk = json.loads(self.path.read_text())
        self.assertEqual(disk["owner"]["child_diagnostic"], diagnostic)

    def test_changed_source_after_run_refuses_success(self):
        owner = FakeTop(on_run=lambda: self.source.write_text("changed source\n"))
        with mock.patch.object(m, "_make_controller", return_value=owner):
            trial = m.run_once(self.path)
        self.assertFalse(trial.accepted)
        self.assertEqual(trial.report["phase"], "hold")
        self.assertNotEqual(trial.report["before"], trial.report["after"])

    def test_changed_worker_source_after_run_refuses_success(self):
        owner = FakeTop(on_run=lambda: m.WORKER_SOURCE.write_text("changed C source\n"))
        with mock.patch.object(m, "_make_controller", return_value=owner):
            trial = m.run_once(self.path)
        self.assertFalse(trial.accepted)
        self.assertEqual(trial.report["phase"], "hold")
        self.assertIn("identity mismatch", trial.report["after"]["error"])

    def test_existing_bytecode_blocks_owner_creation(self):
        cache = m.ADMIN_DIR / "__pycache__"
        cache.mkdir()
        (cache / "cp349_top_controller.cpython-312.pyc").write_bytes(b"old")
        with mock.patch.object(m, "_make_controller") as make:
            trial = m.run_once(self.path)
        self.assertFalse(trial.accepted)
        self.assertIsNone(trial.owner)
        make.assert_not_called()
        self.assertIn("bytecode", trial.report["error"])

    def test_path_escape_or_existing_report_refused_before_trial(self):
        with self.assertRaises(m.Hold):
            m.run_once(m.ADMIN_DIR / self.path.name)
        self.path.write_text("existing")
        with self.assertRaises(m.Hold):
            m.run_once(self.path)
        self.assertFalse(m._used)

    def test_report_write_uncertainty_retains_owner(self):
        owner = FakeTop()
        with (mock.patch.object(m, "_make_controller", return_value=owner),
              mock.patch.object(m, "_write_report", side_effect=OSError("disk full"))):
            with self.assertRaises(m.TrialFailure) as caught:
                m.run_once(self.path)
        self.assertIs(caught.exception.owner, owner)
        self.assertEqual(owner.runs, 1)
        self.assertFalse(caught.exception.report["accepted"])
        self.assertEqual(caught.exception.report["phase"], "hold")


if __name__ == "__main__":
    unittest.main()
