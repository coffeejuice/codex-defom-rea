"""Prepare source-planned object direct imports before native observation."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import struct

IDS={'AVGSTR':146,'LMTSTR':147,'ZSTR':148,'PENVOL':149,'REFTMP':150}
INDICES={146:1,147:0,149:2,150:3}


def hx(v):return b''.join(struct.pack('<d',x)for x in v).hex()


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('cohort',choices=['prototype','training','evaluation']);a=ap.parse_args();evaluation=a.cohort=='evaluation';delta=1 if evaluation else 0
    base=Path('tests/fixtures/object-direct')/a.cohort;base.mkdir(parents=True,exist_ok=True);dest=Path('analysis/object-direct-'+a.cohort+'-inputs.json')
    if dest.exists():raise SystemExit('Refusing to replace prepared inputs')
    dimension=3;mode=0;nstep=1;records={};mid=3+delta
    plan=dict(status='source_planned_before_native',cohort=a.cohort,ordered_imports=[],inputs=[],qualification='Five complete direct-import cases in normal owned2D/3D wrappers,positive bounded object identities and finite signed32 planar mode conversion. 3D planar fields are uninitialized and neither read nor fabricated. General numeric domains,allocation failure,arbitrary aliases/native ABI and scientific consumers remain pending.')
    if evaluation:plan['chronology']='Prepared after training PASS; original executes before C++ with changed values/IDs and reversed dimension/keyword order.'

    def create(oid):
        if oid<1:return
        for i in range(len(records)+1,oid+1):records[i]=dict(values=[.01,1.,1000000.,20. if mode==0 else 68.],planar=dict(penvol_mode_0x1f20=1,zstr_mode_0x1f30=0,zstr_0x1f28_hex=hx([0.]))if dimension==2 else None)

    def state():return {str(i):dict(scalars_0x1ea0_hex=hx(v['values']),planar=copy.deepcopy(v['planar']))for i,v in records.items()}

    def op(k,values=(0.,),integer=0,oid=1,header=None,status=0,selector_eof=False):return dict(k=k,values=list(values),integer=integer,oid=oid,header=header,status=status,selector_eof=selector_eof)

    def apply(o):
        kid=IDS[o['k']];oid=o['oid'];headers=[];reads=[];selections=[];order=[]
        if o['selector_eof']:return o['k'],dict(id=kid,status=2,headers=[],reads=[],refreshes=[],selections=[],order=[],expected_direct=state())
        header=o['header']
        if header is None:header=' '.join(map(str,([o['integer']]if kid==148 else[])+o['values']))
        data=o['k']+' '+str(oid)+(''if header=='<EOF>'else(' '+header if header else'')+'\n')
        before_count=len(records);create(oid);selections.append(dict(id=oid,before_count=before_count,after_count=len(records),result_id=oid if oid>0 else None));order.append('selection')
        status=2 if oid<1 else 0
        if oid>0 and not(kid==148 and dimension==3):
            f=records[oid];kind='intreal'if kid==148 else'pair'if kid==149 and dimension==2 else'real';ints=[f['planar']['zstr_mode_0x1f30']]if kind=='intreal'else[]
            values=[struct.unpack('<d',bytes.fromhex(f['planar']['zstr_0x1f28_hex']))[0]]if kind=='intreal'else[f['values'][INDICES[kid]]]+([0.]if kind=='pair'else[])
            before=dict(integers=ints,reals_hex=[hx([v])for v in values])
            after=dict(integers=[o['integer']]if kind=='intreal'else[],reals_hex=[hx([v])for v in o['values']])
            headers.append(dict(kind=kind,object_id=oid,before=before,after=after,status=o['status']))
            reads.append(dict(capacity=1024,value=None if header=='<EOF>'else(' '+header if header else'')+'\n'));order.extend(['header-enter','fgets','header-leave']);status=2 if o['status']else 0
            if kid==148:f['planar']['zstr_mode_0x1f30']=o['integer'];f['planar']['zstr_0x1f28_hex']=hx([o['values'][0]])
            else:
                f['values'][INDICES[kid]]=o['values'][0]
                if kind=='pair':f['planar']['penvol_mode_0x1f20']=int(o['values'][1])
        return data,dict(id=kid,status=status,headers=headers,reads=reads,refreshes=[],selections=selections,order=order,expected_direct=state())

    def put(label,ops=(),prefix='',switch=None,unit=None,new_object=None,guard=None):
        nonlocal dimension,mode,nstep,records
        if switch is not None and switch!=dimension:dimension=switch;nstep=1;records={}
        if unit is not None:mode=0 if unit==1 else 1
        if new_object is not None:create(new_object)
        data=prefix;handlers=[]
        for o in ops:
            text,h=apply(o);data+=text;handlers.append(h)
        if guard is not None:data+=f'NSTEP {guard}\n';nstep=guard
        b=data.encode('ascii');name=f'{len(plan["inputs"]):02d}_{label}.KEY';(base/name).write_bytes(b)
        plan['ordered_imports'].append(name);plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(b).hexdigest(),expected_handlers=handlers,expected_direct=state(),expected_nstep=nstep,expected_wrapper_status=next((h['status']for h in reversed(handlers)if h['status']),0)))

    def zero_values(k):return [0.,0.]if k=='PENVOL'and dimension==2 else[0.]

    mesh3=Path('tests/fixtures/custom_fields/01_spatial_mesh.KEY').read_text().split('\n',1)[1];mesh2='OBJTYP 1 2 0\nRZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1 4\n1 1 2 3 4\n'
    keys=list(reversed(IDS))if evaluation else list(IDS)
    for dim in ([2,3]if evaluation else[3,2]):
        put(f'd{dim}_reset',prefix=f'* DEFORM-{dim}D\nUNIT 2\nUNIT 1\n'+(mesh3 if dim==3 else mesh2),switch=dim,unit=1,new_object=1)
        positive=[]
        for oid in [1,mid]:
            for i,k in enumerate(keys):positive.append(op(k,[1.25+i+delta,3.75+delta]if k=='PENVOL'and dim==2 else[1.25+i+delta],integer=7+delta,oid=oid))
        put(f'd{dim}_positive',positive,guard=181+delta)
        lexical=[]
        for k in keys:
            values=[-0.,-2.75]if k=='PENVOL'and dim==2 else[-0.];header='0x7,-0.0 tail'if k=='ZSTR'else'-0.0,-2.75 tail'if len(values)==2 else'-0.0 trailing'
            lexical.append(op(k,values,integer=7,header=header,oid=mid))
        put(f'd{dim}_lexical',lexical,guard=183+delta)
        put(f'd{dim}_partial',[op(k,[2.5,0.]if k=='PENVOL'and dim==2 else[0.]if k=='ZSTR'else[2.5],integer=9,header='9'if k=='ZSTR'else'2.5',oid=1)for k in keys],guard=185+delta)
        put(f'd{dim}_matching',[op(k,zero_values(k),header='bad',oid=mid)for k in keys],guard=187+delta)
        put(f'd{dim}_blank',[op(k,zero_values(k),header='',status=1,oid=1)for k in keys],guard=189+delta)
        put(f'd{dim}_unit_new',[op('AVGSTR',[17.5+delta],oid=5)],prefix='UNIT 2\n',unit=2,guard=191+delta)
        put(f'd{dim}_deactivate',[op('LMTSTR',[19.5+delta],oid=1)],prefix=f'OBJTYP 1 0 0\nOBJTYP {mid} 0 0\n',guard=193+delta)
        put(f'd{dim}_same_dimension',[op('REFTMP',[21.5+delta],oid=mid)],prefix=f'* DEFORM-{dim}D\nUNIT 1\n'+(mesh3 if dim==3 else mesh2),unit=1,guard=195+delta)
        for k in ['AVGSTR','LMTSTR','PENVOL','REFTMP','ZSTR']:
            put(f'd{dim}_{k.lower()}_eof',[op(k,zero_values(k),header='<EOF>',status=1,oid=5 if k=='ZSTR'else 1)])
        put(f'd{dim}_new_header_eof',[op('AVGSTR',[0.],header='<EOF>',status=1,oid=6)])
        put(f'd{dim}_selector_failures',[op('AVGSTR',[0.],oid=0),op('PENVOL',zero_values('PENVOL'),oid=-3),op('LMTSTR',selector_eof=True)])
        if a.cohort=='prototype':break
    if a.cohort=='prototype':
        plan['inputs']=plan['inputs'][:8];plan['ordered_imports']=[v['file']for v in plan['inputs']]
        for p in base.glob('*.KEY'):
            if p.name not in plan['ordered_imports']:p.unlink()
    assert len(plan['inputs'])<=32
    dest.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8');print(json.dumps({'imports':len(plan['inputs']),'plan':str(dest)}))


if __name__=='__main__':main()
