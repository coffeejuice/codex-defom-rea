"""Original-only fixed mesh-field audit: source bytes, native mutations and provenance.

Never imports the mesh-field comparator or executes the C++ replacement.
"""
import argparse
from collections import Counter
from copy import deepcopy
import json
from pathlib import Path
import struct

import verify_material_column_observations as base
from verify_boundary_sort_observations import ROOT, binding, check, read
from verify_mesh_array_observations import descriptor_map
from capture_oracle import digest

PROBE=ROOT/'tools/observe_mesh_field_import.js'
CONTRACT=ROOT/'analysis/mesh-field-static-contract.json'
FIELDS={c['id']:c for c in read(CONTRACT)['callsite_verification']['cases']}
raw,array,one,scan,line,lex=base.raw,base.array,base.one,base.scan,base.line,base.lex


def provenance(evidence,run,manifest,freeze,input_freeze,directory,manifest_path,freeze_path):
    base.PROBE=PROBE;base.CONTRACT=CONTRACT
    return base.provenance(evidence,run,manifest,freeze,input_freeze,directory,manifest_path,freeze_path)


def detailed(es,before,after,data,integer):
    serial=before['serial'];kind=before['kind'];width=before['width'];count=before['element_count']
    declared=before['declared_rows'];values=array(before,integer)[:]
    check(len(values)==count*width,'row-major actual destination extent')
    for key in ('serial','parent_serial','kind','integers','declared_rows','element_count','width','explicit_width'):
        check(before[key]==after[key],'row helper argument identity '+key)
    if kind=='explicit' and before['explicit_width']==0:
        child=one(es,'field-rows-before');end=one(es,'field-rows-after')
        check(child['kind']=='implicit' and child['parent_serial']==serial and child['stream']==before['stream']
              and child['width']==width and child['element_count']==count and child['declared_rows']==declared
              and child['values_hex']==before['values_hex'],'zero explicit width delegates same destination')
        inner=[e for e in es if child['sequence']<e['sequence']<end['sequence']]
        result,status,cursor=detailed(inner,child,end,data,False)
    else:
        row_width=width if integer or kind=='implicit' else before['explicit_width']
        scans=[e for e in es if e['operation']=='field-scan' and e['reader_serial']==serial]
        lines=[e for e in es if e['operation']=='field-line' and e['reader_serial']==serial]
        row=[0 if integer else 0.0]*row_width;row_id=None;cursor=before['stream'];index=0;status=0
        for _ in range(max(0,declared)):
            check(index<len(scans),'row ID scan present');e=scans[index];index+=1
            value,c,cursor=scan(e,data,cursor,True)
            if c==-1:status=1;break
            if c==1:row_id=value
            check(row_id is not None,'first unmatched row ID excluded')
            for column in range(row_width):
                check(index<len(scans),'row value scan present');e=scans[index];index+=1
                value,c,cursor=scan(e,data,cursor,integer)
                if c==-1:status=1;break
                if c==1:row[column]=value
            if status:break
            if 1<=row_id<=count:
                for column in range(min(width,row_width)):values[(row_id-1)*width+column]=row[column]
        check(index==len(scans),'no surplus row scalar events')
        discard=(integer or kind=='implicit') and declared>0 and not status
        check(len(lines)==int(discard),'exact trailing fgets policy')
        if discard:
            _,eof,cursor=line(lines[0],data,cursor);status=int(eof)
        result=values
    check(after['status']==status and after['stream']==cursor,'row helper status/cursor')
    check(array(after,integer)==result and after['values_hex']==raw(result,integer),'row partial writes and preserved suffix')
    return result,status,cursor


def header(content,integer,mode):
    kinds=[True,True] if integer else [True]+([True,False] if mode==1 else [False]*(10 if mode==6 else 1))
    fields=[];pos=0;conversions=0
    for kind in kinds:
        value,c,pos=lex(content,pos,kind)
        if c!=1:
            conversions=-1 if c==-1 and not fields else len(fields);break
        fields.append(value);conversions=len(fields)
    fmt='%i %i' if integer else '%i %i %Flf' if mode==1 else '%i'+' %Flf'*(10 if mode==6 else 1)
    return fields,conversions,fmt


