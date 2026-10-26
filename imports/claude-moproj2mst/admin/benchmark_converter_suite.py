"""Time repeated converter test-suite runs against an existing build.

This driver does not configure or build. It invokes run_converter_tests.py and
retains its own process timing plus a copy of each runner receipt.
"""

import argparse
from datetime import datetime, timezone
import json
import math
import os
from pathlib import Path
import shutil
import statistics
import subprocess
import sys
import time


ROOT = Path(__file__).resolve().parents[1]
BUILDS = ROOT / "build"
RUNNER = ROOT / "admin" / "run_converter_tests.py"
SHARED_RECEIPT = ROOT / "analysis" / "test-relocation-results.json"
DEFAULT_RESULTS = ROOT / "optimization" / "test-speed" / "analysis" / "benchmark-runs"
COMPARATOR = ROOT / "admin" / "compare_test_speed_runs.py"
CHECKPOINT45_RUN = BUILDS / "test-runs" / "1792854086489696400"
CHECKPOINT45_RECEIPT = ROOT / "analysis" / "wave3-candidate45-doe-fix-d-tests.json"


def write_json(path, value):
    path.write_text(json.dumps(value, indent=2) + "\n", encoding="utf-8")


def percentile_nearest_rank(values, percentile):
    ordered = sorted(values)
    return ordered[max(0, math.ceil(percentile * len(ordered)) - 1)]


def stats(values):
    if not values:
        return None
    return {
        "count": len(values),
        "median_seconds": round(statistics.median(values), 6),
        "p95_seconds": round(percentile_nearest_rank(values, 0.95), 6),
        "min_seconds": round(min(values), 6),
        "max_seconds": round(max(values), 6),
    }


def load_json(path):
    with path.open(encoding="utf-8") as stream:
        return json.load(stream)


def matching_receipt(summary, build, previous_mtime):
    """Read only a freshly written receipt for the run named by runner stdout."""
    run = summary.get("run") if isinstance(summary, dict) else None
    if not run:
        return None, "Runner did not report a run directory or receipt is missing"
    try:
        run_path = Path(run).resolve(strict=True)
        if not run_path.is_relative_to(BUILDS / "test-runs"):
            return None, "Runner reported a run directory outside build/test-runs"
        bound = run_path / "receipt.json"
        if bound.is_file():
            receipt = load_json(bound)
        elif SHARED_RECEIPT.is_file():
            if previous_mtime is not None and SHARED_RECEIPT.stat().st_mtime_ns == previous_mtime:
                return None, "Shared receipt was not updated by this invocation"
            receipt = load_json(SHARED_RECEIPT)
        else:
            return None, "Runner receipt is missing"
        if Path(receipt["run"]).resolve() != run_path:
            return None, "Receipt belongs to another run (possible concurrent runner)"
        if Path(receipt["build"]).resolve() != build:
            return None, "Shared receipt build differs from requested build"
        return receipt, None
    except (OSError, ValueError, KeyError, json.JSONDecodeError) as exc:
        return None, f"Could not validate runner receipt: {exc}"


