"""Original-only CUSVAR/DYNARY audit; never runs or imports candidate code.

The input bytes and retained native contracts determine expected mutations.
Opaque addresses remain provenance, while raw IEEE-754 bytes are authoritative.
"""
import argparse
from collections import Counter
from copy import deepcopy
import json
import math
from pathlib import Path
import re
import struct

import verify_mesh_field_observations as fixed
import verify_material_column_observations as base
from verify_boundary_sort_observations import ROOT, binding, check, read

PROBE=ROOT/'tools/observe_custom_field_import.js'
CONTRACT=ROOT/'analysis/custom-field-static-contract.json'
raw,array,one,lex=base.raw,base.array,base.one,base.lex
IDS=(198,266)


def provenance(evidence,run,manifest,freeze,input_freeze,directory,manifest_path,freeze_path):
    base.PROBE=PROBE;base.CONTRACT=CONTRACT
    check({k:manifest['probe'][k]for k in('path','sha256')}==binding(PROBE),'manifest probe identity')
    projected=deepcopy(manifest);projected['probe'].pop('evidence_id',None)
    return base.provenance(evidence,run,projected,freeze,input_freeze,directory,manifest_path,freeze_path)


def vector(v,integer=False):
    vs=array(v,integer);size=4 if integer else 8
    check(v['references']>=1 and int(v['block'],16)>0 and int(v['handle'],16)>0,'live COW vector owner')
    check(len(vs)==v['physical_items']<=64 and len(vs)<=v['capacity_items']<=128,'physical versus capacity bounds')
    check(int(v['end'],16)-int(v['pointer'],16)==len(vs)*size,'physical byte pointer extent')
    check(int(v['capacity_end'],16)-int(v['pointer'],16)==v['capacity_items']*size,'capacity byte pointer extent')
    check(not vs or int(v['pointer'],16)>0,'nonnull nonempty data')
    return vs


def custom_entry(e):
    vs=vector(e['storage']);s=e['shape'];p=e['previous']
    check(e['id']==78 and e['bound_slot']=='0x0','custom descriptor base ID and external binding')
    check(type(e['custom_id'])is int and -2**31<=e['custom_id']<2**31,'signed custom ID')
    check(s['multiplicity']==p['multiplicity']==1 and s['count']>=0 and s['width']>=0,'selected custom multiplicities/extents')
    check(len(vs)==s['count']*s['width'],'custom physical shape')


def dynamic_entry(e):
    a=vector(e['real_storage']);b=vector(e['integer_storage'],True)
    check(e['integer_0x1c']in(0,1) and e['source_0x0c']in(0,1),'selected dynamic mode and boolean')
    check(e['count_0x24']>=0 and e['width_0x28']>=0,'bounded dynamic shape')
    if e['source_0x0c']==1:
        check(len(b if e['integer_0x1c'] else a)==e['count_0x24']*e['width_0x28'],'active typed physical extent')
    else:check(not a and not b and e['count_0x24']==e['width_0x28']==0,'default placeholder empty vectors')


def store(s,dynamic=False):
    es=s['entries'];check(s['count']==len(es)<=4,'bounded list count')
    check(len({e['address']for e in es})==len(es),'unique live entry addresses')
    if dynamic:
        check(s['head']==(es[0]['address']if es else'0x0') and s['tail']==(es[-1]['address']if es else'0x0'),'dynamic head/tail')
        for i,e in enumerate(es):
            dynamic_entry(e);check(e['index']==i+1 and e['next']==(es[i+1]['address']if i+1<len(es)else'0x0'),'one-based dynamic node order')
    else:
        check(s['references']>=1 and int(s['sentinel'],16)>0,'custom sentinel owner')
        check(len({e['custom_id']for e in es})==len(es),'unique exact custom IDs')
        for i,e in enumerate(es):
            custom_entry(e)
            check(e['previous_node']==(es[i-1]['node']if i else s['sentinel']) and e['next_node']==(es[i+1]['node']if i+1<len(es)else s['sentinel']),'custom doubly linked insertion order')


