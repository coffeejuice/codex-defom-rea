"""Build the imported converter in isolation; never execute tests or originals."""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "baseline"
LOGS = ROOT / "build" / "logs"
VCVARS = Path(r"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat")


def main():
    started = time.time()
    LOGS.mkdir(parents=True, exist_ok=True)
    env_result = subprocess.run(
        f'cmd.exe /d /s /c ""{VCVARS}" >nul && set"',
        capture_output=True, text=True, check=True,
    )
    env = dict(os.environ)
    for line in env_result.stdout.splitlines():
        key, sep, value = line.partition("=")
        if sep and key:
            env[key] = value
    commands = [
        ("configure", ["cmake", "-S", str(ROOT / "source/apps/moproj2mst"),
                       "-B", str(BUILD), "-G", "Ninja", "-DCMAKE_BUILD_TYPE=Release",
                       f"-DM2M_CORPUS={(ROOT / 'evidence/gui_mo/s1/corpus').as_posix()}"]),
        ("build", ["cmake", "--build", str(BUILD), "--parallel", "4"]),
    ]
    receipt = {"source_baseline": "bb869387a85a89193f3fdd78b69442a6ea567866",
               "started_unix": started, "build_directory": str(BUILD),
               "tests_executed": False, "steps": []}
    for name, args in commands:
        log_path = LOGS / f"baseline-{name}.log"
        step_start = time.monotonic()
        with log_path.open("w", encoding="utf-8") as log:
            result = subprocess.run(args, env=env, cwd=ROOT, stdout=log, stderr=subprocess.STDOUT)
        step = {"step": name, "command": args, "exit_code": result.returncode,
                "elapsed_seconds": round(time.monotonic() - step_start, 3), "log": str(log_path)}
        receipt["steps"].append(step)
        print(json.dumps(step), flush=True)
        if result.returncode:
            break
    receipt["elapsed_seconds"] = round(time.time() - started, 3)
    receipt["artifacts"] = []
    for name in ("moproj2mst.exe", "moproj2mst.dll"):
        path = BUILD / "bin" / name
        if path.exists():
            receipt["artifacts"].append({"path": str(path), "bytes": path.stat().st_size,
                                         "sha256": hashlib.sha256(path.read_bytes()).hexdigest()})
    (ROOT / "analysis/baseline-build.json").write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    return receipt["steps"][-1]["exit_code"]


if __name__ == "__main__":
    raise SystemExit(main())
