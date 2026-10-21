"""Validate retained original mesh-array observations and emit a reusable corpus.

This does not launch the original or validate a replacement. The assertions are
bounded to the two captured 3D fixture sequences; static branches not exercised
by those sequences remain outside the runtime claim.
"""
from __future__ import annotations

import json
from pathlib import Path

from capture_oracle import digest

ROOT = Path(__file__).resolve().parents[1]
EXE_SHA = "f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794"
CONTRACT = ROOT / "analysis" / "mesh-array-contract.json"
PROBE = ROOT / "tools" / "observe_mesh_arrays.js"
ORDERS = {
    1: ["07_tetrahedron.KEY", "08_relabelled_coordinates.KEY",
        "09_truncated_coordinates.KEY", "07_tetrahedron.KEY"],
    2: ["07_tetrahedron.KEY", "13_two_tetrahedra.KEY", "07_tetrahedron.KEY"],
}
TETRA = [0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1]
FIXTURES = {
    "07_tetrahedron.KEY": (TETRA, [1, 2, 3, 4], 0),
    "08_relabelled_coordinates.KEY":
        ([10, 0, 0, 11, 0, 0, 10, 1, 0, 10, 0, 1], [1, 2, 3, 4], 0),
    "09_truncated_coordinates.KEY":
        ([20, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 0], [1, 2, 3, 4], 2),
    "13_two_tetrahedra.KEY": (TETRA + [1, 1, 1], [1, 2, 3, 4, 2, 3, 4, 5], 0),
}
STATE_FIELDS = (
    "count", "width", "multiplicity", "old_count", "old_width",
    "old_multiplicity", "length", "values",
)


def check(condition: bool, message: str) -> None:
    if not condition:
        raise ValueError(message)


def read(path: Path):
    return json.loads(path.read_text(encoding="utf-8-sig"))


def relative(path: Path) -> str:
    return path.resolve().relative_to(ROOT).as_posix()


def descriptor_map(contract: dict) -> list[dict]:
    check(contract["target"]["sha256"] == EXE_SHA, "static target mismatch")
    for entry in contract["evidence"].values():
        path = ROOT / entry["file"]
        evidence = read(path)
        check(digest(path) == entry["file_sha256"] and
              evidence["evidence_id"] == entry["evidence_id"] and
              evidence["subject"]["digest"]["sha256"] == EXE_SHA,
              f"static evidence identity mismatch: {path}")
    table = contract["vtable_evidence"]
    check(digest(ROOT / table["file"]) == table["sha256"],
          "vtable evidence changed")
    result = []
    for kind, count in (("node", 28), ("element", 25)):
        rows = contract["containers"][kind]["descriptors"]
        check(len(rows) == count, f"incomplete {kind} mapping")
        for row in rows:
            result.append({**row, "container": kind})
    result.sort(key=lambda row: int(row["slot_relative_to_object"], 16))
    check(len({r["state_id"] for r in result}) == 53, "duplicate descriptor ID")
    return result


