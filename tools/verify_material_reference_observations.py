"""Original-only MSTMTR observations: allocation lifetime and immediate finalization.
No replacement executable or candidate-derived expectation is used.
"""
import argparse
from collections import Counter
from copy import deepcopy
import json
import math
from pathlib import Path
import re
import struct

from capture_oracle import digest
from verify_boundary_sort_observations import ROOT, binding, check, read
import verify_material_object_observations as shared
from verify_material_import_observations import name_shape, verify_name, verify_selector, verify_token
from verify_step_distance_observations import line_read
from verify_time_controls_observations import stream_position

PROBE=ROOT/'tools/observe_material_reference_import.js'
CONTRACT=ROOT/'analysis/material-reference-next-contract.json'
MODE='mode_0x1cc8';COUNT='reference_count_0x1ccc';PTR='reference_pointer_0x1cd0'
DEFAULTS={'field_0x4':0,'field_0x8':0,'field_0x17e8':0,'field_0x1e40':0,
          'field_0x2150':0,'field_0x2154':0,'field_0x2158':1,'field_0x215c':1}


def provenance(evidence,run,manifest,freeze,directory):
    # Reuse only established process/file identity checks, not material behavior.
    shared.PROBE=PROBE
    inputs=shared.provenance(evidence,run,manifest,freeze,directory)
    for b in read(CONTRACT)['sources']:
        check(digest(ROOT/b['path'])==b['sha256'],'reference static source identity')
    archive={f['path']:f for f in freeze['inputs']}
    for b in [manifest['probe']]+manifest['cases']:
        check(b['path'] in archive and archive[b['path']]['sha256']==b['sha256'],
              'frozen selected probe/fixture binding')
    return inputs


def ints(raw,values):
    check(type(values)is list and all(type(v)is int and -(2**31)<=v<2**31 for v in values),'signed reference values')
    check(raw==b''.join(struct.pack('<i',v)for v in values).hex(),'reference raw bytes')


def record_shape(r,live):
    check(type(r['id'])is int and -(2**31)<=r['id']<2**31 and int(r['address'],16)>0,'material ID/address')
    check(all(r[k]==v for k,v in DEFAULTS.items()),'selected material defaults')
    name_shape(r)
    a=r['reference_allocation'];n=r[COUNT];p=r[PTR]
    check(r[MODE] in (0,1,2) and type(n)is int and -(2**31)<=n<2**31,'mode/logical count')
    check(a['pointer']==p and type(a['capacity'])is int and 0<=a['capacity']<=8
          and len(a['values'])==a['capacity'],'physical reference extent')
    ints(a['values_hex'],a['values'])
    if p=='0x0':
        check(not a['allocation_observed'] and a['capacity']==0,'null physical allocation')
    else:
        check(a['allocation_observed'] and p in live and live[p]['material']==r['address']
              and live[p]['capacity']==a['capacity'],'independently observed live allocation')
    check(n<=a['capacity'] and r['reference_ids']==a['values'][:max(0,n)],'logical versus physical reference projection')


def lists_in(value):
    if isinstance(value,dict):
        if 'records' in value and 'scratch' in value:yield value
        for child in value.values():yield from lists_in(child)
    elif isinstance(value,list):
        for child in value:yield from lists_in(child)


def records_in(value):
    if isinstance(value,dict):
        if 'reference_allocation' in value:yield value
        for child in value.values():yield from records_in(child)
    elif isinstance(value,list):
        for child in value:yield from records_in(child)


def list_shape(s):
    rs=s['records'];n=s['count']
    check(n==len(rs)<=4 and len({r['address']for r in rs})==n,'bounded material list')
    check(s['head']==(rs[0]['address']if rs else '0x0') and s['tail']==(rs[-1]['address']if rs else '0x0'),'material head/tail')
    check(type(s['root_count'])is int and 0<=s['root_count']<=4,'bounded independently stored root count')
    for i,r in enumerate(rs):
        check(r['owner']==s['owner'] and r['next']==(rs[i+1]['address']if i+1<n else '0x0'),'material owner/next linkage')


