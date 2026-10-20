"""Compare the reconstructed CLI with retained original-executable captures.

Runs only the new executable. Inputs are the private database copies named by
the capture manifests. Captured stdout/stderr/result files are read as bytes;
manifest text previews are not used as newline-normalized expected output.
"""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import time
import uuid


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(8 * 1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def main() -> int:
    workspace = Path(__file__).resolve().parents[1]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--exe", type=Path, default=workspace / "build/Release/deform_db.exe")
    parser.add_argument("--report", type=Path, default=workspace / "analysis/oracle-comparison.json")
    parser.add_argument("--timeout", type=float, default=30)
    args = parser.parse_args()
    executable = args.exe.resolve(strict=True)
    manifests = [
        workspace / "analysis/oracle/outside-sandbox-batch/manifest.json",
        workspace / "analysis/oracle/large-fixture/manifest.json",
        workspace / "analysis/oracle/malformed/manifest.json",
    ]
    run_root = workspace / "analysis/oracle/comparison" / uuid.uuid4().hex
    run_root.mkdir(parents=True)
    reports: list[dict] = []
    input_hashes: dict[Path, str] = {}
    binary_hash = sha256(executable)
    for manifest_path in manifests:
        manifest = json.loads(manifest_path.read_text(encoding="utf-8-sig"))
        for index, original in enumerate(manifest["runs"]):
            argv = original["argv"]
            if "-m" not in argv:
                continue
            mode = argv[argv.index("-m") + 1]
            if mode not in {"header", "stepList", "lastStepNo", "simStepList"}:
                continue
            if original["status"] != "completed":
                raise RuntimeError(f"Original capture did not complete: {manifest_path}:{index}")
            original_cwd = Path(original["cwd"]).resolve(strict=True)
            input_path = Path(argv[argv.index("-d") + 1])
            if not input_path.is_absolute():
                input_path = original_cwd / input_path
            input_path = input_path.resolve(strict=True)
            original_result_path = Path(argv[argv.index("-o") + 1])
            if not original_result_path.is_absolute():
                original_result_path = original_cwd / original_result_path
            expected_stdout = (original_cwd / "stdout.bin").read_bytes()
            expected_stderr = (original_cwd / "stderr.bin").read_bytes()
            expected_result = (
                original_result_path.read_bytes() if original_result_path.exists() else None
            )
            for field, expected in (
                ("stdout", expected_stdout), ("stderr", expected_stderr), ("result", expected_result)
            ):
                expected_hash = original.get(field + "_sha256")
                if expected is not None and expected_hash != digest(expected):
                    raise RuntimeError(f"Changed or missing capture hash: {original_cwd}:{field}")
            if (expected_result is None) != bool(original.get("result_missing", False)):
                raise RuntimeError(f"Capture result existence mismatch: {original_cwd}")
            if input_path not in input_hashes:
                input_hashes[input_path] = sha256(input_path)
            expected_input_hash = original.get(
                "input_copy_sha256_before", original.get("input_sha256_before")
            )
            if expected_input_hash != input_hashes[input_path]:
                raise RuntimeError(f"Fixture changed since capture: {input_path}")

            cwd = run_root / f"{len(reports):02d}-{mode}"
            cwd.mkdir()
            output_path = cwd / "result.txt"
            new_argv = [str(executable), "-m", mode, "-d", str(input_path),
                        "-o", str(output_path)]
            start = time.perf_counter()
            completed = subprocess.run(
                new_argv, cwd=cwd, capture_output=True, timeout=args.timeout,
                creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0),
            )
            actual_result = output_path.read_bytes() if output_path.exists() else None
            (cwd / "stdout.bin").write_bytes(completed.stdout)
            (cwd / "stderr.bin").write_bytes(completed.stderr)
            matches = {
                "exit_code": completed.returncode == original["exit_code"],
                "stdout": completed.stdout == expected_stdout,
                "stderr": completed.stderr == expected_stderr,
                "result_exists": (actual_result is None) == (expected_result is None),
                "result_bytes": actual_result == expected_result,
            }
            reports.append({
                "capture_manifest": str(manifest_path),
                "capture_run_index": index,
                "mode": mode,
                "variant": original.get("variant"),
                "input": str(input_path),
                "input_sha256": input_hashes[input_path],
                "argv": new_argv,
                "output_directory": str(cwd),
                "elapsed_seconds": round(time.perf_counter() - start, 6),
                "matches": matches,
                "all_match": all(matches.values()),
                "expected": {
                    "exit_code": original["exit_code"],
                    "stdout_sha256": digest(expected_stdout),
                    "stderr_sha256": digest(expected_stderr),
                    "result_sha256": digest(expected_result) if expected_result is not None else None,
                    "result_bytes": len(expected_result) if expected_result is not None else None,
                },
                "actual": {
                    "exit_code": completed.returncode,
                    "stdout_sha256": digest(completed.stdout),
                    "stderr_sha256": digest(completed.stderr),
                    "result_sha256": digest(actual_result) if actual_result is not None else None,
                    "result_bytes": len(actual_result) if actual_result is not None else None,
                },
            })
    unchanged = {str(path): sha256(path) == before for path, before in input_hashes.items()}
    binary_unchanged = sha256(executable) == binary_hash
    success = all(run["all_match"] for run in reports) and all(unchanged.values()) and binary_unchanged
    report = {
        "executable": str(executable),
        "executable_sha256": binary_hash,
        "executable_unchanged": binary_unchanged,
        "input_unchanged": unchanged,
        "comparison": "Exact bytes, file existence and process exit code; no text normalization",
        "runs": reports,
        "passed": sum(run["all_match"] for run in reports),
        "total": len(reports),
        "all_match": success,
        "limitations": [
            "These batch modes validate metadata and lists, not full array-loading equivalence.",
            "Malformed coverage is limited to the retained capture variants and modes.",
        ],
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({key: report[key] for key in ("passed", "total", "all_match")}))
    for run in reports:
        if not run["all_match"]:
            print(json.dumps({"mode": run["mode"], "input": run["input"],
                              "mismatches": [k for k, v in run["matches"].items() if not v]}))
    return 0 if success else 1


if __name__ == "__main__":
    raise SystemExit(main())
