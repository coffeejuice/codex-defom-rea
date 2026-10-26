"""`-m` utility path cases: original DEF_PRE (oracle cache) against the port, compared by defcmp.

    python -I harness/mcli/mcli_cases.py --list
    python -I harness/mcli/mcli_cases.py --oracle-only            # fill the oracle cache
    python -I harness/mcli/mcli_cases.py --port port/build/def_pre.exe --defcmp port/build/defcmp.exe

Covers every branch of the `-m` path of DEF_PRE_64 (FUN_14001b7b0 argument parser, FUN_14001c500
dispatcher, the four modes FUN_14001bc80 / FUN_14001be50 / FUN_14001bf40 / FUN_14001c100 and the
help screen FUN_14001b4e0): all modes on the corpus DBs, the hidden `-t 1|2` step lists of
simStepList, attached option values, quote stripping, missing -o / -d, unhandled modes, invalid and
unreadable databases, and an output file that cannot be opened.

Synthetic DBs are made from copies of the corpus DBs with deformdb's `dbmanager purge` (fixed
`--date`, so they are byte-identical on every run) into --work. They only test code paths that
were already recovered from the binary.
"""
from __future__ import annotations

import argparse
import concurrent.futures
import os
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO / "tools" / "oracle"))
sys.path.insert(0, str(REPO / "harness" / "corpus"))
from def_pre_oracle import Case, Input, Oracle, OracleError, _rmtree  # noqa: E402
from runner import PortRunner, summarize  # noqa: E402

CORPUS = Path(os.environ.get("DEFORM_CORPUS", r"C:\Users\alext\Projects\DEFORM_corpus"))
COG = CORPUS / "db" / "COG_P1_B1.DB"
INC = CORPUS / "db" / "Inc718_D420_GFM_V_DIES_Angle2_18.DB"
MODES = ("header", "stepList", "lastStepNo", "simStepList")

# Synthetic DBs: name -> purge step list applied to COG (see the directory in dbinfo).
SYNTH = {
    # op 7 keeps only -25: simStepList's "last step" of op 7 is carried over from op 2.
    "SYN_CARRY.DB": ["-1", "1", "-2", "2", "-25"],
    # ops 2 and 4..6 have no stored step; op numbers stay 1, 3, 7.
    "SYN_GAPS.DB": ["-1", "1", "-3", "3", "25"],
    # one step only.
    "SYN_ONE.DB": ["-4"],
}


def make_synth(work: Path, dbmanager: Path) -> dict[str, Path]:
    work.mkdir(parents=True, exist_ok=True)
    out = {}
    for name, steps in SYNTH.items():
        dst = work / name
        if not dst.exists():
            subprocess.run([str(dbmanager), "purge", str(COG), str(dst), *steps,
                            "--date", "01/02/03"], check=True, capture_output=True)
        out[name] = dst
    return out


def raw_db(sig: bytes, rec2: bytes = b"", extra: bytes = b"") -> bytes:
    """Record 1 = signature padded to 512 bytes, then record 2 (padded), then `extra`."""
    r1 = sig.ljust(512, b" ")
    r2 = rec2.ljust(512, b"\0") if rec2 else b""
    return r1 + r2 + extra


