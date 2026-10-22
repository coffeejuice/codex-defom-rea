"""Prepare source-planned direct object integer imports before observation."""
import argparse
import hashlib
import json
from pathlib import Path

IDS={'ELPSOL':234,'OBJUPD':267,'FRCSTP':199,'FRCNEL':200,'FRCMTH':201,'USRSUB':253}
INDEX={kid:i for i,kid in enumerate(IDS.values())}


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('cohort',choices=['prototype','training','evaluation']);a=ap.parse_args();evaluation=a.cohort=='evaluation';delta=1 if evaluation else 0
    base=Path('tests/fixtures/object-integers')/a.cohort;base.mkdir(parents=True,exist_ok=True);dest=Path('analysis/object-integers-'+a.cohort+'-inputs.json')
    if dest.exists():raise SystemExit('Refusing to replace prepared inputs')
    dimension=3;nstep=1;records={};mid=3+delta
    plan=dict(status='source_planned_before_native',cohort=a.cohort,ordered_imports=[],inputs=[],qualification='Six complete unconditional413de0 import cases in normal owned2D/3D wrappers,defined signed32 input and bounded signed object selectors. Scientific consumers,numeric range extremes/uninitialized selector matching failures,allocation failure,arbitrary aliases/native ABI and complete application remain pending.')
    if evaluation:plan['chronology']='Prepared after training PASS; original executes before C++ with changed IDs/values and reversed dimension/keyword order.'

    def create(oid):
        for i in range(len(records)+1,oid+1):records[i]=[2,0,0,0,0,0]

    def state():return {str(k):list(v)for k,v in records.items()}

    def op(k,value=0,oid=1,header=None,status=0,selector_eof=False):return dict(k=k,value=value,oid=oid,header=header,status=status,selector_eof=selector_eof)

    def apply(o):
        kid=IDS[o['k']];oid=o['oid'];headers=[];reads=[];order=[];selections=[]
        if o['selector_eof']:return o['k'],dict(id=kid,status=2,headers=[],reads=[],refreshes=[],selections=[],order=[],expected_integers=state())
        line=o['header']if o['header']is not None else str(o['value']);data=o['k']+' '+str(oid)+(''if line=='<EOF>'else(' '+line if line else'')+'\n')
        count=len(records);create(oid);selections=[dict(id=oid,before_count=count,after_count=len(records),result_id=oid if oid>0 else None)];order=['selection'];status=2
        if oid>0:
            before=records[oid][INDEX[kid]];records[oid][INDEX[kid]]=o['value'];headers=[dict(kind='int',object_id=oid,before=dict(integers=[before],reals_hex=[]),after=dict(integers=[o['value']],reals_hex=[]),status=o['status'])]
            reads=[dict(capacity=1024,value=None if line=='<EOF>'else(' '+line if line else'')+'\n')];order.extend(['header-enter','fgets','header-leave']);status=2 if o['status']else 0
        return data,dict(id=kid,status=status,headers=headers,reads=reads,refreshes=[],selections=selections,order=order,expected_integers=state())

    def put(label,ops=(),prefix='',switch=None,new_object=None,guard=None):
        nonlocal dimension,nstep,records
        if switch is not None and switch!=dimension:dimension=switch;nstep=1;records={}
        if new_object is not None:create(new_object)
        data=prefix;handlers=[]
        for o in ops:
            text,h=apply(o);data+=text;handlers.append(h)
        if guard is not None:data+=f'NSTEP {guard}\n';nstep=guard
        b=data.encode('ascii');name=f'{len(plan["inputs"]):02d}_{label}.KEY';(base/name).write_bytes(b)
        plan['ordered_imports'].append(name);plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(b).hexdigest(),expected_handlers=handlers,expected_integers=state(),expected_nstep=nstep,expected_wrapper_status=next((h['status']for h in reversed(handlers)if h['status']),0)))

    mesh3=Path('tests/fixtures/custom_fields/01_spatial_mesh.KEY').read_text().split('\n',1)[1];mesh2='OBJTYP 1 2 0\nRZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1 4\n1 1 2 3 4\n'
    keys=list(reversed(IDS))if evaluation else list(IDS)
    lexical={'ELPSOL':('0x2f,tail',47),'OBJUPD':('077extra',63),'FRCSTP':('-0x10 junk',-16),'FRCNEL':('+31,999',31),'FRCMTH':('08',0),'USRSUB':('-7 trailing',-7)}
    for dim in ([2,3]if evaluation else[3,2]):
        put(f'd{dim}_reset',prefix=f'* DEFORM-{dim}D\nUNIT 2\nUNIT 1\n'+(mesh3 if dim==3 else mesh2),switch=dim,new_object=1)
        put(f'd{dim}_positive',[op(k,1+i+delta,oid)for oid in [1,mid]for i,k in enumerate(keys)],prefix=f'OBJTYP 1 2 71\nOBJTYP {mid} 2 73\n',new_object=mid,guard=241+delta)
        put(f'd{dim}_lexical',[op(k,lexical[k][1],mid,header=lexical[k][0])for k in keys],guard=243+delta)
        put(f'd{dim}_matching',[op(k,0,1,header='bad')for k in keys],guard=245+delta)
        put(f'd{dim}_blank',[op(k,0,1,header='',status=1)for k in keys],guard=247+delta)
        put(f'd{dim}_unit_reseed',[op('ELPSOL',3+delta,5),*[op(k,11+i+delta,1)for i,k in enumerate(keys)]],prefix='UNIT 2\n',guard=249+delta)
        put(f'd{dim}_deactivate',[op('FRCSTP',17+delta,mid)],prefix=f'OBJTYP 1 0 71\nOBJTYP {mid} 0 73\n',guard=251+delta)
        put(f'd{dim}_same_dimension',[op(k,21+i+delta,1)for i,k in enumerate(keys)],prefix=f'* DEFORM-{dim}D\nUNIT 1\n'+(mesh3 if dim==3 else mesh2)+'OBJTYP 1 2 71\n',guard=253+delta)
        for k in keys:put(f'd{dim}_{k.lower()}_eof',[op(k,0,1,header='<EOF>',status=1)])
        put(f'd{dim}_new_header_eof',[op('USRSUB',0,6,header='<EOF>',status=1)])
        put(f'd{dim}_selector_failures',[op('ELPSOL',0,0),op('OBJUPD',0,-3),op('USRSUB',selector_eof=True)])
        if a.cohort=='prototype':break
    if a.cohort=='prototype':
        plan['inputs']=plan['inputs'][:8];plan['ordered_imports']=[v['file']for v in plan['inputs']]
        for p in base.glob('*.KEY'):
            if p.name not in plan['ordered_imports']:p.unlink()
    assert len(plan['inputs'])<=32
    dest.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8');print(json.dumps({'imports':len(plan['inputs']),'plan':str(dest)}))


if __name__=='__main__':main()
