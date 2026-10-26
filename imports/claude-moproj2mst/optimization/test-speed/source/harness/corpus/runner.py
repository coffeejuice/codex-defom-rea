"""Run corpus cases: original DEF_PRE (through the oracle cache) against the port, compared by defcmp.

    python -I harness/corpus/runner.py --port build/def_pre.exe --defcmp build/defcmp.exe CASE_ID...
    python -I harness/corpus/runner.py --port ... --defcmp ... --tier 2        # smoke set
    python -I harness/corpus/runner.py --port ... --defcmp ... --tier 3 -j 12  # whole corpus

The port runs in a fresh scratch directory set up exactly as the oracle sets up the original's
(same inputs, argv and stdin, TEMP inside the scratch dir), and its outputs are collected into the
same layout as an oracle cache entry, so `defcmp oracle ENTRY ACTUAL` compares the two. Port runs
live in --runs (default $DEFORM_PORT_RUNS, else ~/.deform-port-runs); a passing run is deleted,
a failing one is kept for inspection. Large DB inputs are hard-linked from the oracle's staged
read-only copies, never copied again.

Exit code: 0 if every case passed, 1 otherwise. Prints a pass/fail table; --report writes one TSV
row per case.
"""
from __future__ import annotations

import argparse
import hashlib
import concurrent.futures
import json
import os
import subprocess
import sys
import time
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cases import TestCase, by_id, tier_ids  # noqa: E402
from def_pre_oracle import Oracle, OracleError, _rmtree  # noqa: E402


class PortRunner(Oracle):
    """The oracle's run machinery pointed at the port executable, staging via the oracle."""

    def __init__(self, exe, runs_dir, oracle: Oracle):
        super().__init__(cache_dir=runs_dir, exe=exe, offline=False)
        self.oracle = oracle

    def key(self, case, _hashes=None):
        # One entry per case id: corpus files with identical content (the many PREVIEW_DATA.KWD
        # copies) are the same oracle case, but their port runs must not replace each other.
        return hashlib.sha256(f"{case.label}|{super().key(case, _hashes)}".encode()).hexdigest()

    def file_hash(self, path):
        return self.oracle.file_hash(path)  # one hash memo for both

    def _stage(self, src, digest):
        return self.oracle._stage(src, digest)  # share the read-only staged DBs


def summarize(out: str, err: str) -> str:
    """'differs: a, b | <first report line>' from `defcmp oracle` output."""
    lines = out.splitlines()
    names, first = [], ""
    for i, ln in enumerate(lines):
        if ln.startswith(("DIFF ", "ERR")):
            names.append(ln.split()[-1])
            if not first and i + 1 < len(lines):
                first = lines[i + 1].strip()
    if not names:
        return (err.strip() or out.strip())[:300]
    return f"differs: {', '.join(names)} | {first}"[:300]


# Clock offsets tried around the original's start second: the original seeds rand() with time()
# in some routines (14045c530, CNVT3D caps), so the port runs with DEF_PORT_FIXED_TIME pinned to
# that second and, when the comparison fails, to the neighbouring seconds.
TIME_OFFSETS = (0, -1, 1, -2, 2)


def run_one(tc: TestCase, oracle: Oracle, port: PortRunner, defcmp: str,
            keep_pass: bool = False, time_search: str = "cmd") -> dict:
    row = {"id": tc.id, "kind": tc.kind, "set": tc.set, "status": "ERROR", "detail": ""}
    t0 = time.time()
    try:
        case = tc.oracle_case()
        expected = oracle.run(case)
        row["oracle_cached"] = expected.cached
        start = expected.start_time
        search = time_search == "all" or (time_search == "cmd" and tc.kind == "cmd")
        for off in (TIME_OFFSETS if search else (0,)):
            case.env = {"DEF_PORT_FIXED_TIME": str(start + off)}
            actual = port.run(case, refresh=True)
            p = subprocess.run([defcmp, "oracle", str(expected.dir), str(actual.dir)],
                               capture_output=True, text=True, errors="replace")
            if p.returncode == 0:
                if off != 0:
                    row["time_offset"] = off
                break
        row["status"] = {0: "PASS", 1: "FAIL"}.get(p.returncode, "ERROR")
        if row["status"] != "PASS":
            row["detail"] = summarize(p.stdout, p.stderr)
        row["port_dir"] = str(actual.dir)
        if row["status"] == "PASS" and not keep_pass:
            _rmtree(actual.dir)
    except (OracleError, OSError) as e:
        row["detail"] = str(e)[:300]
    row["seconds"] = round(time.time() - t0, 2)
    return row


