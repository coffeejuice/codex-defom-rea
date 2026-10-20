"""Compare current C++ decoded buffers with retained passive REA observations."""
from __future__ import annotations

import hashlib
import json
from pathlib import Path
import subprocess
import sys
import uuid


def sha256(path: Path) -> str:
    result = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(8 * 1024 * 1024), b""):
            result.update(block)
    return result.hexdigest()


def main() -> int:
    root = Path(__file__).resolve().parents[1]
    executable = root / "build/Release/deform_dump.exe"
    source = root / "tools/observe_loader.js"
    cases = [
        (25, "runtime-step25.json", "rea-step25"),
        (690, "runtime-step690-v2.json", "rea-step690-v2"),
    ]
    output = root / "analysis/oracle/memory-comparison" / uuid.uuid4().hex
    output.mkdir(parents=True)
    report = {"executable": str(executable), "executable_sha256": sha256(executable),
              "host_byteorder": sys.byteorder, "cases": []}
    if sys.byteorder != "little":
        raise RuntimeError("Retained original buffers are little-endian x64")
    for step, evidence_name, run_name in cases:
        evidence = json.loads((root / "analysis" / evidence_name).read_text())
        capture_path = root / "analysis/oracle/interactive" / run_name / "run.json"
        capture = json.loads(capture_path.read_text())
        observation = evidence["normalized_result"]
        assert capture["status"] == "completed" and capture["exit_code"] == 0
        assert capture["pid"] == observation["pid"]
        assert sha256(Path(capture["executable"])) == capture["executable_sha256"]
        assert sha256(source) == observation["source_sha256"]
        fixture = Path(capture["input_copy"])
        assert (sha256(fixture) == capture["input_copy_sha256_before"]
                == capture["input_copy_sha256_after"])
        assert observation["cleanup_error"] is None
        assert observation["messages_truncated"] is False
        returns = [m["payload"] for m in observation["messages"]
                   if m.get("type") == "send"
                   and m.get("payload", {}).get("operation") == "loader-return"]
        assert returns and all(r["status"] == 0 for r in returns)
        prefix = output / f"step{step}"
        run = subprocess.run(
            [str(executable), str(fixture), str(step), str(prefix)],
            capture_output=True, timeout=60,
            creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0),
        )
        if run.returncode:
            raise RuntimeError(run.stderr.decode(errors="replace"))
        case = {"step": step, "evidence_id": evidence["evidence_id"],
                "capture_manifest": str(capture_path), "pid": capture["pid"],
                "original_executable_sha256": capture["executable_sha256"],
                "fixture_sha256": capture["input_copy_sha256_before"], "buffers": []}
        for field, suffix, width in [
            ("header", "header", 8), ("integer", "integers", 4), ("real", "reals", 8)
        ]:
            candidates = [r for r in returns if f"{field}_sha256" in r]
            assert candidates
            expected = {(r[f"{field}_count"], r[f"{field}_sha256"]) for r in candidates}
            assert len(expected) == 1, "Original repeated reads disagree"
            count, original_hash = expected.pop()
            path = Path(str(prefix) + f".{suffix}.bin")
            actual_hash = sha256(path)
            matches = actual_hash == original_hash and path.stat().st_size == count * width
            case["buffers"].append({
                "field": field, "count": count, "bytes": path.stat().st_size,
                "original_sha256": original_hash, "cpp_sha256": actual_hash, "matches": matches,
            })
        case["all_match"] = all(b["matches"] for b in case["buffers"])
        report["cases"].append(case)
    report["all_match"] = all(c["all_match"] for c in report["cases"])
    (root / "analysis/runtime-comparison.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8"
    )
    for case in report["cases"]:
        print(f"step {case['step']}: {'MATCH' if case['all_match'] else 'MISMATCH'} "
              "(header, integer, real buffers)")
    return 0 if report["all_match"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
