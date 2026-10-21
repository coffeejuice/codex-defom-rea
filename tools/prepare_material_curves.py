"""Prepare source-planned material table cases without reading native outputs."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import struct

IDS={'FPERV':104,'DIFCOE':107,'DIFBND':129}


def hx(values):return b''.join(struct.pack('<d',x)for x in values).hex()


def table(kind=0):
    rank,selectors,width={0:(0,[0,0,0],1),0x29:(2,[3,12,0],1),0x27:(1,[3,0,0],2),0x28:(1,[12,0,0],2),0xaf:(0,[0,0,0],2)}[kind]
    return dict(metadata=[kind,rank,*selectors,0,0,0,width,0,0,0],constant_hex=hx([0.]),preserved60_hex='00'*8,
                axes_hex=['','',''],dependent_hex=hx([0.,0.])if kind==0xaf else'')


def initial():
    tables={'0x1ef0':table(),'0x1f68':table(),'0x958':table(0x29),'0x9d0':table(0x27),'0xa48':table(0x28),'0x2160':table(0xaf)}
    tables['0x958']['metadata'][1]=0
    return dict(difcoe_mode_0x950=0,difcoe_reference_0xac0=0,tables=tables)


def payload(axes=((),(),()),dependent=(),rank=1,width=1,status=0,tail='\n'):
    values=[]
    if rank==1:
        for i,x in enumerate(axes[0]):values.extend([x,*dependent[i*width:(i+1)*width]])
    else:values=[*axes[0],*axes[1],*axes[2],*dependent]
    text=' '.join(map(str,values))
    if values and status==0:text+=tail if tail is not None else''
    return dict(axes=[list(v)for v in axes],dependent=list(dependent),status=status,text=text,tail=tail)


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('cohort',choices=['training','evaluation']);a=ap.parse_args();evaluation=a.cohort=='evaluation';delta=1 if evaluation else 0
    base=Path('tests/fixtures/material-curves')/a.cohort;base.mkdir(parents=True,exist_ok=True)
    dest=Path('analysis/material-curves-'+a.cohort+'-inputs.json')
    if dest.exists():raise SystemExit('Refusing to replace a prepared cohort')
    dimension=3;nstep=1;records={};ident=5+delta;second=-9-delta
    plan=dict(status='source_planned_before_native',cohort=a.cohort,ordered_imports=[],inputs=[],qualification='Three complete dispatcher cases within defined finite signed32 conversions and owned bounded rank0/1/2 table profiles. Scientific evaluation, general numerical domains, arbitrary aliases/native ABI and complete application remain pending.')
    if evaluation:plan['chronology']='Prepared after training PASS; original executes before C++ on changed values/IDs/extents and reversed dimension order.'

    def curves():return copy.deepcopy({str(k):v for k,v in records.items()})

    def op(k,values,parts=(),header=None,header_status=0,material=None,selector_eof=False):
        return dict(k=k,values=values,parts=list(parts),header=header,header_status=header_status,material=ident if material is None else material,selector_eof=selector_eof)

    def apply(o):
        kid=IDS[o['k']];mid=o['material'];values=o['values'];hs=o['header_status'];parts=iter(o['parts']);calls=[];actions=[];reads=[]
        if o['selector_eof']:return o['k'],dict(id=kid,status=2,calls=[],headers=[],reads=[],refreshes=[],expected_curves=curves(),expected_actions=[])
        is_new=mid not in records
        if is_new:records[mid]=initial()
        f=records[mid];tables=f['tables'];status=hs
        kind='quad'if kid==104 else'int4'if kid==107 else'int2'
        integers=[]if kid==104 else[values[0]];reals=values if kid==104 else values[1:]
        header=o['header']
        if header is None:header=' '.join(map(str,values))
        data=o['k']+' '+str(mid)+(''if header=='<EOF>'else(' '+header if header else'')+'\n')
        reads.append(dict(capacity=1024,value=None if header=='<EOF>'else(' '+header if header else'')+'\n'))
        headers=[dict(kind=kind,integers=integers,reals_hex=[hx([v])for v in reals],status=hs)]

        def reset(slot,kind,preserve=True):
            old=tables[slot];new=table(kind)
            if preserve:new['metadata'][10]=old['metadata'][10];new['preserved60_hex']=old['preserved60_hex']
            tables[slot]=new
            if preserve:actions.append(dict(kind='reset-preserve',slot=slot))
            actions.append(dict(kind='reset-direct',slot=slot));return new

        def read(slot):
            nonlocal data
            spec=next(parts);data+=spec['text'];t=tables[slot];entry=copy.deepcopy(t);m=t['metadata'];rank=m[1];width=max(1,m[8]);m[8]=width
            for i in [5,6,7]:m[i]=max(0,m[i])
            counts=m[5:8];axis_counts=[counts[i]if i<rank else 0 for i in range(3)];size=width
            for i in range(rank):size*=counts[i]
            if rank==0 and width<=1:size=0
            t['axes_hex']=[hx([*spec['axes'][i],*([0.]*(n-len(spec['axes'][i])))])if n else''for i,n in enumerate(axis_counts)]
            dependent=[0.]*size
            if rank==1 or spec['status']==0:dependent[:len(spec['dependent'])]=spec['dependent']
            t['dependent_hex']=hx(dependent)
            if spec['status']==0 and size>0:reads.append(dict(capacity=1024,value=spec['tail']))
            calls.append(dict(slot=slot,material_id=mid,entry=entry,after=copy.deepcopy(t),status=spec['status']))
            actions.extend([dict(kind='payload-enter',slot=slot),dict(kind='payload-leave',slot=slot)])
            return spec['status']

        if hs==0:
            if kid==104:
                reset('0x1ef0',0);reset('0x1f68',0)
                for i,slot in enumerate(['0x1ef0','0x1f68']):
                    value,mode=values[i*2:i*2+2];mode=int(mode);t=tables[slot]
                    if mode==0:t['constant_hex']=hx([value])
                    elif mode==1:
                        t['metadata'][1]=1;t['metadata'][2]=1;t['metadata'][5]=int(value);status=read(slot)
            elif kid==107:
                mode=values[0]
                if 0<=mode<=3:
                    f['difcoe_mode_0x950']=mode;slot='0x958'if mode<2 else'0x9d0'if mode==2 else'0xa48'
                    if mode==0:reset(slot,0,False)['constant_hex']=hx([values[1]])
                    else:
                        t=reset(slot,{1:0x29,2:0x27,3:0x28}[mode]);t['metadata'][5]=int(values[1])
                        if mode==1:t['metadata'][6]=int(values[2])
                        status=read(slot)
                elif mode<0:f['difcoe_mode_0x950']=4;f['difcoe_reference_0xac0']=-mode
            else:
                t=tables['0x2160'];mode=values[0]
                if mode==7:
                    t['metadata'][1]=2;t['metadata'][2:4]=[3,4];t['metadata'][5:7]=[int(values[1]),int(values[2])];status=read('0x2160')
                elif mode==0:t['metadata'][5:7]=[0,0];t['constant_hex']=hx([values[1]])
                else:status=1
        assert next(parts,None)is None,'unused source-planned payload'
        return data,dict(id=kid,status=2 if status else 0,calls=calls,headers=headers,reads=reads,refreshes=[1]if is_new else[],expected_curves=curves(),expected_actions=actions)

    def put(label,ops=(),prefix='',switch=None,guard=None):
        nonlocal dimension,nstep,records
        if switch is not None and switch!=dimension:dimension=switch;nstep=1;records={}
        data=prefix;handlers=[]
        for o in ops:
            text,h=apply(o);data+=text;handlers.append(h)
        if guard is not None:data+=f'NSTEP {guard}\n';nstep=guard
        b=data.encode('ascii');name=f'{len(plan["inputs"]):02d}_{label}.KEY';(base/name).write_bytes(b)
        plan['ordered_imports'].append(name);plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(b).hexdigest(),expected_handlers=handlers,expected_curves=curves(),expected_nstep=nstep,expected_wrapper_status=next((h['status']for h in reversed(handlers)if h['status']),0)))

    mesh3=Path('tests/fixtures/custom_fields/01_spatial_mesh.KEY').read_text()
    mesh2='* DEFORM-2D\nOBJTYP 1 2 0\nRZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1 4\n1 1 2 3 4\n'
    for dim in ([2,3]if evaluation else[3,2]):
        put(f'd{dim}_reset',prefix=mesh3 if dim==3 else mesh2,switch=dim)
        p1=payload(([10.+delta,20.+delta],[],[]),[1.5+delta,2.5+delta]);p2=payload(([30.+delta],[],[]),[3.5+delta])
        d2=payload(([10.+delta,20.+delta],[.1+delta],[]),[.5+delta,.75+delta],rank=2)
        b2=payload(([40.+delta],[50.+delta],[]),[1.25+delta,1.5+delta],rank=2,width=2)
        put(f'd{dim}_positive',[op('FPERV',[2,1,1,1],[p1,p2]),op('DIFCOE',[1,2,1,0,0],[d2]),op('DIFBND',[7,1,1],[b2])],guard=81+delta)
        put(f'd{dim}_inactive_and_constants',[op('DIFCOE',[2,1,0,0,0],[payload(([10.+delta],[],[]),[2.+delta,3.+delta],width=2)]),op('DIFCOE',[3,1,0,0,0],[payload(([20.+delta],[],[]),[4.+delta,5.+delta],width=2)]),op('DIFCOE',[-9-delta,0,0,0,0]),op('DIFBND',[0,6.5+delta,0]),op('FPERV',[.25+delta,0,-0.,0])],guard=83+delta)
        put(f'd{dim}_selectors',[op('FPERV',[2.9,1.9,-2.5,1.1],[p1,payload()]),op('FPERV',[99,2,88,-1]),op('DIFCOE',[5,19,23,29,31]),op('DIFBND',[2,3,4])],guard=85+delta)
        put(f'd{dim}_stale_boundary',[op('DIFBND',[7,1,1],[b2]),op('DIFBND',[0,-0.,0]),op('DIFCOE',[0,.125+delta,0,0,0])],prefix=f'UNIT {2 if dim==3 else 1}\n',guard=87+delta)
        put(f'd{dim}_partial_headers',[op('FPERV',[7,0,0,0],header='7'),op('DIFCOE',[1,0,0,0,0],[payload(rank=2)],header='1'),op('DIFBND',[7,0,0],[payload(rank=2)],header='7')],guard=89+delta)
        put(f'd{dim}_matching',[op('FPERV',[0,0,0,0],header='bad'),op('DIFCOE',[0,0,0,0,0],header='bad'),op('DIFBND',[0,0,0],header='bad')],guard=91+delta)
        put(f'd{dim}_blank',[op('FPERV',[0,0,0,0],header='',header_status=1),op('DIFCOE',[0,0,0,0,0],header='',header_status=1),op('DIFBND',[0,0,0],header='',header_status=1)],guard=93+delta)
        fail=payload(([17.+delta],[],[]),[],status=1)
        put(f'd{dim}_masked_first_error',[op('FPERV',[1,1,0,1],[fail,payload()])])
        put(f'd{dim}_preserved_first_error',[op('FPERV',[1,1,9.25+delta,0],[fail])])
        put(f'd{dim}_both_errors',[op('FPERV',[1,1,1,1],[fail,payload(status=1)])])
        put(f'd{dim}_staged_failure',[op('DIFCOE',[1,1,1,0,0],[payload(([10.+delta],[20.+delta],[]),[],rank=2,status=1)])])
        put(f'd{dim}_boundary_failure',[op('DIFBND',[7,1,1],[payload(([10.+delta],[20.+delta],[]),[3.25+delta],rank=2,width=2,status=1)])])
        put(f'd{dim}_header_eof',[op('FPERV',[0,0,0,0],header='<EOF>',header_status=1,material=second)])
        put(f'd{dim}_selector_eof',[op('DIFCOE',[0,0,0,0,0],selector_eof=True)])
        put(f'd{dim}_retention',[op('DIFCOE',[-11-delta,0,0,0,0]),op('DIFBND',[0,8.25+delta,0])],prefix=f'* DEFORM-{dim}D\n',guard=95+delta)
    assert len(plan['inputs'])<=32
    dest.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8');print(json.dumps({'imports':len(plan['inputs']),'plan':str(dest)}))


if __name__=='__main__':main()
