"""Independent original-only audit of relation property keywords135..143.

Expectations come from retained native contracts, fixture bytes and passive
events. This module never executes or imports a candidate/comparator.
"""
import argparse
from collections import Counter
from copy import deepcopy
import json
import math
from pathlib import Path
import struct

import verify_property_table_observations as tables
import verify_relation_observations as relations
import verify_material_column_observations as base
import verify_material_reference_observations as refs
import verify_custom_field_observations as custom
from verify_boundary_sort_observations import ROOT, binding, check, read
from verify_mesh_array_observations import descriptor_map

PROBE=ROOT/'tools/observe_relation_property_import.js'
CONTRACT=ROOT/'analysis/relation-property-static-contract.json'
STATIC=read(CONTRACT)
IDS=tuple(range(135,144))
raw,one,lex=base.raw,base.one,tables.lex
COUNTER,GATE=relations.COUNTER,relations.GATE
CALLS=read(ROOT/'analysis/relation-property-byte-audit.json')['cases']
FORMATS={'int1':[True],'int2':[True]*2,'int3':[True]*3,'int4':[True]*4,
    'real2':[False]*2,'real4':[False]*4,'real5':[False]*5,
    'coord2':[True,False,False],'int2real2':[True,True,False,False]}


def provenance(evidence,run,manifest,freeze,input_freeze,directory,manifest_path,freeze_path):
    base.PROBE=PROBE;base.CONTRACT=CONTRACT
    for key in ('native_source_bindings','source_bindings'):
        for b in manifest.get(key,[]):check(binding(ROOT/b['path'])['sha256']==b['sha256'],'static source binding')
    projected=deepcopy(manifest);projected['probe'].pop('evidence_id',None)
    return base.provenance(evidence,run,projected,freeze,input_freeze,directory,manifest_path,freeze_path)


def total(t):
    if t['rank_0x04']==0:return max(0,t['values_per_point_0x40']) if t['values_per_point_0x40']>1 else 0
    return max(0,math.prod(t['counts_0x1c'][:t['rank_0x04']])*t['values_per_point_0x40'])


def table_shape(t):
    check(int(t['address'],16)>0 and t['rank_0x04']in range(4),'table address/rank')
    check(-4<=t['values_per_point_0x40']<=4 and t['pointers']['cache_0x68']=='0x0','table bounded width/null cache')
    check(len(t['counts_0x1c'])==len(t['selectors_0x10'])==3 and all(type(n)is int and -4<=n<=3 for n in t['counts_0x1c']),'bounded axis metadata')
    check(t['flag_0x50']==t['preserved_0x54']==t['mode_0x58']==0 and t['preserved_0x60_hex']=='00'*8,'selected preserved table metadata')
    tables.floats([t['constant_0x08']],t['raw_hex']['constant'])
    check(len(t['axes'])==len(t['raw_hex']['axes'])==len(t['pointers']['axes'])==len(t['physical_capacities']['axes'])==3,'three physical axes')
    for values,h,p,n in zip(t['axes']+[t['dependent']],t['raw_hex']['axes']+[t['raw_hex']['dependent']],t['pointers']['axes']+[t['pointers']['dependent']],t['physical_capacities']['axes']+[t['physical_capacities']['dependent']]):
        tables.floats(values,h);check(len(values)==n and 0<=n<=32 and (p=='0x0')==(n==0),'physical allocation length/pointer')
    ps=[p for p in t['pointers']['axes']+[t['pointers']['dependent']]if p!='0x0']
    check(len(ps)==len(set(ps)),'owned table allocations distinct')


def equal(a,b,label):
    table_shape(a);table_shape(b);check(a==b,label)


