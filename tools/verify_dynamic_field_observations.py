"""Independent original-only audit of six dynamic mesh-field handlers.

Uses source-byte scanners and retained native contracts, never candidate output
or the dynamic-field comparator. Prior original-only validators are unchanged.
"""
import argparse
from collections import Counter
from copy import deepcopy
import json
from pathlib import Path
import struct

import verify_mesh_field_observations as fixed
import verify_material_column_observations as base
from verify_boundary_sort_observations import ROOT,binding,check,read
from capture_oracle import digest

PROBE=ROOT/'tools/observe_dynamic_field_import.js'
CONTRACT=ROOT/'analysis/dynamic-field-static-contract.json'
IDS={170:8,196:17,197:60,254:46,255:24,263:25}
raw,array,one,lex,line,scan=base.raw,base.array,base.one,base.lex,base.line,base.scan


def provenance(evidence,run,manifest,freeze,input_freeze,directory,manifest_path,freeze_path):
    base.PROBE=PROBE;base.CONTRACT=CONTRACT
    return base.provenance(evidence,run,manifest,freeze,input_freeze,directory,manifest_path,freeze_path)


def values(d):
    if d['values'] is None:
        check(d.get('length',0)==0 and d['values_hex']=='','null descriptor zero extent');return []
    return array(d)


def user_metadata(s):
    for kind,field in [('nodes','user_node_width_0x3398'),('elements','user_element_width_0x33a8')]:
        m=s['user_field_metadata'][kind]
        check(m['logical_count']==s[field] and m['allocation_observed'],'metadata logical and observed allocation')
        if m['pointer']=='0x0':check(m['capacity']==0 and m['names']==[],'null metadata capacity')
        else:
            check(type(m['capacity'])is int and 0<=m['capacity']<=8 and len(m['names'])==m['capacity'],'metadata physical capacity')
            check(len({r['pointer']for r in m['names']})==m['capacity'],'distinct live metadata records')
            for r in m['names']:
                payload=bytes.fromhex(r['bytes_hex']);check(len(payload)==64 and b'\0'in payload,'64 initialized metadata bytes')
                check(r['name_hex']==payload.split(b'\0')[0].hex() and int(r['pointer'],16)>0,'metadata raw/name projection')
    if s.get('linked_state')is not None:user_metadata(s['linked_state'])


def metadata_events(events):
    stack={};clear={};count=0
    for e in events:
        op=e['operation']
        if op=='dynamic-metadata-before':stack[e['serial']]=e
        elif op=='dynamic-metadata-after':
            b=stack.pop(e['serial']);check(all(b[k]==e[k]for k in('rva','kind','root','old_count','requested_count')),'metadata resize identity')
            old=b['metadata'];new=e['metadata'];n=e['requested_count'];check(0<=n<=8,'bounded metadata requested count')
            check(new['logical_count']==b['old_count'] and new['capacity']==n,'metadata resized before logical count store')
            check(new['names'][:min(n,len(old['names']))]==old['names'][:n],'metadata preserves retained record pointers and bytes')
            check(all(r['bytes_hex']=='00'*64 for r in new['names'][len(old['names']):]),'new metadata calloc64 zero records')
            count+=1
        elif op=='dynamic-metadata-clear-before':clear[e['serial']]=e
        elif op=='dynamic-metadata-clear-after':
            b=clear.pop(e['serial']);check(all(b[k]==e[k]for k in('rva','kind','root')),'metadata clear owner')
            check(e['metadata']['pointer']=='0x0' and e['metadata']['capacity']==0 and e['metadata']['names']==[],'metadata cleanup nulls physical allocation')
    check(not stack and not clear,'complete metadata lifetimes')
    return count


def header(e,data):
    content,end,eof,after=base.line_read(data,base.stream_position(e['before'],data))
    check(e['after']==after and len(e['lines'])==1,'415590 single line/cursor')
    check(e['lines'][0]=={'limit':1024,'eof':eof,'hex':None if eof else content.hex()},'415590 source line')
    result=[0,0.0,0];conversion=-1;scans=[]
    if not eof:
        normalized=content.replace(b',',b' ');pos=0;conversion=0
        for i,integer in enumerate([True,False,True]):
            value,c,pos=lex(normalized,pos,integer)
            if c!=1:
                if i==0:conversion=c
                break
            result[i]=value;conversion+=1
        scans=[{'format':'%i %Flf %i','input_hex':normalized.hex(),'conversions':conversion}]
    check(e['scans']==scans and e['conversions']==conversion and e['status']==int(conversion==-1),'415590 conversion/status')
    check(e['values']==result and e['output_hex']==[raw([v],i!=1)for i,v in enumerate(result)],'415590 zero/partial typed outputs')
    return result,int(conversion==-1),after


