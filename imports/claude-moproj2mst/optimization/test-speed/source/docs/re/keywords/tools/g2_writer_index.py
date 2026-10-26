"""Index, per keyword, the decompiled functions that print its name.

usage: python -I g2_writer_index.py <keyword_registry.tsv> <decompiled.c>...
A function "prints" keyword id N when it calls FUN_1402f7720(N) (keyword-name lookup)
or when it is the keyword's registry write handler (column handler2). Also lists the
type codes passed to the generic array writer FUN_1402a4a80(model, file, obj, code).
"""
import re, sys
from collections import defaultdict

names, h2 = {}, {}
for row in open(sys.argv[1], encoding="utf-8").read().split("\n")[1:]:
    f = row.split("\t")
    if len(f) > 7:
        names[int(f[0])] = f[1]
        if f[6] != "0x1402f75d0":
            h2[f[6].replace("0x", "")] = int(f[0])
funcs = {}
for p in sys.argv[2:]:
    t = open(p, encoding="utf-8", errors="replace").read()
    for m in re.finditer(r"^// ===== (\S+) @ ([0-9a-f]+) .*?$(.*?)(?=^// ===== |\Z)", t, re.S | re.M):
        funcs.setdefault(m.group(2), m.group(3))
by_kw = defaultdict(set)
arr_codes = defaultdict(set)
for a, body in funcs.items():
    for kid in re.findall(r"FUN_1402f7720\((0x[0-9a-f]+|\d+)\)", body):
        by_kw[int(kid, 0)].add("FUN_" + a)
    if a in h2:
        by_kw[h2[a]].add("FUN_" + a + " (handler2)")
    for c in re.findall(r"FUN_1402a4a80\([^;]*?,(0x[0-9a-f]+|\d+)\);", body):
        arr_codes["FUN_" + a].add(int(c, 0))
for kid in sorted(by_kw):
    print(f"{kid}\t{names.get(kid, '?')}\t{' '.join(sorted(by_kw[kid]))}")
print()
for a in sorted(arr_codes):
    print(f"array-writer codes from {a}: {' '.join(hex(c) for c in sorted(arr_codes[a]))}")
