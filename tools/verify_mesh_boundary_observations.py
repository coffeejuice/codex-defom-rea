"""Validate retained original boundary-builder observations for four imports.

This reads saved evidence only. It does not implement the boundary algorithm,
launch the reference executable, or establish general mesh compatibility.
"""
from __future__ import annotations

import json
from pathlib import Path

from capture_oracle import digest

ROOT = Path(__file__).resolve().parents[1]
EXE_SHA = "f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794"
ORDER = [
    "07_tetrahedron.KEY", "13_two_tetrahedra.KEY",
    "14_duplicate_tetrahedron.KEY", "07_tetrahedron.KEY",
]
TETRA_FACES = [1, 3, 2, 1, 1, 2, 4, 1, 1, 4, 3, 1, 2, 3, 4, 2]
EXPECTATIONS = {
    "07_tetrahedron.KEY": {
        "connectivity": [1, 2, 3, 4],
        "faces": TETRA_FACES, "owners": [1, 1, 1, 1],
    },
    "13_two_tetrahedra.KEY": {
        "connectivity": [1, 2, 3, 4, 2, 3, 4, 5],
        "faces": [1, 3, 2, 1, 1, 2, 4, 1, 1, 4, 3, 1,
                  2, 3, 5, 2, 2, 5, 4, 2, 3, 4, 5, 3],
        "owners": [1, 1, 1, 2, 2, 2],
    },
    "14_duplicate_tetrahedron.KEY": {
        "connectivity": [1, 2, 3, 4, 1, 2, 3, 4],
        "faces": [], "owners": [],
    },
}


def check(condition: bool, message: str) -> None:
    if not condition:
        raise ValueError(message)


def read(path: Path):
    return json.loads(path.read_text(encoding="utf-8-sig"))


def relative(path: Path) -> str:
    return path.resolve().relative_to(ROOT).as_posix()


def verify_static_context() -> list[dict]:
    records = []
    for address in ("0x1401fc290", "0x140354ed0", "0x140355cbf",
                    "0x14043c760", "0x14043c7f0"):
        path = ROOT / "analysis" / f"mesh-{address}.json"
        evidence = read(path)
        check(evidence["subject"]["digest"]["sha256"] == EXE_SHA,
              "static boundary evidence target mismatch")
        records.append({
            "file": relative(path), "file_sha256": digest(path),
            "procedure": address, "evidence_id": evidence["evidence_id"],
        })
    path = ROOT / "analysis" / "mesh-boundary-tables.json"
    table = read(path)
    check(table["target_sha256"] == EXE_SHA, "static table target mismatch")
    evidence = table["read_evidence"]
    check(evidence["subject"]["digest"]["sha256"] == EXE_SHA,
          "table-read target mismatch")
    data = evidence["normalized_result"]
    raw = bytes.fromhex(data["bytes_hex"])
    check(data["complete"] is True and
          data["requested_bytes"] == data["returned_bytes"] == len(raw),
          "static table read is incomplete")
    check(len(raw) % 4 == 0 and
          [int.from_bytes(raw[i:i + 4], "little", signed=True)
           for i in range(0, len(raw), 4)] == table["decoded_little_endian_int32"],
          "static table decoding mismatch")
    records.append({
        "file": relative(path), "file_sha256": digest(path),
        "read_evidence_id": evidence["evidence_id"],
        "call_site_evidence_id": table["call_site_evidence_id"],
        "qualification": table["qualification"],
    })
    return records


