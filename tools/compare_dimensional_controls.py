"""Compare source-bound dimensional-control imports with the original observation."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import struct
import subprocess

DEFINITIONS = {39:0,56:0,57:0,58:0,59:1,60:2}


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
        for key in ['dimensional_step','time_step_imports','stopping_vectors_hex','stopping_controls','dimension','components','object_count','material_count','stpinc_0x8b4','heat_method_0x868','element_layout_0x888']:
            equal(primary[key],observed[key],label+'/'+key)
        equal(linked['dimensional_step'],observed['linked_dimensional_step'],label+'/linked dimensional controls')
        equal(linked['time_step_imports'],observed['linked_time_step_imports'],label+'/linked dimensional-control imports')
        equal(linked['stopping_vectors_hex'],observed['linked_vectors_hex'],label+'/linked vectors')
        equal(linked['stopping_controls'],observed['linked_stopping'],label+'/linked stopping')
        equal(linked['dimension'],observed['linked_dimension'],label+'/linked dimension')
        time=primary['finalization_controls']
        raw=b''.join(struct.pack('<d',time[k]) for k in ['time_step_0x920','time_step_secondary_0x928','step_increment_0x930']).hex()
        equal(dict(values_hex=raw,control=time['control_0x93c'],mode=time['mode_0x990']),observed['time'],label+'/time controls')
        equal([time[k]for k in ['step_number_0x8b0','saved_step_0x744']],observed['steps'],label+'/step controls')

    def default_fields(dimension):
        table=dict(type_0x00=181,rank_0x04=0,constant_0x08=0,selectors_0x10=[0]*3,counts_0x1c=[0]*3,values_per_point_0x40=1,flag_0x50=0,preserved_0x54=0,mode_0x58=0,preserved_0x60_hex='00'*8,axes=[[],[],[]],dependent=[0],raw_hex=dict(constant='00'*8,axes=['','',''],dependent='00'*8))
        return dict(vfactr_mode=0,reals_hex=b''.join(struct.pack('<d',x)for x in [1.0 if dimension==3 else 0.0,0.5,0.0]).hex(),planar=dict(osctrl=[0]*4,cnterr_int=0,cnterr_real_hex='00'*8)if dimension==2 else None,spatial=dict(active_count=0,table=table)if dimension==3 else None)
    equal(groups[0]['before']['state']['dimensional_step'],default_fields(3),'native3D defaults')
    equal(groups[0]['before']['state']['linked_dimensional_step'],default_fields(2),'native2D defaults')
    dimensions=Counter();ignored=payloads=payload_eof=allocations=setters=lookups=0
    for name,row,group in zip(plan['ordered_imports'],rows,groups):
        expected=next(x for x in plan['inputs']if x['file']==name)
        equal(group['before']['file'],name,name+'/input identity');equal(group['after']['file'],name,name+'/output identity')
        equal(group['after']['status'],expected['expected_wrapper_status'],name+'/planned wrapper')
        equal(group['after']['state']['dimensional_step'],expected['expected_final'],name+'/planned final fields')
        if 'expected_nstep'in expected:equal(group['after']['state']['stopping_controls']['nstep'],expected['expected_nstep'],name+'/positive same-line neighbor')
        equal(row['result']['outcome'],'parsed',name+'/supported');equal(row['result']['status'],group['after']['status'],name+'/wrapper')
        state(row['before_state'],row['before_linked_state'],group['before']['state'],name+'/before');state(row['state'],row['linked_state'],group['after']['state'],name+'/after')
        native_tokens=[{k:t[k]for k in ['token','id','category']}for t in group['lookups']]
        cpp_tokens=[{k:t[k]for k in ['token','id','category']}for t in row['result']['events']]
        equal(cpp_tokens,native_tokens,name+'/complete lookup order and ignored payload');lookups+=len(native_tokens)
        handlers=[h for h in row['handler_states']if h['id']in DEFINITIONS]
        equal(len(handlers),len(expected['expected_handlers']),name+'/planned handlers');equal(len(group['handlers']),len(handlers),name+'/native handlers');equal(len(group['entries']),len(handlers),name+'/entry pairing')
        for h,observed,entry,planned in zip(handlers,group['handlers'],group['entries'],expected['expected_handlers']):
            keyword=planned['id'];counts[keyword]+=1;dimension=entry['state']['dimension'];dimensions[str(keyword)+'/'+str(dimension)]+=1
            equal(h['id'],keyword,name+'/C++ keyword');equal(observed['id'],keyword,name+'/native keyword');equal(entry['id'],keyword,name+'/entry keyword')
            equal(observed['status'],planned['status'],name+'/planned handler status');equal(h['status'],observed['status'],name+'/C++ status')
            equal(observed['state']['dimensional_step'],planned['expected_fields'],name+'/source-planned handler fields')
            equal(observed['refreshes'],[],name+'/no refresh')
            ignored_here=(keyword in [39,56]and dimension==3)or(keyword in [59,60]and dimension==2)
            equal(len(observed['calls']),0 if ignored_here else 1,name+'/direct reader count')
            order=[]
            if ignored_here:ignored+=1
            else:
                call=observed['calls'][0]
                kind={39:'quad',56:'mixed',57:'integer'if dimension==2 else'pair'}.get(keyword,'scalar')
                equal(call['kind'],kind,name+'/reader kind');equal(call['after'],planned['value'],name+'/planned live assignments');equal(call['status'],0 if planned['status']==0 else 1,name+'/reader status')
                previous=entry['state']['dimensional_step']
                if keyword==39:before=previous['planar']['osctrl']
                elif keyword==56:before=[previous['planar']['cnterr_int'],previous['planar']['cnterr_real_hex']]
                elif keyword==57:before=[previous['vfactr_mode']]+([0]if dimension==3 else [])
                else:before=['00'*8]if keyword==60 else [previous['reals_hex'][DEFINITIONS[keyword]*16:(DEFINITIONS[keyword]+1)*16]]
                equal(call['before'],before,name+'/live versus temporary prestate');order=['reader-enter','reader-leave']
            payload=planned.get('payload')
            equal(len(observed['payloads']),int(payload is not None),name+'/conditional payload count')
            if payload is not None:
                payloads+=1;payload_eof+=int(payload['status']==1);allocations+=1
                call=observed['payloads'][0];equal(call['rank'],0,name+'/payload rank');equal(call['width'],payload['width'],name+'/width alias');equal(call['status'],payload['status'],name+'/payload return');equal(call['after_hex'],payload['hex'],name+'/partial payload bytes')
                events=observed['allocations'];equal([e['phase']for e in events],['allocate-enter','release-enter','release-leave','allocate-leave'],name+'/ownership ordering')
                equal([e['width']for e in events],[payload['width']]*4,name+'/width before allocation');equal([e['dependent_null']for e in events],[False,False,True,False],name+'/owned allocation release')
                equal(events[-1]['zero_hex'],'00'*(payload['width']*8),name+'/calloc before direct writes')
                order+=['payload-enter','allocate-enter','release-enter','release-leave','allocate-leave','payload-leave']
            else:equal(observed['allocations'],[],name+'/no table allocation')
            setter_here=keyword==60 and dimension==3
            equal(len(observed['setters']),int(setter_here),name+'/ODMAX setter count')
            if setter_here:
                setters+=1;setter=observed['setters'][0]
                equal(setter['before'],entry['state']['dimensional_step']['reals_hex'][32:48],name+'/ODMAX old owner value');equal(setter['after'],planned['value'][0],name+'/ODMAX temporary applied after read')
                order+=['setter-enter','setter-leave']
            equal(observed['order'],order,name+'/complete immediate call ordering')
            state(h['state'],h['linked_state'],observed['state'],name+'/handler')
            if observed['cursor']:equal(h['cursor'],observed['cursor'],name+'/cursor')
            else:eof+=1
    for keyword in DEFINITIONS:
        for dimension in [2,3]:equal(dimensions[str(keyword)+'/'+str(dimension)]>0,True,'required dimension coverage '+str(keyword)+'/'+str(dimension))
    for value in [ignored,payloads,payload_eof,setters]:equal(value>0,True,'nonvacuous conditional coverage')
    report=dict(status='PASS_NATIVE_DIMENSIONAL_CONTROLS_COMPARISON',imports=len(rows),handlers=sum(counts.values()),handler_counts=dict(counts),dimension_counts=dict(dimensions),ignored_handlers=ignored,table_payloads=payloads,ignored_payload_eof=payload_eof,owned_reallocations=allocations,odmax_setters=setters,lookup_tokens=lookups,checks=checks,eof_cursors_unobserved=eof,evidence_id=evidence['evidence_id'],capture_sha256=hashlib.sha256(args.capture.read_bytes()).hexdigest(),run_sha256=hashlib.sha256(args.run.read_bytes()).hexdigest(),scope='Six dimensional handlers39,56..60; typed initialized fields only, owned typeB5 vector, partial failure and ignored payload returns, allocation/setter ordering, token consumption and selected neighboring state. No scientific consumer or native object ABI claim.')
    if args.report:args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8',newline='\n')
    print(json.dumps(report))


if __name__=='__main__':main()
