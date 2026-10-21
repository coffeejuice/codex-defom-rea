"""Compare TRANS and grain-gated component state with retained original imports."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['driver', 'reader-driver', 'capture', 'run', 'plan']:
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    audit = subprocess.run([sys.executable, str(Path(__file__).with_name('audit_trans_discovery.py')),
                            '--capture', str(args.capture), '--run', str(args.run), '--plan', str(args.plan)],
                           text=True, capture_output=True, check=True)
    source_audit = json.loads(audit.stdout)
    evidence = json.loads(args.capture.read_bytes())
    plan = json.loads(args.plan.read_bytes())
    groups = []
    queries = []
    for message in evidence['normalized_result']['messages']:
        if message['type'] != 'send':
            continue
        value = message['payload']
        if value['operation'] == 'import-before':
            groups.append(dict(before=value, entries=[], handlers=[]))
        elif value['operation'] == 'import-after':
            groups[-1]['after'] = value
        elif value['operation'] == 'handler-before':
            groups[-1]['entries'].append(value)
        elif value['operation'] == 'handler-after':
            groups[-1]['handlers'].append(value)
        elif value['operation'] == 'feature-query':
            queries.append(value)
    data = [(args.run.parent / name).read_bytes().replace(b'\r\n', b'\n') for name in plan['ordered_imports']]
    commands = ['initialized 3 20 0', 'link 2'] + ['import ' + b.hex() for b in data]
    cpp = subprocess.run([str(args.driver.resolve())], input='\n'.join(commands) + '\n',
                         text=True, capture_output=True, check=True)
    rows = [json.loads(line) for line in cpp.stdout.splitlines()]
    assert len(rows) == len(groups) == len(data)
    reader_commands = []
    for b, row in zip(data, rows):
        for event in row['result']['events']:
            if event['id'] == 32:
                reader_commands.append('int5 0 ' + str(event['offset'] + len(event['token'])) + ' ' + b.hex() + ' -')
    readers = subprocess.run([str(args.reader_driver.resolve())], input='\n'.join(reader_commands) + '\n',
                             text=True, capture_output=True, check=True)
    reads = iter(json.loads(line) for line in readers.stdout.splitlines())
    checks = handlers = eof = 0

    def equal(actual, expected, label):
        nonlocal checks
        checks += 1
        if actual != expected:
            raise AssertionError((label, str(actual)[:400], str(expected)[:400]))

    def state(primary, linked, native, label):
        for key in ['trans', 'dimension', 'components', 'object_count', 'material_count']:
            equal(primary[key], native[key], label + '/' + key)
        equal(primary['simulation_mode']['mode_0x844'], native['mode'], label + '/mode')
        equal(primary['grain_mode_0x898'], native['field_0x898'], label + '/898')
        equal(linked['dimension'], native['linked_dimension'], label + '/linked dimension')
        equal(linked['trans'], native['linked']['trans'], label + '/linked TRANS')
        equal(linked['grain_mode_0x898'], native['linked']['field_0x898'], label + '/linked898')
        equal(linked['simulation_mode']['mode_0x844'], native['linked']['mode'], label + '/linked mode')

    for name, row, group in zip(plan['ordered_imports'], rows, groups):
        planned = next(item for item in plan['inputs'] if item['file'] == name)
        if 'expected_wrapper_status' in planned:
            equal(group['after']['status'], planned['expected_wrapper_status'], name + '/planned native wrapper')
        if 'expected_grain_widths' in planned:
            equal([group['after']['state']['components'][i] for i in [29, 53]],
                  planned['expected_grain_widths'], name + '/planned positive grain widths')
        equal(row['result']['outcome'], 'parsed', name + '/supported')
        equal(row['result']['status'], group['after']['status'], name + '/wrapper')
        state(row['before_state'], row['before_linked_state'], group['before']['state'], name + '/before')
        state(row['state'], row['linked_state'], group['after']['state'], name + '/after')
        selected = [h for h in row['handler_states'] if h['id'] == 32]
        equal(len(selected), len(group['handlers']), name + '/handler count')
        for h, native in zip(selected, group['handlers']):
            handlers += 1
            read = next(reads)
            call = native['calls'][0]
            equal(h['status'], native['status'], name + '/handler status')
            equal(read['status'], call['status'], name + '/reader status')
            equal(read['values'], call['values'], name + '/five integer outputs')
            state(h['state'], h['linked_state'], native['state'], name + '/handler')
            if native['cursor']:
                equal(h['cursor'], native['cursor'], name + '/cursor')
                equal(read['cursor'], native['cursor'], name + '/reader cursor')
            else:
                eof += 1
    equal(next(reads, None), None, 'all reader requests consumed')
    coverage = {str(component): sum(group['after']['state']['components'][component] > 0 for group in groups)
                for component in [29, 53]}
    for component, minimum in plan.get('minimum_positive_width_imports', {}).items():
        equal(coverage[component] >= minimum, True, 'required nonzero component ' + component)
    for query in queries:
        if 'fallback_index' in query:
            equal(query['fallback_index'], 185 if query['state']['dimension'] == 2 else 186, 'dimension fallback index')
            equal(query['fallback_value'], 0, 'native keyword fixture fallback table remains zero')
    report = dict(status='PASS_NATIVE_TRANS_COMPARISON', imports=len(rows), handlers=handlers,
                  checks=checks, positive_width_imports=coverage,
                  eof_cursors_unobserved=eof, source_audit=source_audit,
                  evidence_id=evidence['evidence_id'],
                  capture_sha256=hashlib.sha256(args.capture.read_bytes()).hexdigest(),
                  run_sha256=hashlib.sha256(args.run.read_bytes()).hexdigest(),
                  scope='Bounded console roots and finite signed32 integer fixtures; TRANS raw helper outputs, state, all79 component widths and linked isolation. Native DB-populated fallback and scientific consumers remain unverified.')
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8', newline='\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
