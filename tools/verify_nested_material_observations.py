"""Original-only audit for FSTRES94/mode14 and GRNDAT108/selector4.

Uses retained native contracts and fixture bytes; no candidate/comparator outputs.
"""
import argparse
from collections import Counter
from copy import deepcopy
import json,math,re,struct
from pathlib import Path
import verify_property_table_observations as old_tables
import verify_relation_property_observations as relation_props
import verify_material_column_observations as base
import verify_material_reference_observations as refs
import verify_custom_field_observations as custom
from verify_boundary_sort_observations import ROOT,binding,check,read
from verify_mesh_array_observations import descriptor_map

PROBE=ROOT/'tools/observe_nested_material_import.js'
CONTRACT=ROOT/'analysis/nested-material-static-contract.json'
IDS=(94,108)
OFFSETS={94:0x388,108:0x1740}
RETURNS={94:0x3858bc,108:0x387923}
raw,one=base.raw,base.one


def provenance(evidence,run,manifest,freeze,input_freeze,directory,manifest_path,freeze_path):
    base.PROBE=PROBE;base.CONTRACT=CONTRACT
    for key in ('native_source_bindings','source_bindings'):
        for b in manifest.get(key,[]):check(binding(ROOT/b['path'])['sha256']==b['sha256'],'native static source binding')
    projected=deepcopy(manifest);projected['probe'].pop('evidence_id',None)
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
    check(len(vs)==cap<=16 and (t['pointers']['dependent']=='0x0')==(cap==0),'dependent pointer/capacity')
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


def nested_table(r,id):return r['fstres'if id==94 else'grndat']['property_tables'][hex(OFFSETS[id])]


def line_helper(e,data,kinds,fmt):
    content,_,eof,after=base.line_read(data,base.stream_position(e['before'],data))
    check(e['after']==after and e['lines']==[{'limit':1024,'eof':eof,'hex':None if eof else content.hex()}],'line helper exact source and cursor')
    result=[0 if k else 0.0 for k in kinds];conversion=-1;scans=[]
    if not eof:
        normalized=content.replace(b',',b' ');pos=0;conversion=0
        for i,integer in enumerate(kinds):
            value,c,pos=lex(normalized,pos,integer)
            if c!=1:
                if not i:conversion=c
                break
            result[i]=value;conversion+=1
        scans=[{'format':fmt,'input_hex':normalized.hex(),'conversions':conversion}]
    check(e['scans']==scans and e['status']==int(conversion==-1),'line helper sscanf and status')
    check(e['values']==result and e['output_hex']==[raw([v],k)for v,k in zip(result,kinds)],'line helper zero/partial output bytes')
    return result,e['status'],after



def record_shape(r):
    projected=deepcopy(r);projected['field_0x17e8']=0;refs.record_shape(projected,{})
    check(r['field_0x17e8']==r['grndat']['selector_0x17e8']and r['field_0x17e8']in(0,4),'selected GRNDAT selector projections')
    check(r['grndat']['secondary_0x17ec']==0,'unused GRNDAT secondary remains constructor zero')
    check(r['fstres']['mode_0x50']in(1,11),'selected FSTRES mode')
    check(len(r['fstres']['coefficients_0x368'])==4,'four coefficients');floats(r['fstres']['coefficients_0x368'],r['fstres']['coefficients_0x368_hex'])
    for id in IDS:
        t=nested_table(r,id);table_shape(t);check(int(t['address'],16)==int(r['address'],16)+OFFSETS[id],'nested table object offset')
    for t in r['property_tables'].values():old_tables.table_shape(t)


