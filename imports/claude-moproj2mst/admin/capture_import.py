"""Local, read-only acquisition of Claude converter sources and evidence.

Writes only beneath this import's root. Never runs upstream scripts or programs.
Each copied file is hashed during acquisition and checked by rereading its copy.
"""
from pathlib import Path
import concurrent.futures as futures
import datetime
import hashlib
import json
import os
import shutil
import subprocess
import tarfile
import time

ROOT = Path(__file__).resolve().parents[1]
PROJECTS = Path(r'C:\Users\alext\Projects')
UPSTREAM = PROJECTS / 'claude-deform-rea'
WORK = PROJECTS / 'deform-rea-work'
GIT = UPSTREAM / '.git'
BASE = 'bb869387a85a89193f3fdd78b69442a6ea567866'
SKIP_DIRS = {'.git', '.claude', '__pycache__', 'CMakeFiles', '.vs'}
COPY_PARTS = ['apps/moproj2mst', 'port', 'deformdb', 'harness', 'tests',
              'tools/moproj2mst', 'docs/re/moproj2mst', 'docs/port-conventions.md',
              'docs/plan-gui-mo-stage1-moproj2mst.md', 'docs/re/gui-mo-moproj-mst.md',
              'README.md', '.gitignore']

def git(*args):
    return subprocess.run(['git', '--no-optional-locks', '--git-dir='+str(GIT), *args],
                          check=True, capture_output=True, text=True).stdout

def digest(path):
    with path.open('rb') as f:
        return hashlib.file_digest(f, 'sha256').hexdigest()

def walk(path, source=False):
    if path.is_file():
        yield path
    elif path.is_dir():
        for parent, dirs, names in os.walk(path, followlinks=False):
            dirs[:] = sorted(d for d in dirs if d not in SKIP_DIRS and
                             not (source and d.lower().startswith(('build', 'bld_'))))
            for name in sorted(names):
                f = Path(parent) / name
                if f.is_symlink():
                    raise RuntimeError('Symlink requires explicit acquisition: '+str(f))
                yield f

def copy_one(pair):
    src, dst = pair
    dst.parent.mkdir(parents=True, exist_ok=True)
    before = src.stat()
    h = hashlib.sha256()
    with src.open('rb') as fi, dst.open('xb') as fo:
        while chunk := fi.read(4*1024*1024):
            h.update(chunk)
            fo.write(chunk)
    after = src.stat()
    sha = h.hexdigest()
    if digest(dst) != sha:
        raise RuntimeError('Destination checksum mismatch: '+str(dst))
    shutil.copystat(src, dst)
    return {'source': str(src), 'destination': dst.relative_to(ROOT).as_posix(),
            'bytes': dst.stat().st_size, 'sha256': sha,
            'source_size': before.st_size, 'source_mtime_ns': before.st_mtime_ns,
            'source_stable_during_copy': (before.st_size, before.st_mtime_ns) ==
                                       (after.st_size, after.st_mtime_ns)}

