"""End-to-end def_pre check of command cases: run the port's def_pre.exe on a tests/commands case
exactly as the oracle ran the original (same inputs, stdin, argv; stdout redirected to a file) and
compare the WHOLE stdout byte for byte with the cached oracle run, and every file the run
created or changed (names and bytes).

    python -I tools/int/e2e_stdout.py [--exe port/build/def_pre.exe] [--ignore fort.0] LABEL...
    python -I tools/int/e2e_stdout.py --all [-j 8] [--ignore fort.0] [--report out.tsv] [FILTER...]

--all runs every case of tests/commands/*/cases*.json and every single-case tests/commands/<CMD>/
<stem>.json (label <CMD>/<stem>) that has a cached oracle run (FILTER:
substrings of the label). Exit 0 when every case matches. Port runs go to %TEMP%/p67e2e/<label>.
For a difference the first differing file and line (1-based) are reported. Before the stdout
comparison each run's own work directory (<cache>/tmp/<key12>-<random>, printed by child
programs such as DEF_MORP2) is replaced by <RUNDIR> on both sides.
"""
import concurrent.futures
import glob
import json
import re
import shutil
import sys
import tempfile
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "oracle"))
from def_pre_oracle import Case, Oracle, OracleError  # noqa: E402


def all_cases():
    out = {}
    for f in sorted(glob.glob(str(ROOT / "tests" / "commands" / "*" / "cases*.json"))):
        for d in json.load(open(f, encoding="utf-8")):
            if "label" in d and d["label"] not in out:
                out[d["label"]] = (d, Path(f).parent)
    # single-case files (tests/commands/<CMD>/<stem>.json, e.g. GFREAD, MORP): label <CMD>/<stem>
    for f in sorted(glob.glob(str(ROOT / "tests" / "commands" / "*" / "*.json"))):
        p = Path(f)
        if p.name.startswith("cases"):
            continue
        try:
            d = json.load(open(f, encoding="utf-8"))
        except (OSError, ValueError):
            continue
        if isinstance(d, dict) and "label" in d and "inputs" in d:
            out.setdefault(f"{p.parent.name}/{p.stem}", (d, p.parent))
    return out


RUNDIR = re.compile(rb'[A-Za-z]:\\[^\r\n?*"<>|]*?\\tmp\\[0-9a-f]{12}-[a-z0-9_]{8}')


def norm_rundir(b: bytes) -> bytes:
    return RUNDIR.sub(b"<RUNDIR>", b)


def first_diff(a: bytes, b: bytes):
    n = min(len(a), len(b))
    i = next((k for k in range(n) if a[k] != b[k]), n)
    ln = a[:i].count(b"\n") + 1

    def line(x):
        s = x.rfind(b"\n", 0, i) + 1
        e = x.find(b"\n", i)
        return x[s:e if e >= 0 else len(x)][:160]
    return ln, line(a), line(b)


def files(r):
    d = r.dir / "files"
    if not d.exists():
        return {}
    return {f.relative_to(d).as_posix(): f.read_bytes() for f in d.rglob("*") if f.is_file()}


def check(label, d, base, exe, ignore, runs):
    case = Case.from_json(d, base=base)
    try:
        orig = Oracle(offline=True).run(case)
    except OracleError:
        return label, "SKIP", "no oracle entry"
    cache = runs / label.replace("/", "_")
    shutil.rmtree(cache, ignore_errors=True)
    try:
        port = Oracle(cache_dir=str(cache), exe=str(exe), offline=False).run(case, refresh=True)
    except OracleError as e:
        return label, "FAIL", f"port run: {e}"
    so, sp = norm_rundir(orig.stdout), norm_rundir(port.stdout)
    if so != sp:
        ln, lo, lp = first_diff(so, sp)
        return label, "FAIL", f"stdout line {ln}: orig {lo!r} | port {lp!r}"
    fo = {k: v for k, v in files(orig).items() if k not in ignore}
    fp = {k: v for k, v in files(port).items() if k not in ignore}
    for n in sorted(set(fo) | set(fp)):
        if n not in fp:
            return label, "FAIL", f"{n}: only in original"
        if n not in fo:
            return label, "FAIL", f"{n}: only in port"
        if fo[n] != fp[n]:
            ln, lo, lp = first_diff(fo[n], fp[n])
            return label, "FAIL", f"{n} line {ln}: orig {lo!r} | port {lp!r}"
    return label, "PASS", f"stdout and {len(fo)} file(s) equal"


def main():
    args = sys.argv[1:]
    exe, ignore, jobs, report, run_all = ROOT / "port" / "build" / "def_pre.exe", set(), 8, None, False
    rest = []
    while args:
        a = args.pop(0)
        if a == "--exe":
            exe = Path(args.pop(0))
        elif a == "--ignore":
            ignore.add(args.pop(0))
        elif a == "-j":
            jobs = int(args.pop(0))
        elif a == "--report":
            report = args.pop(0)
        elif a == "--all":
            run_all = True
        else:
            rest.append(a)
    cases = all_cases()
    labels = [l for l in cases if not rest or any(f in l for f in rest)] if run_all else rest
    runs = Path(tempfile.gettempdir()) / "p67e2e"
    rows = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
        futs = [ex.submit(check, l, *cases[l], exe, ignore, runs) for l in labels]
        for f in concurrent.futures.as_completed(futs):
            r = f.result()
            rows.append(r)
            print(f"{r[0]:40} {r[1]:4}  {r[2]}", flush=True)
    per = defaultdict(lambda: defaultdict(int))
    for l, s, _ in rows:
        per[l.split("/")[0]][s] += 1
    print("\nper command:")
    for c in sorted(per):
        print(f"  {c:10} " + "  ".join(f"{k} {v}" for k, v in sorted(per[c].items())))
    if report:
        with open(report, "w", encoding="utf-8") as fh:
            fh.write("label\tstatus\tdetail\n")
            for r in sorted(rows):
                fh.write("\t".join(r) + "\n")
    return 1 if any(s == "FAIL" for _, s, _ in rows) else 0


if __name__ == "__main__":
    sys.exit(main())