def defaults(r):
    record_shape(r)
    check(r['fstres']['mode_0x50']==1 and r['fstres']['coefficients_0x368_hex']=='00'*32,'FSTRES constructor mode and four +0 doubles')
    a=nested_table(r,94);want=empty_table(a,174);want['constant_0x08']=1.;want['raw_hex']['constant']=raw([1.]);table_equal(a,want,'FSTRES constructor type174 scalar1')
    a=nested_table(r,108);want=empty_table(a,172);want['rank_0x04']=2;want['selectors_0x10']=[3,8,0];table_equal(a,want,'GRNDAT constructor type172 rank2 empty axes')
    check(r['field_0x17e8']==r['grndat']['secondary_0x17ec']==0,'GRNDAT constructor flags')


def handler(es,data):
    first,last=es[0],es[-1];id=first['id'];check(id==last['id']and first['category']==last['category']==4,'nested material handler identity')
    sel=one(es,'material-selector');check(sel['id']==id and sel['before']==first['stream'],'selector ABI/cursor')
    selected=refs.verify_selector({**sel,'id':92},data);check(selected is not None,'defined material selector')
    old=first['state']['material_list']['records'];created=refs.find(old,selected)is None
    appends=[e for e in es if e['operation']=='material-append-after'];creation=[e for e in es if e['operation']=='material-refresh-after'and e['option']==1]
    check(len(appends)==len(creation)==int(created),'first creation alone appends and refreshes option1')
    expected=deepcopy(creation[0]['state']if created else first['state'])
    if created:
        fresh=appends[0]['state']['records'][-1];defaults(fresh);check(fresh['id']==0,'append initializes ID zero before assignment')
        check(appends[0]['sequence']<creation[0]['sequence'],'append before creation refresh')
        check(expected['material_count']==len(old)+1 and[r['id']for r in expected['material_list']['records']]==[r['id']for r in old]+[selected],'stable insertion exact signed ID')
        maps=[e for e in es if e['operation']in('material-scratch-before','material-scratch-after')]
        check(len(maps)==4 and[e['mode']for e in maps]==[0,0,1,1],'creation scratch allocate then cleanup')
        check(maps[0]['state']==first['state']['material_list'],'scratch prior list')
        ids=[r['id']for r in old];ranks=[sum(x<id for x in ids)for id in ids];scratch=maps[1]['state']['scratch']
        check(scratch['allocation_observed']and scratch['count']==len(ids)and scratch['ids']==ids and scratch['ranks']==ranks,'scratch exact old IDs/sorted ranks')
        check(maps[1]['sequence']<appends[0]['sequence']<creation[0]['sequence']<maps[2]['sequence']<maps[3]['sequence'],'scratch encloses creation refresh')
        check(maps[2]['state']==expected['material_list'],'cleanup receives refreshed material list')
        expected['material_list']['scratch']={'ids_pointer':'0x0','ranks_pointer':'0x0','allocation_observed':False,'count':None,'ids':None,'ranks':None}
        check(maps[3]['state']==expected['material_list'],'scratch cleanup before outer payload')
    r=refs.find(expected['material_list']['records'],selected);check(r is not None,'selected live record')
    cursor=sel['after'];used=set()
    def row(role):
        nonlocal cursor
        b=next(e for e in es if e['operation']=='nested-material-row-before'and e['role']==role);a=next(e for e in es if e['operation']=='nested-material-row-after'and e['serial']==b['serial']);used.add(b['serial'])
        kind,helper,ret=({94:('int3',0x413fb0,0x384c3a),108:('int2',0x413ee0,0x386c3b)}[id]if role=='outer'else('real4',0x414e40,0x385896))
        check(b['id']==a['id']==id and b['role']==a['role']==role and b['kind']==a['kind']==kind,'nested row scope')
        check(b['helper_rva']==helper and b['completion_rva']==a['completion_rva']==ret,'nested row native call continuation')
        check(b['before']==a['before']==cursor and b['material']==r,'row receives exact prior material/cursor')
        kinds=[False]*4 if role=='coefficients'else[True]*(3 if id==94 else 2);fmt=' '.join('%i'if k else'%Flf'for k in kinds)
        values,status,cursor=line_helper(a,data,kinds,fmt)
        if role=='coefficients':r['fstres']['coefficients_0x368']=values;r['fstres']['coefficients_0x368_hex']=raw(values)
        check(a['material']==r,'helper changes only live coefficient destinations; outerlocals do not store yet')
        return values,status,a['sequence']
    v,outer_status,lastseq=row('outer');status=outer_status;coeff_status=None
    starts=[e for e in es if e['operation']=='property-reader-before'];ends=[e for e in es if e['operation']=='property-reader-after']
    post_before=[e for e in es if e['operation']=='material-refresh-before'and e['option']==0];post_after=[e for e in es if e['operation']=='material-refresh-after'and e['option']==0]
    if outer_status==0:
        check(v[0]==(14 if id==94 else 4),'selected supported outer branch')
        if id==94:
            r['fstres']['mode_0x50']=11;_,coeff_status,lastseq=row('coefficients')
        else:r['field_0x17e8']=r['grndat']['selector_0x17e8']=4
        check(len(starts)==len(ends)==1,'generic table called regardless coefficient result')
        b,a=starts[0],ends[0];check(lastseq<b['sequence']and b['before']==cursor,'table follows outer/coefficient row')
        table_equal(b['table'],nested_table(r,id),'table preserves initial or prior state before header')
        check(b['aux_flag']==0 and b['aux_pointer']=='0x0','nested table flag0/null auxiliary')
        frame=[e for e in es if b['sequence']<=e['sequence']<=a['sequence']];t,status,cursor,_=wrapper(frame,data,b,a)
        r['fstres'if id==94 else'grndat']['property_tables'][hex(OFFSETS[id])]=t
        if id==108:
            check(len(post_before)==len(post_after)==1 and a['sequence']<post_before[0]['sequence']<post_after[0]['sequence'],'GRNDAT always refresh0 after table, including failure')
            check(post_before[0]['state']==expected,'GRNDAT table/selector mutation before refresh')
            after=post_after[0]['state'];x=deepcopy(after);x['objects']=deepcopy(expected['objects'])
            check(x==expected,'disabled grain refresh changes only selected object storage metadata')
            expected=deepcopy(after)
        else:check(not post_before and not post_after,'FSTRES no extra refresh')
    else:check(not starts and not ends and not post_before and not post_after,'outer failure skips mutations/table/postrefresh')
    check(len(used)==sum(e['operation']=='nested-material-row-before'for e in es),'complete nested row call inventory')
    check(last['status']==(2 if status else 0)and last['stream']==cursor,'nested handler final status/cursor')
    check(last['state']==expected,'entire persistent material/root state after nested branch')
    return {'id':id,'dimension':first['state']['dimension'],'material_id':selected,'created':created,'outer_status':outer_status,'coefficient_status':coeff_status,'status':last['status'],'post_refreshes':len(post_after),'sequence':first['sequence']}


