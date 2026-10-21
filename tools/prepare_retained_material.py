"""Prepare source-planned BURGRS/ALPHA/NDISFM/RECVRY cases before observation."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import struct
from prepare_material_curves import payload

IDS={'BURGRS':110,'ALPHA':111,'NDISFM':112,'RECVRY':113}
SLOTS={110:'0x18f0',111:'0x1968',112:'0x19e0',113:'0x1a58'}


def hx(values):return b''.join(struct.pack('<d',v)for v in values).hex()


def initial():return {slot:dict(metadata=[0,0,0,0,0,0,0,0,1,0,0,0],constant_hex=hx([0.]),preserved60_hex='00'*8,axes_hex=['','',''],dependent_hex='')for slot in SLOTS.values()}


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('cohort',choices=['prototype','training','evaluation']);a=ap.parse_args()
    evaluation=a.cohort=='evaluation';delta=1 if evaluation else 0
    base=Path('tests/fixtures/retained-material')/a.cohort;base.mkdir(parents=True,exist_ok=True);dest=Path('analysis/retained-material-'+a.cohort+'-inputs.json')
    if dest.exists():raise SystemExit('Refusing to replace prepared inputs')
    dimension=3;nstep=1;records={};mid=5+delta;second=-9-delta
    plan=dict(status='source_planned_before_native',cohort=a.cohort,ordered_imports=[],inputs=[],qualification='Four complete dispatcher cases within finite signed32 conversions and bounded owned rank0/1/2/3 tables. Retained inactive metadata and physical buffers compared; scientific evaluation,arbitrary aliases,native ABI and general numerical domains remain pending.')
    if evaluation:plan['chronology']='Prepared after training PASS; original executes before C++ with changed IDs/values/extents,reversed positive selector/keyword/dimension order.'

    def state():return copy.deepcopy({str(k):v for k,v in records.items()})

    def op(k,selector=0,values=(0.,0.),part=None,extra=None,header=None,header_status=0,extra_header=None,extra_status=0,material=None,selector_eof=False):
        return dict(k=k,selector=selector,values=list(values),part=part,extra=extra,header=header,header_status=header_status,extra_header=extra_header,extra_status=extra_status,material=mid if material is None else material,selector_eof=selector_eof)

    def apply(o):
        kid=IDS[o['k']];material=o['material']
        if o['selector_eof']:return o['k'],dict(id=kid,status=2,calls=[],headers=[],reads=[],refreshes=[],expected_retained_tables=state(),expected_actions=[])
        is_new=material not in records
        if is_new:records[material]=initial()
        t=records[material][SLOTS[kid]];m=t['metadata'];headers=[];reads=[];calls=[];actions=[]
        header=o['header']if o['header']is not None else' '.join(map(str,[o['selector'],*o['values']]))
        data=o['k']+' '+str(material)+(''if header=='<EOF>'else(' '+header if header else'')+'\n')
        reads.append(dict(capacity=1024,value=None if header=='<EOF>'else(' '+header if header else'')+'\n'))
        headers.append(dict(kind='int2r',integers=[o['selector']],reals_hex=[hx([v])for v in o['values']],status=o['header_status']))
        status=2 if o['header_status']else 0
        selector=o['selector'];valid=0<=selector<=(7 if kid==110 else 3)
        if status==0 and not valid:status=2
        if status==0 and kid==110 and selector==7:
            line=o['extra_header']if o['extra_header']is not None else' '.join(map(str,o['extra']))
            if line!='<EOF>':data+=line+'\n'
            reads.append(dict(capacity=1024,value=None if line=='<EOF>'else line+'\n'))
            headers.append(dict(kind='int3',integers=o['extra'],reals_hex=[],status=o['extra_status']))
            if o['extra_status']:status=2
        if status==0:
            if selector==0:m[1]=0;t['constant_hex']=hx([o['values'][0]])
            elif kid==110:
                if selector<4:m[1]=1;m[2]={1:3,2:6,3:24}[selector];m[5]=int(o['values'][0])
                elif selector<7:m[1]=2;m[2]=6 if selector==6 else 3;m[3]=6 if selector==4 else 24;m[5:7]=list(map(int,o['values']))
                else:m[1]=3;m[2:5]=[3,6,24];m[5:8]=o['extra']
            else:
                if selector<3:m[1]=1;m[2]=3 if selector==1 else 8;m[5]=int(o['values'][0])
                else:m[1]=2;m[2:4]=[3,8];m[5:7]=list(map(int,o['values']))
            entry=copy.deepcopy(t);rank=m[1];spec=o['part'];assert spec is not None,(o['k'],selector)
            data+=spec['text']
            if rank:
                for i in [5,6,7]:m[i]=max(0,m[i])
                size=1
                for i in range(rank):size*=m[5+i]
                t['axes_hex']=[hx([*spec['axes'][i],*([0.]*(m[5+i]-len(spec['axes'][i])))])if i<rank else''for i in range(3)]
                dependent=[0.]*size
                if rank==1 or spec['status']==0:dependent[:len(spec['dependent'])]=spec['dependent']
                t['dependent_hex']=hx(dependent)
                if spec['status']==0 and size>0:reads.append(dict(capacity=1024,value=spec['tail']))
            else:assert spec['text']==''and spec['status']==0
            calls.append(dict(slot=SLOTS[kid],material_id=material,entry=entry,after=copy.deepcopy(t),status=spec['status']))
            actions.extend([dict(kind='payload-enter',slot=SLOTS[kid]),dict(kind='payload-leave',slot=SLOTS[kid])]);status=2 if spec['status']else 0
        return data,dict(id=kid,status=status,calls=calls,headers=headers,reads=reads,refreshes=[1]if is_new else[],expected_retained_tables=state(),expected_actions=actions)

    def put(label,ops=(),prefix='',switch=None,guard=None):
        nonlocal dimension,nstep,records
        if switch is not None and switch!=dimension:dimension=switch;nstep=1;records={}
        data=prefix;handlers=[]
        for o in ops:
            text,handler=apply(o);data+=text;handlers.append(handler)
        if guard is not None:data+=f'NSTEP {guard}\n';nstep=guard
        b=data.encode('ascii');name=f'{len(plan["inputs"]):02d}_{label}.KEY';(base/name).write_bytes(b)
        plan['ordered_imports'].append(name);plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(b).hexdigest(),expected_handlers=handlers,expected_retained_tables=state(),expected_nstep=nstep,expected_wrapper_status=next((h['status']for h in reversed(handlers)if h['status']),0)))

    def curve(k,selector=1,rows=None):
        rows=2+delta if rows is None else rows
        return op(k,selector,(float(rows)+.75,0.),payload(([10.+i+delta for i in range(max(0,rows))],[],[]),[1.25+i+delta for i in range(max(0,rows))]))

    def surface(k,selector=None,counts=None,part=None):
        selector=(4 if k=='BURGRS'else 3)if selector is None else selector;counts=[1,2+delta]if counts is None else counts
        if part is None:part=payload(([20.+i+delta for i in range(max(0,counts[0]))],[30.+i+delta for i in range(max(0,counts[1]))],[]),[2.5+i+delta for i in range(max(0,counts[0])*max(0,counts[1]))],rank=2)
        return op(k,selector,counts,part)

    def cube(part=None):
        counts=[1,2+delta,1]
        if part is None:part=payload(([10.+delta],[20.+i+delta for i in range(counts[1])],[40.+delta]),[3.5+i+delta for i in range(counts[1])],rank=3)
        return op('BURGRS',7,(91.,92.),part,extra=counts)

    empty=payload(rank=0)
    mesh3=Path('tests/fixtures/custom_fields/01_spatial_mesh.KEY').read_text();mesh2='* DEFORM-2D\nOBJTYP 1 2 0\nRZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1 4\n1 1 2 3 4\n'
    keys=list(reversed(IDS))if evaluation else list(IDS)
    for dim in ([2,3]if evaluation else[3,2]):
        put(f'd{dim}_reset',prefix=mesh3 if dim==3 else mesh2,switch=dim)
        positive=[]
        for k in keys:
            positive.append(op(k,0,(.625+delta,9.),empty))
            selectors=list(range(1,4 if k=='BURGRS'else 3))
            if evaluation:selectors.reverse()
            positive.extend(curve(k,i)for i in selectors)
            if k=='BURGRS':positive.extend(surface(k,i)for i in ([6,5,4]if evaluation else[4,5,6]));positive.append(cube())
            else:positive.append(surface(k))
        put(f'd{dim}_positive',positive,guard=161+delta)
        put(f'd{dim}_transitions',[o for k in keys for o in [op(k,0,(7.5+delta,0.),empty),curve(k,2),op(k,0,(-0.,0.),empty),surface(k)]],guard=163+delta)
        put(f'd{dim}_invalid',[op(k,i,(91.,92.))for k in keys for i in [-1,8 if k=='BURGRS'else 4]],guard=165+delta)
        partial=[]
        for k in keys:
            partial.extend([op(k,1,(0.,0.),payload(),header='1'),op(k,0,(0.,0.),empty,header='bad')])
        partial.append(op('BURGRS',7,(0.,0.),payload(([31.+delta],[],[]),[],rank=3),extra=[1,0,0],extra_header='1'))
        put(f'd{dim}_partial_headers',partial,guard=167+delta)
        put(f'd{dim}_blank_headers',[*[op(k,0,(0.,0.),header='',header_status=1)for k in keys],op('BURGRS',7,(91.,92.),extra=[0,0,0],extra_header='',extra_status=1)],guard=169+delta)
        put(f'd{dim}_reseed',[cube(),*[surface(k)for k in keys if k!='BURGRS']],guard=171+delta)
        for k in keys:
            put(f'd{dim}_{k.lower()}_rank1_eof',[op(k,1,(float(2+delta),0.),payload(([13.+delta],[],[]),[],status=1))])
        counts=[1,2+delta,1]
        put(f'd{dim}_rank3_eof',[cube(payload(([10.],[20.+i for i in range(counts[1])],[40.]),[4.],rank=3,status=1))])
        put(f'd{dim}_extra_eof',[op('BURGRS',7,(91.,92.),extra=[0,0,0],extra_header='<EOF>',extra_status=1)])
        put(f'd{dim}_new_header_eof',[op('ALPHA',0,(0.,0.),header='<EOF>',header_status=1,material=second)])
        put(f'd{dim}_selector_eof',[op('RECVRY',selector_eof=True)])
        put(f'd{dim}_retention',[o for k in keys for o in [op(k,0,(17.5+delta,0.),empty),curve(k,2,-2),surface(k,counts=[0,1]),op(k,0,(19.5+delta,0.),empty)]],prefix=f'* DEFORM-{dim}D\nUNIT {2 if dim==3 else 1}\n',guard=173+delta)
        if a.cohort=='prototype':break
    if a.cohort=='prototype':
        plan['inputs']=plan['inputs'][:8];plan['ordered_imports']=[v['file']for v in plan['inputs']]
        for p in base.glob('*.KEY'):
            if p.name not in plan['ordered_imports']:p.unlink()
    assert len(plan['inputs'])<=32
    dest.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8');print(json.dumps({'imports':len(plan['inputs']),'plan':str(dest)}))


if __name__=='__main__':main()