def main():
    admin = ROOT / 'admin'
    history = ROOT / 'history'
    history.mkdir(exist_ok=True)
    started = datetime.datetime.now(datetime.timezone.utc).isoformat()
    refs = [line.split(' ', 1) for line in git('for-each-ref',
            '--format=%(refname) %(objectname)', 'refs/heads', 'refs/remotes').splitlines()
            if any(s in line.split(' ')[0] for s in
                   ('m2m', 'mdt', 'rel-p18', 'mb-into-ops', 'widget-into-ops', 'tmp-s-l34'))]
    (admin/'refs.json').write_text(json.dumps(refs, indent=2))
    bundle = history/'converter.bundle'
    print('Creating local converter history bundle', len(refs), 'refs', flush=True)
    git('bundle', 'create', str(bundle), *(r[0] for r in refs))
    git('bundle', 'verify', str(bundle))
    archive = history/'baseline.tar'
    git('archive', '--format=tar', '--output='+str(archive), BASE)
    source = ROOT/'source'
    source.mkdir()
    with tarfile.open(archive) as tf:
        members = [m for m in tf.getmembers() if not m.name.startswith(
            ('apps/def_gui_mo/', 'docs/re/gmo/', '.claude/'))]
        tf.extractall(source, members=members, filter='data')
    baseline_files = list(walk(source))
    with (admin/'baseline-manifest.jsonl').open('w', encoding='utf-8') as out:
        for f in baseline_files:
            out.write(json.dumps({'path': f.relative_to(ROOT).as_posix(),
                                  'bytes': f.stat().st_size, 'sha256': digest(f)})+'\n')
    tasks = {}
    exclusions = []
    def add(src, dst, source_mode=False):
        if not src.exists():
            return
        for f in walk(src, source_mode):
            target = dst if src.is_file() else dst/f.relative_to(src)
            if target in tasks and tasks[target] != f:
                raise RuntimeError('Destination collision: '+str(target))
            tasks[target] = f

    worktrees = []
    for meta in sorted((GIT/'worktrees').iterdir()):
        head = (meta/'HEAD').read_text().strip()
        if 'm2m' not in head:
            continue
        wt = Path((meta/'gitdir').read_text().strip()).parent
        if not wt.exists():
            continue
        worktrees.append({'path':str(wt), 'head':head})
        for part in COPY_PARTS:
            add(wt/part, ROOT/'working-snapshots'/wt.name/part, True)
        # Historical result logs and shipped binaries, kept outside editable source.
        app = wt/'apps/moproj2mst'
        for pattern in ('build*/Testing/Temporary/*.log', 'build*/bin/moproj2mst.*',
                        'build*/bin/*.dll', '*.log', '*.txt'):
            for f in app.glob(pattern):
                if f.is_file():
                    add(f, ROOT/'historical-builds'/wt.name/f.relative_to(app))
    (admin/'worktrees.json').write_text(json.dumps(worktrees, indent=2))

    gui = WORK/'gui_mo'
    for p in sorted(gui.iterdir()):
        if p.name in {'s2', 'out_L01', 'run_L01.sh'} or p.name.startswith(('gproj_gmo_', 'run_gmo_')):
            exclusions.append({'path':str(p), 'reason':'Full GUI reconstruction; outside converter scope'})
            continue
        add(p, ROOT/'evidence/gui_mo'/p.name)
    for pattern in ('ghidra_mdt*', 'm2m_p*'):
        for p in WORK.glob(pattern):
            add(p, ROOT/'evidence/deform-rea-work'/p.name)
    add(WORK/'scripts', ROOT/'evidence/deform-rea-work/scripts')
    add(WORK/'NOTES.md', ROOT/'evidence/deform-rea-work/NOTES.md')
    for p in (WORK/'out').iterdir():
        if p.name.startswith(('mdt', 'last_run_ghidra_mdt')):
            add(p, ROOT/'evidence/deform-rea-work/out'/p.name)
    add(PROJECTS/'DEFORM_examples/big_projects', ROOT/'evidence/corpus-big')
    for name in ('COG_P1_B1', 'Inc718_D420_GFM_V_DIES_Angle2_18'):
        add(Path('C:/PROBLEM/TEST_REA')/name, ROOT/'evidence/test-rea'/name)

    temp = Path(r'C:\Users\alext\AppData\Local\Temp')
    for cache in temp.glob('rea-ghidra-*'):
        for p in cache.iterdir():
            if p.name == 'project' or p.name.startswith('target-') or p.name in {'ghidra.log','script.log'}:
                add(p, ROOT/'evidence/rea'/cache.name/p.name)
            else:
                exclusions.append({'path':str(p), 'reason':'Session/control/config/cache state; not reusable analysis evidence'})
    install = Path(r'C:\Program Files\SFTC\DEFORM\v11.0.2')
    for name in ('DEF_GUI_MO_64.exe','DEF_PRE_64.exe','QtCore4_64.dll','QtGui4_64.dll','QtXml4_64.dll'):
        add(install/name, ROOT/'evidence/original-binaries'/name)
    memory = Path(r'C:\Users\alext\.claude\projects\C--Users-alext-Projects-claude-deform-rea\memory')
    for p in memory.iterdir():
        if p.name.startswith(('m2m-', 'moproj2mst', 'oracle-b-', 'gui-mo-research', 'rea-setup', 'tool-choice-ghidra')):
            add(p, ROOT/'context/claude-memory'/p.name)
    for name in ('AGENTS.md', 'CLAUDE.md'):
        add(UPSTREAM/name, ROOT/'context/upstream-guidance'/name)
    (admin/'exclusions.json').write_text(json.dumps(exclusions, indent=2))
    pairs = [(src,dst) for dst,src in tasks.items()]
    print('Copying', len(pairs), 'files; baseline extracted:', len(baseline_files), flush=True)
    total = 0
    unstable = []
    errors = []
    tick = time.monotonic()
    with (admin/'copy-manifest.jsonl').open('w', encoding='utf-8') as out, futures.ThreadPoolExecutor(max_workers=4) as pool:
        submitted = {pool.submit(copy_one,pair):pair for pair in pairs}
        for n, future in enumerate(futures.as_completed(submitted),1):
            try:
                row = future.result()
                out.write(json.dumps(row)+'\n')
                total += row['bytes']
                if not row['source_stable_during_copy']:
                    unstable.append(row['source'])
            except Exception as e:
                errors.append({'source':str(submitted[future][0]),'error':str(e)})
            if time.monotonic()-tick > 15:
                print('Copied',n,'/',len(pairs),'files,',round(total/1e9,2),'GB; errors',len(errors),flush=True)
                tick = time.monotonic()
    receipt = {'started_utc':started,'finished_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),
        'baseline_commit':BASE,'baseline_files':len(baseline_files),'refs':len(refs),
        'worktrees':len(worktrees),'copied_files':len(pairs)-len(errors),'copied_bytes':total,
        'bundle_sha256':digest(bundle),'baseline_tar_sha256':digest(archive),
        'unstable_files':unstable,'errors':errors,
        'qualification':'Per-file SHA-256 verified; filesystem copies are not a transaction-consistent Ghidra/database backup. No upstream programs, builds, tests or analysis engines executed.'}
    (admin/'import-receipt.json').write_text(json.dumps(receipt,indent=2))
    print(json.dumps(receipt),flush=True)
    return bool(errors or unstable)

if __name__ == '__main__':
    raise SystemExit(main())
