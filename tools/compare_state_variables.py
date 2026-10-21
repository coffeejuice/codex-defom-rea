"""Replay captured original mesh allocations against the C++ state-variable core.

Uses retained evidence; never launches the original executable. Full provenance
and static-contract checks are in verify_mesh_array_observations.py.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import subprocess

from capture_oracle import digest

ROOT = Path(__file__).resolve().parents[1]
FIELDS = ("count", "width", "multiplicity", "old_count", "old_width",
          "old_multiplicity", "length")


def require(condition: bool, message: str) -> None:
    if not condition:
        raise ValueError(message)


def compare(number: int, driver: Path) -> dict:
    source = ROOT / "analysis" / f"mesh-arrays-runtime-{number}.json"
    evidence = json.loads(source.read_text(encoding="utf-8-sig"))
    capture = evidence["normalized_result"]
    require(capture["messages_truncated"] is False and capture["cleanup_error"] is None,
            "incomplete runtime capture")
    messages = capture["messages"]
    require(all(m["type"] == "send" for m in messages), "runtime script error")
    payloads = [m["payload"] for m in messages]
    require(payloads[0]["operation"] == "hooks-installed" and
            payloads[0]["pid"] == capture["pid"], "hook identity mismatch")
    events = payloads[1:]
    require([e["sequence"] for e in events] == list(range(1, len(events) + 1)),
            "missing event")
    snapshots = []
    for event in events:
        require(event["operation"] != "probe-error", "probe error")
        if event["operation"] in ("nodes-replaced", "elements-replaced"):
            snapshots.append((event, event["object"]))
        elif event["operation"] == "import-after":
            require(event["object_count"] == len(event["objects"]) == 1,
                    "this replay requires the observed single-object cases")
            snapshots.append((event, event["objects"][0]))
    require(len(snapshots) == (11 if number == 1 else 9), "unexpected snapshot count")
    reference = snapshots[0][1]
    components = reference["components"]
    require(len(components) == 79 and reference["dimension"] == 3,
            "unsupported component layout")
    for _, obj in snapshots:
        require(obj["components"] == components and obj["address"] == reference["address"],
                "model identity/layout changed")
        require(len(obj["descriptors"]) == 53, "incomplete descriptor map")

    comparisons = []
    for index, first in enumerate(reference["descriptors"]):
        identifier, slot = first["id"], first["slot_offset"]
        policy = {0x54d210: 1, 0x54e028: 2}.get(first["vtable_rva"], 0)
        lines = [f"{identifier} {policy}", " ".join(map(str, components))]
        expected = []
        for event, obj in snapshots:
            descriptor = obj["descriptors"][index]
            require((descriptor["id"], descriptor["slot_offset"], descriptor["vtable_rva"]) ==
                    (identifier, slot, first["vtable_rva"]), "descriptor identity changed")
            node = slot < 0x9e0
            relevant = event["operation"] == ("nodes-replaced" if node else "elements-replaced")
            if relevant:
                lines.append("clear")
                count = obj["nodes" if node else "elements"]
                if count > 0:
                    # STS/TEPS/DAMAG use owner-dependent multiplicity. The
                    # captured default controls select 1 in these fixtures.
                    multiplicity = 1 if identifier in (38, 43, 45) else -1
                    lines.append(f"allocate {count} -1 {multiplicity}")
                    if identifier == 8:
                        lines.append("fill 20")
                    elif identifier == 44:
                        lines.append("fill 1")
                    elif identifier == 45:
                        lines.append("fill 0")
            lines.append("snapshot")
            expected.append([descriptor[k] for k in FIELDS] + (descriptor["values"] or []))
        result = subprocess.run([str(driver)], input="\n".join(lines) + "\n",
                                text=True, capture_output=True, timeout=10, check=True)
        rows = [[float(x) for x in line.split()] for line in result.stdout.splitlines()]
        require(len(rows) == len(expected), "C++ snapshot count mismatch")
        for position, (actual, wanted) in enumerate(zip(rows, expected)):
            require(actual == wanted,
                    f"run {number}, descriptor {identifier}, slot {slot:#x}, "
                    f"snapshot {position}: expected {wanted}, got {actual}")
        comparisons.append({"id": identifier, "slot_offset": hex(slot),
                            "snapshots": len(rows), "verdict": "exact"})
    return {"capture": source.relative_to(ROOT).as_posix(), "capture_sha256": digest(source),
            "evidence_id": evidence["evidence_id"], "pid": capture["pid"],
            "descriptors": comparisons, "snapshot_comparisons": len(snapshots) * 53}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--driver", type=Path,
                        default=ROOT / "build" / "Release" / "state_variable_replay.exe")
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    driver = args.driver.resolve(strict=True)
    runs = [compare(number, driver) for number in (1, 2)]
    report = {
        "status": "pass", "driver_sha256": digest(driver), "runs": runs,
        "scope": "Internal C++ state-variable core replay; no replacement keyword importer or full mesh model.",
        "comparison": "Exact counts, widths, multiplicities, previous-shape fields, storage lengths and values.",
        "design_choices": [
            "Portable vector storage replaces reference-counted legacy backing; pointer identity is not compared.",
            "Invalid IDs, negative extents and signed-int extent overflow are rejected; legacy behavior there is not established."
        ],
        "remaining": [
            "2D mesh allocation and nondefault controls",
            "Post-only explicit allocation, nonunit multiplicity and nine-component material-axis defaults",
            "Shared backing detach/copy behavior and preservation-enabled resize",
            "Derived geometry and complete object/model lifecycle"
        ]
    }
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {sum(r['snapshot_comparisons'] for r in runs)} original descriptor "
          "snapshots exactly reproduced by the C++ state-variable core")


if __name__ == "__main__":
    main()
