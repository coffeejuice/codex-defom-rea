#!/usr/bin/env python3
"""Read-only B98 color-preflight fixture and -f argument validator.

This gate validates existing directories only. It never creates a clone or
spawns the original application. A matching manifest does not prove freshness.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import stat
import sys
from pathlib import Path
from typing import Any

EXPECTED_REFERENCE_SHA256 = "9857bb69d2d63fb65e6da1d6f618e9236249579e6bfebf6f902b237ffa6049ad"
EXPECTED_CONTENT_DIGEST = "55e2a58cf8155995f942f89ac0ebb91454308b521ae76a228edec0ffdc4cedbd"
EXPECTED_PROJECT_SHA256 = "e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd"
EXPECTED_SOURCE_ROOT = Path(
    r"C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\gui_mo\oracle_b\98_B_DS3F_before"
)
HISTORICAL_CLONE_ROOT = Path(r"C:\PROBLEM\p214a")
PROBLEM_ROOT = Path(r"C:\PROBLEM")
EXECUTABLE = Path(r"C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe")
PROJECT_NAME = "Gear_blank.moproj"
EXPECTED_ENTRIES = 33
EXPECTED_FILES = 27


class GateError(ValueError):
    pass


def _norm(path: Path) -> str:
    return os.path.normcase(os.path.abspath(os.fspath(path)))


def _inside(path: Path, root: Path) -> bool:
    try:
        return os.path.commonpath((_norm(path), _norm(root))) == _norm(root)
    except ValueError:
        return False


def _is_reparse(st: os.stat_result) -> bool:
    attrs = getattr(st, "st_file_attributes", 0)
    return bool(attrs & getattr(stat, "FILE_ATTRIBUTE_REPARSE_POINT", 0x400))


def _check_components(path: Path) -> None:
    """Reject symlinks/reparse points in the root's existing component chain."""
    absolute = Path(os.path.abspath(path))
    parts = [absolute, *absolute.parents]
    for component in reversed(parts):
        try:
            st = component.lstat()
        except OSError as exc:
            raise GateError(f"missing/unreadable path component: {component}: {exc}") from exc
        if stat.S_ISLNK(st.st_mode) or _is_reparse(st):
            raise GateError(f"reparse point in path: {component}")


def _stable_signature(st: os.stat_result) -> tuple[int, int, int, int, int]:
    return (st.st_dev, st.st_ino, st.st_size, st.st_mtime_ns, st.st_ctime_ns)


def _opened_file_identity(st: os.stat_result) -> tuple[int, int, int, int]:
    # Windows path stat and handle fstat expose ctime differently; compare
    # handle identity/size/mtime here and compare path ctime across lstat calls.
    return (st.st_dev, st.st_ino, st.st_size, st.st_mtime_ns)


