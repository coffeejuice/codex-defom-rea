"""List which header variants of group 4/5/7 keywords occur in KEY files (corpus coverage).

Usage: python -I kwvariants457.py <registry.tsv> <dir or file>...
Prints, per keyword, the distinct values of the first field after the object number(s) on the
keyword line (the "type" selector for most keywords) with counts and one example file.
"""
import os
import sys
from collections import defaultdict

reg = {}
for row in open(sys.argv[1]).read().splitlines()[1:]:
    c = row.split('\t')
    reg[c[1]] = (int(c[0]), int(c[2]))
paths = []
for a in sys.argv[2:]:
    if os.path.isdir(a):
        for dp, _, fs in os.walk(a):
            paths += [os.path.join(dp, x) for x in fs if x.upper().endswith('.KEY')]
    else:
        paths.append(a)
var = defaultdict(lambda: defaultdict(lambda: [0, None]))
for p in sorted(paths):
    for line in open(p, encoding='latin-1', newline='').read().replace('\r\n', '\n').split('\n'):
        w = line.replace(',', ' ').split()
        if not w or w[0] not in reg or reg[w[0]][1] not in (4, 5, 7):
            continue
        skip = {4: 2, 5: 3, 7: 1}[reg[w[0]][1]]
        sel = w[skip] if len(w) > skip else '-'
        e = var[w[0]][sel]
        e[0] += 1
        e[1] = e[1] or os.path.relpath(p, os.path.dirname(os.path.dirname(p.rstrip(os.sep))))
for kw in sorted(var, key=lambda k: reg[k][0]):
    print('%-7s' % kw, '  '.join('%s:%d' % (s, n) for s, (n, _) in sorted(var[kw].items())))
    for s, (n, ex) in sorted(var[kw].items()):
        print('          %s e.g. %s' % (s, ex))
