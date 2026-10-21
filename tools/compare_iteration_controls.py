"""Compare source-bound iteration-control imports with the original observation."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import struct
import subprocess

DEFINITIONS = {61:0,62:0,63:0,64:3,65:4,66:5,67:6,68:7,69:8,70:1}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['driver','capture','run','plan']:
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    evidence = json.loads(args.capture.read_bytes())
    run = json.loads(args.run.read_bytes())
    plan = json.loads(args.plan.read_bytes())
    native = evidence['normalized_result']
    assert run['status']=='completed' and run['exit_code']==0
    assert run['executable_sha256']=='f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
    assert native['pid']==run['pid'] and native['state']=='detached'
    assert native['cleanup_error'] is None and not native['messages_truncated']
    assert hashlib.sha256((args.run.parent/'observer-source.js').read_bytes()).hexdigest()==native['source_sha256']
    assert run['ordered_imports']==plan['ordered_imports']
    for item in run['inputs']:
        expected=next(x for x in plan['inputs'] if x['file']==item['name'])
        assert item['sha256_before']==item['sha256_after']==item['source_sha256_after']==expected['sha256']
        assert hashlib.sha256((args.run.parent/item['name']).read_bytes()).hexdigest()==expected['sha256']
    groups=[]
    for message in native['messages']:
        assert message['type']!='error', message
        if message['type']!='send':
            continue
        value=message['payload']
        assert value['operation']!='probe-error', value
        if value['operation']=='import-before':groups.append(dict(before=value,entries=[],handlers=[],lookups=[]))
        elif value['operation']=='import-after':groups[-1]['after']=value
        elif value['operation']=='lookup':groups[-1]['lookups'].append(value)
        elif value['operation']=='handler-before':groups[-1]['entries'].append(value)
        elif value['operation']=='handler-after':groups[-1]['handlers'].append(value)
    commands=['initialized 3 20 0','link 2']+['import '+(args.run.parent/name).read_bytes().replace(b'\r\n',b'\n').hex() for name in plan['ordered_imports']]
    result=subprocess.run([str(args.driver.resolve())],input='\n'.join(commands)+'\n',text=True,capture_output=True,check=True)
    rows=[json.loads(line) for line in result.stdout.splitlines()]
    assert len(rows)==len(groups)==len(plan['ordered_imports'])
    checks=eof=0
    counts=Counter()

    def equal(actual, expected, label):
        nonlocal checks
        checks+=1
        if actual!=expected:raise AssertionError((label,str(actual)[:400],str(expected)[:400]))

    def state(primary, linked, observed, label):
        for key in ['iteration','dimensional_step','time_step_imports','stopping_vectors_hex','stopping_controls','dimension','components','object_count','material_count','stpinc_0x8b4','heat_method_0x868','element_layout_0x888']:
            equal(primary[key],observed[key],label+'/'+key)
        equal(linked['iteration'],observed['linked_iteration'],label+'/linked iteration controls')
        equal(linked['dimensional_step'],observed['linked_dimensional_step'],label+'/linked dimensional controls')
        equal(linked['time_step_imports'],observed['linked_time_step_imports'],label+'/linked iteration-control imports')
        equal(linked['stopping_vectors_hex'],observed['linked_vectors_hex'],label+'/linked vectors')
        equal(linked['stopping_controls'],observed['linked_stopping'],label+'/linked stopping')
        equal(linked['dimension'],observed['linked_dimension'],label+'/linked dimension')
        time=primary['finalization_controls']
        raw=b''.join(struct.pack('<d',time[k]) for k in ['time_step_0x920','time_step_secondary_0x928','step_increment_0x930']).hex()
        equal(dict(values_hex=raw,control=time['control_0x93c'],mode=time['mode_0x990']),observed['time'],label+'/time controls')
        equal([time[k]for k in ['step_number_0x8b0','saved_step_0x744']],observed['steps'],label+'/step controls')

    def defaults(dim):
        bits=[0x3f747ae147ae147b,0x3fa999999999999a]if dim==3 else[0x3f50624dd2f1a9fc,0x3f847ae147ae147b]
        return dict(integers=[2 if dim==3 else 1,1,0,200,1,2 if dim==3 else 1,200,1,0],reals_hex=b''.join(struct.pack('<Q',x)for x in bits).hex()+'00'*32)
    equal(groups[0]['before']['state']['iteration'],defaults(3),'native3D defaults');equal(groups[0]['before']['state']['linked_iteration'],defaults(2),'native2D defaults')
    dimensions=Counter();accepted=rejected=payloads=diagnostics=lookups=0;modes=set()
    for name,row,group in zip(plan['ordered_imports'],rows,groups):
        expected=next(x for x in plan['inputs']if x['file']==name)
        equal(group['before']['file'],name,name+'/input identity');equal(group['after']['file'],name,name+'/output identity')
        equal(group['after']['status'],expected['expected_wrapper_status'],name+'/planned wrapper');equal(group['after']['state']['iteration'],expected['expected_final'],name+'/planned final fields')
        if 'expected_nstep'in expected:equal(group['after']['state']['stopping_controls']['nstep'],expected['expected_nstep'],name+'/neighbor consumed')
        equal(row['result']['outcome'],'parsed',name+'/supported');equal(row['result']['status'],group['after']['status'],name+'/wrapper')
        state(row['before_state'],row['before_linked_state'],group['before']['state'],name+'/before');state(row['state'],row['linked_state'],group['after']['state'],name+'/after')
        equal([{k:t[k]for k in ['token','id','category']}for t in row['result']['events']],[{k:t[k]for k in ['token','id','category']}for t in group['lookups']],name+'/complete lookup order');lookups+=len(group['lookups'])
        handlers=[h for h in row['handler_states']if h['id']in DEFINITIONS];events=[e for e in row['result']['events']if e['id']in DEFINITIONS]
        equal(len(handlers),len(expected['expected_handlers']),name+'/planned handlers');equal(len(group['handlers']),len(handlers),name+'/native handlers');equal(len(group['entries']),len(handlers),name+'/entry pairing');equal(len(events),len(handlers),name+'/diagnostic event pairing')
        for h,event,observed,entry,planned in zip(handlers,events,group['handlers'],group['entries'],expected['expected_handlers']):
            keyword=planned['id'];dimension=entry['state']['dimension'];counts[keyword]+=1;dimensions[str(keyword)+'/'+str(dimension)]+=1
            for source in [h,observed,entry]:equal(source['id'],keyword,name+'/keyword')
            equal(observed['status'],planned['status'],name+'/planned status');equal(h['status'],observed['status'],name+'/C++ status');equal(observed['state']['iteration'],planned['expected_fields'],name+'/source-planned fields')
            equal(observed['diagnostics'],planned['diagnostics'],name+'/planned native diagnostics');equal(event.get('diagnostics',[]),observed['diagnostics'],name+'/exact diagnostic text');diagnostics+=len(observed['diagnostics'])
            equal(observed['refreshes'],[],name+'/no refresh');equal(len(observed['calls']),len(planned['calls']),name+'/reader count')
            order=[]
            for index,(call,truth)in enumerate(zip(observed['calls'],planned['calls'])):
                equal(call['kind'],truth['kind'],name+'/reader kind');equal(call['after'],truth['value'],name+'/reader outputs');equal(call['status'],truth['status'],name+'/reader status')
                previous=entry['state']['iteration']
                before=([previous['reals_hex'][:16],previous['reals_hex'][16:32]]if keyword==61 else [previous['integers'][DEFINITIONS[keyword]]]if keyword in [64,65,67,68,69]else None)
                if index>0:before=None;payloads+=1
                equal(call['before'],before,name+'/initialized live prestate or excluded temporary');order+=['reader-enter','reader-leave']
                if index==0 and keyword in [63,66,70]:order+=['validator-enter','validator-leave']
            validator=planned.get('accepted');equal(len(observed['validators']),int(validator is not None),name+'/validator count')
            if validator is not None:
                v=observed['validators'][0];requested=planned['calls'][0]['value'][0]
                equal(v,dict(before=requested,after=requested,accepted=validator),name+'/validator ABI and unchanged input')
                if validator:accepted+=1;modes.add((keyword,dimension,requested))
                else:rejected+=1
            order+=['diagnostic']*len(planned['diagnostics']);equal(observed['order'],order,name+'/reader-validation-diagnostic ordering')
            state(h['state'],h['linked_state'],observed['state'],name+'/handler')
            if observed['cursor']:equal(h['cursor'],observed['cursor'],name+'/cursor')
            else:eof+=1
    for keyword in DEFINITIONS:
        for dimension in [2,3]:equal(dimensions[str(keyword)+'/'+str(dimension)]>0,True,'required dimension coverage '+str(keyword)+'/'+str(dimension))
    for keyword,maximum in [(63,3),(66,5),(70,6)]:
        for dimension in [2,3]:
            for mode in range(1,maximum+1):equal((keyword,dimension,mode)in modes,True,'accepted selector coverage '+str((keyword,dimension,mode)))
    for value in [rejected,payloads,diagnostics]:equal(value>0,True,'nonvacuous branch coverage')
    report=dict(status='PASS_NATIVE_ITERATION_CONTROLS_COMPARISON',imports=len(rows),handlers=sum(counts.values()),handler_counts=dict(counts),dimension_counts=dict(dimensions),accepted_validators=accepted,rejected_validators=rejected,solver_payloads=payloads,diagnostic_lines=diagnostics,lookup_tokens=lookups,checks=checks,eof_cursors_unobserved=eof,evidence_id=evidence['evidence_id'],capture_sha256=hashlib.sha256(args.capture.read_bytes()).hexdigest(),run_sha256=hashlib.sha256(args.run.read_bytes()).hexdigest(),scope='Ten typed controls61..70, selector validation/remapping and retained parameters, exact payload bytes/diagnostics including original S4 warning bug, token order and selected neighboring state. Uninitialized temporary prestates and scientific execution excluded.')
    if args.report:args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8',newline='\n')
    print(json.dumps(report))


if __name__=='__main__':main()
