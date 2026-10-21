"""Compare direct spatial geometry imports with retained original observations.

Only replacement replay drivers execute. Original buffers, row reads, handler
states, and completed plane classification provide the comparison oracle.
"""
import argparse
from collections import Counter
import json
import math
from pathlib import Path
import subprocess
import struct

from capture_oracle import digest
from compare_keyword_import import ROOT, EXE_SHA, require, read, binding
from compare_mesh_refresh import compare_values as differences_at, values_from_hex
from compare_mesh_storage import normalized_expected
from verify_mesh_array_observations import descriptor_map

def verify_candidate(driver, reader_driver, enforce, path):
    path = path.resolve(strict=True)
    freeze = read(path)
    current_files = []
    for item in freeze["files"]:
        require(digest(ROOT / item["archive"]) == item["sha256"],
                "frozen archive changed: " + item["archive"])
        current = ROOT / item["path"]
        current_sha = digest(current) if current.is_file() else None
        current_files.append({"path": item["path"], "sha256": current_sha,
                              "matches_freeze": current_sha == item["sha256"]})
    binaries = {Path(item["path"]).name: item["sha256"] for item in freeze["files"] if item["path"].endswith(".exe")}
    driver_matches = digest(driver) == binaries[driver.name]
    reader_matches = digest(reader_driver) == binaries[reader_driver.name]
    current_matches = all(item["matches_freeze"] for item in current_files)
    if enforce:
        require(current_matches and driver_matches and reader_matches, "candidate changed under frozen enforcement")
    return {"manifest": binding(path), "created_utc": freeze["created_utc"],
            "stage": freeze["stage"], "verified_archives": freeze["files"],
            "current_files": current_files, "current_files_match_freeze": current_matches,
            "driver_matches_freeze": driver_matches, "reader_matches_freeze": reader_matches, "frozen_candidate_required": enforce}


def provenance(path, run_path, fixtures):
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
    require(script in (directory.with_suffix(".js"),
                       directory.with_name(directory.name + "-probe.js"))
            and digest(script) == cap["source_sha256"], "probe path/hash mismatch")
    gate = Path(run["ready_gate"]).resolve()
    require(gate == directory.with_suffix(".ready"), "gate path mismatch")
    template = ROOT / "tools/observe_geometry_import.js"
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


def normalize_geometry(snapshot):
    require(snapshot["snapshot_completeness"] == "spatial-direct-vectors-classification-planes",
            "geometry snapshot not complete spatial layout")
    data = snapshot["data"]
    require(data is not None, "missing geometry slot0 object")
    def vector(item, integers):
        values = values_from_hex(item["raw_hex"], integers)
        require(len(values) == item["length"] <= item["capacity"] and item["references"] >= 1,
                "geometry COW extent/reference mismatch")
        require(all(math.isfinite(v) for v in values), "nonfinite geometry value outside scope")
        return values
    coordinates = vector(data["coordinates"], False)
    faces = vector(data["faces"], True)
    auxiliary = vector(data["face_vector_0x68"], True)
    classification = values_from_hex(data["classification"]["raw_hex"], True)
    require(len(coordinates) == data["vertices"]*3 and len(faces) == data["face_count"]*4
            and len(classification) == len(auxiliary) == data["face_count"],
            "incomplete geometry arrays")
    require(not data["face_count"] or data["classification"]["allocation_observed"],
            "classification allocation not observed")
    planes=[]
    for plane in data["plane_list"]["planes"]:
        point, normal = values_from_hex(plane["point_hex"]), values_from_hex(plane["normal_hex"])
        require(len(point)==len(normal)==3 and all(math.isfinite(v) for v in point+normal),
                "invalid plane numeric capture")
        planes.append({"id":plane["id"],"point":point,"normal":normal,
                       "point_hex":plane["point_hex"],"normal_hex":plane["normal_hex"]})
    require(len(planes) == data["plane_list"]["count"], "plane count mismatch")
    return {"enabled_0x3990":snapshot["enabled_0x3990"],"flag_0x3994":snapshot["flag_0x3994"],
            "vertices":data["vertices"],"face_count":data["face_count"],
            "coordinates":coordinates,"coordinates_hex":data["coordinates"]["raw_hex"],
            "faces":faces,"classification":classification,"auxiliary68":auxiliary,"planes":planes}


def normalize_table(table):
    fields = ("type_0x00","rank_0x04","constant_0x08","selectors_0x10","counts_0x1c",
              "values_per_point_0x40","flag_0x50","preserved_0x54","mode_0x58",
              "preserved_0x60_hex","axes","dependent")
    require(table["type_0x00"] == 128 and table["rank_0x04"] in (0,1),
            "time table type/rank outside selected scope")
    result = {key:table[key] for key in fields}
    result["constant_0x08"] = struct.unpack("<d",bytes.fromhex(table["raw_hex"]["constant"]))[0]
    result["axes"] = [values_from_hex(encoded) for encoded in table["raw_hex"]["axes"]]
    result["dependent"] = values_from_hex(table["raw_hex"]["dependent"])
    require(all(math.isfinite(value) for values in result["axes"] + [result["dependent"]]
                for value in values), "nonfinite table value outside compatibility domain")
    require(len(result["axes"]) == 3 and len(result["counts_0x1c"]) == 3,
            "incomplete axis metadata")
    return result


