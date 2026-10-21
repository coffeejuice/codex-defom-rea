"""Original-only property table audit from source bytes and passive native events.

No reconstruction outputs or candidate comparison code are used. Raw binary64
bytes are authoritative; numeric JSON may lose only the sign of zero.
"""
import argparse
from collections import Counter
from copy import deepcopy
import json
import math
from pathlib import Path
import re
import struct

import verify_material_column_observations as base
import verify_material_reference_observations as refs
from verify_boundary_sort_observations import ROOT, binding, check, read
from verify_mesh_array_observations import descriptor_map

PROBE=ROOT/'tools/observe_property_table_import.js'
CONTRACT=ROOT/'analysis/property-table-static-contract.json'
IDS=(95,96,97,98,99,100,115,121,122,125,126,131,133)
OFFSETS=dict(zip(IDS,(0x670,0x6e8,0x760,0x7e0,0x858,0x8d8,0x1cd8,0x1d50,0x1dc8,0x1fe0,0x20d8,0x17f0,0x1878)))
RETURNS=dict(zip(IDS,(0x385a46,0x385a71,0x385aa8,0x385ad3,0x385b13,0x385d15,0x388614,0x38863f,0x388687,0x385b52,0x385bae,0x388c66,0x388cbc)))
EXPAND='expansion_reference_0x7d8';HEAT='heat_capacity_mode_0x8d0';COARSE='coarsening_enabled_0x20d0'
raw,one=base.raw,base.one


def provenance(evidence,run,manifest,freeze,input_freeze,directory,manifest_path,freeze_path):
    base.PROBE=PROBE;base.CONTRACT=CONTRACT
    projected=deepcopy(manifest);projected['probe'].pop('evidence_id',None)
    for b in manifest.get('native_source_bindings',[]):
        check(binding(ROOT/b['path'])['sha256']==b['sha256'],'native source binding')
    return base.provenance(evidence,run,projected,freeze,input_freeze,directory,manifest_path,freeze_path)


def lex(data,pos,integer=False):
    if integer:return base.lex(data,pos,True)
    while pos<len(data) and data[pos] in b' \t\r\n\v\f':pos+=1
    m=re.match(rb'[+-]?(?:[0-9]+(?:\.[0-9]*)?|\.[0-9]+)(?:[eE][+-]?[0-9]+)?',data[pos:])
    if m:
        value=float(m[0]);check(math.isfinite(value),'finite decimal scope')
        return value,1,pos+len(m[0])
    if pos<len(data) and data[pos] in b'+-':pos+=1
    if pos<len(data) and data[pos]==ord('.'):pos+=1
    return None,-1 if pos==len(data) else 0,pos


def parse_header(content):
    values=[0,0.0,0.0,0.0];pos=0;conversions=0
    for i in range(4):
        value,c,pos=lex(content,pos,i==0)
        if c!=1:
            if i==0:conversions=c
            break
        values[i]=value;conversions+=1
    return values,conversions


def floats(values,payload):
    return base.array({'values':values,'values_hex':payload})


def table_shape(t):
    check(int(t['address'],16)>0 and t['rank_0x04']in(0,1,2),'live selected table rank')
    check(t['values_per_point_0x40']==1 and t['pointers']['cache_0x68']=='0x0','selected width/cache')
    check(len(t['counts_0x1c'])==len(t['selectors_0x10'])==3 and all(type(n)is int and -4<=n<=3 for n in t['counts_0x1c']),'bounded counts')
    check(all(type(n)is int for n in t['selectors_0x10']),'integer axis selectors')
    check(t['flag_0x50']==t['preserved_0x54']==t['mode_0x58']==0 and t['preserved_0x60_hex']=='00'*8,'selected reset metadata')
    floats([t['constant_0x08']],t['raw_hex']['constant'])
    check(len(t['axes'])==len(t['raw_hex']['axes'])==len(t['pointers']['axes'])==len(t['physical_capacities']['axes'])==3,'three axes')
    for i in range(3):
        vs=floats(t['axes'][i],t['raw_hex']['axes'][i]);cap=t['physical_capacities']['axes'][i];ptr=t['pointers']['axes'][i]
        check(len(vs)==cap<=3 and (ptr=='0x0')==(cap==0),'axis physical pointer/capacity')
        check(cap==0 or cap==max(0,t['counts_0x1c'][i]) and i<t['rank_0x04'],'axis allocated count/rank')
    vs=floats(t['dependent'],t['raw_hex']['dependent']);cap=t['physical_capacities']['dependent']
    check(len(vs)==cap<=4 and (t['pointers']['dependent']=='0x0')==(cap==0),'dependent pointer/capacity')
    check(cap==0 or cap==total(t),'dependent allocated product')
    pointers=[p for p in t['pointers']['axes']+[t['pointers']['dependent']] if p!='0x0']
    check(len(pointers)==len(set(pointers)),'separate owned axis/dependent allocations')


