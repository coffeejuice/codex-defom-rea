"""Prepare source-planned MOVCTL prototype inputs; wider cohorts follow validation."""
import argparse
import hashlib
import json
from pathlib import Path
import struct


def hx(values):
    return b''.join(struct.pack('<d', float(v)) for v in values).hex()


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('cohort', choices=['prototype', 'training', 'evaluation'])
    ap.add_argument('--dimension', type=int, choices=[2, 3])
    a = ap.parse_args()
    if a.cohort != 'prototype':
        if a.dimension is None:
            ap.error('wide cohorts require --dimension')
        return prepare_wide(a.cohort, a.dimension)
    dest = Path('analysis/movctl-prototype-inputs.json')
    if dest.exists():
        raise SystemExit('Refusing to replace prepared inputs')
    base = Path('tests/fixtures/movctl/prototype')
    base.mkdir(parents=True, exist_ok=True)
    plan = dict(status='source_planned_before_native', cohort=a.cohort,
                ordered_imports=[], inputs=[], qualification='Initial all-mode prototype only. Owned2D/3D object1 and finite bounded table extents. Full nested-selector/failure/lifecycle training,independent evaluation and full regression remain pending. Native/C++ stores compare exact bits;source-only trigonometric semantic checks allow4ULP relative to independent libm,not a relaxation of native/C++ equality.')

    def put(label, text, handlers=()):
        name = f'{len(plan["inputs"]):02d}_{label}.KEY'
        data = text.encode('ascii')
        (base / name).write_bytes(data)
        plan['ordered_imports'].append(name)
        plan['inputs'].append(dict(file=name, sha256=hashlib.sha256(data).hexdigest(),
                                   expected_handlers=list(handlers), expected_wrapper_status=0))

    for dim in [3, 2]:
        put(f'd{dim}_reset', f'* DEFORM-{dim}D\nUNIT 2\nUNIT 1\nOBJTYP 1 2 0\nOBJNAM 1\nIndependent object\nPRSNAM 1\nIndependent motion label\n')
        text = ''
        cases = []

        def op(mode, values, extra=None, tokens=None, slot=None, selector=None,
               rows=None, width=1, payload=None, expected_reals=None,
               expected_integers=None, constant=None, direct_type=None):
            nonlocal text
            h = dict(id=151, object_id=1, mode=mode, status=0,
                     headers=[dict(kind='header', selector=mode, status=0,
                                   values_hex=hx(values + [0]*(7-len(values))))],
                     resets=[], payloads=[], math=[], real_writes=expected_reals or {},
                     integer_writes=expected_integers or {})
            h['integer_writes']['0'] = mode
            text += 'MOVCTL 1 ' + str(mode) + ' ' + ' '.join(str(v) for v in values) + '\n'
            if mode not in [6, 11]:
                start = 1 if mode in [0, 1, 7, 8] else 0
                if dim == 2:
                    h['math'] = ['cos', 'sin']
                    h['angle'] = values[start]
                    h['cleanup'] = True
                else:
                    h['real_writes'].update({str(i): values[start+i] for i in range(3)})
            if extra is not None:
                #Mode6/type2 has a live triple followed by a live six-value line.
                groups = extra if isinstance(extra[0], list) else [extra]
                for group in groups:
                    kind = {3:'triple', 4:'quad', 6:'six'}[len(group)]
                    text += ' '.join(str(v) for v in group) + '\n'
                    h['headers'].append(dict(kind=kind, status=0, values_hex=hx(group)))
            if tokens is not None:
                text += tokens + '\n'
                words = tokens.split()[:7]
                h['headers'].append(dict(kind='tokens', status=0, tokens=words, token_count=len(words)))
            if slot is not None:
                h['resets'] = [dict(kind='reset-direct' if direct_type is not None else 'reset-preserve', slot=slot, type=direct_type or 0)]
                table = dict(slot=slot, type=direct_type or 0)
                if constant is not None:
                    table.update(rank=0, constant_hex=hx([constant]))
                if rows is not None:
                    table.update(rank=1, selector=selector, rows=rows, width=width)
                    h['payloads'] = [dict(slot=slot, status=0)]
                    text += ''.join(' '.join(str(v) for v in row)+'\n' for row in payload)
                    table['axes_hex'] = [hx([row[0] for row in payload]), '', '']
                    table['dependent_hex'] = hx([v for row in payload for v in row[1:]])
                h['table'] = table
            cases.append(h)

        op(0, [0, 30, 5] if dim == 2 else [0, 2, 3, 4, 5], slot=0, constant=5)
        op(1, [0, 60, 6] if dim == 2 else [0, 3, 4, 5, 6], slot=1, constant=6)
        op(2, [45, 5, 6, 7, 8, 0, 0] if dim == 2 else [2, 3, 4],
           extra=None if dim == 2 else [5, 6, 7, 8, 0, 0], slot=2, constant=0,
           expected_reals={'7':7, '8':5, '9':6}, expected_integers={'2':8})
        op(3, [90, 6, 7, 8, 9, 0, 0] if dim == 2 else [3, 4, 5],
           extra=None if dim == 2 else [6, 7, 8, 9, 0, 0], slot=3, constant=0,
           expected_reals={str(i+10):v for i,v in enumerate([6,7,8,9])})
        op(4, [123.25, 7, 8, 9, 0] if dim == 2 else [4, 5, 6],
           extra=None if dim == 2 else [7, 8, 9, 0], slot=4, constant=0,
           expected_reals={'14':7, '15':8, '16':9})
        op(5, [-45] if dim == 2 else [5, 6, 7], tokens='0 8.5 9.5 10.5 11.5 12 13', slot=5, constant=8.5,
           expected_reals={'17':9.5,'18':10.5,'19':11.5},expected_integers={'3':12,'4':13})
        width = 2 if dim == 2 else 4
        op(6, [1,14,1] if dim == 2 else [1,1,14,1], slot=6, direct_type=0x79 if dim == 2 else 0x7a,
           selector=1, rows=1, width=width, payload=[[1.5]+[i+2.5 for i in range(width)]], expected_integers={'6':14,'7':0})
        op(7, [0, 180, 10, 11, 12] if dim == 2 else [0,7,8,9,10,11,12], slot=7, constant=10,expected_reals={'29':11,'30':12})
        op(8, [0, 270, 11, 12] if dim == 2 else [0,8,9,10,11,12], slot=8, constant=11,expected_reals={'29':12})
        op(9, [360,12] if dim == 2 else [9,10,11,12],expected_reals={'29':12})
        op(10, [0.125,13,14] if dim == 2 else [10,11,12,13,14],expected_reals={'29':13,'31':14})
        op(11, [11,12,13],expected_reals={str(i+4):v for i,v in enumerate([11,12,13][:2 if dim == 2 else 3])})
        put(f'd{dim}_all_modes', text+'NSTEP 501\n', cases)
        text, cases = '', []
        for mode, sub, slot, selector in [(0,1,0,2),(0,2,0,1),(1,4,1,5),(7,2,7,1),(8,1,8,2)]:
            op(mode, [sub,33,2,17,18] if dim == 2 else [sub,2,3,4,2,17,18], slot=slot, selector=selector, rows=2,
               payload=[[1.25,4.25],[2.25,5.25]])
        put(f'd{dim}_curves', text+'NSTEP 503\n', cases)
        text, cases = '', []
        if dim == 2:
            for angle in [0.,1e-8,.125,30.,45.,60.,90.,123.25,-45.,180.,270.,360.,1e6,1e9,1e20,1e100,1e300]:
                op(9,[angle,21.25],expected_reals={'29':21.25})
        else:
            op(6,[2,2,19,2],extra=[[1,2,3],[4,5,6,7,8,9]],slot=6,direct_type=0x7b,
               selector=26,rows=2,width=3,payload=[[1.25,2.25,3.25,4.25],[5.25,6.25,7.25,8.25]],
               expected_reals={str(i+20):i+1 for i in range(9)},expected_integers={'5':2,'6':19})
        put(f'd{dim}_math_and_type2', text+'NSTEP 505\n', cases)
    dest.write_text(json.dumps(plan,indent=2)+'\n')
    print(json.dumps(dict(imports=len(plan['inputs']),handlers=sum(len(v['expected_handlers'])for v in plan['inputs']),plan=str(dest))))


