"""Build oracle cases (tools/oracle on branch claude/oracle-runner) for group 4/5/7 KEY round trips.

Each case feeds DEF_PRE_64 a corpus KEY file as the menu-2 keyword file with "KFWRIT OUT.KEY"
appended, so the original reads every keyword and writes them back. Picks the material library
KEYs plus, from the example KEYs, one file per (keyword, header-type) variant (SI first).

Usage:
  python -I oracle_cases457.py <registry.tsv> <DEFORM_corpus dir> <out dir>
  python -I <oracle>/def_pre_oracle.py --copy-to <res dir> -j 4 cases <out dir>/kw457_rw.json

Writes kw457_rw.json (cases; contains copies of the corpus texts, keep it out of git) and
kw457_variants.json (variant -> corpus file).
"""
import json
import os
import sys

FREE = ('FRAE2H', 'FPERV', 'MATDEN', 'LTHEAT', 'TMPSOL', 'TMPLIQ', 'GBMOBI', 'MTNAME')


def main(reg_path, corpus, out):
    reg = {}
    for row in open(reg_path).read().splitlines()[1:]:
        c = row.split('\t')
        reg[c[1]] = (int(c[0]), int(c[2]))
    paths = []
    for dp, _, fs in os.walk(os.path.join(corpus, 'key')):
        paths += [os.path.join(dp, x).replace(os.sep, '/') for x in fs if x.upper().endswith('.KEY')]
    paths.sort(key=lambda p: ('/SI/' not in p, p))
    pick = {}
    for p in paths:
        for line in open(p, encoding='latin-1', newline='').read().replace('\r\n', '\n').split('\n'):
            w = line.replace(',', ' ').split()
            if not w or w[0] not in reg or reg[w[0]][1] not in (4, 5, 7):
                continue
            skip = {4: 2, 5: 3, 7: 1}[reg[w[0]][1]]
            sel = '*' if w[0] in FREE else (w[skip] if len(w) > skip else '-')
            pick.setdefault((w[0], sel), p)
    mats = sorted(os.path.join(corpus, 'materials', x).replace(os.sep, '/')
                  for x in os.listdir(os.path.join(corpus, 'materials')) if x.upper().endswith('.KEY'))
    cases = []
    for p in mats + sorted(set(pick.values())):
        txt = open(p, encoding='latin-1', newline='').read()
        if not txt.endswith('\n'):
            txt += '\n'
        rel = os.path.relpath(p, corpus).replace(os.sep, '/')
        cases.append({'label': 'kw457 rw ' + rel, 'stdin': '\n2\n1\nCMD.KEY\n\nE\nE\nY\n',
                      'timeout': 300,
                      'inputs': [{'name': 'CMD.KEY', 'text': txt + 'KFWRIT\nOUT.KEY\n'}]})
    os.makedirs(out, exist_ok=True)
    json.dump(cases, open(os.path.join(out, 'kw457_rw.json'), 'w'), indent=1)
    json.dump({'%s %s' % k: os.path.relpath(v, corpus).replace(os.sep, '/')
               for k, v in sorted(pick.items())},
              open(os.path.join(out, 'kw457_variants.json'), 'w'), indent=1)
    print(len(cases), 'cases,', len(pick), 'variants')


if __name__ == '__main__':
    main(*sys.argv[1:4])
