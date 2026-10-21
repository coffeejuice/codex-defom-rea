"""Verify saved planar oracle provenance and chain/consumer consistency only."""
from copy import deepcopy
import json

from capture_oracle import digest
from verify_boundary_sort_observations import (
    EXE_SHA, ROOT, binding, check, read, relative, verify_files,
)
from pathlib import Path

SPECS = [("training", "training", 10720), ("held_out", "held-out", 23132)]
OPERATIONS = ["import-before", "planar-before", "planar-after", "object-planar", "import-after"]
REVIEWED_SOURCE_SHA = "6703d5f1995a8698bffbc65cf89aa8e53ff535344478015c9809e2ad58af7a70"


def check_chains(after):
    count, ranges, nodes = after["count"], after["ranges"], after["nodes"]
    check(type(count) is int and 0 <= count <= 100 and len(ranges) == count,
          "group count/ranges mismatch")
    check(1 <= len(nodes) <= 512 and all(type(n) is int for n in nodes),
          "flat node array shape mismatch")
    if count == 0:
        check(nodes == [-1], "empty boundary must contain only terminal -1")
        return []
    groups = []
    next_start = 1
    for index, row in enumerate(ranges):
        check(len(row) == 2 and all(type(n) is int for n in row), "invalid range shape")
        start, end = row
        check(start == next_start and start < end < len(nodes),
              "range is not contiguous with exactly one separator")
        group = nodes[start - 1:end]
        check(all(n > 0 for n in group), "nonpositive chain node")
        check(nodes[end] == (-1 if index == count - 1 else 0), "separator/sentinel mismatch")
        groups.append(group)
        next_start = end + 2
    check(len(nodes) == ranges[-1][1] + 1, "trailing flat-array data")
    return groups


def verify_events(evidence, run, cases, pid):
    capture = evidence["normalized_result"]
    check(run["pid"] == capture["pid"] == evidence["parameters"]["pid"] == pid, "PID mismatch")
    check(capture["session_id"] == evidence["parameters"]["session_id"], "session mismatch")
    check(capture["mode"] == evidence["parameters"]["mode"] == "attach"
          and capture["state"] == "detached" and capture["source_kind"] == "file",
          "capture lifecycle mismatch")
    check(capture["device_id"] == evidence["parameters"]["device_id"] == "local",
          "capture device mismatch")
    check(capture["messages_truncated"] is False and capture["cleanup_error"] is None,
          "capture truncated or cleanup failed")
    scripts = [s["script_id"] for s in capture["scripts"]]
    messages = capture["messages"]
    check(len(scripts) == 1 and all(m["script_id"] == scripts[0] and m["type"] == "send"
                                  for m in messages), "script identity mismatch or probe error")
    check(len(messages) == 5 * len(cases) + 1, "missing/extra events")
    first = messages[0]["payload"]
    check(first["operation"] == "hooks-installed" and first["pid"] == pid
          and int(first["base"], 16) > 0, "hook initialization mismatch")
    events = [m["payload"] for m in messages[1:]]
    check([e["sequence"] for e in events] == list(range(1, len(events) + 1)),
          "noncontiguous sequence")
    observations = []
    for index, case in enumerate(cases):
        group = events[index * 5:index * 5 + 5]
        check([e["operation"] for e in group] == OPERATIONS
              and all(e["file"] == Path(case["path"]).name for e in group),
              "event operation/file order mismatch")
        entered, before, after, obj, returned = group
        check(returned["status"] == 0, "nonzero import status")
        check(returned["dimension"] == obj["dimension"] == 2 and obj["arity"] == 4,
              "dimension/arity mismatch")
        check(entered["dimension"] in (2, 3), "unexpected entry dimension")
        if index:
            check(entered["dimension"] == observations[-1]["dimension_after"],
                  "dimension discontinuity between imports")
        check(before["elements"] == case["element_count"]
              and before["connectivity"] == [n for row in case["connectivity"] for n in row],
              "input differs from fixture")
        groups = check_chains(after)
        check(obj["id"] == 1 and obj["groups"] == groups, "consumer groups differ from builder")
        check(type(obj["total"]) is int and obj["total"] == sum(len(g) - 1 for g in groups),
              "consumer total differs from sum of chain lengths minus one")
        check(all(n <= case["node_count"] for g in groups for n in g), "chain node outside input")
        observations.append({
            "case_id": case["id"], "classification": case["classification"],
            "fixture": case["path"], "fixture_sha256": case["sha256"],
            "sequence_range": [group[0]["sequence"], group[-1]["sequence"]],
            "status": returned["status"], "dimension_before": entered["dimension"],
            "dimension_after": returned["dimension"],
            "builder_input": {k: before[k] for k in ("elements", "connectivity")},
            "builder_output": {k: after[k] for k in ("count", "ranges", "nodes")},
            "object_output": {k: obj[k] for k in ("id", "dimension", "arity", "total", "groups")},
        })
    return observations


