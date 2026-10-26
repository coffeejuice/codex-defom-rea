"""Choose the tier-2 (smoke) set: the smallest set of primary corpus cases that covers every
feature the primary corpus covers, and write it to harness/corpus/tier2.tsv.

Features (cases.py): every registry keyword and command the case's input carries (KEY side from
key_coverage.tsv, DB side from db_coverage.tsv), every batch path (-m modes, DB KEY export of a
positive and a negative step, KFREAD+KFWRIT per file kind and dimension, command files per
extension).

Greedy set cover (most new features; ties to the smaller input, then the id), followed by a pass
that drops any chosen case whose features the others already cover. Deterministic: the same
manifest gives the same file. Re-run at phase gates:

    python -I harness/corpus/select_tier2.py
"""
from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cases import TIER2_FILE, all_cases  # noqa: E402


def select(cases):
    universe = set().union(*(c.features for c in cases))
    left = set(universe)
    chosen = []
    while left:
        best = min(cases, key=lambda c: (-len(c.features & left), c.cost, c.id))
        if not best.features & left:
            break
        chosen.append(best)
        left -= best.features
    # Drop redundant picks, most expensive first.
    for c in sorted(chosen, key=lambda c: (-c.cost, c.id)):
        rest = set().union(*(o.features for o in chosen if o is not c))
        if c.features <= rest:
            chosen.remove(c)
    return chosen, universe


def main() -> int:
    cases = [c for c in all_cases() if c.set == "primary"]
    chosen, universe = select(cases)
    optional_only = set().union(*(c.features for c in all_cases())) - universe
    lines = ["id\tkind\tdim\tinput_bytes\tfeatures\tunique_features"]
    for c in sorted(chosen, key=lambda c: c.id):
        others = set().union(*(o.features for o in chosen if o is not c))
        uniq = sorted(f.split(":", 1)[1] if f.startswith("kw:") else f
                      for f in c.features - others)
        lines.append(f"{c.id}\t{c.kind}\t{c.dim}\t{c.cost}\t{len(c.features)}\t{' '.join(uniq)}")
    TIER2_FILE.write_text("\n".join(lines) + "\n", encoding="utf-8")
    kw = {f for f in universe if f.startswith("kw:")}
    print(f"tier 2: {len(chosen)} cases cover {len(universe)} features "
          f"({len(kw)} keywords/commands) of {len(cases)} primary cases; "
          f"input {sum(c.cost for c in chosen) / 1e6:.1f} MB")
    if optional_only:
        print("covered only by British-unit cases (tier 3):",
              " ".join(sorted(f.replace('kw:', '') for f in optional_only)))
    print(f"wrote {TIER2_FILE}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