def states(s):
    check(type(s['custom_last_id_0x33b8'])is int and -2**31<=s['custom_last_id_0x33b8']<2**31,'root custom selection signed value')
    for o in s['objects']:
        store(o['custom_fields']);store(o['dynamic_arrays'],True)
        check(int(o['custom_fields']['container'],16)==int(o['address'],16)+0xb58 and o['custom_fields']['root']==s['address'] and o['custom_fields']['object']==o['address'],'custom owner offsets')
        check(int(o['dynamic_arrays']['container'],16)==int(o['address'],16)+0xba8,'dynamic owner offset')
    if s.get('linked_state')is not None:states(s['linked_state'])


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


def resized(old,n):return old[:n]+[0]*(max(0,n-len(old)))


def resize_check(old,new,n,integer=False):
    before=vector(old,integer);after=vector(new,integer)
    check(new['handle']==old['handle'],'vector owner handle stable')
    check(new['values_hex']==raw(resized(before,n),integer),'resize preserves linear prefix and zeros growth')
    if n==len(before):check(new==old,'equal physical size is a complete vector no-op')
    return after


def without(e,*keys):return {k:v for k,v in e.items()if k not in keys}


def fill(before,after,value,integer=False):
    check(without(before,'values','values_hex')==without(after,'values','values_hex'),'fill retains vector identity and shape')
    check(after['values_hex']==raw([value]*before['physical_items'],integer),'whole physical default fill')


def sparse(es,data,b,a,cursor,expected):
    check(b['stream']==cursor and b['storage']['values_hex']==raw(expected),'raw rows begin from exact defaults')
    check(a['completion_rva']==({198:0x397e46,266:0x397760}[b['id']]),'native caller continuation proves row return')
    check(all(b[k]==a[k]for k in('serial','id','kind','payload','declared_rows','count','width')),'raw rows argument identity')
    def projection(e):
        check(e['logical_values_hex']==raw(array(e['storage'])) and e['logical_values']==e['storage']['values'],'logical raw rows cover initialized physical extent')
        return {'serial':e['serial'],'parent_serial':None,'kind':'implicit','integers':False,'declared_rows':e['declared_rows'],
                'element_count':e['count'],'width':e['width'],'explicit_width':0,'stream':e['stream'],
                'values':e['storage']['values'],'values_hex':e['storage']['values_hex'],**({'status':e['status']}if 'status'in e else{})}
    inner=[]
    for e in es:
        if b['sequence']<e['sequence']<a['sequence'] and e['operation']in('custom-scan','custom-line'):
            x=deepcopy(e);x['operation']=e['operation'].replace('custom-','field-');inner.append(x)
    return fixed.detailed(inner,projection(b),projection(a),data,False)


