"""Replay captured mesh-preserving refreshes without using AFTER values as inputs.

Each object retains one replacement instance across its observed refresh sequence.
BEFORE values seed the replay; descriptor shape/history and allocation metadata are
checked before each refresh. Direct-buffer grown tails remain unknown (JSON null).
"""
import argparse
from collections import defaultdict
import json
import math
from pathlib import Path
import struct
import subprocess

from capture_oracle import digest
from compare_keyword_import import ROOT, EXE_SHA, require, read, binding
from verify_mesh_array_observations import descriptor_map

def verify_candidate(driver, enforce):
    path = ROOT / "analysis/mesh-refresh-candidate-freeze.json"
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
    driver_matches = digest(driver) == binary["sha256"]
    current_matches = all(item["matches_freeze"] for item in current_files)
    if enforce:
        require(current_matches and driver_matches, "candidate changed under frozen enforcement")
    return {"manifest": binding(path), "created_utc": freeze["created_utc"],
            "stage": freeze["stage"], "verified_archives": freeze["files"],
            "current_files": current_files, "current_files_match_freeze": current_matches,
            "driver_matches_freeze": driver_matches, "frozen_candidate_required": enforce}


def provenance(path, run_path, fixtures, template):
    evidence, run = read(path), read(run_path)
    cap, params = evidence["normalized_result"], evidence["parameters"]
    directory = run_path.parent.resolve()
    require(Path(run["cwd"]).resolve() == directory, "run directory mismatch")
    require(run["status"] == "completed" and run["exit_code"] == 0,
            "original run did not complete normally")
    executable = ROOT / "source/DEF_PRE_64.exe"
    require(Path(run["executable"]).resolve() == executable.resolve()
            and run["executable_sha256"] == digest(executable) == EXE_SHA,
            "original executable identity mismatch")
    require(cap["pid"] == params["pid"] == run["pid"]
            and cap["target"] == f"DEF_PRE_64.exe (pid {run['pid']})"
            and cap["session_id"] == params["session_id"], "process/session mismatch")
    require(cap["state"] == "detached" and cap["mode"] == params["mode"] == "attach"
            and cap["device_id"] == params["device_id"] == "local"
            and cap["source_kind"] == "file", "capture lifecycle mismatch")
    require(cap["messages_truncated"] is False and cap["cleanup_error"] is None,
            "incomplete capture")
    script = Path(cap["source_path"]).resolve()
    require(script == directory.with_suffix(".js")
            and digest(script) == cap["source_sha256"], "probe path/hash mismatch")
    gate = Path(run["ready_gate"]).resolve()
    require(gate == directory.with_suffix(".ready"), "gate path mismatch")
    require(script.read_text(encoding="utf-8-sig")
            == template.read_text(encoding="utf-8-sig").replace("__READY_GATE__", gate.as_posix()),
            "retained probe differs from passive template")
    pid_record = read(directory.with_suffix(".pid.json"))
    require(pid_record["pid"] == run["pid"]
            and pid_record["executable_sha256"] == EXE_SHA, "PID record mismatch")
    for item in run["files"]:
        output = directory / item["name"]
        require(output.parent.resolve() == directory and output.is_file()
                and output.stat().st_size == item["bytes"] and digest(output) == item["sha256"],
                "run output hash/length mismatch: " + item["name"])
    require(sum((directory / name).stat().st_size for name in ("stdout.bin", "stderr.bin"))
            < run["max_output_bytes"], "run output exceeded its bound")
    require((directory / "stdin.bin").read_bytes() == run["stdin"].encode("utf-8"),
            "stdin transcript mismatch")
    require([item["name"] for item in run["inputs"]] == run["ordered_imports"],
            "run input order mismatch")
    source_files, fixture_bindings = {}, []
    for item in run["inputs"]:
        source = (fixtures / item["name"] if fixtures else Path(item["source"])).resolve()
        copy = directory / item["name"]
        require(Path(item["source"]).resolve() == source and Path(item["copy"]).resolve() == copy,
                "input source/copy path mismatch")
        require(digest(source) == digest(copy) == item["sha256_before"]
                == item["sha256_after"] == item["source_sha256_after"], "input hash mismatch")
        source_files[item["name"]] = source
        fixture_bindings.append(binding(source))
    messages, scripts = cap["messages"], cap["scripts"]
    require(len(scripts) == 1 and all(m["type"] == "send"
            and m["script_id"] == scripts[0]["script_id"] for m in messages), "probe message error")
    hook = messages[0]["payload"]
    require(hook["operation"] == "hooks-installed" and hook["pid"] == run["pid"],
            "hook identity mismatch")
    events = [message["payload"] for message in messages[1:]]
    require([event["sequence"] for event in events] == list(range(1, len(events) + 1)),
            "event sequence gaps")
    require(not any(event["operation"] == "probe-error" for event in events), "probe failure")
    return evidence, run, events, source_files, {
        "capture": binding(path), "run_record": binding(run_path),
        "evidence_id": evidence["evidence_id"], "pid": run["pid"],
        "probe": binding(script), "probe_template": binding(template),
        "pid_record": binding(directory.with_suffix(".pid.json")),
        "fixtures": fixture_bindings, "executable_sha256": EXE_SHA,
    }


