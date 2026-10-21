"""Validate saved original eight-slot observations without running either program."""
from collections import Counter
from copy import deepcopy
import json
from pathlib import Path
import struct

from capture_oracle import digest
from verify_boundary_sort_observations import (
    EXE_SHA, ROOT, binding, check, read, relative, verify_files,
)

SPECS = [("training", "training", 34328),
         ("held_out", "held-out", 33656),
         ("discovery", "discovery", 20360)]
OPERATIONS = [
    "import-before", "boundary-before", "hexa-before", "hexa-classification",
    "sort-before", "sort-after", "hexa-after", "boundary-after",
    "object-boundary", "import-after",
]


def static_context():
    table_path = ROOT / "analysis" / "hexa-tables.json"
    evidence = read(table_path)
    check(evidence["subject"]["digest"]["sha256"] == EXE_SHA, "table target mismatch")
    result = evidence["normalized_result"]
    raw = bytes.fromhex(result["bytes_hex"])
    check(result["complete"] is True and
          result["requested_bytes"] == result["returned_bytes"] == len(raw) == 384,
          "incomplete table bytes")
    check(result == evidence["raw_result"], "raw/normalized table mismatch")
    base = int(result["address"], 16)
    tables = {}
    for name, address, count in [
        ("standard_table", 0x140604fd0, 24),
        ("tetra_table", 0x140604f10, 18),
        ("wedge_table", 0x140605030, 24),
    ]:
        offset = address - base
        check(offset >= 0 and offset + count * 4 <= len(raw), "table outside read")
        tables[name] = list(struct.unpack("<" + "i" * count, raw[offset:offset + count * 4]))
    references = [dict(binding(table_path), evidence_id=evidence["evidence_id"])]
    for name in ("mesh-0x14035672f.json", "mesh-0x140354ed0.json", "hexa-callsite.json"):
        path = ROOT / "analysis" / name
        record = read(path)
        check(record["subject"]["digest"]["sha256"] == EXE_SHA, "static target mismatch")
        references.append(dict(binding(path), evidence_id=record["evidence_id"]))
    return tables, references


def output_array(event):
    count = event["count"]
    check(type(count) is int and 0 <= count <= 128
          and len(event["faces"]) == 4 * count
          and len(event["owner_elements"]) == count, "boundary array shape mismatch")
    check(all(type(n) is int for field in ("faces", "owner_elements")
              for n in event[field]), "noninteger output")
    return {k: event[k] for k in ("count", "faces", "owner_elements")}


def face_owner_multiset(event):
    return Counter(tuple(event["faces"][i * 4:i * 4 + 4]) + (owner,)
                   for i, owner in enumerate(event["owner_elements"]))


