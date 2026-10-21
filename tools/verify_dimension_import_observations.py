"""Independently verify retained dimension-import observations; execute neither app."""
from collections import Counter
from copy import deepcopy
import json
from pathlib import Path

from capture_oracle import digest
from verify_boundary_sort_observations import ROOT, EXE_SHA, binding, check, read, verify_files
from verify_keyword_import_observations import held_out_scan, IDS
from verify_mesh_array_observations import descriptor_map
from verify_root_layout_observations import header_dimension

DIRECTORY = ROOT / "analysis/oracle/dimension-import-evaluation"
CAPTURE = ROOT / "analysis/dimension-import-runtime-evaluation.json"
MANIFEST = ROOT / "analysis/dimension-import-evaluation-cases.json"


def provenance(evidence, run, manifest):
    directory = DIRECTORY
    check(run["status"] == "completed" and run["exit_code"] == 0, "original completion")
    check(Path(run["cwd"]) == directory, "run directory")
    check(Path(run["executable"]) == ROOT / "source/DEF_PRE_64.exe"
          and digest(Path(run["executable"])) == run["executable_sha256"] == EXE_SHA,
          "executable identity")
    verify_files(run, directory)
    check(sum((directory / n).stat().st_size for n in ("stdout.bin", "stderr.bin"))
          < run["max_output_bytes"], "output bound")
    order = [Path(c["path"]).name for c in manifest["cases"]]
    check(order == run["ordered_imports"] == [i["name"] for i in run["inputs"]]
          and len(order) == len(set(order)) == 6, "input order")
    inputs = {}
    for case, item in zip(manifest["cases"], run["inputs"]):
        source, copy = ROOT / case["path"], directory / item["name"]
        check(Path(item["source"]) == source and Path(item["copy"]) == copy, "fixture paths")
        check(digest(source) == digest(copy) == case["sha256"] == item["sha256_before"]
              == item["sha256_after"] == item["source_sha256_after"], "fixture hashes")
        inputs[item["name"]] = source.read_bytes().replace(b"\r\n", b"\n")
        check(len(inputs[item["name"]]) < 4096 and b"\0" not in inputs[item["name"]]
              and b"\x1a" not in inputs[item["name"]], "fixture stream scope")
    cap = evidence["normalized_result"]
    script, gate = directory.with_suffix(".js"), directory.with_suffix(".ready")
    template = ROOT / "tools/observe_dimension_import.js"
    check(Path(cap["source_path"]) == script and digest(script) == cap["source_sha256"]
          and Path(run["ready_gate"]) == gate, "probe hash/path")
    text = template.read_text(encoding="utf-8-sig")
    check(text.count("__READY_GATE__") == 2 and script.read_text(encoding="utf-8-sig")
          == text.replace("__READY_GATE__", gate.as_posix()), "passive probe template")
    pid = read(directory.with_suffix(".pid.json"))
    check(pid["pid"] == run["pid"] and pid["executable_sha256"] == EXE_SHA, "PID record")
    return inputs


