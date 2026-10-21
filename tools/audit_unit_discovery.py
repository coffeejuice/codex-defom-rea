"""Audit original UNIT observations against retained source; no C++ equivalence claim."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
from extract_unit_data import TARGET, extract


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--capture', type=Path, required=True)
    p.add_argument('--run', type=Path, required=True)
    p.add_argument('--plan', type=Path, default=Path('analysis/unit-discovery-inputs.json'))
    p.add_argument('--static', type=Path, default=Path('analysis/unit-static-evidence.json'))
    p.add_argument('--data', type=Path, default=Path('analysis/unit-static-data.json'))
    p.add_argument('--report', type=Path)
    args = p.parse_args()
    e, run, plan = [json.loads(x.read_bytes()) for x in [args.capture,args.run,args.plan]]
    n = e['normalized_result']
    assert run['status'] == 'completed' and run['exit_code'] == 0 and run['executable_sha256'] == TARGET
    assert n['state'] == 'detached' and n['cleanup_error'] is None and not n['messages_truncated']
    assert n['pid'] == run['pid'] and run['ordered_imports'] == plan['ordered_imports']
    assert hashlib.sha256((args.run.parent/'observer-source.js').read_bytes()).hexdigest() == n['source_sha256']
    for f in run['inputs']:
        binding = next(x for x in plan['inputs'] if x['file'] == f['name'])
        assert f['sha256_before'] == f['sha256_after'] == f['source_sha256_after'] == binding['sha256']
        assert hashlib.sha256((args.run.parent/f['name']).read_bytes()).hexdigest() == binding['sha256']
    data = extract(args.static)
    assert data == json.loads(args.data.read_bytes())
    records = json.loads(args.static.read_bytes())['records']
    code = next(r['normalized_result'] for r in records if r['operation']=='procedure_pseudo_code' and r['parameters']['procedure']=='0x1401f4100')
    first, second = code.split('  else {', 1)
    control_offsets = [0x3b0,0x4b8,0x3d0,0x4c8,0x540,0x5b8,0x628,0x630]
    profiles = {}
    for selector, part in [(1,first),(2,second)]:
        stores = {int(a,16):int(b,16).to_bytes(8,'little').hex() for a,b in re.findall(r'param_1 \+ 0x([0-9a-f]+)\) = 0x([0-9a-f]+)',part)}
        profiles[selector] = [stores[o] for o in control_offsets]
    groups = []
    for m in n['messages']:
        assert m['type'] != 'error',m
        if m['type'] != 'send': continue
        x = m['payload']
        assert x['operation'] not in ('probe-error','outside-unit-call'),x
        if x['operation']=='import-before': groups.append(dict(before=x,entries=[],handlers=[]))
        elif x['operation']=='import-after': groups[-1]['after']=x
        elif x['operation']=='handler-before': groups[-1]['entries'].append(x)
        elif x['operation']=='handler-after': groups[-1]['handlers'].append(x)
    assert len(groups)==len(plan['ordered_imports'])
    checks = changes = handlers = eof = 0

    def equal(a,b,label):
        nonlocal checks
        checks += 1
        def difference(a,b,path=''):
            if isinstance(a,dict) and isinstance(b,dict) and a.keys()==b.keys():
                for k in a:
                    if a[k]!=b[k]:return difference(a[k],b[k],path+'/'+str(k))
            if isinstance(a,list) and isinstance(b,list) and len(a)==len(b):
                for i,(left,right)in enumerate(zip(a,b)):
                    if left!=right:return difference(left,right,path+'/'+str(i))
            return path,repr(a)[:160],repr(b)[:160]
        if a!=b:raise AssertionError((label,*difference(a,b)))

    def unit(selector):
        profile=profiles[2 if selector==2 else 1]
        tables=[]
        for constant in profile[2:6]:
            tables.append(dict(type=0,rank=0,constant_hex=constant,selectors=[0]*3,counts=[0]*3,width=1,flag50=0,metadata54=0,mode58=0,metadata60_hex='00'*8,storage_null=[True]*5))
        return dict(selector=selector,scalar_hex=profile,tables=tables)

    def global_state(mode,initialized):
        a,b=copy.deepcopy(data['table_a']),copy.deepcopy(data['table_b'])
        for row in a: row['raw64'][3]=row['raw64'][0]
        for row in b:
            value=row['text'][3 if mode==0 else 2]
            row['text'][1]=row['text'][4]=value
        return dict(mode=mode,display_mode=mode,count=28 if initialized else 0,limit=6000 if initialized else 0,field90=4 if initialized else 0,field94=0,table_a=a if initialized else None,table_b=b if initialized else None)

    selector=0
    global_mode=0
    initialized=False
    equal(groups[0]['before']['state']['unit'],unit(0),'observed startup root profile; selector0 distinct from process mode')
    for filename,g in zip(plan['ordered_imports'],groups):
        expected=next(x['expected_values_and_handler_statuses']for x in plan['inputs']if x['file']==filename)
        equal(g['before']['file'],filename,'import identity')
        equal(g['before']['state']['unit'],unit(selector),filename+' root continuity')
        equal(g['before']['state']['global_units'],global_state(global_mode,initialized),filename+' globals before')
        equal(len(g['handlers']),len(expected),filename+' handler count')
        equal(len(g['entries']),len(expected),filename+' entry count')
        for entry,after,(value,status) in zip(g['entries'],g['handlers'],expected):
            handlers+=1
            equal(entry['file'],filename,'handler input identity')
            equal(after['file'],filename,'handler output identity')
            equal(after['status'],status,'source-derived handler status')
            equal(entry['state']['unit'],unit(selector),'handler before unit')
            equal(after['calls'][0],dict(call='reader',option=0,value=value,status=0 if status==0 else 1),'temporary reader status/value')
            changed=value in (1,2) and value!=selector
            calls=['reader']
            if changed:
                changes+=1
                calls+=['reset']+['table-reset']*4+['reset-after','global-mode','display-mode','display-refresh']
                pre=unit(selector);pre['selector']=value
                equal(after['calls'][1],dict(call='reset',before=pre),'selector stored before reset')
                equal(after['calls'][2:6],[dict(call='table-reset',offset=o,type=0)for o in [0xb18,0xc10,0xc88,0xd00]],'ordered destructive table resets')
                equal(after['calls'][6],dict(call='reset-after',after=unit(value)),'all exact reset defaults')
                equal(after['calls'][7:],[dict(call='global-mode',value=value),dict(call='display-mode',value=value),dict(call='display-refresh')],'global setter ordering')
                selector=value;global_mode=1 if value==2 else 0;initialized=True
            equal([c['call']for c in after['calls']],calls,'conditional call sequence')
            equal(after['state']['unit'],unit(selector),'handler after unit')
            equal(after['state']['global_units'],global_state(global_mode,initialized),'all28 display and numeric records')
            equal(after['state']['linked_unit'],entry['state']['linked_unit'],'linked controls isolation')
            equal(after['refreshes'],[],'no layout refresh')
            for key in ['dimension','linked_dimension','components','object_count','material_count','mesh_number','mode','type']:
                equal(after['state'][key],entry['state'][key],'unaffected '+key)
            if after['cursor']==0:eof+=1
        equal(g['after']['file'],filename,'wrapper output identity')
        equal(g['after']['status'],2 if any(s!=0 for v,s in expected)else 0,'sticky wrapper status')
        equal(g['after']['state']['unit'],unit(selector),'unit retained through header/reset/finalizer')
        equal(g['after']['state']['linked_unit'],unit(0),'linked reset profile')
        equal(g['after']['state']['global_units'],global_state(global_mode,initialized),'globals retained through wrapper')
    report=dict(status='PASS_ORIGINAL_UNIT_SOURCE_AUDIT',imports=len(groups),handlers=handlers,unit_changes=changes,table_resets=4*changes,checks=checks,eof_cursors_unobserved=eof,evidence_id=e['evidence_id'],
                capture_sha256=hashlib.sha256(args.capture.read_bytes()).hexdigest(),run_sha256=hashlib.sha256(args.run.read_bytes()).hexdigest(),
                scope='Source-bound original observation audit only; C++ comparison and input chronology are separate claims. Fresh empty four-table profile, selected original-console imports; full labels/raw64 entries compare in both modes. Populated table storage and scientific consumers remain pending.')
    if args.report:args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8',newline='\n')
    print(json.dumps(report))


if __name__=='__main__':main()
