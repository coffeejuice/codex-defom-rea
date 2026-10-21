"""Verify the bounded integrated keyword-import trace, without running either app."""
from collections import Counter
from copy import deepcopy
import argparse
import json
from pathlib import Path
import re

from capture_oracle import digest
from verify_boundary_sort_observations import EXE_SHA, ROOT, binding, check, read, verify_files
from verify_mesh_array_observations import descriptor_map
from verify_mesh_storage_observations import fixture_blocks

DIRECTORY = ROOT / "analysis/oracle/keyword-import-training"
CAPTURE = ROOT / "analysis/keyword-import-runtime-training.json"
MANIFEST = ROOT / "analysis/keyword-import-training-cases.json"
ORDER = ["01_baseline.KEY", "02_identifiers.KEY", "03_malformed.KEY", "04_eof.KEY"]
IDS = {"OBJNAM": 144, "OBJTYP": 145, "RZ": 159, "ELMCON": 173}


def provenance(evidence, run, manifest, directory=DIRECTORY, order=ORDER):
    check(run["status"] == "completed" and run["exit_code"] == 0, "original completion")
    check(Path(run["cwd"]) == directory and run["ordered_imports"] == order, "run directory/order")
    check(Path(run["executable"]) == ROOT / "source/DEF_PRE_64.exe"
          and run["executable_sha256"] == digest(Path(run["executable"])) == EXE_SHA,
          "target executable identity")
    verify_files(run, directory)
    check((directory / "stdout.bin").stat().st_size < run["max_output_bytes"], "output bound")
    check([Path(c["path"]).name for c in manifest["cases"]] == order
          and [i["name"] for i in run["inputs"]] == order, "fixture order")
    inputs = {}
    for case, item in zip(manifest["cases"], run["inputs"]):
        source, copy = ROOT / case["path"], directory / item["name"]
        check(Path(item["source"]) == source and Path(item["copy"]) == copy, "fixture paths")
        check(case["sha256"] == digest(source) == digest(copy) == item["sha256_before"]
              == item["sha256_after"] == item["source_sha256_after"], "fixture hashes")
        inputs[item["name"]] = copy.read_bytes().replace(b"\r\n", b"\n")
        check(len(inputs[item["name"]]) < 4096 and b"\0" not in inputs[item["name"]]
              and b"\x1a" not in inputs[item["name"]], "uncovered stream domain")
    script, gate = directory.with_suffix(".js"), directory.with_suffix(".ready")
    template = ROOT / "tools/observe_keyword_import.js"
    cap = evidence["normalized_result"]
    check(Path(cap["source_path"]) == script and digest(script) == cap["source_sha256"]
          and Path(run["ready_gate"]) == gate, "probe path/hash")
    text = template.read_text(encoding="utf-8")
    check(text.count("__READY_GATE__") == 2 and script.read_text(encoding="utf-8")
          == text.replace("__READY_GATE__", gate.as_posix()), "passive template mismatch")
    return inputs


def scan(event, data, previous_end, eof):
    before, after = event["before"], event["after"]
    for side in (before, after):
        check(type(side["cursor"]) is int and type(side["count"]) is int
              and 0 <= side["cursor"] <= 4096 and -1 <= side["count"] <= 4096,
              "stream buffer bounds")
        if side["buffer_hex"] in (None, ""):
            check(side["cursor"] == side["count"] == 0, "empty buffer offset")
        else:
            check(side["buffer_hex"] == data.hex()
                  and side["cursor"] + side["count"] == len(data), "buffer contents/count")
    start = len(data) if eof else before["cursor"]
    if eof:
        check(before["buffer_hex"] == "", "post-EOF stream did not remain reset")
    elif before["buffer_hex"] is None:
        check(previous_end == 0 and before["cursor"] == 0, "initial buffer state")
    else:
        check(start >= previous_end, "stream rewound unexpectedly")
    pos = start
    while pos < len(data) and data[pos] in b" \t\n\r\v\f":
        pos += 1
    fmt, conversions, value = event["format"], event["conversions"], event["value"]
    check(fmt in ("%39s", "%i"), "unexpected fscanf format")
    if pos == len(data):
        check(conversions == -1 and after == {"cursor": 0, "count": 0, "buffer_hex": ""}
              and value == (None if fmt == "%39s" else 0), "EOF result/reset")
        return len(data), True, {"start": start, "end": len(data), "eof_reset": True}
    check(not eof, "scan succeeded after EOF")
    if fmt == "%39s":
        match = re.match(rb"\S{1,39}", data[pos:])
        end = pos + len(match[0])
        check(conversions == 1 and value == match[0].decode("ascii"), "token bytes/result")
    else:
        # Narrow integer-prefix consistency check for these small ASCII fixture values.
        match = re.match(rb"[+-]?(?:0[xX][0-9a-fA-F]+|0[0-7]*|[1-9][0-9]*)", data[pos:])
        if match:
            token = match[0]
            unsigned = token.lstrip(b"+-")
            radix = 16 if unsigned.lower().startswith(b"0x") else 8 if unsigned.startswith(b"0") else 10
            end = pos + len(token)
            check(conversions == 1 and value == int(token, radix), "integer-prefix conversion")
        else:
            end = pos + int(data[pos:pos + 1] in (b"+", b"-"))
            check(conversions == 0 and value == 0, "failed integer conversion")
    check(after["cursor"] == end and after["count"] == len(data) - end
          and after["buffer_hex"] == data.hex(), "scan cursor/remaining bytes")
    return end, False, {"start": start, "end": end, "eof_reset": False}