def reader(es,before,after,data):
    integer=before['integers'];width=before['width'];count=before['element_count'];mode=before['mode'];serial=before['serial']
    for key in ('serial','id','keyword','object','slot','component','domain','integers','mode','element_count','width'):
        check(before[key]==after[key],'field reader argument identity '+key)
    values=array(before,integer)[:];check(len(values)==count*width,'field destination prefix extent')
    lines=[e for e in es if e['operation']=='field-line' and e['reader_serial']==serial]
    rows=[e for e in es if e['operation']=='field-rows-before' and e['parent_serial']==serial]
    status=1;scans=[];cursor=before['stream']
    if mode==6 and width>10:
        check(not lines and not rows,'mode6 overwidth early rejection')
    else:
        check(len(lines)==1,'one field header fgets')
        content,eof,cursor=line(lines[0],data,cursor)
        if not eof:
            fields,conversions,fmt=header(content,integer,mode)
            scans=[{'format':fmt,'input_hex':content.hex(),'conversions':conversions}]
            if conversions>=1:
                defaults=fields[1:] if integer or mode!=1 else fields[2:]
                if defaults:
                    values=[defaults[i%width] if len(defaults)==width else defaults[0] for i in range(count*width)]
                check(len(rows)==1,'valid field header calls row helper');b=rows[0]
                end=next(e for e in es if e['operation']=='field-rows-after' and e['serial']==b['serial'])
                explicit=fields[1] if not integer and mode==1 and len(fields)>1 else 0
                expected_kind='integerrows' if integer else 'implicit' if mode==0 else 'explicit'
                check(b['kind']==expected_kind and b['declared_rows']==fields[0] and b['element_count']==count
                      and b['width']==width and b['explicit_width']==explicit and b['stream']==cursor
                      and b['values_hex']==raw(values,integer),'exact header default, arguments and row-helper dispatch')
                inner=[e for e in es if b['sequence']<e['sequence']<end['sequence']]
                values,status,cursor=detailed(inner,b,end,data,integer)
            else:
                check(not rows,'failed header skips rows')
                check(not integer or conversions==-1,'undefined integer header matching failure excluded')
        else:check(not rows,'EOF header skips rows')
    check(after['scans']==scans,'exact header sscanf input/format/conversion count')
    observed=[{k:e[k]for k in ('limit','eof','hex')}for e in es if e['operation']=='field-line']
    check(after['lines']==observed,'complete nested fgets inventory')
    check(after['status']==status and after['stream']==cursor,'field return status/cursor')
    check(array(after,integer)==values and after['values_hex']==raw(values,integer),'in-place header and partial row writes')
    return values,status,cursor


def object_at(state,address):return next(o for o in state['objects']if o['address']==address)


