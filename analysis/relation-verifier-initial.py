"""Independent original-only PHASTF / DVOLF / TICF observation audit.

Uses retained native contracts, source fixture bytes and passive observations.
Never executes the replacement or imports a candidate comparison program.
"""
import argparse
from collections import Counter
from copy import deepcopy
import json
from pathlib import Path
import struct

import verify_material_column_observations as base
import verify_material_reference_observations as refs
import verify_mesh_field_observations as fixed
import verify_custom_field_observations as custom
from verify_boundary_sort_observations import ROOT, binding, check, read
from verify_mesh_array_observations import descriptor_map

PROBE=ROOT/'tools/observe_relation_import.js'
CONTRACT=ROOT/'analysis/relation-import-static-contract.json'
raw,one,lex=base.raw,base.one,base.lex
COUNTER='relation_count_0x13f4'
GATE='material_gate_0x85c'


def provenance(evidence,run,manifest,freeze,input_freeze,directory,manifest_path,freeze_path):
    base.PROBE=PROBE;base.CONTRACT=CONTRACT
    projected=deepcopy(manifest);projected['probe'].pop('evidence_id',None)
    return base.provenance(evidence,run,projected,freeze,input_freeze,directory,manifest_path,freeze_path)


def record(r):
    check(int(r['address'],16)>0 and int(r['owner'],16)>0,'live relation owner/address')
    for k in ('key0','key1','mode_0x08'):
        check(type(r[k])is int and -2**31<=r[k]<2**31,'signed relation field '+k)
    check(r['active_0x0c']in(0,1) and r['field_0x10']==r['field_0x14']==0,'selected initialized relation flags')
    check(r['name_prefix_hex']=='2000' and r['name_hex']=='20' and r['name_terminator_hex']=='00','only initialized relation name prefix')


def relation_list(s):
    rs=s['records'];check(s['count']==len(rs)<=3,'bounded relation list count')
    check(len({r['address']for r in rs})==len(rs),'distinct live relation records')
    check(s['head']==(rs[0]['address']if rs else'0x0') and s['tail']==(rs[-1]['address']if rs else'0x0'),'relation head/tail')
    for i,r in enumerate(rs):
        record(r);check(r['owner']==s['owner'] and r['next']==(rs[i+1]['address']if i+1<len(rs)else'0x0'),'relation ownership/next links')


def nested_relation_lists(v):
    if isinstance(v,dict):
        if {'records','head','tail','owner','count'}<=v.keys() and 'root_count'not in v and all('key0'in r for r in v['records']):
            # Material lists have scratch/owner too; their nonempty records differ.
            if 'address'in v and 'scratch'not in v:yield v
        for x in v.values():yield from nested_relation_lists(x)
    elif isinstance(v,list):
        for x in v:yield from nested_relation_lists(x)


def root_shape(s,rows):
    relation_list(s['relation_list'])
    check(s['relation_list']['owner']==s['address'] and int(s['relation_list']['address'],16)==int(s['address'],16)+0x1358,'root relation owner/offset')
    check(type(s[COUNTER])is int and 0<=s[COUNTER]<=3,'bounded cached relation count')
    check(s[GATE]==0,'observed predeclared root gate zero; never seeded')
    for o in s['objects']:base.shared.object_shape(o,rows)
    if s.get('linked_state')is not None:root_shape(s['linked_state'],rows)


def selector(e,data):
    check(e['id']==134 and e['format']=='%i %i' and e['conversions']==2,'defined pair selector ABI')
    p=base.stream_position(e['before'],data);values=[]
    for _ in range(2):
        v,c,p=lex(data,p,True);check(c==1,'both source pair conversions assigned');values.append(v)
    check(e['values']==values and e['output_hex']==[raw([v],True)for v in values],'exact signed ordered pair source/raw bytes')
    check(base.stream_position(e['after'],data)==p,'pair selector exact cursor')
    return values