def verify_identity(number: int) -> tuple[dict, dict, list[dict], dict]:
    evidence_path = ROOT / "analysis" / f"mesh-arrays-runtime-{number}.json"
    run_path = ROOT / "analysis" / "oracle" / f"mesh-arrays-{number}" / "run.json"
    evidence, run = read(evidence_path), read(run_path)
    capture = evidence["normalized_result"]
    check(Path(run["executable"]) == ROOT / "source" / "DEF_PRE_64.exe" and
          run["executable_sha256"] == EXE_SHA == digest(Path(run["executable"])),
          "executable identity mismatch")
    check(run["status"] == "completed" and run["exit_code"] == 0,
          "original process did not complete normally")
    check(capture["pid"] == evidence["parameters"]["pid"] == run["pid"],
          "PID mismatch")
    check(capture["session_id"] == evidence["parameters"]["session_id"],
          "capture session mismatch")
    check(capture["mode"] == "attach" and capture["state"] == "detached",
          "unexpected capture lifecycle")
    check(capture["messages_truncated"] is False and capture["cleanup_error"] is None,
          "truncated capture or cleanup error")
    script = Path(capture["source_path"])
    check(script == ROOT / "analysis" / "oracle" / f"mesh-arrays-{number}.js",
          "unexpected probe path")
    check(digest(script) == capture["source_sha256"], "probe hash mismatch")
    expected_probe = PROBE.read_text(encoding="utf-8-sig").replace(
        "__READY_GATE__", Path(run["ready_gate"]).as_posix())
    check(script.read_text(encoding="utf-8-sig") == expected_probe,
          "generated probe differs from current passive template")
    check(run["ordered_imports"] == ORDERS[number], "manifest import order mismatch")
    check(Path(run["cwd"]) == run_path.parent, "working directory mismatch")
    check((run_path.parent / "stdin.bin").read_bytes() == run["stdin"].encode("ascii"),
          "saved transcript mismatch")
    check((run_path.parent / "stderr.bin").read_bytes() == b"",
          "original stderr is nonempty")
    for item in run["files"]:
        path = run_path.parent / item["name"]
        check(path.stat().st_size == item["bytes"] and digest(path) == item["sha256"],
              f"recorded file identity mismatch: {path}")
    fixture_hashes = {}
    for item in run["inputs"]:
        source, copy = Path(item["source"]), Path(item["copy"])
        check(source == ROOT / "tests" / "fixtures" / "keywords" / item["name"] and
              copy == run_path.parent / item["name"], "fixture path mismatch")
        hashes = {item["sha256_before"], item["sha256_after"],
                  item["source_sha256_after"], digest(source), digest(copy)}
        check(len(hashes) == 1, f"fixture changed: {item['name']}")
        fixture_hashes[item["name"]] = item["sha256_before"]
    check(set(fixture_hashes) == set(ORDERS[number]), "fixture set mismatch")
    scripts = {item["script_id"] for item in capture["scripts"]}
    messages = capture["messages"]
    check(len(scripts) == 1 and all(m["script_id"] in scripts for m in messages),
          "message/script mismatch")
    check(all(m["type"] == "send" for m in messages), "script error message")
    payloads = [m["payload"] for m in messages]
    check(payloads[0]["operation"] == "hooks-installed" and
          payloads[0]["pid"] == run["pid"], "missing initial hook/PID record")
    events = payloads[1:]
    check(all(e["operation"] in {"import-before", "nodes-replaced",
                                "elements-replaced", "import-after"} for e in events),
          "probe error or unknown operation")
    check([e["sequence"] for e in events] == list(range(1, len(events) + 1)),
          "event sequence gap")
    metadata = {
        "run": number, "evidence_id": evidence["evidence_id"],
        "evidence_file": relative(evidence_path), "evidence_sha256": digest(evidence_path),
        "run_record": relative(run_path), "run_record_sha256": digest(run_path),
        "pid": run["pid"], "probe_source": relative(script),
        "probe_sha256": capture["source_sha256"], "probe_template": relative(PROBE),
        "probe_template_sha256": digest(PROBE), "fixture_hashes": fixture_hashes,
        "event_count": len(events),
    }
    return metadata, payloads[0], events, run