def total(t):
    if t['rank_0x04']==0:return 0
    return max(0,math.prod(t['counts_0x1c'][:t['rank_0x04']]))


def empty_table(t,type_id=0):
    r=deepcopy(t)
    r.update(type_0x00=type_id,rank_0x04=0,constant_0x08=0.0,selectors_0x10=[0,0,0],counts_0x1c=[0,0,0],values_per_point_0x40=1,flag_0x50=0,mode_0x58=0,axes=[[],[],[]],dependent=[])
    r['raw_hex']={'constant':'00'*8,'axes':['','',''],'dependent':''}
    r['pointers']={'axes':['0x0']*3,'dependent':'0x0','cache_0x68':'0x0'}
    r['physical_capacities']={'axes':[0]*3,'dependent':0}
    return r


def put(t,kind,index,value,axis=None):
    if kind=='axis':
        t['axes'][axis][index]=value;t['raw_hex']['axes'][axis]=raw(t['axes'][axis])
    else:
        t['dependent'][index]=value;t['raw_hex']['dependent']=raw(t['dependent'])


def table_equal(a,b,label):
    table_shape(a);table_shape(b)
    # Numeric sign of zero has already been checked against raw data.
    check(a==b,label)


def allocation(expected,observed):
    want=deepcopy(expected);want['counts_0x1c']=[max(0,n) for n in want['counts_0x1c']]
    rank=want['rank_0x04'];counts=want['counts_0x1c'];n=total(want)
    want['axes']=[[0.0]*counts[i] if i<rank else [] for i in range(3)];want['dependent']=[0.0]*n
    want['raw_hex']['axes']=[raw(x) for x in want['axes']];want['raw_hex']['dependent']=raw(want['dependent'])
    want['physical_capacities']={'axes':[len(x) for x in want['axes']],'dependent':n}
    want['pointers']=deepcopy(observed['pointers'])
    table_equal(observed,want,'calloc normalization and all physical values initially zero')
    return want


def nested_tables(v):
    if isinstance(v,dict):
        if 'physical_capacities'in v and 'type_0x00'in v:yield v
        for x in v.values():yield from nested_tables(x)
    elif isinstance(v,list):
        for x in v:yield from nested_tables(x)


def record_shape(r):
    refs.record_shape(r,{})
    check(set(r['property_tables'])=={str(i) for i in IDS},'thirteen material tables')
    for id,t in r['property_tables'].items():
        table_shape(t);check(int(t['address'],16)==int(r['address'],16)+OFFSETS[int(id)],'native material table offset')
    floats([r[EXPAND]],r[EXPAND+'_hex'])
    check(type(r[HEAT])is int and type(r[COARSE])is int,'auxiliary integer fields')


def defaults(r):
    record_shape(r)
    for id,t in r['property_tables'].items():table_equal(t,empty_table(t,93 if int(id)==131 else 182 if int(id)==133 else 0),'constructor table defaults')
    check(r[EXPAND+'_hex']=='00'*8 and r[HEAT]==r[COARSE]==0,'constructor auxiliary defaults')


def scalar(e,data,cursor):
    check(e['before']==cursor and e['format']=='%Flf','scalar type and ordered cursor')
    value,c,end=lex(data,base.stream_position(cursor,data))
    check(e['conversions']==c and e['after']==base.stream_after(data,end),'scalar source conversion/cursor')
    if c==1:check(e['output_hex']==raw([value]) and e['value']==value,'scalar value raw source')
    else:check(e['output_hex']is None and e['value']is None,'failed assignment output remains unread')
    return value,c,e['after']


