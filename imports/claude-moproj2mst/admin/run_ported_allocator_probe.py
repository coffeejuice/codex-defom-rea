"""Compile and run an isolated allocator experiment without changing product sources."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time
from build_candidate import ROOT, VCVARS

parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--variant',choices=['legacy','faithful'],default='legacy')
args=parser.parse_args()
folder=ROOT/('build/ported-allocator-probe-001' if args.variant=='legacy' else 'build/faithful-allocator-probe-001')
out=ROOT/('analysis/wave3-ported-allocator-probe-receipt.json' if args.variant=='legacy' else 'analysis/wave3-faithful-allocator-probe-receipt.json')
assert not out.exists(), 'Preserve the existing experiment receipt; choose a new experiment for a rerun.'
helper='realloc_zero_tail' if args.variant=='legacy' else 'realloc_uninitialized'
if args.variant=='faithful':
    folder.mkdir(exist_ok=False)
    source=(ROOT/'build/ported-allocator-probe-001/probe.cpp').read_text()
    assert source.count('realloc_zero_tail')==4
    (folder/'probe.cpp').write_text(source.replace('realloc_zero_tail',helper),encoding='utf-8',newline='\n')
environment=subprocess.run(f'cmd.exe /d /s /c ""{VCVARS}" >nul && set"',capture_output=True,text=True,check=True)
env=dict(os.environ)
for line in environment.stdout.splitlines():
    key,sep,value=line.partition('=')
    if key and sep:
        for existing in list(env):
            if existing.lower()==key.lower(): del env[existing]
        env[key]=value
compiler=Path(env['VCToolsInstallDir'])/'bin/Hostx64/x64/cl.exe'
assert compiler.is_file()
command=[str(compiler),'/nologo','/std:c++20','/EHsc','/O2','/MD',str(folder/'probe.cpp'),
         '/I'+str(ROOT/'source/port/defmodel/include'),'/Fe:'+str(folder/'probe.exe'),
         '/Fo:'+str(folder/'probe.obj')]
start=time.monotonic()
build=subprocess.run(command,cwd=folder,env=env,capture_output=True,text=True)
(folder/'build.log').write_text(build.stdout+build.stderr,encoding='utf-8')
assert build.returncode==0, build.stdout+build.stderr
run=subprocess.run([str(folder/'probe.exe')],cwd=folder,capture_output=True,text=True)
(folder/'output.json').write_text(run.stdout,encoding='utf-8')
assert run.returncode==0,run.stderr
records=json.loads(run.stdout)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
receipt=dict(command=command,exit_code=run.returncode,elapsed_seconds=time.monotonic()-start,
             records=records,source_sha256=sha(folder/'probe.cpp'),exe_sha256=sha(folder/'probe.exe'),
             variant=args.variant,helper_sha256=sha(ROOT/('source/port/defmodel/include/defmodel/llobj/'+helper+'.hpp')),
             qualification='Controlled port allocator experiment, no product rebuild or original conversion. Raw bytes read using ReadProcessMemory. CRT/heap histories differ; compare initialization policy, not addresses or exact uninitialized bytes.')
out.write_text(json.dumps(receipt,indent=2)+'\n',encoding='utf-8')
print(json.dumps(dict(receipt=str(out),prefixes_preserved=sum(x['prefixPreserved'] for x in records),
                     zero_grown_tails=sum(x['grownTailZero'] for x in records),old_sizes=[x['oldAllocatedBytes'] for x in records])))