def verify_run(number: int, rows: list[dict]) -> dict:
    metadata, hooks, events, _ = verify_identity(number)
    check(int(hooks["base"], 16) > 0, "invalid loaded module base")
    reference_image_base = 0x140000000
    groups, active = [], None
    for event in events:
        if event["operation"] == "import-before":
            check(active is None, "nested or unclosed import")
            active = []
        check(active is not None, "event outside import")
        active.append(event)
        if event["operation"] == "import-after":
            groups.append(active)
            active = None
    check(active is None and len(groups) == len(ORDERS[number]),
          "incomplete import boundaries")
    check([g[0]["file"] for g in groups] == ORDERS[number], "capture import order mismatch")
    previous = None
    object_address = None
    descriptor_addresses = None
    components = None
    node_replacements = element_replacements = 0
    snapshots, imports = [], []
    for ordinal, group in enumerate(groups, 1):
        name = group[0]["file"]
        expected_coordinates, expected_connectivity, expected_status = FIXTURES[name]
        expected_ops = ["import-before", "nodes-replaced"]
        if expected_status == 0:
            expected_ops.append("elements-replaced")
        expected_ops.append("import-after")
        check([e["operation"] for e in group] == expected_ops and
              all(e["file"] == name for e in group), "unexpected fixture event order")
        check(group[-1]["status"] == expected_status, "import status mismatch")
        for event in group[1:]:
            operation = event["operation"]
            if operation == "import-after":
                check(event["object_count"] == len(event["objects"]) == 1,
                      "incomplete object traversal")
                obj = event["objects"][0]
            else:
                obj = event["object"]
            if object_address is None:
                object_address = obj["address"]
            check(obj["address"] == object_address and obj["id"] == 1,
                  "object identity changed")
            check(obj["dimension"] == 3 and obj["arity"] == 4,
                  "unexpected dimension/connectivity width in bounded fixtures")
            if components is None:
                components = obj["components"]
            check(obj["components"] == components and len(components) == 79 and
                  all(isinstance(c, int) and c >= 0 for c in components),
                  "component table changed or incomplete")
            observed = obj["descriptors"]
            check(len(observed) == 53, "descriptor traversal incomplete")
            addresses = [d["address"] for d in observed]
            check(len(set(addresses)) == 53, "duplicate descriptor address")
            if descriptor_addresses is None:
                descriptor_addresses = addresses
            check(addresses == descriptor_addresses, "descriptor identities changed")
            if operation == "nodes-replaced":
                check(event["requested_count"] == obj["nodes"] ==
                      len(expected_coordinates) // 3, "node replacement count mismatch")
                check(obj["coordinates"] == [0] * (obj["nodes"] * 3),
                      "new coordinate storage is not zero before parsing")
                check(obj["elements"] == (previous["elements"] if previous else 0) and
                      obj["connectivity"] == (previous["connectivity"] if previous else None),
                      "node replacement changed connectivity")
                node_replacements += 1
            elif operation == "elements-replaced":
                check(event["requested_count"] == obj["elements"] ==
                      len(expected_connectivity) // 4, "element replacement count mismatch")
                check(obj["coordinates"] == expected_coordinates and
                      obj["connectivity"] == [0] * (obj["elements"] * 4),
                      "element replacement/coordinate parsing mismatch")
                element_replacements += 1
            else:
                check(obj["coordinates"] == expected_coordinates and
                      obj["connectivity"] == expected_connectivity and obj["mesh_flag"] == 1,
                      "final mesh or partial-error state mismatch")
            normalized_descriptors = []
            for index, (desc, row) in enumerate(zip(observed, rows)):
                identifier = row["state_id"]
                check(desc["id"] == identifier and
                      desc["slot_offset"] == int(row["slot_relative_to_object"], 16) and
                      desc["vtable_rva"] ==
                      int(row["vtable_address"], 16) - reference_image_base,
                      "runtime descriptor ID/slot/vtable differs from static mapping")
                check(desc["storage_pointer"] == desc["bound_pointer"],
                      "bound pointer differs from descriptor storage")
                check(desc["storage_references"] == 1, "unexpected shared storage in fixture")
                check(desc["length"] == desc["count"] * desc["width"] * desc["multiplicity"],
                      "descriptor storage length differs from dimensions")
                if desc["length"] == 0:
                    check(desc["values"] is None and desc["storage_pointer"] == "0x0",
                          "expected empty null storage")
                else:
                    check(desc["storage_pointer"] != "0x0" and
                          len(desc["values"]) == desc["length"],
                          "incomplete nonempty array capture")
                post = not row["selector_minus_one_included"]
                allocated = node_replacements if row["container"] == "node" else element_replacements
                expected_count = obj["nodes"] if row["container"] == "node" else obj["elements"]
                if post or allocated == 0:
                    check([desc[k] for k in ("count", "width", "multiplicity",
                                             "old_count", "old_width", "old_multiplicity")] ==
                          [0, 1, 1, 0, 1, 1], "unallocated descriptor defaults changed")
                else:
                    check(desc["count"] == expected_count and desc["width"] == components[identifier]
                          and desc["multiplicity"] == 1 and desc["old_count"] == 0 and
                          desc["old_width"] == (1 if allocated == 1 else components[identifier])
                          and desc["old_multiplicity"] == 1,
                          "descriptor allocation shape/history mismatch")
                default = 20 if identifier == 8 else 1 if identifier == 44 else 0
                if desc["values"] is not None:
                    check(desc["values"] == [default] * desc["length"],
                          f"fixture default values differ for descriptor {identifier}")
                normalized = {k: desc[k] for k in STATE_FIELDS}
                normalized.update({
                    "id": identifier, "container": row["container"],
                    "slot_offset": desc["slot_offset"],
                    "recovered_class_name": row["recovered_class_name"],
                    "storage_references": desc["storage_references"],
                    "storage_is_null": desc["storage_pointer"] == "0x0",
                    "bound_pointer_matches_storage": True,
                })
                if previous is not None:
                    unaffected = operation == "import-after" or (
                        operation == "nodes-replaced" and row["container"] == "element") or (
                        operation == "elements-replaced" and row["container"] == "node")
                    if unaffected:
                        check(normalized == previous["descriptors"][index],
                              "unaffected descriptor changed")
                normalized_descriptors.append(normalized)
            current = {k: obj[k] for k in (
                "id", "dimension", "mesh_flag", "nodes", "elements", "arity",
                "components", "coordinates", "connectivity")}
            current["descriptors"] = normalized_descriptors
            snapshot = {
                "run": number, "import_ordinal": ordinal, "file": name,
                "sequence": event["sequence"], "operation": operation,
                "requested_count": event.get("requested_count"),
                "status": event.get("status"), "object": current,
            }
            snapshots.append(snapshot)
            previous = current
        imports.append({
            "ordinal": ordinal, "file": name, "status": expected_status,
            "sequence_range": [group[0]["sequence"], group[-1]["sequence"]],
            "final_nodes": previous["nodes"], "final_elements": previous["elements"],
        })
    return {**metadata, "imports": imports, "snapshots": snapshots}


def main() -> None:
    contract = read(CONTRACT)
    rows = descriptor_map(contract)
    runs = [verify_run(number, rows) for number in (1, 2)]
    snapshot_count = sum(len(run["snapshots"]) for run in runs)
    report = {
        "target": "source/DEF_PRE_64.exe", "target_sha256": EXE_SHA, "result": "PASS",
        "method": "Validate retained passive original-process evidence, static descriptor mapping and fixture identities.",
        "contract": relative(CONTRACT), "contract_sha256": digest(CONTRACT),
        "process_runs": 2, "imports": 7, "object_snapshots": snapshot_count,
        "descriptors_per_snapshot": 53, "descriptor_observations": snapshot_count * 53,
        "normalization": [
            "Object, descriptor and storage addresses are omitted; within-run identity and bound/storage pointer equality are checked before normalization.",
            "All observed descriptor dimensions, previous-dimension fields, values and null-storage state are retained for replacement comparison.",
            "Raw scientific names remain unassigned; recovered class names and object-relative slot offsets identify fields.",
            "Floating values in these fixtures are exactly representable; this report establishes no general floating-point tolerance.",
        ],
        "assertions": [
            "All 28 node and 25 element descriptor IDs, bound slots and allocation vtables match the static mapping.",
            "Every retained array length equals count * width * multiplicity, with complete bounded value capture.",
            "Bulk replacement leaves post-only node IDs 30..33 unallocated.",
            "Repeated and changed-count replacements update ordinary descriptors and preserve descriptor identity.",
            "Node replacement preserves element descriptor state; element replacement preserves node descriptor state.",
            "New coordinates/connectivity are zero before row parsing; node ID 8 defaults to 20 and element ID 44 to 1 in these fixtures.",
            "The truncated coordinate import returns 2, retains parsed prefix plus zeros and preserves connectivity.",
            "The second run grows from 4 nodes/1 element to 5 nodes/2 elements and shrinks back.",
        ],
        "scope": [
            "Seven sequential imports across two fresh original 3D processes; one object per snapshot.",
            "No 2D, alternate units, targeted post-variable allocation, nonunit extra multiplicity or nine-component MATAXI branch is exercised.",
            "Reference-count sharing, allocation failures, overflow and arbitrary malformed input remain unvalidated.",
            "Probe starts before fixture input, but this harness does not claim suspended-at-creation startup observation.",
            "This verifier does not execute the original or establish replacement equivalence; the corpus supports a separate comparison.",
        ],
        "runs": runs,
    }
    path = ROOT / "analysis" / "mesh-array-observations.json"
    path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: 2 bound process captures, 7 imports, {snapshot_count} snapshots, "
          f"{snapshot_count * 53} descriptor observations")
    print(relative(path))


if __name__ == "__main__":
    main()
