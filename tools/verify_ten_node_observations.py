"""Verify retained arity-10 rejection traces, not global ten-node reachability."""
from copy import deepcopy
import json
from pathlib import Path

from capture_oracle import digest
from verify_boundary_sort_observations import (
    EXE_SHA, ROOT, binding, check, read, relative, verify_files,
)

BASE = "07_tetrahedron.KEY"
REJECT = "15_unsupported_ten_nodes.KEY"
FOLLOW = "16_unsupported_ten_followed_name.KEY"
SPECS = [("existing", 16224, [BASE, REJECT, FOLLOW, BASE]),
         ("fresh", 25120, [REJECT, FOLLOW, BASE])]
IDS = {"OBJTYP": 145, "OBJNAM": 144, "RZ": 159, "ELMCON": 173, "1": 305}


def object_name(obj):
    raw = obj["name_bytes"]
    check(len(raw) == 81 and all(type(x) is int and 0 <= x <= 255 for x in raw),
          "name capture shape mismatch")
    check(0 in raw, "name not terminated within probe")
    return bytes(raw[:raw.index(0)]).decode("ascii")


def check_object(obj):
    check(int(obj["address"], 16) > 0 and obj["id"] == 1, "object identity mismatch")
    object_name(obj)
    check(0 <= obj["nodes"] <= 16 and 0 <= obj["elements"] <= 16
          and obj["arity"] == 4 and obj["element_kind"] == 2, "object shape/kind mismatch")
    for count, pointer, field, width in [
        (obj["nodes"], "coordinates_pointer", "coordinates", 3),
        (obj["elements"], "connectivity_pointer", "connectivity", obj["arity"]),
    ]:
        if count:
            check(int(obj[pointer], 16) > 0 and len(obj.get(field, [])) == count * width,
                  "positive array missing or truncated")
        else:
            check(int(obj[pointer], 16) == 0 and field not in obj, "empty array is not null")
    for field, count in [("array_8e0_prefix", obj["nodes"]),
                         ("array_a30_prefix", obj["elements"]),
                         ("array_a38_prefix", obj["elements"])]:
        check((field in obj and len(obj[field]) == count) if count else field not in obj,
              "dependent array capture incomplete")


def check_state(state):
    check(int(state["address"], 16) > 0 and state["truncated"] is False
          and not state.get("cycle", False), "state capture incomplete")
    check(state["object_count"] == state["list_count"] == len(state["objects"])
          and state["object_count"] in (0, 1), "object list count mismatch")
    for obj in state["objects"]:
        check_object(obj)


def mesh(obj):
    return {k: v for k, v in obj.items() if k != "name_bytes"}


def normalized_state(state):
    result = {"object_count": state["object_count"], "list_count": state["list_count"], "objects": []}
    for obj in state["objects"]:
        clean = {k: v for k, v in obj.items()
                 if k not in ("address", "name_bytes", "coordinates_pointer", "connectivity_pointer")}
        clean["name"] = object_name(obj)
        clean["coordinates_pointer_nonnull"] = int(obj["coordinates_pointer"], 16) != 0
        clean["connectivity_pointer_nonnull"] = int(obj["connectivity_pointer"], 16) != 0
        result["objects"].append(clean)
    return result


def expected_pattern(name):
    keys = ["OBJTYP", "OBJNAM", "RZ", "ELMCON"] if name == BASE else ["ELMCON", "1"]
    if name == FOLLOW:
        keys.append("OBJNAM")
    pattern = [("import-before", None)]
    for key in keys:
        pattern.append(("keyword-resolved", key))
        if key == "1":
            continue
        pattern.extend([("handler-before", key), ("object-resolved", key)])
        if name == BASE and key == "ELMCON":
            pattern.append(("boundary-enter", key))
        pattern.append(("handler-after", key))
    pattern.extend((op, keys[-1]) for op in ("finalize-before", "finalize-after", "import-after"))
    return pattern


