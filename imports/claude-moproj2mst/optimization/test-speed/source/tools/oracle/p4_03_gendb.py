"""Oracle cases for packet p4-03: GENDB straight from every corpus KEY file.

    python -I tools/oracle/p4_03_gendb.py [-j N] [--list] [--dim 2D|3D]

Each case is the keyword program `KFREAD / <key> / GENDB / OUT.DB` run through menu 2 -> 1, so
DEF_PRE's stdout holds the GENDB input checks (simulation control, materials, inter-material
data, objects, inter-object data) for that KEY file. The p4-03 oracle test compares the material,
inter-material and object sections with the port. Results go to the shared oracle cache; running
again only runs what is missing (resumable).
"""
from __future__ import annotations

import argparse
import concurrent.futures
import json
import os
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "oracle"))
sys.path.insert(0, str(ROOT / "harness" / "corpus"))

from def_pre_oracle import Case, Input, Oracle  # noqa: E402
import cases as corpus  # noqa: E402

PROGRAM = "KFREAD\n{name}\nGENDB\nOUT.DB\n"


def gendb_cases(dim: str | None = None) -> list[Case]:
    root = corpus.corpus_root()
    out = []
    for tc in corpus.all_cases():
        if tc.kind != "kfrw" or not tc.path.upper().endswith(".KEY"):
            continue
        if dim and tc.dim != dim:
            continue
        src = root / tc.path
        c = Case.keyfile(PROGRAM.format(name=src.name), [Input(src.name, source=str(src))])
        c.label = f"gendb/{tc.path}"
        out.append(c)
    return out


def main(argv=None) -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("-j", "--jobs", type=int, default=max(1, (os.cpu_count() or 4) // 2))
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--dim")
    a = ap.parse_args(argv)
    cs = gendb_cases(a.dim)
    if a.list:
        for c in cs:
            print(c.label)
        return 0
    oracle = Oracle()
    todo = [c for c in cs if oracle.lookup(c) is None]
    print(f"{len(cs)} cases, {len(todo)} to run", flush=True)
    fails = 0
    with concurrent.futures.ThreadPoolExecutor(a.jobs) as ex:
        futs = {ex.submit(oracle.run, c): c for c in todo}
        for f in concurrent.futures.as_completed(futs):
            c = futs[f]
            try:
                r = f.result()
                print(json.dumps({"label": c.label, "exit": r.exit_code}), flush=True)
            except Exception as e:  # keep going; a rerun retries
                fails += 1
                print(json.dumps({"label": c.label, "error": str(e)}), flush=True)
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