def custom_handler(es,data,obj,target,cursor,oldroot):
    h=one(es,'custom-header');check(h['before']==cursor,'custom header follows object selector')
    fields,status,cursor=line_helper(h,data,[True,True,True,False,True],'%i %i %i %Flf %i')
    if status:
        check(not any(e['operation']=='custom-configure-before'for e in es),'failed header stops before configuration')
        check(target['custom_fields']==obj['custom_fields'],'failed header preserves store');return status,cursor,oldroot
    id,kind,declared,default,width=fields;b=one(es,'custom-configure-before');a=one(es,'custom-configure-after')
    check((b['custom_id'],b['kind'],b['rows'],b['width'])==(id,kind,declared,width) and b['store']==obj['custom_fields'],'configure caller exact arguments/store')
    check(a['serial']==b['serial'],'configure pair identity');entry=a['entry'];custom_entry(entry)
    prior=next((e for e in b['store']['entries']if e['custom_id']==id),None)
    look=one(es,'custom-lookup');check(look['custom_id']==id and look['container']==b['container'] and look['pointer']==(prior['address']if prior else'0x0'),'exact signed ID lookup')
    if prior is None:
        prior=one(es,'custom-created')['entry'];custom_entry(prior)
        check(prior['custom_id']==id and prior['kind']==0 and prior['shape']==prior['previous']=={'count':0,'width':1,'multiplicity':1} and not array(prior['storage']),'new descriptor constructor defaults')
    else:check(not any(e['operation']=='custom-created'for e in es),'existing exact ID does not append')
    wanted=deepcopy(prior);wanted['kind']=kind
    if kind in(0,1,2):
        wanted['previous']['count']=prior['shape']['count'];wanted['shape']['count']=declared if kind==0 else obj['nodes'if kind==1 else'elements']
    wanted['previous']['width']=prior['shape']['width'];wanted['shape']['width']=width
    check(without(entry,'storage')==without(wanted,'storage','node','previous_node','next_node'),'custom shape/history/identity ordering')
    resize_check(prior['storage'],entry['storage'],wanted['shape']['count']*width)
    oldentries=b['store']['entries'];newentries=a['store']['entries'];check(len(newentries)==len(oldentries)+int(not any(e['custom_id']==id for e in oldentries)),'append count only on missing ID')
    check(without(next(e for e in newentries if e['custom_id']==id),'node','previous_node','next_node')==entry,'configuration entry matches owned list payload')
    check([e['custom_id']for e in newentries]==[e['custom_id']for e in oldentries]+([]if any(e['custom_id']==id for e in oldentries)else[id]),'insertion order exact IDs')
    for x in oldentries:
        if x['custom_id']!=id:check(without(x,'next_node')==without(next(y for y in newentries if y['custom_id']==x['custom_id']),'next_node'),'unrelated custom entry retained')
    db=one(es,'custom-default-before')['entry'];da=one(es,'custom-default-after')['entry']
    check(db==entry and without(db,'storage')==without(da,'storage'),'default fill follows configuration')
    fill(db['storage'],da['storage'],default)
    rb=one(es,'custom-rows-before');ra=one(es,'custom-rows-after')
    check(rb['payload']==entry['address'] and rb['declared_rows']==declared and rb['count']==entry['shape']['count'] and rb['width']==width,'custom row ABI')
    values,status,cursor=sparse(es,data,rb,ra,cursor,[default]*entry['storage']['physical_items'])
    final=next(e for e in target['custom_fields']['entries']if e['custom_id']==id)
    check(without(final,'storage','node','previous_node','next_node')==without(entry,'storage') and final['storage']==ra['storage'],'final custom descriptor and partial writes')
    check(target['custom_fields']['count']==a['store']['count'],'final custom count')
    finalstore=deepcopy(a['store']);next(e for e in finalstore['entries']if e['custom_id']==id)['storage']=deepcopy(ra['storage'])
    check(target['custom_fields']==finalstore,'complete custom list continuity after rows')
    updates=[e for e in es if e['operation'].startswith('custom-root-update-')]
    check(len(updates)==(0 if status else 2),'root selection update only after successful rows')
    if updates:
        x,y=updates;check(x['operation']=='custom-root-update-before' and y['operation']=='custom-root-update-after' and x['value']==y['value']==id and x['previous']==oldroot,'root last-ID overwrite')
        check(x['target']==y['target'] and int(x['target'],16)==int(x['root'],16)+0x33b8,'root selection direct field ABI')
    return status,cursor,oldroot if status else id


def integer_token(token):
    m=re.match(rb'\s*([+-]?[0-9]+)',token);v=int(m[1])if m else 0
    check(-2**31<=v<2**31,'defined strtol signed32 domain');return v


def real_token(token):
    m=re.match(rb'\s*([+-]?(?:[0-9]+(?:\.[0-9]*)?|\.[0-9]+)(?:[eEdD][+-]?[0-9]+)?)',token)
    v=float(m[1].replace(b'd',b'e').replace(b'D',b'E'))if m else 0.0
    check(math.isfinite(v),'bounded finite atof input');return v