def verify_capture() -> dict:
    evidence_path = ROOT / "analysis" / "mesh-boundary-runtime-1.json"
    run_path = ROOT / "analysis" / "oracle" / "mesh-boundary-1" / "run.json"
    probe_path = ROOT / "tools" / "observe_mesh_boundary.js"
    evidence, run = read(evidence_path), read(run_path)
    capture = evidence["normalized_result"]
    check(Path(run["executable"]) == ROOT / "source" / "DEF_PRE_64.exe" and
          run["executable_sha256"] == EXE_SHA == digest(Path(run["executable"])),
          "executable identity mismatch")
    check(run["status"] == "completed" and run["exit_code"] == 0,
          "reference process did not complete normally")
    check(run["pid"] == capture["pid"] == evidence["parameters"]["pid"] and
          capture["session_id"] == evidence["parameters"]["session_id"],
          "capture process/session mismatch")
    check(capture["mode"] == "attach" and capture["state"] == "detached" and
          capture["source_kind"] == "file", "unexpected capture lifecycle")
    check(capture["messages_truncated"] is False and capture["cleanup_error"] is None,
          "capture truncated or cleanup failed")
    script = Path(capture["source_path"])
    check(script == ROOT / "analysis" / "oracle" / "mesh-boundary-1.js" and
          digest(script) == capture["source_sha256"], "passive probe identity mismatch")
    expected = probe_path.read_text(encoding="utf-8-sig").replace(
        "__READY_GATE__", Path(run["ready_gate"]).as_posix())
    check(script.read_text(encoding="utf-8-sig") == expected,
          "generated probe differs from current template")
    check(Path(run["cwd"]) == run_path.parent and run["ordered_imports"] == ORDER,
          "manifest directory/import order mismatch")
    check((run_path.parent / "stdin.bin").read_bytes() == run["stdin"].encode("ascii") and
          (run_path.parent / "stderr.bin").read_bytes() == b"",
          "transcript or stderr mismatch")
    for item in run["files"]:
        path = run_path.parent / item["name"]
        check(path.stat().st_size == item["bytes"] and digest(path) == item["sha256"],
              f"captured file changed: {path}")
    fixtures = {}
    for item in run["inputs"]:
        source, copy = Path(item["source"]), Path(item["copy"])
        check(source == ROOT / "tests" / "fixtures" / "keywords" / item["name"] and
              copy == run_path.parent / item["name"], "fixture path mismatch")
        hashes = {item["sha256_before"], item["sha256_after"],
                  item["source_sha256_after"], digest(source), digest(copy)}
        check(len(hashes) == 1, f"fixture changed: {item['name']}")
        fixtures[item["name"]] = item["sha256_before"]
    check(set(fixtures) == set(ORDER), "fixture set mismatch")
    scripts = {item["script_id"] for item in capture["scripts"]}
    messages = capture["messages"]
    check(len(scripts) == 1 and
          all(m["script_id"] in scripts and m["type"] == "send" for m in messages),
          "script identity mismatch or Frida error")
    payloads = [m["payload"] for m in messages]
    check(payloads[0]["operation"] == "hooks-installed" and
          payloads[0]["pid"] == run["pid"] and int(payloads[0]["base"], 16) > 0,
          "initial hook/PID/base record mismatch")
    events = payloads[1:]
    check(len(events) == 20 and
          [e["sequence"] for e in events] == list(range(1, 21)),
          "event sequence incomplete")
    operations = ["import-before", "boundary-before", "boundary-after",
                  "object-boundary", "import-after"]
    imports = []
    for index, name in enumerate(ORDER):
        group = events[index * 5:index * 5 + 5]
        check([e["operation"] for e in group] == operations and
              all(e["file"] == name for e in group),
              "probe error or incorrect event/file ordering")
        _, before, after, obj, returned = group
        expected = EXPECTATIONS[name]
        check(returned["status"] == 0, "import did not succeed")
        check(before["arity"] == 4 and
              before["elements"] * 4 == len(before["connectivity"]) and
              before["connectivity"] == expected["connectivity"],
              "builder input connectivity mismatch")
        check(after["count"] == len(after["owner_elements"]) and
              len(after["faces"]) == after["count"] * 4,
              "builder output arrays incomplete")
        check(after["faces"] == expected["faces"] and
              after["owner_elements"] == expected["owners"],
              "builder output differs from retained fixture expectation")
        check(obj["id"] == 1 and obj["dimension"] == 3 and
              obj["total"] == after["count"] and len(obj["boundaries"]) == 1,
              "object boundary summary mismatch")
        builder = {key: after[key] for key in ("count", "faces", "owner_elements")}
        check(obj["boundaries"][0] == builder,
              "object consumer differs from builder output")
        for row_index in range(after["count"]):
            row = after["faces"][row_index * 4:row_index * 4 + 4]
            check(row[3] == row[0], "tetrahedron face closing slot mismatch")
            owner = after["owner_elements"][row_index]
            check(1 <= owner <= before["elements"], "owner outside element range")
            element = before["connectivity"][(owner - 1) * 4:owner * 4]
            check(all(node in element for node in row[:3]),
                  "face nodes are not in its observed owner")
        imports.append({
            "ordinal": index + 1, "file": name, "fixture_sha256": fixtures[name],
            "sequence_range": [group[0]["sequence"], group[-1]["sequence"]],
            "status": returned["status"],
            "builder_input": {key: before[key] for key in
                              ("elements", "arity", "connectivity")},
            "builder_output": builder,
            "object_output": {key: obj[key] for key in
                              ("id", "dimension", "total", "boundaries")},
        })
    check(imports[0]["builder_output"] == imports[-1]["builder_output"],
          "reimport did not restore tetrahedron boundary")
    return {
        "evidence_file": relative(evidence_path), "evidence_id": evidence["evidence_id"],
        "evidence_sha256": digest(evidence_path), "run_record": relative(run_path),
        "run_record_sha256": digest(run_path), "pid": run["pid"],
        "probe_source": relative(script), "probe_sha256": capture["source_sha256"],
        "probe_template": relative(probe_path), "probe_template_sha256": digest(probe_path),
        "fixture_hashes": fixtures, "event_count": len(events), "imports": imports,
    }


