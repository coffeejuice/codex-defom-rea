"""Unit tests for configure-reuse grouped builds; no compiler is invoked."""

import json
import contextlib
import io
import sys
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

ADMIN = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ADMIN))

import build_candidate


class FakeRunner:
    def __init__(self, source, build, drift=None, fail_phase=None):
        self.source = source
        self.build = build
        self.drift = drift
        self.fail_phase = fail_phase
        self.counts = {"configure": 0, "preflight": 0, "full": 0}
        self.commands = []

    def __call__(self, command, *, env, cwd, stdout, stderr, **kwargs):
        self.commands.append(list(command))
        if "-S" in command:
            phase = "configure"
            self.counts[phase] += 1
            self.build.mkdir(parents=True, exist_ok=True)
            (self.build / "CMakeCache.txt").write_bytes(b"stable-cache")
        elif "--target" in command:
            phase = "preflight"
            self.counts[phase] += 1
        else:
            phase = "full"
            self.counts[phase] += 1
            out = self.build / "bin"
            out.mkdir(parents=True, exist_ok=True)
            for name in ("moproj2mst.exe", "moproj2mst.dll", "deformdb.dll"):
                (out / name).write_bytes(f"{name}-{self.counts[phase]}".encode())
        if self.drift == (phase, self.counts[phase]):
            if phase == "configure":
                (self.source / "unit.cpp").write_text("int value = 2;\n", encoding="utf-8")
            elif phase == "preflight":
                (self.source / "unit.cpp").write_text(
                    f"int value = {self.counts[phase] + 2};\n", encoding="utf-8")
            else:
                (self.source / "unit.cpp").write_text("int value = 99;\n", encoding="utf-8")
        if self.drift == ("cache", phase, self.counts[phase]):
            (self.build / "CMakeCache.txt").write_bytes(b"changed-cache")
        if stdout is not None:
            stdout.write(f"simulated {phase}\n")
        return SimpleNamespace(returncode=(3 if phase == self.fail_phase else 0))


class GroupedBuildTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.source = self.root / "source"
        (self.source / "apps/moproj2mst").mkdir(parents=True)
        (self.source / "unit.cpp").write_text("int value = 1;\n", encoding="utf-8")
        self.build = self.root / "build/candidate"
        self.logs = self.root / "build/logs"
        self.targets = ["m2m_doe_sample", "m2m_mdt_sample"]

    def run_grouped(self, fake, retry_limit=2):
        with contextlib.redirect_stdout(io.StringIO()):
            return build_candidate.build_grouped(
                self.source, self.build, self.logs, "unit", self.targets, {},
                max_reconfigure=retry_limit, runner=fake)

    def test_success_configures_once_then_preflight_and_full(self):
        fake = FakeRunner(self.source, self.build)
        receipt, code = self.run_grouped(fake)
        self.assertEqual(code, 0)
        self.assertEqual(fake.counts, {"configure": 1, "preflight": 1, "full": 1})
        self.assertEqual([step["phase"] for step in receipt["steps"]],
                         ["configure", "focused-preflight", "full-build"])
        self.assertIsNone(receipt["targets"])
        self.assertEqual(receipt["preflight_targets"], self.targets)
        self.assertTrue(receipt["source_stable_during_build"])
        self.assertEqual(receipt["source_tree_sha256"], receipt["source_tree_after_sha256"])
        self.assertEqual(receipt["configure_reuse"]["reconfigure_count"], 0)
        self.assertEqual(len(receipt["artifacts"]), 3)
        self.assertEqual(receipt["steps"][1]["expected_cache_sha256"],
                         receipt["steps"][2]["expected_cache_sha256"])
        self.assertNotIn("source_preflight", receipt)

    def test_explicit_source_preflight_runs_before_configure_and_is_receipted(self):
        fake = FakeRunner(self.source, self.build)
        files = ["tests/new_a.cpp", "tests/new_b.cpp"]
        events = []

        def checker(project_root, sources):
            events.append("checker")
            self.assertEqual(project_root, self.source / "apps/moproj2mst")
            self.assertEqual(sources, files)
            return {"ok": True, "files": []}

        def runner(command, **kwargs):
            events.append("configure" if "-S" in command else
                          "focused-preflight" if "--target" in command else "full-build")
            return fake(command, **kwargs)

        with patch.object(build_candidate.check_new_cpp_bindings, "check_files", side_effect=checker):
            with contextlib.redirect_stdout(io.StringIO()):
                receipt, code = build_candidate.build_grouped(
                    self.source, self.build, self.logs, "preflight", self.targets, {},
                    runner=runner, preflight_sources=files)
        self.assertEqual(code, 0)
        self.assertEqual(events, ["checker", "configure", "focused-preflight", "full-build"])
        self.assertEqual(fake.counts, {"configure": 1, "preflight": 1, "full": 1})
        self.assertEqual(receipt["source_preflight"]["files"], files)
        self.assertTrue(receipt["source_preflight"]["result"]["ok"])
        self.assertEqual(receipt["source_preflight"]["source_tree_before"],
                         receipt["source_preflight"]["source_tree_after"])

    def test_source_preflight_issue_fails_before_any_build_command(self):
        fake = FakeRunner(self.source, self.build)
        with patch.object(build_candidate.check_new_cpp_bindings, "check_files",
                          return_value={"ok": False, "files": [{"ok": False}]}):
            with contextlib.redirect_stdout(io.StringIO()):
                receipt, code = build_candidate.build_grouped(
                    self.source, self.build, self.logs, "preflight-fail", self.targets, {},
                    runner=fake, preflight_sources=["tests/bad.cpp"])
        self.assertEqual(code, 1)
        self.assertEqual(fake.counts, {"configure": 0, "preflight": 0, "full": 0})
        self.assertEqual(receipt["status"], "source-preflight-failed")
        self.assertEqual(receipt["steps"], [])
        self.assertFalse(receipt["source_stable_during_build"])
        self.assertFalse(receipt["source_preflight"]["result"]["ok"])

    def test_source_preflight_drift_fails_before_configure(self):
        fake = FakeRunner(self.source, self.build)

        def checker(_project_root, _sources):
            (self.source / "unit.cpp").write_text("int value = 8;\n", encoding="utf-8")
            return {"ok": True, "files": []}

        with patch.object(build_candidate.check_new_cpp_bindings, "check_files", side_effect=checker):
            with contextlib.redirect_stdout(io.StringIO()):
                receipt, code = build_candidate.build_grouped(
                    self.source, self.build, self.logs, "preflight-drift", self.targets, {},
                    runner=fake, preflight_sources=["tests/test.cpp"])
        self.assertEqual(code, 1)
        self.assertEqual(fake.counts, {"configure": 0, "preflight": 0, "full": 0})
        self.assertEqual(receipt["status"], "source-preflight-source-drift")
        self.assertNotEqual(receipt["source_preflight"]["source_tree_before"],
                            receipt["source_preflight"]["source_tree_after"])

    def test_configure_source_drift_reconfigures_before_preflight(self):
        fake = FakeRunner(self.source, self.build, drift=("configure", 1))
        receipt, code = self.run_grouped(fake)
        self.assertEqual(code, 0)
        self.assertEqual(fake.counts, {"configure": 2, "preflight": 1, "full": 1})
        self.assertEqual(receipt["configure_reuse"]["reconfigure_count"], 1)
        self.assertEqual(receipt["configure_reuse"]["drift_events"][0]["reason"],
                         "source changed during configure")
        self.assertTrue(receipt["source_stable_during_build"])

    def test_source_drift_after_preflight_reconfigures_and_repeats_group(self):
        fake = FakeRunner(self.source, self.build, drift=("preflight", 1))
        receipt, code = self.run_grouped(fake)
        self.assertEqual(code, 0)
        self.assertEqual(fake.counts, {"configure": 2, "preflight": 2, "full": 1})
        self.assertEqual(receipt["configure_reuse"]["reconfigure_count"], 1)
        self.assertEqual(receipt["steps"][-1]["expected_source_tree_sha256"],
                         receipt["source_tree_sha256"])
        self.assertTrue(receipt["source_stable_during_build"])

    def test_cache_drift_reconfigures_and_repeats_group(self):
        fake = FakeRunner(self.source, self.build, drift=("cache", "preflight", 1))
        receipt, code = self.run_grouped(fake)
        self.assertEqual(code, 0)
        self.assertEqual(fake.counts, {"configure": 2, "preflight": 2, "full": 1})
        self.assertEqual(receipt["configure_reuse"]["drift_events"][0]["reason"],
                         "source or CMake cache changed during focused build")
        self.assertTrue(receipt["source_stable_during_build"])

    def test_retry_limit_fails_closed_without_full_build(self):
        fake = FakeRunner(self.source, self.build)
        original = fake.__call__

        def always_drift(command, **kwargs):
            result = original(command, **kwargs)
            if "--target" in command:
                (self.source / "unit.cpp").write_text(
                    f"int value = {fake.counts['preflight'] + 10};\n", encoding="utf-8")
            return result

        with contextlib.redirect_stdout(io.StringIO()):
            receipt, code = build_candidate.build_grouped(
                self.source, self.build, self.logs, "unit", self.targets, {},
                max_reconfigure=1, runner=always_drift)
        self.assertNotEqual(code, 0)
        self.assertEqual(fake.counts, {"configure": 2, "preflight": 2, "full": 0})
        self.assertEqual(receipt["status"], "focused-preflight-drift-retry-limit")
        self.assertFalse(receipt["source_stable_during_build"])
        self.assertEqual(receipt["artifacts"], [])

    def test_preflight_compile_failure_stops_before_full_build(self):
        fake = FakeRunner(self.source, self.build, fail_phase="preflight")
        receipt, code = self.run_grouped(fake)
        self.assertEqual(code, 1)
        self.assertEqual(fake.counts, {"configure": 1, "preflight": 1, "full": 0})
        self.assertEqual(receipt["status"], "focused-preflight-failed")
        self.assertFalse(receipt["source_stable_during_build"])

    def test_configure_failure_with_concurrent_source_drift_stops(self):
        fake = FakeRunner(self.source, self.build, drift=("configure", 1),
                          fail_phase="configure")
        receipt, code = self.run_grouped(fake)
        self.assertEqual(code, 1)
        self.assertEqual(fake.counts, {"configure": 1, "preflight": 0, "full": 0})
        self.assertEqual(receipt["status"], "configure-failed")
        self.assertEqual([step["exit_code"] for step in receipt["steps"]], [3])
        self.assertEqual(receipt["configure_reuse"]["reconfigure_count"], 0)
        self.assertFalse(receipt["source_stable_during_build"])

    def test_preflight_failure_with_concurrent_source_drift_stops(self):
        fake = FakeRunner(self.source, self.build, drift=("preflight", 1),
                          fail_phase="preflight")
        receipt, code = self.run_grouped(fake)
        self.assertEqual(code, 1)
        self.assertEqual(fake.counts, {"configure": 1, "preflight": 1, "full": 0})
        self.assertEqual(receipt["status"], "focused-preflight-failed")
        self.assertEqual([step["exit_code"] for step in receipt["steps"]], [0, 3])
        self.assertEqual(receipt["configure_reuse"]["reconfigure_count"], 0)
        self.assertFalse(receipt["source_stable_during_build"])

    def test_full_build_failure_with_concurrent_source_drift_stops(self):
        fake = FakeRunner(self.source, self.build, drift=("full", 1),
                          fail_phase="full")
        receipt, code = self.run_grouped(fake)
        self.assertEqual(code, 1)
        self.assertEqual(fake.counts, {"configure": 1, "preflight": 1, "full": 1})
        self.assertEqual(receipt["status"], "full-build-failed")
        self.assertEqual([step["exit_code"] for step in receipt["steps"]], [0, 0, 3])
        self.assertEqual(receipt["configure_reuse"]["reconfigure_count"], 0)
        self.assertFalse(receipt["source_stable_during_build"])
        self.assertEqual(receipt["artifacts"], [])

    def test_full_build_source_drift_retries_entire_sequence(self):
        fake = FakeRunner(self.source, self.build, drift=("full", 1))
        receipt, code = self.run_grouped(fake)
        self.assertEqual(code, 0)
        self.assertEqual(fake.counts, {"configure": 2, "preflight": 2, "full": 2})
        self.assertEqual(receipt["configure_reuse"]["reconfigure_count"], 1)
        self.assertTrue(receipt["source_stable_during_build"])
        self.assertEqual([Path(item["path"]).name for item in receipt["artifacts"]],
                         ["moproj2mst.exe", "deformdb.dll", "moproj2mst.dll"])

    def test_full_build_cache_drift_retries_entire_sequence(self):
        fake = FakeRunner(self.source, self.build, drift=("cache", "full", 1))
        with contextlib.redirect_stdout(io.StringIO()):
            receipt, code = build_candidate.build_grouped(
                self.source, self.build, self.logs, "unit", self.targets, {},
                max_reconfigure=2, runner=fake)
        self.assertEqual(code, 0)
        self.assertEqual(fake.counts, {"configure": 2, "preflight": 2, "full": 2})
        self.assertEqual(receipt["configure_reuse"]["drift_events"][0]["reason"],
                         "source or CMake cache changed during full build")
        self.assertTrue(receipt["source_stable_during_build"])

    def test_legacy_target_cli_shape_remains_focused_only(self):
        analysis = self.root / "analysis"
        analysis.mkdir()
        (self.source / "apps/moproj2mst/CMakeLists.txt").write_text("", encoding="utf-8")
        fake = FakeRunner(self.source, self.build)
        fake_vcvars = SimpleNamespace(stdout="PATH=C:\\Windows\\System32\n")
        def mocked_run(command, **kwargs):
            if isinstance(command, str) and command.startswith("cmd.exe"):
                return fake_vcvars
            return fake(command, **kwargs)
        with patch.object(build_candidate, "ROOT", self.root), \
             patch.object(build_candidate, "BUILD", self.build), \
             patch.object(build_candidate, "VCVARS", self.root / "vcvars.bat"), \
             patch.object(build_candidate.subprocess, "run", side_effect=mocked_run), \
             contextlib.redirect_stdout(io.StringIO()):
            code = build_candidate.main(["--label", "legacy", "--target", self.targets[0]])
        receipt = json.loads((analysis / "legacy-build.json").read_text(encoding="utf-8"))
        self.assertEqual(code, 0)
        self.assertEqual(receipt["targets"], [self.targets[0]])
        self.assertEqual(fake.counts, {"configure": 1, "preflight": 1, "full": 0})
        self.assertIn("--target", fake.commands[-1])

    def test_grouped_cli_writes_freeze_compatible_full_receipt(self):
        analysis = self.root / "analysis"
        analysis.mkdir()
        (self.source / "apps/moproj2mst/CMakeLists.txt").write_text("", encoding="utf-8")
        fake = FakeRunner(self.source, self.build)
        fake_vcvars = SimpleNamespace(stdout="PATH=C:\\Windows\\System32\n")
        def mocked_run(command, **kwargs):
            if isinstance(command, str) and command.startswith("cmd.exe"):
                return fake_vcvars
            return fake(command, **kwargs)
        with patch.object(build_candidate.check_new_cpp_bindings, "check_files") as checker, \
             patch.object(build_candidate, "ROOT", self.root), \
             patch.object(build_candidate, "BUILD", self.build), \
             patch.object(build_candidate, "VCVARS", self.root / "vcvars.bat"), \
             patch.object(build_candidate.subprocess, "run", side_effect=mocked_run), \
             contextlib.redirect_stdout(io.StringIO()):
            code = build_candidate.main([
                "--label", "grouped", "--preflight-target", self.targets[0],
                "--preflight-target", self.targets[1], "--then-full"])
        receipt = json.loads((analysis / "grouped-build.json").read_text(encoding="utf-8"))
        self.assertEqual(code, 0)
        self.assertIsNone(receipt["targets"])
        self.assertEqual(receipt["preflight_targets"], self.targets)
        self.assertTrue(receipt["source_stable_during_build"])
        self.assertEqual(fake.counts, {"configure": 1, "preflight": 1, "full": 1})
        checker.assert_not_called()

    def test_grouped_cli_repeats_explicit_source_preflight(self):
        analysis = self.root / "analysis"
        analysis.mkdir()
        (self.source / "apps/moproj2mst/CMakeLists.txt").write_text("", encoding="utf-8")
        fake = FakeRunner(self.source, self.build)
        fake_vcvars = SimpleNamespace(stdout="PATH=C:\\Windows\\System32\n")
        files = ["tests/new_a.cpp", "tests/new_b.cpp"]
        checker_result = {"ok": True, "files": []}

        def mocked_run(command, **kwargs):
            if isinstance(command, str) and command.startswith("cmd.exe"):
                return fake_vcvars
            return fake(command, **kwargs)

        with patch.object(build_candidate.check_new_cpp_bindings, "check_files",
                          return_value=checker_result) as checker, \
             patch.object(build_candidate, "ROOT", self.root), \
             patch.object(build_candidate, "BUILD", self.build), \
             patch.object(build_candidate, "VCVARS", self.root / "vcvars.bat"), \
             patch.object(build_candidate.subprocess, "run", side_effect=mocked_run), \
             contextlib.redirect_stdout(io.StringIO()):
            code = build_candidate.main([
                "--label", "grouped-source-preflight",
                "--preflight-target", self.targets[0], "--then-full",
                "--preflight-source", files[0], "--preflight-source", files[1]])
        receipt = json.loads((analysis / "grouped-source-preflight-build.json").read_text(encoding="utf-8"))
        self.assertEqual(code, 0)
        self.assertEqual(checker.call_count, 1)
        self.assertEqual(checker.call_args.args[1], files)
        self.assertEqual(receipt["source_preflight"]["files"], files)
        self.assertEqual(fake.counts, {"configure": 1, "preflight": 1, "full": 1})

    def test_source_preflight_requires_grouped_full_mode(self):
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit) as raised:
                build_candidate.main(["--label", "invalid", "--preflight-source", "tests/new.cpp"])
        self.assertEqual(raised.exception.code, 2)

    def test_focused_ctests_run_after_compile_before_full_with_exact_names(self):
        fake = FakeRunner(self.source, self.build)
        events = []

        def runner(command, **kwargs):
            if command[0] == "ctest":
                events.append(("ctest", command[-1]))
                kwargs["stdout"].write("focused test passed\n")
                return SimpleNamespace(returncode=0)
            events.append(("build", "--target" in command))
            return fake(command, **kwargs)

        names = ["m2m.doe sample", "m2m.mdt_sample"]
        with contextlib.redirect_stdout(io.StringIO()):
            receipt, code = build_candidate.build_grouped(
                self.source, self.build, self.logs, "runtime", self.targets, {},
                runner=runner, preflight_ctests=names)
        self.assertEqual(code, 0)
        self.assertEqual(events, [("build", False), ("build", True),
                                  ("ctest", "^m2m\\.doe\\ sample$"),
                                  ("ctest", "^m2m\\.mdt_sample$"),
                                  ("build", False)])
        self.assertEqual(receipt["focused_runtime_preflight"]["tests"], names)
        self.assertEqual([row["exit_code"] for row in
                          receipt["focused_runtime_preflight"]["results"]], [0, 0])
        runtime_steps = [step for step in receipt["steps"]
                         if step["phase"] == "focused-runtime"]
        self.assertEqual(len(runtime_steps), 2)
        self.assertTrue(all(Path(step["log"]).read_text(encoding="utf-8") ==
                            "focused test passed\n" for step in runtime_steps))

    def test_focused_ctest_failure_stops_before_full_build_and_keeps_log(self):
        fake = FakeRunner(self.source, self.build)

        def runner(command, **kwargs):
            if command[0] == "ctest":
                kwargs["stdout"].write("focused test failed\n")
                return SimpleNamespace(returncode=8)
            return fake(command, **kwargs)

        with contextlib.redirect_stdout(io.StringIO()):
            receipt, code = build_candidate.build_grouped(
                self.source, self.build, self.logs, "runtime-fail", self.targets, {},
                runner=runner, preflight_ctests=["m2m.doe.sample"])
        self.assertEqual(code, 1)
        self.assertEqual(fake.counts, {"configure": 1, "preflight": 1, "full": 0})
        self.assertEqual(receipt["status"], "focused-runtime-failed")
        result = receipt["focused_runtime_preflight"]["results"][0]
        self.assertEqual(result["exit_code"], 8)
        self.assertEqual(Path(result["log"]).read_text(encoding="utf-8"),
                         "focused test failed\n")
        self.assertEqual([step["exit_code"] for step in receipt["steps"]], [0, 0, 8])

    def test_preflight_ctest_requires_grouped_full_mode(self):
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit) as raised:
                build_candidate.main(["--label", "invalid", "--preflight-ctest", "test.name"])
        self.assertEqual(raised.exception.code, 2)

    def test_grouped_cli_repeats_focused_ctests(self):
        analysis = self.root / "analysis"
        analysis.mkdir()
        (self.source / "apps/moproj2mst/CMakeLists.txt").write_text("", encoding="utf-8")
        fake = FakeRunner(self.source, self.build)
        fake_vcvars = SimpleNamespace(stdout="PATH=C:\\Windows\\System32\n")
        names = ["m2m.doe.sample", "m2m.mdt.sample"]
        ctest_commands = []

        def mocked_run(command, **kwargs):
            if isinstance(command, str) and command.startswith("cmd.exe"):
                return fake_vcvars
            if command[0] == "ctest":
                ctest_commands.append(list(command))
                kwargs["stdout"].write("focused passed\n")
                return SimpleNamespace(returncode=0)
            return fake(command, **kwargs)

        with patch.object(build_candidate, "ROOT", self.root), \
             patch.object(build_candidate, "BUILD", self.build), \
             patch.object(build_candidate, "VCVARS", self.root / "vcvars.bat"), \
             patch.object(build_candidate.subprocess, "run", side_effect=mocked_run), \
             contextlib.redirect_stdout(io.StringIO()):
            code = build_candidate.main([
                "--label", "grouped-ctest", "--preflight-target", self.targets[0],
                "--then-full", "--preflight-ctest", names[0],
                "--preflight-ctest", names[1]])
        receipt = json.loads((analysis / "grouped-ctest-build.json").read_text(encoding="utf-8"))
        self.assertEqual(code, 0)
        self.assertEqual([command[-1] for command in ctest_commands],
                         ["^m2m\\.doe\\.sample$", "^m2m\\.mdt\\.sample$"])
        self.assertEqual(receipt["focused_runtime_preflight"]["tests"], names)
        self.assertEqual(fake.counts, {"configure": 1, "preflight": 1, "full": 1})


if __name__ == "__main__":
    unittest.main()
