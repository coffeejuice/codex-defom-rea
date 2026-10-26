"""Synthetic filesystem/provenance tests only. No original or candidate process runs."""
import argparse
import datetime as dt
import hashlib
import json
from pathlib import Path
import shutil
import sys
import tempfile
import unittest

sys.dont_write_bytecode = True
import harness as h


class HarnessTests(unittest.TestCase):
    def setUp(self):
        h.STORE.mkdir(parents=True, exist_ok=True)
        self.root = Path(tempfile.mkdtemp(prefix="synthetic-", dir=h.STORE))
        self.fixture = self.root / "fixture"
        self.fixture.mkdir()
        (self.fixture / "p.moproj").write_text("<Project><Path>Objects/x.KEY</Path></Project>")
        (self.fixture / "Objects").mkdir()
        (self.fixture / "Objects/x.KEY").write_bytes(b"original input")
        (self.fixture / "p.MST").write_bytes(b"historical\r\n")
        (self.fixture / "empty").mkdir()
        self.source = self.root / "source"
        self.source.mkdir()
        (self.source / "main.cpp").write_text("// synthetic\n")
        self.original = self.root / "original.exe"
        self.original.write_bytes(b"synthetic reference identity; never executable")
        self.candidate = self.root / "candidate.exe"
        self.candidate.write_bytes(b"synthetic candidate identity; never executable")
        self.case = h.STORE / (self.root.name + "-case")
        self.args = argparse.Namespace(case_id=self.case.name, fixture=str(self.fixture), project="p.moproj",
            source=str(self.source), original_binary=str(self.original), candidate_binary=[str(self.candidate)],
            split="training", historical_oracle="synthetic historical provenance")
        h.prepare(self.args)

    def cfg(self):
        return h.read_json(self.case / "case.json")

    def observation(self, role="original", outcome="completed"):
        cfg = self.cfg()
        artifact = self.case / "evidence" / f"{role}.txt"
        artifact.parent.mkdir(exist_ok=True)
        artifact.write_text("SYNTHETIC, NOT RUNTIME EVIDENCE\n")
        executable = cfg["original_binary"] if role == "original" else cfg["candidate_binaries"][0]
        stamp = h.utc()
        obs = {"schema": h.SCHEMA, "role": role, "outcome": outcome,
               "session": {"fresh_state": True, "previous_projects": [], "project_count": 1,
                   "started_utc": stamp, "ended_utc": stamp, "pid": 1234,
                   "argv": [executable["path"]], "executable_sha256": executable["sha256"],
                   "cwd": str(self.case / "sandboxes" / role / "cwd"),
                   "environment": {"synthetic": True}, "settings": {"synthetic": True}, "exit_status": 0,
                   "save_mode": "full-save"},
               "actions": [{"action": x, "status": "completed", "evidence": f"evidence/{role}.txt"}
                           for x in ("open", "save", "close")], "dialogs": []}
        if outcome != "completed":
            obs["actions"] = [obs["actions"][0]]
        return obs

    def record(self, role="original", outcome="completed", obs=None):
        obs = obs or self.observation(role, outcome)
        path = self.root / (role + "-observation.json")
        path.write_text(json.dumps(obs))
        return h.record(argparse.Namespace(case=str(self.case), role=role, observation=str(path)))

    def compare(self, qualifications=None, name="raw.json"):
        return h.compare(argparse.Namespace(case=str(self.case), qualifications=qualifications, name=name))

    def test_preserves_inputs_and_executable_bytes(self):
        self.assertEqual(h.content(h.manifest(self.fixture)["entries"]),
                         h.content(h.manifest(self.case / "inputs")["entries"]))
        self.assertEqual(h.digest(self.case / "frozen-candidate/candidate.exe"), h.digest(self.candidate))
        self.assertTrue((self.case / "frozen-source/main.cpp").is_file())
        self.assertTrue((self.case / "inputs/empty").is_dir())

    def test_no_overwrite(self):
        with self.assertRaises(ValueError):
            h.prepare(self.args)

    def test_missing_reference_is_reported(self):
        (self.case / "inputs/Objects/x.KEY").unlink()
        with self.assertRaises(ValueError):
            h.preflight(argparse.Namespace(case=str(self.case)))
        scan = h.ref_scan(self.fixture / "p.moproj")
        self.assertTrue(scan["relative_references"][0]["exists"])
        (self.fixture / "Objects/x.KEY").unlink()
        self.assertFalse(h.ref_scan(self.fixture / "p.moproj")["relative_references"][0]["exists"])

    def test_external_reference_is_reported(self):
        p = self.root / "external.moproj"
        p.write_text('<Project><Path>C:\\PROBLEM\\original.KEY</Path><RestartKey>../outside.KEY</RestartKey></Project>')
        scan = h.ref_scan(p)
        self.assertEqual(len(scan["absolute_values"]), 1)
        self.assertTrue(scan["relative_references"][0]["escapes_project"])

    def test_source_mutation_invalidates_freeze(self):
        (self.source / "main.cpp").write_text("changed")
        with self.assertRaises(ValueError):
            h.preflight(argparse.Namespace(case=str(self.case)))

    def test_frozen_binary_mutation_invalidates_freeze(self):
        (self.case / "frozen-candidate/candidate.exe").write_bytes(b"changed")
        with self.assertRaises(ValueError):
            h.preflight(argparse.Namespace(case=str(self.case)))

    def test_build_receipt_is_hash_bound_when_supplied(self):
        receipt = self.root / "build.json"
        receipt.write_text('{"qualification":"synthetic unverified build"}')
        self.args.case_id += "-receipt"
        self.args.build_receipt = str(receipt)
        self.case = h.STORE / self.args.case_id
        h.prepare(self.args)
        self.assertEqual(self.cfg()["build_receipt"]["sha256"], h.digest(receipt))
        self.assertEqual(self.cfg()["source_binary_binding"]["status"], "unverified_unsupported")
        receipt.write_text("changed")
        # The retained build receipt survives changes to the coordinator's source receipt.
        h.preflight(argparse.Namespace(case=str(self.case)))
        Path(self.cfg()["build_receipt"]["path"]).write_text("tampered frozen receipt")
        with self.assertRaises(ValueError):
            h.preflight(argparse.Namespace(case=str(self.case)))

    def build_receipt(self):
        # Independent expected framing for the one-file fixture, rather than reusing the implementation.
        file_bytes = (self.source / "main.cpp").read_bytes()
        source_digest = hashlib.sha256(b"main.cpp\0" + hashlib.sha256(file_bytes).digest()).hexdigest()
        return {"schema": "m2m-candidate-build-v1", "source_tree_sha256": source_digest,
                "source_file_count": 1, "source_tree_after_sha256": source_digest,
                "source_file_count_after": 1, "source_stable_during_build": True,
                "targets": None, "steps": [
                    {"step": "configure", "command": ["cmake", "-S", str(self.source)], "exit_code": 0},
                    {"step": "build", "command": ["cmake", "--build", "synthetic"], "exit_code": 0}],
                "artifacts": [{"path": str(self.candidate), "bytes": self.candidate.stat().st_size,
                               "sha256": h.digest(self.candidate)}]}

    def bind(self, receipt):
        return h.build_binding(receipt, h.manifest(self.source)["entries"], [h.identity(self.candidate)])

    def test_build_binding_source_and_binary_match(self):
        receipt = self.root / "supported-build.json"
        receipt.write_text(json.dumps(self.build_receipt()))
        self.args.case_id += "-v1"
        self.args.build_receipt = str(receipt)
        self.case = h.STORE / self.args.case_id
        h.prepare(self.args)
        binding = self.cfg()["source_binary_binding"]
        self.assertEqual(binding["status"], "verified_receipt_consistency")
        self.assertTrue(binding["full_build"])
        self.assertTrue(binding["source_endpoints_match"])
        h.preflight(argparse.Namespace(case=str(self.case)))

    def test_build_binding_absent_is_explicit(self):
        self.assertEqual(self.cfg()["source_binary_binding"]["status"], "unverified_absent")

    def test_build_binding_legacy_is_unverified(self):
        receipt = self.build_receipt()
        receipt.pop("schema")
        self.assertEqual(self.bind(receipt)["status"], "unverified_unsupported")

    def test_build_binding_source_digest_mismatch_rejected(self):
        receipt = self.build_receipt()
        receipt["source_tree_sha256"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "source digest mismatch"):
            self.bind(receipt)

    def test_build_binding_source_count_mismatch_rejected(self):
        receipt = self.build_receipt()
        receipt["source_file_count"] = 2
        with self.assertRaisesRegex(ValueError, "file count mismatch"):
            self.bind(receipt)

    def test_build_binding_changed_binary_rejected(self):
        receipt = self.build_receipt()
        self.candidate.write_bytes(b"different build output")
        with self.assertRaisesRegex(ValueError, "binary identity mismatch"):
            self.bind(receipt)

    def test_build_binding_same_named_binary_at_different_path_rejected(self):
        receipt = self.build_receipt()
        receipt["artifacts"][0]["path"] = str(self.root / "other/candidate.exe")
        with self.assertRaisesRegex(ValueError, "binary identity mismatch"):
            self.bind(receipt)

    def test_build_binding_failed_build_rejected(self):
        receipt = self.build_receipt()
        receipt["steps"][1]["exit_code"] = 1
        with self.assertRaisesRegex(ValueError, "successful configure and build"):
            self.bind(receipt)

    def test_build_binding_partial_build_rejected(self):
        receipt = self.build_receipt()
        receipt["targets"] = ["moproj2mst_cli"]
        with self.assertRaisesRegex(ValueError, "full build"):
            self.bind(receipt)

    def test_build_binding_targeted_command_rejected(self):
        receipt = self.build_receipt()
        receipt["steps"][1]["command"].extend(["--target", "moproj2mst_cli"])
        with self.assertRaisesRegex(ValueError, "targeted"):
            self.bind(receipt)

    def test_build_binding_inline_target_rejected(self):
        receipt = self.build_receipt()
        receipt["steps"][1]["command"].append("--target=moproj2mst_cli")
        with self.assertRaisesRegex(ValueError, "targeted"):
            self.bind(receipt)

    def test_build_source_digest_framing_and_native_path_order(self):
        (self.source / "a2.txt").write_bytes(b"second")
        (self.source / "A.txt").write_bytes(b"first")
        (self.source / "dir").mkdir()
        (self.source / "dir/B.txt").write_bytes(b"nested")
        # Source paths use native Windows Path comparison, and directories do not enter the digest.
        order = ["A.txt", "a2.txt", "dir/B.txt", "main.cpp"]
        expected = hashlib.sha256()
        for name in order:
            expected.update(name.encode("utf-8") + b"\0" + hashlib.sha256((self.source / name).read_bytes()).digest())
        actual = h.build_source_fingerprint(h.manifest(self.source)["entries"])
        self.assertEqual(actual["sha256"], expected.hexdigest())
        self.assertEqual(actual["file_count"], 4)

    def test_build_binding_missing_build_step_rejected(self):
        receipt = self.build_receipt()
        receipt["steps"].pop()
        with self.assertRaisesRegex(ValueError, "successful configure and build"):
            self.bind(receipt)

    def test_build_binding_unstable_source_rejected(self):
        receipt = self.build_receipt()
        receipt["source_stable_during_build"] = False
        with self.assertRaisesRegex(ValueError, "stable source endpoints"):
            self.bind(receipt)

    def test_build_binding_changed_after_digest_rejected(self):
        receipt = self.build_receipt()
        receipt["source_tree_after_sha256"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "stable source endpoints"):
            self.bind(receipt)

    def test_build_binding_missing_after_fields_rejected(self):
        receipt = self.build_receipt()
        receipt.pop("source_file_count_after")
        with self.assertRaisesRegex(ValueError, "stable source endpoints"):
            self.bind(receipt)

    def test_root_symlink_is_rejected(self):
        link = self.root / "linked-fixture"
        try:
            link.symlink_to(self.fixture, target_is_directory=True)
        except OSError as e:
            self.skipTest(f"symlink creation unavailable: {e}")
        with self.assertRaises(ValueError):
            h.manifest(link)

    def test_candidate_full_save_required(self):
        self.record()
        obs = self.observation("candidate")
        obs["session"].pop("save_mode")
        with self.assertRaises(ValueError):
            self.record("candidate", obs=obs)

    def test_candidate_before_original_rejected(self):
        with self.assertRaises(ValueError):
            self.record("candidate")

    def test_changed_candidate_before_reference_rejected(self):
        (self.case / "sandboxes/candidate/project/p.MST").write_bytes(b"changed")
        with self.assertRaises(ValueError):
            self.record()

    def test_fresh_session_required(self):
        obs = self.observation()
        obs["session"]["previous_projects"] = ["previous.moproj"]
        with self.assertRaises(ValueError):
            self.record(obs=obs)

    def test_default_answer_evidence_required(self):
        obs = self.observation()
        obs["dialogs"] = [{"stage": "save", "observed_default": "Yes", "selected": "No",
                            "evidence": "evidence/original.txt"}]
        with self.assertRaises(ValueError):
            self.record(obs=obs)

    def test_missing_runtime_artifact_rejected(self):
        obs = self.observation()
        obs["actions"][0]["evidence"] = "evidence/missing.txt"
        with self.assertRaises(ValueError):
            self.record(obs=obs)

    def test_completed_requires_close(self):
        obs = self.observation()
        obs["actions"].pop()
        with self.assertRaises(ValueError):
            self.record(obs=obs)

    def test_identity_and_manifest_tampering_rejected(self):
        (self.case / "original-before.json").write_text("{}")
        with self.assertRaises(ValueError):
            self.record()

    def test_failed_reference_keeps_partial_effects(self):
        path = self.case / "sandboxes/original/project"
        (path / "p.MST").write_bytes(b"partial write")
        (path / "Objects/x.KEY").unlink()
        (path / "created.INI").write_bytes(b"created")
        result = self.record(outcome="crashed")
        receipt = h.read_json(result["receipt"]["path"])
        self.assertIn("project/p.MST", receipt["effects"]["changed"])
        self.assertIn("project/Objects/x.KEY", receipt["effects"]["removed"])
        self.assertIn("project/created.INI", receipt["effects"]["added"])
        self.assertEqual((self.case / "captures/original/files/project/p.MST").read_bytes(), b"partial write")
        self.assertEqual((self.fixture / "p.MST").read_bytes(), b"historical\r\n")

    def test_candidate_timestamp_must_follow_reference_receipt(self):
        obs = self.observation("candidate")
        self.record()
        with self.assertRaises(ValueError):
            self.record("candidate", obs=obs)

    def test_equal_bytes_not_automatic_acceptance(self):
        self.record()
        self.record("candidate")
        result = self.compare()
        self.assertTrue(result["raw_content_equal"])
        self.assertIn("never overall acceptance", h.read_json(result["receipt"]["path"])["qualification"])

    def test_changed_capture_is_rejected(self):
        self.record()
        self.record("candidate")
        (self.case / "captures/original/files/project/p.MST").write_bytes(b"tampered")
        with self.assertRaises(ValueError):
            self.compare()

    def test_qualified_comparison_retains_raw_mismatch(self):
        (self.case / "sandboxes/original/project/p.MST").write_bytes(b"KFREAD C:/old/key\r\n")
        self.record()
        (self.case / "sandboxes/candidate/project/p.MST").write_bytes(b"KFREAD D:/new/key\r\n")
        self.record("candidate")
        a = self.case / "captures/original/files/project/p.MST"
        b = self.case / "captures/candidate/files/project/p.MST"
        rule = {"path": "project/p.MST", "original_sha256": h.digest(a), "candidate_sha256": h.digest(b),
                "reason": "synthetic exact path qualification", "evidence": ["evidence/original.txt"],
                "original": [{"offset": 7, "old_hex": b"C:/old".hex(), "canonical_hex": b"<ROOT>".hex()}],
                "candidate": [{"offset": 7, "old_hex": b"D:/new".hex(), "canonical_hex": b"<ROOT>".hex()}]}
        path = self.root / "rules.json"
        path.write_text(json.dumps([rule]))
        result = self.compare(str(path))
        report = h.read_json(result["receipt"]["path"])
        self.assertFalse(report["raw_content_equal"])
        self.assertTrue(report["qualified_views"][0]["equal"])
        self.assertIn("project/p.MST", report["raw"]["changed"])

    def test_bad_qualification_rejected(self):
        data = b"one"
        rule = {"original_sha256": h.object_hash(None), "original": []}
        with self.assertRaises(ValueError):
            h.qualified_bytes(data, "original", rule)

    def test_outputs_outside_store_rejected(self):
        with self.assertRaises(ValueError):
            h.write_json(h.ROOT / "analysis/forbidden-test.json", {})

    def test_external_runtime_root_keeps_receipts_local_and_projects_isolated(self):
        problem = Path(tempfile.mkdtemp(prefix="synthetic-problem-", dir=h.ROOT / "build"))
        self.addCleanup(shutil.rmtree, problem, True)
        runtime = problem / "m2m005"
        old_problem, old_limit = h.PROBLEM_ROOT, h.MAX_RUNTIME_PATH_CHARS
        h.PROBLEM_ROOT, h.MAX_RUNTIME_PATH_CHARS = problem, 1000
        try:
            external_case = h.STORE / (self.root.name + "-runtime-case")
            args = argparse.Namespace(case_id=external_case.name, fixture=str(self.fixture), project="p.moproj",
                source=str(self.source), original_binary=str(self.original), candidate_binary=[str(self.candidate)],
                build_receipt=None, split="training", historical_oracle="synthetic",
                runtime_root=str(runtime))
            h.prepare(args)
            cfg = h.read_json(external_case / "case.json")
            self.assertEqual(Path(cfg["runtime_sandboxes"]["original"]["project_file"]).parent.parent,
                             runtime / "original")
            self.assertNotEqual(cfg["runtime_sandboxes"]["original"]["sandbox"],
                                cfg["runtime_sandboxes"]["candidate"]["sandbox"])
            self.assertTrue((external_case / "original-runtime-before.json").is_file())
            self.assertFalse((Path(cfg["runtime_sandboxes"]["original"]["sandbox"]) / "original-runtime-before.json").exists())
            preflight = h.preflight(argparse.Namespace(case=str(external_case)))
            self.assertTrue(preflight["freeze_intact"])
            self.assertTrue(str(external_case).startswith(str(h.STORE)))
            self.assertTrue(str(runtime).startswith(str(problem)))

            def runtime_observation(role):
                artifact = external_case / "evidence" / f"{role}.txt"
                artifact.parent.mkdir(exist_ok=True)
                artifact.write_text("SYNTHETIC, NOT RUNTIME EVIDENCE\n")
                executable = cfg["original_binary"] if role == "original" else cfg["candidate_binaries"][0]
                stamp = h.utc()
                return {"schema": h.SCHEMA, "role": role, "outcome": "completed",
                    "session": {"fresh_state": True, "previous_projects": [], "project_count": 1,
                        "started_utc": stamp, "ended_utc": stamp, "pid": 1234,
                        "argv": ([executable["path"], cfg["runtime_sandboxes"][role]["project_file"], "--write-save"]
                                if role == "candidate" else [executable["path"]]),
                        "project_path": cfg["runtime_sandboxes"][role]["project_file"],
                        "executable_sha256": executable["sha256"],
                        "cwd": cfg["runtime_sandboxes"][role]["cwd"], "environment": {"synthetic": True},
                        "settings": {"synthetic": True}, "exit_status": 0, "save_mode": "full-save"},
                    "actions": [{"action": action, "status": "completed", "evidence": f"evidence/{role}.txt"}
                                for action in ("open", "save", "close")], "dialogs": [],
                    "artifacts": [f"evidence/{role}.txt"]}

            original_project = Path(cfg["runtime_sandboxes"]["original"]["project"])
            (original_project / "p.MST").write_bytes(b"original external runtime result")
            obs_path = self.root / "external-original-observation.json"
            obs_path.write_text(json.dumps(runtime_observation("original")))
            h.record(argparse.Namespace(case=str(external_case), role="original", observation=str(obs_path)))
            candidate_path = self.root / "external-candidate-observation.json"
            candidate_observation = runtime_observation("candidate")
            wrong_project = dict(candidate_observation)
            wrong_project["session"] = dict(candidate_observation["session"])
            wrong_project["session"]["argv"] = [cfg["candidate_binaries"][0]["path"],
                                                    str(external_case / "inputs" / "p.moproj"), "--write-save"]
            with self.assertRaisesRegex(ValueError, "candidate argv project"):
                h.validate_observation(external_case, cfg, "candidate", wrong_project)
            wrong_project["session"]["argv"] = [cfg["candidate_binaries"][0]["path"],
                                                    cfg["runtime_sandboxes"]["candidate"]["project_file"], "--write-save"]
            wrong_project["session"]["project_path"] = str(external_case / "inputs" / "p.moproj")
            with self.assertRaisesRegex(ValueError, "observed project path"):
                h.validate_observation(external_case, cfg, "candidate", wrong_project)
            candidate_path.write_text(json.dumps(candidate_observation))
            h.record(argparse.Namespace(case=str(external_case), role="candidate", observation=str(candidate_path)))
            compared = h.compare(argparse.Namespace(case=str(external_case), qualifications=None, name="raw.json"))
            result = h.read_json(compared["receipt"]["path"])
            self.assertIn("project/p.MST", result["raw"]["changed"])
            self.assertIn("project/p.MST", result["first_differences"])
        finally:
            h.PROBLEM_ROOT, h.MAX_RUNTIME_PATH_CHARS = old_problem, old_limit

    def test_runtime_root_rejects_outside_overlap_and_long_paths(self):
        problem = Path(tempfile.mkdtemp(prefix="synthetic-problem-", dir=h.ROOT / "build"))
        self.addCleanup(shutil.rmtree, problem, True)
        old_problem, old_limit = h.PROBLEM_ROOT, h.MAX_RUNTIME_PATH_CHARS
        h.PROBLEM_ROOT, h.MAX_RUNTIME_PATH_CHARS = problem, 100
        try:
            args = argparse.Namespace(case_id="x", fixture=str(self.fixture), project="p.moproj",
                source=str(self.source), original_binary=str(self.original), candidate_binary=[str(self.candidate)],
                build_receipt=None, split="training", historical_oracle="synthetic")
            original = h.identity(self.original)
            candidates = [h.identity(self.candidate)]
            with self.assertRaisesRegex(ValueError, "strictly below"):
                h.runtime_layout(str(self.root / "outside"), self.case, self.fixture, self.source,
                                 original, candidates, Path("p.moproj"))
            protected_source = problem / "protected-source"
            protected_source.mkdir()
            with self.assertRaisesRegex(ValueError, "overlaps protected"):
                h.runtime_layout(str(protected_source / "runtime"), self.case, self.fixture,
                                 protected_source, original, candidates, Path("p.moproj"))
            long_root = problem / ("x" * 75)
            with self.assertRaisesRegex(ValueError, "shorter than"):
                h.runtime_layout(str(long_root), self.case, self.fixture, self.source,
                                 original, candidates, Path("p.moproj"))
        finally:
            h.PROBLEM_ROOT, h.MAX_RUNTIME_PATH_CHARS = old_problem, old_limit

    def test_external_runtime_cwd_mismatch_and_symlink_rejected(self):
        problem = Path(tempfile.mkdtemp(prefix="synthetic-problem-", dir=h.ROOT / "build"))
        self.addCleanup(shutil.rmtree, problem, True)
        runtime = problem / "m2m005"
        old_problem, old_limit = h.PROBLEM_ROOT, h.MAX_RUNTIME_PATH_CHARS
        h.PROBLEM_ROOT, h.MAX_RUNTIME_PATH_CHARS = problem, 1000
        try:
            external_case = h.STORE / (self.root.name + "-runtime-validation")
            args = argparse.Namespace(case_id=external_case.name, fixture=str(self.fixture), project="p.moproj",
                source=str(self.source), original_binary=str(self.original), candidate_binary=[str(self.candidate)],
                build_receipt=None, split="training", historical_oracle="synthetic", runtime_root=str(runtime))
            h.prepare(args)
            cfg = h.read_json(external_case / "case.json")
            observation = self.observation()
            observation["session"]["cwd"] = str(external_case / "sandboxes/original/cwd")
            with self.assertRaisesRegex(ValueError, "execution cwd"):
                h.validate_observation(external_case, cfg, "original", observation)
            link = Path(cfg["runtime_sandboxes"]["original"]["sandbox"]) / "linked"
            try:
                link.symlink_to(self.fixture, target_is_directory=True)
            except OSError:
                self.skipTest("symlink creation unavailable")
            with self.assertRaisesRegex(ValueError, "link/reparse point"):
                h.verify_runtime_layout(external_case, cfg)
        finally:
            h.PROBLEM_ROOT, h.MAX_RUNTIME_PATH_CHARS = old_problem, old_limit

    def test_external_runtime_preflight_rejects_preexisting_changes(self):
        problem = Path(tempfile.mkdtemp(prefix="synthetic-problem-", dir=h.ROOT / "build"))
        self.addCleanup(shutil.rmtree, problem, True)
        runtime = problem / "m2m005"
        old_problem, old_limit = h.PROBLEM_ROOT, h.MAX_RUNTIME_PATH_CHARS
        h.PROBLEM_ROOT, h.MAX_RUNTIME_PATH_CHARS = problem, 1000
        try:
            external_case = h.STORE / (self.root.name + "-runtime-preflight")
            args = argparse.Namespace(case_id=external_case.name, fixture=str(self.fixture), project="p.moproj",
                source=str(self.source), original_binary=str(self.original), candidate_binary=[str(self.candidate)],
                build_receipt=None, split="training", historical_oracle="synthetic", runtime_root=str(runtime))
            h.prepare(args)
            cfg = h.read_json(external_case / "case.json")
            Path(cfg["runtime_sandboxes"]["candidate"]["project"], "p.MST").write_bytes(b"unexpected preflight write")
            with self.assertRaisesRegex(ValueError, "runtime candidate tree changed"):
                h.preflight(argparse.Namespace(case=str(external_case)))
        finally:
            h.PROBLEM_ROOT, h.MAX_RUNTIME_PATH_CHARS = old_problem, old_limit

    def test_post_run_runtime_paths_enforce_absolute_length_cap(self):
        old_limit = h.MAX_RUNTIME_PATH_CHARS
        h.MAX_RUNTIME_PATH_CHARS = 100
        try:
            short_root = Path("C:/PROBLEM/m2m005/original")
            h.validate_runtime_path_lengths(short_root, {"project/BILLET.moproj": {"kind": "file"}})
            too_long_rel = "project/" + ("x" * 100) + ".KEY"
            with self.assertRaisesRegex(ValueError, "runtime output path"):
                h.validate_runtime_path_lengths(short_root, {too_long_rel: {"kind": "file"}})
        finally:
            h.MAX_RUNTIME_PATH_CHARS = old_limit


if __name__ == "__main__":
    unittest.main()
