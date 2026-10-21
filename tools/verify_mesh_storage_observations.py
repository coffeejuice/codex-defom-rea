"""Verify original integrated mesh-storage capture; no application execution."""
from copy import deepcopy
import json
from pathlib import Path

from capture_oracle import digest
from verify_boundary_sort_observations import (
    EXE_SHA, ROOT, binding, check, read, relative, verify_files,
)
from verify_mesh_array_observations import descriptor_map

ORDER = ["01_cube.KEY", "02_tetra.KEY", "03_partial_coordinates.KEY", "04_rotated_cube.KEY"]
STATUSES = [0, 0, 2, 0]
PID = 15532


def fixture_blocks(path):
    """Read only these fixture numeric blocks; not a replacement KEY scanner."""
    lines = path.read_text(encoding="ascii").splitlines()
    blocks, i = {}, 0
    while i < len(lines):
        fields = lines[i].split()
        i += 1
        if not fields or fields[0] not in ("RZ", "ELMCON"):
            continue
        kind = fields[0]
        count = int(fields[2])
        width = 3 if kind == "RZ" else int(fields[3])
        values = []
        for row in range(count):
            if i == len(lines):
                break
            items = lines[i].split()
            i += 1
            check(len(items) == width + 1 and int(items[0]) == row + 1, "fixture row shape/label")
            values.extend((float if kind == "RZ" else int)(v) for v in items[1:])
        complete = len(values) == count * width
        values += [0] * (count * width - len(values))
        blocks[kind] = {"count": count, "width": width, "values": values, "complete": complete}
    return blocks


def verify_capture_identity(manifest):
    evidence_path = ROOT / "analysis" / "mesh-storage-runtime-evaluation.json"
    directory = ROOT / "analysis" / "oracle" / "mesh-storage-evaluation"
    run_path = directory / "run.json"
    evidence, run = read(evidence_path), read(run_path)
    check(evidence["evidence_id"] == "ev_f41cf17c31fa0560969a5f34f94bae2301aef666957b5a65ca3cad267c4bf062",
          "unexpected evidence identity")
    exe = ROOT / "source" / "DEF_PRE_64.exe"
    check(Path(run["executable"]) == exe and
          run["executable_sha256"] == EXE_SHA == digest(exe), "executable identity mismatch")
    check(run["status"] == "completed" and run["exit_code"] == 0, "process did not complete")
    check(Path(run["cwd"]) == directory and run["ordered_imports"] == ORDER, "run path/order mismatch")
    verify_files(run, directory)
    cases = manifest["cases"]
    check([Path(c["path"]).name for c in cases] == ORDER and
          [i["name"] for i in run["inputs"]] == ORDER, "fixture manifest/input order mismatch")
    blocks = {}
    for item, case in zip(run["inputs"], cases):
        source, copy = ROOT / case["path"], directory / item["name"]
        check(Path(item["source"]) == source and Path(item["copy"]) == copy, "fixture path mismatch")
        check(len({case["sha256"], item["sha256_before"], item["sha256_after"],
                   item["source_sha256_after"], digest(source), digest(copy),
                   digest(ROOT / case["source"])}) == 1, "fixture/source hash mismatch")
        blocks[item["name"]] = fixture_blocks(source)
    cap = evidence["normalized_result"]
    script, gate = directory.with_suffix(".js"), directory.with_suffix(".ready")
    template = ROOT / "tools" / "observe_mesh_storage.js"
    check(Path(run["ready_gate"]) == gate and Path(cap["source_path"]) == script
          and digest(script) == cap["source_sha256"], "probe path/hash mismatch")
    text = template.read_text(encoding="utf-8")
    check(text.count("__READY_GATE__") == 2 and
          script.read_text(encoding="utf-8") == text.replace("__READY_GATE__", gate.as_posix()),
          "probe differs from passive template")
    return evidence, run, blocks, {
        "evidence": binding(evidence_path), "evidence_id": evidence["evidence_id"],
        "run_record": binding(run_path), "probe": binding(script), "probe_template": binding(template),
    }


