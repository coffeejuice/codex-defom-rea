"""Compare the frozen component-layout generator with retained original calls.

Only the replacement driver is executed. Expected arrays come directly from REA
layout-leave observations; no second implementation computes expected values.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import subprocess

from capture_oracle import digest

ROOT = Path(__file__).resolve().parents[1]
EXE_SHA = "f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794"
OFFSETS = ("0x86c", "0x870", "0x874", "0x878", "0x87c", "0x880", "0x884",
           "0x1320", "0x13f4", "0x3398", "0x33a8", "0x850", "0x888",
           "0x88c", "0x890", "0x33c8", "0x33d8", "0x11c4", "0x123c", "0x12b4")
QUERY_IDS = (24, 49, 53, 29, 54, 55, 46)
PAIRS = {"import-before": "import-after", "dimension-before": "dimension-after",
         "root-clear-enter": "root-clear-leave", "layout-enter": "layout-leave"}


def require(condition, message):
    if not condition:
        raise ValueError(message)


def read(path):
    return json.loads(path.read_text(encoding="utf-8-sig"))


def binding(path):
    path = path.resolve()
    return {"path": path.relative_to(ROOT).as_posix() if path.is_relative_to(ROOT)
            else str(path), "sha256": digest(path)}


def int32(value):
    return type(value) is int and -(2**31) <= value < 2**31


def verify_freeze(driver, require_frozen_candidate):
    path = ROOT / "analysis/component-layout-candidate-freeze.json"
    freeze = read(path)
    current_files = []
    for item in freeze["files"]:
        require(digest(ROOT / item["archive"]) == item["sha256"],
                "frozen archive changed: " + item["archive"])
        current = ROOT / item["path"]
        current_sha = digest(current) if current.is_file() else None
        current_files.append({"path": item["path"], "sha256": current_sha,
                              "matches_freeze": current_sha == item["sha256"]})
    binary = next(item for item in freeze["files"] if item["path"].endswith(".exe"))
    driver_binding = binding(driver)
    driver_matches = driver_binding["sha256"] == binary["sha256"]
    current_matches = all(item["matches_freeze"] for item in current_files)
    if require_frozen_candidate:
        require(current_matches and driver_matches,
                "current candidate differs from freeze under --require-frozen-candidate")
    return {"manifest": binding(path), "created_utc": freeze["created_utc"],
            "stage": freeze["stage"], "verified_files": freeze["files"],
            "current_files": current_files, "current_files_match_freeze": current_matches,
            "driver": driver_binding, "driver_matches_freeze": driver_matches,
            "frozen_candidate_required": require_frozen_candidate}


def provenance(path):
    evidence = read(path)
    cap, params = evidence["normalized_result"], evidence["parameters"]
    script = Path(cap["source_path"]).resolve()
    directory = script.with_suffix("")
    require(directory.is_relative_to(ROOT / "analysis/oracle"), "run outside oracle directory")
    run_path = directory / "run.json"
    run = read(run_path)
    require(Path(run["cwd"]).resolve() == directory, "run directory mismatch")
    require(run["status"] == "completed" and run["exit_code"] == 0,
            "original did not exit normally")
    executable = ROOT / "source/DEF_PRE_64.exe"
    require(Path(run["executable"]).resolve() == executable.resolve()
            and digest(executable) == run["executable_sha256"] == EXE_SHA,
            "original executable identity mismatch")
    require(cap["pid"] == params["pid"] == run["pid"]
            and cap["target"] == f"DEF_PRE_64.exe (pid {run['pid']})"
            and cap["session_id"] == params["session_id"], "process/session mismatch")
    require(cap["state"] == "detached" and cap["mode"] == params["mode"] == "attach"
            and cap["device_id"] == params["device_id"] == "local"
            and cap["source_kind"] == "file", "capture lifecycle mismatch")
    require(cap["messages_truncated"] is False and cap["cleanup_error"] is None,
            "incomplete capture")
    gate = Path(run["ready_gate"]).resolve()
    require(gate == directory.with_suffix(".ready"), "gate path mismatch")
    template = ROOT / "tools/observe_root_layout.js"
    require(digest(script) == cap["source_sha256"]
            and script.read_text(encoding="utf-8-sig") ==
            template.read_text(encoding="utf-8-sig").replace("__READY_GATE__", gate.as_posix()),
            "probe differs from retained passive template")
    pid_path = directory.with_suffix(".pid.json")
    pid = read(pid_path)
    require(pid["pid"] == run["pid"] and pid["executable_sha256"] == EXE_SHA,
            "PID record mismatch")
    for item in run["files"]:
        output = directory / item["name"]
        require(output.parent.resolve() == directory and output.is_file()
                and output.stat().st_size == item["bytes"]
                and digest(output) == item["sha256"], "run output mismatch: " + item["name"])
    require(sum((directory / name).stat().st_size for name in ("stdout.bin", "stderr.bin"))
            < run["max_output_bytes"], "output bound exceeded")
    require((directory / "stdin.bin").read_bytes() == run["stdin"].encode("utf-8"),
            "stdin transcript mismatch")
    require([item["name"] for item in run["inputs"]] == run["ordered_imports"],
            "input order mismatch")
    fixtures = []
    for item in run["inputs"]:
        source, copy = Path(item["source"]).resolve(), Path(item["copy"]).resolve()
        require(copy == directory / item["name"] and copy.parent == directory,
                "input copy path mismatch")
        require(digest(source) == digest(copy) == item["sha256_before"]
                == item["sha256_after"] == item["source_sha256_after"],
                "fixture changed: " + item["name"])
        fixtures.append(binding(source))
    case_manifest = {
        "root-layout-runtime-training.json": "root-layout-training-cases.json",
        "root-layout-runtime-held-out.json": "root-layout-evaluation-cases.json",
    }.get(path.name)
    case_binding = None
    if case_manifest:
        manifest_path = ROOT / "analysis" / case_manifest
        cases = read(manifest_path)
        require(cases["cases"] == fixtures, "predeclared fixture manifest mismatch")
        case_binding = {"manifest": binding(manifest_path), "purpose": cases["purpose"]}
    messages, scripts = cap["messages"], cap["scripts"]
    require(len(scripts) == 1 and messages and all(message["type"] == "send"
            and message["script_id"] == scripts[0]["script_id"] for message in messages),
            "script/message error")
    hook = messages[0]["payload"]
    require(hook["operation"] == "hooks-installed" and hook["pid"] == run["pid"],
            "hook process identity mismatch")
    events = [message["payload"] for message in messages[1:]]
    require([event["sequence"] for event in events] == list(range(1, len(events) + 1)),
            "event sequence gaps")
    return events, run, {
        "capture": binding(path), "evidence_id": evidence["evidence_id"],
        "run_manifest": binding(run_path), "pid": run["pid"],
        "probe": binding(script), "probe_template": binding(template),
        "pid_record": binding(pid_path), "fixtures": fixtures,
        "case_manifest": case_binding,
        "executable_sha256": EXE_SHA, "event_count": len(events),
        "event_counts": dict(sorted(Counter(e["operation"] for e in events).items())),
    }


def pair_events(events, order):
    stack, imports, layouts, serials = [], [], [], []
    for event in events:
        operation = event["operation"]
        if operation in PAIRS:
            if operation == "import-before":
                require(not stack, "nested import outside capture scope")
                imports.append(event["file"])
            if operation == "layout-enter":
                serials.append(event["serial"])
            stack.append(event)
            continue
        require(stack and PAIRS[stack[-1]["operation"]] == operation,
                f"unpaired or unexpected event at {event['sequence']}: {operation}")
        before = stack.pop()
        require(before["file"] == event["file"], "paired event file mismatch")
        if operation in ("import-after", "dimension-after"):
            require(before["state"]["address"] == event["state"]["address"],
                    "paired root identity mismatch")
            if operation == "dimension-after":
                require(before["request"] == event["request"], "dimension request mismatch")
        else:
            require(before["root"] == event["root"], "paired root identity mismatch")
        if operation == "layout-leave":
            require(all(before[key] == event[key]
                        for key in ("serial", "kind", "dimension", "scalars")),
                    "layout enter/leave mismatch")
            layouts.append((before["sequence"], event))
    require(not stack and imports == order, "incomplete or reordered imports/events")
    require(serials == list(range(1, len(serials) + 1)) and layouts,
            "missing or nonsequential layout calls")
    return layouts


def replay(driver, before_sequence, event):
    kind, dimension, scalars = event["kind"], event["dimension"], event["scalars"]
    require(kind in ("initialize", "refresh") and dimension in (2, 3),
            "unsupported captured layout kind/dimension")
    require(set(scalars) == set(OFFSETS) and all(int32(v) for v in scalars.values()),
            "incomplete or invalid scalar capture")
    # All captured scalars are stable here; refresh-only fields are not inputs to initialize.
    require(event["after_scalars"] == scalars, "layout changed a captured input scalar")
    queries = event["queries"]
    expected_ids = QUERY_IDS if dimension == 3 else QUERY_IDS[:-1]
    require([query["id"] for query in queries] == list(expected_ids), "query order mismatch")
    query_inputs = {}
    for query in queries:
        qid, enabled = query["id"], query["enabled"]
        require(type(enabled) is int and 0 <= enabled <= 255, "invalid query return byte")
        if qid in (24, 49, 46):
            require(query["width"] is None and query["width_before"] is None,
                    "unexpected output pointer")
            width = 0
        else:
            require(int32(query["width"]) and int32(query["width_before"]),
                    "missing query width")
            width = query["width"]
        query_inputs[qid] = (int(bool(enabled)), width)
    # The 2D path does not call feature46; the driver's unused pair is explicitly zero-filled.
    query_inputs.setdefault(46, (0, 0))
    values = [dimension] + [value for qid in QUERY_IDS for value in query_inputs[qid]]
    values += [scalars[offset] for offset in OFFSETS]
    require(len(values) == 35, "driver input extent mismatch")
    expected = event["components"]
    require(len(expected) == 79 and all(int32(value) for value in expected),
            "incomplete original output")
    command = kind + " " + " ".join(map(str, values)) + "\n"
    process = subprocess.run([str(driver)], input=command, text=True, capture_output=True,
                             timeout=10, creationflags=subprocess.CREATE_NO_WINDOW)
    require(process.returncode == 0 and not process.stderr,
            "replacement failed: " + process.stderr)
    actual = json.loads(process.stdout)
    require(type(actual) is list and len(actual) == 79 and all(int32(v) for v in actual),
            "invalid replacement output")
    differences = [{"slot": index, "root_offset": hex(0x15f8 + index * 4),
                    "original": original, "candidate": candidate}
                   for index, (original, candidate) in enumerate(zip(expected, actual))
                   if original != candidate]
    return {
        "serial": event["serial"], "file": event["file"], "kind": kind,
        "dimension": dimension, "enter_sequence": before_sequence,
        "leave_sequence": event["sequence"], "used_scalar_offsets":
            list(OFFSETS if kind == "refresh" else OFFSETS[:15]),
        "scalars_before_equal_after": True, "queries": queries,
        "uncalled_query_ids_zero_filled": [] if dimension == 3 else [46],
        "driver_input": command.rstrip(), "original_components": expected,
        "compared_slots": 79, "differences": differences, "matched": not differences,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--capture", action="append", type=Path)
    parser.add_argument("--driver", type=Path,
                        default=ROOT / "build/Release/component_layout_replay.exe")
    parser.add_argument("--report", type=Path)
    parser.add_argument("--require-frozen-candidate", action="store_true",
                        help="Require current sources and driver to match the archived freeze")
    args = parser.parse_args()
    driver = args.driver.resolve()
    report = {"scope": "Internal component-table generators only; original arrays are the oracle.",
              "candidate": verify_freeze(driver, args.require_frozen_candidate), "captures": [],
              "limitations": [
                  "Capture classification and freeze chronology rely on retained manifests, not an external timestamp authority.",
                  "Feature-query results are captured inputs; the feature-query implementation is not recreated or tested.",
                  "An absent 2D feature46 call is supplied as disabled/width0 only because that input is unused.",
                  "Query widths use the captured post-call value, including any retained prior local value.",
                  "Only observed scalar/query combinations are tested; unobserved branches remain outside runtime coverage.",
                  "Root transitions, object cleanup, parser integration and scientific finalization are not replayed.",
              ]}
    for path in args.capture or [ROOT / "analysis/root-layout-runtime-training.json"]:
        events, run, entry = provenance(path.resolve())
        entry["layouts"] = [replay(driver, sequence, event)
                            for sequence, event in pair_events(events, run["ordered_imports"])]
        report["captures"].append(entry)
    rows = [row for capture in report["captures"] for row in capture["layouts"]]
    report["layout_count"] = len(rows)
    report["compared_slots"] = 79 * len(rows)
    report["coverage"] = dict(sorted(Counter(f"{r['kind']}:dimension{r['dimension']}"
                                            for r in rows).items()))
    report["matched"] = all(row["matched"] for row in rows)
    report["observed_query_results"] = {
        str(qid): sorted({(q["enabled"], q["width"]) for row in rows
                          for q in row["queries"] if q["id"] == qid},
                         key=lambda value: (value[0], value[1] or 0))
        for qid in QUERY_IDS}
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{'PASS' if report['matched'] else 'FAIL'}: {len(rows)} layouts, "
          f"{report['compared_slots']} integer slots, {len(report['captures'])} captures")
    return 0 if report["matched"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
