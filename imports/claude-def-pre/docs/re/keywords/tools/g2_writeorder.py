"""List, in source order, what a decompiled KEY writer function emits.

usage: python -I g2_writeorder.py <decompiled.c> <func_addr> <keyword_registry.tsv>
For each line of the function that names a keyword (FUN_1402f7720(id)), calls a
registry write handler (column handler2) or calls another FUN_1402xxxxx writer,
prints: line, kind, keyword/function.
"""
import re, sys

text = open(sys.argv[1], encoding="utf-8", errors="replace").read()
addr = sys.argv[2].lower().replace("0x", "")
reg = {}
names = {}
for row in open(sys.argv[3], encoding="utf-8").read().split("\n")[1:]:
    f = row.split("\t")
    if len(f) > 7:
        names[int(f[0])] = f[1]
        h2 = f[6].replace("0x", "")
        if h2 not in ("1402f75d0",):
            reg.setdefault(h2, []).append(f[1])
m = re.search(r"^// ===== \S+ @ " + addr + r" .*?$(.*?)(?=^// ===== |\Z)", text, re.S | re.M)
body = m.group(1).split("\n")
for i, l in enumerate(body, 1):
    for kid in re.findall(r"FUN_1402f7720\((0x[0-9a-f]+|\d+)\)", l):
        print(f"{i}\tname\t{names.get(int(kid, 0), '?')} ({int(kid, 0)})")
    for fn in re.findall(r"FUN_(1402[0-9a-f]{5})\(", l):
        if fn in reg:
            print(f"{i}\thandler\t{'/'.join(reg[fn])} (FUN_{fn})")
        elif fn.startswith("1402a") or fn.startswith("1402b"):
            print(f"{i}\tcall\tFUN_{fn}")
    for fn in re.findall(r"FUN_(1403[c-d][0-9a-f]{4})\(", l):
        print(f"{i}\tcall\tFUN_{fn}")