def verify_events(evidence, run, cases, pid, tables):
    capture = evidence["normalized_result"]
    check(run["pid"] == capture["pid"] == evidence["parameters"]["pid"] == pid,
          "PID mismatch")
    check(capture["session_id"] == evidence["parameters"]["session_id"],
          "session mismatch")
    check(capture["mode"] == evidence["parameters"]["mode"] == "attach"
          and capture["state"] == "detached" and capture["source_kind"] == "file",
          "capture lifecycle mismatch")
    check(capture["device_id"] == evidence["parameters"]["device_id"] == "local",
          "capture device mismatch")
    check(capture["messages_truncated"] is False and capture["cleanup_error"] is None,
          "truncated capture or cleanup failure")
    scripts = [s["script_id"] for s in capture["scripts"]]
    messages = capture["messages"]
    check(len(scripts) == 1 and all(m["script_id"] == scripts[0] and m["type"] == "send"
                                  for m in messages), "script mismatch or runtime probe error")
    check(len(messages) == len(cases) * 10 + 1, "missing or extra events")
    first = messages[0]["payload"]
    check(first["operation"] == "hooks-installed" and first["pid"] == pid
          and int(first["base"], 16) > 0, "hook initialization mismatch")
    events = [m["payload"] for m in messages[1:]]
    check([e["sequence"] for e in events] == list(range(1, len(events) + 1)),
          "noncontiguous event sequence")
    observations = []
    for i, case in enumerate(cases):
        group = events[i * 10:i * 10 + 10]
        check([e["operation"] for e in group] == OPERATIONS
              and all(e["file"] == Path(case["path"]).name for e in group),
              "event operation/file order mismatch")
        _, before, hexa, classes, sort_before, sort_after, inner, wrapper, obj, returned = group
        check(returned["status"] == 0, "import status is nonzero")
        check(before["elements"] == hexa["elements"] == case["element_count"]
              and before["arity"] == hexa["arity"] == case["arity"] == 8
              and before["connectivity"] == [n for row in case["connectivity"] for n in row],
              "input differs from fixture manifest")
        check(hexa["face_width"] == 4 and hexa["faces_per_element"] == 6,
              "unexpected inner call shape")
        check(all(hexa[key] == value for key, value in tables.items()),
              "runtime table differs from static bytes")
        classification = {k: classes[k] for k in ("node_kinds", "face_counts", "face_widths")}
        check(all(len(v) == case["element_count"] and all(type(n) is int for n in v)
                  for v in classification.values()), "classification array shape mismatch")
        check(all(row in {(8, 6, 4), (6, 5, 4), (4, 4, 3)}
                  for row in zip(*classification.values())), "unrecognized classification tuple")
        count = sort_before["count"]
        check(type(count) is int and 0 <= count <= 256
              and sort_after["count"] == count
              and len(sort_before["records"]) == len(sort_after["records"]) == count * 2,
              "sort shape mismatch")
        check(all(type(n) is int for e in (sort_before, sort_after) for n in e["records"]),
              "noninteger sort record")
        pairs_before = list(zip(sort_before["records"][::2], sort_before["records"][1::2]))
        pairs_after = list(zip(sort_after["records"][::2], sort_after["records"][1::2]))
        check(Counter(pairs_before) == Counter(pairs_after), "sort permutation changed records")
        check([p[0] for p in pairs_after] == sorted(p[0] for p in pairs_before),
              "sort keys are not nondecreasing")
        check(sorted(p[1] for p in pairs_before) == list(range(1, count + 1)),
              "candidate indices incomplete")
        inner_output, wrapper_output = output_array(inner), output_array(wrapper)
        check(inner["count"] <= count and face_owner_multiset(inner) == face_owner_multiset(wrapper),
              "inner/wrapper face-owner multiset mismatch")
        check(obj["id"] == 1 and obj["dimension"] == 3 and obj["total"] == wrapper["count"]
              and obj["boundaries"] == [wrapper_output], "consumer differs from wrapper")
        # Output owner integers are deliberately not required to index an input row.
        observations.append({
            "case_id": case["id"], "fixture": case["path"], "fixture_sha256": case["sha256"],
            "sequence_range": [group[0]["sequence"], group[-1]["sequence"]],
            "status": returned["status"], "builder_input": {
                k: before[k] for k in ("elements", "arity", "connectivity")},
            "inner_call": {k: hexa[k] for k in (
                "elements", "arity", "face_width", "faces_per_element",
                "standard_table", "tetra_table", "wedge_table")},
            "observed_classification": classification,
            "sort_before": {k: sort_before[k] for k in ("count", "records")},
            "sort_after": {k: sort_after[k] for k in ("count", "records")},
            "inner_output": inner_output, "wrapper_output": wrapper_output,
            "object_output": {k: obj[k] for k in ("id", "dimension", "total", "boundaries")},
            "observed_owner_values_outside_input_row_range": sorted(
                {n for n in wrapper["owner_elements"] if not 1 <= n <= case["element_count"]}),
        })
    return observations


