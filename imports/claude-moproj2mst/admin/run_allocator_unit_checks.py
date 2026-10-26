"""Run existing low-level allocator callers against the converter's built libraries."""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time
from build_candidate import ROOT, VCVARS

folder=ROOT/'build/allocator-unit-checks-001'
folder.mkdir(exist_ok=False)
env=dict(os.environ)
setup=subprocess.run(f'cmd.exe /d /s /c ""{VCVARS}" >nul && set"',capture_output=True,text=True,check=True)
for line in setup.stdout.splitlines():
    key,sep,value=line.partition('=')
    if key and sep:
        for existing in list(env):
            if existing.lower()==key.lower(): del env[existing]
        env[key]=value
for key in ['TEMP','TMP','TMPDIR']: env[key]=str(folder)
compiler=Path(env['VCToolsInstallDir'])/'bin/Hostx64/x64/cl.exe'
includes=[ROOT/'source/port/defmodel/include',ROOT/'source/port/defrt/include',ROOT/'source/port/testkit/include']
libraries=[ROOT/'build/candidate'/p for p in ['defmodel/defmodel.lib','defrt/defrt.lib','deformdb/deformdb_core.lib']]
assert all(p.is_file() for p in libraries)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
receipt={'scope':'Three existing low-level tests, linked to candidate41 libraries; no product source change',
         'libraries':{str(p.relative_to(ROOT)):sha(p) for p in libraries},'tests':[]}
for name in ['llobj_varsets','node_access','gui_twins_view']:
    source=ROOT/('source/port/defmodel/tests/test_'+name+'.cpp')
    exe=folder/(name+'.exe')
    command=[str(compiler),'/nologo','/std:c++20','/EHsc','/O2','/MT',str(source),
             *['/I'+str(p) for p in includes],'/Fe:'+str(exe),'/Fo:'+str(folder/(name+'.obj')),
             *map(str,libraries)]
    start=time.monotonic()
    build=subprocess.run(command,cwd=folder,env=env,capture_output=True,text=True)
    (folder/(name+'-build.log')).write_text(build.stdout+build.stderr,encoding='utf-8')
    row={'name':name,'source_sha256':sha(source),'compile_exit':build.returncode,'command':command}
    if not build.returncode:
        run=subprocess.run([str(exe)],cwd=folder,env=env,capture_output=True,text=True)
        (folder/(name+'-test.log')).write_text(run.stdout+run.stderr,encoding='utf-8')
        row.update(test_exit=run.returncode,exe_sha256=sha(exe),summary=[l for l in run.stdout.splitlines() if 'cases failed' in l])
    row['elapsed_seconds']=time.monotonic()-start
    receipt['tests'].append(row)
    print(json.dumps({k:v for k,v in row.items() if k not in ['command','source_sha256','exe_sha256']}),flush=True)
receipt['libraries_stable']=all(sha(ROOT/p)==digest for p,digest in receipt['libraries'].items())
out=ROOT/'analysis/wave3-allocator-low-level-tests.json'
assert not out.exists()
out.write_text(json.dumps(receipt,indent=2)+'\n',encoding='utf-8')
if not all(t.get('test_exit')==0 and t['compile_exit']==0 for t in receipt['tests']): raise SystemExit(1)
