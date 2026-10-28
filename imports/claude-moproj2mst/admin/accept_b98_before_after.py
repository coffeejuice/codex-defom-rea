#!/usr/bin/env python3
"""Reproduce the retained B98 before-to-after candidate comparison.

This is an opt-in acceptance tool, separate from the fast test suite. It never
launches the protected original application and never cleans up its run folder.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import time
import uuid


ROOT = Path(__file__).resolve().parents[1]
BEFORE = ROOT / "evidence" / "gui_mo" / "oracle_b" / "98_B_DS3F_before"
AFTER = ROOT / "evidence" / "gui_mo" / "oracle_b" / "98_B_DS3F"
SCRATCH_PARENT = Path(r"C:\PROBLEM\b98r")
CP88_SOURCE_SHA256 = "eacfe403abee631861befe6f9f35b274c9be1928af0e52f95e6e1e671c54cfef"
INPUT_MODE_ENV = "M2M_MO_INPUT_MODE"
INPUT_MODE_VALUE = "0"
CP88_ARTIFACTS = {
    "moproj2mst.exe": "eef23401ece560ca6405c5ae23cb315cea88a1b14dc907f97c4c7e689a4be373",
    "moproj2mst.dll": "eed0f155561e8c95c46a77a046fcd3deb2ef23ac1770b767c05c3d76ab8401cd",
    "deformdb.dll": "c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3",
}
SIDECAR_SUFFIXES = {".png", ".oraclea"}
EXPECTED_COMPARABLE_PATHS = 29
ORIGINAL_KEY_PATH = (
    b"C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/"
    b"98_B_DS3F/Gear_blank.KEY"
)


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def file_sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def manifest(folder: Path, *, comparable_only: bool = False) -> dict[str, dict[str, object]]:
    result: dict[str, dict[str, object]] = {}
    for path in sorted(folder.rglob("*")):
        if not path.is_file():
            continue
        rel = path.relative_to(folder).as_posix()
        if comparable_only and path.suffix.lower() in SIDECAR_SUFFIXES:
            continue
        result[rel] = {"bytes": path.stat().st_size, "sha256": file_sha256(path)}
    return result


def assert_no_reparse(path: Path) -> None:
    if path.is_symlink() or (path.exists() and path.is_dir() and os.path.islink(path)):
        raise RuntimeError(f"Refusing reparse/symlink path: {path}")
    if path.exists() and getattr(path.stat(), "st_file_attributes", 0) & 0x400:
        raise RuntimeError(f"Refusing reparse-point directory: {path}")


def checked_scratch_root(candidate_version: str) -> Path:
    problem = Path(r"C:\PROBLEM")
    assert_no_reparse(problem)
    if not problem.is_dir():
        raise RuntimeError(r"Required short-path parent C:\PROBLEM is absent")
    assert_no_reparse(SCRATCH_PARENT)
    SCRATCH_PARENT.mkdir(parents=True, exist_ok=True)
    # Use a short, unique, never-reused path. No deletion or cleanup is done.
    for _ in range(20):
        match = re.fullmatch(r"CP(\d+)", candidate_version, re.IGNORECASE)
        prefix = f"r{match.group(1)}-" if match else "r-"
        run = SCRATCH_PARENT / (prefix + uuid.uuid4().hex[:8])
        if len(str(run / "p" / "Gear_blank.moproj")) >= 100:
            raise RuntimeError(f"Generated project path is not short enough: {run}")
        try:
            run.mkdir()
            return run.resolve(strict=True)
        except FileExistsError:
            continue
    raise RuntimeError("Could not allocate a unique disposable run directory")


def validate_cli(
    cli_arg: str,
    expected_artifacts: dict[str, str],
    *,
    candidate_version: str,
    expected_source_sha256: str,
    freeze_receipt_arg: str | None,
    current_freeze_receipt_arg: str | None = None,
) -> tuple[Path, dict[str, str], Path | None, str | None]:
    cli = Path(cli_arg).resolve(strict=True)
    if cli.name.lower() != "moproj2mst.exe":
        raise RuntimeError("--cli must name the frozen candidate moproj2mst.exe")
    if not cli.is_file():
        raise RuntimeError("--cli is not a regular file")
    dependencies = {name: cli.with_name(name) for name in expected_artifacts}
    if set(dependencies) != set(expected_artifacts):
        raise AssertionError("Invalid frozen-artifact table")
    hashes: dict[str, str] = {}
    for name, path in dependencies.items():
        if not path.is_file():
            raise RuntimeError(f"Frozen candidate dependency missing beside CLI: {path}")
        actual = file_sha256(path)
        expected = expected_artifacts[name].lower()
        if actual != expected:
            raise RuntimeError(f"Frozen candidate identity mismatch for {name}: {actual}")
        hashes[name] = actual

    freeze_receipt = None
    freeze_receipt_hash = None
    if freeze_receipt_arg and current_freeze_receipt_arg:
        raise RuntimeError("Select only one frozen receipt layout")
    selected_receipt = current_freeze_receipt_arg or freeze_receipt_arg
    if selected_receipt:
        freeze_receipt = Path(selected_receipt).resolve(strict=True)
        match = re.fullmatch(r"CP(\d+)", candidate_version, re.IGNORECASE)
        if current_freeze_receipt_arg and not match:
            raise RuntimeError("--current-freeze-receipt requires a CP<number> candidate version")
        if match:
            # Opt into the current layout without accepting an arbitrary receipt
            # directory or changing the checkpoint label to bypass its binding.
            folder = (f"cp{match.group(1)}-verified" if current_freeze_receipt_arg
                      else f"wave3-checkpoint{match.group(1)}-verified")
            expected_receipt = (ROOT / "build" / folder / "receipt.json").resolve()
            if freeze_receipt != expected_receipt:
                raise RuntimeError(f"Freeze receipt does not match candidate version {candidate_version}: {freeze_receipt}")
        receipt = json.loads(freeze_receipt.read_text(encoding="utf-8"))
        if receipt.get("source_tree_sha256", "").lower() != expected_source_sha256.lower():
            raise RuntimeError("Frozen receipt source-tree hash does not match the explicit source hash")
        entries = {Path(item["path"]).name: item for item in receipt.get("artifacts", [])}
        for name, actual_path in dependencies.items():
            entry = entries.get(name)
            if not entry:
                raise RuntimeError(f"Frozen receipt lacks required artifact {name}")
            if Path(entry["path"]).resolve() != actual_path.resolve():
                raise RuntimeError(f"CLI/dependency path is not the artifact bound by freeze receipt: {name}")
            if entry.get("sha256", "").lower() != expected_artifacts[name].lower():
                raise RuntimeError(f"Explicit hash does not match frozen receipt for {name}")
        freeze_receipt_hash = file_sha256(freeze_receipt)
    elif candidate_version.upper() != "CP88":
        raise RuntimeError("Non-CP88 candidate runs require a frozen receipt")
    return cli, hashes, freeze_receipt, freeze_receipt_hash


def validate_inputs() -> None:
    for folder in (BEFORE, AFTER):
        if not folder.is_dir():
            raise RuntimeError(f"Required retained fixture is missing: {folder}")
        assert_no_reparse(folder)
    # Do not follow links within retained trees; they are not part of the fixture contract.
    for folder in (BEFORE, AFTER):
        for path in folder.rglob("*"):
            if path.is_symlink() or getattr(path.lstat(), "st_file_attributes", 0) & 0x400:
                raise RuntimeError(f"Refusing linked fixture entry: {path}")
    before = manifest(BEFORE, comparable_only=True)
    after = manifest(AFTER, comparable_only=True)
    if len(after) != EXPECTED_COMPARABLE_PATHS:
        raise RuntimeError(f"Retained after-state has {len(after)} comparable paths, expected 29")
    if "Gear_blank.MST" not in after or "Gear_blank.MST" in before:
        raise RuntimeError("B98 before/after MST presence contract changed")


def write_json(path: Path, value: object) -> None:
    path.write_text(json.dumps(value, indent=2, sort_keys=True) + "\n", encoding="utf-8")


def child_environment(parent: dict[str, str] | None = None) -> tuple[dict[str, str], str | None]:
    """Copy normal process context while pinning the adapter's input mode."""
    effective = dict(os.environ if parent is None else parent)
    inherited = effective.get(INPUT_MODE_ENV)
    effective[INPUT_MODE_ENV] = INPUT_MODE_VALUE
    return effective, inherited