def profile(t,type_id):
    r=tables.empty_table(t,type_id)
    p=STATIC['type_profiles'][str(type_id)]
    for x in p['ordered_int32_overrides']:
        off=int(x['offset'],16);v=x['int32_value']
        if off==4:r['rank_0x04']=v
        elif off==0x40:r['values_per_point_0x40']=v
        elif off in (0x10,0x14,0x18):r['selectors_0x10'][(off-0x10)//4]=v
        elif off in (8,12):
            bits=bytearray.fromhex(r['raw_hex']['constant']);bits[off-8:off-4]=struct.pack('<i',v)
            r['raw_hex']['constant']=bits.hex();r['constant_0x08']=struct.unpack('<d',bits)[0]
        else:raise ValueError('unreviewed constructor override')
    return r


def allocation(expected,observed):
    w=deepcopy(expected);w['counts_0x1c']=[max(0,n)for n in w['counts_0x1c']];w['values_per_point_0x40']=max(1,w['values_per_point_0x40'])
    w['axes']=[[0.]*n if i<w['rank_0x04']else[]for i,n in enumerate(w['counts_0x1c'])];w['dependent']=[0.]*total(w)
    w['raw_hex']['axes']=[raw(x)for x in w['axes']];w['raw_hex']['dependent']=raw(w['dependent'])
    w['physical_capacities']={'axes':[len(x)for x in w['axes']],'dependent':len(w['dependent'])};w['pointers']=deepcopy(observed['pointers'])
    equal(observed,w,'allocation normalizes counts/width and zeroes complete physical arrays');return w


def continuation(id,callee,rva):
    c=next(c for c in CALLS if c['id']==id)
    check(any(int(x['callee'],16)-0x140000000==callee and int(x['return'],16)-0x140000000==rva for x in c['calls']),'native call/return ABI')


def record(r):
    check(int(r['address'],16)>0 and int(r['owner'],16)>0,'live relation identity')
    for k in ('key0','key1','mode_0x08','field_0x10','field_0x14'):
        check(type(r[k])is int and -2**31<=r[k]<2**31,'signed relation field')
    check(r['active_0x0c']in(0,1),'active flag')
    name=bytes.fromhex(r['name_hex']);check(len(name)<=80 and b'\0'not in name and r['name_terminator_hex']=='00','bounded relation name')
    check(r['name_prefix_hex']==(name+b'\0')[:2].hex() if name else r['name_prefix_hex'].startswith('00'),'initialized name prefix')
    offsets={x['offset'] for x in STATIC['record_layout']['tables']}
    check(set(r['property_tables'])==offsets,'twelve relation tables')
    for off,t in r['property_tables'].items():table_shape(t);check(int(t['address'],16)==int(r['address'],16)+int(off,16),'relation table address offset')
    ints={k:v for k,v in STATIC['record_layout']['initialized_int32'].items()if int(k,16)>=0x6c};ds=STATIC['record_layout']['initialized_double_bits']
    check(set(r['scalar_fields'])==set(ints)|set(ds),'all initialized scalar fields')
    for k,x in r['scalar_fields'].items():
        if k in ints:check(x['type']=='int32'and type(x['value'])is int and -2**31<=x['value']<2**31,'int32 scalar')
        else:check(x['type']=='binary64','double scalar type');tables.floats([x['value']],x['raw_hex'])


def defaults(r):
    record(r)
    check(all(r[k]==0 for k in('key0','key1','mode_0x08','active_0x0c','field_0x10','field_0x14'))and r['name_hex']=='20','relation constructor fields/name')
    for off,x in r['scalar_fields'].items():
        if x['type']=='int32':check(x['value']==STATIC['record_layout']['initialized_int32'][off],'constructor int32')
        else:check(x['raw_hex']==STATIC['record_layout']['initialized_double_bits'][off]['little_endian_hex'],'constructor binary64 bits')
    for x in STATIC['record_layout']['tables']:
        t=r['property_tables'][x['offset']];equal(t,profile(t,x['initial_type']),'constructor exact table profile')


def relation_list(s):
    rs=s['records'];check(s['count']==len(rs)<=3,'relation count/list')
    check(len({r['address']for r in rs})==len(rs),'distinct live relation records')
    check(s['head']==(rs[0]['address']if rs else'0x0')and s['tail']==(rs[-1]['address']if rs else'0x0'),'head/tail')
    for i,r in enumerate(rs):record(r);check(r['owner']==s['owner']and r['next']==(rs[i+1]['address']if i+1<len(rs)else'0x0'),'owner/next')


def root_shape(s,rows):
    relation_list(s['relation_list']);check(s['relation_list']['owner']==s['address'],'root/list owner')
    check(0<=s[COUNTER]<=3 and s[GATE]==0,'bounded cached count and observed gate0')
    for o in s['objects']:base.shared.object_shape(o,rows)
    if s.get('linked_state')is not None:root_shape(s['linked_state'],rows)


def selector(e,data):
    check(e['id']in range(134,144),'relation selector keyword')
    return relations.selector({**e,'id':134},data)


def payload(es,data,b,a):
    check(a['serial']==b['serial']and a['id']==b['id']and a['offset']==b['offset'],'payload identity')
    continuation(b['id'],0x41a800,a['completion_rva']);check(a['completion_rva']==b['completion_rva'],'payload caller continuation')
    want=deepcopy(b['table']);cursor=b['before'];status=0;rank=want['rank_0x04'];width=want['values_per_point_0x40']
    allocs=[e for e in es if e['operation']=='property-allocation'and e['serial']==b['serial']]
    allocates=rank>0 or width>1;check(len(allocs)==int(allocates),'rank0 width<=1 returns before normalization/allocation')
    if allocates:want=allocation(want,allocs[0]['table'])
    width=want['values_per_point_0x40'];plan=[]
    if rank==0:plan=[('dependent',None,i)for i in range(total(want))]
    elif rank==1:
        for i in range(want['counts_0x1c'][0]):plan += [('axis',0,i)]+[('dependent',None,i*width+j)for j in range(width)]
    else:
        for axis in range(rank):plan += [('axis',axis,i)for i in range(want['counts_0x1c'][axis])]
        plan += [('temporary',None,i)for i in range(total(want))]
    scans=[e for e in es if e['operation']=='property-data-scan'and e['serial']==b['serial']]
    temp=[e for e in es if e['operation']=='property-temporary-allocated'and e['serial']==b['serial']]
    commits=[e for e in es if e['operation']=='property-temporary-committed'and e['serial']==b['serial']]
    frees=[e for e in es if e['operation']=='property-temporary-freed'and e['serial']==b['serial']]
    assigned=[None]*total(want);n=0;last=allocs[0]['sequence']if allocs else b['sequence']
    for e,(kind,axis,index)in zip(scans,plan):
        check(e['sequence']>last,'ordered payload scans');last=e['sequence'];n+=1
        check(e['location']=={'kind':kind,'axis':axis,'index':index},'axis/point/column native scan order')
        if kind=='temporary':check(len(temp)==1 and temp[0]['sequence']<e['sequence'],'temporary allocated before scan');pointer=temp[0]['pointer']
        else:pointer=want['pointers']['axes'][axis]if kind=='axis'else want['pointers']['dependent']
        check(int(e['destination'],16)==int(pointer,16)+index*8,'scalar exact destination')
        value,c,cursor=tables.scalar(e,data,cursor)
        if c==-1:status=1;break
        if kind=='temporary':check(c==1,'uninitialized staged matching failure excluded');assigned[index]=raw([value])
        elif c==1:tables.put(want,kind,index,value,axis)
    check(n==len(scans)and(n==len(plan)or status==1),'complete scan inventory and immediate EOF stop')
    axis_n=sum(want['counts_0x1c'][:rank])if rank>=2 else 0
    staged=rank>=2 and(not status or n>axis_n)
    check(len(temp)==int(staged),'staging only after all axes')
    if staged:check(temp[0]['count']==len(assigned)and temp[0]['assigned']==[None]*len(assigned),'uninitialized staging mask')
    check(len(commits)==len(frees)==int(staged and not status),'memcpy/free only after complete stage')
    if commits:
        check(all(x is not None for x in assigned),'every copied temporary value assigned')
        want['dependent']=list(struct.unpack('<'+'d'*len(assigned),bytes.fromhex(''.join(assigned))))if assigned else[];want['raw_hex']['dependent']=''.join(assigned)
        check(commits[0]['assigned']==assigned and commits[0]['pointer']==frees[0]['pointer']==temp[0]['pointer'],'exact staging source')
        check(last<commits[0]['sequence']<frees[0]['sequence']<a['sequence'],'memcpy/free order');equal(commits[0]['table'],want,'staging copied without permutation')
    expected_temp=None if not staged else{'pointer':temp[0]['pointer'],'count':len(assigned),'assigned':assigned,'freed':not bool(status)}
    check(a['temporary']==expected_temp,'temporary lifetime/partial initialized mask')
    discard=[e for e in es if e['operation']=='property-discard'and e['serial']==b['serial']]
    check(len(discard)==int(not status and total(want)>0),'positive total trailing fgets')
    if discard:_,_,cursor=base.line(discard[0],data,cursor)
    equal(a['table'],want,'complete or partial payload storage');check(a['status']==status and a['after']==cursor,'payload status/cursor')
    return want,status,cursor


def handler(es,data):
    first,last=es[0],es[-1];id=first['id'];check(id==last['id']and first['category']==last['category']==5,'relation property handler pair')
    sel=one(es,'relation-selector');keys=selector(sel,data);check(sel['before']==first['stream'],'selector starts at handler cursor')
    expected=deepcopy(first['state']);ls=expected['relation_list'];prior=next((r for r in ls['records']if[r['key0'],r['key1']]==keys),None)
    lookup=one(es,'relation-lookup');check([lookup['key0'],lookup['key1']]==keys and lookup['list']==ls['address'],'exact ordered signed pair lookup')
    check(lookup['record']==prior and lookup['pointer']==('0x0'if prior is None else prior['address']),'lookup result')
    created=prior is None;appends=[e for e in es if e['operation']=='relation-append-after']
    check(len(appends)==int(created),'only missing ordered pair appends')
    if created:
        before=one(es,'relation-append-before');a=appends[0];check(before['state']==ls and lookup['sequence']<before['sequence']<a['sequence'],'lookup then append')
        r=deepcopy(a['record']);defaults(r);check(r['next']=='0x0'and r['address']==a['pointer']and r['owner']==expected['address'],'new stable record identity')
        if ls['records']:ls['records'][-1]['next']=r['address']
        else:ls['head']=r['address']
        ls['tail']=r['address'];ls['count']+=1;ls['records'].append(r)
        check(ls==a['state'],'insertion order and constructor state before key assignment');r['key0'],r['key1']=keys
    else:r=prior
    cursor=sel['after'];used=set();row_count=0;reset_count=0;data_count=0;combined_count=0
    def sf(off,value):
        k=hex(off);x=r['scalar_fields'][k];x['value']=value
        if x['type']=='binary64':x['raw_hex']=raw([value])
    def row(kind):
        nonlocal cursor,row_count
        b=next(e for e in es if e['operation']=='relation-property-row-before'and e['serial']not in used)
        a=next(e for e in es if e['operation']=='relation-property-row-after'and e['serial']==b['serial']);used.add(b['serial']);row_count+=1
        check(b['id']==a['id']==id and b['kind']==a['kind']==kind and b['before']==a['before']==cursor,'line helper order/type/cursor')
        continuation(id,b['helper_rva'],b['completion_rva']);check(a['completion_rva']==b['completion_rva'],'line helper return')
        if kind=='name81':
            check(b['previous_name']==a['previous_name']=={'name_hex':r['name_hex'],'name_terminator_hex':'00'},'name prior value')
            lines=[];content=None;eof=True
            for _ in range(2):
                content,_,eof,cursor=base.line_read(data,base.stream_position(cursor,data));lines.append({'limit':1024,'eof':eof,'hex':None if eof else content.hex()})
                if eof:break
            check(a['lines']==lines and a['scans']==[] and a['after']==cursor and a['status']==int(eof),'two fgets name status/cursor')
            if not eof:
                name=content.split(b'\n',1)[0].split(b'\0',1)[0][:80].rstrip(b' \r\n');r['name_hex']=name.hex();r['name_terminator_hex']='00'
                # The native strncpy pads its bounded destination with NULs.
                if name:r['name_prefix_hex']=(name+b'\0')[:2].hex()
                else:r['name_prefix_hex']='0000'
            check(a['name']=={'name_hex':r['name_hex'],'name_terminator_hex':'00'},'name bounded trim/preservation');return [],a['status']
        kinds=FORMATS[kind];fmt=' '.join('%i'if x else'%Flf'for x in kinds)
        values,status,cursor=custom.line_helper(a,data,kinds,fmt);return values,status
    def reset(off,type_id):
        nonlocal reset_count
        b=next(e for e in es if e['operation']=='relation-property-reset-before'and e['serial']not in used);a=next(e for e in es if e['operation']=='relation-property-reset-after'and e['serial']==b['serial']);used.add(b['serial']);reset_count+=1
        check(b['id']==a['id']==id and b['offset']==a['offset']==off and b['type']==a['type']==type_id,'reset type/offset')
        continuation(id,0x12fc70,a['completion_rva']);check(b['completion_rva']==a['completion_rva'],'reset entry records actual caller return')
        join_returns=(0x383402,0x383436,0x383467,0x383498)
        check(a['observation_rva']==(0x383498 if a['completion_rva']in join_returns else a['completion_rva']),'reset observation join precedes caller writes')
        equal(b['table'],r['property_tables'][hex(off)],'reset receives prior table')
        t=profile(b['table'],type_id);equal(a['table'],t,'reset initialized profile and preserved metadata');r['property_tables'][hex(off)]=t;return t
    def get(off):return r['property_tables'][hex(off)]
    def data_read(off):
        nonlocal cursor,data_count
        b=next(e for e in es if e['operation']=='relation-property-data-before'and e['serial']not in used);a=next(e for e in es if e['operation']=='relation-property-data-after'and e['serial']==b['serial']);used.add(b['serial']);data_count+=1
        check(b['offset']==off and b['before']==cursor,'payload table/cursor');equal(b['table'],get(off),'header commits before table read')
        frame=[e for e in es if b['sequence']<=e['sequence']<=a['sequence']];t,status,cursor=payload(frame,data,b,a);r['property_tables'][hex(off)]=t;return 2 if status else 0
    def select(t,selector,v1,v2,second=12):
        if selector==0:t['rank_0x04']=0;t['constant_0x08']=v1;t['raw_hex']['constant']=raw([v1])
        elif selector in(1,2):t['rank_0x04']=1;t['selectors_0x10'][0]=3 if selector==1 else second;t['counts_0x1c'][0]=math.trunc(v1)
        elif selector==3:t['rank_0x04']=2;t['selectors_0x10'][:2]=[3,second];t['counts_0x1c'][:2]=[math.trunc(v1),math.trunc(v2)]
    def combined(off,n):
        nonlocal cursor,combined_count
        e=one(es,'relation-property-combined-scan');combined_count+=1
        check(e['id']==id and e['offset']==off and e['count']==n and e['format']==' '.join(['%Flf']*n)and e['before']==cursor,'combined scanner ABI/cursor')
        before=[r['scalar_fields'][hex(off+i*8)]['raw_hex']for i in range(n)];check(e['previous_hex']==before,'combined persistent initialized prior bytes')
        pos=base.stream_position(cursor,data);converted=0
        for i in range(n):
            v,c,pos=lex(data,pos)
            if c!=1:
                if not i:converted=c
                break
            sf(off+i*8,v);converted+=1
        cursor=base.stream_after(data,pos);want=[r['scalar_fields'][hex(off+i*8)]['raw_hex']for i in range(n)]
        check(e['conversions']==converted and e['after']==cursor and e['output_hex']==want,'combined aggregate partial conversion and raw retained outputs')
        for v,h in zip(e['values'],want):tables.floats([v],h)
        return 2 if converted==-1 else 0
    def run():
        if id==136:return 2 if row('name81')[1]else 0
        if id==143:
            v,s=row('int1');sf(0x768,v[0]);return 2 if s else 0
        if id==135:
            v,s=row('int2')
            if s:return 2
            mode,option=v
            if option in(0,1,2):r['field_0x10']=option
            if mode in(1,9):
                opts,s=row('int3'if mode==1 else'int2')
                if s:return 2
                coeff,s=row('real2')
                if s:return 2
                off=0x78 if mode==1 else 0xf8
                if mode==9:r['field_0x14']=11;t=reset(off,44)
                elif opts[0]in range(4):r['field_0x14']=opts[0];t=reset(off,44+opts[0])
                else:t=get(off)
                width=opts[1];sf(0x6c,width);t['values_per_point_0x40']=width
                if mode==1:sf(0xf0,opts[2])
                if width==1:sf(0x70,coeff[0]);sf(0x170,coeff[1])
                elif mode==1:sf(0x170,coeff[0]);sf(0x178,coeff[1])
                else:sf(0x3e8,coeff[0]);sf(0x3f0,coeff[1])
                counts,s=row('int3');t['counts_0x1c']=counts
                return 2 if s else data_read(off)
            if mode==2:
                r['field_0x14']=4;v,s=row('int4')
                if s:return 2
                t=reset(0x1f8,48);t['counts_0x1c']=v[1:]
                if v[0]!=2:return 3
                t['values_per_point_0x40']=2;return data_read(0x1f8)
            if mode in(3,5,7):
                r['field_0x14']={3:5,5:7,7:9}[mode];return combined({3:0x270,5:0x360,7:0x3b0}[mode],{3:11,5:6,7:7}[mode])
            if mode in(4,6):
                r['field_0x14']=6 if mode==4 else 8;v,s=row('real4');off=0x340 if mode==4 else 0x390
                for i,x in enumerate(v):sf(off+i*8,x)
                if s or mode==6:return 2 if s else 0
                v,s=row('int1')
                if s:return 2
                t=reset(0x2c8,50);t['counts_0x1c'][0]=v[0];return data_read(0x2c8)
            if mode==8:
                r['field_0x14']=10;v,s=row('int1')
                if s:return 2
                t=reset(0x3f8,54);t['counts_0x1c'][0]=v[0];return data_read(0x3f8)
            if 10<=mode<=13:
                r['field_0x14']=mode+2;v,s=row('int1')
                if s:return 2
                get(0x180)['values_per_point_0x40']=v[0];return data_read(0x180)
            if mode<0:r['field_0x14']=16;sf(0x470,struct.unpack('<i',struct.pack('<I',(-mode)&0xffffffff))[0])
            return 0
        if id==139:
            v,s=row('int2real2')
            if s:return 2
            sf(0x5e4,int(v[0]!=0))
            if v[0]==0:return 0
            select(get(0x5e8),v[1],v[2],v[3]);return data_read(0x5e8)
        if id==141:
            v,s=row('real5');sf(0x668,v[0]);sf(0x670,v[1])
            if s:return 2
            sf(0x660,1);selector=math.trunc(v[2]);t=reset(0x678,109)
            if selector in(0,1):select(t,selector,v[3],0.)
            return data_read(0x678)
        v,s=row('coord2')
        if s:return 2
        off,type_id={137:(0x478,31),138:(0x4f0,32),140:(0x568,108),142:(0x6f0,110)}[id];t=reset(off,type_id);selection=v[0]
        if id==140:
            sf(0x5e0,selection)
            if 4<=selection<=7:selection-=4
        select(t,selection,v[1],v[2],23 if id==142 else 12);return data_read(off)
    status=run()
    check(row_count==sum(e['operation']=='relation-property-row-before'for e in es),'complete header inventory')
    check(reset_count==sum(e['operation']=='relation-property-reset-before'for e in es),'complete reset inventory')
    check(data_count==sum(e['operation']=='relation-property-data-before'for e in es),'complete payload inventory')
    check(combined_count==sum(e['operation']=='relation-property-combined-scan'for e in es),'complete combined scan inventory')
    check(not any(e['operation']in('relation-recount-before','material-refresh-before')for e in es),'scientific property handlers never recount/refresh')
    check(last['status']==status and last['stream']==cursor,'handler exact status/cursor')
    check(last['state']==expected,'all persistent scalar/table mutations and all unrelated root state')
    return {'id':id,'dimension':first['state']['dimension'],'keys':keys,'created':created,'status':status,'rows':row_count,'resets':reset_count,'payloads':data_count,'combined':combined_count,'sequence':first['sequence']}


def validate(evidence,inputs):
    cap=evidence['normalized_result'];messages=cap['messages']
    check(len(cap['scripts'])==1 and all(m['type']=='send'and m['script_id']==cap['scripts'][0]['script_id']for m in messages),'single error-free passive script')
    check(messages[0]['payload']['operation']=='hooks-installed'and messages[0]['payload']['pid']==cap['pid'],'hook process identity')
    events=[m['payload']for m in messages[1:]]
    check([e['sequence']for e in events]==list(range(1,len(events)+1)),'complete event order')
    check(not any(e['operation']=='probe-error'for e in events),'no invalid probe observations')
    rows=descriptor_map(read(ROOT/'analysis/mesh-array-contract.json'));groups=[];g=None;live={};pending=None;clears=[];destroyed={}
    for e in events:
        op=e['operation']
        if op=='relation-append-after':defaults(e['record'])
        if op=='material-reference-allocate':
            check(e['call_return_rva']==0x3883ee and e['width']==4 and e['values']==[0]*e['capacity']and e['pointer']not in live,'reference allocation provenance')
            live[e['pointer']]={k:e[k]for k in('capacity','material','serial')}
        for r in refs.records_in(e):refs.record_shape(r,live)
        for s in refs.lists_in(e):refs.list_shape(s)
        for s in relations.nested_relation_lists(e):relation_list(s)
        if op=='material-reference-free-before':check(e['pointer']in live and pending is None,'live reference free');pending=e
        elif op=='material-reference-free-after':check(pending is not None and e['pointer']==pending['pointer'],'paired reference free');del live[e['pointer']];pending=None
        if 'state'in e and 'objects'in e['state']:root_shape(e['state'],rows)
        if op=='relation-clear-before':clears.append((e,[]))
        elif op=='relation-destroy-before':
            record(e['record']);check(clears and e['record']['address']not in destroyed,'destruction inside clear');destroyed[e['record']['address']]=e['record'];clears[-1][1].append(e['record'])
        elif op=='relation-destroy-after':check(e['address']in destroyed,'paired record destruction');del destroyed[e['address']]
        elif op=='relation-clear-after':
            check(clears,'clear pairing');before,dead=clears.pop()
            check(dead==before['state']['records'],'clear destroys insertion-order records before releasing nodes')
            expected=deepcopy(before['state']);expected.update(count=0,records=[],head='0x0',tail='0x0')
            check(e['state']==expected,'clear zero list and retain owner')
        if op=='material-reference-feature':
            want=e['control_0x85c']!=0 or any(r[refs.MODE]==1 for r in e['state']['records'])
            check(e['result']==int(want),'feature gate derived from live material modes')
        if op=='import-before':check(g is None,'non-nested imports');g=[]
        check(g is not None,'event inside import');g.append(e)
        if op=='import-after':groups.append(g);g=None
    check(g is None and pending is None and not clears and not destroyed,'all captured lifecycle calls completed')
    check([g[0]['file']for g in groups]==list(inputs),'complete ordered fixture imports')
    catalog={r['name']:(r['id'],r['category'])for r in read(ROOT/'analysis/keyword-catalog.json')['rows']}
    results=[];previous=None;refresh_count=0;changed_sizes=Counter()
    for g in groups:
        data=inputs[g[0]['file']];start=None;refresh_start=None;refstart=None;last_state=None;token=None;statuses=[]
        if previous is not None:check(g[0]['state']==previous,'complete between-import continuity')
        previous=g[-1]['state']
        for i,e in enumerate(g):
            op=e['operation']
            if op=='stream-scan'and e['format']=='%39s':refs.verify_token(e,data);token=e
            elif op=='keyword-resolved':check(token is not None and token['value']==e['token']and(e['id'],e['category'])==catalog.get(e['token'],(305,0)),'source-bound keyword resolution')
            if op=='material-refresh-before':check(refresh_start is None,'non-nested refresh');refresh_start=i
            elif op=='material-refresh-after':
                check(refresh_start is not None,'refresh pairing');frame=g[refresh_start:i+1];refresh_start=None
                relations.refresh(frame);refresh_count+=1
                for a,b in zip(frame[0]['state']['objects'],e['state']['objects']):
                    for c in (51,52):
                        x,y=base.descriptor(a,c),base.descriptor(b,c)
                        if x['length']!=y['length']:changed_sizes[f"{c}:{x['width']}->{y['width']}"]+=1
            if op=='handler-before':
                check(start is None,'handler entry pairing');start=i
                if last_state is not None:check(e['state']==last_state,'within-import full state continuity')
            elif op=='handler-after':
                check(start is not None,'handler return pairing');frame=g[start:i+1];start=None;statuses.append(e['status'])
                if e['id']==134:results.append(relations.relation_handler(frame,data))
                elif e['id']in IDS:results.append(handler(frame,data))
                elif e['category']==4:refs.handler(frame,data)
                if e['id']not in (134,)+IDS:
                    old=frame[0]['state'];check(old['relation_list']==e['state']['relation_list'] and old[COUNTER]==e['state'][COUNTER],'other handlers preserve relation list and stale cache')
                last_state=e['state']
            if op=='material-reference-finalize-before':refstart=i
            elif op=='material-reference-finalize-after':check(refstart is not None,'reference finalizer pairing');refs.finalize(g[refstart:i+1]);refstart=None
        check(start is None and refresh_start is None and refstart is None,'closed handler and refresh intervals')
        before,after=one(g,'finalize-before'),one(g,'finalize-after')
        if last_state is not None:check(before['state']==last_state,'finalizer receives last handler state')
        boundary=next(e for e in g if e['operation']in('handler-before','finalize-before'))['state']
        if boundary['dimension']!=g[0]['state']['dimension']:
            check(not boundary['objects'] and not boundary['material_list']['records'] and not boundary['relation_list']['records'] and boundary[COUNTER]==0 and boundary['components'][51:53]==[0,0],'changed dimension resets objects/materials/relations/cached counter')
        reference_end=one(g,'material-reference-finalize-after')
        check(refs.projection(after['state']['material_list']['records'])==refs.projection(reference_end['state']['records']),'root finalizer returns verified material references')
        expected=deepcopy(before['state']);expected['material_list']=deepcopy(after['state']['material_list'])
        if expected['finalization_controls']['control_0x93c']==0:
            mode=int(expected['finalization_controls']['step_increment_0x930']<=0);expected['finalization_controls']['mode_0x990']=expected['time_table']['mode_0x58']=mode
        check(all(o['flag_0x3990']==0 for o in expected['objects']),'selected no geometry finalization flag')
        check(expected==after['state'] and after['state']==g[-1]['state'],'finalization retains relation state/stale cache and does not refresh')
        check(g[-1]['status']==next((s for s in reversed(statuses)if s),0),'retained import failure status')
    check({(r['id'],r['dimension'])for r in results if r['id']in IDS}=={(i,d)for i in IDS for d in(2,3)},'all nine scientific routes in both dimensions')
    return {'imports':len(groups),'handlers':results,'refreshes':refresh_count,'coverage':{'id_dimensions':sorted(set((r['id'],r['dimension'])for r in results)),'failures':sum(r['status']!=0 for r in results),'relation_descriptor_width_changes':dict(changed_sizes),'reference_allocations_still_live_at_capture_end':len(live)},'event_counts':dict(Counter(e['operation']for e in events))}


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for k,v in [('capture','analysis/relation-property-runtime-training-v2.json'),('run-record','analysis/oracle/relation-property-training-b/run.json'),('manifest','analysis/relation-property-evaluation-cases-v2.json'),('freeze','analysis/relation-property-candidate-freeze.json'),('input-freeze','analysis/relation-property-v2-input-freeze.json'),('output','analysis/relation-property-observations.json')]:p.add_argument('--'+k,type=Path,default=ROOT/v)
    args=p.parse_args()
    for k in vars(args):setattr(args,k,getattr(args,k).resolve())
    evidence,run,manifest,freeze,input_freeze=map(read,[args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze])
    inputs=provenance(evidence,run,manifest,freeze,input_freeze,args.run_record.parent,args.manifest,args.freeze)
    result=validate(evidence,inputs);rejected=[];skipped=[]
    def tamper(label,op,fn,predicate=lambda e:True):
        changed=deepcopy(evidence);target=next((m['payload']for m in changed['normalized_result']['messages']if m['payload']['operation']==op and predicate(m['payload'])),None)
        if target is None:skipped.append(label);return
        fn(target)
        try:validate(changed,inputs)
        except (ValueError,KeyError,IndexError,AssertionError,StopIteration,TypeError,struct.error):rejected.append(label)
        else:raise ValueError('tamper accepted '+label)
    tamper('event order','relation-property-row-before',lambda e:e.update(sequence=0))
    tamper('selector signed pair','relation-selector',lambda e:e['values'].__setitem__(0,e['values'][0]+1))
    tamper('selector source cursor','relation-selector',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('constructor coefficient','relation-append-after',lambda e:e['record']['scalar_fields']['0x170'].update(value=0.,raw_hex=raw([0.])))
    tamper('constructor table type','relation-append-after',lambda e:e['record']['property_tables']['0x78'].update(type_0x00=0))
    tamper('row output raw','relation-property-row-after',lambda e:e['output_hex'].__setitem__(0,'00'*4),lambda e:e['kind']=='int2'and e['values'][0]!=0)
    tamper('row helper status','relation-property-row-after',lambda e:e.update(status=1-e['status']))
    tamper('row helper cursor','relation-property-row-after',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('row source line','relation-property-row-after',lambda e:e['lines'][0].update(hex='3939390a'),lambda e:bool(e['lines'])and not e['lines'][0]['eof'])
    tamper('row callsite','relation-property-row-before',lambda e:e.update(completion_rva=0))
    tamper('name trimming','relation-property-row-after',lambda e:e['name'].update(name_hex='78'),lambda e:e['kind']=='name81')
    tamper('reset type','relation-property-reset-after',lambda e:e['table'].update(type_0x00=999))
    tamper('reset preserved54','relation-property-reset-after',lambda e:e['table'].update(preserved_0x54=1))
    tamper('reset axis selector','relation-property-reset-after',lambda e:e['table']['selectors_0x10'].__setitem__(0,99))
    tamper('payload header rank','relation-property-data-before',lambda e:e['table'].update(rank_0x04=0),lambda e:e['table']['rank_0x04']>0)
    tamper('payload logical count','relation-property-data-before',lambda e:e['table']['counts_0x1c'].__setitem__(0,3),lambda e:e['table']['counts_0x1c'][0]!=3)
    tamper('payload width','relation-property-data-before',lambda e:e['table'].update(values_per_point_0x40=4),lambda e:e['table']['values_per_point_0x40']!=4)
    tamper('calloc default','property-allocation',lambda e:(e['table']['dependent'].__setitem__(0,1.),e['table']['raw_hex'].update(dependent=raw([1.]+e['table']['dependent'][1:]))),lambda e:bool(e['table']['dependent']))
    tamper('physical capacity','property-allocation',lambda e:e['table']['physical_capacities'].update(dependent=99))
    tamper('scalar raw source','property-data-scan',lambda e:e.update(value=999.,output_hex=raw([999.])),lambda e:e['conversions']==1)
    tamper('scalar destination','property-data-scan',lambda e:e.update(destination=hex(int(e['destination'],16)+8)))
    tamper('scalar axis/column order','property-data-scan',lambda e:e['location'].update(index=e['location']['index']+1))
    tamper('scalar EOF','property-data-scan',lambda e:e.update(conversions=0),lambda e:e['conversions']==-1)
    tamper('scalar cursor','property-data-scan',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('temporary initial mask','property-temporary-allocated',lambda e:e['assigned'].__setitem__(0,raw([0.])),lambda e:e['count']>0)
    tamper('temporary copied bytes','property-temporary-committed',lambda e:e['assigned'].__setitem__(0,raw([999.])),lambda e:bool(e['assigned']))
    tamper('temporary free pointer','property-temporary-freed',lambda e:e.update(pointer='0x1'))
    tamper('native EOF leak hidden','relation-property-data-after',lambda e:e['temporary'].update(freed=True),lambda e:e['temporary']is not None and not e['temporary']['freed'])
    tamper('partial dependent commit on EOF','relation-property-data-after',lambda e:(e['table']['dependent'].__setitem__(0,1.),e['table']['raw_hex'].update(dependent=raw([1.]+e['table']['dependent'][1:]))),lambda e:e['status']==1 and bool(e['table']['dependent']))
    tamper('payload status','relation-property-data-after',lambda e:e.update(status=1-e['status']))
    tamper('payload continuation','relation-property-data-after',lambda e:e.update(completion_rva=0))
    tamper('discard line','property-discard',lambda e:e.update(hex='780a'),lambda e:not e['eof'])
    tamper('combined prior bytes','relation-property-combined-scan',lambda e:e['previous_hex'].__setitem__(0,raw([999.])))
    tamper('combined partial retention','relation-property-combined-scan',lambda e:e['output_hex'].__setitem__(-1,raw([999.])))
    tamper('combined conversion count','relation-property-combined-scan',lambda e:e.update(conversions=0),lambda e:e['conversions']!=0)
    tamper('handler direct coefficient mutation','handler-after',lambda e:e['state']['relation_list']['records'][0]['scalar_fields']['0x668'].update(value=999.,raw_hex=raw([999.])),lambda e:e['id']==141)
    tamper('handler mode mutation','handler-after',lambda e:e['state']['relation_list']['records'][0].update(field_0x14=999),lambda e:e['id']==135)
    tamper('stale count mutated','handler-after',lambda e:e['state'].update(relation_count_0x13f4=3),lambda e:e['id']in IDS and e['state'][COUNTER]!=3)
    tamper('finalization table mutation','finalize-after',lambda e:e['state']['relation_list']['records'][0]['property_tables']['0x78'].update(type_0x00=999),lambda e:bool(e['state']['relation_list']['records']))
    for label,key,value in [('process identity','pid',-1),('capture truncation','messages_truncated',True),('probe bytes','source_sha256','0'*64)]:
        d=deepcopy(evidence);d['normalized_result'][key]=value
        try:provenance(d,run,manifest,freeze,input_freeze,args.run_record.parent,args.manifest,args.freeze)
        except ValueError:rejected.append(label)
        else:raise ValueError('provenance tamper accepted '+label)
    for label,which in [('fixture binding','manifest'),('frozen candidate archive','freeze'),('frozen input archive','input')]:
        m,f,i=deepcopy(manifest),deepcopy(freeze),deepcopy(input_freeze)
        (m['cases'][0]if which=='manifest'else f['files'][0]if which=='freeze'else i['inputs'][0])['sha256']='0'*64
        try:provenance(evidence,run,m,f,i,args.run_record.parent,args.manifest,args.freeze)
        except ValueError:rejected.append(label)
        else:raise ValueError('binding tamper accepted '+label)
    m=deepcopy(manifest);key='native_source_bindings'if m.get('native_source_bindings')else'source_bindings';m[key][0]['sha256']='0'*64
    try:provenance(evidence,run,m,freeze,input_freeze,args.run_record.parent,args.manifest,args.freeze)
    except ValueError:rejected.append('native source binding')
    else:raise ValueError('native source tamper accepted')
    report={'status':'PASS','result':result,'bindings':[binding(x)for x in(Path(__file__),args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze,PROBE,CONTRACT)],'tampering_rejected':rejected,'tampering_not_applied':skipped,'dependencies':[binding(ROOT/'tools'/n)for n in('verify_property_table_observations.py','verify_relation_observations.py','verify_material_column_observations.py','verify_material_reference_observations.py','verify_custom_field_observations.py','verify_boundary_sort_observations.py','verify_mesh_array_observations.py','verify_material_object_observations.py','verify_material_import_observations.py','capture_oracle.py')],'limitations':['Original-only selected finite initialized tables/scalars; no candidate output used.','Allocation failure, overflow, nonfinite conversion, scientific interpolation/cache, and uninitialized staged matching failures excluded.','Native staging EOF leak and retained physical buffers remain visible; owned C++ cleanup is a separate resource-lifetime boundary.','Prior invalid probe capture remains preserved and is rejected for its probe errors.']}
    args.output.write_text(json.dumps(report,indent=2)+'\n');print('PASS',len(result['handlers']),'handlers;',len(rejected),'actual corruption checks;',len(skipped),'not applicable')


if __name__=='__main__':main()

