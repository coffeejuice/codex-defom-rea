#!/usr/bin/env python3
"""Assign every Phase 5, 8, 9 and 10 ledger function to a porting packet (docs/phase5_8_9_10-packets.md).

    python -I docs/re/phase5_8_9_10_packets.py [--work C:/Users/alext/Projects/deform-rea-work/out]

Reads docs/re/pre64_batch_ledger.tsv and <work>/pre64_funcs.tsv (Ghidra function list with
callers/callees) and writes docs/re/phase5_8_9_10_packet_map.tsv plus a summary on stdout.

Phases 8-9: the immediate-dominator tree of the call graph restricted to the new P8/P9 functions,
with the command handlers as roots (Cooper-Harvey-Kennedy). A function goes to the packet of its
nearest dominator listed in CUTS; a function that only one cut reaches therefore follows that cut,
and helpers shared by several cuts get an explicit cut of their own. Functions no direct call
reaches (vtable slots, the DXF entity table, function pointers, the MORP argument list built
inline in FUN_140378e00) are listed in CUTS by hand.
Phase 5: one packet, plus the menu-2 and menu-9 functions (first phase P9).
Phase 10: closure of menus 3-4 (p10-01), then menus 5-6 (p10-02); the rest are the registry
column-3 label handlers (p10-03).
"""
from __future__ import annotations

import argparse
import collections
import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
LEDGER = ROOT / "docs/re/pre64_batch_ledger.tsv"
OUT = ROOT / "docs/re/phase5_8_9_10_packet_map.tsv"

# Command handlers and menu entries of phases 8-9 (docs/re/commands/phase8_9-index.md).
ROOTS = ["1401a9a10", "1401a9be0", "1401a9ac0", "1401aa000", "1401aa370", "1401aa750",
         "1401aa9a0", "1401ac020", "1401ac310", "1401ac680", "1401467f0", "14000afd0",
         "140014ba0", "14000b170"]

# Packet -> dominator-tree nodes that start it (the node and everything it dominates).
CUTS = {
    "nn-shape-roll": [
        "1402bd210",
    ],
    "p5-01": [
        "14000afd0", "140014ba0", "14000b170",
    ],
    "p8-01": [
        "1401aa9a0", "1401aa750", "1401ac020", "1401ac310", "1401ac680", "140382510", "140382580", "1403825a0",
        "140382660", "1403826a0", "1403827f0", "140382890", "1403828d0", "1403829a0", "140382ab0", "140382b60",
        "140382be0", "140382cd0", "140382d00", "140382e80", "140383000", "140383020", "140383060", "140383080",
    ],
    "p8-02": [
        "140243a70",
    ],
    "p8-03": [
        "14040c220", "140409af0", "14040cd90", "14040cda0", "140454b50",
    ],
    "p8-04": [
        "1404093a0", "1404245b0", "1404246c0", "140424700", "140424b90", "140424c80", "140424da0", "1404250f0",
        "1404251e0", "1404258d0", "1404259c0", "140425cf0", "140425d50", "1404261f0", "1404262e0", "140426810",
        "140427dd0", "140427ec0", "140428b80", "140428cf0", "140428e70", "140428eb0", "140428ef0", "140409ae0",
        "140409880", "140409890",
    ],
    "p8-05": [
        "1401a9ac0", "14013ca60",
    ],
    "p8-06": [
        "140140110", "14026a4f0",
    ],
    "p8-07": [
        "1401a9be0", "140144a70", "14013f010", "140158d50", "140158d70", "140158d80", "140158d90", "140158da0",
        "140158db0", "140158dc0", "140158dd0", "140158de0", "14013e790", "14013e840", "14013ee00", "14013f410",
        "140141580", "140143920", "140144200", "1401443c0", "1401444f0", "140148840", "140155340", "1402062b0",
    ],
    "p8-08": [
        "1401a9a10", "1401aa000", "1401aa370", "14013e880", "140155c90", "1401563c0", "140156640", "140156950",
        "140156cd0", "140157040", "1401573c0", "1401575a0", "140159300", "140159dd0", "1401b1520", "1401b17b0",
        "1401d7320", "1401d7390", "1401f3660", "1401e8040", "1401e84f0", "1401ea900", "14029b630", "140339d30",
        "140180a40",
    ],
    "p8-09": [
        "1402566d0", "140246d00", "140246d40", "140246e40", "140246f70", "14024bac0", "140256800", "14040e620",
        "14040e810", "14040fde0", "140410fd0", "140411080", "1404115d0", "1402570e0", "140411620", "140411920",
        "140411940", "140411ad0", "14023a380", "140202700", "1402027e0", "140158fc0", "140198270", "1401ffd80",
        "140006730",
    ],
    "p9-01": [
        "1401467f0", "1402e9040", "1402e9090", "140007660",
    ],
    "p9-02": [
        "1402b8090", "1402b8520", "1402b7ad0", "1402b8980", "1402f0620",
    ],
    "p9-03": [
        "140147b70",
    ],
    "p9-04": [
        "1402ba500", "1402e24f0", "1402baa60",
    ],
    "p9-05": [
        "1402ce190", "14041f830",
    ],
}

