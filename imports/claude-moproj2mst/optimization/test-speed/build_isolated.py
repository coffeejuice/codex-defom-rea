"""Build the frozen checkpoint-45 test-speed experiment without touching recovery builds."""

import argparse
import json
import os
import re
from pathlib import Path
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "optimization/test-speed/source/apps/moproj2mst"
BUILD = ROOT / "build/test-speed-candidate"
VCVARS = Path(r"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat")


def native_targets():
    """Allow build products only; never custom targets that execute corpus tests."""
    targets = {"m2m_core", "moproj2mst", "moproj2mst_cli", "m2m_sample_cpp",
               "m2m_sample_csharp"}
    for source in (SOURCE / "tests").glob("*/test_*.cpp"):
        targets.add(f"m2m_{source.parent.name}_{source.stem.removeprefix('test_')}")
    return targets


def validate_existing_cache():
    """Ninja still checks CMake inputs/globs when explicit configure is skipped."""
    cache = BUILD / "CMakeCache.txt"
    if not cache.is_file() or not (BUILD / "build.ninja").is_file():
        raise ValueError("--skip-configure requires an existing configured Ninja build")
    values = {}
    for line in cache.read_text(encoding="utf-8").splitlines():
        match = re.match(r"([^:#]+):[^=]+=(.*)", line)
        if match:
            values[match[1]] = match[2]
    expected = {"CMAKE_GENERATOR": "Ninja", "CMAKE_BUILD_TYPE": "Release"}
    for key, value in expected.items():
        if values.get(key) != value:
            raise ValueError(f"Existing cache {key} does not match {value}")
    for key, path in {"CMAKE_HOME_DIRECTORY": SOURCE,
                      "M2M_EXPERIMENT_TAKEOVER_ROOT": ROOT}.items():
        if not values.get(key) or Path(values[key]).resolve() != path.resolve():
            raise ValueError(f"Existing cache {key} does not match {path}")


def vc_environment():
    result = subprocess.run(
        f'cmd.exe /d /s /c ""{VCVARS}" >nul && set"',
        capture_output=True, text=True, check=True,
    )
    env = dict(os.environ)
    for line in result.stdout.splitlines():
        key, sep, value = line.partition("=")
        if key and sep:
            env[key] = value
    return env


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--label", required=True)
    parser.add_argument("--jobs", type=int, default=4)
    parser.add_argument("--target", action="append", default=[],
                        help="Build one native product (repeatable); default builds all")
    parser.add_argument("--skip-configure", action="store_true",
                        help="Reuse verified Release/Ninja cache; Ninja retains regeneration")
    args = parser.parse_args()
    if not args.label.replace("-", "").replace("_", "").isalnum():
        parser.error("Use a filename-safe label")
    unknown = sorted(set(args.target) - native_targets())
    if unknown:
        parser.error(f"Unknown or non-build target(s): {', '.join(unknown)}")
    if args.skip_configure:
        try:
            validate_existing_cache()
        except ValueError as error:
            parser.error(str(error))
    receipt = ROOT / "optimization/test-speed/analysis" / f"{args.label}-build.json"
    if receipt.exists():
        raise FileExistsError(receipt)
    commands = [
        ["cmake", "-S", str(SOURCE), "-B", str(BUILD), "-G", "Ninja",
         "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_TRY_COMPILE_CONFIGURATION=Release",
         f"-DM2M_EXPERIMENT_TAKEOVER_ROOT={ROOT}"],
        ["cmake", "--build", str(BUILD), "--parallel", str(max(1, args.jobs))],
    ]
    if args.target:
        commands[-1].extend(["--target", *dict.fromkeys(args.target)])
    if args.skip_configure:
        commands = commands[1:]
    env = vc_environment()
    steps = []
    for command in commands:
        started = time.perf_counter()
        run = subprocess.run(command, env=env, cwd=ROOT, capture_output=True, text=True)
        step = {"command": command, "seconds": round(time.perf_counter() - started, 3),
                "exit_code": run.returncode, "tail": (run.stdout + run.stderr)[-5000:]}
        steps.append(step)
        print(json.dumps({k: v for k, v in step.items() if k != "tail"}), flush=True)
        if run.returncode:
            break
    receipt.write_text(json.dumps({"source": str(SOURCE), "build": str(BUILD),
                                   "targets": list(dict.fromkeys(args.target)) or ["all"],
                                   "explicit_configure": not args.skip_configure,
                                   "steps": steps}, indent=2) + "\n", encoding="utf-8")
    return steps[-1]["exit_code"]


if __name__ == "__main__":
    raise SystemExit(main())
