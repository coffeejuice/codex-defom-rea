"""Original-only audit of material assignment, column producers and rank remaps.

Uses frozen source bytes and passive native observations, never replay output.
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
import verify_material_reference_observations as references
from verify_material_import_observations import verify_input_freeze
from verify_mesh_array_observations import descriptor_map
from verify_step_distance_observations import line_read
from verify_time_controls_observations import stream_position

PROBE=ROOT/'tools/observe_material_column_import.js'
CONTRACT=ROOT/'analysis/material-column-model-contract.json'
GROUP='material_group_0x3980'


def provenance(evidence,run,manifest,freeze,input_freeze,directory,manifest_path,freeze_path):
    shared.PROBE=PROBE;shared.CONTRACT=CONTRACT
    combined={**freeze,'inputs':input_freeze['inputs']}
    inputs=shared.provenance(evidence,run,manifest,combined,directory)
    verify_input_freeze(input_freeze,manifest,manifest_path,freeze_path)
    for item in freeze['files']:
        check(digest(ROOT/item['path'])==item['sha256'],'current candidate still frozen')
    return inputs


def raw(values,integer=False):
    return b''.join(struct.pack('<i' if integer else '<d',v) for v in values).hex()


def array(e,integer=False):
    values=e['values']
    check(isinstance(values,list) and len(values)<=512,'bounded captured array')
    check(all(type(v)is int and -(2**31)<=v<2**31 for v in values) if integer else
          all(type(v)in(int,float) and math.isfinite(v) for v in values),'defined finite numeric array')
    payload=bytes.fromhex(e['values_hex']);size=4 if integer else 8
    check(len(payload)==len(values)*size,'captured raw array extent')
    decoded=list(struct.unpack('<'+('i'if integer else'd')*len(values),payload)) if values else []
    check(all(a==b for a,b in zip(values,decoded)),'captured raw/numeric consistency')
    check(integer or all(math.isfinite(v)for v in decoded),'finite raw double array')
    # JSON's numeric telemetry loses the sign of zero; IEEE-754 bytes retain it.
    return decoded


def one(es,op):
    found=[e for e in es if e['operation']==op]
    check(len(found)==1,'one '+op)
    return found[0]


def lex(data,pos,integer):
    while pos<len(data) and data[pos] in b' \t\r\n\v\f':pos+=1
    start=pos
    pattern=(rb'[+-]?(?:0[xX][0-9a-fA-F]+|0[0-7]*|[1-9][0-9]*)' if integer else
             rb'[+-]?(?:[0-9]+(?:\.[0-9]*)?|\.[0-9]+)(?:[eE][+-]?[0-9]+)?')
    m=re.match(pattern,data[pos:])
    if not m:
        # These fixtures exclude the native incomplete-sign/exponent ambiguity.
        check(pos==len(data) or data[pos:pos+1] not in (b'+',b'-',b'.'),'selected complete numeric grammar')
        return None,-1 if pos==len(data) else 0,pos
    token=m[0];pos+=len(token)
    if integer:
        unsigned=token.lstrip(b'+-')
        base=16 if unsigned.lower().startswith(b'0x') else 8 if unsigned.startswith(b'0') else 10
        value=int(token,base);check(-(2**31)<=value<2**31,'bounded int conversion')
    else:
        value=float(token);check(math.isfinite(value),'finite real conversion')
    check(pos>start,'numeric progress')
    return value,1,pos


def stream_after(data,pos):
    return {'cursor':pos,'count':len(data)-pos,'buffer_hex':data.hex()} if pos<len(data) else {'cursor':0,'count':0,'buffer_hex':''}


def scan(e,data,cursor,integer):
    check(e['before']==cursor and e['format']==('%i' if integer else '%Flf'),'scalar order/type')
    value,converted,end=lex(data,stream_position(cursor,data),integer)
    check(e['conversions']==converted and e['after']==stream_after(data,end),'scalar source/cursor/conversion')
    if converted==1:
        check(e['value']==value and e['output_hex']==raw([value],integer),'scalar output from source bytes')
    else:
        check(e['value'] is None and e['output_hex'] is None,'failed scalar destination not exposed')
    return value,converted,e['after']


def line(e,data,cursor):
    check(e['before']==cursor and e['limit']==1024,'line order/limit')
    content,end,eof,after=line_read(data,stream_position(cursor,data))
    check(e['eof']==eof and e['hex']==(None if eof else content.hex()) and e['after']==after,'line source/cursor')
    return content,eof,after


def header_scan(content,integer):
    values=[];pos=0;conversion=0
    for kind in ([True,True] if integer else [True,True,False]):
        value,c,pos=lex(content,pos,kind)
        if c!=1:
            conversion=-1 if c==-1 and not values else len(values)
            break
        values.append(value);conversion=len(values)
    return values,conversion


def detailed(es,before,after,data,integer):
    serial=before['serial'];kind=before['kind'];width=before['width' if integer else 'input_width']
    count=before['element_count'];declared=before['declared_rows'];values=array(before,integer)[:]
    check(len(values)==count*width,'detailed row-major extent')
    check(all(before[k]==after[k] for k in ('serial','parent_serial','kind','declared_rows','element_count')),'detailed identity')
    if kind=='explicit' and before['explicit_width']==0:
        child=one(es,'column-rows-before');end=one(es,'column-rows-after')
        check(child['kind']=='implicit' and child['parent_serial']==serial and child['stream']==before['stream']
              and child['values_hex']==before['values_hex'],'zero explicit width delegates implicit reader')
        inner=[e for e in es if child['sequence']<e['sequence']<end['sequence']]
        result,status,cursor=detailed(inner,child,end,data,False)
    else:
        row_width=width if integer or kind=='implicit' else before['explicit_width']
        scans=[e for e in es if e['operation']=='column-scan' and e['reader_serial']==serial]
        lines=[e for e in es if e['operation']=='column-line' and e['reader_serial']==serial]
        row=[0 if integer else 0.0]*row_width;row_id=None;cursor=before['stream'];index=0;status=0
        for _ in range(max(0,declared)):
            check(index<len(scans),'complete row ID scan');e=scans[index];index+=1
            value,c,cursor=scan(e,data,cursor,True)
            if c==-1:status=1;break
            if c==1:row_id=value
            check(row_id is not None,'first row-ID matching failure excluded')
            for column in range(row_width):
                check(index<len(scans),'complete scalar scan');e=scans[index];index+=1
                value,c,cursor=scan(e,data,cursor,integer)
                if c==-1:status=1;break
                if c==1:row[column]=value
            if status:break
            if 1<=row_id<=count:
                for column in range(min(width,row_width)):values[(row_id-1)*width+column]=row[column]
        check(index==len(scans),'no surplus row scalars')
        discard=(integer or kind=='implicit') and declared>0 and not status
        check(len(lines)==int(discard),'exact terminal line call')
        if discard:
            _,eof,cursor=line(lines[0],data,cursor);status=int(eof)
        result=values
    check(after['status']==status and after['stream']==cursor,'detailed status/cursor')
    check(array(after,integer)==result and after['values_hex']==raw(result,integer),'detailed partial row commits')
    return result,status,cursor


def reader(es,before,after,data,integer):
    serial=before['serial'];width=before['width' if integer else 'input_width'];count=before['element_count']
    check(before['kind']==after['kind'] and before['serial']==after['serial'],'reader identity')
    initial=array(before,integer);check(len(initial)==count*width,'reader initial extent')
    if not integer:check(before['mode']==1 and initial==[0.0]*(count*width),'fresh zeroed column scratch')
    lines=[e for e in es if e['operation']=='column-line' and e['reader_serial']==serial]
    check(len(lines)==1,'exactly one header fgets')
    content,eof,cursor=line(lines[0],data,before['stream']);values=initial[:]
    status=1;scans=[]
    rows=[e for e in es if e['operation']==('group-rows-before' if integer else 'column-rows-before')
          and e['parent_serial']==serial]
    if not eof:
        fields,conversions=header_scan(content,integer)
        scans=[{'format':'%i %i' if integer else '%i %i %Flf','input_hex':content.hex(),'conversions':conversions}]
        if conversions>=1:
            if (integer and conversions>=2) or (not integer and conversions>=3):
                values=[fields[1 if integer else 2]]*(count*width)
            check(len(rows)==1,'valid header invokes rows helper');b=rows[0]
            end=next(e for e in es if e['operation']==('group-rows-after' if integer else 'column-rows-after') and e['serial']==b['serial'])
            check(b['declared_rows']==fields[0] and b['element_count']==count and b['stream']==cursor
                  and b['values_hex']==raw(values,integer),'header default/count before detailed rows')
            if not integer:check(b['explicit_width']==(fields[1]if len(fields)>1 else 0),'header explicit width')
            inner=[e for e in es if b['sequence']<e['sequence']<end['sequence']]
            values,status,cursor=detailed(inner,b,end,data,integer)
        else:
            check(not rows,'failed header has no row helper')
            check(conversions==-1 or not integer,'uninitialized integer header excluded')
    else:check(not rows,'EOF header has no row helper')
    check(after['scans']==scans,'header scanf source and conversions')
    observed_lines=[{'limit':e['limit'],'eof':e['eof'],'hex':e['hex']} for e in es if e['operation']=='column-line']
    check(after['lines']==observed_lines,'complete reader line inventory')
    check(after['status']==status and after['stream']==cursor,'reader status/cursor')
    check(array(after,integer)==values and after['values_hex']==raw(values,integer),'reader scratch/partial destination')
    return values,status,cursor


def mapping(e,object,materials):
    check(e['object']==object['address'] and e['mesh_flag']==object['mesh_flag']!=0,'mapping owned live mesh')
    check(e['direct61']==object['buffer_metadata']['direct61'] and e['materials']==materials,'mapping live assigned IDs and material records')
    rs=materials['records'];ids=sorted(r['id']for r in rs);marks=[0]*len(ids);previous=0
    direct=e['direct61'];n=object['elements']
    check(direct['known_prefix']>=n and e['material_count']==len(ids),'mapping initialized extent')
    for id in direct['values'][:n]:
        if id==previous:continue
        previous=id
        if id not in ids:continue
        rank=ids.index(id);r=next(r for r in rs if r['id']==id)
        if marks[rank]!=0 or r[references.MODE]!=1:continue
        marks[rank]=1
        for ref in r['reference_ids']:
            if ref in ids:marks[ids.index(ref)]=2
    result=[];width=0
    for flag in marks:
        result.append(width if flag==2 else -1)
        if flag==2:width+=1
    check(e['map']==result and e['map_hex']==raw(result,True) and e['input_width']==width,'ordered native mark/rank mapping')
    return result,width


def descriptor(o,id):return next(d for d in o['descriptors']if d['id']==id)


def object_at(state,address):return next(o for o in state['objects']if o['address']==address)


def column_handler(es,data):
    first,last=es[0],es[-1];before=one(es,'column-wrapper-before');after=one(es,'column-wrapper-after')
    old=object_at(first['state'],before['object']);new=object_at(last['state'],before['object'])
    id=49 if first['id']==205 else 50;d=descriptor(old,id);end=descriptor(new,id)
    check(before['id']==after['id']==first['id'] and before['destination']==d['storage_pointer']
          and before['element_count']==old['elements'] and before['destination_width']==first['state']['components'][id]
          and before['values_hex']==d['values_hex'],'wrapper bound descriptor/extent')
    array(before);array(after)
    m=one(es,'column-map-after');columns,width=mapping(m,old,first['state']['material_list'])
    b=one(es,'column-reader-before');a=one(es,'column-reader-after')
    check(before['sequence']<m['sequence']<b['sequence']<a['sequence']<after['sequence'],'map before scratch read before commit')
    check(m['wrapper_serial']==b['wrapper_serial']==before['serial']==after['serial']
          and b['input_width']==width and b['element_count']==old['elements'] and b['stream']==before['stream'],'wrapper reader linkage')
    inner=[e for e in es if b['sequence']<e['sequence']<a['sequence']]
    values,status,cursor=reader(inner,b,a,data,False);wanted=array(before)[:]
    if not status:
        for row in range(old['elements']):
            for col in range(before['destination_width']):
                if columns[col]>=0:wanted[row*before['destination_width']+col]=values[row*width+columns[col]]
    check(after['values_hex']==raw(wanted) and end['values_hex']==raw(wanted),'success-only mapped destination commit')
    check(after['status']==status and last['status']==2*bool(status) and after['stream']==last['stream']==cursor,'wrapper/handler status and cursor')
    for x,y in zip(old['descriptors'],new['descriptors']):
        if x['id']!=id:check(x==y,'column producer preserves other descriptors')
    check(old['buffer_metadata']==new['buffer_metadata'] and old[GROUP]==new[GROUP],'column producer preserves direct data/default')
    return {'id':first['id'],'rows':old['elements'],'width':width,'status':status,'nonzero_committed':sum(v!=0 for v in wanted) if not status else 0}


def group_handler(es,data):
    first,last=es[0],es[-1];b=one(es,'group-reader-before');a=one(es,'group-reader-after')
    old=object_at(first['state'],b['object']);new=object_at(last['state'],b['object'])
    check(b['element_count']==old['elements'] and b['width']==first['state']['components'][61]==1
          and b['values']==old['direct']['61'],'group reader initial binding')
    inner=[e for e in es if b['sequence']<e['sequence']<a['sequence']]
    values,status,cursor=reader(inner,b,a,data,True)
    p=one(es,'group-postprocess-before');q=one(es,'group-postprocess-after')
    check(a['sequence']<p['sequence']<q['sequence'] and p['object']==q['object']==old['address'],'unconditional postprocess order/identity')
    check(p[GROUP]==old[GROUP] and p['direct61']['values']==values and p['element_count']==old['elements']
          and p['mesh_flag']==old['mesh_flag'],'postprocess receives mutated group buffer')
    wanted=old[GROUP];selected=0;counts=Counter();order=[]
    if p['mesh_flag'] and p['direct61']['pointer']!='0x0':
        for value in values[:p['element_count']]:
            if value not in counts:order.append(value)
            counts[value]+=1
        if order:selected=max(order,key=lambda value:counts[value])
        if selected:wanted=selected
    check(all(q[k]==(wanted if k==GROUP else v) for k,v in p.items() if k not in ('operation','sequence')),
          'postprocess first-tie/majority/zero-retain only mutation')
    check(new[GROUP]==wanted and new['direct']['61']==values and last['status']==2*bool(status)
          and last['stream']==cursor,'group handler partial effects/status/cursor')
    return {'status':status,'selected':selected,'previous':old[GROUP],'result':wanted,'rows':old['elements'],
            'tie':len(counts)>1 and len([v for v in counts.values()if v==max(counts.values())])>1}


def refresh(es):
    first,last=es[0],es[-1];old,new=first['state'],last['state'];scratch=old['material_list']['scratch']
    enabled=any(r[references.MODE]==1 for r in new['material_list']['records'])
    remap=first['option']==1 and enabled and scratch['ids_pointer']!='0x0' and scratch['ranks_pointer']!='0x0'
    projected=deepcopy(es);projected_last=projected[-1]['state'];nonzero=0;multirow=0;rank_changes=0
    if scratch['allocation_observed']:
        ids=scratch['ids'];check(scratch['ranks']==[sum(x<id for x in ids)for id in ids],'old ID/rank scratch binding')
    for a,b,p in zip(old['objects'],new['objects'],projected_last['objects']):
        if a['type']==7 or not remap:continue
        rows=a['elements'];ow=old['components'][49];nw=new['components'][49]
        current_ids=[r['id']for r in new['material_list']['records']]
        check(ow<=scratch['count'] and nw==len(current_ids),'remap saved/new material widths')
        for id in (49,50):
            source=descriptor(a,id);target=descriptor(b,id);dest=descriptor(p,id)
            wanted=[0.0]*(rows*nw)
            for c in range(ow):
                material=scratch['ids'][c];oldrank=scratch['ranks'][c]
                if material not in current_ids:continue
                newrank=sum(x<material for x in current_ids);rank_changes+=int(oldrank!=newrank)
                source_values=array(source)
                for row in range(rows):wanted[row*nw+newrank]=source_values[row*ow+oldrank]
            check(target['values_hex']==raw(wanted),'append row-major rank permutation with zero new columns')
            nonzero+=sum(v!=0 for v in wanted);multirow+=int(rows>1 and any(v!=0 for v in wanted))
            # Reuse old allocation/copy audit only after independently checking
            # the remap; substitute its expected preserving-resize projection.
            payload=bytes.fromhex(source['values_hex']);size=target['length']*8
            payload=payload[:size]+bytes(max(0,size-len(payload)))
            dest['values_hex']=payload.hex();dest['values']=list(struct.unpack('<'+'d'*(len(payload)//8),payload)) if payload else []
    copies,updates=shared.refresh_events(projected)
    return {'copies':copies,'container_updates':updates,'nonzero_remapped':nonzero,'multirow_arrays':multirow,'rank_changes':rank_changes}


# Reference allocation/finalizer audit retained locally to allow arbitrary
# verified unknown-token spellings; the earlier verifier stays immutable.
def reference_observations(evidence,inputs):
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
            references.ints(e['values_hex'],e['values']);live[e['pointer']]={k:e[k]for k in ['capacity','material','serial']};allocs+=1
        for r in references.records_in(e):references.record_shape(r,live)
        for s in references.lists_in(e):
            references.list_shape(s)
            if op in ('import-before','import-after','handler-before','handler-after','material-reference-finalize-before','material-reference-finalize-after'):
                check(s['root_count']==s['count'],'boundary root/list count agreement')
        if op=='material-reference-free-before':
            check(pending_free is None and e['pointer']in live and all(live[e['pointer']][k]==e[k]for k in ['capacity','material','serial']),'free live allocation identity')
            references.ints(e['values_hex'],e['values']);check(len(e['values'])==e['capacity'],'free byte extent');pending_free=e
            if e['material']in destructors:references.check_free(e,destructors[e['material']])
        elif op=='material-reference-free-after':
            check(pending_free is not None and all(e[k]==pending_free[k]for k in ['pointer','capacity','material','serial','call_return_rva']),'free paired event')
            del live[e['pointer']];pending_free=None
        elif op=='material-destroy-before':destructors[e['record']['address']]=e['record']
        elif op=='material-destroy-after':
            check(e['address']in destructors and all(v['material']!=e['address']for v in live.values()),'destructor releases retained allocation');del destructors[e['address']]
        elif op=='material-reference-feature':
            want=int(e['control_0x85c']!=0 or any(r[references.MODE]==1 for r in e['state']['records']))
            check(49<=e['feature']<=52 and e['result']==want,'feature49..52 live material modes');features[f"{e['feature']}:{want}"]+=1
        if op=='import-before':check(group is None,'import nesting');group=[]
        check(group is not None and e['file']==(group[0]['file']if group else e['file']),'event import ownership');group.append(e)
        if op=='import-after':groups.append(group);group=None
    check(group is None and pending_free is None and not destructors,'complete event lifecycle')
    check([g[0]['file']for g in groups]==list(inputs),'ordered complete imports')
    results=[];total_lookups=0;all_handlers=[];retained=Counter();nonzero_columns=0
    catalog={r['name']:(r['id'],r['category'])for r in read(ROOT/'analysis/keyword-catalog.json')['rows']}
    for g in groups:
        data=inputs[g[0]['file']];active=None;fstart=None;statuses=[];finals=0;last_materials=g[0]['state']['material_list']['records'];last_token=None
        for i,e in enumerate(g):
            op=e['operation']
            if op=='stream-scan' and e['format']=='%39s':references.verify_token(e,data);last_token=e
            if op=='keyword-resolved':
                check(last_token is not None and last_token['conversions']==1 and last_token['value']==e['token']
                      and (e['id'],e['category'])==catalog.get(e['token'],(305,0)),
                      'exact source token and retained catalog lookup')
            if op=='handler-before':check(active is None,'handler nesting');active=i
            elif op=='handler-after':
                check(active is not None,'handler leave pairing');info=references.handler(g[active:i+1],data);active=None
                statuses.append(e['status']);last_materials=e['state']['material_list']['records']
                if info:all_handlers.append(info)
            elif op=='material-clear-after' and e['state']['owner']==g[0]['state']['address']:
                last_materials=e['state']['records']
            elif op=='material-reference-finalize-before':
                check(fstart is None and active is None,'reference finalizer after parser');fstart=i
                check(references.projection(e['state']['records'])==references.projection(last_materials),'finalizer receives parser material state')
            elif op=='material-reference-finalize-after':
                check(fstart is not None,'reference finalizer pairing');total_lookups+=references.finalize(g[fstart:i+1]);fstart=None;finals+=1
                last_materials=e['state']['records']
            if op=='material-refresh-after':
                for o in e['state']['objects']:
                    for d in o['descriptors']:
                        if d['id'] in (49,50):nonzero_columns+=sum(v!=0 for v in (d['values'] or []))
        check(active is None and fstart is None and finals==1,'one complete material finalizer')
        check(references.projection(g[-1]['state']['material_list']['records'])==references.projection(last_materials),'post-import material state')
        expected=next((s for s in reversed(statuses)if s),0)
        unknown=any(e['operation']=='keyword-resolved'and e['id']==305 for e in g)
        # Sentinel305 emits a warning but does not replace the accumulated status.
        if unknown:
            for token in (e for e in g if e['operation']=='keyword-resolved' and e['id']==305):
                check(token['category']==0 and any(scan['operation']=='stream-scan' and scan.get('format')=='%39s' and scan.get('value')==token['token'] and scan['sequence']<token['sequence'] for scan in g),'unknown token follows verified source scan')
        check(g[-1]['status']==expected,'parser/finalizer retained status')
        rs=g[-1]['state']['material_list']['records']
        for r in rs:
            if r[references.PTR]!='0x0' and r[references.COUNT]<=0:retained[str(r[references.COUNT])]+=1
        results.append({'file':g[0]['file'],'status':g[-1]['status'],'sequence':[g[0]['sequence'],g[-1]['sequence']],
                        'materials':[{'id':r['id'],'mode':r[references.MODE],'logical_count':r[references.COUNT],
                                      'capacity':r['reference_allocation']['capacity'],'physical_ids':r['reference_allocation']['values']}for r in rs]})
    check(not live,'last reset releases all tracked reference allocations')
    return {'imports':results,'handlers':all_handlers,'allocation_count':allocs,'finalizer_lookups':total_lookups,
            'features':dict(features),'retained_nonpositive_snapshots':dict(retained),
            'nonzero_descriptor49_50_values_at_refresh':nonzero_columns,'event_counts':dict(Counter(e['operation']for e in events))}


def validate(evidence,inputs):
    reference_result=reference_observations(evidence,inputs)
    events=[m['payload']for m in evidence['normalized_result']['messages'][1:]]
    rows=descriptor_map(read(ROOT/'analysis/mesh-array-contract.json'));groups=[];current=[];previous=None;handlers=[];refreshes=[];defaults=[]
    for e in events:
        if e['operation']=='import-before':current=[]
        current.append(e)
        if e['operation']=='import-after':groups.append(current)
        if 'state'in e and 'objects'in e['state']:
            for o in e['state']['objects']:
                shared.object_shape(o,rows);check(type(o[GROUP])is int and -(2**31)<=o[GROUP]<2**31,'signed persistent group')
        if e['operation']=='column-map-after':
            shared.buffer_shape(e['direct61'],'direct61',e['object'])
        if e['operation'].startswith('group-postprocess-'):
            shared.buffer_shape(e['direct61'],'direct61',e['object'])
    for g in groups:
        data=inputs[g[0]['file']]
        if previous is not None:check(previous==g[0]['state'],'between-import complete continuity')
        previous=g[-1]['state'];start=None;rstart=None
        for index,e in enumerate(g):
            if e['operation']=='material-refresh-before':check(rstart is None,'refresh nesting');rstart=index
            elif e['operation']=='material-refresh-after':
                check(rstart is not None,'refresh pairing');refreshes.append(refresh(g[rstart:index+1]));rstart=None
            if e['operation']=='handler-before':start=index
            elif e['operation']=='handler-after':
                check(start is not None,'handler boundary');es=g[start:index+1];start=None;id=e['id']
                if id in (205,216):handlers.append({'file':e['file'],**column_handler(es,data)})
                elif id==174:handlers.append({'file':e['file'],'id':id,**group_handler(es,data)})
                elif id==173:
                    old={o['address']:o for o in es[0]['state']['objects']}
                    for o in e['state']['objects']:
                        prior=old.get(o['address']);value=prior[GROUP]if prior else 0
                        check(o[GROUP]==value,'connectivity preserves group default')
                        replacements=[x for x in es if x['operation']=='allocation-update' and x['object']==o['address'] and x['kind']=='elements' and x['mode']=='replacement']
                        if replacements:
                            check(o['direct']['61']==[max(value,0)]*len(o['direct']['61']),'positive-only ELMCON material default fill')
                            defaults.append({'default':value,'effective':max(value,0),'count':len(o['direct']['61'])})
                elif id==145:
                    old={o['address']:o for o in es[0]['state']['objects']}
                    for o in e['state']['objects']:check(o[GROUP]==(old[o['address']][GROUP]if o['address']in old else 0),'creation/deactivation group lifecycle')
                else:
                    old={o['address']:o for o in es[0]['state']['objects']}
                    for o in e['state']['objects']:
                        if o['address']in old:check(o[GROUP]==old[o['address']][GROUP],'other handler retains group default')
        check(start is None and rstart is None,'closed handlers and refreshes')
    coverage={'columns_success':sum(x['id']in(205,216) and x['status']==0 for x in handlers),
        'columns_failure_no_commit':sum(x['id']in(205,216) and x['status']!=0 for x in handlers),
        'group_first_ties':sum(x.get('tie',False)for x in handlers),
        'group_zero_retained':sum(x.get('selected')==0 and x.get('result')!=0 for x in handlers),
        'negative_replacement_defaults_zeroed':sum(x['default']<0 and x['effective']==0 for x in defaults),
        'positive_replacement_defaults':sum(x['default']>0 for x in defaults),
        'negative_zero_reader_slots':sum(e['values_hex'][i:i+16]=='0000000000000080'for e in events if e['operation']=='column-reader-after'for i in range(0,len(e['values_hex']),16)),
        'negative_zero_remapped_slots':sum(d['values_hex'][i:i+16]=='0000000000000080'for e in events if e['operation']=='material-refresh-after'for o in e['state']['objects']for d in o['descriptors']if d['id']in(49,50)for i in range(0,len(d['values_hex']),16)),
        **{k:sum(x[k]for x in refreshes)for k in ('copies','container_updates','nonzero_remapped','multirow_arrays','rank_changes')}}
    return {'imports':len(groups),'coverage':coverage,'column_handlers':handlers,'refreshes':refreshes,'connectivity_defaults':defaults,
            'references':reference_result,'event_counts':dict(Counter(e['operation']for e in events))}


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--capture',type=Path,default=ROOT/'analysis/material-column-runtime-training.json')
    p.add_argument('--run-record',type=Path,default=ROOT/'analysis/oracle/material-column-training/run.json')
    p.add_argument('--manifest',type=Path,default=ROOT/'tests/fixtures/material_columns/cases.json')
    p.add_argument('--freeze',type=Path,default=ROOT/'analysis/material-column-candidate-freeze.json')
    p.add_argument('--input-freeze',type=Path,default=ROOT/'analysis/material-column-input-freeze.json')
    p.add_argument('--output',type=Path,default=ROOT/'analysis/material-column-observations.json')
    args=p.parse_args()
    for key in ('capture','run_record','manifest','freeze','input_freeze','output'):setattr(args,key,getattr(args,key).resolve())
    evidence,run,manifest,freeze,input_freeze=map(read,[args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze])
    inputs=provenance(evidence,run,manifest,freeze,input_freeze,args.run_record.parent,args.manifest,args.freeze)
    result=validate(evidence,inputs);rejected=[];not_applied=[]
    def tamper(label,op,mutate,predicate=lambda e:True):
        damaged=deepcopy(evidence);es=[m['payload']for m in damaged['normalized_result']['messages']]
        target=next((e for e in es if e['operation']==op and predicate(e)),None)
        if target is None:
            not_applied.append({'test':label,'reason':'No qualifying native observation in this capture.'});return
        mutate(target)
        try:validate(damaged,inputs)
        except (ValueError,KeyError,IndexError,AssertionError,StopIteration,TypeError,struct.error):rejected.append(label)
        else:raise ValueError('tamper accepted: '+label)
    tamper('event sequence','column-map-after',lambda e:e.update(sequence=0))
    tamper('map selected rank','column-map-after',lambda e:e['map'].__setitem__(0,77))
    tamper('map raw bytes','column-map-after',lambda e:e.update(map_hex='00'*len(bytes.fromhex(e['map_hex']))))
    tamper('input width','column-map-after',lambda e:e.update(input_width=e['input_width']+1))
    tamper('live source assignments','column-map-after',lambda e:e['direct61']['values'].__setitem__(0,777))
    tamper('source material mode','column-map-after',lambda e:e['materials']['records'][1].update(mode_0x1cc8=0))
    tamper('header source','column-line',lambda e:e.update(hex='30203020390a'),lambda e:e['kind']=='columns' and not e['eof'])
    tamper('header cursor','column-line',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('scalar value','column-scan',lambda e:e.update(value=e['value']+1),lambda e:e['conversions']==1)
    tamper('scalar bytes','column-scan',lambda e:e.update(output_hex='00'*8),lambda e:e['conversions']==1 and e['format']=='%Flf' and e['value']!=0)
    tamper('failed scalar exposed','column-scan',lambda e:e.update(value=0),lambda e:e['conversions']==-1)
    tamper('scratch initial zero','column-reader-before',lambda e:e['values'].__setitem__(0,1),lambda e:bool(e['values']))
    tamper('scratch partial row','column-reader-after',lambda e:e['values'].__setitem__(0,777),lambda e:bool(e['values']))
    tamper('reader status','column-reader-after',lambda e:e.update(status=1-e['status']))
    tamper('explicit row width','column-rows-before',lambda e:e.update(explicit_width=e['explicit_width']+1),lambda e:e['kind']=='explicit')
    tamper('row count','column-rows-before',lambda e:e.update(declared_rows=e['declared_rows']+1))
    tamper('failure destination commit','column-wrapper-after',lambda e:e['values'].__setitem__(0,777),lambda e:e['status']==1 and bool(e['values']))
    tamper('group partial write','group-reader-after',lambda e:e['values'].__setitem__(0,777),lambda e:bool(e['values']))
    tamper('group selected tie/majority','group-postprocess-after',lambda e:e.update(material_group_0x3980=e[GROUP]+1))
    tamper('group zero retention','group-postprocess-after',lambda e:e.update(material_group_0x3980=999),lambda e:all(v==0 for v in e['direct61']['values']) and bool(e['direct61']['values']))
    tamper('old material rank','material-refresh-before',lambda e:e['state']['material_list']['scratch']['ranks'].__setitem__(0,999),lambda e:bool(e['state']['material_list']['scratch'].get('ranks')))
    tamper('nonzero remapped value','material-refresh-after',lambda e:descriptor(e['state']['objects'][0],49)['values'].__setitem__(0,999),lambda e:bool(e['state']['objects']) and any(descriptor(e['state']['objects'][0],49)['values']or[]))
    tamper('positive-only replacement default','handler-after',lambda e:e['state']['objects'][0]['direct']['61'].__setitem__(0,-99),lambda e:e['id']==173 and bool(e['state']['objects'][0]['direct']['61']))
    def coherent_values(e,values,integer=False):
        e['values']=values;e['values_hex']=raw(values,integer)
    def transpose_remap(e):
        o=e['state']['objects'][0];d=descriptor(o,49);r=o['elements'];w=d['width'];v=d['values']
        coherent_values(d,[v[row*w+column]for column in range(w)for row in range(r)])
    tamper('coherent remap row-stride transpose','material-refresh-after',transpose_remap,
           lambda e:bool(e['state']['objects']) and e['state']['objects'][0]['elements']>1 and any(descriptor(e['state']['objects'][0],49)['values']or[]))
    tamper('coherent failed wrapper commit','column-wrapper-after',lambda e:coherent_values(e,[e['values'][0]+1]+e['values'][1:]),
           lambda e:e['status']==1 and bool(e['values']))
    tamper('coherent header default before rows','column-rows-before',lambda e:coherent_values(e,[v+1 for v in e['values']]),
           lambda e:e['kind']=='explicit' and bool(e['values']) and e['values'][0]!=0 and all(v==e['values'][0]for v in e['values']))
    def coherent_map(e):
        e['map'][0]=-1;e['map_hex']=raw(e['map'],True)
    tamper('coherent selected map removed','column-map-after',coherent_map,lambda e:e['map'][0]>=0)
    def negative_default(e):
        o=e['state']['objects'][0];v=[o[GROUP]]*len(o['direct']['61']);o['direct']['61']=v
        b=o['buffer_metadata']['direct61'];b['values']=v;b['known_hex']=raw(v,True)
    tamper('coherent negative default wrongly propagated','handler-after',negative_default,
           lambda e:e['id']==173 and e['state']['objects'][0][GROUP]<0 and bool(e['state']['objects'][0]['direct']['61']))
    tamper('last rather than first tied group','group-postprocess-after',lambda e:e.update(material_group_0x3980=e['direct61']['values'][-1]),
           lambda e:len(set(e['direct61']['values']))==3)
    def flip_negative_zero(e):
        chunks=[e['values_hex'][i:i+16]for i in range(0,len(e['values_hex']),16)]
        chunks[chunks.index('0000000000000080')]='0000000000000000';e['values_hex']=''.join(chunks)
    tamper('negative-zero scratch sign lost','column-reader-after',flip_negative_zero,
           lambda e:'0000000000000080'in[e['values_hex'][i:i+16]for i in range(0,len(e['values_hex']),16)])
    tamper('unknown token catalog category','keyword-resolved',lambda e:e.update(category=2),lambda e:e['id']==305)
    for label,key,value in [('process identity','pid',-1),('capture completeness','messages_truncated',True),('probe source hash','source_sha256','0'*64)]:
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
    report={'status':'PASS','scope':__doc__.strip(),'evidence_id':evidence['evidence_id'],'pid':run['pid'],
        'result':result,'tampering_rejected':rejected,'tampering_not_applied':not_applied,
        'bindings':[binding(x)for x in [Path(__file__),args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze,PROBE,CONTRACT,ROOT/'analysis/keyword-catalog.json']],
        'verifier_dependencies':[binding(ROOT/'tools'/name)for name in ['capture_oracle.py','verify_boundary_sort_observations.py',
            'verify_material_object_observations.py','verify_material_reference_observations.py','verify_material_import_observations.py',
            'verify_mesh_array_observations.py','verify_mesh_refresh_observations.py','verify_geometry_segments_observations.py',
            'verify_step_distance_observations.py','verify_time_controls_observations.py']],
        'limitations':['No replacement process or comparator output is consulted. Selected bounded finite captures do not establish complete application compatibility.',
            'First matching failure of an uninitialized native header/row ID, allocation failure, overflow and arbitrary shared storage remain excluded.',
            'Other scientific keyword algorithms retain earlier milestone scope; captured descriptors and direct buffers are checked for owned shape and raw consistency.']}
    args.output.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(f"PASS {result['imports']} imports; {len(result['column_handlers'])} column/group handlers; {len(rejected)} tamper checks")


if __name__=='__main__':main()
