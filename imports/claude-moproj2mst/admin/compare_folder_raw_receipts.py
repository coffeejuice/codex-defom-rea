"""Read-only comparison of two complete schema-1 folder raw receipt directories.

Exit 0: all recorded semantics and raw file hashes agree; 1: differences;
2: missing, malformed or inconsistent evidence. Never infer path-only file
differences from SHA-256 values. No converter, fixture or original app is run.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath, PureWindowsPath
import re
import sys


MANIFEST_KEYS = {"schema", "mode", "coordinator_pid", "executable", "executable_sha256",
                 "eligible_cases", "cases", "complete"}
IDENTITY_KEYS = {"ordinal", "case", "receipt", "sha256", "expected", "work"}
CASE_KEYS = {"schema", "mode", "case", "ordinal", "writer_pid", "worker", "expected", "work",
             "state", "converter_called", "converter_code", "message", "work_exists", "generated_files",
             "decisions", "differ_before_older_window", "differ", "missing", "extra", "expected_older_window",
             "moproj_differs", "banner_only_files", "complete"}
VECTORS = ("differ_before_older_window", "differ", "missing", "extra")
STAGES = {"raw_equal", "relocated_equal", "masked_equal", "banner_only", "different", "missing", "extra"}
SHA = re.compile(r"[0-9a-f]{64}\Z")


class EvidenceError(ValueError):
    pass


def require(condition, message):
    if not condition:
        raise EvidenceError(message)


def integer(value, minimum=None):
    return type(value) is int and (minimum is None or value >= minimum)


def unique_object(pairs):
    result = {}
    for key, value in pairs:
        require(key not in result, f"Duplicate JSON key: {key}")
        result[key] = value
    return result


def read_document(path):
    raw = path.read_bytes()
    doc = json.loads(raw.decode("utf-8"), object_pairs_hook=unique_object,
                     parse_constant=lambda value: (_ for _ in ()).throw(EvidenceError(f"Invalid JSON number: {value}")))
    require(isinstance(doc, dict), f"Not a JSON object: {path}")
    return doc, hashlib.sha256(raw).hexdigest()


def relative(value):
    return (isinstance(value, str) and bool(value) and "\\" not in value and
            not PureWindowsPath(value).is_absolute() and not PureWindowsPath(value).drive and
            not PurePosixPath(value).is_absolute() and all(p not in {"", ".", ".."} for p in value.split("/")))


def absolute(value):
    return isinstance(value, str) and PureWindowsPath(value).is_absolute()


def validate_case(case, identity, manifest):
    name = identity["case"]
    expected_keys = CASE_KEYS | ({"prefetch"} if "prefetch" in manifest else set())
    require(set(case) == expected_keys, f"{name}: unknown/missing case fields")
    if "prefetch" in manifest:
        require(type(case["prefetch"]) is bool and case["prefetch"] == manifest["prefetch"], f"{name}: prefetch provenance mismatch")
    require(type(case["schema"]) is int and case["schema"] == 1 and case["complete"] is True,
            f"{name}: incomplete/unknown case schema")
    for key in ("ordinal", "case", "expected", "work"):
        require(case[key] == identity[key], f"{name}: manifest/case {key} mismatch")
    require(case["mode"] == manifest["mode"], f"{name}: mode mismatch")
    require(integer(case["writer_pid"], 1), f"{name}: invalid writer PID")
    require(case["state"] in {"compared", "copy_failed", "refused"}, f"{name}: unknown terminal state")
    for key in ("converter_called", "work_exists", "expected_older_window", "moproj_differs"):
        require(type(case[key]) is bool, f"{name}: invalid {key}")
    require(integer(case["converter_code"]) and isinstance(case["message"], str), f"{name}: invalid converter result")
    require(integer(case["banner_only_files"], 0), f"{name}: invalid banner count")
    require(case["converter_called"] == (case["state"] != "copy_failed"), f"{name}: converter-called mismatch")
    require((case["converter_code"] == 0) == (case["state"] != "refused"), f"{name}: converter state/code mismatch")
    if manifest["mode"] == "serial":
        require(case["worker"] == "serial" and case["writer_pid"] == manifest["coordinator_pid"], f"{name}: invalid serial writer")
    elif case["state"] == "compared":
        require(case["worker"] == "pipeline-post" and case["writer_pid"] != manifest["coordinator_pid"], f"{name}: invalid post writer")
    else:
        require(case["worker"] == "pipeline-coordinator" and case["writer_pid"] == manifest["coordinator_pid"], f"{name}: invalid coordinator writer")
    files = case["generated_files"]
    require(isinstance(files, list), f"{name}: invalid file inventory")
    paths = []
    for entry in files:
        require(isinstance(entry, dict) and set(entry) == {"relative", "size", "sha256"}, f"{name}: invalid file entry")
        require(relative(entry["relative"]) and integer(entry["size"], 0) and isinstance(entry["sha256"], str) and SHA.fullmatch(entry["sha256"]), f"{name}: invalid path/size/hash")
        paths.append(entry["relative"])
    require(paths == sorted(set(paths)), f"{name}: unsorted/duplicate generated paths")
    require(case["work_exists"] or not files, f"{name}: files in absent work directory")
    decisions = case["decisions"]
    require(isinstance(decisions, list), f"{name}: invalid decisions")
    keys = []
    by_stage = {stage: [] for stage in STAGES}
    for entry in decisions:
        require(isinstance(entry, dict) and set(entry) == {"relative_key", "stage"}, f"{name}: invalid decision entry")
        require(relative(entry["relative_key"]) and entry["stage"] in STAGES, f"{name}: invalid decision key/stage")
        keys.append(entry["relative_key"])
        by_stage[entry["stage"]].append(entry["relative_key"])
    require(len(keys) == len(set(keys)), f"{name}: duplicate decision key")
    for key in VECTORS:
        require(isinstance(case[key], list) and all(relative(v) for v in case[key]), f"{name}: invalid {key}")
        require(case[key] == sorted(set(case[key])), f"{name}: duplicate/unsorted {key}")
    require(case["missing"] == by_stage["missing"] and case["extra"] == by_stage["extra"] and
            case["differ_before_older_window"] == by_stage["different"], f"{name}: truncated/inconsistent decision vectors")
    require(case["banner_only_files"] == len(by_stage["banner_only"]), f"{name}: inconsistent banner count")
    if case["state"] == "compared":
        def lower_ascii(value):
            return value.translate(str.maketrans("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyz"))
        compared_paths = {lower_ascii(path) for path in paths
                          if PurePosixPath(path).suffix not in {".png", ".oracleA"}
                          and PurePosixPath(path).name != "view_state.json"}
        present_keys = {entry["relative_key"] for entry in decisions if entry["stage"] != "missing"}
        require(case["work_exists"] and present_keys == compared_paths and
                not (set(by_stage["missing"]) & compared_paths), f"{name}: incomplete generated-file decision coverage")
        ordinary = [entry["relative_key"] for entry in decisions if entry["stage"] != "extra"]
        require(ordinary == sorted(ordinary) and by_stage["extra"] == sorted(by_stage["extra"]), f"{name}: unordered decisions")
        require(case["moproj_differs"] == any(PurePosixPath(key).suffix == ".moproj" for key in by_stage["different"]), f"{name}: inconsistent moproj flag")
    if case["expected_older_window"]:
        require(not case["differ"] and len(case["differ_before_older_window"]) == 1 and
                case["moproj_differs"] and not case["missing"] and not case["extra"], f"{name}: invalid older-window exception")
    else:
        require(case["differ"] == case["differ_before_older_window"], f"{name}: truncated differ vector")
    if case["state"] != "compared":
        require(not decisions and all(not case[key] for key in VECTORS) and not case["expected_older_window"] and
                not case["moproj_differs"] and case["banner_only_files"] == 0, f"{name}: comparison data in un-compared case")


def load_capture(directory):
    directory = directory.resolve(strict=True)
    require(directory.is_dir(), f"Not a receipt directory: {directory}")
    manifest, manifest_hash = read_document(directory / "complete.json")
    require(set(manifest) in (MANIFEST_KEYS, MANIFEST_KEYS | {"prefetch"}), "Unknown/missing completion fields")
    if "prefetch" in manifest:
        require(type(manifest["prefetch"]) is bool, "Invalid prefetch provenance")
    require(type(manifest["schema"]) is int and manifest["schema"] == 1 and manifest["complete"] is True, "Incomplete/unknown completion schema")
    require(manifest["mode"] in {"serial", "pipeline"} and integer(manifest["coordinator_pid"], 1), "Invalid mode/coordinator")
    require(absolute(manifest["executable"]) and isinstance(manifest["executable_sha256"], str) and SHA.fullmatch(manifest["executable_sha256"]), "Invalid executable identity")
    identities = manifest["cases"]
    require(isinstance(identities, list) and integer(manifest["eligible_cases"], 1) and len(identities) == manifest["eligible_cases"], "Missing/truncated/empty case membership")
    cases, artifacts, seen_names, expected_files, work_root = [], {"complete.json": manifest_hash}, set(), {"complete.json"}, None
    helper_pids = set()
    for ordinal, identity in enumerate(identities, 1):
        require(isinstance(identity, dict) and set(identity) == IDENTITY_KEYS, "Invalid manifest case identity")
        name = identity["case"]
        require(isinstance(name, str) and bool(name) and name not in seen_names, "Missing/duplicate case name")
        seen_names.add(name)
        require(type(identity["ordinal"]) is int and identity["ordinal"] == ordinal and identity["receipt"] == f"{ordinal}.json", f"{name}: missing/duplicate/out-of-order ordinal")
        require(absolute(identity["expected"]) and absolute(identity["work"]), f"{name}: invalid absolute path")
        work = PureWindowsPath(identity["work"])
        require(work.name == str(ordinal) and work.parent.name == f"m2m_folder_sweep_{manifest['coordinator_pid']}", f"{name}: work path not bound to ordinal/coordinator")
        if work_root is None:
            work_root = work.parent
        require(work.parent == work_root, f"{name}: mixed work roots")
        filename = identity["receipt"]
        expected_files.add(filename)
        case, case_hash = read_document(directory / filename)
        require(identity["sha256"] == case_hash, f"{name}: case SHA-256 mismatch")
        artifacts[filename] = case_hash
        validate_case(case, identity, manifest)
        if case["worker"] == "pipeline-post":
            helper_pids.add(case["writer_pid"])
        cases.append(case)
    require(len(helper_pids) <= 1, "Multiple pipeline post writer identities")
    entries = list(directory.iterdir())
    require(all(entry.is_file() and not entry.is_symlink() for entry in entries) and {entry.name for entry in entries} == expected_files, "Unexpected/missing/nonregular/unfinished receipt files")
    return {"directory": directory, "manifest": manifest, "cases": cases, "artifacts": artifacts, "work_root": str(work_root)}


def work_message(message, case):
    # Replace only this case's complete verified work prefix at a path boundary.
    parts = re.split(r"[/\\]", case["work"].rstrip("/\\"))
    pattern = r"[/\\]".join(re.escape(part) for part in parts)
    return re.sub(pattern + r"(?=$|[/\\\s\"'<>;)])", lambda _: "<CASE_WORK>", message, flags=re.IGNORECASE)


def load_capture98(directory, run):
    directory = directory.resolve(strict=True)
    names = {"capture.tsv", "generated-Gear_blank.MST", "expected-Gear_blank.MST"}
    entries = list(directory.iterdir())
    require({entry.name for entry in entries} == names and
            all(entry.is_file() and not entry.is_symlink() for entry in entries), "Invalid capture98 directory membership")
    receipt_path = directory / "capture.tsv"
    fields = {}
    for line in receipt_path.read_text(encoding="utf-8").splitlines():
        pieces = line.split("\t")
        require(len(pieces) == 2 and pieces[0] not in fields, "Malformed/duplicate capture98 TSV field")
        fields[pieces[0]] = pieces[1]
    require(set(fields) == {"schema", "case", "worker", "pid", "ordinal", "input", "expected", "work", "generated_relative"}, "Missing/unknown capture98 fields")
    require(fields["schema"] == "M2M_FOLDER_CAPTURE_98_1" and fields["case"] == "B:98_B_DS3F" and
            fields["generated_relative"] == "Gear_blank.MST", "Not the approved capture98 case/file")
    case = next((case for case in run["cases"] if case["case"] == fields["case"]), None)
    require(case is not None and case["state"] == "compared", "Capture98 lacks compared raw case")
    manifest = run["manifest"]
    require(fields["pid"] == str(manifest["coordinator_pid"]) and fields["ordinal"] == str(case["ordinal"]) and
            fields["work"] == case["work"] and fields["expected"] == case["expected"], "Capture98 is not bound to this raw run's identity")
    require(fields["worker"] == ("serial" if manifest["mode"] == "serial" else "pipeline-coordinator") and
            absolute(fields["input"]), "Invalid capture98 coordinator/input identity")
    require(re.fullmatch(r"m2m_folder_capture_" + str(manifest["coordinator_pid"]) + r"_[0-9]+", directory.name), "Capture98 directory PID identity mismatch")
    generated = next((entry for entry in case["generated_files"] if entry["relative"] == "Gear_blank.MST"), None)
    require(generated is not None, "Capture98 file absent from raw inventory")
    data = (directory / "generated-Gear_blank.MST").read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    require(len(data) == generated["size"] and digest == generated["sha256"], "Capture98 bytes do not match this raw run's generated-file SHA/size")
    require(b"<WORK>" not in data, "Ambiguous existing capture98 normalization token")
    work = case["work"].encode("utf-8")
    normalized, count = re.subn(re.escape(work) + rb"(?=$|[/\\\s\"'<>;)])", b"<WORK>", data)
    require(count > 0, "Capture98 contains no exact metadata-bound work prefix")
    expected = (directory / "expected-Gear_blank.MST").read_bytes()
    return {"directory": str(directory), "case": fields["case"], "ordinal": case["ordinal"],
            "work": case["work"], "input": fields["input"], "fields": fields,
            "generated_sha256": digest, "expected_sha256": hashlib.sha256(expected).hexdigest(),
            "capture_tsv_sha256": hashlib.sha256(receipt_path.read_bytes()).hexdigest(),
            "normalized_sha256": hashlib.sha256(normalized).hexdigest(), "replacements": count,
            "normalized": normalized}


def compare_captures(a, b, capture98=None):
    blockers, permitted, raw = [], [], []
    ma, mb = a["manifest"], b["manifest"]
    for key in ("eligible_cases", "executable_sha256"):
        if ma[key] != mb[key]:
            blockers.append(f"completion.{key} differs")
    if PureWindowsPath(ma["executable"]).name != PureWindowsPath(mb["executable"]).name:
        blockers.append("Executable filename differs")
    for key in ("mode", "coordinator_pid", "executable", "prefetch"):
        if ma.get(key) != mb.get(key):
            permitted.append(f"Execution provenance differs: completion.{key}")
    if a["work_root"] != b["work_root"]:
        permitted.append("Verified coordinator-owned work root differs; only identity paths and exact message prefix are qualified")
    names_a = [(c["ordinal"], c["case"]) for c in a["cases"]]
    names_b = [(c["ordinal"], c["case"]) for c in b["cases"]]
    if names_a != names_b:
        blockers.append("Ordered case membership differs")
    by_b = {case["case"]: case for case in b["cases"]}
    for old in a["cases"]:
        new = by_b.get(old["case"])
        if new is None:
            continue
        name = old["case"]
        for key in CASE_KEYS - {"mode", "writer_pid", "worker", "work", "message", "generated_files"}:
            if old[key] != new[key]:
                blockers.append(f"{name}: {key} differs")
        for key in ("mode", "writer_pid", "worker", "work", "prefetch"):
            if old.get(key) != new.get(key):
                permitted.append(f"{name}: validated execution provenance differs: {key}")
        if old["message"] != new["message"]:
            path_only = work_message(old["message"], old) == work_message(new["message"], new)
            raw.append({"kind": "converter_message", "case": name, "baseline": old["message"], "candidate": new["message"],
                        "baseline_sha256": hashlib.sha256(old["message"].encode("utf-8")).hexdigest(),
                        "candidate_sha256": hashlib.sha256(new["message"].encode("utf-8")).hexdigest(),
                        "qualified_case_work_prefix_only": path_only})
            if path_only:
                permitted.append(f"{name}: full converter message differs only by verified case work prefix")
            else:
                blockers.append(f"{name}: full converter message differs")
        old_files = {f["relative"]: f for f in old["generated_files"]}
        new_files = {f["relative"]: f for f in new["generated_files"]}
        for path in sorted(old_files.keys() | new_files.keys()):
            x, y = old_files.get(path), new_files.get(path)
            if x != y:
                qualified = False
                if capture98 is not None and name == "B:98_B_DS3F" and path == "Gear_blank.MST":
                    left, right = capture98
                    qualified = (left["normalized"] == right["normalized"] and
                                 left["replacements"] == right["replacements"] and
                                 left["input"] == right["input"] and
                                 left["expected_sha256"] == right["expected_sha256"])
                if qualified:
                    permitted.append(f"{name}: Gear_blank.MST raw SHA differs; same-run capture bytes match after exact verified work-prefix substitution only")
                else:
                    blockers.append(f"{name}: generated file differs: {path}")
                raw.append({"kind": "generated_file", "case": name, "relative": path, "baseline": x, "candidate": y,
                            "qualified_same_run_work_prefix_only": qualified,
                            "qualification": "Same-run retained bytes establish exact work-prefix-only difference" if qualified else
                                             "Raw hash/size inequality; receipts alone cannot establish path-only byte differences"})
    for filename in sorted(a["artifacts"].keys() | b["artifacts"].keys()):
        x, y = a["artifacts"].get(filename), b["artifacts"].get(filename)
        if x != y:
            raw.append({"kind": "receipt", "baseline": str(a["directory"] / filename), "candidate": str(b["directory"] / filename),
                        "baseline_sha256": x, "candidate_sha256": y})
    capture_evidence = None
    if capture98 is not None:
        capture_evidence = {label: {key: value for key, value in evidence.items() if key != "normalized"}
                            for label, evidence in zip(("baseline", "candidate"), capture98)}
    return {"equivalent": not blockers, "blockers": blockers, "permitted_differences": permitted, "raw_differences": raw,
            "normal_case_evidence_equivalent": not blockers,
            "prefetch_provenance": {"baseline": ma.get("prefetch"), "candidate": mb.get("prefetch")},
            "physical_resource_failure_behavior_established": False,
            "capture98_evidence": capture_evidence,
            "baseline": str(a["directory"]), "candidate": str(b["directory"]), "case_count": len(a["cases"]),
            "completion_sha256": {"baseline": a["artifacts"]["complete.json"], "candidate": b["artifacts"]["complete.json"]},
            "executable_identity": {"baseline": {key: ma[key] for key in ("executable", "executable_sha256")},
                                    "candidate": {key: mb[key] for key in ("executable", "executable_sha256")}},
            "work_roots": {"baseline": a["work_root"], "candidate": b["work_root"]},
            "qualifications": ["Receipt hashes and identities verified independently; generated files were hashed before cleanup and their bytes are not retained by these receipts.",
                               "Comparison establishes recorded normal-case evidence only. Prefetch can change physical resource-failure ordering; full-fidelity failure behavior is unproven and is not accepted by this report.",
                               "Mode, validated writer roles/PIDs, executable location and verified case work prefixes are execution provenance; generated SHA differences block except the explicitly bound same-run B:98_B_DS3F/Gear_blank.MST byte qualification when supplied.",
                               "Source/build and fixture-before/after evidence are required separately; this comparator does not establish original-application parity."]}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("baseline", type=Path)
    parser.add_argument("candidate", type=Path)
    parser.add_argument("--baseline-capture98", type=Path)
    parser.add_argument("--candidate-capture98", type=Path)
    args = parser.parse_args()
    if bool(args.baseline_capture98) != bool(args.candidate_capture98):
        parser.error("Capture98 directories must be supplied as a pair")
    captures, issues = [], []
    for label, path in (("baseline", args.baseline), ("candidate", args.candidate)):
        try:
            captures.append(load_capture(path))
        except (OSError, ValueError, TypeError, KeyError) as exc:
            issues.append(f"{label}: {exc}")
    if issues:
        print(json.dumps({"equivalent": False, "evidence_issues": issues}, indent=2))
        return 2
    capture98 = None
    if args.baseline_capture98:
        try:
            capture98 = (load_capture98(args.baseline_capture98, captures[0]),
                         load_capture98(args.candidate_capture98, captures[1]))
        except (OSError, ValueError, TypeError, KeyError) as exc:
            print(json.dumps({"equivalent": False, "evidence_issues": [f"capture98: {exc}"]}, indent=2))
            return 2
    result = compare_captures(*captures, capture98=capture98)
    print(json.dumps(result, indent=2))
    return 0 if result["equivalent"] else 1


if __name__ == "__main__":
    sys.exit(main())
