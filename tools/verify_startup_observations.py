"""Verify retained suspended-startup captures; never launch the executable."""
from __future__ import annotations

import json
from pathlib import Path

from capture_oracle import digest

ROOT = Path(__file__).resolve().parents[1]
EXE_SHA = "f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794"
CASES = {
    "default-v3": ([], None, 3, "DEFORM", None),
    "pre3-priority": (["DEF_PRE3.INI", "DEF_PRE.INI"], "DEF_PRE3.INI",
                      3, "REA_STARTUP_3D", "REA object from PRE3"),
    "pre2": (["DEF_PRE.INI"], "DEF_PRE.INI", 2, "REA_STARTUP_2D", "REA object from PRE"),
    "multi-priority": (["DEF_MULTI.INI", "DEF_PRE3.INI", "DEF_PRE.INI"],
                       "DEF_MULTI.INI", 3, "REA_STARTUP_MULTI", None),
}
COMPONENTS_3D = [
    3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 3, 3,
    1, 1, 1, 1, 5, 3, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 6, 6,
    6, 1, 1, 1, 1, 1, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 2, 6, 2, 3,
    0, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
]
COMPONENTS_2D = COMPONENTS_3D.copy()
for _index, _value in {
    **{i: 2 for i in range(7)}, 18: 2, 19: 2, 25: 2, 37: 4,
    38: 4, 39: 4, 40: 4, 46: 1, 57: 4, 59: 2, 62: 2,
}.items():
    COMPONENTS_2D[_index] = _value
OBSERVED_CONTROL_INTS = {
    "0xe4": 1, "0xe8": 0, "0xec": 0, "0xf0": 1, "0xf4": 1, "0xf8": 1,
    "0xfc": 0, "0x100": 1, "0x104": 0, "0x108": 1, "0x10c": 0,
    "0x110": 0, "0x114": 0, "0x118": 0, "0x11c": 0, "0x120": 0,
    "0x124": 0, "0x128": 0, "0x12c": 1, "0x130": 0, "0x134": 0,
    "0x160": -1, "0x164": 10, "0x168": 0, "0x16c": 1, "0x1c8": 1,
    "0x2e0": 2, "0x2e4": 1, "0x310": 200, "0x314": 1,
    "0x318": 2, "0x31c": 200, "0x320": 1,
}
OBSERVED_CONTROL_DOUBLES = {
    "0x170": 0, "0x178": 0, "0x1d0": 0, "0x1d8": 0, "0x260": 0,
    "0x268": 0, "0x270": 0, "0x278": 0, "0x280": 0, "0x2c0": 1,
    "0x2c8": 0.5, "0x2d0": 0.005, "0x2d8": 0.05, "0x3a8": 1e9,
    "0x3b0": 1, "0x3b8": 0.75, "0x3d0": 20, "0x660": 1e-6, "0x668": 1e-4,
}


def check(condition: bool, message: str) -> None:
    if not condition:
        raise ValueError(message)


