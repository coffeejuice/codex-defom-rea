"""Index which corpus KEY files use each group-2 keyword.

usage: python -I g2_corpus_index.py <keyword_registry.tsv> <corpus_root> <out.tsv>
A keyword occurrence is a line whose first whitespace token equals the keyword
(the reader takes tokens with fscanf "%39s", so keywords start a token; in
practice they start a line). Writes keyword, files, occurrences, smallest example file.
Read-only on the corpus.
"""
import os, sys
from collections import defaultdict

names = set()
for row in open(sys.argv[1], encoding="utf-8").read().split("\n")[1:]:
    f = row.split("\t")
    if len(f) > 3 and f[2] == "2":
        names.add(f[1])
files = defaultdict(set); occ = defaultdict(int)
for dp, _, fns in os.walk(sys.argv[2]):
    for fn in fns:
        if not fn.upper().endswith(".KEY"):
            continue
        p = os.path.join(dp, fn)
        with open(p, "rb") as fh:
            for line in fh:
                t = line.split(None, 1)
                if t:
                    k = t[0].decode("latin-1")
                    if k in names:
                        files[k].add(p); occ[k] += 1
with open(sys.argv[3], "w", encoding="utf-8") as out:
    out.write("keyword\tfiles\toccurrences\tsmallest_example\n")
    for k in sorted(names):
        ex = min(files[k], key=os.path.getsize) if files[k] else ""
        out.write(f"{k}\t{len(files[k])}\t{occ[k]}\t{ex}\n")