def physical(e):
    v=array(e);check(len(v)==e['physical_items']<=128,'dynamic physical initialized extent')
    n=e['count']*e['width_current'];valid=0<=n<=len(v)
    check(e['logical_items']==n and e['logical_extent_valid']==valid,'logical versus physical extent')
    check(e['logical_values_hex']==(raw(v[:n])if valid else None),'logical prefix raw view')
    check(e['logical_values']==(v[:n]if valid else None),'logical prefix numeric view')
    a=e['allocation']
    if e['extent_source']=='registered-descriptor-begin-end':
        check(a['storage_pointer']==e['buffer'] and a['length']==len(v) and a['values_hex']==e['values_hex'],'registered physical descriptor binding')
    else:
        check(e['extent_source']=='observed-calloc' and a['pointer']==e['buffer'] and not a['freed'] and a['bytes']==len(v)*8,'observed temporary physical bound')
    return v


def row_translate(e):
    q=deepcopy(e);q['operation']=q['operation'].replace('dynamic-','field-')
    q['integers']=False;q['element_count']=q['count'];q['width']=q['width_current']
    q['values']=q['logical_values'];q['values_hex']=q['logical_values_hex']
    return q


def row_reader(es,before,after,data):
    initial=physical(before);physical(after)
    check(before['buffer']==after['buffer'] and before['physical_items']==after['physical_items'],'rows retain physical allocation')
    converted=[]
    for e in es:
        if e['operation'].startswith('dynamic-rows-'):
            physical(e);converted.append(row_translate(e))
        elif e['operation']in('dynamic-scan','dynamic-line'):
            q=deepcopy(e);q['operation']=q['operation'].replace('dynamic-','field-');converted.append(q)
    prefix,status,cursor=fixed.detailed(converted,row_translate(before),row_translate(after),data,False)
    initial[:len(prefix)]=prefix
    check(after['values_hex']==raw(initial),'row writes preserve complete physical suffix')
    return initial,status,cursor


def mode_reader(es,before,after,data):
    initial=physical(before);physical(after);serial=before['serial'];mode=before['mode'];width=before['width_current']
    check(mode in(0,2),'selected dynamic header mode');lines=[e for e in es if e['operation']=='dynamic-line'and e['reader_serial']==serial]
    rows=[e for e in es if e['operation']=='dynamic-rows-before'and e['parent_serial']==serial]
    check(len(lines)==1,'one dynamic header line');content,eof,cursor=line(lines[0],data,before['stream'])
    status=1;scans=[];expected=initial[:]
    if not eof:
        kinds=[True,False]if mode==0 else[True,True]+[False]*10;fields=[];pos=0;conv=0
        for integer in kinds:
            value,c,pos=lex(content,pos,integer)
            if c!=1:
                conv=-1 if not fields and c==-1 else len(fields);break
            fields.append(value);conv=len(fields)
        fmt='%i %Flf'if mode==0 else'%i %i'+' %Flf'*10
        scans=[{'format':fmt,'input_hex':content.hex(),'conversions':conv}]
        if mode==2:width=fields[1]if len(fields)>1 else 0
        if conv>=1:
            defaults=fields[1:]if mode==0 else fields[2:];n=before['count']*width
            check(0<=n<=len(expected),'bounded mode2 actual physical extent')
            if defaults:
                expected[:n]=[defaults[i%width]if len(defaults)==width else defaults[0]for i in range(n)]
            check(len(rows)==1,'dynamic header calls row helper');b=rows[0]
            end=next(e for e in es if e['operation']=='dynamic-rows-after'and e['serial']==b['serial'])
            check(b['declared_rows']==fields[0]and b['count']==before['count']and b['width_current']==width
                  and b['kind']==('implicit'if mode==0 else'explicit')and b['explicit_width']==(0 if mode==0 else width)
                  and b['values_hex']==raw(expected)and b['stream']==cursor,'dynamic defaults/rootwidth before rows')
            inner=[e for e in es if b['sequence']<e['sequence']<end['sequence']]
            expected,status,cursor=row_reader(inner,b,end,data)
        else:check(not rows,'failed conversion skips rows after mode2 width store')
    else:check(not rows,'EOF skips mode2 width store and rows')
    check(after['width_current']==width and after['status']==status and after['stream']==cursor,'mode2 width/status/cursor')
    check(after['values_hex']==raw(expected)and after['scans']==scans,'mode header exact mutations/scanf')
    return expected,status,cursor