def verify(evidence, run, manifest, inputs, rows, profiles):
    cap, params = evidence["normalized_result"], evidence["parameters"]
    check(cap["pid"] == params["pid"] == run["pid"] == 32056
          and cap["target"] == "DEF_PRE_64.exe (pid 32056)", "PID/target")
    check(cap["session_id"] == params["session_id"] and cap["state"] == "detached"
          and cap["mode"] == params["mode"] == "attach"
          and cap["device_id"] == params["device_id"] == "local"
          and cap["source_kind"] == "file", "lifecycle")
    check(cap["messages_truncated"] is False and cap["cleanup_error"] is None, "incomplete capture")
    messages, scripts = cap["messages"], cap["scripts"]
    check(len(messages) == 113 and len(scripts) == 1 and all(m["type"] == "send"
          and m["script_id"] == scripts[0]["script_id"] for m in messages), "message inventory")
    hook = messages[0]["payload"]
    check(hook["operation"] == "hooks-installed" and hook["pid"] == run["pid"]
          and int(hook["base"], 16) > 0, "hook identity")
    events = [m["payload"] for m in messages[1:]]
    check([e["sequence"] for e in events] == list(range(1, 113)), "sequence completeness")
    roots, generations, identities, history = {}, {}, {}, []
    unit_values, descriptors, snapshots = set(), 0, 0

    def state(s, linked=False):
        nonlocal descriptors, snapshots
        snapshots += 1
        address, dim = s["address"], s["dimension"]
        check(int(address, 16) > 0 and dim in (2, 3), "root address/dimension")
        if address not in roots:
            roots[address], generations[address], identities[address] = dim, 0, {}
        elif roots[address] != dim:
            history.append({"sequence": event["sequence"], "linked": linked,
                            "from": roots[address], "to": dim})
            roots[address] = dim
            generations[address] += 1
            identities[address] = {}
        check(s["components"] == profiles[dim], "fresh root profile differs from original layout capture")
        check(s["suppress_boundaries"] == s["alternate_enabled"] == 0, "flags outside captured scope")
        check(type(s["process_units_raw"]) is int and s["node8_default"]
              == (20 if s["process_units_raw"] == 0 else 68), "process units/default")
        unit_values.add(s["process_units_raw"])
        objects = s["objects"]
        check(s["object_count"] == len(objects) <= 8 and [o["id"] for o in objects]
              == list(range(1, len(objects) + 1)), "object traversal")
        if linked:
            check(not objects and s["linked_state"] is None, "linked root scope")
        addresses = set()
        for obj in objects:
            check(obj["dimension"] == dim and obj["components"] == s["components"]
                  and obj["arity"] in (4, 8) and obj["element_kind"] in (0, 2, 4)
                  and obj["mesh_flag"] in (0, 1), "object controls")
            check(all(type(obj[k]) is int and obj[k] >= 0 for k in ("nodes", "elements")),
                  "mesh counts")
            for key, length in [("coordinates", obj["nodes"] * dim),
                                ("connectivity", obj["elements"] * obj["arity"])]:
                check(0 <= length <= 512 and (obj[key] is None if not length else
                      isinstance(obj[key], list) and len(obj[key]) == length), "primary array shape")
            ds = obj["descriptors"]
            check(len(ds) == len({d["id"] for d in ds}) == 53, "descriptor inventory")
            identity = (obj["address"], tuple(d["address"] for d in ds))
            previous = identities[address].setdefault(obj["id"], identity)
            check(previous == identity, "identity changed within dimension generation")
            for pointer in (identity[0], *identity[1]):
                check(int(pointer, 16) > 0 and pointer not in addresses, "identity alias/null")
                addresses.add(pointer)
            for d, row in zip(ds, rows):
                check(d["id"] == row["state_id"]
                      and d["slot_offset"] == int(row["slot_relative_to_object"], 16)
                      and d["vtable_rva"] == int(row["vtable_address"], 16) - 0x140000000,
                      "descriptor registration")
                check(all(type(d[k]) is int and d[k] >= 0 for k in
                          ("count", "width", "multiplicity", "old_count", "old_width",
                           "old_multiplicity", "length")), "descriptor fields")
                check(d["length"] == d["count"] * d["width"] * d["multiplicity"] <= 512
                      and d["storage_references"] == 1
                      and d["bound_pointer"] == d["storage_pointer"], "descriptor shape/binding")
                check((d["values"] is None and int(d["storage_pointer"], 16) == 0)
                      if not d["length"] else
                      (int(d["storage_pointer"], 16) > 0 and len(d["values"]) == d["length"]),
                      "descriptor payload")
                default = s["node8_default"] if d["id"] == 8 else 1 if d["id"] == 44 else 0
                check(d["values"] is None or d["values"] == [default] * d["length"],
                      "descriptor defaults")
                if not row["selector_minus_one_included"]:
                    check(d["count"] == 0, "post-only descriptor allocated")
                descriptors += 1
            check(set(obj["direct"]) == {str(i) for i in range(18, 24)} | {"61"}, "direct inventory")
            for key, values in obj["direct"].items():
                n = (obj["elements"] if key == "61" else obj["nodes"]) * s["components"][int(key)]
                check(0 <= n <= 512 and (values is None if n == 0 else values == [0] * n),
                      "direct shape/default")
            total = 0
            check(len(obj["boundary_groups"]) <= 100, "boundary groups bound")
            for group in obj["boundary_groups"]:
                if dim == 2:
                    nodes = group["nodes"]
                    check(group["faces"] == group["owners"] == [] and isinstance(nodes, list)
                          and 2 <= len(nodes) <= 512
                          and all(type(n) is int and 1 <= n <= obj["nodes"] for n in nodes),
                          "planar group")
                    total += len(nodes) - 1
                else:
                    faces, owners = group["faces"], group["owners"]
                    check(group["nodes"] == [] and len(faces) == 4 * len(owners) <= 512
                          and all(1 <= n <= obj["nodes"] for n in faces)
                          and all(1 <= n <= obj["elements"] for n in owners), "spatial group")
                    total += len(owners)
            check(total == obj["boundary_total"], "boundary total")
        if s["linked_state"] is not None:
            check(not linked and s["linked_state"]["address"] != address, "linked root identity")
            state(s["linked_state"], True)

    imports, active, previous, token, selected, last_scan = [], None, None, None, False, None
    for event in events:
        op = event["operation"]
        if op == "import-before":
            check(active is None and len(imports) < len(inputs), "nested import")
            check(event["file"] == list(inputs)[len(imports)], "import order")
            if previous is not None:
                check(event["state"] == previous, "state changed between imports")
            active = {"file": event["file"], "before": deepcopy(event["state"]),
                      "handlers": [], "scans": [], "sequence_start": event["sequence"]}
            pos, seen, token, selected, last_scan = 0, False, None, False, None
        else:
            check(active is not None and event["file"] == active["file"], "event outside import")
        if "state" in event:
            state(event["state"])
        if op == "stream-scan":
            check("final_before" not in active, "scan after finalizer")
            pos, detail = held_out_scan(event, inputs[event["file"]], pos, seen)
            seen, last_scan = True, event
            active["scans"].append({"sequence": event["sequence"], **detail})
        elif op == "keyword-resolved":
            check(token is None and last_scan and last_scan["format"] == "%39s"
                  and last_scan["value"] == event["token"] and event["category"] == 2
                  and IDS.get(event["token"]) == event["id"], "keyword dispatch")
            token, selected = event, False
        elif op == "object-selected":
            check(token is not None and not selected and last_scan["format"] == "%i"
                  and last_scan["value"] == event["requested"] and event["found"] is True,
                  "object selector pairing")
            selected = True
        elif op == "handler-after":
            check(token is not None and selected and token["id"] == event["id"]
                  and event["status"] == 0, "handler pairing/status")
            active["handlers"].append(deepcopy(event))
            token, selected = None, False
        elif op == "finalize-before":
            check(token is None and "final_before" not in active and last_scan
                  and last_scan["conversions"] == -1
                  and pos == len(inputs[event["file"]]), "finalizer before EOF/pair")
            active["final_before"] = deepcopy(event["state"])
        elif op == "finalize-after":
            check("final_before" in active and "final_after" not in active, "finalizer pairing")
            active["final_after"] = deepcopy(event["state"])
            check(active["final_before"] == active["final_after"], "tracked finalizer state changed")
        elif op == "import-after":
            check("final_after" in active and event["state"] == active["final_after"]
                  and event["status"] == 0, "import outcome")
            case = manifest["cases"][len(imports)]
            s = event["state"]
            check(s["dimension"] == case["expected_dimension"]
                  and generations[s["address"]] == case["expected_generation"], "scenario generation")
            check(header_dimension(inputs[event["file"]])[0] == s["dimension"]
                  and s["linked_state"]["dimension"] == 5 - s["dimension"], "header/linked result")
            active.update(after=deepcopy(s), sequence_end=event["sequence"])
            imports.append(active)
            previous, active = deepcopy(s), None
        elif op != "import-before":
            check(op in ("stream-scan", "keyword-resolved", "object-selected", "handler-after",
                         "finalize-before", "finalize-after"), "unknown event")
    check(active is None and len(imports) == 6 and len(roots) == 2
          and len(unit_values) == 1, "incomplete trace/global unit drift")
    for index in (1, 3):
        before, after = deepcopy(imports[index]["before"]), deepcopy(imports[index]["after"])
        before["objects"][0]["name"] = after["objects"][0]["name"]
        check(before == after, "same-dimension name-only import changed other tracked state")
    for index in (2, 4):
        first = imports[index]["handlers"][0]["state"]
        check(first["object_count"] == 1 and first["objects"][0]["nodes"]
              == first["objects"][0]["elements"] == 0, "changed dimension retained mesh")
    late = imports[5]["handlers"]
    check(late[0]["state"]["dimension"] == 3 and late[0]["state"]["object_count"] == 4
          and late[0]["state"]["objects"][3]["name"] == "Preheader object to remove",
          "missing preheader object")
    check(late[1]["state"]["dimension"] == 2 and late[1]["state"]["object_count"] == 1
          and late[1]["state"]["objects"][0]["name"] == "", "preheader objects not cleared")
    check(late[-1]["state"]["dimension"] == 2 and late[-1]["state"]["object_count"] == 2,
          "second header not ignored")
    return {"imports": [{"file": x["file"], "sequence_range": [x["sequence_start"], x["sequence_end"]],
                         "before_dimension": x["before"]["dimension"],
                         "after_dimension": x["after"]["dimension"],
                         "linked_dimension": x["after"]["linked_state"]["dimension"],
                         "objects": x["after"]["object_count"], "handlers": len(x["handlers"]),
                         "scans": len(x["scans"]), "status": 0} for x in imports],
            "dimension_changes": history, "process_units_raw": sorted(unit_values),
            "root_snapshots": snapshots, "descriptor_snapshots": descriptors,
            "operation_counts": dict(Counter(e["operation"] for e in events))}


