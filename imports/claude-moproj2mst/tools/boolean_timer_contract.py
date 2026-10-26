"""Verify immutable Boolean timer evidence against the fixed shipped PEs; no runtime."""
import hashlib
import json
from doe_static_pe import PE, ROOT

def sha(data): return hashlib.sha256(data).hexdigest()

def main():
    doc = json.loads((ROOT/'analysis/wave3-boolean-timer-contract.json').read_text(encoding='utf-8'))
    pes = {name: PE(ROOT/'evidence/original-binaries'/name) for name in doc['binary_sha256']}
    for name, expected in doc['binary_sha256'].items(): assert sha(pes[name].data) == expected
    for name, expected in doc['evidence_sha256'].items(): assert sha((ROOT/name).read_bytes()) == expected
    for c in doc['code_regions']:
        p = pes[c['binary']]; a, b = int(c['begin'], 0), int(c['end_exclusive'], 0)
        assert sha(p.data[p.off(a):p.off(b)]) == c['sha256']
    p = pes['DEF_GUI_MO_64.exe']
    for m in doc['qt_metadata']:
        at = p.off(int(m['table'], 0))+(14+m['local_id']*5)*4
        address = int(m['strings'], 0)+p.u32(at)
        assert address == int(m['signature_va'], 0)
        b = p.off(address); assert p.data[b:p.data.find(b'\0', b)].decode() == m['signature']
    for v in doc['vtable_slots']:
        p = pes[v['binary']]
        assert p.u64(p.off(int(v['table'], 0)+int(v['offset'], 0))) == int(v['target'], 0)
    print(json.dumps({'code_regions':len(doc['code_regions']), 'evidence_hashes':len(doc['evidence_sha256']),
                      'metadata_methods':len(doc['qt_metadata']), 'vtable_slots':len(doc['vtable_slots']),
                      'target_runtime':False, 'source_edits':False}))
if __name__ == '__main__': main()
