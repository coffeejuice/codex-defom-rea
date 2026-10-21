"""Prepare bounded, source-planned name imports without reading native outputs."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re

IDS={'USRDEF':87,'UENAME':88,'UNNAME':89,'CVNAME':90}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cohort',choices=['training','evaluation'])
    args=parser.parse_args();evaluation=args.cohort=='evaluation'
    base=Path('tests/fixtures/root-names')/args.cohort;base.mkdir(parents=True,exist_ok=True)
    destination=Path('analysis/root-names-'+args.cohort+'-inputs.json')
    if destination.exists():raise SystemExit('Refusing to replace an existing plan')
    dimension=3;selected=0;node=[];element=[];custom=[];nstep=1
    user=[dict(known_prefix=1,bytes_hex='00')for _ in range(10)]
    plan=dict(status='source_planned_before_native',cohort=args.cohort,ordered_imports=[],inputs=[],
              qualification='Bounded defined ASCII name imports, normal wrapper option0, owned storage in both dimensions. Complete app/scientific consumers, arbitrary COW aliases, allocation failures and unsafe native negative resize remain pending.')
    if evaluation:plan['chronology']='Prepared after training PASS; original must run before C++ on these changed values, IDs, extents and operation ordering.'
    tag='eval'if evaluation else'train';delta=1 if evaluation else 0

    def snapshot():
        return copy.deepcopy(dict(root_names=dict(custom=custom,user_text=user),user_fields=dict(
            node_count=len(node),element_count=len(element),node_records_hex=node,element_records_hex=element),
            dimension=dimension,custom_last_id_0x33b8=selected))

    def reset(new):
        nonlocal dimension,node,element,nstep
        if dimension==new:return
        dimension=new;node=[];element=[];nstep=1
        for record in user:record['bytes_hex']='00'+record['bytes_hex'][2:]

    def op(keyword,values,payload='',header=None,header_status=0):
        # values are the source reader's intended outputs, including zeroed
        # matching failures. No C++ or original outputs are used here.
        return dict(keyword=keyword,values=values,payload=payload,header=header,header_status=header_status)

    def apply(operation):
        keyword=operation['keyword'];kid=IDS[keyword];values=operation['values'];raw=operation['payload']
        header=operation['header'];hs=operation['header_status']
        if header is None:header=' '.join(map(str,values))
        reads=[dict(phase='header',capacity=1024,value=None if header=='<EOF>'else header+'\n')]
        # The first byte after the parsed keyword belongs to its line reader.
        if reads[0]['value']is not None:reads[0]['value']=' '+reads[0]['value']if header else'\n'
        body=keyword+(''if header=='<EOF>'else(' '+header if header else'')+'\n')+raw
        status=2 if hs else 0;refresh=[];operations=[]

        def line(capacity,phase):
            nonlocal raw,status
            if not raw:reads.append(dict(phase=phase,capacity=capacity,value=None));status=2;return None
            n=min(len(raw),capacity-1);newline=raw.find('\n',0,n)
            if newline>=0:n=newline+1
            value=raw[:n];raw=raw[n:];reads.append(dict(phase=phase,capacity=capacity,value=value));return value

        def trim(value):return value.rstrip(' \r\n')

        if not hs:
            if kid==87:
                for i in range(min(10,max(0,values[0]))):
                    value=line(1024,'direct')
                    if value is None:break
                    user[i]=dict(known_prefix=80,bytes_hex=trim(value).encode('ascii')[:80].ljust(80,b'\0').hex())
            elif kid in[88,89]:
                records=node if kid==89 else element;flag,count=values
                if flag<2:
                    assert count>=0,'Native negative resizing is outside the safe domain'
                    if count!=len(records):
                        refresh=[0];del records[count:]
                        records.extend(['00'*64]*(count-len(records)))
                for i in range(len(records)):
                    value=line(64,'direct')
                    if value is None:break
                    value=trim(value)
                    if flag<2 and value:
                        old=bytes.fromhex(records[i]);new=value.encode('ascii')+b'\0'
                        records[i]=(new+old[len(new):]).hex()
            else:
                ident,count=values;value=line(1024,'text')
                if value is not None:
                    record=next((r for r in custom if r['id']==ident),None)
                    if record is None:record=dict(id=ident,name='',labels=[]);custom.append(record)
                    record['name']=trim(value[:63]);record['labels']=[];operations=['name','clear']
                    for _ in range(max(0,count)):
                        value=line(1024,'text')
                        if value is None:break
                        record['labels'].append(trim(value[:63]));operations.append('append')
        return body,dict(id=kid,status=status,headers=[dict(values=values,status=hs)],reads=reads,
                         refreshes=refresh,custom_operations=operations,expected_after=snapshot())

    def put(label,ops=(),prefix='',suffix='',switch=None,selection=None):
        nonlocal selected,nstep
        if switch is not None:reset(switch)
        if selection is not None:selected=selection
        data=prefix;handlers=[]
        for operation in ops:
            body,expected=apply(operation);data+=body;handlers.append(expected)
        data+=suffix;b=data.encode('ascii');name=f'{len(plan["inputs"]):02d}_{label}.KEY';(base/name).write_bytes(b)
        guard=re.search(r'NSTEP (\d+)\n',suffix)
        if guard:nstep=int(guard.group(1))
        plan['ordered_imports'].append(name);plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(b).hexdigest(),
             expected_wrapper_status=next((h['status']for h in reversed(handlers)if h['status']),0),
             expected_after=snapshot(),expected_handlers=handlers,expected_nstep=nstep))

    mesh=Path('tests/fixtures/custom_fields/01_spatial_mesh.KEY').read_text()
    node=['00'*64]*2;element=['00'*64]*3
    put('mesh_and_selection',prefix=mesh+'CUSVAR 1 101 1 2 11 2\n1 13 17\n4 19 23\n'
        +'USRNOD 1 1 31 2\n4 32 -0\nUSRELM 1 1 79 3\n1 83 89 97\n',selection=101)
    for dim in ([2,3]if evaluation else[3,2]):
        put(f'd{dim}_reset',prefix=f'* DEFORM-{dim}D\n',switch=dim)
        count=3+delta
        usrlines=' leading '+tag+' \t  \n'+'U'*(80+delta)+'\n'+'v'*(1000-delta)+'\n'
        put(f'd{dim}_names',[
            op('USRDEF',[3],usrlines),op('UENAME',[0,count],''.join(f'element-{tag}-{i}-long-name\n'for i in range(count))),
            op('UNNAME',[1,count],''.join(f'node-{tag}-{i}-long-name\n'for i in range(count))),
            op('CVNAME',[-9-delta,2],f'custom-{tag}-long-name\n first label  \nsecond-label-{tag}\t \n'),
            op('CVNAME',[0,1],f'zero-{tag}\n\n')],suffix='NSTEP 51\n')
        put(f'd{dim}_same_width',[
            op('UENAME',[1,count],'E\n'+'   \n'*(count-1)),op('UNNAME',[-1,count],'N\n'+'\n'*(count-1)),
            op('CVNAME',[-9-delta,1],f'\n replacement-{tag} \n')])
        put(f'd{dim}_discard',[
            op('UENAME',[2,-17],'ignore\n'*count),op('UNNAME',[4,200],'ignored-node\n'*count)],suffix='NSTEP 53\n')
        put(f'd{dim}_split',[
            op('UENAME',[0,3],'S'*(70+delta)+'\nlast\n'),
            op('UNNAME',[0,3],'Q'*63+'\nlast-node\n'),
            op('CVNAME',[17+delta,1],'C'*(200+delta)+'\n'+'L'*(160+delta)+'\n')],suffix='NSTEP 55\n')
        put(f'd{dim}_shrink_grow',[
            op('UENAME',[0,1],'retained\n'),op('UENAME',[1,4],'\nnew-1\n\nnew-3\n'),
            op('UNNAME',[0,1],'kept\n'),op('UNNAME',[0,4],'\n\nnew-node-2\nnew-node-3\n')])
        put(f'd{dim}_partial', [op('UENAME',[0,5],'committed-prefix\n')])
        put(f'd{dim}_node_partial',[op('UNNAME',[0,5],'committed-node-prefix\n')])
        put(f'd{dim}_usr_partial',[op('USRDEF',[4],f'changed-{tag}\n\n')])
        put(f'd{dim}_label_partial',[op('CVNAME',[17+delta,3],f'new-name-{tag}\nkept-prefix\n')])
        put(f'd{dim}_name_failure',[op('CVNAME',[-9-delta,3])])
        put(f'd{dim}_nonpositive',[
            op('USRDEF',[-3]),op('USRDEF',[0]),op('UENAME',[0,0]),op('UNNAME',[1,0]),
            op('CVNAME',[0,-3],f'negative-labels-{tag}\n'),op('CVNAME',[17+delta,0],f'zero-labels-{tag}\n')],
            prefix='UNIT 2\n',suffix='NSTEP 57\n')
        put(f'd{dim}_cap10',[op('USRDEF',[12],''.join(f'{tag}-row-{i}\n'for i in range(10)))],suffix='NSTEP 59\n')
    put('matching_headers',[op('USRDEF',[0],header='junk'),op('UENAME',[0,0],header='junk'),
        op('UNNAME',[1,0],header='1'),op('CVNAME',[0,0],f'matching-{tag}\n',header='junk')])
    put('blank_header',[op('CVNAME',[0,0],header='',header_status=1)])
    put('eof_header',[op('USRDEF',[0],header='<EOF>',header_status=1)])
    put('last_reset',prefix=f'* DEFORM-{3 if dimension==2 else 2}D\n',switch=3 if dimension==2 else 2)
    assert len(plan['inputs'])<=32
    destination.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8');print(json.dumps({'plan':str(destination),'imports':len(plan['inputs'])}))


if __name__=='__main__':main()
