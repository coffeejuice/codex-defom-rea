"""Check retained original boundary/sort captures; never generate expected outputs."""
from __future__ import annotations

from collections import Counter
from copy import deepcopy
import json
from pathlib import Path

from capture_oracle import digest

ROOT = Path(__file__).resolve().parents[1]
EXE_SHA = "f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794"
RUNS = [
    ("training", "training", "boundary-sort-training", 30892),
    ("held_out", "held-out", "boundary-sort-held-out-retry", 2344),
]
OPERATIONS = [
    "import-before", "boundary-before", "sort-before", "sort-after",
    "boundary-after", "object-boundary", "import-after",
]


def check(condition, message):
    if not condition:
        raise ValueError(message)


def read(path):
    return json.loads(path.read_text(encoding="utf-8-sig"))


def relative(path):
    return path.resolve().relative_to(ROOT).as_posix()


def binding(path):
    return {"path": relative(path), "sha256": digest(path)}


def verify_files(run, directory):
    names = [item["name"] for item in run["files"]]
    check(len(names) == len(set(names)), "duplicate recorded file")
    check({"stdin.bin", "stdout.bin", "stderr.bin"} <= set(names),
          "missing transcript binding")
    for item in run["files"]:
        path = directory / item["name"]
        check(path.parent == directory and path.stat().st_size == item["bytes"]
              and digest(path) == item["sha256"], "captured file identity mismatch")
    check((directory / "stdin.bin").read_bytes() == run["stdin"].encode("ascii"),
          "stdin transcript mismatch")
    check((directory / "stderr.bin").read_bytes() == b"", "nonempty stderr")


def verify_probe(capture, run, run_name):
    template = ROOT / "tools" / "observe_boundary_sort.js"
    script = ROOT / "analysis" / "oracle" / (run_name + ".js")
    check(Path(capture["source_path"]) == script
          and digest(script) == capture["source_sha256"], "probe hash/path mismatch")
    raw = script.read_bytes()
    source = raw.decode("utf-8-sig")
    text = template.read_bytes().decode("utf-8-sig")
    check(text.count("__READY_GATE__") == 1, "template gate placeholder mismatch")
    gate = ROOT / "analysis" / "oracle" / (run_name + ".ready")
    check(Path(run["ready_gate"]) == gate, "gate path mismatch")
    forward = text.replace("__READY_GATE__", gate.as_posix())
    escaped = text.replace("__READY_GATE__", str(gate).replace("\\", "\\\\"))
    # These are the two retained serializations, not arbitrary whitespace removal.
    if run_name == "boundary-sort-training":
        check(source == forward, "training source differs from passive template")
        serialization = "forward-slash gate substitution; exact remaining bytes"
    else:
        check(source == escaped + "\r\n",
              "retry source differs beyond gate escaping and terminal CRLF")
        serialization = "escaped Windows gate substitution plus one terminal CRLF"
    return {
        "script": binding(script), "template": binding(template),
        "utf8_bom_present": raw.startswith(b"\xef\xbb\xbf"),
        "validated_serialization": serialization,
    }