def run_once(index, build, jobs, output, config=None):
    folder = output / f"run-{index:03d}"
    folder.mkdir()
    command = [sys.executable, str(RUNNER), "--build", str(build), "--jobs", str(jobs)]
    env = None
    if config is not None:
        command += ["--hash-engine", config["hash_engine"], "--hash-workers", str(config["hash_workers"])]
        env = dict(os.environ)
        env["M2M_FOLDER_WORKERS"] = str(config["folder_workers"])
        # Profiling changes diagnostic output and adds overhead; matrix runs
        # retain ordinary semantic output. Record this explicitly below.
        env.pop("M2M_FOLDER_PROFILE", None)
    previous_mtime = SHARED_RECEIPT.stat().st_mtime_ns if SHARED_RECEIPT.exists() else None
    wall_start = datetime.now(timezone.utc).isoformat()
    start = time.perf_counter()
    with (folder / "stdout.log").open("w", encoding="utf-8") as stdout, \
            (folder / "stderr.log").open("w", encoding="utf-8") as stderr:
        process = subprocess.run(command, cwd=ROOT, stdout=stdout, stderr=stderr, env=env,
                                 check=False)
    wall_seconds = round(time.perf_counter() - start, 6)
    record = {
        "index": index, "command": command, "started_utc": wall_start,
        "external_wall_seconds": wall_seconds, "exit_code": process.returncode,
        "stdout": str(folder / "stdout.log"), "stderr": str(folder / "stderr.log"),
        "configuration": config,
    }
    try:
        summary = load_json(folder / "stdout.log")
    except (OSError, ValueError, json.JSONDecodeError):
        summary = None
        record["receipt_error"] = "Runner stdout is not a complete JSON summary; see logs"
    receipt, issue = matching_receipt(summary, build, previous_mtime)
    if receipt is not None:
        copied = folder / "runner-receipt.json"
        write_json(copied, receipt)
        record["runner_receipt"] = str(copied)
        record["runner_run"] = receipt["run"]
        record["runner_status"] = receipt.get("status")
        record["ctest_seconds"] = receipt.get("elapsed_seconds")
        record["runner_timing"] = receipt.get("timing")
        record["artifacts"] = {
            "inventory": receipt.get("inventory"),
            "console": str(Path(receipt["run"]) / "console.log"),
            "ctest_log": str(Path(receipt["run"]) / "ctest.log"),
            "junit": str(Path(receipt["run"]) / "junit.xml"),
        }
        timing = Path(receipt["run"]) / "timing.json"
        if timing.is_file():
            copied_timing = folder / "runner-timing.json"
            shutil.copy2(timing, copied_timing)
            record["runner_timing_file"] = str(copied_timing)
        record["valid"] = (process.returncode == 0 and receipt.get("exit_code") == 0
                           and receipt.get("status") in ("completed", "completed-with-recorded-gaps")
                           and not receipt.get("fixture_changes")
                           and not receipt.get("unreported_tests")
                           and not receipt.get("incomplete_output_tests"))
        if config is not None:
            expected_hash = {"engine": config["hash_engine"], "workers": config["hash_workers"],
                             "algorithm": "sha256", "cache": False}
            if receipt.get("fixture_hash") != expected_hash:
                record["valid"] = False
                record["receipt_error"] = "Runner fixture hash settings differ from requested configuration"
    else:
        record["valid"] = False
        record["receipt_error"] = issue
    write_json(folder / "benchmark-record.json", record)
    return record


def check_equivalence(record, output, reference_run, reference_receipt, baseline_source, candidate_source):
    """Retain comparator output and exit status independently of runner timing."""
    folder = output / f"run-{record['index']:03d}"
    if not record.get("runner_receipt"):
        record["equivalent"] = False
        record["equivalence_error"] = "No validated run receipt to compare"
        return
    command = [sys.executable, str(COMPARATOR), str(reference_run), str(reference_receipt),
               record["runner_run"], record["runner_receipt"],
               "--baseline-source", str(baseline_source), "--candidate-source", str(candidate_source)]
    with (folder / "equivalence.json").open("w", encoding="utf-8") as stdout, \
            (folder / "equivalence-stderr.log").open("w", encoding="utf-8") as stderr:
        result = subprocess.run(command, cwd=ROOT, stdout=stdout, stderr=stderr, check=False)
    record["equivalence_command"] = command
    record["equivalence_report"] = str(folder / "equivalence.json")
    record["equivalence_exit_code"] = result.returncode
    try:
        report = load_json(folder / "equivalence.json")
        record["equivalent"] = result.returncode == 0 and report.get("equivalent") is True
    except (OSError, ValueError):
        record["equivalent"] = False
        record["equivalence_error"] = "Comparator did not produce valid JSON"
    write_json(folder / "benchmark-record.json", record)


def matrix_configurations(hash_counts, folder_counts):
    control = {"hash_engine": "serial", "hash_workers": 1, "folder_workers": 1}
    configurations = []
    for folders in folder_counts:
        configurations.append({"hash_engine": "serial", "hash_workers": 1, "folder_workers": folders})
        configurations.extend({"hash_engine": "threaded", "hash_workers": workers, "folder_workers": folders}
                              for workers in hash_counts)
    return control, [config for config in configurations if config != control]


def matrix_summary(records):
    """Timing success and semantic acceptance are separate reported results."""
    groups = {}
    for record in records:
        config = record["configuration"]
        key = f"{config['hash_engine']}-h{config['hash_workers']}-f{config['folder_workers']}"
        groups.setdefault(key, []).append(record)
    result = {}
    for key, rows in groups.items():
        valid = [row for row in rows if row.get("valid")]
        accepted = [row for row in valid if row.get("equivalent")]
        result[key] = {
            "configuration": rows[0]["configuration"], "runs": len(rows),
            "timing_valid_runs": len(valid), "equivalent_runs": len(accepted),
            "all_evidence_equivalent": len(accepted) == len(rows),
            "observed_external_wall": stats([row["external_wall_seconds"] for row in valid]),
            "equivalent_external_wall": stats([row["external_wall_seconds"] for row in accepted]),
            "equivalent_ctest": stats([float(row["ctest_seconds"]) for row in accepted
                                      if isinstance(row.get("ctest_seconds"), (int, float))]),
        }
    return result