def verify_events(evidence, run, spec, follow_name):
    mode, pid, order = spec
    cap = evidence["normalized_result"]
    check(run["pid"] == cap["pid"] == evidence["parameters"]["pid"] == pid, "PID mismatch")
    check(cap["session_id"] == evidence["parameters"]["session_id"], "session mismatch")
    check(cap["mode"] == evidence["parameters"]["mode"] == "attach"
          and cap["state"] == "detached" and cap["source_kind"] == "file",
          "capture lifecycle mismatch")
    check(cap["device_id"] == evidence["parameters"]["device_id"] == "local", "device mismatch")
    check(cap["messages_truncated"] is False and cap["cleanup_error"] is None,
          "truncation or cleanup error")
    scripts = [s["script_id"] for s in cap["scripts"]]
    messages = cap["messages"]
    check(len(scripts) == 1 and all(m["type"] == "send" and m["script_id"] == scripts[0]
                                  for m in messages), "script mismatch or probe error")
    first = messages[0]["payload"]
    check(first["operation"] == "hooks-installed" and first["pid"] == pid
          and int(first["base"], 16) > 0, "hook initialization mismatch")
    events = [m["payload"] for m in messages[1:]]
    check([e.get("sequence") for e in events] == list(range(1, len(events) + 1)),
          "event sequence incomplete")
    check(len(events) == sum(len(expected_pattern(n)) for n in order), "missing/extra events")
    observations, offset, prior, object_address = [], 0, None, None
    for index, name in enumerate(order):
        pattern = expected_pattern(name)
        group = events[offset:offset + len(pattern)]
        offset += len(pattern)
        check([(e["operation"], e["keyword"]) for e in group] == pattern
              and all(e["file"] == name for e in group), "event order/keyword/file mismatch")
        before, after = group[0]["state"], group[-1]["state"]
        if prior is not None:
            check(before == prior, "state discontinuity between imports")
        else:
            check(before["object_count"] == 0, "initial model is not fresh")
        prior = after
        for event in group:
            if "state" in event:
                check_state(event["state"])
                check(event["state"]["address"] == before["address"], "root pointer changed")
                objs = event["state"]["objects"]
            else:
                objs = [event["object"]] if "object" in event else []
            for obj in objs:
                check(obj is not None, "object resolution failed")
                check_object(obj)
                if object_address is None:
                    object_address = obj["address"]
                check(obj["address"] == object_address, "object identity changed")
            if event["operation"] == "keyword-resolved":
                check(event["id"] == IDS[event["keyword"]] and
                      event["category"] == (0 if event["keyword"] == "1" else 2),
                      "keyword resolution mismatch")
            if event["operation"] in ("handler-before", "handler-after"):
                check(event["rva"] == 0x38b8a0 and event["id"] == IDS[event["keyword"]],
                      "handler identity mismatch")
            if event["operation"] == "handler-after":
                expected_status = 2 if name != BASE and event["keyword"] == "ELMCON" else 0
                check(event["status"] == expected_status, "handler status mismatch")
            if event["operation"] == "object-resolved":
                check(event["requested"] == 1, "object request mismatch")
        check(group[-1]["status"] == (0 if name == BASE else 2), "import status mismatch")
        check(group[-3]["state"] == group[-2]["state"] == after,
              "finalization changed captured fields")
        last_handler_state = [e["state"] for e in group if e["operation"] == "handler-after"][-1]
        check(last_handler_state == group[-3]["state"], "state changed between handler and finalization")
        created = before["object_count"] == 0 and after["object_count"] == 1
        if name != BASE:
            resolved = next(e["object"] for e in group if e["operation"] == "object-resolved")
            rejected = next(e["state"] for e in group if e["operation"] == "handler-after")
            check(rejected["objects"] == [resolved], "rejection changed resolved object")
            check(mesh(after["objects"][0]) == mesh(resolved), "rejection/followup changed mesh or pointers")
            if before["objects"]:
                check(mesh(before["objects"][0]) == mesh(resolved), "existing mesh changed before rejection")
            else:
                check(mode == "fresh" and index == 0 and created, "unexpected creation branch")
                check(resolved["type"] == 0 and resolved["mesh_flag"] == 0
                      and resolved["nodes"] == resolved["elements"] == 0
                      and object_name(resolved) == "", "fresh rejected object defaults mismatch")
            check(object_name(after["objects"][0]) ==
                  (follow_name if name == FOLLOW else object_name(resolved)), "name continuation mismatch")
            if name == REJECT:
                check(after["objects"] == [resolved], "rejection changed object fields")
        else:
            final = after["objects"][0]
            check(final["type"] == final["mesh_flag"] == 1
                  and final["nodes"] == 4 and final["elements"] == 1
                  and final["coordinates"] == [0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1]
                  and final["connectivity"] == [1, 2, 3, 4]
                  and object_name(final) == "REA tetrahedron", "supported fixture state mismatch")
            boundary = next(e for e in group if e["operation"] == "boundary-enter")
            check(boundary["arity"] == 4 and boundary["elements"] == 1, "supported boundary call mismatch")
        observations.append({
            "file": name, "sequence_range": [group[0]["sequence"], group[-1]["sequence"]],
            "status": group[-1]["status"], "object_created_during_import": created,
            "before": normalized_state(before), "after": normalized_state(after),
            "events": [{k: e[k] for k in ("sequence", "operation", "keyword", "id", "status")
                        if k in e} for e in group],
            "boundary_entry_count": sum(e["operation"] == "boundary-enter" for e in group),
            "finalization_observed": True,
            "captured_mesh_and_pointer_fields_unchanged_after_resolution": name != BASE,
        })
    return observations


