"""Test cases of the DEF_PRE port, derived from the corpus manifest (docs/corpus).

Every corpus file becomes one or more cases; a case is an oracle `Case` (tools/oracle) plus the
features it exercises, which drive the tier-2 selection (select_tier2.py).

Case ids, stable across runs and used as ctest names:

  m/<db>/<mode>        DEF_PRE -m <mode> -d <db> -o <mode>.txt          (each corpus DB, 4 modes)
  dbkey/<db>/<step>    DBREAD <step> / <db> / KFWRIT / STEP_<n>.KEY     (every step of each DB)
  kfrw/<path>          KFREAD <file> / KFWRIT / KFWRIT_OUT.KEY          (every KEY, KWD, STD file)
  cmd/<path>           the command file itself as CMD.KEY, run in a copy of its folder

<db> is the DB file name without extension, <path> the file's path in the manifest.

Only docs/corpus/*.tsv are read to list cases; corpus files are needed only to build the oracle
case (`oracle_case`). The corpus root is $DEFORM_CORPUS, else C:\\Users\\alext\\Projects\\DEFORM_corpus.
"""
from __future__ import annotations

import csv
import os
import sys
from dataclasses import dataclass, field
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
CORPUS_DOCS = REPO / "docs" / "corpus"
REGISTRY = REPO / "docs" / "re" / "keyword_registry.tsv"
DEFAULT_CORPUS = r"C:\Users\alext\Projects\DEFORM_corpus"

sys.path.insert(0, str(REPO / "tools" / "oracle"))
from def_pre_oracle import M_MODES, Case as OracleCase, Input, dir_inputs  # noqa: E402

KFRW_OUT = "KFWRIT_OUT.KEY"
CMD_TIMEOUT = 300.0


def corpus_root() -> Path:
    return Path(os.environ.get("DEFORM_CORPUS") or DEFAULT_CORPUS)


@dataclass
class TestCase:
    id: str
    kind: str          # m | dbkey | kfrw | cmd
    set: str           # primary | optional
    dim: str           # 2D | 3D | -
    cost: int          # bytes of input, the tie-break when selecting
    path: str          # manifest path of the main input
    features: set[str] = field(default_factory=set)
    step: int | None = None
    mode: str | None = None

    def oracle_case(self, root: Path | None = None) -> OracleCase:
        root = root or corpus_root()
        src = root / self.path
        name = src.name
        if self.kind == "m":
            c = OracleCase.m_mode(self.mode, str(src))
        elif self.kind == "dbkey":
            c = OracleCase.key_export(str(src), self.step)
        elif self.kind == "kfrw":
            c = OracleCase.keyfile(f"KFREAD\n{name}\nKFWRIT\n{KFRW_OUT}\n",
                                   [Input(name, source=str(src))])
        elif self.kind == "cmd":
            # The command file runs in a copy of its own folder, so the files it names
            # (Operations/..., Materials/...) are where it expects them.
            inputs = [i for i in dir_inputs(src.parent, "") if i.name.upper() != "CMD.KEY"]
            c = OracleCase.keyfile(src.read_text(encoding="latin-1"), inputs)
            c.timeout = CMD_TIMEOUT
        else:
            raise ValueError(self.kind)
        c.label = self.id
        return c


def _tsv(path: Path) -> list[dict]:
    with open(path, encoding="utf-8", newline="") as f:
        return list(csv.DictReader(f, delimiter="\t"))


def registry() -> dict[str, str]:
    """keyword name -> group."""
    return {r["name"]: r["group"] for r in _tsv(REGISTRY)}


def all_cases() -> list[TestCase]:
    reg = registry()
    manifest = _tsv(CORPUS_DOCS / "manifest.tsv")
    keycov = {r["path"]: r for r in _tsv(CORPUS_DOCS / "key_coverage.tsv")}
    dbsteps: dict[str, list[dict]] = {}
    for r in _tsv(CORPUS_DOCS / "db_coverage.tsv"):
        dbsteps.setdefault(r["db"], []).append(r)

    def kws(text: str) -> set[str]:
        return {f"kw:{k}" for k in text.split() if k in reg}

    out: list[TestCase] = []
    for m in manifest:
        path, kind, size = m["path"], m["kind"], int(m["size"])
        base = dict(set=m["set"], dim=m["dim"], path=path)
        if kind == "DB":
            stem = Path(path).stem
            for mode in M_MODES:
                out.append(TestCase(f"m/{stem}/{mode}", "m", cost=size, mode=mode,
                                    features={f"path:m:{mode}"}, **base))
            for r in dbsteps.get(path, []):
                st = int(r["step"])
                out.append(TestCase(f"dbkey/{stem}/{st}", "dbkey", cost=size, step=st,
                                    features={"path:dbkey", "kw:DBREAD", "kw:KFWRIT",
                                              f"dbstep:{'neg' if st < 0 else 'pos'}"}
                                    | kws(r["keywords"]), **base))
        elif kind in ("KEY", "KWD", "STD"):
            kc = keycov.get(path, {})
            out.append(TestCase(f"kfrw/{path}", "kfrw", cost=size,
                                features={f"path:kfrw:{kind}:{m['dim']}", "kw:KFREAD",
                                          "kw:KFWRIT"}
                                | kws(kc.get("keywords", "")) | kws(kc.get("commands", "")),
                                **base))
        elif kind == "CMD":
            kc = keycov.get(path, {})
            ext = Path(path).suffix.lstrip(".").upper()
            out.append(TestCase(f"cmd/{path}", "cmd", cost=size,
                                features={f"path:cmd:{ext}"}
                                | kws(kc.get("keywords", "")) | kws(kc.get("commands", "")),
                                **base))
    out.sort(key=lambda c: c.id)
    return out


def by_id() -> dict[str, TestCase]:
    return {c.id: c for c in all_cases()}


TIER2_FILE = Path(__file__).with_name("tier2.tsv")


def tier_ids(tier: int, include_optional: bool = True) -> list[str]:
    if tier == 2:
        rows = _tsv(TIER2_FILE)
        return [r["id"] for r in rows]
    return [c.id for c in all_cases() if include_optional or c.set == "primary"]


def main(argv=None) -> int:
    import argparse
    ap = argparse.ArgumentParser(description="list corpus test cases")
    ap.add_argument("--tier", type=int, choices=(2, 3), default=3)
    ap.add_argument("--primary", action="store_true", help="tier 3: SI files only")
    ap.add_argument("--long", action="store_true", help="id, kind, set, dim, cost")
    a = ap.parse_args(argv)
    cases = by_id()
    for i in tier_ids(a.tier, not a.primary):
        c = cases[i]
        print(f"{c.id}\t{c.kind}\t{c.set}\t{c.dim}\t{c.cost}" if a.long else c.id)
    return 0


if __name__ == "__main__":
    sys.exit(main())