def state_check(state, mappings, identities, baseline):
    check(state["dimension"] == 3 and state["suppress_boundaries"] == 0,
          "root dimension/suppression changed")
    components = state["components"]
    check(len(components) == 79 and components[0] == 3
          and all(type(n) is int and n >= 0 for n in components), "root component table")
    objects = state["objects"]
    check(state["object_count"] == len(objects) <= 4
          and [o["id"] for o in objects] == list(range(1, len(objects) + 1)), "object traversal/order")
    all_addresses = set()
    clean = deepcopy(state)
    descriptor_count = 0
    for obj, normalized in zip(objects, clean["objects"]):
        identifier = obj["id"]
        check(obj["dimension"] == 3 and obj["components"] == components and obj["arity"] == 4
              and obj["element_kind"] == 2 and obj["secondary"] == 0
              and obj["type"] == (1 if identifier == 1 else 0), "object metadata")
        check(obj["nodes"] in (0, 4) and obj["elements"] in (0, 1)
              and obj["mesh_flag"] == int(obj["elements"] > 0), "mesh counts/flag")
        check(obj["coordinates"] == (baseline["RZ"]["values"] if obj["nodes"] else None)
              and obj["connectivity"] == (baseline["ELMCON"]["values"] if obj["elements"] else None),
              "primary fixture arrays")
        ds = obj["descriptors"]
        check(len(ds) == len({d["id"] for d in ds}) == len({d["slot_offset"] for d in ds}) == 53,
              "descriptor completeness")
        address_tuple = (obj["address"], tuple(d["address"] for d in ds))
        check(all(int(a, 16) > 0 for a in (address_tuple[0], *address_tuple[1])),
              "null object/descriptor address")
        if identifier in identities:
            check(identities[identifier] == address_tuple, "object/descriptor identity changed")
        identities[identifier] = address_tuple
        for address in (address_tuple[0], *address_tuple[1]):
            check(address not in all_addresses, "aliased object/descriptor identities")
            all_addresses.add(address)
        normalized.pop("address")
        for d, nd, row in zip(ds, normalized["descriptors"], mappings):
            sid, count = row["state_id"], obj["nodes"] if row["container"] == "node" else obj["elements"]
            allocated = bool(count and row["selector_minus_one_included"])
            expected = [count, components[sid], 1, 0, 1, 1] if allocated else [0, 1, 1, 0, 1, 1]
            check(d["id"] == sid and d["slot_offset"] == int(row["slot_relative_to_object"], 16)
                  and d["vtable_rva"] == int(row["vtable_address"], 16) - 0x140000000,
                  "descriptor mapping")
            check([d[k] for k in ("count", "width", "multiplicity", "old_count", "old_width", "old_multiplicity")]
                  == expected, "descriptor shape/history")
            check(d["length"] == d["count"] * d["width"] * d["multiplicity"]
                  and 0 <= d["length"] <= 512 and d["storage_references"] == 1
                  and d["bound_pointer"] == d["storage_pointer"], "storage shape/binding")
            default = 20 if sid == 8 else 1 if sid == 44 else 0
            check((d["values"] is None and int(d["storage_pointer"], 16) == 0) if not d["length"]
                  else (int(d["storage_pointer"], 16) > 0 and d["values"] == [default] * d["length"]),
                  "storage null/default values")
            for key in ("address", "storage_pointer", "bound_pointer"):
                nd.pop(key)
            descriptor_count += 1
        check(set(obj["direct"]) == {str(i) for i in range(18, 24)} | {"61"}, "direct slots")
        for key, values in obj["direct"].items():
            n = (obj["elements"] if key == "61" else obj["nodes"]) * components[int(key)]
            check(values == [0] * n if n else values is None, "direct buffer state")
        groups = obj["boundary_groups"]
        if obj["elements"]:
            check(len(groups) == 1 and obj["boundary_total"] == 4
                  and len(groups[0]["faces"]) == 16 and groups[0]["owners"] == [1] * 4
                  and all(type(n) is int and 1 <= n <= 4 for n in groups[0]["faces"]),
                  "boundary shape/ownership")
        else:
            check(groups == [] and obj["boundary_total"] == 0, "empty boundary state")
    return clean, descriptor_count