def directory_manifest(root: Path, *, enforce_problem_containment: bool) -> dict[str, Any]:
    """Hash a tree without following links and reject changes during the read."""
    root = Path(os.path.abspath(root))
    if not root.is_dir():
        raise GateError(f"directory does not exist: {root}")
    if enforce_problem_containment and not _inside(root, PROBLEM_ROOT):
        raise GateError(f"clone escapes {PROBLEM_ROOT}: {root}")
    if enforce_problem_containment and len(os.fspath(root)) >= 100:
        raise GateError(f"project path is 100+ characters: {root}")
    _check_components(root)
    entries: list[dict[str, str]] = []
    stack = [root]
    while stack:
        parent = stack.pop()
        try:
            _check_components(parent)
            parent_before = parent.lstat()
            if stat.S_ISLNK(parent_before.st_mode) or _is_reparse(parent_before):
                raise GateError(f"reparse point in directory traversal: {parent}")
            with os.scandir(parent) as iterator:
                children = sorted(iterator, key=lambda item: item.name)
            parent_after = parent.lstat()
            if (_stable_signature(parent_before) != _stable_signature(parent_after)
                    or stat.S_ISLNK(parent_after.st_mode) or _is_reparse(parent_after)):
                raise GateError(f"directory changed during enumeration: {parent}")
            _check_components(parent)
        except OSError as exc:
            raise GateError(f"cannot enumerate {parent}: {exc}") from exc
        for child in children:
            path = Path(child.path)
            rel = path.relative_to(root).as_posix()
            if enforce_problem_containment and len(os.fspath(path)) >= 100:
                raise GateError(f"project path is 100+ characters: {path}")
            try:
                before = path.lstat()
            except OSError as exc:
                raise GateError(f"cannot stat {path}: {exc}") from exc
            if stat.S_ISLNK(before.st_mode) or _is_reparse(before):
                raise GateError(f"reparse point in tree: {path}")
            if stat.S_ISDIR(before.st_mode):
                entries.append({"kind": "directory", "path": rel})
                stack.append(path)
            elif stat.S_ISREG(before.st_mode):
                digest = hashlib.sha256()
                try:
                    with path.open("rb") as stream:
                        opened_before = os.fstat(stream.fileno())
                        if (not stat.S_ISREG(opened_before.st_mode)
                                or _is_reparse(opened_before)
                                or _opened_file_identity(before) != _opened_file_identity(opened_before)):
                            raise GateError(f"opened file identity differs from path entry: {path}")
                        while chunk := stream.read(1024 * 1024):
                            digest.update(chunk)
                        opened_after = os.fstat(stream.fileno())
                        after = path.lstat()
                except OSError as exc:
                    raise GateError(f"cannot hash {path}: {exc}") from exc
                if (stat.S_ISLNK(after.st_mode) or _is_reparse(after)
                        or _is_reparse(opened_after)
                        or _opened_file_identity(before) != _opened_file_identity(opened_before)
                        or _opened_file_identity(before) != _opened_file_identity(opened_after)
                        or _stable_signature(before) != _stable_signature(after)):
                    raise GateError(f"file changed while hashing: {path}")
                entries.append({"kind": "file", "path": rel,
                                "bytes": str(before.st_size), "sha256": digest.hexdigest()})
            else:
                raise GateError(f"unsupported filesystem entry: {path}")
    entries.sort(key=lambda entry: (entry["path"], entry["kind"]))
    return {"root": str(root), "entries": entries,
            "fileCount": sum(entry["kind"] == "file" for entry in entries),
            "entryCount": len(entries)}


def load_reference(path: Path) -> list[dict[str, str]]:
    raw = path.read_bytes()
    digest = hashlib.sha256(raw).hexdigest()
    if digest != EXPECTED_REFERENCE_SHA256:
        raise GateError(f"CP214 source reference receipt hash mismatch: {digest}")
    ref = json.loads(raw.decode("utf-8"))
    if (ref.get("root", "").casefold() != str(EXPECTED_SOURCE_ROOT).casefold()
            or ref.get("contentDigest") != EXPECTED_CONTENT_DIGEST
            or len(ref.get("entries", [])) != EXPECTED_ENTRIES
            or sum(e.get("kind") == "file" for e in ref.get("entries", [])) != EXPECTED_FILES):
        raise GateError("CP214 source reference identity/count mismatch")
    return ref["entries"]


def encode_utf16_units(text: str) -> str:
    raw = text.encode("utf-16-le", errors="strict")
    return "".join(f"{int.from_bytes(raw[i:i + 2], 'little'):04x}"
                   for i in range(0, len(raw), 2))


def decode_utf16_units(value: str) -> str:
    if not value or len(value) % 4 or re.fullmatch(r"[0-9a-f]+", value) is None:
        raise GateError("-f value must be nonempty lowercase hex in four-digit code-unit groups")
    raw = b"".join(int(value[i:i + 4], 16).to_bytes(2, "little")
                   for i in range(0, len(value), 4))
    try:
        text = raw.decode("utf-16-le", errors="strict")
    except UnicodeDecodeError as exc:
        raise GateError("-f value contains an invalid UTF-16 sequence") from exc
    if encode_utf16_units(text) != value:
        raise GateError("-f value is not canonical lowercase UTF-16 code-unit hex")
    return text