def payload(es,data,b,a):
    check(a['serial']==b['serial'] and a['parent_serial']==b['parent_serial'] and a['completion_rva']==0x41b651,'payload identity/continuation')
    expected=deepcopy(b['table']);cursor=b['before'];status=0
    allocations=[e for e in es if e['operation']=='property-allocation' and b['sequence']<e['sequence']<a['sequence']]
    rank=expected['rank_0x04']
    check(len(allocations)==int(rank>0),'rank0 scalar early return versus allocation')
    if rank>0:expected=allocation(expected,allocations[0]['table'])
    scans=[e for e in es if e['operation']=='property-data-scan' and e['serial']==b['serial']]
    plan=[]
    if rank==1:
        for i in range(expected['counts_0x1c'][0]):plan.extend([('axis',0,i),('dependent',None,i)])
    elif rank==2:
        for axis in range(2):plan.extend(('axis',axis,i) for i in range(expected['counts_0x1c'][axis]))
        plan.extend(('temporary',None,i) for i in range(total(expected)))
    assigned=[None]*total(expected);temp_events=[e for e in es if e['operation']=='property-temporary-allocated' and e['serial']==b['serial']]
    commits=[e for e in es if e['operation']=='property-temporary-committed' and e['serial']==b['serial']]
    frees=[e for e in es if e['operation']=='property-temporary-freed' and e['serial']==b['serial']]
    count=0;last_seq=allocations[0]['sequence'] if allocations else b['sequence']
    for e,(kind,axis,index) in zip(scans,plan):
        check(e['sequence']>last_seq,'ordered scalar events after allocation');last_seq=e['sequence'];count+=1
        check(e['location']=={'kind':kind,'axis':axis,'index':index},'axis/point/staging scan order')
        if kind=='temporary':
            check(len(temp_events)==1 and temp_events[0]['sequence']<e['sequence'],'temporary exists before dependent scan')
            pointer=temp_events[0]['pointer']
        else:pointer=expected['pointers']['axes'][axis] if kind=='axis' else expected['pointers']['dependent']
        check(int(e['destination'],16)==int(pointer,16)+index*8,'scalar destination physical element')
        value,c,cursor=scalar(e,data,cursor)
        if c==-1:status=1;break
        if kind=='temporary':
            check(c==1,'uninitialized temporary matching failure outside fixture domain');assigned[index]=raw([value])
        elif c==1:put(expected,kind,index,value,axis)
    check(count==len(scans) and (count==len(plan) or status==1),'complete scans with immediate EOF stop')
    axis_scans=sum(expected['counts_0x1c'][:2]) if rank==2 else 0
    reached_temp=rank==2 and (not status or count>axis_scans)
    check(len(temp_events)==int(reached_temp),'temporary allocation only after all axis scans')
    if reached_temp:
        t=temp_events[0];check(t['count']==len(assigned) and t['assigned']==[None]*len(assigned),'fresh uninitialized temporary mask')
        check(len(assigned)==0 or int(t['pointer'],16)>0,'nonnull positive temporary')
    check(len(commits)==len(frees)==int(reached_temp and not status),'commit/free only after complete staged reads')
    if commits:
        check(all(x is not None for x in assigned),'all staging elements initialized before memcpy')
        expected['dependent']=list(struct.unpack('<'+'d'*len(assigned),bytes.fromhex(''.join(assigned)))) if assigned else []
        expected['raw_hex']['dependent']=''.join(assigned)
        check(commits[0]['assigned']==assigned and commits[0]['pointer']==frees[0]['pointer']==temp_events[0]['pointer'],'exact temporary commit source')
        check(last_seq<commits[0]['sequence']<frees[0]['sequence']<a['sequence'],'memcpy then free before return')
        table_equal(commits[0]['table'],expected,'dependent commit copies staging without permutation')
    want_temp=None if not reached_temp else {'pointer':temp_events[0]['pointer'],'count':len(assigned),'assigned':assigned,'freed':not bool(status)}
    check(a['temporary']==want_temp,'returned temporary lifetime and initialized mask')
    discard=[e for e in es if e['operation']=='property-discard' and e['serial']==b['serial']]
    check(len(discard)==int(not status and total(expected)>0),'positive product successful read discards one line')
    if discard:
        _,_,cursor=base.line(discard[0],data,cursor)
        check(discard[0]['sequence']>(frees[0]['sequence'] if frees else last_seq),'discard after all commits')
    table_equal(a['table'],expected,'payload partial or complete final table')
    check(a['after']==cursor and a['status']==status,'payload return cursor/status; discard EOF ignored')
    return expected,status,cursor


