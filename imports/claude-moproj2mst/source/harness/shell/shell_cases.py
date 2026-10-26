"""Run JSON oracle cases (tests/commands/*/*.json) through the original and the port, compare.

    python -I harness/shell/shell_cases.py --port port/build/def_pre.exe \
        --defcmp port/build/defcmp/defcmp.exe tests/commands/SHELL/*.json [--offline] [-j N]

Packet p5-01 (menu shell): the T5-1 cases in tests/commands/SHELL, FILE_INPUT and MENU9.
The original runs through tools/oracle (cached); the port in $DEFORM_PORT_RUNS like the corpus
runner; defcmp oracle compares exit code, stdout, stderr and every file written.
"""
import argparse
import concurrent.futures
import json
import os
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "oracle"))
sys.path.insert(0, str(ROOT / "harness" / "corpus"))
from def_pre_oracle import Case, Oracle, OracleError  # noqa: E402
from runner import PortRunner, summarize  # noqa: E402
from cases import by_id, tier_ids  # noqa: E402


def neweq(exp_dir: str, act_dir: str) -> tuple[bool, str]:
    """NEWEQ.MSG after the run: present in both or in neither, and byte-identical.

    main 14001ce7b: OPEN(1, 'NEWEQ.MSG', STATUS='UNKNOWN') + CLOSE(1, STATUS='DELETE') (an old file
    goes), OPEN again + REWIND (an empty file exists from start-up on); Fortran WRITE(1, ...) records
    go into it; 14001d1bf CLOSE(1, STATUS='DELETE') at the end of a menu session, so it remains only
    after exit(0) (DEF_MULTI.INI, -m) or an abnormal end."""
    e = Path(exp_dir) / "files" / "NEWEQ.MSG"
    a = Path(act_dir) / "files" / "NEWEQ.MSG"
    if e.exists() != a.exists():
        return False, f"NEWEQ.MSG present: original {e.exists()}, port {a.exists()}"
    if e.exists() and e.read_bytes() != a.read_bytes():
        return False, f"NEWEQ.MSG bytes differ ({e.stat().st_size} vs {a.stat().st_size})"
    fe = sorted(q.name for q in (Path(exp_dir) / "files").glob("fort.*"))
    fa = sorted(q.name for q in (Path(act_dir) / "files").glob("fort.*"))
    if fe != fa:
        return False, f"stray Fortran unit files: original {fe or 'none'}, port {fa or 'none'}"
    return True, (f"NEWEQ.MSG {'present, ' + str(e.stat().st_size) + ' bytes' if e.exists() else 'absent'}"
                  f" in both; fort.* {fe or 'none'}")


def main(argv=None) -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--port", required=True)
    ap.add_argument("--defcmp", required=True)
    ap.add_argument("--offline", action="store_true")
    ap.add_argument("--runs", default=os.environ.get("DEFORM_PORT_RUNS")
                    or str(Path.home() / ".deform-port-runs"))
    ap.add_argument("-j", "--jobs", type=int, default=4)
    ap.add_argument("--corpus", nargs="*", default=[], metavar="ID",
                    help="corpus case ids (harness/corpus/cases.py), e.g. the tier-2 ids")
    ap.add_argument("--tier2", action="store_true", help="add every tier-2 corpus case")
    ap.add_argument("files", nargs="*")
    a = ap.parse_args(argv)
    cases = []
    for f in a.files:
        d = json.loads(Path(f).read_text(encoding="utf-8"))
        for item in d if isinstance(d, list) else [d]:
            c = Case.from_json(item, Path(f).parent)
            cases.append((Path(f).stem, c))
    corpus = by_id()
    for cid in (tier_ids(2) if a.tier2 else []) + a.corpus:
        cases.append((cid, corpus[cid].oracle_case()))
    oracle = Oracle(offline=a.offline or None)
    port = PortRunner(Path(a.port).resolve(), a.runs, oracle)
    defcmp = str(Path(a.defcmp).resolve())

    def one(fc):
        name, c = fc
        try:
            exp = oracle.run(c)
            act = port.run(c, refresh=True)
            p = subprocess.run([defcmp, "oracle", str(exp.dir), str(act.dir)],
                               capture_output=True, text=True, errors="replace")
            st = {0: "PASS", 1: "FAIL"}.get(p.returncode, "ERROR")
            det = "" if st == "PASS" else summarize(p.stdout, p.stderr)
            ok, why = neweq(str(exp.dir), str(act.dir))
            if not ok:
                st = "FAIL"
            det = (det + " | " if det else "") + why
            return name, st, det, str(exp.dir), str(act.dir)
        except (OracleError, OSError) as e:
            return name, "ERROR", str(e)[:300], "", ""

    n_pass = 0
    with concurrent.futures.ThreadPoolExecutor(a.jobs) as ex:
        for name, st, det, ed, ad in ex.map(one, cases):
            n_pass += st == "PASS"
            print(f"{st:<5} {name}  {det}" + (f"\n      oracle {ed}\n      port   {ad}" if st != "PASS" else ""),
                  flush=True)
    print(f"{n_pass}/{len(cases)} passed")
    return 0 if n_pass == len(cases) else 1


if __name__ == "__main__":
    sys.exit(main())