def verify_events(evidence, run, blocks, rows):
    cap = evidence["normalized_result"]
    check(cap["pid"] == run["pid"] == evidence["parameters"]["pid"] == PID, "PID mismatch")
    check(cap["session_id"] == evidence["parameters"]["session_id"], "session mismatch")
    check(cap["mode"] == evidence["parameters"]["mode"] == "attach" and
          cap["device_id"] == evidence["parameters"]["device_id"] == "local"
          and cap["state"] == "detached" and cap["source_kind"] == "file", "capture lifecycle mismatch")
    check(cap["messages_truncated"] is False and cap["cleanup_error"] is None, "capture incomplete")
    scripts = [s["script_id"] for s in cap["scripts"]]
    messages = cap["messages"]
    check(len(scripts) == 1 and all(m["type"] == "send" and m["script_id"] == scripts[0]
                                  for m in messages), "script identity/probe error")
    first = messages[0]["payload"]
    check(first["operation"] == "hooks-installed" and first["pid"] == PID
          and int(first["base"], 16) > 0, "hook initialization mismatch")
    events = [m["payload"] for m in messages[1:]]
    check(len(events) == 15 and [e["sequence"] for e in events] == list(range(1, 16)),
          "missing/extra/noncontiguous events")
    offset, previous, address, identities, components = 0, None, None, None, None
    allocations = {"node": 0, "element": 0}
    snapshots, imports = [], []
    for name, status in zip(ORDER, STATUSES):
        fixture = blocks[name]
        operations = ["import-before", "nodes-replaced"]
        if "ELMCON" in fixture:
            operations.append("elements-replaced")
        operations.append("import-after")
        group = events[offset:offset + len(operations)]
        offset += len(operations)
        check([e["operation"] for e in group] == operations and
              all(e["file"] == name for e in group), "event operation/file order mismatch")
        check(group[-1]["status"] == status, "import status mismatch")
        for event in group[1:]:
            operation = event["operation"]
            if operation == "import-after":
                check(event["object_count"] == len(event["objects"]) == 1, "object traversal incomplete")
                obj = event["objects"][0]
            else:
                obj = event["object"]
            if address is None:
                address, components = obj["address"], obj["components"]
            check(obj["address"] == address and int(address, 16) > 0 and obj["id"] == 1,
                  "object identity changed")
            check(obj["dimension"] == 3 and obj["arity"] in (4, 8), "dimension/arity mismatch")
            check(obj["components"] == components and len(components) == 79
                  and components[0] == 3 and all(type(c) is int and c >= 0 for c in components),
                  "component table incomplete/changed")
            for field, length in [("coordinates", obj["nodes"] * components[0]),
                                  ("connectivity", obj["elements"] * obj["arity"])]:
                check((obj[field] is None if length == 0 else isinstance(obj[field], list)
                       and len(obj[field]) == length), "primary array shape/null mismatch")
            if operation == "nodes-replaced":
                allocations["node"] += 1
                check(event["requested_count"] == obj["nodes"] == fixture["RZ"]["count"]
                      and obj["coordinates"] == [0] * (obj["nodes"] * 3),
                      "coordinate replacement count/initialization mismatch")
                check(obj["elements"] == (previous["elements"] if previous else 0)
                      and obj["connectivity"] == (previous["connectivity"] if previous else None),
                      "node replacement changed existing connectivity")
            elif operation == "elements-replaced":
                allocations["element"] += 1
                block = fixture["ELMCON"]
                check(event["requested_count"] == obj["elements"] == block["count"]
                      and obj["arity"] == block["width"]
                      and obj["connectivity"] == [0] * (obj["elements"] * obj["arity"]),
                      "element replacement shape/initialization mismatch")
                check(obj["coordinates"] == fixture["RZ"]["values"], "coordinate row values mismatch")
            else:
                check(obj["coordinates"] == fixture["RZ"]["values"] and obj["mesh_flag"] == 1,
                      "final coordinate/mesh state mismatch")
                expected_connectivity = fixture["ELMCON"]["values"] if "ELMCON" in fixture else previous["connectivity"]
                check(obj["connectivity"] == expected_connectivity, "final connectivity mismatch")
            observed = obj["descriptors"]
            check(len(observed) == 53 and len({d["id"] for d in observed}) == 53
                  and len({d["slot_offset"] for d in observed}) == 53
                  and len({d["address"] for d in observed}) == 53, "descriptor traversal/uniqueness")
            current_ids = [d["address"] for d in observed]
            if identities is None:
                identities = current_ids
            check(current_ids == identities, "descriptor identity changed")
            normalized = []
            for position, (d, row) in enumerate(zip(observed, rows)):
                identifier, kind = row["state_id"], row["container"]
                check(d["id"] == identifier and d["slot_offset"] == int(row["slot_relative_to_object"], 16)
                      and d["vtable_rva"] == int(row["vtable_address"], 16) - 0x140000000,
                      "descriptor mapping/vtable mismatch")
                check(d["bound_pointer"] == d["storage_pointer"] and d["storage_references"] == 1,
                      "descriptor pointer binding/reference count mismatch")
                check(type(d["length"]) is int and 0 <= d["length"] <= 512
                      and d["length"] == d["count"] * d["width"] * d["multiplicity"],
                      "descriptor storage shape mismatch")
                check((d["values"] is None and int(d["storage_pointer"], 16) == 0)
                      if d["length"] == 0 else
                      (int(d["storage_pointer"], 16) > 0 and len(d["values"]) == d["length"]),
                      "descriptor null/value completeness mismatch")
                allocated = allocations[kind]
                shape = [d[k] for k in ("count", "width", "multiplicity", "old_count", "old_width", "old_multiplicity")]
                expected = [0, 1, 1, 0, 1, 1] if not row["selector_minus_one_included"] or not allocated else [
                    obj["nodes"] if kind == "node" else obj["elements"], components[identifier], 1,
                    0, 1 if allocated == 1 else components[identifier], 1]
                check(shape == expected, "descriptor shape/history mismatch")
                default = 20 if identifier == 8 else 1 if identifier == 44 else 0
                check(d["values"] is None or d["values"] == [default] * d["length"],
                      "descriptor fixture defaults changed")
                clean = {k: v for k, v in d.items() if k not in ("address", "storage_pointer", "bound_pointer")}
                clean.update(container=kind, storage_is_null=int(d["storage_pointer"], 16) == 0,
                             bound_pointer_matches_storage=True)
                unaffected = operation == "import-after" or (operation == "nodes-replaced" and kind == "element") or (
                    operation == "elements-replaced" and kind == "node")
                if previous is not None and unaffected:
                    check(clean == previous["descriptors"][position], "unaffected descriptor changed")
                normalized.append(clean)
            check(set(obj["direct"]) == {str(i) for i in range(18, 24)} | {"61"}, "direct buffer set mismatch")
            for key, values in obj["direct"].items():
                count = obj["elements"] if key == "61" else obj["nodes"]
                length = count * components[int(key)]
                check(values is None if length == 0 else values == [0] * length, "direct buffer shape/default mismatch")
            check(len(obj["boundary_groups"]) <= 100, "boundary group count out of range")
            total = 0
            for boundary in obj["boundary_groups"]:
                faces, owners = boundary["faces"], boundary["owners"]
                check(isinstance(faces, list) and isinstance(owners, list)
                      and len(faces) == len(owners) * 4 and len(faces) <= 512
                      and all(type(n) is int and n > 0 for n in faces + owners),
                      "boundary records incomplete")
                total += len(owners)
            check(obj["boundary_total"] == total, "boundary total mismatch")
            if operation != "import-after" or status == 2:
                check(obj["boundary_groups"] == (previous["boundary_groups"] if previous else [])
                      and obj["boundary_total"] == (previous["boundary_total"] if previous else 0),
                      "replacement/partial import changed retained boundaries")
            current = {k: v for k, v in obj.items() if k not in ("address", "descriptors")}
            current["descriptors"] = normalized
            snapshots.append({"file": name, "sequence": event["sequence"], "operation": operation,
                              "requested_count": event.get("requested_count"), "status": event.get("status"),
                              "object": current})
            previous = current
        imports.append({"file": name, "status": status, "sequence_range": [group[0]["sequence"], group[-1]["sequence"]]})
    check(offset == len(events) and len(snapshots) == 11, "snapshot count mismatch")
    return snapshots, imports


