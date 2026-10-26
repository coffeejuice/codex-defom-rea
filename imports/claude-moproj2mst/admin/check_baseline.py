"""Run bounded baseline checks on disposable copies; retain all outputs."""
import csv
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/baseline"
CORPUS = ROOT / "evidence/gui_mo/s1/corpus"


def main():
    started = time.time()
    scratch = ROOT / "build" / f"baseline-check-{time.time_ns()}"
    scratch.mkdir(parents=True, exist_ok=False)
    temp = scratch / "temp"
    temp.mkdir()
    env = dict(os.environ, TEMP=str(temp), TMP=str(temp))
    runner = ROOT / "source/docs/re/moproj2mst/runner/run_corpus.py"
    tiers = ROOT / "source/docs/re/moproj2mst/runner/tier2.txt"
    cases = [line.split()[0] for line in tiers.read_text().splitlines() if line and line[0].isdigit()]
    with (CORPUS / "cases.tsv").open(encoding="utf-8") as f:
        records = {row["case_id"]: row for row in csv.DictReader(f, delimiter="\t")}
    for case in cases:
        work = (scratch / case).resolve()
        assert work.is_relative_to(scratch.resolve()) and work != scratch.resolve()
        source = (CORPUS / case).resolve()
        assert source.is_relative_to(CORPUS.resolve())
        for field in ("moproj", "mst"):
            if records[case].get(field):
                assert (source / records[case][field]).resolve().is_relative_to(source)
    out_tsv = ROOT / "analysis/baseline-tier2.tsv"
    commands = [
        ("release-smoke", ["ctest", "--test-dir", str(BUILD), "--output-on-failure",
                           "-R", r"^m2m\.(release\.|g0\.cli_missing_project$)"]),
        ("tier2", ["python", "-I", str(runner), "--cli", str(BUILD / "bin/moproj2mst.exe"),
                   "--corpus", str(CORPUS), "--scratch", str(scratch), "--keep",
                   "--tsv", str(out_tsv), "--cases", *cases]),
    ]
    receipt = {"started_unix": started, "scratch": str(scratch), "cases": cases,
               "scope": "Imported release smoke and historical tier2 MST-only cases; not full Save or fresh original captures",
               "runner_sha256": hashlib.sha256(runner.read_bytes()).hexdigest(),
               "qualifications": ["Inherited comparator waivers retained and reported per case",
                                  "Scratch project before/after hashes checked by inherited runner",
                                  "No original process executed; no behavioral source changes"], "steps": []}
    for name, command in commands:
        log_path = ROOT / "build/logs" / f"baseline-{name}.log"
        step_start = time.monotonic()
        with log_path.open("w", encoding="utf-8") as log:
            result = subprocess.run(command, env=env, cwd=ROOT, stdout=log, stderr=subprocess.STDOUT)
        step = {"name": name, "exit_code": result.returncode,
                "elapsed_seconds": round(time.monotonic() - step_start, 3), "log": str(log_path)}
        receipt["steps"].append(step)
        print(json.dumps(step), flush=True)
    receipt["elapsed_seconds"] = round(time.time() - started, 3)
    if out_tsv.exists():
        with out_tsv.open(encoding="utf-8") as f:
            receipt["results"] = list(csv.DictReader(f, delimiter="\t"))
        receipt["raw_comparisons"] = []
        for case in cases:
            expected = CORPUS / case / records[case].get("mst", "")
            actual = scratch / case / "out" / (case + ".MST")
            comparison = {"case": case, "available": expected.is_file() and actual.is_file(),
                          "declared_waiver": records[case].get("waiver", "")}
            if comparison["available"]:
                comparison["expected_sha256"] = hashlib.sha256(expected.read_bytes()).hexdigest()
                comparison["actual_sha256"] = hashlib.sha256(actual.read_bytes()).hexdigest()
                comparison["raw_equal"] = comparison["expected_sha256"] == comparison["actual_sha256"]
            receipt["raw_comparisons"].append(comparison)
    (ROOT / "analysis/baseline-checks.json").write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    return int(any(step["exit_code"] for step in receipt["steps"]))


if __name__ == "__main__":
    raise SystemExit(main())