def set_count(r,n):
    r[COUNT]=n;r['reference_ids']=r['reference_allocation']['values'][:max(0,n)]


def set_allocation(r,p,values):
    r[PTR]=p;r['reference_allocation']={'pointer':p,'capacity':len(values),'values':values[:],
        'values_hex':b''.join(struct.pack('<i',v)for v in values).hex(),'allocation_observed':p!='0x0'}
    r['reference_ids']=values[:max(0,r[COUNT])]


def projection(rs):
    return [{k:v for k,v in r.items() if k not in ('next',)}for r in rs]


def find(rs,id):return next((r for r in rs if r['id']==id),None)


def integer_row(e,data):
    pos=stream_position(e['before'],data);line,end,eof,after=line_read(data,pos)
    check(e['kind']=='int1' and e['skip']==0 and e['lines']==[{'limit':1024,'eof':eof,'hex':None if eof else line.hex()}]
          and e['after']==after,'count helper exact line/cursor')
    scans=[];value=0;status=1
    if not eof:
        cleaned=line.replace(b',',b' ');text=cleaned.lstrip()
        match=re.match(rb'[+-]?(?:0[xX][0-9a-fA-F]+|0[0-7]*|[1-9][0-9]*)',text)
        conversions=-1 if not text else int(match is not None)
        if match:
            token=match[0];u=token.lstrip(b'+-');base=16 if u.lower().startswith(b'0x')else 8 if u.startswith(b'0')else 10
            value=int(token,base)
        scans=[{'format':'%i','input_hex':cleaned.hex(),'conversions':conversions}]
        status=int(conversions==-1)
    check(e['scans']==scans and e['value']==value and e['output_hex']==struct.pack('<i',value).hex()
          and e['status']==status,'count helper zero/partial/status')
    return value,status


def scalar(e,data,previous):
    pos=stream_position(e['before'],data)
    while pos<len(data) and data[pos] in b' \t\r\n\v\f':pos+=1
    # This predeclared finite fixture set uses complete decimal/exponent items.
    match=re.match(rb'[+-]?(?:[0-9]+(?:\.[0-9]*)?|\.[0-9]+)(?:[eE][+-]?[0-9]+)?',data[pos:])
    conversions=-1 if pos==len(data)else int(match is not None)
    end=pos+(len(match[0])if match else 0)
    after={'cursor':end,'count':len(data)-end,'buffer_hex':data.hex()}
    if end==len(data):after={'cursor':0,'count':0,'buffer_hex':''}
    check(e['format']=='%Flf' and e['conversions']==conversions and e['after']==after,'reference scalar grammar/cursor')
    if conversions==1:
        value=float(match[0]);check(math.isfinite(value),'finite scalar scope')
        check(e['output_hex']==struct.pack('<d',value).hex() and e['value']==value,'reference scalar raw result')
        truncated=math.trunc(value);check(-(2**31)<=truncated<2**31,'bounded CVTTSD2SI scope')
        return value,conversions
    check(e['output_hex'] is None and e['value'] is None,'failed scalar output not read')
    check(conversions==-1 or previous is not None,'first matching failure excluded')
    return previous,conversions


def check_free(e,r):
    a=r['reference_allocation']
    check(e['material']==r['address'] and e['pointer']==r[PTR] and e['capacity']==a['capacity']
          and e['values']==a['values'] and e['values_hex']==a['values_hex'],'free old allocation contents/owner')