def verify_capture(spec, manifest):
    split, suffix, pid = spec
    evidence_path = ROOT / "analysis" / ("planar-runtime-" + suffix + ".json")
    directory = ROOT / "analysis" / "oracle" / ("planar-boundary-" + suffix)
    run_path = directory / "run.json"
    evidence, run = read(evidence_path), read(run_path)
    cases = [c for c in manifest["cases"] if c["split"] == split]
    check(len(cases) == 6, "fixture split size mismatch")
    order = [Path(c["path"]).name for c in cases]
    check(Path(run["cwd"]) == directory and run["ordered_imports"] == order,
          "run directory/import order mismatch")
    exe = ROOT / "source" / "DEF_PRE_64.exe"
    check(Path(run["executable"]) == exe and
          run["executable_sha256"] == EXE_SHA == digest(exe), "executable identity mismatch")
    check(run["status"] == "completed" and run["exit_code"] == 0, "run did not complete")
    verify_files(run, directory)
    check([item["name"] for item in run["inputs"]] == order, "input list mismatch")
    for item, case in zip(run["inputs"], cases):
        source, copy = ROOT / case["path"], directory / item["name"]
        check(Path(item["source"]) == source and Path(item["copy"]) == copy, "fixture path mismatch")
        check(source.stat().st_size == copy.stat().st_size == case["size_bytes"], "fixture size mismatch")
        check(len({case["sha256"], item["sha256_before"], item["sha256_after"],
                   item["source_sha256_after"], digest(source), digest(copy)}) == 1,
              "fixture hash mismatch")
    capture = evidence["normalized_result"]
    script, gate = directory.with_suffix(".js"), directory.with_suffix(".ready")
    template = ROOT / "tools" / "observe_planar_boundary.js"
    check(Path(run["ready_gate"]) == gate and Path(capture["source_path"]) == script
          and digest(script) == capture["source_sha256"], "probe path/hash mismatch")
    source = template.read_text(encoding="utf-8")
    check(source.count("__READY_GATE__") == 1 and script.read_text(encoding="utf-8") ==
          source.replace("__READY_GATE__", gate.as_posix()), "probe differs from passive template")
    observations = verify_events(evidence, run, cases, pid)
    return {
        "split": split, "pid": pid, "evidence": binding(evidence_path),
        "evidence_id": evidence["evidence_id"], "run_record": binding(run_path),
        "probe": binding(script), "probe_template": binding(template),
        "event_count": len(observations) * 5, "imports": observations,
    }, (evidence, run, cases, pid)


def tampering_checks(data):
    def payload(e, index):
        return e["normalized_result"]["messages"][index]["payload"]

    changes = {
        "PID": lambda e: e["normalized_result"].update(pid=-1),
        "sequence": lambda e: payload(e, 1).update(sequence=99),
        "range_start": lambda e: payload(e, 3)["ranges"][0].__setitem__(0, 2),
        "terminal": lambda e: payload(e, 3)["nodes"].__setitem__(-1, 0),
        "separator": lambda e: payload(e, 28)["nodes"].__setitem__(13, 42),
        "consumer_group": lambda e: payload(e, 4)["groups"][0].__setitem__(0, 99),
        "consumer_total": lambda e: payload(e, 4).update(total=99),
        "dimension": lambda e: payload(e, 4).update(dimension=3),
        "arity": lambda e: payload(e, 4).update(arity=8),
        "status": lambda e: payload(e, 5).update(status=2),
        "truncation": lambda e: e["normalized_result"].update(messages_truncated=True),
        "missing_event": lambda e: e["normalized_result"]["messages"].pop(),
    }
    results = []
    for name, change in changes.items():
        evidence = deepcopy(data[0])
        change(evidence)
        try:
            verify_events(evidence, *data[1:])
        except ValueError as error:
            results.append({"mutation": name, "result": "REJECTED", "reason": str(error)})
        else:
            raise ValueError("tampering accepted: " + name)
    # The held-out opposite-orientation duplicate case observes the empty branch.
    try:
        check_chains({"count": 0, "ranges": [], "nodes": [0, -1]})
    except ValueError as error:
        results.append({"mutation": "empty_has_extra_data", "result": "REJECTED", "reason": str(error)})
    else:
        raise ValueError("invalid empty chain accepted")
    return results


