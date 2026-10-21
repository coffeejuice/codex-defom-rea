"""Compare original STYPE live integer reads and root state with the replacement."""
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
    p.add_argument('--plan', type=Path, default=Path('analysis/stype-fixture-plan.json'))
    p.add_argument('--report', type=Path)
    args = p.parse_args()
    plan = json.loads(args.plan.read_bytes())
    run = json.loads(args.run.read_bytes())
    e = json.loads(args.capture.read_bytes())
    n = e['normalized_result']
    assert run['status'] == 'completed' and run['exit_code'] == 0
    assert run['executable_sha256'] == 'f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
    assert n['pid'] == run['pid'] and n['cleanup_error'] is None and n['state'] == 'detached'
    assert not n['messages_truncated']
    assert hashlib.sha256((args.run.parent / 'observer-source.js').read_bytes()).hexdigest() == n['source_sha256']
    assert run['ordered_imports'] == plan['ordered_imports']
    for f in run['inputs']:
        binding = next(x for x in plan['inputs'] if x['file'] == f['name'])
        assert f['sha256_before'] == f['sha256_after'] == f['source_sha256_after'] == binding['sha256']
        assert hashlib.sha256(Path(f['copy']).read_bytes()).hexdigest() == binding['sha256']
    observations = []
    for m in n['messages']:
        assert m['type'] != 'error', m
        if m['type'] != 'send': continue
        x = m['payload']
        assert x['operation'] != 'probe-error', x
        if x['operation'] == 'import-before': observations.append({'before': x, 'entries': [], 'handlers': []})
        elif x['operation'] == 'import-after': observations[-1]['after'] = x
        elif x['operation'] == 'handler-before': observations[-1]['entries'].append(x)
        elif x['operation'] == 'handler-after': observations[-1]['handlers'].append(x)
    commands = ['initialized 3 20 0', 'link 2'] + ['import ' + (args.run.parent / f).read_bytes().replace(b'\r\n', b'\n').hex() for f in plan['ordered_imports']]
    proc = subprocess.run([str(args.driver.resolve())], input='\n'.join(commands) + '\n', text=True, capture_output=True, check=True)
    results = [json.loads(line) for line in proc.stdout.splitlines()]
    assert len(results) == len(observations) == len(plan['ordered_imports'])
    checks = handlers = eof = 0

    def equal(got, expected, where):
        nonlocal checks
        checks += 1
        assert got == expected, (where, got, expected)

    def state(primary, linked, native, where):
        for key in ['simulation_type_0x848', 'simulation_mode', 'dimension', 'components', 'object_count', 'material_count']:
            equal(primary[key], native[key], where + '/' + key)
        equal(linked['simulation_type_0x848'], native['linked_type'], where + '/linked type')
        equal(linked['simulation_mode'], native['linked_mode'], where + '/linked mode')
        equal(linked['dimension'], native['linked_dimension'], where + '/linked dimension')

    equal(observations[0]['before']['state']['simulation_type_0x848'], 1, 'original startup default')
    for file, cpp, native in zip(plan['ordered_imports'], results, observations):
        equal(native['before']['file'], file, file + ' native input identity')
        equal(native['after']['file'], file, file + ' native output identity')
        equal(cpp['result']['outcome'], 'parsed', file + ' supported')
        equal(cpp['result']['status'], native['after']['status'], file + ' wrapper status')
        state(cpp['before_state'], cpp['before_linked_state'], native['before']['state'], file + ' before')
        state(cpp['state'], cpp['linked_state'], native['after']['state'], file + ' after')
        if file in ('reset2.KEY', 'reset3.KEY', 'change.KEY', 'back.KEY'):
            equal(cpp['state']['simulation_type_0x848'], 1, file + ' source-derived changed-dimension default')
        selected = [h for h in cpp['handler_states'] if h['id'] == 26]
        expected = next(f['expected_values_and_handler_statuses'] for f in plan['inputs'] if f['file'] == file)
        equal(len(selected), len(expected), file + ' expected handlers')
        equal(len(native['handlers']), len(expected), file + ' native handlers')
        equal(len(native['entries']), len(expected), file + ' entry pairing')
        for actual, observed, entry, (value, status) in zip(selected, native['handlers'], native['entries'], expected):
            handlers += 1
            equal(entry['file'], file, file + ' native handler entry identity')
            equal(observed['file'], file, file + ' native handler exit identity')
            equal(actual['status'], status, file + ' source-derived status')
            equal(actual['status'], observed['status'], file + ' native status')
            state(actual['state'], actual['linked_state'], observed['state'], file + ' handler')
            equal(observed['refreshes'], [], file + ' no native refresh')
            equal(len(observed['calls']), 1, file + ' one integer reader')
            call = observed['calls'][0]
            equal(call['after'], value, file + ' source-derived raw value, including failures')
            equal(call['before'], entry['state']['simulation_type_0x848'], file + ' reader prestate')
            equal(call['after'], observed['state']['simulation_type_0x848'], file + ' live reader poststate')
            equal(call['status'], 0 if status == 0 else 1, file + ' reader status')
            equal(observed['state']['simulation_mode'], entry['state']['simulation_mode'], file + ' mode fields unchanged')
            equal(observed['state']['components'], entry['state']['components'], file + ' component widths unchanged')
            if observed['cursor']: equal(actual['cursor'], observed['cursor'], file + ' cursor')
            else: eof += 1
    report = {'status': 'PASS_NATIVE_STYPE_COMPARISON', 'imports': len(results), 'handlers': handlers,
              'checks': checks, 'eof_cursors_unobserved': eof, 'evidence_id': e['evidence_id'],
              'capture_sha256': hashlib.sha256(args.capture.read_bytes()).hexdigest(),
              'run_sha256': hashlib.sha256(args.run.read_bytes()).hexdigest(),
              'scope': 'Representable integer inputs, initialized roots, ordinary wrapper. Raw type, mode, flags, component table and counts; no scientific enum inference. Native reader live destination and no-refresh checks are original observations; linked isolation compares.'}
    if args.report: args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__': main()
