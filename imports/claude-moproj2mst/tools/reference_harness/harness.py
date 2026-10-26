"""Local, stdlib-only evidence preparation. Never launches an executable or drives a GUI.

Receipts bind operator-supplied runtime evidence; they do not authenticate its truth.
Receipts are append-only under this project's build/reference-harness. Optional
disposable runtime trees are copied below C:/PROBLEM and are never executed here.
"""
from __future__ import annotations

import argparse
import datetime as dt
import hashlib
import json
import os
from pathlib import Path, PureWindowsPath
import platform
import re
import shutil
import sys
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[2]
STORE = ROOT / "build" / "reference-harness"
PROBLEM_ROOT = Path("C:/PROBLEM")
MAX_RUNTIME_PATH_CHARS = 100
SCHEMA = "m2m-reference-harness-v1"
BUILD_SCHEMA = "m2m-candidate-build-v1"
BUILD_DIGEST_ALGORITHM = "sha256(concat(sorted(Path.rglob): relative POSIX path UTF-8 + NUL + raw file SHA-256)); files only"


def utc():
    return dt.datetime.now(dt.timezone.utc).isoformat()


def digest(path):
    h = hashlib.sha256()
    with Path(path).open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def require(condition, message):
    if not condition:
        raise ValueError(message)


def beneath(path, root):
    no_links(path)
    no_links(root)
    p, r = Path(path).resolve(), Path(root).resolve()
    require(p != r and r in p.parents, f"path must be strictly below {r}: {p}")
    return p


def no_links(path):
    """Reject symlinks, junctions and any Windows reparse-point object."""
    p = Path(path)
    for item in [p, *p.parents]:
        if item.exists():
            st = item.lstat()
            require(not item.is_symlink() and not (getattr(st, "st_file_attributes", 0) & 0x400),
                    f"link/reparse point prohibited: {item}")


def manifest(root):
    no_links(root)
    root = Path(root).resolve()
    no_links(root)
    require(root.is_dir(), f"missing directory: {root}")
    entries = {}
    for p in sorted(root.rglob("*")):
        no_links(p)
        rel = p.relative_to(root).as_posix()
        st = p.stat()
        entries[rel] = {"kind": "dir" if p.is_dir() else "file", "mtime_ns": st.st_mtime_ns,
                        "mode": st.st_mode, "attributes": getattr(st, "st_file_attributes", None)}
        if p.is_file():
            entries[rel].update(size=st.st_size, sha256=digest(p))
        else:
            require(p.is_dir(), f"unsupported filesystem object: {p}")
    return {"schema": SCHEMA, "root": str(root), "captured_utc": utc(), "entries": entries}


def content(entries):
    return {k: {f: v[f] for f in ("kind", "size", "sha256") if f in v} for k, v in entries.items()}


def object_hash(obj):
    return hashlib.sha256(json.dumps(obj, sort_keys=True, separators=(",", ":")).encode()).hexdigest()


