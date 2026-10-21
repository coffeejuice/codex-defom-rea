"""Prepare source-planned material direct-field imports without native outputs."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import struct

IDS={'FRAE2H':93,'LTHEAT':101,'TMPSOL':102,'TMPLIQ':103,'MATDEN':124,'GBMOBI':132}
SLOTS={93:0,102:2,103:3,124:4}


def hx(values):return b''.join(struct.pack('<d',v)for v in values).hex()


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('cohort',choices=['training','evaluation']);a=ap.parse_args()
    evaluation=a.cohort=='evaluation';delta=1 if evaluation else 0
    base=Path('tests/fixtures/material-scalars')/a.cohort;base.mkdir(parents=True,exist_ok=True)
    dest=Path('analysis/material-scalars-'+a.cohort+'-inputs.json')
    if dest.exists():raise SystemExit('Refusing to replace a prepared cohort')
    dimension=3;nstep=1;records={};first=5+delta;second=-9-delta;ignored=7+delta;fresh=500+delta
    plan=dict(status='source_planned_before_native',cohort=a.cohort,ordered_imports=[],inputs=[],
              qualification='Six material dispatcher cases, normal wrapper option0, defined bounded decimals/integers, owned material records and constant neighboring tables. General numerical/alias/native ABI domains and scientific consumers remain pending.')
    if evaluation:plan['chronology']='Prepared after training PASS; original must execute before C++ on changed IDs, values, operation order and dimension order.'

    def fields():
        return {str(k):dict(scalars_0x1ec8_hex=hx(v[0]),gbmobi_0x1868_hex=hx(v[1]))for k,v in records.items()}

    def op(keyword,ident,payload,values,status=0,selector_eof=False):
        return dict(keyword=keyword,ident=ident,payload=payload,values=values,status=status,selector_eof=selector_eof)

    def apply(o):
        keyword=o['keyword'];kid=IDS[keyword];ident=o['ident'];payload=o['payload'];values=o['values'];status=o['status']
        if o['selector_eof']:
            return keyword,dict(id=kid,status=2,calls=[],reads=[],refreshes=[],expected_fields=fields())
        is_new=ident not in records
        if is_new:records[ident]=[[.9,0.,0.,0.,0.],[0.,0.]]
        material=records[ident]
        if kid in SLOTS:material[0][SLOTS[kid]]=values[0]
        elif kid==132:material[1]=list(values)
        kind='integer'if kid==101 else'pair'if kid==132 else'real'
        call=dict(kind=kind,material_id=None if kid==101 else ident,status=status)
        if kid==101:call['value']=values[0]
        else:call['values_hex']=[hx([v])for v in values]
        lexeme=hex(ident)if evaluation and ident else str(ident)
        body=keyword+' '+lexeme+(''if payload is None else(' '+payload if payload else'')+'\n')
        read=dict(capacity=1024,value=None if payload is None else(' '+payload if payload else'')+'\n')
        return body,dict(id=kid,status=2 if status else 0,calls=[call],reads=[read],refreshes=[1]if is_new else[],expected_fields=fields())

    def put(label,ops=(),prefix='',suffix='',switch=None,guard=None):
        nonlocal dimension,nstep,records
        if switch is not None and switch!=dimension:dimension=switch;nstep=1;records={}
        data=prefix;handlers=[]
        for operation in ops:
            body,h=apply(operation);data+=body;handlers.append(h)
        if guard is not None:suffix+=f'NSTEP {guard}\n';nstep=guard
        data+=suffix;b=data.encode('ascii');name=f'{len(plan["inputs"]):02d}_{label}.KEY';(base/name).write_bytes(b)
        plan['ordered_imports'].append(name);plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(b).hexdigest(),expected_handlers=copy.deepcopy(handlers),expected_fields=fields(),expected_nstep=nstep,expected_wrapper_status=next((h['status']for h in reversed(handlers)if h['status']),0)))

    put('initial',prefix='\n')
    mesh3=Path('tests/fixtures/custom_fields/01_spatial_mesh.KEY').read_text()
    mesh2='* DEFORM-2D\nOBJTYP 1 2 0\nRZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1 4\n1 1 2 3 4\n'
    for dim in ([2,3]if evaluation else[3,2]):
        put(f'd{dim}_reset_mesh',prefix=mesh3 if dim==3 else mesh2,switch=dim)
        positive=[op('FRAE2H',first,str(.625+delta*.125),[.625+delta*.125]),
                  op('TMPSOL',second,str(123.+delta*11),[123.+delta*11]),
                  op('TMPLIQ',0,str(456.+delta*13),[456.+delta*13]),
                  op('MATDEN',first,str(7.75+delta*.5),[7.75+delta*.5]),
                  op('GBMOBI',second,f'{9.+delta} {10.+delta}',[9.+delta,10.+delta]),
                  op('LTHEAT',ignored,str(15+delta),[15+delta])]
        if evaluation:positive.reverse()
        put(f'd{dim}_positive',positive,guard=61+delta)
        put(f'd{dim}_neighbors',prefix=f'MTNAME {first}\n{a.cohort} material\nYOUNG {first} 0 {1200.+delta}\nMASDEN {first} 0 {13.+delta}\n',guard=63+delta)
        put(f'd{dim}_matching',[op(k,first,'bad',[0,0]if k=='GBMOBI'else[0])for k in IDS],guard=65+delta)
        put(f'd{dim}_partial',[
            op('FRAE2H',first,'-0,ignored',[-0.]),op('LTHEAT',first,'0x1b suffix',[27]),
            op('TMPSOL',first,'7e+more',[7.]),op('TMPLIQ',first,'0x1p2',[0.]),
            op('MATDEN',first,'6.25,ignored',[6.25]),op('GBMOBI',first,str(11.25+delta),[11.25+delta,0.])],guard=67+delta)
        put(f'd{dim}_blank',[op(k,first,'',[0,0]if k=='GBMOBI'else[0],status=1)for k in IDS],guard=69+delta)
        for k in IDS:
            put(f'd{dim}_eof_{k.lower()}',[op(k,second,None,[0,0]if k=='GBMOBI'else[0],status=1)])
        put(f'd{dim}_selector_eof',[op('MATDEN',0,None,[],selector_eof=True)])
        put(f'd{dim}_create_before_error',[op('TMPSOL',fresh,None,[0.],status=1)])
        put(f'd{dim}_retention',positive,prefix=f'* DEFORM-{dim}D\nUNIT {2 if dim==3 else 1}\n',guard=71+delta)
    assert len(plan['inputs'])<=32
    dest.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8');print(json.dumps({'imports':len(plan['inputs']),'plan':str(dest)}))


if __name__=='__main__':main()