def verify(evidence, run, inputs, mappings, baseline):
    cap, params = evidence["normalized_result"], evidence["parameters"]
    check(cap["pid"] == params["pid"] == run["pid"] == 26384
          and cap["target"] == "DEF_PRE_64.exe (pid 26384)", "PID/target")
    check(cap["session_id"] == params["session_id"] and cap["state"] == "detached"
          and cap["source_kind"] == "file" and cap["device_id"] == params["device_id"] == "local"
          and cap["mode"] == params["mode"] == "attach", "capture lifecycle")
    check(cap["messages_truncated"] is False and cap["cleanup_error"] is None, "capture incomplete")
    messages, scripts = cap["messages"], cap["scripts"]
    check(len(scripts) == 1 and len(messages) == 109 and all(m["type"] == "send"
          and m["script_id"] == scripts[0]["script_id"] for m in messages), "script/events")
    first = messages[0]["payload"]
    check(first["operation"] == "hooks-installed" and first["pid"] == 26384
          and int(first["base"], 16) > 0, "hook identity")
    events = [m["payload"] for m in messages[1:]]
    check([e["sequence"] for e in events] == list(range(1, 109)), "event sequence")
    identities, snapshots, scans, imports = {}, [], [], []
    active, stage, prior_state, components = None, None, None, None
    previous_end, eof, latest_scan, keyword, selection, pending_status = 0, False, None, None, None, 0
    final_before, descriptor_total = None, 0
    for event in events:
        op, name = event["operation"], event["file"]
        before_state = prior_state
        if op == "import-before":
            check(active is None and name == ORDER[len(imports)], "import order")
            active, stage, previous_end, eof, pending_status = name, "scan-token", 0, False, 0
        else:
            check(name == active and active is not None, "event file pairing")
        if "state" in event:
            state, number = state_check(event["state"], mappings, identities, baseline)
            descriptor_total += number
            if components is None:
                components = state["components"]
            check(state["components"] == components, "component table changed")
            if op == "import-before" and prior_state is not None:
                check(state == prior_state, "state changed between imports")
            snapshots.append({"sequence": event["sequence"], "file": name, "operation": op, "state": state})
            prior_state = state
        if op == "stream-scan":
            check(stage in ("scan-token", "scan-id"), "scan phase")
            check(event["format"] == ("%39s" if stage == "scan-token" else "%i"), "scan format pairing")
            previous_end, eof, offsets = scan(event, inputs[name], previous_end, eof)
            latest_scan = event
            scans.append({k: v for k, v in event.items() if k not in ("before", "after")} | offsets
                         | {"before": event["before"], "after": event["after"]})
            if stage == "scan-token":
                stage = "final-before" if event["conversions"] == -1 else (
                    "scan-token" if event["value"].startswith("*") else "keyword")
            else:
                stage = "handler" if event["conversions"] == -1 else "selected"
                selection = None
        elif op == "keyword-resolved":
            check(stage == "keyword" and event["token"] == latest_scan["value"], "keyword pairing")
            expected = IDS.get(event["token"], 305)
            check(event["id"] == expected and event["category"] == (0 if expected == 305 else 2),
                  "keyword lookup mismatch")
            keyword = event
            stage = "scan-token" if expected == 305 else "scan-id"
        elif op == "object-selected":
            check(stage == "selected" and event["requested"] == latest_scan["value"]
                  and event["found"] is (event["requested"] > 0), "object selection pairing")
            selection, stage = event, "handler"
        elif op == "handler-after":
            check(stage == "handler" and event["id"] == keyword["id"], "handler pairing")
            expected = 0 if selection and selection["found"] else 2
            check(event["status"] == expected, "handler status")
            if expected:
                check(prior_state == before_state, "failed selector changed state")
                pending_status = expected
            elif keyword["id"] == 144:
                requested = selection["requested"]
                for old, new in zip(before_state["objects"], prior_state["objects"]):
                    old, new = deepcopy(old), deepcopy(new)
                    if old["id"] == requested:
                        old.pop("name")
                        new.pop("name")
                    check(old == new, "name update changed other tracked fields")
            stage = "scan-token"
        elif op == "finalize-before":
            check(stage == "final-before" and prior_state == before_state, "finalizer entry pairing/state")
            final_before, stage = deepcopy(event["state"]), "final-after"
        elif op == "finalize-after":
            check(stage == "final-after" and event["state"] == final_before,
                  "tracked state changed during finalizer")
            stage = "import-after"
        elif op == "import-after":
            check(stage == "import-after" and event["status"] == pending_status
                  and event["state"] == final_before, "import completion state/status")
            imports.append({"file": name, "status": event["status"],
                            "object_count": event["state"]["object_count"]})
            active, stage = None, None
        elif op != "import-before":
            raise ValueError("unexpected operation/probe error")
    check(active is None and [i["status"] for i in imports] == [0, 0, 2, 2], "missing imports")
    check(len(scans) == 41 and len(snapshots) == 32 and len(identities) == 4,
          "capture coverage mismatch")
    return {"imports": imports, "scans": scans, "snapshots": snapshots,
            "descriptor_observations": descriptor_total,
            "operation_counts": dict(Counter(e["operation"] for e in events))}


