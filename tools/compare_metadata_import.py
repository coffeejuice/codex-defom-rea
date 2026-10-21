"""Replay the independently reviewed metadata fixtures against the replacement.

Expectations remain the frozen static specification, not a native runtime oracle.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    fixtures = root / 'analysis/chat-b-roundtrip-fixtures'
    manifest_bytes = (fixtures / 'manifest.json').read_bytes()
    assert hashlib.sha256(manifest_bytes).hexdigest() == 'b291dd920d8c4928f09bef3df3a0d7d561881fee200b33bb86b49ef0babd1270'
    manifest = json.loads(manifest_bytes)
    checks = 0

    def equal(actual, expected, label):
        nonlocal checks
        checks += 1
        if actual != expected:
            raise AssertionError(f'{label}: actual={actual!r}, expected={expected!r}')

    def translated(filename):
        raw = (fixtures / filename).read_bytes()
        binding = next(x for x in manifest['cases'] + manifest['setup_files'] if x['file'] == filename)
        equal(hashlib.sha256(raw).hexdigest(), binding['sha256'], filename + ' hash')
        return raw.replace(b'\r\n', b'\n')

    def run(commands):
        proc = subprocess.run([str(args.driver.resolve())], input='\n'.join(commands) + '\n',
                              text=True, capture_output=True, check=True)
        return [json.loads(line) for line in proc.stdout.splitlines()]

    handlers = 0
    helper_lines = 0
    for case in manifest['cases']:
        name = case['file']
        data = translated(name)
        commands = [f"import {s['target']} {translated(s['file']).hex()}" for s in case['setup_imports']]
        commands.append(f'import primary {data.hex()}')
        replay = run(commands)
        for setup in replay[:-1]:
            equal(setup['outcome'], 0, name + ' setup supported')
            equal(setup['status'], 0, name + ' setup status')
        actual = replay[-1]
        trace = case['trace']
        expected = case['expected_after_future_metadata_implementation']
        equal(actual['outcome'], 0, name + ' supported')
        equal(actual['status'], expected['parser_final_status'], name + ' sticky status')
        equal(actual['cursor'], trace['expected_parser_cursor'], name + ' final cursor')
        equal(actual['eof'], trace['expected_reached_eof'], name + ' EOF')
        equal(actual['metadata']['primary'], {k: expected[k] for k in
              ['title', 'operation_name', 'simulation_name', 'cursim']}, name + ' final primary')
        equal(actual['metadata']['linked'], expected['linked_metadata'], name + ' final linked')
        equal(actual['dimensions'], {'primary': expected['primary_dimension'],
              'linked': expected['linked_dimension_if_present']}, name + ' final dimensions')
        # Parser callbacks intentionally exclude comments/header/unknown records.
        events = [e for e in trace['events'] if e['observer_callback']]
        equal(len(actual['handlers']), len(events), name + ' callback count')
        for got, event in zip(actual['handlers'], events):
            handlers += 1
            for key in ['token', 'offset', 'cursor']:
                equal(got[key], event[key], name + ' callback ' + key)
            equal(got['status'], event['handler_status'], name + ' callback status')
            equal(got['metadata'], event['metadata_after'], name + ' callback metadata')
            equal(got['dimensions'], event['dimensions_after'], name + ' callback dimensions')
        parser_events = [e for e in trace['events'] if e['kind'] != 'header_or_comment']
        equal(len(actual['events']), len(parser_events), name + ' event count')
        for got, event in zip(actual['events'], parser_events):
            for key in ['token', 'offset', 'cursor']:
                equal(got[key], event[key], name + ' parser ' + key)
            equal(got['unknown_warning'], event['kind'] == 'unknown', name + ' warning')
        # Independently exercise reader stages from the frozen entry cursor and
        # prestate. This does not add observer callbacks or expose parser internals.
        commands = []
        for event in events:
            key = {'TITLE': 'title', 'OPRNAM': 'operation_name', 'SIMNAM': 'simulation_name'}.get(event['token'])
            previous = event['metadata_before']['primary'].get(key, '')
            commands.append(f"helper {event['token']} {event['handler_entry_cursor']} {data.hex()} {previous.encode('ascii').hex() or '-'}")
        for got, event in zip(run(commands), events):
            equal(got['status'], event['helper_status'], name + ' helper status')
            equal(got['cursor'], event['cursor'], name + ' helper cursor')
            equal(len(got['lines']), len(event['line_reads']), name + ' read count')
            helper_lines += len(got['lines'])
            for line, bound in zip(got['lines'], event['line_reads']):
                equal(line, {'offset': bound['translated_offset'], 'cursor': bound['translated_cursor'],
                      'status': bound['status'], 'bytes': bytes.fromhex(bound['bytes_hex']).decode('ascii')}, name + ' line stage')
            if event['token'] == 'CURSIM':
                equal(got['values'], event['metadata_after']['primary']['cursim'], name + ' partial integers')
            else:
                key = {'TITLE': 'title', 'OPRNAM': 'operation_name', 'SIMNAM': 'simulation_name'}[event['token']]
                equal(got['text'], event['metadata_after']['primary'][key], name + ' bounded text')
    report = {'status': 'PASS_REPLACEMENT_AGAINST_STATIC_EXPECTATIONS', 'cases': len(manifest['cases']),
              'handlers': handlers, 'helper_line_attempts': helper_lines, 'checks': checks,
              'native_runtime_comparison': 'not performed by this test',
              'manifest_sha256': hashlib.sha256(manifest_bytes).hexdigest()}
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
