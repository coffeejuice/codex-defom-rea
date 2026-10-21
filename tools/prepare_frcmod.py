"""Prepare source-planned FRCMOD cases without reading native observations."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import struct

ORDER=[0,1,4,5,6,7,8,2,3]


def hx(values):return b''.join(struct.pack('<d',x)for x in values).hex()


def table(kind):
    rank,selector={0x1b:(0,0),0x1c:(1,31),0xb4:(1,32)}[kind]
    return dict(metadata=[kind,rank,selector,0,0,0,0,0,1,0,0,0],constant_hex=hx([0.]),preserved60_hex='00'*8,axes_hex=['','',''],dependent_hex='')


def initial():
    return dict(mode=0,reference=0,scalars=[0.]*6,last=0.,coefficients=[0.]*9,tables={'0x1b10':table(0x1b),'0x1b90':table(0x1c),'0x1c08':table(0xb4)})


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('cohort',choices=['training','evaluation']);a=ap.parse_args();evaluation=a.cohort=='evaluation';delta=1 if evaluation else 0
    base=Path('tests/fixtures/frcmod')/a.cohort;base.mkdir(parents=True,exist_ok=True);dest=Path('analysis/frcmod-'+a.cohort+'-inputs.json')
    if dest.exists():raise SystemExit('Refusing to replace prepared inputs')
    dimension=3;nstep=1;records={};mid=5+delta;second=-9-delta
    plan=dict(status='source_planned_before_native',cohort=a.cohort,ordered_imports=[],inputs=[],qualification='All FRCMOD selector paths in the defined finite signed32, owned normal-wrapper profile. Numeric extremes, arbitrary aliases, scientific evaluation and complete app/native ABI remain pending.')
    if evaluation:plan['chronology']='Prepared after training PASS; original must run before C++ on changed values/IDs/extents, selector/prefix order and reversed dimension order.'

    def state():
        return copy.deepcopy({str(k):dict(mode_0x1ad0=v['mode'],reference_0x1b08=v['reference'],scalars_0x1ad8_hex=hx(v['scalars']),scalar_0x1b88_hex=hx([v['last']]),coefficients_0x1c80_hex=hx(v['coefficients']),tables=v['tables'])for k,v in records.items()})

    def op(mode,values=None,header=None,header_status=0,material=mid,coefficient_line=None,coefficient_values=None,coefficient_status=0,axes=(),dependent=(),payload_status=0,selector_eof=False):
        if values is None:values=[mode+.125+delta,mode+.25+delta,mode+.375+delta,mode+.5+delta,mode+.625+delta]
        return dict(mode=mode,values=values,header=header,header_status=header_status,material=material,coefficient_line=coefficient_line,coefficient_values=coefficient_values,coefficient_status=coefficient_status,axes=list(axes),dependent=list(dependent),payload_status=payload_status,selector_eof=selector_eof)

    def apply(o):
        material=o['material'];mode=o['mode'];values=o['values'];hs=o['header_status'];calls=[];coefficient_reads=[];reads=[];actions=[]
        if o['selector_eof']:return 'FRCMOD',dict(id=105,status=2,calls=[],headers=[],coefficient_reads=[],reads=[],refreshes=[],expected_frcmod=state(),expected_actions=[])
        is_new=material not in records
        if is_new:records[material]=initial()
        f=records[material];header=o['header']
        if header is None:header=' '.join(map(str,[mode,*values]))
        data='FRCMOD '+str(material)+(''if header=='<EOF>'else(' '+header if header else'')+'\n')
        reads.append(dict(capacity=1024,value=None if header=='<EOF>'else(' '+header if header else'')+'\n'))
        headers=[dict(kind='int5',integers=[mode],reals_hex=[hx([v])for v in values],status=hs)];status=hs
        if hs==0:
            f['scalars'][5]=values[0];f['last']=values[4]
            if 0<=mode<=10:
                f['mode']=mode
                if mode in[2,4,5]:f['scalars'][{2:0,4:1,5:2}[mode]]=values[1]
                if mode==7:f['scalars'][3:5]=values[1:3]
            elif mode==11:
                f['mode']=12;line=o['coefficient_line'];before=[hx([f['coefficients'][i]])for i in ORDER];assigned=list(o['coefficient_values']or[]);assigned+=[0.]*(9-len(assigned))
                for i,j in enumerate(ORDER):f['coefficients'][j]=assigned[i]
                status=o['coefficient_status'];coefficient_reads=[dict(material_id=material,before_hex=before,values_hex=[hx([v])for v in assigned],status=status)]
                reads.append(dict(capacity=1024,value=line));data+=line or'';actions=[dict(kind='coefficients-enter'),dict(kind='coefficients-leave')]
            elif mode in[12,17]:
                f['mode']=13 if mode==12 else 17
                if mode==12:f['last']=100.
                slot='0x1b90'if mode==12 else'0x1c08';t=f['tables'][slot];m=t['metadata'];m[5]=int(values[1])
                if mode==12:m[1]=1;m[2]=31
                entry=copy.deepcopy(t);m[5]=max(0,m[5]);rows=m[5];axes=o['axes'];dependent=o['dependent'];t['axes_hex']=[hx([*axes,*([0.]*(rows-len(axes)))]),'',''];t['dependent_hex']=hx([*dependent,*([0.]*(rows-len(dependent)))])
                stream=[]
                for i,v in enumerate(axes):
                    stream.append(v)
                    if i<len(dependent):stream.append(dependent[i])
                text=' '.join(map(str,stream));status=o['payload_status']
                if stream and status==0:text+='\n'
                data+=text
                if status==0 and rows:reads.append(dict(capacity=1024,value='\n'))
                calls=[dict(slot=slot,material_id=material,entry=entry,after=copy.deepcopy(t),status=status)];actions=[dict(kind='payload-enter',slot=slot),dict(kind='payload-leave',slot=slot)]
            elif mode==15:f['mode']=15;f['scalars'][2]=values[1]
            elif mode==16:f['mode']=16
            elif mode<0:f['mode']=11;f['reference']=-mode
        return data,dict(id=105,status=2 if status else 0,calls=calls,headers=headers,coefficient_reads=coefficient_reads,reads=reads,refreshes=[1]if is_new else[],expected_frcmod=state(),expected_actions=actions)

    def put(label,ops=(),prefix='',switch=None,guard=None):
        nonlocal dimension,nstep,records
        if switch is not None and switch!=dimension:dimension=switch;nstep=1;records={}
        data=prefix;handlers=[]
        for o in ops:
            text,h=apply(o);data+=text;handlers.append(h)
        if guard is not None:data+=f'NSTEP {guard}\n';nstep=guard
        b=data.encode('ascii');name=f'{len(plan["inputs"]):02d}_{label}.KEY';(base/name).write_bytes(b)
        plan['ordered_imports'].append(name);plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(b).hexdigest(),expected_handlers=handlers,expected_frcmod=state(),expected_nstep=nstep,expected_wrapper_status=next((h['status']for h in reversed(handlers)if h['status']),0)))

    mesh3=Path('tests/fixtures/custom_fields/01_spatial_mesh.KEY').read_text();mesh2='* DEFORM-2D\nOBJTYP 1 2 0\nRZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1 4\n1 1 2 3 4\n'
    put('initial',prefix='\n')
    for dim in ([2,3]if evaluation else[3,2]):
        rows=2+delta;axes=[10.+i*10+delta for i in range(rows)];dependent=[1.5+i+delta for i in range(rows)]
        axes17=axes[:1+delta];values17=dependent[:1+delta]
        put(f'd{dim}_reset',prefix=mesh3 if dim==3 else mesh2,switch=dim)
        put(f'd{dim}_selectors_0_10',[op(i)for i in (reversed(range(11))if evaluation else range(11))],guard=101+delta)
        nine=[float(i+delta)for i in range(1,10)]
        put(f'd{dim}_coefficients_and_tables',[op(11,coefficient_line=' '.join(map(str,nine))+'\n',coefficient_values=nine),op(12,values=[21.+delta,float(rows),31.,41.,51.],axes=axes,dependent=dependent),op(17,values=[22.+delta,float(len(axes17)),32.,42.,52.],axes=axes17,dependent=values17)],guard=103+delta)
        put(f'd{dim}_negative_unknown',[op(-17-delta),op(13),op(14),op(99),op(15),op(16)],guard=105+delta)
        put(f'd{dim}_nonpositive_tables',[op(12,values=[4.,0.,6.,7.,8.]),op(17,values=[5.,-3.5,7.,8.,9.])],guard=107+delta)
        put(f'd{dim}_partial_headers',[op(2,values=[0.]*5,header='2'),op(7,values=[123.+delta,456.+delta,0.,0.,0.],header=f'7 {123.+delta} {456.+delta}'),op(0,values=[0.]*5,header='bad')],guard=109+delta)
        put(f'd{dim}_blank_headers',[op(0,values=[0.]*5,header='',header_status=1),op(0,values=[0.]*5,header='',header_status=1,material=second)],guard=111+delta)
        prefixes=[]
        for count in (reversed(range(10))if evaluation else range(10)):
            vals=[float(i+.25+delta)for i in range(count)];line=' '.join(map(str,vals))+'\n';prefixes.append(op(11,coefficient_line=line,coefficient_values=vals,coefficient_status=1 if count==0 else 0))
        put(f'd{dim}_coefficient_prefixes',prefixes,guard=113+delta)
        put(f'd{dim}_coefficient_lexical',[op(11,coefficient_line='bad\n',coefficient_values=[]),op(11,coefficient_line='2.5 -0 bad\n',coefficient_values=[2.5,-0.]),op(11,coefficient_line='1e+,-0,3,4,5,6,7,8,9\n',coefficient_values=[1.,-0.,3.,4.,5.,6.,7.,8.,9.])],guard=115+delta)
        put(f'd{dim}_coefficient_eof',[op(11,coefficient_status=1)])
        put(f'd{dim}_table12_eof',[op(12,values=[11.,float(rows),13.,14.,15.],axes=[13.+delta],payload_status=1)])
        put(f'd{dim}_table17_eof',[op(17,values=[12.,float(len(axes17)),14.,15.,16.],axes=[17.+delta],payload_status=1)])
        put(f'd{dim}_header_eof',[op(0,values=[0.]*5,header='<EOF>',header_status=1)])
        put(f'd{dim}_selector_eof',[op(0,selector_eof=True)])
        put(f'd{dim}_retention',[op(12,values=[17.,rows+.9,19.,20.,21.],axes=axes,dependent=dependent),op(17,values=[18.,len(axes17)+.8,20.,21.,22.],axes=axes17,dependent=values17),op(13)],prefix=f'* DEFORM-{dim}D\nUNIT {2 if dim==3 else 1}\n',guard=117+delta)
    assert len(plan['inputs'])<=32
    dest.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8');print(json.dumps({'imports':len(plan['inputs']),'plan':str(dest)}))


if __name__=='__main__':main()