def verify_capture(spec, manifest, tables):
    split, suffix, pid = spec
    evidence_path = ROOT / "analysis" / ("hexa-runtime-" + suffix + ".json")
    directory = ROOT / "analysis" / "oracle" / ("hexa-boundary-" + suffix)
    run_path = directory / "run.json"
    evidence, run = read(evidence_path), read(run_path)
    cases = [c for c in manifest["cases"] if c["split"] == split]
    check(len(cases) == (6 if split == "discovery" else 5), "fixture split count mismatch")
    order = [Path(c["path"]).name for c in cases]
    check(Path(run["cwd"]) == directory and run["ordered_imports"] == order,
          "run directory/order mismatch")
    exe = ROOT / "source" / "DEF_PRE_64.exe"
    check(Path(run["executable"]) == exe and
          run["executable_sha256"] == EXE_SHA == digest(exe), "executable identity mismatch")
    check(run["status"] == "completed" and run["exit_code"] == 0, "run did not complete")
    verify_files(run, directory)
    check([item["name"] for item in run["inputs"]] == order, "input list mismatch")
    for item, case in zip(run["inputs"], cases):
        source, copy = ROOT / case["path"], directory / item["name"]
        check(Path(item["source"]) == source and Path(item["copy"]) == copy,
              "fixture path mismatch")
        check(source.stat().st_size == copy.stat().st_size == case["size_bytes"],
              "fixture size mismatch")
        check(len({item["sha256_before"], item["sha256_after"], item["source_sha256_after"],
                   case["sha256"], digest(source), digest(copy)}) == 1, "fixture hash mismatch")
    capture = evidence["normalized_result"]
    script = directory.with_suffix(".js")
    template = ROOT / "tools" / "observe_hexa_boundary.js"
    gate = directory.with_suffix(".ready")
    check(Path(run["ready_gate"]) == gate and Path(capture["source_path"]) == script
          and digest(script) == capture["source_sha256"], "probe path/hash mismatch")
    # Python text-mode generation normalizes source newlines on read and emits
    # platform newlines on write. No code/whitespace changes are otherwise allowed.
    expected = template.read_text(encoding="utf-8").replace("__READY_GATE__", gate.as_posix())
    check(template.read_text(encoding="utf-8").count("__READY_GATE__") == 1
          and script.read_text(encoding="utf-8") == expected, "probe differs from template")
    observations = verify_events(evidence, run, cases, pid, tables)
    return {
        "split": split, "pid": pid, "evidence": binding(evidence_path),
        "evidence_id": evidence["evidence_id"], "run_record": binding(run_path),
        "probe": binding(script), "probe_template": binding(template),
        "event_count": len(observations) * 10, "imports": observations,
    }, (evidence, run, cases, pid, tables)


