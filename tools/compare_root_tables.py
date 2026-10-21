"""Compare root-table state, allocation order and token consumption with REA."""
import argparse
from collections import Counter
import copy
import hashlib
import json
from pathlib import Path
import struct
import subprocess

from prepare_root_tables_fixtures import defaults, table


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['driver', 'capture', 'run', 'plan']:
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    evidence = json.loads(args.capture.read_bytes())
    run = json.loads(args.run.read_bytes())
    plan = json.loads(args.plan.read_bytes())
    native = evidence['normalized_result']
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
        operation = value['operation']
        if operation == 'import-before':
            groups.append(dict(before=value, entries=[], handlers=[], lookups=[]))
        elif operation == 'import-after':
            groups[-1]['after'] = value
        elif operation == 'lookup':
            groups[-1]['lookups'].append(value)
        elif operation == 'handler-before':
            groups[-1]['entries'].append(value)
        elif operation == 'handler-after':
            groups[-1]['handlers'].append(value)
    # Assert source-planned native outcomes before executing the replacement.
    checks = eof = allocations = retained = lookups = 0
    counts = Counter()
    dimensions = Counter()

    def equal(actual, expected, label):
        nonlocal checks
        checks += 1
        if actual != expected:
            raise AssertionError((label, str(actual)[:600], str(expected)[:600]))

    equal(len(groups), len(plan['inputs']), 'complete imports')
    equal(groups[0]['before']['state']['root_record_tables'], defaults(), 'native initial primary tables')
    equal(groups[0]['before']['state']['linked_root_record_tables'], defaults(), 'native initial linked tables')
    for expected, group in zip(plan['inputs'], groups):
        name = expected['file']
        equal(group['before']['file'], name, name+'/input identity')
        equal(group['after']['file'], name, name+'/output identity')
        equal(group['after']['status'], expected['expected_wrapper_status'], name+'/planned wrapper status')
        equal(group['after']['state']['root_record_tables'], expected['expected_final'], name+'/planned final state')
        if 'expected_nstep' in expected:
            equal(group['after']['state']['stopping_controls']['nstep'], expected['expected_nstep'], name+'/neighbor consumption')
        equal(len(group['handlers']), len(expected['expected_handlers']), name+'/planned handler count')
        equal(len(group['entries']), len(group['handlers']), name+'/entry pairing')
        for truth, observed, entry in zip(expected['expected_handlers'], group['handlers'], group['entries']):
            kid = truth['id']
            counts[kid] += 1
            dimensions[f"{kid}/{entry['state']['dimension']}"] += 1
            equal(observed['id'], kid, name+'/handler id')
            equal(entry['id'], kid, name+'/entry id')
            equal(observed['status'], truth['status'], name+'/planned handler status')
            equal(entry['state']['root_record_tables'], truth['expected_before'], name+'/planned entry state')
            equal(observed['state']['root_record_tables'], truth['expected_fields'], name+'/planned exit state')
            equal(observed['refreshes'], [], name+'/no refresh')
            order = observed['order']
            kinds = ['header-enter', 'header-leave']
            allocate = ['allocate-enter', 'release-enter', 'release-leave', 'allocate-leave']
            if truth['reset']:
                kinds += ['reset-enter', 'release-enter', 'release-leave'] + allocate + ['reset-leave']
                if truth['payload']:
                    kinds += allocate
            if truth['payload']:
                kinds += ['payload-enter']
                if kid != 91 or truth['expected_fields']['frequency']['metadata'][8] > 1:
                    kinds += allocate
                kinds += ['payload-leave']
            equal([x['kind'] for x in order], kinds, name+'/header-reset-release-allocation-payload order')
            equal(order[1], dict(kind='header-leave', values=truth['header_values'], status=truth['header_status']), name+'/header helper outputs')
            allocations += truth['allocations']
            allocated = [x['table'] for x in order if x['kind'] == 'allocate-leave']
            equal(len(allocated), truth['allocations'], name+'/allocation count')
            final_table = truth['expected_fields']['frequency'] if kid == 91 else truth['expected_fields']['records'][kid-84]
            zeroed = copy.deepcopy(final_table)
            zeroed['axes_hex'] = ['0'*len(x) for x in zeroed['axes_hex']]
            zeroed['dependent_hex'] = '0'*len(zeroed['dependent_hex'])
            for index, value in enumerate(allocated):
                expected_table = table(176+kid-84) if truth['reset'] and index == 0 else zeroed
                equal(value, expected_table, name+'/calloc and initialized metadata')
            for item in order:
                if item['kind'] == 'reset-leave':
                    equal(item['table'], table(176+kid-84), name+'/reset default profile')
                elif item['kind'] == 'payload-enter':
                    equal(item['table'], truth['payload_before'], name+'/payload entry storage versus logical extent')
                elif item['kind'] == 'payload-leave':
                    equal(item['status'], truth['payload_status'], name+'/payload helper status')
                    equal(item['table'], final_table, name+'/payload retained prefix')
            if kid == 91 and truth['payload'] and truth['allocations'] == 0:
                retained += 1
                equal(final_table['dependent_hex'], truth['expected_before']['frequency']['dependent_hex'], name+'/width1 retained storage')

    commands = ['initialized 3 20 0', 'link 2'] + ['import '+(args.run.parent/name).read_bytes().replace(b'\r\n', b'\n').hex() for name in plan['ordered_imports']]
    result = subprocess.run([str(args.driver.resolve())], input='\n'.join(commands)+'\n', text=True, capture_output=True, check=True)
    rows = [json.loads(line) for line in result.stdout.splitlines()]
    equal(len(rows), len(groups), 'C++ import count')

    def state(primary, linked, observed, label):
        for key in ['root_record_tables', 'iteration', 'dimensional_step', 'time_step_imports', 'stopping_vectors_hex', 'stopping_controls', 'dimension', 'components', 'object_count', 'material_count', 'stpinc_0x8b4', 'heat_method_0x868', 'element_layout_0x888']:
            equal(primary[key], observed[key], label+'/'+key)
        for key, source in [('root_record_tables','linked_root_record_tables'), ('iteration','linked_iteration'), ('dimensional_step','linked_dimensional_step'), ('time_step_imports','linked_time_step_imports'), ('stopping_vectors_hex','linked_vectors_hex'), ('stopping_controls','linked_stopping'), ('dimension','linked_dimension')]:
            equal(linked[key], observed[source], label+'/'+source)
        time = primary['finalization_controls']
        raw = b''.join(struct.pack('<d', time[k]) for k in ['time_step_0x920', 'time_step_secondary_0x928', 'step_increment_0x930']).hex()
        equal(dict(values_hex=raw, control=time['control_0x93c'], mode=time['mode_0x990']), observed['time'], label+'/time')
        equal([time[k] for k in ['step_number_0x8b0', 'saved_step_0x744']], observed['steps'], label+'/step numbers')

    for row, group, expected in zip(rows, groups, plan['inputs']):
        name = expected['file']
        equal(row['result']['outcome'], 'parsed', name+'/supported')
        equal(row['result']['status'], group['after']['status'], name+'/C++ wrapper status')
        state(row['before_state'], row['before_linked_state'], group['before']['state'], name+'/before')
        state(row['state'], row['linked_state'], group['after']['state'], name+'/after')
        equal([{k:t[k] for k in ['token','id','category']} for t in row['result']['events']], [{k:t[k] for k in ['token','id','category']} for t in group['lookups']], name+'/complete token order')
        lookups += len(group['lookups'])
        handlers = [h for h in row['handler_states'] if h['id'] in [84,85,86,91]]
        equal(len(handlers), len(group['handlers']), name+'/C++ handlers')
        for h, observed in zip(handlers, group['handlers']):
            equal(h['id'], observed['id'], name+'/C++ keyword')
            equal(h['status'], observed['status'], name+'/C++ handler status')
            state(h['state'], h['linked_state'], observed['state'], name+'/C++ handler state')
            if observed['cursor']:
                equal(h['cursor'], observed['cursor'], name+'/cursor')
            else:
                eof += 1
    for kid in [84,85,86,91]:
        for dim in [2,3]:
            equal(dimensions[f'{kid}/{dim}'] > 0, True, f'required dimension {kid}/{dim}')
    equal(retained > 0, True, 'nonvacuous retained-buffer cases')
    report = dict(status='PASS_NATIVE_ROOT_TABLES_COMPARISON', imports=len(rows), handlers=sum(counts.values()),
                  handler_counts=dict(counts), dimension_counts=dict(dimensions), checks=checks,
                  allocations=allocations, retained_buffers=retained, lookup_tokens=lookups, eof_cursors_unobserved=eof,
                  evidence_id=evidence['evidence_id'], capture_sha256=hashlib.sha256(args.capture.read_bytes()).hexdigest(),
                  run_sha256=hashlib.sha256(args.run.read_bytes()).hexdigest(),
                  scope='Four root tables; exact initialized metadata and payload bytes, observed ownership extents, calloc/reset/read order, prefix retention, errors and complete token order. Scientific consumers, huge extents, allocation failure and uninitialized padding excluded.')
    if args.report:
        args.report.write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8', newline='\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