def getters(es,old,components):
    frames={};last={};count=0
    for e in es:
        if e['operation']=='dynamic-width-after':components=e['components']
        if e['operation']=='dynamic-getter-before':frames[e['serial']]=(e,components[:])
        elif e['operation']=='dynamic-getter-after':
            b,cs=frames.pop(e['serial']);component=e['component'];obj=next(o for o in old['objects']if o['address']==e['object'])
            check(all(b[k]==e[k]for k in('rva','component','object','option')),'getter identity')
            a=b['descriptor'];d=e['descriptor'];v=values(a);n=obj['node_allocation_count'if component<36 else'element_allocation_count']
            if n>0:
                w=cs[component];size=n*w;changed=size!=len(v);wanted=(v+[0.0]*max(0,size-len(v)))[:size]
                if component==46 and changed and w==9:
                    for row in range(n):
                        for col in(0,4,8):wanted[row*9+col]=1.0
                check(d['count']==n and d['width']==w and d['old_count']==a['count']and d['old_width']==a['width'],'getter shape/history')
                check(d['values_hex']==raw(wanted),'getter prefix preservation/growth/conditional diagonal')
                if not changed:check(d['storage_pointer']==a['storage_pointer'],'same size getter preserves pointer')
            else:check(d==a,'nonpositive storedcount getter is no-op')
            check(e['pointer']==d['bound_pointer']==d['storage_pointer'],'getter return bound actual pointer')
            check(all(a[k]==d[k]for k in('address','id','vtable_rva','slot_offset','multiplicity','storage_references')),'getter stable identity')
            last[component]=d;count+=1
        elif e['operation']=='dynamic-property-guard':check(e['value']==1,'property guard always true')
    check(not frames,'complete getter callbacks')
    return last,count


def stable_state(state):
    """Remove only observation-ledger labels changed by selective allocator hooks."""
    s=deepcopy(state)
    for o in s['objects']:
        for b in o['buffer_metadata'].values():
            b.pop('allocation_serial',None);b.pop('extent_source',None)
    if s['linked_state']is not None:s['linked_state']=stable_state(s['linked_state'])
    return s


def validate(evidence,inputs):
    previous=base.validate(evidence,inputs);events=[m['payload']for m in evidence['normalized_result']['messages'][1:]]
    metadata=metadata_events(events);groups=[];g=[];results=[];temporaries={}
    for e in events:
        if e['operation']=='dynamic-allocation':
            check(e['size']==8 and e['bytes']==e['count']*8 and 0<=e['bytes']<=1024,'bounded native calloc extent')
            if e['pointer']!='0x0':temporaries[e['pointer']]=e
        elif e['operation']=='dynamic-free':
            check(e['pointer']in temporaries and temporaries[e['pointer']]['serial']==e['allocation_serial'],'temporary lifetime/free identity')
            del temporaries[e['pointer']]
        if e.get('extent_source')=='observed-calloc':
            a=e['allocation'];check(a['pointer']in temporaries and all(temporaries[a['pointer']][k]==a[k]for k in('serial','bytes','pointer','count','size','object','return_rva')),'temporary read bound to live observed calloc')
        if 'state'in e and 'objects'in e['state']:user_metadata(e['state'])
        if e['operation']=='import-before':g=[]
        g.append(e)
        if e['operation']=='import-after':groups.append(g)
    for g in groups:
        start=None
        for i,e in enumerate(g):
            if e['operation']=='handler-before':start=i
            elif e['operation']=='handler-after':
                check(start is not None,'handler pair')
                if e['id']in IDS:results.append(handler(g[start:i+1],inputs[e['file']]))
                start=None
        fixed.finalization(g)
    for allocation in temporaries.values():
        # 4199a0 returns immediately on scalar EOF, bypassing free(local_28).
        # Record this native leak; the outer NDTMP temporary must still free.
        check(allocation['return_rva']==0x4199f0,'only source-backed EOF row-buffer leaks retained')
        begins=[e for e in events if e['operation']=='dynamic-rows-before'and e['file']==allocation['file']
                and e['kind']=='implicit'and e['sequence']<allocation['sequence']]
        check(begins,'leaked row buffer has enclosing reader');b=begins[-1]
        a=next(e for e in events if e['operation']=='dynamic-rows-after'and e['serial']==b['serial'])
        check(a['status']==1 and allocation['count']==b['width_current']and any(e['operation']=='dynamic-scan'
              and e['reader_serial']==b['serial']and e['conversions']==-1 for e in events),'leak tied to observed early scalar EOF')
    return {'imports':len(groups),'handlers':results,'metadata_resizes':metadata,'native_EOF_row_buffer_leaks':list(temporaries.values()),
            'coverage':{'id_dimensions':sorted(set((r['id'],r['dimension'])for r in results)),
                'failures':sum(r['status']!=0 for r in results),'getter_calls':sum(r['getters']for r in results)},
            'event_counts':dict(Counter(e['operation']for e in events))}


