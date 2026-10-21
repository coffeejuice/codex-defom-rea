"""Recheck complete FSTRES branch and helper bytes against original PE."""
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
    for id in [94]:
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
    c=cases[0]; index={int(x['address'],16):x for x in c['instructions']}
    for call in c['calls']:
        ret=int(call['return'],16);call['return_instruction']=index[ret]
        patch=[];n=0
        for x in c['instructions']:
            if int(x['address'],16)>=ret and n<5:patch.append(x);n+=len(bytes.fromhex(x['bytes']))
        call['minimum_five_byte_patch_instructions']=patch
        call['nearby_incoming_branches']=[]
        for branch in dispatch:
            if branch['mnemonic'].startswith('j') and re.fullmatch('[0-9A-F]{16}',branch['operands']) and ret<=int(branch['operands'],16)<ret+n:
                call['nearby_incoming_branches'].append(serial(branch))
    branches=[]
    for x in dispatch:
        if x['mnemonic'].startswith('j') and re.fullmatch('[0-9A-F]{16}',x['operands']):
            dest=int(x['operands'],16)
            if int(c['entry'],16)<=x['address']<int(c['end_exclusive'],16) or int(c['entry'],16)<=dest<int(c['end_exclusive'],16):
                b=x['bytes'];disp=struct.unpack('<i',b[-4:])[0]if len(b)>2 else struct.unpack('<b',b[-1:])[0]
                assert x['address']+len(b)+disp==dest
                branches.append(serial(x))
    assert index[0x14038573c]['mnemonic']=='cvttsd2si' and index[0x140385762]['mnemonic']=='cvttsd2si'
    assert index[0x140385748]['mnemonic']=='add' and index[0x14038576e]['mnemonic']=='imul' and index[0x1403859d5]['bytes']=='f7d9'
    helpers=decode('analysis/fstres-seven-real-dumpbin.txt')
    e=json.loads(Path('analysis/fstres-0x1404151c0.json').read_text());body=e['normalized_result']['procedure']['body']['ranges'];lo=int(body[0]['start'],16);hi=int(body[0]['end'],16)
    helpers=[x for x in helpers if lo<=x['address']<=hi];assert helpers[0]['address']==lo and helpers[-1]['address']+len(helpers[-1]['bytes'])==hi+1
    for a,b in zip(helpers,helpers[1:]):assert a['address']+len(a['bytes'])==b['address']
    reset_code=[x for x in decode('analysis/fstres-table-reset-dumpbin.txt')if x['address']<=0x14012e1c4]
    assert reset_code[0]['address']==0x14012d2a0 and reset_code[-1]['address']==0x14012e1c4
    for a,b in zip(reset_code,reset_code[1:]):assert a['address']+len(a['bytes'])==b['address']
    reset_cases=[]
    for kind in [0x24,0x25,0x26,0x4f,0x50,0x51,0xa6,0xa7,0xa8,0xae]:
        ix=native(0x14012e33c+kind-7,1)[0];entry=base+struct.unpack('<I',native(0x14012e1c8+4*ix,4))[0]
        xs=[]
        if entry!=0x14012e196:
            for x in reset_code:
                if x['address']>=entry:
                    xs.append(x)
                    if x['mnemonic']=='jmp':break
        reset_cases.append({'type':kind,'compressed_selector':ix,'entry':hex(entry),'instructions':[serial(x)for x in xs]})
    report={'status':'PASS_complete_FSTRES_case_PE_bytes','target':bind(target),'jump_table_address':'0x140388cec','cases':cases,'branches':branches,'lifecycle_bodies':verified,'typed_reset':{'source':bind('analysis/configuration-0x14012d2a0.json'),'instruction_count':len(reset_code),'cases':reset_cases,'allocation_tail':[serial(x)for x in reset_code if x['address']>=0x14012e196]},'seven_real_helper':{'source':bind('analysis/fstres-0x1404151c0.json'),'evidence_id':e['evidence_id'],'instructions':[serial(x)for x in helpers]},'arithmetic_instructions':[index[a]for a in [0x14038573c,0x140385748,0x140385762,0x14038576e,0x1403859d5]],'verified_instruction_count':len(dispatch)+sum(len(x['instructions'])for x in verified)+len(helpers)+len(reset_code),'bindings':[bind(p)for p in [__file__,'analysis/property-table-material-dispatch-dumpbin.txt','analysis/material-import-0x140384a50.json','analysis/fstres-seven-real-dumpbin.txt','analysis/fstres-table-reset-dumpbin.txt']+paths],'limitations':['Every direct branch target decoded from bytes; corrupted indirect flow excluded.','A return instruction beginning a short branch is not automatically a safe instrumentation landing site. Patch range and nearby incoming edges are retained for observer review.']}
    Path('analysis/fstres-byte-audit.json').write_text(json.dumps(report,indent=2)+'\n')
    print('PASS',len(c['calls']),'call sites',len(branches),'branches',report['verified_instruction_count'],'instructions')

if __name__=='__main__':main()
