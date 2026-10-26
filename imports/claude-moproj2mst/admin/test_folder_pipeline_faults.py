"""Test the direct folder post-worker protocol without invoking conversion.

An explicit, hash-pinned existing build is required. Every mutation is below a
new scratch directory beneath this takeover's build directory. This driver does
not test parent injection, parent cleanup, GUI behavior or converter fidelity.
"""

from __future__ import annotations

import argparse
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import time
import uuid


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "source/apps/moproj2mst/tests/s/test_save_folder.cpp"
JOB = b"M2M_FOLDER_POST_JOB_MINIMAL_1"
READY = b"M2M_FOLDER_POST_READY_MINIMAL_1"
RESULT = b"M2M_FOLDER_POST_RESULT_MINIMAL_1"
ERROR_PREFIX = b"Folder post helper infrastructure failure: "
MAX_FIELD = 64 * 1024 * 1024


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def number(value: int) -> bytes:
    return struct.pack("<Q", value)


def field(value: bytes | str) -> bytes:
    data = value.encode("utf-8") if isinstance(value, str) else value
    return number(len(data)) + data


def job(cases: list[tuple[int, Path, Path, str]]) -> bytes:
    data = field(JOB) + number(len(cases))
    for ordinal, expected, work, name in cases:
        data += number(ordinal) + field(str(expected)) + field(str(work)) + field(name)
    return data


def ready(ordinal: int, compare: int) -> bytes:
    return field(READY) + number(ordinal) + number(compare)


class Reader:
    """Bounded exact reads; a missing or truncated field is never empty success."""

    def __init__(self, data: bytes):
        self.data = data
        self.offset = 0

    def take(self, size: int) -> bytes:
        if size > len(self.data) - self.offset:
            raise ValueError("Truncated result field")
        value = self.data[self.offset:self.offset + size]
        self.offset += size
        return value

    def number(self) -> int:
        return struct.unpack("<Q", self.take(8))[0]

    def field(self) -> bytes:
        size = self.number()
        if size > MAX_FIELD:
            raise ValueError("Oversized result field")
        return self.take(size)

    def end(self) -> None:
        if self.offset != len(self.data):
            raise ValueError("Unexpected result tail")


def manifest(directory: Path) -> dict:
    return {
        "directories": sorted(p.relative_to(directory).as_posix()
                              for p in directory.rglob("*") if p.is_dir()),
        "files": [
            {"path": p.relative_to(directory).as_posix(),
             "bytes": p.stat().st_size, "sha256": sha256(p)}
            for p in sorted(directory.rglob("*")) if p.is_file()
        ],
    }


def prepare_case(index: int, directory: Path) -> tuple[bytes, int, bytes | None]:
    """Return job bytes, expected exit code and expected error substring."""
    expected, work = directory / "expected", directory / "work"
    if index == 9:
        expected.write_bytes(b"deliberately a regular file\n")
    else:
        expected.mkdir()
    work.mkdir()
    if index == 10:
        (expected / "a.txt").write_bytes(b"identical owned fixture\n")
        (work / "a.txt").write_bytes((expected / "a.txt").read_bytes())

    data = job([(1, expected, work, "unit")])
    code, error, request = 2, None, None
    if index == 0:
        data, code = job([]), 0
    elif index == 1:
        data, error = b"\0" * 3, b"Truncated folder IPC"
    elif index == 2:
        data, error = number(10) + b"abc", b"Truncated folder IPC field"
    elif index == 3:
        data, error = field(JOB) + number(100001), b"Oversized folder post job"
    elif index == 4:
        request, error = b"", b"Truncated folder IPC"
    elif index == 5:
        request, error = ready(2, 0), b"Post request identity mismatch"
    elif index == 6:
        request, error = ready(1, 2), b"Invalid folder post operation"
    elif index == 7:
        request, error = ready(1, 0) + b"x", b"Unexpected folder IPC tail"
    elif index == 8:
        request, code = ready(1, 0), 0
    elif index == 9:
        request = ready(1, 1)
    elif index == 10:
        request, code = ready(1, 1), 0
    else:
        raise ValueError("Unknown direct-worker case")
    (directory / "job.bin").write_bytes(data)
    if request is not None:
        (directory / "1.ready").write_bytes(request)
    return data, code, error


