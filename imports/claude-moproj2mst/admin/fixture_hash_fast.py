"""Bounded SHA-256 fixture snapshots compatible with run_converter_tests.snapshot.

Integration: snapshot(roots, evidence=EVIDENCE, workers=4). No persistent cache is
used: every occurrence of every fixture file is read on every call.
"""
from collections import deque
from concurrent.futures import ThreadPoolExecutor
import hashlib
import os
import stat
from pathlib import Path


EVIDENCE = Path(__file__).resolve().parents[1] / "evidence"


def _child(value, parent):
    # parent is canonicalized once by snapshot; every file still resolves here.
    path = Path(value).resolve()
    if path == parent or not path.is_relative_to(parent):
        raise ValueError(f"Path must remain beneath {parent}: {path}")
    return path


def _paths(roots, evidence):
    # Keep the legacy enumeration, sort order, symlink resolution and duplicate
    # occurrences. In particular, do not prune nested roots or follow dir links.
    for root in sorted(roots):
        files = [root] if root.is_file() else sorted(p for p in root.rglob("*") if p.is_file())
        for path in files:
            yield _child(path, evidence)


def _entry(path, evidence):
    st = path.stat()
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return str(path.relative_to(evidence)), [st.st_size, st.st_mtime_ns, h.hexdigest()]


def _parent_cached_paths(roots, evidence):
    # Snapshot-local canonical directory cache; never a content/hash cache.
    parents = {}
    for root in sorted(roots):
        direct_file = root.is_file()
        files = [root] if direct_file else sorted(p for p in root.rglob("*") if p.is_file())
        for path in files:
            metadata = path.lstat()
            attributes = getattr(metadata, "st_file_attributes", None)
            uncertain = os.name == "nt" and attributes is None
            reparse = bool(attributes is not None and attributes & stat.FILE_ATTRIBUTE_REPARSE_POINT)
            if direct_file or uncertain or reparse or not stat.S_ISREG(metadata.st_mode):
                yield _child(path, evidence)
                continue
            parent = parents.get(path.parent)
            if parent is None:
                parent = path.parent.resolve()
                parents[path.parent] = parent
            resolved = parent / path.name
            if resolved == evidence or not resolved.is_relative_to(evidence):
                raise ValueError(f"Path must remain beneath {evidence}: {resolved}")
            yield resolved


def snapshot(roots, *, evidence=EVIDENCE, workers=4, path_engine="exact"):
    """Return relative-path -> [size, mtime_ns, SHA256], in legacy order.

    Roots must be Path objects, as in the original runner. ``evidence`` should
    be absolute (the runner's EVIDENCE is absolute). Concurrency is explicitly
    bounded to 1..32 threads, with at most twice that many pending futures.
    Exceptions abort the call; a partial snapshot is never returned. Running
    reads finish before an exception escapes. Files must remain quiescent during
    capture, the same requirement as the serial snapshot.
    ``parent-cache`` additionally requires stable directory/reparse topology;
    it caches canonical parents for this call and fully resolves reparse files.
    """
    if isinstance(workers, bool) or not isinstance(workers, int) or not 1 <= workers <= 32:
        raise ValueError("workers must be an integer from 1 through 32")
    if path_engine not in ("exact", "parent-cache"):
        raise ValueError("path_engine must be exact or parent-cache")
    evidence = Path(evidence)
    enumerate_paths = _paths if path_engine == "exact" else _parent_cached_paths
    paths = iter(enumerate_paths(roots, evidence.resolve()))
    result = {}
    if workers == 1:
        for path in paths:
            key, value = _entry(path, evidence)
            result[key] = value
        return result
    pending = deque()
    with ThreadPoolExecutor(max_workers=workers, thread_name_prefix="fixture-sha") as pool:
        try:
            for _ in range(workers * 2):
                path = next(paths, None)
                if path is None:
                    break
                pending.append(pool.submit(_entry, path, evidence))
            while pending:
                key, value = pending.popleft().result()
                result[key] = value
                path = next(paths, None)
                if path is not None:
                    pending.append(pool.submit(_entry, path, evidence))
        finally:
            for future in pending:
                future.cancel()
    return result