def tampering_checks(data):
    def obj(e, sequence=2):
        p = e["normalized_result"]["messages"][sequence]["payload"]
        return p["object"] if "object" in p else p["objects"][0]

    changes = {
        "PID": lambda e: e["normalized_result"].update(pid=-1),
        "status": lambda e: e["normalized_result"]["messages"][11]["payload"].update(status=0),
        "descriptor_missing": lambda e: obj(e)["descriptors"].pop(),
        "descriptor_duplicate": lambda e: obj(e)["descriptors"][1].update(id=1),
        "descriptor_slot": lambda e: obj(e)["descriptors"][0].update(slot_offset=0),
        "pointer_binding": lambda e: obj(e)["descriptors"][0].update(bound_pointer="0x1234"),
        "history": lambda e: obj(e)["descriptors"][0].update(old_width=99),
        "descriptor_value": lambda e: obj(e)["descriptors"][0]["values"].__setitem__(0, 1),
        "direct_buffer": lambda e: obj(e)["direct"]["18"].pop(),
        "boundary_owner": lambda e: obj(e, 4)["boundary_groups"][0]["owners"].pop(),
        "boundary_total": lambda e: obj(e, 4).update(boundary_total=99),
        "missing_event": lambda e: e["normalized_result"]["messages"].pop(),
    }
    results = []
    for name, change in changes.items():
        altered = deepcopy(data[0])
        change(altered)
        try:
            verify_events(altered, *data[1:])
        except ValueError as error:
            results.append({"mutation": name, "result": "REJECTED", "reason": str(error)})
        else:
            raise ValueError("tampering accepted: " + name)
    return results