def check_case(index: int, directory: Path, data: bytes, exit_code: int,
               expected_exit: int, error: bytes | None) -> tuple[list[str], dict]:
    failures = []
    details = {}

    def require(condition: bool, message: str) -> None:
        if not condition:
            failures.append(message)

    require(exit_code == expected_exit,
            f"Expected exit {expected_exit}; received {exit_code}")
    worker_stderr_path = directory / "worker.stderr"
    require(worker_stderr_path.is_file(), "Missing worker.stderr receipt")
    worker_stderr = worker_stderr_path.read_bytes() if worker_stderr_path.is_file() else b""
    require((directory / "launcher.stdout").read_bytes() == b"", "Unexpected launcher stdout")
    require((directory / "launcher.stderr").read_bytes() == b"", "Unexpected launcher stderr")
    if expected_exit == 0:
        require(worker_stderr == b"", "Successful worker emitted stderr")
    else:
        require(worker_stderr.startswith(ERROR_PREFIX), "Missing worker failure prefix")
        if error is not None:
            require(error in worker_stderr, "Expected worker diagnostic absent")
        if index == 9:
            require(any(token in worker_stderr for token in
                        (b"filesystem error", b"recursive_directory_iterator")),
                    "Comparator failure lacks filesystem-operation diagnostic")
        details["worker_error"] = worker_stderr.decode("utf-8", errors="backslashreplace")

    done_files = sorted(p.name for p in directory.glob("*.done"))
    require(done_files == (["1.done"] if index in (8, 10) else []),
            "Unexpected set of published done results")
    if index in (8, 10) and (directory / "1.done").is_file():
        try:
            reader = Reader((directory / "1.done").read_bytes())
            require(reader.field() == RESULT, "Result schema mismatch")
            require(reader.field() == data, "Result manifest identity mismatch")
            require(reader.number() == 1, "Result ordinal mismatch")
            compare = reader.number()
            require(compare == int(index == 10), "Result compare operation mismatch")
            if index == 10:
                tally = [reader.number() for _ in range(6)]
                output = reader.field()
                details["tally"] = tally
                details["result_output_hex"] = output.hex()
                require(tally == [1, 1, 0, 0, 0, 0], "Comparison tally mismatch")
                require(output == b"SAME unit: 0 differ, 0 missing, 0 extra\r\n",
                        "Comparison output bytes mismatch")
            reader.end()
        except (ValueError, struct.error) as exc:
            failures.append(f"Cannot parse exact worker result: {exc}")
    require((directory / "work").is_dir() == (index != 10), "Work leaf cleanup mismatch")
    if index != 10 and (directory / "work").is_dir():
        require(not any((directory / "work").iterdir()), "Owned empty work fixture changed")
    if index == 10:
        require((directory / "expected" / "a.txt").read_bytes() == b"identical owned fixture\n",
                "Expected fixture changed")
        require(sorted(p.name for p in (directory / "expected").iterdir()) == ["a.txt"],
                "Expected fixture contains unexpected entries")
    elif index == 9:
        require((directory / "expected").read_bytes() == b"deliberately a regular file\n",
                "Expected regular-file fixture changed")
    else:
        require((directory / "expected").is_dir() and not any((directory / "expected").iterdir()),
                "Expected empty fixture changed")
    return failures, details