def held_out_scan(event, data, previous_end, stream_seen):
    """Check captured bytes and cursor relations without predicting new edge results."""
    before, after = event["before"], event["after"]
    for side in (before, after):
        check(type(side["cursor"]) is int and type(side["count"]) is int
              and 0 <= side["cursor"] <= 4096 and -1 <= side["count"] <= 4096,
              "held-out buffer bounds")
        if side["buffer_hex"] in (None, ""):
            check(side["cursor"] == side["count"] == 0, "held-out empty buffer offset")
        else:
            check(side["buffer_hex"] == data.hex()
                  and side["cursor"] + side["count"] == len(data), "held-out buffer identity")
    start = len(data) if before["buffer_hex"] == "" else before["cursor"]
    if before["buffer_hex"] is None:
        check(not stream_seen and previous_end == 0, "unexpected null stream buffer")
    check(start >= previous_end, "held-out stream rewind")
    reset = after["buffer_hex"] == ""
    end = len(data) if reset else after["cursor"]
    check(start <= end <= len(data) and after["buffer_hex"] is not None, "held-out scan range")
    consumed = data[start:end]
    trimmed = consumed.lstrip(b" \t\n\r\v\f")
    fmt, conversions, value = event["format"], event["conversions"], event["value"]
    check(fmt in ("%39s", "%i") and type(conversions) is int and conversions in (-1, 0, 1),
          "held-out scan format/conversions")
    if fmt == "%39s":
        if conversions == 1:
            check(trimmed and len(trimmed) <= 39 and not any(b in b" \t\n\r\v\f" for b in trimmed)
                  and value == trimmed.decode("ascii"), "held-out token bytes")
            if end < len(data) and len(trimmed) < 39:
                check(data[end] in b" \t\n\r\v\f", "short token ended before delimiter")
        else:
            check(conversions == -1 and reset and not trimmed and value is None,
                  "held-out token EOF")
    else:
        check(type(value) is int, "held-out integer value type")
        if conversions == 1:
            check(re.fullmatch(rb"[+-]?(?:0[xX][0-9a-fA-F]*|0[0-7]*|[1-9][0-9]*)", trimmed),
                  "held-out consumed integer prefix")
            unsigned = trimmed.lstrip(b"+-")
            if unsigned.lower() == b"0x":
                check(value == 0, "hex-prefix-only output consistency")
            else:
                radix = 16 if unsigned.lower().startswith(b"0x") else 8 if unsigned.startswith(b"0") else 10
                check(value == int(trimmed, radix), "held-out integer value/bytes")
        else:
            check(trimmed in (b"", b"+", b"-", b"0x", b"0X", b"+0x", b"-0x", b"+0X", b"-0X")
                  and value == 0, "failed selector output/consumption")
            if conversions == -1:
                check(reset, "integer EOF missing buffer reset")
    return end, {"start": start, "end": end, "eof_reset": reset, "consumed_hex": consumed.hex()}