class RootIdentity:
    """Track identity within an observed dimension generation, never across resets."""
    def __init__(self, descriptor_rows):
        self.rows = descriptor_rows
        self.roots = {}

    def normalize(self, state):
        address, dimension = state["address"], state["dimension"]
        require(int(address, 16) > 0 and dimension in (2, 3), "invalid root identity/dimension")
        tracked = self.roots.setdefault(address, {
            "dimension": dimension, "generation": 0, "identities": {}})
        if tracked["dimension"] != dimension:
            tracked.update(dimension=dimension, generation=tracked["generation"] + 1,
                           identities={})
        require(state["suppress_boundaries"] in (0, 1) and state["alternate_enabled"] in (0, 1)
                and len(state["components"]) == 79
                and state["components"][0] == dimension, "incomplete root controls")
        require(state["node8_default"] == (20 if state["process_units_raw"] == 0 else 68),
                "units/default mapping mismatch")
        require(type(state["geometry_mode"]) is int and len(state["output_flags"]) == 7
                and all(type(value) is int for value in state["output_flags"]),
                "incomplete simulation controls")
        require(state["material_count"] == 0 and state["nstart_option_0x1740"] == 0,
                "material or NSTART-option branch outside selected finalization scope")
        require(state["import_flag_0x2f88"] in (0, 1)
                and state["import_flag_0x2f89"] in (0, 1),
                "invalid import flags")
        require(set(state["finalization_controls"]) == {
            "step_number_0x8b0", "saved_step_0x744", "step_increment_0x930",
            "control_0x93c", "mode_0x990", "time_step_0x920",
            "time_step_secondary_0x928", "selected_object_0x8b8", "pdie_secondary_0x1104"}, "incomplete finalization controls")
        objects = state["objects"]
        require(state["object_count"] == len(objects)
                and [obj["id"] for obj in objects] == list(range(1, len(objects) + 1)),
                "object list/count mismatch")
        result = {key: state[key] for key in ("dimension", "suppress_boundaries",
                  "components", "object_count", "alternate_enabled", "node8_default",
                  "geometry_mode", "output_flags", "finalization_controls", "time_table",
                  "import_flag_0x2f88", "import_flag_0x2f89")}
        result["time_table"] = normalize_table(state["time_table"])
        result["objects"] = []
        for obj in objects:
            require(obj["flag_0x3990"] in (0,1), "invalid geometry finalization flag")
            require(obj["dimension"] == dimension and obj["components"] == state["components"],
                    "object/root controls differ")
            require(obj["name"].isascii(), "non-ASCII CString outside fixture scope")
            descriptors = obj["descriptors"]
            require(len(descriptors) == len(self.rows) == 53, "incomplete descriptor inventory")
            identity = (obj["address"], tuple(d["address"] for d in descriptors))
            require(int(identity[0], 16) > 0, "null object identity")
            previous = tracked["identities"].setdefault(obj["id"], identity)
            require(previous == identity, "object/descriptor changed inside dimension generation")
            for descriptor, row in zip(descriptors, self.rows):
                require(descriptor["id"] == row["state_id"]
                        and descriptor["slot_offset"] == int(row["slot_relative_to_object"], 16),
                        "descriptor registration mismatch")
                require(descriptor["storage_references"] == 1
                        and descriptor["bound_pointer"] == descriptor["storage_pointer"],
                        "shared storage or broken descriptor binding")
                require(descriptor["length"] == descriptor["count"] * descriptor["width"]
                        * descriptor["multiplicity"] == len(descriptor["values"] or []),
                        "descriptor extent/value mismatch")
            normalized = normalized_expected(obj)
            # The new probe includes persisted planar boundary nodes. Temporary
            # builder ranges/chains are not stored in the original object snapshot.
            for group, captured in zip(normalized["boundaries"], obj["boundary_groups"]):
                group["nodes"] = captured["nodes"] or []
            normalized.update({key: obj[key] for key in (
                "node_allocation_count", "element_allocation_count", "allocation_arity",
                "initialized_prefixes")})
            require(obj["node_allocation_count"] == obj["nodes"]
                    and obj["element_allocation_count"] == obj["elements"],
                    "fixture logical/allocation counts differ")
            require(obj["allocation_arity"] == (obj["arity"] if obj["elements"] else 0),
                    "fixture allocation arity differs")
            for descriptor, original in zip(
                    normalized["descriptors"]["node"] + normalized["descriptors"]["element"],
                    descriptors):
                values = values_from_hex(original["values_hex"])
                require(len(values) == original["length"], "descriptor raw extent mismatch")
                require(all(math.isfinite(value) for value in values),
                        "nonfinite descriptor values outside fixture scope")
                descriptor["values"] = values
            require(set(obj["buffer_metadata"]) == {
                "coordinates", "connectivity", "direct18", "direct19",
                "direct20", "direct21", "direct22", "direct23", "direct61"},
                "incomplete buffer metadata")
            for name, buffer in obj["buffer_metadata"].items():
                extent, prefix = buffer["allocated_items"], buffer["known_prefix"]
                require(type(extent) is int and type(prefix) is int
                        and 0 <= prefix <= extent <= 512
                        and buffer["unknown_items"] == extent-prefix,
                        "unknown or invalid tracked allocation extent")
                key = name[6:] if name.startswith("direct") else name
                require(obj["initialized_prefixes"][key] == prefix, "prefix metadata mismatch")
                values = values_from_hex(buffer["known_hex"], name != "coordinates")
                require(len(values) == prefix, "known raw prefix extent mismatch")
                require(all(math.isfinite(value) for value in values),
                        "nonfinite known buffer values outside fixture scope")
                masked = values + [None] * (extent-prefix)
                if name.startswith("direct"):
                    normalized["direct_integer_buffers"][key] = masked
                else:
                    require(name in ("coordinates", "connectivity"), "unexpected raw buffer")
                    normalized[name] = masked
            normalized.update({key: obj[key] for key in ("id", "type", "secondary", "element_kind", "flag_0x3990")})
            normalized["stroke_0x22d0_hex"] = obj["stroke_hex"]
            normalized["stroke_0x22d0"] = values_from_hex(obj["stroke_hex"])
            require(len(normalized["stroke_0x22d0"]) == 3 and all(math.isfinite(v) for v in normalized["stroke_0x22d0"]), "invalid object stroke outside scope")
            normalized["geometry"] = normalize_geometry(obj["geometry"])
            require(normalized["geometry"]["enabled_0x3990"] == obj["flag_0x3990"], "object geometry gate mismatch")
            normalized["name_hex"] = obj["name"].encode("ascii").hex()
            result["objects"].append(normalized)
        linked = state["linked_state"]
        normalized_linked = self.normalize(linked) if linked is not None else None
        return {"state": result, "generation": tracked["generation"],
                "linked_state": normalized_linked["state"] if normalized_linked else None,
                "linked_generation": normalized_linked["generation"] if normalized_linked else None}


