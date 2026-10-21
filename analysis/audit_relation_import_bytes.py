"""Reproduce native-byte bindings for PHASTF and its root refresh."""
import hashlib
import json
from pathlib import Path
import re
import struct


def bind(path):
    return {'path':str(path).replace('\\','/'),'sha256':hashlib.sha256(Path(path).read_bytes()).hexdigest()}


def main():
    target=Path('source/DEF_PRE_64.exe');data=target.read_bytes();pe=struct.unpack_from('<I',data,0x3c)[0]
    assert data[pe:pe+4]==b'PE\0\0'
    n=struct.unpack_from('<H',data,pe+6)[0];optional=pe+24;size=struct.unpack_from('<H',data,pe+20)[0]
    assert struct.unpack_from('<H',data,optional)[0]==0x20b
    base=struct.unpack_from('<Q',data,optional+24)[0];sections=[]
    for i in range(n):
        _,rva,length,raw=struct.unpack_from('<IIII',data,optional+size+i*40+8);sections.append((base+rva,length,raw))
    def native(address,length):
        for start,n,raw in sections:
            if start<=address and address+length<=start+n:return data[raw+address-start:raw+address-start+length]
        raise AssertionError('unmapped source range')
    def decode(path):
        instructions=[]
        for line in Path(path).read_text(encoding='utf-8-sig').splitlines():
            m=re.match(r'^\s+([0-9A-F]{16}): ((?:[0-9A-F]{2} )+)\s*(\S+)(?:\s+(.*))?$',line)
            if m:instructions.append({'address':int(m[1],16),'bytes':bytes.fromhex(m[2]),'mnemonic':m[3],'operands':m[4]or''})
            elif instructions and re.fullmatch(r'\s+(?:[0-9A-F]{2}\s*)+',line):instructions[-1]['bytes']+=bytes.fromhex(line)
        for item in instructions:
            assert native(item['address'],len(item['bytes']))==item['bytes'],hex(item['address'])
            if item['mnemonic']=='call'and item['bytes'][0]==0xe8:
                assert item['address']+5+struct.unpack('<i',item['bytes'][1:])[0]==int(item['operands'],16)
        return instructions
    dispatch=decode('analysis/relation-import-dispatch-dumpbin.txt');refresh=decode('analysis/relation-import-refresh-dumpbin.txt')
    def serialized(xs):return [{**x,'address':hex(x['address']),'bytes':x['bytes'].hex()}for x in xs]
    selected=[i for i in dispatch if 0x140383120<=i['address']<0x1403832b3]
    for a,b in zip(selected,selected[1:]):assert a['address']+len(a['bytes'])==b['address']
    assert selected[-1]['address']+len(selected[-1]['bytes'])==0x1403832b3
    assert base+struct.unpack('<I',native(0x140384a28,4))[0]==0x140383237
    byaddress={i['address']:i for i in selected}
    continuations=[]
    for a,callee in [(0x140383164,0x140484924),(0x140383196,0x14033a040),(0x1403831bd,0x140339df0),(0x14038324a,0x140413de0),(0x14038329a,0x1401d6450),(0x1403832a9,0x1401d0fc0)]:
        x=byaddress[a];assert x['mnemonic']=='call'and int(x['operands'],16)==callee
        continuations.append({'call':hex(a),'callee':hex(callee),'return':hex(a+len(x['bytes'])),'call_bytes':x['bytes'].hex(),'next_instruction':serialized([byaddress[a+len(x['bytes'])]])[0]})
    raw_sources=[]
    for p in Path('analysis').glob('relation-import-bytes-*.json'):
        e=json.loads(p.read_text());r=e['normalized_result'];payload=bytes.fromhex(r['bytes_hex'])
        assert r['complete']and r['returned_bytes']==r['requested_bytes']==len(payload)
        assert native(int(r['address'],16),len(payload))==payload
        raw_sources.append({**bind(p),'evidence_id':e['evidence_id'],'address':r['address'],'bytes':len(payload)})
    ghidra=json.loads(Path('analysis/relation-import-383120-instructions.json').read_text())['normalized_result']['instructions']
    ghidra_addresses={int(x.split(':',1)[0],16)for x in ghidra}
    assert all(i['address']in ghidra_addresses for i in selected)
    result={'status':'PASS','target':bind(target),'tool':bind('C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe'),
            'verified_dispatch_instructions':len(dispatch),'verified_refresh_instructions':len(refresh),'selected_shared_and_PHASTF_instructions':len(selected),
            'jump_table':{'entry':'0x140384a28','bytes':native(0x140384a28,4).hex(),'index':0,'case_entry':'0x140383237','keyword_id':134},
            'continuations':continuations,'selected_instructions':serialized(selected),'epilogue_instructions':serialized([i for i in dispatch if 0x140384a07<=i['address']<=0x140384a25]),
            'refresh_instructions':serialized(refresh),'raw_REA_source_matches':raw_sources,
            'bindings':[bind(p)for p in ['analysis/relation-import-dispatch-dumpbin.txt','analysis/relation-import-refresh-dumpbin.txt','analysis/relation-import-383120-instructions.json','analysis/root-layout-0x1401d0fc0.json',__file__]],
            'limitation':'Independent PE byte identity and call-target validation; semantic claims additionally require retained dossiers and branch analysis. No original execution.'}
    Path('analysis/relation-import-byte-audit.json').write_text(json.dumps(result,indent=2)+'\n')
    print({k:result[k]for k in ['status','verified_dispatch_instructions','verified_refresh_instructions','selected_shared_and_PHASTF_instructions']})


if __name__=='__main__':main()
