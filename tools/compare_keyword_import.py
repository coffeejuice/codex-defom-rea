"""Replay translated KEY bytes and compare the selected importer to saved REA evidence.

Runs only the replacement driver. Original file/process/probe provenance is
validated before comparing parser statuses, token/handler order and the tracked
model at finalize-before. Scientific finalization is explicitly not reproduced.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import subprocess

from capture_oracle import digest
from compare_mesh_storage import normalized_expected
from verify_mesh_array_observations import descriptor_map

ROOT = Path(__file__).resolve().parents[1]
EXE_SHA = "f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794"


def require(condition, message):
    if not condition:
        raise ValueError(message)


def read(path):
    return json.loads(path.read_text(encoding="utf-8-sig"))


def binding(path):
    path = path.resolve()
    return {"path": path.relative_to(ROOT).as_posix() if path.is_relative_to(ROOT)
            else str(path), "sha256": digest(path)}


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
    require(script == directory.with_suffix(".js")
            and digest(script) == cap["source_sha256"], "probe path/hash mismatch")
    gate = Path(run["ready_gate"]).resolve()
    require(gate == directory.with_suffix(".ready"), "gate path mismatch")
    template = ROOT / "tools/observe_keyword_import.js"
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


def normalize_state(state, descriptor_rows, identities):
    require(state["dimension"] == 3, "this complete-state probe captures 3D boundary records only")
    require(state["suppress_boundaries"] in (0, 1) and len(state["components"]) == 79
            and state["components"][0] == 3, "incomplete root controls")
    objects = state["objects"]
    require(state["object_count"] == len(objects)
            and [obj["id"] for obj in objects] == list(range(1, len(objects) + 1)),
            "object list/count mismatch")
    result = {key: state[key] for key in ("dimension", "suppress_boundaries",
                                        "components", "object_count")}
    result["objects"] = []
    for obj in objects:
        require(obj["dimension"] == state["dimension"] and obj["components"] == state["components"],
                "object/root controls differ")
        require(obj["name"].isascii(), "capture CString decoding supports only ASCII names here")
        descriptors = obj["descriptors"]
        require(len(descriptors) == 53, "incomplete descriptor inventory")
        identity = (obj["address"], tuple(d["address"] for d in descriptors))
        require(int(identity[0], 16) > 0, "null object address")
        previous = identities.setdefault(obj["id"], identity)
        require(identity == previous, "original object/descriptor identity changed")
        for descriptor, row in zip(descriptors, descriptor_rows):
            require(descriptor["id"] == row["state_id"]
                    and descriptor["slot_offset"] == int(row["slot_relative_to_object"], 16),
                    "descriptor registration mismatch")
            require(descriptor["storage_references"] == 1
                    and descriptor["bound_pointer"] == descriptor["storage_pointer"],
                    "uncovered shared storage or broken binding")
            require(descriptor["length"] == descriptor["count"] * descriptor["width"]
                    * descriptor["multiplicity"] == len(descriptor["values"] or []),
                    "descriptor extent/value mismatch")
        normalized = normalized_expected(obj)
        normalized.update({key: obj[key] for key in ("id", "type", "secondary", "element_kind")})
        normalized["name_hex"] = obj["name"].encode("ascii").hex()
        result["objects"].append(normalized)
    return result


def retained_imports(events, order, descriptor_rows):
    imports, current, identities, previous_after = [], None, {}, None
    for event in events:
        op = event["operation"]
        if op == "import-before":
            require(current is None, "nested import outside supported corpus")
            before = normalize_state(event["state"], descriptor_rows, identities)
            require(before == previous_after if previous_after is not None
                    else before["object_count"] == 0, "unexpected state between imports")
            current = {"file": event["file"], "tokens": [], "before": before,
                       "start_sequence": event["sequence"]}
        else:
            require(current is not None and event["file"] == current["file"],
                    "event outside active import")
            if "state" in event:
                state = normalize_state(event["state"], descriptor_rows, identities)
            if op == "keyword-resolved":
                current["tokens"].append({
                    "token": event["token"], "id": event["id"], "category": event["category"],
                    "handler_status": None,
                    "unknown_warning": event["id"] == 305 and event["token"] != "SRFNRG",
                })
            elif op == "handler-after":
                require(bool(current["tokens"]) and current["tokens"][-1]["id"] == event["id"]
                        and current["tokens"][-1]["handler_status"] is None,
                        "handler/token ordering mismatch")
                current["tokens"][-1]["handler_status"] = event["status"]
            elif op == "finalize-before":
                require("parser_state" not in current, "duplicate finalization")
                current["parser_state"] = state
                current["finalize_before_sequence"] = event["sequence"]
            elif op == "finalize-after":
                require("parser_state" in current and "finalized_state" not in current,
                        "unpaired finalization")
                current["finalized_state"] = state
                current["finalize_after_sequence"] = event["sequence"]
            elif op == "import-after":
                require("finalized_state" in current and state == current["finalized_state"],
                        "incomplete finalization/import state")
                current.update(status=event["status"], after=state, end_sequence=event["sequence"])
                statuses = [token["handler_status"] for token in current["tokens"]
                            if token["handler_status"] not in (None, 0)]
                require(event["status"] == (statuses[-1] if statuses else 0),
                        "capture import status differs from last handler error")
                previous_after = state
                imports.append(current)
                current = None
            elif op not in ("stream-scan", "object-selected"):
                raise ValueError("unexpected capture operation: " + op)
    require(current is None and [item["file"] for item in imports] == order,
            "incomplete import sequence")
    return imports


def differences_at(actual, expected, path, differences):
    if isinstance(expected, dict):
        if not isinstance(actual, dict):
            differences.append({"field": path, "expected": expected, "actual": actual})
            return
        for key, value in expected.items():
            if key not in actual:
                differences.append({"field": path + "." + key, "error": "missing field"})
            else:
                differences_at(actual[key], value, path + "." + key, differences)
    elif isinstance(expected, list):
        if not isinstance(actual, list) or len(actual) != len(expected):
            differences.append({"field": path, "expected": expected, "actual": actual})
        elif actual != expected:
            for index, (got, wanted) in enumerate(zip(actual, expected)):
                differences_at(got, wanted, f"{path}[{index}]", differences)
    elif actual != expected:
        differences.append({"field": path, "expected": expected, "actual": actual})


def compare(args):
    driver = args.driver.resolve(strict=True)
    evidence, run, events, source_files, sources = provenance(
        args.capture.resolve(), args.run_record.resolve(),
        args.fixtures.resolve() if args.fixtures else None)
    contract = ROOT / "analysis/mesh-array-contract.json"
    descriptor_rows = descriptor_map(read(contract))
    imports = retained_imports(events, run["ordered_imports"], descriptor_rows)
    controls = imports[0]["before"]
    for item in imports:
        for phase in ("before", "parser_state", "after"):
            require(all(item[phase][key] == controls[key] for key in
                        ("dimension", "suppress_boundaries", "components")), "root controls changed")
    commands = [
        f"{controls['dimension']} {args.node8_default} {args.direct61_default} "
        f"{controls['suppress_boundaries']}",
        " ".join(map(str, controls["components"])),
    ]
    translated_inputs = []
    for item in imports:
        raw = source_files[item["file"]].read_bytes()
        require(b"\0" not in raw and b"\x1a" not in raw, "fixture text translation outside scope")
        translated = raw.replace(b"\r\n", b"\n")
        translated_inputs.append(translated)
        commands.append("import " + (translated.hex() or "-"))
    process = subprocess.run([str(driver)], input="\n".join(commands) + "\n",
                             text=True, capture_output=True, timeout=30, check=True)
    # Preserve the candidate's "-0" spelling, although the original probe's
    # numeric JSON has already lost signed-zero information. This comparison
    # therefore deliberately makes no binary64 byte-equivalence claim.
    actual = [json.loads(line, parse_int=lambda text: -0.0 if text == "-0" else int(text))
              for line in process.stdout.splitlines()]
    require(len(actual) == len(imports), "driver import result count mismatch")
    # Diagnostics are checked against the retained original transcript, not
    # inferred solely from the static unknown-keyword policy.
    stdout = (args.run_record.parent / "stdout.bin").read_text(encoding="ascii")
    warning_counts = Counter(token["token"] for item in imports for token in item["tokens"]
                             if token["unknown_warning"])
    for token, count in warning_counts.items():
        require(stdout.count(f"WARNING: Keyword {token} not found. Skip ") == count,
                "unknown warning transcript mismatch")
    require("WARNING: Keyword SRFNRG not found." not in stdout, "silent-keyword warning mismatch")
    comparisons, all_differences = [], []
    for index, (got, item, data) in enumerate(zip(actual, imports, translated_inputs)):
        differences = []
        expected_result = {"outcome": "parsed", "status": item["status"],
                           "cursor": len(data), "reached_eof": True,
                           "finalization_pending": True, "unsupported_reason": "",
                           "events": item["tokens"]}
        differences_at(got["result"], expected_result, "result", differences)
        differences_at(got["state"], item["parser_state"], "state", differences)
        differences_at(got["stable_identity"], True, "stable_identity", differences)
        comparisons.append({
            "file": item["file"], "status": item["status"],
            "sequence_range": [item["start_sequence"], item["end_sequence"]],
            "comparison_phase": "finalize-before", "tokens": len(item["tokens"]),
            "objects": item["parser_state"]["object_count"],
            "descriptor_snapshots": item["parser_state"]["object_count"] * 53,
            "original_finalization_changed_tracked_subset":
                item["parser_state"] != item["finalized_state"],
            "differences": differences, "verdict": "different" if differences else "exact",
        })
        all_differences.extend(differences)
    return {
        "status": "fail" if all_differences else "pass",
        "provenance": sources, "driver": binding(driver),
        "comparison_sources": [binding(Path(__file__)), binding(ROOT / "tools/compare_mesh_storage.py"),
                               binding(ROOT / "tools/verify_mesh_array_observations.py"),
                               binding(contract), binding(ROOT / "analysis/keyword-catalog.json")],
        "candidate_sources": [binding(ROOT / name) for name in (
            "src/recovered/object_model.hpp", "src/recovered/object_model.cpp",
            "src/recovered/keyword_import.hpp", "src/recovered/keyword_import.cpp",
            "src/recovered/keyword_catalog.hpp", "tests/keyword_import_replay.cpp")],
        "explicit_controls": {
            "captured": {key: controls[key] for key in
                         ("dimension", "suppress_boundaries", "components")},
            "supplied_node8_default": args.node8_default,
            "supplied_direct61_default": args.direct61_default,
            "qualification": "Default-source slots were not captured by this probe. Values 20/0 "
                             "are explicit replay inputs corroborated by recorded allocated arrays.",
        },
        "imports": comparisons, "tokens": sum(item["tokens"] for item in comparisons),
        "object_snapshots": sum(item["objects"] for item in comparisons),
        "descriptor_snapshots": sum(item["descriptor_snapshots"] for item in comparisons),
        "comparison": "Original parser-complete tracked model, import statuses, keyword resolution "
                      "and handler order, warning transcript, and stable object identity.",
        "limitations": [
            "Only the captured object fields and registered mesh arrays are compared, not a full model.",
            "Candidate state is compared before original scientific finalization. The candidate "
            "explicitly leaves finalization pending; matching later tracked fields would not prove "
            "equivalence of unobserved scientific/material effects.",
            "Native addresses are normalized away; original identity/bindings and candidate identity "
            "stability are checked within each process, not compared numerically across processes.",
            "Names are restricted to ASCII logical CString contents; native padding is not compared.",
            "Numeric state uses captured JSON values, not a bitwise comparison of original double storage.",
            "CRLF translation is applied explicitly to these fixtures; Ctrl-Z, embedded NUL, physical "
            "file offsets and general filesystem text-mode behavior are outside this comparison.",
            "Training/held-out classification and candidate-freeze timing belong to the experiment "
            "manifest; a passing retained-capture comparison alone does not establish independence.",
        ],
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--driver", type=Path,
                        default=ROOT / "build/Release/keyword_import_replay.exe")
    parser.add_argument("--capture", type=Path,
                        default=ROOT / "analysis/keyword-import-runtime-training.json")
    parser.add_argument("--run-record", type=Path,
                        default=ROOT / "analysis/oracle/keyword-import-training/run.json")
    parser.add_argument("--fixtures", type=Path)
    parser.add_argument("--node8-default", type=float, default=20.0)
    parser.add_argument("--direct61-default", type=int, default=0)
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    report = compare(args)
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{report['status'].upper()}: {len(report['imports'])} imports, "
          f"{report['tokens']} tokens, {report['object_snapshots']} object snapshots, "
          f"{report['descriptor_snapshots']} descriptor snapshots")
    raise SystemExit(0 if report["status"] == "pass" else 1)


if __name__ == "__main__":
    main()
