"""Rewrite a decompiled C file so region-index globals read as region names.

  *(int *)(P + 0x1b90 + (longlong)DAT_x * 4)  ->  MARSTART(P,IISIM)  / MARWIDTH(P,IISIM)
  *(int *)(P + 0x1dc0 + (longlong)DAT_x * 4)  ->  ASTART(P,LLSIM)    / AWIDTH(P,LLSIM)
  remaining DAT_x                              ->  R_<name>
usage: annotate.py region_globals.tsv in.c out.c
"""
import re, sys
g = {}
for line in open(sys.argv[1]).read().splitlines()[1:]:
    glob, arr, i2, n2, i3, n3 = line.split("\t")
    g[glob] = (arr, n3 or n2)
src = open(sys.argv[2]).read()

def rep(m):
    p, off, glob = m.group(1), m.group(2), m.group(3)
    if glob not in g:
        return m.group(0)
    arr, name = g[glob]
    kind = "WIDTH" if name.startswith("width:") else "START"
    name = name.replace("width:", "")
    return "%s%s(%s,%s)" % ("MAR" if off == "0x1b90" else "A", kind, p.strip(), name)

src = re.sub(r"\*\(int \*\)\(([^()]*?(?:\([^()]*\))?[^()]*?) \+ (0x1b90|0x1dc0) \+ \(longlong\)(DAT_[0-9a-f]+) \* 4\)", rep, src)
src = re.sub(r"\b(DAT_1406e8[0-9a-f]{3})\b", lambda m: "R_" + g[m.group(1)][1].replace("width:", "W_") if m.group(1) in g else m.group(1), src)
open(sys.argv[3], "w").write(src)
