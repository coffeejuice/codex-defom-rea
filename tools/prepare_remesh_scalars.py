"""Prepare source-planned TMPLMT and remeshing scalar imports before observation."""
import argparse
import hashlib
import json
from pathlib import Path
import struct

IDS={'TMPLMT':202,'MGSIZR':186,'MGWTMP':188,'MGWSTN':189,'MGWSTR':190,'MGWCUV':187,'RMDPTH':180,'RMSTRK':181,'RMSTEP':183,'RMTIME':182}
INDEX={kid:i for i,kid in enumerate(IDS.values())}


def hx(values):return b''.join(struct.pack('<d',v)for v in values).hex()


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('cohort',choices=['prototype','training','evaluation']);a=ap.parse_args();evaluation=a.cohort=='evaluation';delta=1 if evaluation else 0
    base=Path('tests/fixtures/remesh-scalars')/a.cohort;base.mkdir(parents=True,exist_ok=True);dest=Path('analysis/remesh-scalars-'+a.cohort+'-inputs.json')
    if dest.exists():raise SystemExit('Refusing to replace prepared inputs')
    dimension=3;nstep=1;records={};mid=3+delta
    plan=dict(status='source_planned_before_native',cohort=a.cohort,ordered_imports=[],inputs=[],qualification='Ten complete unconditional414990 import cases in normal owned2D/3D wrappers and bounded signed object selectors. Scientific remeshing/generation,other remeshing fields,numeric extremes,uninitialized selector matching failures,allocation failure,arbitrary aliases/native ABI and complete application remain pending.')
    if evaluation:plan['chronology']='Prepared after training PASS; original executes before C++ with changed values/IDs,reversed dimension/keyword order and complementary per-dimension EOF subjects.'

    def create(oid):
        for i in range(len(records)+1,oid+1):records[i]=[0.,3.,0.,1.,1.,1.,0.,0.,0.,0.]if dimension==2 else[0.,2.,0.,.25,.25,.5,-.7,0.,0.,0.]

    def state():return {str(k):dict(temperature_limit_0x1f10_hex=hx(v[:1]),remeshing_scalars_hex=hx(v[1:]))for k,v in records.items()}

    def op(k,value=0.,oid=1,header=None,status=0,selector_eof=False):return dict(k=k,value=value,oid=oid,header=header,status=status,selector_eof=selector_eof)

    def apply(o):
        kid=IDS[o['k']];oid=o['oid']
        if o['selector_eof']:return o['k'],dict(id=kid,status=2,headers=[],reads=[],refreshes=[],selections=[],order=[],expected_scalars=state())
        header=o['header']if o['header']is not None else str(o['value']);data=o['k']+' '+str(oid)+(''if header=='<EOF>'else(' '+header if header else'')+'\n')
        count=len(records);create(oid);selection=dict(id=oid,before_count=count,after_count=len(records),result_id=oid if oid>0 else None)
        headers=[];reads=[];order=['selection'];status=2
        if oid>0:
            before=records[oid][INDEX[kid]];records[oid][INDEX[kid]]=o['value']
            headers=[dict(kind='real',object_id=oid,before=dict(integers=[],reals_hex=[hx([before])]),after=dict(integers=[],reals_hex=[hx([o['value']])]),status=o['status'])]
            reads=[dict(capacity=1024,value=None if header=='<EOF>'else(' '+header if header else'')+'\n')];order.extend(['header-enter','fgets','header-leave']);status=2 if o['status']else 0
        return data,dict(id=kid,status=status,headers=headers,reads=reads,refreshes=[],selections=[selection],order=order,expected_scalars=state())

    def put(label,ops=(),prefix='',switch=None,new_object=None,guard=None):
        nonlocal dimension,nstep,records
        if switch is not None and switch!=dimension:dimension=switch;nstep=1;records={}
        if new_object is not None:create(new_object)
        data=prefix;handlers=[]
        for o in ops:
            text,h=apply(o);data+=text;handlers.append(h)
        if guard is not None:data+=f'NSTEP {guard}\n';nstep=guard
        b=data.encode('ascii');name=f'{len(plan["inputs"]):02d}_{label}.KEY';(base/name).write_bytes(b)
        plan['ordered_imports'].append(name);plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(b).hexdigest(),expected_handlers=handlers,expected_scalars=state(),expected_nstep=nstep,expected_wrapper_status=next((h['status']for h in reversed(handlers)if h['status']),0)))

    mesh3=Path('tests/fixtures/custom_fields/01_spatial_mesh.KEY').read_text().split('\n',1)[1];mesh2='OBJTYP 1 2 0\nRZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1 4\n1 1 2 3 4\n'
    keys=list(reversed(IDS))if evaluation else list(IDS)
    for dim in ([2,3]if evaluation else[3,2]):
        put(f'd{dim}_reset',prefix=f'* DEFORM-{dim}D\nUNIT 2\nUNIT 1\n'+(mesh3 if dim==3 else mesh2),switch=dim,new_object=1)
        put(f'd{dim}_positive',[op(k,1.25+i+delta,oid)for oid in [1,mid]for i,k in enumerate(keys)],guard=201+delta)
        put(f'd{dim}_lexical',[op(k,-0.,mid,header='-0.0,9 trailing')for k in keys],guard=203+delta)
        put(f'd{dim}_matching',[op(k,0.,mid,header='bad')for k in keys],guard=205+delta)
        put(f'd{dim}_blank',[op(k,0.,1,header='',status=1)for k in keys],guard=207+delta)
        put(f'd{dim}_unit_reseed',[op('TMPLMT',17.5+delta,5),*[op(k,30.5+i+delta,1)for i,k in enumerate(keys)]],prefix='UNIT 2\n',guard=209+delta)
        put(f'd{dim}_deactivate',[op('TMPLMT',19.5+delta,mid)],prefix=f'OBJTYP 1 0 0\nOBJTYP {mid} 0 0\n',guard=211+delta)
        put(f'd{dim}_same_dimension',[op(k,50.5+i+delta,1)for i,k in enumerate(keys)],prefix=f'* DEFORM-{dim}D\nUNIT 1\n'+(mesh3 if dim==3 else mesh2),guard=213+delta)
        eof_keys=list(IDS)[:5]if (dim==3)!=evaluation else list(IDS)[5:]
        for k in eof_keys:put(f'd{dim}_{k.lower()}_eof',[op(k,0.,1,header='<EOF>',status=1)])
        put(f'd{dim}_new_header_eof',[op('TMPLMT',0.,6,header='<EOF>',status=1)])
        put(f'd{dim}_selector_failures',[op('TMPLMT',0.,0),op('MGSIZR',0.,-3),op('RMDPTH',selector_eof=True)])
        put(f'd{dim}_retention',[op(k,70.5+i+delta,1)for i,k in enumerate(keys)],prefix=f'* DEFORM-{dim}D\nUNIT 1\n',guard=215+delta)
        if a.cohort=='prototype':break
    if a.cohort=='prototype':
        plan['inputs']=plan['inputs'][:8];plan['ordered_imports']=[v['file']for v in plan['inputs']]
        for p in base.glob('*.KEY'):
            if p.name not in plan['ordered_imports']:p.unlink()
    assert len(plan['inputs'])<=32
    dest.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8');print(json.dumps({'imports':len(plan['inputs']),'plan':str(dest)}))


if __name__=='__main__':main()