def main():
    manifest_path = ROOT / "analysis" / "planar-boundary-evaluation-cases.json"
    manifest = read(manifest_path)
    check(manifest["target_sha256"] == EXE_SHA, "fixture manifest target mismatch")
    runs, data = [], []
    for spec in SPECS:
        result, inputs = verify_capture(spec, manifest)
        runs.append(result)
        data.append(inputs)
    static = []
    for name in ("mesh-0x14032da62.json", "mesh-0x14032d65e.json",
                 "mesh-0x1401fc290.json", "mesh-0x14017ebc0.json"):
        path = ROOT / "analysis" / name
        record = read(path)
        check(record["subject"]["digest"]["sha256"] == EXE_SHA, "static target mismatch")
        static.append(dict(binding(path), evidence_id=record["evidence_id"]))
    freeze_path = ROOT / "analysis" / "planar-candidate-freeze.json"
    freeze = read(freeze_path)
    current = []
    for item in freeze["files"]:
        path = ROOT / item["path"]
        sha = digest(path) if path.is_file() else None
        current.append(dict(item, current_sha256=sha, current_matches=sha == item["sha256"]))
    source = ROOT / "src" / "recovered" / "planar_boundary.cpp"
    report = {
        "result": "PASS", "target_sha256": EXE_SHA,
        "method": "Read-only original-capture provenance and structural consistency checks; no original or replacement execution and no replacement-generated expected arrays.",
        "fixture_manifest": binding(manifest_path), "static_evidence": static,
        "process_runs": 2, "imports": 12, "ordered_events": 60, "runs": runs,
        "candidate_freeze": {
            "record": binding(freeze_path), "declared_stage": freeze["stage"],
            "files": current,
            "qualification": "Current matches are informational and do not determine original-capture provenance. Stage wording is not an independently trusted timestamp.",
        },
        "static_review": {
            "source": relative(source), "reviewed_source_sha256": REVIEWED_SOURCE_SHA,
            "current_source_matches_review": source.is_file() and digest(source) == REVIEWED_SOURCE_SHA,
            "conclusion": "No discrepancy found within the positive-node bounded-size domain.",
            "checked": ["Closed-triangle fourth-edge suppression",
                        "First later reverse-directed edge cancellation and in-place compaction",
                        "Chain lookup restarts at outer starting edge plus one",
                        "Inclusive one-based ranges, zero separators, final -1 and empty [-1]"],
            "limitation": "Nonpositive node IDs and out-of-domain sizes are rejected rather than reproducing native invalid-input behavior.",
        },
        "tampering_checks": tampering_checks(data[0]),
        "observations": {
            "group_counts_by_split": {run["split"]: [
                i["builder_output"]["count"] for i in run["imports"]] for run in runs},
            "ring_cases": [{"case_id": i["case_id"], "observed_groups": i["builder_output"]["count"]}
                          for run in runs for i in run["imports"] if "ring_with_hole" in i["case_id"]],
        },
        "limitations": [
            "Exact original ordering and orientation are preserved; structural validation does not reconstruct cancellation or predict expected chains.",
            "The probe observes dimension3 before the first header import and dimension2 afterward. Later imports retain dimension2; consumer arity is4.",
            "Reordered and reversed-orientation held-out inputs, duplicate stress cases, and two-group rings are bounded observations, not universal planar compatibility.",
            "Current fixture-manifest not-run fields describe preparation time; this report records later captures without modifying the manifest.",
            "Replacement agreement is assessed separately. Source review is bound to its recorded hash and is not a proof of complete application equivalence.",
        ],
    }
    output = ROOT / "analysis" / "planar-observations.json"
    output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print("PASS: 2 original runs, 12 imports, 60 events; 13 tampering checks rejected")
    print(relative(output))


if __name__ == "__main__":
    main()