def tampering_checks(data):
    def payload(e, index):
        return e["normalized_result"]["messages"][index]["payload"]

    def reverse_pairs(e):
        records = payload(e, 6)["records"]
        records[:] = [n for pair in reversed(list(zip(records[::2], records[1::2]))) for n in pair]

    changes = {
        "PID": lambda e: e["normalized_result"].update(pid=-1),
        "sequence": lambda e: payload(e, 1).update(sequence=500),
        "table": lambda e: payload(e, 3)["standard_table"].__setitem__(0, 99),
        "classification_shape": lambda e: payload(e, 4)["node_kinds"].pop(),
        "sort_record": lambda e: payload(e, 6)["records"].__setitem__(1, 99),
        "sort_order": reverse_pairs,
        "inner_owner": lambda e: payload(e, 7)["owner_elements"].__setitem__(0, 99),
        "consumer": lambda e: payload(e, 9)["boundaries"][0]["faces"].__setitem__(0, 99),
        "status": lambda e: payload(e, 10).update(status=2),
        "missing_event": lambda e: e["normalized_result"]["messages"].pop(),
        "truncation": lambda e: e["normalized_result"].update(messages_truncated=True),
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
    return results


def main():
    manifest_path = ROOT / "analysis" / "hexa-boundary-evaluation-cases.json"
    manifest = read(manifest_path)
    check(manifest["target_sha256"] == EXE_SHA, "manifest target mismatch")
    tables, static = static_context()
    runs, data = [], []
    for spec in SPECS:
        observation, inputs = verify_capture(spec, manifest, tables)
        runs.append(observation)
        data.append(inputs)
    freeze_path = ROOT / "analysis" / "hexa-candidate-freeze.json"
    freeze = read(freeze_path)
    current_artifacts_match = all(digest(ROOT / item["path"]) == item["sha256"]
                                  for item in freeze["files"])
    # Later component additions can relink the shared replay executable. Keep
    # the evaluated binary separately, rather than treating a rebuild as an
    # original-capture provenance failure.
    archived_driver = ROOT / "analysis/oracle/frozen-hexa-candidate/mesh_boundary_replay.exe"
    frozen_driver = next(item for item in freeze["files"] if item["path"].startswith("build/"))
    if archived_driver.exists():
        check(digest(archived_driver) == frozen_driver["sha256"], "archived candidate changed")
    else:
        check(current_artifacts_match, "frozen candidate is neither current nor archived")
    report = {
        "result": "PASS", "target_sha256": EXE_SHA,
        "method": "Read-only provenance and internal-consistency verification of original passive captures; no expected outputs from a replacement and no program execution.",
        "fixture_manifest": binding(manifest_path), "static_evidence": static,
        "candidate_freeze": dict(binding(freeze_path), current_artifacts_match=current_artifacts_match,
                                 archived_driver=binding(archived_driver) if archived_driver.exists() else None,
                                 files=freeze["files"]),
        "process_runs": 3, "imports": 16, "ordered_events": 160,
        "runs": runs, "tampering_checks": tampering_checks(data[0]),
        "static_review": {
            "reviewed_source_sha256": next(item["sha256"] for item in freeze["files"]
                                          if item["path"] == "src/recovered/hexa_boundary.cpp"),
            "conclusion": "No discrepancy found between reviewed hexa_boundary.cpp and retained helper/wrapper evidence within its declared nonnegative, non-overflowing input domain.",
            "checked": ["Exact table bytes", "Classification precedence", "Candidate filtering",
                        "Asymmetric duplicate membership and pairwise cancellation",
                        "Compressed-index ownership", "Collapsed-face compaction",
                        "Wrapper equal-sum first-two-node ordering"],
            "limitations": [
                "Skipping padded tetra table rows preserves filtered output but does not reproduce possible native pre-array-read access faults.",
                "Negative IDs and face-sum overflow raise exceptions instead of reproducing native behavior.",
                "Static review is a bounded code comparison, not proof of complete program equivalence.",
            ],
        },
        "limitations": [
            "Observed classifications and owner integers are retained verbatim. Owner values are not reinterpreted or constrained to input row IDs.",
            "The verifier compares inner/wrapper multisets of exact ordered face tuples plus owners; it permits wrapper row reordering and checks the consumer's exact final arrays.",
            "Sort checking establishes pair preservation and nondecreasing sums, not a universal equal-key permutation algorithm.",
            "The frozen driver is checked against its retained hash; current build identity is reported separately. The stated pre-capture stage is a local record, not an independent trusted timestamp.",
            "No replacement comparison is performed here. Separate comparison evidence is required for behavioral agreement.",
            "Coverage is ten standard evaluation inputs and six collapsed/mixed discovery inputs; broader degeneracy, negative IDs, overflow, allocation failure and 2D remain outside this corpus.",
            "Fixture-manifest not-run fields describe fixture preparation. This later report records completed captures without editing that manifest.",
        ],
    }
    output = ROOT / "analysis" / "hexa-observations.json"
    output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print("PASS: 3 original runs, 16 imports, 160 events; 11 tampering cases rejected")
    print(relative(output))


if __name__ == "__main__":
    main()
