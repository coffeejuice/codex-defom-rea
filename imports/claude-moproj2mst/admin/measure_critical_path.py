"""Summarize frozen build/test timings and optional worker-overlap events.

This tool reads receipts only. It never invokes builds, tests or source hashing.
Worker overlap can be computed only from an event file recorded on one clock.
"""

import argparse
import json
import math
from pathlib import Path
import sys


EVENT_SCHEMA = "m2m-worker-events-v1"


def read_json(path):
    return json.loads(Path(path).read_text(encoding="utf-8"))


def finite_number(value, label):
    if isinstance(value, bool) or not isinstance(value, (int, float)) or not math.isfinite(value):
        raise ValueError(f"{label} must be a finite number")
    return float(value)


def build_summary(receipt):
    steps = receipt.get("steps")
    if not isinstance(steps, list):
        raise ValueError("Build receipt has no steps list")
    rows = []
    for step in steps:
        rows.append({
            "step": step.get("step", step.get("phase", "unknown")),
            "seconds": finite_number(step.get("elapsed_seconds"), "build step elapsed_seconds"),
            "exit_code": step.get("exit_code"),
        })
    return {
        "status": receipt.get("status", "unknown"),
        "source_tree_sha256": receipt.get("source_tree_sha256"),
        "steps": rows,
        "recorded_elapsed_seconds": receipt.get("elapsed_seconds"),
    }


def test_summary(receipt, timing=None):
    counts = receipt.get("counts")
    if not isinstance(counts, dict):
        raise ValueError("Test receipt has no counts object")
    elapsed = finite_number(receipt.get("elapsed_seconds"), "test elapsed_seconds")
    binding = receipt.get("execution_binding") or {}
    before = binding.get("source_tree_before", {}).get("sha256")
    after = binding.get("source_tree_after", {}).get("sha256")
    stable = binding.get("stable_during_test")
    if stable is None and before is not None and after is not None:
        stable = before == after
    result = {
        "registered_tests": receipt.get("registered_tests"),
        "elapsed_seconds": elapsed,
        "counts": counts,
        "token_measurement": receipt.get("token_measurement", "unavailable"),
        "source_tree_before_sha256": before,
        "source_tree_after_sha256": after,
        "binary_binding_stable": stable,
        "fixture_changes": receipt.get("fixture_changes"),
        "fixture_hash_settings": receipt.get("fixture_hash"),
    }
    if timing is not None:
        phases = timing.get("phase_seconds")
        if not isinstance(phases, dict):
            raise ValueError("Timing receipt has no phase_seconds object")
        result["launcher_elapsed_seconds"] = finite_number(
            timing.get("launcher_elapsed_seconds"), "launcher_elapsed_seconds")
        result["launcher_phases_seconds"] = {
            key: finite_number(value, f"phase {key}") for key, value in phases.items()
        }
    return result


