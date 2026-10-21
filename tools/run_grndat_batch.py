"""Launch one prepared bounded console batch; the REA observer releases its gate."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--id', required=True)
    args = p.parse_args()
    root = Path(__file__).resolve().parents[1]
    batches = json.loads((root / 'analysis/grndat-native-batch-plan.json').read_text())['batches']
    b = next(x for x in batches if x['id'] == args.id)
    for f in b['inputs']:
        assert hashlib.sha256((Path(b['fixtures']) / f['file']).read_bytes()).hexdigest() == f['sha256']
    oracle = root / 'analysis/oracle'
    command = [sys.executable, str(root / 'tools/capture_keywords.py'), '--exe', str(root / 'source/DEF_PRE_64.exe'),
               '--fixtures', b['fixtures'], '--keys', *b['ordered_imports'], '--output', str(oracle / b['id']),
               '--ready-gate', str(oracle / (b['id'] + '.ready')), '--pid-file', str(oracle / (b['id'] + '.pid.json')),
               '--gate-timeout', '120', '--timeout', '60']
    log = oracle / (b['id'] + '.launch.log')
    with log.open('xb') as output:
        code = subprocess.call(command, stdout=output, stderr=subprocess.STDOUT)
    record_path = oracle / b['id'] / 'run.json'
    record = json.loads(record_path.read_text()) if record_path.exists() else {}
    print(json.dumps({'batch': b['id'], 'harness_exit': code, 'status': record.get('status'),
                      'target_exit': record.get('exit_code'), 'run': str(record_path)}))
    return code


if __name__ == '__main__': raise SystemExit(main())