def wrapper(es,data,b,a):
    id=b['id'];serial=b['serial'];expected=deepcopy(b['table']);aux=b['aux_hex']
    check(a['id']==id and a['serial']==serial and a['completion_rva']==RETURNS[id],'wrapper native continuation/identity')
    check(a['before']==b['before'] and a['aux_flag']==b['aux_flag'] and a['aux_pointer']==b['aux_pointer'],'wrapper input identity')
    check(b['aux_flag']==int(id in (97,99)) and (b['aux_pointer']!='0x0')==(id in (97,99)),'selected auxiliary ABI')
    content,_,eof,cursor=base.line_read(data,base.stream_position(b['before'],data))
    lines=[{'limit':1024,'eof':eof,'hex':None if eof else content.hex()}]
    check(a['lines']==lines,'one header fgets exact source')
    reset=[e for e in es if e['operation']=='property-reset' and e['serial']==serial]
    headers=[e for e in es if e['operation']=='property-header-after' and e['serial']==serial]
    hscans=[e for e in es if e['operation']=='property-header-scan' and e['serial']==serial]
    starts=[e for e in es if e['operation']=='property-data-before' and e['parent_serial']==serial]
    status=1;scans=[];conversions=-1
    if not eof:
        cleaned=content.replace(b',',b' ');values,conversions=parse_header(cleaned)
        scans=[{'format':'%i %Flf %Flf %Flf','input_hex':cleaned.hex(),'conversions':conversions}]
        check(len(headers)==len(hscans)==1,'header scanf continuations')
        h=headers[0];s=hscans[0]
        check(h['conversions']==conversions and h['after']==cursor and h['completion_rva']==0x41b34f and h['lines']==lines and h['scans']==scans,'header completion cursor/source')
        check(s['format']==scans[0]['format'] and s['conversions']==conversions and s['values']==values and s['output_hex']==[raw([v],i==0) for i,v in enumerate(values)],'header initialized partial output bytes')
    else:check(not headers and not hscans,'EOF skips header scanf')
    check(a['scans']==scans,'wrapper header scan inventory')
    if conversions!=-1:
        check(len(reset)==len(starts)==1,'non-EOF scan resets and invokes payload')
        expected=empty_table(expected);table_equal(reset[0]['table'],expected,'reset preserves opaque54/60 and clears old payload')
        check(reset[0]['completion_rva']==0x41b379,'reset continuation')
        first_alloc=[e for e in es if e['operation']=='property-allocation' and e['serial']==serial and e['sequence']<reset[0]['sequence']]
        check(len(first_alloc)==1,'type0 reset allocation invocation');table_equal(first_alloc[0]['table'],expected,'reset allocation state')
        selector=values[0]
        if selector==0:expected['constant_0x08']=values[1];expected['raw_hex']['constant']=raw([values[1]])
        elif selector in (1,2,3,6):
            expected['rank_0x04']=1;expected['selectors_0x10'][0]={1:3,2:9,3:12,6:8}[selector];expected['counts_0x1c'][0]=math.trunc(values[1])
        elif selector in (4,5,7):
            expected['rank_0x04']=2;expected['selectors_0x10'][:2]=[3,{4:12,5:8,7:4}[selector]];expected['counts_0x1c'][:2]=[math.trunc(values[1]),math.trunc(values[2])]
        if id in (97,99):
            if 0<=selector<=3 and conversions>=3:aux=raw([values[2]])
            elif selector==4 and conversions>=4:aux=raw([values[3]])
        child=starts[0];check(reset[0]['sequence']<child['sequence'] and child['before']==cursor,'reset then header mutation then payload')
        table_equal(child['table'],expected,'header exact selector/rank/count before normalization')
        end=next(e for e in es if e['operation']=='property-data-after' and e['serial']==child['serial'])
        expected,status,cursor=payload(es,data,child,end)
    else:
        check(not reset and not starts and not any(e['operation']=='property-allocation' for e in es),'failed header preserves table and avoids allocation')
    table_equal(a['table'],expected,'wrapper final table')
    check(a['status']==status and a['after']==cursor and a['aux_hex']==aux,'wrapper status/cursor/auxiliary writes')
    return expected,status,cursor,aux