def handler(es,data):
    first,last=es[0],es[-1];id=first['id']
    check(id==last['id'] and first['category']==last['category'],'handler pair')
    old=first['state']['material_list']['records'];actual=last['state']['material_list']['records']
    if first['category']!=4:
        check(old==actual,'other selected handler preserves material records');return None
    check(id in (92,116),'selected material handler')
    sels=[e for e in es if e['operation']=='material-selector'];check(len(sels)==1,'single material selector')
    sel=sels[0];check(sel['id']==id and sel['before']==first['stream'],'selector begins handler')
    selected=verify_selector({**sel,'id':92},data);check(selected is not None,'successful selected selector')
    expected=deepcopy(old);r=find(expected,selected);created=r is None
    counts=[e for e in es if e['operation']=='material-reference-count']
    names=[e for e in es if e['operation']=='material-name']
    check(len(counts)==int(id==116) and len(names)==int(id==92),'selected reader dispatch')
    read_event=(counts or names)[0]
    append=[e for e in es if e['operation']=='material-append-after']
    refresh=[e for e in es if e['operation']=='material-refresh-after']
    check(len(append)==len(refresh)==int(created),'creation-only refresh timing')
    if created:
        check(append[0]['sequence']<refresh[0]['sequence']<read_event['sequence'],'append refresh before reader')
        r=deepcopy(append[0]['state']['records'][-1]);check(r['id']==0 and r[MODE]==0 and r[COUNT]==0 and r[PTR]=='0x0','new record defaults')
        r['id']=selected;expected.append(r)
    check(len(expected)==len(actual),'material count after selected handler')
    if id==92:
        value,status=verify_name(names[0],data,r)
        r.update(name_hex=value.hex(),name_length=len(value),name_terminator_hex='00')
        cursor=names[0]['after'];status=2 if status else 0
    else:
        c=counts[0];check(projection([c['previous']])==projection([r]) and c['material']==r['address'],'count prior record')
        check(c['before']==sel['after'],'count follows selector')
        n,status=integer_row(c,data);old_count=r[COUNT];set_count(r,n);cursor=c['after'];status=2 if status else 0
        alloc=[e for e in es if e['operation']=='material-reference-allocate']
        frees=[e for e in es if e['operation']=='material-reference-free-before']
        scans=[e for e in es if e['operation']=='material-reference-scan']
        if status:
            check(not alloc and not frees and not scans,'failed count returns before storage change')
        elif n>0:
            check(len(alloc)==1 and alloc[0]['capacity']==n and alloc[0]['material']==r['address'],'positive count calloc')
            if r[PTR]!='0x0':
                check(len(frees)==1 and c['sequence']<frees[0]['sequence']<alloc[0]['sequence'],'replacement free before calloc');check_free(frees[0],r)
            else:check(not frees,'no old allocation free')
            r[MODE]=1;values=[0]*n;previous=None
            for i,e in enumerate(scans):
                check(i<n and e['before']==cursor and e['material']==r['address'] and e['sequence']>alloc[0]['sequence'],'reference scan order/owner')
                previous,conv=scalar(e,data,previous);cursor=e['after']
                if conv==-1:
                    status=2;check(i==len(scans)-1,'EOF returns immediately');break
                values[i]=math.trunc(previous)
            check(len(scans)==n or status==2,'reference loop completeness')
            set_allocation(r,alloc[0]['pointer'],values)
        else:
            check(not alloc and not frees and not scans,'nonpositive count retains allocation')
            if r[MODE]==1 and r[PTR]!='0x0' and old_count>0:
                for target_id in r['reference_allocation']['values'][:old_count]:
                    target=find(expected,target_id)
                    if target is not None:target[MODE]=0
                r[MODE]=0
        check(status in (0,2),'selected handler status domain')
    check(projection(actual)==projection(expected),'exact selected material mutation')
    check(last['status']==status and last['stream']==cursor,'selected status/cursor with no final discard')
    return {'keyword':id,'selected_id':selected,'created':created,'status':status,'sequence':first['sequence']}


