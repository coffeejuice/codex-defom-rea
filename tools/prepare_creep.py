"""Prepare source-planned CREEP imports without consulting native outputs."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import struct
from prepare_material_curves import payload

SLOTS=['0xad0','0xb48','0xbc0','0xc38','0xcb0','0xd28','0xda8','0xe20','0xe98']
KINDS=[0x43,0x44,0x45,0x46,0x47,0x48,0xa9,0xaa,0xab]


def hx(values):return b''.join(struct.pack('<d',v)for v in values).hex()


def table(kind):
    rank,selectors,width={0x43:(3,[1,6,3],1),0x44:(3,[7,6,3],1),0x45:(1,[3,0,0],2),0x46:(1,[3,0,0],2),0x47:(1,[3,0,0],5),0x48:(1,[3,0,0],3),0xa9:(1,[0,0,0],2),0xaa:(0,[0,0,0],1),0xab:(0,[0,0,0],1)}[kind]
    return dict(metadata=[kind,rank,*selectors,1 if kind==0xa9 else 0,0,0,width,0,0,0],constant_hex=hx([0.]),preserved60_hex='00'*8,axes_hex=[hx([0.])if kind==0xa9 else'','',''],dependent_hex=hx([0.,0.])if kind==0xa9 else hx([0.])if kind in [0xaa,0xab]else'')


def initial():return dict(mode_0xac8=0,reference_0xda0=0,axis_count_0xda4=-1,tables={slot:table(kind)for slot,kind in zip(SLOTS,KINDS)})


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('cohort',choices=['prototype','training','evaluation']);a=ap.parse_args()
    evaluation=a.cohort=='evaluation';delta=1 if evaluation else 0
    base=Path('tests/fixtures/creep')/a.cohort;base.mkdir(parents=True,exist_ok=True);dest=Path('analysis/creep-'+a.cohort+'-inputs.json')
    if dest.exists():raise SystemExit('Refusing to replace prepared inputs')
    dimension=3;nstep=1;records={};mid=5+delta;second=-9-delta
    plan=dict(status='source_planned_before_native',cohort=a.cohort,ordered_imports=[],inputs=[],qualification='Complete CREEP selector dispatcher within finite signed32,nonoverflowing derived sums/products and bounded owned table profiles. Scientific evaluation,arbitrary aliases,native ABI and general numerical domains remain pending.')
    if evaluation:plan['chronology']='Prepared after training PASS; original executes before C++ with changed IDs/values/extents and reversed dimension/positive-selector order.'

    def state():return copy.deepcopy({str(k):v for k,v in records.items()})

    def op(values,parts=(),extra=None,extra_header=None,extra_status=0,header=None,header_status=0,material=None,selector_eof=False):
        return dict(values=list(values),parts=list(parts),extra=extra,extra_header=extra_header,extra_status=extra_status,header=header,header_status=header_status,material=mid if material is None else material,selector_eof=selector_eof)

    def apply(o):
        if o['selector_eof']:return 'CREEP',dict(id=106,status=2,calls=[],headers=[],reads=[],refreshes=[],expected_creep=state(),expected_actions=[])
        material=o['material'];is_new=material not in records
        if is_new:records[material]=initial()
        f=records[material];headers=[];calls=[];reads=[];actions=[];parts=iter(o['parts'])
        header=o['header']if o['header']is not None else' '.join(map(str,o['values']))
        data='CREEP '+str(material)+(''if header=='<EOF>'else(' '+header if header else'')+'\n')
        reads.append(dict(capacity=1024,value=None if header=='<EOF>'else(' '+header if header else'')+'\n'))
        headers.append(dict(kind='int3',integers=o['values'],reals_hex=[],status=o['header_status']))
        status=2 if o['header_status']else 0

        def reset(index):
            slot=SLOTS[index];old=f['tables'][slot];t=table(KINDS[index]);t['metadata'][10]=old['metadata'][10];t['preserved60_hex']=old['preserved60_hex'];f['tables'][slot]=t
            actions.extend([dict(kind='reset-preserve',slot=slot),dict(kind='reset-direct',slot=slot)]);return t

        def read_extra(live=False):
            nonlocal data
            values=o['extra'];line=o['extra_header']if o['extra_header']is not None else' '.join(map(str,values))
            if line!='<EOF>':data+=line+'\n'
            reads.append(dict(capacity=1024,value=None if line=='<EOF>'else line+'\n'))
            h=dict(kind='int1'if live else'int3',integers=values,reals_hex=[],status=o['extra_status'])
            if live:h.update(material_id=material,before_value=f['axis_count_0xda4']);f['axis_count_0xda4']=values[0]
            headers.append(h);return o['extra_status']

        def read(index):
            nonlocal data
            slot=SLOTS[index];t=f['tables'][slot];entry=copy.deepcopy(t);m=t['metadata'];rank=m[1];width=m[8];spec=next(parts)
            data+=spec['text']
            if rank!=0 or width>1:
                for i in [5,6,7]:m[i]=max(0,m[i])
                width=m[8]=max(1,width);size=width
                for i in range(rank):size*=m[5+i]
                t['axes_hex']=[hx([*spec['axes'][i],*([0.]*(m[5+i]-len(spec['axes'][i])))])if i<rank else''for i in range(3)]
                dependent=[0.]*size
                if rank<2 or spec['status']==0:dependent[:len(spec['dependent'])]=spec['dependent']
                t['dependent_hex']=hx(dependent)
                if spec['status']==0 and size>0:reads.append(dict(capacity=1024,value=spec['tail']))
            else:assert spec['text']==''and spec['status']==0
            calls.append(dict(slot=slot,material_id=material,entry=entry,after=copy.deepcopy(t),status=spec['status']))
            actions.extend([dict(kind='payload-enter',slot=slot),dict(kind='payload-leave',slot=slot)])
            return 2 if spec['status']else 0

        if status==0:
            selector=o['values'][0]
            if selector==0:f['mode_0xac8']=0
            elif 1<=selector<=4:
                f['mode_0xac8']=selector;reset(selector+1)['metadata'][5]=o['values'][1];status=read(selector+1)
            elif selector in [5,6]:
                f['mode_0xac8']=selector;status=2 if read_extra()else 0
                if status==0:
                    index=1 if selector==5 else 0;reset(index)['metadata'][5:8]=o['extra'];status=read(index)
            elif selector in [7,8]:
                f['mode_0xac8']=selector;status=2 if read_extra(True)else 0
                if status==0:
                    reset(6)['metadata'][5]=o['extra'][0];status=read(6)
                    if status==0:
                        axes=list(struct.unpack('<'+'d'*(len(f['tables'][SLOTS[6]]['axes_hex'][0])//16),bytes.fromhex(f['tables'][SLOTS[6]]['axes_hex'][0])))
                        total=sum(map(int,axes));product=1
                        for v in axes:product*=int(v)
                        reset(7)['metadata'][8]=total;status=read(7)
                        if status==0:reset(8)['metadata'][8]=product;status=read(8)
            elif selector<0:f['mode_0xac8']=9;f['reference_0xda0']=-selector
        return data,dict(id=106,status=status,calls=calls,headers=headers,reads=reads,refreshes=[1]if is_new else[],expected_creep=state(),expected_actions=actions)

    def put(label,ops=(),prefix='',switch=None,guard=None):
        nonlocal dimension,nstep,records
        if switch is not None and switch!=dimension:dimension=switch;nstep=1;records={}
        data=prefix;handlers=[]
        for o in ops:
            text,h=apply(o);data+=text;handlers.append(h)
        if guard is not None:data+=f'NSTEP {guard}\n';nstep=guard
        b=data.encode('ascii');name=f'{len(plan["inputs"]):02d}_{label}.KEY';(base/name).write_bytes(b)
        plan['ordered_imports'].append(name);plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(b).hexdigest(),expected_handlers=handlers,expected_creep=state(),expected_nstep=nstep,expected_wrapper_status=next((h['status']for h in reversed(handlers)if h['status']),0)))

    def rank1(mode,rows=None,part=None):
        rows=2+delta if rows is None else rows;width={1:2,2:2,3:5,4:3}[mode]
        if part is None:part=payload(([10.+i+delta for i in range(max(0,rows))],[],[]),[1.25+i+delta for i in range(max(0,rows)*width)],width=width)
        return op([mode,rows,91],parts=[part])

    def cube(mode,counts=None,part=None):
        counts=[1,2+delta,1]if counts is None else counts
        if part is None:
            axes=[[20.+j+i*10+delta for j in range(max(0,n))]for i,n in enumerate(counts)];size=1
            for n in counts:size*=max(0,n)
            part=payload(axes,[2.5+i+delta for i in range(size)],rank=3)
        return op([mode,99,88],parts=[part],extra=counts)

    def derived(mode,axes=None,parts=None,count=None):
        axes=[2.+delta,3.]if axes is None else axes;count=len(axes)if count is None else count
        if parts is None:
            total=sum(map(int,axes));product=1
            for v in axes:product*=int(v)
            parts=[payload((axes,[],[]),[3.25+i+delta for i in range(len(axes)*2)],width=2),payload(dependent=[4.5+i+delta for i in range(max(0,total))]if total>1 else[],rank=0),payload(dependent=[5.5+i+delta for i in range(max(0,product))]if product>1 else[],rank=0)]
        return op([mode,777,888],parts,extra=[count])

    mesh3=Path('tests/fixtures/custom_fields/01_spatial_mesh.KEY').read_text();mesh2='* DEFORM-2D\nOBJTYP 1 2 0\nRZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1 4\n1 1 2 3 4\n'
    for dim in ([2,3]if evaluation else[3,2]):
        put(f'd{dim}_reset',prefix=mesh3 if dim==3 else mesh2,switch=dim)
        modes=[6,5,4,3,2,1]if evaluation else[1,2,3,4,5,6]
        put(f'd{dim}_positive',[rank1(i)if i<5 else cube(i)for i in modes],guard=141+delta)
        put(f'd{dim}_derived',[derived(7),derived(8)],guard=143+delta)
        put(f'd{dim}_preserve',[op([-17-delta,4,5]),op([9,6,7]),op([99,6,7]),op([0,8,9]),op([1,0,0],parts=[payload(width=2)],header='1'),op([0,0,0],header='bad')],guard=145+delta)
        put(f'd{dim}_small_widths',[derived(7,[]),derived(8,[1.]),derived(7,[0.]),derived(8,[-1.,2.]),derived(7,[2.75,3.25])],guard=147+delta)
        put(f'd{dim}_reseed',[rank1(3),cube(5),derived(8)],guard=149+delta)
        blanks=[op([0,0,0],header='',header_status=1),op([5,0,0],extra=[0,0,0],extra_header='',extra_status=1),op([6,0,0],extra=[0,0,0],extra_header='',extra_status=1),op([7,0,0],extra=[0],extra_header='',extra_status=1)]
        put(f'd{dim}_failed_headers',blanks,guard=151+delta)
        put(f'd{dim}_rank1_eof',[rank1(3,part=payload(([13.+delta],[],[]),[4.],width=5,status=1))])
        put(f'd{dim}_rank3_eof',[cube(6,part=payload(([10.],[20.,30.],[40.]),[4.],rank=3,status=1))])
        put(f'd{dim}_derived_first_eof',[derived(7,parts=[payload(([2.],[],[]),[4.],width=2,status=1)])])
        put(f'd{dim}_derived_second_eof',[derived(8,parts=[payload(([2.,3.],[],[]),[2.,3.,4.,5.],width=2),payload(dependent=[9.,8.],rank=0,status=1)])])
        put(f'd{dim}_derived_third_eof',[derived(7,parts=[payload(([2.,3.],[],[]),[2.,3.,4.,5.],width=2),payload(dependent=[9.,8.,7.,6.,5.],rank=0),payload(dependent=[4.,3.],rank=0,status=1)])])
        put(f'd{dim}_extra_eof',[op([7,0,0],extra=[0],extra_header='<EOF>',extra_status=1)])
        put(f'd{dim}_new_header_eof',[op([0,0,0],header='<EOF>',header_status=1,material=second)])
        put(f'd{dim}_selector_eof',[op([0,0,0],selector_eof=True)])
        put(f'd{dim}_retention',[rank1(2,-2),cube(5,[0,1,1]),derived(8,[],count=-2),op([-23-delta,0,0])],prefix=f'* DEFORM-{dim}D\nUNIT {2 if dim==3 else 1}\n',guard=153+delta)
        if a.cohort=='prototype':break
    if a.cohort=='prototype':
        keep=[0,1,2,3,4,7,10]
        plan['inputs']=[plan['inputs'][i]for i in keep]
        # Prototype is an ordered prefix: omitted transitions must not seed expectations.
        # Retain only the first five files, then the two EOFs without source snapshots.
        for v in plan['inputs'][5:]:
            v.pop('expected_handlers');v.pop('expected_creep');v.pop('expected_nstep')
        plan['ordered_imports']=[v['file']for v in plan['inputs']]
        for p in base.glob('*.KEY'):
            if p.name not in plan['ordered_imports']:p.unlink()
    assert len(plan['inputs'])<=32
    dest.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8');print(json.dumps({'imports':len(plan['inputs']),'plan':str(dest)}))


if __name__=='__main__':main()
