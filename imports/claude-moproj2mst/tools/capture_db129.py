"""Retain a copied-fixture training run for the historical DBSTEP_CNV mismatch."""
import difflib
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]


def manifest(root):
    return {p.relative_to(root).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
            for p in sorted(root.rglob('*')) if p.is_file()}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--build-receipt', type=Path)
    parser.add_argument('--case', default='129_B_DBSTEP_CNV')
    parser.add_argument('--label', default='wave2-db129')
    args = parser.parse_args()
    report = ROOT / 'analysis' / (args.label + '-capture.json')
    if report.exists():
        raise RuntimeError('Receipt already exists; use a new --label: ' + str(report))
    if args.build_receipt:
        cfg = json.loads(args.build_receipt.read_text())
        binaries = [b for b in cfg['artifacts'] if Path(b['path']).name in
                    ('moproj2mst.exe', 'moproj2mst.dll', 'deformdb.dll')]
        assert len(binaries) == 3
        for b in binaries:
            assert hashlib.sha256(Path(b['path']).read_bytes()).hexdigest() == b['sha256']
        binary = next(b for b in binaries if Path(b['path']).name == 'moproj2mst.exe')
    else:
        case = ROOT / 'build/reference-harness/billet-training-004'
        cfg = json.loads((case / 'case.json').read_text())
        binary = cfg['candidate_binaries'][0]
    exe = Path(binary['path'])
    assert hashlib.sha256(exe.read_bytes()).hexdigest() == binary['sha256']
    oracle = ROOT / 'evidence/gui_mo/oracle_b'
    fixture = oracle / (args.case + '_before')
    expected = oracle / args.case
    before = manifest(fixture)
    input_dirs = sorted(p.relative_to(fixture).as_posix() for p in fixture.rglob('*') if p.is_dir())
    run = ROOT / 'build' / args.label / str(time.time_ns())
    project = run / 'project'
    shutil.copytree(fixture, project)
    assert manifest(project) == before
    projects = list(project.glob('*.moproj'))
    assert len(projects) == 1
    command = [str(exe), str(projects[0]), '--write-save']
    started = time.monotonic()
    with (run / 'candidate.log').open('w') as log:
        process = subprocess.run(command, cwd=run, stdout=log, stderr=subprocess.STDOUT, timeout=120)
    elapsed = time.monotonic() - started
    actual, wanted = manifest(project), manifest(expected)
    changes = []
    for name in sorted(actual.keys() | wanted.keys()):
        if actual.get(name) == wanted.get(name):
            continue
        row = dict(path=name, candidate=actual.get(name), historical=wanted.get(name))
        if name in actual and name in wanted:
            a, b = (project / name).read_bytes(), (expected / name).read_bytes()
            row['first_difference'] = next((i for i, (x, y) in enumerate(zip(a, b)) if x != y), min(len(a), len(b)))
            if b'\0' not in a + b and len(a) + len(b) < 2_000_000:
                row['diff'] = list(difflib.unified_diff(a.decode('latin1').splitlines(), b.decode('latin1').splitlines(), fromfile='candidate', tofile='historical', n=2))
        changes.append(row)
    result = dict(run=str(run), binary=binary, command=command, exit_code=process.returncode,
                  elapsed_seconds=elapsed, fixture_unchanged=manifest(fixture) == before,
                  input_directories=input_dirs,
                  input_directories_unchanged=input_dirs == sorted(p.relative_to(fixture).as_posix() for p in fixture.rglob('*') if p.is_dir()),
                  candidate_directories=sorted(p.relative_to(project).as_posix() for p in project.rglob('*') if p.is_dir()),
                  historical_directories=sorted(p.relative_to(expected).as_posix() for p in expected.rglob('*') if p.is_dir()),
                  original_executed=False, qualification='Historical training comparison; no normalization and no fresh-reference acceptance.',
                  input_manifest=before, candidate_manifest=actual, historical_manifest=wanted, differences=changes)
    (run / 'receipt.json').write_text(json.dumps(result, indent=2) + '\n')
    report.write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps({k: result[k] for k in ('run', 'exit_code', 'elapsed_seconds', 'fixture_unchanged')}))
    print(json.dumps({'raw_differences': [{'path': x['path'], 'diff_lines': len(x.get('diff', []))} for x in changes]}))


if __name__ == '__main__':
    main()
