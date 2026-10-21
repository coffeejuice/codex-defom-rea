"""Prepare source-planned environmental cases without reading runtime results."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import struct

IDS={'ENVTMP':75,'CNVCOF':78,'ENVATM':79,'ACVCOF':80}
TYPES={75:2,78:1,79:30,80:29}


def hx(values):
    return b''.join(struct.pack('<d',v)for v in values).hex()


def table(kind,rank=0,selector=0,rows=0,width=1,constant=0.,axes=(),values=(),next_index=None):
    t=dict(metadata=[kind,rank,selector,0,0,rows,0,0,width,0,0,0],constant_hex=hx([constant]),
           axes_hex=[hx(axes),'',''],dependent_hex=hx(values))
    if next_index is None:t['preserved60_hex']='00'*8
    else:t['next_index']=next_index
    return t


def constructors(count):
    count=max(21,count);result=[]
    for i in range(count):
        special=i==10
        result.append(dict(table=table(142+i if i<21 else 163,1,int(special),int(special),
                         axes=[0.]if special else[],values=[0.]if special else[],
                         next_index=i+2 if i+1<count else 0),known_bytes_hex=''))
    return result


def imperial_defaults():
    #375e90 skips3769f0 when the process selector is2. All data below is from
    #the static layout/writes, never a captured model or replacement output.
    source=json.loads(Path('analysis/furnace-source-extraction.json').read_bytes())
    result=constructors(22)
    for i,p in enumerate(result):
        t=p['table'];row=source['layout_rows'][i]if i<20 else[0,1,0,0,0]
        t['metadata'][2]=1 if i==10 else 0;t['metadata'][5]=row[3];t['metadata'][8]=row[4]
        if i<10 or i==13:
            t['axes_hex']=[hx([0.]*row[3]),'',''];t['dependent_hex']=hx([0.]*(row[3]*row[4]))
    for parameter,row,column,bits in source['default_writes']:
        t=result[parameter-1]['table'];width=t['metadata'][8]
        if row>=max(1,t['metadata'][5])or column<0 or column>=max(1,width):continue
        data=bytearray.fromhex(t['dependent_hex']);offset=(row*width+column)*8
        if offset+8<=len(data):data[offset:offset+8]=struct.pack('<Q',bits)
        t['dependent_hex']=data.hex()
    return result


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cohort',choices=['training-a','training-b','evaluation-a','evaluation-b'])
    a=parser.parse_args();evaluation=a.cohort.startswith('evaluation');b=a.cohort.endswith('b')
    ordinal=['training-a','training-b','evaluation-a','evaluation-b'].index(a.cohort)
    base=Path('tests/fixtures/environment')/a.cohort;base.mkdir(parents=True,exist_ok=True)
    path=Path('analysis/environment-'+a.cohort+'-inputs.json')
    if path.exists():raise SystemExit('Refusing to replace prepared cohort')
    catalog=json.loads(Path('analysis/keyword-catalog.json').read_bytes())['rows']
    assert {r['name']:r['id']for r in catalog if r['id']in IDS.values()}==IDS
    dimension=2 if b else 3;unit=0;enabled=0;parameters=None
    constants=[20.,.02,1.69,.00025]
    #Use exact source bit values from unit_system's retained static contract.
    constants=[struct.unpack('<d',struct.pack('<Q',x))[0]for x in [0x4034000000000000,0x3f947ae147ae147b,0x3ffb0a3d70a3d70a,0x3f30624dd2f1a9fc]]
    tables=[table(0,constant=c)for c in constants]
    plan=dict(status='source_planned_before_original',cohort=a.cohort,ordered_imports=[],inputs=[],scope='Four environmental keywords and the complete furnace import loop; safe owned profiles, both dimensions, defined ASCII packed text.')
    if evaluation:plan['chronology']='Prepared after training PASS; original captures precede C++ on these changed values/orders/extents.'
    delta=ordinal*.375
    keys=list(IDS)
    if evaluation:keys.reverse()

    def environment():
        return dict(enabled=enabled,tables=copy.deepcopy(tables),parameters=copy.deepcopy(parameters))

    def rootcall(k,selector,value,header='auto',payload='',axes=(),values=(),status=0,payload_status=0):
        return dict(k=k,selector=selector,value=value,header=f'{selector} {value}'if header=='auto'else header,
                    payload=payload,axes=list(axes),values=list(values),status=status,payload_status=payload_status)

    def furnace(requested,entries):
        return dict(k='ENVTMP',selector=10,value=float(requested),header=f'10 {requested}',payload='',
                    entries=entries,status=0,requested=requested)

    def entry(index,rank=0,selector=1,real0=0.,real1=0.,payload='',axes=(),values=(),status=0,header_status=0,header=None,lines=None):
        return dict(integers=[index,rank,selector],reals=[real0,real1],payload=payload,axes=list(axes),values=list(values),status=status,
                    header_status=header_status,header=f'{index} {rank} {selector} {real0} {real1}\n'if header is None else header,lines=lines)

    def numeric_scalar(index):return entry(index,real0=index+.125+delta)

    def apply_entry(p,e,index):
        t=p['table'];rank=e['integers'][1];third=e['integers'][2];r0,r1=e['reals'];t['metadata'][1]=rank
        if index==21:
            if rank!=1:return
            rows=max(0,int(r0));width=max(1,int(r1));t['metadata'][2]=third;t['metadata'][5]=rows;t['metadata'][8]=width
            t['axes_hex']=[hx([0.]*rows),'',''];data=bytearray(rows*width*8);known=bytearray([1])*len(data);previous=None
            for row,line in enumerate(e['lines']or[]):
                if line['status']==0:previous=line['value']
                if previous is None:known[row*64:(row+1)*64]=b'\0'*64
                else:data[row*64:(row+1)*64]=previous.encode('ascii').ljust(64,b' ')
            t['dependent_hex']=data.hex();p['known_bytes_hex']=known.hex();return
        if rank==0:
            width=max(1,third);t['metadata'][8]=width;t['axes_hex']=['','','']
            t['dependent_hex']=hx(e['values']if width>1 else[])
            if width<2:t['constant_hex']=hx([r0])
        elif rank==1:
            rawrows=int(r0);rows=max(0,rawrows);width=max(1,0 if rawrows==0 else int(r1))
            t['metadata'][2]=third;t['metadata'][5]=rows;t['metadata'][8]=width
            if rawrows==0:t['constant_hex']=hx([r1])
            t['axes_hex']=[hx(e['axes']),'',''];t['dependent_hex']=hx(e['values'])

    def put(name,calls=(),prefix='',suffix='',switch=None,unit_change=None,nstep=None):
        nonlocal enabled,parameters,tables,dimension,unit
        if unit_change is not None and unit_change in [1,2]and unit_change!=unit:
            unit=unit_change
            bits=[0x4051000000000000,0x3ee025e7f1158172,0x3ffb0a3d70a3d70a,0x3ee4f8b588e368f1]if unit==2 else[0x4034000000000000,0x3f947ae147ae147b,0x3ffb0a3d70a3d70a,0x3f30624dd2f1a9fc]
            tables=[table(0,constant=struct.unpack('<d',struct.pack('<Q',v))[0])for v in bits]
        if switch is not None and switch!=dimension:
            dimension=switch;enabled=0;parameters=imperial_defaults()
        data=prefix;handlers=[]
        for c in calls:
            kid=IDS[c['k']];slot=0 if kid==75 else kid-77
            data+=c['k']if c['header'] is None else c['k']+(' '+c['header']if c['header']else'')+'\n'
            handler_status=2 if c['status']else 0;payload=False;inner=[]
            if not c['status']:
                tables[slot]=table(TYPES[kid]);selector=c['selector'];value=c['value']
                if kid==75:enabled=0
                if kid==75 and selector==10:
                    enabled=1;requested=c['requested'];parameters=constructors(requested)
                    for index,e in enumerate(c['entries'],1):
                        data+=e['header']+e['payload'];apply_entry(parameters[index-1],e,index)
                        inner.append(dict(kind='packed-text'if index==21 else'parameter',header=dict(integers=e['integers'],reals_hex=[hx([x])for x in e['reals']],status=e['header_status']),lines=e['lines']or[],status=e['status']))
                    assert len(c['entries'])==max(0,requested)
                else:
                    if selector==0:tables[slot]['constant_hex']=hx([value])
                    elif selector==1 or(kid==80 and selector in[2,3]):
                        axis=(3 if kid in[78,80]else 1)if selector==1 else 12 if selector==2 else 1
                        tables[slot]=table(TYPES[kid],1,axis,max(0,int(value)),axes=c['axes'],values=c['values'])
                    payload=kid!=75 or selector in[0,1]
                    if payload:handler_status=2 if c['payload_status']else 0
                    data+=c['payload']
            else:data+=c['payload']
            handlers.append(dict(id=kid,status=handler_status,header=dict(kind='header',integer=c['selector'],real_hex=hx([c['value']]),status=c['status']),payload=payload,inner=inner,expected_environment=environment()))
        data=(data+suffix).encode('ascii');(base/name).write_bytes(data)
        item=dict(file=name,sha256=hashlib.sha256(data).hexdigest(),expected_handlers=handlers,expected_environment=environment(),
                  expected_wrapper_status=next((h['status']for h in reversed(handlers)if h['status']),0))
        if nstep is not None:item['expected_nstep']=nstep
        plan['ordered_imports'].append(name);plan['inputs'].append(item)

    put('initial.KEY',prefix=f'* DEFORM-{dimension}D\n')
    put('constants.KEY',[rootcall(k,0,i+1.125+delta)for i,k in enumerate(keys)],prefix='FRNNAM\n'+a.cohort+' furnace\n')
    matching_key=list(IDS)[ordinal]
    curves=[]
    for k in keys:
        matching=k==matching_key;ax=[0.,0.]if matching else[1.25+delta,2.5+delta];vals=[0.,0.]if matching else[3.75+delta,-4.125-delta]
        payload='bad\n'if matching else f'{ax[0]} {vals[0]}\n{ax[1]} {vals[1]} NSTEP 99\n'
        curves.append(rootcall(k,1,2.75,payload=payload,axes=ax,values=vals))
    put('curves.KEY',curves,suffix='NSTEP 17\n',nstep=17)
    eof_key=list(IDS)[ordinal]
    zero=[rootcall(k,1,0. if i%2 else-2.75)for i,k in enumerate(keys)]
    zero.append(rootcall(eof_key,1,2.,payload='6.25 7.5',axes=[6.25,0.],values=[7.5,0.],payload_status=1))
    put('zeroAndPayloadEOF.KEY',zero)
    put('ignoredModes.KEY',[rootcall(k,99,-3.5)for k in keys])
    put('matchingHeaders.KEY',[rootcall(k,0,0.,header='bad')for k in keys])
    put('blankHeaders.KEY',[rootcall(k,0,0.,header='',status=1)for k in keys])
    partial=[rootcall(k,1,0.,header='1')for k in keys]
    partial.append(rootcall(eof_key,0,0.,header=None,status=1));put('partialAndHeaderEOF.KEY',partial)
    put('furnaceEmpty.KEY',[furnace(-1 if b else 0,[])])
    small=[entry(1,0,3,payload=f'1.125 -0.0 {2.375+delta}\n',values=[1.125,-0.,2.375+delta])]
    if b:small.append(entry(2,1,3,2.,2.,payload='1 2 3\n4 5 6\n',axes=[1.,4.],values=[2.,3.,5.,6.]))
    put('furnaceSmall.KEY',[furnace(len(small),small)])
    names=[a.cohort+' first',a.cohort+' second\t']
    text=entry(21,1,0,2.,8.,payload='\n'.join(names)+'\n',lines=[dict(status=0,value=v)for v in names])
    put('furnaceText.KEY',[furnace(21,[numeric_scalar(i)for i in range(1,21)]+[text])])
    mixed=[entry(1,1,1,2.,2.,payload='1 2 3\n4 5 6\n',axes=[1.,4.],values=[2.,3.,5.,6.]),
           entry(2,0,2,payload='-0.0 8.625\n',values=[-0.,8.625]),entry(3,2,0,status=1),
           entry(4,1,0,0.,9.375),entry(5,1,3,1.75,2.5,payload='2.25 3.5 4.75\n',axes=[2.25],values=[3.5,4.75]),
           entry(6,1,0,-2.,-1.)]+[numeric_scalar(i)for i in range(7,21)]
    width=9 if b else 8
    mixed.append(entry(21,1,0,2.,float(width),payload='Long '+a.cohort+'\n\n',lines=[dict(status=0,value='Long '+a.cohort),dict(status=0,value='')]))
    mixed.append(entry(22,0,2,payload='11.125 12.375\n',values=[11.125,12.375]))
    if b:mixed.append(entry(23,1,1,1.,1.,payload='13.625 14.875\n',axes=[13.625],values=[14.875]))
    put('furnaceExtended.KEY',[furnace(len(mixed),mixed)])
    failed=[entry(1,1,1,2.,2.,payload='1.25 2.5 3.75',axes=[1.25,0.],values=[2.5,3.75,0.,0.],status=1)]
    failed +=[entry(0,header='',rank=0,selector=0,header_status=1)for _ in range(21)]
    put('furnaceNumericEOF.KEY',[furnace(22,failed)])
    textlast=a.cohort+' retained'
    put('furnaceTextEOF.KEY',[furnace(21,[numeric_scalar(i)for i in range(1,21)]+[entry(21,1,0,2.,8.,payload=textlast+'\n',lines=[dict(status=0,value=textlast),dict(status=1,value=textlast)])])])
    put('unitRetain.KEY',prefix='UNIT 2\n',unit_change=2)
    newdim=2 if dimension==3 else 3
    mesh=('RZ 1 4\n1 0 0 0\n2 1 0 0\n3 0 1 0\n4 0 0 1\nELMCON 1 1 4\n1 1 2 3 4\n'if newdim==3 else
          'RZ 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\nELMCON 1 1\n1 1 2 3 4\n')
    calls=[rootcall(k,2 if k=='ACVCOF'and not b else 3 if k=='ACVCOF'else 1,1.5,payload=f'5.25 {6.625+delta}\n',axes=[5.25],values=[6.625+delta])for k in keys]
    put('dimensionMesh.KEY',calls,prefix=f'* DEFORM-{newdim}D\n'+mesh,suffix='UNIT 2\n',switch=newdim)
    assert len(plan['inputs'])==16,len(plan['inputs'])
    path.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8',newline='\n');print(json.dumps(dict(cohort=a.cohort,imports=len(plan['inputs']))))


if __name__=='__main__':main()