def cases(synth: dict[str, Path]) -> list[tuple[str, Case]]:
    out: list[tuple[str, Case]] = []

    def add(cid, args, inputs=(), stdin=b""):
        out.append((cid, Case(inputs=list(inputs), args=list(args), stdin=stdin, label=f"mcli/{cid}")))

    cog = Input("COG_P1_B1.DB", source=str(COG), link=True)
    inc = Input(INC.name, source=str(INC), link=True)
    dbs = [("cog", cog), ("inc", inc)] + [
        (n.split(".")[0].lower(), Input(n, source=str(p), link=True)) for n, p in synth.items()]

    for tag, db in dbs:
        for m in MODES:
            add(f"{tag}/{m}", ["-m", m, "-d", db.name, "-o", f"{m}.txt"], [db])
        for t in ("1", "2", "3"):
            add(f"{tag}/simStepList-t{t}",
                ["-m", "simStepList", "-d", db.name, "-o", "s.txt", "-t", t], [db])

    # Help, alone and after other options; options after -h are never parsed.
    add("help", ["-h"])
    add("help-upper", ["-H"])
    add("help-late", ["-d", "COG_P1_B1.DB", "-H", "-m", "header"], [cog])
    add("help-after-o", ["-o", "x.txt", "-h", "-o"])
    # Missing -o / -d (checked in that order), and options that only set flags.
    add("no-o", ["-m", "header", "-d", "COG_P1_B1.DB"], [cog])
    add("no-d", ["-m", "header", "-o", "h.txt"])
    add("no-od", ["-d2", "-s", "3"])
    add("only-s", ["-s", "5"])
    add("only-r", ["-r", "7", "-T", "1"])
    add("only-d", ["-d", "COG_P1_B1.DB"], [cog])
    # Mode not given / not handled / unknown: banner only.
    for m in ("", "summary", "reclist", "diff", "HEADER", "steplistx"):
        args = (["-m", m] if m else []) + ["-d", "COG_P1_B1.DB", "-o", "o.txt"]
        add(f"mode-{m or 'none'}", args, [cog])
    # Lower-case aliases, attached values, upper-case option letters, last -m wins.
    add("alias-steplist", ["-m", "steplist", "-d", "COG_P1_B1.DB", "-o", "o.txt"], [cog])
    add("alias-simsteplist", ["-m", "simsteplist", "-d", "COG_P1_B1.DB", "-o", "o.txt"], [cog])
    add("attached", ["-mheader", "-dCOG_P1_B1.DB", "-oh.txt"], [cog])
    add("upper", ["-M", "lastStepNo", "-D", "COG_P1_B1.DB", "-O", "o.txt"], [cog])
    add("last-m-wins", ["-m", "header", "-m", "lastStepNo", "-d", "COG_P1_B1.DB", "-o", "o.txt"], [cog])
    add("unknown-m-keeps", ["-m", "stepList", "-m", "bogus", "-d", "COG_P1_B1.DB", "-o", "o.txt"], [cog])
    add("unknown-opt", ["-x", "-q", "-m", "header", "-d", "COG_P1_B1.DB", "-o", "o.txt"], [cog])
    add("dash-only", ["-", "-m", "header", "-d", "COG_P1_B1.DB", "-o", "o.txt"], [cog])
    # Second -d goes to the unused second DB name; the first one is used.
    add("two-d", ["-m", "lastStepNo", "-d", "COG_P1_B1.DB", "-d", "nothere.DB", "-o", "o.txt"], [cog])
    add("two-d-swap", ["-m", "lastStepNo", "-d", "nothere.DB", "-d", "COG_P1_B1.DB", "-o", "o.txt"], [cog])
    # Two -o: the last one wins.
    add("two-o", ["-m", "lastStepNo", "-d", "COG_P1_B1.DB", "-o", "a.txt", "-o", "b.txt"], [cog])
    # Quote stripping (FUN_14001b700), on values the C runtime leaves quotes in.
    add("quote-both", ["-m", "lastStepNo", "-d", '"COG_P1_B1.DB"', "-o", "o.txt"], [cog])
    add("quote-trail", ["-m", "lastStepNo", "-d", 'COG_P1_B1.DB"', "-o", "o.txt"], [cog])
    add("quote-lead", ["-m", "lastStepNo", "-d", '"COG_P1_B1.DB', "-o", "o.txt"], [cog])
    add("quote-o", ["-m", "lastStepNo", "-d", "COG_P1_B1.DB", "-o", '"q.txt"'], [cog])
    add("quote-only", ["-m", "lastStepNo", "-d", '"', "-o", "o.txt"])
    for m in MODES:
        add(f"empty-d/{m}", ["-m", m, "-d", "", "-o", "o.txt"])
        add(f"blank-d/{m}", ["-m", m, "-d", "  ", "-o", "o.txt"])
    add("empty-o", ["-m", "lastStepNo", "-d", "COG_P1_B1.DB", "-o", ""], [cog])
    add("empty-o-sim", ["-m", "simStepList", "-d", "COG_P1_B1.DB", "-o", ""], [cog])
    # NEWEQ.MSG is created before the arguments are parsed.
    add("o-neweq", ["-m", "stepList", "-d", "COG_P1_B1.DB", "-o", "NEWEQ.MSG"], [cog])
    add("d-neweq", ["-m", "header", "-d", "NEWEQ.MSG", "-o", "o.txt"])
    # Values of 1023 and 1024 characters (FUN_14001b700 copies into a 1025-byte stack buffer).
    add("long-1023", ["-m", "lastStepNo", "-d", "COG_P1_B1.DB", "-r", "7" * 1023, "-o", "o.txt"], [cog])
    add("long-1024", ["-m", "lastStepNo", "-d", "COG_P1_B1.DB", "-r", "7" * 1024, "-o", "o.txt"], [cog])
    add("t-text", ["-m", "simStepList", "-d", "COG_P1_B1.DB", "-o", "o.txt", "-t", "1x"], [cog])
    add("t-quoted", ["-m", "simStepList", "-d", "COG_P1_B1.DB", "-o", "o.txt", "-t", '"2"'], [cog])
    # Bad databases, for every mode.
    bad = {
        "missing": None,
        "empty": b"",
        "text": b"hello world\r\n",
        "sig720": raw_db(b"SFTC-DEFORM-2D-DB720"),
        "nohdr": raw_db(b"SFTC-DEFORM-23-DB100"),
        "zero-rec2": raw_db(b"SFTC-DEFORM-23-DB100", b"\0" * 136),
        "short-sig": b"SFTC-DEFORM-23-DB100",
    }
    cog_bytes = COG.read_bytes()
    bad["cut-3rec"] = cog_bytes[:3 * 512]
    bad["cut-20rec"] = cog_bytes[:20 * 512]
    for tag, data in bad.items():
        inputs = [] if data is None else [Input("BAD.DB", data=data)]
        for m in MODES:
            add(f"bad-{tag}/{m}", ["-m", m, "-d", "BAD.DB", "-o", "o.txt"], inputs)
    # Output file that cannot be opened.
    for m in MODES:
        add(f"o-nodir/{m}", ["-m", m, "-d", "COG_P1_B1.DB", "-o", "nodir/o.txt"], [cog])
    add("o-nodir/bad-header", ["-m", "header", "-d", "BAD.DB", "-o", "nodir/o.txt"],
        [Input("BAD.DB", data=b"x")])
    # Value missing at the end of the command line (argv[argc] is NULL).
    add("tail-o", ["-m", "header", "-d", "COG_P1_B1.DB", "-o"], [cog])
    add("tail-d", ["-m", "header", "-o", "o.txt", "-d"], [cog])
    add("tail-m", ["-d", "COG_P1_B1.DB", "-o", "o.txt", "-m"], [cog])
    add("tail-t", ["-m", "header", "-d", "COG_P1_B1.DB", "-o", "o.txt", "-t"], [cog])
    # Existing output and NEWEQ.MSG are replaced.
    add("overwrite", ["-m", "lastStepNo", "-d", "COG_P1_B1.DB", "-o", "o.txt"],
        [cog, Input("o.txt", data=b"old contents\r\nmore\r\n"), Input("NEWEQ.MSG", data=b"old msg\n")])
    return out


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--port", help="the port's def_pre executable")
    ap.add_argument("--defcmp", help="defcmp executable")
    ap.add_argument("--dbmanager", default=str(REPO / "port" / "build" / "deformdb" / "dbmanager.exe"))
    ap.add_argument("--work", default=str(REPO / "port" / "build" / "mcli_synth"))
    ap.add_argument("--runs", default=os.environ.get("DEFORM_PORT_RUNS")
                    or str(Path.home() / ".deform-port-runs"))
    ap.add_argument("--oracle-only", action="store_true")
    ap.add_argument("--offline", action="store_true", help="never run the original")
    ap.add_argument("--list", action="store_true")
    ap.add_argument("-j", "--jobs", type=int, default=8)
    ap.add_argument("--report", help="write one TSV row per case")
    ap.add_argument("ids", nargs="*", help="case ids or prefixes (default: all)")
    a = ap.parse_args(argv)

    allc = cases(make_synth(Path(a.work), Path(a.dbmanager)))
    if a.ids:
        allc = [(i, c) for i, c in allc if any(i == p or i.startswith(p) for p in a.ids)]
    if a.list:
        for i, c in allc:
            print(i, " ".join(c.args))
        return 0
    oracle = Oracle(offline=a.offline or None)
    port = None
    if not a.oracle_only:
        if not (a.port and a.defcmp):
            ap.error("--port and --defcmp are required unless --oracle-only")
        port = PortRunner(Path(a.port).resolve(), a.runs, oracle)
        a.defcmp = str(Path(a.defcmp).resolve())

    def one(item):
        cid, case = item
        row = {"id": cid, "status": "ERROR", "detail": "", "exit": ""}
        try:
            exp = oracle.run(case)
            row["exit"] = exp.exit_code
            if port is None:
                row["status"] = "CACHED"
                row["detail"] = ",".join(sorted(exp.files))
                return row
            act = port.run(case, refresh=True)
            p = subprocess.run([a.defcmp, "oracle", str(exp.dir), str(act.dir)],
                               capture_output=True, text=True, errors="replace")
            row["status"] = {0: "PASS", 1: "FAIL"}.get(p.returncode, "ERROR")
            if row["status"] == "PASS":
                _rmtree(act.dir)
            else:
                row["detail"] = summarize(p.stdout, p.stderr) + f" | port {act.dir}"
        except (OracleError, OSError) as e:
            row["detail"] = str(e)[:300]
        return row

    with concurrent.futures.ThreadPoolExecutor(a.jobs) as ex:
        rows = list(ex.map(one, allc))
    for r in rows:
        print(f"{r['status']:<6} {r['id']:<34} exit={r['exit']!s:<11} {r['detail']}")
    n = {s: sum(r["status"] == s for r in rows) for s in ("PASS", "FAIL", "ERROR", "CACHED")}
    print(f"total {len(rows)}: " + ", ".join(f"{k} {v}" for k, v in n.items() if v))
    if a.report:
        with open(a.report, "w", encoding="utf-8") as f:
            f.write("id\tstatus\texit\tdetail\n")
            for r in rows:
                f.write(f"{r['id']}\t{r['status']}\t{r['exit']}\t{r['detail']}\n")
    return 0 if all(r["status"] in ("PASS", "CACHED") for r in rows) else 1


if __name__ == "__main__":
    sys.exit(main())
