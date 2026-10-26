"""Prepare/run focused synthetic failure checks for the opt-in folder pipeline.

All generated fixtures, scratch outputs, logs, and malformed IPC stay under a
unique build/test-speed-candidate run directory. Execution is opt-in with
--execute so a caller can coordinate around other suite activity first.
"""
from __future__ import annotations

import argparse
import ctypes
from ctypes import wintypes
import difflib
import hashlib
import json
import os
from pathlib import Path
import subprocess
import uuid


PROJECT = Path(__file__).resolve().parents[3]
CANDIDATE_ROOT = PROJECT / "build" / "test-speed-candidate"
DEFAULT_EXE = CANDIDATE_ROOT / "bin" / "m2m_s_save_folder.exe"
RUNS_ROOT = CANDIDATE_ROOT / "pipeline-failure-runs"
GENERIC_READ = 0x80000000
GENERIC_WRITE = 0x40000000
OPEN_EXISTING = 3
INVALID_HANDLE_VALUE = ctypes.c_void_p(-1).value


def assert_beneath(path: Path, parent: Path) -> Path:
    resolved = path.resolve()
    base = parent.resolve()
    if resolved == base or not resolved.is_relative_to(base):
        raise ValueError(f"Path must be a strict child of {base}: {resolved}")
    return resolved


def write(path: Path, content: bytes) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(content)


def make_fixtures(run_dir: Path) -> dict[str, Path]:
    roots = {
        "corpus": run_dir / "fixtures" / "corpus",
        "oracle_b": run_dir / "fixtures" / "oracle_b",
        "scratch": run_dir / "scratch",
    }
    for path in roots.values():
        path.mkdir(parents=True, exist_ok=True)
    # The A-corpus pass sees these names but has no completed RS entries.
    for name in ("11_THEXP", "19_XY_H01", "52_BEND_2D"):
        (roots["corpus"] / name).mkdir()
    write(roots["oracle_b"] / "_tools" / "rs_log.tsv", b"")

    # Fixed B cases exercise a normal converter refusal and two preflight drops:
    # no exact lowercase .moproj, and missing expected folder.
    write(roots["corpus"] / "11_THEXP" / "Bad.moproj", b"not an XML project\n")
    (roots["oracle_b"] / "R11").mkdir()
    (roots["corpus"] / "19_XY_H01").mkdir(exist_ok=True)
    (roots["oracle_b"] / "R19").mkdir()
    write(roots["corpus"] / "52_BEND_2D" / "Bad.moproj", b"not an XML project\n")
    # Deliberately do not create oracle_b/82_R52_BEND_2D.

    # Generic B cases exercise copy failure, a second refusal, no .moproj, and
    # absent expected output. Only the copy-failure sentinel is locked at run time.
    write(roots["oracle_b"] / "copyfail_before" / "Bad.moproj", b"not an XML project\n")
    write(roots["oracle_b"] / "copyfail_before" / "locked.bin", b"owned copy-failure sentinel\n")
    (roots["oracle_b"] / "copyfail").mkdir()
    write(roots["oracle_b"] / "refusal_before" / "Bad.moproj", b"not an XML project\n")
    (roots["oracle_b"] / "refusal").mkdir()
    write(roots["oracle_b"] / "nomoproj_before" / "README.txt", b"no project file\n")
    (roots["oracle_b"] / "nomoproj").mkdir()
    write(roots["oracle_b"] / "missingexpected_before" / "Bad.moproj", b"not an XML project\n")
    # Deliberately do not create oracle_b/missingexpected.
    return roots