def verify_capture(spec, manifest):
    mode, pid, order = spec
    evidence_path = ROOT / "analysis" / ("ten-node-runtime-" + mode + ".json")
    directory = ROOT / "analysis" / "oracle" / ("ten-node-" + mode)
    run_path = directory / "run.json"
    evidence, run = read(evidence_path), read(run_path)
    check(Path(run["cwd"]) == directory and run["ordered_imports"] == order, "run path/order mismatch")
    exe = ROOT / "source" / "DEF_PRE_64.exe"
    check(Path(run["executable"]) == exe and
          run["executable_sha256"] == EXE_SHA == digest(exe), "executable identity mismatch")
    check(run["status"] == "completed" and run["exit_code"] == 0, "run did not complete")
    verify_files(run, directory)
    fixtures = {Path(c["path"]).name: c for c in manifest["cases"]}
    fixtures[BASE] = manifest["required_baseline"]
    check({i["name"] for i in run["inputs"]} == set(order)
          and len(run["inputs"]) == len(set(order)), "input set mismatch")
    for item in run["inputs"]:
        case = fixtures[item["name"]]
        source, copy = ROOT / case["path"], directory / item["name"]
        check(Path(item["source"]) == source and Path(item["copy"]) == copy, "fixture path mismatch")
        check(len({item["sha256_before"], item["sha256_after"], item["source_sha256_after"],
                   case["sha256"], digest(source), digest(copy)}) == 1, "fixture hash mismatch")
    cap = evidence["normalized_result"]
    script, gate = directory.with_suffix(".js"), directory.with_suffix(".ready")
    template = ROOT / "tools" / "observe_connectivity_rejection.js"
    check(Path(run["ready_gate"]) == gate and Path(cap["source_path"]) == script
          and digest(script) == cap["source_sha256"], "probe path/hash mismatch")
    text = template.read_text(encoding="utf-8")
    check(script.read_text(encoding="utf-8") == text.replace("__READY_GATE__", gate.as_posix()),
          "probe differs from passive template")
    name = fixtures[FOLLOW]["following_name"]
    observations = verify_events(evidence, run, spec, name)
    stdout = (directory / "stdout.bin").read_text(encoding="utf-8", errors="replace")
    diagnostics = [line.strip() for line in stdout.splitlines()
                   if "ERROR: Keyword ELMCON Reading Error." in line or "WARNING: Keyword 1 not found." in line]
    check(sum("ERROR:" in line for line in diagnostics) == 2
          and sum("WARNING:" in line for line in diagnostics) == 2,
          "expected observed rejection/unknown-token diagnostics missing")
    return {
        "scenario": mode, "pid": pid, "evidence": binding(evidence_path),
        "evidence_id": evidence["evidence_id"], "run_record": binding(run_path),
        "probe": binding(script), "probe_template": binding(template),
        "fixture_hashes": {n: fixtures[n]["sha256"] for n in sorted(set(order))},
        "diagnostics": diagnostics, "event_count": sum(len(i["events"]) for i in observations),
        "imports": observations,
    }, (evidence, run, spec, name)


