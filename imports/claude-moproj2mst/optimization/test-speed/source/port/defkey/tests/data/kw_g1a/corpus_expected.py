"""Corpus parse check input for kw-g1a (not a tier-1 test; run by hand).

For every kfrw/<path> corpus case (harness/corpus/cases.py) whose oracle result is cached, write
into OUT (default %USERPROFILE%\\.kwg1a-corpus):
  files/NNNN.KEY        a copy of the corpus KEY file (the corpus itself is never opened by the port)
  files/NNNN.expected   DEF_PRE's KFWRIT output, "Process Definition" .. "Iteration Controls"
  files/NNNN.msgs       DEF_PRE's "ERROR: Keyword <KW> ..." lines for the keywords of ids 22..60
  manifest.tsv          NNNN <tab> case id
Then: set KWG1A_CORPUS=OUT and run defkey_test_kw_g1a_corpus.

Usage: python -I corpus_expected.py [OUT]
"""
import os
import shutil
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[4]
sys.path.insert(0, str(REPO / "tools" / "oracle"))
sys.path.insert(0, str(REPO / "harness" / "corpus"))
import cases  # noqa: E402
from def_pre_oracle import Oracle  # noqa: E402

START = "*\n*  Process Definition\n"
END = "*\n*  Iteration Controls\n"
G1A = """TITLE CURSIM OPRNAM SIMNAM STYPE SMODE GEOTYP MESHNO UNIT TNOW TRANS HTMTHD STNOUT ELMNOD
NSTART STPINC PDIE OSCTRL NSTEP EMAX TMAX SMAX VMIN LMAX STPDEF DEMAX DSMAX DSMAX_NSTEP DTMAX
DTTYPE SLDERR DTPMAX DVMAX MDSOBJ CNTERR VFACTR DTSUB DPLEN ODMAX""".split()


def main() -> int:
    out = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(os.environ["USERPROFILE"]) / ".kwg1a-corpus"
    files = out / "files"
    files.mkdir(parents=True, exist_ok=True)
    oracle = Oracle(offline=True)
    rows, missing, noslice = [], 0, 0
    for c in cases.all_cases():
        if c.kind != "kfrw":
            continue
        oc = c.oracle_case()
        r = oracle.lookup(oc)
        if r is None or cases.KFRW_OUT not in r.files:
            missing += 1
            continue
        text = r.read_text(cases.KFRW_OUT).replace("\r\n", "\n")
        i, j = text.find(START), text.find(END)
        if i < 0 or j < i:
            noslice += 1
            continue
        n = f"{len(rows):04d}"
        shutil.copyfile(cases.corpus_root() / c.path, files / f"{n}.KEY")
        (files / f"{n}.expected").write_text(text[i:j], encoding="latin-1", newline="\n")
        msgs = [ln for ln in r.stdout.decode("latin-1").replace("\r\n", "\n").split("\n")
                if ln.startswith("ERROR: Keyword ") and ln.split()[2] in G1A]
        (files / f"{n}.msgs").write_text("".join(m + "\n" for m in msgs), encoding="latin-1",
                                        newline="\n")
        rows.append(f"{n}\t{c.id}\n")
    (out / "manifest.tsv").write_text("".join(rows), encoding="utf-8", newline="\n")
    print(f"{len(rows)} cases, {missing} not cached, {noslice} without the sections")
    return 0


if __name__ == "__main__":
    sys.exit(main())