# GEOPRIM class code that only the CNVT3D root dominates goes with the GEOPRIM object packet.
RANGE_MOVES = [("1402b7400", "1402f2fff", "p9-01", "p9-02")]

P10_MENUS_34 = ["140017540", "1400157c0"]
P10_MENUS_56 = ["14001a050", "140019c20"]


def load(work: Path):
    led = {r["address"][2:]: r for r in csv.DictReader(LEDGER.open(), delimiter="\t")}
    callees, size = {}, {}
    for line in (work / "pre64_funcs.tsv").open(encoding="utf-8", errors="replace"):
        c = line.rstrip("\n").split("\t")
        if len(c) < 6:
            continue
        size[c[0]] = int(c[1])
        callees[c[0]] = [x for x in c[4][2:].split(",") if x]
    return led, callees, size


def dominators(nodes: set[str], roots: list[str], callees) -> dict[str, str]:
    succ = {a: [b for b in callees.get(a, []) if b in nodes and b not in roots] for a in nodes}
    succ["ROOT"] = roots
    order, seen = [], {"ROOT"}
    stack = [("ROOT", iter(succ["ROOT"]))]
    while stack:
        x, it = stack[-1]
        for y in it:
            if y not in seen:
                seen.add(y)
                stack.append((y, iter(succ[y])))
                break
        else:
            order.append(x)
            stack.pop()
    rpo = order[::-1]
    idx = {a: i for i, a in enumerate(rpo)}
    pred = collections.defaultdict(list)
    for a in rpo:
        for b in succ[a]:
            pred[b].append(a)
    idom = {"ROOT": "ROOT"}
    changed = True
    while changed:
        changed = False
        for b in rpo[1:]:
            ps = [p for p in pred[b] if p in idom]
            new = ps[0]
            for p in ps[1:]:
                x, y = p, new
                while x != y:
                    while idx[x] > idx[y]:
                        x = idom[x]
                    while idx[y] > idx[x]:
                        y = idom[y]
                new = x
            if idom.get(b) != new:
                idom[b] = new
                changed = True
    return idom


def closure(roots, nodes, callees) -> set[str]:
    seen = {r for r in roots if r in nodes}
    stack = list(roots)
    while stack:
        for b in callees.get(stack.pop(), []):
            if b in nodes and b not in seen:
                seen.add(b)
                stack.append(b)
    return seen


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--work", default="C:/Users/alext/Projects/deform-rea-work/out")
    led, callees, size = load(Path(ap.parse_args().work))
    phase = {a: r["first_phase"] for a, r in led.items()}

    p89 = {a for a, p in phase.items() if p in ("P8", "P9")}
    idom = dominators(p89 | set(ROOTS), ROOTS, callees)
    cut = {a: p for p, xs in CUTS.items() for a in xs}
    asg, via = {}, {}
    for a in p89:
        x = a
        while x not in cut and x in idom and x != "ROOT":
            x = idom[x]
        if x not in cut:
            raise SystemExit(f"no packet for {a} (dominator chain ends at {x})")
        asg[a], via[a] = cut[x], x
        for lo, hi, src, dst in RANGE_MOVES:
            if asg[a] == src and int(lo, 16) <= int(a, 16) <= int(hi, 16):
                asg[a], via[a] = dst, f"range {lo}-{hi}"
    for a, p in phase.items():
        if p == "P5":
            asg[a], via[a] = "p5-01", "main"
    p10 = {a for a, p in phase.items() if p == "P10"}
    m34 = closure(P10_MENUS_34, p10, callees)
    m56 = closure(P10_MENUS_56, p10, callees) - m34
    for a in p10:
        asg[a] = "p10-01" if a in m34 else "p10-02" if a in m56 else "p10-03"
        via[a] = "menus 3-4" if a in m34 else "menus 5-6" if a in m56 else "registry col 3"

    with OUT.open("w", newline="\n") as f:
        f.write("address\tsize\tarea\tfirst_phase\tvia\tpacket\tdominator_cut\n")
        for a in sorted(asg):
            r = led[a]
            f.write(f"0x{a}\t{r['size']}\t{r['area']}\t{r['first_phase']}\t{r['via']}\t{asg[a]}\t{via[a]}\n")

    stats = collections.defaultdict(lambda: [0, 0, collections.Counter(), collections.Counter()])
    for a, p in asg.items():
        s = stats[p]
        s[0] += 1
        s[1] += size.get(a, 0)
        for b in callees.get(a, []):
            if b in asg and asg[b] != p:
                s[2][asg[b]] += 1
            elif b in phase and b not in asg:
                s[3][phase[b]] += 1
    print("packet\tfunctions\tbytes\tcalls into packets (edges)\tcalls into earlier phases (edges)")
    key = lambda p: (p.startswith("nn"), int(p[1:].split("-")[0]) if p[0] == "p" else 0, p)
    for p in sorted(stats, key=key):
        n, b, d, e = stats[p]
        print(p, n, b, ", ".join(f"{q}({k})" for q, k in d.most_common()),
              ", ".join(f"{q}({k})" for q, k in sorted(e.items())), sep="\t")


if __name__ == "__main__":
    main()
