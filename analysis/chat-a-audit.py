import json, hashlib, re, struct
from pathlib import Path
SOURCE=Path(r'C:/Users/alext/Projects/codex-deform-rea')
OUT=Path(r'C:/Users/alext/.codex/worktrees/chat-a-contract/codex-deform-rea/analysis')
raw=(SOURCE/'source/DEF_PRE_64.exe').read_bytes()
assert hashlib.sha256(raw).hexdigest()=='f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
pe=struct.unpack_from('<I',raw,0x3c)[0];opt=pe+24
base=struct.unpack_from('<Q',raw,opt+24)[0]; size=struct.unpack_from('<H',raw,pe+20)[0]
sections=[]
for i in range(struct.unpack_from('<H',raw,pe+6)[0]):
    _,rva,n,offset=struct.unpack_from('<IIII',raw,opt+size+i*40+8);sections.append((base+rva,n,offset))
def native(a,n):
    for va,size,offset in sections:
        if va<=a and a+n<=va+size:return offset+a-va,raw[offset+a-va:offset+a-va+n]
    raise ValueError(hex(a))
def decode(path):
    xs=[]
    for line in path.read_text(encoding='utf-8-sig').splitlines():
        m=re.match(r'^\s+([0-9A-F]{16}): ((?:[0-9A-F]{2} )+)\s*(\S+)(?:\s+(.*))?$',line)
        if m:xs.append(dict(address=int(m[1],16),bytes=bytes.fromhex(m[2]),mnemonic=m[3],operands=m[4]or''))
        elif xs and re.fullmatch(r'\s+(?:[0-9A-F]{2}\s*)+',line):xs[-1]['bytes']+=bytes.fromhex(line)
    for x in xs:
        off,b=native(x['address'],len(x['bytes']));assert b==x['bytes'];x['file_offset']=off
        if x['mnemonic']=='call' and b[0]==0xe8:assert x['address']+5+struct.unpack('<i',b[1:])[0]==int(x['operands'],16)
    return xs
def serial(x):return {**x,'address':hex(x['address']),'bytes':x['bytes'].hex()}
if __name__=='__main__':
    xs=decode(SOURCE/'analysis/property-table-material-dispatch-dumpbin.txt')
    jt=struct.unpack('<42I',native(0x140388cec,168)[1]); start=base+jt[108-92];end=min(base+x for x in jt if base+x>start)
    ys=[x for x in xs if start<=x['address']<end]
    for a,b in zip(ys,ys[1:]):assert a['address']+len(a['bytes'])==b['address']
    result={'target_sha256':hashlib.sha256(raw).hexdigest(),'range':[hex(start),hex(end)],'instructions':[serial(x)for x in ys],'calls':[serial(x)for x in ys if x['mnemonic']=='call'],'source_sha256':hashlib.sha256((SOURCE/'analysis/property-table-material-dispatch-dumpbin.txt').read_bytes()).hexdigest()}
    result['shared_dispatch_prefix']=[serial(x) for x in xs if 0x140384a50<=x['address']<0x140384bbf]
    result['shared_return']=[serial(x) for x in xs if 0x140388ccc<=x['address']<0x140388cec]
    result['branch_checks']=[]
    for x in ys:
        if x['mnemonic'].startswith('j') and re.fullmatch('[0-9A-F]{16}',x['operands']):
            b=x['bytes']; disp=struct.unpack('<i',b[-4:])[0] if len(b)>2 else struct.unpack('<b',b[-1:])[0]
            assert x['address']+len(b)+disp==int(x['operands'],16)
            result['branch_checks'].append(serial(x))
    result['helpers']=[]
    for path in OUT.glob('chat-a-grndat-*-dumpbin.txt'):
        decoded=decode(path)
        result['helpers'].append({'file':path.name,'sha256':hashlib.sha256(path.read_bytes()).hexdigest(),'instructions':[serial(x)for x in decoded]})
    result['assembly_crosschecks']=[]
    evidence=json.loads((OUT/'chat-a-grndat-native-evidence.json').read_text())
    all_instructions={x['address']:x for h in result['helpers'] for x in h['instructions']}
    for e in evidence:
        if e['operation']!='procedure_assembly':continue
        lines=e['normalized_result'].splitlines(); addresses=[line.split(':',1)[0]for line in lines]
        assert all(a in all_instructions for a in addresses), e['parameters']
        chosen=[all_instructions[a] for a in addresses]
        for a,b in zip(chosen,chosen[1:]):assert int(a['address'],16)+len(bytes.fromhex(a['bytes']))==int(b['address'],16)
        result['assembly_crosschecks'].append({'procedure':e['parameters']['procedure'],'evidence_id':e['evidence_id'],'instructions':len(chosen),'range':[addresses[0],addresses[-1]]})
    result['literal_strings']=[]
    for addr in [0x1405661f0,0x1405662ec,0x140566790,0x140566798,0x1405667a0]:
        offset,b=native(addr,100);s=b.split(b'\0',1)[0];result['literal_strings'].append({'address':hex(addr),'file_offset':offset,'bytes':s.hex(),'ascii':s.decode('ascii')})
    assert next(x for x in ys if x['address']==0x1403879b6)['bytes']==bytes.fromhex('f7d9')
    (OUT/'chat-a-grndat-byte-audit.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':'PASS','dispatcher_instructions':len(ys),'branch_checks':len(result['branch_checks']),'helper_instructions':sum(len(h['instructions'])for h in result['helpers']),'assembly_crosschecks':result['assembly_crosschecks'],'literal_strings':result['literal_strings']},indent=2))
