"""Compare C++ root/global unit state against source-audited original captures."""
import argparse
import json
from pathlib import Path
import subprocess
import sys
import struct


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--driver',type=Path,required=True)
    p.add_argument('--capture',type=Path,required=True)
    p.add_argument('--run',type=Path,required=True)
    p.add_argument('--plan',type=Path,default=Path('analysis/unit-discovery-inputs.json'))
    p.add_argument('--report',type=Path)
    args=p.parse_args()
    root=Path(__file__).resolve().parents[1]
    audit=subprocess.run([sys.executable,str(root/'tools/audit_unit_discovery.py'),'--capture',str(args.capture),'--run',str(args.run),'--plan',str(args.plan),'--static',str(root/'analysis/unit-static-evidence.json'),'--data',str(root/'analysis/unit-static-data.json')],capture_output=True,text=True,check=True)
    source_audit=json.loads(audit.stdout)
    plan=json.loads(args.plan.read_bytes())
    e=json.loads(args.capture.read_bytes())
    groups=[]
    for m in e['normalized_result']['messages']:
        if m['type']!='send':continue
        x=m['payload']
        if x['operation']=='import-before':groups.append(dict(before=x,handlers=[]))
        elif x['operation']=='import-after':groups[-1]['after']=x
        elif x['operation']=='handler-after':groups[-1]['handlers'].append(x)
    commands=['initialized 3 20 0','link 2']+['import '+(args.run.parent/f).read_bytes().replace(b'\r\n',b'\n').hex()for f in plan['ordered_imports']]
    result=subprocess.run([str(args.driver.resolve())],input='\n'.join(commands)+'\n',text=True,capture_output=True,check=True)
    rows=[json.loads(line)for line in result.stdout.splitlines()]
    assert len(rows)==len(groups)==len(plan['ordered_imports'])
    checks=handlers=eof=mesh_snapshots=0

    def equal(a,b,label):
        nonlocal checks
        checks+=1
        if a!=b:
            def first(a,b,path=''):
                if isinstance(a,dict)and isinstance(b,dict)and a.keys()==b.keys():
                    for k in a:
                        if a[k]!=b[k]:return first(a[k],b[k],path+'/'+str(k))
                if isinstance(a,list)and isinstance(b,list)and len(a)==len(b):
                    for i,(x,y)in enumerate(zip(a,b)):
                        if x!=y:return first(x,y,path+'/'+str(i))
                return path,repr(a)[:160],repr(b)[:160]
            raise AssertionError((label,*first(a,b)))

    def state(primary,linked,observed,label):
        nonlocal mesh_snapshots
        for key in ['unit','global_units','dimension','components','object_count','material_count']:
            equal(primary[key],observed[key],label+'/'+key)
        equal(primary['mesh_number_0x840'],observed['mesh_number'],label+'/mesh number')
        equal(primary['simulation_mode']['mode_0x844'],observed['mode'],label+'/SMODE')
        equal(primary['simulation_type_0x848'],observed['type'],label+'/STYPE')
        equal(linked['unit'],observed['linked_unit'],label+'/linked units')
        equal(linked['dimension'],observed['linked_dimension'],label+'/linked dimension')
        equal(linked['global_units'],primary['global_units'],label+'/shared process tables')
        if 'mesh_samples'in observed:
            mesh_snapshots+=1
            actual=[]
            for obj in primary['objects']:
                temperatures=next(d['values']for d in obj['descriptors']['node']if d['id']==8)
                bits=lambda values:b''.join(struct.pack('<d',v)for v in values).hex()
                actual.append(dict(id=obj['id'],coordinates_hex=bits(obj['coordinates']),temperature_hex=bits(temperatures)))
            equal(actual,observed['mesh_samples'],label+'/mesh coordinates and temperatures')

    for filename,cpp,native in zip(plan['ordered_imports'],rows,groups):
        equal(cpp['result']['outcome'],'parsed',filename+'/supported')
        equal(cpp['result']['status'],native['after']['status'],filename+'/wrapper status')
        state(cpp['before_state'],cpp['before_linked_state'],native['before']['state'],filename+'/before')
        state(cpp['state'],cpp['linked_state'],native['after']['state'],filename+'/after')
        selected=[h for h in cpp['handler_states']if h['id']==30]
        equal(len(selected),len(native['handlers']),filename+'/handler count')
        for h,n in zip(selected,native['handlers']):
            handlers+=1
            equal(h['status'],n['status'],filename+'/handler status')
            state(h['state'],h['linked_state'],n['state'],filename+'/handler')
            if n['cursor']:equal(h['cursor'],n['cursor'],filename+'/cursor')
            else:eof+=1
    report=dict(status='PASS_NATIVE_UNIT_COMPARISON',imports=len(rows),handlers=handlers,checks=checks,mesh_snapshots=mesh_snapshots,eof_cursors_unobserved=eof,source_audit=source_audit,
                scope='Initialized console roots; exact four-table and scalar defaults, complete28-entry process tables, dimension resets and linked isolation. Populated property buffers and scientific evaluation remain outside native comparison.')
    if args.report:args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8',newline='\n')
    print(json.dumps(report))


if __name__=='__main__':main()
