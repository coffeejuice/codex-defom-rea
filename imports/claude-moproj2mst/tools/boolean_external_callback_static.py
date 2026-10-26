"""Verify retained offline SfTreeView callback metadata and capture hashes."""
import hashlib
import json
from doe_static_pe import PE, ROOT


def main():
    path = ROOT/'analysis/wave3-boolean-external-callback-contract.json'
    receipt = json.loads(path.read_text(encoding='utf-8'))
    pe = PE()
    sha = lambda data: hashlib.sha256(data).hexdigest()
    assert sha(pe.data) == receipt['binary_sha256']
    metadata = receipt['metadata']
    base = int(metadata['table'], 0)
    strings = int(metadata['strings'], 0)
    assert sha(pe.data[pe.off(base):pe.off(0x142a6dda8)]) == metadata['bytes_sha256']
    for item in metadata['methods']:
        offset = pe.u32(pe.off(base)+(14+item['local_id']*5)*4)
        address = strings+offset
        at = pe.off(address)
        assert hex(address) == item['signature_va']
        assert pe.data[at:pe.data.find(b'\0', at)].decode() == item['signature']
    for capture in receipt['captures']:
        a, b = int(capture['entry'], 0), int(capture['end_exclusive'], 0)
        assert sha(pe.data[pe.off(a):pe.off(b)]) == capture['code_sha256']
        assert sha((ROOT/capture['artifact']).read_bytes()) == capture['artifact_sha256']
    for path, expected in receipt['evidence_sha256'].items():
        assert sha((ROOT/path).read_bytes()) == expected
    print(json.dumps({'metadata_methods': len(metadata['methods']),
                      'captures_verified': len(receipt['captures']),
                      'evidence_verified': len(receipt['evidence_sha256']),
                      'target_execution': False}))


if __name__ == '__main__':
    main()
