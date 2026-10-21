"""Bind all relation-property dispatcher cases and probe continuations to PE bytes."""
import hashlib
import json
from pathlib import Path
import re
import struct


def bind(p):return {'path':str(p).replace('\\','/'),'sha256':hashlib.sha256(Path(p).read_bytes()).hexdigest()}


def main():
    data=Path('source/DEF_PRE_64.exe').read_bytes();pe=struct.unpack_from('<I',data,0x3c)[0]
    base=struct.unpack_from('<Q',data,pe+48)[0];sec=pe+24+struct.unpack_from('<H',data,pe+20)[0]
    sections=[struct.unpack_from('<IIII',data,sec+i*40+8) for i in range(struct.unpack_from('<H',data,pe+6)[0])]
    def native(a,n):
        for _,rva,length,raw in sections:
            if base+rva<=a and a+n<=base+rva+length:return data[raw+a-base-rva:raw+a-base-rva+n]
        raise AssertionError('unmapped bytes')
    def decode(p):
        xs=[]
        for line in Path(p).read_text(encoding='utf-8-sig').splitlines():
            m=re.match(r'^\s+([0-9A-F]{16}): ((?:[0-9A-F]{2} )+)\s*(\S+)(?:\s+(.*))?$',line)
            if m:xs.append({'address':int(m[1],16),'bytes':bytes.fromhex(m[2]),'mnemonic':m[3],'operands':m[4]or''})
            elif xs and re.fullmatch(r'\s+(?:[0-9A-F]{2}\s*)+',line):xs[-1]['bytes']+=bytes.fromhex(line)
        for x in xs:
            assert native(x['address'],len(x['bytes']))==x['bytes'],hex(x['address'])
            if x['mnemonic']=='call' and x['bytes'][0]==0xe8:
                assert x['address']+5+struct.unpack('<i',x['bytes'][1:])[0]==int(x['operands'],16)
        return xs
    paths=['analysis/relation-property-'+n+'-dumpbin.txt' for n in ['dispatch','constructor','real-helpers','mixed-helper','abs']]
    groups=[decode(p) for p in paths];dispatch=[x for x in groups[0] if base+0x383120<=x['address']<=base+0x384a25]
    for a,b in zip(dispatch,dispatch[1:]):assert a['address']+len(a['bytes'])==b['address']
    assert dispatch[-1]['mnemonic']=='ret'
    jt=[base+x for x in struct.unpack('<10I',native(base+0x384a28,40))]
    catalog={r['id']:r['name'] for r in json.loads(Path('analysis/keyword-catalog.json').read_text())['rows']}
    serial=lambda x:{**x,'address':hex(x['address']),'bytes':x['bytes'].hex()}
    cases=[]
    for i,entry in enumerate(jt):
        end=jt[i+1] if i<9 else base+0x384a00
        xs=[x for x in dispatch if entry<=x['address']<end]
        assert xs and xs[0]['address']==entry
        calls=[]
        for j,x in enumerate(xs):
            if x['mnemonic']!='call':continue
            ret=x['address']+len(x['bytes']);incoming=[]
            for branch in dispatch:
                if branch['mnemonic'].startswith('j') and re.fullmatch('[0-9A-F]{16}',branch['operands']) and int(branch['operands'],16)==ret:
                    raw=branch['bytes'];off=struct.unpack('<i',raw[-4:])[0] if len(raw)>2 else struct.unpack('<b',raw[-1:])[0]
                    assert branch['address']+len(raw)+off==ret
                    incoming.append(serial(branch))
            assert ret not in jt
            calls.append({'call':hex(x['address']),'callee':hex(int(x['operands'],16)),'return':hex(ret),'bytes':x['bytes'].hex(),'incoming_direct_branches':incoming,'preceding_instructions':[serial(y) for y in xs[max(0,j-14):j]]})
        cases.append({'id':134+i,'name':catalog[134+i],'entry':hex(entry),'end_exclusive':hex(end),'calls':calls,'instructions':[serial(x) for x in xs]})
    ghidra=json.loads(Path('analysis/volume-width-0x140383120.json').read_text())['normalized_result']['assembly']
    addresses={int(s.split(':',1)[0],16) for s in ghidra}
    assert all(x['address'] in addresses for x in dispatch)
    selected=[x for x in groups[-1] if base+0x484ab8<=x['address']<=base+0x484ac1]
    assert b''.join(x['bytes'] for x in selected).hex()=='8bc1f7d885c90f49c1c3'
    result={'status':'PASS_all_nine_handlers_and_shared_selector_byte_bound','target':bind('source/DEF_PRE_64.exe'),'verified_instruction_count':sum(map(len,groups)),
        'dispatcher_instruction_count':len(dispatch),'jump_table_address':'0x140384a28','jump_table_bytes':native(base+0x384a28,40).hex(),'jump_targets':[hex(x) for x in jt],
        'shared_selector_and_append':[serial(x) for x in dispatch if x['address']<jt[0]],'cases':cases,'epilogue':[serial(x) for x in dispatch if x['address']>=base+0x384a00],
        'absolute_value_32bit':[serial(x) for x in selected],'constructor_instructions':[serial(x) for x in groups[1] if x['address']<=base+0x334487],
        'helper_instructions':[[serial(x) for x in xs] for xs in groups[2:4]],
        'limitations':['Incoming direct branches and selected dispatch jump-table entries checked for every call continuation. No universal assertion about arbitrary corrupted indirect control flow.','Ghidra pseudocode is non-source; all listed dumpbin instruction bytes independently match the immutable PE. No target execution.'],
        'bindings':[bind(p) for p in [__file__,'analysis/volume-width-0x140383120.json','analysis/volume-width-0x140333f60.json']+paths]}
    out=Path('analysis/relation-property-byte-audit.json');out.write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],result['verified_instruction_count'],'instructions')
    for c in cases[1:]:print(c['id'],c['name'],[(x['callee'],x['return'],len(x['incoming_direct_branches'])) for x in c['calls']])


if __name__=='__main__':main()