def verify_stages(item):
    before = item["parser_state"]["state"]
    after = item["finalized_state"]["state"]
    active = [obj["id"] for obj in before["objects"] if obj["type"] != 7]
    expected = [("stage-before", "control"), ("stage-after", "control"),
                ("stage-before", "materials"), ("stage-after", "materials"),
                ("stage-before", "objects")]
    for object_id in active:
        expected += [("object-stage-before", object_id), ("object-stage-after", object_id)]
    expected.append(("stage-after", "objects"))
    observed = [(event["operation"], event.get("stage", event.get("id")))
                for event in item["stages"]]
    require(observed == expected, "finalization stage ordering or active-object visits differ")
    for event in item["stages"]:
        require(event["root"] == item["root_address"]
                and event["finalizer_serial"] == item["finalizer_serial"],
                "stage root/finalizer identity mismatch")
        if event["operation"].startswith("object-"):
            require(event["flag_0x3990"] in (0,1) and event["option"] == 0
                    and event["type"] != 7, "unexpected object finalization branch")
        else:
            require(event["material_count"] == 0 and event["nstart_option_0x1740"] == 0,
                    "stage outside empty-material/default-option scope")
            for key in ("step_number_0x8b0", "saved_step_0x744"):
                require(event["finalization_controls"][key] == before["finalization_controls"][key],
                        "NSTART normalization occurred before stage completion")
            if event["stage"] == "objects":
                require(event["option"] == 0, "unexpected object-list finalization option")
    controls = before["finalization_controls"]
    final = after["finalization_controls"]
    seen = before["import_flag_0x2f88"]
    start = controls["step_number_0x8b0"]
    normalized_start = -(start + 1) if seen and start >= 0 else start
    saved = normalized_start if seen else controls["saved_step_0x744"]
    require(final["step_number_0x8b0"] == normalized_start and final["saved_step_0x744"] == saved,
            "original NSTART finalization differs from retained scalar rule")
    mode = (1 if controls["step_increment_0x930"] <= 0 else 0
            ) if controls["control_0x93c"] == 0 else controls["mode_0x990"]
    require(final["mode_0x990"] == mode, "original control finalization differs")
    require(all(after[key] == before[key] for key in
                ("import_flag_0x2f88", "import_flag_0x2f89")),
            "finalizer changed import flags")