def held_out_state(state, mappings, identities, root_components):
    """Validate captured shape/identity; retain unknown state outcomes verbatim."""
    check(state["dimension"] == 3 and state["suppress_boundaries"] == 0
          and state["components"] == root_components, "held-out root control baseline")
    objects = state["objects"]
    check(state["object_count"] == len(objects) <= 8 and len({o["id"] for o in objects}) == len(objects)
          and all(type(o["id"]) is int and o["id"] > 0 for o in objects), "held-out object list")
    clean, count, addresses = deepcopy(state), 0, set()
    for obj, normalized in zip(objects, clean["objects"]):
        check(obj["components"] == root_components and obj["dimension"] == 3
              and obj["arity"] in (4, 8) and obj["element_kind"] in (2, 4)
              and obj["mesh_flag"] in (0, 1), "held-out object controls")
        check(type(obj["nodes"]) is int and type(obj["elements"]) is int
              and obj["nodes"] >= 0 and obj["elements"] >= 0, "held-out mesh counts")
        for field, length in [("coordinates", obj["nodes"] * 3),
                              ("connectivity", obj["elements"] * obj["arity"])]:
            check(0 <= length <= 512 and (obj[field] is None if not length else
                  isinstance(obj[field], list) and len(obj[field]) == length), "held-out primary buffer")
        ds = obj["descriptors"]
        check(len(ds) == len({d["id"] for d in ds}) == len({d["slot_offset"] for d in ds}) == 53,
              "held-out descriptor traversal")
        identity = (obj["address"], tuple(d["address"] for d in ds))
        if obj["id"] in identities:
            check(identity == identities[obj["id"]], "held-out object/descriptor identity changed")
        identities[obj["id"]] = identity
        for address in (identity[0], *identity[1]):
            check(int(address, 16) > 0 and address not in addresses, "held-out aliased/null identities")
            addresses.add(address)
        normalized.pop("address")
        for d, nd, row in zip(ds, normalized["descriptors"], mappings):
            sid = row["state_id"]
            check(d["id"] == sid and d["slot_offset"] == int(row["slot_relative_to_object"], 16)
                  and d["vtable_rva"] == int(row["vtable_address"], 16) - 0x140000000,
                  "held-out descriptor mapping")
            check(all(type(d[k]) is int and d[k] >= 0 for k in
                      ("count", "width", "multiplicity", "old_count", "old_width", "old_multiplicity", "length")),
                  "held-out descriptor field bounds")
            check(d["length"] == d["count"] * d["width"] * d["multiplicity"] <= 512
                  and d["storage_references"] == 1 and d["bound_pointer"] == d["storage_pointer"],
                  "held-out descriptor shape/binding")
            check((d["values"] is None and int(d["storage_pointer"], 16) == 0) if not d["length"]
                  else (int(d["storage_pointer"], 16) > 0 and len(d["values"]) == d["length"]),
                  "held-out descriptor storage")
            default = 20 if sid == 8 else 1 if sid == 44 else 0
            check(d["values"] is None or d["values"] == [default] * d["length"],
                  "held-out descriptor baseline values")
            if not row["selector_minus_one_included"]:
                check(d["count"] == 0, "post-only descriptor unexpectedly allocated")
            for key in ("address", "storage_pointer", "bound_pointer"):
                nd.pop(key)
            count += 1
        check(set(obj["direct"]) == {str(i) for i in range(18, 24)} | {"61"}, "held-out direct set")
        for key, values in obj["direct"].items():
            n = (obj["elements"] if key == "61" else obj["nodes"]) * root_components[int(key)]
            check(0 <= n <= 512 and (values is None if n == 0 else values == [0] * n),
                  "held-out direct shape/default")
        check(len(obj["boundary_groups"]) <= 100, "held-out boundary group bound")
        total = 0
        for group in obj["boundary_groups"]:
            faces, owners = group["faces"], group["owners"]
            if faces is None or owners is None:
                check(faces is None and owners is None, "partially cleared boundary payload")
            else:
                check(len(faces) == 4 * len(owners) <= 512
                      and all(type(v) is int and v > 0 for v in faces + owners), "boundary payload shape")
                total += len(owners)
        check(obj["boundary_total"] == total, "held-out boundary total")
    return clean, count


