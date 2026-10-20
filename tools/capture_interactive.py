"""Run one bounded interactive reference transcript in a new private directory."""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from capture_oracle import digest


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", required=True, type=Path)
    parser.add_argument("--fixture", required=True, type=Path)
    parser.add_argument("--reuse-existing-fixture", action="store_true",
                        help="reuse an existing private copy under analysis/oracle")
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--input", required=True, type=Path)
    parser.add_argument("--timeout", type=float, default=10)
    parser.add_argument("--max-output-bytes", type=int, default=2 * 1024 * 1024)
    parser.add_argument("--ready-gate", type=Path,
                        help="wait for this new file before writing the transcript to stdin")
    parser.add_argument("--pid-file", type=Path,
                        help="write launched process identity for a passive observer")
    parser.add_argument("--gate-timeout", type=float, default=120,
                        help="maximum seconds waiting for observer readiness (at most 120)")
    args = parser.parse_args()
    if bool(args.ready_gate) != bool(args.pid_file):
        parser.error("--ready-gate and --pid-file must be supplied together")
    if not 0 < args.gate_timeout <= 120:
        parser.error("--gate-timeout must be greater than zero and at most 120")
    ready_gate = args.ready_gate.resolve() if args.ready_gate else None
    pid_file = args.pid_file.resolve() if args.pid_file else None
    if ready_gate and ready_gate.exists():
        parser.error("ready gate already exists; use a new path to avoid stale readiness")
    if pid_file and pid_file.exists():
        parser.error("PID file already exists; use a new path")
    root = args.output.resolve()
    root.mkdir(parents=True, exist_ok=False)
    if args.reuse_existing_fixture:
        copy = args.fixture.resolve()
        oracle_root = Path(__file__).resolve().parents[1] / "analysis" / "oracle"
        if not copy.is_relative_to(oracle_root) or not copy.is_file():
            parser.error("reused fixture must be an existing private copy under analysis/oracle")
    else:
        copy = root / "input.db"
        shutil.copyfile(args.fixture, copy)
    before = digest(copy)
    stdin = args.input.read_bytes()
    (root / "stdin.bin").write_bytes(stdin)
    start = time.monotonic()
    status = "completed"
    code = None
    gate_wait_seconds = 0.0
    process_id = None
    with (root / "stdout.bin").open("wb") as stdout, (root / "stderr.bin").open("wb") as stderr:
        try:
            proc = subprocess.Popen(
                [str(args.exe.resolve())], cwd=root,
                stdin=subprocess.PIPE, stdout=stdout, stderr=stderr,
                creationflags=subprocess.CREATE_NO_WINDOW,
            )
            process_id = proc.pid
            if ready_gate:
                pid_file.parent.mkdir(parents=True, exist_ok=True)
                identity = {
                    "pid": proc.pid,
                    "executable": str(args.exe.resolve()),
                    "cwd": str(root),
                    "ready_gate": str(ready_gate),
                    "input_copy": str(copy),
                    "input_copy_sha256_before": before,
                }
                pid_file.write_text(json.dumps(identity, indent=2))
                print(json.dumps({"observer_waiting": identity}), flush=True)
                gate_start = time.monotonic()
                while not ready_gate.exists() and proc.poll() is None:
                    if time.monotonic() - gate_start >= args.gate_timeout:
                        status = "gate_timeout"
                        break
                    time.sleep(0.05)
                gate_wait_seconds = time.monotonic() - gate_start
                if status == "gate_timeout" and proc.poll() is None:
                    proc.kill()
                if proc.poll() is not None:
                    proc.wait()
            if proc.poll() is None:
                proc.stdin.write(stdin)
            proc.stdin.close()
            activity_start = time.monotonic()
            while proc.poll() is None:
                if time.monotonic() - activity_start >= args.timeout:
                    status = "timeout"
                    break
                if ((root / "stdout.bin").stat().st_size +
                        (root / "stderr.bin").stat().st_size > args.max_output_bytes):
                    status = "output_limit"
                    break
                time.sleep(0.01)
            if proc.poll() is None:
                proc.kill()
                proc.wait()
            code = proc.returncode
        except OSError as error:
            status = "launch_error"
            stderr.write(str(error).encode())
    record = {
        "executable": str(args.exe.resolve()),
        "executable_sha256": digest(args.exe),
        "fixture_source": str(args.fixture.resolve()),
        "reused_existing_private_fixture": args.reuse_existing_fixture,
        "input_copy": str(copy),
        "input_copy_sha256_before": before,
        "input_copy_sha256_after": digest(copy),
        "status": status,
        "exit_code": code,
        "elapsed_seconds": round(time.monotonic() - start, 3),
        "timeout_seconds": args.timeout,
        "pid": process_id,
        "ready_gate": str(ready_gate) if ready_gate else None,
        "gate_wait_seconds": round(gate_wait_seconds, 3),
        "stdin": stdin.decode(errors="replace"),
        "stdout_preview": (root / "stdout.bin").read_bytes()[:65536].decode(errors="replace"),
        "stderr_preview": (root / "stderr.bin").read_bytes()[:65536].decode(errors="replace"),
        "preview_limit_bytes": 65536,
        "max_output_bytes": args.max_output_bytes,
        "files": [
            {"name": p.name, "bytes": p.stat().st_size, "sha256": digest(p)}
            for p in root.iterdir() if p.is_file()
        ],
    }
    (root / "run.json").write_text(json.dumps(record, indent=2))
    print(json.dumps({key: record[key] for key in
                      ("status", "exit_code", "elapsed_seconds",
                       "input_copy_sha256_before", "input_copy_sha256_after", "files")}, indent=2))


if __name__ == "__main__":
    main()