def run_case(index: int, scratch: Path, executable: Path, timeout: float,
             deadline: float) -> dict:
    directory = scratch / f"W{index}"
    directory.mkdir()
    started = time.monotonic()
    record = {"id": f"W{index}", "directory": str(directory), "status": "failed"}
    process = None
    try:
        data, expected_exit, error = prepare_case(index, directory)
        record["before"] = manifest(directory)
        record["expected_exit_code"] = expected_exit
        record["job_sha256"] = hashlib.sha256(data).hexdigest()
        command = [str(executable), "--folder-post-worker", str(directory / "job.bin")]
        record["command"] = command
        remaining = deadline - time.monotonic()
        if remaining <= 0:
            record.update(status="not_run", failures=["Suite deadline exhausted"])
            return record
        environment = dict(os.environ)
        for key in list(environment):
            if key.upper() in {"M2M_TEST_SCRATCH", "TEMP", "TMP", "TMPDIR",
                               "M2M_CORPUS", "M2M_ORACLE_B", "M2M_FOLDER_SWEEP",
                               "M2M_FOLDER_PIPELINE", "M2M_FOLDER_PREFETCH",
                               "M2M_FOLDER_WORKERS", "M2M_FOLDER_RAW_RECEIPT",
                               "M2M_FOLDER_PROFILE"}:
                del environment[key]
        for key in ("M2M_TEST_SCRATCH", "TEMP", "TMP", "TMPDIR"):
            environment[key] = str(directory)
        record["environment_sha256"] = hashlib.sha256(
            json.dumps(environment, sort_keys=True).encode("utf-8")).hexdigest()
        record["environment_overrides"] = {key: str(directory) for key in
                                            ("M2M_TEST_SCRATCH", "TEMP", "TMP", "TMPDIR")}
        with (directory / "launcher.stdout").open("wb") as output, \
                (directory / "launcher.stderr").open("wb") as errors:
            record["launched_utc"] = datetime.now(timezone.utc).isoformat()
            process = subprocess.Popen(command, cwd=directory, env=environment,
                                       stdout=output, stderr=errors, shell=False,
                                       creationflags=subprocess.CREATE_NO_WINDOW)
            record["pid"] = process.pid
            try:
                record["exit_code"] = process.wait(timeout=min(timeout, remaining))
            except subprocess.TimeoutExpired:
                process.kill()
                record["exit_code"] = process.wait(timeout=5)
                record.update(status="timeout", failures=["Owned worker exceeded timeout"],
                              driver_terminated_worker=True)
        if record["status"] != "timeout":
            failures, details = check_case(index, directory, data, record["exit_code"],
                                          expected_exit, error)
            record.update(details)
            record.update(status="failed" if failures else "passed", failures=failures)
    except Exception as exc:
        record.update(status="infrastructure_error", failures=[f"{type(exc).__name__}: {exc}"])
    finally:
        if process is not None and process.poll() is None:
            # Direct dispatch has no child creation. This is only our Popen handle.
            process.kill()
            try:
                process.wait(timeout=5)
            except subprocess.TimeoutExpired:
                record.setdefault("failures", []).append("Cannot reap owned worker")
            record["driver_terminated_worker"] = True
            record["status"] = "infrastructure_error"
        record["elapsed_seconds"] = time.monotonic() - started
        record["after"] = manifest(directory)
    return record


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--exe", required=True, type=Path)
    parser.add_argument("--exe-sha256", required=True)
    parser.add_argument("--scratch", required=True, type=Path,
                        help="New absolute directory strictly below this checkout's build directory")
    parser.add_argument("--timeout-seconds", type=float, default=5.0)
    parser.add_argument("--suite-timeout-seconds", type=float, default=120.0)
    args = parser.parse_args()
    if os.name != "nt":
        parser.error("The post-worker executable and framing are Windows-specific")
    if not 0.1 <= args.timeout_seconds <= 20 or not 1 <= args.suite_timeout_seconds <= 120:
        parser.error("Timeout must be 0.1..20 seconds and suite timeout 1..120 seconds")
    if not re.fullmatch(r"[0-9a-fA-F]{64}", args.exe_sha256):
        parser.error("--exe-sha256 must be the explicit 64-digit expected executable hash")
    if not args.exe.is_absolute() or not args.scratch.is_absolute():
        parser.error("--exe and --scratch must be absolute paths")
    executable = args.exe.resolve(strict=True)
    build = (ROOT / "build").resolve(strict=True)
    scratch = args.scratch.resolve()
    if not executable.is_relative_to(build) or executable.name.lower() != "m2m_s_save_folder.exe":
        parser.error("--exe must identify m2m_s_save_folder.exe beneath this checkout's build directory")
    if not executable.is_file() or sha256(executable) != args.exe_sha256.lower():
        parser.error("Executable file/hash mismatch; no worker was launched")
    if scratch == build or not scratch.is_relative_to(build):
        parser.error("--scratch must resolve strictly below this checkout's build directory")
    if scratch.exists() or args.scratch.is_symlink():
        parser.error("Preserve existing scratch; choose a new directory")
    scratch.mkdir(parents=True, exist_ok=False)
    started = time.monotonic()
    receipt = {
        "schema": "M2M_DIRECT_POST_WORKER_FAULTS_1",
        "attempt_uuid": str(uuid.uuid4()),
        "started_utc": datetime.now(timezone.utc).isoformat(),
        "executable": str(executable), "exe_sha256": args.exe_sha256.lower(),
        "source": str(SOURCE), "source_sha256": sha256(SOURCE),
        "driver_sha256": sha256(Path(__file__).resolve()),
        "scratch": str(scratch), "timeout_seconds": args.timeout_seconds,
        "suite_timeout_seconds": args.suite_timeout_seconds,
        "qualification": "Direct post-worker protocol only; no conversion, parent injection, "
                         "parent cleanup, GUI, original-app or converter parity claim.",
        "cases": [],
    }
    for index in range(11):
        receipt["cases"].append(run_case(index, scratch, executable, args.timeout_seconds,
                                         started + args.suite_timeout_seconds))
        # Preserve progress even when a later attempt or the driver is interrupted.
        (scratch / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    receipt["exe_sha256_after"] = sha256(executable)
    receipt["elapsed_seconds"] = time.monotonic() - started
    receipt["passed"] = sum(case["status"] == "passed" for case in receipt["cases"])
    receipt["registered"] = 11
    receipt["accepted"] = receipt["passed"] == 11 and receipt["exe_sha256_after"] == receipt["exe_sha256"]
    (scratch / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    rows = ["# Direct post-worker fault results", "", receipt["qualification"], "",
            f"Executable SHA-256: `{receipt['exe_sha256']}`", "",
            "| Case | Status | Exit | Seconds |", "|---|---|---:|---:|"]
    rows += [f"| {case['id']} | {case['status']} | {case.get('exit_code', '')} | "
             f"{case['elapsed_seconds']:.3f} |" for case in receipt["cases"]]
    rows += ["", f"Passed: {receipt['passed']}/11. Accepted: {receipt['accepted']}.", "",
             "Full protocol/file/stream receipts are in `receipt.json` and the W* directories."]
    (scratch / "receipt.md").write_text("\n".join(rows) + "\n", encoding="utf-8")
    print(json.dumps({"receipt": str(scratch / "receipt.json"), "passed": receipt["passed"],
                      "registered": 11, "accepted": receipt["accepted"]}))
    return 0 if receipt["accepted"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
