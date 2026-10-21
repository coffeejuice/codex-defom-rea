"""Compare source-bound stopping-vector imports with the original observation."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import struct
import subprocess

DEFINITIONS = {43:0, 44:1, 45:2}


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
        if value['operation']=='import-before':groups.append(dict(before=value,entries=[],handlers=[]))
        elif value['operation']=='import-after':groups[-1]['after']=value
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
        for key in ['stopping_vectors_hex','stopping_controls','dimension','components','object_count','material_count','stpinc_0x8b4','heat_method_0x868','element_layout_0x888']:
            equal(primary[key],observed[key],label+'/'+key)
        equal(linked['stopping_vectors_hex'],observed['linked_vectors_hex'],label+'/linked vectors')
        equal(linked['stopping_controls'],observed['linked_stopping'],label+'/linked stopping')
        equal(linked['dimension'],observed['linked_dimension'],label+'/linked dimension')
        time=primary['finalization_controls']
        raw=b''.join(struct.pack('<d',time[k]) for k in ['time_step_0x920','time_step_secondary_0x928','step_increment_0x930']).hex()
        equal(dict(values_hex=raw,control=time['control_0x93c'],mode=time['mode_0x990']),observed['time'],label+'/time controls')
        equal([time[k]for k in ['step_number_0x8b0','saved_step_0x744']],observed['steps'],label+'/step controls')

    equal(groups[0]['before']['state']['stopping_vectors_hex'],'00'*72,'native3D defaults')
    equal(groups[0]['before']['state']['linked_vectors_hex'],'00'*72,'native2D defaults')
    widths=Counter()
    for name,row,group in zip(plan['ordered_imports'],rows,groups):
        expected=next(x for x in plan['inputs']if x['file']==name)
        equal(group['before']['file'],name,name+'/input identity')
        equal(group['after']['file'],name,name+'/output identity')
        equal(group['after']['status'],expected['expected_wrapper_status'],name+'/planned wrapper')
        equal(group['after']['state']['stopping_vectors_hex'],expected['expected_final'],name+'/planned final vectors')
        equal(row['result']['outcome'],'parsed',name+'/supported')
        equal(row['result']['status'],group['after']['status'],name+'/wrapper')
        state(row['before_state'],row['before_linked_state'],group['before']['state'],name+'/before')
        state(row['state'],row['linked_state'],group['after']['state'],name+'/after')
        handlers=[h for h in row['handler_states']if h['id']in DEFINITIONS]
        equal(len(handlers),len(expected['expected_handlers']),name+'/planned handlers')
        equal(len(group['handlers']),len(handlers),name+'/native handlers')
        equal(len(group['entries']),len(handlers),name+'/entry pairing')
        for h,observed,entry,planned in zip(handlers,group['handlers'],group['entries'],expected['expected_handlers']):
            keyword=planned['id'];counts[keyword]+=1
            equal(h['id'],keyword,name+'/C++ keyword')
            equal(observed['id'],keyword,name+'/native keyword')
            equal(entry['id'],keyword,name+'/entry keyword')
            equal(observed['status'],planned['status'],name+'/planned status')
            equal(h['status'],observed['status'],name+'/handler status')
            equal(len(observed['calls']),1,name+'/one direct reader')
            call=observed['calls'][0]
            width=3 if entry['state']['dimension']==3 else 2
            widths[str(keyword)+'/'+str(width)]+=1
            equal(call['count'],width,name+'/dimension width')
            equal(call['after'],planned['value'],name+'/source-planned live value')
            equal(call['status'],0 if planned['status']==0 else 1,name+'/reader status')
            start=DEFINITIONS[keyword]*48;end=start+width*16
            previous=entry['state']['stopping_vectors_hex']
            equal(call['before'],previous[start:end],name+'/reader prestate')
            truth=previous[:start]+call['after']+previous[end:]
            equal(observed['state']['stopping_vectors_hex'],truth,name+'/only selected destinations change')
            equal(observed['refreshes'],[],name+'/no refresh')
            state(h['state'],h['linked_state'],observed['state'],name+'/handler')
            if observed['cursor']:equal(h['cursor'],observed['cursor'],name+'/cursor')
            else:eof+=1
    for keyword in DEFINITIONS:
        for width in [2,3]:equal(widths[str(keyword)+'/'+str(width)]>0,True,'required dimension coverage '+str(keyword)+'/'+str(width))
    report=dict(status='PASS_NATIVE_STOPPING_VECTORS_COMPARISON',imports=len(rows),handlers=sum(counts.values()),handler_counts=dict(counts),dimension_counts=dict(widths),checks=checks,eof_cursors_unobserved=eof,evidence_id=evidence['evidence_id'],capture_sha256=hashlib.sha256(args.capture.read_bytes()).hexdigest(),run_sha256=hashlib.sha256(args.run.read_bytes()).hexdigest(),scope='Three raw dimension-dependent vectors, exact binary64, owned2D/3D defaults, scalar stopping/time/step/layout neighbors and linked isolation. Finite initialized input domain; scientific generation consumers remain pending.')
    if args.report:args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8',newline='\n')
    print(json.dumps(report))


if __name__=='__main__':main()
