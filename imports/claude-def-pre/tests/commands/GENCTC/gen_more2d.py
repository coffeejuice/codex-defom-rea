"""Generate 2D GENCTC cases on further corpus seeds whose dies have fillet arcs (packet p67-6).

For each seed (a COPY is placed by the oracle runner), the slave objects are those with negative
BCCDEF codes; each case runs INICTC on them, then GENCTC with the automatic tolerance, and a
second case with a large tolerance (0.5 x the automatic one is not known here, so 1.0e-3 x the
seed's coordinate range).

Usage: python -I gen_more2d.py   (writes genctc_m_*.KEY and cases_p676_more.json next to this file)
"""
import json
import os

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = "C:/Users/alext/Projects/DEFORM_corpus/key/2D/SI/"
SEEDS = ["Forging/VALVE_SI.KEY", "MultipleHammerOperations/HAMMER_SI.KEY",
         "ElastoPlastic/PLATE_BEND-EP_SI.KEY", "ElastoPlastic/Extrusion-EP_SI.KEY",
         "SheetForming/SHEET_SI.KEY", "Fracture/FRAC_SI.KEY"]


def slaves_and_range(path):
    lines = open(path, errors="replace").read().splitlines()
    slaves, lo, hi = [], None, None
    i = 0
    while i < len(lines):
        t = lines[i].split()
        if t and t[0] in ("BCCDEF", "RZ"):
            obj, j, neg = int(t[1]), i + 1, False
            while j < len(lines) and lines[j].startswith(" "):
                u = lines[j].split()
                if t[0] == "BCCDEF" and len(u) >= 3 and (int(u[1]) < 0 or int(u[2]) < 0):
                    neg = True
                if t[0] == "RZ" and len(u) >= 3:
                    for v in (float(u[1]), float(u[2])):
                        lo = v if lo is None else min(lo, v)
                        hi = v if hi is None else max(hi, v)
                j += 1
            if neg and obj not in slaves:
                slaves.append(obj)
            i = j
            continue
        i += 1
    return slaves, (hi - lo)


def main():
    entries = []
    for rel in SEEDS:
        seed = ROOT + rel
        name = os.path.basename(rel)
        stem = os.path.splitext(name)[0].lower().replace("-", "_")
        slaves, rng = slaves_and_range(seed)
        for tag, tol in (("0", "0"), ("big", "%.4E" % (rng * 1e-3))):
            fn = "genctc_m_%s_%s.KEY" % (stem, tag)
            body = ["* 2D GENCTC on %s: INICTC %s, GENCTC %s" % (name, " ".join(map(str, slaves)), tol),
                    "KFREAD 0", name] + ["INICTC %d" % s for s in slaves] + ["GENCTC " + tol, "KFWRIT", "OUT.KEY"]
            with open(os.path.join(HERE, fn), "w", newline="\n") as f:
                f.write("\n".join(body) + "\n")
            entries.append({"label": "GENCTC/m_%s_%s" % (stem, tag), "stdin": "\n2\n1\nCMD.KEY\n\nE\nE\nY\n",
                            "timeout": 600,
                            "inputs": [{"name": name, "source": seed}, {"name": "CMD.KEY", "source": fn}]})
    with open(os.path.join(HERE, "cases_p676_more.json"), "w", newline="\n") as f:
        json.dump(entries, f, indent=1)


if __name__ == "__main__":
    main()
