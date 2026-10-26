"""Oracle cases of packet p4-07: DEF_PRE reads one corpus KEY file and writes it with GENDB.

    python -I make_cases.py CORPUS_ROOT OUT.json

Every KEY file under CORPUS_ROOT/key smaller than 3 MB that has a STRESS record becomes the case
"p4-07/gendb/<path>" with CMD.KEY = KFREAD / <file> / GENDB / NEW.DB. Run the cases with
tools/oracle/def_pre_oracle.py cases OUT.json; test_step_header_oracle reads the cache.
"""
import json
import os
import sys

root, out = sys.argv[1], sys.argv[2]
cases = []
for d, _, files in os.walk(os.path.join(root, 'key')):
    for f in sorted(files):
        if not f.upper().endswith('.KEY'):
            continue
        p = os.path.join(d, f)
        if os.path.getsize(p) >= 3 * 1024 * 1024:
            continue
        with open(p, 'rb') as fh:
            if b'\nSTRESS' not in fh.read():
                continue
        rel = os.path.relpath(p, root).replace(os.sep, '/')
        cases.append({
            'label': 'p4-07/gendb/' + rel,
            'stdin': '\n2\n1\nCMD.KEY\n\nE\nE\nY\n',
            'timeout': 300,
            'inputs': [{'name': 'IN.KEY', 'source': p},
                       {'name': 'CMD.KEY', 'text': 'KFREAD\nIN.KEY\nGENDB\nNEW.DB\n'}],
        })
with open(out, 'w') as fh:
    json.dump(cases, fh, indent=1)
print(len(cases), 'cases')
