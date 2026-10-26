"""Generate the extra 2D GENCTC command files (packet p67-6) from a COPY of the SPIKE_SI seed.

Each file reads the seed with KFREAD, then overrides part of the model with ordinary keywords
(RZ of object 1 with some contact nodes displaced, OBJTYP, BCCDEF) before INICTC/GENCTC, so that
the cases reach the code paths the plain seed does not: nodes that actually move, the 1004
warning, the meshed-master kernel, the element check rejecting a pair, the 1001 warning.

Usage: python -I gen_spike2d.py <path of SPIKE_SI.KEY>   (writes next to this script)
"""
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
SEED = "C:/Users/alext/Projects/DEFORM_corpus/key/2D/SI/Forging/SPIKE_SI.KEY"


def block(lines, kw, obj):
    """lines of the record 'kw obj' (header + continuation lines)"""
    for i, l in enumerate(lines):
        t = l.split()
        if len(t) >= 2 and t[0] == kw and t[1] == str(obj):
            j = i + 1
            while j < len(lines) and lines[j].startswith(" "):
                j += 1
            return lines[i:j]
    raise KeyError((kw, obj))


def rz_block(lines, shift):
    """RZ 1 with node y values shifted: shift = {node: dy}"""
    b = block(lines, "RZ", 1)
    out = [b[0]]
    for l in b[1:]:
        t = l.split()
        n = int(t[0])
        x, y = float(t[1]), float(t[2])
        y += shift.get(n, 0.0)
        out.append("%8d %20.10E %20.10E" % (n, x, y))
    return out


def contact_nodes(lines):
    b = block(lines, "BCCDEF", 1)
    return [int(l.split()[0]) for l in b[1:] if int(l.split()[2]) < 0]


def main():
    seed = sys.argv[1] if len(sys.argv) > 1 else SEED
    lines = open(seed).read().splitlines()
    cn = contact_nodes(lines)
    head = ["KFREAD 0", "SPIKE_SI.KEY"]
    tail = ["KFWRIT", "OUT.KEY"]
    cases = {}
    # nodes displaced off the die by alternating +-0.002, tol 0.01: all pulled back
    move = {n: (0.002 if k % 2 == 0 else -0.002) for k, n in enumerate(cn)}
    cases["spike_move"] = ("* 2D GENCTC: contact nodes of obj 1 displaced by +-0.002, tol 0.01: moved back onto the die",
                           rz_block(lines, move) + ["INICTC 1", "GENCTC 0.01"])
    # a few nodes pushed 0.5 into the die with tol 0.001: moved > 100*tol -> warning 1004, put back
    deep = dict(move)
    for n in cn[2:6]:
        deep[n] = -0.5
    cases["spike_warn"] = ("* 2D GENCTC: four nodes 0.5 inside the die, tol 0.001: warning 1004, nodes put back",
                           rz_block(lines, deep) + ["INICTC 1", "GENCTC 0.001"])
    # dies made plastic (internal type 1): the meshed-master kernel 14034fc56
    cases["spike_meshed"] = ("* 2D GENCTC: objects 2 and 3 as plastic (meshed master), displaced contact nodes",
                             ["OBJTYP 2 2 0", "OBJTYP 3 2 0"] + rz_block(lines, move) + ["INICTC 1", "GENCTC 0.01"])
    cases["spike_meshed_0"] = ("* 2D GENCTC: objects 2 and 3 as plastic (meshed master), seed coordinates, tol 0",
                               ["OBJTYP 2 2 0", "OBJTYP 3 2 0", "INICTC 1", "GENCTC 0"])
    # very large tolerance: far nodes pulled onto the dies, elements turn over -> pair not committed
    cases["spike_huge"] = ("* 2D GENCTC: tol 3.0 pulls distant nodes; the element check may reject the pair",
                           rz_block(lines, move) + ["INICTC 1", "GENCTC 3.0"])
    # codes 1 1 on a contact node: warning 1001 (unable to change BCC)
    bcc = ["BCCDEF       1       2       0", "%8d %7d %7d" % (cn[3], 1, 1), "%8d %7d %7d" % (cn[4], 1, 1)]
    cases["spike_bcc"] = ("* 2D GENCTC: two contact nodes with codes (1,1): warning 1001",
                          ["INICTC 1"] + bcc + ["GENCTC 0"])
    entries = []
    for name, (comment, body) in cases.items():
        fn = "genctc_%s.KEY" % name
        with open(os.path.join(HERE, fn), "w", newline="\n") as f:
            f.write("\n".join([comment] + head + body + tail) + "\n")
        entries.append({
            "label": "GENCTC/%s" % name,
            "stdin": "\n2\n1\nCMD.KEY\n\nE\nE\nY\n",
            "timeout": 600,
            "inputs": [{"name": "SPIKE_SI.KEY", "source": SEED}, {"name": "CMD.KEY", "source": fn}],
        })
    with open(os.path.join(HERE, "cases_p676.json"), "w", newline="\n") as f:
        json.dump(entries, f, indent=1)


if __name__ == "__main__":
    main()
