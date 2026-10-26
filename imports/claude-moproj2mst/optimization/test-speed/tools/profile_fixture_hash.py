"""Read-only phase attribution of the existing hash helper; never edits fixtures."""
import hashlib
import json
from pathlib import Path
import sys
import time
from collections import defaultdict

ROOT = Path(__file__).resolve().parents[3]
sys.dont_write_bytecode = True
sys.path.insert(0, str(ROOT / "admin"))
import fixture_hash_fast as engine


def profile(roots, workers):
    phases = defaultdict(float)
    entries = []
    original_paths, original_entry = engine._paths, engine._entry

    def paths(roots, evidence):
        for root in sorted(roots):
            t = time.perf_counter()
            files = [root] if root.is_file() else list(p for p in root.rglob("*") if p.is_file())
            phases["enumeration_and_is_file"] += time.perf_counter() - t
            t = time.perf_counter()
            files.sort()
            phases["path_sort"] += time.perf_counter() - t
            for path in files:
                t = time.perf_counter()
                resolved = engine._child(path, evidence)
                phases["path_resolution_and_containment"] += time.perf_counter() - t
                yield resolved

    def entry(path, evidence):
        spans = {}
        t = time.perf_counter()
        st = path.stat()
        spans["stat"] = time.perf_counter() - t
        h = hashlib.sha256()
        t = time.perf_counter()
        stream = path.open("rb")
        spans["open"] = time.perf_counter() - t
        read, digest, blocks = 0.0, 0.0, 0
        try:
            while True:
                t = time.perf_counter()
                block = stream.read(1024 * 1024)
                read += time.perf_counter() - t
                if not block:
                    break
                blocks += 1
                t = time.perf_counter()
                h.update(block)
                digest += time.perf_counter() - t
        finally:
            t = time.perf_counter()
            stream.close()
            spans["close"] = time.perf_counter() - t
        spans["read"] = read
        spans["sha256_update"] = digest
        t = time.perf_counter()
        value = (str(path.relative_to(evidence)), [st.st_size, st.st_mtime_ns, h.hexdigest()])
        spans["key_and_digest_format"] = time.perf_counter() - t
        entries.append((st.st_size, blocks, spans))
        return value

    engine._paths, engine._entry = paths, entry
    try:
        t = time.perf_counter()
        result = engine.snapshot(roots, evidence=engine.EVIDENCE, workers=workers)
        elapsed = time.perf_counter() - t
    finally:
        engine._paths, engine._entry = original_paths, original_entry
    sums = defaultdict(float)
    for _, _, spans in entries:
        for key, seconds in spans.items():
            sums[key] += seconds
    ordered_digest = hashlib.sha256(json.dumps(list(result.items()), separators=(",", ":")).encode()).hexdigest()
    return {"workers": workers, "wall_seconds": elapsed, "producer_phase_seconds": dict(phases),
            "worker_phase_summed_seconds": dict(sums), "entries": len(entries),
            "bytes": sum(v[0] for v in entries), "read_blocks": sum(v[1] for v in entries),
            "files_under_64KiB": sum(v[0] < 65536 for v in entries),
            "bytes_under_64KiB": sum(v[0] for v in entries if v[0] < 65536),
            "snapshot_ordered_sha256": ordered_digest}


if __name__ == "__main__":
    baseline = json.loads((ROOT / "optimization/test-speed/analysis/hash-real-benchmark.json").read_text())
    roots = {Path(p) for p in baseline["roots"]}
    results = []
    for workers in (1, 2):
        row = profile(roots, workers)
        row["matches_retained_serial_snapshot"] = row["snapshot_ordered_sha256"] == baseline["snapshot_ordered_sha256"]
        assert row["matches_retained_serial_snapshot"]
        results.append(row)
        print(json.dumps(row), flush=True)
    output = ROOT / "optimization/test-speed/analysis/hash-phase-profile.md"
    output.write_text("# Fixture hash phase profile\n\nMeasured read-only using the exact helper schedule and retained four roots. All ordered mappings match the retained serial snapshot digest. Timers add instrumentation overhead; worker phase sums overlap each other and producer time under two workers, so they must not be added to explain wall time. Enumeration includes directory traversal and is_file checks; ordering measures path sorting only. Future waits/result insertion remain in unassigned wall time. OS cache was not cleared.\n\n```json\n" + json.dumps(results, indent=2) + "\n```\n", encoding="utf-8")
