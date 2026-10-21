"""Replay retained original 2D boundary inputs against the C++ component."""
import argparse
import json
from pathlib import Path
import subprocess

from capture_oracle import digest

ROOT = Path(__file__).resolve().parents[1]
OPS = ["import-before", "planar-before", "planar-after", "object-planar", "import-after"]


def require(condition, message):
    if not condition:
        raise ValueError(message)


def compare(path, driver):
    evidence = json.loads(path.read_text(encoding="utf-8-sig"))
    capture = evidence["normalized_result"]
    require(capture["messages_truncated"] is False and capture["cleanup_error"] is None,
            "incomplete capture")
    require(all(m["type"] == "send" for m in capture["messages"]), "runtime script error")
    payloads = [m["payload"] for m in capture["messages"]]
    require(payloads[0]["operation"] == "hooks-installed" and
            payloads[0]["pid"] == capture["pid"], "hook identity mismatch")
    events = payloads[1:]
    require([e["sequence"] for e in events] == list(range(1, len(events) + 1))
            and len(events) % 5 == 0, "incomplete event sequence")
    rows = []
    for i in range(0, len(events), 5):
        group = events[i:i + 5]
        require([e["operation"] for e in group] == OPS and
                len({e["file"] for e in group}) == 1, "event grouping mismatch")
        _, before, after, consumer, end = group
        require(end["status"] == 0 and end["dimension"] == consumer["dimension"] == 2,
                "import or dimension mismatch")
        require(consumer["arity"] == 4 and
                len(before["connectivity"]) == before["elements"] * 4, "input shape mismatch")
        result = subprocess.run(
            [str(driver)],
            input=str(before["elements"]) + "\n" +
            " ".join(map(str, before["connectivity"])) + "\n",
            text=True, capture_output=True, check=True, timeout=10)
        actual = json.loads(result.stdout)
        expected = {k: after[k] for k in ("ranges", "nodes")}
        require(actual == expected, f"{path.name}: {before['file']} differs: {actual} != {expected}")
        require(after["count"] == len(after["ranges"]), "output group count mismatch")
        groups = [after["nodes"][a - 1:b] for a, b in after["ranges"][:100]]
        require(consumer["groups"] == groups and
                consumer["total"] == sum(len(g) - 1 for g in groups), "object consumer mismatch")
        rows.append({"file": before["file"], "elements": before["elements"],
                     "groups": after["count"], "stream_length": len(after["nodes"]),
                     "boundary_edges": consumer["total"], "verdict": "exact"})
    return {"capture": path.relative_to(ROOT).as_posix(), "capture_sha256": digest(path),
            "evidence_id": evidence["evidence_id"], "pid": capture["pid"], "cases": rows}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--driver", type=Path,
                        default=ROOT / "build/Release/planar_boundary_replay.exe")
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    driver = args.driver.resolve(strict=True)
    paths = sorted((ROOT / "analysis").glob("planar-runtime-*.json"))
    require(bool(paths), "no retained planar captures")
    runs = [compare(path, driver) for path in paths]
    freeze_path = ROOT / "analysis/planar-candidate-freeze.json"
    freeze = json.loads(freeze_path.read_text())
    unchanged = all(digest(ROOT / item["path"]) == item["sha256"]
                    for item in freeze["files"] if not item["path"].startswith("build/"))
    report = {
        "status": "pass", "candidate_source_unchanged_since_freeze": unchanged,
        "candidate_freeze_sha256": digest(freeze_path),
        "driver_sha256": digest(driver), "runs": runs,
        "comparison": "Exact ranges, full node stream including separators and terminal -1, ordered object groups and total edge count.",
        "limitations": [
            "Positive node IDs and bounded input sizes only.",
            "No allocation-failure, malformed-ID or object-group truncation evaluation.",
            "Internal component, not a complete keyword importer or original application."
        ]
    }
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {sum(len(r['cases']) for r in runs)} exact planar boundary and consumer outputs")


if __name__ == "__main__":
    main()
