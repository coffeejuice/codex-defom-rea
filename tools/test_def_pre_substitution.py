"""Bounded, on-copies compatibility trial. No installation mutations here."""
import argparse
import hashlib
import json
import os
import shutil
import subprocess
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
INSTALL = Path(r'C:\Program Files\SFTC\DEFORM\v11.0.2')
PATHS = ('DEF_PRE_64.exe', '2D/DEF_PRE_64.exe', '3D/DEF_PRE_64.exe')
MODES = ('header', 'stepList', 'lastStepNo', 'simStepList')
DBS = (
    Path(r'C:\PROBLEM\TEST_REA\COG_P1_B1\COG_P1_B1.DB'),
    Path(r'C:\PROBLEM\TEST_REA\Inc718_D420_GFM_V_DIES_Angle2_18\Inc718_D420_GFM_V_DIES_Angle2_18.DB'),
)

def digest(p):
    with open(p, 'rb') as f:
        return hashlib.file_digest(f, 'sha256').hexdigest()

def run(exe, args, cwd, timeout=180):
    cwd.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ, DEF_PRE_PORT_INSTALL_DIR=str(INSTALL))
    start = time.monotonic()
    result = subprocess.run([str(exe), *args], cwd=cwd, env=env, input=b'',
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=timeout)
    (cwd/'stdout.bin').write_bytes(result.stdout)
    (cwd/'stderr.bin').write_bytes(result.stderr)
    output = cwd/'OUT.txt'
    return dict(exit_code=result.returncode, seconds=round(time.monotonic()-start, 3),
                output_sha256=digest(output) if output.exists() else None,
                output_bytes=output.stat().st_size if output.exists() else None,
                stdout_sha256=hashlib.sha256(result.stdout).hexdigest(),
                stderr_sha256=hashlib.sha256(result.stderr).hexdigest())

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('phase', choices=('baseline', 'installed'))
    ap.add_argument('--trial', type=Path, required=True)
    a = ap.parse_args()
    trial = a.trial.resolve()
    trial.relative_to(ROOT/'build'/'substitution-trials')
    report = trial/(a.phase+'.json')
    if report.exists():
        raise RuntimeError('Existing report: use a new trial')
    records = []
    try:
        if a.phase == 'baseline':
            for i, source in enumerate(DBS):
                target = trial/'inputs'/str(i)/source.name
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(source, target)
                assert digest(source) == digest(target)
                for mode in MODES:
                    cwd = trial/'baseline'/str(i)/mode
                    records.append(dict(db=i, mode=mode, **run(INSTALL/PATHS[0],
                        ['-m', mode, '-d', str(target), '-o', 'OUT.txt'], cwd)))
                    assert records[-1]['exit_code'] == 0 and records[-1]['output_bytes'], records[-1]
        else:
            baseline = json.loads((trial/'baseline.json').read_text())
            assert baseline['status'] == 'PASS'
            candidate_hash = digest(trial/'candidate'/'DEF_PRE_64.exe')
            for index, rel in enumerate(PATHS):
                exe = INSTALL/rel
                assert digest(exe) == candidate_hash
                version = run(exe, ['--port-version'], trial/'installed'/str(index)/'version')
                assert version['exit_code'] == 0
                for ref in baseline['cases']:
                    target = trial/'inputs'/str(ref['db'])/DBS[ref['db']].name
                    cwd = trial/'installed'/str(index)/str(ref['db'])/ref['mode']
                    observed = run(exe, ['-m', ref['mode'], '-d', str(target), '-o', 'OUT.txt'], cwd)
                    equal = all(observed[k] == ref[k] for k in ('exit_code','output_sha256','output_bytes'))
                    records.append(dict(path=rel, db=ref['db'], mode=ref['mode'], matches=equal, **observed))
                    assert equal, records[-1]
        payload = dict(status='PASS', cases=records,
                       qualification='DB utility subset only; GUI, edits, export/reopen, errors and helper workflows remain untested.')
    except Exception as e:
        payload = dict(status='FAIL', error=str(e), cases=records)
    report.write_text(json.dumps(payload, indent=2)+'\n')
    print(json.dumps(dict(status=payload['status'], cases=len(records), report=str(report))))
    return 0 if payload['status'] == 'PASS' else 1

if __name__ == '__main__':
    raise SystemExit(main())
