"""Audit source-derived TRANS mutations in a retained original-app observation.

This is not a comparison with a C++ candidate and does not launch the original.
"""
import argparse
import hashlib
import json
from pathlib import Path


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--capture', type=Path, required=True)
    parser.add_argument('--run', type=Path, required=True)
    parser.add_argument('--plan', type=Path, required=True)
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    evidence = json.loads(args.capture.read_bytes())
    native = evidence['normalized_result']
    run = json.loads(args.run.read_bytes())
    plan = json.loads(args.plan.read_bytes())
    assert run['status'] == 'completed' and run['exit_code'] == 0
    assert run['executable_sha256'] == 'f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
    assert native['pid'] == run['pid'] and native['state'] == 'detached'
    assert native['cleanup_error'] is None and not native['messages_truncated']
    assert digest(args.run.parent / 'observer-source.js') == native['source_sha256']
    assert run['ordered_imports'] == plan['ordered_imports']
    for item in run['inputs']:
        expected = next(x for x in plan['inputs'] if x['file'] == item['name'])
        assert item['sha256_before'] == item['sha256_after'] == item['source_sha256_after'] == expected['sha256']
        assert digest(args.run.parent / item['name']) == expected['sha256']
    groups = []
    queries = []
    for message in native['messages']:
        assert message['type'] != 'error', message
        if message['type'] != 'send':
            continue
        value = message['payload']
        operation = value['operation']
        assert operation != 'probe-error', value
        if operation == 'import-before':
            groups.append(dict(before=value, entries=[], handlers=[]))
        elif operation == 'import-after':
            groups[-1]['after'] = value
        elif operation == 'handler-before':
            groups[-1]['entries'].append(value)
        elif operation == 'handler-after':
            groups[-1]['handlers'].append(value)
        elif operation == 'feature-query':
            queries.append(value)
    checks = handlers = failed = partial = eof = 0

    def equal(actual, expected, label):
        nonlocal checks
        checks += 1
        if actual != expected:
            raise AssertionError((label, actual, expected))

    equal(len(groups), len(plan['ordered_imports']), 'imports')
    equal(groups[0]['before']['state']['trans'], [0, 1, 0, 0, 0], 'initial controls')
    equal(groups[0]['before']['state']['field_0x898'], 0, 'initial898')
    for filename, group in zip(plan['ordered_imports'], groups):
        expected = next(x['expected_handler_statuses'] for x in plan['inputs'] if x['file'] == filename)
        equal(group['before']['file'], filename, 'file before')
        equal(group['after']['file'], filename, 'file after')
        equal(len(group['handlers']), len(expected), filename + '/planned count')
        equal(len(group['entries']), len(expected), filename + '/entry count')
        for before, after, status in zip(group['entries'], group['handlers'], expected):
            handlers += 1
            equal(after['status'], status, filename + '/status')
            equal(len(after['calls']), 1, filename + '/reader calls')
            call = after['calls'][0]
            equal(call['skip'], 0, filename + '/skip')
            equal(call['status'], 0 if status == 0 else 1, filename + '/reader status')
            truth = list(before['state']['trans'])
            if call['status'] == 0:
                for i, value in enumerate(call['values']):
                    if value >= 0:
                        truth[i] = value
            else:
                failed += 1
                equal(call['values'], [0] * 5, filename + '/failed reader initialized outputs')
            equal(after['state']['trans'], truth, filename + '/conditional ordered stores')
            for key in before['state']:
                if key != 'trans':
                    equal(after['state'][key], before['state'][key], filename + '/preserved ' + key)
            equal(after['refreshes'], [], filename + '/no refresh')
            if not after['cursor']:
                eof += 1
        if filename.startswith('reset'):
            equal(group['after']['state']['trans'], [0, 1, 0, 0, 0], filename + '/reset defaults')
            equal(group['after']['state']['field_0x898'], 0, filename + '/reset898')
    # Explicit observations complement the general source-derived store rule.
    by_file = {x['after']['file']: x for x in groups}
    for filename, widths, mode in [('grain5.KEY', [0, 16], 0), ('disable.KEY', [0, 0], 0),
                                  ('grain2.KEY', [0, 0], 1), ('grain1.KEY', [0, 16], 0),
                                  ('grain4.KEY', [0, 16], 0), ('grain0.KEY', [0, 0], 0)]:
        if filename not in by_file:
            continue
        state = by_file[filename]['after']['state']
        equal([state['components'][i] for i in [29, 53]], widths, filename + '/observed widths')
        equal(state['field_0x898'], mode, filename + '/query side effect')
    for filename in ['partial.KEY', 'matching.KEY']:
        if filename not in by_file:
            continue
        for call in by_file[filename]['handlers']:
            partial += 1
            equal(call['status'], 0, filename + '/partial or matching success')
    report = dict(status='PASS_ORIGINAL_TRANS_SOURCE_AUDIT', imports=len(groups),
                  handlers=handlers, checks=checks, failed_read_handlers=failed,
                  partial_or_matching_handlers=partial, feature_queries=len(queries),
                  eof_cursors_unobserved=eof, evidence_id=evidence['evidence_id'],
                  capture_sha256=digest(args.capture), run_sha256=digest(args.run),
                  scope='Original observation versus source-derived TRANS stores only. No C++ comparison or held-out evaluation; dependent grain sizing is outside this source audit.')
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8', newline='\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
