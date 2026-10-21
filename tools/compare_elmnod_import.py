"""Compare defined ELMNOD payloads and refresh state with retained original data."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['driver', 'reader-driver', 'capture', 'run', 'plan']:
        parser.add_argument('--'+name, type=Path, required=True)
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
    assert hashlib.sha256((args.run.parent/'observer-source.js').read_bytes()).hexdigest() == native['source_sha256']
    assert run['ordered_imports'] == plan['ordered_imports']
    for item in run['inputs']:
        expected = next(x for x in plan['inputs'] if x['file'] == item['name'])
        assert item['sha256_before'] == item['sha256_after'] == item['source_sha256_after'] == expected['sha256']
        assert hashlib.sha256((args.run.parent/item['name']).read_bytes()).hexdigest() == expected['sha256']
    groups = []
    for message in native['messages']:
        assert message['type'] != 'error', message
        if message['type'] != 'send':
            continue
        value = message['payload']
        assert value['operation'] != 'probe-error', value
        if value['operation'] == 'import-before':
            groups.append(dict(before=value, entries=[], handlers=[]))
        elif value['operation'] == 'import-after':
            groups[-1]['after'] = value
        elif value['operation'] == 'handler-before':
            groups[-1]['entries'].append(value)
        elif value['operation'] == 'handler-after':
            groups[-1]['handlers'].append(value)
    data = [(args.run.parent/name).read_bytes().replace(b'\r\n', b'\n') for name in plan['ordered_imports']]
    commands = ['initialized 3 20 0', 'link 2'] + ['import '+b.hex() for b in data]
    cpp = subprocess.run([str(args.driver.resolve())], input='\n'.join(commands)+'\n', text=True, capture_output=True, check=True)
    rows = [json.loads(line) for line in cpp.stdout.splitlines()]
    assert len(rows) == len(groups) == len(data)
    requests = []
    for b, row, group in zip(data, rows, groups):
        events = [event for event in row['result']['events'] if event['id'] == 35]
        assert len(events) == len(group['handlers'])
        for event, handler in zip(events, group['handlers']):
            start = event['offset']+len(event['token'])
            end = b.find(b'\n', start, start+1023)
            payload_start = end+1 if end >= 0 else min(len(b), start+1023)
            requests.append('int1 0 '+str(start)+' '+b.hex()+' -')
            requests.append('int10 0 '+str(payload_start)+' '+b.hex()+' -')
    result = subprocess.run([str(args.reader_driver.resolve())], input='\n'.join(requests)+'\n', text=True, capture_output=True, check=True)
    reads = iter(json.loads(line) for line in result.stdout.splitlines())
    checks = handlers = eof = aliases = 0

    def equal(actual, expected, label):
        nonlocal checks
        checks += 1
        if actual != expected:
            raise AssertionError((label, str(actual)[:500], str(expected)[:500]))

    def state(primary, linked, observed, label):
        for key in ['element_layout_0x888', 'heat_method_0x868', 'trans', 'dimension', 'components']:
            equal(primary[key], observed[key], label+'/'+key)
        equal(primary['element_layout_known'], [True]*10, label+'/defined controls')
        equal(primary['grain_mode_0x898'], observed['element_layout_0x888'][4], label+'/grain alias')
        equal(primary['simulation_mode']['mode_0x844'], observed['mode'], label+'/mode')
        equal(primary['object_count'], observed['objects'], label+'/objects')
        equal(primary['material_count'], observed['materials'], label+'/materials')
        for key in ['element_layout_0x888', 'heat_method_0x868', 'trans']:
            equal(linked[key], observed['linked'][key], label+'/linked '+key)
        equal(linked['simulation_mode']['mode_0x844'], observed['linked']['mode'], label+'/linked mode')
        equal(linked['dimension'], observed['linked_dimension'], label+'/linked dimension')

    equal(groups[0]['before']['state']['element_layout_0x888'], [0]*10, 'native initialized defaults')
    for name, row, group in zip(plan['ordered_imports'], rows, groups):
        expected = next(item for item in plan['inputs'] if item['file'] == name)
        equal(group['before']['file'], name, name+'/input identity')
        equal(group['after']['file'], name, name+'/output identity')
        equal(group['after']['status'], expected['expected_wrapper_status'], name+'/planned wrapper')
        equal(group['after']['state']['element_layout_0x888'], expected['expected_final_layout'], name+'/planned final layout')
        equal(group['after']['state']['heat_method_0x868'], expected['expected_final_heat_method'], name+'/planned heat method')
        equal(row['result']['outcome'], 'parsed', name+'/supported')
        equal(row['result']['status'], group['after']['status'], name+'/wrapper')
        state(row['before_state'], row['before_linked_state'], group['before']['state'], name+'/before')
        state(row['state'], row['linked_state'], group['after']['state'], name+'/after')
        selected = [h for h in row['handler_states'] if h['id'] == 35]
        equal(len(selected), len(expected['expected_handlers']), name+'/planned handlers')
        equal(len(group['handlers']), len(selected), name+'/native handlers')
        equal(len(group['entries']), len(selected), name+'/entry pairing')
        for cpp_handler, observed, entry, planned in zip(selected, group['handlers'], group['entries'], expected['expected_handlers']):
            handlers += 1
            count_read, payload_read = next(reads), next(reads)
            count, header_status, payload_status = planned
            equal(len(observed['headers']), 1, name+'/count calls')
            equal(len(observed['payloads']), 1, name+'/payload calls even after failed header')
            header, payload = observed['headers'][0], observed['payloads'][0]
            equal(header['value'], count, name+'/planned count')
            equal(header['status'], header_status, name+'/planned count status')
            equal(count_read['values'], [count], name+'/replacement count')
            equal(count_read['status'], header_status, name+'/replacement count status')
            equal(payload['count_before'], count, name+'/payload count input')
            equal(payload['count_after'], count, name+'/clamp does not modify count pointer')
            equal(payload['status'], payload_status, name+'/planned payload status')
            equal(payload_read['status'], payload_status, name+'/replacement payload status')
            if payload['cursor_before']:
                equal(count_read['cursor'], payload['cursor_before'], name+'/payload begins after count line')
            copied = min(max(count, 0), 10) if payload_status == 0 else 0
            equal(len(payload_read['values']) >= copied, True, name+'/native copied prefix is defined')
            truth = list(payload['before'])
            truth[:copied] = payload_read['values'][:copied]
            equal(payload['after'], truth, name+'/source-derived copied prefix and retained tail')
            equal(payload['before'], entry['state']['element_layout_0x888'], name+'/no count-read destination mutation')
            equal(observed['status'], 0 if header_status == 0 else 2, name+'/payload status ignored')
            equal(cpp_handler['status'], observed['status'], name+'/handler status')
            equal([r['stage'] for r in observed['refreshes']], ['before','after'], name+'/one unconditional refresh')
            before, after = observed['refreshes']
            equal(before['option'], 0, name+'/refresh option')
            equal(before['state']['element_layout_0x888'], payload['after'], name+'/copied values precede refresh')
            equal(after['state'], observed['state'], name+'/refresh poststate')
            if payload['after'][4] != observed['state']['element_layout_0x888'][4]:
                aliases += 1
            state(cpp_handler['state'], cpp_handler['linked_state'], observed['state'], name+'/handler')
            if header['cursor']:
                equal(count_read['cursor'], header['cursor'], name+'/count cursor')
            if observed['cursor']:
                equal(cpp_handler['cursor'], observed['cursor'], name+'/handler cursor')
                equal(payload_read['cursor'], observed['cursor'], name+'/payload cursor')
            else:
                eof += 1
    equal(next(reads, None), None, 'all readers consumed')
    if plan.get('minimum_alias_overwrites'):
        equal(aliases >= plan['minimum_alias_overwrites'], True, 'required grain alias overwrite coverage')
    report = dict(status='PASS_NATIVE_ELMNOD_COMPARISON', imports=len(rows), handlers=handlers,
                  checks=checks, observed_grain_alias_overwrites=aliases, eof_cursors_unobserved=eof,
                  evidence_id=evidence['evidence_id'], capture_sha256=hashlib.sha256(args.capture.read_bytes()).hexdigest(),
                  run_sha256=hashlib.sha256(args.run.read_bytes()).hexdigest(),
                  scope='Defined copied prefixes, ordinary initialized roots. Header/payload status, count clamp, exact ten fields, aliasing, all79 widths, refresh order and linked state. Native indeterminate-stack payloads and full scientific buffers excluded.')
    if args.report:
        args.report.write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8', newline='\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
