"""Prepare source-planned hardening cases without reading native outputs."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import struct
from prepare_material_curves import payload

IDS={'GRNMDL':109,'HDNPHA':114,'HDNTIM':117,'JOMINY':118,'UTSDAT':119,'HDNRUL':120}


def hx(values):return b''.join(struct.pack('<d',v)for v in values).hex()


def table(kind=0):
    rank,selectors={0:(0,[0,0,0]),0x23:(0,[0,0,0]),0x53:(1,[1,0,0]),0x52:(2,[13,12,0]),0x1b:(0,[0,0,0])}[kind]
    return dict(metadata=[kind,rank,*selectors,0,0,0,1,0,0,0],constant_hex=hx([0.]),preserved60_hex='00'*8,axes_hex=['','',''],dependent_hex='')


def initial():return dict(hdnrul_0x668=0,tables={'0xf10':table(),'0xf88':table(0x53),'0x1000':table(0x52)},utsdat=table(0x1b))


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('cohort',choices=['training','evaluation']);a=ap.parse_args();evaluation=a.cohort=='evaluation';delta=1 if evaluation else 0
    base=Path('tests/fixtures/hardening')/a.cohort;base.mkdir(parents=True,exist_ok=True);dest=Path('analysis/hardening-'+a.cohort+'-inputs.json')
    if dest.exists():raise SystemExit('Refusing to replace prepared inputs')
    dimension=3;nstep=1;records={};mid=5+delta;second=-9-delta
    plan=dict(status='source_planned_before_native',cohort=a.cohort,ordered_imports=[],inputs=[],qualification='Six hardening/ignored/direct material handlers in the finite signed32,owned normal-wrapper profile. UTSDAT shares existing1b10 storage. General numerical domains,scientific evaluation and arbitrary aliases/native ABI remain pending.')
    if evaluation:plan['chronology']='Prepared after training PASS; original runs before C++ with changed IDs/values/extents and reversed keyword/dimension order.'

    def state():return copy.deepcopy({str(k):v for k,v in records.items()})

    def op(k,ints,reals=(),part=None,header=None,header_status=0,material=mid,selector_eof=False):
        return dict(k=k,ints=list(ints),reals=list(reals),part=part,header=header,header_status=header_status,material=material,selector_eof=selector_eof)

    def apply(o):
        k=o['k'];kid=IDS[k];ints=o['ints'];reals=o['reals'];hs=o['header_status'];material=o['material'];calls=[];reads=[];actions=[]
        if o['selector_eof']:return k,dict(id=kid,status=2,calls=[],headers=[],reads=[],refreshes=[],expected_hardening=state(),expected_actions=[])
        is_new=material not in records
        if is_new:records[material]=initial()
        f=records[material];before=f['hdnrul_0x668'];kind='int2r'if kid==114 else'int3'if kid==118 else'int1r'if kid==119 else'int1'
        header=o['header']
        if header is None:header=' '.join(map(str,[*ints,*reals]))
        data=k+' '+str(material)+(''if header=='<EOF>'else(' '+header if header else'')+'\n')
        reads.append(dict(capacity=1024,value=None if header=='<EOF>'else(' '+header if header else'')+'\n'))
        h=dict(kind=kind,integers=ints,reals_hex=[hx([v])for v in reals],status=hs)
        if kid==120:h.update(material_id=material,before_value=before);f['hdnrul_0x668']=ints[0]
        status=2 if hs else 0

        def get(slot):return f['utsdat']if slot=='0x1b10'else f['tables'][slot]

        def reset(slot,kind):
            old=get(slot);t=table(kind);t['metadata'][10]=old['metadata'][10];t['preserved60_hex']=old['preserved60_hex']
            if slot=='0x1b10':f['utsdat']=t
            else:f['tables'][slot]=t
            actions.extend([dict(kind='reset-preserve',slot=slot),dict(kind='reset-direct',slot=slot)]);return t

        def read(slot):
            nonlocal data
            t=get(slot);m=t['metadata'];entry=copy.deepcopy(t);rank=m[1];size=1
            if rank==0:size=0
            else:
                for i in [5,6,7]:m[i]=max(0,m[i])
                for i in range(rank):size*=m[5+i]
            spec=o['part'];assert spec is not None,(k,slot)
            data+=spec['text']
            t['axes_hex']=[hx([*spec['axes'][i],*([0.]*(m[5+i]-len(spec['axes'][i])))])if i<rank else''for i in range(3)]
            dependent=[0.]*size
            if rank==1 or spec['status']==0:dependent[:len(spec['dependent'])]=spec['dependent']
            t['dependent_hex']=hx(dependent)
            if spec['status']==0 and size>0:reads.append(dict(capacity=1024,value=spec['tail']))
            calls.append(dict(slot=slot,material_id=material,entry=entry,after=copy.deepcopy(t),status=spec['status']));actions.extend([dict(kind='payload-enter',slot=slot),dict(kind='payload-leave',slot=slot)])
            return 2 if spec['status']else 0

        if hs==0:
            if kid==117:
                reset('0xf88',0x53)['metadata'][5]=ints[0];status=read('0xf88')
            elif kid==118:
                t=reset('0x1000',0x52)
                if ints[0]==1:t['metadata'][5:7]=ints[1:3];status=read('0x1000')
            elif kid==119:
                t=reset('0x1b10',0x1b)
                if ints[0]==0:t['constant_hex']=hx([reals[0]])
                elif ints[0]==1:t['metadata'][1]=1;t['metadata'][2]=3;t['metadata'][5]=int(reals[0]);status=read('0x1b10')
            elif kid==114:
                t=reset('0xf10',0x23);selector=ints[0]
                if selector==0:t['constant_hex']=hx([reals[0]]);status=read('0xf10')
                elif 1<=selector<=3:t['metadata'][1]=1;t['metadata'][2]={1:12,2:3,3:9}[selector];t['metadata'][5]=int(reals[0]);status=read('0xf10')
                elif selector==4:t['metadata'][1]=2;t['metadata'][2:4]=[3,12];t['metadata'][5:7]=list(map(int,reals));status=read('0xf10')
                else:status=3
        return data,dict(id=kid,status=status,calls=calls,headers=[h],reads=reads,refreshes=[1]if is_new else[],expected_hardening=state(),expected_actions=actions)

    def put(label,ops=(),prefix='',switch=None,guard=None):
        nonlocal dimension,nstep,records
        if switch is not None and switch!=dimension:dimension=switch;nstep=1;records={}
        data=prefix;handlers=[]
        for o in ops:
            text,h=apply(o);data+=text;handlers.append(h)
        if guard is not None:data+=f'NSTEP {guard}\n';nstep=guard
        b=data.encode('ascii');name=f'{len(plan["inputs"]):02d}_{label}.KEY';(base/name).write_bytes(b)
        plan['ordered_imports'].append(name);plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(b).hexdigest(),expected_handlers=handlers,expected_hardening=state(),expected_nstep=nstep,expected_wrapper_status=next((h['status']for h in reversed(handlers)if h['status']),0)))

    mesh3=Path('tests/fixtures/custom_fields/01_spatial_mesh.KEY').read_text();mesh2='* DEFORM-2D\nOBJTYP 1 2 0\nRZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1 4\n1 1 2 3 4\n'
    for dim in ([2,3]if evaluation else[3,2]):
        rows=2+delta;p1=payload(([10.+i*10+delta for i in range(rows)],[],[]),[1.5+i+delta for i in range(rows)])
        p2=payload(([30.+delta],[40.+i+delta for i in range(rows)],[]),[2.5+i+delta for i in range(rows)],rank=2)
        empty=payload();empty2=payload(rank=2)
        put(f'd{dim}_reset',prefix=mesh3 if dim==3 else mesh2,switch=dim)
        positive=[op('HDNRUL',[123+delta]),op('GRNMDL',[-7-delta]),op('HDNPHA',[0],[.625+delta,0],empty)]
        positive +=[op('HDNPHA',[i],[float(rows),0],p1)for i in ([3,2,1]if evaluation else[1,2,3])]
        positive +=[op('HDNPHA',[4],[1.,float(rows)],p2),op('HDNTIM',[rows],part=p1),op('JOMINY',[1,1,rows],part=p2),op('UTSDAT',[0],[8.5+delta]),op('UTSDAT',[1],[float(rows)],p1)]
        put(f'd{dim}_positive',positive,guard=121+delta)
        put(f'd{dim}_disabled_unknown',[op('HDNPHA',[5],[1.,1.]),op('HDNPHA',[-1],[1.,1.]),*[op('JOMINY',[i,3,3])for i in[0,2,-3]],op('UTSDAT',[2],[7.]),op('UTSDAT',[-1],[9.]),op('GRNMDL',[97]),op('HDNRUL',[47],header='0x2f')],guard=123+delta)
        put(f'd{dim}_reseed',[op('HDNPHA',[4],[1.,float(rows)],p2),op('HDNTIM',[rows],part=p1),op('JOMINY',[1,1,rows],part=p2),op('UTSDAT',[1],[float(rows)],p1)],guard=125+delta)
        put(f'd{dim}_alias',prefix=f'FRCMOD {mid} 12 21 1 31 41 51\n10 5.5\n',guard=127+delta)
        put(f'd{dim}_partial_headers',[op('HDNPHA',[1],[0.,0.],empty,header='1'),op('HDNTIM',[rows],part=p1,header=str(rows)+'junk'),op('JOMINY',[1,1,0],part=payload(([8.+delta],[],[]),[],rank=2),header='1 1'),op('UTSDAT',[1],[0.],empty,header='1'),op('HDNRUL',[0],header='bad'),op('GRNMDL',[0],header='bad')],guard=129+delta)
        blanks=[op('HDNRUL',[77])]+[op(k,[0,0,0]if k=='JOMINY'else[0],[0.,0.]if k=='HDNPHA'else[0.]if k=='UTSDAT'else[],header='',header_status=1)for k in IDS]
        put(f'd{dim}_blank_headers',blanks,guard=131+delta)
        put(f'd{dim}_error_order',[op('HDNPHA',[8],[1.,1.]),op('HDNRUL',[0],header='',header_status=1),op('GRNMDL',[19])],guard=133+delta)
        put(f'd{dim}_reverse_error_order',[op('HDNRUL',[0],header='',header_status=1),op('HDNPHA',[8],[1.,1.]),op('GRNMDL',[23])],guard=135+delta)
        fail=payload(([13.+delta],[],[]),[],status=1)
        put(f'd{dim}_hdntim_eof',[op('HDNTIM',[rows],part=fail)])
        put(f'd{dim}_jominy_eof',[op('JOMINY',[1,1,rows],part=payload(([10.+delta],[20.+i for i in range(rows)],[]),[3.5],rank=2,status=1))])
        put(f'd{dim}_hdnpha_eof',[op('HDNPHA',[1],[float(rows),0.],fail)])
        put(f'd{dim}_utsdat_eof',[op('UTSDAT',[1],[float(rows)],fail)])
        put(f'd{dim}_header_eof',[op('HDNTIM',[0],header='<EOF>',header_status=1,material=second)])
        put(f'd{dim}_selector_eof',[op('UTSDAT',[0],[0.],selector_eof=True)])
        put(f'd{dim}_retention',[op('HDNRUL',[83+delta]),op('GRNMDL',[999]),op('HDNTIM',[-2],part=empty),op('HDNPHA',[4],[0.,1.],payload(([],[31.+delta],[]),[],rank=2)),op('JOMINY',[1,0,1],part=payload(([],[41.+delta],[]),[],rank=2)),op('UTSDAT',[1],[-2.5],empty)],prefix=f'* DEFORM-{dim}D\nUNIT {2 if dim==3 else 1}\n',guard=137+delta)
    assert len(plan['inputs'])<=32
    dest.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8');print(json.dumps({'imports':len(plan['inputs']),'plan':str(dest)}))


if __name__=='__main__':main()