def retained_imports(events, order, descriptor_rows):
    imports, current, previous_after = [], None, None
    identities = RootIdentity(descriptor_rows)
    for event in events:
        op = event["operation"]
        if op == "import-before":
            require(current is None, "nested import")
            before = identities.normalize(event["state"])
            require(before == previous_after if previous_after is not None
                    else before["state"]["object_count"] == 0, "state changed between imports")
            current = {"file": event["file"], "tokens": [], "before": before,
                       "root_address": event["state"]["address"], "stages": [], "flag_resets": [],
                       "rows": [], "geometry_rows": [], "geometry_events": [], "header_rows": [], "mixed_rows": [], "lookups": [], "table_events": [], "handler_states": [], "handler_before": [],
                       "start_sequence": event["sequence"]}
            continue
        require(current is not None and current["file"] == event["file"],
                "event outside current import")
        state = identities.normalize(event["state"]) if "state" in event and not op.startswith("geometry-") else None
        if op == "keyword-resolved":
            require(len(current["flag_resets"]) == 2, "keyword resolved before flag reset")
            current["tokens"].append({
                "token": event["token"], "id": event["id"], "category": event["category"],
                "handler_status": None,
                "unknown_warning": event["id"] == 305 and event["token"] != "SRFNRG"})
            require(((event["id"] in (144, 145, 153, 158, 159, 173) and event["category"] == 2)
                     or (event["id"] in (28, 34, 38, 48, 49, 50, 51) and event["category"] == 1)
                     or (event["id"] == 305 and event["category"] == 0)),
                    "fixture exceeded selected keyword scope")
        elif op == "handler-after":
            require(current["tokens"] and current["tokens"][-1]["id"] == event["id"]
                    and current["tokens"][-1]["category"] == event["category"]
                    and current["tokens"][-1]["handler_status"] is None, "handler/token mismatch")
            current["tokens"][-1]["handler_status"] = event["status"]
            if event["id"] in (48, 49, 50, 153):
                require("pending_real_row" in current,
                        "time handler must consume exactly one captured real helper")
                row = current.pop("pending_real_row")
                require(row["kind"] == ({48:"real1",49:"realint",50:"real2",153:"real2" if state["state"]["dimension"] == 2 else "real3"}[event["id"]]),
                        "handler/helper pairing mismatch")
                cursor = row["logical_before"] + sum(
                    len(bytes.fromhex(line["hex"])) for line in row["lines"] if not line["eof"])
            else:
                require("pending_real_row" not in current,
                        "unexpected real helper inside configuration/object handler")
                cursor = event["logical_handler_cursor"]
            current["handler_states"].append({"id":event["id"],"category":event["category"],
                "status":event["status"],"cursor":cursor,
                "object_strokes":[{key:obj[key] for key in ("id","type","stroke_0x22d0","stroke_0x22d0_hex")} for obj in state["state"]["objects"]],
                "object_geometries":[{key:obj[key] for key in ("id","type","geometry")} for obj in state["state"]["objects"]],
                "dimension":state["state"]["dimension"],
                **{key:state["state"][key] for key in
                   ("finalization_controls","import_flag_0x2f88","import_flag_0x2f89","time_table")}})
        elif op == "geometry-row":
            require(current["tokens"][-1]["id"] == 158, "geometry row outside DIEGEO")
            current["geometry_rows"].append(event)
        elif op in ("geometry-flag-before","geometry-flag-after","geometry-access",
                    "geometry-replace-before","geometry-replace-after",
                    "geometry-classification-allocated","geometry-normal"):
            current["geometry_events"].append(event)
        elif op == "step-object-lookup":
            require(current["tokens"][-1]["id"] == 49, "lookup outside step handler")
            current["lookups"].append(event)
        elif op == "handler-before":
            require(current["tokens"] and current["tokens"][-1]["id"] == event["id"],
                    "handler entry/token mismatch")
            current["handler_before"].append({"id":event["id"],"category":event["category"],
                "sequence":event["sequence"],"state":state["state"]})
        elif op in ("header-row", "selection-row"):
            require(current["tokens"][-1]["id"] == (51 if op == "header-row" else 38), "integer pair outside expected handler")
            current["header_rows"].append(event)
        elif op in ("table-scan","table-discard","table-reader-before","table-reader-after",
                    "table-reset-before","table-reset-after"):
            require(current["tokens"][-1]["id"] == 51, "table event outside DTTYPE")
            current["table_events"].append(event)
        elif op in ("real-row", "mixed-row"):
            require(current["tokens"] and current["tokens"][-1]["handler_status"] is None,
                    "real helper outside active handler")
            require("pending_real_row" not in current, "multiple real helpers in one handler")
            current["pending_real_row"] = event
            current["mixed_rows" if op == "mixed-row" else "rows"].append(event)
        elif op == "finalize-before":
            require("parser_state" not in current and len(current["flag_resets"]) == 2,
                    "duplicate finalization or missing flag reset")
            current["parser_state"] = state
            current["finalize_before_sequence"] = event["sequence"]
            current["finalizer_serial"] = event["finalizer_serial"]
        elif op == "finalize-after":
            require("parser_state" in current and "finalized_state" not in current,
                    "unpaired finalization")
            require(event["finalizer_serial"] == current["finalizer_serial"],
                    "finalizer serial mismatch")
            current["finalized_state"] = state
            current["finalize_after_sequence"] = event["sequence"]
        elif op in ("flags-reset-before", "flags-reset-after"):
            wanted = "flags-reset-before" if not current["flag_resets"] else "flags-reset-after"
            require(op == wanted and len(current["flag_resets"]) < 2
                    and not current["tokens"] and "parser_state" not in current,
                    "flag reset ordering mismatch")
            require(event["root"] == current["root_address"]
                    and event["finalization_controls"] == current["before"]["state"]["finalization_controls"],
                    "flag reset changed controls or root identity")
            for key in ("import_flag_0x2f88", "import_flag_0x2f89"):
                wanted_flag = current["before"]["state"][key] if op.endswith("before") else 0
                require(event[key] == wanted_flag, "flag reset value mismatch")
            current["flag_resets"].append(event)
        elif op in ("stage-before", "stage-after", "object-stage-before", "object-stage-after"):
            require("parser_state" in current and "finalized_state" not in current,
                    "stage outside active finalizer")
            current["stages"].append(event)
        elif op == "import-after":
            require("finalized_state" in current and state == current["finalized_state"],
                    "incomplete finalization/import state")
            statuses = [token["handler_status"] for token in current["tokens"]
                        if token["handler_status"] not in (None, 0)]
            require(event["status"] == (statuses[-1] if statuses else 0),
                    "status differs from last handler error")
            current.update(status=event["status"], after=state, end_sequence=event["sequence"])
            verify_stages(current)
            imports.append(current)
            previous_after, current = state, None
        elif op not in ("stream-scan", "object-selected", "allocation-update", "allocation-free"):
            raise ValueError("unexpected probe event: " + op)
    require(current is None and [item["file"] for item in imports] == order,
            "incomplete import order")
    return imports


