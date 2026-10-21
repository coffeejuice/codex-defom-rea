"""Compare integrated C++ mesh storage with retained original snapshots.

The small fixture reader below supplies replay inputs only; it is not a
replacement for the application's keyword parser.
"""
import argparse
import json
from pathlib import Path
import subprocess

from capture_oracle import digest

ROOT = Path(__file__).resolve().parents[1]


def require(condition, message):
    if not condition:
        raise ValueError(message)


def fixture_operations(path):
    lines = path.read_text().splitlines()
    result = []
    i = 0
    while i < len(lines):
        fields = lines[i].split()
        i += 1
        # Only the separately bound deactivation fixtures use these type codes.
        # This supplies an observed lifecycle operation, not general OBJTYP parsing.
        if fields and fields[0] == "OBJTYP" and fields[2] in ("99", "6"):
            result.append(("deactivate",))
            continue
        if not fields or fields[0] not in ("RZ", "ELMCON"):
            continue
        node = fields[0] == "RZ"
        count = int(fields[2])
        width = 3 if node else int(fields[3])
        values = []
        for _ in range(count):
            if i == len(lines):
                break
            row = lines[i].split()
            i += 1
            require(len(row) == width + 1, "replay fixture has unexpected row shape")
            values.extend((float if node else int)(v) for v in row[1:])
        complete = len(values) == count * width
        values += [0] * (count * width - len(values))
        result.append(("nodes" if node else "elements", count, width, values, complete))
    return result


def normalized_expected(obj):
    descriptors = {"node": [], "element": []}
    for d in obj["descriptors"]:
        group = "node" if d["slot_offset"] < 0x9e0 else "element"
        descriptors[group].append({
            "id": d["id"],
            "shape": {k: d[k] for k in ("count", "width", "multiplicity")},
            "previous": {k: d["old_" + k] for k in ("count", "width", "multiplicity")},
            "values": d["values"] or [],
        })
    result = {
        "nodes": obj["nodes"], "elements": obj["elements"], "arity": obj["arity"],
        "mesh_present": bool(obj["mesh_flag"]),
        "coordinates": obj["coordinates"] or [], "connectivity": obj["connectivity"] or [],
        "descriptors": descriptors,
    }
    if "direct" in obj:
        result["direct_integer_buffers"] = {k: v or [] for k, v in obj["direct"].items()}
        result["total"] = obj["boundary_total"]
        result["boundaries"] = [
            {"nodes": [], "faces": [g["faces"][i:i + 4]
                                   for i in range(0, len(g["faces"] or []), 4)],
             "owners": g["owners"] or []} for g in obj["boundary_groups"]
        ]
    return result


