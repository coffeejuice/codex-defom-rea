#!/usr/bin/env python3
"""Assign every Phase 1 ledger function to a porting packet (docs/phase1-packets.md).

    python -I docs/re/phase1_packets.py [--work C:/Users/alext/Projects/deform-rea-work/out]

Reads docs/re/pre64_batch_ledger.tsv (first_phase P1), <work>/pre64_funcs.tsv (Ghidra function
list with callers/callees) and <work>/ledger/ptrtables.tsv (vtables with RTTI names), and writes
docs/re/phase1_packet_map.tsv (one row per function) plus a summary on stdout.

Packets follow address ranges, because MSVC and the Fortran compiler emit one object file's
functions contiguously, so a range is roughly one source file; virtual methods go to the packet
that owns their class (CLASS_OWNER) wherever the linker put them. Two groups are not packets:
  m-cli    the -m CLI functions ported by the -m thread;
  licence  licence/Sentinel/process-monitor code and functions only it calls (never ported).
"""
from __future__ import annotations

import argparse
import collections
import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
LEDGER = ROOT / "docs/re/pre64_batch_ledger.tsv"
OUT = ROOT / "docs/re/phase1_packet_map.tsv"

M_CLI = {"14001b7b0", "14001c500", "14001bc80", "14001be50", "14001bf40", "14001c100"}
# Functions whose strings show licence checks or the SDSPM process monitor (noted, not analysed).
LICENCE_SEEDS = {"1403c725d", "1403c768e", "14044cf00", "14044cfc0"}

# (lo, hi, packet): first match wins.
RANGES = [
    ("140001000", "14001c770", "p1-01"), ("14050402c", "1405041ec", "p1-01"),
    ("1401b1ff0", "1401b5140", "p1-01"),
    ("140501bd0", "140503fff", "p1-02"),
    ("1401b8000", "1401ee5a0", "p1-03"),
    ("140130530", "140197fff", "p1-04"),
    ("1401999c0", "1401b0fff", "p1-05"),
    ("1402248c0", "140224b10", "p1-12"), ("140235620", "14023b720", "p1-12"),
    ("1401f4390", "14026ffff", "p1-06"),
    ("140275b00", "140280760", "p1-07"), ("1402b7400", "1402fe990", "p1-07"),
    ("1402838d0", "14029e210", "p1-08"),
    ("1402fe991", "140309550", "p1-09"), ("14032a000", "14032d1ff", "p1-09"),
    ("1403239f0", "1403769f0", "p1-10"),
    ("140412010", "14041f590", "p1-11"), ("14008c3c0", "14008ffff", "p1-11"),
    ("1404cf5c0", "1404dffff", "p1-11"),
    ("1403d84b0", "1403e5d60", "p1-13"),
    ("1403b1e45", "140407e81", "p1-12"), ("14043658f", "14046f6d0", "p1-12"),
    ("14042a870", "140431930", "p1-14"),
]
CLASS_OWNER = {
    "_OBJ_GEO2D": "p1-07", "OBJ_GEO_BASE": "p1-07", "C2d3dGeometry": "p1-07",
    "_OBJ_GEO3D": "p1-07", "_DEF_FACE": "p1-07",
    "SdctlGList": "p1-05", "SdctlPtrList<_OBJ_GEO2D>": "p1-05", "SdctlPtrCollection": "p1-05",
    "OBJ_MESH2D": "p1-08", "_DEF_GEOPRIM": "p1-08",
}


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--work", default="C:/Users/alext/Projects/deform-rea-work/out")
    work = Path(ap.parse_args().work)

    led = {r["address"][2:]: r for r in csv.DictReader(LEDGER.open(), delimiter="\t")}
    p1 = {a for a, r in led.items() if r["first_phase"] == "P1"}
    callers, callees, size = {}, {}, {}
    for line in (work / "pre64_funcs.tsv").open(encoding="utf-8", errors="replace"):
        c = line.rstrip("\n").split("\t")
        if len(c) < 6:
            continue
        size[c[0]] = int(c[1])
        callers[c[0]] = [x for x in c[3][2:].split(",") if x]
        callees[c[0]] = [x for x in c[4][2:].split(",") if x]
    vt = collections.defaultdict(list)
    for line in (work / "ledger/ptrtables.tsv").open():
        c = line.rstrip("\n").split("\t")
        if "vftable" in c[2] + c[3]:
            name = (c[2] or c[3]).replace("::vftable", "")
            slots = c[4].split(",")
            if name == "OBJ_GEO2D_DIGITIZED" and slots[0] != "14041f0b0":
                # No RTTI meta pointer, so IndirectRefs started one slot late: the vtable
                # begins at 0x1405681d8 with the deleting destructor 14041f0b0 (read from the PE).
                slots = ["14041f0b0"] + slots
            for i, t in enumerate(slots):
                vt[t].append(f"{name}[{i}]")

    licence = set(LICENCE_SEEDS)
    changed = True
    while changed:  # functions whose every Phase 1 caller is licence code
        changed = False
        for a in p1 - licence - M_CLI:
            cs = [x for x in callers.get(a, []) if x in p1]
            if cs and all(x in licence for x in cs):
                licence.add(a)
                changed = True

    def packet(a: str) -> str:
        if a in M_CLI:
            return "m-cli"
        if a in licence:
            return "licence"
        for v in vt.get(a, []):
            owner = CLASS_OWNER.get(v.split("[")[0])
            if owner:
                return owner
        x = int(a, 16)
        for lo, hi, p in RANGES:
            if int(lo, 16) <= x <= int(hi, 16):
                return p
        raise SystemExit(f"no packet for {a}")

    asg = {a: packet(a) for a in p1}
    with OUT.open("w", newline="\n") as f:
        f.write("address\tsize\tarea\tvia\tpacket\tvtable_slots\n")
        for a in sorted(p1):
            r = led[a]
            f.write(f"0x{a}\t{r['size']}\t{r['area']}\t{r['via']}\t{asg[a]}\t{';'.join(vt.get(a, []))}\n")

    stats = collections.defaultdict(lambda: [0, 0, 0])
    deps = collections.defaultdict(collections.Counter)
    for a, p in asg.items():
        s = int(led[a]["size"])
        stats[p][0] += 1
        stats[p][1] += s >= 64
        stats[p][2] += s
        for b in callees.get(a, []):
            if b in asg and asg[b] != p:
                deps[p][asg[b]] += 1
    print("packet\tfunctions\tsubstantial(>=64B)\tbytes\tcalls into (edge count)")
    for p in sorted(stats):
        print(p, *stats[p], ", ".join(f"{q}({n})" for q, n in deps[p].most_common()), sep="\t")


if __name__ == "__main__":
    main()
