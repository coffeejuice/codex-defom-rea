"""Compare bounded name imports with retained passive original observations."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

TARGET='f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
KEYS=['root_names','user_fields','custom_last_id_0x33b8','dimension','components','object_count','material_count','stopping_controls']


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--plan',type=Path,required=True)
    ap.add_argument('--run',type=Path,required=True)
    ap.add_argument('--evidence',type=Path,required=True)
    ap.add_argument('--driver',type=Path,required=True)
    ap.add_argument('--report',type=Path,required=True)
    ap.add_argument('--freeze',type=Path)
    args=ap.parse_args()
    freeze=json.loads(args.freeze.read_bytes())if args.freeze else None
    if freeze:
        for item in freeze['files']:
            assert hashlib.sha256(Path(item['path']).read_bytes()).hexdigest()==item['sha256'],item['path']
    plan=json.loads(args.plan.read_bytes());run=json.loads((args.run/'run.json').read_bytes())
    evidence=json.loads(args.evidence.read_bytes());native=evidence['normalized_result']
    assert run['status']=='completed'and run['exit_code']==0
    assert run['executable_sha256']==TARGET
    assert native['pid']==run['pid']and native['state']=='detached'
    assert native['cleanup_error']is None and not native['messages_truncated']
    assert hashlib.sha256((args.run/'observer-source.js').read_bytes()).hexdigest()==native['source_sha256']
    assert run['ordered_imports']==plan['ordered_imports']
    for item,truth in zip(run['inputs'],plan['inputs']):
        assert item['name']==truth['file']
        assert item['sha256_before']==item['sha256_after']==item['source_sha256_after']==truth['sha256']
        assert hashlib.sha256((args.run/item['name']).read_bytes()).hexdigest()==truth['sha256']
    groups=[];installed=0
    for message in native['messages']:
        assert message['type']!='error',message
        if message['type']!='send':continue
        v=message['payload'];assert v['operation']!='probe-error',v
        if v['operation']=='hooks-installed':
            installed+=1;assert v['pid']==run['pid']
        elif v['operation']=='import-before':groups.append(dict(before=v,entries=[],handlers=[],lookups=[]))
        elif v['operation']=='import-after':groups[-1]['after']=v
        elif v['operation']=='handler-before':groups[-1]['entries'].append(v)
        elif v['operation']=='handler-after':groups[-1]['handlers'].append(v)
        elif v['operation']=='lookup':groups[-1]['lookups'].append(v)
    assert installed==1 and len(groups)==len(plan['inputs'])
    checks=0;unobserved=0

    def equal(a,b,label):
        nonlocal checks
        checks+=1
        def difference(x,y,path):
            if type(x)!=type(y):return path,x,y
            if isinstance(x,dict):
                if x.keys()!=y.keys():return path,list(x),list(y)
                for k in x:
                    if x[k]!=y[k]:return difference(x[k],y[k],path+'/'+k)
            elif isinstance(x,list):
                if len(x)!=len(y):return path,len(x),len(y)
                for i in range(len(x)):
                    if x[i]!=y[i]:return difference(x[i],y[i],path+'/'+str(i))
            else:return path,x,y
        if a!=b:raise AssertionError(difference(a,b,label))

    def selected(cpp,observed,label):
        for key in KEYS:equal(cpp[key],observed[key],label+'/'+key)

    commands=['initialized 3 20 0','link 2']+['import '+(args.run/name).read_bytes().hex()for name in plan['ordered_imports']]
    result=subprocess.run([str(args.driver.resolve())],input='\n'.join(commands)+'\n',text=True,capture_output=True,check=True)
    args.report.with_suffix('.cpp.jsonl').write_text(result.stdout,encoding='utf-8')
    rows=[json.loads(v)for v in result.stdout.splitlines()]
    equal(len(rows),len(groups),'replacement import count')
    for truth,row,group in zip(plan['inputs'],rows,groups):
        name=truth['file']
        equal(Path(group['before']['file']).name,name,name+'/native file')
        equal(row['result']['outcome'],'parsed',name+'/supported')
        equal(row['result']['status'],group['after']['status'],name+'/status')
        equal(group['after']['status'],truth['expected_wrapper_status'],name+'/source wrapper status')
        for phase,cpp,linked in [('before',row['before_state'],row['before_linked_state']),('after',row['state'],row['linked_state'])]:
            selected(cpp,group[phase]['state']['primary'],name+'/'+phase)
            selected(linked,group[phase]['state']['linked'],name+'/'+phase+'/linked')
        equal([{k:t[k]for k in ['token','id','category']}for t in row['result']['events']],
              [{k:t[k]for k in ['token','id','category']}for t in group['lookups']],name+'/lookup order')
        handlers=[h for h in row['handler_states']if 87<=h['id']<=90]
        equal(len(handlers),len(group['handlers']),name+'/handler count')
        equal(len(group['entries']),len(handlers),name+'/handler entry count')
        for i,(h,observed)in enumerate(zip(handlers,group['handlers'])):
            label=name+'/handler'+str(i)
            equal(h['id'],observed['id'],label+'/id');equal(h['status'],observed['status'],label+'/status')
            selected(h['state'],observed['state']['primary'],label)
            selected(h['linked_state'],observed['state']['linked'],label+'/linked')
            if observed['cursor']:equal(h['cursor'],observed['cursor'],label+'/cursor')
            else:unobserved+=1
            if observed['id']in[87,90]:equal(observed['refreshes'],[],label+'/no refresh')
            if observed['id']==90:
                operations=[v['kind']for v in observed['order']if v['kind']in['name','clear','append']]
                if 'name'in operations:equal(operations[:2],['name','clear'],label+'/name before clear')
                equal(h['state']['custom_last_id_0x33b8'],group['entries'][i]['state']['primary']['custom_last_id_0x33b8'],label+'/selection unchanged')
            if 'expected_handlers'in truth:
                expected=truth['expected_handlers'][i]
                for key in ['id','status','headers','reads','refreshes']:
                    equal(observed[key],expected[key],label+'/source '+key)
                operations=[v['kind']for v in observed['order']if v['kind']in['name','clear','append']]
                equal(operations,expected['custom_operations'],label+'/source custom mutation order')
                for key,value in expected['expected_after'].items():
                    equal(observed['state']['primary'][key],value,label+'/source after/'+key)
        if 'expected_handlers'in truth:
            equal(len(handlers),len(truth['expected_handlers']),name+'/source handler count')
        for key,value in truth.get('expected_after',{}).items():
            equal(group['after']['state']['primary'][key],value,name+'/source after/'+key)
        if 'expected_nstep'in truth:
            equal(group['after']['state']['primary']['stopping_controls']['nstep'],truth['expected_nstep'],name+'/source token-consumption guard')
    if freeze:
        for item in freeze['files']:
            assert hashlib.sha256(Path(item['path']).read_bytes()).hexdigest()==item['sha256'],item['path']
    report=dict(status='PASS_NATIVE_ROOT_NAMES',imports=len(rows),handlers=sum(len(g['handlers'])for g in groups),checks=checks,
                unobserved_eof_cursors=unobserved,evidence_id=evidence['evidence_id'],
                evidence_sha256=hashlib.sha256(args.evidence.read_bytes()).hexdigest(),
                frozen_files=len(freeze['files'])if freeze else 0,
                qualification=plan['qualification'])
    args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps(report))


if __name__=='__main__':main()
