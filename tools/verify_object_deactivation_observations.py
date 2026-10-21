"""Validate the bounded populated-object deactivation capture without execution."""
from copy import deepcopy
import json
from pathlib import Path

from capture_oracle import digest
from verify_boundary_sort_observations import EXE_SHA, ROOT, binding, check, read, verify_files
from verify_mesh_array_observations import descriptor_map
from verify_mesh_storage_observations import fixture_blocks

ORDER = ["01_cube.KEY", "02_inactive.KEY", "03_keep_inactive.KEY", "04_reactivate.KEY"]
PATTERN = [
    ("import-before", 0), ("nodes-replaced", 0), ("elements-replaced", 0), ("import-after", 0),
    ("import-before", 1), ("object-deactivated", 1), ("import-after", 1),
    ("import-before", 2), ("object-deactivated", 2), ("import-after", 2),
    ("import-before", 3), ("import-after", 3),
]


def provenance(evidence, run, manifest):
    directory = ROOT / "analysis/oracle/object-deactivation"
    check(run["status"] == "completed" and run["exit_code"] == 0, "original did not complete")
    check(Path(run["cwd"]) == directory and run["ordered_imports"] == ORDER, "run order/path")
    check(Path(run["executable"]) == ROOT / "source/DEF_PRE_64.exe"
          and run["executable_sha256"] == digest(Path(run["executable"])) == EXE_SHA,
          "original identity mismatch")
    verify_files(run, directory)
    check((directory / "stdout.bin").stat().st_size < run["max_output_bytes"], "output bound")
    check([Path(c["path"]).name for c in manifest["cases"]] == ORDER
          and [i["name"] for i in run["inputs"]] == ORDER, "fixture manifest/order")
    for case, item in zip(manifest["cases"], run["inputs"]):
        source, copy = ROOT / case["path"], directory / item["name"]
        check(Path(item["source"]) == source and Path(item["copy"]) == copy, "fixture paths")
        check(case["sha256"] == digest(source) == digest(copy) == item["sha256_before"]
              == item["sha256_after"] == item["source_sha256_after"], "fixture hashes")
    cap = evidence["normalized_result"]
    template = ROOT / "tools/observe_object_deactivation.js"
    script, gate = directory.with_suffix(".js"), directory.with_suffix(".ready")
    check(Path(cap["source_path"]) == script and digest(script) == cap["source_sha256"]
          and Path(run["ready_gate"]) == gate, "probe path/hash")
    text = template.read_text(encoding="utf-8")
    check(text.count("__READY_GATE__") == 2 and script.read_text(encoding="utf-8")
          == text.replace("__READY_GATE__", gate.as_posix()), "passive template mismatch")