def handler(es,data):
    first,last=es[0],es[-1];id=first['id'];check(id==last['id'] and first['category']==last['category']==4,'property handler pair')
    sel=one(es,'material-selector');check(sel['id']==id and sel['before']==first['stream'],'material selector ABI/cursor')
    selected=refs.verify_selector({**sel,'id':92},data);check(selected is not None,'defined material selector')
    old=first['state']['material_list']['records'];created=refs.find(old,selected)is None
    appends=[e for e in es if e['operation']=='material-append-after'];refreshes=[e for e in es if e['operation']=='material-refresh-after']
    check(len(appends)==len(refreshes)==int(created),'first creation alone appends and refreshes')
    expected=deepcopy(refreshes[0]['state'] if created else first['state'])
    if created:
        fresh=appends[0]['state']['records'][-1];defaults(fresh);check(fresh['id']==0,'append constructor ID zero before assignment')
        check(appends[0]['sequence']<refreshes[0]['sequence'],'append precedes refresh')
        check(expected['material_count']==len(old)+1 and [r['id'] for r in expected['material_list']['records']]==[r['id'] for r in old]+[selected],'stable insertion and exact signed ID')
        maps=[e for e in es if e['operation']in('material-scratch-before','material-scratch-after')]
        check(len(maps)==4 and [e['mode'] for e in maps]==[0,0,1,1],'scratch allocate then cleanup modes')
        check(maps[0]['state']==first['state']['material_list'],'scratch begins from prior list')
        ids=[r['id'] for r in old];ranks=[sum(x<id for x in ids) for id in ids]
        scratch=maps[1]['state']['scratch']
        check(scratch['allocation_observed'] and scratch['count']==len(ids) and scratch['ids']==ids and scratch['ranks']==ranks,'old ID insertion order and sorted rank scratch')
        check(maps[1]['sequence']<appends[0]['sequence']<refreshes[0]['sequence']<maps[2]['sequence']<maps[3]['sequence'],'scratch lifetime encloses append and refresh')
        check(maps[2]['state']==expected['material_list'],'cleanup receives refreshed list')
        expected['material_list']['scratch']={'ids_pointer':'0x0','ranks_pointer':'0x0','allocation_observed':False,'count':None,'ids':None,'ranks':None}
        check(maps[3]['state']==expected['material_list'],'native29ca10 mode1 frees scratch before property payload')
    r=refs.find(expected['material_list']['records'],selected);check(r is not None,'selected live material')
    cursor=sel['after'];status=0
    if id==126:
        cb,ca=one(es,'property-coarse-before'),one(es,'property-coarse-after')
        check(cb['before']==cursor and cb['previous']==r[COARSE] and int(cb['output'],16)==int(r['address'],16)+0x20d0,'COARSE destination and prior value')
        check(ca['serial']==cb['serial'] and ca['before']==cursor and ca['completion_rva']==0x385b7a,'COARSE continuation')
        value,status=refs.integer_row({**ca,'skip':0},data);r[COARSE]=value;cursor=ca['after']
    bs=[e for e in es if e['operation']=='property-reader-before'];ends=[e for e in es if e['operation']=='property-reader-after']
    invoke=id!=126 or r[COARSE]>0
    check(len(bs)==len(ends)==int(invoke),'COARSE positive gate and generic wrapper count')
    if invoke:
        b,a=bs[0],ends[0];check(b['before']==cursor,'wrapper follows selector or coarse row')
        table_equal(b['table'],r['property_tables'][str(id)],'reader receives selected existing table')
        if created:check(refreshes[0]['sequence']<b['sequence'],'creation refresh before payload')
        if id==97:check(int(b['aux_pointer'],16)==int(r['address'],16)+0x7d8 and b['aux_hex']==r[EXPAND+'_hex'],'persistent EXPAND auxiliary')
        if id==99:check(b['aux_hex']=='00'*8,'HEATCP local auxiliary zero each call')
        frame=[e for e in es if b['sequence']<=e['sequence']<=a['sequence']]
        t,status,cursor,aux=wrapper(frame,data,b,a);r['property_tables'][str(id)]=deepcopy(t)
        if id==97:r[EXPAND]=struct.unpack('<d',bytes.fromhex(aux))[0];r[EXPAND+'_hex']=aux
        if id==99:r[HEAT]=math.trunc(struct.unpack('<d',bytes.fromhex(aux))[0])
        if id==121 and t['rank_0x04']==2:r['property_tables'][str(id)]['selectors_0x10'][1]=20
    check(last['status']==(2 if status else 0) and last['stream']==cursor,'handler status/cursor')
    check(expected==last['state'],'exact table/aux mutation with unrelated root/object/material state preserved')
    return {'id':id,'dimension':first['state']['dimension'],'selected_id':selected,'created':created,'status':last['status'],'rank':r['property_tables'][str(id)]['rank_0x04'],'sequence':first['sequence']}