def run_matrix(args, build, output, control, configurations):
    result = {
        "schema": 2, "mode": "paired-matrix", "build": str(build), "jobs": args.jobs,
        "cache_state": args.cache_state, "cache_note": "Operator label only; no cache flushing",
        "notes": args.note, "runs_per_configuration": args.runs,
        "reference_run": str(args.reference_run.resolve()),
        "reference_receipt": str(args.reference_receipt.resolve()),
        "baseline_source": str(args.baseline_source.resolve()),
        "candidate_source": str(args.candidate_source.resolve()),
        "control": control, "treatments": configurations,
        "planned_suite_invocations": 2 * args.runs * len(configurations),
        "profile_environment": "M2M_FOLDER_PROFILE removed for every matrix invocation",
        "order": "Each treatment paired with serial/folder=1; order alternates by pair",
        "started_utc": datetime.now(timezone.utc).isoformat(), "records": [], "pairs": [],
    }
    summary_path = output / "summary.json"
    write_json(summary_path, result)
    stopped = False
    for repetition in range(1, args.runs + 1):
        for config in configurations:
            pair_id = len(result["pairs"]) + 1
            ordered = [("control", control), ("treatment", config)]
            if pair_id % 2 == 0:
                ordered.reverse()
            pair = {"pair": pair_id, "repetition": repetition, "treatment": config, "records": []}
            for role, current in ordered:
                record = run_once(len(result["records"]) + 1, build, args.jobs, output, current)
                record.update(pair=pair_id, repetition=repetition, role=role)
                check_equivalence(record, output, args.reference_run.resolve(), args.reference_receipt.resolve(),
                                  args.baseline_source.resolve(), args.candidate_source.resolve())
                write_json(output / f"run-{record['index']:03d}" / "benchmark-record.json", record)
                result["records"].append(record)
                pair["records"].append(record["index"])
                result["configurations"] = matrix_summary(result["records"])
                write_json(summary_path, result)
                print(json.dumps({"run": record["index"], "pair": pair_id, "role": role,
                                  "configuration": current, "valid": record["valid"],
                                  "equivalent": record.get("equivalent"), "summary": str(summary_path)}), flush=True)
                if not record["valid"] or not record.get("equivalent"):
                    stopped = True
                    break
            pair["accepted"] = len(pair["records"]) == 2 and not stopped
            if pair["accepted"]:
                rows = result["records"][-2:]
                old = next(row for row in rows if row["role"] == "control")
                new = next(row for row in rows if row["role"] == "treatment")
                pair["external_wall_change_seconds"] = round(new["external_wall_seconds"] - old["external_wall_seconds"], 6)
                pair["external_wall_change_percent"] = round((new["external_wall_seconds"] / old["external_wall_seconds"] - 1) * 100, 3)
            result["pairs"].append(pair)
            write_json(summary_path, result)
            if stopped:
                break
        if stopped:
            break
    result["completed_utc"] = datetime.now(timezone.utc).isoformat()
    result["stopped_on_failure"] = stopped
    result["completed_suite_invocations"] = len(result["records"])
    result["all_evidence_equivalent"] = (not stopped and len(result["records"]) == result["planned_suite_invocations"])
    result["timing_claim"] = "Measured host/cache observations only; equivalence comes from retained comparator receipts"
    write_json(summary_path, result)
    print(json.dumps({"summary": str(summary_path), "all_evidence_equivalent": result["all_evidence_equivalent"],
                      "completed_suite_invocations": len(result["records"])}, indent=2))
    return 1 if stopped else 0