def verify_events(evidence, run, cases, expected_pid):
    capture = evidence["normalized_result"]
    check(run["pid"] == capture["pid"] == evidence["parameters"]["pid"] == expected_pid,
          "PID mismatch")
    check(capture["session_id"] == evidence["parameters"]["session_id"],
          "session mismatch")
    check(capture["mode"] == evidence["parameters"]["mode"] == "attach"
          and capture["state"] == "detached" and capture["source_kind"] == "file",
          "capture lifecycle mismatch")
    check(capture["device_id"] == evidence["parameters"]["device_id"] == "local",
          "device mismatch")
    check(capture["messages_truncated"] is False and capture["cleanup_error"] is None,
          "truncated capture or cleanup error")
    scripts = [item["script_id"] for item in capture["scripts"]]
    messages = capture["messages"]
    check(len(scripts) == 1 and all(m["script_id"] == scripts[0]
          and m["type"] == "send" for m in messages), "script mismatch or probe error")
    check(len(messages) == 1 + len(cases) * 7, "missing or extra events")
    first = messages[0]["payload"]
    check(first["operation"] == "hooks-installed" and first["pid"] == expected_pid
          and int(first["base"], 16) > 0, "hook initialization mismatch")
    events = [m["payload"] for m in messages[1:]]
    check([e["sequence"] for e in events] == list(range(1, len(events) + 1)),
          "noncontiguous sequence")
    observations = []
    for index, case in enumerate(cases):
        group = events[index * 7:index * 7 + 7]
        name = Path(case["path"]).name
        check([e["operation"] for e in group] == OPERATIONS
              and all(e["file"] == name for e in group), "event operation/file order")
        _, before, unsorted, sorted_, after, obj, returned = group
        check(returned["status"] == 0, "import returned nonzero status")
        check(before["elements"] == case["element_count"]
              and before["arity"] == case["arity"]
              and before["connectivity"] == [n for row in case["connectivity"] for n in row],
              "builder input differs from bound fixture")
        count = unsorted["count"]
        check(type(count) is int and count > 0 and sorted_["count"] == count
              and len(unsorted["records"]) == len(sorted_["records"]) == count * 2,
              "sort array shape mismatch")
        check(all(type(n) is int for e in (unsorted, sorted_) for n in e["records"]),
              "noninteger sort record")
        pairs_before = list(zip(unsorted["records"][::2], unsorted["records"][1::2]))
        pairs_after = list(zip(sorted_["records"][::2], sorted_["records"][1::2]))
        check(Counter(pairs_before) == Counter(pairs_after), "sort lost/changed records")
        check([p[0] for p in pairs_after] == sorted(p[0] for p in pairs_before),
              "sort keys are not nondecreasing")
        check(sorted(p[1] for p in pairs_before) == list(range(1, count + 1)),
              "candidate IDs are not a complete permutation")
        check(type(after["count"]) is int and 0 <= after["count"] <= count
              and len(after["faces"]) == after["count"] * 4
              and len(after["owner_elements"]) == after["count"],
              "boundary array shape mismatch")
        check(all(type(n) is int for field in ("faces", "owner_elements")
                  for n in after[field]), "noninteger boundary data")
        builder = {k: after[k] for k in ("count", "faces", "owner_elements")}
        check(obj["id"] == 1 and obj["dimension"] == 3
              and obj["total"] == after["count"] and obj["boundaries"] == [builder],
              "builder/consumer mismatch")
        for i, owner in enumerate(after["owner_elements"]):
            check(1 <= owner <= before["elements"], "owner outside input range")
            row = after["faces"][i * 4:i * 4 + 4]
            check(all(n in case["connectivity"][owner - 1] for n in row),
                  "face node not in observed owning element")
            if before["arity"] == 4:
                check(row[0] == row[3] and len(set(row[:3])) == 3,
                      "tetra face shape mismatch")
            else:
                check(len(set(row)) == 4, "cube face shape mismatch")
        observations.append({
            "case_id": case["id"], "fixture": case["path"],
            "fixture_sha256": case["sha256"], "sequence_range": [
                group[0]["sequence"], group[-1]["sequence"]],
            "status": returned["status"], "builder_input": {
                k: before[k] for k in ("elements", "arity", "connectivity")},
            "sort_before": {k: unsorted[k] for k in ("count", "records")},
            "sort_after": {k: sorted_[k] for k in ("count", "records")},
            "builder_output": builder,
            "object_output": {k: obj[k] for k in ("id", "dimension", "total", "boundaries")},
        })
    return observations


def verify_capture(spec, manifest):
    split, suffix, run_name, pid = spec
    evidence_path = ROOT / "analysis" / ("mesh-boundary-sort-runtime-" + suffix + ".json")
    run_path = ROOT / "analysis" / "oracle" / run_name / "run.json"
    evidence, run = read(evidence_path), read(run_path)
    cases = [c for c in manifest["cases"] if c["split"] == split]
    check(len(cases) == 5, "fixture split count mismatch")
    order = [Path(c["path"]).name for c in cases]
    check(Path(run["cwd"]) == run_path.parent and run["ordered_imports"] == order,
          "run directory/import order mismatch")
    exe = ROOT / "source" / "DEF_PRE_64.exe"
    check(Path(run["executable"]) == exe
          and run["executable_sha256"] == EXE_SHA == digest(exe), "executable mismatch")
    check(run["status"] == "completed" and run["exit_code"] == 0,
          "process did not complete normally")
    verify_files(run, run_path.parent)
    check([i["name"] for i in run["inputs"]] == order, "fixture input list mismatch")
    for item, case in zip(run["inputs"], cases):
        source = ROOT / case["path"]
        copy = run_path.parent / item["name"]
        check(Path(item["source"]) == source and Path(item["copy"]) == copy,
              "fixture path mismatch")
        check(source.stat().st_size == copy.stat().st_size == case["size_bytes"],
              "fixture size mismatch")
        check(len({item["sha256_before"], item["sha256_after"], item["source_sha256_after"],
                   case["sha256"], digest(source), digest(copy)}) == 1,
              "fixture hash mismatch")
    probe = verify_probe(evidence["normalized_result"], run, run_name)
    observations = verify_events(evidence, run, cases, pid)
    return {
        "split": split, "pid": pid, "evidence": binding(evidence_path),
        "evidence_id": evidence["evidence_id"], "run_record": binding(run_path),
        "probe": probe, "event_count": len(observations) * 7, "imports": observations,
    }, (evidence, run, cases, pid)