def prepare_wide(cohort, dim):
    """Source-planned branch witnesses; no replacement execution seeds data."""
    evaluation = cohort == 'evaluation'
    delta = 2 if evaluation else 1
    tag = f'{cohort}-d{dim}'
    dest = Path('analysis/movctl-'+tag+'-inputs.json')
    if dest.exists():
        raise SystemExit('Refusing to replace prepared inputs')
    base = Path('tests/fixtures/movctl')/tag
    base.mkdir(parents=True, exist_ok=True)
    plan = dict(status='source_planned_before_native', cohort=cohort, dimension=dim,
                ordered_imports=[], inputs=[], qualification='Complete outer MOVCTL151 branch families within owned2D/3D wrappers,finite defined signed32 conversions and bounded tables. All twelve modes,nested selectors,positive stores,stale stores,table-buffer cleanup,reader/payload EOF and failure ordering covered by source witnesses. Native/C++ bits remain exact;source-only trig semantic check4ULP is independent. Allocation failure,nonfinite/overflow/INT_MIN,shared/native ABI,all rounding boundaries,scientific consumers and full application remain pending.')
    if evaluation:
        plan['chronology'] = 'Prepared after both training profiles PASS;original executes before frozen C++ with changed IDs/data and reversed selected orders.'
    text, cases = '', []

    def put(label, guard=True):
        nonlocal text, cases
        if guard:
            text += f'NSTEP {601+len(plan["inputs"])*2+delta}\n'
        name = f'{len(plan["inputs"]):02d}_{label}.KEY'
        data = text.encode('ascii')
        (base/name).write_bytes(data)
        plan['ordered_imports'].append(name)
        plan['inputs'].append(dict(file=name,sha256=hashlib.sha256(data).hexdigest(),
                                   expected_handlers=cases,
                                   expected_wrapper_status=next((h['status']for h in reversed(cases)if h['status']),0)))
        text, cases = '', []

    def emit(mode, values=(), *, oid=1, extra=(), tokens=None, slot=None,
             constant=None, selector=None, rows=None, width=1, body=None,
             direct_type=None, status=0, early=False, real=None, integers=None,
             token_status=0, header_status=0, header_override=None):
        nonlocal text
        values = list(values)
        h = dict(id=151, object_id=oid, mode=mode, status=status,
                 headers=[dict(kind='header',selector=mode if header_status==0 else 0,
                               status=header_status,values_hex=hx(values+[0]*(7-len(values))))],
                 resets=[],payloads=[],math=[],real_writes=real or {},integer_writes=integers or {})
        header = str(mode)+' '+ ' '.join(str(v)for v in values)
        text += 'MOVCTL '+str(oid)+((' '+header+'\n')if header_override is None else header_override)
        if header_status:
            h['preserve_all_motion'] = True
            cases.append(h)
            return h
        if 0<=mode<=11:
            h['integer_writes']['0'] = mode
        if mode in [0,1,2,3,4,5,7,8,9,10]:
            start = 1 if mode in [0,1,7,8]else 0
            if dim==2:
                h.update(math=['cos','sin'],angle=values[start],cleanup=not early)
            else:
                h['real_writes'].update({str(i):values[start+i]for i in range(3)})
        for kind, vals, result in extra:
            if vals is not None:
                text += ' '.join(str(v)for v in vals)+'\n'
            count = dict(triple=3,quad=4,six=6)[kind]
            h['headers'].append(dict(kind=kind,status=result,values_hex=hx(vals if vals is not None else [0]*count)))
        if tokens is not None or token_status:
            import re
            if tokens is not None:
                text += tokens+'\n'
            words = [v for v in re.split('[ ,\n\t\r]+',tokens or '')if v][:7]or ['']
            h['headers'].append(dict(kind='tokens',status=token_status,token_count=None if token_status else len(words),tokens=[]if token_status else words))
        if slot is not None:
            h['resets']=[dict(kind='reset-direct'if direct_type is not None else'reset-preserve',slot=slot,type=direct_type or 0)]
            table=dict(slot=slot,type=direct_type or 0)
            if constant is not None:
                table.update(rank=0,constant_hex=hx([constant]))
            if rows is not None:
                table.update(rank=1,selector=selector,rows=max(0,rows)if body is not None else rows,width=width)
                if body is not None:
                    h['payloads']=[dict(slot=slot,status=status)]
                    text += body
                    if status==0:
                        numbers=[float(v)for v in body.split()]
                        count=max(0,rows)
                        table['axes_hex']=[hx(numbers[::width+1]),'','']
                        table['dependent_hex']=hx([numbers[i*(width+1)+j+1]for i in range(count)for j in range(width)])
            h['table']=table
        cases.append(h)
        return h

    def payload(rows,width=1):
        return ''.join(' '.join(str(1.25+delta+r*5+c)for c in range(width+1))+'\n'for r in range(max(0,rows)))

    def scalar(mode,sub,value=7.25,*,rows=None,oid=1,body=None,status=0):
        slot={0:0,1:1,7:7,8:8}[mode]
        values=[sub,90,value,17+delta,18+delta]if dim==2 else[sub,2+delta,3+delta,4+delta,value,17+delta,18+delta]
        ri={};ii={}
        if mode in [0,7,8]:
            ii.update({'9':int(sub<0),'10':-sub if sub<0 else 0})
        if mode==1:
            ii.update({'9':int(sub<0),'10':-sub if sub<0 else 0,'1':sub-1000 if 1001<=sub<=1999 else 0})
            ri['3']=value if 1001<=sub<=1999 else 0.
        if mode in [7,8]:ri['29']=17+delta
        if mode==7:ri['30']=18+delta
        if rows is None:
            return emit(mode,values,oid=oid,slot=slot,constant= value if sub==0 else 0.,real=ri,integers=ii)
        sel=2 if sub==1 else 1 if sub==2 else 5
        return emit(mode,values,oid=oid,slot=slot,selector=sel,rows=rows,body=payload(rows)if body is None else body,status=status,real=ri,integers=ii)

    def mode2(sub,rows,*,count=2,body=None,status=0):
        data=[21+delta,count,23+delta,24,sub,rows]
        h=emit(2,[90]+data if dim==2 else[2+delta,3+delta,4+delta],
               extra=()if dim==2 else[('six',data,0)],slot=2,
               constant=0. if sub==rows==0 else None,
               selector=None if sub==rows==0 else 5 if sub==4 else 0,
               rows=None if sub==rows==0 else rows if rows else count,
               body=None if sub==rows==0 else payload(rows if rows else count)if body is None else body,
               status=status,early=bool(status),real={'7':23+delta,'8':21+delta},integers={'2':24})
        if sub==0 or rows!=0:h['real_writes']['9']=count
        else:h['preserve_reals']=[9]

    def mode3(sub,rows,*,body=None,status=0):
        data=[31+delta,32+delta,33+delta,34+delta,sub,rows]
        emit(3,[90]+data if dim==2 else[3+delta,4+delta,5+delta],extra=()if dim==2 else[('six',data,0)],slot=3,
             constant=0. if sub!=4 else None,selector=5 if sub==4 else None,rows=rows if sub==4 else None,
             body=(payload(rows)if body is None else body)if sub==4 else None,status=status,early=bool(status),real={str(i+10):v for i,v in enumerate(data[:4])})

    def mode4(rows,*,body=None,status=0):
        data=[41+delta,42+delta,43+delta,rows]
        h=emit(4,[90]+data if dim==2 else[4+delta,5+delta,6+delta],extra=()if dim==2 else[('quad',data,0)],slot=4,
               constant=0. if int(rows)<1 else None,selector=10 if int(rows)>0 else None,rows=int(rows)if int(rows)>0 else None,
               body=(payload(int(rows))if body is None else body)if int(rows)>0 else None,status=status,real={str(i+14):v for i,v in enumerate(data[:3])})
        h['flag2490']=int(int(rows)>0)

    def mode6(kind,sub,rows,*,body=None,status=0,extras=None):
        width=2 if dim==2 else 4 if kind==1 else 3
        typeid=0x79 if dim==2 else 0x7a if kind==1 else 0x7b
        extra=[]
        if dim==3 and kind==2 and sub>=0:
            extra=extras if extras is not None else[('triple',[51+delta,52+delta,53+delta],0),('six',[54+delta,55+delta,56+delta,57+delta,58+delta,59+delta],0)]
        ii={'6':29+delta,'7':int(sub<0)}
        if dim==3:ii['5']=kind
        if sub<0:ii['8']=-sub
        ri={}
        if extra:
            #Explicit known live destinations,without depending on C++ state.
            vals=[]
            for reader,vs,_status in extra:vals+=vs if vs is not None else[0]*(3 if reader=='triple'else 6)
            ri={str(i+20):v for i,v in enumerate(vals)}
        h=emit(6,[sub,29+delta,rows]if dim==2 else[kind,sub,29+delta,rows],extra=extra,slot=6,direct_type=typeid,
               selector=26 if sub==2 else 1,rows=rows,width=width,
               body=None if sub<0 else payload(rows,width)if body is None else body,status=status,real=ri,integers=ii)
        if sub>=0:h['preserve_integers']=[8]
        return h

    #Each profile starts from a fresh process/model and contains24 bounded files.
    text=f'* DEFORM-{dim}D\nUNIT 2\nUNIT 1\nOBJTYP 1 2 0\nOBJNAM 1\nSeparate identity\nPRSNAM 1\nMotion label {cohort}\n'
    put('reset')
    modes=list(range(12));modes=modes[::-1]if evaluation else modes
    for mode in modes:
        if mode in [0,1,7,8]:scalar(mode,0,7.25+mode+delta)
        elif mode==2:mode2(0,0,count=22+delta)
        elif mode==3:mode3(0,0)
        elif mode==4:mode4(0)
        elif mode==5:emit(5,[33]if dim==2 else[5,6,7],tokens=f'0 {8.5+delta} 9.5 10.5 11.5 12 13',slot=5,constant=8.5+delta)
        elif mode==6:mode6(1,1,2)
        elif mode in [9,10]:emit(mode,[123.25+delta,17.5+delta,19.5+delta]if dim==2 else[9+delta,10+delta,11+delta,17.5+delta,19.5+delta])
        else:emit(11,[11+delta,12+delta,13+delta])
    put('all_modes')
    for mode,sub in [(0,1),(0,2),(1,1),(1,2),(1,4),(7,1),(7,2),(8,1),(8,2)]:scalar(mode,sub,2,rows=2)
    put('scalar_curves')
    for mode in [0,1,7,8]:
        for sub in [-7-delta,3]:scalar(mode,sub,9.25+delta)
    for sub in [1000,1001,1999,2000,4]:
        if sub==4:scalar(1,4,2,rows=2)
        else:scalar(1,sub,11.25+delta)
    put('external_and_coupled')
    for sub,rows in [(0,0),(4,2),(1,2),(4,0),(3,0),(0,-1)]:mode2(sub,rows,count=2)
    put('mode2_branches')
    for sub,rows in [(0,0),(4,2),(5,2)]:mode3(sub,rows)
    put('mode3_branches')
    for rows in [-1,0,2,2.8]:mode4(rows)
    put('mode4_branches')
    for tokens in ['0 7.5 1 2 3 4 5','0','',', ,','1 2 4 5 6 7 8 ignored ignored','010 2 7 8 9 10 11']:
        curve=tokens.startswith(('1 ','010'))
        emit(5,[33]if dim==2 else[5,6,7],tokens=tokens,slot=5,
             constant=None if curve else 7.5 if tokens.startswith('0 7.5')else 0.,selector=19 if curve else None,
             rows=2 if curve else None,body=payload(2)if curve else None)
    put('mode5_tokens')
    for kind,sub,rows in [(1,1,2),(2,2,2),(3,3,2),(2,-9-delta,2),(1,1,0),(1,2,-1)]:mode6(kind,sub,rows)
    put('mode6_branches')
    angles=[0.,1e-8,.125,30.,45.,60.,90.,123.25,-45.,180.,270.,360.,1e6,1e9,1e20,1e100,1e300]
    if evaluation:angles=angles[::-1]
    for angle in angles if dim==2 else[13.25,21.5]:
        emit(9,[angle,17.5+delta]if dim==2 else[angle,2+delta,3+delta,17.5+delta])
    put('math_and_tails')
    mid=4 if evaluation else 3
    text='UNIT 2\nOBJTYP 1 0 0\n'
    emit(9,[33,27.5+delta]if dim==2 else[1,2,3,27.5+delta],oid=mid)
    text+=f'* DEFORM-{dim}D\nUNIT 1\nOBJTYP 1 2 0\n'
    put('lifecycle')
    emit(0,[],oid=1,status=2,header_status=1,header_override='')
    put('existing_header_eof',False)
    if evaluation:
        text='MOVCTL';cases=[dict(id=151,object_id=None,mode=None,status=2,headers=[],resets=[],payloads=[],math=[],real_writes={},integer_writes={})]
    else:emit(0,[],oid=6,status=2,header_status=1,header_override='')
    put('new_header_or_selector_eof',False)
    if dim==3:
        mode=3 if evaluation else 2
        emit(mode,[1,2,3],extra=[('six',None,1)],slot=3 if mode==3 else 2,constant=0.,status=2,
             real={str(i+10):0. for i in range(4)}if mode==3 else{'7':0.,'8':0.,'9':0.},integers={}if mode==3 else{'2':0})
    else:emit(5,[90],token_status=1,status=2,early=True)
    put('continuation_eof',False)
    emit(5,[90]if dim==2 else[1,2,3],token_status=1,status=2,early=True)
    put('tokens_eof',False)
    h=mode6(2,1,0,body='',extras=[('triple',None,1),('six',None,1)])
    if dim==2:h['real_writes'].update({'0':0.,'1':1.})
    put('status_overwrite_eof',False)
    for mode in [0,1]:
        scalar(mode,-9-delta,9.5)
        scalar(mode,1,2,rows=2,body='1.25',status=2)
        put(f'mode{mode}_payload_eof',False)
    mode2(4,0,count=2,body='1.25',status=2)
    put('mode2_payload_eof',False)
    mode3(4,2,body='1.25',status=2)
    put('mode3_payload_eof',False)
    mode4(2,body='1.25',status=2)
    put('mode4_payload_eof',False)
    emit(5,[90]if dim==2 else[1,2,3],tokens='1 2 4 5 6 7 8',slot=5,selector=19,rows=2,body='1.25',status=2)
    put('mode5_payload_eof',False)
    mode6(2,2,2,body='1.25',status=2)
    put('mode6_payload_eof',False)
    text+='MOVCTL 0\nMOVCTL -3\n'
    for _ in range(2):cases.append(dict(id=151,object_id=None,mode=None,status=2,headers=[],resets=[],payloads=[],math=[],real_writes={},integer_writes={}))
    #Matching failures are defined for the motion header,not an uninitialized object selector.
    emit(0,[0,0,0,0]if dim==3 else[0,0],slot=0,constant=0.,header_override=' bad\n')
    cases[-1]['headers'][0]['values_hex']=hx([0]*7)
    emit(0,[],status=2,header_status=1,header_override='\n')
    scalar(8 if evaluation else 7,1,2,rows=2,body='1.25',status=2)
    put('matching_blank_and_trailing_payload_eof',False)
    assert len(plan['inputs'])==24,len(plan['inputs'])
    dest.write_text(json.dumps(plan,indent=2)+'\n')
    print(json.dumps(dict(imports=len(plan['inputs']),handlers=sum(len(i['expected_handlers'])for i in plan['inputs']),plan=str(dest))))


if __name__ == '__main__':
    main()
