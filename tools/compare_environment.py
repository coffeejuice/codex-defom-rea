"""Compare source-planned environmental/furnace state and call order with REA."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import struct
import subprocess


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cohort',required=True,choices=['training-a','training-b','evaluation-a','evaluation-b'])
    parser.add_argument('--driver',type=Path,required=True)
    parser.add_argument('--report',type=Path)
    a=parser.parse_args();root=Path('analysis/oracle/environment-'+a.cohort)
    plan=json.loads(Path('analysis/environment-'+a.cohort+'-inputs.json').read_bytes())
    run=json.loads((root/'run.json').read_bytes());capture=Path('analysis/environment-runtime-'+a.cohort+'.json')
    evidence=json.loads(capture.read_bytes());native=evidence['normalized_result']
    assert run['status']=='completed'and run['exit_code']==0
    assert run['executable_sha256']=='f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
    assert native['pid']==run['pid']and native['state']=='detached'
    assert native['cleanup_error']is None and not native['messages_truncated']
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
        operation=v['operation']
        if operation=='import-before':groups.append(dict(before=v,handlers=[],entries=[],lookups=[]))
        elif operation=='import-after':groups[-1]['after']=v
        elif operation=='handler-before':groups[-1]['entries'].append(v)
        elif operation=='handler-after':groups[-1]['handlers'].append(v)
        elif operation=='lookup':groups[-1]['lookups'].append(v)
    checks=eof=inner_errors=packed_lines=retained_lines=allocations=lookups=0
    counts=Counter();dimensions=Counter()

    def equal(actual,expected,label):
        nonlocal checks
        checks+=1
        def diff(x,y,path):
            if type(x)!=type(y):return path,x,y
            if isinstance(x,dict):
                if x.keys()!=y.keys():return path,list(x),list(y)
                for k in x:
                    if x[k]!=y[k]:return diff(x[k],y[k],path+'/'+k)
            elif isinstance(x,list):
                if len(x)!=len(y):return path,len(x),len(y)
                for i in range(len(x)):
                    if x[i]!=y[i]:return diff(x[i],y[i],path+'/'+str(i))
            else:return path,x,y
        if actual!=expected:raise AssertionError(diff(actual,expected,label))

    def planned(actual,expected,label):
        for key in expected:
            if expected[key]is not None:equal(actual[key],expected[key],label+'/'+key)

    #Compare the independently source-planned effects/statuses with the original
    #before running the replacement, so matching errors alone cannot pass.
    equal(len(groups),len(plan['inputs']),'complete imports')
    initial=groups[0]['before']['state']['environment']
    equal(len(initial['parameters']),22,'source startup collection size')
    equal(initial['parameters'][0]['table']['dependent_hex'][:48],struct.pack('<3d',2.,1.,1.).hex(),'source rich startup values')
    allocate=['allocate-enter','release-enter','release-leave','allocate-leave']
    for truth,group in zip(plan['inputs'],groups):
        name=truth['file'];equal(group['before']['file'],name,name+'/input identity');equal(group['after']['file'],name,name+'/output identity')
        equal(group['after']['status'],truth['expected_wrapper_status'],name+'/planned wrapper')
        planned(group['after']['state']['environment'],truth['expected_environment'],name+'/planned final')
        if 'expected_nstep'in truth:equal(group['after']['state']['stopping_controls']['nstep'],truth['expected_nstep'],name+'/NSTEP guard')
        equal(len(group['entries']),len(truth['expected_handlers']),name+'/planned entry count')
        equal(len(group['handlers']),len(truth['expected_handlers']),name+'/planned handler count')
        for t,h,entry in zip(truth['expected_handlers'],group['handlers'],group['entries']):
            kid=t['id'];counts[kid]+=1;dimensions[f"{kid}/{entry['state']['dimension']}"]+=1
            equal(h['id'],kid,name+'/native id');equal(entry['id'],kid,name+'/entry id');equal(h['status'],t['status'],name+'/planned handler status')
            planned(h['state']['environment'],t['expected_environment'],name+'/planned handler state')
            equal(h['calls'],[t['header']]+t['inner'],name+'/outer and furnace header outputs, inner statuses, text retention')
            equal(h['refreshes'],[],name+'/no refresh')
            kinds=['header-enter','header-leave']
            if t['header']['status']==0:
                kinds+=['reset-enter','release-enter','release-leave']+allocate+['reset-leave']
                if kid==75 and t['header']['integer']==10:
                    old=len(entry['state']['environment']['parameters']);new=len(t['expected_environment']['parameters'])
                    kinds+=['clear-enter']+['release-enter','release-leave']*old+['clear-leave']
                    for index in range(new):kinds+=['entry-created','release-enter','release-leave']+allocate
                    for inner in t['inner']:
                        kind=inner['kind'];rank=inner['header']['integers'][1]
                        kinds+=[kind+'-enter','furnace-header-enter','furnace-header-leave']
                        if rank in[0,1]and not(kind=='packed-text'and rank==0):kinds+=allocate
                        kinds+=['text-line']*len(inner['lines'])+[kind+'-leave']
                        inner_errors+=bool(inner['status']);packed_lines+=len(inner['lines']);retained_lines+=sum(bool(l['status'])for l in inner['lines'])
                    created=[x['index']for x in h['order']if x['kind']=='entry-created']
                    equal(created,list(range(1,new+1)),name+'/typed entry creation order')
                    equal(next(x['count']for x in h['order']if x['kind']=='clear-enter'),old,name+'/old owned entry count')
                elif t['payload']:
                    kinds+=['payload-enter']
                    if t['expected_environment']['tables'][0 if kid==75 else kid-77]['metadata'][1]==1:kinds+=allocate
                    kinds+=['payload-leave']
                else:equal(h['state']['environment']['parameters'],entry['state']['environment']['parameters'],name+'/retained dormant furnace entries')
            else:equal(h['state']['environment'],entry['state']['environment'],name+'/failed header preserves entire environment')
            equal([x['kind']for x in h['order']],kinds,name+'/reset-release-allocation-read ordering')
            allocations+=kinds.count('allocate-leave')
            payloads=[x['status']for x in h['order']if x['kind']=='payload-leave']
            if payloads:equal(payloads,[int(t['status']!=0)],name+'/propagated root payload status')
    commands=['initialized 3 20 0','link 2']+['import '+(root/name).read_bytes().hex()for name in plan['ordered_imports']]
    r=subprocess.run([str(a.driver.resolve())],input='\n'.join(commands)+'\n',text=True,capture_output=True,check=True)
    rows=[json.loads(line)for line in r.stdout.splitlines()];equal(len(rows),len(groups),'replacement imports')

    def state(primary,linked,observed,label):
        for key in ['environment','root_fields','root_record_tables','iteration','dimensional_step','time_step_imports','stopping_vectors_hex','stopping_controls','dimension','components','object_count','material_count','stpinc_0x8b4','heat_method_0x868','element_layout_0x888']:
            equal(primary[key],observed[key],label+'/'+key)
        for key,source in [('environment','linked_environment'),('root_fields','linked_root_fields'),('root_record_tables','linked_root_record_tables'),('iteration','linked_iteration'),('dimensional_step','linked_dimensional_step'),('time_step_imports','linked_time_step_imports'),('stopping_vectors_hex','linked_vectors_hex'),('stopping_controls','linked_stopping'),('dimension','linked_dimension')]:
            equal(linked[key],observed[source],label+'/'+source)
        f=primary['finalization_controls'];raw=b''.join(struct.pack('<d',f[k])for k in ['time_step_0x920','time_step_secondary_0x928','step_increment_0x930']).hex()
        equal(dict(values_hex=raw,control=f['control_0x93c'],mode=f['mode_0x990']),observed['time'],label+'/time')
        equal([f[k]for k in ['step_number_0x8b0','saved_step_0x744']],observed['steps'],label+'/steps')

    for row,group,truth in zip(rows,groups,plan['inputs']):
        name=truth['file'];equal(row['result']['outcome'],'parsed',name+'/supported');equal(row['result']['status'],group['after']['status'],name+'/wrapper status')
        state(row['before_state'],row['before_linked_state'],group['before']['state'],name+'/before');state(row['state'],row['linked_state'],group['after']['state'],name+'/after')
        equal([{k:t[k]for k in['token','id','category']}for t in row['result']['events']],[{k:t[k]for k in['token','id','category']}for t in group['lookups']],name+'/complete lookup order');lookups+=len(group['lookups'])
        hs=[h for h in row['handler_states']if h['id']in[75,78,79,80]];equal(len(hs),len(group['handlers']),name+'/replacement handler count')
        for h,observed in zip(hs,group['handlers']):
            equal(h['id'],observed['id'],name+'/replacement id');equal(h['status'],observed['status'],name+'/replacement status');state(h['state'],h['linked_state'],observed['state'],name+'/handler')
            if observed['cursor']:equal(h['cursor'],observed['cursor'],name+'/cursor')
            else:eof+=1
    for kid in[75,78,79,80]:
        for dim in[2,3]:equal(dimensions[f'{kid}/{dim}']>0,True,f'dimension {kid}/{dim}')
    for value in[packed_lines,retained_lines,inner_errors]:equal(value>0,True,'nonvacuous failure/text coverage')
    report=dict(status='PASS_NATIVE_ENVIRONMENT_COMPARISON',cohort=a.cohort,imports=len(rows),handlers=sum(counts.values()),handler_counts=dict(counts),dimension_counts=dict(dimensions),checks=checks,allocations=allocations,ignored_inner_errors=inner_errors,packed_text_lines=packed_lines,retained_text_lines=retained_lines,lookup_tokens=lookups,eof_cursors_unobserved=eof,evidence_id=evidence['evidence_id'],capture_sha256=hashlib.sha256(capture.read_bytes()).hexdigest(),scope='All four environmental dispatchers, owned tables and source-planned numeric/packed furnace entries, errors, UNIT/dimension lifecycle and complete lookup order. Unsafe native buffer writes and first-read uninitialized text are not claimed deterministic.')
    if a.report:a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8',newline='\n')
    print(json.dumps(report))


if __name__=='__main__':main()
