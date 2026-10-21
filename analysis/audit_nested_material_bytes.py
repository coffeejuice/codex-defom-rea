"""Bind selected nested material branches, constructors and observer joins to PE."""
import hashlib,json,re,struct
from pathlib import Path

def bind(path):return {'path':str(path).replace('\\','/'),'sha256':hashlib.sha256(Path(path).read_bytes()).hexdigest()}

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

    dispatch=decode('analysis/property-table-material-dispatch-dumpbin.txt')
    jt=struct.unpack('<42I',native(0x140388cec,42*4));starts=sorted(set(base+x for x in jt))
    serial=lambda x:{**x,'address':hex(x['address']),'bytes':x['bytes'].hex()}
    cases=[]
    for id in [94,108]:
        entry=base+jt[id-92];end=next(a for a in starts if a>entry);xs=[x for x in dispatch if entry<=x['address']<end]
        for a,b in zip(xs,xs[1:]):assert a['address']+len(a['bytes'])==b['address']
        calls=[]
        for j,x in enumerate(xs):
            if x['mnemonic']!='call':continue
            ret=x['address']+len(x['bytes']);incoming=[]
            for branch in dispatch:
                if branch['mnemonic'].startswith('j') and re.fullmatch('[0-9A-F]{16}',branch['operands']) and int(branch['operands'],16)==ret:
                    raw=branch['bytes'];off=struct.unpack('<i',raw[-4:])[0]if len(raw)>2 else struct.unpack('<b',raw[-1:])[0]
                    assert branch['address']+len(raw)+off==ret;incoming.append(serial(branch))
            calls.append({'call':hex(x['address']),'callee':hex(int(x['operands'],16)),'return':hex(ret),'bytes':x['bytes'].hex(),'incoming_direct_branches':incoming,'preceding_instructions':[serial(y)for y in xs[max(0,j-16):j]]})
        cases.append({'id':id,'entry':hex(entry),'end_exclusive':hex(end),'calls':calls,'instructions':[serial(x)for x in xs]})
    paths=['analysis/nested-material-'+n+'-dumpbin.txt'for n in ['fstres-constructor','grndat-table','fstres-destructor','grndat-container']]
    bodies={0x288ae0:(0x288f70,'analysis/nested-material-0x140288ae0.json'),0x28a3e0:(0x28a416,'analysis/nested-material-0x14028a3e0.json'),0x28a420:(0x28a437,'analysis/nested-material-0x14028a420.json'),0x29cdc0:(0x29ce86,'analysis/nested-material-0x14029cdc0.json'),0x28b850:(0x28b8d7,'analysis/material-import-0x14028b850.json'),0x28b8e0:(0x28b936,'analysis/nested-material-0x14028b8e0.json')}
    aux=[x for p in paths for x in decode(p)];verified=[]
    for start,(end,p)in bodies.items():
        e=json.loads(Path(p).read_text());body=e['normalized_result']['procedure']['body']['ranges'];start=int(body[0]['start'],16);end=int(body[0]['end'],16)
        xs=[x for x in aux if start<=x['address']<=end];assert xs[0]['address']==start and xs[-1]['address']+len(xs[-1]['bytes'])==end+1
        for a,b in zip(xs,xs[1:]):assert a['address']+len(a['bytes'])==b['address']
        addresses={int(x.split(':',1)[0],16)for x in e['normalized_result']['assembly']};assert all(x['address']in addresses for x in xs)
        verified.append({'function':hex(start),'evidence_id':e['evidence_id'],'source':bind(p),'instructions':[serial(x)for x in xs]})
    selected=[]
    for c in cases:
        generic=[x for x in c['calls']if x['callee']=='0x14041b270'];assert len(generic)==1
        table=generic[0]
        selected.append({'id':c['id'],'table':table,'outer_header':c['calls'][0],**({'coefficients':next(x for x in c['calls']if x['callee']=='0x140414e40'and int(x['call'],16)>0x140385800)}if c['id']==94 else {'post_refresh':c['calls'][-1]})})
    report={'status':'PASS_native_PE_bytes_and_selected_nested_import_calls','target':bind(target),'jump_table_address':'0x140388cec','selected':selected,'cases':cases,'lifecycle_bodies':verified,'verified_instruction_count':len(dispatch)+sum(len(x['instructions'])for x in verified),'bindings':[bind(p)for p in [__file__,'analysis/property-table-material-dispatch-dumpbin.txt','analysis/material-import-0x140384a50.json']+paths],'limitations':['Direct incoming branch targets and actual native call bytes checked; arbitrary corrupted indirect flow excluded.','Short branch return sites must not be instrumented as independent completion points without evaluating patch overlap/relocation; selected current header/coefficient/table returns start MOV stores.']}
    Path('analysis/nested-material-byte-audit.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(selected,indent=2))

if __name__=='__main__':main()