BUFFER_NAMES = ("coordinates", "connectivity", "direct18", "direct19",
                "direct20", "direct21", "direct22", "direct23", "direct61")


def values_from_hex(item, integers=False):
    size, code = (4, "i") if integers else (8, "d")
    raw = bytes.fromhex(item)
    require(len(raw) % size == 0, "unaligned captured bytes")
    return list(struct.unpack("<" + code * (len(raw) // size), raw))


def descriptor_values(descriptor):
    values = values_from_hex(descriptor["values_hex"])
    require(len(values) == descriptor["length"]
            == descriptor["count"] * descriptor["width"] * descriptor["multiplicity"],
            "descriptor byte/shape extent mismatch")
    require(all(math.isfinite(value) for value in values), "nonfinite descriptor input")
    return values


def buffer_values(buffer, name):
    require(buffer["allocated_items"] is not None and buffer["bytes_hex"] is not None,
            f"{name}: missing initial extent; use the complete V2 capture")
    integers = name != "coordinates"
    values = values_from_hex(buffer["bytes_hex"], integers)
    require(len(values) == buffer["allocated_items"]
            and len(bytes.fromhex(buffer["bytes_hex"])) == buffer["allocated_bytes"],
            "buffer byte/extent mismatch")
    return values


def pair_wrappers(events):
    pairs = {"root-refresh-enter": "root-refresh-leave", "layout-enter": "layout-leave",
             "mesh-refresh-enter": "mesh-refresh-leave", "resize-enter": "resize-leave"}
    stack, wrappers, allocations, import_active = [], [], [], False
    for event in events:
        op = event["operation"]
        if op == "import-before":
            require(not import_active and not stack, "nested import")
            import_active = True
        elif op == "import-after":
            require(import_active and not stack and event["status"] == 0, "incomplete import")
            import_active = False
        elif op in pairs:
            require(import_active, "refresh outside import")
            require(event["parent_serial"] == (stack[-1]["serial"] if stack else None),
                    "refresh parent mismatch")
            stack.append(event)
        elif op in pairs.values():
            require(stack, "unpaired refresh leave")
            before = stack.pop()
            require(pairs[before["operation"]] == op
                    and before["serial"] == event["serial"]
                    and before["parent_serial"] == event["parent_serial"],
                    "refresh nesting mismatch")
            if op == "mesh-refresh-leave":
                require(before["object"]["address"] == event["object"]["address"],
                        "wrapper object identity mismatch")
                wrappers.append((before, event))
        elif op == "allocation-established":
            require(event["parent_serial"] == (stack[-1]["serial"] if stack else None),
                    "allocation parent mismatch")
            allocations.append(event["sequence"])
        else:
            require(op in ("keyword-resolved", "finalize-before", "finalize-after"),
                    "unexpected probe event: " + op)
    require(not stack and not import_active and wrappers and allocations,
            "incomplete wrappers or missing initial allocation evidence")
    return wrappers, allocations


def skipped_type7(events, wrappers):
    starts = {}
    skipped = []
    for event in events:
        if event["operation"] == "root-refresh-enter":
            starts[event["serial"]] = event
        elif event["operation"] == "root-refresh-leave":
            before = starts.pop(event["serial"])
            if before["kind"] != "all-objects":
                continue
            called_ids = {entry["object"]["id"] for entry, _ in wrappers
                          if entry["parent_serial"] == before["serial"]}
            for obj in before["state"]["objects"]:
                if obj["type"] != 7:
                    continue
                require(obj["id"] not in called_ids, "type7 object unexpectedly refreshed")
                after = next(item for item in event["state"]["objects"] if item["id"] == obj["id"])
                require({key: value for key, value in obj.items() if key != "root_components"}
                        == {key: value for key, value in after.items() if key != "root_components"},
                        "skipped type7 storage changed")
                skipped.append({"root_refresh_serial": before["serial"], "object_id": obj["id"],
                                "storage_unchanged": True, "wrapper_called": False})
    require(not starts and skipped, "missing skipped type7 scenario")
    return skipped


def validate_object(obj, rows):
    require(len(obj["descriptors"]) == len(rows) == 53, "incomplete descriptor inventory")
    require(obj["nodes"] == obj["allocated_nodes"] >= 0
            and obj["elements"] == obj["allocated_elements"] >= 0,
            "fixture logical/container counts differ")
    require(obj["allocated_arity"] == (obj["arity"] if obj["elements"] else 0),
            "fixture allocation arity mismatch")
    for d, row in zip(obj["descriptors"], rows):
        require(d["id"] == row["state_id"]
                and d["slot_offset"] == int(row["slot_relative_to_object"], 16),
                "descriptor registration mismatch")
        require(d["storage_references"] == 1 and d["storage_pointer"] == d["bound_pointer"],
                "shared/unbound descriptor outside scope")
        descriptor_values(d)
    for name in BUFFER_NAMES:
        buffer_values(obj["buffers"][name], name)


def normalized(obj, prefixes):
    descriptors = {"node": [], "element": []}
    for d in obj["descriptors"]:
        descriptors["node" if d["slot_offset"] < 0x9e0 else "element"].append({
            "id": d["id"], "shape": {key: d[key] for key in ("count", "width", "multiplicity")},
            "previous": {key: d["old_" + key] for key in ("count", "width", "multiplicity")},
            "values": descriptor_values(d)})
    arrays = {}
    for name in BUFFER_NAMES:
        values = buffer_values(obj["buffers"][name], name)
        require(0 <= prefixes[name] <= len(values), "prefix outside allocation")
        arrays[name] = values[:prefixes[name]] + [None] * (len(values) - prefixes[name])
    return {
        "nodes": obj["nodes"], "elements": obj["elements"], "arity": obj["arity"],
        "node_allocation_count": obj["allocated_nodes"],
        "element_allocation_count": obj["allocated_elements"],
        "allocation_arity": obj["allocated_arity"], "mesh_present": bool(obj["mesh_flag"]),
        "coordinates": arrays["coordinates"], "connectivity": arrays["connectivity"],
        "direct_integer_buffers": {name[6:]: arrays[name] for name in BUFFER_NAMES
                                   if name.startswith("direct")},
        "initialized_prefixes": {(name[6:] if name.startswith("direct") else name): count
                                 for name, count in prefixes.items()},
        "descriptors": descriptors,
        "boundaries": [{"nodes": group["nodes"], "faces":
                       [group["faces"][i:i+4] for i in range(0, len(group["faces"]), 4)],
                       "owners": group["owners"]} for group in obj["boundary_groups"]],
        "total": obj["boundary_total"],
    }


def compare_values(actual, expected, path, differences):
    if isinstance(expected, dict):
        if not isinstance(actual, dict):
            differences.append({"field": path, "expected": expected, "actual": actual})
            return
        for key, value in expected.items():
            compare_values(actual.get(key), value, f"{path}.{key}", differences)
    elif isinstance(expected, list):
        if not isinstance(actual, list) or len(actual) != len(expected):
            differences.append({"field": path, "expected": expected, "actual": actual})
            return
        for index, (got, wanted) in enumerate(zip(actual, expected)):
            compare_values(got, wanted, f"{path}[{index}]", differences)
    elif isinstance(expected, float):
        if (type(actual) not in (float, int)
                or struct.pack("<d", actual) != struct.pack("<d", expected)):
            differences.append({"field": path, "expected": expected, "actual": actual})
    elif actual != expected:
        differences.append({"field": path, "expected": expected, "actual": actual})


def seed_command(command, values):
    require(all(type(value) is int or (type(value) is float and math.isfinite(value))
                for value in values), "nonfinite seed outside driver protocol")
    return command + " " + str(len(values)) + (" " + " ".join(map(repr, values)) if values else "")


def replay_object(driver, wrappers, rows):
    first = wrappers[0][0]
    obj, table = first["object"], first["prior_components_argument"]
    require(len(table) == 79 and all(type(width) is int and width >= 0 for width in table),
            "invalid initial table")
    validate_object(obj, rows)
    commands = [" ".join(map(str, table))]
    if obj["nodes"]:
        commands.append(f"nodes {obj['nodes']} 20")
    if obj["elements"]:
        commands.append(f"elements {obj['elements']} {obj['arity']} 0")
    # Initial replacement metadata/history is tested against the first BEFORE.
    # Values are inputs, including default-bearing state arrays, not predictions.
    if obj["mesh_flag"]:
        commands.append(seed_command("set_coordinates",
                                     buffer_values(obj["buffers"]["coordinates"], "coordinates")))
        commands.append(seed_command("set_connectivity",
                                     buffer_values(obj["buffers"]["connectivity"], "connectivity")))
        commands.append(f"finalize {obj['dimension']}")
    prefixes = {name: obj["buffers"][name]["allocated_items"] for name in BUFFER_NAMES}
    expected, metadata, tails = [], [], []
    identities = tuple(d["address"] for d in obj["descriptors"])
    for before, after in wrappers:
        a, b = before["object"], after["object"]
        validate_object(a, rows)
        validate_object(b, rows)
        require(a["id"] == b["id"] == obj["id"]
                and a["address"] == b["address"] == obj["address"]
                and tuple(d["address"] for d in a["descriptors"]) ==
                    tuple(d["address"] for d in b["descriptors"]) == identities,
                "object/descriptor identity changed")
        for key in ("nodes", "elements", "arity", "allocated_nodes", "allocated_elements",
                    "allocated_arity", "mesh_flag", "dimension", "type"):
            require(a[key] == b[key] == obj[key], "wrapper changed fixed fixture metadata: " + key)
        for d in a["descriptors"]:
            kind = "node" if d["slot_offset"] < 0x9e0 else "element"
            commands.append(seed_command(f"set_{kind} {d['id']}", descriptor_values(d)))
        for name in BUFFER_NAMES:
            values = buffer_values(a["buffers"][name], name)[:prefixes[name]]
            command = ("set_direct " + name[6:] if name.startswith("direct") else "set_" + name)
            commands.append(seed_command(command, values))
        commands.append("snapshot")
        expected.append(normalized(a, prefixes))
        metadata.append({"phase": "before", "serial": before["serial"]})
        commands.append("refresh " + " ".join(map(str, before["requested_components"])))
        for name in BUFFER_NAMES:
            prefixes[name] = min(prefixes[name], b["buffers"][name]["allocated_items"])
            if b["buffers"][name]["allocated_items"] > prefixes[name]:
                size = 8 if name == "coordinates" else 4
                tails.append({"serial": before["serial"], "buffer": name,
                              "unknown_from_item": prefixes[name],
                              "unknown_items": b["buffers"][name]["allocated_items"] - prefixes[name],
                              "observed_original_tail_hex":
                                  b["buffers"][name]["bytes_hex"][prefixes[name] * size * 2:],
                              "interpretation": "Observed bytes only; initialization not established"})
        commands.append("snapshot")
        expected.append(normalized(b, prefixes))
        metadata.append({"phase": "after", "serial": before["serial"]})
    process = subprocess.run([str(driver)], input="\n".join(commands) + "\n", text=True,
                             capture_output=True, timeout=30,
                             creationflags=subprocess.CREATE_NO_WINDOW)
    require(process.returncode == 0 and not process.stderr, "replacement failed: " + process.stderr)
    snapshots = [json.loads(line, parse_int=lambda text: -0.0 if text == "-0" else int(text))
                 for line in process.stdout.splitlines()]
    require(len(snapshots) == len(expected), "replay snapshot count mismatch")
    comparisons = []
    for actual, wanted, label in zip(snapshots, expected, metadata):
        differences = []
        compare_values(actual, wanted, label["phase"], differences)
        comparisons.append({**label, "matched": not differences, "differences": differences})
    return {"object_id": obj["id"], "type": obj["type"], "wrappers": len(wrappers),
            "comparison": comparisons, "observed_unknown_tails": tails,
            "matched": all(item["matched"] for item in comparisons)}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--driver", type=Path, default=ROOT / "build/Release/mesh_refresh_replay.exe")
    parser.add_argument("--capture", type=Path,
                        default=ROOT / "analysis/mesh-refresh-runtime-complete.json")
    parser.add_argument("--run-record", type=Path,
                        default=ROOT / "analysis/oracle/mesh-refresh-complete/run.json")
    parser.add_argument("--probe-template", type=Path,
                        default=ROOT / "tools/observe_mesh_refresh_complete.js")
    parser.add_argument("--fixtures", type=Path)
    parser.add_argument("--require-frozen-candidate", action="store_true")
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    driver = args.driver.resolve(strict=True)
    candidate = verify_candidate(driver, args.require_frozen_candidate)
    _, run, events, source_files, sources = provenance(
        args.capture.resolve(), args.run_record.resolve(),
        args.fixtures.resolve() if args.fixtures else None, args.probe_template.resolve())
    manifest_path = ROOT / "analysis/mesh-refresh-evaluation-cases.json"
    manifest = read(manifest_path)
    require([Path(case["path"]).name for case in manifest["cases"]] == run["ordered_imports"],
            "predeclared import order mismatch")
    for case in manifest["cases"]:
        source = (ROOT / case["path"]).resolve()
        require(source == source_files[source.name] and digest(source) == case["sha256"],
                "predeclared fixture hash mismatch")
    wrappers, allocation_events = pair_wrappers(events)
    skipped = skipped_type7(events, wrappers)
    contract = ROOT / "analysis/mesh-array-contract.json"
    rows = descriptor_map(read(contract))
    grouped = defaultdict(list)
    for before, after in wrappers:
        grouped[before["object"]["address"]].append((before, after))
    results = [replay_object(driver, pairs, rows) for pairs in grouped.values()]
    report = {
        "status": "pass" if all(item["matched"] for item in results) else "fail",
        "provenance": sources, "candidate": candidate, "driver": binding(driver),
        "comparator": binding(Path(__file__)), "cases": binding(manifest_path),
        "mesh_contract": binding(contract), "allocation_established_sequences": allocation_events,
        "skipped_type7_observations": skipped,
        "objects": results, "wrappers": len(wrappers), "after_descriptor_snapshots": len(wrappers)*53,
        "before_descriptor_metadata_guards": len(wrappers)*53,
        "comparison_scope": "Sequential mesh wrapper refreshes, with BEFORE values supplied as inputs. "
                            "AFTER values never initialize or seed the candidate.",
        "limitations": [
            "This compares storage refresh, not GEOTYP/STNOUT parsing or scientific finalization.",
            "Captured BEFORE descriptor values and known direct prefixes are seeded before each call; "
            "their input values are not independent reconstruction predictions.",
            "BEFORE shape/history, allocation metadata and boundaries guard the initial replacement "
            "and continuing sequence; all53 AFTER descriptors are compared.",
            "Initial replacement uses explicit defaults20/0 and then captured BEFORE values; this "
            "experiment does not independently test those replacement defaults.",
            "Unknown direct-buffer tails are masked to null in candidate expectations; their native "
            "raw bytes are retained only as observations and never asserted to be zero.",
            "Double values are decoded from captured raw hex and compared as binary64, preserving "
            "signed zero. Nonfinite descriptor inputs are outside this replay domain.",
            "The discovery V1 capture lacked initial allocation extents and is not sufficient for "
            "this complete comparison; V2 observes the legitimate replacement allocator returns.",
            "Private storage and unchanged logical/container counts/arity are required by this fixture.",
            "Native addresses only establish identity within the original process.",
        ],
    }
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{report['status'].upper()}: {len(wrappers)} wrappers, {len(results)} objects, "
          f"{len(wrappers)*53} AFTER descriptor snapshots")
    return 0 if report["status"] == "pass" else 1


if __name__ == "__main__":
    raise SystemExit(main())

