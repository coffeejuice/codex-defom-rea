"""Independent original-only audit of material shape properties and fields.

Reads fixture bytes, passive native observations and retained static contracts.
Never runs the reconstruction or imports its comparison program.
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

PROBE=ROOT/'tools/observe_material_shape_import.js'
CONTRACT=ROOT/'analysis/material-shape-static-contract.json'
FIELDS=('field_0x2150','field_0x2154','field_0x2158','field_0x215c')
DEFAULTS=(0,0,1,1)
SAVED=('saved_shape_width_0xadc','saved_texture_width_0xae0')
IDS=(127,128,210,211)
raw,array,one,lex=base.raw,base.array,base.one,base.lex


def array(e,integer=False):
    if e['values']is None:
        check(e.get('length')==0 and e.get('storage_pointer')=='0x0' and e['values_hex']=='','null descriptor has zero extent')
        return []
    return base.array(e,integer)


def provenance(evidence,run,manifest,freeze,input_freeze,directory,manifest_path,freeze_path):
    base.PROBE=PROBE;base.CONTRACT=CONTRACT
    check({k:manifest['probe'][k]for k in ('path','sha256')}==binding(PROBE),'manifest probe identity')
    projected=deepcopy(manifest);projected['probe'].pop('evidence_id',None)
    base.shared.PROBE=PROBE;base.shared.CONTRACT=CONTRACT
    inputs=base.shared.provenance(evidence,run,projected,{**freeze,'inputs':input_freeze['inputs']},directory)
    base.verify_input_freeze(input_freeze,projected,manifest_path,freeze_path)
    return inputs


def texture(r):
    mode=r[FIELDS[1]]
    if mode not in (1,2,3) or r[FIELDS[2]]!=1 or r[FIELDS[3]]not in (1,2,3,4):return 0
    return ((7,50,388,3080)if mode==3 else(10,76,600,4784))[r[FIELDS[3]]-1]


def widths(records):
    a=sum({1:1,2:4,3:3,4:2,5:4,6:1}.get(r[FIELDS[0]],0)for r in records)
    b=0
    for r in records:
        if r[refs.MODE]==0:b+=texture(r)
        else:
            for identifier in r['reference_ids']:
                target=refs.find(records,identifier)
                if target is not None:b+=texture(target)
    return a,b


def shape_header(es,data,cursor):
    content,_,eof,after=base.line_read(data,base.stream_position(cursor,data))
    e=one(es,'material-shape-reader-after')
    check(e['lines']==[{'limit':1024,'eof':eof,'hex':None if eof else content.hex()}],'shape header exact fgets source')
    fields=[0,0,0,0.0];conversions=-1;scans=[x for x in es if x['operation']=='material-shape-header-scan']
    if not eof:
        conversions=0;pos=0
        for i,integer in enumerate((True,True,True,False)):
            value,c,pos=lex(content,pos,integer)
            if c!=1:
                if not i:conversions=c
                break
            fields[i]=value;conversions+=1
        s=one(es,'material-shape-header-scan')
        check(s['format']=='%i %i %i %Flf' and s['input_hex']==content.hex() and s['conversions']==conversions,'unmodified mixed header scan')
        check(s['values']==fields and s['output_hex']==[raw([v],i<3)for i,v in enumerate(fields)],'zeroed partial mixed header raw outputs')
    else:check(not scans,'EOF has no sscanf')
    return fields,conversions,after


def allocated(before,after,count,width):
    for k in ('address','id','vtable_rva','slot_offset','storage_references'):
        check(after[k]==before[k],'selected descriptor stable identity '+k)
    check((after['old_count'],after['old_width'],after['old_multiplicity'])==(before['count'],before['width'],before['multiplicity']),'allocation records previous shape')
    check((after['count'],after['width'],after['multiplicity'])==(count,width,1),'allocation uses stored count and root width')
    wanted=array(before)[:count*width]+[0.0]*max(0,count*width-before['length'])
    check(after['length']==len(wanted) and after['values_hex']==raw(wanted),'selected resize prefix and zero growth')
    array(after)
    check(after['bound_pointer']==after['storage_pointer'],'descriptor external slot bound')
    if before['length']==after['length']:check(before['storage_pointer']==after['storage_pointer'],'equal size retains allocation')


def consumer(es,data):
    first,last=es[0],es[-1];old,new=first['state'],last['state'];identifier=first['id'];component=54 if identifier==210 else 55
    selected=one(es,'object-selected');scan=next(e for e in es if e['operation']=='stream-scan'and e['format']=='%i')
    base.shared.object_selector(scan,data);check(selected['found'] and selected['requested']==scan['value'],'shape shared object selector')
    obj=next(o for o in old['objects']if o['id']==scan['value']);target=next(o for o in new['objects']if o['address']==obj['address'])
    b,a=one(es,'material-shape-reader-before'),one(es,'material-shape-reader-after')
    check(b['id']==a['id']==identifier and b['serial']==a['serial'] and b['component']==a['component']==component and b['object']==a['object']==obj['address'],'shape reader owner/ABI')
    check(b['count']==a['count']==obj['elements'] and b['stream']==scan['after'] and b['descriptor']==base.descriptor(obj,component),'shape reader exact source descriptor/count')
    check(b['root_width']==old['components'][component] and all(b[k]==obj[k]for k in SAVED),'initial independent widths')
    check(a['completion_rva']==(0x39354f if identifier==210 else 0x3935a0),'consumer native continuation')
    fields,conversions,cursor=shape_header(es,data,b['stream']);declared,selector,width,default=fields
    expected=deepcopy(old);out=next(o for o in expected['objects']if o['address']==obj['address']);status=1
    storage=[e for e in es if e['operation'].startswith('material-shape-storage-')]
    rowevents=[e for e in es if e['operation'].startswith('material-shape-rows-')]
    if width>0:
        expected['components'][component]=width
        for owned in expected['objects']:owned['components'][component]=width
        if selector==0:
            check([(e['operation'],e['kind'])for e in storage]==[(op,k)for k in('resize','getter')for op in('material-shape-storage-before','material-shape-storage-after')],'setter then getter order')
            d=b['descriptor'];saved={k:obj[k]for k in SAVED}
            for pre,post in zip(storage[::2],storage[1::2]):
                check(pre['serial']==post['serial'] and pre['object']==post['object']==obj['address'] and pre['component']==post['component']==component,'storage pair identity')
                check(pre['descriptor']==d and all(pre[k]==saved[k]for k in SAVED),'storage receives previous descriptor and metadata')
                check(pre['requested_width']==(width if pre['kind']=='resize'else None),'setter requested width/getter ABI')
                if obj['element_allocation_count']>0:
                    allocated(d,post['descriptor'],obj['element_allocation_count'],width)
                    if pre['kind']=='resize':saved[SAVED[component-54]]=width
                else:check(post['descriptor']==d,'nonpositive stored allocation count getter/setter no-op')
                check(all(post[k]==saved[k]for k in SAVED),'saved width conditional setter only')
                if pre['kind']=='getter':check(post['pointer']==post['descriptor']['storage_pointer'],'getter returned bound data')
                d=post['descriptor']
            rb,ra=one(es,'material-shape-rows-before'),one(es,'material-shape-rows-after')
            check(rb['parent_serial']==ra['parent_serial']==b['serial'] and rb['serial']==ra['serial'],'row helper parent binding')
            check(rb['width']==ra['width']==rb['explicit_width']==ra['explicit_width']==width and rb['declared_rows']==ra['declared_rows']==declared,'row helper exact widths/count')
            check(rb['stream']==cursor and ra['completion_rva']==0x41889c,'row entry cursor and native continuation')
            expected_values=array(d)
            if conversions>3:expected_values[:obj['elements']*width]=[default]*(obj['elements']*width)
            check(rb['descriptor']['values_hex']==raw(expected_values),'header default exact prefix with raw zero sign')
            check(custom.without(rb['descriptor'],'values','values_hex')==custom.without(d,'values','values_hex'),'default fill preserves descriptor metadata')
            def projection(e):
                return {'serial':e['serial'],'parent_serial':e['parent_serial'],'kind':'explicit','integers':False,'declared_rows':declared,'element_count':obj['elements'],'width':width,'explicit_width':width,'stream':e['stream'],'values':array(e['descriptor']),'values_hex':e['descriptor']['values_hex'],**({'status':e['status']}if 'status'in e else{})}
            inner=[]
            for e in es:
                if e['operation']=='material-shape-scan':
                    x=deepcopy(e);x['operation']='field-scan';inner.append(x)
            _,status,cursor=fixed.detailed(inner,projection(rb),projection(ra),data,False)
            check(custom.without(ra['descriptor'],'values','values_hex')==custom.without(rb['descriptor'],'values','values_hex'),'sparse row writes preserve descriptor metadata')
            out['descriptors'][next(i for i,d0 in enumerate(out['descriptors'])if d0['id']==component)]=deepcopy(ra['descriptor'])
            out.update(saved)
        else:check(not storage and not rowevents,'unsupported selector returns after root width only')
    else:check(not storage and not rowevents,'invalid header returns before allocation/root mutation')
    check(a['descriptor']==base.descriptor(out,component) and a['root_width']==expected['components'][component] and all(a[k]==out[k]for k in SAVED),'reader final exact descriptor/root/saved widths')
    check(a['status']==status and a['stream']==cursor and last['status']==(2 if status else 0) and last['stream']==cursor,'reader and parent failure status/cursor')
    check(new==expected,'complete selected consumer mutation boundary '+first['file']+' '+str([k for k in new if new[k]!=expected[k]]))
    return {'id':identifier,'dimension':old['dimension'],'status':last['status'],'width':width,'selector':selector,'declared_rows':declared,'sequence':first['sequence']}


def producer(es,data):
    first,last=es[0],es[-1];id=first['id'];old,new=first['state'],last['state']
    sel=one(es,'material-selector');selected=refs.verify_selector({**sel,'id':92},data)
    check(sel['before']==first['stream'] and selected is not None,'producer exact signed selector')
    h=one(es,'material-shape-producer-header');check(h['id']==id and h['before']==sel['after'],'producer header follows selector')
    values,status,cursor=custom.line_helper(h,data,[True]*(1 if id==127 else 3),'%i'if id==127 else'%i %i %i')
    prior=refs.find(old['material_list']['records'],selected);created=prior is None
    appends=[e for e in es if e['operation']=='material-append-after'];refreshes=[e for e in es if e['operation']=='material-refresh-after']
    check(len(appends)==len(refreshes)==int(created),'creation-only property refresh')
    source=refreshes[0]['state']if created else old
    if created:
        check(appends[0]['sequence']<refreshes[0]['sequence']<h['sequence'],'append and refresh precede property read')
        born=appends[0]['state']['records'][-1];check(tuple(born[k]for k in FIELDS)==DEFAULTS,'new material shape defaults')
    prior=refs.find(source['material_list']['records'],selected);check(prior is not None and h['before_material']==prior,'producer exact prior record')
    expected_record=deepcopy(prior);keys=FIELDS[:1]if id==127 else FIELDS[1:]
    for k,v in zip(keys,values):expected_record[k]=v
    check(h['after_material']==expected_record and h['material']==prior['address'],'producer partial direct destination mutation')
    actual=refs.find(new['material_list']['records'],selected);check(actual==expected_record,'handler retains property mutation')
    expected=deepcopy(source);expected['material_list']=deepcopy(new['material_list'])
    check(new==expected,'property has no post-write layout/object mutation')
    wanted=deepcopy(source['material_list']['records']);wanted[next(i for i,r in enumerate(wanted)if r['id']==selected)]=expected_record
    check(wanted==new['material_list']['records'],'only selected material property changes')
    check(last['status']==(2 if status else 0) and last['stream']==cursor,'producer status and cursor')
    return {'id':id,'dimension':old['dimension'],'status':last['status'],'selected_id':selected,'created':created,'sequence':first['sequence']}


def validate(evidence,inputs):
    cap=evidence['normalized_result'];messages=cap['messages'];check(len(cap['scripts'])==1,'one passive script')
    check(all(m['type']=='send'and m['script_id']==cap['scripts'][0]['script_id']for m in messages),'all messages from original passive script')
    check(messages[0]['payload']['operation']=='hooks-installed'and messages[0]['payload']['pid']==cap['pid'],'original hook process')
    events=[m['payload']for m in messages[1:]];check([e['sequence']for e in events]==list(range(1,len(events)+1)),'complete event order')
    check(not any(e['operation']=='probe-error'for e in events),'no invalid probe observations')
    rows=descriptor_map(read(ROOT/'analysis/mesh-array-contract.json'));groups=[];g=None;live={};pending=None;queries=[]
    for e in events:
        op=e['operation']
        if op=='material-reference-allocate':
            check(e['call_return_rva']==0x3883ee and e['width']==4 and e['values']==[0]*e['capacity']and e['pointer']not in live,'reference allocation provenance')
            live[e['pointer']]={k:e[k]for k in('capacity','material','serial')}
        for r in refs.records_in(e):
            check(all(type(r[k])is int and -2**31<=r[k]<2**31 for k in FIELDS),'signed material property domain')
            projected=deepcopy(r);projected.update(dict(zip(FIELDS,DEFAULTS)));refs.record_shape(projected,live)
        for s in refs.lists_in(e):refs.list_shape(s)
        if op=='material-reference-free-before':check(e['pointer']in live and pending is None,'live reference free');pending=e
        elif op=='material-reference-free-after':
            check(pending is not None and e['pointer']==pending['pointer'],'paired reference free');del live[e['pointer']];pending=None
        if 'state'in e and 'objects'in e['state']:
            for o in e['state']['objects']:
                base.shared.object_shape(o,rows);check(all(type(o[k])is int and o[k]>=0 for k in SAVED),'saved widths signed nonnegative selected domain')
        if op=='material-shape-query-before':queries.append(e)
        elif op=='material-shape-query-after':
            check(queries,'query entry exists');b=queries.pop();n=widths(b['materials']['records'])[b['feature']-54]
            check(e['root']==b['root'] and e['feature']==b['feature'] and e['output_pointer']==b['output_pointer'],'query exact identity')
            check(e['enabled']==(n>0) and e['output']==(n if n>0 and e['output_pointer']!='0x0'else None),'query sum/boolean/conditional output')
        if op=='import-before':check(g is None,'non-nested imports');g=[]
        check(g is not None,'event inside import');g.append(e)
        if op=='import-after':groups.append(g);g=None
    check(g is None and not queries and pending is None,'all captured lifecycle calls completed')
    check([g[0]['file']for g in groups]==list(inputs),'complete ordered fixture imports')
    catalog={r['name']:(r['id'],r['category'])for r in read(ROOT/'analysis/keyword-catalog.json')['rows']}
    results=[];previous=None;refresh_count=0
    for g in groups:
        data=inputs[g[0]['file']];start=None;refresh_start=None;refstart=None;last_state=None;token=None;statuses=[]
        if previous is not None:check(g[0]['state']==previous,'complete between-import continuity')
        previous=g[-1]['state']
        for i,e in enumerate(g):
            op=e['operation']
            if op=='stream-scan'and e['format']=='%39s':refs.verify_token(e,data);token=e
            elif op=='keyword-resolved':check(token is not None and token['value']==e['token']and(e['id'],e['category'])==catalog.get(e['token'],(305,0)),'source-bound keyword resolution')
            if op=='material-refresh-before':refresh_start=i
            elif op=='material-refresh-after':
                check(refresh_start is not None,'refresh pairing');frame=g[refresh_start:i+1];refresh_start=None
                check(tuple(e['state']['components'][54:56])==widths(e['state']['material_list']['records']),'refreshed widths derive current material properties')
                base.refresh(frame);refresh_count+=1
                for a,b in zip(frame[0]['state']['objects'],e['state']['objects']):check(all(a[k]==b[k]for k in SAVED),'global refresh preserves independent saved widths')
            if op=='handler-before':
                check(start is None,'handler entry pairing');start=i
                if last_state is not None:check(e['state']==last_state,'within-import full state continuity')
            elif op=='handler-after':
                check(start is not None,'handler return pairing');frame=g[start:i+1];start=None;statuses.append(e['status'])
                if e['id']in(127,128):results.append(producer(frame,data))
                elif e['id']in(210,211):results.append(consumer(frame,data))
                elif e['category']==4:refs.handler(frame,data)
                else:
                    check(frame[0]['state']['material_list']['records']==e['state']['material_list']['records'],'other handlers preserve material properties')
                    oldobjects={o['address']:o for o in frame[0]['state']['objects']}
                    for o in e['state']['objects']:
                        check(all(o[k]==oldobjects[o['address']][k]for k in SAVED)if o['address']in oldobjects else all(o[k]==0 for k in SAVED),'object creation zeroes and other operations retain saved widths')
                last_state=e['state']
            if op=='material-reference-finalize-before':refstart=i
            elif op=='material-reference-finalize-after':check(refstart is not None,'reference finalizer pairing');refs.finalize(g[refstart:i+1]);refstart=None
        check(start is None and refresh_start is None and refstart is None,'closed handler and refresh intervals')
        before,after=one(g,'finalize-before'),one(g,'finalize-after')
        if last_state is not None:check(before['state']==last_state,'finalizer receives last handler full state')
        expected=deepcopy(before['state']);expected['material_list']=deepcopy(after['state']['material_list'])
        if expected['finalization_controls']['control_0x93c']==0:
            mode=int(expected['finalization_controls']['step_increment_0x930']<=0);expected['finalization_controls']['mode_0x990']=expected['time_table']['mode_0x58']=mode
        check(all(o['flag_0x3990']==0 for o in expected['objects']),'selected no geometry finalization flag')
        check(expected==after['state'] and after['state']==g[-1]['state'],'bounded full finalizer state, without eager shape refresh')
        check(g[-1]['status']==next((s for s in reversed(statuses)if s),0),'retained import failure status')
    return {'imports':len(groups),'handlers':results,'refreshes':refresh_count,'coverage':{'id_dimensions':sorted(set((r['id'],r['dimension'])for r in results)),'failures':sum(r['status']!=0 for r in results),'max_query_width':max([widths(e['materials']['records'])[e['feature']-54]for e in events if e['operation']=='material-shape-query-before']or[0]),'reference_allocations_still_live_at_capture_end':len(live)},'event_counts':dict(Counter(e['operation']for e in events))}


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for k,v in [('capture','analysis/material-shape-runtime-training.json'),('run-record','analysis/oracle/material-shape-training/run.json'),('manifest','analysis/material-shape-evaluation-cases.json'),('freeze','analysis/material-shape-candidate-freeze.json'),('input-freeze','analysis/material-shape-input-freeze.json'),('output','analysis/material-shape-observations.json')]:p.add_argument('--'+k,type=Path,default=ROOT/v)
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
    tamper('event order','material-shape-reader-before',lambda e:e.update(sequence=0))
    tamper('producer partial raw output','material-shape-producer-header',lambda e:e['output_hex'].__setitem__(0,'ffffff7f'))
    tamper('producer status','material-shape-producer-header',lambda e:e.update(status=1-e['status']))
    tamper('producer source bytes','material-shape-producer-header',lambda e:e['lines'][0].update(hex='390a'),lambda e:not e['lines'][0]['eof'])
    tamper('query duplicate contribution','material-shape-query-after',lambda e:e.update(output=e['output']+1),lambda e:e['output']is not None)
    tamper('false query result','material-shape-query-after',lambda e:e.update(enabled=True),lambda e:not e['enabled'])
    tamper('mixed header count','material-shape-header-scan',lambda e:e.update(conversions=e['conversions']+1))
    tamper('mixed header comma bytes','material-shape-header-scan',lambda e:e.update(input_hex='302c302c320a'))
    tamper('mixed header raw default','material-shape-header-scan',lambda e:e['output_hex'].__setitem__(3,'000000000000f03f'))
    tamper('root width before failure','material-shape-reader-after',lambda e:e.update(root_width=e['root_width']+1),lambda e:e['status']!=0)
    tamper('native continuation','material-shape-reader-after',lambda e:e.update(completion_rva=0))
    tamper('saved width setter','material-shape-storage-after',lambda e:e.update(saved_shape_width_0xadc=999),lambda e:e['kind']=='resize')
    tamper('descriptor history','material-shape-storage-after',lambda e:e['descriptor'].update(old_width=999),lambda e:e['kind']=='getter')
    tamper('getter pointer','material-shape-storage-after',lambda e:e.update(pointer='0x1'),lambda e:e['kind']=='getter')
    tamper('default mutation','material-shape-rows-before',lambda e:e['descriptor']['values'].__setitem__(0,999),lambda e:bool(e['descriptor']['values']))
    tamper('row width/stride','material-shape-rows-before',lambda e:e.update(width=e['width']+1))
    tamper('row cursor','material-shape-rows-after',lambda e:e['stream'].update(cursor=e['stream']['cursor']+1))
    tamper('row failure partial commit','material-shape-rows-after',lambda e:e['descriptor']['values'].__setitem__(0,999),lambda e:e['status']!=0 and bool(e['descriptor']['values']))
    tamper('scalar source value','material-shape-scan',lambda e:e.update(value=e['value']+1),lambda e:e['conversions']==1)
    tamper('failed scalar exposed','material-shape-scan',lambda e:e.update(value=0),lambda e:e['conversions']==-1)
    tamper('negative zero raw sign','material-shape-rows-before',lambda e:e['descriptor'].update(values_hex=e['descriptor']['values_hex'].replace('0000000000000080','0000000000000000',1)),lambda e:'0000000000000080'in e['descriptor']['values_hex'])
    tamper('unrelated object field','handler-after',lambda e:e['state']['objects'][0].update(material_group_0x3980=999),lambda e:e['id']in IDS and bool(e['state']['objects']))
    tamper('finalization normalization','finalize-after',lambda e:e['state']['finalization_controls'].update(mode_0x990=999))
    for label,key,value in [('process identity','pid',-1),('capture truncation','messages_truncated',True),('probe bytes','source_sha256','0'*64)]:
        d=deepcopy(evidence);d['normalized_result'][key]=value
        try:provenance(d,run,manifest,freeze,input_freeze,args.run_record.parent,args.manifest,args.freeze)
        except ValueError:rejected.append(label)
        else:raise ValueError('provenance tamper accepted '+label)
    report={'status':'PASS','result':result,'bindings':[binding(x)for x in(Path(__file__),args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze,PROBE,CONTRACT)],'tampering_rejected':rejected,'tampering_not_applied':not_applied,'dependencies':[binding(ROOT/'tools'/n)for n in('verify_material_column_observations.py','verify_material_reference_observations.py','verify_material_object_observations.py','verify_mesh_field_observations.py','verify_custom_field_observations.py','verify_boundary_sort_observations.py','verify_mesh_array_observations.py','verify_material_import_observations.py','verify_step_distance_observations.py','verify_time_controls_observations.py','capture_oracle.py')],'limitations':['Original-only finite bounded fixture domain; no reconstruction outputs consulted.','No OOM, native signed overflow, uninitialized first row-ID matching failure or unsafe header/allocation mismatch is modeled.','Native row scratch lifetime on EOF is not established by this payload audit.']}
    args.output.write_text(json.dumps(report,indent=2)+'\n');print('PASS',len(result['handlers']),'shape handlers;',len(rejected),'applied corruption checks')


if __name__=='__main__':main()
