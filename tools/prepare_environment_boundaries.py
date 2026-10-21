"""Append original-first text/header boundary cases to the prepared evaluation."""
import copy
import hashlib
import json
from pathlib import Path
import struct

from prepare_environment_fixtures import constructors,hx


def main():
    path=Path('analysis/environment-evaluation-b-inputs.json')
    plan=json.loads(path.read_bytes());assert len(plan['inputs'])==16
    root=Path('tests/fixtures/environment/evaluation-b')
    for case in ['textBoundary','textRankZero','textRankInvalid']:
        parameters=constructors(21)
        env=copy.deepcopy(plan['inputs'][-1]['expected_environment'])
        env['enabled']=1;env['parameters']=parameters
        selected=env['tables'][0]
        selected.update(metadata=[2,0,0,0,0,0,0,0,1,0,0,0],constant_hex=hx([0.]),axes_hex=['','',''],dependent_hex='')
        data='ENVTMP 10 21\n';inner=[]
        for index in range(1,21):
            t=parameters[index-1]['table'];t['metadata'][1]=0;t['axes_hex']=['','',''];t['dependent_hex']=''
            value=index+.0625;integers=[index,0,1];reals=[value,0.];header_status=0
            if case=='textBoundary'and index==1:
                data+='1 0 2 0 0\nbad\n';integers=[1,0,2];reals=[0.,0.]
                t['metadata'][8]=2;t['dependent_hex']=hx([0.,0.])
            elif case=='textBoundary'and index in[2,3]:
                data+='bad\n'if index==2 else'\n';integers=[0,0,0];reals=[0.,0.]
                header_status=int(index==3);t['constant_hex']=hx([0.])
            else:
                data+=f'{index} 0 1 {value} 0\n';t['constant_hex']=hx([value])
            inner.append(dict(kind='parameter',header=dict(integers=integers,reals_hex=[hx([x])for x in reals],status=header_status),lines=[],status=0))
        t=parameters[20]['table'];lines=[];status=0
        if case=='textBoundary':
            header=[21,1,0];reals=[2.,8.];data+='21 1 0 2 8\n'+'  '+'Q'*75+'   \n'+'  TabTail\t\r  \n'
            names=['  '+'Q'*61,'  TabTail\t'];lines=[dict(status=0,value=v)for v in names]
            t['metadata'][1]=1;t['metadata'][5]=2;t['metadata'][8]=8;t['axes_hex']=[hx([0.,0.]),'','']
            t['dependent_hex']=b''.join(v.encode('ascii').ljust(64,b' ')for v in names).hex()
            parameters[20]['known_bytes_hex']='01'*128
        else:
            rank=0 if case=='textRankZero'else 2;header=[21,rank,9];reals=[123.5,9.]
            data+=f'21 {rank} 9 123.5 9\n';t['metadata'][1]=rank;status=int(rank==2)
        inner.append(dict(kind='packed-text',header=dict(integers=header,reals_hex=[hx([x])for x in reals],status=0),lines=lines,status=status))
        handler=dict(id=75,status=0,header=dict(kind='header',integer=10,real_hex=hx([21.]),status=0),payload=False,inner=inner,expected_environment=env)
        name=case+'.KEY';raw=data.encode('ascii');(root/name).write_bytes(raw)
        plan['ordered_imports'].append(name);plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(raw).hexdigest(),expected_handlers=[handler],expected_environment=env,expected_wrapper_status=0))
    plan['supplemental_scope']='Three source-planned original-first boundary cases:63-byte packed truncation and tab/CR trimming, inner matching/blank headers, packed rank0 positive ignored fields and packed invalid rank2 error ignored by parent.'
    path.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8',newline='\n')
    proof=dict(status='PREPARED_BEFORE_ORIGINAL_AND_CPP',script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),plan_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),new_cases=plan['ordered_imports'][-3:],qualification='Existing18 frozen candidate files unchanged. Supplemental generator creates new held-out inputs only; it does not read C++ or native outputs.')
    Path('analysis/environment-boundary-preparation.json').write_text(json.dumps(proof,indent=2)+'\n',encoding='utf-8',newline='\n');print(json.dumps(dict(evaluation_b_imports=len(plan['inputs']))))


if __name__=='__main__':main()