def handler(es,data):
    first,last=es[0],es[-1];definition=FIELDS[first['id']];integer=definition['mode'] is None
    before=one(es,'field-reader-before');after=one(es,'field-reader-after')
    selected=one(es,'object-selected');selector=next(e for e in es if e['operation']=='stream-scan' and e['format']=='%i')
    base.shared.object_selector(selector,data)
    check(selected['found'] and selected['requested']==selector['value'],'native object selector successful')
    obj=object_at(first['state'],before['object']);component=definition['component'];width=first['state']['components'][component]
    domain='nodes' if definition['count_field']=='0x880' else 'elements';count=obj[domain]
    check(obj['id']==selected['requested'] and before['id']==first['id'] and before['keyword']==definition['keyword']
          and before['slot']==int(definition['slot'],16) and before['component']==component
          and before['domain']==domain and before['integers']==integer and before['mode']==definition['mode']
          and before['width']==width and before['element_count']==count,'native field ABI and root/header bindings')
    check(before['stream']==selector['after'],'field line follows same-line object selector')
    if integer:
        storage=obj['buffer_metadata']['direct'+str(component)]
        check(storage['known_prefix']>=count*width and storage['allocated_items']>=count*width,'known direct prefix covers native header')
        check(before['values_hex']==storage['known_hex'][:count*width*8],'reader receives actual direct destination')
    else:
        storage=next(d for d in obj['descriptors']if d['id']==component)
        check(storage['width']==width and storage['length']>=count*width,'descriptor actual extent and width')
        check(before['values_hex']==storage['values_hex'][:count*width*16],'reader receives actual descriptor prefix')
    inner=[e for e in es if before['sequence']<e['sequence']<after['sequence']]
    values,status,cursor=reader(inner,before,after,data)
    converted=0
    post=[e for e in es if e['operation']=='field-postprocess']
    if first['id']==162:
        check(len(post)==1,'BCCDEF postprocess observed');event=post[0]
        check(event['reader_serial']==before['serial'] and event['object']==obj['address']
              and event['dimension']==first['state']['dimension'] and event['before']['values_hex']==raw(values,True),
              'BCCDEF begins from reader return even failure')
        if first['state']['dimension']==3:
            check(width==3,'selected spatial BCCDEF triple extent')
            for row in range(count):
                j=3*row
                if values[j]<0 and values[j]==values[j+1]==values[j+2]:
                    values[j]=((values[j]-200+2**31)%2**32)-2**31
                    values[j+2]=0;values[j+1]=0;converted+=1
        check(array(event['before'],True)==array(after,True),'BCCDEF pre-view raw consistency')
        check(array(event['after'],True)==values and event['after']['values_hex']==raw(values,True),'BCCDEF signed32 wrap and spatial-only conversion')
    else:check(not post,'no unrelated field postprocess')
    expected=deepcopy(first['state']);target=object_at(expected,obj['address'])
    if integer:
        key=str(component);b=target['buffer_metadata']['direct'+key]
        b['values'][:len(values)]=values;b['known_hex']=raw(b['values'][:b['known_prefix']],True)
        target['direct'][key][:len(values)]=values
    else:
        d=next(d for d in target['descriptors']if d['id']==component)
        if values:
            all_values=array(d);all_values[:len(values)]=values;d['values']=all_values;d['values_hex']=raw(all_values)
    check(last['state']==expected,'field mutates only selected array, all root/object metadata retained')
    check(last['status']==(0 if status==0 else 2) and last['stream']==cursor,'field parent status mapping and cursor')
    return {'id':first['id'],'keyword':definition['keyword'],'dimension':first['state']['dimension'],'width':width,
            'count':count,'status':last['status'],'bccdef_converted_rows':converted,'sequence':first['sequence'],
            'negative_zero_values':sum(raw([v])=='0000000000000080'for v in values) if not integer else 0}


def finalization(events):
    first=one(events,'finalize-before');last=one(events,'finalize-after');pre=first['state'];post=last['state']
    check(first['finalizer_serial']==last['finalizer_serial'],'finalizer pair')
    stages=[e for e in events if e['operation']in('stage-before','stage-after')]
    check([(e['operation'],e['stage'])for e in stages]==[(op,s)for s in('control','materials','objects')for op in('stage-before','stage-after')],'ordered finalizer stages')
    expected=deepcopy(pre)
    if expected['finalization_controls']['control_0x93c']==0:
        mode=int(expected['finalization_controls']['step_increment_0x930']<=0)
        expected['finalization_controls']['mode_0x990']=expected['time_table']['mode_0x58']=mode
    check(pre['material_count']==0 and all(o['flag_0x3990']==0 for o in pre['objects']),'selected empty-material, geometry-stage guard profile')
    check(pre['import_flag_0x2f88']==pre['import_flag_0x2f89']==0,'selected absent NSTART mutations')
    check(post==expected,'complete selected finalizer state and conditional mode normalization')
    for index,e in enumerate(stages):
        wanted=pre if index==0 else expected
        check(first['sequence']<e['sequence']<last['sequence'] and e['root']==pre['address']
              and e['finalizer_serial']==first['finalizer_serial'],'stage root/interval identity')
        for key in ('finalization_controls','time_table','time_controls_hex','import_flag_0x2f88','import_flag_0x2f89','material_count','material_list'):
            check(e[key]==wanted[key],'stage exact selected control/material view '+key)
    calls=[e for e in events if e['operation']=='object-stage-before']
    check([(e['object'],e['id'])for e in calls]==[(o['address'],o['id'])for o in pre['objects']if o['type']!=7],'active object finalization order')
    check(last['state']==events[-1]['state'],'import result equals finalizer return')


