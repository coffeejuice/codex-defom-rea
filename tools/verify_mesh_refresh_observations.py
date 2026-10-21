"""Validate passive original mesh refresh evidence without executing either application."""
from collections import Counter
from copy import deepcopy
import argparse
import hashlib
import json
import math
from pathlib import Path
import struct

from capture_oracle import digest
from verify_boundary_sort_observations import ROOT, EXE_SHA, binding, check, read, verify_files
from verify_mesh_array_observations import descriptor_map

DIRECTORY = ROOT / "analysis/oracle/mesh-refresh-complete"
CAPTURE = ROOT / "analysis/mesh-refresh-runtime-complete.json"
MANIFEST = ROOT / "analysis/mesh-refresh-evaluation-cases.json"
COMPLETE = True
NODE_BUFFERS = ["coordinates"] + [f"direct{i}" for i in range(18, 24)]
ELEMENT_BUFFERS = ["connectivity", "direct61"]
SLOTS = {"coordinates": 0x8a8, "connectivity": 0x9e8, "direct61": 0x9f8,
         **{f"direct{i}": 0x990 + (i - 18) * 8 for i in range(18, 24)}}


def provenance(evidence, run, manifest):
    check(run["status"] == "completed" and run["exit_code"] == 0
          and Path(run["cwd"]) == DIRECTORY, "run completion/path")
    check(Path(run["executable"]) == ROOT / "source/DEF_PRE_64.exe"
          and digest(Path(run["executable"])) == run["executable_sha256"] == EXE_SHA,
          "original identity")
    verify_files(run, DIRECTORY)
    check(sum((DIRECTORY / n).stat().st_size for n in ("stdout.bin", "stderr.bin"))
          < run["max_output_bytes"], "output bound")
    order = [Path(c["path"]).name for c in manifest["cases"]]
    check(order == run["ordered_imports"] == [i["name"] for i in run["inputs"]]
          and len(order) == 1, "fixture inventory/order")
    for case, item in zip(manifest["cases"], run["inputs"]):
        source, copy = ROOT / case["path"], DIRECTORY / item["name"]
        check(Path(item["source"]) == source and Path(item["copy"]) == copy, "fixture paths")
        check(digest(source) == digest(copy) == case["sha256"] == item["sha256_before"]
              == item["sha256_after"] == item["source_sha256_after"], "fixture hashes")
    cap = evidence["normalized_result"]
    script, gate = DIRECTORY.with_suffix(".js"), DIRECTORY.with_suffix(".ready")
    template = ROOT / ("tools/observe_mesh_refresh_complete.js" if COMPLETE
                       else "tools/observe_mesh_refresh.js")
    check(Path(cap["source_path"]) == script and digest(script) == cap["source_sha256"]
          and Path(run["ready_gate"]) == gate
          and digest(ROOT / manifest["probe"]["path"]) == manifest["probe"]["sha256"], "probe identity")
    check(script.read_text(encoding="utf-8-sig") ==
          template.read_text(encoding="utf-8-sig").replace("__READY_GATE__", gate.as_posix()),
          "passive source differs from template")
    pid = read(DIRECTORY.with_suffix(".pid.json"))
    check(pid["pid"] == run["pid"] and pid["executable_sha256"] == EXE_SHA, "PID record")
    for item in manifest["static_evidence"]:
        check(digest(ROOT / item["path"]) == item["sha256"], "static evidence changed")


