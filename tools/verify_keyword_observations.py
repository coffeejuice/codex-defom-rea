"""Verify and summarize retained original-application keyword observations.

This reads existing evidence only; it neither launches the original program nor
tests a replacement importer. Assertions describe the two measured transcripts.
"""
from __future__ import annotations

import json
from pathlib import Path

from capture_oracle import digest

ROOT = Path(__file__).resolve().parents[1]
EXE_SHA256 = "f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794"
ORDERS = [
    ["01_object.KEY", "02_name_patch.KEY", "01_object.KEY", "03_repeated.KEY",
     "04_name_before_type.KEY", "05_unknown.KEY", "06_invalid_numeric.KEY"],
    ["07_tetrahedron.KEY", "08_relabelled_coordinates.KEY", "09_truncated_coordinates.KEY",
     "07_tetrahedron.KEY", "10_sparse_id.KEY", "11_type_six.KEY", "12_lowercase.KEY"],
]


def check(condition: bool, message: str) -> None:
    if not condition:
        raise ValueError(message)


def read(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8-sig"))


def relative(path: Path) -> str:
    return path.resolve().relative_to(ROOT).as_posix()


def normalize_object(obj: dict) -> dict:
    raw = bytes(obj["name_bytes"])
    check(b"\0" in raw, "name probe must contain a terminator")
    name = raw.split(b"\0", 1)[0]
    result = {key: value for key, value in obj.items()
              if key not in {"address", "coordinates_pointer",
                             "connectivity_pointer", "name_bytes"}}
    # Latin-1 is a reversible byte-to-codepoint display, not an encoding claim.
    result["name"] = name.decode("latin-1")
    result["name_bytes_hex"] = name.hex()
    return result


def normalize_state(state: dict) -> dict:
    check(state.get("truncated") is False, "state probe was truncated")
    check(not state.get("cycle"), "object traversal encountered a cycle")
    objects = state["objects"]
    check(state["object_count"] == state["list_count"] == len(objects),
          "object traversal/count mismatch")
    check(len({obj["address"] for obj in objects}) == len(objects),
          "duplicate object addresses in one state")
    for obj in objects:
        if obj["nodes"] > 0:
            check(len(obj.get("coordinates", [])) == obj["nodes"] * 3,
                  "coordinate probe incomplete")
        if obj["elements"] > 0:
            check(len(obj.get("connectivity", [])) == obj["elements"] * obj["arity"],
                  "connectivity probe incomplete")
    return {"object_count": state["object_count"], "list_count": state["list_count"],
            "objects": [normalize_object(obj) for obj in objects]}


def verify_run(number: int) -> dict:
    evidence_path = ROOT / "analysis" / f"keyword-runtime-{number}.json"
    run_path = ROOT / "analysis" / "oracle" / f"keyword-lifecycle-{number}" / "run.json"
    evidence, run = read(evidence_path), read(run_path)
    capture = evidence["normalized_result"]
    check(run["executable_sha256"] == EXE_SHA256 == digest(Path(run["executable"])),
          "executable identity mismatch")
    check(run["status"] == "completed" and run["exit_code"] == 0,
          "reference process did not complete normally")
    check(run["pid"] == capture["pid"] == evidence["parameters"]["pid"],
          "capture PID mismatch")
    check(capture["mode"] == "attach" and capture["state"] == "detached",
          "unexpected capture lifecycle")
    check(capture["messages_truncated"] is False and capture["cleanup_error"] is None,
          "capture truncated or cleanup failed")
    script = Path(capture["source_path"])
    check(script == ROOT / "analysis" / "oracle" / f"keyword-lifecycle-{number}.js",
          "unexpected passive probe source")
    check(digest(script) == capture["source_sha256"], "probe source hash mismatch")
    check(run["ordered_imports"] == ORDERS[number - 1], "import order mismatch")
    run_dir = run_path.parent
    check((run_dir / "stderr.bin").read_bytes() == b"", "reference stderr is nonempty")
    check((run_dir / "stdin.bin").read_bytes() == run["stdin"].encode("ascii"),
          "saved transcript mismatch")
    for item in run["files"]:
        path = run_dir / item["name"]
        check(path.stat().st_size == item["bytes"] and digest(path) == item["sha256"],
              f"recorded output identity mismatch: {path}")
    fixture_hashes = {}
    for item in run["inputs"]:
        hashes = {item["sha256_before"], item["sha256_after"], item["source_sha256_after"],
                  digest(Path(item["copy"])), digest(Path(item["source"]))}
        check(len(hashes) == 1, f"fixture changed: {item['name']}")
        fixture_hashes[item["name"]] = item["sha256_before"]
    messages = capture["messages"]
    script_ids = {script["script_id"] for script in capture["scripts"]}
    check(len(script_ids) == 1 and
          all(message["script_id"] in script_ids for message in messages),
          "message/probe script identity mismatch")
    check(all(message["type"] == "send" for message in messages),
          "Frida script error or unexpected message")
    payloads = [message["payload"] for message in messages]
    hooks = [p for p in payloads if p["operation"] == "hooks-installed"]
    check(len(hooks) == 1 and hooks[0]["pid"] == run["pid"], "hook/PID mismatch")
    check(payloads[0] == hooks[0], "capture began after observations")
    events = payloads[1:]
    check(all(p["operation"] != "probe-error" for p in events), "probe failed")
    check([p["sequence"] for p in events] == list(range(1, len(events) + 1)),
          "event sequence gap")
    groups, current = [], None
    for event in events:
        if "state" in event:
            normalize_state(event["state"])
        if event["operation"] == "import-before":
            check(current is None, "nested/unclosed import in bounded fixture")
            current = []
        check(current is not None, "event outside import")
        current.append(event)
        if event["operation"] == "import-after":
            groups.append(current)
            current = None
    check(current is None and len(groups) == 7, "incomplete import boundaries")
    check([g[0]["file"] for g in groups] == ORDERS[number - 1],
          "observed import order mismatch")
    imports = []
    previous_after = None
    for ordinal, group in enumerate(groups, 1):
        name = group[0]["file"]
        check(all(event["file"] == name for event in group), "file context mismatch")
        final_before = [p for p in group if p["operation"] == "finalize-before"]
        final_after = [p for p in group if p["operation"] == "finalize-after"]
        check(len(final_before) == len(final_after) == 1, "missing/repeated finalization")
        check(group[-3:] == [final_before[0], final_after[0], group[-1]],
              "unexpected finalization ordering")
        active_handler = None
        for event in group:
            if event["operation"] == "handler-before":
                check(active_handler is None, "nested handler")
                active_handler = event
            elif event["operation"] == "handler-after":
                check(active_handler is not None and
                      all(event[k] == active_handler[k] for k in ("rva", "id", "keyword")),
                      "handler entry/exit mismatch")
                active_handler = None
        check(active_handler is None, "handler exit missing")
        before, after = group[0]["state"], group[-1]["state"]
        check(before["address"] == after["address"], "model address changed during import")
        check(final_after[0]["state"] == after, "state changed after finalization")
        if previous_after is not None:
            check(before == previous_after, "state changed between consecutive imports")
        previous_after = after
        old_objects = {obj["id"]: obj for obj in before["objects"]}
        retained = {str(obj["id"]): old_objects[obj["id"]]["address"] == obj["address"]
                    for obj in after["objects"] if obj["id"] in old_objects}
        imports.append({
            "ordinal": ordinal, "file": name, "fixture_sha256": fixture_hashes[name],
            "sequence_range": [group[0]["sequence"], group[-1]["sequence"]],
            "status": group[-1]["status"],
            "retained_object_identity": retained,
            "before": normalize_state(before), "after": normalize_state(after),
            "finalization": {
                "sequences": [final_before[0]["sequence"], final_after[0]["sequence"]],
                "observed_fields_changed":
                    normalize_state(final_before[0]["state"]) != normalize_state(final_after[0]["state"]),
            },
            "keywords": [{key: p[key] for key in ("sequence", "keyword", "id", "category")}
                         for p in group if p["operation"] == "keyword-resolved"],
            "handler_results": [
                {key: p[key] for key in ("sequence", "keyword", "id", "status")}
                for p in group if p["operation"] == "handler-after"],
        })
    return {
        "run": number, "evidence_id": evidence["evidence_id"],
        "evidence_file": relative(evidence_path), "evidence_sha256": digest(evidence_path),
        "run_record": relative(run_path), "run_record_sha256": digest(run_path),
        "pid": run["pid"], "probe_source": relative(script),
        "probe_sha256": capture["source_sha256"], "event_count": len(events),
        "imports": imports,
    }


def verify_behavior(runs: list[dict]) -> list[str]:
    first, second = (run["imports"] for run in runs)
    obj = lambda item: item["after"]["objects"][0]
    check(first[0]["before"]["objects"] == [] and second[0]["before"]["objects"] == [],
          "runs did not start with empty object lists")
    check(all(i["status"] == 0 for i in first), "unexpected first-run status")
    check(obj(first[0])["type"] == obj(first[1])["type"] == 1,
          "name-only patch changed type")
    check(first[1]["retained_object_identity"] == {"1": True} and
          obj(first[1])["name"] == "REA renamed object", "name patch did not retain object")
    check(first[2]["after"] == first[0]["after"] and
          first[2]["retained_object_identity"] == {"1": True}, "reimport behavior changed")
    check(obj(first[3])["name"] == "REA second name" and obj(first[3])["type"] == 0,
          "repeated definitions did not leave final values")
    check(obj(first[4])["name"] == "REA name before type" and obj(first[4])["type"] == 1,
          "name-before-type update changed")
    check(obj(first[5])["name"] == "REA after unknown" and
          any(k["keyword"] == "REA_UNKNOWN_TEST" and k["id"] == 305 and k["category"] == 0
              for k in first[5]["keywords"]), "unknown keyword behavior changed")
    check(obj(first[6])["type"] == 7 and obj(first[6])["name"] == "REA after invalid value",
          "malformed type behavior changed")
    check([i["status"] for i in second] == [0, 0, 2, 0, 0, 0, 0],
          "mesh/error run statuses changed")
    xyz = [0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1]
    check(obj(second[0])["coordinates"] == xyz and
          obj(second[0])["connectivity"] == [1, 2, 3, 4], "tetrahedron data mismatch")
    check(obj(second[1])["coordinates"] == [10, 0, 0, 11, 0, 0, 10, 1, 0, 10, 0, 1] and
          obj(second[1])["connectivity"] == [1, 2, 3, 4], "row-label behavior changed")
    check(obj(second[2])["coordinates"] == [20, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 0] and
          obj(second[2])["nodes"] == 4 and obj(second[2])["connectivity"] == [1, 2, 3, 4],
          "partial failed read did not retain observed data")
    check(second[3]["after"] == second[0]["after"], "mesh reimport did not restore values")
    sparse = second[4]["after"]["objects"]
    check([o["id"] for o in sparse] == [1, 2, 3, 4] and
          [o["type"] for o in sparse] == [1, 7, 7, 0], "sparse-ID expansion changed")
    check(sparse[3]["name"] == "REA sparse fourth object", "sparse requested name mismatch")
    check(second[5]["before"] == second[5]["after"], "type code 6 changed observed fields")
    check(any(k["keyword"] == "objtyp" and k["id"] == 305 and k["category"] == 0
              for k in second[6]["keywords"]) and
          obj(second[6])["type"] == 1 and obj(second[6])["name"] == "REA after lowercase",
          "lowercase/continued-parse behavior changed")
    check(all(i["retained_object_identity"].get("1", True)
              for run in runs for i in run["imports"]), "object 1 identity was not retained")
    return [
        "Both process runs begin with empty observed object lists.",
        "Name-only import preserves object identity and type in the tested sequence.",
        "Repeated import updates object 1 in place; repeated name/type keywords leave the last tested values.",
        "Name-before-type succeeds for an already existing object; fresh-object ordering is not established by this run.",
        "Unknown REA_UNKNOWN_TEST resolves to ID 305/category 0; later OBJNAM is processed.",
        "OBJTYP 1 BAD 0 produces status 0, internal type 7, and parsing continues to the following name.",
        "The tetrahedron imports four XYZ rows and one four-node connectivity row.",
        "Changed coordinate row labels and element row label do not reorder the tested arrays.",
        "Truncated RZ returns status 2 after replacing coordinates with two read rows and two zero rows; existing connectivity survives.",
        "Finalization still runs before the failed RZ import returns.",
        "Reimporting the tetrahedron restores the observed mesh values.",
        "Requesting name for ID 4 expands IDs to 1..4; gap objects 2/3 have type 7 and requested object 4 has type 0.",
        "OBJTYP code 6 retains prior observed state for object 1 in this run.",
        "Lowercase objtyp resolves as unknown; following uppercase OBJNAM still updates the name.",
    ]


def main() -> None:
    runs = [verify_run(number) for number in (1, 2)]
    assertions = verify_behavior(runs)
    report = {
        "target": "source/DEF_PRE_64.exe", "target_sha256": EXE_SHA256,
        "method": "Validation of retained REA passive Frida captures and original console-run records",
        "result": "PASS", "process_runs": 2, "imports": 14,
        "normalization": [
            "Object names stop at the first NUL; name_bytes_hex preserves exact name bytes.",
            "name is a reversible Latin-1 display, not a claim about the application's text encoding.",
            "Addresses and pointer values are excluded from state summaries; within-run object identity is checked separately.",
            "Coordinates, connectivity, and offset-named auxiliary array prefixes are retained without assigning unverified meaning.",
        ],
        "scope": [
            "Two exact 3D transcripts in the identified Windows x64 executable; each run shares state across its seven imports.",
            "Observed fields only, not a full memory comparison or complete application-model specification.",
            "Only selected keywords and small valid/invalid fixtures; no general error-handling or allocation-failure guarantee.",
            "No runtime rerun, C++ importer implementation, or replacement-equivalence claim is made by this verifier.",
        ],
        "assertions": assertions, "runs": runs,
    }
    output = ROOT / "analysis" / "keyword-observations.json"
    output.write_text(json.dumps(report, indent=2, ensure_ascii=True) + "\n", encoding="utf-8")
    print(f"PASS: 2 bound process captures, 14 complete imports, {len(assertions)} behavior assertions")
    print(relative(output))


if __name__ == "__main__":
    main()