def token_header(es,data,cursor):
    e=one(es,'custom-token-header');check(e['before']==cursor and e['limit']==10,'token header ABI and cursor')
    content,_,eof,after=base.line_read(data,base.stream_position(cursor,data))
    check(e['after']==after and e['status']==int(eof) and e['lines']==[{'limit':1024,'eof':eof,'hex':None if eof else content.hex()}],'token header line/status')
    if eof:
        check(e['count']is None and e['tokens_hex']is None,'failed token header does not read uninitialized count');return None,1,after
    tokens=[v for v in re.split(rb'[ ,\n\t\r]+',content)if v][:10]
    check(e['count']==max(1,len(tokens)) and e['tokens_hex']==([t.hex()for t in tokens]if tokens else[None]),'legacy tokenizer count and delimiters')
    expected=tokens+[b'']*(7-len(tokens));n=e['count'];indices=list(range(5))+([5]if n>=6 else[])+([6]if n>=7 else[])
    accesses=[e for e in es if e['operation']=='custom-token-access'];ints=[e for e in es if e['operation']=='custom-token-integer']
    check([e['index']for e in accesses]==indices and [e['value_hex']for e in accesses]==[expected[i].hex()for i in indices],'ordered token access and empty fallback')
    iv=[integer_token(expected[i])for i in range(5+(n>=6))]
    check([(e['input_hex'],e['value'])for e in ints]==[(expected[i].hex(),v)for i,v in enumerate(iv)],'base10 prefix conversion values')
    reals=[e for e in es if e['operation']=='custom-token-real-input']
    check([e['input_hex']for e in reals]==([expected[6].hex()]if n>=7 else[]),'optional default conversion source')
    return (*iv[:5],iv[5]if n>=6 else iv[3],real_token(expected[6])if n>=7 else 0.0),0,after


def dynamic_handler(es,data,obj,target,cursor):
    fields,status,cursor=token_header(es,data,cursor)
    if status:
        check(target['dynamic_arrays']==obj['dynamic_arrays'] and not any(e['operation']=='custom-dynamic-lookup-before'for e in es),'failed token header no lookup/mutation');return status,cursor
    position,mode,kind,count,width,declared,default=fields
    b=one(es,'custom-dynamic-lookup-before');a=one(es,'custom-dynamic-lookup-after')
    lookup_store=a['store']
    check(b['store']==obj['dynamic_arrays'] and b['index']==a['index']==position and b['serial']==a['serial'],'dynamic position lookup identity')
    prior=b['store']['entries'];entries=a['store']['entries'];check(len(entries)==max(len(prior),position,0),'positive position appends gaps')
    for x,y in zip(prior,entries):check(without(x,'next')==without(y,'next'),'lookup preserves existing payloads')
    for e in entries[len(prior):]:
        check(all(e[k]==0 for k in('source_0x0c','type_0x18','integer_0x1c','discriminator_0x20','count_0x24','width_0x28')) and not vector(e['real_storage']) and not vector(e['integer_storage'],True),'gap payload constructor defaults')
    selected=entries[max(1,position)-1]if entries else None
    check(a['pointer']==(selected['address']if selected else'0x0'),'nonpositive position selects head or null')
    cast=one(es,'custom-dynamic-cast');check(cast['input']==cast['payload']==a['pointer'],'typed cast retains payload or null identity')
    if selected is None:
        check(target['dynamic_arrays']==a['store'] and not any(e['operation']=='custom-dynamic-configure-before'for e in es),'null lookup returns without payload consumption');return 0,cursor
    cb=one(es,'custom-dynamic-configure-before');ca=one(es,'custom-dynamic-configure-after');entry=ca['entry']
    check(without(cb['entry'],'index')==without(selected,'index') and cb['requested']=={'source':1,'type':kind,'count':count,'width':width},'mode forced to one and configure ABI')
    wanted=deepcopy(cb['entry']);wanted.update(source_0x0c=1,type_0x18=kind,count_0x24=count,width_0x28=width)
    if kind in(1,2,3):wanted.update(integer_0x1c=int(kind==2),discriminator_0x20=kind)
    check(without(entry,'real_storage','integer_storage')==without(wanted,'real_storage','integer_storage'),'dynamic metadata and retained unknown selector')
    integer=bool(entry['integer_0x1c']);active='integer_storage'if integer else'real_storage';inactive='real_storage'if integer else'integer_storage'
    check(entry[inactive]==cb['entry'][inactive],'inactive typed buffer retained exactly')
    values=resize_check(cb['entry'][active],entry[active],count*width,integer);discriminator=entry['discriminator_0x20']
    rows=[e for e in es if e['operation']=='custom-typed-row-before'];sparse_rows=[e for e in es if e['operation']=='custom-rows-before']
    if discriminator==3 and not integer:
        db=one(es,'custom-dynamic-default-before')['entry'];da=one(es,'custom-dynamic-default-after')['entry'];check(db==entry,'sparse default after configure')
        fill(db[active],da[active],default);check(db[inactive]==da[inactive],'default preserves inactive vector')
        rb=one(es,'custom-rows-before');ra=one(es,'custom-rows-after')
        check(rb['payload']==entry['address'] and (rb['declared_rows'],rb['count'],rb['width'])==(declared,count,width),'dynamic sparse ABI')
        values,status,cursor=sparse(es,data,rb,ra,cursor,[default]*(count*width));check(not rows,'sparse branch no typed lines')
    elif (discriminator==1 and not integer and width in(2,3))or(discriminator==2 and integer and width in(3,4,8)):
        check(not sparse_rows,'typed branch no sparse reader');processed=0
        for b in rows:
            check(not status and processed<count,'no rows after failure or count')
            a=next(e for e in es if e['operation']=='custom-typed-row-after'and e['serial']==b['serial'])
            check(b['payload']==entry['address'] and b['offset']==processed*width and b['width']==width and b['integers']==integer and b['before']==cursor and b['storage']['values_hex']==raw(values,integer),'typed row destination, stride and cursor')
            check(all(a[k]==b[k]for k in('serial','kind','width','integers','payload','offset','before')),'typed row pair ABI')
            kinds=[True]+[integer]*width;fmt='%i'+(' %i'if integer else' %Flf')*width
            fields,status,cursor=line_helper(a,data,kinds,fmt);values[processed*width:(processed+1)*width]=fields[1:]
            check(a['storage']['values_hex']==raw(values,integer),'sequential zero/partial row copied even failure');processed+=1
        check(processed==count or status==1,'all requested typed rows or early failure')
    else:check(not rows and not sparse_rows,'unsupported width/discriminator native no-reader branch')
    final=next(e for e in target['dynamic_arrays']['entries']if e['address']==entry['address'])
    check(without(target['dynamic_arrays'],'entries')==without(lookup_store,'entries'),'dynamic owner/list metadata retained after configuration')
    check(without(final,'index','real_storage','integer_storage')==without(entry,'index','real_storage','integer_storage'),'final dynamic metadata')
    check(final[inactive]==entry[inactive] and final[active]['values_hex']==raw(values,integer),'final active mutation and retained inactive bytes')
    for x in entries:
        if x['address']!=entry['address']:check(x==next(y for y in target['dynamic_arrays']['entries']if y['address']==x['address']),'unselected dynamic entries unchanged')
    return status,cursor