def bytes_values(raw, values, integers):
    data = bytes.fromhex(raw)
    size = 4 if integers else 8
    check(len(data) % size == 0 and len(values) == len(data) // size, "raw/value length")
    decoded = struct.unpack("<" + ("i" if integers else "d") * len(values), data)
    for observed, value in zip(values, decoded):
        check(observed == value if math.isfinite(value) else observed is None, "raw/value mismatch")
    return data


def verify(evidence, run, rows):
    cap, params = evidence["normalized_result"], evidence["parameters"]
    pid = 18808 if COMPLETE else 31164
    message_count = 136 if COMPLETE else 134
    check(cap["pid"] == params["pid"] == run["pid"] == pid
          and cap["target"] == f"DEF_PRE_64.exe (pid {pid})", "PID/target")
    check(cap["session_id"] == params["session_id"] and cap["state"] == "detached"
          and cap["mode"] == params["mode"] == "attach"
          and cap["device_id"] == params["device_id"] == "local"
          and cap["source_kind"] == "file", "capture lifecycle")
    check(cap["messages_truncated"] is False and cap["cleanup_error"] is None, "incomplete capture")
    messages, scripts = cap["messages"], cap["scripts"]
    check(len(messages) == message_count and len(scripts) == 1 and all(m["type"] == "send"
          and m["script_id"] == scripts[0]["script_id"] for m in messages), "message inventory")
    hook = messages[0]["payload"]
    check(hook["operation"] == "hooks-installed" and hook["pid"] == run["pid"]
          and int(hook["base"], 16) > 0, "hooks identity")
    events = [m["payload"] for m in messages[1:]]
    check([e["sequence"] for e in events] == list(range(1, message_count)), "sequence completeness")
    check(all(e["file"] == run["ordered_imports"][0] for e in events), "event file")
    identities, descriptors, snapshots = {}, 0, 0

    def objcheck(obj):
        nonlocal snapshots, descriptors
        snapshots += 1
        check(obj["dimension"] == 2 and len(obj["root_components"]) == 79, "object dimension/table")
        check(all(type(obj[k]) is int and obj[k] >= 0 for k in
                  ("nodes", "elements", "allocated_nodes", "allocated_elements", "allocated_arity")),
              "object counts")
        ds = obj["descriptors"]
        check(len(ds) == len({d["id"] for d in ds}) == 53, "descriptor inventory")
        identity = (obj["address"], obj["owner"], tuple(d["address"] for d in ds))
        check(identities.setdefault(obj["id"], identity) == identity, "object/descriptor identity")
        check(len(set(identity[2])) == 53 and all(int(p, 16) > 0 for p in identity[2]),
              "descriptor address alias/null")
        for d, row in zip(ds, rows):
            check(d["id"] == row["state_id"]
                  and d["slot_offset"] == int(row["slot_relative_to_object"], 16)
                  and d["vtable_rva"] == int(row["vtable_address"], 16) - 0x140000000,
                  "descriptor mapping")
            check(all(type(d[k]) is int and d[k] >= 0 for k in
                      ("count", "width", "multiplicity", "old_count", "old_width",
                       "old_multiplicity", "length")), "descriptor field bounds")
            check(d["length"] == d["count"] * d["width"] * d["multiplicity"] <= 512
                  and d["storage_references"] == 1
                  and d["bound_pointer"] == d["storage_pointer"], "descriptor extent/binding")
            check(len(bytes_values(d["values_hex"], d["values"], False)) == d["length"] * 8,
                  "descriptor byte extent")
            check(d["length"] == 0 or int(d["storage_pointer"], 16) > 0, "null descriptor payload")
            descriptors += 1
        check(set(obj["buffers"]) == set(SLOTS), "direct buffer inventory")
        for name, b in obj["buffers"].items():
            check(int(b["slot"], 16) == int(obj["address"], 16) + SLOTS[name]
                  and b["integers"] == (name != "coordinates"), "buffer slot/type")
            if b["extent_source"] == "unknown-not-read":
                check(int(b["pointer"], 16) > 0 and all(b[k] is None for k in
                      ("allocated_bytes", "allocated_items", "values", "bytes_hex")),
                      "unknown buffer was read")
            else:
                check(b["extent_source"] in ("null-pointer", "completed-helper-request"),
                      "buffer extent source")
                raw = bytes_values(b["bytes_hex"], b["values"], b["integers"])
                check(len(raw) == b["allocated_bytes"] == b["allocated_items"] *
                      (4 if b["integers"] else 8) <= 4096, "buffer extent")
                if b["extent_source"] == "null-pointer":
                    check(int(b["pointer"], 16) == 0 and not raw, "null buffer")
                else:
                    check(type(b["allocation_serial"]) is int, "allocation serial")
        check(len(obj["boundary_groups"]) <= 100, "boundary count")
        total = 0
        for group in obj["boundary_groups"]:
            check(group["faces"] == group["owners"] == [] and len(group["nodes"]) >= 2,
                  "planar boundary shape")
            total += len(group["nodes"]) - 1
        check(total == obj["boundary_total"], "boundary total")

    def rootcheck(s):
        check(s["dimension"] in (2, 3) and len(s["components"]) == 79
              and s["object_count"] == len(s["objects"]), "root shape")
        check([o["id"] for o in s["objects"]] == list(range(1, s["object_count"] + 1)),
              "root traversal")
        for o in s["objects"]:
            check(o["owner"] == s["address"] and o["root_components"] == s["components"],
                  "root/object relationship")
            objcheck(o)

    pairs, summaries, root_summaries, established = [], [], [], {}

    def resize_pair(a, b):
        old, new = a["object"], b["object"]
        kind, count = a["kind"], a["requested_count"]
        names = NODE_BUFFERS if kind == "nodes" else ELEMENT_BUFFERS
        check(b["kind"] == kind and b["buffers"] == {n: new["buffers"][n] for n in names}
              and set(b["preservation"]) == set(names), "resize duplicate payload")
        check(count == old["allocated_nodes" if kind == "nodes" else "allocated_elements"],
              "resize used header rather than allocation count")
        check(int(a["container"], 16) == int(old["address"], 16) +
              (0x8a0 if kind == "nodes" else 0x9e0), "container address")
        check(a["requested_arity"] == (None if kind == "nodes" else old["allocated_arity"]),
              "allocated arity argument")
        check(a["requested_components"] == old["root_components"], "requested table")
        unchanged = [k for k in old if k not in ("buffers", "descriptors")]
        check(all(old[k] == new[k] for k in unchanged), "resize altered unrelated object state")
        if count <= 0:
            check(old == new, "nonpositive container resize mutated state")
        if COMPLETE and count > 0:
            for name in names:
                x = old["buffers"][name]
                check(x["allocated_bytes"] is not None, "complete capture has unknown initial extent")
                if x["allocation_serial"] in established:
                    baseline = established[x["allocation_serial"]]["buffers"][name]
                    check(all(x[k] == baseline[k] for k in
                              ("slot", "pointer", "allocated_bytes", "allocated_items", "allocation_serial")),
                          "initial allocation metadata lost before first resize")
        prefix_checks, growth = [], []
        for name in SLOTS:
            x, y = old["buffers"][name], new["buffers"][name]
            if name not in names:
                check(x == y, "other container buffer changed")
                continue
            if count > 0:
                component = 0 if name == "coordinates" else int(name[6:]) if name.startswith("direct") else None
                width = a["requested_arity"] if component is None else a["requested_components"][component]
                size = count * width * (4 if y["integers"] else 8)
                check(y["allocated_bytes"] == size and
                      (y["extent_source"] == "null-pointer" if int(y["pointer"], 16) == 0 else
                       y["allocation_serial"] == a["serial"]), "new requested extent/serial")
            claim = b["preservation"][name]
            if x["allocated_bytes"] is None or y["allocated_bytes"] is None:
                check(claim["known_prefix_bytes"] is None and claim["prefix_equal"] is None,
                      "unknown extent has preservation claim")
                continue
            n = min(x["allocated_bytes"], y["allocated_bytes"])
            before, after = x["bytes_hex"][:n*2], y["bytes_hex"][:n*2]
            check(before == after and claim["prefix_equal"] is True
                  and claim["old_bytes"] == x["allocated_bytes"]
                  and claim["new_bytes"] == y["allocated_bytes"]
                  and claim["known_prefix_bytes"] == n
                  and claim["prefix_before_hex"] == before == claim["prefix_after_hex"]
                  and claim["grown_tail_bytes"] == max(0, y["allocated_bytes"] - x["allocated_bytes"])
                  and claim["grown_tail_initialization"] == "not-established", "raw preserved prefix")
            prefix_checks.append({"buffer": name, "bytes": n,
                                  "sha256": hashlib.sha256(bytes.fromhex(before)).hexdigest()})
            if y["allocated_bytes"] > x["allocated_bytes"]:
                tail = bytes.fromhex(y["bytes_hex"])[x["allocated_bytes"]:]
                growth.append({"buffer": name, "bytes": len(tail), "observed_nonzero_bytes": sum(v != 0 for v in tail),
                               "initialization": "unspecified; observed bytes are not reusable defaults"})
        affected = 0
        for x, y, row in zip(old["descriptors"], new["descriptors"], rows):
            selected = count > 0 and row["selector_minus_one_included"] and (
                (row["slot_relative_to_object"] and int(row["slot_relative_to_object"], 16) < 0x9e0) == (kind == "nodes"))
            if not selected:
                check(x == y, "unselected descriptor changed")
                continue
            affected += 1
            check(y["count"] == count and y["width"] == a["requested_components"][x["id"]]
                  and y["old_count"] == x["count"] and y["old_width"] == x["width"],
                  "descriptor count/width history")
            check(y["multiplicity"] == x["multiplicity"] == 1
                  and y["old_multiplicity"] == (x["multiplicity"] if x["id"] in (38, 43, 45)
                                               else x["old_multiplicity"]), "multiplicity history")
            raw = bytes.fromhex(x["values_hex"])
            length = y["length"] * 8
            expected = bytearray(raw[:length] + b"\0" * max(0, length - len(raw)))
            if len(raw) != length and x["id"] == 46 and y["width"] == 9:
                for r in range(count):
                    for c in (0, 4, 8):
                        expected[(r*9+c)*8:(r*9+c+1)*8] = struct.pack("<d", 1)
            check(y["values_hex"] == expected.hex(), "descriptor prefix/zero-growth/MATAXI")
            if length == len(raw):
                check(y["bound_pointer"] == x["bound_pointer"], "same-size descriptor pointer changed")
        summaries.append({"serial": a["serial"], "object_id": old["id"], "kind": kind,
                          "count": count, "no_op": count <= 0, "descriptor_updates": affected,
                          "prefix_checks": prefix_checks, "direct_growth": growth})

    stack, serial, active, finalized, last_root = [], 0, False, 0, None
    for event in events:
        op = event["operation"]
        if "state" in event:
            rootcheck(event["state"])
        if isinstance(event.get("object"), dict):
            objcheck(event["object"])
        if op == "import-before":
            check(not active and not stack, "nested import")
            active = True
        elif op == "allocation-established":
            serial += 1
            check(COMPLETE and active and not stack and finalized == 0
                  and event["serial"] == serial and event["parent_serial"] is None
                  and event["selector"] == -1 and event["requested_count"] > 0,
                  "replacement allocation context")
            names = NODE_BUFFERS if event["kind"] == "nodes" else ELEMENT_BUFFERS
            check(event["established_buffers"] == names and set(event["buffers"]) == set(names),
                  "initial allocation inventory")
            check(event["coordinates_were_null"] == (True if event["kind"] == "nodes" else None),
                  "initial coordinate selection")
            for name in names:
                b = event["buffers"][name]
                component = 0 if name == "coordinates" else int(name[6:]) if name.startswith("direct") else None
                width = event["requested_arity"] if component is None else event["requested_components"][component]
                check(b["allocation_serial"] == serial
                      and int(b["slot"], 16) == int(event["object"], 16) + SLOTS[name]
                      and b["integers"] == (name != "coordinates")
                      and b["allocated_items"] == event["requested_count"] * width
                      and b["allocated_bytes"] == b["allocated_items"] * (4 if b["integers"] else 8)
                      and b["extent_source"] == "completed-helper-request", "initial allocation shape")
                raw = bytes_values(b["bytes_hex"], b["values"], b["integers"])
                check(len(raw) == b["allocated_bytes"] and raw == b"\0" * len(raw),
                      "initial replacement allocation zeros")
            established[serial] = event
        elif op.endswith("-enter"):
            check(active and finalized == 0, "helper outside import")
            serial += 1
            check(event["serial"] == serial and event["parent_serial"] ==
                  (stack[-1]["event"]["serial"] if stack else None), "serial/nesting")
            expected_parent = {"root-refresh-enter": None, "layout-enter": "root-refresh-enter",
                               "mesh-refresh-enter": "root-refresh-enter", "resize-enter": "mesh-refresh-enter"}
            check(op in expected_parent and expected_parent[op] ==
                  (stack[-1]["event"]["operation"] if stack else None), "call graph nesting")
            if op == "root-refresh-enter" and last_root and last_root["object_count"] > 0:
                check(event["state"] == last_root, "state changed between refreshes")
            stack.append({"event": event, "children": []})
        elif op.endswith("-leave"):
            check(stack and op == stack[-1]["event"]["operation"].replace("-enter", "-leave"),
                  "unpaired helper leave")
            frame = stack.pop()
            before, children = frame["event"], frame["children"]
            check(event["serial"] == before["serial"] and event["parent_serial"] == before["parent_serial"],
                  "leave identity")
            if op == "resize-leave":
                check(not children, "nested resize")
                resize_pair(before, event)
            elif op == "mesh-refresh-leave":
                check([a["kind"] for a, b in children] == ["nodes", "elements"]
                      and before["object"] == children[0][0]["object"]
                      and children[0][1]["object"] == children[1][0]["object"]
                      and event["object"] == children[1][1]["object"], "mesh child continuity")
                check(all(a["requested_components"] == before["requested_components"] for a, b in children),
                      "mesh table propagation")
            elif op == "layout-leave":
                old, new = deepcopy(before["state"]), deepcopy(event["state"])
                old["components"] = new["components"]
                for o in old["objects"]:
                    o["root_components"] = new["components"]
                check(old == new and not children, "layout changed allocated state")
            else:
                check(before["kind"] == event["kind"] == "all-objects"
                      and before["argument"] == event["argument"] == 0
                      and children and children[0][0]["operation"] == "layout-enter",
                      "root refresh scope")
                layout_before, layout_after = children[0]
                check(layout_before["state"] == before["state"], "root/layout entry continuity")
                expected = deepcopy(layout_after["state"])
                called = []
                for a, b in children[1:]:
                    oid = a["object"]["id"]
                    check(a["operation"] == "mesh-refresh-enter"
                          and a["object"] == expected["objects"][oid - 1]
                          and a["prior_components_argument"] == before["state"]["components"],
                          "root/object entry continuity")
                    expected["objects"][oid - 1] = b["object"]
                    called.append(oid)
                eligible = [o["id"] for o in layout_after["state"]["objects"] if o["type"] != 7]
                check(called == eligible and event["state"] == expected, "type7 skip/root result")
                last_root = event["state"]
                root_summaries.append({"serial": before["serial"], "called_ids": called,
                                       "skipped_type7": [o["id"] for o in expected["objects"] if o["type"] == 7],
                                       "before_components": before["state"]["components"],
                                       "after_components": event["state"]["components"]})
            pairs.append((before, event))
            if stack:
                stack[-1]["children"].append((before, event))
        elif op == "keyword-resolved":
            check(active and not stack and finalized == 0 and event["id"] >= 0, "keyword placement")
        elif op == "finalize-before":
            check(active and not stack and finalized == 0, "finalize entry")
            final_before, finalized = event["state"], 1
        elif op == "finalize-after":
            check(finalized == 1 and not stack, "finalize exit")
            final_after, finalized = event["state"], 2
        elif op == "import-after":
            check(active and not stack and finalized == 2 and event["status"] == 0
                  and event["state"] == final_after, "import outcome")
            active = False
        else:
            raise ValueError("unexpected event " + op)
    check(not active and not stack and len(root_summaries) == 8 and len(summaries) == 28,
          "incomplete refresh inventory")
    check(len(established) == (2 if COMPLETE else 0), "initial allocation completeness")
    return {"root_refreshes": root_summaries, "resize_pairs": summaries,
            "initial_allocations": [{"serial": e["serial"], "kind": e["kind"],
                                     "count": e["requested_count"], "arity": e["requested_arity"],
                                     "buffers": e["established_buffers"]} for e in established.values()],
            "object_snapshots": snapshots, "descriptor_observations": descriptors,
            "operation_counts": dict(Counter(e["operation"] for e in events)),
            "tracked_finalization_changed": final_before != final_after}


def main():
    global COMPLETE, DIRECTORY, CAPTURE
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--legacy", action="store_true", help="Recheck the partial v1 capture.")
    args = parser.parse_args()
    if args.legacy:
        COMPLETE = False
        DIRECTORY = ROOT / "analysis/oracle/mesh-refresh-evaluation"
        CAPTURE = ROOT / "analysis/mesh-refresh-runtime-evaluation.json"
    evidence, run, manifest = read(CAPTURE), read(DIRECTORY / "run.json"), read(MANIFEST)
    provenance(evidence, run, manifest)
    rows = descriptor_map(read(ROOT / "analysis/mesh-array-contract.json"))
    result = verify(evidence, run, rows)
    mutations = []

    def event(e, seq):
        if COMPLETE and seq >= 10:
            seq += 2
        return e["normalized_result"]["messages"][seq]["payload"]

    def reject(label, fn, prov=False):
        e, r, m = deepcopy(evidence), deepcopy(run), deepcopy(manifest)
        fn(e, r, m)
        try:
            provenance(e, r, m) if prov else verify(e, r, rows)
        except (ValueError, KeyError, TypeError, IndexError) as error:
            mutations.append({"mutation": label, "rejected": True, "reason": str(error)})
            return
        raise ValueError("mutation escaped: " + label)

    reject("wrong PID", lambda e,r,m: r.update(pid=1))
    reject("event missing", lambda e,r,m: e["normalized_result"]["messages"].pop(18))
    reject("parent nesting", lambda e,r,m: event(e,17).update(parent_serial=None))
    reject("wrong container count", lambda e,r,m: event(e,17).update(requested_count=5))
    reject("wrong allocated arity", lambda e,r,m: event(e,19).update(requested_arity=8))
    reject("corrupt preserved byte claim", lambda e,r,m: event(e,35)["preservation"]["direct18"].update(prefix_before_hex="00"))
    reject("new extent corruption", lambda e,r,m: event(e,35)["object"]["buffers"]["direct18"].update(allocated_bytes=4))
    reject("descriptor history corruption", lambda e,r,m: event(e,18)["object"]["descriptors"][0].update(old_count=99))
    reject("empty container mutation", lambda e,r,m: event(e,24)["object"].update(mesh_flag=1))
    reject("type7 eligibility corruption", lambda e,r,m: event(e,13)["state"]["objects"][1].update(type=1))
    reject("vtable corruption", lambda e,r,m: event(e,17)["object"]["descriptors"][0].update(vtable_rva=1))
    reject("probe error", lambda e,r,m: event(e,17).update(operation="probe-error"))
    reject("fixture hash", lambda e,r,m: m["cases"][0].update(sha256="0"*64), True)
    reject("exe hash", lambda e,r,m: r.update(executable_sha256="0"*64), True)
    reject("probe hash", lambda e,r,m: e["normalized_result"].update(source_sha256="0"*64), True)
    reject("transcript hash", lambda e,r,m: r["files"][0].update(sha256="0"*64), True)
    def corrupt_growth(e, r, m):
        descriptor = next(d for d in event(e, 18)["object"]["descriptors"] if d["id"] == 2)
        descriptor["values"][-1] = 1.0
        descriptor["values_hex"] = descriptor["values_hex"][:-16] + struct.pack("<d", 1).hex()
    reject("nonzero descriptor growth with consistent raw bytes", corrupt_growth)
    if COMPLETE:
        reject("initial allocation extent", lambda e,r,m:
               e["normalized_result"]["messages"][10]["payload"]["buffers"]["direct18"].update(allocated_items=1))
    freeze_path = ROOT / "analysis/mesh-refresh-candidate-freeze.json"
    freeze = read(freeze_path)
    current = []
    for item in freeze["files"]:
        check(digest(ROOT / item["archive"]) == item["sha256"], "frozen archive hash")
        p = ROOT / item["path"]
        current.append({"path": item["path"], "matches_freeze": p.is_file() and digest(p) == item["sha256"]})
    report = {
        "status": "pass", "scope": __doc__, "capture": binding(CAPTURE),
        "evidence_id": evidence["evidence_id"], "pid": run["pid"],
        "run": binding(DIRECTORY / "run.json"), "fixture_manifest": binding(MANIFEST),
        "probe": binding(DIRECTORY.with_suffix(".js")),
        "template": binding(ROOT / ("tools/observe_mesh_refresh_complete.js" if COMPLETE
                                   else "tools/observe_mesh_refresh.js")),
        "verifier": binding(Path(__file__)),
        "dependencies": [binding(ROOT / p) for p in (
            "tools/capture_oracle.py", "tools/verify_boundary_sort_observations.py",
            "tools/verify_mesh_array_observations.py", "analysis/mesh-array-contract.json")],
        "candidate_freeze": {"manifest": binding(freeze_path), "created_utc": freeze["created_utc"],
                             "verified_archives": len(current), "current_matches": current},
        **result, "tampering_checks": mutations,
        "limits": [
            "One planar fixture, eight root refreshes, fourteen object wrappers and twenty-eight container pairs; unit multiplicities and argument0 root refresh only.",
            ("Initial replacement-allocation hooks establish every direct-buffer extent before first resize; all first preserved prefixes are checked."
             if COMPLETE else "First populated direct-buffer allocations have unknown extents and are not read before first resize; first preservation is not established."),
            "Subsequent min(old,new) byte-prefix checks include bytes that originally entered through unspecified growth tails. Exact retention is distinct from deterministic initialization.",
            "Grown direct bytes may be nonzero and allocator-dependent; no replacement-generated expected tail values are used.",
            "Descriptor predictions use prior original bytes plus recovered append zero-fill and shape/history rules. MATAXI width9 branch exists in the verifier but this fixture does not exercise it.",
            "Empty-container no-op and type7 skip are exercised. Negative counts, independent header/allocation divergence, nonunit multiplicity and shared storage are not.",
            "Root-table generation is observed, not recomputed. Object arrays may retain older widths while the root table has already changed.",
            "Finalization comparison covers the captured subset only. No full scientific-finalizer equivalence claim.",
            "Freeze timestamps are retained metadata; current build/source matches are informational. Neither application was executed by this verifier."
        ]}
    (ROOT / ("analysis/mesh-refresh-observations.json" if COMPLETE
             else "analysis/mesh-refresh-v1-partial-observations.json")).write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {len(result['root_refreshes'])} root refreshes, "
          f"{len(result['resize_pairs'])} resize pairs, {len(mutations)} tampering checks")


if __name__ == "__main__":
    main()
