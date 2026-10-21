"""Compare retained passive retained material observations with the C++ model."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

IDS=[110,111,112,113]
BASE=['field_0x4','field_0x8','field_0x17e8','mode_0x1cc8','reference_count_0x1ccc','field_0x1e40','field_0x2150','field_0x2154','field_0x2158','field_0x215c']


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    for name in ['plan','run','evidence','driver','report']:ap.add_argument('--'+name,type=Path,required=True)
    ap.add_argument('--freeze',type=Path)
    a=ap.parse_args();plan=json.loads(a.plan.read_bytes());run=json.loads((a.run/'run.json').read_bytes())
    evidence=json.loads(a.evidence.read_bytes());native=evidence['normalized_result']
    assert run['status']=='completed'and run['exit_code']==0
    assert run['executable_sha256']=='f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
    assert native['pid']==run['pid']and native['state']=='detached'
    assert native['cleanup_error']is None and not native['messages_truncated']
    assert hashlib.sha256((a.run/'observer-source.js').read_bytes()).hexdigest()==native['source_sha256']
    assert run['ordered_imports']==plan['ordered_imports']
    for item,truth in zip(run['inputs'],plan['inputs']):
        assert item['name']==truth['file']
        assert item['sha256_before']==item['sha256_after']==item['source_sha256_after']==truth['sha256']
        assert hashlib.sha256((a.run/item['name']).read_bytes()).hexdigest()==truth['sha256']
    freeze=json.loads(a.freeze.read_bytes())if a.freeze else None

    def frozen():
        if freeze:
            for f in freeze['files']:assert hashlib.sha256(Path(f['path']).read_bytes()).hexdigest()==f['sha256'],f['path']
    frozen();groups=[];installed=0
    for message in native['messages']:
        assert message['type']!='error',message
        if message['type']!='send':continue
        v=message['payload'];assert v['operation']!='probe-error',v
        op=v['operation']
        if op=='hooks-installed':installed+=1;assert v['pid']==run['pid']
        elif op=='import-before':groups.append(dict(before=v,handlers=[],entries=[],lookups=[]))
        elif op=='import-after':groups[-1]['after']=v
        elif op=='handler-before':groups[-1]['entries'].append(v)
        elif op=='handler-after':groups[-1]['handlers'].append(v)
        elif op=='lookup':groups[-1]['lookups'].append(v)
    assert installed==1 and len(groups)==len(plan['inputs'])
    checks=eof=0

    def equal(x,y,label):
        nonlocal checks
        checks+=1
        def diff(x,y,p):
            if type(x)!=type(y):return p,x,y
            if isinstance(x,dict):
                if x.keys()!=y.keys():return p,list(x),list(y)
                for k in x:
                    if x[k]!=y[k]:return diff(x[k],y[k],p+'/'+k)
            elif isinstance(x,list):
                if len(x)!=len(y):return p,len(x),len(y)
                for i in range(len(x)):
                    if x[i]!=y[i]:return diff(x[i],y[i],p+'/'+str(i))
            else:return p,x,y
        if x!=y:raise AssertionError(diff(x,y,label))

    def state(cpp,observed,label):
        for key in ['dimension','material_count','object_count','components']:equal(cpp[key],observed[key],label+'/'+key)
        equal(cpp['root_fields']['unit_selector'],observed['unit_selector'],label+'/unit')
        equal(cpp['stopping_controls']['nstep'],observed['nstep'],label+'/nstep')
        equal(len(cpp['materials']),len(observed['materials']),label+'/materials')
        for c,m in zip(cpp['materials'],observed['materials']):
            for key in ['id','name_hex','direct_fields']:equal(c[key],m[key],label+'/material/'+str(m['id'])+'/'+key)
            equal([c[k]for k in BASE],m['base_integers'],label+'/base integers')
            equal(c['hardening_imports']['hdnrul_0x668'],m['hardening_imports']['hdnrul_0x668'],label+'/HDNRUL')
            for key,t in m['hardening_imports']['tables'].items():
                ct=c['hardening_imports']['tables'][key]
                compact=dict(metadata=[ct['type_0x00'],ct['rank_0x04'],*ct['selectors_0x10'],*ct['counts_0x1c'],ct['values_per_point_0x40'],ct['flag_0x50'],ct['preserved_0x54'],ct['mode_0x58']],constant_hex=ct['raw_hex']['constant'],preserved60_hex=ct['preserved_0x60_hex'],axes_hex=ct['raw_hex']['axes'],dependent_hex=ct['raw_hex']['dependent'])
                equal(compact,t,label+'/hardening'+key)

            for key in ['mode_0xac8','reference_0xda0','axis_count_0xda4']:
                equal(c['creep'][key],m['creep'][key],label+'/creep/'+key)
            for key,t in m['creep']['tables'].items():
                ct=c['creep']['tables'][key]
                compact=dict(metadata=[ct['type_0x00'],ct['rank_0x04'],*ct['selectors_0x10'],*ct['counts_0x1c'],ct['values_per_point_0x40'],ct['flag_0x50'],ct['preserved_0x54'],ct['mode_0x58']],constant_hex=ct['raw_hex']['constant'],preserved60_hex=ct['preserved_0x60_hex'],axes_hex=ct['raw_hex']['axes'],dependent_hex=ct['raw_hex']['dependent'])
                equal(compact,t,label+'/creep/table'+key)
            for key,t in m['retained_tables'].items():
                ct=c['retained_tables'][key]
                compact=dict(metadata=[ct['type_0x00'],ct['rank_0x04'],*ct['selectors_0x10'],*ct['counts_0x1c'],ct['values_per_point_0x40'],ct['flag_0x50'],ct['preserved_0x54'],ct['mode_0x58']],constant_hex=ct['raw_hex']['constant'],preserved60_hex=ct['preserved_0x60_hex'],axes_hex=ct['raw_hex']['axes'],dependent_hex=ct['raw_hex']['dependent'])
                equal(compact,t,label+'/retained/table'+key)
            frc_keys=['mode_0x1ad0','reference_0x1b08','scalars_0x1ad8_hex','scalar_0x1b88_hex','coefficients_0x1c80_hex']
            equal({k:c['frcmod'][k]for k in frc_keys},{k:m['frcmod'][k]for k in frc_keys},label+'/FRCMOD fields')
            for key,t in m['frcmod']['tables'].items():
                ct=c['frcmod']['tables'][key]
                compact=dict(metadata=[ct['type_0x00'],ct['rank_0x04'],*ct['selectors_0x10'],*ct['counts_0x1c'],ct['values_per_point_0x40'],ct['flag_0x50'],ct['preserved_0x54'],ct['mode_0x58']],constant_hex=ct['raw_hex']['constant'],preserved60_hex=ct['preserved_0x60_hex'],axes_hex=ct['raw_hex']['axes'],dependent_hex=ct['raw_hex']['dependent'])
                equal(compact,t,label+'/FRCMOD'+key)

            equal({key:c['curve_imports'][key]for key in ['difcoe_mode_0x950','difcoe_reference_0xac0']},{key:m['curve_imports'][key]for key in ['difcoe_mode_0x950','difcoe_reference_0xac0']},label+'/curve flags')
            for key,t in m['curve_imports']['tables'].items():
                ct=c['curve_imports']['tables'][key]
                compact=dict(metadata=[ct['type_0x00'],ct['rank_0x04'],*ct['selectors_0x10'],*ct['counts_0x1c'],ct['values_per_point_0x40'],ct['flag_0x50'],ct['preserved_0x54'],ct['mode_0x58']],constant_hex=ct['raw_hex']['constant'],preserved60_hex=ct['preserved_0x60_hex'],axes_hex=ct['raw_hex']['axes'],dependent_hex=ct['raw_hex']['dependent'])
                equal(compact,t,label+'/curve'+key)

            for key,t in m['property_tables'].items():
                ct=c['property_tables'][key]
                equal([ct['type_0x00'],ct['rank_0x04'],*ct['selectors_0x10'],*ct['counts_0x1c'],ct['values_per_point_0x40'],ct['flag_0x50'],ct['preserved_0x54'],ct['mode_0x58']],t['metadata'],label+'/table'+key+'/metadata')
                equal(ct['raw_hex']['constant'],t['constant_hex'],label+'/table'+key+'/constant')
                equal(ct['preserved_0x60_hex'],t['preserved60_hex'],label+'/table'+key+'/preserved60')
                equal(ct['raw_hex']['axes'],['','',''],label+'/constant table axes')
                equal(ct['raw_hex']['dependent'],'',label+'/constant table dependent')

    commands=['initialized 3 20 0','link 2']+['import '+(a.run/name).read_bytes().hex()for name in plan['ordered_imports']]
    result=subprocess.run([str(a.driver.resolve())],input='\n'.join(commands)+'\n',text=True,capture_output=True,check=True)
    a.report.with_suffix('.cpp.jsonl').write_text(result.stdout,encoding='utf-8')
    rows=[json.loads(v)for v in result.stdout.splitlines()];equal(len(rows),len(groups),'replacement imports')
    for truth,row,group in zip(plan['inputs'],rows,groups):
        name=truth['file'];equal(Path(group['before']['file']).name,name,name+'/native file')
        equal(row['result']['outcome'],'parsed',name+'/supported')
        equal(row['result']['status'],group['after']['status'],name+'/status')
        equal(group['after']['status'],truth['expected_wrapper_status'],name+'/source status')
        for phase,c,l in [('before',row['before_state'],row['before_linked_state']),('after',row['state'],row['linked_state'])]:
            state(c,group[phase]['state']['primary'],name+'/'+phase)
            state(l,group[phase]['state']['linked'],name+'/'+phase+'/linked')
        equal([{k:t[k]for k in ['token','id','category']}for t in row['result']['events']],
              [{k:t[k]for k in ['token','id','category']}for t in group['lookups']],name+'/lookup order')
        hs=[h for h in row['handler_states']if h['id']in IDS]
        equal(len(hs),len(group['handlers']),name+'/handlers')
        for i,(h,n)in enumerate(zip(hs,group['handlers'])):
            label=name+'/handler'+str(i);equal(h['id'],n['id'],label+'/id');equal(h['status'],n['status'],label+'/status')
            state(h['state'],n['state']['primary'],label);state(h['linked_state'],n['state']['linked'],label+'/linked')
            if n['cursor']:equal(h['cursor'],n['cursor'],label+'/cursor')
            else:eof+=1
            if 'expected_handlers'in truth:
                expected=truth['expected_handlers'][i]
                for key in ['id','status','calls','headers','reads','refreshes']:equal(n[key],expected[key],label+'/source '+key)
                equal([{k:v[k]for k in ['kind','slot']if k in v}for v in n['order']if v['kind']in['reset-preserve','reset-direct','payload-enter','payload-leave','coefficients-enter','coefficients-leave']],expected['expected_actions'],label+'/source reset and payload order')
                equal({str(v['id']):v['retained_tables']for v in n['state']['primary']['materials']},expected['expected_retained_tables'],label+'/source fields')
                order=[v['kind']for v in n['order']]
                if expected['refreshes']:equal(order.index('refresh')<order.index('header-enter'),True,label+'/creation refresh before read')
        if 'expected_handlers'in truth:equal(len(hs),len(truth['expected_handlers']),name+'/source handler count')
        if 'expected_retained_tables'in truth:
            equal({str(v['id']):v['retained_tables']for v in group['after']['state']['primary']['materials']},truth['expected_retained_tables'],name+'/source final fields')
            equal(group['after']['state']['primary']['nstep'],truth['expected_nstep'],name+'/source trailing-token guard')
    frozen()
    report=dict(status='PASS_NATIVE_RETAINED_MATERIAL',imports=len(rows),handlers=sum(len(g['handlers'])for g in groups),checks=checks,unobserved_eof_cursors=eof,evidence_id=evidence['evidence_id'],evidence_sha256=hashlib.sha256(a.evidence.read_bytes()).hexdigest(),frozen_files=len(freeze['files'])if freeze else 0,qualification=plan['qualification'])
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps(report))


if __name__=='__main__':main()