def self_check() -> int:
    """Exercise binding and input-mode guards without launching the converter."""
    env, inherited = child_environment({"PATH": "sentinel", INPUT_MODE_ENV: "999"})
    if inherited != "999" or env.get(INPUT_MODE_ENV) != INPUT_MODE_VALUE:
        print("input-mode environment pinning self-check failed", file=sys.stderr)
        return 1
    validate_hash_overrides(
        candidate_version="CP88",
        supplied_hashes={},
        supplied_source_hash=None,
        has_freeze_receipt=False,
    )
    try:
        validate_hash_overrides(
            candidate_version="CP88",
            supplied_hashes={"moproj2mst.exe": "0" * 64},
            supplied_source_hash=None,
            has_freeze_receipt=False,
        )
    except ValueError as exc:
        rejected_override = "--freeze-receipt" in str(exc)
    else:
        rejected_override = False
    if not rejected_override:
        print("CP88 unbound hash override was not rejected", file=sys.stderr)
        return 1
    print(json.dumps({
        "self_check_pass": True,
        "converter_launched": False,
        "inherited_input_mode": inherited,
        "effective_input_mode": env[INPUT_MODE_ENV],
        "cp88_unbound_hash_override_rejected": rejected_override,
    }, indent=2))
    return 0


def validate_hash_overrides(
    *,
    candidate_version: str,
    supplied_hashes: dict[str, str],
    supplied_source_hash: str | None,
    has_freeze_receipt: bool,
) -> None:
    """Prevent a caller from relabeling non-CP88 bytes as the CP88 default."""
    if candidate_version.upper() != "CP88" or has_freeze_receipt:
        return
    changed = [
        name for name, value in supplied_hashes.items()
        if value and value.lower() != CP88_ARTIFACTS[name]
    ]
    if supplied_source_hash and supplied_source_hash.lower() != CP88_SOURCE_SHA256:
        changed.append("source tree")
    if changed:
        raise ValueError(
            "CP88 hash overrides require a validated --freeze-receipt: " + ", ".join(changed)
        )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--candidate-version", default="CP88", help="Checkpoint label, for example CP88 or CP90")
    receipts = parser.add_mutually_exclusive_group()
    receipts.add_argument("--freeze-receipt", help="Legacy build/wave3-checkpoint<N>-verified/receipt.json")
    receipts.add_argument("--current-freeze-receipt", help="Current build/cp<N>-verified/receipt.json; requires matching CP<N> label")
    parser.add_argument("--source-tree-sha256", help="Expected source-tree SHA-256 from the checkpoint freeze")
    parser.add_argument("--cli", help="Explicit path to frozen candidate moproj2mst.exe")
    parser.add_argument("--expected-cli-sha256", help="Expected frozen candidate executable SHA-256")
    parser.add_argument("--expected-moproj2mst-dll-sha256", help="Expected adjacent moproj2mst.dll SHA-256")
    parser.add_argument("--expected-deformdb-sha256", help="Expected adjacent deformdb.dll SHA-256")
    parser.add_argument("--self-check", action="store_true", help="Check environment pinning without launching the converter")
    args = parser.parse_args()

    if args.self_check:
        return self_check()
    if not args.cli:
        parser.error("--cli is required unless --self-check is used")

    is_cp88_default = args.candidate_version.upper() == "CP88"
    required_values = {
        "--expected-cli-sha256": args.expected_cli_sha256,
        "--expected-moproj2mst-dll-sha256": args.expected_moproj2mst_dll_sha256,
        "--expected-deformdb-sha256": args.expected_deformdb_sha256,
        "--source-tree-sha256": args.source_tree_sha256,
    }
    if not is_cp88_default and any(value is None for value in required_values.values()):
        parser.error("non-CP88 runs require explicit source, executable, and both DLL hashes")
    expected_artifacts = dict(CP88_ARTIFACTS)
    if args.expected_cli_sha256:
        expected_artifacts["moproj2mst.exe"] = args.expected_cli_sha256
    if args.expected_moproj2mst_dll_sha256:
        expected_artifacts["moproj2mst.dll"] = args.expected_moproj2mst_dll_sha256
    if args.expected_deformdb_sha256:
        expected_artifacts["deformdb.dll"] = args.expected_deformdb_sha256
    expected_source_sha256 = args.source_tree_sha256 or CP88_SOURCE_SHA256
    has_freeze_receipt = bool(args.freeze_receipt or args.current_freeze_receipt)
    if not is_cp88_default and not has_freeze_receipt:
        parser.error("non-CP88 runs require --freeze-receipt or --current-freeze-receipt")
    try:
        validate_hash_overrides(
            candidate_version=args.candidate_version,
            supplied_hashes={
                "moproj2mst.exe": args.expected_cli_sha256,
                "moproj2mst.dll": args.expected_moproj2mst_dll_sha256,
                "deformdb.dll": args.expected_deformdb_sha256,
            },
            supplied_source_hash=args.source_tree_sha256,
            has_freeze_receipt=has_freeze_receipt,
        )
    except ValueError as exc:
        parser.error(str(exc))

    validate_inputs()
    cli, artifact_hashes, freeze_receipt, freeze_receipt_hash = validate_cli(
        args.cli,
        expected_artifacts,
        candidate_version=args.candidate_version,
        expected_source_sha256=expected_source_sha256,
        freeze_receipt_arg=args.freeze_receipt,
        current_freeze_receipt_arg=args.current_freeze_receipt,
    )
    run_root = checked_scratch_root(args.candidate_version)
    project_dir = run_root / "p"
    project_dir.mkdir()
    shutil.copytree(BEFORE, project_dir, dirs_exist_ok=True, symlinks=False)
    for path in project_dir.rglob("*"):
        if len(str(path)) >= 100:
            raise RuntimeError(f"Disposable input path exceeds 99 characters: {path}")
    input_manifest = manifest(project_dir)
    retained_input_manifest = manifest(BEFORE)
    if input_manifest != retained_input_manifest:
        raise RuntimeError("Disposable copy did not preserve the retained before-state bytes")

    project = project_dir / "Gear_blank.moproj"
    argv = [str(cli), "--strict", "--codepage", "1252", "--write-save", str(project)]
    child_env, inherited_mode = child_environment()
    start = time.time()
    proc = subprocess.run(argv, cwd=project_dir, capture_output=True, check=False, env=child_env)
    elapsed = time.time() - start
    (run_root / "stdout.bin").write_bytes(proc.stdout)
    (run_root / "stderr.bin").write_bytes(proc.stderr)
    output_manifest = manifest(project_dir)
    candidate_comparable = manifest(project_dir, comparable_only=True)
    after_comparable = manifest(AFTER, comparable_only=True)
    candidate_names, after_names = set(candidate_comparable), set(after_comparable)
    differences = []
    for name in sorted(candidate_names | after_names):
        if name not in candidate_comparable:
            differences.append({"path": name, "kind": "missing_candidate"})
        elif name not in after_comparable:
            differences.append({"path": name, "kind": "extra_candidate"})
        elif candidate_comparable[name] != after_comparable[name]:
            differences.append({
                "path": name,
                "kind": "content",
                "candidate": candidate_comparable[name],
                "after": after_comparable[name],
            })

    raw_mst_path = project_dir / "Gear_blank.MST"
    after_mst_path = AFTER / "Gear_blank.MST"
    path_check: dict[str, object] = {"performed": False}
    if raw_mst_path.is_file() and after_mst_path.is_file():
        raw_mst = raw_mst_path.read_bytes()
        original_mst = after_mst_path.read_bytes()
        candidate_key = (project_dir / "Gear_blank.KEY").as_posix().encode("ascii")
        raw_count = raw_mst.count(candidate_key)
        original_count = original_mst.count(ORIGINAL_KEY_PATH)
        if raw_count == 1 and original_count == 1:
            normalized = raw_mst.replace(candidate_key, ORIGINAL_KEY_PATH)
            normalized_path = run_root / "candidate.path-qualified.MST"
            normalized_path.write_bytes(normalized)
            path_check = {
                "performed": True,
                "candidate_path": candidate_key.decode("ascii"),
                "candidate_occurrences": raw_count,
                "retained_path": ORIGINAL_KEY_PATH.decode("ascii"),
                "retained_occurrences": original_count,
                "raw_candidate_bytes": len(raw_mst),
                "raw_candidate_sha256": sha256(raw_mst),
                "retained_bytes": len(original_mst),
                "retained_sha256": sha256(original_mst),
                "normalized_bytes": len(normalized),
                "normalized_sha256": sha256(normalized),
                "normalized_equal": normalized == original_mst,
                "normalized_copy": str(normalized_path),
            }

    expected_difference = (
        len(candidate_comparable) == EXPECTED_COMPARABLE_PATHS
        and len(after_comparable) == EXPECTED_COMPARABLE_PATHS
        and len(differences) == 1
        and differences[0].get("path") == "Gear_blank.MST"
        and differences[0].get("kind") == "content"
        and path_check.get("normalized_equal") is True
    )
    receipt = {
        "schema": "b98-before-after-candidate-acceptance-v2",
        "candidate_version": args.candidate_version,
        "freeze_receipt": str(freeze_receipt) if freeze_receipt else None,
        "freeze_receipt_layout": "current" if args.current_freeze_receipt else ("legacy" if args.freeze_receipt else None),
        "freeze_receipt_sha256": freeze_receipt_hash,
        "retained_before": str(BEFORE),
        "retained_after": str(AFTER),
        "retained_evidence_modified": False,
        "original_executable_launched": False,
        "candidate_cli": str(cli),
        "candidate_artifact_hashes": artifact_hashes,
        "candidate_source_tree_sha256": expected_source_sha256,
        "argv": argv,
        "cwd": str(project_dir),
        "effective_environment": {INPUT_MODE_ENV: child_env[INPUT_MODE_ENV]},
        "inherited_input_mode_overridden": inherited_mode != INPUT_MODE_VALUE,
        "parent_input_mode_before_launch": inherited_mode,
        "exit_code": proc.returncode,
        "elapsed_seconds": round(elapsed, 3),
        "stdout": {"bytes": len(proc.stdout), "sha256": sha256(proc.stdout)},
        "stderr": {"bytes": len(proc.stderr), "sha256": sha256(proc.stderr)},
        "run_root": str(run_root),
        "project_dir": str(project_dir),
        "max_generated_path_chars": max((len(str(p)) for p in project_dir.rglob("*")), default=0),
        "input_copy_manifest": input_manifest,
        "candidate_output_manifest": output_manifest,
        "after_comparable_manifest": after_comparable,
        "candidate_comparable_manifest": candidate_comparable,
        "path_count": {"candidate": len(candidate_comparable), "after": len(after_comparable)},
        "raw_differences": differences,
        "exact_one_path_normalization": path_check,
        "acceptance_pass": proc.returncode == 0 and expected_difference,
        "action_contract_note": (
            "This validates candidate output against retained before/after artifacts only; it does not replay or qualify the original GUI action."
        ),
        "artifacts_retained_no_cleanup": True,
    }
    receipt_path = run_root / "receipt.json"
    write_json(receipt_path, receipt)
    print(json.dumps({
        "acceptance_pass": receipt["acceptance_pass"],
        "exit_code": proc.returncode,
        "run_root": str(run_root),
        "receipt": str(receipt_path),
        "path_count": receipt["path_count"],
        "raw_differences": differences,
        "path_check": path_check,
    }, indent=2))
    return 0 if receipt["acceptance_pass"] else 1


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        print(f"B98 acceptance failed: {exc}", file=sys.stderr)
        raise SystemExit(2)