def table(rows: list[dict]) -> str:
    by_kind = defaultdict(Counter)
    for r in rows:
        by_kind[f"{r['kind']} ({r['set']})"][r["status"]] += 1
    out = [f"{'cases':<22}{'PASS':>7}{'FAIL':>7}{'ERROR':>7}{'total':>7}"]
    tot = Counter()
    for k in sorted(by_kind):
        c = by_kind[k]
        tot.update(c)
        out.append(f"{k:<22}{c['PASS']:>7}{c['FAIL']:>7}{c['ERROR']:>7}{sum(c.values()):>7}")
    out.append(f"{'all':<22}{tot['PASS']:>7}{tot['FAIL']:>7}{tot['ERROR']:>7}{sum(tot.values()):>7}")
    return "\n".join(out)


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--port", required=True, help="the port's def_pre executable")
    ap.add_argument("--defcmp", required=True, help="defcmp executable")
    ap.add_argument("--time-search", choices=("off", "cmd", "all"), default="cmd",
                    help="retry a failing case with the port clock at the original's start "
                         "second -1/+1/-2/+2 (default: command-file cases only)")
    ap.add_argument("--tier", type=int, choices=(2, 3))
    ap.add_argument("--primary", action="store_true", help="SI files only (tier 3)")
    ap.add_argument("--kind", action="append", help="only cases of this kind (m, dbkey, kfrw, cmd)")
    ap.add_argument("--runs", default=os.environ.get("DEFORM_PORT_RUNS")
                    or str(Path.home() / ".deform-port-runs"))
    ap.add_argument("--offline", action="store_true", help="never run the original")
    ap.add_argument("--keep-pass", action="store_true", help="keep passing port runs too")
    ap.add_argument("-j", "--jobs", type=int, default=max(1, (os.cpu_count() or 4) - 2))
    ap.add_argument("--report", help="write one TSV row per case")
    ap.add_argument("-q", "--quiet", action="store_true", help="table only, no per-case lines")
    ap.add_argument("ids", nargs="*")
    a = ap.parse_args(argv)

    cases = by_id()
    ids = list(a.ids) or (tier_ids(a.tier, not a.primary) if a.tier else [])
    if not ids:
        ap.error("give case ids or --tier")
    unknown = [i for i in ids if i not in cases]
    if unknown:
        ap.error(f"unknown case ids: {unknown[:5]}")
    sel = [cases[i] for i in ids if not a.kind or cases[i].kind in a.kind]

    oracle = Oracle(offline=a.offline or None)
    port = PortRunner(Path(a.port).resolve(), a.runs, oracle)
    a.defcmp = str(Path(a.defcmp).resolve())
    rows = []
    with concurrent.futures.ThreadPoolExecutor(a.jobs) as ex:
        futs = [ex.submit(run_one, tc, oracle, port, a.defcmp, a.keep_pass, a.time_search)
                for tc in sel]
        for f in concurrent.futures.as_completed(futs):
            r = f.result()
            rows.append(r)
            if not a.quiet or len(sel) == 1:
                print(f"{r['status']:<5} {r['id']}  {r['detail']}", flush=True)
    rows.sort(key=lambda r: r["id"])
    if a.report:
        cols = ["id", "kind", "set", "status", "seconds", "oracle_cached", "detail", "port_dir"]
        with open(a.report, "w", encoding="utf-8") as f:
            f.write("\t".join(cols) + "\n")
            for r in rows:
                f.write("\t".join(str(r.get(c, "")).replace("\t", " ") for c in cols) + "\n")
    if len(rows) > 1:
        print(table(rows))
    return 0 if all(r["status"] == "PASS" for r in rows) else 1


if __name__ == "__main__":
    sys.exit(main())