def handler(es,data):
    first,last=es[0],es[-1];old,new=first['state'],last['state'];id=first['id']
    selected=one(es,'object-selected');selector=next(e for e in es if e['operation']=='stream-scan'and e['format']=='%i')
    base.shared.object_selector(selector,data);check(selected['found'] and selected['requested']==selector['value'],'shared object selector')
    obj=next(o for o in old['objects']if o['id']==selected['requested']);target=next(o for o in new['objects']if o['address']==obj['address'])
    cursor=selector['after'];rootvalue=old['custom_last_id_0x33b8']
    if id==198:status,cursor,rootvalue=custom_handler(es,data,obj,target,cursor,rootvalue)
    else:status,cursor=dynamic_handler(es,data,obj,target,cursor)
    expected=deepcopy(old);expected['custom_last_id_0x33b8']=rootvalue
    field='custom_fields'if id==198 else'dynamic_arrays';next(o for o in expected['objects']if o['address']==obj['address'])[field]=deepcopy(target[field])
    check(new==expected,'only verified selected store/root selection may change')
    check(last['status']==(2 if status else 0) and last['stream']==cursor,'parent status mapping and exact payload cursor')
    return {'id':id,'file':first['file'],'dimension':old['dimension'],'status':last['status'],'sequence':first['sequence']}


