"""Verify retained root-layout captures without calculating component-table outputs."""
import argparse
from collections import Counter
from copy import deepcopy
import json
from pathlib import Path
import re

from capture_oracle import digest
from verify_boundary_sort_observations import EXE_SHA, ROOT, binding, check, read, verify_files

SCALARS = {hex(v) for v in [
    0x86c, 0x870, 0x874, 0x878, 0x87c, 0x880, 0x884, 0x1320, 0x13f4,
    0x3398, 0x33a8, 0x850, 0x888, 0x88c, 0x890, 0x33c8, 0x33d8, 0x11c4,
    0x123c, 0x12b4,
]}


def scalar_shape(values):
    check(set(values) == SCALARS and all(type(v) is int for v in values.values()),
          "scalar capture shape")


def table_shape(values):
    check(len(values) == 79 and all(type(v) is int and v >= 0 for v in values),
          "component table shape")


def state_shape(state):
    check(int(state["address"], 16) > 0 and state["dimension"] in (2, 3), "root identity/dimension")
    scalar_shape(state["scalars"])
    table_shape(state["components"])
    check(state["material_count"] == state["scalars"]["0x1320"] >= 0, "material count mismatch")
    check(state["suppress_boundaries"] in (0, 1) and state["alternate_enabled"] in (0, 1)
          and int(state["alternate_root"], 16) >= 0, "root flags/link shape")
    objects = state["objects"]
    check(state["object_count"] == len(objects) <= 8
          and len({o["id"] for o in objects}) == len(objects)
          and len({o["address"] for o in objects}) == len(objects), "object traversal")
    for obj in objects:
        check(type(obj["id"]) is int and obj["id"] > 0 and int(obj["address"], 16) > 0
              and obj["nodes"] >= 0 and obj["elements"] >= 0
              and obj["arity"] in (4, 8) and obj["mesh"] in (0, 1)
              and isinstance(obj["name"], str), "object snapshot shape")


def header_dimension(data):
    """Only the short declared header syntax in these fixtures, not a KEY scanner."""
    found = []
    for line in data.decode("ascii").splitlines():
        if not line.startswith("*"):
            continue
        match = re.match(r"\*\s+DEFORM(?:-| )([23]D|F[23])(?:\s|$)", line)
        if match:
            token = match[1]
            found.append(2 if "2" in token else 3)
        elif re.match(r"\*\s+DEFORM-(?:MACH2|DIESTRESS2|MACHDIST2|MICROSTRUCTURE2)(?:\s|$)", line):
            found.append(2)
    return found


def provenance(evidence, run, manifest, directory):
    order = [Path(c["path"]).name for c in manifest["cases"]]
    check(run["status"] == "completed" and run["exit_code"] == 0, "run completion")
    check(Path(run["cwd"]) == directory and run["ordered_imports"] == order, "run path/order")
    check(Path(run["executable"]) == ROOT / "source/DEF_PRE_64.exe"
          and run["executable_sha256"] == digest(Path(run["executable"])) == EXE_SHA,
          "executable identity")
    verify_files(run, directory)
    check((directory / "stdout.bin").stat().st_size < run["max_output_bytes"], "transcript bound")
    check([i["name"] for i in run["inputs"]] == order, "input order")
    inputs = {}
    for case, item in zip(manifest["cases"], run["inputs"]):
        source, copy = ROOT / case["path"], directory / item["name"]
        check(Path(item["source"]) == source and Path(item["copy"]) == copy, "fixture paths")
        check(case["sha256"] == digest(source) == digest(copy) == item["sha256_before"]
              == item["sha256_after"] == item["source_sha256_after"], "fixture hashes")
        inputs[item["name"]] = copy.read_bytes()
    cap = evidence["normalized_result"]
    template = ROOT / "tools/observe_root_layout.js"
    script, gate = directory.with_suffix(".js"), directory.with_suffix(".ready")
    check(Path(cap["source_path"]) == script and digest(script) == cap["source_sha256"]
          and Path(run["ready_gate"]) == gate, "probe identity")
    text = template.read_text(encoding="utf-8")
    check(text.count("__READY_GATE__") == 2 and script.read_text(encoding="utf-8")
          == text.replace("__READY_GATE__", gate.as_posix()), "passive template mismatch")
    return inputs