def main():
    evidence, run, manifest = read(CAPTURE), read(DIRECTORY / "run.json"), read(MANIFEST)
    inputs = provenance(evidence, run, manifest)
    rows = descriptor_map(read(ROOT / "analysis/mesh-array-contract.json"))
    layout_path = ROOT / "analysis/root-layout-observations.json"
    layout = read(layout_path)
    profiles = {}
    for item in layout["layout_observations"] + layout["held_out"]["layout_observations"]:
        if item["kind"] == "initialize":
            check(profiles.setdefault(item["dimension"], item["components"]) == item["components"],
                  "initial original profiles vary")
    result = verify(evidence, run, manifest, inputs, rows, profiles)
    mutations = []

    def reject(label, mutate, provenance_check=False):
        e, r, m = deepcopy(evidence), deepcopy(run), deepcopy(manifest)
        mutate(e, r, m)
        try:
            if provenance_check:
                provenance(e, r, m)
            else:
                verify(e, r, m, inputs, rows, profiles)
        except (ValueError, KeyError, TypeError, IndexError) as error:
            mutations.append({"mutation": label, "rejected": True, "reason": str(error)})
            return
        raise ValueError("tampering escaped: " + label)

    def event(e, seq):
        return e["normalized_result"]["messages"][seq]["payload"]

    reject("wrong PID", lambda e, r, m: r.update(pid=1))
    reject("missing event", lambda e, r, m: e["normalized_result"]["messages"].pop(4))
    reject("sequence gap", lambda e, r, m: event(e, 4).update(sequence=99))
    reject("failed handler", lambda e, r, m: event(e, 7).update(status=2))
    reject("corrupt scan bytes", lambda e, r, m: event(e, 2)["after"].update(buffer_hex="00"))
    reject("wrong generation expectation", lambda e, r, m: m["cases"][2].update(expected_generation=8))
    reject("wrong units-derived default", lambda e, r, m: event(e, 1)["state"].update(node8_default=68))
    reject("linked dimension corruption", lambda e, r, m: event(e, 1)["state"]["linked_state"].update(dimension=3))
    reject("descriptor vtable corruption", lambda e, r, m: event(e, 7)["state"]["objects"][0]["descriptors"][0].update(vtable_rva=1))
    reject("descriptor bound pointer corruption", lambda e, r, m: event(e, 7)["state"]["objects"][0]["descriptors"][0].update(bound_pointer="0x1"))
    reject("same-dimension mesh loss", lambda e, r, m: event(e, 33)["state"]["objects"][0].update(nodes=0))
    reject("preheader objects remain", lambda e, r, m: event(e, 97).update(state=deepcopy(event(e, 91)["state"])))
    reject("unpaired finalization", lambda e, r, m: event(e, 24).update(operation="finalize-after"))
    reject("fixture hash corruption", lambda e, r, m: m["cases"][0].update(sha256="0"*64), True)
    reject("executable hash corruption", lambda e, r, m: r.update(executable_sha256="0"*64), True)
    reject("probe hash corruption", lambda e, r, m: e["normalized_result"].update(source_sha256="0"*64), True)
    reject("transcript hash corruption", lambda e, r, m: r["files"][0].update(sha256="0"*64), True)
    freeze_path = ROOT / "analysis/dimension-import-candidate-freeze.json"
    freeze = read(freeze_path)
    current = []
    for item in freeze["files"]:
        check(digest(ROOT / item["archive"]) == item["sha256"], "candidate archive changed")
        path = ROOT / item["path"]
        current.append({"path": item["path"], "matches_freeze":
                        path.is_file() and digest(path) == item["sha256"]})
    report = {
        "status": "pass", "scope": __doc__, "capture": binding(CAPTURE),
        "evidence_id": evidence["evidence_id"], "pid": run["pid"],
        "run": binding(DIRECTORY / "run.json"), "fixtures": binding(MANIFEST),
        "probe": binding(DIRECTORY.with_suffix(".js")),
        "template": binding(ROOT / "tools/observe_dimension_import.js"),
        "verifier": binding(Path(__file__)), "original_layout_report": binding(layout_path),
        "verification_dependencies": [binding(ROOT / p) for p in (
            "tools/capture_oracle.py", "tools/verify_boundary_sort_observations.py",
            "tools/verify_keyword_import_observations.py", "tools/verify_mesh_array_observations.py",
            "tools/verify_root_layout_observations.py", "analysis/mesh-array-contract.json",
            "analysis/root-reset-contract-audit.json")],
        "candidate_freeze": {"manifest": binding(freeze_path), "created_utc": freeze["created_utc"],
                             "archives_verified": len(current), "current_matches": current,
                             "qualification": "Current candidate equality is informational; no candidate was executed. Freeze timestamp is retained metadata, not an independent timestamp authority."},
        **result, "tampering_checks": mutations,
        "comparator_audit": {
            "source": binding(ROOT / "tools/compare_dimension_import.py"),
            "assessment": "No material mismatch found in its bounded numeric replay comparison. Independent verifier strengthens original provenance and event/descriptor checks.",
            "gaps": [
                "Comparator does not require every resolved token to receive a handler or pair object-selected and scan events; this verifier enforces both.",
                "Comparator accepts stream-scan events without inspecting bytes/cursor/EOF; this verifier checks all43 scans.",
                "Comparator validates descriptor IDs/slots but does not check vtable or unique descriptor addresses; this verifier does.",
                "Generation is inferred from sampled dimension changes; no transition hook means unobserved reset-and-return cycles are not excluded in arbitrary inputs.",
                "RootIdentity does not itself enforce an empty linked root on every snapshot; this verifier confirms that bound for this capture.",
                "Comparator computes expected cursor/EOF from fixture length rather than validating captured stream events; the independent scan checks substantiate these fixture outcomes."
            ]},
        "limitations": [
            "Six bounded ASCII fixtures with empty linked roots, alternate dispatch0 and suppression0; no arbitrary controls, materials, nonzero flags or populated linked meshes.",
            "All observed global unit values are retained above; no runtime global-unit change or alternate-unit behavior was exercised.",
            "Fresh component profiles are cross-checked against prior original captures, not generated by replacement code.",
            "Same-dimension preservation excludes the intended name change; native identity is checked within observed generations, allowing allocator address reuse across resets.",
            "Finalizer equality concerns the captured subset only, not complete scientific side effects.",
            "No instruction-level destructor or root-reset hook in this probe; clearing is established at subsequent handler snapshots.",
            "Numeric JSON checks do not establish bitwise floating-point equivalence.",
            "Direct61 source object+0x3980 is not captured; observed direct arrays are zero, with constructor source bound separately in root-reset audit."
        ]}
    output = ROOT / "analysis/dimension-import-observations.json"
    output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {len(result['imports'])} imports, {result['descriptor_snapshots']} descriptor observations, "
          f"{len(mutations)} tampering checks")


if __name__ == "__main__":
    main()