def handler(es,data):
    first,last=es[0],es[-1];id=first['id'];component=IDS[id];old=first['state'];new=last['state'];cs=old['components'][:]
    selected=one(es,'object-selected');selector=next(e for e in es if e['operation']=='stream-scan'and e['format']=='%i')
    base.shared.object_selector(selector,data);check(selected['found']and selected['requested']==selector['value'],'selected object binding')
    obj=next(o for o in old['objects']if o['id']==selected['requested']);target=next(o for o in new['objects']if o['id']==obj['id'])
    count=obj['nodes'if component<36 else'elements'];cursor=selector['after'];status=0
    if id==254:cs[46]=9 if old['dimension']==3 else 1
    if id==255:cs[24]=5
    latest,getter_count=getters(es,old,cs)
    expected=deepcopy(old)
    if id in(254,255):
        expected['components']=cs[:]
        for o in expected['objects']:o['components']=cs[:]
    expected_target=next(o for o in expected['objects']if o['id']==obj['id'])
    for c,d in latest.items():
        expected_target['descriptors'][next(i for i,x in enumerate(expected_target['descriptors'])if x['id']==c)]=deepcopy(d)
    headers=[e for e in es if e['operation']=='dynamic-header'];readers=[e for e in es if e['operation']=='dynamic-reader-before']
    direct=[e for e in es if e['operation']=='dynamic-rows-before'and e['parent_serial']is None]
    if id in(196,197)or(id==170 and old['dimension']==2):
        check(len(headers)==1,'one415590header');h=headers[0];check(h['before']==cursor,'header followsselector');fields,status,cursor=header(h,data)
        if status:check(not direct and not readers and getter_count==0,'header failure returns before mutations')
        else:
            declared,default,width=fields
            if id in(196,197):
                if id==197 and width==0:width=2
                b=one(es,'dynamic-width-before');a=one(es,'dynamic-width-after');key='user_node_width_0x3398'if id==196 else'user_element_width_0x33a8'
                check(b['requested']==a['requested']==width and b[key]==old[key]and a[key]==width,'setter count ordering')
                refreshes=[e for e in es if e['operation']=='material-refresh-before']
                check(len(refreshes)==int(width!=old[key]),'equal width setter no global refresh')
                if width!=old[key]:
                    refreshed=one(es,'material-refresh-after')['state'];expected=deepcopy(refreshed)
                    want_components=old['components'][:];want_components[component]=width
                    want_components[46]=1 if old['dimension']==2 else 0
                    # Refresh differs from constructor: root11c4/12b4/123c
                    # selected reset fields are zero and supply slots34/35/63.
                    for zero in (34,35,63):want_components[zero]=0
                    check(a['components']==want_components==refreshed['components'],'user setter source-derived layout regeneration')
                else:
                    check(a['components']==old['components']and a['user_field_metadata']==old['user_field_metadata'],'equal setter complete table/metadata no-op')
                expected_target=next(o for o in expected['objects']if o['id']==obj['id'])
                expected_target['descriptors'][next(i for i,x in enumerate(expected_target['descriptors'])if x['id']==component)]=deepcopy(latest[component])
                check(getter_count==count*width+1,'one getter per default scalar plus final getter')
                wanted=[default]*(count*width)
            else:wanted=[default]*(count*(2 if width==2 else 1))
            check(len(direct)==1,'one direct raw row call');b=direct[0];a=next(e for e in es if e['operation']=='dynamic-rows-after'and e['serial']==b['serial'])
            check(b['declared_rows']==declared and b['count']==count and b['stream']==cursor,'direct rows source binding')
            check(b['values_hex']==raw(wanted),'selected defaults before raw rows')
            inner=[e for e in es if b['sequence']<e['sequence']<a['sequence']];wanted,status,cursor=row_reader(inner,b,a,data)
            if id==170 and width==2:
                temperatures=wanted[1::2];difference=[wanted[j+1]-wanted[j]for j in range(0,len(wanted),2)]
            else:temperatures=wanted;difference=[0.0]*count
            d=next(d for d in target['descriptors']if d['id']==component);check(d['values_hex']==raw(temperatures),'direct final values even row failure')
            if id==170:check(next(d for d in target['descriptors']if d['id']==12)['values_hex']==raw(difference),'planar second-minus-first transform even failure')
            for selected,vs in [(component,temperatures)]+([(12,difference)]if id==170 else[]):
                d=next(d for d in expected_target['descriptors']if d['id']==selected)
                if vs:d['values']=vs;d['values_hex']=raw(vs)
    elif id==263 and old['dimension']!=3:
        check(not readers and not direct,'planar DRMESH no payload read')
    elif id==255 and latest[24]['storage_pointer']=='0x0':
        check(not readers,'null NWEAR no payload read')
    else:
        check(len(readers)==1,'one dynamic mode reader');b=readers[0];a=next(e for e in es if e['operation']=='dynamic-reader-after'and e['serial']==b['serial'])
        check(b['stream']==cursor and b['count']==count and b['component']==component,'dynamic reader header/slot binding')
        inner=[e for e in es if b['sequence']<e['sequence']<a['sequence']];wanted,status,cursor=mode_reader(inner,b,a,data)
        check(next(d for d in target['descriptors']if d['id']==component)['values_hex']==raw(wanted),'dynamic final physical array')
        d=next(d for d in expected_target['descriptors']if d['id']==component)
        if wanted:d['values']=wanted;d['values_hex']=raw(wanted)
        if id==254:
            check(new['components'][46]==a['width_current'],'root mode2 width mutation retained independently of descriptor')
            expected['components'][46]=a['width_current']
            for o in expected['objects']:o['components'][46]=a['width_current']
    check(last['status']==(2 if status else 0)and last['stream']==cursor,'dynamic parent status/cursor')
    check(stable_state(new)==stable_state(expected),'complete handler state, unrelated objects/data unchanged')
    return {'file':first['file'],'id':id,'dimension':old['dimension'],'status':last['status'],'getters':getter_count,'sequence':first['sequence']}


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for key,path in [('capture','analysis/dynamic-field-runtime-training.json'),('run-record','analysis/oracle/dynamic-field-training/run.json'),('manifest','analysis/dynamic-field-evaluation-cases.json'),('freeze','analysis/dynamic-field-candidate-freeze.json'),('input-freeze','analysis/dynamic-field-input-freeze.json'),('output','analysis/dynamic-field-observations.json')]:p.add_argument('--'+key,type=Path,default=ROOT/path)
    args=p.parse_args()
    for k in vars(args):setattr(args,k,getattr(args,k).resolve())
    evidence,run,manifest,freeze,input_freeze=map(read,[args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze])
    inputs=provenance(evidence,run,manifest,freeze,input_freeze,args.run_record.parent,args.manifest,args.freeze);result=validate(evidence,inputs)
    rejected=[];not_applied=[]
    def tamper(label,op,mutate,predicate=lambda e:True):
        damaged=deepcopy(evidence);es=[m['payload']for m in damaged['normalized_result']['messages']]
        target=next((e for e in es if e['operation']==op and predicate(e)),None)
        if target is None:not_applied.append({'test':label,'reason':'No qualifying native observation.'});return
        mutate(target)
        try:validate(damaged,inputs)
        except (ValueError,KeyError,IndexError,AssertionError,StopIteration,TypeError,struct.error):rejected.append(label)
        else:raise ValueError('tamper accepted '+label)
    tamper('event order','dynamic-header',lambda e:e.update(sequence=0))
    tamper('header partial output','dynamic-header',lambda e:e['values'].__setitem__(2,9))
    tamper('header conversion count','dynamic-header',lambda e:e.update(conversions=e['conversions']+1))
    tamper('header status','dynamic-header',lambda e:e.update(status=1-e['status']))
    tamper('header source','dynamic-header',lambda e:e['lines'][0].update(hex='300a'),lambda e:not e['lines'][0]['eof'])
    tamper('header cursor','dynamic-header',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('scalar value','dynamic-scan',lambda e:e.update(value=e['value']+1),lambda e:e['conversions']==1)
    tamper('failed scalar exposed','dynamic-scan',lambda e:e.update(value=0),lambda e:e['conversions']==-1)
    tamper('rows source cursor','dynamic-line',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('root width before mode2','dynamic-reader-before',lambda e:e.update(width_current=8),lambda e:e['mode']==2)
    tamper('root width after mode2','dynamic-reader-after',lambda e:e.update(width_current=e['width_current']+1),lambda e:e['mode']==2)
    tamper('physical extent','dynamic-reader-after',lambda e:e.update(physical_items=e['physical_items']+1))
    tamper('physical suffix','dynamic-reader-after',lambda e:e['values'].__setitem__(-1,999),lambda e:e['physical_items']>e['logical_items'])
    tamper('default row prefix','dynamic-rows-before',lambda e:e['values'].__setitem__(0,999),lambda e:bool(e['values']))
    tamper('row stride','dynamic-rows-before',lambda e:e.update(width_current=e['width_current']+1))
    tamper('partial rows','dynamic-rows-after',lambda e:e['values'].__setitem__(0,999),lambda e:e['status']!=0 and bool(e['values']))
    tamper('getter selector','dynamic-getter-after',lambda e:e.update(component=8))
    tamper('getter history','dynamic-getter-after',lambda e:e['descriptor'].update(old_width=999))
    tamper('getter returned pointer','dynamic-getter-after',lambda e:e.update(pointer='0x0'),lambda e:e['pointer']!='0x0')
    tamper('getter diagonal/rawdata','dynamic-getter-after',lambda e:e['descriptor'].update(values_hex='00'*e['descriptor']['length']*8),lambda e:e['component']==46 and any(values(e['descriptor'])))
    tamper('metadata resize capacity','dynamic-metadata-after',lambda e:e['metadata'].update(capacity=e['metadata']['capacity']+1))
    tamper('metadata retained pointer','dynamic-metadata-after',lambda e:e['metadata']['names'][0].update(pointer='0x1'),lambda e:e['old_count']>0 and bool(e['metadata']['names']))
    tamper('metadata new record zero','dynamic-metadata-after',lambda e:e['metadata']['names'][-1].update(bytes_hex='01'+'00'*63),lambda e:e['requested_count']>e['old_count'])
    tamper('metadata cleanup','dynamic-metadata-clear-after',lambda e:e['metadata'].update(pointer='0x1'))
    tamper('setter stored width','dynamic-width-after',lambda e:e.update(requested=e['requested']+1))
    tamper('property guard no-op','dynamic-property-guard',lambda e:e.update(value=0))
    tamper('temporary allocation extent','dynamic-allocation',lambda e:e.update(bytes=e['bytes']+8))
    tamper('temporary free identity','dynamic-free',lambda e:e.update(allocation_serial=999))
    tamper('NDTMP failure transform','handler-after',lambda e:next(d for d in e['state']['objects'][0]['descriptors']if d['id']==12).update(values_hex='00'*32),lambda e:e['id']==170 and e['status']==2 and e['state']['dimension']==2 and any(values(next(d for d in e['state']['objects'][0]['descriptors']if d['id']==12))))
    tamper('unrelated object data','handler-after',lambda e:e['state']['objects'][-1].update(material_group_0x3980=99),lambda e:e['id']in IDS and len(e['state']['objects'])>1)
    tamper('planar DRMESH status','handler-after',lambda e:e.update(status=2),lambda e:e['id']==263 and e['state']['dimension']==2)
    tamper('final mode normalization','finalize-after',lambda e:e['state']['finalization_controls'].update(mode_0x990=99))
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
        'limitations':['No candidate outputs or dynamic comparator imported; source-byte input scans and original passive captures only.',
            'Direct allocation ledger serial/extent-source labels are omitted only for whole-state equality because inherited selective getter hooks relabel existing buffers; all shape/raw/known-prefix checks retained.',
            'Negativewidth/OOM/unsafeovercapacity mode2 and sharedborrowedstorage excluded; metadata opaque64bytes in selected reset profile.']}
    args.output.write_text(json.dumps(report,indent=2)+'\n');print('PASS',len(result['handlers']),'dynamic handlers;',len(rejected),'corruption checks')


if __name__=='__main__':main()
