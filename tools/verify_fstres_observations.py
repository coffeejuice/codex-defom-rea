"""Original-only audit for complete FSTRES94 family and GRNDAT108 regression.

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

PROBE=ROOT/'tools/observe_fstres_import.js'
CONTRACT=ROOT/'analysis/fstres-static-contract.json'
IDS=(94,108)
OFFSETS={94:0x388,108:0x1740}
RETURNS={94:0x3858bc,108:0x387923}
raw,one=base.raw,base.one
STATIC=read(CONTRACT)
PROFILES={int(x['type'],16):x for x in STATIC['owned_model']['table_defaults']}
CALLS={x['role']:x for x in STATIC['calls']}
TABLE_OFFSETS={x['offset'] for x in STATIC['owned_model']['table_defaults']}
DS={int(g['offset'],16)+8*i for g in STATIC['owned_model']['initialized_double_groups'] for i in range(g['count'])}


def physical_total(t):
    w=t['values_per_point_0x40'];rank=t['rank_0x04']
    if rank==0:return w if w>1 or t['type_0x00']in(0xa7,0xa8,0xaa,0xab,0xb5)else 0
    return max(0,w*math.prod(t['counts_0x1c'][:rank]))


def total(t):
    return 0 if t['rank_0x04']==0 and t['values_per_point_0x40']<=1 else physical_total(t)


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
    check(int(t['address'],16)>0 and t['rank_0x04']in range(4),'table address/rank')
    check(-4<=t['values_per_point_0x40']<=16 and t['pointers']['cache_0x68']=='0x0','table bounded width/null cache')
    check(len(t['counts_0x1c'])==len(t['selectors_0x10'])==3 and all(type(n)is int and -4<=n<=3 for n in t['counts_0x1c']),'bounded axis metadata')
    check(t['flag_0x50']in(0,1) and t['preserved_0x54']==t['mode_0x58']==0 and t['preserved_0x60_hex']=='00'*8,'selected preserved table metadata')
    floats([t['constant_0x08']],t['raw_hex']['constant'])
    check(len(t['axes'])==len(t['raw_hex']['axes'])==len(t['pointers']['axes'])==len(t['physical_capacities']['axes'])==3,'three physical axes')
    for values,h,p,n in zip(t['axes']+[t['dependent']],t['raw_hex']['axes']+[t['raw_hex']['dependent']],t['pointers']['axes']+[t['pointers']['dependent']],t['physical_capacities']['axes']+[t['physical_capacities']['dependent']]):
        floats(values,h);check(len(values)==n and 0<=n<=32 and (p=='0x0')==(n==0),'physical allocation length/pointer')
    ps=[p for p in t['pointers']['axes']+[t['pointers']['dependent']]if p!='0x0']
    check(len(ps)==len(set(ps)),'owned table allocations distinct')


def equal(a,b,label):
    table_shape(a);table_shape(b);check(a==b,label)


def allocation(expected,observed):
    w=deepcopy(expected);w['counts_0x1c']=[max(0,n)for n in w['counts_0x1c']];w['values_per_point_0x40']=max(1,w['values_per_point_0x40'])
    w['axes']=[[0.]*n if i<w['rank_0x04']else[]for i,n in enumerate(w['counts_0x1c'])];w['dependent']=[0.]*physical_total(w)
    w['raw_hex']['axes']=[raw(x)for x in w['axes']];w['raw_hex']['dependent']=raw(w['dependent'])
    w['physical_capacities']={'axes':[len(x)for x in w['axes']],'dependent':len(w['dependent'])};w['pointers']=deepcopy(observed['pointers'])
    equal(observed,w,'allocation normalizes counts/width and zeroes complete physical arrays');return w


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


def table_equal(a,b,label):return equal(a,b,label)

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
    check(a['serial']==b['serial'],'payload identity')
    if 'id'in b:check(a['id']==b['id']==94 and a['offset']==b['offset'] and a['completion_rva']==b['completion_rva'],'direct payload identity/return')
    else:check(a['parent_serial']==b['parent_serial']and a['completion_rva']==0x41b651,'wrapper payload continuation')
    want=deepcopy(b['table']);cursor=b['before'];status=0;rank=want['rank_0x04'];width=want['values_per_point_0x40']
    # Wrapper allocations carry the wrapper serial; direct allocations carry
    # this data serial. Both belong to the exact table within this call interval.
    allocs=[e for e in es if e['operation']=='property-allocation'and b['sequence']<e['sequence']<a['sequence'] and e['table']['address']==b['table']['address']]
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
        value,c,cursor=scalar(e,data,cursor)
        if c==-1:status=1;break
        if kind=='temporary':check(c==1,'uninitialized staged matching failure excluded');assigned[index]=raw([value])
        elif c==1:put(want,kind,index,value,axis)
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



def grndat_handler(es,data):
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




def profile(t,type_id):
    w=empty_table(t,type_id)
    if type_id in PROFILES:
        p=PROFILES[type_id];w.update(rank_0x04=p['rank'],selectors_0x10=p['selectors'][:],counts_0x1c=p['counts'][:],values_per_point_0x40=p['width'],constant_0x08=float(p['constant']))
        w['raw_hex']['constant']=raw([w['constant_0x08']])
    return w


def default_table(t,type_id):
    w=profile(t,type_id)
    return w if type_id==0x51 else allocation(w,t)


def fstres_shape(f):
    check(type(f['mode_0x50'])is int and -1<=f['mode_0x50']<=14,'stored FSTRES mode')
    check(set(f['property_tables'])==TABLE_OFFSETS,'ten FSTRES owners')
    check(set(f['scalar_fields'])=={hex(x)for x in DS if x<0x368}|{'0x478','0x660'},'all initialized FSTRES scalar fields')
    for off,x in f['scalar_fields'].items():
        if int(off,16)in DS:check(x['type']=='binary64','binary64 coefficient');floats([x['value']],x['raw_hex'])
        else:check(x['type']=='int32'and type(x['value'])is int and -2**31<=x['value']<2**31,'signed FSTRES int32')
    check(len(f['coefficients_0x368'])==4,'four existing coefficients');floats(f['coefficients_0x368'],f['coefficients_0x368_hex'])
    for t in f['property_tables'].values():table_shape(t)
    extents=[]
    for t in f['property_tables'].values():
        for ptr,n in zip(t['pointers']['axes']+[t['pointers']['dependent']],t['physical_capacities']['axes']+[t['physical_capacities']['dependent']]):
            if n:extents.append((int(ptr,16),int(ptr,16)+n*8))
    extents.sort();check(all(a[1]<=b[0]for a,b in zip(extents,extents[1:])),'ten owned table payloads do not alias')


def fstres_defaults(f):
    fstres_shape(f);check(f['mode_0x50']==1 and f['coefficients_0x368_hex']=='00'*32,'FSTRES mode1 and coefficients+0')
    for x in f['scalar_fields'].values():check(x['value']==0 and(x['type']=='int32'or x['raw_hex']=='00'*8),'constructor initialized scalar zero')
    for off,t in f['property_tables'].items():
        kind=next(int(p['type'],16)for p in STATIC['owned_model']['table_defaults']if p['offset']==off)
        equal(t,default_table(t,kind),'constructor typed profile and physical allocation')


def record_shape(r):
    projected=deepcopy(r);projected['field_0x17e8']=0;refs.record_shape(projected,{})
    check(r['field_0x17e8']==r['grndat']['selector_0x17e8']and r['field_0x17e8']in(0,4),'GRNDAT projection')
    check(r['grndat']['secondary_0x17ec']==0,'retained unused secondary')
    fstres_shape(r['fstres'])
    for off,t in r['fstres']['property_tables'].items():check(int(t['address'],16)==int(r['address'],16)+int(off,16),'FSTRES table owner offset')
    for t in r['property_tables'].values():old_tables.table_shape(t)
    old_tables.table_shape(nested_table(r,108))


def defaults(r):
    record_shape(r);fstres_defaults(r['fstres']);t=nested_table(r,108);w=empty_table(t,172);w['rank_0x04']=2;w['selectors_0x10']=[3,8,0]
    equal(t,w,'GRNDAT constructor empty rank2 type172');check(r['field_0x17e8']==0,'constructor GRNDAT selector zero')


class NativeStop(Exception):pass


def fstres_handler(es,data):
    first,last=es[0],es[-1];check(first['id']==last['id']==94 and first['category']==last['category']==4,'FSTRES handler identity')
    sel=one(es,'material-selector');selected=refs.verify_selector({**sel,'id':92},data);check(selected is not None and sel['before']==first['stream'],'defined signed material selector')
    old=first['state']['material_list']['records'];created=refs.find(old,selected)is None
    appends=[e for e in es if e['operation']=='material-append-after'];creation=[e for e in es if e['operation']=='material-refresh-after'and e['option']==1]
    check(len(appends)==len(creation)==int(created),'append and creation refresh exactly once')
    expected=deepcopy(creation[0]['state']if created else first['state'])
    if created:
        fresh=appends[0]['state']['records'][-1];defaults(fresh);check(fresh['id']==0,'constructor ID before assignment')
        check(expected['material_count']==len(old)+1 and[r['id']for r in expected['material_list']['records']]==[r['id']for r in old]+[selected],'stable insertion order')
        maps=[e for e in es if e['operation']in('material-scratch-before','material-scratch-after')]
        check(len(maps)==4 and[e['mode']for e in maps]==[0,0,1,1],'creation scratch phases')
        check(maps[0]['state']==first['state']['material_list'],'scratch original list');ids=[r['id']for r in old];sc=maps[1]['state']['scratch']
        check(sc['allocation_observed']and sc['count']==len(ids)and sc['ids']==ids and sc['ranks']==[sum(x<id for x in ids)for id in ids],'old signed ranks')
        check(maps[1]['sequence']<appends[0]['sequence']<creation[0]['sequence']<maps[2]['sequence']<maps[3]['sequence'],'creation phase order')
        check(maps[2]['state']==expected['material_list'],'scratch receives refreshed state')
        expected['material_list']['scratch']={'ids_pointer':'0x0','ranks_pointer':'0x0','allocation_observed':False,'count':None,'ids':None,'ranks':None}
        check(maps[3]['state']==expected['material_list'],'scratch cleanup before payload')
    r=refs.find(expected['material_list']['records'],selected);f=r['fstres'];cursor=sel['after'];used=set();lastseq=sel['sequence'];mode=None;status=0;reads=[];derived=None
    def sf(off,value):
        if 0x368<=off<=0x380:
            f['coefficients_0x368'][(off-0x368)//8]=value;f['coefficients_0x368_hex']=raw(f['coefficients_0x368'])
        else:
            x=f['scalar_fields'][hex(off)];x['value']=value
            if x['type']=='binary64':x['raw_hex']=raw([value])
    def table(off):return f['property_tables'][hex(off)]
    def start(role,op):
        nonlocal lastseq
        c=CALLS[role];ret=int(c['return'],16)-0x140000000
        e=next(e for e in es if e['operation']==op and e['completion_rva']==ret)
        check(e['sequence']>lastseq and e['serial']not in used,'ordered exact native call');used.add(e['serial']);lastseq=e['sequence'];return e,c
    def row(role,kind,live=None):
        nonlocal cursor,lastseq
        b,c=start(role,'fstres-row-before');a=next(e for e in es if e['operation']=='fstres-row-after'and e['serial']==b['serial'])
        check(a['completion_rva']==b['completion_rva']and b['helper_rva']==int(c['callee'],16)-0x140000000 and b['kind']==a['kind']==kind,'line helper ABI')
        check(b['fstres']==f and b['before']==cursor and b['material']==r['address'],'line helper prior state/cursor')
        kinds=[kind.startswith('int')]*int(re.search(r'\d+',kind)[0]);fmt=' '.join('%i'if x else'%Flf'for x in kinds)
        values,st,cursor=line_helper(a,data,kinds,fmt)
        if live is not None:
            for i,off in enumerate(live):
                if isinstance(off,tuple):table(off[0])[off[1]][off[2]]=values[i]
                elif off==0x628:table(0x5e8)['values_per_point_0x40']=values[i]
                else:sf(off,values[i])
        check(a['fstres']==f,'exact initialized partial helper writes');lastseq=a['sequence'];reads.append({'role':role,'status':st});return values,st
    def reset(role,off,kind):
        nonlocal lastseq
        b,c=start(role,'fstres-reset-before');a=next(e for e in es if e['operation']=='fstres-reset-after'and e['serial']==b['serial'])
        check(b['offset']==a['offset']==off and b['type']==a['type']==kind and a['completion_rva']==b['completion_rva'],'typed reset ABI')
        equal(b['table'],table(off),'typed reset prior state');w=profile(table(off),kind)
        allocs=[e for e in es if e['operation']=='property-allocation'and e['serial']==b['serial']]
        check(len(allocs)==int(kind!=0x51),'type51 reset defers allocation')
        if allocs:w=allocation(w,allocs[0]['table']);check(b['sequence']<allocs[0]['sequence']<a['sequence'],'reset allocation order')
        equal(a['table'],w,'typed reset exact result');f['property_tables'][hex(off)]=w;lastseq=a['sequence']
    def body(role,off):
        nonlocal lastseq,cursor
        b,c=start(role,'fstres-data-before');a=next(e for e in es if e['operation']=='fstres-data-after'and e['serial']==b['serial'])
        check(b['offset']==off and b['before']==cursor,'payload caller offset/cursor');equal(b['table'],table(off),'payload header state')
        w,st,cursor=payload(es,data,b,a);f['property_tables'][hex(off)]=w;lastseq=a['sequence'];return st
    def require(st):
        if st:raise NativeStop()
    def configure(off,opt,values,axis):
        t=table(off)
        if opt==0:t['constant_0x08']=values[0];t['raw_hex']['constant']=raw([values[0]])
        elif opt in(1,2,3):
            t['rank_0x04']=2 if opt==3 else 1;t['selectors_0x10'][0]=3 if opt!=2 else axis;t['counts_0x1c'][0]=math.trunc(values[0])
            if opt==3:t['selectors_0x10'][1]=axis;t['counts_0x1c'][1]=math.trunc(values[1])
    try:
        v,status=row('outer','int3');require(status);mode,o1,o2=v
        if mode==0:f['mode_0x50']=-1
        elif mode<0:f['mode_0x50']=14;sf(0x660,struct.unpack('<i',struct.pack('<I',(-mode)&0xffffffff))[0])
        elif mode in(1,4,5,11):
            stored,off,n={1:(0,0x58,4),4:(2,0xf0,5),5:(3,0x118,4),11:(8,0x340,5)}[mode];f['mode_0x50']=stored
            _,status=row('mode'+str(mode)+'-real'+str(n),'real'+str(n),[off+8*i for i in range(n)])
        elif mode in(2,3,7,8):
            off,kind,stored,prefix=(0x78,0x4f,1,'mode2or3')if mode in(2,3)else(0x228,0x50,5,'mode7or8');f['mode_0x50']=stored
            reset(prefix+'-reset',off,kind);_,status=row(prefix+'-counts','int3',[(off,'counts_0x1c',i)for i in range(3)]);require(status)
            status=body(prefix+'-data',off);table(off)['flag_0x50']=int(mode in(2,7))
        elif mode==6:
            f['mode_0x50']=4
            for label,off,kind,opt in [('first',0x1b0,0x24,o1),('second',0x138,0x25,o2)]:
                v,status=row('mode6-'+label+'-pair','real2');require(status);reset('mode6-'+label+'-reset',off,kind);configure(off,opt,v,12);status=body('mode6-'+label+'-data',off);require(status)
        elif mode==9:
            f['mode_0x50']=6
            for label,off in [('first',0x2a0),('second',0x2d8)]:_,status=row('mode9-'+label+'-real7','real7',[off+8*i for i in range(7)])
        elif mode==10:f['mode_0x50']=7;_,status=row('mode10-real7','real7',[0x310+8*i for i in range(6)])
        elif mode==12:
            f['mode_0x50']=9;v,status=row('mode12-pair','real2');require(status);reset('mode12-reset',0x400,0x26);configure(0x400,o1,v,8);status=body('mode12-data',0x400)
        elif mode in(13,15):
            f['mode_0x50']=10 if mode==13 else 12;v,status=row('mode13or15-count','int1',[0x478]);require(status);reset('mode13or15-first-reset',0x480,0xa6);table(0x480)['counts_0x1c'][0]=v[0];status=body('mode13or15-first-data',0x480);require(status)
            vals=[math.trunc(x)for x in floats(table(0x480)['axes'][0],table(0x480)['raw_hex']['axes'][0])];summ,prod=0,1
            for value in vals:summ+=value;prod*=value;check(-2**31<=summ<2**31 and -2**31<=prod<2**31,'bounded native32 arithmetic')
            derived={'axis_ints':vals,'sum':summ,'product':prod}
            for label,off,kind,width in [('sum',0x4f8,0xa7,summ),('product',0x570,0xa8,prod)]:reset('mode13or15-'+label+'-reset',off,kind);table(off)['values_per_point_0x40']=width;status=body('mode13or15-'+label+'-data',off);require(status)
        elif mode==14:
            f['mode_0x50']=11;_,status=row('mode14-real4','real4',[0x368+8*i for i in range(4)])
            b=one(es,'property-reader-before');a=one(es,'property-reader-after');check(b['sequence']>lastseq and b['before']==cursor,'mode14 unconditional wrapper ordering');equal(b['table'],table(0x388),'mode14 prior table')
            w,status,cursor,_=wrapper([e for e in es if b['sequence']<=e['sequence']<=a['sequence']],data,b,a);f['property_tables']['0x388']=w
        elif mode==16:
            f['mode_0x50']=13;reset('mode16-reset',0x5e8,0x51);_,status=row('mode16-width','int1',[0x628]);require(status);_,status=row('mode16-counts','int3',[(0x5e8,'counts_0x1c',i)for i in range(3)]);require(status);status=body('mode16-data',0x5e8);table(0x5e8)['flag_0x50']=0
    except NativeStop:status=1
    check(len(used)==sum(e['operation']in('fstres-row-before','fstres-reset-before','fstres-data-before')for e in es),'complete reached branch call inventory')
    check(sum(e['operation']=='property-reader-before'for e in es)==int(mode==14),'only reached mode14 invokes generic table wrapper')
    check(not any(e['operation']=='material-refresh-before'and e['option']==0 for e in es),'FSTRES no eager postpayload refresh')
    check(last['state']==expected and last['stream']==cursor and last['status']==(2 if status else 0),'full FSTRES persistent state/status/cursor')
    return {'id':94,'dimension':first['state']['dimension'],'material_id':selected,'created':created,'mode':mode,'status':last['status'],'reads':reads,'derived':derived,'sequence':first['sequence']}
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
    constructors(events)
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
                if e['id']==94:results.append(fstres_handler(frame,data))
                elif e['id']==108:results.append(grndat_handler(frame,data))
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
    check(any(r['id']==94 for r in results),'selected FSTRES routes observed')
    return {'imports':len(groups),'handlers':results,'refreshes':refresh_count,'disabled_feature53_queries':feature_count,'coverage':{'id_dimensions':sorted({(r['id'],r['dimension'])for r in results}),'modes':sorted({r['mode']for r in results if r['id']==94 and r['mode']is not None}),'failures':sum(r['status']!=0 for r in results),'ignored_failed_coefficients':sum(any(x['role']=='mode14-real4'and x['status']==1 for x in r.get('reads',[]))and r['status']==0 for r in results),'GRNDAT_failed_table_with_refresh':sum(r['id']==108 and r['status']==2 and r.get('post_refreshes')==1 for r in results)},'event_counts':dict(Counter(e['operation']for e in events))}



def constructors(events):
    pending={}
    offsets=[0x78,0x138,0x1b0,0x228,0x388,0x400,0x480,0x4f8,0x570,0x5e8]
    typed=[(0x78,0x4f),(0x138,0x25),(0x1b0,0x24),(0x228,0x50),(0x388,0xae),(0x400,0x26),(0x480,0xa6),(0x4f8,0xa7),(0x570,0xa8)]
    for e in events:
        if e['operation']=='fstres-constructor-before':
            check(e['serial']not in pending,'constructor unique serial');pending[e['serial']]=(e,[])
        elif e['operation']=='property-allocation'and e['serial']in pending:pending[e['serial']][1].append(e)
        elif e['operation']=='fstres-constructor-after':
            check(e['serial']in pending,'paired constructor');before,allocs=pending.pop(e['serial']);check(before['material']==e['material'],'constructor owner stable')
            expected=[(off,0)for off in offsets]+typed;check(len(allocs)==len(expected),'ten type0 then nine typed allocation calls; type51 deferred')
            for a,(off,kind)in zip(allocs,expected):
                t=a['table'];check(int(t['address'],16)==int(e['material'],16)+off,'constructor exact owned table address');w=profile(t,kind);allocation(w,t)
            fstres_defaults(e['fstres'])
    check(not pending,'all constructor frames completed')

def main():
    p=argparse.ArgumentParser(description=__doc__)
    for k,v in [('capture','analysis/fstres-runtime-training-a.json'),('run-record','analysis/oracle/fstres-training-a-v2/run.json'),('manifest','analysis/fstres-training-a-cases.json'),('freeze','analysis/fstres-candidate-freeze.json'),('input-freeze','analysis/fstres-training-a-input-freeze.json'),('output','analysis/fstres-training-a-observations.json')]:p.add_argument('--'+k,type=Path,default=ROOT/v)
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
    tamper('event order','fstres-row-before',lambda e:e.update(sequence=0))
    tamper('selector source cursor','material-selector',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('constructor mode','fstres-constructor-after',lambda e:e['fstres'].update(mode_0x50=0))
    tamper('constructor physical special default','fstres-constructor-after',lambda e:e['fstres']['property_tables']['0x4f8']['dependent'].__setitem__(0,1.))
    tamper('constructor type51 deferred','fstres-constructor-after',lambda e:e['fstres']['property_tables']['0x5e8'].update(type_0x00=0))
    tamper('constructor scalar default','fstres-constructor-after',lambda e:e['fstres']['scalar_fields']['0x478'].update(value=1))
    tamper('constructor allocation owner','property-allocation',lambda e:e['table'].update(address=hex(int(e['table']['address'],16)+8)))
    tamper('physical capacity','property-allocation',lambda e:e['table']['physical_capacities'].update(dependent=99))
    tamper('calloc zero default','property-allocation',lambda e:(e['table']['dependent'].__setitem__(0,1.),e['table']['raw_hex'].update(dependent=raw([1.]+e['table']['dependent'][1:]))),lambda e:bool(e['table']['dependent']))
    tamper('row helper ABI','fstres-row-before',lambda e:e.update(helper_rva=0))
    tamper('row continuation','fstres-row-after',lambda e:e.update(completion_rva=0))
    tamper('row partial values','fstres-row-after',lambda e:e['values'].__setitem__(0,999))
    tamper('row raw output','fstres-row-after',lambda e:e['output_hex'].__setitem__(0,'00'*4),lambda e:e['kind'].startswith('int')and e['values'][0]!=0)
    tamper('row status','fstres-row-after',lambda e:e.update(status=1-e['status']))
    tamper('row cursor','fstres-row-after',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('row source bytes','fstres-row-after',lambda e:e['lines'][0].update(hex='3939390a'),lambda e:bool(e['lines'])and not e['lines'][0]['eof'])
    tamper('ignored seventh local','fstres-row-after',lambda e:e['values'].__setitem__(6,12345.),lambda e:e['kind']=='real7')
    tamper('helper prior persistent state','fstres-row-before',lambda e:e['fstres']['scalar_fields']['0x58'].update(value=999.,raw_hex=raw([999.])))
    tamper('typed reset metadata','fstres-reset-after',lambda e:e['table'].update(type_0x00=999))
    tamper('typed reset opaque retention','fstres-reset-after',lambda e:e['table'].update(preserved_0x54=1))
    tamper('typed reset owner','fstres-reset-before',lambda e:e.update(offset=e['offset']+8))
    tamper('payload logical count','fstres-data-before',lambda e:e['table']['counts_0x1c'].__setitem__(0,3),lambda e:e['table']['counts_0x1c'][0]!=3)
    tamper('payload width','fstres-data-before',lambda e:e['table'].update(values_per_point_0x40=16))
    tamper('payload axis selector','fstres-data-before',lambda e:e['table']['selectors_0x10'].__setitem__(0,99))
    tamper('payload status','fstres-data-after',lambda e:e.update(status=1-e['status']))
    tamper('payload native continuation','fstres-data-after',lambda e:e.update(completion_rva=0))
    tamper('scalar source value','property-data-scan',lambda e:e.update(value=999.,output_hex=raw([999.])),lambda e:e['conversions']==1)
    tamper('scalar destination','property-data-scan',lambda e:e.update(destination=hex(int(e['destination'],16)+8)))
    tamper('scalar index order','property-data-scan',lambda e:e['location'].update(index=e['location']['index']+1))
    tamper('scalar cursor','property-data-scan',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('temporary initial assignment mask','property-temporary-allocated',lambda e:e['assigned'].__setitem__(0,raw([0.])),lambda e:e['count']>0)
    tamper('temporary commit bytes','property-temporary-committed',lambda e:e['assigned'].__setitem__(0,raw([999.])),lambda e:bool(e['assigned']))
    tamper('temporary free pointer','property-temporary-freed',lambda e:e.update(pointer='0x1'))
    tamper('discard exact source','property-discard',lambda e:e.update(hex='780a'),lambda e:not e['eof'])
    tamper('stored mode mutation','handler-after',lambda e:e['state']['material_list']['records'][0]['fstres'].update(mode_0x50=14),lambda e:e['id']==94 and e['state']['material_list']['records'][0]['fstres']['mode_0x50']!=14)
    tamper('persistent negative reference','handler-after',lambda e:e['state']['material_list']['records'][0]['fstres']['scalar_fields']['0x660'].update(value=12345),lambda e:e['id']==94)
    tamper('early flag mutation','fstres-data-after',lambda e:e['table'].update(flag_0x50=1-e['table']['flag_0x50']))
    tamper('finalization retained table','finalize-after',lambda e:e['state']['material_list']['records'][0]['fstres']['property_tables']['0x78'].update(type_0x00=999),lambda e:bool(e['state']['material_list']['records']))
    tamper('creation refresh option','material-refresh-before',lambda e:e.update(option=0),lambda e:e['option']==1)
    tamper('root startup gate','import-before',lambda e:e['state'].update(grain_gate_0x864=1))
    tamper('disabled feature output','nested-material-feature',lambda e:e.update(assigned=True,output=0))
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
    report={'status':'PASS','result':result,'bindings':[binding(x)for x in(Path(__file__),args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze,PROBE,CONTRACT)],'tampering_rejected':rejected,'tampering_not_applied':skipped,'dependencies':[binding(ROOT/'tools'/n)for n in('verify_property_table_observations.py','verify_relation_property_observations.py','verify_relation_observations.py','verify_material_column_observations.py','verify_material_reference_observations.py','verify_custom_field_observations.py','verify_boundary_sort_observations.py','verify_mesh_array_observations.py','verify_material_object_observations.py','verify_material_import_observations.py','capture_oracle.py')],'limitations':['Original-only complete FSTRES94 family, source-bound constructors and physical tables; retained GRNDAT108 regression. No candidate/comparator outputs used.','Native gate864 remains observed zero. Enabling producer, enabled feature53 side effects, scientific table evaluation remains outside this receipt.','Overflow, nonfinite values, allocation failure, and uninitialized staged matching failures are excluded.','Rank2 staging EOF retains the native leak in observations; owned C++ resource cleanup is a separate lifetime boundary.']}
    args.output.write_text(json.dumps(report,indent=2)+'\n');print('PASS',len(result['handlers']),'handlers;',len(rejected),'actual corruption checks;',len(skipped),'not applicable')


if __name__=='__main__':main()


