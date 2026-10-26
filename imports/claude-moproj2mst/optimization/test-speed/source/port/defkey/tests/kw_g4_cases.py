"""Build the corpus round-trip cases for test_kw_g4_corpus (packet kw-g4b, group 4 materials).

For every kfrw/ corpus case (KFREAD <file> / KFWRIT) whose oracle result is cached, writes
  <out>/<n>.in    the input KEY reduced to its comment lines and its group 4 keyword blocks
  <out>/<n>.exp   the material blocks of the original's KFWRIT_OUT.KEY (from the first
                  "Property Data of Material <n>" banner to the "Inter-Material Data" banner)
  <out>/index.tsv n, case id, number of group 4 blocks
Group 4 blocks are cut with the reference group-4/5/7 reader docs/re/keywords/tools/kwcheck457.py
(the reduced file leaves the port's reader on the same keyword boundaries); keywords of other
groups and their data are dropped. Files without materials in the output are skipped.

Usage: python -I kw_g4_cases.py [out dir]   (default %USERPROFILE%/.deform-port-runs/kw_g4)
Reads the corpus ($DEFORM_CORPUS) and the oracle cache read-only; never runs DEF_PRE.
"""
import os
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(REPO / "harness" / "corpus"))
sys.path.insert(0, str(REPO / "docs" / "re" / "keywords" / "tools"))
from cases import KFRW_OUT, all_cases, corpus_root  # noqa: E402
from def_pre_oracle import Oracle, OracleError  # noqa: E402
import kwcheck457 as kc  # noqa: E402

BANNER = b"*\r\n*  Property Data of Material     "
END = b"*\r\n*  Inter-Material Data"


def reduce_input(text, reg):
    """Comment lines and group 4 blocks of `text`, in file order (LF line ends)."""
    f = kc.CFile(text)
    out, n4 = [], 0
    mats, pairs, model = {}, {}, {}
    while True:
        start = f.p
        r, tok = f.scan('s39')
        if r == -1:
            break
        if not tok or tok[0] == '*':
            f.fgets()
            out.append(f.t[start:f.p].lstrip('\n'))
            continue
        if tok not in reg:
            f.fgets()
            continue
        kid, grp = reg[tok]
        if grp in (4, 5, 7):
            if grp == 4:
                kc.g4(f, kid, mats)
            elif grp == 5:
                kc.g5(f, kid, pairs)
            else:
                kc.g7(f, kid, model)
            if f.p > 0 and f.t[f.p - 1] != '\n':  # rest of the last line belongs to the block
                e = f.t.find('\n', f.p)
                f.p = len(f.t) if e < 0 else e + 1
            if grp == 4:
                out.append(f.t[start:f.p].lstrip(' \t\n'))
                n4 += 1
            continue
        f.fgets()
        while not f.eof():  # resync as kwcheck457.check_file does
            save = f.p
            w = f.fgets().split()
            if not w or w[0][0] == '*' or w[0] in reg:
                f.p = save
                break
    return ''.join(s if s.endswith('\n') else s + '\n' for s in out), n4


def main(argv):
    out = Path(argv[1]) if len(argv) > 1 else Path(os.environ["USERPROFILE"]) / ".deform-port-runs" / "kw_g4"
    out.mkdir(parents=True, exist_ok=True)
    reg = kc.load_registry(str(REPO / "docs" / "re" / "keyword_registry.tsv"))
    oracle = Oracle(offline=True)
    root = corpus_root()
    rows, missing = [], 0
    for tc in all_cases():
        if tc.kind != "kfrw":
            continue
        try:
            res = oracle.run(tc.oracle_case(root))
        except (OracleError, OSError):
            missing += 1
            continue
        key = res.dir / "files" / KFRW_OUT
        if not key.exists():
            continue
        data = key.read_bytes()
        i = data.find(BANNER)
        if i < 0:
            continue
        j = data.find(END, i)
        exp = data[i:j if j >= 0 else len(data)]
        text = (root / tc.path).read_bytes().decode("latin-1")  # keeps lone CRs
        red, n4 = reduce_input(text, reg)
        n = len(rows)
        (out / f"{n}.in").write_bytes(red.encode("latin-1"))
        (out / f"{n}.exp").write_bytes(exp)
        rows.append(f"{n}\t{tc.id}\t{n4}")
    (out / "index.tsv").write_text("\n".join(rows) + "\n", encoding="utf-8")
    print(f"{len(rows)} cases in {out} ({missing} kfrw cases not in the oracle cache)")


if __name__ == "__main__":
    main(sys.argv)