def compare(path, driver, fixtures):
    evidence = json.loads(path.read_text(encoding="utf-8-sig"))
    capture = evidence["normalized_result"]
    require(capture["messages_truncated"] is False and capture["cleanup_error"] is None,
            "incomplete capture")
    require(all(m["type"] == "send" for m in capture["messages"]), "probe error")
    payloads = [m["payload"] for m in capture["messages"]]
    require(payloads[0]["operation"] == "hooks-installed"
            and payloads[0]["pid"] == capture["pid"], "hook identity mismatch")
    events = payloads[1:]
    require([e["sequence"] for e in events] == list(range(1, len(events) + 1)),
            "incomplete event order")
    commands, expected, metadata = [], [], []
    components = None
    operations = []
    fixture_bindings = {}
    for event in events:
        op = event["operation"]
        require(op != "probe-error", "runtime probe error")
        if op == "import-before":
            source = fixtures / event["file"]
            operations = fixture_operations(source)
            fixture_bindings[source.relative_to(ROOT).as_posix()] = digest(source)
            continue
        if op == "import-after":
            require(event["object_count"] == len(event["objects"]) == 1,
                    "replay expects a single original object")
            obj = event["objects"][0]
            require(not operations, "fixture operations were not observed")
        elif op in ("nodes-replaced", "elements-replaced", "object-deactivated"):
            obj = event["object"]
        else:
            raise ValueError("unexpected event: " + op)
        require(obj["dimension"] == 3, "this storage replay covers captured 3D layouts")
        if components is None:
            components = obj["components"]
            require(len(components) == 79, "incomplete component table")
            commands.append(" ".join(map(str, components)))
        require(obj["components"] == components, "component table changed")
        if op == "object-deactivated":
            require(bool(operations) and operations.pop(0) == ("deactivate",),
                    "unexpected object deactivation")
            commands.append("deactivate")
        if op in ("nodes-replaced", "elements-replaced"):
            require(bool(operations), "unexpected allocation")
            kind, count, width, values, complete = operations.pop(0)
            require(op == kind + "-replaced" and count == event["requested_count"],
                    "fixture/capture operation mismatch")
            commands.append(f"nodes {count} 20" if kind == "nodes"
                            else f"elements {count} {width} 0")
        commands.append("snapshot")
        expected.append(normalized_expected(obj))
        metadata.append({"file": event["file"], "operation": op, "sequence": event["sequence"]})
        if op in ("nodes-replaced", "elements-replaced"):
            setter = "set_coordinates" if kind == "nodes" else "set_connectivity"
            commands.append(f"{setter} {len(values)} " + " ".join(map(str, values)))
            if kind == "elements" and complete:
                commands.append("finalize 3")
    process = subprocess.run([str(driver)], input="\n".join(commands) + "\n",
                             text=True, capture_output=True, check=True, timeout=15)
    actual = [json.loads(line) for line in process.stdout.splitlines()]
    require(len(actual) == len(expected), "snapshot count mismatch")
    for index, (a, wanted) in enumerate(zip(actual, expected)):
        for key, value in wanted.items():
            require(a[key] == value,
                    f"{path.name} snapshot {index} {metadata[index]} {key} differs: "
                    f"{a[key]} != {value}")
    return {
        "capture": path.relative_to(ROOT).as_posix(), "sha256": digest(path),
        "evidence_id": evidence["evidence_id"], "pid": capture["pid"],
        "fixtures": fixture_bindings, "snapshots": len(expected),
        "descriptor_snapshots": len(expected) * 53,
        "direct_and_boundary_snapshots": sum("direct_integer_buffers" in e for e in expected),
        "verdict": "exact",
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--driver", type=Path,
                        default=ROOT / "build/Release/mesh_storage_replay.exe")
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    driver = args.driver.resolve(strict=True)
    runs = [compare(ROOT / f"analysis/mesh-arrays-runtime-{n}.json", driver,
                    ROOT / "tests/fixtures/keywords") for n in (1, 2)]
    for path in sorted((ROOT / "analysis").glob("mesh-storage-runtime-*.json")):
        runs.append(compare(path, driver, ROOT / "tests/fixtures/mesh_storage"))
    runs.append(compare(ROOT / "analysis/object-deactivation-runtime.json", driver,
                        ROOT / "tests/fixtures/object_deactivation"))
    report = {
        "status": "pass", "driver_sha256": digest(driver), "runs": runs,
        "comparison": "Integrated counts, mesh flag, coordinates, connectivity and all registered descriptor shapes/history/values; direct buffers and boundary state where captured.",
        "scope": "Recovered 3D bulk mesh replacement with default component controls. Fixture rows supply inputs independently of expected snapshots.",
        "remaining": ["Full keyword scanner and dispatch", "Object identity/type/name lifecycle",
                      "2D allocation and dimension switching", "Nondefault multiplicities",
                      "Sharing, boundary-dependent arrays and derived geometry"]
    }
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {sum(r['snapshots'] for r in runs)} integrated mesh snapshots, "
          f"{sum(r['descriptor_snapshots'] for r in runs)} descriptor snapshots")


if __name__ == "__main__":
    main()
