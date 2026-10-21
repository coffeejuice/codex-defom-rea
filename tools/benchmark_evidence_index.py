"""One bounded pilot: digest-validated address index vs focused direct JSON scan."""
import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import statistics
import subprocess
import sys
import time
import uuid

ROOT = Path(__file__).resolve().parents[1]
HELPER = ROOT / "tools/evidence_index.py"
TARGET = "f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794"
SOURCES = [ROOT / p for p in ("analysis/oracle/import-snapshot.json", "analysis/oracle/fstres-static-evidence.json", "analysis/material-import-0x140384a50.json")]
QUERIES = ["0x1401dc3f0", "0x140384a50", "0xffffffffffffffff"]


def direct(procedure):
    """Independent parser/filter: no index/helper imports or broad body printing."""
    sources, matches = [], []
    for path in sorted(SOURCES, key=str):
        raw = path.read_bytes()
        digest = hashlib.sha256(raw).hexdigest()
        doc = json.loads(raw)
        if "evidence_bundle" in doc:
            rows, prefix = doc["evidence_bundle"]["records"], "/evidence_bundle/records/"
        elif "records" in doc:
            rows, prefix = doc["records"], "/records/"
        else:
            rows, prefix = [doc], None
        count = 0
        for i, row in enumerate(rows):
            parameters = row.get("parameters") or {}
            proc = parameters.get("procedure")
            subject = row.get("subject") or {}
            target = (subject.get("digest") or {}).get("sha256")
            if not (isinstance(proc, str) and proc.lower().startswith("0x") and len(proc) > 2
                    and all(c in "0123456789abcdefABCDEF" for c in proc[2:])
                    and isinstance(target, str) and len(target) == 64
                    and all(c in "0123456789abcdefABCDEF" for c in target)
                    and row.get("evidence_id") and row.get("operation")):
                continue
            proc = hex(int(proc, 16))
            count += 1
            if proc != procedure or target.lower() != TARGET:
                continue
            bodies = []
            normal = row.get("normalized_result")
            value = (row.get("raw_result") or {}).get("value")
            if isinstance(normal, str) and row["operation"] == "procedure_pseudo_code":
                bodies.append("/normalized_result")
            if isinstance(normal, dict) and isinstance(normal.get("pseudocode"), str):
                bodies.append("/normalized_result/pseudocode")
            if isinstance(value, str) and row["operation"] == "procedure_pseudo_code":
                bodies.append("/raw_result/value")
            if isinstance(value, dict) and isinstance(value.get("pseudocode"), str):
                bodies.append("/raw_result/value/pseudocode")
            matches.append({"source_path": str(path), "source_sha256": digest,
                            "record_pointer": "" if prefix is None else prefix + str(i),
                            "evidence_id": row["evidence_id"], "target_sha256": target.lower(),
                            "operation": row["operation"], "procedure": proc, "body_pointers": bodies})
        sources.append({"path": str(path), "bytes": len(raw), "sha256": digest,
                        "records_seen": len(rows), "records_indexed": count, "records_skipped": len(rows) - count})
    return {"scope": sources, "matches": matches}


