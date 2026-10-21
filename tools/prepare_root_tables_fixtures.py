"""Prepare source-planned root-table cases; never reads C++ or native results."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import struct


def raw(values):
    return b''.join(struct.pack('<d', v) for v in values).hex()


def table(kind, rows=0, width=1, values=()):
    rank = 0 if kind == 181 else 1
    axes = [list(values[i * (width + 1)] for i in range(rows)), [], []] if rank else [[], [], []]
    dependent = [v for i, v in enumerate(values) if i % (width + 1)] if rank else list(values)
    return dict(metadata=[kind, rank, 0, 0, 0, rows, 0, 0, width, 0, 0, 0],
                constant_hex='00'*8, preserved60_hex='00'*8,
                axes_hex=[raw(v) for v in axes], dependent_hex=raw(dependent))


def defaults():
    return dict(frequency_rows=6, frequency_columns=1,
                frequency=table(181, width=6, values=[1, 0, 100, 20, 1, 1]),
                records=[table(k) for k in [176, 177, 178]])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cohort', choices=['training', 'evaluation'])
    args = parser.parse_args()
    evaluation = args.cohort == 'evaluation'
    root = Path('tests/fixtures/root_tables') / args.cohort
    root.mkdir(parents=True, exist_ok=True)
    plan_path = Path('analysis/root-tables-' + ('evaluation-inputs' if evaluation else 'fixture-plan') + '.json')
    if plan_path.exists():
        raise SystemExit('Refusing to overwrite a prepared fixture plan')
    catalog = json.loads(Path('analysis/keyword-catalog.json').read_bytes())['rows']
    ids = {r['name']: r['id'] for r in catalog if r['id'] in [84, 85, 86, 91]}
    state = defaults()
    dimension = 3
    plan = dict(id='root-tables-'+args.cohort, status='prepared_before_original_capture',
                scope='FRQNCY91 and TIMEID84/ERECID85/NRECID86; initialized owned rank0/1 tables, bounded finite payloads.',
                ordered_imports=[], inputs=[])
    if evaluation:
        plan['chronology'] = 'Prepared after training PASS; original capture precedes C++ execution. New values, ordering, extents and state sequences.'

    def call(k, header, pair, payload='', values=None, header_status=0, payload_status=0):
        return dict(keyword=k, header=header, pair=pair, payload=payload, values=values,
                    header_status=header_status, payload_status=payload_status)

    def put(name, calls=(), prefix='', suffix='', change=None, nstep=None):
        nonlocal state, dimension
        if change is not None and change != dimension:
            state = defaults()
            dimension = change
        data = prefix
        expected = []
        for c in calls:
            kid = ids[c['keyword']]
            data += c['keyword'] if c['header'] is None else c['keyword'] + (' '+c['header'] if c['header'] else '') + '\n'
            rows, width = c['pair']
            header_status = c['header_status']
            status = 2 if header_status else 0
            reset = False
            payload = False
            allocations = 0
            before = copy.deepcopy(state)
            if kid == 91:
                if rows > 0 and width > 0:
                    state['frequency_rows'], state['frequency_columns'] = rows, width
                    state['frequency']['metadata'][8] = rows * width
                    payload = True
                    payload_before = copy.deepcopy(state['frequency'])
                    if rows * width > 1:
                        allocations = 1
                        state['frequency'] = table(181, width=rows*width, values=c['values'])
                        status = 2 if c['payload_status'] else 0
            elif not header_status:
                reset = True
                allocations = 1
                state['records'][kid-84] = table(176+kid-84)
                if rows <= 0 or width <= 0:
                    status = 3
                else:
                    allocations = 3
                    payload = True
                    payload_before = table(176+kid-84, rows, width, [0.]*(rows*(width+1)))
                    state['records'][kid-84] = table(176+kid-84, rows, width, c['values'])
                    status = 2 if c['payload_status'] else 0
            h = dict(id=kid, header_values=c['pair'], header_status=header_status,
                     status=status, reset=reset, payload=payload, allocations=allocations,
                     expected_before=before, expected_fields=copy.deepcopy(state))
            if payload:
                h.update(payload_before=payload_before, payload_status=c['payload_status'])
            expected.append(h)
            data += c['payload']
        data = (data + suffix).encode('ascii')
        (root/name).write_bytes(data)
        plan['ordered_imports'].append(name)
        item = dict(file=name, bytes=len(data), sha256=hashlib.sha256(data).hexdigest(),
                    expected_handlers=expected,
                    expected_wrapper_status=next((h['status'] for h in reversed(expected) if h['status']), 0),
                    expected_final=copy.deepcopy(state))
        if nstep is not None:
            item['expected_nstep'] = nstep
        plan['inputs'].append(item)

    shift = 0.375 if evaluation else 0.
    values = [v+shift for v in [.25, -.5, 1.5, 2.25, -3.5, 4.75]]
    values[1] = -0.0
    keywords = ['FRQNCY', 'TIMEID', 'ERECID', 'NRECID']
    if evaluation:
        keywords = ['NRECID', 'ERECID', 'FRQNCY', 'TIMEID']

    def seed():
        result = []
        for k in keywords:
            rows, width = ((3, 2) if evaluation else (2, 3)) if k == 'FRQNCY' else ((3, 1) if evaluation else (2, 2))
            result.append(call(k, f'{rows} {width}', [rows, width], ' '.join(map(str, values))+'\n', values))
        return result

    def invalid():
        return [call(k, '-1 2', [-1, 2]) for k in keywords] + [call('FRQNCY', '0 3', [0, 3]), call('FRQNCY', 'bad', [0, 0])]

    def blank():
        return [call(k, '', [0, 0], header_status=1) for k in keywords]

    def partial():
        return [call(k, '2', [2, 0]) for k in keywords]

    def matching():
        return [call(k, '2 2', [2, 2], 'invalid\n', [0.]*(4 if k == 'FRQNCY' else 6)) for k in keywords]

    order = [3, 2] if not evaluation else [2, 3]
    put('defaults.KEY', prefix='\n')
    for dim in order:
        put(f'dimension{dim}.KEY', prefix=f'* DEFORM-{dim}D\n', change=dim)
        put(f'seed{dim}.KEY', seed())
        put(f'skip{dim}.KEY', [call('FRQNCY', '1 1', [1, 1], 'NSTEP 19\n')], nstep=19)
        put(f'invalid{dim}.KEY', invalid())
        put(f'blank{dim}.KEY', blank())
        put(f'partial{dim}.KEY', partial())
        for k in keywords:
            vals = [values[0], values[1], values[2]] + [0.]*(1 if k == 'FRQNCY' else 3)
            put(f'payload{dim}{k}.KEY', [call(k, '2 2', [2, 2], ' '.join(map(str, vals[:3])), vals, payload_status=1)])
        put(f'matching{dim}.KEY', matching(), suffix='NSTEP 23\n', nstep=23)
        # Wrapper retains the last nonzero status even after a later success.
        mixed = [call('FRQNCY', '', [0, 0], header_status=1), call('TIMEID', '0 2', [0, 2]), call('FRQNCY', '1 2', [1, 2], '7 8\n', [7., 8.])]
        if dim == 2:
            mixed = [mixed[1], mixed[0], mixed[2]]
        put(f'mixed{dim}.KEY', mixed)
    for k in keywords:
        put('headerEOF'+k+'.KEY', [call(k, None, [0, 0], header_status=1)])
    # A complete payload consumes the remainder of its last line, including the
    # first NSTEP. The next line must remain available to the outer parser.
    put('grow.KEY', [call('FRQNCY', '1 3', [1, 3], '.5 1.5 -0.0 NSTEP 99\nNSTEP 31\n', [.5, 1.5, -0.0])], nstep=31)
    dim = dimension
    mesh = ('RZ 1 4\n1 0 0 0\n2 1 0 0\n3 0 1 0\n4 0 0 1\nELMCON 1 1 4\n1 1 2 3 4\n' if dim == 3 else
            'RZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1\n1 1 2 3 4\n')
    put('mesh.KEY', seed(), prefix=mesh)
    put('unit.KEY', seed(), prefix=f'* DEFORM-{dim}D\nUNIT '+('2' if evaluation else '1')+'\n')
    assert len(plan['inputs']) == 32, len(plan['inputs'])
    plan_path.write_text(json.dumps(plan, indent=2)+'\n', encoding='utf-8', newline='\n')
    print(json.dumps(dict(cohort=args.cohort, imports=len(plan['inputs']), plan=str(plan_path))))


if __name__ == '__main__':
    main()
