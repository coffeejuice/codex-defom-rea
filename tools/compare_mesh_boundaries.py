"""Compare reconstructed boundary/sort operations with retained original outputs.

Never launches the original. Provenance checks for each capture are separate;
comparisons preserve face orientation, ordering and owning-element identity.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import subprocess

from capture_oracle import digest

ROOT = Path(__file__).resolve().parents[1]


def require(condition: bool, message: str) -> None:
    if not condition:
        raise ValueError(message)


def replay(driver: Path, mode: str, count: int, values: list[int]) -> list[list[int]]:
    result = subprocess.run([str(driver)],
                            input=f"{mode} {count}\n" + " ".join(map(str, values)) + "\n",
                            text=True, capture_output=True, timeout=10, check=True)
    return [[int(x) for x in line.split()] for line in result.stdout.splitlines()]


def compare(path: Path, driver: Path, include_hexa: bool = False) -> dict:
    evidence = json.loads(path.read_text(encoding="utf-8-sig"))
    capture = evidence["normalized_result"]
    require(capture["messages_truncated"] is False and capture["cleanup_error"] is None,
            "incomplete capture")
    require(all(m["type"] == "send" for m in capture["messages"]), "script error")
    payloads = [m["payload"] for m in capture["messages"]]
    require(payloads[0]["operation"] == "hooks-installed" and
            payloads[0]["pid"] == capture["pid"], "hook identity mismatch")
    events = payloads[1:]
    require([e["sequence"] for e in events] == list(range(1, len(events) + 1)),
            "incomplete event sequence")
    rows, sort_rows, discovery = [], [], []
    boundary_input = sort_input = boundary_output = None
    for event in events:
        op = event["operation"]
        require(op != "probe-error", "probe error")
        if op == "boundary-before":
            require(boundary_input is None, "nested/missing boundary completion")
            boundary_input = event
        elif op == "sort-before":
            require(sort_input is None, "nested/missing sort completion")
            sort_input = event
        elif op == "sort-after":
            require(sort_input is not None and sort_input["count"] == event["count"],
                    "sort pairing mismatch")
            require(len(sort_input["records"]) == len(event["records"]) == event["count"] * 2,
                    "sort capture size mismatch")
            actual = replay(driver, "sort", sort_input["count"], sort_input["records"])
            expected = [event["records"][i:i + 2] for i in range(0, len(event["records"]), 2)]
            require(actual == expected, f"{path.name}: {event['file']} sort permutation differs")
            sort_rows.append({"file": event["file"], "records": event["count"], "verdict": "exact"})
            sort_input = None
        elif op == "boundary-after":
            require(boundary_input is not None, "boundary output without input")
            require(len(event["faces"]) == event["count"] * 4 and
                    len(event["owner_elements"]) == event["count"], "boundary size mismatch")
            arity = boundary_input["arity"]
            if arity != 4 and not (include_hexa and arity == 8):
                discovery.append({"file": event["file"], "arity": boundary_input["arity"],
                                  "status": "not_implemented", "observed_faces": event["count"]})
            else:
                actual = replay(driver, "tetra" if arity == 4 else "hexa", boundary_input["elements"],
                                boundary_input["connectivity"])
                expected = [event["faces"][i * 4:(i + 1) * 4] +
                            [event["owner_elements"][i]] for i in range(event["count"])]
                require(actual == expected,
                        f"{path.name}: {event['file']} boundary differs: {actual} != {expected}")
                rows.append({"file": event["file"], "elements": boundary_input["elements"],
                             "faces": event["count"], "verdict": "exact"})
            boundary_output = event
            boundary_input = None
        elif op == "object-boundary":
            require(boundary_output is not None and event["total"] == boundary_output["count"],
                    "object boundary count mismatch")
            require(len(event["boundaries"]) == 1 and event["boundaries"][0] ==
                    {k: boundary_output[k] for k in ("count", "faces", "owner_elements")},
                    "object boundary arrays differ")
            boundary_output = None
        elif op == "import-after":
            require(event["status"] == 0, "unexpected import failure")
    require(boundary_input is None and sort_input is None and boundary_output is None,
            "incomplete operation")
    return {"capture": path.relative_to(ROOT).as_posix(), "capture_sha256": digest(path),
            "evidence_id": evidence["evidence_id"], "pid": capture["pid"],
            "boundaries": rows, "sorts": sort_rows, "discovery": discovery}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--driver", type=Path,
                        default=ROOT / "build/Release/mesh_boundary_replay.exe")
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    driver = args.driver.resolve(strict=True)
    paths = [ROOT / "analysis/mesh-boundary-runtime-1.json"]
    paths += sorted((ROOT / "analysis").glob("mesh-boundary-sort-runtime-*.json"))
    runs = [compare(path, driver) for path in paths]
    freeze_path = ROOT / "analysis/boundary-candidate-freeze.json"
    freeze = json.loads(freeze_path.read_text())
    unchanged = all(digest(ROOT / row["path"]) == row["sha256"]
                    for row in freeze["files"] if not row["path"].startswith("build/"))
    report = {
        "status": "pass", "candidate_source_unchanged_since_freeze": unchanged,
        "candidate_freeze_sha256": digest(freeze_path),
        "driver_sha256": digest(driver), "runs": runs,
        "scope": "Four-node 3D boundary construction and embedded sorting; other arities remain explicit discovery gaps.",
        "comparison": "Exact integer sequence, face orientation, repeated fourth node and owner-element identity.",
        "remaining": ["2D boundary construction", "Eight-/ten-node elements",
                      "Downstream geometry finalization", "Invalid IDs, overflow and allocation failures"]
    }
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {sum(len(r['boundaries']) for r in runs)} boundary outputs and "
          f"{sum(len(r['sorts']) for r in runs)} sort permutations match exactly")


if __name__ == "__main__":
    main()
