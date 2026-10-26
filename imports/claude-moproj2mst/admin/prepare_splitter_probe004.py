"""Verify reviewed passive capture inputs and retain disposable-folder baseline."""
import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

root = Path(__file__).resolve().parents[1]
folder = root/'build/original-splitter-probe-004'
script = folder/'probe.js'
assert hashlib.sha256(script.read_bytes()).hexdigest() == '582b6291dc9abc631048732631bac058f8697d69049bb28a2a4c496e73f394b1'
receipt = json.loads((root/'analysis/wave3-close-widget-constructor-tail-probe004-freeze.json').read_text())
installed = Path(r'C:\Program Files\SFTC\DEFORM\v11.0.2')
modules = []
for item in receipt['checked_module_files']:
    path = installed/item['name']
    digest = hashlib.sha256(path.read_bytes()).hexdigest()
    assert digest == item['sha256'], path
    modules.append(dict(path=str(path),sha256=digest))
scratch = Path(r'C:\PROBLEM\m2m42')
assert scratch.is_dir() and str(scratch.resolve()).lower() == str(scratch).lower()
files = {}
for path in sorted(scratch.rglob('*')):
    assert not path.is_symlink() and not path.is_junction(), path
    if path.is_file():
        assert len(str(path)) < 100
        files[path.relative_to(scratch).as_posix()] = hashlib.sha256(path.read_bytes()).hexdigest()
assert len(files) == 8
record = dict(utc=datetime.now(timezone.utc).isoformat(),script_sha256=hashlib.sha256(script.read_bytes()).hexdigest(),
    modules=modules,cwd=str(scratch),cwd_files=files,argv=[str(installed/'DEF_GUI_MO_64.exe')],
    review='Root read complete script and source/spec: process-local Set/GetCWD only; passive reads/hooks; selected constructor tail; d35a80 pause; no Open argv, Qt getters, input, event pump or target-memory stores.',
    cleanup='Persist exact spawned PID/session before resume. Retain full inline evidence, terminate only that exact owned process, verify PID absence, then close Frida.',
    gui_control_acquired=False)
target = folder/'root-preflight.json'
assert not target.exists()
target.write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'modules_verified':len(modules),'cwd_files':len(files),'gui_control_acquired':False}))