def main():
    manifest_path = ROOT / "analysis" / "mesh-storage-evaluation-cases.json"
    contract_path = ROOT / "analysis" / "mesh-array-contract.json"
    manifest, contract = read(manifest_path), read(contract_path)
    rows = descriptor_map(contract)
    evidence, run, blocks, provenance = verify_capture_identity(manifest)
    snapshots, imports = verify_events(evidence, run, blocks, rows)
    freeze_path = ROOT / "analysis" / "mesh-storage-candidate-freeze.json"
    freeze = read(freeze_path)
    current = []
    for item in freeze["files"]:
        path = ROOT / item["path"]
        sha = digest(path) if path.is_file() else None
        current.append(dict(item, current_sha256=sha, current_matches=sha == item["sha256"]))
    comparator = ROOT / "tools" / "compare_mesh_storage.py"
    report = {
        "result": "PASS", "target_sha256": EXE_SHA,
        "method": "Independent read-only original provenance, descriptor-contract and storage-consistency checks. No application execution or replacement-generated expected state.",
        "provenance": provenance, "fixture_manifest": binding(manifest_path),
        "descriptor_contract": binding(contract_path),
        "process_runs": 1, "imports": imports, "ordered_events": 15,
        "object_snapshots": 11, "descriptors_per_snapshot": 53, "descriptor_observations": 583,
        "snapshots": snapshots,
        "candidate_freeze": {"record": binding(freeze_path), "declared_stage": freeze["stage"],
                             "files": current, "qualification": "Current matches are informational, separate from original provenance; local stage wording is not a trusted timestamp."},
        "comparator_review": {
            "source": binding(comparator),
            "assessment": "No demonstrated value-comparison defect for this bounded corpus; two material scope gaps require separate qualification.",
            "gaps": [
                {"topic": "Import status", "finding": "Comparator does not check import-after status; its exact verdict compares retained state only. It cannot establish parser return-code compatibility.", "coverage_here": "Original status sequence 0,0,2,0 is explicitly checked."},
                {"topic": "Allocation identity", "finding": "Null values become empty arrays, and pointer binding, reference count, slot offset and vtable identity are omitted from replacement comparison.", "coverage_here": "Original nullness, binding, reference count and all 53 static mappings are checked; replacement pointer/allocation equivalence remains unclaimed."},
                {"topic": "Fixture reader", "finding": "Replay uses a limited numeric-row reader and hardcoded default control arguments, not the original keyword scanner or full importer.", "coverage_here": "Only the exact four bound 3D fixtures and default component controls are validated."},
            ],
        },
        "tampering_checks": tampering_checks((evidence, run, blocks, rows)),
        "limitations": [
            "New sequence combines previously used fixtures; individual inputs are not independently unseen.",
            "Intermediate allocation snapshots intentionally retain old connectivity/boundaries; their node IDs need not fit the newly replaced coordinate count.",
            "All pointers are compared internally where captured and omitted from normalized output.",
            "Original buffer shape/value completeness is checked, but pointer identity is available only for descriptors; direct buffer nullness is retained.",
            "Full importer, arbitrary parsing, 2D allocation, nondefault multiplicities/units, shared storage, overflow and allocation failure remain outside scope.",
            "Independent original validation does not itself establish replacement equivalence; use the separately scoped comparator.",
        ],
    }
    output = ROOT / "analysis" / "mesh-storage-observations.json"
    output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print("PASS: 4 imports, 11 snapshots, 583 descriptor observations; 12 tampering checks rejected")
    print(relative(output))


if __name__ == "__main__":
    main()