def verify(evidence, run, rows, blocks):
    cap, params = evidence["normalized_result"], evidence["parameters"]
    check(cap["pid"] == params["pid"] == run["pid"] == 8472, "PID mismatch")
    check(cap["target"] == "DEF_PRE_64.exe (pid 8472)"
          and cap["session_id"] == params["session_id"], "target/session mismatch")
    check(cap["state"] == "detached" and cap["source_kind"] == "file"
          and cap["mode"] == params["mode"] == "attach"
          and cap["device_id"] == params["device_id"] == "local", "capture lifecycle")
    check(cap["messages_truncated"] is False and cap["cleanup_error"] is None, "incomplete capture")
    messages, scripts = cap["messages"], cap["scripts"]
    check(len(scripts) == 1 and len(messages) == 13
          and all(m["type"] == "send" and m["script_id"] == scripts[0]["script_id"] for m in messages),
          "script/messages mismatch")
    hook = messages[0]["payload"]
    check(hook["operation"] == "hooks-installed" and hook["pid"] == 8472
          and int(hook["base"], 16) > 0, "hook mismatch")
    events = [m["payload"] for m in messages[1:]]
    check([e["sequence"] for e in events] == list(range(1, 13)), "event sequence mismatch")
    check([(e["operation"], e["file"]) for e in events]
          == [(op, ORDER[i]) for op, i in PATTERN], "event order mismatch")
    snapshots, previous, identities, address, components = [], None, None, None, None
    for event in events:
        op, seq = event["operation"], event["sequence"]
        if op == "import-before":
            continue
        if op == "import-after":
            check(event["status"] == 0 and event["object_count"] == len(event["objects"]) == 1,
                  "import status/object list")
            obj = event["objects"][0]
        else:
            obj = event["object"]
        if previous is None:
            address, components = obj["address"], obj["components"]
        check(int(address, 16) > 0 and obj["address"] == address and obj["id"] == 1,
              "object identity changed")
        check(obj["components"] == components and len(components) == 79 and components[0] == 3
              and all(type(n) is int and n >= 0 for n in components), "component table mismatch")
        check(obj["dimension"] == 3 and obj["arity"] == (4 if seq == 2 else 8)
              and obj["element_kind"] == (2 if seq == 2 else 4), "retained geometry metadata")
        check(obj["type"] == (7 if 6 <= seq <= 10 else 1)
              and obj["secondary"] == (0 if seq <= 4 else 37 if seq <= 7 else 41 if seq <= 10 else 43),
              "type/secondary mismatch")
        expected_name = ("REA 05_unit_hexahedron" if seq <= 4 else
                         "Reactivated without mesh" if seq == 12 else "")
        check(obj["name"] == expected_name, "name mismatch")
        check(obj["nodes"] == (8 if seq <= 4 else 0)
              and obj["elements"] == (1 if seq in (3, 4) else 0)
              and obj["mesh_flag"] == int(seq == 4), "mesh count/flag mismatch")
        if seq == 2:
            check(event["requested_count"] == 8 and obj["coordinates"] == [0] * 24
                  and obj["connectivity"] is None, "node allocation snapshot")
        elif seq in (3, 4):
            check(obj["coordinates"] == blocks["RZ"]["values"], "coordinate fixture mismatch")
            check(obj["connectivity"] == ([0] * 8 if seq == 3 else blocks["ELMCON"]["values"]),
                  "connectivity fixture mismatch")
            if seq == 3:
                check(event["requested_count"] == 1, "element allocation count")
        else:
            check(obj["coordinates"] is None and obj["connectivity"] is None, "primary buffers retained")
        check(set(obj["direct"]) == {str(i) for i in range(18, 24)} | {"61"}, "direct mapping")
        for key, values in obj["direct"].items():
            length = (obj["elements"] if key == "61" else obj["nodes"]) * components[int(key)]
            check(values == [0] * length if length else values is None, "direct buffer state")
        ds = obj["descriptors"]
        check(len(ds) == len({d["id"] for d in ds}) == len({d["address"] for d in ds})
              == len({d["slot_offset"] for d in ds}) == 53, "descriptor completeness")
        ids = [d["address"] for d in ds]
        identities = ids if identities is None else identities
        check(ids == identities and all(int(p, 16) > 0 for p in ids), "descriptor identity changed")
        clean_ds = []
        for index, (d, row) in enumerate(zip(ds, rows)):
            sid = row["state_id"]
            check(d["id"] == sid and d["slot_offset"] == int(row["slot_relative_to_object"], 16)
                  and d["vtable_rva"] == int(row["vtable_address"], 16) - 0x140000000,
                  "descriptor static mapping")
            check(d["storage_references"] == 1 and d["bound_pointer"] == d["storage_pointer"],
                  "storage reference/binding")
            check(type(d["length"]) is int and 0 <= d["length"] <= 512
                  and d["length"] == d["count"] * d["width"] * d["multiplicity"], "storage shape")
            check((d["values"] is None and int(d["storage_pointer"], 16) == 0) if not d["length"]
                  else (int(d["storage_pointer"], 16) > 0 and len(d["values"]) == d["length"]),
                  "storage null/values")
            if seq <= 4:
                allocated = row["selector_minus_one_included"] and (row["container"] == "node" or seq >= 3)
                shape = [8 if row["container"] == "node" else 1, components[sid], 1, 0, 1, 1] if allocated else [0, 1, 1, 0, 1, 1]
                check([d[k] for k in ("count", "width", "multiplicity", "old_count", "old_width", "old_multiplicity")]
                      == shape, "initial shape/history")
                default = 20 if sid == 8 else 1 if sid == 44 else 0
                check(d["values"] is None or d["values"] == [default] * d["length"], "initial defaults")
            else:
                prior = previous["descriptors"][index]
                check(d["count"] == d["length"] == 0, "deactivation count not cleared")
                check(all(d[k] == prior[k] for k in ("width", "multiplicity", "old_width", "old_multiplicity")),
                      "deactivation reset width/history")
                check(d["old_count"] == (prior["count"] if op == "object-deactivated" else prior["old_count"]),
                      "clear old_count transition")
            clean_ds.append({k: v for k, v in d.items()
                             if k not in ("address", "storage_pointer", "bound_pointer")})
        groups = obj["boundary_groups"]
        if seq < 4:
            check(groups == [] and obj["boundary_total"] == 0, "premature boundaries")
        elif seq == 4:
            check(len(groups) == 1 and obj["boundary_total"] == 6
                  and len(groups[0]["faces"]) == 24 and groups[0]["owners"] == [1] * 6
                  and all(type(n) is int and 1 <= n <= 8 for n in groups[0]["faces"]),
                  "cube boundary record shape")
        else:
            check(groups == [{"faces": None, "owners": None}] and obj["boundary_total"] == 0,
                  "retained group count/cleared payload mismatch")
        current = {k: v for k, v in obj.items() if k not in ("address", "descriptors")}
        current["descriptors"] = clean_ds
        if seq in (7, 10):
            check(current == previous, "post-deactivation finalization changed observed state")
        snapshots.append({"sequence": seq, "file": event["file"], "operation": op, "object": current})
        previous = current
    check(len(snapshots) == 8, "snapshot count")
    return snapshots


