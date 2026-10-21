"""Compare eight-slot boundary outputs with saved original-process captures."""
import argparse
import json
from pathlib import Path

from capture_oracle import digest
from compare_mesh_boundaries import ROOT, compare


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--driver", type=Path,
                        default=ROOT / "build/Release/mesh_boundary_replay.exe")
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    driver = args.driver.resolve(strict=True)
    paths = sorted((ROOT / "analysis").glob("hexa-runtime-*.json"))
    if not paths:
        raise ValueError("No original eight-slot captures")
    runs = [compare(path, driver, include_hexa=True) for path in paths]
    freeze_path = ROOT / "analysis/hexa-candidate-freeze.json"
    freeze = json.loads(freeze_path.read_text())
    unchanged = all(digest(ROOT / row["path"]) == row["sha256"]
                    for row in freeze["files"] if not row["path"].startswith("build/"))
    report = {
        "status": "pass", "candidate_source_unchanged_since_freeze": unchanged,
        "candidate_freeze_sha256": digest(freeze_path),
        "driver_sha256": digest(driver), "runs": runs,
        "comparison": "Exact integer sequence: oriented face nodes, ordering, owner IDs, and key/index sort permutation.",
        "limitations": [
            "Finite synthetic fixtures; no arbitrary-input equivalence claim.",
            "Negative IDs, signed overflow, allocation failures and native invalid-memory faults remain outside the implemented compatibility domain.",
            "This is an internal boundary component; importer integration and other element widths remain outstanding."
        ]
    }
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {sum(len(r['boundaries']) for r in runs)} eight-slot boundaries and "
          f"{sum(len(r['sorts']) for r in runs)} sort permutations match exactly")


if __name__ == "__main__":
    main()