def finalize(es):
    expected=deepcopy(es[0]['state']['records']);core=[e for e in es[1:-1] if e['operation'] in
        ('material-reference-lookup-before','material-lookup','material-reference-free-before','material-reference-free-after')]
    index=0;lookups=0
    def take(op):
        nonlocal index
        check(index<len(core) and core[index]['operation']==op,'immediate finalizer event order')
        e=core[index];index+=1;return e
    for source in expected:
        if source[MODE]!=1:continue
        i=0
        while i<source[COUNT]:
            target_id=source['reference_allocation']['values'][i]
            before=take('material-reference-lookup-before');check(before['requested']==target_id and projection(before['state']['records'])==projection(expected),'live-count insertion-order lookup state')
            lookup=take('material-lookup');target=find(expected,target_id);lookups+=1
            check(lookup['requested']==target_id and lookup['result']==(target['address']if target else '0x0'),'exact finalizer lookup target')
            if target is not None:
                check(projection([lookup['record']])==projection([target]),'lookup observes before target mutation')
                if target[MODE]!=2:
                    if target[COUNT]>0:
                        if target[PTR]!='0x0':
                            freed=take('material-reference-free-before');check_free(freed,target)
                            after=take('material-reference-free-after');check(all(after[k]==freed[k]for k in ['serial','pointer','material','capacity','call_return_rva']),'paired finalizer free')
                        set_count(target,0);set_allocation(target,'0x0',[])
                    target[MODE]=2
            i+=1
    check(index==len(core),'no surplus finalizer mutation events')
    check(projection(es[-1]['state']['records'])==projection(expected),'immediate alias-sensitive finalizer outcome')
    return lookups


def validate(evidence,inputs):
    cap=evidence['normalized_result'];messages=cap['messages'];check(len(cap['scripts'])==1,'single passive script')
    check(all(m['type']=='send' and m['script_id']==cap['scripts'][0]['script_id']for m in messages),'no runtime errors')
    check(messages[0]['payload']['operation']=='hooks-installed' and messages[0]['payload']['pid']==cap['pid'],'hook identity')
    events=[m['payload']for m in messages[1:]]
    check([e['sequence']for e in events]==list(range(1,len(events)+1)),'continuous event order')
    check(not any(e['operation']=='probe-error'for e in events),'no probe errors')
    live={};pending_free=None;groups=[];group=None;allocs=0;destructors={};features=Counter()
    for e in events:
        op=e['operation']
        if op=='material-reference-allocate':
            check(e['call_return_rva']==0x3883ee and e['width']==4 and 1<=e['capacity']<=8
                  and e['values']==[0]*e['capacity'] and e['pointer']not in live,'proven zeroed calloc capacity')
            ints(e['values_hex'],e['values']);live[e['pointer']]={k:e[k]for k in ['capacity','material','serial']};allocs+=1
        for r in records_in(e):record_shape(r,live)
        for s in lists_in(e):
            list_shape(s)
            if op in ('import-before','import-after','handler-before','handler-after','material-reference-finalize-before','material-reference-finalize-after'):
                check(s['root_count']==s['count'],'boundary root/list count agreement')
        if op=='material-reference-free-before':
            check(pending_free is None and e['pointer']in live and all(live[e['pointer']][k]==e[k]for k in ['capacity','material','serial']),'free live allocation identity')
            ints(e['values_hex'],e['values']);check(len(e['values'])==e['capacity'],'free byte extent');pending_free=e
            if e['material']in destructors:check_free(e,destructors[e['material']])
        elif op=='material-reference-free-after':
            check(pending_free is not None and all(e[k]==pending_free[k]for k in ['pointer','capacity','material','serial','call_return_rva']),'free paired event')
            del live[e['pointer']];pending_free=None
        elif op=='material-destroy-before':destructors[e['record']['address']]=e['record']
        elif op=='material-destroy-after':
            check(e['address']in destructors and all(v['material']!=e['address']for v in live.values()),'destructor releases retained allocation');del destructors[e['address']]
        elif op=='material-reference-feature':
            want=int(e['control_0x85c']!=0 or any(r[MODE]==1 for r in e['state']['records']))
            check(49<=e['feature']<=52 and e['result']==want,'feature49..52 live material modes');features[f"{e['feature']}:{want}"]+=1
        if op=='import-before':check(group is None,'import nesting');group=[]
        check(group is not None and e['file']==(group[0]['file']if group else e['file']),'event import ownership');group.append(e)
        if op=='import-after':groups.append(group);group=None
    check(group is None and pending_free is None and not destructors,'complete event lifecycle')
    check([g[0]['file']for g in groups]==list(inputs),'ordered complete imports')
    results=[];total_lookups=0;all_handlers=[];retained=Counter();nonzero_columns=0
    for g in groups:
        data=inputs[g[0]['file']];active=None;fstart=None;statuses=[];finals=0;last_materials=g[0]['state']['material_list']['records']
        for i,e in enumerate(g):
            op=e['operation']
            if op=='stream-scan' and e['format']=='%39s':verify_token(e,data)
            if op=='handler-before':check(active is None,'handler nesting');active=i
            elif op=='handler-after':
                check(active is not None,'handler leave pairing');info=handler(g[active:i+1],data);active=None
                statuses.append(e['status']);last_materials=e['state']['material_list']['records']
                if info:all_handlers.append(info)
            elif op=='material-clear-after' and e['state']['owner']==g[0]['state']['address']:
                last_materials=e['state']['records']
            elif op=='material-reference-finalize-before':
                check(fstart is None and active is None,'reference finalizer after parser');fstart=i
                check(projection(e['state']['records'])==projection(last_materials),'finalizer receives parser material state')
            elif op=='material-reference-finalize-after':
                check(fstart is not None,'reference finalizer pairing');total_lookups+=finalize(g[fstart:i+1]);fstart=None;finals+=1
                last_materials=e['state']['records']
            if op=='material-refresh-after':
                for o in e['state']['objects']:
                    for d in o['descriptors']:
                        if d['id'] in (49,50):nonzero_columns+=sum(v!=0 for v in (d['values'] or []))
        check(active is None and fstart is None and finals==1,'one complete material finalizer')
        check(projection(g[-1]['state']['material_list']['records'])==projection(last_materials),'post-import material state')
        expected=next((s for s in reversed(statuses)if s),0)
        unknown=any(e['operation']=='keyword-resolved'and e['id']==305 for e in g)
        # Sentinel305 emits a warning but does not replace the accumulated status.
        if unknown:check(any(e.get('token')=='x' and e.get('id')==305 for e in g),'selected late-failure unknown token')
        check(g[-1]['status']==expected,'parser/finalizer retained status')
        rs=g[-1]['state']['material_list']['records']
        for r in rs:
            if r[PTR]!='0x0' and r[COUNT]<=0:retained[str(r[COUNT])]+=1
        results.append({'file':g[0]['file'],'status':g[-1]['status'],'sequence':[g[0]['sequence'],g[-1]['sequence']],
                        'materials':[{'id':r['id'],'mode':r[MODE],'logical_count':r[COUNT],
                                      'capacity':r['reference_allocation']['capacity'],'physical_ids':r['reference_allocation']['values']}for r in rs]})
    check(not live,'last reset releases all tracked reference allocations')
    return {'imports':results,'handlers':all_handlers,'allocation_count':allocs,'finalizer_lookups':total_lookups,
            'features':dict(features),'retained_nonpositive_snapshots':dict(retained),
            'nonzero_descriptor49_50_values_at_refresh':nonzero_columns,'event_counts':dict(Counter(e['operation']for e in events))}


