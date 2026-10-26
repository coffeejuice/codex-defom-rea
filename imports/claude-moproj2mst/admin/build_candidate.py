"""Build a candidate without replacing the preserved baseline receipts/binaries."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time

import check_new_cpp_bindings

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/candidate"
VCVARS = Path(r"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat")


def source_fingerprint(source):
    fingerprint = hashlib.sha256()
    files = 0
    for path in sorted(source.rglob("*")):
        if path.is_file():
            fingerprint.update(path.relative_to(source).as_posix().encode())
            fingerprint.update(b"\0")
            fingerprint.update(hashlib.sha256(path.read_bytes()).digest())
            files += 1
    return fingerprint.hexdigest(), files


def file_sha256(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def _fingerprint_record(value):
    return {"sha256": value[0], "files": value[1]}


class _FingerprintTiming:
    """Observe existing fingerprint calls without caching or changing their result."""

    def __init__(self, provider):
        self.provider = provider
        self.samples = []

    def __call__(self, source):
        started = time.monotonic()
        result = self.provider(source)
        self.samples.append(time.monotonic() - started)
        return result

    def receipt(self):
        return {"completed_calls": len(self.samples),
                "elapsed_seconds": round(sum(self.samples), 6),
                "sample_seconds": [round(value, 6) for value in self.samples],
                "scope": "Existing source-fingerprint calls only; includes hash and file count"}


def build_grouped(source, build, logs, label, targets, env, max_reconfigure=2,
                  runner=None, fingerprint_fn=None, preflight_sources=None,
                  preflight_ctests=None):
    """Configure once, preflight grouped targets, then build all with drift retries."""
    runner = runner or subprocess.run
    fingerprint_fn = _FingerprintTiming(fingerprint_fn or source_fingerprint)
    started_at = time.time()
    started = time.monotonic()
    initial = fingerprint_fn(source)
    steps = []
    configure_count = 0
    reconfigure_count = 0
    drift_events = []
    artifacts = []
    selected = None
    final_source = initial
    final_cache = None
    result_code = 1
    status = "failed"
    source_preflight = None
    runtime_preflight = []
    cache_path = build / "CMakeCache.txt"
    configure_command = ["cmake", "-S", str(source / "apps/moproj2mst"), "-B", str(build),
                         "-G", "Ninja", "-DCMAKE_BUILD_TYPE=Release"]

    def run_step(name, phase, command, expected_source=None, expected_cache=None):
        log = logs / f"{label}-{name}-{len(steps) + 1:02d}.log"
        log.parent.mkdir(parents=True, exist_ok=True)
        before = fingerprint_fn(source)
        cache_before = file_sha256(cache_path) if cache_path.is_file() else None
        step_started = time.monotonic()
        with log.open("w", encoding="utf-8") as output:
            completed = runner(command, env=env, cwd=ROOT, stdout=output,
                               stderr=subprocess.STDOUT)
        command_elapsed = time.monotonic() - step_started
        after = fingerprint_fn(source)
        cache_after = file_sha256(cache_path) if cache_path.is_file() else None
        item = dict(step=name, phase=phase, command=command,
                    exit_code=completed.returncode,
                    elapsed_seconds=round(time.monotonic() - step_started, 3),
                    command_elapsed_seconds=round(command_elapsed, 6),
                    log=str(log), source_tree_before=_fingerprint_record(before),
                    source_tree_after=_fingerprint_record(after),
                    cache_sha256_before=cache_before, cache_sha256_after=cache_after)
        if expected_source is not None:
            item["expected_source_tree_sha256"] = expected_source[0]
        if expected_cache is not None:
            item["expected_cache_sha256"] = expected_cache
        steps.append(item)
        print(json.dumps(item), flush=True)
        return completed.returncode, before, after, cache_before, cache_after

    def retry_for_drift(reason, before=None, after=None, cache_before=None, cache_after=None):
        nonlocal reconfigure_count
        drift_events.append(dict(reason=reason,
                                 source_before=_fingerprint_record(before) if before else None,
                                 source_after=_fingerprint_record(after) if after else None,
                                 cache_before=cache_before, cache_after=cache_after,
                                 reconfigure_count=reconfigure_count))
        if reconfigure_count >= max_reconfigure:
            return False
        reconfigure_count += 1
        return True

    preflight_gate_failed = False
    if preflight_sources:
        preflight_started = time.monotonic()
        preflight_result = None
        try:
            preflight_result = check_new_cpp_bindings.check_files(
                source / "apps/moproj2mst", preflight_sources)
        except (OSError, UnicodeError, ValueError) as exc:
            preflight_result = {"ok": False,
                                "error": f"{type(exc).__name__}: {exc}"}
        preflight_after = fingerprint_fn(source)
        source_preflight = dict(
            files=list(preflight_sources),
            elapsed_seconds=round(time.monotonic() - preflight_started, 6),
            source_tree_before=_fingerprint_record(initial),
            source_tree_after=_fingerprint_record(preflight_after),
            result=preflight_result)
        final_source = preflight_after
        if preflight_after != initial:
            status = "source-preflight-source-drift"
            preflight_gate_failed = True
        elif not preflight_result.get("ok", False):
            status = "source-preflight-failed"
            preflight_gate_failed = True

    while not preflight_gate_failed:
        configure_count += 1
        configure_before = fingerprint_fn(source)
        code, configure_step_before, configure_after, _, _ = run_step(
            "configure", "configure", configure_command,
            expected_source=configure_before)
        final_source = configure_after
        if code != 0:
            status = "configure-failed"
            break
        if configure_before != configure_step_before or configure_step_before != configure_after:
            if retry_for_drift("source changed during configure", configure_step_before, configure_after):
                continue
            status = "source-drift-retry-limit"
            break

        selected = configure_after
        if not cache_path.is_file():
            status = "missing-cmake-cache"
            break
        selected_cache = file_sha256(cache_path)

        current = fingerprint_fn(source)
        current_cache = file_sha256(cache_path)
        if current != selected or current_cache != selected_cache:
            if retry_for_drift("source or CMake cache changed before focused build",
                               selected, current, selected_cache, current_cache):
                continue
            status = "preflight-boundary-retry-limit"
            break

        preflight_command = ["cmake", "--build", str(build), "--parallel", "4",
                             "--target", *targets]
        code, pre_before, pre_after, pre_cache_before, pre_cache_after = run_step(
            "focused-preflight", "focused-preflight", preflight_command,
            expected_source=selected, expected_cache=selected_cache)
        final_source = pre_after
        if code != 0:
            status = "focused-preflight-failed"
            break
        preflight_drift = (pre_before != selected or pre_after != selected or
                           pre_cache_before != selected_cache or pre_cache_after != selected_cache)
        if preflight_drift:
            if retry_for_drift("source or CMake cache changed during focused build",
                               pre_before, pre_after, pre_cache_before, pre_cache_after):
                continue
            status = "focused-preflight-drift-retry-limit"
            break

        runtime_failed = False
        for test_name in preflight_ctests or []:
            before_runtime = fingerprint_fn(source)
            cache_before_runtime = file_sha256(cache_path)
            if before_runtime != selected or cache_before_runtime != selected_cache:
                if retry_for_drift("source or CMake cache changed before focused runtime",
                                   selected, before_runtime, selected_cache,
                                   cache_before_runtime):
                    runtime_failed = True
                    break
                status = "focused-runtime-boundary-retry-limit"
                runtime_failed = True
                break
            expression = "^" + re.escape(test_name) + "$"
            ctest_command = ["ctest", "--test-dir", str(build), "--output-on-failure",
                             "-R", expression]
            step_name = f"focused-ctest-{len(runtime_preflight) + 1:02d}"
            code, runtime_before, runtime_after, runtime_cache_before, runtime_cache_after = run_step(
                step_name, "focused-runtime", ctest_command,
                expected_source=selected, expected_cache=selected_cache)
            record = dict(name=test_name, regex=expression,
                          step=steps[-1]["step"], exit_code=code,
                          log=steps[-1]["log"])
            runtime_preflight.append(record)
            final_source = runtime_after
            if code != 0:
                status = "focused-runtime-failed"
                runtime_failed = True
                break
            runtime_drift = (runtime_before != selected or runtime_after != selected or
                             runtime_cache_before != selected_cache or
                             runtime_cache_after != selected_cache)
            if runtime_drift:
                if retry_for_drift("source or CMake cache changed during focused runtime",
                                   runtime_before, runtime_after,
                                   runtime_cache_before, runtime_cache_after):
                    runtime_failed = True
                    break
                status = "focused-runtime-drift-retry-limit"
                runtime_failed = True
                break
        if runtime_failed:
            if status in ("focused-runtime-boundary-retry-limit",
                          "focused-runtime-drift-retry-limit",
                          "focused-runtime-failed"):
                break
            continue

        before_full = fingerprint_fn(source)
        cache_before_full = file_sha256(cache_path)
        if before_full != selected or cache_before_full != selected_cache:
            if retry_for_drift("source or CMake cache changed before full build",
                               selected, before_full, selected_cache, cache_before_full):
                continue
            status = "full-build-boundary-retry-limit"
            break

        full_command = ["cmake", "--build", str(build), "--parallel", "4"]
        code, full_before, full_after, full_cache_before, full_cache_after = run_step(
            "build", "full-build", full_command, expected_source=selected,
            expected_cache=selected_cache)
        final_source = full_after
        if code != 0:
            status = "full-build-failed"
            break
        full_drift = (full_before != selected or full_after != selected or
                      full_cache_before != selected_cache or full_cache_after != selected_cache)
        if full_drift:
            if retry_for_drift("source or CMake cache changed during full build",
                               full_before, full_after, full_cache_before, full_cache_after):
                continue
            status = "full-build-drift-retry-limit"
            break

        artifacts = []
        for path in sorted((build / "bin").glob("*.exe")) + sorted((build / "bin").glob("*.dll")):
            artifacts.append(dict(path=str(path), bytes=path.stat().st_size,
                                  sha256=file_sha256(path)))
        final_source = fingerprint_fn(source)
        final_cache = file_sha256(cache_path)
        if final_source != selected or final_cache != selected_cache:
            if retry_for_drift("source or CMake cache changed during artifact hashing",
                               selected, final_source, selected_cache, final_cache):
                artifacts = []
                continue
            status = "artifact-binding-drift-retry-limit"
            artifacts = []
            break

        result_code = 0
        status = "completed"
        break

    if selected is not None and status != "completed":
        final_source = fingerprint_fn(source)
        final_cache = file_sha256(cache_path) if cache_path.is_file() else None
    stable = bool(result_code == 0 and selected == final_source and final_cache ==
                  (steps[-1].get("expected_cache_sha256") if steps else None))
    if not stable:
        result_code = 1
        if status == "completed":
            status = "final-source-or-cache-drift"
    receipt = dict(schema="m2m-candidate-build-v1", started_unix=started_at,
                   source_tree_sha256=(selected or initial)[0],
                   source_file_count=(selected or initial)[1], steps=steps,
                   targets=None, preflight_targets=list(targets), artifacts=artifacts,
                   configure_reuse=dict(enabled=True, build_dir=str(build),
                                        cmake_cache=str(cache_path),
                                        cmake_cache_sha256=final_cache,
                                        configure_count=configure_count,
                                        reconfigure_count=reconfigure_count,
                                        max_reconfigurations=max_reconfigure,
                                        drift_events=drift_events),
                   elapsed_seconds=round(time.time() - started_at, 3),
                   source_tree_after_sha256=final_source[0],
                   source_file_count_after=final_source[1],
                   source_stable_during_build=stable, status=status,
                   source_fingerprint_timing=fingerprint_fn.receipt())
    if source_preflight is not None:
        receipt["source_preflight"] = source_preflight
    if preflight_ctests:
        receipt["focused_runtime_preflight"] = dict(tests=list(preflight_ctests),
            elapsed_seconds=round(sum(step["command_elapsed_seconds"] for step in steps
                                      if step["phase"] == "focused-runtime"), 6),
            results=runtime_preflight)
    return receipt, result_code


def main(argv=None):
    parser = argparse.ArgumentParser()
    parser.add_argument("--label", required=True)
    parser.add_argument("--target", action="append")
    parser.add_argument("--preflight-target", action="append", default=[])
    parser.add_argument("--then-full", action="store_true",
                        help="configure once, build all preflight targets, then build the full project")
    parser.add_argument("--max-reconfigure", type=int, default=2,
                        help="maximum configure retries after source/cache drift in grouped mode")
    parser.add_argument("--preflight-source", action="append", default=[], metavar="FILE",
                        help="explicit C++ file to check with the read-only binding preflight; may repeat")
    parser.add_argument("--preflight-ctest", action="append", default=[], metavar="NAME",
                        help="run this exact CTest name after focused compilation; may repeat")
    args = parser.parse_args(argv)
    if not args.label or any(c not in "abcdefghijklmnopqrstuvwxyz0123456789-_" for c in args.label):
        parser.error("Use a lowercase filename-safe label")
    if args.then_full and (args.target or not args.preflight_target):
        parser.error("--then-full requires --preflight-target and cannot be combined with --target")
    if args.preflight_target and not args.then_full:
        parser.error("--preflight-target requires --then-full")
    if args.preflight_source and not args.then_full:
        parser.error("--preflight-source requires --then-full")
    if args.preflight_ctest and not args.then_full:
        parser.error("--preflight-ctest requires --then-full")
    if any(not name or "\n" in name or "\r" in name for name in args.preflight_ctest):
        parser.error("--preflight-ctest requires a non-empty single-line CTest name")
    if args.max_reconfigure < 0:
        parser.error("--max-reconfigure must be zero or greater")
    if not args.then_full and args.max_reconfigure != 2:
        parser.error("--max-reconfigure is only available with --then-full")
    receipt_path = ROOT / "analysis" / (args.label + "-build.json")
    if receipt_path.exists():
        raise FileExistsError(receipt_path)
    logs = ROOT / "build/logs"
    logs.mkdir(parents=True, exist_ok=True)
    source = ROOT / "source"
    start = time.time()
    fingerprint_fn = _FingerprintTiming(source_fingerprint)
    if not args.then_full:
        fingerprint, files = fingerprint_fn(source)
    result = subprocess.run(f'cmd.exe /d /s /c ""{VCVARS}" >nul && set"',
                            capture_output=True, text=True, check=True)
    env = dict(os.environ)
    for line in result.stdout.splitlines():
        key, sep, value = line.partition("=")
        if key and sep:
            env[key] = value
    if args.then_full:
        receipt, return_code = build_grouped(
            source, BUILD, logs, args.label, args.preflight_target, env,
            max_reconfigure=args.max_reconfigure,
            preflight_sources=args.preflight_source or None,
            preflight_ctests=args.preflight_ctest or None)
        receipt_path.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
        summary = {"receipt": str(receipt_path),
                   "source_tree_sha256": receipt["source_tree_sha256"],
                   "preflight_targets": receipt["preflight_targets"],
                   "status": receipt["status"],
                   "source_stable_during_build": receipt["source_stable_during_build"]}
        if "source_preflight" in receipt:
            summary["source_preflight"] = receipt["source_preflight"]
        if "focused_runtime_preflight" in receipt:
            summary["focused_runtime_preflight"] = receipt["focused_runtime_preflight"]
        print(json.dumps(summary, indent=2))
        return return_code
    receipt = dict(schema="m2m-candidate-build-v1", started_unix=start, source_tree_sha256=fingerprint,
                   source_file_count=files, steps=[], targets=args.target, artifacts=[])
    commands = [
        ("configure", ["cmake", "-S", str(source / "apps/moproj2mst"), "-B", str(BUILD),
                       "-G", "Ninja", "-DCMAKE_BUILD_TYPE=Release"]),
        ("build", ["cmake", "--build", str(BUILD), "--parallel", "4"] +
         (["--target", *args.target] if args.target else [])),
    ]
    for step, command in commands:
        log = logs / f"{args.label}-{step}.log"
        started = time.monotonic()
        with log.open("w", encoding="utf-8") as f:
            run = subprocess.run(command, env=env, cwd=ROOT, stdout=f, stderr=subprocess.STDOUT)
        command_elapsed = time.monotonic() - started
        item = dict(step=step, command=command, exit_code=run.returncode,
                    elapsed_seconds=round(time.monotonic()-started, 3),
                    command_elapsed_seconds=round(command_elapsed, 6), log=str(log))
        receipt["steps"].append(item)
        print(json.dumps(item), flush=True)
        if run.returncode:
            break
    for path in sorted((BUILD / "bin").glob("*.exe")) + sorted((BUILD / "bin").glob("*.dll")):
        receipt["artifacts"].append(dict(path=str(path), bytes=path.stat().st_size,
                                         sha256=hashlib.sha256(path.read_bytes()).hexdigest()))
    receipt["elapsed_seconds"] = round(time.time()-start, 3)
    after, after_count = fingerprint_fn(source)
    receipt["source_tree_after_sha256"] = after
    receipt["source_file_count_after"] = after_count
    receipt["source_stable_during_build"] = (fingerprint, files) == (after, after_count)
    receipt["source_fingerprint_timing"] = fingerprint_fn.receipt()
    receipt_path.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    return receipt["steps"][-1]["exit_code"] or (0 if receipt["source_stable_during_build"] else 1)


if __name__ == "__main__":
    raise SystemExit(main())