def verify_held_out(evidence, run, inputs, mappings, root_components):
    cap, params = evidence["normalized_result"], evidence["parameters"]
    pid = run["pid"]
    check(type(pid) is int and pid > 0 and cap["pid"] == params["pid"] == pid
          and cap["target"] == f"DEF_PRE_64.exe (pid {pid})", "held-out target/PID")
    check(cap["session_id"] == params["session_id"] and cap["state"] == "detached"
          and cap["source_kind"] == "file" and cap["device_id"] == params["device_id"] == "local"
          and cap["mode"] == params["mode"] == "attach", "held-out lifecycle")
    check(cap["messages_truncated"] is False and cap["cleanup_error"] is None, "held-out incomplete")
    messages, scripts = cap["messages"], cap["scripts"]
    check(messages and len(scripts) == 1 and all(m["type"] == "send"
          and m["script_id"] == scripts[0]["script_id"] for m in messages), "held-out script/messages")
    first = messages[0]["payload"]
    check(first["operation"] == "hooks-installed" and first["pid"] == pid
          and int(first["base"], 16) > 0, "held-out hooks")
    events = [m["payload"] for m in messages[1:]]
    check([e["sequence"] for e in events] == list(range(1, len(events) + 1)), "held-out sequence gaps")
    active, stage, prior_state, last_scan, keyword, selection = None, None, None, None, None, None
    identities, snapshots, scans, imports = {}, [], [], []
    previous_end, seen, aggregate, descriptor_total, final_before = 0, False, 0, 0, None
    for event in events:
        op, name = event["operation"], event["file"]
        old_state = prior_state
        if op == "import-before":
            check(active is None and len(imports) < len(run["ordered_imports"])
                  and name == run["ordered_imports"][len(imports)], "held-out import order")
            active, stage, previous_end, seen, aggregate = name, "scan-token", 0, False, 0
        else:
            check(active is not None and name == active, "held-out event pairing")
        if "state" in event:
            state, n = held_out_state(event["state"], mappings, identities, root_components)
            descriptor_total += n
            if op == "import-before" and prior_state is not None:
                check(state == prior_state, "held-out inter-import state change")
            if op == "import-before" and prior_state is None:
                check(state["object_count"] == 0, "held-out initial model is not empty")
            snapshots.append({"sequence": event["sequence"], "file": name, "operation": op, "state": state})
            prior_state = state
        if op == "stream-scan":
            check(stage in ("scan-token", "scan-id") and event["format"] ==
                  ("%39s" if stage == "scan-token" else "%i"), "held-out scan pairing")
            previous_end, offsets = held_out_scan(event, inputs[name], previous_end, seen)
            seen, last_scan = True, event
            scans.append(deepcopy(event) | offsets)
            if stage == "scan-token":
                stage = "final-before" if event["conversions"] == -1 else (
                    "scan-token" if event["value"].startswith("*") else "keyword")
            else:
                selection = None
                stage = "handler" if event["conversions"] == -1 else "selected"
        elif op == "keyword-resolved":
            check(stage == "keyword" and event["token"] == last_scan["value"], "held-out keyword pairing")
            expected = IDS.get(event["token"], 305)
            check(event["id"] == expected and event["category"] == (0 if expected == 305 else 2),
                  "held-out keyword lookup")
            keyword, stage = event, "scan-token" if expected == 305 else "scan-id"
        elif op == "object-selected":
            check(stage == "selected" and event["requested"] == last_scan["value"]
                  and type(event["found"]) is bool, "held-out selector pairing")
            selection, stage = event, "handler"
        elif op == "handler-after":
            check(stage == "handler" and event["id"] == keyword["id"]
                  and type(event["status"]) is int and event["status"] in (0, 2, 3),
                  "held-out handler pairing/status domain")
            if selection is None or not selection["found"]:
                check(event["status"] == 2 and prior_state == old_state, "failed selector state/status")
            if event["status"]:
                aggregate = event["status"]
            stage = "scan-token"
        elif op == "finalize-before":
            check(stage == "final-before" and prior_state == old_state, "held-out finalizer entry")
            final_before, stage = deepcopy(event["state"]), "final-after"
        elif op == "finalize-after":
            check(stage == "final-after" and event["state"] == final_before,
                  "held-out tracked finalization change")
            stage = "import-after"
        elif op == "import-after":
            check(stage == "import-after" and event["status"] == aggregate
                  and event["state"] == final_before, "held-out import completion")
            imports.append({"file": name, "status": event["status"], "object_count": prior_state["object_count"]})
            active, stage = None, None
        elif op != "import-before":
            raise ValueError("held-out unexpected operation")
    check(active is None and len(imports) == len(inputs), "held-out missing import")
    return {"imports": imports, "scans": scans, "snapshots": snapshots,
            "descriptor_observations": descriptor_total, "unique_objects": len(identities),
            "operation_counts": dict(Counter(e["operation"] for e in events)),
            "sequenced_events": len(events), "messages_including_hook": len(messages)}


