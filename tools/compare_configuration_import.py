"""Replay configuration imports against retained allocation-aware original snapshots.

Only the replacement driver executes. The passive dimension probe extends the
historical keyword probe with configuration state and conservative buffer validity.
"""
import argparse
import json
import math
from pathlib import Path
import subprocess

from capture_oracle import digest
from compare_keyword_import import ROOT, EXE_SHA, require, read, binding
from compare_mesh_refresh import compare_values as differences_at, values_from_hex
from compare_mesh_storage import normalized_expected
from verify_mesh_array_observations import descriptor_map

def verify_candidate(driver, enforce):
    path = ROOT / "analysis/configuration-import-candidate-freeze.json"
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
    template = ROOT / "tools/observe_configuration_import.js"
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
        objects = state["objects"]
        require(state["object_count"] == len(objects)
                and [obj["id"] for obj in objects] == list(range(1, len(objects) + 1)),
                "object list/count mismatch")
        result = {key: state[key] for key in ("dimension", "suppress_boundaries",
                  "components", "object_count", "alternate_enabled", "node8_default",
                  "geometry_mode", "output_flags")}
        result["objects"] = []
        for obj in objects:
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
            normalized.update({key: obj[key] for key in ("id", "type", "secondary", "element_kind")})
            normalized["name_hex"] = obj["name"].encode("ascii").hex()
            result["objects"].append(normalized)
        linked = state["linked_state"]
        normalized_linked = self.normalize(linked) if linked is not None else None
        return {"state": result, "generation": tracked["generation"],
                "linked_state": normalized_linked["state"] if normalized_linked else None,
                "linked_generation": normalized_linked["generation"] if normalized_linked else None}


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
                       "start_sequence": event["sequence"]}
            continue
        require(current is not None and current["file"] == event["file"],
                "event outside current import")
        state = identities.normalize(event["state"]) if "state" in event else None
        if op == "keyword-resolved":
            current["tokens"].append({
                "token": event["token"], "id": event["id"], "category": event["category"],
                "handler_status": None, "unknown_warning": False})
            require(((event["id"] in (144, 145, 159, 173) and event["category"] == 2)
                     or (event["id"] in (28, 34) and event["category"] == 1)),
                    "fixture exceeded selected keyword scope")
        elif op == "handler-after":
            require(current["tokens"] and current["tokens"][-1]["id"] == event["id"]
                    and current["tokens"][-1]["category"] == event["category"]
                    and current["tokens"][-1]["handler_status"] is None, "handler/token mismatch")
            current["tokens"][-1]["handler_status"] = event["status"]
        elif op == "finalize-before":
            require("parser_state" not in current, "duplicate finalization")
            current["parser_state"] = state
            current["finalize_before_sequence"] = event["sequence"]
        elif op == "finalize-after":
            require("parser_state" in current and "finalized_state" not in current,
                    "unpaired finalization")
            current["finalized_state"] = state
        elif op == "import-after":
            require("finalized_state" in current and state == current["finalized_state"],
                    "incomplete finalization/import state")
            statuses = [token["handler_status"] for token in current["tokens"]
                        if token["handler_status"] not in (None, 0)]
            require(event["status"] == (statuses[-1] if statuses else 0),
                    "status differs from last handler error")
            current.update(status=event["status"], after=state, end_sequence=event["sequence"])
            imports.append(current)
            previous_after, current = state, None
        elif op not in ("stream-scan", "object-selected", "allocation-update", "allocation-free"):
            raise ValueError("unexpected probe event: " + op)
    require(current is None and [item["file"] for item in imports] == order,
            "incomplete import order")
    return imports


def compare(args):
    driver = args.driver.resolve(strict=True)
    candidate = verify_candidate(driver, args.require_frozen_candidate)
    evidence, run, events, source_files, sources = provenance(
        args.capture.resolve(), args.run_record.resolve(),
        args.fixtures.resolve() if args.fixtures else None)
    manifest = read(args.cases)
    cases = manifest["cases"]
    require([Path(case["path"]).name for case in cases] == run["ordered_imports"],
            "predeclared case order mismatch")
    for case in cases:
        path = (ROOT / case["path"]).resolve()
        require(path == source_files[path.name] and digest(path) == case["sha256"],
                "predeclared fixture hash mismatch")
    require(manifest["probe"] == binding(ROOT / "tools/observe_configuration_import.js"),
            "predeclared probe hash mismatch")
    contract = ROOT / "analysis/mesh-array-contract.json"
    imports = retained_imports(events, run["ordered_imports"], descriptor_map(read(contract)))
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
        expected = item["parser_state"]
        require(expected["state"]["dimension"] == case["expected_dimension"]
                and expected["generation"] == case["expected_generation"]
                and expected["state"]["geometry_mode"] == case["expected_geometry_mode"]
                and expected["state"]["output_flags"] == case["expected_output_flags"]
                and item["status"] == case["expected_status"],
                "observed scenario differs from predeclared configuration/status expectation")
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
        comparisons.append({
            "file": item["file"], "sequence_range": [item["start_sequence"], item["end_sequence"]],
            "finalize_before_sequence": item["finalize_before_sequence"],
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
            "original_finalization_changed_tracked_subset":
                item["parser_state"] != item["finalized_state"],
            "differences": differences, "matched": not differences})
    return {
        "status": "pass" if all(item["matched"] for item in comparisons) else "fail",
        "provenance": sources, "cases": binding(args.cases), "candidate": candidate,
        "driver": binding(driver), "comparator": binding(Path(__file__)),
        "mesh_contract": binding(contract),
        "explicit_initial_controls": initial,
        "startup_protocol": "initialized DIMENSION NODE8_DEFAULT SUPPRESS_BOUNDARIES; no injected table",
        "imports": comparisons,
        "tokens": sum(item["tokens"] for item in comparisons),
        "descriptor_snapshots": sum(item["objects"] * 53 for item in comparisons),
        "limitations": [
            "Original arrays, descriptors and persisted boundary groups are the oracle; no expected arrays are generated from the reconstruction.",
            "Only the tracked model subset at finalize-before is compared; full scientific finalization remains pending.",
            "Before-state comparison covers the primary root. Linked-root state is compared at finalize-before.",
            "Original generations are inferred from observed dimension changes; allocator addresses may be reused after reset.",
            "Stable native object/descriptor identity is checked within each observed generation, not across resets or processes.",
            "Temporary planar builder ranges/chains are not captured; persisted planar boundary-node groups are compared.",
            "Known doubles are decoded from original raw hex and compared as binary64; direct grown tails remain null and are not read.",
            "The initialized root profile supplies direct61 default; the original process slot is not captured separately.",
            "Only these bounded fixtures are covered; disabled alternate dispatch and empty secondary roots are required.",
            "Case manifests record predeclared expectations; independence also requires a separately recorded candidate freeze.",
        ],
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--driver", type=Path, default=ROOT / "build/Release/keyword_import_replay.exe")
    parser.add_argument("--capture", type=Path,
                        default=ROOT / "analysis/configuration-import-runtime-evaluation.json")
    parser.add_argument("--run-record", type=Path,
                        default=ROOT / "analysis/oracle/configuration-import-evaluation/run.json")
    parser.add_argument("--fixtures", type=Path)
    parser.add_argument("--cases", type=Path,
                        default=ROOT / "analysis/configuration-import-evaluation-cases.json")
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