def verify(evidence, run, inputs, training=False):
    cap, params = evidence["normalized_result"], evidence["parameters"]
    pid = run["pid"]
    check(type(pid) is int and pid > 0 and cap["pid"] == params["pid"] == pid
          and cap["target"] == f"DEF_PRE_64.exe (pid {pid})", "PID/target")
    if training:
        check(pid == 4084, "training PID")
    check(cap["session_id"] == params["session_id"] and cap["mode"] == params["mode"] == "attach"
          and cap["device_id"] == params["device_id"] == "local" and cap["state"] == "detached"
          and cap["source_kind"] == "file", "capture lifecycle")
    check(cap["messages_truncated"] is False and cap["cleanup_error"] is None, "capture incomplete")
    messages, scripts = cap["messages"], cap["scripts"]
    check(messages and len(scripts) == 1 and all(m["type"] == "send"
          and m["script_id"] == scripts[0]["script_id"] for m in messages), "script/errors")
    first = messages[0]["payload"]
    check(first["operation"] == "hooks-installed" and first["pid"] == pid
          and int(first["base"], 16) > 0, "hooks identity")
    events = [m["payload"] for m in messages[1:]]
    check([e["sequence"] for e in events] == list(range(1, len(events) + 1)), "event sequence")
    if training:
        check(len(events) == 64, "training event count")
    stack, imports, dimensions, layouts, shutdown = [], [], [], [], []
    active, current_before, serial, previous_import = None, None, 0, None
    roots, last_states, deterministic = {}, {}, {}
    for event in events:
        op = event["operation"]
        check(event["file"] == active or op == "import-before", "event file pairing")
        if "state" in event:
            state_shape(event["state"])
        if op == "import-before":
            check(active is None and not stack and len(imports) < len(inputs)
                  and event["file"] == run["ordered_imports"][len(imports)], "import order")
            active, current_before = event["file"], event
            if previous_import is not None:
                check(event["state"] == previous_import, "inter-import state changed")
            if not roots:
                roots["primary"] = event["state"]["address"]
                roots["secondary"] = event["state"]["alternate_root"]
            check(event["state"]["address"] == roots["primary"], "import root changed")
            stack.append(("import", event))
        elif op == "dimension-before":
            check(stack and stack[-1][0] == "import" and event["request"] in (2, 3),
                  "dimension call nesting")
            root = event["state"]["address"]
            check(root in roots.values(), "unknown dimension root")
            if root in last_states and root == roots["secondary"]:
                check(event["state"] == last_states[root], "secondary changed outside observed transition")
            stack.append(("dimension", {"enter": event, "clear": None, "layout": None}))
        elif op == "root-clear-enter":
            check(event["root"] in roots.values() and event["dimension"] in (2, 3)
                  and 0 <= event["object_count"] <= 8, "clear root/count")
            if active is None:
                check(not stack and len(imports) == len(inputs), "unexpected out-of-import cleanup")
            else:
                check(stack and stack[-1][0] == "dimension", "clear not inside transition")
                frame = stack[-1][1]
                before = frame["enter"]
                check(before["state"]["dimension"] != before["request"]
                      and event["root"] == before["state"]["address"]
                      and event["dimension"] == before["request"]
                      and event["object_count"] == before["state"]["object_count"]
                      and frame["clear"] is None, "clear ordering/new dimension")
            stack.append(("clear", event))
        elif op == "root-clear-leave":
            check(stack and stack[-1][0] == "clear", "clear leave nesting")
            before = stack.pop()[1]
            check(event["root"] == before["root"] and event["dimension"] == before["dimension"]
                  and event["object_count"] == 0, "clear did not empty observed list")
            if active is None:
                shutdown.append({"before": before, "after": event})
            else:
                stack[-1][1]["clear"] = {"before": before, "after": event}
        elif op == "layout-enter":
            check(stack and stack[-1][0] in ("import", "dimension"), "layout enter nesting")
            check(event["serial"] == serial + 1 and event["root"] in roots.values()
                  and event["dimension"] in (2, 3) and event["kind"] in ("initialize", "refresh"),
                  "layout identity")
            serial = event["serial"]
            scalar_shape(event["scalars"])
            if stack[-1][0] == "dimension":
                frame = stack[-1][1]
                check(frame["clear"] is not None and frame["layout"] is None
                      and event["kind"] == "initialize"
                      and event["root"] == frame["enter"]["state"]["address"]
                      and event["dimension"] == frame["enter"]["request"], "initialization before clear")
            else:
                check(event["kind"] == "refresh", "initialization outside changed dimension")
            stack.append(("layout", event))
        elif op == "layout-leave":
            check(stack and stack[-1][0] == "layout", "layout leave nesting")
            before = stack.pop()[1]
            check(all(event[k] == before[k] for k in ("serial", "kind", "root", "dimension", "scalars")),
                  "layout pair mismatch")
            table_shape(event["components"])
            scalar_shape(event["after_scalars"])
            expected_ids = [24, 49, 53, 29, 54, 55] + ([46] if event["dimension"] == 3 else [])
            check([q["id"] for q in event["queries"]] == expected_ids, "layout query order")
            width = 0
            for query in event["queries"]:
                check(type(query["enabled"]) is int and query["enabled"] in (0, 1),
                      "query return shape")
                if query["id"] in (24, 49, 46):
                    check(query["width_before"] is None and query["width"] is None, "null query output")
                else:
                    check(query["width_before"] == width and type(query["width"]) is int,
                          "shared query-output accumulator")
                    width = query["width"]
            # Determinism across identical observed inputs, not a reimplementation.
            key = json.dumps({k: event[k] for k in ("kind", "dimension", "scalars", "queries")}, sort_keys=True)
            output = {"components": event["components"], "after_scalars": event["after_scalars"]}
            if key in deterministic:
                check(output == deterministic[key], "identical captured layout inputs disagree")
            deterministic[key] = output
            layouts.append(event)
            if stack[-1][0] == "dimension":
                stack[-1][1]["layout"] = event
        elif op == "dimension-after":
            check(stack and stack[-1][0] == "dimension", "dimension leave nesting")
            frame = stack.pop()[1]
            before = frame["enter"]
            check(event["request"] == before["request"]
                  and event["state"]["address"] == before["state"]["address"]
                  and event["state"]["dimension"] == before["request"], "dimension pair/result")
            changed = before["state"]["dimension"] != before["request"]
            if changed:
                check(frame["clear"] and frame["layout"] and event["state"]["object_count"] == 0
                      and event["state"]["material_count"] == 0
                      and event["state"]["components"] == frame["layout"]["components"]
                      and event["state"]["scalars"] == frame["layout"]["after_scalars"],
                      "changed dimension reset/layout result")
                for field in ("alternate_root", "alternate_enabled", "suppress_boundaries"):
                    check(event["state"][field] == before["state"][field], "tracked retained root control")
            else:
                check(frame["clear"] is None and frame["layout"] is None
                      and event["state"] == before["state"], "same-dimension state changed")
            dimensions.append({"file": active, "changed": changed, "before": before, "after": event})
            last_states[event["state"]["address"]] = event["state"]
        elif op == "import-after":
            check(len(stack) == 1 and stack[0][0] == "import"
                  and event["state"]["address"] == roots["primary"] and type(event["status"]) is int
                  and event["status"] in (0, 1, 2, 3),
                  "import completion")
            stack.pop()
            headers = header_dimension(inputs[active])
            calls = [d for d in dimensions if d["file"] == active]
            wanted = [] if not headers else [(roots["primary"], headers[0]), (roots["secondary"], 5 - headers[0])]
            check([(d["before"]["state"]["address"], d["before"]["request"]) for d in calls] == wanted,
                  "first recognized header rule")
            # The latest producer on this root must agree with the end-of-import table.
            produced = [x for x in layouts if x["file"] == active and x["root"] == roots["primary"]]
            if produced:
                check(event["state"]["components"] == produced[-1]["components"],
                      "import table differs from last producer")
            else:
                check(event["state"]["components"] == current_before["state"]["components"],
                      "table changed without observed producer")
            imports.append({"file": active, "status": event["status"], "recognized_headers": headers,
                            "before": current_before["state"], "after": event["state"]})
            previous_import = event["state"]
            active, current_before = None, None
        else:
            raise ValueError("unexpected operation/probe error")
    check(not stack and active is None and len(imports) == len(inputs), "unfinished events")
    if training:
        check([i["status"] for i in imports] == [0] * 6
              and len(dimensions) == 10 and sum(d["changed"] for d in dimensions) == 6
              and Counter(x["kind"] for x in layouts) == {"initialize": 6, "refresh": 2}
              and len(shutdown) == 2, "training coverage")
        # Observe the tested compositions without rebuilding scientific arrays.
        same = imports[1]
        old, new = deepcopy(same["before"]["objects"]), deepcopy(same["after"]["objects"])
        check(len(old) == len(new) == 1, "same-dimension fixture object count")
        old[0].pop("name"); new[0].pop("name")
        check(old == new, "same dimension did not retain captured mesh metadata")
        refresh = imports[3]
        check(refresh["before"]["dimension"] == refresh["after"]["dimension"] == 2
              and refresh["before"]["scalars"]["0x850"] == 1
              and refresh["after"]["scalars"]["0x850"] == 3
              and refresh["before"]["components"] != refresh["after"]["components"],
              "refresh distinction missing")
        check(refresh["before"]["objects"][0]["address"] == refresh["after"]["objects"][0]["address"],
              "refresh unexpectedly replaced object identity")
        late = [d for d in dimensions if d["file"] == imports[-1]["file"] and
                d["before"]["state"]["address"] == roots["primary"]][0]
        check(late["before"]["state"]["object_count"] == 2 and late["after"]["state"]["object_count"] == 0
              and imports[-1]["after"]["dimension"] == 2
              and [o["name"] for o in imports[-1]["after"]["objects"]]
              == ["Created after header", "Second dimension header ignored"],
              "late/second header observation mismatch")
    return {"imports": imports, "dimension_transitions": dimensions, "layout_observations": layouts,
            "shutdown_cleanup": shutdown, "root_roles": roots,
            "operation_counts": dict(Counter(e["operation"] for e in events)),
            "sequenced_events": len(events)}