def write_json(path, obj):
    path = beneath(path, STORE)
    no_links(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("x", encoding="utf-8", newline="\n") as f:
        json.dump(obj, f, indent=2, sort_keys=True)
        f.write("\n")
    return {"path": str(path), "sha256": digest(path)}


def read_json(path):
    return json.loads(Path(path).read_text(encoding="utf-8-sig"))


def identity(path):
    no_links(path)
    p = Path(path).resolve()
    no_links(p)
    require(p.is_file(), f"missing identity file: {p}")
    return {"path": str(p), "size": p.stat().st_size, "sha256": digest(p)}


def check_identity(record):
    require(identity(record["path"]) == record, f"identity changed: {record['path']}")


def copy_tree(src, dst):
    before = manifest(src)
    dst = beneath(dst, STORE)
    no_links(dst)
    require(not dst.exists(), f"destination already exists: {dst}")
    shutil.copytree(src, dst, copy_function=shutil.copy2)
    after = manifest(dst)
    require(content(before["entries"]) == content(after["entries"]), "copy content mismatch")
    require(content(before["entries"]) == content(manifest(src)["entries"]), "source changed during copy")
    return after


def case_dir(value):
    p = beneath(value, STORE)
    no_links(p)
    require((p / "case.json").is_file(), f"not a prepared case: {p}")
    return p


def runtime_layout(root_value, case, fixture, source, original, candidates, project_rel):
    """Validate short, link-free runtime trees below C:\\PROBLEM, separate from receipts."""
    require(root_value, "--runtime-root is required for disposable runtime paths")
    no_links(PROBLEM_ROOT)
    problem = Path(PROBLEM_ROOT).resolve()
    require(problem.is_dir(), f"runtime parent is unavailable: {problem}")
    no_links(root_value)
    root = beneath(root_value, problem)
    require(not root.exists(), f"runtime root already exists; choose a new path: {root}")
    protected = [case, fixture, source, original["path"], *(x["path"] for x in candidates), STORE]
    for item in protected:
        p = Path(item).resolve()
        require(root != p and root not in p.parents and p not in root.parents,
                f"runtime root overlaps protected path: {p}")
    layout = {}
    for role in ("original", "candidate"):
        sandbox = root / role
        project = sandbox / "project"
        cwd = sandbox / "cwd"
        project_file = project / Path(project_rel)
        runtime_paths = [sandbox, project, cwd, project_file]
        runtime_paths.extend(project / p.relative_to(fixture) for p in Path(fixture).rglob("*"))
        for p in runtime_paths:
            require(len(str(p)) < MAX_RUNTIME_PATH_CHARS,
                    f"runtime path must be shorter than {MAX_RUNTIME_PATH_CHARS} characters: {p}")
        layout[role] = {"sandbox": str(sandbox), "project": str(project),
                        "cwd": str(cwd), "project_file": str(project_file)}
    return root, layout


def runtime_sandbox(case, cfg, role):
    """Return the role's external runtime sandbox, or the legacy case-local sandbox."""
    layouts = cfg.get("runtime_sandboxes")
    if layouts:
        layout = layouts[role]
        sandbox = Path(layout["sandbox"]).resolve()
        root = Path(cfg["runtime_root"]).resolve()
        no_links(sandbox)
        require(sandbox == root / role and root in sandbox.parents,
                f"runtime sandbox escapes recorded root: {sandbox}")
        return sandbox
    return case / "sandboxes" / role


def verify_runtime_layout(case, cfg):
    layouts = cfg.get("runtime_sandboxes")
    if not layouts:
        return
    no_links(PROBLEM_ROOT)
    root = Path(cfg["runtime_root"]).resolve()
    problem = Path(PROBLEM_ROOT).resolve()
    require(root != problem and problem in root.parents, "runtime root must remain below C:/PROBLEM")
    no_links(root)
    require(root.is_dir(), f"runtime root missing: {root}")
    require(root not in case.parents and case not in root.parents, "runtime root overlaps case store")
    protected = [case, cfg["fixture"], cfg["source"], cfg["original_binary"]["path"],
                 *(x["path"] for x in cfg["candidate_binaries"]), STORE]
    for item in protected:
        p = Path(item).resolve()
        require(root != p and root not in p.parents and p not in root.parents,
                f"runtime root overlaps protected path: {p}")
    for role in ("original", "candidate"):
        expected = {"sandbox": root / role, "project": root / role / "project",
                    "cwd": root / role / "cwd", "project_file": root / role / "project" / cfg["project"]}
        actual = {key: Path(layouts[role][key]).resolve() for key in expected}
        require(actual == expected, f"runtime {role} path mapping changed")
        require(all(len(str(p)) < MAX_RUNTIME_PATH_CHARS for p in expected.values()),
                f"runtime {role} path exceeds {MAX_RUNTIME_PATH_CHARS - 1} characters")
        no_links(expected["sandbox"])
        require(beneath(expected["project_file"], expected["project"]) == expected["project_file"],
                f"runtime {role} project file escapes its project directory")
        require(expected["project"].is_dir() and expected["project_file"].is_file() and expected["cwd"].is_dir(),
                f"runtime {role} directories are incomplete")
    original = Path(layouts["original"]["sandbox"])
    candidate = Path(layouts["candidate"]["sandbox"])
    require(original != candidate and original not in candidate.parents and candidate not in original.parents,
            "original and candidate runtime trees overlap")


def copy_tree_to_runtime(src, dst, runtime_root):
    """Copy a fixture into a validated external runtime tree; receipts stay in STORE."""
    before = manifest(src)
    dst = beneath(dst, runtime_root)
    no_links(dst)
    require(not dst.exists(), f"runtime destination already exists: {dst}")
    shutil.copytree(src, dst, copy_function=shutil.copy2)
    after = manifest(dst)
    require(content(before["entries"]) == content(after["entries"]), "runtime copy content mismatch")
    require(content(before["entries"]) == content(manifest(src)["entries"]), "source changed during runtime copy")
    return after


def validate_runtime_path_lengths(sandbox, entries):
    """Reject any captured runtime path that violates the configured absolute-path cap."""
    for rel in entries:
        path = Path(sandbox).joinpath(*Path(rel).parts)
        require(len(str(path)) < MAX_RUNTIME_PATH_CHARS,
                f"runtime output path must be shorter than {MAX_RUNTIME_PATH_CHARS} characters: {path}")


def build_source_fingerprint(entries):
    """Exactly admin/build_candidate.py's native Path sorting and byte framing."""
    fingerprint, count = hashlib.sha256(), 0
    for name in sorted(entries, key=Path):
        item = entries[name]
        if item["kind"] == "file":
            fingerprint.update(name.encode("utf-8"))
            fingerprint.update(b"\0")
            fingerprint.update(bytes.fromhex(item["sha256"]))
            count += 1
    return {"sha256": fingerprint.hexdigest(), "file_count": count, "algorithm": BUILD_DIGEST_ALGORITHM}


def build_binding(receipt, source_entries, candidates):
    """Validate declared build correspondence, without authenticating its producer."""
    qualification = ("local receipt consistency, not independent compiler attestation or a reproducible-build proof; "
                     "matching source endpoints do not observe every intermediate source state")
    if receipt is None:
        return {"status": "unverified_absent", "reason": "no build receipt supplied"}
    if not isinstance(receipt, dict) or receipt.get("schema") != BUILD_SCHEMA:
        return {"status": "unverified_unsupported", "reason": "unsupported or legacy receipt; verified binding requires m2m-candidate-build-v1"}
    expected = build_source_fingerprint(source_entries)
    require(receipt.get("targets", "missing") is None, "build receipt must represent a full build (targets null)")
    steps = receipt.get("steps", [])
    require(isinstance(steps, list) and all(isinstance(x, dict) for x in steps) and
            [x.get("step") for x in steps] == ["configure", "build"] and
            all(type(x.get("exit_code")) is int and x["exit_code"] == 0 for x in steps),
            "build receipt requires successful configure and build steps")
    require(all(isinstance(x.get("command"), list) and x["command"] and
                all(isinstance(arg, str) for arg in x["command"]) for x in steps) and
            steps[0]["command"][0] == "cmake" and "-S" in steps[0]["command"] and
            steps[1]["command"][0] == "cmake" and "--build" in steps[1]["command"] and
            not any(arg == "--target" or arg.startswith("--target=") for arg in steps[1]["command"]),
            "targeted or unsupported build command")
    require(receipt.get("source_tree_sha256") == expected["sha256"], "build receipt source digest mismatch")
    require(type(receipt.get("source_file_count")) is int and receipt["source_file_count"] == expected["file_count"],
            "build receipt source file count mismatch")
    require(receipt.get("source_tree_after_sha256") == expected["sha256"] and
            type(receipt.get("source_file_count_after")) is int and
            receipt["source_file_count_after"] == expected["file_count"] and
            receipt.get("source_stable_during_build") is True,
            "build receipt lacks matching stable source endpoints")
    # Paths are part of the binding: a same-named stale binary elsewhere is not the supplied artifact.
    artifacts = receipt.get("artifacts", [])
    require(isinstance(artifacts, list), "unsupported artifact list")
    by_path = {}
    for item in artifacts:
        require(isinstance(item, dict) and isinstance(item.get("path"), str), "malformed build artifact")
        path = str(Path(item["path"]).resolve())
        require(path not in by_path, "duplicate build artifact path")
        by_path[path] = item
    matched = []
    for candidate in candidates:
        item = by_path.get(str(Path(candidate["path"]).resolve()))
        require(item is not None and item.get("sha256") == candidate["sha256"] and
                type(item.get("bytes")) is int and item["bytes"] == candidate["size"],
                f"build receipt binary identity mismatch: {candidate['path']}")
        matched.append(candidate)
    return {"status": "verified_receipt_consistency", "schema": BUILD_SCHEMA, "source": expected,
            "full_build": True, "source_endpoints_match": True, "matched_artifacts": matched,
            "qualification": qualification}


def prepare(args):
    require(re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9_.-]{0,79}", args.case_id), "unsafe case id")
    no_links(args.fixture)
    fixture = Path(args.fixture).resolve()
    target = beneath(STORE / args.case_id, STORE)
    require(not target.exists(), "case already exists; use a new case id (nothing is deleted)")
    require(target not in fixture.parents and fixture not in target.parents, "fixture/store overlap")
    project = beneath(fixture / args.project, fixture)
    require(project.suffix.lower() == ".moproj" and project.is_file(), "project must name an existing .moproj")
    no_links(args.source)
    source = Path(args.source).resolve()
    require(source not in target.parents and target not in source.parents, "source/store overlap")
    frozen_source = manifest(source)
    original = identity(args.original_binary)
    candidates = [identity(x) for x in args.candidate_binary]
    require(bool(candidates), "candidate executable and runtime dependencies must be frozen")
    build_origin = identity(args.build_receipt) if getattr(args, "build_receipt", None) else None
    build_record = read_json(build_origin["path"]) if build_origin else None
    binding = build_binding(build_record, frozen_source["entries"], candidates)
    runtime_root = None
    runtime_layouts = None
    if getattr(args, "runtime_root", None):
        runtime_root, runtime_layouts = runtime_layout(args.runtime_root, target, fixture, source,
                                                       original, candidates, project.relative_to(fixture))
    # No selective corpus filtering: retain the full caller-selected fixture, including existing outputs.
    initial = manifest(fixture)
    target.mkdir(parents=True)
    # Preserve runnable candidate bytes and source, rather than freezing mutable build paths by name.
    require(len({Path(x["path"]).name.casefold() for x in candidates}) == len(candidates),
            "candidate binary basenames collide")
    source_snapshot = copy_tree(source, target / "frozen-source")
    require(content(source_snapshot["entries"]) == content(frozen_source["entries"]),
            "source changed between initial fingerprint and source snapshot")
    frozen_build_receipt = None
    if build_origin:
        check_identity(build_origin)
        shutil.copy2(build_origin["path"], target / "frozen-build-receipt.json")
        frozen_build_receipt = identity(target / "frozen-build-receipt.json")
        require(frozen_build_receipt["sha256"] == build_origin["sha256"], "build receipt changed while freezing")
    (target / "frozen-candidate").mkdir()
    frozen_candidates = []
    for candidate in candidates:
        copied = target / "frozen-candidate" / Path(candidate["path"]).name
        shutil.copy2(candidate["path"], copied)
        item = identity(copied)
        require(item["sha256"] == candidate["sha256"], "candidate changed while copying")
        frozen_candidates.append(item)
    write_json(target / "fixture-before.json", initial)
    inputs = copy_tree(fixture, target / "inputs")
    write_json(target / "inputs.json", inputs)
    for role in ("original", "candidate"):
        if runtime_layouts:
            work = Path(runtime_layouts[role]["sandbox"])
            work.mkdir(parents=True)
            copy_tree_to_runtime(target / "inputs", work / "project", runtime_root)
            Path(runtime_layouts[role]["cwd"]).mkdir()
            write_json(target / f"{role}-runtime-before.json", manifest(work))
        else:
            work = target / "sandboxes" / role
            copy_tree(target / "inputs", work / "project")
            (work / "cwd").mkdir()
            write_json(target / f"{role}-before.json", manifest(work))
    write_json(target / "source-freeze.json", frozen_source)
    record = {"schema": SCHEMA, "prepared_utc": utc(), "case_id": args.case_id,
              "split": args.split, "fixture": str(fixture), "project": project.relative_to(fixture).as_posix(),
              "source": str(source), "source_content_sha256": object_hash(content(frozen_source["entries"])),
              "original_binary": original, "candidate_binaries": frozen_candidates,
              "candidate_build_artifacts": candidates,
              "build_receipt": frozen_build_receipt, "build_receipt_origin": build_origin,
              "source_binary_binding": binding,
              "harness": identity(__file__), "python": sys.version, "platform": platform.platform(),
              "historical_oracle": args.historical_oracle,
              "historical_qualification": "historical provenance only; never a fresh-reference expectation",
              "scope": "one project, fresh original state, Open > Save > Close; original default dialog answers",
              "comparison_policy": "raw effects first; no automatic waivers; failures and partial writes retained",
              "chronology": "freeze candidate before reference; record original before candidate evaluation",
              "dependencies_qualification": "supplied identity list is not proof of complete loaded-module closure"}
    if runtime_layouts:
        record["runtime_root"] = str(runtime_root)
        record["runtime_sandboxes"] = runtime_layouts
        record["runtime_storage_qualification"] = "disposable role trees are under C:/PROBLEM; receipts and retained snapshots remain in the checkout case store"
    manifest_names = ["fixture-before.json", "inputs.json", "source-freeze.json"]
    manifest_names.extend(("original-runtime-before.json", "candidate-runtime-before.json")
                          if runtime_layouts else ("original-before.json", "candidate-before.json"))
    record["frozen_manifests"] = {name: digest(target / name) for name in manifest_names}
    if runtime_layouts:
        record["frozen_manifests"].update({f"{role}-runtime-before.json": digest(target / f"{role}-runtime-before.json")
                                           for role in ("original", "candidate")})
    receipt = write_json(target / "case.json", record)
    return {"case": str(target), "receipt": receipt, "runtime_started": False}


def verify_freeze(case, cfg):
    for name, expected in cfg["frozen_manifests"].items():
        require(digest(case / name) == expected, f"frozen manifest changed: {name}")
    check_identity(cfg["original_binary"])
    for binary in cfg["candidate_binaries"]:
        check_identity(binary)
    check_identity(cfg["harness"])
    if cfg.get("build_receipt"):
        check_identity(cfg["build_receipt"])
        rebound = build_binding(read_json(cfg["build_receipt"]["path"]),
                               read_json(case / "source-freeze.json")["entries"], cfg["candidate_build_artifacts"])
        require(rebound == cfg["source_binary_binding"], "build receipt binding changed")
    verify_runtime_layout(case, cfg)
    require(object_hash(content(manifest(cfg["source"])["entries"])) == cfg["source_content_sha256"],
            "candidate source differs from freeze; prepare a new case")
    require(object_hash(content(manifest(case / "frozen-source")["entries"])) == cfg["source_content_sha256"],
            "frozen source snapshot changed")
    for path, expected in ((cfg["fixture"], "fixture-before.json"), (case / "inputs", "inputs.json")):
        require(content(manifest(path)["entries"]) == content(read_json(case / expected)["entries"]),
                f"preserved input changed: {path}")


def ref_scan(project):
    """Bounded, syntax-only XML path scan. Compressed blobs are explicitly outside its coverage."""
    data = Path(project).read_bytes()
    require(len(data) <= 8 * 1024 * 1024, "project XML exceeds 8 MiB scan bound")
    root = ET.fromstring(data)
    refs, external = [], []
    for elem in root.iter():
        text = (elem.text or "").strip()
        if not text or "\n" in text:
            continue
        if PureWindowsPath(text).is_absolute() or text.startswith(("/", "\\")):
            external.append({"tag": elem.tag, "value": text[:1000]})
        elif elem.tag in ("Path", "RestartKey", "UserDefinedKey"):
            p = (Path(project).parent / text.replace("\\", "/")).resolve()
            escaped = p != Path(project).parent and Path(project).parent not in p.parents
            refs.append({"tag": elem.tag, "value": text, "exists": p.exists(), "escapes_project": escaped})
    return {"relative_references": refs, "absolute_values": external,
            "coverage": "selected XML path fields and absolute element text only; no compressed KEY/DB, side-file, attribute, registry or transitive dependency analysis"}


def preflight(args):
    case = case_dir(args.case)
    cfg = read_json(case / "case.json")
    verify_freeze(case, cfg)
    if cfg.get("runtime_sandboxes"):
        for role in ("original", "candidate"):
            before = read_json(case / f"{role}-runtime-before.json")
            current = manifest(runtime_sandbox(case, cfg, role))
            require(content(current["entries"]) == content(before["entries"]),
                    f"runtime {role} tree changed before preflight")
    scan = ref_scan(case / "inputs" / cfg["project"])
    blockers = []
    if scan["absolute_values"] or any(x["escapes_project"] for x in scan["relative_references"]):
        blockers.append("review external paths before execution; copying a fixture does not contain program I/O")
    if any(not x["exists"] for x in scan["relative_references"]):
        blockers.append("missing selected XML reference: preserve and classify expected failure, or choose a complete fixture")
    return {"case": str(case), "freeze_intact": True, "source_binary_binding": cfg["source_binary_binding"],
            "path_scan": scan, "blockers": blockers,
            "runtime_readiness": "not established: no process/session was connected or launched",
            "required_runtime_checks": ["installed original identity and required runtime/licence available",
                "no preexisting project/session; observe fresh initial state and record startup settings",
                "observe actual GUI control and actual default button before each action",
                "record launch argv/cwd/PID, environment, settings, dialogs, messages, close/exit outcome",
                "retain evidence for outside-project writes; sandbox is a directory layout, not an OS security boundary",
                "candidate must request full Save via --write-save; MST-only is insufficient"]}


def artifact_records(case, paths):
    records = []
    for name in paths:
        p = beneath(case / name, case / "evidence")
        records.append(identity(p))
    require(records, "at least one retained runtime evidence artifact is required")
    return records


def validate_observation(case, cfg, role, obs):
    require(obs.get("schema") == SCHEMA, "observation schema mismatch")
    require(obs.get("role") == role, "observation role mismatch")
    require(obs.get("outcome") in ("completed", "refused", "error", "crashed", "timeout"), "invalid outcome")
    session = obs["session"]
    require(session.get("fresh_state") is True and session.get("previous_projects") == [],
            "acceptance requires declared fresh state with no previous projects")
    require(session.get("project_count") == 1, "single-project boundary required")
    for field in ("started_utc", "ended_utc", "pid", "argv", "cwd", "environment", "settings", "exit_status"):
        require(field in session, f"session missing {field}")
    require(isinstance(session["pid"], int) and session["pid"] > 0, "positive observed PID required")
    start = dt.datetime.fromisoformat(session["started_utc"])
    end = dt.datetime.fromisoformat(session["ended_utc"])
    prepared = dt.datetime.fromisoformat(cfg["prepared_utc"])
    require(start.tzinfo is not None and end.tzinfo is not None and prepared <= start <= end,
            "invalid runtime timestamps or execution before freeze")
    if role == "candidate":
        original = read_json(case / "captures" / "original" / "receipt.json")
        require(start >= dt.datetime.fromisoformat(original["recorded_utc"]),
                "candidate execution must follow original receipt")
        require(session.get("save_mode") == "full-save", "candidate must declare full-save request")
        if Path(cfg["candidate_binaries"][0]["path"]).name.lower() == "moproj2mst.exe":
            require("--write-save" in session["argv"], "moproj2mst CLI must request --write-save")
    sandbox = runtime_sandbox(case, cfg, role)
    expected_cwd = (Path(cfg["runtime_sandboxes"][role]["cwd"]).resolve()
                    if cfg.get("runtime_sandboxes") else sandbox / "cwd")
    require(Path(session["cwd"]).resolve() == expected_cwd, "execution cwd must be disposable cwd directory")
    if cfg.get("runtime_sandboxes"):
        expected_project = Path(cfg["runtime_sandboxes"][role]["project_file"]).resolve()
        require(Path(session.get("project_path", "")).resolve() == expected_project,
                "observed project path differs from disposable project freeze")
        if role == "candidate":
            require(len(session["argv"]) > 1 and Path(session["argv"][1]).resolve() == expected_project,
                    "candidate argv project differs from disposable project freeze")
    require(session["argv"] and Path(session["argv"][0]).resolve() == Path(
        cfg["original_binary"]["path"] if role == "original" else cfg["candidate_binaries"][0]["path"]),
        "argv executable differs from freeze")
    expected = cfg["original_binary"] if role == "original" else cfg["candidate_binaries"][0]
    require(session.get("executable_sha256") == expected["sha256"], "observed executable digest differs")
    actions = obs.get("actions", [])
    names = [x["action"] for x in actions]
    require(names and names == ["open", "save", "close"][:len(names)] and len(names) <= 3,
            "actions must be a nonempty prefix of open/save/close; no retries or second project")
    if obs["outcome"] == "completed":
        require(names == ["open", "save", "close"] and all(x.get("status") == "completed" for x in actions),
                "completed observation requires all lifecycle stages")
    for action in actions:
        require(action.get("evidence"), "each action requires retained evidence")
    for dialog in obs.get("dialogs", []):
        require(dialog.get("stage") in names and dialog.get("observed_default") and
                dialog.get("selected") == dialog["observed_default"] and dialog.get("evidence"),
                "dialog must retain evidence of its actual observed default and matching selection")
    paths = list(obs.get("artifacts", []))
    paths.extend(x["evidence"] for x in actions)
    paths.extend(x["evidence"] for x in obs.get("dialogs", []))
    return artifact_records(case, sorted(set(paths)))


def effects(before, after):
    a, b = content(before), content(after)
    return {"added": sorted(b.keys() - a.keys()), "removed": sorted(a.keys() - b.keys()),
            "changed": sorted(k for k in a.keys() & b.keys() if a[k] != b[k]),
            "metadata_changed": sorted(k for k in before.keys() & after.keys() if
                                       {x: y for x, y in before[k].items() if x not in ("kind", "size", "sha256")} !=
                                       {x: y for x, y in after[k].items() if x not in ("kind", "size", "sha256")})}


def record(args):
    case = case_dir(args.case)
    cfg = read_json(case / "case.json")
    role = args.role
    require(not (case / "captures" / role).exists(), "capture already exists; use a new case")
    require(role == "original" or (case / "captures" / "original" / "receipt.json").is_file(),
            "record original before candidate evaluation")
    verify_freeze(case, cfg)
    original_receipt = None
    if role == "candidate":
        verify_capture(case, "original")
        original_receipt = digest(case / "captures/original/receipt.json")
    else:
        candidate_sandbox = runtime_sandbox(case, cfg, "candidate")
        candidate_before_name = ("candidate-runtime-before.json" if cfg.get("runtime_sandboxes")
                                 else "candidate-before.json")
        require(content(manifest(candidate_sandbox)["entries"]) ==
                content(read_json(case / candidate_before_name)["entries"]),
                "candidate sandbox changed before original was recorded")
    obs = read_json(args.observation)
    artifacts = validate_observation(case, cfg, role, obs)
    dest = case / "captures" / role
    # Record actual runtime metadata, not attributes assigned while copying the evidence snapshot.
    sandbox = runtime_sandbox(case, cfg, role)
    after = manifest(sandbox)
    if cfg.get("runtime_sandboxes"):
        validate_runtime_path_lengths(sandbox, after["entries"])
    snapshot = copy_tree(sandbox, dest / "files")
    require(content(after["entries"]) == content(snapshot["entries"]), "runtime files changed during retention")
    before_name = f"{role}-runtime-before.json" if cfg.get("runtime_sandboxes") else f"{role}-before.json"
    before = read_json(case / before_name)
    delta = effects(before["entries"], after["entries"])
    write_json(dest / "after.json", after)
    write_json(dest / "observation.json", obs)
    receipt = {"schema": SCHEMA, "role": role, "case_sha256": digest(case / "case.json"),
               "recorded_utc": utc(), "outcome": obs["outcome"], "artifacts": artifacts,
               "after_manifest_sha256": digest(dest / "after.json"),
               "before_manifest_sha256": digest(case / before_name),
               "observation_sha256": digest(dest / "observation.json"), "effects": delta,
               "original_receipt_sha256": original_receipt,
               "qualification": "operator-declared runtime provenance, not independent process attestation; final-state manifests do not establish write ordering; failures/partial effects preserved"}
    result = write_json(dest / "receipt.json", receipt)
    return {"receipt": result, "outcome": obs["outcome"], "effect_counts": {k: len(v) for k, v in delta.items()}}


def verify_capture(case, role):
    dest = case / "captures" / role
    receipt = read_json(dest / "receipt.json")
    require(receipt["case_sha256"] == digest(case / "case.json"), "case receipt changed")
    for filename, key in (("after.json", "after_manifest_sha256"), ("observation.json", "observation_sha256")):
        require(digest(dest / filename) == receipt[key], f"capture metadata changed: {filename}")
    before_name = f"{role}-runtime-before.json" if read_json(case / "case.json").get("runtime_sandboxes") else f"{role}-before.json"
    require(digest(case / before_name) == receipt["before_manifest_sha256"], "before manifest changed")
    entries = read_json(dest / "after.json")["entries"]
    require(content(manifest(dest / "files")["entries"]) == content(entries), "capture files changed")
    for art in receipt["artifacts"]:
        check_identity(art)
    return receipt, entries


def first_difference(a, b):
    pos = next((i for i, (x, y) in enumerate(zip(a, b)) if x != y), min(len(a), len(b)))
    return {"offset": pos, "original_window_hex": a[max(0, pos-16):pos+32].hex(),
            "candidate_window_hex": b[max(0, pos-16):pos+32].hex(), "original_size": len(a), "candidate_size": len(b)}


def qualified_bytes(data, role, rule):
    require(hashlib.sha256(data).hexdigest() == rule[f"{role}_sha256"], "qualification input digest mismatch")
    patches = sorted(rule[role], key=lambda x: x["offset"])
    output, previous = [], 0
    require(patches, "qualification must specify explicit byte slices for both roles")
    for patch in patches:
        offset = patch["offset"]
        old, new = bytes.fromhex(patch["old_hex"]), bytes.fromhex(patch["canonical_hex"])
        require(old and offset >= previous and data[offset:offset+len(old)] == old,
                "qualification slice mismatch, overlap or empty old bytes")
        output.extend((data[previous:offset], new))
        previous = offset + len(old)
    output.append(data[previous:])
    return b"".join(output)


def compare(args):
    case = case_dir(args.case)
    original, a = verify_capture(case, "original")
    candidate, b = verify_capture(case, "candidate")
    require(candidate["original_receipt_sha256"] == digest(case / "captures/original/receipt.json"),
            "original receipt changed after candidate evaluation")
    raw = effects(a, b)
    details = {}
    for name in raw["changed"]:
        if a[name]["kind"] == b[name]["kind"] == "file":
            details[name] = first_difference((case / "captures/original/files" / name).read_bytes(),
                                             (case / "captures/candidate/files" / name).read_bytes())
    qualified = []
    if args.qualifications:
        rules = read_json(args.qualifications)
        seen = set()
        for rule in rules:
            name = rule["path"]
            require(name in details and name not in seen, "qualification must select one changed file exactly once")
            seen.add(name)
            require(rule.get("reason") and rule.get("evidence"), "qualification needs reason and retained evidence")
            evidence = artifact_records(case, rule["evidence"])
            aa = qualified_bytes((case / "captures/original/files" / name).read_bytes(), "original", rule)
            bb = qualified_bytes((case / "captures/candidate/files" / name).read_bytes(), "candidate", rule)
            qualified.append({"rule": rule, "evidence": evidence, "equal": aa == bb,
                              "original_transformed_sha256": hashlib.sha256(aa).hexdigest(),
                              "candidate_transformed_sha256": hashlib.sha256(bb).hexdigest()})
    byte_equal = not any(raw[k] for k in ("added", "removed", "changed"))
    result = {"schema": SCHEMA, "compared_utc": utc(), "case": str(case),
              "original_receipt_sha256": digest(case / "captures/original/receipt.json"),
              "candidate_receipt_sha256": digest(case / "captures/candidate/receipt.json"),
              "outcomes": {"original": original["outcome"], "candidate": candidate["outcome"]},
              "raw_content_equal": byte_equal, "raw": raw, "first_differences": details,
              "qualified_views": qualified,
              "qualification": "file-state comparison only; never overall acceptance or architectural fidelity; no mapping of original status/messages to adapter return codes; timestamps listed separately; no automatic historical waivers"}
    receipt = write_json(case / "comparisons" / args.name, result)
    return {"receipt": receipt, "raw_content_equal": byte_equal, "difference_counts": {k: len(v) for k, v in raw.items()},
            "qualified_files": len(qualified)}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("prepare")
    p.add_argument("--fixture", required=True)
    p.add_argument("--project", required=True, help="relative path inside fixture")
    p.add_argument("--source", default=str(ROOT / "source"))
    p.add_argument("--original-binary", required=True)
    p.add_argument("--candidate-binary", action="append", required=True, help="executable first, then DLLs/dependencies")
    p.add_argument("--build-receipt", help="optional v1 build receipt; verifies source endpoints, full build and supplied artifacts")
    p.add_argument("--runtime-root", help="new disposable root below C:/PROBLEM; evidence remains in the case store")
    p.add_argument("--case-id", required=True)
    p.add_argument("--split", choices=("training", "evaluation"), required=True)
    p.add_argument("--historical-oracle", default="none")
    p.set_defaults(run=prepare)
    for command, fn in (("preflight", preflight), ("record", record), ("compare", compare)):
        p = sub.add_parser(command)
        p.add_argument("--case", required=True)
        p.set_defaults(run=fn)
        if command == "record":
            p.add_argument("--role", choices=("original", "candidate"), required=True)
            p.add_argument("--observation", required=True)
        elif command == "compare":
            p.add_argument("--name", default="raw.json", help="new receipt filename; never overwrite")
            p.add_argument("--qualifications")
    p = sub.add_parser("manifest")
    p.add_argument("--root", required=True)
    p.add_argument("--output", required=True, help="new file below build/reference-harness")
    p.set_defaults(run=lambda a: write_json(a.output, manifest(a.root)))
    args = parser.parse_args(argv)
    try:
        if getattr(args, "name", None):
            require(Path(args.name).name == args.name and args.name.endswith(".json"), "comparison name must be a JSON basename")
        print(json.dumps(args.run(args), sort_keys=True))
    except (ValueError, OSError, KeyError, TypeError, ET.ParseError) as e:
        print(json.dumps({"error": str(e), "runtime_started": False}), file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