def main():
    capture_path = ROOT / "analysis/object-deactivation-runtime.json"
    run_path = ROOT / "analysis/oracle/object-deactivation/run.json"
    manifest_path = ROOT / "analysis/object-deactivation-evaluation-cases.json"
    evidence, run, manifest = read(capture_path), read(run_path), read(manifest_path)
    provenance(evidence, run, manifest)
    contract = ROOT / "analysis/mesh-array-contract.json"
    rows = descriptor_map(read(contract))
    blocks = fixture_blocks(ROOT / manifest["cases"][0]["path"])
    snapshots = verify(evidence, run, rows, blocks)
    def obj(e, seq):
        payload = e["normalized_result"]["messages"][seq]["payload"]
        return payload.get("object") or payload["objects"][0]
    mutations = {
        "PID": lambda e: e["normalized_result"].update(pid=1),
        "missing event": lambda e: e["normalized_result"]["messages"].pop(),
        "wrong import status": lambda e: e["normalized_result"]["messages"][7]["payload"].update(status=2),
        "missing descriptor": lambda e: obj(e, 6)["descriptors"].pop(),
        "changed descriptor slot": lambda e: obj(e, 6)["descriptors"][0].update(slot_offset=0),
        "lost descriptor history": lambda e: obj(e, 6)["descriptors"][0].update(old_count=0),
        "incorrect repeated clear history": lambda e: obj(e, 9)["descriptors"][0].update(old_count=8),
        "reset width": lambda e: obj(e, 6)["descriptors"][0].update(width=1),
        "non-null cleared storage": lambda e: obj(e, 6)["descriptors"][0].update(storage_pointer="0x1234"),
        "reset selected arity": lambda e: obj(e, 6).update(arity=4),
        "lost secondary": lambda e: obj(e, 6).update(secondary=0),
        "removed boundary group": lambda e: obj(e, 6).update(boundary_groups=[]),
        "retained primary buffer": lambda e: obj(e, 6).update(coordinates=[]),
        "retained direct buffer": lambda e: obj(e, 6)["direct"].update({"18": []}),
        "lost reactivation name": lambda e: obj(e, 12).update(name=""),
    }
    tests = []
    for label, mutate in mutations.items():
        altered = deepcopy(evidence)
        mutate(altered)
        try:
            verify(altered, run, rows, blocks)
        except ValueError as error:
            tests.append({"mutation": label, "rejected": True, "reason": str(error)})
        else:
            raise ValueError("accepted tamper: " + label)
    for label, mutate in [
        ("executable hash", lambda e, r: r.update(executable_sha256="0" * 64)),
        ("probe hash", lambda e, r: e["normalized_result"].update(source_sha256="0" * 64)),
        ("fixture hash", lambda e, r: r["inputs"][0].update(sha256_after="0" * 64)),
        ("transcript hash", lambda e, r: next(f for f in r["files"] if f["name"] == "stdout.bin").update(sha256="0" * 64)),
    ]:
        altered, badrun = deepcopy(evidence), deepcopy(run)
        mutate(altered, badrun)
        try:
            provenance(altered, badrun, manifest)
        except ValueError as error:
            tests.append({"mutation": label, "rejected": True, "reason": str(error)})
        else:
            raise ValueError("accepted provenance tamper: " + label)
    report = {
        "status": "pass", "scope": "One original 3D populated cube -> invalid type -> code6 while inactive -> reactivate/name sequence.",
        "verifier": binding(Path(__file__)), "capture": binding(capture_path),
        "evidence_id": evidence["evidence_id"], "pid": 8472,
        "run_record": binding(run_path), "fixture_manifest": binding(manifest_path),
        "probe": binding(ROOT / "analysis/oracle/object-deactivation.js"),
        "template": binding(ROOT / "tools/observe_object_deactivation.js"),
        "descriptor_contract": binding(contract),
        "imports": [{"file": name, "status": 0} for name in ORDER],
        "messages_including_hook": 13, "sequenced_events": 12, "snapshot_count": 8,
        "descriptor_observations": 424, "snapshots": snapshots, "tampering_checks": tests,
        "confirmed": [
            "Existing object identity and all 53 descriptor identities survive both deactivations and reactivation.",
            "Primary/direct buffers and descriptor storage become null; counts and mesh flag clear.",
            "First clear copies populated descriptor count to old_count; second clear changes old_count to zero. Width, multiplicity and previous-width/multiplicity fields remain unchanged.",
            "Selected element kind4 and arity8 survive cleanup and type reactivation.",
            "Boundary group count remains1 while its face/owner payloads are null and total becomes0.",
            "Deactivation sets type7 and empty logical name; supplied secondary37 then41 survive cleanup and subsequent import finalization.",
            "Reactivation sets type1/secondary43 and the following name without restoring mesh allocations or resetting retained metadata.",
        ],
        "limitations": [
            "One 3D object, one populated hexahedron, one active boundary record and normal returns; not all types, optional extensions, dimensions or cleanup failures.",
            "Boundary chain payloads, inactive records2..100, face metadata+898/+89c, linked-list links and owner pointer are not directly captured.",
            "Name is read through NUL; full buffer bytes and one-byte clearing are established only by the separate static evidence.",
            "Primary/direct nullness is observed through the probe's null return. Their native pointer identities are not emitted.",
            "No original or replacement execution by this verifier; cube boundary output is retained, only shape/owner bounds checked here.",
            "Secondary retention is bounded to inputs37/41 and this object configuration; nested cleanup and optional callbacks remain outside the universal claim.",
        ],
    }
    (ROOT / "analysis/object-deactivation-observations.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: 4 imports, 8 snapshots, 424 descriptors, {len(tests)} tampering checks")


if __name__ == "__main__":
    main()