def acquire_exclusive_lock(path: Path):
    if os.name != "nt":
        raise RuntimeError("The deterministic copy-failure lock requires Windows")
    kernel = ctypes.WinDLL("kernel32", use_last_error=True)
    kernel.CreateFileW.argtypes = [wintypes.LPCWSTR, wintypes.DWORD, wintypes.DWORD,
                                   wintypes.LPVOID, wintypes.DWORD, wintypes.DWORD,
                                   wintypes.HANDLE]
    kernel.CreateFileW.restype = wintypes.HANDLE
    kernel.CloseHandle.argtypes = [wintypes.HANDLE]
    kernel.CloseHandle.restype = wintypes.BOOL
    handle = kernel.CreateFileW(str(path), GENERIC_READ | GENERIC_WRITE, 0, None,
                                OPEN_EXISTING, 0, None)
    if handle == INVALID_HANDLE_VALUE:
        code = ctypes.get_last_error()
        raise OSError(code, ctypes.FormatError(code), str(path))
    return kernel, handle


def run_binary(exe: Path, roots: dict[str, Path], run_dir: Path,
               mode: str, hold_lock: bool) -> dict:
    scratch = run_dir / "scratch" / mode
    scratch.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    for key in ("M2M_FOLDER_WORKERS", "M2M_FOLDER_PROFILE", "M2M_FOLDER_TIMING"):
        env.pop(key, None)
    env.update({
        "M2M_FOLDER_SWEEP": "1",
        "M2M_CORPUS": str(roots["corpus"]),
        "M2M_ORACLE_B": str(roots["oracle_b"]),
        "M2M_TEST_SCRATCH": str(scratch),
        "TEMP": str(scratch), "TMP": str(scratch), "TMPDIR": str(scratch),
    })
    if mode == "pipeline":
        env["M2M_FOLDER_PIPELINE"] = "1"
    else:
        env.pop("M2M_FOLDER_PIPELINE", None)

    lock = None
    try:
        if hold_lock:
            lock = acquire_exclusive_lock(roots["oracle_b"] / "copyfail_before" / "locked.bin")
        completed = subprocess.run([str(exe)], cwd=run_dir, env=env,
                                    capture_output=True, text=False, timeout=300)
    finally:
        if lock:
            kernel, handle = lock
            kernel.CloseHandle(handle)
    stdout = completed.stdout.decode("utf-8", errors="replace")
    stderr = completed.stderr.decode("utf-8", errors="replace")
    (run_dir / f"{mode}.stdout.bin").write_bytes(completed.stdout)
    (run_dir / f"{mode}.stderr.bin").write_bytes(completed.stderr)
    result = {"mode": mode, "return_code": completed.returncode,
              "stdout": stdout, "stderr": stderr,
              "stdout_sha256": hashlib.sha256(completed.stdout).hexdigest(),
              "stderr_sha256": hashlib.sha256(completed.stderr).hexdigest()}
    expected_markers = {
        "refusal": "REFUSED B:refusal (",
        "copy_failure": "SKIP B:copyfail: copy failed (",
        "normal_refusal": "REFUSED B:R11 (",
        "section_summary": "oracle B: 3 cases, 0 identical (0 with an expected older-window .moproj view profile), 2 refused, 0 file diffs, 0 files with another version banner",
    }
    marker_results = {name: token in stdout for name, token in expected_markers.items()}
    omitted = ("B:R19", "B:82_R52_BEND_2D", "B:nomoproj", "B:missingexpected")
    marker_results["preflight_omissions_silent"] = all(name not in stdout for name in omitted)
    marker_results["no_test_failure"] = "[FAIL] save_folder_sweep" not in stdout and "1 of 1 cases failed" not in stdout
    marker_results["testkit_passed"] = "0 of 1 cases failed" in stdout
    result["assertions"] = marker_results
    return result


