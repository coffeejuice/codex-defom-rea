"""Final read-only exact-thread qualification; freeze probe without launching."""
import hashlib
import json
from pathlib import Path
root=Path(__file__).resolve().parents[1]
source=root/'analysis/wave3-editor-base-grid-probe-v2.js'
assert hashlib.sha256(source.read_bytes()).hexdigest()=='e8a896a7753d2660951f36319eeac9f29d4ffada29e37bd085754537fe1f93db'
s=source.read_text()
a='this.f={object:cp(args[0]),private:cp(args[1]),parent:cp(args[2])};qobjectFrame=this.f;'
assert s.count(a)==1
s=s.replace(a,'this.f={object:cp(args[0]),private:cp(args[1]),parent:cp(args[2]),tid:Process.getCurrentThreadId()};qobjectFrame=this.f;')
a='if(!qobjectFrame)return;'
assert s.count(a)==2
s=s.replace(a,'if(!qobjectFrame||qobjectFrame.tid!==Process.getCurrentThreadId())return;')
target=root/'analysis/wave3-editor-base-grid-probe-v3.js'
assert not target.exists()
target.write_text(s,encoding='utf-8')
record=dict(status='Final prepared source; root must verify inputs, review and record before launch',
 script=str(target.relative_to(root)),sha256=hashlib.sha256(target.read_bytes()).hexdigest(),
 predecessor=str(source.relative_to(root)),predecessor_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
 changes=['Selected QObject instruction-site observations also require actual selected OS thread; concurrent unrelated constructors cannot contaminate the retained frame.'],
 review=['Complete generated v1 source derived from fully reviewed004 helpers and source-qualified002 selector.',
  'v2 preserves source fixes for dispatcher thunk, map refcount, grid layout register and owner-filtered wrapper callbacks.',
  '45 immutable module-site checks; expected endpoint140ed9a11 after UI publication before spacing.',
  'No original process, Qt API, input or filesystem action executed by preparation.'])
(root/'analysis/wave3-editor-base-grid-probe-v3-freeze.json').write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'script':record['script'],'sha256':record['sha256'],'launched':False}))
