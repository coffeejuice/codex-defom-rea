"""Reproduce native-byte bindings for property-table readers and material dispatch."""
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
    dispatch=decode('analysis/property-table-material-dispatch-dumpbin.txt');readers=decode('analysis/property-table-readers-dumpbin.txt')
    serial=lambda xs:[{**x,'address':hex(x['address']),'bytes':x['bytes'].hex()}for x in xs]
    by={x['address']:x for x in dispatch+readers}
    ids=[95,96,97,98,99,100,115,121,122,125,126,131,133]
    offsets=[0x670,0x6e8,0x760,0x7e0,0x858,0x8d8,0x1cd8,0x1d50,0x1dc8,0x1fe0,0x20d8,0x17f0,0x1878]
    jt=struct.unpack('<42I',native(0x140388cec,42*4));starts=sorted(set(base+x for x in jt));cases=[]
    catalog={x['id']:x['name']for x in json.loads(Path('analysis/keyword-catalog.json').read_text())['rows']}
    for id,offset in zip(ids,offsets):
        entry=base+jt[id-92];end=next((a for a in starts if a>entry),0x140388cc5)
        xs=[x for x in dispatch if entry<=x['address']<end]
        assert xs and xs[0]['address']==entry
        for a,b in zip(xs,xs[1:]):assert a['address']+len(a['bytes'])==b['address']
        call=next(x for x in xs if x['mnemonic']=='call'and x['operands']=='000000014041B270')
        assert any(x['mnemonic']=='add'and x['operands']==f'rdx,{offset:X}h'for x in xs)
        assert by[call['address']+5]['mnemonic']=='mov'and by[call['address']+5]['operands'].endswith(',eax')
        cases.append({'id':id,'name':catalog[id],'case_entry':hex(entry),'table_offset':hex(offset),'call':hex(call['address']),'return':hex(call['address']+5),'call_bytes':call['bytes'].hex(),'instructions':serial(xs)})
    selected=[x for x in readers if 0x14041b270<=x['address']<=0x14041b676]
    for a,b in zip(selected,selected[1:]):assert a['address']+len(a['bytes'])==b['address']
    assert selected[-1]['mnemonic']=='ret'
    load=[x for x in readers if 0x14041a800<=x['address']<=0x14041ac09]
    for a,b in zip(load,load[1:]):assert a['address']+len(a['bytes'])==b['address']
    assert load[-1]['mnemonic']=='ret'
    continuation=[]
    for x in selected+load:
        if x['mnemonic']=='call':continuation.append({'call':hex(x['address']),'callee':x['operands'],'return':hex(x['address']+len(x['bytes'])),'bytes':x['bytes'].hex()})
    raw_sources=[]
    for p in Path('analysis').glob('property-table-bytes-*.json'):
        e=json.loads(p.read_text());r=e['normalized_result'];payload=bytes.fromhex(r['bytes_hex'])
        assert r['complete']and r['returned_bytes']==r['requested_bytes']==len(payload)
        assert native(int(r['address'],16),len(payload))==payload
        raw_sources.append({**bind(p),'evidence_id':e['evidence_id'],'bytes':len(payload)})
    ghidra=json.loads(Path('analysis/property-table-41b270-instructions.json').read_text())['normalized_result']['instructions']
    ga={int(x.split(':',1)[0],16)for x in ghidra};assert all(x['address']in ga for x in selected)
    result={'status':'PASS','target':bind(target),'cases':cases,'reader_continuations':continuation,'header_jump_table':[hex(base+x)for x in struct.unpack('<8I',native(0x14041b678,32))],'header_format_bytes':native(0x140566758,17).hex(),'verified_dumpbin_instructions':len(dispatch)+len(readers),'header_instructions':serial(selected),'payload_instructions':serial(load),'material_prelude':serial([x for x in dispatch if 0x140384a50<=x['address']<0x140384bbf]),'material_epilogue':serial([x for x in dispatch if 0x140388cc5<=x['address']<0x140388cec]),'raw_REA_source_matches':raw_sources,'bindings':[bind(x)for x in [__file__,'analysis/property-table-material-dispatch-dumpbin.txt','analysis/property-table-readers-dumpbin.txt','analysis/property-table-41b270-instructions.json']]}
    Path('analysis/property-table-byte-audit.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],len(cases),'cases',result['verified_dumpbin_instructions'],'instructions')


if __name__=='__main__':main()