def refresh(es):
    before,after=es[0],es[-1];old,new=before['state'],after['state']
    gate=new[GATE]!=0 or any(r[refs.MODE]==1 for r in new['material_list']['records'])
    check(new['components'][51:53]==[new[COUNTER]if gate else 0]*2,'relation width from cached count and live material gate')
    check(old['relation_list']==new['relation_list'] and old[COUNTER]==new[COUNTER],'refresh does not recount or mutate relations')
    for a,b in zip(old['objects'],new['objects']):
        for component in (51,52):
            x,y=base.descriptor(a,component),base.descriptor(b,component)
            if a['type']==7 or a['element_allocation_count']<=0:
                check(x==y,'inactive/zero stored-count relation descriptor untouched')
            else:
                count=a['element_allocation_count'];width=new['components'][component]
                payload=bytes.fromhex(x['values_hex']);wanted=payload[:count*width*8]+bytes(max(0,count*width*8-len(payload)))
                check(y['values_hex']==wanted.hex() and y['length']==count*width,'relation flat prefix resize without semantic column permutation')
                check((y['old_count'],y['old_width'],y['old_multiplicity'])==(x['count'],x['width'],x['multiplicity']),'relation descriptor shape history')
                check((y['count'],y['width'],y['multiplicity'])==(count,width,1),'relation descriptor stored-count/current-width allocation')
    # Earlier material audit predates the explicit option0 path. Its copy-selection
    # predicate uses old scratch presence; option0 never enters that copy branch.
    # Project only that predicate after proving no copy events, retaining all arrays.
    projected=deepcopy(es)
    if before['option']==0:
        check(not any(e['operation'].startswith('material-copy-')for e in es),'option0 never creates temporary objects')
        scratch=projected[0]['state']['material_list']['scratch']
        scratch['ids_pointer']=scratch['ranks_pointer']='0x0'
    return base.refresh(projected)


