"""Prepare source-derived PRSNAM string and lifecycle cases before observation."""
import argparse
import copy
import hashlib
import json
from pathlib import Path


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('cohort', choices=['prototype', 'training', 'evaluation'])
    a = ap.parse_args()
    evaluation = a.cohort == 'evaluation'
    delta = 1 if evaluation else 0
    base = Path('tests/fixtures/prsnam') / a.cohort
    dest = Path('analysis/prsnam-' + a.cohort + '-inputs.json')
    if dest.exists():
        raise SystemExit('Refusing to replace prepared inputs')
    base.mkdir(parents=True, exist_ok=True)
    dimension = 3
    records = {}
    nstep = 1
    mid = 3 + delta
    plan = dict(status='source_planned_before_native', cohort=a.cohort,
                ordered_imports=[], inputs=[], qualification='PRSNAM257 owned2D/3D string import,capacity80,defined signed selectors,logical byte strings and bounded1023-byte fgets chunks. Includes truncation,whitespace,high bytes,long header/name spill,EOF and lifecycle. Uninitialized padding,embedded NUL/control-Z/text-mode CRLF offsets,allocation failure,arbitrary aliases/native ABI,scientific consumers and complete application remain pending.')
    if evaluation:
        plan['chronology'] = 'Prepared after training PASS;original runs before C++ with changed IDs/text and reversed dimension/selected object order.'

    def create(oid):
        for i in range(len(records) + 1, oid + 1):
            records[i] = ''

    def state():
        return copy.deepcopy({str(k): v for k, v in records.items()})

    def op(name='', oid=1, inline='', mode='normal'):
        return dict(name=name, oid=oid, inline=inline, mode=mode)

    def apply(o):
        oid = o['oid']
        expected = dict(id=257, status=2, headers=[], reads=[], refreshes=[],
                        selections=[], order=[])
        if o['mode'] == 'selector_eof':
            expected['expected_names'] = state()
            return 'PRSNAM', expected
        text = 'PRSNAM ' + str(oid) + o['inline']
        if o['mode'] != 'header_eof':
            text += '\n'
            if o['mode'] != 'name_eof':
                text += o['name'] + ('' if o['mode'] == 'unterminated' else '\n')
        count = len(records)
        create(oid)
        expected['selections'] = [dict(id=oid, before_count=count,
                                      after_count=len(records), result_id=oid if oid > 0 else None)]
        expected['order'] = ['selection']
        if oid > 0:
            remaining = text[len('PRSNAM ' + str(oid)):]
            before = records[oid]
            header = dict(kind='text', capacity=80, before_hex=before, status=1, after_hex=before)
            expected['order'].append('text-enter')
            for index in range(2):
                value = None
                if remaining:
                    end = min(1023, len(remaining))
                    lf = remaining.find('\n', 0, end)
                    if lf >= 0:
                        end = lf + 1
                    value, remaining = remaining[:end], remaining[end:]
                expected['reads'].append(dict(capacity=1024, value=value))
                expected['order'].append('fgets')
                if value is None:
                    break
                if index == 1:
                    records[oid] = value.split('\n', 1)[0][:79].rstrip('\0 \n\r').encode('latin1').hex()
                    header['status'] = 0
                    header['after_hex'] = records[oid]
                    expected['status'] = 0
            expected['headers'].append(header)
            expected['order'].append('text-leave')
        expected['expected_names'] = state()
        return text, expected

    def put(label, ops=(), prefix='', switch=None, new_object=None, guard=None):
        nonlocal dimension, nstep, records
        if switch is not None and switch != dimension:
            dimension, nstep, records = switch, 1, {}
        if new_object is not None:
            create(new_object)
        data, handlers = prefix, []
        for o in ops:
            text, expected = apply(o)
            data += text
            handlers.append(expected)
        if guard is not None:
            data += f'NSTEP {guard}\n'
            nstep = guard
        b = data.encode('latin1')
        name = f'{len(plan["inputs"]):02d}_{label}.KEY'
        (base / name).write_bytes(b)
        plan['ordered_imports'].append(name)
        plan['inputs'].append(dict(file=name, sha256=hashlib.sha256(b).hexdigest(),
                                   expected_handlers=handlers, expected_names=state(),
                                   expected_nstep=nstep,
                                   expected_wrapper_status=next((h['status'] for h in reversed(handlers) if h['status']), 0)))

    mesh3 = Path('tests/fixtures/custom_fields/01_spatial_mesh.KEY').read_text().split('\n', 1)[1]
    mesh2 = 'OBJTYP 1 2 0\nRZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1 4\n1 1 2 3 4\n'
    for dim in ([2, 3] if evaluation else [3, 2]):
        mesh = mesh3 if dim == 3 else mesh2
        tag = f'evaluation{delta}' if evaluation else 'training0'
        put(f'd{dim}_reset', prefix=f'* DEFORM-{dim}D\nUNIT 2\nUNIT 1\n' + mesh + 'OBJNAM 1\nIndependent object name\n', switch=dim, new_object=1)
        put(f'd{dim}_positive', [op(f'  {tag} object {oid}  ', oid) for oid in ([mid, 1] if evaluation else [1, mid])], guard=301+delta)
        put(f'd{dim}_limits', [op('A'*79), op('B'*80), op('C'*81), op('D'*78+' '+ 'ignored')], guard=303+delta)
        put(f'd{dim}_whitespace', [op(''), op('   '), op(' leading, punctuation  \r  '), op('tab\t   '), op('vertical\v\f  '), op('high\x80\xff bytes')], guard=305+delta)
        put(f'd{dim}_ignored_header', [op('NSTEP 999', inline=' discarded inline name')], guard=307+delta)
        put(f'd{dim}_unit_reseed', [op('Other owner '+tag, 5), op('Before cleanup '+tag)], prefix='UNIT 2\n', guard=309+delta)
        put(f'd{dim}_deactivate', [op('Changed separate owner', mid)], prefix=f'OBJTYP 1 0 0\nOBJTYP {mid} 0 0\n', guard=311+delta)
        put(f'd{dim}_same_dimension', [op('After mesh '+tag)], prefix=f'* DEFORM-{dim}D\nUNIT 1\n'+mesh, guard=313+delta)
        put(f'd{dim}_long_names', [op('L'*1022), op('M'*1023), op('Q'*1024)], guard=315+delta)
        put(f'd{dim}_long_header', [op('ORPHAN', inline=' '+'X'*1040)], guard=317+delta)
        put(f'd{dim}_unterminated', [op('Last line '+tag, mode='unterminated')])
        put(f'd{dim}_header_eof', [op(mode='header_eof')])
        put(f'd{dim}_name_eof', [op(mode='name_eof')])
        put(f'd{dim}_new_header_eof', [op(oid=6, mode='header_eof')])
        put(f'd{dim}_new_name_eof', [op(oid=7, mode='name_eof')])
        put(f'd{dim}_selector_failures', [op('discarded', oid=0), op('discarded', oid=-3), op(mode='selector_eof')])
        if a.cohort == 'prototype':
            break
    assert len(plan['inputs']) <= 32
    dest.write_text(json.dumps(plan, indent=2)+'\n', encoding='utf-8')
    print(json.dumps(dict(imports=len(plan['inputs']), plan=str(dest))))


if __name__ == '__main__':
    main()
