"""Compare original SMODE setter calls and root state with the replacement."""
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
    p.add_argument('--plan', type=Path, default=Path('analysis/smode-fixture-plan.json'))
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
        for key in ['simulation_mode', 'dimension', 'components', 'object_count', 'material_count']:
            equal(primary[key], native[key], where + '/' + key)
        equal(linked['simulation_mode'], native['linked_mode'], where + '/linked mode')
        equal(linked['dimension'], native['linked_dimension'], where + '/linked dimension')

    for file, cpp, native in zip(plan['ordered_imports'], results, observations):
        equal(cpp['result']['outcome'], 'parsed', file + ' supported')
        equal(cpp['result']['status'], native['after']['status'], file + ' wrapper status')
        state(cpp['before_state'], cpp['before_linked_state'], native['before']['state'], file + ' before')
        state(cpp['state'], cpp['linked_state'], native['after']['state'], file + ' after')
        selected = [h for h in cpp['handler_states'] if h['id'] == 27]
        expected = next(f['expected_setter_values_and_handler_statuses'] for f in plan['inputs'] if f['file'] == file)
        equal(len(selected), len(expected), file + ' expected handlers')
        equal(len(native['handlers']), len(expected), file + ' native handlers')
        equal(len(native['entries']), len(expected), file + ' entry pairing')
        for actual, observed, entry, (value, status) in zip(selected, native['handlers'], native['entries'], expected):
            handlers += 1
            equal(actual['status'], status, file + ' source-derived status')
            equal(actual['status'], observed['status'], file + ' native status')
            state(actual['state'], actual['linked_state'], observed['state'], file + ' handler')
            equal(observed['refreshes'], [], file + ' no native refresh')
            equal(len(observed['calls']), 1, file + ' unconditional setter')
            call = observed['calls'][0]
            equal(call['value'], value, file + ' setter input')
            equal(call['before'], entry['state']['simulation_mode'], file + ' setter prestate')
            equal(call['after'], observed['state']['simulation_mode'], file + ' setter poststate')
            expected_mode = dict(call['before'], mode_0x844=value)
            if value in (1, 2, 3): expected_mode.update(plan['semantic_table'][str(value)])
            equal(call['after'], expected_mode, file + ' independent store truth table')
            if observed['cursor']: equal(actual['cursor'], observed['cursor'], file + ' cursor')
            else: eof += 1
    report = {'status': 'PASS_NATIVE_SMODE_COMPARISON', 'imports': len(results), 'handlers': handlers,
              'checks': checks, 'eof_cursors_unobserved': eof, 'evidence_id': e['evidence_id'],
              'capture_sha256': hashlib.sha256(args.capture.read_bytes()).hexdigest(),
              'run_sha256': hashlib.sha256(args.run.read_bytes()).hexdigest(),
              'scope': 'Representable integer inputs, initialized roots, ordinary wrapper. Raw mode and flags; no scientific enum inference. Native setter and no-refresh checks are original observations; C++ state/flags/component tables and linked isolation compare.'}
    if args.report: args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__': main()