def selected_widths(state):
    dimension=state['dimension'];flags=state['output_flags'];mode=state['geometry_mode']
    check(len(flags)==7 and all(type(v)is int for v in flags),'seven output controls')
    vectors={1,2,3,5,18,19,62};scalars={9,11,13,14,15,16,20,21,22,23,44}
    widths={i:dimension for i in vectors};widths.update({i:1 for i in scalars})
    widths.update({40:max(1,sum(flags[:5]))*dimension*2+flags[5]+flags[6],51:0,52:0,
                   56:2,57:dimension*2,58:2,59:3 if dimension==3 else 2})
    if dimension==2 and mode==3:
        widths.update({2:3,3:3,18:3,19:3,40:6*max(1,sum(flags[:5]))+flags[5]+flags[6],57:6})
    for component,width in widths.items():
        check(state['components'][component]==width,'source-derived selected component width '+str(component))
    if state['linked_state'] is not None:selected_widths(state['linked_state'])


def validate(evidence,inputs):
    previous_result=base.validate(evidence,inputs) # Prior original-only token/material/allocation invariants.
    events=[m['payload']for m in evidence['normalized_result']['messages'][1:]]
    rows=descriptor_map(read(ROOT/'analysis/mesh-array-contract.json'));groups=[];current=[];results=[]
    for e in events:
        if 'state'in e and 'objects'in e['state']:
            base.shared.root_shape(e['state'],rows)
            if e['operation']in('import-before','import-after','handler-before','handler-after','finalize-before','finalize-after'):
                selected_widths(e['state'])
        if e['operation']=='import-before':current=[]
        current.append(e)
        if e['operation']=='import-after':groups.append(current)
    for g in groups:
        start=None;last_handler=None
        for i,e in enumerate(g):
            if e['operation']=='handler-before':
                start=i
                if last_handler is not None:check(e['state']==last_handler,'within-import full state continuity')
            elif e['operation']=='handler-after':
                check(start is not None,'field handler nesting')
                if e['id']in FIELDS:results.append({'file':e['file'],**handler(g[start:i+1],inputs[e['file']])})
                start=None;last_handler=e['state']
        if last_handler is not None:check(one(g,'finalize-before')['state']==last_handler,'finalizer receives exact last handler state')
        finalization(g)
    return {'imports':len(groups),'field_handlers':results,'coverage':{'ids':sorted(set(r['id']for r in results)),
            'id_dimensions':sorted(set((r['id'],r['dimension'])for r in results)),
            'failures':sum(r['status']!=0 for r in results),'zero_width':sum(r['width']==0 for r in results),
            'bccdef_converted_rows':sum(r['bccdef_converted_rows']for r in results),
            'bccdef_failure_converted_rows':sum(r['bccdef_converted_rows']for r in results if r['status']!=0),
            'negative_zero_values':sum(r['negative_zero_values']for r in results)},
            'event_counts':dict(Counter(e['operation']for e in events)),
            'prior_invariants':{'imports':previous_result['imports'],'references':previous_result['references']['event_counts']}}


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for key,value in [('capture','analysis/mesh-field-runtime-training-v2.json'),('run-record','analysis/oracle/mesh-field-training-v2/run.json'),
        ('manifest','analysis/mesh-field-evaluation-cases-v2.json'),('freeze','analysis/mesh-field-candidate-freeze.json'),
        ('input-freeze','analysis/mesh-field-v2-input-freeze.json'),('output','analysis/mesh-field-observations.json')]:
        p.add_argument('--'+key,type=Path,default=ROOT/value)
    args=p.parse_args()
    for key in vars(args):setattr(args,key,getattr(args,key).resolve())
    evidence,run,manifest,freeze,input_freeze=map(read,[args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze])
    inputs=provenance(evidence,run,manifest,freeze,input_freeze,args.run_record.parent,args.manifest,args.freeze)
    result=validate(evidence,inputs);rejected=[];not_applied=[]
    def tamper(label,op,mutate,predicate=lambda e:True):
        damaged=deepcopy(evidence);es=[m['payload']for m in damaged['normalized_result']['messages']]
        target=next((e for e in es if e['operation']==op and predicate(e)),None)
        if target is None:not_applied.append({'test':label,'reason':'No qualifying observation.'});return
        mutate(target)
        try:validate(damaged,inputs)
        except (ValueError,KeyError,IndexError,AssertionError,StopIteration,TypeError,struct.error):rejected.append(label)
        else:raise ValueError('tamper accepted: '+label)
    for key,value in [('sequence',0),('slot',0),('component',0),('domain','invalid'),('mode',99),('width',99),('element_count',99),('object','0x0')]:
        tamper('reader '+key,'field-reader-before',lambda e,k=key,v=value:e.update({k:v}))
    tamper('header source','field-line',lambda e:e.update(hex='302039390a'),lambda e:not e['eof'])
    tamper('line cursor','field-line',lambda e:e['after'].update(cursor=e['after']['cursor']+1))
    tamper('scalar source value','field-scan',lambda e:e.update(value=e['value']+1),lambda e:e['conversions']==1)
    tamper('failed scalar exposed','field-scan',lambda e:e.update(value=0),lambda e:e['conversions']==-1)
    tamper('header default destination','field-rows-before',lambda e:e['values'].__setitem__(0,987),lambda e:bool(e['values']))
    tamper('row stride width','field-rows-before',lambda e:e.update(width=e['width']+1))
    tamper('row declared count','field-rows-before',lambda e:e.update(declared_rows=e['declared_rows']+1))
    tamper('explicit width','field-rows-before',lambda e:e.update(explicit_width=e['explicit_width']+1),lambda e:e['kind']=='explicit')
    tamper('reader result status','field-reader-after',lambda e:e.update(status=1-e['status']))
    tamper('partial failure destination','field-reader-after',lambda e:e['values'].__setitem__(0,987),lambda e:e['status']!=0 and bool(e['values']))
    def coherent(e):
        e['values'][0]+=1;e['values_hex']=raw(e['values'],e.get('integers',False))
    tamper('coherent default write','field-rows-before',coherent,lambda e:bool(e['values']))
    tamper('coherent partial row write','field-reader-after',coherent,lambda e:e['status']!=0 and bool(e['values']))
    tamper('BCCDEF conversion omitted','field-postprocess',lambda e:e.update(after=deepcopy(e['before'])),lambda e:e['before']!=e['after'])
    tamper('BCCDEF first before wrap','field-postprocess',lambda e:e['before']['values'].__setitem__(0,1),lambda e:bool(e['before']['values']))
    def flip_zero(e):
        chunks=[e['values_hex'][i:i+16]for i in range(0,len(e['values_hex']),16)]
        chunks[chunks.index('0000000000000080')]='0000000000000000';e['values_hex']=''.join(chunks)
    tamper('negativezero sign lost','field-reader-after',flip_zero,lambda e:not e['integers'] and '0000000000000080'in[e['values_hex'][i:i+16]for i in range(0,len(e['values_hex']),16)])
    tamper('parent status','handler-after',lambda e:e.update(status=2-e['status']),lambda e:e['id']in FIELDS)
    tamper('root normalization mode','finalize-after',lambda e:e['state']['finalization_controls'].update(mode_0x990=99))
    tamper('unrelated object mutation','handler-after',lambda e:e['state']['objects'][0].update(material_group_0x3980=99),lambda e:e['id']in FIELDS)
    for label,key,value in [('process identity','pid',-1),('capture completeness','messages_truncated',True),('probe hash','source_sha256','0'*64)]:
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
    report={'status':'PASS','scope':__doc__.strip(),'evidence_id':evidence['evidence_id'],'result':result,
        'tampering_rejected':rejected,'tampering_not_applied':not_applied,
        'bindings':[binding(x)for x in [Path(__file__),args.capture,args.run_record,args.manifest,args.freeze,args.input_freeze,PROBE,CONTRACT]],
        'dependencies':[binding(ROOT/'tools'/n)for n in ['verify_material_column_observations.py','verify_material_object_observations.py',
            'verify_material_reference_observations.py','verify_material_import_observations.py','verify_boundary_sort_observations.py',
            'verify_mesh_array_observations.py','verify_mesh_refresh_observations.py','verify_geometry_segments_observations.py',
            'verify_time_controls_observations.py','verify_step_distance_observations.py','capture_oracle.py']],
        'limits':['Original-only selected finite numeric domain; no candidate execution/output or mesh-field comparator import.',
            'Fixture setup algorithms retain prior milestones; shapes, raw bits, continuity and field-only mutations checked here.',
            'Undefined first matching failures, OOM, positive DVOLF/TICF widths and arbitrary shared allocations remain outside scope.']}
    args.output.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(f"PASS {result['imports']} imports; {len(result['field_handlers'])} field handlers; {len(rejected)} corruption checks")


if __name__=='__main__':main()