def validate(evidence,inputs):
    cap=evidence['normalized_result'];messages=cap['messages']
    check(len(cap['scripts'])==1 and all(m['type']=='send'and m['script_id']==cap['scripts'][0]['script_id']for m in messages),'single passive script without runtime errors')
    check(messages[0]['payload']['operation']=='hooks-installed'and messages[0]['payload']['pid']==cap['pid'],'hook process identity')
    events=[m['payload']for m in messages[1:]];check([e['sequence']for e in events]==list(range(1,len(events)+1)),'complete event order')
    check(not any(e['operation']=='probe-error'for e in events),'zero probe errors required')
    rows=descriptor_map(read(ROOT/'analysis/mesh-array-contract.json'));groups=[];g=None;feature_count=0
    for e in events:
        for r in refs.records_in(e):record_shape(r)
        for ls in refs.lists_in(e):refs.list_shape(ls)
        for ls in relation_props.relations.nested_relation_lists(e):relation_props.relation_list(ls)
        if 'state'in e and 'objects'in e['state']:
            s=e['state'];check(s['grain_gate_0x864']==0,'native startup grain gate zero; no injected enabling value')
            for o in s['objects']:base.shared.object_shape(o,rows)
        if e['operation']=='nested-material-feature':
            check(e['feature']==53 and e['gate_0x864']==0 and e['enabled']is False and e['assigned']is False and e['output']is None,'disabled query53 leaves output unassigned/unread');feature_count+=1
        if e['operation']=='material-append-after':defaults(e['state']['records'][-1])
        if e['operation']=='import-before':check(g is None,'non-nested imports');g=[]
        check(g is not None,'events inside imports');g.append(e)
        if e['operation']=='import-after':groups.append(g);g=None
    check(g is None and[g[0]['file']for g in groups]==list(inputs),'complete ordered fixture imports')
    catalog={r['name']:(r['id'],r['category'])for r in read(ROOT/'analysis/keyword-catalog.json')['rows']};results=[];previous=None;refresh_count=0
    for g in groups:
        data=inputs[g[0]['file']];start=None;refresh_start=None;last_state=None;token=None;statuses=[]
        if previous is not None:check(g[0]['state']==previous,'full state continuity between imports')
        previous=g[-1]['state']
        boundary=next(e for e in g if e['operation']in('handler-before','finalize-before'))['state']
        if boundary['dimension']!=g[0]['state']['dimension']:check(not boundary['objects']and not boundary['material_list']['records']and not boundary['relation_list']['records'],'dimension change destroys nested material/relation payloads')
        else:check(boundary['material_list']['records']==g[0]['state']['material_list']['records'],'same dimension retains nested tables before parsing')
        for i,e in enumerate(g):
            op=e['operation']
            if op=='stream-scan'and e['format']=='%39s':refs.verify_token(e,data);token=e
            elif op=='keyword-resolved':check(token is not None and token['value']==e['token']and(e['id'],e['category'])==catalog.get(e['token'],(305,0)),'source token/catalog resolution')
            if op=='material-refresh-before':check(refresh_start is None,'refresh pairing');refresh_start=i
            elif op=='material-refresh-after':
                check(refresh_start is not None,'refresh return pairing');base.shared.refresh_events(g[refresh_start:i+1]);refresh_start=None;refresh_count+=1
            if op=='handler-before':
                check(start is None,'handler pairing');start=i
                if last_state is not None:check(e['state']==last_state,'full state continuity between handlers')
            elif op=='handler-after':
                check(start is not None,'handler return pairing');frame=g[start:i+1];start=None;statuses.append(e['status'])
                if e['id']in IDS:results.append(handler(frame,data))
                elif e['id']in(135,136):relation_props.handler(frame,data)
                else:check(frame[0]['state']['material_list']['records']==e['state']['material_list']['records'],'other fixture handlers retain all nested material fields')
                last_state=e['state']
        check(start is None and refresh_start is None,'all handler/refresh frames closed')
        before,after=one(g,'finalize-before'),one(g,'finalize-after')
        if last_state is not None:check(before['state']==last_state,'finalizer starts from last handler')
        expected=deepcopy(before['state'])
        check(all(r['mode_0x1cc8']==0 for r in expected['material_list']['records']),'selected no material reference finalization effects')
        if expected['finalization_controls']['control_0x93c']==0:
            mode=int(expected['finalization_controls']['step_increment_0x930']<=0);expected['finalization_controls']['mode_0x990']=expected['time_table']['mode_0x58']=mode
        check(all(o['flag_0x3990']==0 for o in expected['objects']),'no scientific geometry finalization in selected fixtures')
        check(after['state']==expected==g[-1]['state'],'finalization preserves nested tables and only normalizes selected controls')
        check(g[-1]['status']==next((s for s in reversed(statuses)if s),0),'retained import failure status')
    check({(r['id'],r['dimension'])for r in results}=={(i,d)for i in IDS for d in(2,3)},'both nested routes in both dimensions')
    return {'imports':len(groups),'handlers':results,'refreshes':refresh_count,'disabled_feature53_queries':feature_count,'coverage':{'id_dimensions':sorted({(r['id'],r['dimension'])for r in results}),'failures':sum(r['status']!=0 for r in results),'ignored_failed_coefficients':sum(r['coefficient_status']==1 and r['status']==0 for r in results),'GRNDAT_failed_table_with_refresh':sum(r['id']==108 and r['status']==2 and r['post_refreshes']==1 for r in results)},'event_counts':dict(Counter(e['operation']for e in events))}


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for k,v in [('capture','analysis/nested-material-runtime-training.json'),('run-record','analysis/oracle/nested-material-training/run.json'),('manifest','analysis/nested-material-evaluation-cases.json'),('freeze','analysis/nested-material-candidate-freeze.json'),('input-freeze','analysis/nested-material-input-freeze.json'),('output','analysis/nested-material-observations.json')]:p.add_argument('--'+k,type=Path,default=ROOT/v)
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
    tamper('event order','nested-material-row-before',lambda e:e.update(sequence=0))
    tamper('selector source cursor','material-selector',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('constructor FSTRES mode','material-append-after',lambda e:e['state']['records'][-1]['fstres'].update(mode_0x50=11))
    tamper('constructor FSTRES scalar','material-append-after',lambda e:nested_table(e['state']['records'][-1],94).update(constant_0x08=0.))
    tamper('constructor GRNDAT type','material-append-after',lambda e:nested_table(e['state']['records'][-1],108).update(type_0x00=0))
    tamper('outer partial values','nested-material-row-after',lambda e:e['values'].__setitem__(0,999),lambda e:e['role']=='outer')
    tamper('coefficient raw bytes','nested-material-row-after',lambda e:e['output_hex'].__setitem__(0,raw([999.])),lambda e:e['role']=='coefficients')
    tamper('row helper status','nested-material-row-after',lambda e:e.update(status=1-e['status']))
    tamper('ignored failed coefficient status','nested-material-row-after',lambda e:e.update(status=0),lambda e:e['role']=='coefficients'and e['status']==1)
    tamper('row helper cursor','nested-material-row-after',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('row source line','nested-material-row-after',lambda e:e['lines'][0].update(hex='3939390a'),lambda e:bool(e['lines'])and not e['lines'][0]['eof'])
    tamper('row continuation','nested-material-row-before',lambda e:e.update(completion_rva=0))
    tamper('outer premature material mutation','nested-material-row-after',lambda e:e['material']['fstres'].update(mode_0x50=11),lambda e:e['role']=='outer'and e['material']['fstres']['mode_0x50']==1)
    tamper('table wrapper ABI','property-reader-before',lambda e:e.update(aux_flag=1))
    tamper('table wrapper continuation','property-reader-after',lambda e:e.update(completion_rva=0))
    tamper('table wrapper status','property-reader-after',lambda e:e.update(status=1-e['status']))
    tamper('table reset type','property-reset',lambda e:e['table'].update(type_0x00=999))
    tamper('table reset preserved54','property-reset',lambda e:e['table'].update(preserved_0x54=1))
    tamper('header conversion count','property-header-scan',lambda e:e.update(conversions=0),lambda e:e['conversions']!=0)
    tamper('header partial output','property-header-scan',lambda e:e['output_hex'].__setitem__(1,raw([999.])))
    tamper('table logical count','property-data-before',lambda e:e['table']['counts_0x1c'].__setitem__(0,3),lambda e:e['table']['counts_0x1c'][0]!=3)
    tamper('table selector','property-data-before',lambda e:e['table']['selectors_0x10'].__setitem__(0,99))
    tamper('calloc zero default','property-allocation',lambda e:(e['table']['dependent'].__setitem__(0,1.),e['table']['raw_hex'].update(dependent=raw([1.]+e['table']['dependent'][1:]))),lambda e:bool(e['table']['dependent']))
    tamper('physical capacity','property-allocation',lambda e:e['table']['physical_capacities'].update(dependent=99))
    tamper('scalar source value','property-data-scan',lambda e:e.update(value=999.,output_hex=raw([999.])),lambda e:e['conversions']==1)
    tamper('scalar destination','property-data-scan',lambda e:e.update(destination=hex(int(e['destination'],16)+8)))
    tamper('scalar index order','property-data-scan',lambda e:e['location'].update(index=e['location']['index']+1))
    tamper('scalar EOF','property-data-scan',lambda e:e.update(conversions=0),lambda e:e['conversions']==-1)
    tamper('scalar cursor','property-data-scan',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('temporary initial assigned mask','property-temporary-allocated',lambda e:e['assigned'].__setitem__(0,raw([0.])),lambda e:e['count']>0)
    tamper('temporary copied bytes','property-temporary-committed',lambda e:e['assigned'].__setitem__(0,raw([999.])),lambda e:bool(e['assigned']))
    tamper('temporary free pointer','property-temporary-freed',lambda e:e.update(pointer='0x1'))
    tamper('native staging EOF leak hidden','property-data-after',lambda e:e['temporary'].update(freed=True),lambda e:e['temporary']is not None and not e['temporary']['freed'])
    tamper('partial dependent commit on EOF','property-data-after',lambda e:(e['table']['dependent'].__setitem__(0,1.),e['table']['raw_hex'].update(dependent=raw([1.]+e['table']['dependent'][1:]))),lambda e:e['status']==1 and bool(e['table']['dependent']))
    tamper('discard source line','property-discard',lambda e:e.update(hex='780a'),lambda e:not e['eof'])
    tamper('GRNDAT postrefresh option','material-refresh-before',lambda e:e.update(option=1),lambda e:e['option']==0)
    tamper('disabled feature assigned output','nested-material-feature',lambda e:e.update(assigned=True,output=0))
    tamper('disabled feature enabled','nested-material-feature',lambda e:e.update(enabled=True))
    tamper('root startup gate','import-before',lambda e:e['state'].update(grain_gate_0x864=1))
    tamper('GRNDAT secondary mutation','handler-after',lambda e:e['state']['material_list']['records'][0]['grndat'].update(secondary_0x17ec=7),lambda e:e['id']==108)
    tamper('finalization nested table mutation','finalize-after',lambda e:nested_table(e['state']['material_list']['records'][0],94).update(type_0x00=999),lambda e:bool(e['state']['material_list']['records']))
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
    report={'status':'PASS','result':result,'bindings':[binding(x)for x in(Path(__file__),args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze,PROBE,CONTRACT)],'tampering_rejected':rejected,'tampering_not_applied':skipped,'dependencies':[binding(ROOT/'tools'/n)for n in('verify_property_table_observations.py','verify_relation_property_observations.py','verify_relation_observations.py','verify_material_column_observations.py','verify_material_reference_observations.py','verify_custom_field_observations.py','verify_boundary_sort_observations.py','verify_mesh_array_observations.py','verify_material_object_observations.py','verify_material_import_observations.py','capture_oracle.py')],'limitations':['Original-only FSTRES94 mode14 and GRNDAT108 selector4, with finite initialized generic tables; no candidate/comparator outputs used.','Native gate864 remains observed zero. Enabling producer, enabled feature53 side effects, other outer modes and scientific table evaluation remain outside this receipt.','Overflow, nonfinite values, allocation failure, and uninitialized staged matching failures are excluded.','Rank2 staging EOF retains the native leak in observations; owned C++ resource cleanup is a separate lifetime boundary.']}
    args.output.write_text(json.dumps(report,indent=2)+'\n');print('PASS',len(result['handlers']),'handlers;',len(rejected),'actual corruption checks;',len(skipped),'not applicable')


if __name__=='__main__':main()