def invoke(arguments):
    started = time.perf_counter()
    run = subprocess.run([sys.executable, *map(str, arguments)], capture_output=True, check=True)
    elapsed = time.perf_counter() - started
    return json.loads(run.stdout), {"seconds": elapsed, "stdout_bytes": len(run.stdout), "stderr_bytes": len(run.stderr), "exit_code": run.returncode}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--direct")
    parser.add_argument("--output", default=str(ROOT / f"analysis/bottleneck-index-benchmark-{uuid.uuid4().hex}.json"),
                        help="Fresh report path; defaults to a unique filename")
    parser.add_argument("--repeats", type=int, default=5)
    args = parser.parse_args()
    if args.direct:
        sys.stdout.buffer.write((json.dumps(direct(args.direct), ensure_ascii=False, separators=(",", ":")) + "\n").encode("utf-8"))
        return
    if os.path.lexists(args.output):
        parser.error("Benchmark output already exists; choose a fresh path")
    scratch = ROOT / "build/evidence-slice-tests"
    scratch.mkdir(parents=True, exist_ok=True)
    index_path = scratch / f"bottleneck-evidence-index-{uuid.uuid4().hex}.json"
    receipt, setup = invoke([HELPER, "build", *SOURCES, "--output", index_path])
    result = {"schema_version": 1, "scope": receipt["sources"], "setup": setup,
              "index_receipt": receipt, "repeats": args.repeats, "queries": [],
              "measurement": "perf_counter around subprocess.run; includes Python startup, all source reads/hashes, parsing where applicable, filtering and serialization. Alternating order; likely warm OS cache, no cold-cache claim.",
              "baseline": "Independent stdlib direct parse of the same three explicit files, exact address+target filter, source hashing and compact identity/pointer output. No full JSON/body dumping; no helper imports. Baseline omits the helper status/notice wrapper, slightly favoring direct output.",
              "tool_calls": {"observed_exec_calls_to_launch_this_benchmark": 1, "cli_commands_per_direct_query": 1, "cli_commands_per_index_query": 1, "extra_index_setup_commands": 1, "agent_reasoning_seconds": None, "agent_tokens": None, "tool_roundtrip_seconds": None},
              "limitations": ["Discovery metadata only; body extraction, qualification review and agent reasoning excluded.", "Three explicitly named local ignored files; no recursive search or global absence claim.", "Missing optional identity/address records are counted as skipped, not proved absent.", "Full hashes on every query cost O(total source bytes); no persistent service or target execution.", "Index digest must be pinned from the build receipt; hashes bind bytes, not authenticity. Rebuild after source edits; new files require explicit new scope.", "Sequential local descriptive timings; no peak-memory or isolated startup measurement."]}
    for proc in QUERIES:
        runs = {"direct": [], "indexed": []}
        outputs = {}
        for repeat in range(args.repeats):
            for mode in (["direct", "indexed"] if repeat % 2 == 0 else ["indexed", "direct"]):
                command = [Path(__file__).resolve(), "--direct", proc] if mode == "direct" else [HELPER, "query", index_path, "--expect-index-sha256", receipt["index_sha256"], "--procedure", proc, "--target-sha256", TARGET]
                output, sample = invoke(command)
                outputs[mode] = output
                runs[mode].append(sample)
            assert outputs["direct"] == {key: outputs["indexed"][key] for key in ("scope", "matches")}, "Independent fidelity comparison failed"
        medians = {mode: statistics.median(x["seconds"] for x in samples) for mode, samples in runs.items()}
        saving = medians["direct"] - medians["indexed"]
        result["queries"].append({"procedure": proc, "status": outputs["indexed"]["status"],
                                 "matches": outputs["indexed"]["matches"], "independent_fidelity": True,
                                 "samples": runs, "median_seconds": medians, "saved_seconds_per_query": saving,
                                 "direct_over_index_ratio": medians["direct"] / medians["indexed"],
                                 "queries_to_amortize_setup": math.ceil(setup["seconds"] / saving) if saving > 0 else None})
    result["code_sha256"] = {str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest() for p in [HELPER, Path(__file__).resolve(), ROOT / "tests/test_evidence_index.py"]}
    result["source_unchanged_after"] = all(hashlib.sha256(Path(s["path"]).read_bytes()).hexdigest() == s["sha256"] for s in receipt["sources"])
    with Path(args.output).open("x", encoding="utf-8") as stream:
        stream.write(json.dumps(result, ensure_ascii=False, indent=2) + "\n")
    print(json.dumps({"output": args.output, "setup": setup, "queries": [{key: q[key] for key in ["procedure", "median_seconds", "queries_to_amortize_setup", "independent_fidelity"]} for q in result["queries"]], "source_unchanged_after": result["source_unchanged_after"]}))


if __name__ == "__main__":
    main()
