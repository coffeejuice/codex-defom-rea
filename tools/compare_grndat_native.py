"""Compare complete known GRNDAT table/list state at original handler returns."""
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
    p.add_argument('--batch-id', help='bind action boundaries and refresh expectations to the prepared batch')
    args = p.parse_args()
    captured = json.loads(args.capture.read_bytes())
    run = json.loads(args.run.read_bytes())
    assert run['status'] == 'completed' and run['exit_code'] == 0
    assert run['executable_sha256'] == 'f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
    n = captured['normalized_result']
    assert n['pid'] == run['pid'], 'capture/run process identity mismatch'
    retained_probe = args.run.parent / 'observer-source.js'
    assert hashlib.sha256(retained_probe.read_bytes()).hexdigest() == n['source_sha256'], 'probe identity mismatch'
    assert n['state'] == 'detached' and n['cleanup_error'] is None and not n['messages_truncated']
    for item in run['inputs']:
        assert item['sha256_before'] == item['sha256_after'] == item['source_sha256_after']
        assert hashlib.sha256(Path(item['copy']).read_bytes()).hexdigest() == item['sha256_before']
    imports = []
    for m in n['messages']:
        assert m['type'] != 'error', m
        if m['type'] != 'send': continue
        e = m['payload']
        assert e['operation'] != 'probe-error', e
        if e['operation'] == 'import-before': imports.append({'before': e, 'handlers': []})
        elif e['operation'] == 'import-after': imports[-1]['after'] = e
        elif e['operation'] == 'handler-after': imports[-1]['handlers'].append(e)
    assert len(imports) == len(run['ordered_imports'])
    commands = ['initialized 3 20 0'] + ['import ' + (args.run.parent / name).read_bytes().replace(b'\r\n', b'\n').hex()
                                       for name in run['ordered_imports']]
    proc = subprocess.run([str(args.driver.resolve())], input='\n'.join(commands) + '\n',
                          text=True, capture_output=True, check=True)
    replacements = [json.loads(line) for line in proc.stdout.splitlines()]
    assert len(replacements) == len(imports)
    checks = 0

    def equal(got, expected, where):
        nonlocal checks
        checks += 1
        if got != expected: raise AssertionError((where, got, expected))

    def project(got, expected, where):
        if isinstance(expected, dict):
            for k, v in expected.items(): project(got[k], v, where + '/' + k)
        elif isinstance(expected, list):
            equal(len(got), len(expected), where + ' length')
            for i, v in enumerate(expected): project(got[i], v, where + '/' + str(i))
        else: equal(got, expected, where)

    def state(actual, native, where):
        equal(actual['dimension'], native['dimension'], where + ' dimension')
        equal(len(actual['materials']), len(native['materials']), where + ' materials')
        for cpp, original in zip(actual['materials'], native['materials']):
            equal(cpp['id'], original['id'], where + ' ID')
            equal(cpp['field_0x17e8'], original['selector'], where + ' selector')
            equal(cpp['grndat']['secondary_0x17ec'], original['grndat']['secondary_0x17ec'], where + ' negative')
            project(cpp['grndat']['property_tables']['0x1740'], original['grndat']['table_0x1740'], where + ' selector4')
            project(cpp['grndat_extended'], original['grndat_extended'], where + ' complete')
    handlers = 0
    eof_cursors = 0
    for name, native, cpp in zip(run['ordered_imports'], imports, replacements):
        equal(cpp['result']['outcome'], 'parsed', name + ' supported')
        equal(native['before']['file'], name, name + ' import order')
        state(cpp['before_state'], native['before']['state'], name + ' before')
        state(cpp['state'], native['after']['state'], name + ' after')
        equal(cpp['result']['status'], native['after']['status'], name + ' parser status')
        selected = [h for h in cpp['handler_states'] if h['id'] == 108]
        equal(len(selected), len(native['handlers']), name + ' handler count')
        for actual, observed in zip(selected, native['handlers']):
            handlers += 1
            equal(actual['status'], observed['status'], name + ' handler status')
            state(actual['state'], observed['state'], name + ' handler')
            if observed['cursor']:
                equal(actual['cursor'], observed['cursor'], name + ' cursor')
            else: eof_cursors += 1
    diagnostic = b'Error - SubbdryVarSize is incorrect.\n'
    native_diagnostics = (args.run.parent / 'stdout.bin').read_bytes().replace(b'\r\n', b'\n').count(diagnostic)
    replacement_diagnostics = sum(bytes.fromhex(c['native_stdout_hex']).count(diagnostic) for c in replacements)
    equal(replacement_diagnostics, native_diagnostics, 'count clamp stdout diagnostic')
    report = {'status': 'PASS_COMPLETE_KNOWN_GRNDAT_STATE', 'imports': len(imports),
              'handlers': handlers, 'checks': checks, 'eof_cursor_unavailable': eof_cursors,
              'clamp_diagnostics': native_diagnostics,
              'evidence_id': captured['evidence_id'], 'capture_sha256': hashlib.sha256(args.capture.read_bytes()).hexdigest(),
              'qualification': 'Table buffers read only with observed allocation extents; opaque metadata/padding, pointer ABI, allocation failure, enabled grain profile and scientific evaluation excluded. Refresh events retained for separate branch-order checks.'}
    if args.batch_id:
        root = Path(__file__).resolve().parents[1]
        batch = next(b for b in json.loads((root / 'analysis/grndat-native-batch-plan.json').read_text())['batches'] if b['id'] == args.batch_id)
        plan = json.loads((root / 'analysis/chat-a-grndat-fixture-plan.json').read_text())
        cases = {c['id']: c for c in plan['cases']}
        equal(run['ordered_imports'], batch['ordered_imports'], 'prepared import order')
        refresh_checks = []
        for step, observed in zip(batch['sequence'], imports):
            if step['role'] != 'action': continue
            definition = cases.get(step['case']) or batch.get('case_expectations', {})[step['case']]
            expected = definition['expected']
            if observed['handlers']:
                calls = observed['handlers'][-1]['refreshes']
                equal(observed['handlers'][-1]['status'], expected['handler_status'], step['case'] + ' source-derived status')
                equal(calls.count(0), int(expected['postbranch_refresh0']), step['case'] + ' postbranch refresh')
                equal(calls, sorted(calls, reverse=True), step['case'] + ' creation precedes postbranch')
                equal(all(x in (0, 1) for x in calls), True, step['case'] + ' refresh options')
                refresh_checks.append({'case': step['case'], 'native_refreshes': calls, 'postbranch_expected': expected['postbranch_refresh0']})
        report['action_refresh_checks'] = refresh_checks
        report['checks'] = checks
    if args.report: args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__': main()