def run_malformed_helper(exe: Path, run_dir: Path) -> dict:
    work = run_dir / "malformed-helper"
    work.mkdir()
    manifest = work / "job.bin"
    manifest.write_bytes(b"\x05\x00")  # incomplete length prefix; helper should reject, not hang/pass
    completed = subprocess.run([str(exe), "--folder-post-worker", str(manifest)],
                               cwd=run_dir, capture_output=True, timeout=30)
    stdout = completed.stdout.decode("utf-8", errors="replace")
    stderr_path = work / "worker.stderr"
    stderr = stderr_path.read_text(errors="replace") if stderr_path.exists() else completed.stderr.decode(errors="replace")
    (run_dir / "malformed-helper.stdout.bin").write_bytes(completed.stdout)
    (run_dir / "malformed-helper.stderr.bin").write_text(stderr, encoding="utf-8")
    return {"return_code": completed.returncode, "stdout": stdout, "stderr": stderr,
            "assertions": {"nonzero_exit": completed.returncode != 0,
                           "no_stdout_partial_success": not stdout,
                           "truncated_ipc_diagnostic": "Truncated folder IPC" in stderr}}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--test-exe", type=Path, default=DEFAULT_EXE)
    parser.add_argument("--execute", action="store_true",
                        help="run serial, pipeline, and malformed private-helper checks after preparation")
    args = parser.parse_args()
    candidate = CANDIDATE_ROOT.resolve()
    exe = assert_beneath(args.test_exe, candidate)
    run_dir = assert_beneath(RUNS_ROOT / uuid.uuid4().hex[:10], CANDIDATE_ROOT)
    run_dir.mkdir(parents=True)
    roots = make_fixtures(run_dir)
    result = {"run_dir": str(run_dir), "test_executable": str(exe),
              "execution_requested": args.execute,
              "fixtures": {key: str(value) for key, value in roots.items()},
              "execution_status": "prepared-only",
              "limitations": ["No parent-helper crash/incomplete-result injection seam was found in source; direct malformed private-helper check is narrower."]}
    if args.execute:
        if not exe.is_file():
            parser.error(f"Isolated test executable does not exist: {exe}")
        serial = run_binary(exe, roots, run_dir, "serial", hold_lock=True)
        pipeline = run_binary(exe, roots, run_dir, "pipeline", hold_lock=True)
        malformed = run_malformed_helper(exe, run_dir)
        outputs_equal = (serial["return_code"] == pipeline["return_code"] == 0 and
                         (run_dir / "serial.stdout.bin").read_bytes() ==
                         (run_dir / "pipeline.stdout.bin").read_bytes() and
                         (run_dir / "serial.stderr.bin").read_bytes() ==
                         (run_dir / "pipeline.stderr.bin").read_bytes())
        if not outputs_equal:
            diff = "\n".join(difflib.unified_diff(
                serial["stdout"].splitlines(), pipeline["stdout"].splitlines(),
                fromfile="serial.stdout", tofile="pipeline.stdout", lineterm="")) + "\n"
            (run_dir / "serial-vs-pipeline.diff").write_text(diff, encoding="utf-8")
        assertions = {
            "serial_expected_behavior": all(serial["assertions"].values()) and serial["return_code"] == 0,
            "pipeline_expected_behavior": all(pipeline["assertions"].values()) and pipeline["return_code"] == 0,
            "serial_pipeline_ordered_logs_identical": outputs_equal,
            "malformed_helper_rejected": all(malformed["assertions"].values()),
        }
        result.update({"execution_status": "passed" if all(assertions.values()) else "failed",
                       "serial": serial, "pipeline": pipeline,
                       "malformed_private_helper": malformed,
                       "coordinator_partial_output_on_helper_failure": "not injectable in current source" if
                       not any("M2M_FOLDER_PIPELINE_FAIL" in line for line in
                               (PROJECT / "optimization/test-speed/source/apps/moproj2mst/tests/s/test_save_folder.cpp").read_text(encoding="utf-8").splitlines())
                       else "requires a follow-up seam-specific run",
                       "assertions": assertions})
    receipt = run_dir / "receipt.json"
    receipt.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"run_dir": str(run_dir), "execution_status": result["execution_status"],
                      "receipt": str(receipt)}, indent=2))
    return 0 if result["execution_status"] in ("prepared-only", "passed") else 1


if __name__ == "__main__":
    raise SystemExit(main())