def held_out_main():
    directory = ROOT / "analysis/oracle/keyword-import-held-out"
    capture = ROOT / "analysis/keyword-import-runtime-held-out.json"
    manifest_path = ROOT / "analysis/keyword-import-evaluation-cases.json"
    check(capture.exists() and (directory / "run.json").exists(),
          "Held-out capture and completed run record are required; no report generated.")
    evidence, run, manifest = read(capture), read(directory / "run.json"), read(manifest_path)
    order = [Path(c["path"]).name for c in manifest["cases"]]
    inputs = provenance(evidence, run, manifest, directory, order)
    mappings = descriptor_map(read(ROOT / "analysis/mesh-array-contract.json"))
    training, training_run, training_manifest = read(CAPTURE), read(DIRECTORY / "run.json"), read(MANIFEST)
    training_inputs = provenance(training, training_run, training_manifest)
    baseline = fixture_blocks(ROOT / training_manifest["cases"][0]["path"])
    verify(training, training_run, training_inputs, mappings, baseline)
    root_components = training["normalized_result"]["messages"][1]["payload"]["state"]["components"]
    result = verify_held_out(evidence, run, inputs, mappings, root_components)
    checks = []
    def payload(e, operation):
        return next(m["payload"] for m in e["normalized_result"]["messages"]
                    if m["payload"]["operation"] == operation)
    def eof_scan(e):
        return next(m["payload"] for m in e["normalized_result"]["messages"]
                    if m["payload"]["operation"] == "stream-scan"
                    and m["payload"]["conversions"] == -1)
    mutations = {
        "PID": lambda e: e["normalized_result"].update(pid=0),
        "missing final event": lambda e: e["normalized_result"]["messages"].pop(),
        "scan offset": lambda e: payload(e, "stream-scan")["after"].update(cursor=4097),
        "scan buffer": lambda e: payload(e, "stream-scan")["after"].update(buffer_hex="00"),
        "EOF reset": lambda e: eof_scan(e)["after"].update(cursor=1),
        "keyword ID": lambda e: payload(e, "keyword-resolved").update(id=999),
        "selector pairing": lambda e: payload(e, "object-selected").update(requested=999),
        "descriptor mapping": lambda e: payload(e, "handler-after")["state"]["objects"][0]["descriptors"][0].update(slot_offset=0),
        "missing descriptor": lambda e: payload(e, "handler-after")["state"]["objects"][0]["descriptors"].pop(),
        "descriptor binding": lambda e: payload(e, "handler-after")["state"]["objects"][0]["descriptors"][0].update(bound_pointer="0x1234"),
        "control baseline": lambda e: payload(e, "import-before")["state"].update(suppress_boundaries=1),
        "finalizer state": lambda e: payload(e, "finalize-after")["state"]["objects"][0].update(name="tampered"),
        "import status": lambda e: payload(e, "import-after").update(status=999),
    }
    for label, mutate in mutations.items():
        altered = deepcopy(evidence)
        mutate(altered)
        try:
            verify_held_out(altered, run, inputs, mappings, root_components)
        except (ValueError, KeyError, IndexError) as error:
            checks.append({"mutation": label, "rejected": True, "reason": str(error)})
        else:
            raise ValueError("accepted held-out mutation: " + label)
    for label, mutate in [
        ("original hash", lambda e, r: r.update(executable_sha256="0" * 64)),
        ("probe hash", lambda e, r: e["normalized_result"].update(source_sha256="0" * 64)),
        ("fixture hash", lambda e, r: r["inputs"][0].update(sha256_after="0" * 64)),
        ("transcript hash", lambda e, r: next(f for f in r["files"] if f["name"] == "stdout.bin").update(sha256="0" * 64)),
    ]:
        altered, badrun = deepcopy(evidence), deepcopy(run)
        mutate(altered, badrun)
        try:
            provenance(altered, badrun, manifest, directory, order)
        except ValueError as error:
            checks.append({"mutation": label, "rejected": True, "reason": str(error)})
        else:
            raise ValueError("accepted held-out provenance mutation: " + label)
    freeze_path = ROOT / "analysis/keyword-import-candidate-freeze.json"
    freeze = read(freeze_path)
    frozen_files = []
    for item in freeze["files"]:
        archive, current = ROOT / item["archive"], ROOT / item["path"]
        check(archive.is_file() and digest(archive) == item["sha256"], "archived candidate hash mismatch")
        frozen_files.append(dict(item, archive_matches=True,
                                 current_matches=current.is_file() and digest(current) == item["sha256"]))
    report = {
        "status": "pass", "scope": "Held-out original import capture provenance and tracked-state consistency; no replacement execution.",
        "capture": binding(capture), "evidence_id": evidence["evidence_id"], "pid": run["pid"],
        "run_record": binding(directory / "run.json"), "fixture_manifest": binding(manifest_path),
        "probe": binding(directory.with_suffix(".js")), "template": binding(ROOT / "tools/observe_keyword_import.js"),
        "verifier": binding(Path(__file__)), "control_baseline": {
            "source": binding(CAPTURE), "sequence": 1, "dimension": 3, "suppress_boundaries": 0,
            "components": root_components, "training_verification": "passed in this invocation"},
        **result, "tampering_checks": checks,
        "candidate_freeze": {"manifest": binding(freeze_path), "declared_stage": freeze["stage"],
                             "declared_created_utc": freeze.get("created_utc"), "files": frozen_files},
        "limitations": [
            "No predetermined held-out statuses or process IDs are used; run/evidence identity and parser status aggregation are checked.",
            "Equality around finalization covers only emitted root/object fields; simulation scalar+990, materials and geometry state are not emitted.",
            "Descriptor mappings, shape, defaults and persistent identities are checked; complete allocation/history transitions are retained observations, not independently regenerated expected state.",
            "Short single-buffer ASCII inputs only; no general scanf grammar or overflow/locale equivalence.",
            "Sign-only and prefix-only EOF scans retain native conversion results and buffer resets; structural byte/value consistency is checked without assuming all C runtimes consume these prefixes identically.",
            "The independent training baseline establishes default controls only for these runs, not every application configuration.",
            "Frozen archive bytes are checked; declared chronology is not a trusted timestamp. Current source/binary matches are informational and do not determine original-capture provenance.",
        ],
    }
    output = ROOT / "analysis/keyword-import-held-out-observations.json"
    output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS held-out: {len(result['imports'])} imports, {len(result['scans'])} scans, "
          f"{len(result['snapshots'])} snapshots, {len(checks)} tampering checks")