def validate(source: Path, clone: Path, encoded_path: str, cwd: Path,
             argv: list[str]) -> dict[str, Any]:
    reference_file = Path(__file__).resolve().parents[1] / "analysis" / "resume214-original-postopen-2026-10-26T13-17-52-922Z" / "source-fixture-reference.json"
    expected = load_reference(reference_file)
    source_abs, clone_abs = Path(os.path.abspath(source)), Path(os.path.abspath(clone))
    if _norm(source_abs) != _norm(EXPECTED_SOURCE_ROOT):
        raise GateError("source must be the fixed CP214 source fixture root")
    if _norm(clone_abs) == _norm(EXPECTED_SOURCE_ROOT):
        raise GateError("source and clone roots must be distinct")

    source_before = directory_manifest(source_abs, enforce_problem_containment=False)
    if source_before["entries"] != expected:
        raise GateError("current source tree differs from the frozen 27-file CP214 manifest")
    clone_before = directory_manifest(clone_abs, enforce_problem_containment=True)
    clone_manifest = directory_manifest(clone_abs, enforce_problem_containment=True)
    if clone_before != clone_manifest:
        raise GateError("clone changed during validation")
    if clone_manifest["entryCount"] != EXPECTED_ENTRIES or clone_manifest["fileCount"] != EXPECTED_FILES:
        raise GateError("clone must contain exactly 33 entries including 27 files")
    if clone_manifest["entries"] != expected:
        raise GateError("clone relative paths, sizes, kinds, or SHA-256 values differ from source")
    source_after = directory_manifest(source_abs, enforce_problem_containment=False)
    if source_before != source_after:
        raise GateError("source changed during validation")

    project = clone_abs / PROJECT_NAME
    if len(os.fspath(project)) >= 100:
        raise GateError("full project path must be shorter than 100 characters")
    decoded = decode_utf16_units(encoded_path)
    if os.path.normcase(decoded) != _norm(project):
        raise GateError("-f argument does not decode to the proposed clone project path")
    if encode_utf16_units(decoded) != encoded_path:
        raise GateError("-f argument failed canonical byte-for-byte re-encoding")
    if not _inside(project, PROBLEM_ROOT):
        raise GateError("project path escapes C:\\PROBLEM")
    if _norm(cwd) != _norm(clone_abs):
        raise GateError("cwd must equal the proposed clone directory")

    expected_argv = [str(EXECUTABLE), "-m", "open", "-f", encoded_path]
    if argv != expected_argv:
        raise GateError("argv must exactly equal [EXE, '-m', 'open', '-f', HEX]")
    historical = _norm(clone_abs) == _norm(HISTORICAL_CLONE_ROOT)
    return {
        "schema": "color-clone-manifest-gate-v1",
        "readyForIndependentReview": False,
        "status": "HOLD",
        "historicalIdentities": {
            "sourceRoot": str(EXPECTED_SOURCE_ROOT),
            "sourceContentDigest": EXPECTED_CONTENT_DIGEST,
            "historicalCloneRoot": str(HISTORICAL_CLONE_ROOT),
            "historicalCloneBeforeDigest": EXPECTED_CONTENT_DIGEST,
            "historicalCloneAfterDigest": "37ae637460c5f280b20a498018ea9d52fb47de2407b0ff511d7f177dd7c19c8a",
            "historicalCloneAfterOnlyDelta": "empty Lubricants directory",
        },
        "proposedClone": {
            "root": str(clone_abs),
            "identityClass": "historical CP214 clone" if historical else "proposed distinct path",
            "freshness": "not established by static manifest validation",
            "entryCount": clone_manifest["entryCount"],
            "fileCount": clone_manifest["fileCount"],
            "matchesFrozenSource": True,
            "stableAcrossRead": True,
        },
        "sourceStableAcrossRead": True,
        "sourceMatchesFrozenManifest": True,
        "projectPath": decoded,
        "projectSha256": EXPECTED_PROJECT_SHA256,
        "encodedPath": encoded_path,
        "canonicalUtf16CodeUnitHex": True,
        "argv": expected_argv,
        "cwd": str(Path(os.path.abspath(cwd))),
        "externalGates": {
            "fresh_destination_reservation_and_creation_receipt": "unknown",
            "original_pe_and_callsite_byte_gate": "unknown",
            "observer_coverage_and_overlap_proof": "unknown",
            "lifecycle_neutrality_ownership_cleanup_and_independent_policy_review": "unknown",
        },
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--clone", type=Path, required=True)
    parser.add_argument("--hex-path", required=True, help="literal -f argument")
    parser.add_argument("--cwd", type=Path, required=True)
    parser.add_argument("--argv-json", required=True,
                        help="JSON array, checked against exact expected argv")
    args = parser.parse_args()
    try:
        argv = json.loads(args.argv_json)
        if not isinstance(argv, list) or not all(isinstance(x, str) for x in argv):
            raise GateError("--argv-json must be a JSON array of strings")
        receipt = validate(args.source, args.clone, args.hex_path, args.cwd, argv)
        print(json.dumps(receipt, indent=2, sort_keys=True))
        return 0
    except (GateError, OSError, json.JSONDecodeError) as exc:
        print(json.dumps({"schema": "color-clone-manifest-gate-v1",
                          "readyForIndependentReview": False, "status": "HOLD",
                          "error": str(exc)}, indent=2, sort_keys=True))
        return 2


if __name__ == "__main__":
    sys.exit(main())
