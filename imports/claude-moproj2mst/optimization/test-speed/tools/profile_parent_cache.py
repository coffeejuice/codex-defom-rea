"""One read-only full-root phase profile of the four-worker parent-cache path."""
import hashlib
import json
import os
from pathlib import Path
import stat
import time
from collections import defaultdict
import profile_fixture_hash as profiler

engine = profiler.engine
phases = defaultdict(float)
counts = defaultdict(int)


def paths(roots, evidence):
    parents = {}
    for root in sorted(roots):
        t = time.perf_counter()
        direct_file = root.is_file()
        files = [root] if direct_file else list(p for p in root.rglob("*") if p.is_file())
        phases["enumeration_and_is_file"] += time.perf_counter() - t
        t = time.perf_counter()
        files.sort()
        phases["sort"] += time.perf_counter() - t
        for path in files:
            t = time.perf_counter()
            metadata = path.lstat()
            attributes = getattr(metadata, "st_file_attributes", None)
            uncertain = os.name == "nt" and attributes is None
            reparse = bool(attributes is not None and attributes & stat.FILE_ATTRIBUTE_REPARSE_POINT)
            fallback = direct_file or uncertain or reparse or not stat.S_ISREG(metadata.st_mode)
            phases["lstat_and_reparse_classification"] += time.perf_counter() - t
            if fallback:
                t = time.perf_counter()
                resolved = engine._child(path, evidence)
                phases["full_resolution_fallback"] += time.perf_counter() - t
                counts["fallback_files"] += 1
                yield resolved
                continue
            t = time.perf_counter()
            parent = parents.get(path.parent)
            phases["parent_cache_lookup"] += time.perf_counter() - t
            if parent is None:
                t = time.perf_counter()
                parent = path.parent.resolve()
                parents[path.parent] = parent
                phases["parent_resolve_and_cache_insert"] += time.perf_counter() - t
                counts["parent_resolves"] += 1
            t = time.perf_counter()
            resolved = parent / path.name
            if resolved == evidence or not resolved.is_relative_to(evidence):
                raise ValueError(f"Path must remain beneath {evidence}: {resolved}")
            phases["path_join_and_containment"] += time.perf_counter() - t
            yield resolved


if __name__ == "__main__":
    baseline = json.loads((profiler.ROOT / "optimization/test-speed/analysis/hash-real-benchmark.json").read_text())
    roots = {Path(p) for p in baseline["roots"]}
    original_snapshot, original_paths = engine.snapshot, engine._parent_cached_paths
    engine._parent_cached_paths = paths
    engine.snapshot = lambda roots, **kw: original_snapshot(roots, **kw, path_engine="parent-cache")
    try:
        result = profiler.profile(roots, 4)
    finally:
        engine.snapshot, engine._parent_cached_paths = original_snapshot, original_paths
    result["producer_phase_seconds"] = dict(phases)
    result["counts"] = dict(counts)
    result["matches_retained_serial_snapshot"] = result["snapshot_ordered_sha256"] == baseline["snapshot_ordered_sha256"]
    result["helper_sha256"] = hashlib.sha256((profiler.ROOT / "admin/fixture_hash_fast.py").read_bytes()).hexdigest()
    assert result["matches_retained_serial_snapshot"]
    text = json.dumps(result, indent=2)
    print(text)
    (profiler.ROOT / "optimization/test-speed/analysis/hash-parent-cache-phase-profile.md").write_text(
        "# Four-worker parent-cache phase profile\n\nSingle instrumented pass; worker sums overlap producer time and each other. Timers introduce overhead. Same bounded scheduling and full reads as helper. OS caches were not cleared.\n\n```json\n" + text + "\n```\n", encoding="utf-8")
