"""Prepare source-planned OTPRNG token/conversion cases before observation."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import struct


def hx(values):return b''.join(struct.pack('<d',v)for v in values).hex()


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('cohort',choices=['prototype','training','evaluation']);a=ap.parse_args();evaluation=a.cohort=='evaluation';delta=1 if evaluation else 0
    base=Path('tests/fixtures/output-range')/a.cohort;base.mkdir(parents=True,exist_ok=True);dest=Path('analysis/output-range-'+a.cohort+'-inputs.json')
    if dest.exists():raise SystemExit('Refusing to replace prepared inputs')
    dimension=3;nstep=1;records={};mid=3+delta
    plan=dict(status='source_planned_before_native',cohort=a.cohort,ordered_imports=[],inputs=[],qualification='Complete OTPRNG240 caller within owned2D/3D wrappers,at most six logical tokens and defined decimal/finite legacy-real conversions. Header token cap,blank/missing fallbacks,optional fourth token and stale reference compared. Numeric extremes/INT_MIN,embedded NUL,uninitialized selector matching failures,allocation failure,arbitrary aliases/native ABI and full application remain pending.')
    if evaluation:plan['chronology']='Prepared after training PASS; original executes before C++ with changed IDs/values and reversed dimension/selected operation order.'

    def initial():return dict(mode_0x1eec=0,reference_0x1ef0=0,option_0x1f08=0,values_0x1ef8_hex=hx([0.,0.]))

    def create(oid):
        for i in range(len(records)+1,oid+1):records[i]=initial()

    def state():return copy.deepcopy({str(k):v for k,v in records.items()})

    def op(header,selector=0,values=(0.,0.),option=0,oid=1,status=0,selector_eof=False):return dict(header=header,selector=selector,values=list(values),option=option,oid=oid,status=status,selector_eof=selector_eof)

    def apply(o):
        oid=o['oid'];headers=[];gets=[];conversions=[];reads=[];selections=[];order=[]
        if o['selector_eof']:return 'OTPRNG',dict(id=240,status=2,headers=[],reads=[],refreshes=[],selections=[],order=[],token_gets=[],conversions=[],expected_range=state())
        line=o['header'];data='OTPRNG '+str(oid)+(''if line=='<EOF>'else(' '+line if line else'')+'\n')
        count=len(records);create(oid);selections=[dict(id=oid,before_count=count,after_count=len(records),result_id=oid if oid>0 else None)];order=['selection'];status=2
        if oid>0:
            f=records[oid];tokens=[v for v in re.split('[ ,\n\t\r]+',line)if v][:6]if o['status']==0 else[]
            if o['status']==0 and not tokens:tokens=['']
            before=copy.deepcopy(f);headers=[dict(kind='tokens',limit=6,before_fields=before,after_fields=copy.deepcopy(f),status=o['status'],token_count=len(tokens)if o['status']==0 else None,tokens=tokens)]
            reads=[dict(capacity=1024,value=None if line=='<EOF>'else(' '+line if line else'')+'\n')];order.extend(['token-enter','fgets','token-leave']);status=2 if o['status']else 0
            if status==0:
                def token(i):
                    value=tokens[i]if i<len(tokens)else'';gets.append(dict(index=i,text=value));order.append('token-get');return value
                def convert(kind,text,value=None):
                    c=dict(kind=kind,text=text,before_fields=copy.deepcopy(f))
                    if kind=='integer':c['value']=value
                    conversions.append(c);order.append(kind+'-convert')
                convert('integer',token(0),o['selector'])
                convert('real',token(1));old=struct.unpack('<dd',bytes.fromhex(f['values_0x1ef8_hex']));f['values_0x1ef8_hex']=hx([o['values'][0],old[1]])
                convert('real',token(2));f['values_0x1ef8_hex']=hx(o['values'])
                if len(tokens)>=4:convert('integer',token(3),o['option']);f['option_0x1f08']=o['option']
                else:f['option_0x1f08']=0
                f['mode_0x1eec']=3 if o['selector']<0 else o['selector']
                if o['selector']<0:f['reference_0x1ef0']=-o['selector']
        return data,dict(id=240,status=status,headers=headers,reads=reads,refreshes=[],selections=selections,order=order,token_gets=gets,conversions=conversions,expected_range=state())

    def put(label,ops=(),prefix='',switch=None,new_object=None,guard=None):
        nonlocal dimension,nstep,records
        if switch is not None and switch!=dimension:dimension=switch;nstep=1;records={}
        if new_object is not None:create(new_object)
        data=prefix;handlers=[]
        for o in ops:
            text,h=apply(o);data+=text;handlers.append(h)
        if guard is not None:data+=f'NSTEP {guard}\n';nstep=guard
        b=data.encode('ascii');name=f'{len(plan["inputs"]):02d}_{label}.KEY';(base/name).write_bytes(b)
        plan['ordered_imports'].append(name);plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(b).hexdigest(),expected_handlers=handlers,expected_range=state(),expected_nstep=nstep,expected_wrapper_status=next((h['status']for h in reversed(handlers)if h['status']),0)))

    def full(selector,first,second,fourth,oid=1):return op(f'{selector} {first} {second} {fourth}',selector,(first,second),fourth,oid)

    mesh3=Path('tests/fixtures/custom_fields/01_spatial_mesh.KEY').read_text().split('\n',1)[1];mesh2='OBJTYP 1 2 0\nRZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1 4\n1 1 2 3 4\n'
    for dim in ([2,3]if evaluation else[3,2]):
        put(f'd{dim}_reset',prefix=f'* DEFORM-{dim}D\nUNIT 2\nUNIT 1\n'+(mesh3 if dim==3 else mesh2),switch=dim,new_object=1)
        put(f'd{dim}_positive',[full(1+delta,1.25+delta,2.5+delta,4+delta,oid)for oid in([mid,1]if evaluation else[1,mid])],guard=261+delta)
        put(f'd{dim}_references',[full(-7-delta,3.5,4.5,9),full(2,5.5,6.5,3),full(3,7.5,8.5,5),full(-5-delta,9.5,10.5,7),op('-0 1.25 2.5',0,(1.25,2.5))],guard=263+delta)
        put(f'd{dim}_lexical',[op('010 -0.0 1.5D+1 077',10,(-0.,15.),77),op('0x2 1e+ 0x1p4 0x20',0,(1.,0.),0),op('2e3 1d2 2E+ 3.9',2,(100.,2.),3)],guard=265+delta)
        put(f'd{dim}_missing',[op('5',5),op('-2 7.5',-2,(7.5,0.)),op('2 8.5 9.5',2,(8.5,9.5))],guard=267+delta)
        put(f'd{dim}_blank',[op(''),op(' ,\t ')],guard=269+delta)
        put(f'd{dim}_cap',[op('1 2.5 3.5 4 ignored1 ignored2',1,(2.5,3.5),4),op('2 4.5 5.5 6 ignored1 ignored2 999 1000',2,(4.5,5.5),6),op('3 6.5 7.5 8 ignored',3,(6.5,7.5),8)],guard=271+delta)
        put(f'd{dim}_unit_reseed',[full(-9,11.5+delta,12.5+delta,10,5),full(-11,13.5+delta,14.5+delta,12)],prefix='UNIT 2\n',guard=273+delta)
        put(f'd{dim}_deactivate',[full(2,15.5+delta,16.5+delta,13,mid)],prefix=f'OBJTYP 1 0 0\nOBJTYP {mid} 0 0\n',guard=275+delta)
        put(f'd{dim}_same_dimension',[full(0,17.5+delta,18.5+delta,14)],prefix=f'* DEFORM-{dim}D\nUNIT 1\n'+(mesh3 if dim==3 else mesh2),guard=277+delta)
        put(f'd{dim}_matching',[op('bad nope other bad',0,(0.,0.),0),op('--2 + 5tail nope',0,(0.,5.),0)],guard=279+delta)
        put(f'd{dim}_stale_optional',[full(-13,19.5+delta,20.5+delta,15),op('3 21.5 22.5',3,(21.5,22.5)),full(1,23.5+delta,24.5+delta,16)],guard=281+delta)
        put(f'd{dim}_header_eof',[op('<EOF>',status=1)])
        put(f'd{dim}_new_header_eof',[op('<EOF>',oid=6,status=1)])
        put(f'd{dim}_selector_failures',[op('0 0 0',oid=0),op('0 0 0',oid=-3),op('',selector_eof=True)])
        put(f'd{dim}_header_recovery',[full(2,25.5+delta,26.5+delta,17),op('')],guard=283+delta)
        if a.cohort=='prototype':break
    if a.cohort=='prototype':
        plan['inputs']=plan['inputs'][:10];plan['ordered_imports']=[v['file']for v in plan['inputs']]
        for p in base.glob('*.KEY'):
            if p.name not in plan['ordered_imports']:p.unlink()
    assert len(plan['inputs'])<=32
    dest.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8');print(json.dumps({'imports':len(plan['inputs']),'plan':str(dest)}))


if __name__=='__main__':main()