def decode_raw_controls(value):
    """Decode original raw doubles before JSON numeric representations can erase -0."""
    if isinstance(value, list):
        for item in value:
            decode_raw_controls(item)
    elif isinstance(value, dict):
        if "time_controls_hex" in value:
            require(set(value["time_controls_hex"]) == {
                "step_increment_0x930", "time_step_0x920", "time_step_secondary_0x928"},
                "incomplete raw scalar capture")
            controls = dict(value["finalization_controls"])
            # Corrected address arithmetic: root1104 is controls+9b4, an
            # independent int32. The preceding four bytes are not a double.
            controls.pop("control_unit_0x84c")
            adjacent = bytes.fromhex(controls.pop("pdie_alias_0x1100_hex"))
            require(len(adjacent) == 8, "incomplete raw1100 observation")
            controls["pdie_secondary_0x1104"] = struct.unpack("<i", adjacent[4:])[0]
            for key, encoded in value["time_controls_hex"].items():
                require(len(encoded) == 16, "raw double length mismatch")
                number = struct.unpack("<d", bytes.fromhex(encoded))[0]
                require(math.isfinite(number) and number == controls[key],
                        "raw scalar differs from numeric observation or is nonfinite")
                controls[key] = number
            value["finalization_controls"] = controls
        for item in value.values():
            decode_raw_controls(item)


def compare_helpers(imports, source_files, driver):
    rows = sorted([row for item in imports for row in item["rows"] + item["header_rows"] + item["mixed_rows"] + item["geometry_rows"]], key=lambda row:row["sequence"])
    commands, expected = [], []
    for row in rows:
        require(len(row["lines"]) == 1 and row["lines"][0]["limit"] == 1024,
                "unexpected real-helper fgets calls")
        line = row["lines"][0]
        data = b"" if line["eof"] else bytes.fromhex(line["hex"])
        require(len(row["scans"]) == (0 if line["eof"] else 1), "real sscanf count mismatch")
        if row["scans"]:
            scan = row["scans"][0]
            require(scan["format"] == ({"real1":"%Flf", "real2":"%Flf %Flf", "real3":"%Flf %Flf %Flf", "int2":"%i %i", "realint":"%Flf %i", "geometry-header3":"%i %i %i", "geometry-coord3":"%i %Flf %Flf %Flf", "geometry-face5":"%i %i %i %i %i"}[row["kind"]])
                    and bytes.fromhex(scan["input_hex"]) == data.replace(b",", b" "),
                    "real scanner format or comma replacement mismatch")
        fixture = source_files[row["file"]].read_bytes().replace(b"\r\n", b"\n")
        offset = row["logical_before"]
        require(data == fixture[offset:offset + len(data)],
                "observed helper line does not match fixture at stream cursor")
        logical_after = offset + len(data)
        native_after = row["after"]
        require(native_after["cursor"] == logical_after or (
                    logical_after == len(fixture) and native_after["cursor"] == 0
                    and native_after["count"] == 0 and native_after["buffer_hex"] == ""),
                "helper stream cursor is neither consumed extent nor observed exhausted-buffer reset")
        kind = {"geometry-header3":"int3", "geometry-coord3":"coord3", "geometry-face5":"int5"}.get(row["kind"],row["kind"])
        commands.append(f"{kind} 0 0 {data.hex() or '-'} -")
        expected.append({"kind":kind, "status":row["status"],
            "cursor":len(data), "consumed_hex":data.hex(),
            "conversions":row["scans"][0]["conversions"] if row["scans"] else -1,
            "values":[struct.unpack("<i" if row["kind"] in ("int2","geometry-header3","geometry-face5") or (row["kind"] == "realint" and i == 1) or (row["kind"] == "geometry-coord3" and i == 0) else "<d",bytes.fromhex(h))[0] for i,h in enumerate(row["output_hex"])],
            "lines":[{"offset":0,"cursor":len(data),"hex":data.hex(),"status":int(line["eof"])}]})
    process = subprocess.run([str(driver)], input="\n".join(commands)+"\n", text=True,
                             capture_output=True, timeout=15, creationflags=subprocess.CREATE_NO_WINDOW)
    require(process.returncode == 0 and not process.stderr, "row replay failed: "+process.stderr)
    actual = [json.loads(line,parse_int=lambda text: -0.0 if text == "-0" else int(text))
              for line in process.stdout.splitlines()]
    require(len(actual) == len(rows), "row replay count mismatch")
    results = []
    for row, wanted, got in zip(rows,expected,actual):
        differences = []
        differences_at(got, wanted, "reader", differences)
        results.append({"file":row["file"],"sequence":row["sequence"],"kind":row["kind"],
            "input_lines":row["lines"],"original_scans":row["scans"],
            "original_output_hex":row["output_hex"],"status":row["status"],
            "original_cursor_before":row["before"]["cursor"],
            "original_buffer_cursor_after":row["after"]["cursor"],
            "original_logical_cursor_after":row["logical_before"] +
                sum(len(bytes.fromhex(line["hex"])) for line in row["lines"] if not line["eof"]),
            "differences":differences,"matched":not differences})
    return {"status":"pass" if all(row["matched"] for row in results) else "fail",
            "rows":len(rows),"calls":results}