def relation_handler(es,data):
    first,last=es[0],es[-1];old,new=first['state'],last['state']
    check(first['id']==last['id']==134 and first['category']==last['category']==5,'PHASTF handler identity')
    sel=one(es,'relation-selector');keys=selector(sel,data)
    check(sel['before']==first['stream'],'pair selector starts handler')
    lookup=one(es,'relation-lookup');check([lookup['key0'],lookup['key1']]==keys and lookup['list']==old['relation_list']['address'],'lookup exact ordered pair/container')
    prior=next((r for r in old['relation_list']['records']if[r['key0'],r['key1']]==keys),None)
    check(lookup['record']==prior and lookup['pointer']==('0x0'if prior is None else prior['address']),'exact lookup result with no eligibility filter')
    created=prior is None;expected=deepcopy(old)
    appends=[e for e in es if e['operation']=='relation-append-after']
    check(len(appends)==int(created),'only missing pair appends')
    if created:
        a,b=one(es,'relation-append-before'),appends[0]
        check(a['state']==old['relation_list'] and lookup['sequence']<a['sequence']<b['sequence'],'append order and prior list')
        born=deepcopy(b['record']);check(born['key0']==born['key1']==born['mode_0x08']==born['active_0x0c']==0,'constructor selected zero defaults')
        check(born['next']=='0x0' and born['address']==b['pointer'] and born['owner']==old['address'],'new tail identity')
        wanted=deepcopy(old['relation_list']);wanted['count']+=1
        if wanted['records']:wanted['records'][-1]['next']=born['address']
        else:wanted['head']=born['address']
        wanted['tail']=born['address'];wanted['records'].append(born)
        check(wanted==b['state'],'stable insertion-order append')
        expected['relation_list']=deepcopy(wanted)
        prior=expected['relation_list']['records'][-1];prior['key0'],prior['key1']=keys
    else:prior=next(r for r in expected['relation_list']['records']if r['address']==prior['address'])
    h=one(es,'relation-mode-after');entry=one(es,'relation-mode-before')
    check(h['completion_rva']==0x38324f and h['serial']==entry['serial'] and h['record']==entry['record']==prior['address'],'mode exact native continuation/identity')
    check(h['before']==sel['after']==entry['before'] and h['before_record']==h['after_record']==entry['before_record']==prior,'mode read precedes native record store')
    helper={**h,'values':[h['value']],'output_hex':[h['output_hex']]}
    values,status,cursor=custom.line_helper(helper,data,[True],'%i');mode=values[0]
    prior['mode_0x08']=mode;prior['active_0x0c']=int(mode!=0)
    recounts=[e for e in es if e['operation']=='relation-recount-after'];updates=[e for e in es if e['operation']=='material-refresh-after']
    check(len(recounts)==len(updates)==int(mode!=0),'zero mode leaves cached count/layout stale')
    if mode!=0:
        rb,ra=one(es,'relation-recount-before'),recounts[0]
        check(rb['root']==ra['root']==old['address'] and rb['serial']==ra['serial'],'recount owner/identity')
        check(rb['relations']==ra['relations']==expected['relation_list'] and rb['counter']==old[COUNTER],'mode store before recount')
        check(rb['gate']==ra['gate']==old[GATE] and rb['width51']==ra['width51']==old['components'][51] and rb['width52']==ra['width52']==old['components'][52],'recount does not refresh widths')
        expected[COUNTER]=sum(r['active_0x0c']==1 for r in expected['relation_list']['records'])
        check(ra['counter']==expected[COUNTER],'recount counts exactly active==1')
        fb,fa=one(es,'material-refresh-before'),updates[0]
        check(fb['option']==0 and h['sequence']<rb['sequence']<ra['sequence']<fb['sequence']<fa['sequence'],'PHASTF payload/store/recount/refresh order')
        check(fb['state']==expected,'full state at relation refresh entry')
        expected=deepcopy(fa['state'])
    check(last['state']==expected,'PHASTF full-state mutation boundary')
    check(last['status']==(2 if status else 0) and last['stream']==cursor,'PHASTF status/cursor even failed payload')
    return {'id':134,'dimension':old['dimension'],'keys':keys,'mode':mode,'created':created,'status':last['status'],'cached_count':new[COUNTER],'widths':new['components'][51:53],'sequence':first['sequence']}


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
        if op=='material-reference-allocate':
            check(e['call_return_rva']==0x3883ee and e['width']==4 and e['values']==[0]*e['capacity']and e['pointer']not in live,'reference allocation provenance')
            live[e['pointer']]={k:e[k]for k in('capacity','material','serial')}
        for r in refs.records_in(e):refs.record_shape(r,live)
        for s in refs.lists_in(e):refs.list_shape(s)
        for s in nested_relation_lists(e):relation_list(s)
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
                refresh(frame);refresh_count+=1
                for a,b in zip(frame[0]['state']['objects'],e['state']['objects']):
                    for c in (51,52):
                        x,y=base.descriptor(a,c),base.descriptor(b,c)
                        if x['length']!=y['length']:changed_sizes[f"{c}:{x['width']}->{y['width']}"]+=1
            if op=='handler-before':
                check(start is None,'handler entry pairing');start=i
                if last_state is not None:check(e['state']==last_state,'within-import full state continuity')
            elif op=='handler-after':
                check(start is not None,'handler return pairing');frame=g[start:i+1];start=None;statuses.append(e['status'])
                if e['id']==134:results.append(relation_handler(frame,data))
                elif e['id']in(206,207):results.append(fixed.handler(frame,data))
                elif e['category']==4:refs.handler(frame,data)
                if e['id']!=134:
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
    return {'imports':len(groups),'handlers':results,'refreshes':refresh_count,'coverage':{'id_dimensions':sorted(set((r['id'],r['dimension'])for r in results)),'failures':sum(r['status']!=0 for r in results),'relation_descriptor_width_changes':dict(changed_sizes),'reference_allocations_still_live_at_capture_end':len(live)},'event_counts':dict(Counter(e['operation']for e in events))}


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for k,v in [('capture','analysis/relation-runtime-training.json'),('run-record','analysis/oracle/relation-training/run.json'),('manifest','analysis/relation-evaluation-cases.json'),('freeze','analysis/relation-candidate-freeze.json'),('input-freeze','analysis/relation-input-freeze.json'),('output','analysis/relation-observations.json')]:p.add_argument('--'+k,type=Path,default=ROOT/v)
    args=p.parse_args()
    for k in vars(args):setattr(args,k,getattr(args,k).resolve())
    evidence,run,manifest,freeze,input_freeze=map(read,[args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze])
    inputs=provenance(evidence,run,manifest,freeze,input_freeze,args.run_record.parent,args.manifest,args.freeze);result=validate(evidence,inputs)
    rejected=[];not_applied=[]
    def tamper(label,op,fn,predicate=lambda e:True):
        changed=deepcopy(evidence);target=next((m['payload']for m in changed['normalized_result']['messages']if m['payload']['operation']==op and predicate(m['payload'])),None)
        if target is None:not_applied.append(label);return
        fn(target)
        try:validate(changed,inputs)
        except (ValueError,KeyError,IndexError,AssertionError,StopIteration,TypeError,struct.error):rejected.append(label)
        else:raise ValueError('tamper accepted '+label)
    tamper('event order','relation-mode-after',lambda e:e.update(sequence=0))
    tamper('ordered pair source','relation-selector',lambda e:e['values'].reverse(),lambda e:e['values'][0]!=e['values'][1])
    tamper('pair raw signed output','relation-selector',lambda e:e['output_hex'].__setitem__(0,'ffffff7f'))
    tamper('pair cursor','relation-selector',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('lookup target','relation-lookup',lambda e:e.update(pointer='0x1'))
    tamper('constructor mode default','relation-append-after',lambda e:e['record'].update(mode_0x08=9))
    tamper('append record count','relation-append-after',lambda e:e['state'].update(count=e['state']['count']+1))
    tamper('mode partial raw output','relation-mode-after',lambda e:e.update(output_hex='ffffff7f'))
    tamper('mode status','relation-mode-after',lambda e:e.update(status=1-e['status']))
    tamper('mode source bytes','relation-mode-after',lambda e:e['lines'][0].update(hex='390a'),lambda e:not e['lines'][0]['eof'])
    tamper('mode native continuation','relation-mode-after',lambda e:e.update(completion_rva=0))
    tamper('premature mode record mutation','relation-mode-after',lambda e:e['after_record'].update(mode_0x08=999))
    tamper('recount result','relation-recount-after',lambda e:e.update(counter=e['counter']+1))
    tamper('recount changes widths','relation-recount-after',lambda e:e.update(width51=e['width51']+1))
    tamper('refresh relation width','material-refresh-after',lambda e:e['state']['components'].__setitem__(51,99))
    tamper('refresh relation count','material-refresh-after',lambda e:e['state'].update(relation_count_0x13f4=0),lambda e:e['state'][COUNTER]>0)
    tamper('flat prefix value','material-refresh-after',lambda e:base.descriptor(e['state']['objects'][0],51).update(values_hex='000000000000f03f'+base.descriptor(e['state']['objects'][0],51)['values_hex'][16:]),lambda e:bool(e['state']['objects']) and base.descriptor(e['state']['objects'][0],51)['length']>0)
    tamper('descriptor history','material-refresh-after',lambda e:base.descriptor(e['state']['objects'][0],51).update(old_width=999),lambda e:bool(e['state']['objects']) and e['state']['objects'][0]['type']!=7)
    tamper('field root width','field-reader-before',lambda e:e.update(width=e['width']+1))
    tamper('field source row value','field-scan',lambda e:e.update(value=e['value']+1),lambda e:e['conversions']==1)
    tamper('field default bytes','field-reader-after',lambda e:e.update(values_hex='000000000000f03f'+e['values_hex'][16:]),lambda e:bool(e['values']))
    tamper('field failure status','field-reader-after',lambda e:e.update(status=0),lambda e:e['status']!=0)
    tamper('field row stride','field-rows-before',lambda e:e.update(width=e['width']+1))
    tamper('field row cursor','field-rows-after',lambda e:e['stream'].update(cursor=e['stream']['cursor']+1))
    tamper('raw negative zero sign','field-reader-after',lambda e:e.update(values_hex=e['values_hex'].replace('0000000000000080','0000000000000000',1)),lambda e:'0000000000000080'in e['values_hex'])
    tamper('unrelated field mutation','handler-after',lambda e:e['state']['objects'][0].update(material_group_0x3980=999),lambda e:e['id']in(134,206,207) and bool(e['state']['objects']))
    tamper('clear retained cache','finalize-after',lambda e:e['state'].update(relation_count_0x13f4=999))
    tamper('clear owner','relation-clear-after',lambda e:e['state'].update(owner='0x0'))
    tamper('clear missed record','relation-clear-after',lambda e:e['state'].update(count=1))
    tamper('finalization normalization','finalize-after',lambda e:e['state']['finalization_controls'].update(mode_0x990=999))
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
    report={'status':'PASS','result':result,'bindings':[binding(x)for x in(Path(__file__),args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze,PROBE,CONTRACT)],'tampering_rejected':rejected,'tampering_not_applied':not_applied,'dependencies':[binding(ROOT/'tools'/n)for n in('verify_material_column_observations.py','verify_material_reference_observations.py','verify_material_object_observations.py','verify_mesh_field_observations.py','verify_custom_field_observations.py','verify_boundary_sort_observations.py','verify_mesh_array_observations.py','verify_material_import_observations.py','verify_step_distance_observations.py','verify_time_controls_observations.py','capture_oracle.py')],'limitations':['Original-only finite bounded fixture domain; no reconstruction outputs consulted.','No OOM, signed extent overflow, incomplete pairselector, uninitialized first row-ID matching failure, scientific relation tables or general relation-copy/deserialization claims.','Native row scratch lifetime on EOF is not established by this payload audit.']}
    args.output.write_text(json.dumps(report,indent=2)+'\n');print('PASS',len(result['handlers']),'relation/field handlers;',len(rejected),'applied corruption checks')


if __name__=='__main__':main()
