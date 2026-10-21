"""Capture bounded keyword imports through the original console application.

This supplies console input only. It does not call internal application methods.
Use --ready-gate with --pid-file to attach a passive observer before import.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from capture_oracle import digest


def transcript(names: list[str], export_name: str | None) -> bytes:
    lines = ["", "2"]
    for name in names:
        lines.extend(["1", name, ""])
    lines.append("E")
    if export_name:
        lines.extend(["8", export_name])
    lines.extend(["E", "Y"])
    return ("\n".join(lines) + "\n").encode("ascii")


def preview(path: Path) -> str:
    with path.open("rb") as stream:
        return stream.read(65536).decode(errors="replace")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--fixtures", type=Path, required=True)
    parser.add_argument("--keys", nargs="+", required=True,
                        help="ordered simple .KEY filenames; repetition is permitted")
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--export-name", help="optionally save final state through menu 8")
    parser.add_argument("--timeout", type=float, default=10)
    parser.add_argument("--max-output-bytes", type=int, default=2 * 1024 * 1024)
    parser.add_argument("--ready-gate", type=Path)
    parser.add_argument("--pid-file", type=Path)
    parser.add_argument("--gate-timeout", type=float, default=120)
    parser.add_argument("--prepare-only", action="store_true",
                        help="copy fixtures and save transcript without launching")
    parser.add_argument("--compact", action="store_true",
                        help="print short receipts; retain complete PID and run records")
    args = parser.parse_args()
    if not 0 < args.timeout <= 60 or not 0 < args.gate_timeout <= 120:
        parser.error("timeout must be in (0,60]; gate-timeout must be in (0,120]")
    if not 1 <= len(args.keys) <= 32:
        parser.error("provide between 1 and 32 imports")
    if not 0 < args.max_output_bytes <= 16 * 1024 * 1024:
        parser.error("max-output-bytes must be in (0,16 MiB]")
    names = list(dict.fromkeys(args.keys))
    for name in names + ([args.export_name] if args.export_name else []):
        if not re.fullmatch(r"[A-Za-z0-9_-]+[.][Kk][Ee][Yy]", name):
            parser.error("filenames must be simple ASCII .KEY names")
    if args.export_name and args.export_name.lower() in {n.lower() for n in names}:
        parser.error("export must not overwrite an imported fixture")
    if len({n.lower() for n in names}) != len(names):
        parser.error("fixture names must not differ only by case")
    if bool(args.ready_gate) != bool(args.pid_file):
        parser.error("ready-gate and pid-file must be supplied together")
    root = args.output.resolve()
    oracle = Path(__file__).resolve().parents[1] / "analysis" / "oracle"
    if root == oracle or not root.is_relative_to(oracle):
        parser.error("output must be a new private directory beneath analysis/oracle")
    if root.exists():
        parser.error("output directory must not already exist")
    exe = args.exe.resolve(strict=True)
    fixtures = args.fixtures.resolve(strict=True)
    sources = [fixtures / name for name in names]
    if any(not p.is_file() or p.stat().st_size > 65536 for p in sources):
        parser.error("every fixture must exist and be at most 64 KiB")
    if sum(p.stat().st_size for p in sources) > 1024 * 1024:
        parser.error("combined fixture size exceeds 1 MiB")
    ready = args.ready_gate.resolve() if args.ready_gate else None
    pid_file = args.pid_file.resolve() if args.pid_file else None
    for path in (ready, pid_file):
        if path and (path.exists() or not path.is_relative_to(oracle)):
            parser.error("gate and PID paths must be new paths under analysis/oracle")
    root.mkdir(parents=True)
    inputs = []
    for source in sources:
        copy = root / source.name
        shutil.copyfile(source, copy)
        inputs.append({"name": source.name, "source": str(source),
                       "copy": str(copy), "sha256_before": digest(copy)})
    stdin = transcript(args.keys, args.export_name)
    (root / "stdin.bin").write_bytes(stdin)
    record = {
        "executable": str(exe), "executable_sha256": digest(exe),
        "cwd": str(root), "ordered_imports": args.keys, "inputs": inputs,
        "stdin": stdin.decode("ascii"), "export_name": args.export_name,
        "timeout_seconds": args.timeout, "max_output_bytes": args.max_output_bytes,
        "ready_gate": str(ready) if ready else None, "pid": None,
        "gate_wait_seconds": 0, "status": "prepared", "exit_code": None,
        "limitations": [
            "A completed console transcript alone does not establish successful import.",
            "Inspect parser return values and state observations to classify each case.",
            "Output bounds are checked periodically; one polling interval can overshoot.",
        ],
    }
    start = time.monotonic()
    if not args.prepare_only:
        with (root / "stdout.bin").open("wb") as stdout, (root / "stderr.bin").open("wb") as stderr:
            proc = None
            try:
                proc = subprocess.Popen(
                    [str(exe)], cwd=root, stdin=subprocess.PIPE,
                    stdout=stdout, stderr=stderr,
                    creationflags=subprocess.CREATE_NO_WINDOW,
                )
                record["pid"] = proc.pid
                record["status"] = "completed"
                if ready:
                    pid_file.parent.mkdir(parents=True, exist_ok=True)
                    identity = {
                        "pid": proc.pid, "executable": str(exe),
                        "executable_sha256": record["executable_sha256"],
                        "cwd": str(root), "ready_gate": str(ready), "inputs": inputs,
                    }
                    pid_file.write_text(json.dumps(identity, indent=2), encoding="utf-8")
                    receipt = ({"pid": proc.pid, "pid_file": str(pid_file),
                                "ready_gate": str(ready), "imports": len(args.keys)}
                               if args.compact else identity)
                    print(json.dumps({"observer_waiting": receipt}), flush=True)
                    gate_start = time.monotonic()
                    while not ready.exists() and proc.poll() is None:
                        if time.monotonic() - gate_start >= args.gate_timeout:
                            record["status"] = "gate_timeout"
                            break
                        time.sleep(0.05)
                    record["gate_wait_seconds"] = round(time.monotonic() - gate_start, 3)
                if record["status"] == "completed" and proc.poll() is None:
                    proc.stdin.write(stdin)
                    proc.stdin.flush()
                proc.stdin.close()
                activity_start = time.monotonic()
                while record["status"] == "completed" and proc.poll() is None:
                    if time.monotonic() - activity_start >= args.timeout:
                        record["status"] = "timeout"
                        break
                    if sum((root / name).stat().st_size
                           for name in ("stdout.bin", "stderr.bin")) > args.max_output_bytes:
                        record["status"] = "output_limit"
                        break
                    time.sleep(0.01)
            except OSError as error:
                record["status"] = "process_error"
                stderr.write(str(error).encode())
            finally:
                if proc is not None:
                    if proc.poll() is None:
                        proc.kill()
                    record["exit_code"] = proc.wait()
        record["stdout_preview"] = preview(root / "stdout.bin")
        record["stderr_preview"] = preview(root / "stderr.bin")
    for item in inputs:
        copy = Path(item["copy"])
        item["sha256_after"] = digest(copy) if copy.exists() else None
        item["source_sha256_after"] = digest(Path(item["source"]))
    record["elapsed_seconds"] = round(time.monotonic() - start, 3)
    record["files"] = [
        {"name": path.name, "bytes": path.stat().st_size, "sha256": digest(path)}
        for path in sorted(root.iterdir()) if path.is_file()
    ]
    (root / "run.json").write_text(json.dumps(record, indent=2), encoding="utf-8")
    if args.compact:
        print(json.dumps({key: record[key] for key in (
            "status", "exit_code", "pid", "elapsed_seconds"
        )} | {"imports": len(args.keys), "run_record": str(root / "run.json")}))
    else:
        print(json.dumps({key: record[key] for key in (
            "status", "exit_code", "pid", "elapsed_seconds", "inputs", "files"
        )}, indent=2))


if __name__ == "__main__":
    main()