def compare_stream_helpers(imports, source_files, driver):
    scans = [event for item in imports for event in item["table_events"]
             if event["operation"] == "table-scan"]
    commands, expected = [], []
    for event in scans:
        fixture = source_files[event["file"]].read_bytes().replace(b"\r\n", b"\n")
        def logical(position):
            if position["cursor"] == 0 and position["count"] == 0 and position["buffer_hex"] == "":
                return len(fixture)
            require(position["buffer_hex"] is not None
                    and bytes.fromhex(position["buffer_hex"]) == fixture,
                    "table stream buffer differs from bounded fixture")
            return position["cursor"]
        before, after = logical(event["before"]), logical(event["after"])
        require(0 <= before <= after <= len(fixture), "nonmonotonic stream scan")
        require(event["format"] == "%Flf", "unexpected table scan format")
        commands.append(f"streamreal 0 {before} {fixture.hex()} {event['previous_output_hex']}")
        expected.append({"kind":"streamreal","status":int(event["conversions"] == -1),
            "conversions":event["conversions"],"cursor":after,"consumed_hex":fixture[before:after].hex(),
            "values":[struct.unpack("<d",bytes.fromhex(event["output_hex"]))[0]],"lines":[]})
    process = subprocess.run([str(driver)],input="\n".join(commands)+"\n", text=True,
        capture_output=True,timeout=15,creationflags=subprocess.CREATE_NO_WINDOW)
    require(process.returncode == 0 and not process.stderr,"stream replay failed: "+process.stderr)
    actual = [json.loads(line,parse_int=lambda text:-0.0 if text == "-0" else int(text))
              for line in process.stdout.splitlines()]
    require(len(actual) == len(scans),"stream replay count mismatch")
    results = []
    for event,wanted,got in zip(scans,expected,actual):
        differences=[]
        differences_at(got,wanted,"streamreal",differences)
        results.append({"file":event["file"],"sequence":event["sequence"],
            "original":wanted,"previous_output_hex":event["previous_output_hex"],
            "output_hex":event["output_hex"],"matched":not differences,"differences":differences})
    return {"status":"pass" if all(item["matched"] for item in results) else "fail",
            "calls":results,"count":len(results)}


