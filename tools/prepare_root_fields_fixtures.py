"""Source-planned nine root-field imports, including shared UNIT storage."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import struct

IDS = {'GEOERR':71,'PENINF':72,'UNTE2H':73,'TINTGF':74,'FRNNAM':76,
       'BLZMAN':77,'ENVMPR':81,'ENVMPT':82,'EHRATE':83}
INDICES = {72:4,73:5,74:6,77:7,81:8,82:9,83:10}


def raw(values):
    return b''.join(struct.pack('<d', v) for v in values).hex()


def number(bits):
    return struct.unpack('<d', struct.pack('<Q', bits))[0]


def unit_profile(selector):
    bits = ([0x3fbb645a1cac0831,0x3cedc71df2f4abbd,0x3e61204d7839eab6,0x3d4faa7ab552a552]
            if selector == 2 else [0x3ff0000000000000,0x3dcf2a6880c4f0aa,0x3e15a589e1f37f03,0x3d03d0dac864deb1])
    return [number(v) for v in bits]


def defaults(selector=0, name='Furnace model'):
    u = unit_profile(selector)
    return dict(values=[number(0x3eb0c6f7a0b5ed8d),number(0x3f1a36e2eb1c432d),0.,0.,
                        number(0x41cdcd6500000000),u[0],.75,u[1],u[2],u[3],0.],
                name=name, unit_selector=selector)


def wire(state):
    return dict(reals_hex=raw(state['values']),name=state['name'],unit_selector=state['unit_selector'])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cohort', choices=['training','evaluation'])
    args = parser.parse_args()
    evaluation = args.cohort == 'evaluation'
    root = Path('tests/fixtures/root_fields')/args.cohort
    root.mkdir(parents=True,exist_ok=True)
    plan_path = Path('analysis/root-fields-'+('evaluation-inputs' if evaluation else 'fixture-plan')+'.json')
    if plan_path.exists():
        raise SystemExit('Refusing to replace prepared plan')
    catalog = json.loads(Path('analysis/keyword-catalog.json').read_bytes())['rows']
    assert {r['name']:r['id'] for r in catalog if r['id'] in IDS.values()} == IDS
    state = defaults()
    dimension = 3
    plan = dict(id='root-fields-'+args.cohort,status='source_planned_before_original',ordered_imports=[],inputs=[])
    if evaluation:
        plan['chronology'] = 'Prepared after training PASS. Original captured before C++ execution; reversed dimension/keyword order, new values/names and unit transitions.'

    def call(k,text,values,status=0,payload=''):
        return dict(keyword=k,text=text,values=values,status=status,payload=payload)

    def put(name,calls=(),prefix='',suffix='',change=None,unit=None,nstep=None):
        nonlocal state,dimension
        if change is not None and change != dimension:
            state = defaults(state['unit_selector'],state['name'])
            dimension = change
        if unit in [1,2] and unit != state['unit_selector']:
            state['unit_selector'] = unit
            for index,value in zip([5,7,8,9],unit_profile(unit)):
                state['values'][index] = value
        data = prefix
        expected = []
        for c in calls:
            kid = IDS[c['keyword']]
            before = wire(state)
            data += c['keyword'] if c['text'] is None else c['keyword']+(' '+c['text'] if c['text'] else '')+'\n'
            data += c['payload']
            if kid == 76:
                if not c['status']:
                    state['name'] = c['values']
                before_value = before['name']
                after_value = state['name']
            else:
                if kid == 71:
                    before_value = [before['reals_hex'][i*16:(i+1)*16] for i in range(4)]
                    state['values'][:4] = c['values']
                else:
                    i = INDICES[kid]
                    before_value = [before['reals_hex'][i*16:(i+1)*16]]
                    state['values'][i] = c['values'][0]
                after_value = [raw([v]) for v in c['values']]
            expected.append(dict(id=kid,status=2 if c['status'] else 0,expected_before=before,
                                 expected_fields=wire(state),reader=dict(kind='name' if kid==76 else 'real4' if kid==71 else 'real',
                                 before=before_value,after=after_value,status=c['status'])))
        data = (data+suffix).encode('ascii')
        (root/name).write_bytes(data)
        item = dict(file=name,bytes=len(data),sha256=hashlib.sha256(data).hexdigest(),
                    expected_handlers=expected,expected_final=wire(state),
                    expected_wrapper_status=2 if any(h['status'] for h in expected) else 0)
        if nstep is not None:
            item['expected_nstep'] = nstep
        plan['ordered_imports'].append(name)
        plan['inputs'].append(item)

    numeric = [k for k in IDS if k != 'FRNNAM']
    if evaluation:
        numeric.reverse()
    shift = .375 if evaluation else 0.

    def seed():
        result = []
        for i,k in enumerate(numeric):
            vals = [v+shift for v in [1.25,-2.5,3.75,4.125]] if k=='GEOERR' else [i+.625+shift]
            result.append(call(k,','.join(map(str,vals)),vals))
        return result

    def failed(blank):
        return [call(k,'' if blank else 'invalid',[0.]*(4 if k=='GEOERR' else 1),int(blank)) for k in numeric]

    def names(tag):
        # Tabs are retained, while trailing spaces/CR/LF are removed. A name
        # longer than the63-byte destination still consumes its whole input line.
        values = ['  '+tag+' alpha  ',tag+'\t  ',' '*8,tag*80,tag+' suffix']
        return [call('FRNNAM','ignored inline text',v.split('\0')[0][:63].rstrip(' \r\n'),payload=v+'\n') for v in values]

    first,second = (2,3) if evaluation else (3,2)
    put('defaults.KEY',prefix='\n')
    put(f'seed{first}.KEY',seed(),prefix=f'* DEFORM-{first}D\n',change=first)
    put(f'partial{first}.KEY',[call('GEOERR','-0.0,2.625',[-0.,2.625,0.,0.])])
    put(f'matching{first}.KEY',failed(False))
    put(f'blank{first}.KEY',failed(True))
    put(f'restore{first}.KEY',seed())
    put(f'names{first}.KEY',names('Eval' if evaluation else 'Train'))
    put('nameFirstEOF.KEY',[call('FRNNAM',None,state['name'],1)])
    put('nameSecondEOF.KEY',[call('FRNNAM','',state['name'],1)])
    for k in numeric:
        put('eof'+k+'.KEY',[call(k,None,[0.]*(4 if k=='GEOERR' else 1),1)])
    put('unitChange.KEY',prefix='UNIT 2\n',unit=2)
    put('unitSame.KEY',seed(),suffix='UNIT 2\n',unit=2)
    put('unitInvalid.KEY',prefix='UNIT 0\n',unit=0)
    put('unitMetric.KEY',prefix='UNIT 1\n',unit=1)
    put(f'dimension{second}.KEY',prefix=f'* DEFORM-{second}D\n',change=second)
    put(f'seed{second}.KEY',seed())
    put(f'partial{second}.KEY',[call('GEOERR','7.125, -0.0, 8.375',[7.125,-0.,8.375,0.])])
    put(f'matching{second}.KEY',failed(False))
    put(f'blank{second}.KEY',failed(True))
    put(f'names{second}.KEY',names('B' if evaluation else 'A'))
    mesh = ('RZ 1 4\n1 0 0 0\n2 1 0 0\n3 0 1 0\n4 0 0 1\nELMCON 1 1 4\n1 1 2 3 4\n' if second==3 else
            'RZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1\n1 1 2 3 4\n')
    put('mesh.KEY',seed(),prefix=mesh)
    put('otherDimension.KEY',seed(),prefix=f'* DEFORM-{first}D\n',change=first)
    put('guard.KEY',[call('GEOERR','1,2,3,4 NSTEP 99',[1.,2.,3.,4.]),
                     call('PENINF','2.5 NSTEP 98',[2.5]),
                     call('FRNNAM','NSTEP 97','guard',payload='guard\n')],suffix='NSTEP 37\n',nstep=37)
    put('unitSameAfterSeed.KEY',seed(),suffix='UNIT 1\n',unit=1)
    put('unitFinalChange.KEY',prefix='UNIT 2\n',unit=2)
    assert len(plan['inputs'])==32,len(plan['inputs'])
    plan_path.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8',newline='\n')
    print(json.dumps(dict(cohort=args.cohort,imports=len(plan['inputs']),plan=str(plan_path))))


if __name__=='__main__':
    main()
