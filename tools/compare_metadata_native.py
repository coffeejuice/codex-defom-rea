"""Compare passive original metadata observations with the frozen replacement."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--driver', type=Path, required=True)
    p.add_argument('--capture', type=Path, required=True)
    p.add_argument('--run', type=Path, required=True)
    p.add_argument('--report', type=Path)
    args = p.parse_args()
    capture = json.loads(args.capture.read_bytes())
    run = json.loads(args.run.read_bytes())
    assert run['status'] == 'completed' and run['exit_code'] == 0
    assert run['executable_sha256'] == 'f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
    normalized = capture['normalized_result']
    assert not normalized['messages_truncated'] and normalized['cleanup_error'] is None
    assert normalized['state'] == 'detached'
    for f in run['inputs']:
        assert f['sha256_before'] == f['sha256_after'] == f['source_sha256_after']
        assert hashlib.sha256(Path(f['copy']).read_bytes()).hexdigest() == f['sha256_before']
    observations = []
    for message in normalized['messages']:
        assert message['type'] != 'error', message
        if message['type'] != 'send':
            continue
        event = message['payload']
        assert event['operation'] != 'probe-error', event
        if event['operation'] == 'import-before':
            observations.append({'before': event, 'handlers': [], 'entries': [], 'helpers': []})
        elif event['operation'] == 'import-after':
            observations[-1]['after'] = event
        elif event['operation'] == 'handler-after':
            observations[-1]['handlers'].append(event)
        elif event['operation'] == 'handler-before':
            observations[-1]['entries'].append(event)
        elif event['operation'] == 'helper-after':
            observations[-1]['helpers'].append(event)
    assert len(observations) == len(run['ordered_imports'])
    commands = []
    for name in run['ordered_imports']:
        data = (args.run.parent / name).read_bytes().replace(b'\r\n', b'\n')
        commands.append(f'import primary {data.hex()}')
    proc = subprocess.run([str(args.driver.resolve())], input='\n'.join(commands) + '\n',
                          text=True, capture_output=True, check=True)
    results = [json.loads(line) for line in proc.stdout.splitlines()]
    assert len(results) == len(observations)
    checks = 0
    handlers = 0
    eof_cursor_unavailable = 0
    entry_cursor_unavailable = 0

    def equal(got, expected, where):
        nonlocal checks
        checks += 1
        if got != expected:
            raise AssertionError(f'{where}: {got!r} != {expected!r}')

    defaults = {'title': 'SIMULATION', 'operation_name': 'OPERATION 1',
                'simulation_name': 'SIMULATION 1', 'cursim': [1, 0, 0]}
    before = {'metadata': {'primary': defaults, 'linked': defaults},
              'dimensions': {'primary': 3, 'linked': 2}}
    for actual, native, name in zip(results, observations, run['ordered_imports']):
        equal(native['before']['file'], name, name + ' import order')
        for k in ['metadata', 'dimensions']:
            equal(before[k], native['before'][k], name + ' before ' + k)
            equal(actual[k], native['after'][k], name + ' after ' + k)
        equal(actual['outcome'], 0, name + ' supported')
        equal(actual['status'], native['after']['status'], name + ' wrapper status')
        equal(len(actual['handlers']), len(native['handlers']), name + ' handler count')
        equal(len(native['entries']), len(native['handlers']), name + ' native pairing')
        equal(len(native['helpers']), len(native['handlers']), name + ' helper pairing')
        for got, entry, after, helper in zip(actual['handlers'], native['entries'], native['handlers'], native['helpers']):
            handlers += 1
            equal({'TITLE': 22, 'CURSIM': 23, 'OPRNAM': 24, 'SIMNAM': 25}[got['token']], after['id'], name + ' ID')
            equal(got['status'], after['status'], name + ' handler status')
            # Legacy fgets may reset FILE._ptr to _base on an EOF attempt,
            # including a successful final unterminated line.
            # That loses the absolute logical cursor; retain the raw zero and
            # do not equate it with a replacement offset or invent an ftell.
            lost_eof_cursor = after['stream']['cursor'] == 0
            if lost_eof_cursor:
                eof_cursor_unavailable += 1
            else:
                equal(got['cursor'], after['stream']['cursor'], name + ' translated buffer cursor')
            if entry['stream']['cursor'] == 0:
                entry_cursor_unavailable += 1
            else:
                equal(got['offset'] + len(got['token']), entry['stream']['cursor'], name + ' entry cursor')
            equal(helper['status'], 0 if got['status'] == 0 else 1, name + ' helper status')
            equal(helper['stream']['cursor'], after['stream']['cursor'], name + ' helper cursor')
            for k in ['metadata', 'dimensions']:
                equal(got[k], after[k], name + ' handler ' + k)
                equal(got[k], helper[k], name + ' helper ' + k)
        before = actual
    report = {'status': 'PASS_NATIVE_METADATA_COMPARISON', 'imports': len(results), 'handlers': handlers,
              'checks': checks, 'eof_absolute_cursor_unavailable': eof_cursor_unavailable,
              'entry_absolute_cursor_unavailable': entry_cursor_unavailable,
              'evidence_id': capture['evidence_id'],
              'capture_sha256': hashlib.sha256(args.capture.read_bytes()).hexdigest(),
              'run_sha256': hashlib.sha256(args.run.read_bytes()).hexdigest(),
              'limitations': ['Console path seeds primary only; linked nondefault seeds remain replacement/static coverage.',
                             'Stream positions are bounded translated CRT buffer offsets, not physical ftell.',
                             'Absolute EOF cursor is unobserved when a scanner/fgets EOF attempt resets the native buffer pointer to its base, including a successful final partial line.',
                             'Initialized ASCII and representable integer fixture domain only; no binary ABI or full-app equivalence claim.']}
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