def compare(args):
    driver = args.driver.resolve(strict=True)
    reader_driver = args.reader_driver.resolve(strict=True)
    candidate = verify_candidate(driver, reader_driver, args.require_frozen_candidate, args.freeze)
    evidence, run, events, source_files, sources = provenance(
        args.capture.resolve(), args.run_record.resolve(),
        args.fixtures.resolve() if args.fixtures else None)
    decode_raw_controls(events)
    for index, event in enumerate(events):
        if event["operation"] == "handler-after":
            observed = event["stream"]
            fixture = source_files[event["file"]].read_bytes().replace(b"\r\n", b"\n")
            if observed["cursor"] == 0 and observed["count"] == 0 and observed["buffer_hex"] == "":
                event["logical_handler_cursor"] = len(fixture)
            else:
                require(observed["buffer_hex"] is not None
                        and bytes.fromhex(observed["buffer_hex"]) == fixture,
                        "handler stream buffer must cover bounded complete fixture")
                event["logical_handler_cursor"] = observed["cursor"]
        if event["operation"] in ("real-row", "header-row", "mixed-row", "selection-row", "geometry-row"):
            before = event["before"]
            if before["cursor"] == 0 and before["count"] == 0 and before["buffer_hex"] == "":
                require(event["lines"] == [{"limit":1024,"eof":True,"hex":None}],
                        "empty entry buffer must be observed EOF in bounded fixture")
                event["logical_before"] = len(source_files[event["file"]].read_bytes().replace(b"\r\n", b"\n"))
            else:
                event["logical_before"] = before["cursor"]
    manifest = read(args.cases)
    cases = manifest["cases"]
    require([Path(case["path"]).name for case in cases] == run["ordered_imports"],
            "predeclared case order mismatch")
    for case in cases:
        path = (ROOT / case["path"]).resolve()
        require(path == source_files[path.name] and digest(path) == case["sha256"],
                "predeclared fixture hash mismatch")
    require(manifest["probe"] == binding(ROOT / "tools/observe_geometry_import.js"),
            "predeclared probe hash mismatch")
    contract = ROOT / "analysis/mesh-array-contract.json"
    imports = retained_imports(events, run["ordered_imports"], descriptor_map(read(contract)))
    stdout = (args.run_record.parent / "stdout.bin").read_text(encoding="ascii")
    warning_counts = Counter(token["token"] for item in imports for token in item["tokens"]
                             if token["unknown_warning"])
    for token, count in warning_counts.items():
        require(stdout.count(f"WARNING: Keyword {token} not found. Skip ") == count,
                "unknown warning transcript mismatch")
    require("WARNING: Keyword SRFNRG not found." not in stdout, "silent-keyword warning mismatch")
    initial = imports[0]["before"]
    controls = initial["state"]
    commands = [f"initialized {controls['dimension']} {controls['node8_default']} "
                f"{controls['suppress_boundaries']}"]
    require(initial["generation"] == 0 and controls["alternate_enabled"] == 0,
            "unexpected initial generation or enabled alternate dispatch")
    if initial["linked_state"] is not None:
        require(initial["linked_generation"] == 0
                and initial["linked_state"]["object_count"] == 0,
                "initial linked root outside empty-profile replay scope")
        commands.append(f"link {initial['linked_state']['dimension']}")
    translated = []
    for item in imports:
        raw = source_files[item["file"]].read_bytes()
        require(b"\0" not in raw and b"\x1a" not in raw, "unsupported text translation")
        data = raw.replace(b"\r\n", b"\n")
        translated.append(data)
        commands.append("import " + (data.hex() or "-"))
    process = subprocess.run([str(driver)], input="\n".join(commands) + "\n", text=True,
                             capture_output=True, timeout=30,
                             creationflags=subprocess.CREATE_NO_WINDOW)
    require(process.returncode == 0 and not process.stderr,
            "replacement failed: " + process.stderr)
    actual = [json.loads(line, parse_int=lambda text: -0.0 if text == "-0" else int(text))
              for line in process.stdout.splitlines()]
    require(len(actual) == len(imports), "driver result count mismatch")
    comparisons = []
    for got, item, data, case in zip(actual, imports, translated, cases):
        expected = item["finalized_state"]
        differences = []
        expected_result = {
            "outcome": "parsed", "status": item["status"], "cursor": len(data),
            # Initialized empty-material profile now executes the recovered
            # root finalizer; its new scalar fields are evaluated separately.
            "reached_eof": True, "finalization_pending": False,
            "unsupported_reason": "", "events": item["tokens"]}
        differences_at(got.get("result"), expected_result, "result", differences)
        differences_at(got.get("before_state"), item["before"]["state"],
                       "before_state", differences)
        for key in ("state", "generation", "linked_state", "linked_generation"):
            differences_at(got.get(key), expected[key], key, differences)
        differences_at(got.get("stable_identity"), True, "stable_identity", differences)
        differences_at(got.get("handler_states"), item["handler_states"],
                       "handler_states", differences)
        comparisons.append({
            "file": item["file"], "sequence_range": [item["start_sequence"], item["end_sequence"]],
            "finalize_before_sequence": item["finalize_before_sequence"],
            "finalize_after_sequence": item["finalize_after_sequence"],
            "comparison_phase": "after-finalization",
            "before_dimension": item["before"]["state"]["dimension"],
            "dimension": expected["state"]["dimension"], "generation": expected["generation"],
            "geometry_mode": expected["state"]["geometry_mode"],
            "output_flags": expected["state"]["output_flags"],
            "linked_dimension": expected["linked_state"]["dimension"]
                if expected["linked_state"] else None,
            "linked_generation": expected["linked_generation"],
            "objects": expected["state"]["object_count"],
            "unknown_direct_items": sum(
                values.count(None)
                for obj in expected["state"]["objects"]
                for values in obj["direct_integer_buffers"].values()),
            "tokens": len(item["tokens"]), "status": item["status"],
            "pre_finalization_controls": item["parser_state"]["state"]["finalization_controls"],
            "post_finalization_controls": expected["state"]["finalization_controls"],
            "pre_finalization_import_flags": {
                key: item["parser_state"]["state"][key]
                for key in ("import_flag_0x2f88", "import_flag_0x2f89")},
            "post_finalization_import_flags": {
                key: expected["state"][key]
                for key in ("import_flag_0x2f88", "import_flag_0x2f89")},
            "flag_reset_sequences": [event["sequence"] for event in item["flag_resets"]],
            "observed_stages": [
                {"sequence": event["sequence"], "operation": event["operation"],
                 "stage": event.get("stage"), "object_id": event.get("id")}
                for event in item["stages"]],
            "handler_states": item["handler_states"],
            "handler_before": item["handler_before"],
            "table_events": item["table_events"],
            "lookups": item["lookups"],
            "geometry_events": item["geometry_events"],
            "geometry_before_finalization":[{"id":obj["id"],"geometry":obj["geometry"]} for obj in item["parser_state"]["state"]["objects"]],
            "geometry_after_finalization":[{"id":obj["id"],"geometry":obj["geometry"]} for obj in item["finalized_state"]["state"]["objects"]],
            "hypothesis": case.get("hypothesis", case.get("question")),
            "unknown_keyword_warnings": [token["token"] for token in item["tokens"] if token["unknown_warning"]],
            "object_types_before": [obj["type"] for obj in item["before"]["state"]["objects"]],
            "object_types_after": [obj["type"] for obj in expected["state"]["objects"]],
            "original_finalization_changed_tracked_subset":
                item["parser_state"] != item["finalized_state"],
            "differences": differences, "matched": not differences})
    helper_comparison = compare_helpers(imports, source_files, reader_driver)
    stream_comparison = compare_stream_helpers(imports, source_files, reader_driver)
    return {
        "status": "pass" if all(item["matched"] for item in comparisons)
                  and helper_comparison["status"] == "pass"
                  and stream_comparison["status"] == "pass" else "fail",
        "stream_comparison": stream_comparison,
        "helper_comparison": helper_comparison, "reader_driver": binding(reader_driver),
        "provenance": sources, "cases": binding(args.cases), "candidate": candidate,
        "driver": binding(driver), "comparator": binding(Path(__file__)),
        "mesh_contract": binding(contract),
        "explicit_initial_controls": initial,
        "startup_protocol": "initialized DIMENSION NODE8_DEFAULT SUPPRESS_BOUNDARIES; no unit parameter or injected native state",
        "imports": comparisons,
        "coverage": {
            "deactivation_observed": any(
                before["id"] == after["id"] and before["type"] != 7 and after["type"] == 7
                for item in imports for before in item["before"]["state"]["objects"]
                for after in item["finalized_state"]["state"]["objects"]),
            "type7_final_snapshots": sum(
                obj["type"] == 7 for item in imports
                for obj in item["finalized_state"]["state"]["objects"]),
            "unknown_warnings_verified": dict(warning_counts),
            "failed_objtyp_imports": [item["file"] for item in imports
                if any(token["id"] == 145 and token["handler_status"] not in (None, 0)
                       for token in item["tokens"])],
            "interpretation": "Case names and predeclared questions express intent; failed OBJTYP reads and unknown numeric warnings do not establish a deactivation/reactivation test. Consult observed type transitions.",
        },
        "tokens": sum(item["tokens"] for item in comparisons),
        "descriptor_snapshots": sum(item["objects"] * 53 for item in comparisons),
        "limitations": [
            "Original arrays, descriptors and persisted boundary groups are the oracle; no expected arrays are generated from the reconstruction.",
            "The spatial direct geometry finalizer is compared after completion in the material-empty regime, including enabled plane creation/classification. Alternate geometry formats and transformed-plane option remain outside scope.",
            "Geometry buffer values and plane points/normals compare exact binary64 bits; native COW capacities/addresses are provenance, not replacement memory-layout requirements.",
            "Before-state comparison covers the primary root; primary and linked roots are compared after finalization.",
            "Original generations are inferred from observed dimension changes; allocator addresses may be reused after reset.",
            "Stable native object/descriptor identity is checked within each observed generation, not across resets or processes.",
            "Temporary planar builder ranges/chains are not captured; persisted planar boundary-node groups are compared.",
            "Known doubles are decoded from original raw hex and compared as binary64; direct grown tails remain null and are not read.",
            "The initialized root profile supplies direct61 default; the original process slot is not captured separately.",
            "Handler cursor is observed directly at handler return; exhausted native buffer cursor0 is mapped to logical fixture EOF.",
            "Native FILE buffer cursor resets to0 when fgets drains a non-LF-terminated tail; logical cursor is reconstructed from captured entry position plus exact fgets bytes.",
            "Only these bounded fixtures are covered; disabled alternate dispatch and empty secondary roots are required.",
            "Case manifests record predeclared hypotheses; independence also requires a separately recorded candidate freeze.",
        ],
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--driver", type=Path, default=ROOT / "build/Release/keyword_import_replay.exe")
    parser.add_argument("--reader-driver", type=Path, default=ROOT / "build/Release/keyword_reader_replay.exe")
    parser.add_argument("--capture", type=Path,
                        default=ROOT / "analysis/geometry-import-runtime-training.json")
    parser.add_argument("--run-record", type=Path,
                        default=ROOT / "analysis/oracle/geometry-import-training/run.json")
    parser.add_argument("--fixtures", type=Path)
    parser.add_argument("--cases", type=Path,
                        default=ROOT / "tests/fixtures/geometry_import/cases.json")
    parser.add_argument("--freeze", type=Path,
                        default=ROOT / "analysis/geometry-import-candidate-freeze.json")
    parser.add_argument("--report", type=Path)
    parser.add_argument("--require-frozen-candidate", action="store_true")
    args = parser.parse_args()
    report = compare(args)
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{report['status'].upper()}: {len(report['imports'])} imports, "
          f"{report['tokens']} tokens, {report['descriptor_snapshots']} descriptor snapshots")
    return 0 if report["status"] == "pass" else 1


if __name__ == "__main__":
    raise SystemExit(main())