def read(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8-sig"))


def rel(path: Path) -> str:
    return path.resolve().relative_to(ROOT).as_posix()


def validate_observed_controls(root: dict) -> None:
    dimension = root["dimension"]
    check(dimension in (2, 3), "unexpected root dimension")
    integers, doubles = OBSERVED_CONTROL_INTS.copy(), OBSERVED_CONTROL_DOUBLES.copy()
    if dimension == 2:
        integers.update({"0x2e0": 1, "0x318": 1})
        doubles.update({"0x260": 0.1, "0x2c0": 0, "0x2d0": 0.001, "0x2d8": 0.01})
    check(root["control_ints"] == integers, "observed control integer values differ")
    check(root["control_doubles"] == doubles, "observed control double values differ")
    check(root["component_counts"] == (COMPONENTS_3D if dimension == 3 else COMPONENTS_2D),
          "dimension-dependent component counts differ")
    check(root["simulation_title"] == "SIMULATION" and
          root["operation_name"] == "OPERATION 1" and
          root["simulation_name"] == "SIMULATION 1" and
          root["run_type"] == 1, "initial strings/run type differ")


def normalize_root(root: dict, roles: dict[str, str], complete: bool) -> dict:
    check(root["truncated"] is False, "object capture truncated")
    check(root["object_count"] == root["object_list_count"] == len(root["objects"]),
          "object traversal count mismatch")
    check(root["material_count"] == 0, "unexpected material data in these fixtures")
    check(root["object_owner"] == root["address"], "object-list owner is not its root")
    if root["objects"]:
        check(root["object_head"] == root["objects"][0]["address"] and
              root["object_tail"] == root["objects"][-1]["address"],
              "list endpoint mismatch")
    else:
        check(root["object_head"] == root["object_tail"] == "0x0", "nonempty list links")
    validate_observed_controls(root)
    pointers = {"address", "object_head", "object_tail", "object_owner", "alternate_root",
                "database_helper"}
    result = {k: v for k, v in root.items() if k not in pointers | {"objects", "truncated"}}
    result["objects"] = [{k: v for k, v in obj.items() if k != "address"}
                         for obj in root["objects"]]
    result["identity"] = {
        "root_role": roles[root["address"]], "object_list_owner": "self",
        "head_object_id": root["objects"][0]["id"] if root["objects"] else None,
        "tail_object_id": root["objects"][-1]["id"] if root["objects"] else None,
        "alternate_root": roles.get(root["alternate_root"], "null"),
    }
    check(root["alternate_root"] in roles or root["alternate_root"] == "0x0",
          "unknown alternate-root identity")
    if complete:
        result["identity"]["database_helper"] = roles[root["database_helper"]]
    return result


def validate_case(name: str) -> dict:
    folder = ROOT / "analysis" / "oracle" / f"startup-{name}"
    evidence_path = ROOT / "analysis" / f"startup-runtime-{name}.json"
    run_path, pid_path = folder / "run.json", folder.with_suffix(".pid.json")
    evidence, run, pid_manifest = read(evidence_path), read(run_path), read(pid_path)
    capture = evidence["normalized_result"]
    exe = Path(run["executable"])
    check(run["executable_sha256"] == run["executable_sha256_after"] == digest(exe) == EXE_SHA,
          "executable identity mismatch")
    check(run["created_suspended"] is True and run["primary_thread_resumed"] is True and
          run["resume_calls"] == run["resume_previous_suspend_count"] == 1 and
          run["creation_flags"] == 0x08000004, "original primary thread startup not bound")
    check(run["status"] == "completed" and run["exit_code"] == 0 and
          run["terminated_by_helper"] is False and run["cleanup_error"] is None and
          not run.get("handle_close_errors"), "process completion/cleanup failure")
    check(run["pid"] == capture["pid"] == evidence["parameters"]["pid"] == pid_manifest["pid"],
          "PID mismatch")
    for key in ("primary_thread_id", "executable", "executable_sha256", "cwd", "ready_gate"):
        check(pid_manifest[key] == run[key], f"launch manifest mismatch: {key}")
    check(pid_manifest["created_suspended"] is True and
          pid_manifest["primary_thread_resumed"] is False, "manifest was not emitted suspended")
    check(capture["mode"] == "attach" and capture["state"] == "detached" and
          capture["messages_truncated"] is False and capture["cleanup_error"] is None,
          "capture lifecycle/completeness failure")
    script = Path(capture["source_path"])
    check(script == folder.with_suffix(".js") and digest(script) == capture["source_sha256"],
          "probe source identity mismatch")
    for output in run["files"]:
        path = folder / output["name"]
        check(path.stat().st_size == output["bytes"] and digest(path) == output["sha256"],
              f"output hash mismatch: {path}")
    check((folder / "stdin.bin").read_bytes() == b"\nE\nY\n" and
          run["stdin"] == "\nE\nY\n" and (folder / "stderr.bin").read_bytes() == b"",
          "unexpected transcript or stderr")
    expected_inputs, selected, final_dimension, problem_id, object_name = CASES[name]
    check({Path(item["copy"]).name for item in run["inputs"]} == set(expected_inputs),
          "configuration fixture set mismatch")
    inputs = []
    for item in run["inputs"]:
        check(len({item["sha256_before"], item["sha256_after"], item["source_sha256_after"],
                   digest(Path(item["source"])), digest(Path(item["copy"]))}) == 1,
              "configuration source/copy changed")
        inputs.append({"name": Path(item["copy"]).name, "sha256": item["sha256_before"],
                       "source": rel(Path(item["source"]))})
    check(pid_manifest["inputs"] == [
        {k: item[k] for k in ("source", "copy", "sha256_before")} for item in run["inputs"]
    ], "initial input manifest mismatch")
    messages = capture["messages"]
    ids = {s["script_id"] for s in capture["scripts"]}
    check(len(ids) == 1 and all(m["script_id"] in ids and m["type"] == "send" for m in messages),
          "script error/message identity mismatch")
    payloads = [m["payload"] for m in messages]
    check(payloads[0]["operation"] == "hooks-installed" and payloads[0]["pid"] == run["pid"],
          "hooks were not first in capture")
    events = payloads[1:]
    check([p["sequence"] for p in events] == list(range(1, len(events) + 1)),
          "event sequence gap")
    check(not any(p["operation"] == "probe-error" for p in events), "passive probe error")
    prefix = [
        "main-enter", "arguments-enter", "arguments-leave", "context-construction-enter",
        "root-construction-enter", "set-dimension", "root-construction-leave",
        "root-construction-enter", "set-dimension", "root-construction-leave",
        "link-roots", "set-dimension", "set-dimension", "context-construction-leave",
    ]
    check([p["operation"] for p in events[:14]] == prefix, "startup constructor order changed")
    check(events[0]["argc"] == 1 and events[2]["mode_mask"] == 0,
          "capture did not use default interactive argument route")
    constructed = events[13]["state"]
    context = int(constructed["context"], 16)
    roles = {
        hex(context): "context", hex(context + 0x190): "primary",
        hex(context + 0x4eb0): "secondary", hex(context + 0x3598): "primary_database",
        hex(context + 0x82b8): "secondary_database",
    }
    check(constructed["primary"]["address"] == hex(context + 0x190) and
          constructed["secondary"]["address"] == hex(context + 0x4eb0) and
          constructed["primary_database"] == hex(context + 0x3598) and
          constructed["secondary_database"] == hex(context + 0x82b8),
          "context member addresses differ")
    roots = [p["root"] for p in events if p["operation"] == "root-construction-leave"]
    check(len(roots) == 2 and [r["dimension"] for r in roots] == [3, 3],
          "constructor dimension defaults differ")
    for index, root in enumerate(roots):
        check(root["address"] == constructed[("primary", "secondary")[index]]["address"] and
              root["alternate_root"] == "0x0" and root["object_count"] == 0 and
              root["problem_id"] == "DEFORM", "constructor root defaults differ")
    check(events[10]["primary"] == roots[0]["address"] and
          events[10]["secondary"] == roots[1]["address"], "root link call identity mismatch")
    dims = [(roles[p["root"]], p["value"]) for p in events[:14] if p["operation"] == "set-dimension"]
    check(dims == [("primary", 3), ("secondary", 3), ("secondary", 2), ("secondary", 2)],
          "constructor/context dimension transition mismatch")

    snapshots = []
    for event in events:
        state = event.get("state")
        if not state:
            continue
        check(state["context"] == constructed["context"], "context identity changed")
        check(state["primary"]["alternate_root"] == state["secondary"]["address"] and
              state["secondary"]["alternate_root"] == "0x0", "root links changed")
        check(state["primary"]["database_helper"] == state["primary_database"] and
              state["secondary"]["database_helper"] == state["secondary_database"],
              "database-helper binding mismatch")
        for side in ("primary", "secondary"):
            check(state[side]["address"] == constructed[side]["address"],
                  "root identity changed")
        snapshots.append({
            "sequence": event["sequence"], "phase": event["operation"],
            "primary": normalize_root(state["primary"], roles, True),
            "secondary": normalize_root(state["secondary"], roles, True),
        })
    check(constructed["primary"]["dimension"] == 3 and
          constructed["secondary"]["dimension"] == 2 and
          constructed["primary"]["object_count"] == constructed["secondary"]["object_count"] == 0,
          "context-complete default roots differ")
    keyword_enters = [p for p in events if p["operation"] == "startup-keyword-enter"]
    keyword_leaves = [p for p in events if p["operation"] == "startup-keyword-leave"]
    check([p["path"] for p in keyword_enters] == ([] if selected is None else [selected]) and
          [p["path"] for p in keyword_leaves] == ([] if selected is None else [selected]),
          "startup configuration precedence differs")
    if selected:
        check(keyword_enters[0]["root"] == constructed["primary"]["address"] and
              keyword_leaves[0]["status"] == 0, "startup keyword import target/status mismatch")
    menus = [p for p in events if p["operation"] == "main-menu-enter"]
    stdout = (folder / "stdout.bin").read_text()
    if name == "multi-priority":
        check(not menus and events[-1]["operation"] == "startup-keyword-leave" and
              not any(p["operation"] in ("environment-leave", "main-leave") for p in events),
              "multi-operation startup unexpectedly returned to interactive route")
        check("Multiple operation keyword processed" in stdout and
              "DEFORM Input Preparation Main Menu" not in stdout, "multi console route mismatch")
        final_state = keyword_leaves[0]["state"]
    else:
        check(len(menus) == 1 and events[-1] == {
            "sequence": len(events), "operation": "main-leave", "status": 0
        }, "interactive process ending differs")
        check(events[-2]["operation"] == "main-menu-enter" and
              events[-3]["operation"] == "environment-leave",
              "interactive environment/menu ordering differs")
        final_state = menus[0]["state"]
        check(f"Change Problem ID [{problem_id}]" in stdout, "console problem ID mismatch")
        if selected:
            check(keyword_leaves[0]["sequence"] < menus[0]["sequence"] and
                  keyword_leaves[0]["state"] == final_state, "configuration state changed before menu")
    check(final_state["primary"]["dimension"] == final_dimension and
          final_state["secondary"]["dimension"] == 2 and
          final_state["primary"]["problem_id"] == problem_id, "final root selection differs")
    objects = final_state["primary"]["objects"]
    check(final_state["secondary"]["objects"] == [], "secondary unexpectedly populated")
    if object_name is None:
        check(objects == [], "unexpected startup object")
    else:
        check(len(objects) == 1 and
              {k: v for k, v in objects[0].items() if k != "address"} == {
                  "id": 1, "type": 0, "name": object_name, "mesh_flag": 0, "nodes": 0, "elements": 0,
              }, "startup-created object differs")
    if name == "default-v3":
        check(final_state == constructed, "no-config startup changed observed root state")
    timeline = []
    for event in events:
        item = {k: v for k, v in event.items() if k not in {"state", "root", "address",
                                                          "primary", "secondary"}}
        for key in ("root", "address", "primary", "secondary"):
            if isinstance(event.get(key), str):
                item[key + "_role"] = roles[event[key]]
        timeline.append(item)
    return {
        "case": name, "evidence_id": evidence["evidence_id"],
        "evidence_file": rel(evidence_path), "evidence_sha256": digest(evidence_path),
        "run_record": rel(run_path), "run_record_sha256": digest(run_path),
        "launch_manifest": rel(pid_path), "launch_manifest_sha256": digest(pid_path),
        "probe_source": rel(script), "probe_sha256": capture["source_sha256"],
        "pid": run["pid"], "primary_thread_id": run["primary_thread_id"],
        "inputs": inputs, "selected_configuration": selected,
        "event_count": len(events), "timeline": timeline,
        "constructor_roots": [normalize_root(root, roles, False) for root in roots],
        "snapshots": snapshots, "process_exit": {
            "exit_code": 0, "terminated_by_helper": False,
            "main_leave_observed": name != "multi-priority",
            "first_main_menu_observed": name != "multi-priority",
        },
    }


def main() -> None:
    cases = [validate_case(name) for name in CASES]
    static_path = ROOT / "analysis" / "startup-0x140009f20.json"
    static = read(static_path)
    check('FUN_14047ab18(0)' in static["pseudocode"] and
          "DEF_MULTI.INI" in static["pseudocode"], "static termination-path anchor missing")
    report = {
        "target": "source/DEF_PRE_64.exe", "target_sha256": EXE_SHA, "result": "PASS",
        "method": "Validate four retained REA passive captures of processes created with their primary threads suspended.",
        "capture_count": 4,
        "excluded_superseded_captures": [
            {"file": "analysis/startup-runtime-default.json", "reason": "Earlier probe error; excluded."},
            {"file": "analysis/startup-runtime-default-v2.json", "reason": "Earlier bounded-string decoding was incorrect; excluded."},
        ],
        "assertions": [
            "Hooks are installed before captured main entry, argument parsing, context construction, and both root constructors.",
            "Each process was created suspended; one ResumeThread returned previous suspend count 1; all processes exited 0 without helper termination.",
            "Both root constructors initially use dimension 3 and empty lists; context construction then leaves primary 3 and secondary 2.",
            "Root/list ownership, primary-to-secondary link, and per-root database-helper bindings match observed context offsets.",
            "No-configuration first menu preserves observed constructor/context field values and DEFORM problem ID.",
            "With PRE3 and PRE present, only PRE3 is imported, creating object 1 before the first menu.",
            "With PRE only, the primary becomes dimension 2 while the secondary remains 2; its object exists before the menu.",
            "With MULTI, PRE3, and PRE present, only MULTI is imported; no environment return, main menu, or main return is observed before normal process exit.",
        ],
        "multi_termination_static_support": {
            "file": rel(static_path), "sha256": digest(static_path),
            "evidence_id": static["evidence_id"],
            "description": "The environment routine's MULTI branch invokes the nonreturning FUN_14047ab18(0). Runtime capture corroborates termination but does not directly hook that call.",
        },
        "normalization": [
            "Pointer addresses become role names; original pointer relations are asserted before normalization.",
            "Control maps retain offsets relative to root+0x750 and exact captured numeric values.",
            "Control offset 0xfc is observed as zero here; no initializer for that field has been established, so zero is not specified as a guaranteed default.",
            "All 79 component-count entries are preserved in observed order; unknown semantic labels are not invented.",
            "Constructor snapshots and first-menu snapshots remain distinct phases.",
        ],
        "scope": [
            "Default argc=1 route and three exact small initialization fixtures in this executable.",
            "Selected model fields and constructors, not all process globals or every library constructor.",
            "No allocation-failure, filesystem-error, or general startup-command equivalence claim.",
            "Instrumentation itself may execute observer machinery while the original primary thread is suspended.",
            "Verifier reads existing evidence only; it does not execute the original application.",
        ],
        "cases": cases,
    }
    output = ROOT / "analysis" / "startup-observations.json"
    output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print("PASS: 4 bound suspended-startup captures; constructors, defaults, precedence, and process endings verified")
    print(rel(output))


if __name__ == "__main__":
    main()