def main() -> None:
    static = verify_static_context()
    run = verify_capture()
    report = {
        "target": "source/DEF_PRE_64.exe", "target_sha256": EXE_SHA, "result": "PASS",
        "method": "Validate retained passive builder/consumer captures and file/process provenance; no original execution or replacement implementation.",
        "process_runs": 1, "imports": 4, "boundary_counts": [4, 6, 0, 4],
        "assertions": [
            "All four imports return status zero and contain complete ordered builder and object-consumer observations.",
            "One tetrahedron yields four face records; two tetrahedra sharing one face yield six.",
            "Two identical tetrahedra yield no boundary faces in this specific input.",
            "Reimporting the original tetrahedron restores its exact prior face and owner arrays.",
            "Object boundary count, face arrays and owner arrays exactly match builder outputs in every capture.",
        ],
        "array_convention": [
            "Connectivity, faces and owner_elements retain original integer order and numbering.",
            "Each captured face occupies four integers. In these tetrahedron cases the fourth repeats the first.",
            "No sorting or canonicalization is applied during verification; orientation and output order are retained.",
        ],
        "static_context": static,
        "static_findings": [
            "0x140355cbf builds candidate-face records and passes two-int sorting records to 0x14043c760.",
            "0x14043c760 invokes qsort with eight-byte records and comparator 0x14043c7f0.",
            "The comparator compares only the first int32 key; face construction sets that key from a sum of node IDs.",
        ],
        "remaining_questions": [
            "Equal-key qsort ordering and its interaction with duplicate/nonmanifold ownership are unresolved.",
            "The full candidate cancellation, orientation and final ordering rules are not established by these four cases.",
            "Other element arities, 2D boundaries, degenerate input, nonmanifold faces, overflow and allocation failures are untested.",
            "Full geometric finalization and downstream boundary consumers remain outside this report.",
        ],
        "limitations": [
            "This is a bounded observation corpus, not a complete recovered boundary algorithm or a replacement-equivalence claim.",
            "Static helper/table evidence describes analyzed code and bytes; it is not additional runtime coverage.",
            "The probe starts before fixture input; this harness does not claim suspended-at-creation startup tracing.",
        ],
        "run": run,
    }
    output = ROOT / "analysis" / "mesh-boundary-observations.json"
    output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print("PASS: 1 bound process capture, 4 imports, exact builder/object boundaries 4/6/0/4")
    print(relative(output))


if __name__ == "__main__":
    main()