def compare(current, baseline):
    other = load_json(baseline)
    warnings = []
    for field in ("jobs", "cache_state"):
        if current.get(field) != other.get(field):
            warnings.append(f"{field} differs from baseline")
    if current.get("build") == other.get("build"):
        warnings.append("Same build path; compare intended build contents separately")
    result = {"baseline": str(baseline), "warnings": warnings}
    for metric in ("external_wall", "ctest"):
        here = (current.get("statistics") or {}).get(metric)
        there = (other.get("statistics") or {}).get(metric)
        if here and there and not current.get("invalid_runs") and not other.get("invalid_runs"):
            a, b = here["median_seconds"], there["median_seconds"]
            result[metric] = {
                "baseline_median_seconds": b,
                "current_median_seconds": a,
                "median_change_seconds": round(a - b, 6),
                "median_change_percent": round((a / b - 1) * 100, 3) if b else None,
            }
        else:
            warnings.append(f"{metric} comparison unavailable: missing samples or invalid runs")
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", type=Path, required=True, help="existing build beneath build/")
    parser.add_argument("--runs", type=int, default=5)
    parser.add_argument("--jobs", type=int, default=4)
    parser.add_argument("--cache-state", choices=("warm", "cold", "unspecified"),
                        default="unspecified", help="operator observation; no cache clearing is performed")
    parser.add_argument("--note", action="append", default=[], help="free-form run condition note")
    parser.add_argument("--baseline", type=Path, help="earlier benchmark summary JSON")
    parser.add_argument("--output", type=Path, help="new directory for all results")
    parser.add_argument("--matrix", action="store_true", help="paired full-suite hash/folder worker experiment")
    parser.add_argument("--hash-worker-counts", type=int, nargs="+", default=[1, 2, 4, 8, 10])
    parser.add_argument("--folder-worker-counts", type=int, nargs="+", choices=[1, 2, 4, 8, 10], default=[1],
                        help="opt in to parallel folder workers; default: 1")
    parser.add_argument("--reference-run", type=Path, default=CHECKPOINT45_RUN)
    parser.add_argument("--reference-receipt", type=Path, default=CHECKPOINT45_RECEIPT)
    parser.add_argument("--baseline-source", type=Path, default=ROOT / "source")
    parser.add_argument("--candidate-source", type=Path, default=ROOT / "optimization/test-speed/source")
    parser.add_argument("--plan-only", action="store_true", help="print matrix plan without running/writing anything")
    args = parser.parse_args()
    build = args.build.resolve()
    if not build.is_dir() or not build.is_relative_to(BUILDS.resolve()) or build == BUILDS.resolve():
        parser.error("--build must name an existing directory beneath build/")
    if args.runs < 1 or args.jobs < 1:
        parser.error("--runs and --jobs must be positive")
    if args.plan_only and not args.matrix:
        parser.error("--plan-only requires --matrix")
    if args.matrix:
        if args.baseline:
            parser.error("--baseline is for ordinary timing summaries; matrix uses --reference-run/--reference-receipt")
        if any(not 1 <= count <= 32 for count in args.hash_worker_counts):
            parser.error("--hash-worker-counts must be from 1 through 32")
        args.hash_worker_counts = list(dict.fromkeys(args.hash_worker_counts))
        args.folder_worker_counts = list(dict.fromkeys(args.folder_worker_counts))
        for path in (args.reference_run, args.baseline_source, args.candidate_source):
            if not path.is_dir():
                parser.error(f"Required matrix directory missing: {path}")
        if not args.reference_receipt.is_file():
            parser.error("--reference-receipt must be an existing full-run receipt")
        control, configurations = matrix_configurations(args.hash_worker_counts, args.folder_worker_counts)
        if args.plan_only:
            print(json.dumps({"control": control, "treatments": configurations,
                              "repetitions": args.runs, "suite_invocations": 2 * args.runs * len(configurations),
                              "build": str(build), "jobs": args.jobs,
                              "reference_run": str(args.reference_run.resolve()),
                              "reference_receipt": str(args.reference_receipt.resolve()),
                              "executes_tests": False}, indent=2))
            return 0
    output = (args.output or DEFAULT_RESULTS / datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%S%fZ")).resolve()
    if not output.is_relative_to(ROOT):
        parser.error("--output must remain beneath this project")
    if args.baseline and not args.baseline.is_file():
        parser.error("--baseline must be an existing summary JSON file")
    output.mkdir(parents=True, exist_ok=False)
    if args.matrix:
        return run_matrix(args, build, output, control, configurations)
    result = {
        "schema": 1, "build": str(build), "jobs": args.jobs, "runs_requested": args.runs,
        "cache_state": args.cache_state, "cache_note": "Operator label only; harness does not flush caches",
        "notes": args.note, "started_utc": datetime.now(timezone.utc).isoformat(),
        "records": [],
    }
    summary_path = output / "summary.json"
    for index in range(1, args.runs + 1):
        result["records"].append(run_once(index, build, args.jobs, output))
        write_json(summary_path, result)
    records = result["records"]
    result["invalid_runs"] = [r["index"] for r in records if not r["valid"]]
    good = [r for r in records if r["valid"]]
    result["statistics"] = {
        "external_wall": stats([r["external_wall_seconds"] for r in good]),
        "ctest": stats([float(r["ctest_seconds"]) for r in good if isinstance(r.get("ctest_seconds"), (int, float))]),
    }
    result["completed_utc"] = datetime.now(timezone.utc).isoformat()
    if args.baseline:
        result["comparison"] = compare(result, args.baseline.resolve())
    write_json(summary_path, result)
    print(json.dumps({"summary": str(summary_path), "invalid_runs": result["invalid_runs"],
                      "statistics": result["statistics"], "comparison": result.get("comparison")}, indent=2))
    return 0 if not result["invalid_runs"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