def main():
    evidence, run, manifest = read(CAPTURE), read(DIRECTORY / "run.json"), read(MANIFEST)
    inputs = provenance(evidence, run, manifest)
    contract_path = ROOT / "analysis/mesh-array-contract.json"
    mappings = descriptor_map(read(contract_path))
    baseline = fixture_blocks(ROOT / manifest["cases"][0]["path"])
    result = verify(evidence, run, inputs, mappings, baseline)
    def payload(e, seq):
        return e["normalized_result"]["messages"][seq]["payload"]
    mutations = {
        "PID": lambda e: e["normalized_result"].update(pid=0),
        "missing event": lambda e: e["normalized_result"]["messages"].pop(),
        "scan offset": lambda e: payload(e, 5)["after"].update(cursor=40),
        "scan buffer bytes": lambda e: payload(e, 5)["before"].update(buffer_hex="00"),
        "EOF reset": lambda e: payload(e, 23)["after"].update(cursor=131),
        "integer value": lambda e: payload(e, 35).update(value=3),
        "failed scan conversion": lambda e: payload(e, 64).update(conversions=1),
        "keyword identity": lambda e: payload(e, 4).update(id=144),
        "selection pairing": lambda e: payload(e, 6).update(requested=2),
        "handler pairing": lambda e: payload(e, 7).update(id=144),
        "object identity": lambda e: payload(e, 12)["state"]["objects"][0].update(address="0x1234"),
        "descriptor identity": lambda e: payload(e, 12)["state"]["objects"][0]["descriptors"][0].update(address="0x1234"),
        "descriptor mapping": lambda e: payload(e, 12)["state"]["objects"][0]["descriptors"][0].update(slot_offset=0),
        "descriptor history": lambda e: payload(e, 17)["state"]["objects"][0]["descriptors"][0].update(old_count=4),
        "descriptor binding": lambda e: payload(e, 17)["state"]["objects"][0]["descriptors"][0].update(bound_pointer="0x1234"),
        "direct buffer": lambda e: payload(e, 17)["state"]["objects"][0]["direct"].update({"18": None}),
        "finalizer tracked mutation": lambda e: payload(e, 25)["state"]["objects"][0].update(name="changed"),
        "failed selector mutation": lambda e: payload(e, 66)["state"]["objects"][0].update(name="changed"),
        "import status": lambda e: payload(e, 99).update(status=0),
    }
    checks = []
    for name, change in mutations.items():
        altered = deepcopy(evidence)
        change(altered)
        try:
            verify(altered, run, inputs, mappings, baseline)
        except (ValueError, IndexError, KeyError) as error:
            checks.append({"mutation": name, "rejected": True, "reason": str(error)})
        else:
            raise ValueError("accepted mutation: " + name)
    for name, change in [
        ("original hash", lambda e, r: r.update(executable_sha256="0" * 64)),
        ("probe hash", lambda e, r: e["normalized_result"].update(source_sha256="0" * 64)),
        ("fixture hash", lambda e, r: r["inputs"][0].update(sha256_after="0" * 64)),
        ("transcript hash", lambda e, r: next(f for f in r["files"] if f["name"] == "stdout.bin").update(sha256="0" * 64)),
    ]:
        altered, badrun = deepcopy(evidence), deepcopy(run)
        change(altered, badrun)
        try:
            provenance(altered, badrun, manifest)
        except ValueError as error:
            checks.append({"mutation": name, "rejected": True, "reason": str(error)})
        else:
            raise ValueError("accepted provenance mutation: " + name)
    report = {
        "status": "pass", "scope": "Original integrated training capture provenance and tracked-subset consistency; no replacement-generated oracle.",
        "capture": binding(CAPTURE), "evidence_id": evidence["evidence_id"], "pid": 26384,
        "run_record": binding(DIRECTORY / "run.json"), "fixture_manifest": binding(MANIFEST),
        "probe": binding(DIRECTORY.with_suffix(".js")),
        "template": binding(ROOT / "tools/observe_keyword_import.js"),
        "descriptor_contract": binding(contract_path), "verifier": binding(Path(__file__)),
        "messages_including_hook": 109, "sequenced_events": 108,
        **result, "tampering_checks": checks,
        "limitations": [
            "Four training fixtures only; no held-out importer equivalence or full application claim.",
            "The tracked root state excludes simulation scalar+990, materials, geometry gate+3990 and geometry records. Equality around finalizer proves equality only for emitted fields; it does not establish a complete no-op.",
            "Single-buffer ASCII fixtures under4096 bytes with CRLF-to-LF text translation. EOF resets FILE buffer cursor tozero; normalized scan offsets retain logical end separately.",
            "Handler fgets reads and comment/unknown-line consumption are not hooked here; gaps between scan offsets are bounded fixture bytes, not independent row-reader observations.",
            "Integer-prefix checks cover these small fixture values; they do not define all native scanf overflow, locale or numeric behavior.",
            "Names, coordinates, descriptors and boundaries are emitted snapshots; name padding and primary/direct native pointer identities are not captured.",
            "Object and descriptor addresses are checked for persistence and uniqueness, then omitted from this report. Descriptor storage-to-bound-slot relationships are checked before normalization.",
            "Boundary outputs are retained and checked for shape/owner bounds and persistence; the boundary algorithm is not recomputed here.",
        ],
    }
    (ROOT / "analysis/keyword-import-observations.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: 4 imports, 41 scans, 32 snapshots, {result['descriptor_observations']} descriptors, {len(checks)} tampering checks")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--held-out", action="store_true", help="Verify completed held-out capture separately.")
    args = parser.parse_args()
    held_out_main() if args.held_out else main()