def tampering_checks(inputs):
    evidence, run, cases, pid = inputs

    def reverse_sort_pairs(e):
        records = e["normalized_result"]["messages"][4]["payload"]["records"]
        pairs = list(zip(records[::2], records[1::2]))
        records[:] = [value for pair in reversed(pairs) for value in pair]

    mutations = {
        "PID": lambda e: e["normalized_result"].update(pid=pid + 1),
        "sequence": lambda e: e["normalized_result"]["messages"][1]["payload"].update(sequence=99),
        "sort_record": lambda e: e["normalized_result"]["messages"][4]["payload"]["records"].__setitem__(1, 999),
        "sort_order": reverse_sort_pairs,
        "consumer_face": lambda e: e["normalized_result"]["messages"][6]["payload"]["boundaries"][0]["faces"].__setitem__(0, 999),
        "status": lambda e: e["normalized_result"]["messages"][7]["payload"].update(status=2),
        "truncation": lambda e: e["normalized_result"].update(messages_truncated=True),
        "missing_event": lambda e: e["normalized_result"]["messages"].pop(),
    }
    results = []
    for name, mutate in mutations.items():
        altered = deepcopy(evidence)
        mutate(altered)
        try:
            verify_events(altered, run, cases, pid)
        except ValueError as error:
            results.append({"mutation": name, "result": "REJECTED", "reason": str(error)})
        else:
            raise ValueError("tampering was accepted: " + name)
    return results


def main():
    manifest_path = ROOT / "analysis" / "boundary-evaluation-cases.json"
    manifest = read(manifest_path)
    check(manifest["target_sha256"] == EXE_SHA, "fixture manifest target mismatch")
    runs, inputs = [], []
    for spec in RUNS:
        result, data = verify_capture(spec, manifest)
        runs.append(result)
        inputs.append(data)
    excluded_path = ROOT / "analysis" / "oracle" / "boundary-sort-held-out" / "run.json"
    excluded = read(excluded_path)
    check(excluded["status"] == "gate_timeout" and excluded["pid"] == 940
          and excluded["exit_code"] != 0, "excluded attempt identity/status mismatch")
    verify_files(excluded, excluded_path.parent)
    report = {
        "result": "PASS", "target_sha256": EXE_SHA,
        "method": "Read-only provenance and internal-consistency validation of saved original captures. No replacement-generated expected values or program execution.",
        "fixture_manifest": binding(manifest_path),
        "process_runs": 2, "imports": 10, "ordered_events": 70,
        "runs": runs,
        "excluded_attempt": {
            "run_record": binding(excluded_path), "pid": 940, "status": "gate_timeout",
            "reason": "First held-out attempt never reported hook readiness and is excluded from all observation counts.",
            "qualification": "The harness establishes a readiness-gate timeout; the operator reported timeout before hooks. This is not a successful instrumented run or behavioral result.",
        },
        "tampering_checks": tampering_checks(inputs[0]),
        "limitations": [
            "Checks preserve observed ordering and establish sort permutation preservation/nondecreasing keys, not universal qsort or cancellation equivalence.",
            "Eight tetrahedral cases and two single-cube discovery cases are covered; no other arities, 2D, overflow or allocation-failure coverage.",
            "Fixture split labels are retained from the manifest. Implementation freeze chronology and replacement agreement require separate evidence.",
            "No candidate-face generation, duplicate cancellation, expected boundary count or ownership algorithm is implemented in this verifier.",
            "The current fixture manifest still says inputs were not run; those fields describe fixture-preparation time. This report records the later executions without editing the manifest.",
            "Retry probe bytes have no UTF-8 BOM; differences from template are escaped Windows ready-gate path and one extra terminal CRLF, both explicitly checked.",
        ],
    }
    output = ROOT / "analysis" / "boundary-sort-observations.json"
    output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print("PASS: 2 original runs, 10 imports, 70 ordered events; 8 tampering checks rejected")
    print(relative(output))


if __name__ == "__main__":
    main()
