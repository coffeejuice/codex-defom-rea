"""Development comparison for four source-planned environmental/furnace cases."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--driver',type=Path,required=True)
    parser.add_argument('--report',type=Path,required=True)
    args=parser.parse_args()
    root=Path('analysis/oracle/environment-prototype')
    plan=json.loads(Path('analysis/environment-prototype-plan.json').read_bytes())
    run=json.loads((root/'run.json').read_bytes())
    evidence=json.loads(Path('analysis/environment-runtime-prototype.json').read_bytes())
    native=evidence['normalized_result']
    assert run['status']=='completed' and run['exit_code']==0
    assert run['executable_sha256']=='f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
    assert native['pid']==run['pid'] and native['state']=='detached'
    assert native['cleanup_error'] is None and not native['messages_truncated']
    assert hashlib.sha256((root/'observer-source.js').read_bytes()).hexdigest()==native['source_sha256']
    assert run['ordered_imports']==plan['ordered_imports']
    for item,truth in zip(run['inputs'],plan['inputs']):
        assert item['name']==truth['file']
        assert item['sha256_before']==item['sha256_after']==item['source_sha256_after']==truth['sha256']
        assert hashlib.sha256((root/item['name']).read_bytes()).hexdigest()==truth['sha256']
    groups=[]
    for message in native['messages']:
        assert message['type']!='error',message
        if message['type']!='send':continue
        v=message['payload'];assert v['operation']!='probe-error',v
        if v['operation']=='import-before':groups.append(dict(before=v,handlers=[],entries=[],lookups=[]))
        elif v['operation']=='import-after':groups[-1]['after']=v
        elif v['operation']=='handler-before':groups[-1]['entries'].append(v)
        elif v['operation']=='handler-after':groups[-1]['handlers'].append(v)
        elif v['operation']=='lookup':groups[-1]['lookups'].append(v)
    assert len(groups)==4
    checks=0

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

    initial=groups[0]['before']['state']['environment']
    equal(initial['enabled'],0,'source initial flag')
    equal(len(initial['parameters']),22,'source initial collection size')
    raw=initial['parameters'][0]['table']['dependent_hex']
    equal(raw[:48],struct.pack('<3d',2.,1.,1.).hex(),'source rich initial values')
    for i,(group,status)in enumerate(zip(groups,plan['expected_wrapper_status'])):
        equal(group['after']['status'],status,'source wrapper '+str(i))
    constants=groups[1]['after']['state']['environment']
    equal([t['constant_hex']for t in constants['tables']],[struct.pack('<d',x).hex()for x in [125.5,.125,1.7,.00025]],'source constants')
    zero=groups[2]['after']['state']['environment']
    equal(zero['enabled'],1,'source zero-count flag');equal(len(zero['parameters']),21,'source zero request minimum')
    one=groups[3]['after']['state']['environment'];table=one['parameters'][0]['table']
    equal(len(one['parameters']),21,'source one request minimum')
    equal(table['metadata'][:9],[0x8e,1,1,0,0,2,0,0,2],'source one-table metadata')
    equal(table['axes_hex'],[struct.pack('<2d',10.,20.).hex(),'',''],'source nonzero axis values')
    equal(table['dependent_hex'],struct.pack('<4d',2.5,3.5,4.5,5.5).hex(),'source nonzero dependent values')
    equal(groups[3]['after']['state']['stopping_controls']['nstep'],41,'source trailing-line guard')
    commands=['initialized 3 20 0','link 2']+['import '+(root/name).read_bytes().hex()for name in plan['ordered_imports']]
    result=subprocess.run([str(args.driver.resolve())],input='\n'.join(commands)+'\n',text=True,capture_output=True,check=True)
    rows=[json.loads(v)for v in result.stdout.splitlines()]
    Path('build/environment-prototype-cpp.jsonl').write_text(result.stdout,encoding='utf-8')
    equal(len(rows),4,'replacement imports')

    def state(primary,linked,observed,label):
        for key in ['environment','root_fields','root_record_tables','iteration','dimensional_step','time_step_imports','stopping_vectors_hex','stopping_controls','dimension','components','object_count','material_count','stpinc_0x8b4','heat_method_0x868','element_layout_0x888']:
            equal(primary[key],observed[key],label+'/'+key)
        for key,source in [('environment','linked_environment'),('root_fields','linked_root_fields'),('root_record_tables','linked_root_record_tables'),('iteration','linked_iteration'),('dimensional_step','linked_dimensional_step'),('time_step_imports','linked_time_step_imports'),('stopping_vectors_hex','linked_vectors_hex'),('stopping_controls','linked_stopping'),('dimension','linked_dimension')]:
            equal(linked[key],observed[source],label+'/'+source)

    for row,group,name in zip(rows,groups,plan['ordered_imports']):
        equal(row['result']['outcome'],'parsed',name+'/supported')
        equal(row['result']['status'],group['after']['status'],name+'/status')
        state(row['before_state'],row['before_linked_state'],group['before']['state'],name+'/before')
        state(row['state'],row['linked_state'],group['after']['state'],name+'/after')
        equal([{k:t[k]for k in ['token','id','category']}for t in row['result']['events']],[{k:t[k]for k in ['token','id','category']}for t in group['lookups']],name+'/lookup order')
        hs=[h for h in row['handler_states']if h['id']in[75,78,79,80]]
        equal(len(hs),len(group['handlers']),name+'/handler count')
        for h,observed in zip(hs,group['handlers']):
            equal(h['id'],observed['id'],name+'/handler id');equal(h['status'],observed['status'],name+'/handler status')
            state(h['state'],h['linked_state'],observed['state'],name+'/handler state')
            equal(observed['refreshes'],[],name+'/no refresh')
            if observed['cursor']:equal(h['cursor'],observed['cursor'],name+'/cursor')
    report=dict(status='PASS_NATIVE_ENVIRONMENT_PROTOTYPE',imports=len(rows),handlers=sum(len(g['handlers'])for g in groups),checks=checks,evidence_id=evidence['evidence_id'],capture_sha256=hashlib.sha256(Path('analysis/environment-runtime-prototype.json').read_bytes()).hexdigest(),qualification='Development comparison only: all22 startup parameters and linked-chain topology, four constant headers, minimum-count recreation and one numeric furnace parameter. Candidate is not frozen; text/errors/selectors/dimensional/unit branches and independent evaluation remain required.')
    args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8',newline='\n');print(json.dumps(report))


if __name__=='__main__':main()