def validate(evidence,inputs):
    base.validate(evidence,inputs)
    events=[m['payload']for m in evidence['normalized_result']['messages'][1:]];groups=[];g=[];results=[];known={};destroyed=set();cleared=set()
    for e in events:
        if 'entry'in e and e['operation'].startswith('custom-'):
            (custom_entry if 'custom_id'in e['entry']else dynamic_entry)(e['entry'])
        if 'store'in e and e['operation'].startswith('custom-'):store(e['store'],'head'in e['store'])
        if 'state'in e and 'objects'in e['state']:
            states(e['state'])
            for o in e['state']['objects']:known[o['address']]=o
        if e['operation']=='custom-store-destroy-before':
            check(e['object']in known and e['store']==known[e['object']]['custom_fields'],'destruction receives last observed owned custom store')
            check(e['object']not in destroyed,'one active custom destruction');destroyed.add(e['object'])
        elif e['operation']=='custom-store-destroy-after':
            check(e['object']in destroyed and e['destroyed']is True,'paired custom destruction completion');destroyed.remove(e['object'])
        elif e['operation']=='custom-dynamic-clear-before':
            check(e['object']in known and e['store']==known[e['object']]['dynamic_arrays'],'clear receives last observed dynamic list')
            check(e['object']not in cleared,'one active dynamic clear');cleared.add(e['object'])
        elif e['operation']=='custom-dynamic-clear-after':
            check(e['object']in cleared and e['store']['count']==0,'paired dynamic clear completion');cleared.remove(e['object'])
        if e['operation']=='import-before':g=[]
        g.append(e)
        if e['operation']=='import-after':groups.append(g)
    check(not destroyed and not cleared,'all observed destruction/clear calls completed')
    check(groups[0][0]['state']['custom_last_id_0x33b8']==0,'fresh root constructor initializes custom selection zero')
    for g in groups:
        start=None;old=g[0]['state']
        for i,e in enumerate(g):
            if e['operation']=='handler-before':start=i
            elif e['operation']=='handler-after':
                check(start is not None,'paired custom handler boundaries');frame=g[start:i+1];start=None
                if e['id']in IDS:results.append(handler(frame,inputs[e['file']]))
                else:
                    a,b=frame[0]['state'],e['state'];check(a['custom_last_id_0x33b8']==b['custom_last_id_0x33b8'],'other handlers retain root custom selection')
                    before={o['address']:o for o in a['objects']}
                    for o in b['objects']:
                        if o['address']in before:
                            check(all(o[k]==before[o['address']][k]for k in('custom_fields','dynamic_arrays')),'mesh/configuration/deactivation preserves separate stores')
            if e['operation']=='custom-dynamic-clear-after':store(e['store'],True);check(e['store']['count']==0,'dynamic clear empties list')
            if e['operation']=='custom-store-destroy-after':check(e['destroyed']is True,'custom descriptor destruction recorded')
        # Header dimension transitions precede handlers. They destroy objects,
        # while the root scalar belongs to the constructor-only initializer.
        boundaries=[e for e in g if e['operation']in('handler-before','finalize-before')]
        if boundaries and boundaries[0]['state']['dimension']!=old['dimension']:
            check(boundaries[0]['state']['custom_last_id_0x33b8']==old['custom_last_id_0x33b8'],'dimension reset retains last successful custom ID')
        fixed.finalization(g)
    configurations=[e for e in events if e['operation']=='custom-configure-before']
    dynamic=[e for e in events if e['operation']=='custom-dynamic-configure-before']
    coverage={'id_dimensions':sorted(set((e['id'],e['dimension'])for e in results)),
              'failures':sum(e['status']!=0 for e in results),
              'custom_creations':sum(e['operation']=='custom-created'for e in events),
              'custom_nonpositive_ids':sum(e['custom_id']<=0 for e in configurations),
              'custom_unknown_kind_retains_count':sum(e['kind']not in(0,1,2)for e in configurations),
              'custom_zero_width':sum(e['width']==0 for e in configurations),
              'successful_root_updates':sum(e['operation']=='custom-root-update-after'for e in events),
              'dynamic_unknown_selector':sum(e['requested']['type']not in(1,2,3)for e in dynamic),
              'dynamic_type_switches':sum(e['entry']['integer_0x1c']!=int(e['requested']['type']==2) for e in dynamic if e['requested']['type']in(1,2,3)),
              'dynamic_nonpositive_positions':sum(e['index']<=0 for e in events if e['operation']=='custom-dynamic-lookup-before'),
              'typed_row_failures':sum(e['status']!=0 for e in events if e['operation']=='custom-typed-row-after'),
              'sparse_row_failures':sum(e['status']!=0 for e in events if e['operation']=='custom-rows-after'),
              'store_destructions':sum(e['operation']=='custom-store-destroy-before'for e in events)}
    return {'imports':len(groups),'handlers':results,'coverage':coverage,'event_counts':dict(Counter(e['operation']for e in events))}


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for key,path in [('capture','analysis/custom-field-runtime-training-v2.json'),('run-record','analysis/oracle/custom-field-training-v2c/run.json'),('manifest','analysis/custom-field-evaluation-cases-v2.json'),('freeze','analysis/custom-field-candidate-freeze.json'),('input-freeze','analysis/custom-field-v2-input-freeze.json'),('output','analysis/custom-field-observations.json')]:p.add_argument('--'+key,type=Path,default=ROOT/path)
    args=p.parse_args()
    for k in vars(args):setattr(args,k,getattr(args,k).resolve())
    evidence,run,manifest,freeze,input_freeze=map(read,[args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze])
    inputs=provenance(evidence,run,manifest,freeze,input_freeze,args.run_record.parent,args.manifest,args.freeze);result=validate(evidence,inputs)
    rejected=[];not_applied=[]
    def tamper(label,op,mutate,predicate=lambda e:True):
        damaged=deepcopy(evidence);es=[m['payload']for m in damaged['normalized_result']['messages']]
        target=next((e for e in es if e['operation']==op and predicate(e)),None)
        if target is None:not_applied.append({'test':label,'reason':'No qualifying observation.'});return
        mutate(target)
        try:validate(damaged,inputs)
        except (ValueError,KeyError,IndexError,AssertionError,StopIteration,TypeError,struct.error):rejected.append(label)
        else:raise ValueError('tamper accepted '+label)
    tamper('event order','custom-header',lambda e:e.update(sequence=0))
    tamper('header partial output','custom-header',lambda e:e['values'].__setitem__(2,999))
    tamper('header status','custom-header',lambda e:e.update(status=1-e['status']))
    tamper('header cursor','custom-header',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('header source bytes','custom-header',lambda e:e['lines'][0].update(hex='300a'),lambda e:not e['lines'][0]['eof'])
    tamper('custom exact ID','custom-lookup',lambda e:e.update(custom_id=e['custom_id']+1))
    tamper('custom previous shape','custom-configure-after',lambda e:e['entry']['previous'].update(width=999))
    tamper('custom default fill','custom-default-after',lambda e:e['entry']['storage']['values'].__setitem__(0,999),lambda e:bool(e['entry']['storage']['values']))
    tamper('scalar source conversion','custom-scan',lambda e:e.update(value=e['value']+1),lambda e:e['conversions']==1)
    tamper('failed scalar exposed','custom-scan',lambda e:e.update(value=0),lambda e:e['conversions']==-1)
    tamper('sparse row stride','custom-rows-before',lambda e:e.update(width=e['width']+1))
    tamper('native row continuation','custom-rows-after',lambda e:e.update(completion_rva=0))
    tamper('sparse partial writes','custom-rows-after',lambda e:e['storage']['values'].__setitem__(0,999),lambda e:e['status']!=0 and bool(e['storage']['values']))
    tamper('success root ID','custom-root-update-after',lambda e:e.update(value=e['value']+1))
    tamper('root destination offset','custom-root-update-before',lambda e:e.update(target='0x1'))
    tamper('token count','custom-token-header',lambda e:e.update(count=e['count']+1),lambda e:e['status']==0)
    tamper('token contents','custom-token-access',lambda e:e.update(value_hex='39'))
    tamper('base10 conversion','custom-token-integer',lambda e:e.update(value=e['value']+1))
    tamper('position lookup','custom-dynamic-lookup-after',lambda e:e.update(pointer='0x0'),lambda e:e['pointer']!='0x0')
    tamper('dynamic selector flag','custom-dynamic-configure-after',lambda e:e['entry'].update(integer_0x1c=1-e['entry']['integer_0x1c']))
    tamper('active physical extent','custom-dynamic-configure-after',lambda e:e['entry']['real_storage'].update(physical_items=99))
    tamper('inactive retained buffer','custom-dynamic-configure-after',lambda e:e['entry']['integer_storage'].update(values_hex='00'*e['entry']['integer_storage']['physical_items']*4),lambda e:not e['entry']['integer_0x1c'] and any(e['entry']['integer_storage']['values']))
    tamper('typed row offset','custom-typed-row-before',lambda e:e.update(offset=e['offset']+1))
    tamper('typed row partial output','custom-typed-row-after',lambda e:e['values'].__setitem__(-1,999))
    tamper('typed row failed status','custom-typed-row-after',lambda e:e.update(status=0),lambda e:e['status']==1)
    tamper('typed negative zero raw bits','custom-typed-row-after',lambda e:e['output_hex'].__setitem__(next(i for i,x in enumerate(e['output_hex'])if x=='0000000000000080'),'0000000000000000'),lambda e:'0000000000000080'in e['output_hex'])
    tamper('dynamic list clear','custom-dynamic-clear-after',lambda e:e['store'].update(count=1))
    tamper('failure root selection mutation','handler-after',lambda e:e['state'].update(custom_last_id_0x33b8=999),lambda e:e['id']==198 and e['status']==2)
    tamper('unrelated persistent field','handler-after',lambda e:e['state']['objects'][0].update(material_group_0x3980=999),lambda e:e['id']in IDS and bool(e['state']['objects']))
    tamper('final mode normalization','finalize-after',lambda e:e['state']['finalization_controls'].update(mode_0x990=999))
    for label,key,value in [('process identity','pid',-1),('capture completeness','messages_truncated',True),('probe source','source_sha256','0'*64)]:
        damaged=deepcopy(evidence);damaged['normalized_result'][key]=value
        try:provenance(damaged,run,manifest,freeze,input_freeze,args.run_record.parent,args.manifest,args.freeze)
        except ValueError:rejected.append(label)
        else:raise ValueError('provenance tamper accepted '+label)
    for label,which in [('fixture bytes','manifest'),('frozen archive','freeze'),('input archive','input')]:
        m,f,i=deepcopy(manifest),deepcopy(freeze),deepcopy(input_freeze)
        (m['cases'][0]if which=='manifest'else f['files'][0]if which=='freeze'else i['inputs'][0])['sha256']='0'*64
        try:provenance(evidence,run,m,f,i,args.run_record.parent,args.manifest,args.freeze)
        except ValueError:rejected.append(label)
        else:raise ValueError('binding tamper accepted '+label)
    report={'status':'PASS','result':result,'bindings':[binding(x)for x in[Path(__file__),args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze,PROBE,CONTRACT]],
            'tampering_rejected':rejected,'tampering_not_applied':not_applied,
            'dependencies':[binding(ROOT/'tools'/n)for n in['verify_mesh_field_observations.py','verify_material_column_observations.py','verify_material_object_observations.py','verify_material_reference_observations.py','verify_material_import_observations.py','verify_boundary_sort_observations.py','verify_mesh_array_observations.py','verify_mesh_refresh_observations.py','verify_geometry_segments_observations.py','verify_time_controls_observations.py','verify_step_distance_observations.py','capture_oracle.py']],
            'limitations':['Original-only source-byte audit; no candidate outputs or new comparator imported.','Selected bounded owned profile only; arbitrary COW aliases, negative/overflowing extents, OOM, broad CRT range/locale/nonfinite behavior excluded.','Unused capacity bytes and unrelated root metadata are never inferred as initialized values.']}
    args.output.write_text(json.dumps(report,indent=2)+'\n');print('PASS',len(result['handlers']),'custom handlers;',len(rejected),'applied corruption checks')


if __name__=='__main__':main()