def tampering_checks(data):
    def event(e, sequence):
        return e["normalized_result"]["messages"][sequence]["payload"]

    mutations = {
        "PID": lambda e: e["normalized_result"].update(pid=-1),
        "sequence": lambda e: event(e, 1).update(sequence=99),
        "handler_status": lambda e: event(e, 26).update(status=0),
        "coordinate": lambda e: event(e, 26)["state"]["objects"][0]["coordinates"].__setitem__(0, 99),
        "connectivity": lambda e: event(e, 26)["state"]["objects"][0]["connectivity"].__setitem__(0, 99),
        "coordinate_pointer": lambda e: event(e, 26)["state"]["objects"][0].update(coordinates_pointer="0x1234"),
        "connectivity_pointer": lambda e: event(e, 26)["state"]["objects"][0].update(connectivity_pointer="0x1234"),
        "unexpected_boundary": lambda e: event(e, 27).update(operation="boundary-enter"),
        "following_name": lambda e: event(e, 43)["state"]["objects"][0]["name_bytes"].__setitem__(0, 88),
        "missing_finalize": lambda e: event(e, 28).update(operation="import-after"),
        "truncation": lambda e: e["normalized_result"].update(messages_truncated=True),
    }
    results = []
    for name, mutate in mutations.items():
        evidence = deepcopy(data[0])
        mutate(evidence)
        try:
            verify_events(evidence, *data[1:])
        except ValueError as error:
            results.append({"mutation": name, "result": "REJECTED", "reason": str(error)})
        else:
            raise ValueError("tampering accepted: " + name)
    return results


def main():
    manifest_path = ROOT / "analysis" / "ten-node-evaluation-cases.json"
    manifest = read(manifest_path)
    check(manifest["target_sha256"] == EXE_SHA, "manifest target mismatch")
    runs, data = [], []
    for spec in SPECS:
        result, inputs = verify_capture(spec, manifest)
        runs.append(result)
        data.append(inputs)
    reachability = ROOT / "analysis" / "ten-node-reachability.json"
    check(read(reachability)["target_sha256"] == EXE_SHA, "reachability target mismatch")
    report = {
        "result": "PASS", "target_sha256": EXE_SHA,
        "method": "Verify saved passive original observations and provenance, without execution or replacement-generated state.",
        "fixture_manifest": binding(manifest_path), "static_reachability": binding(reachability),
        "process_runs": 2, "imports": 7, "ordered_events": sum(r["event_count"] for r in runs),
        "runs": runs, "tampering_checks": tampering_checks(data[0]),
        "findings": [
            "Both rejection files return ELMCON-handler status 2 and overall import status 2 in existing and initially fresh scenarios.",
            "Neither rejection file enters the observed 3D boundary wrapper; both still execute finalization.",
            "Existing mesh fields, coordinates, connectivity, dependent arrays, object identity and array pointers remain unchanged during rejection.",
            "The initially fresh rejected ELMCON creates an empty object 1 with kind 2/arity 4 before returning 2.",
            "The following OBJNAM handler succeeds and changes the name despite overall import status 2.",
            "The rejected connectivity row label is subsequently resolved as unknown keyword 1, corroborated by the original console warning.",
            "A later supported tetrahedron import returns 0 and enters the four-slot boundary path.",
        ],
        "limitations": [
            "These observations establish rejection of these two KEY arity 10 inputs, not global absence of ten-node support. Low-level arity 10 code and uninspected routes remain documented separately.",
            "The probe covers selected model fields, not complete model state or transactions.",
            "Absolute pointers are compared internally but omitted from this report. Name bytes after the first NUL are excluded because they are not meaningful string content.",
            "The existing run uses baseline, rejection 15, rejection 16, baseline; it does not reset the baseline immediately before 16 as originally proposed. The unchanged state after 15 is verified. The fresh run intentionally omits a baseline before rejection.",
            "State snapshots verify object creation at resolution before the rejected handler returns; the probe does not capture every constructor instruction.",
        ],
    }
    output = ROOT / "analysis" / "ten-node-observations.json"
    output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print("PASS: 2 original runs, 7 imports, 107 events; 11 tampering checks rejected")
    print(relative(output))


if __name__ == "__main__":
    main()