def validate(evidence,inputs):
    cap=evidence['normalized_result'];messages=cap['messages']
    check(len(cap['scripts'])==1 and all(m['type']=='send' and m['script_id']==cap['scripts'][0]['script_id'] for m in messages),'one passive script and no runtime errors')
    check(messages[0]['payload']['operation']=='hooks-installed' and messages[0]['payload']['pid']==cap['pid'],'hook process identity')
    events=[m['payload'] for m in messages[1:]]
    check([e['sequence'] for e in events]==list(range(1,len(events)+1)),'complete event sequence')
    check(not any(e['operation']=='probe-error' for e in events),'no probe errors')
    rows=descriptor_map(read(ROOT/'analysis/mesh-array-contract.json'));groups=[];g=None
    for e in events:
        for t in nested_tables(e):table_shape(t)
        for r in refs.records_in(e):record_shape(r)
        for s in refs.lists_in(e):refs.list_shape(s)
        if 'state'in e and 'objects'in e['state']:
            s=e['state']
            for o in s['objects']:base.shared.object_shape(o,rows)
            check(s['material_count']==s['material_list']['root_count'],'root material count projection')
        if e['operation']=='material-append-after':defaults(e['state']['records'][-1])
        if e['operation']=='import-before':check(g is None,'non-nested import');g=[]
        check(g is not None,'event inside import')
        if g:check(e['file']==g[0]['file'],'event fixture owner')
        g.append(e)
        if e['operation']=='import-after':groups.append(g);g=None
    check(g is None and [g[0]['file'] for g in groups]==list(inputs),'complete ordered imports')
    results=[];previous=None;refresh_count=0
    catalog={r['name']:(r['id'],r['category']) for r in read(ROOT/'analysis/keyword-catalog.json')['rows']}
    for g in groups:
        data=inputs[g[0]['file']];start=None;refresh_start=None;last_state=None;token=None;statuses=[]
        if previous is not None:check(g[0]['state']==previous,'full state continuity between imports')
        previous=g[-1]['state']
        boundary=next(e for e in g if e['operation']in('handler-before','finalize-before'))['state']
        if boundary['dimension']!=g[0]['state']['dimension']:
            check(not boundary['objects'] and not boundary['material_list']['records'] and boundary['material_count']==0,'changed dimension destroys objects and all material tables before parsing')
        else:
            check(boundary['material_list']['records']==g[0]['state']['material_list']['records'],'same dimension retains all material tables before parsing')
        for i,e in enumerate(g):
            op=e['operation']
            if op=='stream-scan' and e['format']=='%39s':refs.verify_token(e,data);token=e
            elif op=='keyword-resolved':check(token is not None and token['value']==e['token'] and (e['id'],e['category'])==catalog.get(e['token'],(305,0)),'source keyword resolution')
            if op=='material-refresh-before':check(refresh_start is None,'non-nested refresh');refresh_start=i
            elif op=='material-refresh-after':
                check(refresh_start is not None,'refresh pairing');base.shared.refresh_events(g[refresh_start:i+1]);refresh_start=None;refresh_count+=1
            if op=='handler-before':
                check(start is None,'handler pairing');start=i
                if last_state is not None:check(e['state']==last_state,'full state continuity between handlers')
            elif op=='handler-after':
                check(start is not None,'handler return pairing');frame=g[start:i+1];start=None;statuses.append(e['status'])
                if e['id']in IDS:results.append(handler(frame,data))
                else:check(frame[0]['state']['material_list']['records']==e['state']['material_list']['records'],'other selected handlers retain all material tables')
                last_state=e['state']
        check(start is None and refresh_start is None,'closed handler/refresh frames')
        before,after=one(g,'finalize-before'),one(g,'finalize-after')
        if last_state is not None:check(before['state']==last_state,'finalization receives last handler state')
        expected=deepcopy(before['state'])
        if expected['finalization_controls']['control_0x93c']==0:
            mode=int(expected['finalization_controls']['step_increment_0x930']<=0);expected['finalization_controls']['mode_0x990']=expected['time_table']['mode_0x58']=mode
        check(all(o['flag_0x3990']==0 for o in expected['objects']),'selected geometry finalization flag false')
        check(expected==after['state']==g[-1]['state'],'finalization preserves all tables and performs only proved normalization')
        check(g[-1]['status']==next((s for s in reversed(statuses) if s),0),'import retained failure status')
    coverage=set((r['id'],r['dimension']) for r in results)
    check(coverage=={(id,dim) for id in IDS for dim in (2,3)},'all thirteen property routes observed in both dimensions')
    return {'imports':len(groups),'handlers':results,'refreshes':refresh_count,'coverage':{'id_dimensions':sorted(coverage),'failures':sum(r['status']!=0 for r in results)},'event_counts':dict(Counter(e['operation'] for e in events))}


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for k,v in [('capture','analysis/property-table-runtime-training-v3.json'),('run-record','analysis/oracle/property-table-training-c/run.json'),('manifest','analysis/property-table-evaluation-cases-v3.json'),('freeze','analysis/property-table-candidate-freeze.json'),('input-freeze','analysis/property-table-v4-input-freeze.json'),('output','analysis/property-table-observations.json')]:p.add_argument('--'+k,type=Path,default=ROOT/v)
    args=p.parse_args()
    for k in vars(args):setattr(args,k,getattr(args,k).resolve())
    evidence,run,manifest,freeze,input_freeze=map(read,[args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze])
    inputs=provenance(evidence,run,manifest,freeze,input_freeze,args.run_record.parent,args.manifest,args.freeze)
    result=validate(evidence,inputs);rejected=[];skipped=[]
    def tamper(label,op,fn,predicate=lambda e:True):
        changed=deepcopy(evidence);target=next((m['payload'] for m in changed['normalized_result']['messages'] if m['payload']['operation']==op and predicate(m['payload'])),None)
        if target is None:skipped.append(label);return
        fn(target)
        try:validate(changed,inputs)
        except (ValueError,KeyError,IndexError,AssertionError,StopIteration,TypeError,struct.error):rejected.append(label)
        else:raise ValueError('tamper accepted '+label)
    tamper('event sequence','property-reader-before',lambda e:e.update(sequence=0))
    tamper('header raw output','property-header-scan',lambda e:e['output_hex'].__setitem__(1,raw([999.])))
    tamper('header numeric conversion','property-header-scan',lambda e:e.update(conversions=99))
    tamper('header source bytes','property-reader-after',lambda e:e['lines'][0].update(hex='30203939390a'),lambda e:not e['lines'][0]['eof'])
    tamper('header cursor','property-header-after',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('reset type','property-reset',lambda e:e['table'].update(type_0x00=99))
    tamper('preserved metadata54','property-reset',lambda e:e['table'].update(preserved_0x54=1))
    tamper('preserved metadata60','property-reset',lambda e:e['table'].update(preserved_0x60_hex='01'+'00'*7))
    tamper('header axis selector','property-data-before',lambda e:e['table']['selectors_0x10'].__setitem__(0,99),lambda e:e['table']['rank_0x04']>0)
    tamper('header logical count','property-data-before',lambda e:e['table']['counts_0x1c'].__setitem__(0,3),lambda e:e['table']['rank_0x04']>0 and e['table']['counts_0x1c'][0]!=3)
    tamper('calloc default','property-allocation',lambda e:(e['table']['dependent'].__setitem__(0,1.),e['table']['raw_hex'].update(dependent=raw([1.]+e['table']['dependent'][1:]))),lambda e:bool(e['table']['dependent']))
    tamper('physical capacity','property-allocation',lambda e:e['table']['physical_capacities'].update(dependent=99))
    tamper('scalar source value','property-data-scan',lambda e:e.update(value=999.,output_hex=raw([999.])),lambda e:e['conversions']==1)
    tamper('scalar destination','property-data-scan',lambda e:e.update(destination=hex(int(e['destination'],16)+8)))
    tamper('scalar ordering','property-data-scan',lambda e:e['location'].update(index=e['location']['index']+1))
    tamper('scalar EOF status','property-data-scan',lambda e:e.update(conversions=0),lambda e:e['conversions']==-1)
    tamper('scalar cursor','property-data-scan',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('temporary initial mask','property-temporary-allocated',lambda e:e['assigned'].__setitem__(0,raw([0.])),lambda e:e['count']>0)
    tamper('temporary commit bytes','property-temporary-committed',lambda e:e['assigned'].__setitem__(0,raw([999.])),lambda e:bool(e['assigned']))
    tamper('temporary free pointer','property-temporary-freed',lambda e:e.update(pointer='0x1'))
    tamper('staging EOF falsely freed','property-data-after',lambda e:e['temporary'].update(freed=True),lambda e:e['temporary']is not None and not e['temporary']['freed'])
    tamper('partial dependent committed on EOF','property-data-after',lambda e:(e['table']['dependent'].__setitem__(0,1.),e['table']['raw_hex'].update(dependent=raw([1.]+e['table']['dependent'][1:]))),lambda e:e['status']==1 and bool(e['table']['dependent']))
    tamper('payload status','property-data-after',lambda e:e.update(status=1-e['status']))
    tamper('payload continuation','property-data-after',lambda e:e.update(completion_rva=0))
    tamper('discard source','property-discard',lambda e:e.update(hex='780a'),lambda e:not e['eof'])
    tamper('auxiliary write','property-reader-after',lambda e:e.update(aux_hex=raw([999.])),lambda e:e['id']in(97,99))
    tamper('COARSE flag raw','property-coarse-after',lambda e:e.update(output_hex='01000000'),lambda e:e['value']!=1)
    tamper('wrapper return status','property-reader-after',lambda e:e.update(status=1-e['status']))
    tamper('constructor type default','material-append-after',lambda e:e['state']['records'][-1]['property_tables']['131'].update(type_0x00=0))
    tamper('HEATCP integer postprocess','handler-after',lambda e:e['state']['material_list']['records'][0].update(heat_capacity_mode_0x8d0=999),lambda e:e['id']==99)
    tamper('PMEAB axis postprocess','handler-after',lambda e:e['state']['material_list']['records'][0]['property_tables']['121']['selectors_0x10'].__setitem__(1,99),lambda e:e['id']==121)
    tamper('negative zero raw sign','property-reader-after',lambda e:e['table']['raw_hex'].update(constant='00'*8),lambda e:e['table']['raw_hex']['constant']=='0000000000000080')
    tamper('finalization table mutation','finalize-after',lambda e:e['state']['material_list']['records'][0]['property_tables']['95'].update(type_0x00=999),lambda e:bool(e['state']['material_list']['records']))
    for label,key,value in [('process identity','pid',-1),('capture truncation','messages_truncated',True),('probe bytes','source_sha256','0'*64)]:
        d=deepcopy(evidence);d['normalized_result'][key]=value
        try:provenance(d,run,manifest,freeze,input_freeze,args.run_record.parent,args.manifest,args.freeze)
        except ValueError:rejected.append(label)
        else:raise ValueError('provenance tamper accepted '+label)
    for label,which in [('fixture binding','manifest'),('frozen candidate archive','freeze'),('frozen input archive','input')]:
        m,f,i=deepcopy(manifest),deepcopy(freeze),deepcopy(input_freeze)
        (m['cases'][0] if which=='manifest' else f['files'][0] if which=='freeze' else i['inputs'][0])['sha256']='0'*64
        try:provenance(evidence,run,m,f,i,args.run_record.parent,args.manifest,args.freeze)
        except ValueError:rejected.append(label)
        else:raise ValueError('binding tamper accepted '+label)
    m=deepcopy(manifest);key='native_source_bindings' if m.get('native_source_bindings') else 'source_bindings';m[key][0]['sha256']='0'*64
    try:provenance(evidence,run,m,freeze,input_freeze,args.run_record.parent,args.manifest,args.freeze)
    except ValueError:rejected.append('native static source binding')
    else:raise ValueError('native source tamper accepted')
    report={'status':'PASS','result':result,'bindings':[binding(x) for x in(Path(__file__),args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze,PROBE,CONTRACT)],'tampering_rejected':rejected,'tampering_not_applied':skipped,'dependencies':[binding(ROOT/'tools'/n) for n in('verify_material_column_observations.py','verify_material_reference_observations.py','verify_material_object_observations.py','verify_boundary_sort_observations.py','verify_mesh_array_observations.py','verify_material_import_observations.py','verify_step_distance_observations.py','verify_time_controls_observations.py','capture_oracle.py')],'limitations':['Original-only bounded finite selected13 family; rank0..2,width1. Rank3/vector widths not reachable through these wrappers.','No OOM, overflowing numeric conversions/extents, borrowed pointer ownership, cache consumers or uninitialized staged matching failures.','Native temporary leak on staged EOF retained as an observed limitation, not repaired or hidden.','No candidate or comparator outputs used. Historical faulty probe audit superseded before any native observations.']}
    args.output.write_text(json.dumps(report,indent=2)+'\n');print('PASS',len(result['handlers']),'property handlers;',len(rejected),'applied corruption checks;',len(skipped),'not applicable')


if __name__=='__main__':main()
