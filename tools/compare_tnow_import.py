"""Compare TNOW reader outputs and source-derived partial mutations with C++."""
import argparse
import hashlib
import json
import math
from pathlib import Path
import struct
import subprocess


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['driver','reader-driver','capture','run','plan']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--report',type=Path)
    a=p.parse_args();e=json.loads(a.capture.read_bytes());run=json.loads(a.run.read_bytes());plan=json.loads(a.plan.read_bytes());n=e['normalized_result']
    assert run['status']=='completed'and run['exit_code']==0 and run['executable_sha256']=='f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
    assert n['pid']==run['pid']and n['state']=='detached'and n['cleanup_error']is None and not n['messages_truncated']
    assert hashlib.sha256((a.run.parent/'observer-source.js').read_bytes()).hexdigest()==n['source_sha256']
    assert run['ordered_imports']==plan['ordered_imports']
    for f in run['inputs']:
        expected=next(x for x in plan['inputs']if x['file']==f['name'])
        assert f['sha256_before']==f['sha256_after']==f['source_sha256_after']==expected['sha256']
        assert hashlib.sha256((a.run.parent/f['name']).read_bytes()).hexdigest()==expected['sha256']
    groups=[]
    for m in n['messages']:
        assert m['type']!='error',m
        if m['type']!='send':continue
        x=m['payload'];assert x['operation']!='probe-error',x
        if x['operation']=='import-before':groups.append(dict(before=x,entries=[],handlers=[]))
        elif x['operation']=='import-after':groups[-1]['after']=x
        elif x['operation']=='handler-before':groups[-1]['entries'].append(x)
        elif x['operation']=='handler-after':groups[-1]['handlers'].append(x)
    data=[(a.run.parent/f).read_bytes().replace(b'\r\n',b'\n')for f in plan['ordered_imports']]
    commands=['initialized 3 20 0','link 2']+['import '+b.hex()for b in data]
    result=subprocess.run([str(a.driver.resolve())],input='\n'.join(commands)+'\n',text=True,capture_output=True,check=True)
    cpp=[json.loads(line)for line in result.stdout.splitlines()]
    assert len(cpp)==len(groups)==len(data)
    reader_commands=[]
    for b,row in zip(data,cpp):
        for event in row['result']['events']:
            if event['id']==31:reader_commands.append('real4 0 '+str(event['offset']+len(event['token']))+' '+b.hex()+' -')
    reader_result=subprocess.run([str(a.reader_driver.resolve())],input='\n'.join(reader_commands)+'\n',text=True,capture_output=True,check=True)
    readers=iter(json.loads(line)for line in reader_result.stdout.splitlines())
    checks=handlers=eof=0

    def equal(got,expected,label):
        nonlocal checks
        checks+=1
        if got!=expected:raise AssertionError((label,str(got)[:350],str(expected)[:350]))

    def state(primary,linked,native,label):
        for key in ['tnow','dimension','components','object_count','material_count']:equal(primary[key],native[key],label+'/'+key)
        equal(linked['tnow'],native['linked_tnow'],label+'/linked TNOW')
        equal(linked['dimension'],native['linked_dimension'],label+'/linked dimension')
        equal([primary['simulation_mode'][k]for k in ['mode_0x844','flag_0x854','flag_0x858']],native['mode'],label+'/mode')
        equal(primary['unit']['selector'],native['unit'],label+'/unit')
        equal([primary['global_units'][k]for k in ['mode','display_mode']],native['global_unit_flags'],label+'/process flags')

    zero=dict(field_0xd90_hex='00'*8,field_0xd98_hex='00'*8,field_0xda0=0,field_0xda8_hex='00'*8)
    equal(groups[0]['before']['state']['tnow'],zero,'native initialized profile')
    for filename,row,g in zip(plan['ordered_imports'],cpp,groups):
        expected=next(x['expected_handler_statuses']for x in plan['inputs']if x['file']==filename)
        equal(g['before']['file'],filename,'input identity');equal(g['after']['file'],filename,'output identity')
        equal(row['result']['outcome'],'parsed',filename+'/supported');equal(row['result']['status'],g['after']['status'],filename+'/wrapper')
        state(row['before_state'],row['before_linked_state'],g['before']['state'],filename+'/before')
        state(row['state'],row['linked_state'],g['after']['state'],filename+'/after')
        hs=[h for h in row['handler_states']if h['id']==31]
        equal(len(hs),len(expected),filename+'/expected handlers');equal(len(g['handlers']),len(hs),filename+'/native handlers');equal(len(g['entries']),len(hs),filename+'/entry pairing')
        for h,entry,observed,status in zip(hs,g['entries'],g['handlers'],expected):
            handlers+=1;r=next(readers)
            equal(h['status'],status,filename+'/planned status');equal(h['status'],observed['status'],filename+'/native status')
            equal(len(observed['calls']),1,filename+'/reader calls');call=observed['calls'][0]
            equal(call['status'],0 if status==0 else 1,filename+'/native helper status');equal(r['status'],call['status'],filename+'/replacement helper status')
            equal(r['values_hex'],''.join(call['values_hex']),filename+'/all four raw reader outputs')
            values=[struct.unpack('<d',bytes.fromhex(b))[0]for b in call['values_hex']];truth=dict(entry['state']['tnow'])
            for index,key in [(0,'field_0xd90_hex'),(1,'field_0xd98_hex'),(3,'field_0xda8_hex')]:
                if values[index]>-1e20:truth[key]=call['values_hex'][index]
            integer=math.trunc(values[2]);truth['field_0xda0']=integer if -2147483648<=integer<=2147483647 else -2147483648
            equal(observed['state']['tnow'],truth,filename+'/source store and CVTTSD2SI truth')
            state(h['state'],h['linked_state'],observed['state'],filename+'/handler')
            equal(observed['refreshes'],[],filename+'/no refresh')
            equal(observed['state']['linked_tnow'],entry['state']['linked_tnow'],filename+'/linked isolation')
            if observed['cursor']:equal(h['cursor'],observed['cursor'],filename+'/cursor');equal(r['cursor'],observed['cursor'],filename+'/reader cursor')
            else:eof+=1
    equal(next(readers,None),None,'all helper requests consumed')
    report=dict(status='PASS_NATIVE_TNOW_COMPARISON',imports=len(cpp),handlers=handlers,checks=checks,eof_cursors_unobserved=eof,evidence_id=e['evidence_id'],capture_sha256=hashlib.sha256(a.capture.read_bytes()).hexdigest(),run_sha256=hashlib.sha256(a.run.read_bytes()).hexdigest(),scope='Initialized ordinary roots, finite real inputs, source-derived thresholds and x64 truncation. Four raw helper outputs, full selected handler state and linked isolation compare. Scientific consumers and general decimal rounding remain pending.')
    if a.report:a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8',newline='\n')
    print(json.dumps(report))


if __name__=='__main__':main()
