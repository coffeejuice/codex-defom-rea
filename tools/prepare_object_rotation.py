"""Prepare source-planned ROTSYM/TRGVOL cases without native outputs."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import struct


def hx(values):return b''.join(struct.pack('<d',v)for v in values).hex()


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('cohort',choices=['prototype','training','evaluation']);a=ap.parse_args();evaluation=a.cohort=='evaluation';delta=1 if evaluation else 0
    base=Path('tests/fixtures/object-rotation')/a.cohort;base.mkdir(parents=True,exist_ok=True);dest=Path('analysis/object-rotation-'+a.cohort+'-inputs.json')
    if dest.exists():raise SystemExit('Refusing to replace prepared inputs')
    dimension=3;nstep=1;records={};mid=3+delta
    plan=dict(status='source_planned_before_native',cohort=a.cohort,ordered_imports=[],inputs=[],qualification='Complete ROTSYM258/TRGVOL259 import cases in normal owned2D/3D wrappers and bounded signed object selectors. ROTSYM fields uninitialized in2D are neither read nor fabricated. Scientific consumers,numeric extremes,uninitialized selector matching failures,allocation failure,arbitrary aliases/native ABI and complete application remain pending.')
    if evaluation:plan['chronology']='Prepared after training PASS; original executes before C++ with changed IDs/values and reversed dimension/paired-keyword order.'

    def create(oid):
        for i in range(len(records)+1,oid+1):records[i]=dict(r=[0.]*7 if dimension==3 else None,v=0.,m=-1)

    def state():return {str(k):dict(rotation_symmetry=dict(scalar_0x2058_hex=hx(v['r'][:1]),vector_0x2060_hex=hx(v['r'][1:4]),vector_0x2078_hex=hx(v['r'][4:]))if v['r']is not None else None,target_volume=dict(mode_0x2098=v['m'],value_0x2090_hex=hx([v['v']])))for k,v in records.items()}

    def part(values,header=None,status=0):return dict(values=list(values),header=' '.join(map(str,values))if header is None else header,status=status)

    def rotation(oid=1,parts=None,selector_eof=False):
        if parts is None:parts=[part([1.25+delta]),part([2.5+delta,3.5+delta,4.5+delta]),part([5.5+delta,6.5+delta,7.5+delta])]
        return dict(k='ROTSYM',id=258,oid=oid,parts=parts,selector_eof=selector_eof)

    def volume(oid=1,integer=7,value=8.5,header=None,status=0):return dict(k='TRGVOL',id=259,oid=oid,integer=integer,parts=[part([value],f'{integer} {value}'if header is None else header,status)],selector_eof=False)

    def apply(o):
        kid=o['id'];oid=o['oid'];headers=[];reads=[];selections=[];order=[]
        if o['selector_eof']:return o['k'],dict(id=kid,status=2,headers=[],reads=[],refreshes=[],selections=[],order=[],expected_rotation=state())
        count=len(records);create(oid);selections=[dict(id=oid,before_count=count,after_count=len(records),result_id=oid if oid>0 else None)];order=['selection']
        data=o['k']+' '+str(oid)
        for i,p in enumerate(o['parts']):
            if p['header']=='<EOF>':break
            data+=(' '+p['header']if i==0 and p['header']else p['header']if i else'')+'\n'
        status=2 if oid<1 else 0
        if oid>0 and(kid==259 or dimension==3):
            f=records[oid]
            for i,p in enumerate(o['parts']):
                before_values=[f['v']]if kid==259 else f['r'][:1]if i==0 else f['r'][4:]if i==1 else f['r'][1:4]
                before=dict(integers=[f['m']]if kid==259 else[],reals_hex=[hx([v])for v in before_values])
                after=dict(integers=[o['integer']]if kid==259 else[],reals_hex=[hx([v])for v in p['values']])
                headers.append(dict(kind='intreal'if kid==259 else'real'if i==0 else'triple',object_id=oid,before=before,after=after,status=p['status']))
                line=p['header'];reads.append(dict(capacity=1024,value=None if line=='<EOF>'else(' '+line if i==0 and line else line)+'\n'));order.extend(['header-enter','fgets','header-leave'])
                if kid==259:f['v']=p['values'][0];f['m']=-1 if f['v']==0.0 else o['integer']
                elif i==0:f['r'][0]=p['values'][0]
                elif i==1:f['r'][4:]=p['values']
                else:f['r'][1:4]=p['values']
                if p['status']:status=2;break
        return data,dict(id=kid,status=status,headers=headers,reads=reads,refreshes=[],selections=selections,order=order,expected_rotation=state())

    def put(label,ops=(),prefix='',switch=None,new_object=None,guard=None):
        nonlocal dimension,nstep,records
        if switch is not None and switch!=dimension:dimension=switch;nstep=1;records={}
        if new_object is not None:create(new_object)
        data=prefix;handlers=[]
        for o in ops:
            text,h=apply(o);data+=text;handlers.append(h)
        if guard is not None:data+=f'NSTEP {guard}\n';nstep=guard
        b=data.encode('ascii');name=f'{len(plan["inputs"]):02d}_{label}.KEY';(base/name).write_bytes(b)
        plan['ordered_imports'].append(name);plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(b).hexdigest(),expected_handlers=handlers,expected_rotation=state(),expected_nstep=nstep,expected_wrapper_status=next((h['status']for h in reversed(handlers)if h['status']),0)))

    def paired(oid):return [volume(oid,integer=9+delta,value=12.5+delta),rotation(oid)]if evaluation else[rotation(oid),volume(oid,integer=9+delta,value=12.5+delta)]

    mesh3=Path('tests/fixtures/custom_fields/01_spatial_mesh.KEY').read_text().split('\n',1)[1];mesh2='OBJTYP 1 2 0\nRZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1 4\n1 1 2 3 4\n'
    for dim in ([2,3]if evaluation else[3,2]):
        put(f'd{dim}_reset',prefix=f'* DEFORM-{dim}D\nUNIT 2\nUNIT 1\n'+(mesh3 if dim==3 else mesh2),switch=dim,new_object=1)
        put(f'd{dim}_positive',[*paired(1),*paired(mid),volume(mid,integer=0,value=0.),volume(mid,integer=-3,value=-8.5)],guard=221+delta)
        put(f'd{dim}_partial',[rotation(parts=[part([-0.],'-0.0 trailing'),part([1.25,0.,0.],'1.25,bad,9'),part([2.5,0.,0.],'2.5')]),volume(1,integer=5,value=0.,header='5'),volume(mid,integer=7,value=-0.,header='0x7,-0.0 tail')],guard=223+delta)
        put(f'd{dim}_matching',[rotation(parts=[part([0.],'bad'),part([0.,0.,0.],'bad'),part([0.,0.,0.],'bad')]),volume(mid,integer=0,value=0.,header='bad')],guard=225+delta)
        put(f'd{dim}_blank_first',[rotation(),rotation(parts=[part([0.],'',1)]),volume(1,integer=0,value=0.,header='',status=1)],guard=227+delta)
        put(f'd{dim}_blank_second',[rotation(),rotation(parts=[part([18.5+delta]),part([0.,0.,0.],'',1)])],guard=229+delta)
        put(f'd{dim}_blank_third',[rotation(),rotation(parts=[part([19.5+delta]),part([20.5,21.5,22.5]),part([0.,0.,0.],'',1)])],guard=231+delta)
        put(f'd{dim}_unit_reseed',[*paired(5),*paired(1)],prefix='UNIT 2\n',guard=233+delta)
        put(f'd{dim}_deactivate',[volume(mid,integer=13,value=15.5+delta)],prefix=f'OBJTYP 1 0 0\nOBJTYP {mid} 0 0\n',guard=235+delta)
        put(f'd{dim}_same_dimension',paired(1),prefix=f'* DEFORM-{dim}D\nUNIT 1\n'+(mesh3 if dim==3 else mesh2),guard=237+delta)
        put(f'd{dim}_eof_first',[rotation(parts=[part([0.],'<EOF>',1)])])
        put(f'd{dim}_eof_second',[rotation(parts=[part([31.5+delta]),part([0.,0.,0.],'<EOF>',1)])])
        put(f'd{dim}_eof_third',[rotation(parts=[part([32.5+delta]),part([33.5,34.5,35.5]),part([0.,0.,0.],'<EOF>',1)])])
        put(f'd{dim}_volume_eof',[volume(5,integer=0,value=0.,header='<EOF>',status=1)])
        put(f'd{dim}_new_header_eof',[volume(6,integer=0,value=0.,header='<EOF>',status=1)])
        put(f'd{dim}_selector_failures',[rotation(0),volume(-3,integer=0,value=0.),rotation(selector_eof=True)])
        if a.cohort=='prototype':break
    if a.cohort=='prototype':
        plan['inputs']=plan['inputs'][:10];plan['ordered_imports']=[v['file']for v in plan['inputs']]
        for p in base.glob('*.KEY'):
            if p.name not in plan['ordered_imports']:p.unlink()
    assert len(plan['inputs'])<=32
    dest.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8');print(json.dumps({'imports':len(plan['inputs']),'plan':str(dest)}))


if __name__=='__main__':main()