def normalize(result):
    """Omit native addresses after identity checks; preserve object lifetime ambiguity."""
    roles = result["root_roles"]
    addresses = {v: k for k, v in roles.items()}
    def visit(value, key=None):
        if isinstance(value, dict):
            return {k: visit(v, k) for k, v in value.items() if k != "root_roles"}
        if isinstance(value, list):
            return [visit(v, key) for v in value]
        if key in ("address", "root", "alternate_root") and isinstance(value, str):
            if value in addresses:
                return addresses[value]
            return "null" if int(value, 16) == 0 else "object-address-omitted"
        return value
    return visit(result)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--capture", type=Path, default=ROOT / "analysis/root-layout-runtime-training.json")
    parser.add_argument("--run", type=Path, default=ROOT / "analysis/oracle/root-layout-training/run.json")
    parser.add_argument("--manifest", type=Path, default=ROOT / "analysis/root-layout-training-cases.json")
    parser.add_argument("--report", type=Path, default=ROOT / "analysis/root-layout-observations.json")
    args = parser.parse_args()
    evidence, run, manifest = read(args.capture), read(args.run), read(args.manifest)
    directory = args.run.resolve().parent
    inputs = provenance(evidence, run, manifest, directory)
    training = args.capture.name == "root-layout-runtime-training.json"
    result = verify(evidence, run, inputs, training)
    def event(e, op, predicate=lambda x: True):
        return next(m["payload"] for m in e["normalized_result"]["messages"]
                    if m["payload"]["operation"] == op and predicate(m["payload"]))
    mutations = {
        "PID": lambda e: e["normalized_result"].update(pid=0),
        "missing event": lambda e: e["normalized_result"]["messages"].pop(),
        "wrong sequence": lambda e: event(e, "layout-enter").update(sequence=999),
        "layout serial": lambda e: event(e, "layout-leave").update(serial=999),
        "query omission": lambda e: event(e, "layout-leave")["queries"].pop(),
        "query pointer shape": lambda e: event(e, "layout-leave")["queries"][0].update(width=1),
        "query output history": lambda e: event(e, "layout-leave")["queries"][2].update(width_before=99),
        "short table": lambda e: event(e, "layout-leave")["components"].pop(),
        "clear retains objects": lambda e: event(e, "root-clear-leave").update(object_count=1),
        "clear old dimension": lambda e: event(e, "root-clear-enter").update(dimension=3),
        "same-dimension mutation": lambda e: event(e, "dimension-after")["state"].update(suppress_boundaries=1),
        "changed-dimension wrong table": lambda e: event(e, "dimension-after", lambda x: x["sequence"] > 14)["state"]["components"].__setitem__(0, 99),
        "wrong header request": lambda e: event(e, "dimension-before").update(request=2),
        "import status": lambda e: event(e, "import-after").update(status=999),
    }
    checks = []
    for label, mutate in mutations.items():
        altered = deepcopy(evidence)
        mutate(altered)
        try:
            verify(altered, run, inputs, training)
        except (ValueError, KeyError, IndexError) as error:
            checks.append({"mutation": label, "rejected": True, "reason": str(error)})
        else:
            raise ValueError("accepted mutation: " + label)
    for label, mutate in [
        ("executable hash", lambda e, r: r.update(executable_sha256="0" * 64)),
        ("probe hash", lambda e, r: e["normalized_result"].update(source_sha256="0" * 64)),
        ("fixture hash", lambda e, r: r["inputs"][0].update(sha256_after="0" * 64)),
        ("transcript hash", lambda e, r: next(f for f in r["files"] if f["name"] == "stdout.bin").update(sha256="0" * 64)),
    ]:
        altered, badrun = deepcopy(evidence), deepcopy(run)
        mutate(altered, badrun)
        try:
            provenance(altered, badrun, manifest, directory)
        except ValueError as error:
            checks.append({"mutation": label, "rejected": True, "reason": str(error)})
        else:
            raise ValueError("accepted provenance mutation: " + label)
    report = {
        "status": "pass", "scope": "Passive original root-layout provenance and bounded transition observations.",
        "capture": binding(args.capture), "evidence_id": evidence["evidence_id"], "pid": run["pid"],
        "run_record": binding(args.run), "fixture_manifest": binding(args.manifest),
        "probe": binding(directory.with_suffix(".js")), "template": binding(ROOT / "tools/observe_root_layout.js"),
        "verifier": binding(Path(__file__)), **normalize(result), "tampering_checks": checks,
        "findings": [
            "Changed-dimension calls store the requested dimension before observed cleanup, clear the reported object count, generate an initialization table, and return an empty observed object list.",
            "Same-dimension calls have no nested cleanup or generator and preserve every emitted state field.",
            "Header calls match only the first recognized dimension-bearing comment per file; the late-header fixture clears objects created earlier in that import.",
            "The control fixture changes the table through two refresh calls while dimension remains2 and the observed object address persists.",
            "Shutdown cleanup events occur outside any import and are checked separately.",
        ],
        "limitations": [
            "No component generator is recomputed as an oracle. Captured inputs, query order/output shapes, output length, pairing and repeated-input consistency are checked.",
            "Object snapshots contain names, IDs, types, mesh counts and arity, not coordinates, descriptor storage or every root field. Empty list/count is not complete cleanup or memory-safety proof.",
            "The scalar capture omits root+898 and global index state touched by feature predicates. Equal captured scalars does not prove the producer is side-effect free.",
            "Observed addresses may be reused after cleanup. The report omits object addresses and does not equate address inequality/equality with lifetime proof.",
            "The training fixtures cover short ASCII header forms and selected controls; no general parser, material configuration or arbitrary-dimension equivalence is claimed.",
            "Custom capture/run/manifest/report arguments are supported, but training-specific assertions and mutation probes must be reviewed for a materially different event domain.",
        ],
    }
    # The default checkpoint report includes the subsequently captured evaluation.
    held_capture = ROOT / "analysis/root-layout-runtime-held-out.json"
    if training and held_capture.exists():
        held_directory = ROOT / "analysis/oracle/root-layout-held-out"
        held_manifest_path = ROOT / "analysis/root-layout-evaluation-cases.json"
        held_evidence, held_run = read(held_capture), read(held_directory / "run.json")
        held_manifest = read(held_manifest_path)
        held_inputs = provenance(held_evidence, held_run, held_manifest, held_directory)
        held_result = verify(held_evidence, held_run, held_inputs)
        check(held_run["pid"] == 23256 and held_result["sequenced_events"] == 60
              and len(held_result["imports"]) == 4
              and len(held_result["dimension_transitions"]) == 8
              and sum(d["changed"] for d in held_result["dimension_transitions"]) == 4
              and Counter(x["kind"] for x in held_result["layout_observations"])
              == {"initialize": 4, "refresh": 8}
              and [i["status"] for i in held_result["imports"]] == [0] * 4,
              "held-out retained capture identity/coverage")
        held_checks = []
        for label, mutate in mutations.items():
            altered = deepcopy(held_evidence)
            mutate(altered)
            try:
                verify(altered, held_run, held_inputs)
            except (ValueError, KeyError, IndexError) as error:
                held_checks.append({"mutation": label, "rejected": True, "reason": str(error)})
            else:
                raise ValueError("accepted held-out mutation: " + label)
        for label, mutate in [
            ("executable hash", lambda e, r: r.update(executable_sha256="0" * 64)),
            ("probe hash", lambda e, r: e["normalized_result"].update(source_sha256="0" * 64)),
            ("fixture hash", lambda e, r: r["inputs"][0].update(sha256_after="0" * 64)),
            ("transcript hash", lambda e, r: next(f for f in r["files"] if f["name"] == "stdout.bin").update(sha256="0" * 64)),
        ]:
            altered, badrun = deepcopy(held_evidence), deepcopy(held_run)
            mutate(altered, badrun)
            try:
                provenance(altered, badrun, held_manifest, held_directory)
            except ValueError as error:
                held_checks.append({"mutation": label, "rejected": True, "reason": str(error)})
            else:
                raise ValueError("accepted held-out provenance mutation: " + label)
        report["held_out"] = {
            "capture": binding(held_capture), "evidence_id": held_evidence["evidence_id"],
            "pid": held_run["pid"], "run_record": binding(held_directory / "run.json"),
            "fixture_manifest": binding(held_manifest_path),
            "probe": binding(held_directory.with_suffix(".js")),
            **normalize(held_result), "tampering_checks": held_checks,
            "findings": [
                "Material creation and TRANS/STNOUT controls produce refresh calls in a 3D root before any dimension change.",
                "DEFORM-MACH2 is accepted as a same-dimension header and preserves captured state at the virtual call boundary.",
                "The final 2D-to-3D transition resets the captured material count and object list before later OBJNAM creates an object.",
            ],
        }
        report["totals"] = {
            "imports": len(result["imports"]) + len(held_result["imports"]),
            "dimension_calls": len(result["dimension_transitions"]) + len(held_result["dimension_transitions"]),
            "changed_dimension_calls": sum(d["changed"] for d in
                                           result["dimension_transitions"] + held_result["dimension_transitions"]),
            "layout_calls": len(result["layout_observations"]) + len(held_result["layout_observations"]),
            "query_observations": sum(len(x["queries"]) for x in
                                      result["layout_observations"] + held_result["layout_observations"]),
            "tampering_checks": len(checks) + len(held_checks),
        }
    freeze_path = ROOT / "analysis/component-layout-candidate-freeze.json"
    if freeze_path.exists():
        frozen = read(freeze_path)
        frozen_files = []
        for item in frozen["files"]:
            archive, current = ROOT / item["archive"], ROOT / item["path"]
            check(archive.is_file() and digest(archive) == item["sha256"], "frozen archive mismatch")
            frozen_files.append(dict(item, archive_matches=True,
                                     current_matches=current.is_file() and digest(current) == item["sha256"]))
        report["candidate_freeze"] = {"manifest": binding(freeze_path), "declared_stage": frozen["stage"],
                                      "declared_created_utc": frozen.get("created_utc"), "files": frozen_files}
        report["limitations"].append(
            "Frozen archive bytes are verified. Declared chronology is not a trusted timestamp; current artifact matches are informational."
        )
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {len(result['imports'])} imports, {len(result['dimension_transitions'])} dimension calls, "
          f"{len(result['layout_observations'])} generators, {len(checks)} tampering checks")
    if "totals" in report:
        print("Combined: " + json.dumps(report["totals"]))


if __name__ == "__main__":
    main()