def main():
    p=argparse.ArgumentParser();p.add_argument('--capture',type=Path,default=ROOT/'analysis/material-reference-runtime-training.json')
    p.add_argument('--run-record',type=Path,default=ROOT/'analysis/oracle/material-reference-training/run.json')
    p.add_argument('--manifest',type=Path,default=ROOT/'analysis/material-reference-evaluation-cases.json')
    p.add_argument('--freeze',type=Path,default=ROOT/'analysis/material-reference-candidate-freeze.json')
    p.add_argument('--output',type=Path,default=ROOT/'analysis/material-reference-training-observations.json');args=p.parse_args()
    evidence,run,manifest,freeze=map(read,[args.capture,args.run_record,args.manifest,args.freeze]);directory=args.run_record.parent
    inputs=provenance(evidence,run,manifest,freeze,directory);result=validate(evidence,inputs);rejected=[]
    def event_test(label,op,mutate,predicate=lambda e:True):
        damaged=deepcopy(evidence);es=[m['payload']for m in damaged['normalized_result']['messages']]
        target=next(e for e in es if e['operation']==op and predicate(e));mutate(target)
        try:validate(damaged,inputs)
        except (ValueError,KeyError,AssertionError,StopIteration):rejected.append(label)
        else:raise ValueError('tamper accepted: '+label)
    event_test('event order','material-reference-count',lambda e:e.update(sequence=0))
    event_test('count destination','material-reference-count',lambda e:e.update(value=e['value']+1))
    event_test('count raw bytes','material-reference-count',lambda e:e.update(output_hex='00000000'))
    event_test('count status','material-reference-count',lambda e:e.update(status=1-e['status']))
    event_test('count cursor','material-reference-count',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    event_test('count source line','material-reference-count',lambda e:e['lines'][0].update(hex='20390a'))
    event_test('allocated capacity','material-reference-allocate',lambda e:e.update(capacity=e['capacity']+1))
    event_test('calloc zero bytes','material-reference-allocate',lambda e:e.update(values_hex='01'+e['values_hex'][2:]))
    event_test('calloc call site','material-reference-allocate',lambda e:e.update(call_return_rva=0))
    event_test('scalar raw bytes','material-reference-scan',lambda e:e.update(output_hex='0000000000000000'),lambda e:e['conversions']==1)
    event_test('failed scalar destination exposure','material-reference-scan',lambda e:e.update(output_hex='0000000000000000'),lambda e:e['conversions']!=1)
    event_test('free contents','material-reference-free-before',lambda e:e['values'].__setitem__(0,123))
    event_test('free owner','material-reference-free-after',lambda e:e.update(material='0x1'))
    event_test('finalizer insertion order','material-reference-lookup-before',lambda e:e.update(requested=777))
    event_test('finalizer immediate outcome','material-reference-finalize-after',lambda e:e['state']['records'][0].update(mode_0x1cc8=2),lambda e:e['state']['records']and e['state']['records'][0][MODE]!=2)
    event_test('feature result','material-reference-feature',lambda e:e.update(result=1-e['result']))
    event_test('retained allocation bytes','import-after',lambda e:e['state']['material_list']['records'][0]['reference_allocation']['values'].__setitem__(0,888),lambda e:e['state']['material_list']['records']and e['state']['material_list']['records'][0][COUNT]<=0 and e['state']['material_list']['records'][0]['reference_allocation']['capacity']>0)
    for key,value in [('pid',-1),('source_sha256','0'*64),('messages_truncated',True)]:
        damaged=deepcopy(evidence);damaged['normalized_result'][key]=value
        try:provenance(damaged,run,manifest,freeze,directory)
        except ValueError:rejected.append('provenance '+key)
        else:raise ValueError('provenance tamper accepted '+key)
    for label,which in [('fixture hash','manifest'),('frozen archive hash','freeze')]:
        m,f=deepcopy(manifest),deepcopy(freeze)
        if which=='manifest':m['cases'][0]['sha256']='0'*64
        else:f['files'][0]['sha256']='0'*64
        try:provenance(evidence,run,m,f,directory)
        except ValueError:rejected.append(label)
        else:raise ValueError('binding tamper accepted '+label)
    report={'status':'PASS','scope':'Original-only bounded MSTMTR count/storage mutation, finite scalar reads, allocation lifetime, and immediate insertion-order material finalization.',
        'evidence_id':evidence['evidence_id'],'pid':run['pid'],'result':result,'tampering_rejected':rejected,
        'bindings':[binding(x)for x in [args.capture,args.run_record,args.manifest,args.freeze,PROBE,CONTRACT]],
        'limitations':['No replacement execution or candidate-generated expectations. This is a bounded observation audit, not full application compatibility.',
          'Nonzero descriptor49/50 material-column permutation remains pending: this capture contains zero-valued columns at refresh. Feature query results and allocation effects are observed.',
          'First-reference matching failure may read an uninitialized native local and is excluded. The selected later matching failure follows a proven successful scalar in the same handler.',
          'Scientific meanings of material modes are not inferred. Other keyword algorithms retain their earlier milestone scope.',
          'Allocation failure, nonfinite/overflow conversions, shared aliases, and material property tables beyond selected fields are outside this experiment.']}
    args.output.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(f"PASS {len(inputs)} imports; {result['allocation_count']} allocations; {result['finalizer_lookups']} finalizer lookups; {len(rejected)} tamper checks")

if __name__=='__main__':main()

