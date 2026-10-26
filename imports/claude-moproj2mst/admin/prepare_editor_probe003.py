"""Verify/copy reviewed capture script and retain unchanged owned input manifest."""
import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path
root=Path(__file__).resolve().parents[1]
source=root/'analysis/wave3-editor-base-grid-probe-v3.js'
digest=hashlib.sha256(source.read_bytes()).hexdigest()
assert digest=='e860e7ca70b0ea24a5846b6d2f8a04f0e47cd55a8b83d37bdb7ea9dd4bc9ee41'
pre=json.loads((root/'build/original-splitter-probe-004/root-preflight.json').read_text())
for item in pre['modules']:
    assert hashlib.sha256(Path(item['path']).read_bytes()).hexdigest()==item['sha256']
scratch=Path(pre['cwd'])
assert str(scratch.resolve()).lower()==r'c:\problem\m2m42'
actual={}
for path in sorted(scratch.rglob('*')):
    assert not path.is_symlink() and not path.is_junction()
    if path.is_file(): actual[path.relative_to(scratch).as_posix()]=hashlib.sha256(path.read_bytes()).hexdigest()
assert actual==pre['cwd_files']
fixture=json.loads((root/'analysis/wave3-editor-owned-open-fixture-manifest.json').read_text())
for item in fixture['files']:
    assert hashlib.sha256((Path(fixture['source_directory'])/item['relative_path']).read_bytes()).hexdigest()==item['sha256']
folder=root/'build/original-editor-probe-003'
assert not folder.exists()
folder.mkdir()
(folder/'probe.js').write_bytes(source.read_bytes())
path=str(scratch/'DB_deconstruction_02.moproj')
encoded=''.join(format(c,'04x') for c in path.encode('utf-16-le')[::2])
# Fixture path is ASCII; verify the encoded sequence through the native protocol.
assert ''.join(chr(int(encoded[i:i+4],16)) for i in range(0,len(encoded),4))==path
pre.update(utc=datetime.now(timezone.utc).isoformat(),script_sha256=digest,
    source_script=str(source.relative_to(root)),
    argv=[pre['argv'][0],'-m','open','-f',encoded],
    review='Root-reviewed45 checked sites; exact native CLI Open to first editor grid UI publication before spacing; passive reads/hooks, owned-process cwd only. No GUI input, Qt calls or event pump.',
    endpoint='140ed9a11',source_fixture_unchanged=True)
(folder/'root-preflight.json').write_text(json.dumps(pre,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'folder':str(folder),'sha256':digest,'cwd_files':len(actual),'endpoint':pre['endpoint']}))