def validate_event_file(document, checkpoint=None, source_sha256=None):
    if not isinstance(document, dict) or document.get("schema") != EVENT_SCHEMA:
        raise ValueError(f"Worker event file schema must be {EVENT_SCHEMA}")
    if document.get("clock_domain") != "host_monotonic_seconds":
        raise ValueError("Worker events must use one host_monotonic_seconds clock domain")
    if not isinstance(document.get("checkpoint"), str) or not document["checkpoint"]:
        raise ValueError("Worker event file requires a checkpoint label")
    if checkpoint is not None and document["checkpoint"] != checkpoint:
        raise ValueError("Worker event checkpoint does not match requested checkpoint")
    event_source = document.get("source_tree_sha256")
    if not isinstance(event_source, str) or len(event_source) != 64:
        raise ValueError("Worker event file requires a 64-character source_tree_sha256")
    if source_sha256 is not None and event_source != source_sha256:
        raise ValueError("Worker event source hash does not match the test receipt")
    raw = document.get("events")
    if not isinstance(raw, list) or not raw:
        raise ValueError("Worker event file requires a non-empty events list")
    events, by_id = [], {}
    for item in raw:
        if not isinstance(item, dict):
            raise ValueError("Each worker event must be an object")
        event_id = item.get("id")
        if not isinstance(event_id, str) or not event_id or event_id in by_id:
            raise ValueError("Worker event ids must be unique non-empty strings")
        start = finite_number(item.get("start_seconds"), f"{event_id}.start_seconds")
        end = finite_number(item.get("end_seconds"), f"{event_id}.end_seconds")
        if end <= start:
            raise ValueError(f"Worker event {event_id} must have positive duration")
        dependencies = item.get("depends_on", [])
        if not isinstance(dependencies, list) or any(not isinstance(dep, str) for dep in dependencies):
            raise ValueError(f"Worker event {event_id}.depends_on must be a list of ids")
        if len(dependencies) != len(set(dependencies)) or event_id in dependencies:
            raise ValueError(f"Worker event {event_id} has duplicate or self dependency")
        event = {"id": event_id, "role": item.get("role", "worker"), "start": start,
                 "end": end, "duration": end - start, "depends_on": dependencies}
        events.append(event)
        by_id[event_id] = event
    for event in events:
        for dep_id in event["depends_on"]:
            if dep_id not in by_id:
                raise ValueError(f"Worker event {event['id']} depends on missing {dep_id}")
            if by_id[dep_id]["end"] > event["start"]:
                raise ValueError(f"Dependency {dep_id} ends after {event['id']} starts")

    # Dependencies are strictly time-ordered above, so this is an acyclic DAG.
    ordered = sorted(events, key=lambda row: (row["start"], row["end"], row["id"]))
    longest = {}
    for event in ordered:
        if not event["depends_on"]:
            longest[event["id"]] = {"start": event["start"], "end": event["end"],
                                     "active": event["duration"], "path": [event["id"]]}
            continue
        candidates = []
        for dep_id in event["depends_on"]:
            prior = longest[dep_id]
            candidates.append({"start": prior["start"], "end": event["end"],
                               "active": prior["active"] + event["duration"],
                               "path": prior["path"] + [event["id"]]})
        longest[event["id"]] = max(candidates, key=lambda row: (row["end"] - row["start"], row["active"]))

    winner = max(longest.values(), key=lambda row: (row["end"] - row["start"], row["active"]))
    start_time = min(event["start"] for event in events)
    end_time = max(event["end"] for event in events)
    span = end_time - start_time
    worker_events = [event for event in events if event["role"] == "worker"]
    worker_seconds = sum(event["duration"] for event in worker_events)
    # Half-open intervals: end events sort before starts at the same timestamp.
    points = []
    for event in worker_events:
        points.append((event["start"], 1))
        points.append((event["end"], -1))
    active = peak = 0
    for _, delta in sorted(points, key=lambda point: (point[0], point[1])):
        active += delta
        peak = max(peak, active)
    return {
        "status": "measured",
        "checkpoint": document["checkpoint"],
        "source_tree_sha256": event_source,
        "event_count": len(events),
        "worker_event_count": len(worker_events),
        "observed_wall_span_seconds": round(span, 6),
        "worker_busy_seconds": round(worker_seconds, 6),
        "worker_busy_over_wall_span": round(worker_seconds / span, 6) if span else None,
        "peak_concurrent_workers": peak,
        "critical_path_wall_seconds": round(winner["end"] - winner["start"], 6),
        "critical_path_active_seconds": round(winner["active"], 6),
        "critical_path_event_ids": winner["path"],
    }


def make_report(build_receipt=None, test_receipt=None, timing_receipt=None,
                events_document=None, checkpoint=None):
    build = build_summary(build_receipt) if build_receipt is not None else None
    test = test_summary(test_receipt, timing_receipt) if test_receipt is not None else None
    issues = []
    source_hash = None
    if build is not None and build.get("source_tree_sha256"):
        source_hash = build["source_tree_sha256"]
    if test is not None:
        before, after = test.get("source_tree_before_sha256"), test.get("source_tree_after_sha256")
        if before and after and before != after:
            issues.append("Test source tree changed during its run")
        if source_hash and before and source_hash != before:
            issues.append("Build and test receipts bind different source trees")
        if not source_hash:
            source_hash = before
    worker = {"status": "unavailable", "reason": "No worker event receipt supplied"}
    if events_document is not None:
        worker = validate_event_file(events_document, checkpoint=checkpoint,
                                     source_sha256=source_hash)
    return {
        "schema": "m2m-critical-path-review-v1",
        "checkpoint": checkpoint,
        "build": build,
        "test": test,
        "worker_overlap": worker,
        "integrity_issues": issues,
        "causal_attribution": "not inferred from separate receipts",
    }


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--checkpoint", help="Checkpoint label for this report/event set")
    parser.add_argument("--build-receipt", type=Path)
    parser.add_argument("--test-receipt", type=Path)
    parser.add_argument("--timing-receipt", type=Path)
    parser.add_argument("--worker-events", type=Path)
    parser.add_argument("--output", type=Path, help="Write report JSON here; default is stdout")
    args = parser.parse_args(argv)
    try:
        supplied = [args.build_receipt, args.test_receipt, args.timing_receipt, args.worker_events]
        if not any(supplied):
            parser.error("supply at least one receipt")
        if args.timing_receipt and not args.test_receipt:
            parser.error("--timing-receipt requires --test-receipt")
        report = make_report(
            read_json(args.build_receipt) if args.build_receipt else None,
            read_json(args.test_receipt) if args.test_receipt else None,
            read_json(args.timing_receipt) if args.timing_receipt else None,
            read_json(args.worker_events) if args.worker_events else None,
            checkpoint=args.checkpoint)
        rendered = json.dumps(report, indent=2) + "\n"
        if args.output:
            args.output.write_text(rendered, encoding="utf-8")
        else:
            sys.stdout.write(rendered)
        return 1 if report["integrity_issues"] else 0
    except (OSError, ValueError, json.JSONDecodeError) as exc:
        print(f"critical-path telemetry error: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
