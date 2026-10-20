"""Capture bounded, hidden original executable runs against private DB copies."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import time


def digest(path: Path) -> str:
    sha = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(8 * 1024 * 1024), b""):
            sha.update(chunk)
    return sha.hexdigest()


def run(exe: Path, args: list[str], directory: Path, timeout: float) -> dict:
    start = time.monotonic()
    stdout = b""
    stderr = b""
    status = "completed"
    code = None
    try:
        result = subprocess.run(
            [str(exe), *args],
            cwd=directory,
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=timeout,
            creationflags=subprocess.CREATE_NO_WINDOW,
        )
        stdout, stderr, code = result.stdout, result.stderr, result.returncode
    except subprocess.TimeoutExpired as error:
        status = "timeout"
        stdout, stderr = error.stdout or b"", error.stderr or b""
    except OSError as error:
        status = "launch_error"
        stderr = str(error).encode()
    (directory / "stdout.bin").write_bytes(stdout)
    (directory / "stderr.bin").write_bytes(stderr)
    record = {
        "argv": [str(exe), *args],
        "cwd": str(directory),
        "status": status,
        "exit_code": code,
        "elapsed_seconds": round(time.monotonic() - start, 3),
        "timeout_seconds": timeout,
        "stdout": stdout.decode("utf-8", errors="replace"),
        "stderr": stderr.decode("utf-8", errors="replace"),
        "stdout_sha256": hashlib.sha256(stdout).hexdigest(),
        "stderr_sha256": hashlib.sha256(stderr).hexdigest(),
    }
    (directory / "run.json").write_text(json.dumps(record, indent=2))
    return record


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--fixtures", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--timeout", type=float, default=15)
    parser.add_argument("--max-copy-bytes", type=int, default=64 * 1024 * 1024)
    parser.add_argument("--reuse-copy", action="store_true",
                        help="reuse one private copy for each fixture's four query modes")
    args = parser.parse_args()
    exe = args.exe.resolve()
    output = args.output.resolve()
    if output.exists() and any(output.iterdir()):
        parser.error("output directory must be new or empty to prevent stale captures")
    output.mkdir(parents=True, exist_ok=True)
    report = {
        "executable": str(exe),
        "executable_sha256": digest(exe),
        "fixtures": [],
        "runs": [],
    }
    help_dir = output / "help"
    help_dir.mkdir(exist_ok=True)
    report["runs"].append(run(exe, ["-h"], help_dir, args.timeout))
    sources = [args.fixtures] if args.fixtures.is_file() else sorted(args.fixtures.rglob("*"))
    for source in sources:
        if not source.is_file() or source.suffix.lower() != ".db":
            continue
        entry = {"source": str(source), "size": source.stat().st_size}
        report["fixtures"].append(entry)
        if entry["size"] > args.max_copy_bytes:
            entry["status"] = "skipped_copy_size_limit"
            continue
        entry["sha256_before"] = digest(source)
        shared_copy = output / source.stem / "input.db"
        previous_copy_hash = None
        if args.reuse_copy:
            if shutil.disk_usage(output).free < entry["size"] + 1024 * 1024 * 1024:
                raise RuntimeError("insufficient free space for private copy plus 1 GiB reserve")
            shared_copy.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(source, shared_copy)
            previous_copy_hash = digest(shared_copy)
            entry["reused_private_copy"] = str(shared_copy)
            entry["private_copy_initial_sha256"] = previous_copy_hash
        for mode in ("header", "stepList", "lastStepNo", "simStepList"):
            directory = output / source.stem / mode
            directory.mkdir(parents=True, exist_ok=True)
            copy = shared_copy if args.reuse_copy else directory / "input.db"
            if not args.reuse_copy:
                shutil.copyfile(source, copy)
            copy_before = previous_copy_hash if args.reuse_copy else digest(copy)
            record = run(
                exe, ["-m", mode, "-d", str(copy), "-o", "result.txt"],
                directory, args.timeout
            )
            record["input_copy_sha256_before"] = copy_before
            record["input_copy_sha256_after"] = digest(copy)
            previous_copy_hash = record["input_copy_sha256_after"]
            if copy_before != previous_copy_hash:
                raise RuntimeError("reference unexpectedly modified private fixture copy")
            result_path = directory / "result.txt"
            if result_path.exists():
                record["result_sha256"] = digest(result_path)
                record["result_text"] = result_path.read_text(errors="replace")
            else:
                record["result_missing"] = True
            (directory / "run.json").write_text(json.dumps(record, indent=2))
            report["runs"].append(record)
        entry["sha256_after"] = digest(source)
        entry["status"] = "captured"
    (output / "manifest.json").write_text(json.dumps(report, indent=2))
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
