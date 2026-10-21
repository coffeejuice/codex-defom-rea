"""Independent original-only validation of default material/object integration."""
import argparse
from collections import Counter
from copy import deepcopy
import json
from pathlib import Path
import re
import struct

from capture_oracle import digest
from verify_boundary_sort_observations import ROOT, EXE_SHA, binding, check, read, verify_files
from verify_material_import_observations import (material_shape, record_shape, verify_handler as material_handler,
    verify_token, verify_input_freeze)
from verify_mesh_refresh_observations import SLOTS, bytes_values
from verify_mesh_array_observations import descriptor_map
from verify_geometry_segments_observations import shapes as segment_shapes
from verify_time_controls_observations import stream_position

PROBE=ROOT/'tools/observe_material_object_import.js'
CONTRACT=ROOT/'analysis/material-object-integration-contract.json'


def object_selector(e,data):
    check(e['format']=='%i','object selector format')
    pos=stream_position(e['before'],data)
    while pos<len(data) and data[pos] in b' \t\r\n\v\f':pos+=1
    match=re.match(rb'[+-]?(?:0[xX][0-9a-fA-F]+|0[0-7]*|[1-9][0-9]*)',data[pos:])
    check(match is not None,'selected deterministic object selector')
    token=match[0];unsigned=token.lstrip(b'+-')
    base=16 if unsigned.lower().startswith(b'0x') else 8 if unsigned.startswith(b'0') else 10
    value=int(token,base);end=pos+len(token)
    check(e['conversions']==1 and e['value']==value and -(2**31)<=value<2**31,'object selector numeric prefix')
    check(e['after']=={'cursor':end,'count':len(data)-end,'buffer_hex':data.hex()}
          or end==len(data) and e['after']=={'cursor':0,'count':0,'buffer_hex':''},'object selector cursor')


def provenance(evidence,run,manifest,freeze,directory):
    c=evidence['normalized_result'];p=evidence['parameters']
    check(run['status']=='completed' and run['exit_code']==0 and Path(run['cwd'])==directory,'completed original run')
    check(Path(run['executable'])==ROOT/'source/DEF_PRE_64.exe'
          and digest(Path(run['executable']))==run['executable_sha256']==EXE_SHA,'original identity')
    check(c['pid']==p['pid']==run['pid'] and c['session_id']==p['session_id']
          and c['target']==f"DEF_PRE_64.exe (pid {run['pid']})",'capture process/session')
    check(c['state']=='detached' and c['mode']==p['mode']=='attach'
          and c['device_id']==p['device_id']=='local' and c['source_kind']=='file'
          and not c['messages_truncated'] and c['cleanup_error'] is None,'complete passive capture')
    verify_files(run,directory)
    check(sum((directory/n).stat().st_size for n in ['stdout.bin','stderr.bin'])<run['max_output_bytes'],'output bound')
    pid=read(directory.with_suffix('.pid.json'))
    check(all(pid[k]==run[k] for k in ['pid','cwd','executable','executable_sha256','ready_gate']),'PID record')
    check(pid['inputs']==[{k:x[k] for k in ['name','source','copy','sha256_before']} for x in run['inputs']],'PID input inventory')
    script=Path(c['source_path']);gate=directory.with_suffix('.ready')
    check(script in [directory.with_suffix('.js'),directory.with_name(directory.name+'-probe.js')]
          and digest(script)==c['source_sha256'] and Path(run['ready_gate'])==gate,'script/gate identity')
    check(script.read_text(encoding='utf-8-sig')==PROBE.read_text(encoding='utf-8-sig').replace(
          '__READY_GATE__',gate.as_posix()),'exact passive probe template')
    check(manifest['probe']==binding(PROBE),'manifest probe binding')
    names=[Path(x['path']).name for x in manifest['cases']]
    check(0<len(names)<=40 and len(set(names))==len(names)
          and names==run['ordered_imports']==[x['name'] for x in run['inputs']],'ordered imports')
    inputs={}
    for case,item in zip(manifest['cases'],run['inputs']):
        source=ROOT/case['path'];copy=directory/item['name']
        check(Path(item['source'])==source and Path(item['copy'])==copy
              and digest(source)==digest(copy)==case['sha256']==item['sha256_before']
              ==item['sha256_after']==item['source_sha256_after'],'fixture source/copy identity')
        data=source.read_bytes().replace(b'\r\n',b'\n')
        check(data.isascii() and len(data)<4096 and b'\0' not in data and b'\x1a' not in data,'bounded fixture bytes')
        inputs[item['name']]=data
    check(run['stdin']=='\n2\n'+''.join('1\n'+n+'\n\n' for n in names)+'E\nE\nY\n','menu protocol')
    for key in ['files','inputs']:
        check(freeze[key] and len({f['path'] for f in freeze[key]})==len(freeze[key]),'unique freeze inventory')
        for f in freeze[key]:check(digest(ROOT/f['archive'])==f['sha256'],'frozen archive identity')
    declared=manifest.get('source_bindings',[])+manifest.get('syntax_evidence',[])
    check(declared,'manifest static or syntax bindings')
    if 'probe_unchanged_from_v2' in manifest:declared.append(manifest['probe_unchanged_from_v2'])
    for b in read(CONTRACT)['source_bindings']+declared:
        check(digest(ROOT/b['path'])==b['sha256'],'static source binding')
    return inputs


def buffer_shape(b,name,address):
    check(int(b['slot'],16)==int(address,16)+SLOTS[name] and b['integers']==(name!='coordinates'),'buffer slot/type')
    check(b['extent_source']!='unknown-not-read','unknown extent in selected capture')
    n,k=b['allocated_items'],b['known_prefix']
    check(type(n)is int and type(k)is int and 0<=k<=n<=512 and b['unknown_items']==n-k
          and len(b['values'])==n and b['values'][k:]==[None]*(n-k),'known prefix/null mask')
    check(len(bytes_values(b['known_hex'],b['values'][:k],b['integers']))==k*(4 if b['integers'] else 8),'buffer raw extent')
    if b['extent_source']=='null-pointer':
        check(b['pointer']=='0x0' and n==k==0,'null buffer extent')
    else:
        check(int(b['pointer'],16)>0 and b['extent_source'] in
              ['observed-calloc-allocator','observed-preserving-realloc']
              and type(b['allocation_serial'])is int,'buffer allocation provenance')


def object_shape(o,rows,temporary=False):
    check(int(o['address'],16)>0 and all(type(o[k])is int and o[k]>=0 for k in
          ['nodes','elements','node_allocation_count','element_allocation_count','allocation_arity']),'object bounds')
    ds=o['descriptors']
    check(len(ds)==len({d['id'] for d in ds})==len({d['address'] for d in ds})==53,'53 unique descriptors')
    for d,row in zip(ds,rows):
        check(d['id']==row['state_id'] and d['slot_offset']==int(row['slot_relative_to_object'],16)
              and d['vtable_rva']==int(row['vtable_address'],16)-0x140000000,'descriptor mapping')
        check(all(type(d[k])is int and d[k]>=0 for k in
              ['count','width','multiplicity','old_count','old_width','old_multiplicity','length']),'descriptor bounds')
        check(d['length']==d['count']*d['width']*d['multiplicity']<=512
              and d['multiplicity']==d['storage_references']==1
              and d['bound_pointer']==d['storage_pointer'],'owned descriptor shape/binding')
        check(len(bytes_values(d['values_hex'],d['values'] or [],False))==d['length']*8,'descriptor raw bytes')
        check(d['length']==0 or int(d['storage_pointer'],16)>0,'descriptor allocation')
    check(set(o['buffer_metadata'])==set(SLOTS),'direct inventory')
    for name,b in o['buffer_metadata'].items():
        buffer_shape(b,name,o['address'])
        if not temporary:
            key=name[6:] if name.startswith('direct') else name
            check(o['initialized_prefixes'][key]==b['known_prefix']
                  and (o['direct'][key] if name.startswith('direct') else o[name])==b['values'],'direct projections')
    if not temporary:
        segment_shapes(o['geometry_segments'])
        check(o['geometry_segments']['list']['references']==1,'owned alternate list')


def root_shape(s,rows):
    check(s['dimension'] in [2,3] and len(s['components'])==79 and s['components'][0]==s['dimension'],'root dimension/table')
    check(s['object_count']==len(s['objects'])<=3
          and [o['id'] for o in s['objects']]==list(range(1,len(s['objects'])+1)),'object insertion/placeholder list')
    material_shape(s['material_list'])
    check(s['material_list']['owner']==s['address'] and s['material_count']
          ==s['material_list']['root_count']==s['material_list']['count'],'root material count/owner')
    for o in s['objects']:
        check(o['dimension']==s['dimension'] and o['components']==s['components'],'object root projection')
        object_shape(o,rows)
    if s['linked_state'] is not None:
        root_shape(s['linked_state'],rows)
        check(s['linked_state']['dimension']==5-s['dimension'] and s['linked_state']['objects']==[]
              and s['linked_state']['material_count']==0,'linked root selected scope')


def material_projection(events):
    # The reused verifier checks material/list/file semantics only. Object values
    # are checked on the unprojected original snapshots below.
    projected=deepcopy(events)
    def visit(x):
        if isinstance(x,dict):
            if 'material_list' in x and 'components' in x and 'objects' in x:
                x['objects']=[];x['object_count']=0
            for value in x.values():visit(value)
        elif isinstance(x,list):
            for value in x:visit(value)
    visit(projected)
    return projected


def refresh_object(old,new,components):
    expected=deepcopy(old);expected['components']=components
    if old['type']==7:
        check(expected==new,'inactive object untouched by material refresh')
        return
    ignored={'components','descriptors','buffer_metadata','initialized_prefixes','coordinates','connectivity','direct'}
    check(all(old[k]==new[k] for k in old if k not in ignored),'refresh preserves unrelated object state')
    for a,b in zip(old['descriptors'],new['descriptors']):
        count=old['node_allocation_count' if a['id']<36 else 'element_allocation_count']
        if count<=0 or a['id'] in range(30,34):
            check(a==b,'unselected/nonpositive descriptor unchanged');continue
        width=components[a['id']];n=count*width*a['multiplicity']
        for k in ['address','id','vtable_rva','slot_offset','multiplicity','storage_references']:
            check(a[k]==b[k],'descriptor identity/policy')
        check(b['count']==count and b['width']==width and b['length']==n
              and b['old_count']==a['count'] and b['old_width']==a['width']
              and b['old_multiplicity']==(a['multiplicity'] if a['id'] in [38,43,45] else a['old_multiplicity']),
              'refresh descriptor dimensions/history')
        raw=bytes.fromhex(a['values_hex']);wanted=(raw[:n*8]+bytes(max(0,n*8-len(raw))))
        # In this reset profile MATAXI width is 0 or9 and existing9 stays9.
        check(bytes.fromhex(b['values_hex'])==wanted,'descriptor preserving resize values')
    for name,a in old['buffer_metadata'].items():
        b=new['buffer_metadata'][name]
        count=old['node_allocation_count' if name not in ['connectivity','direct61'] else 'element_allocation_count']
        if count<=0:check(a==b,'nonpositive direct container unchanged');continue
        width=(old['allocation_arity'] if name=='connectivity' else components[0 if name=='coordinates' else int(name[6:])])
        n=count*width;k=min(a['known_prefix'],n);size=4 if a['integers'] else 8
        check(b['allocated_items']==n and b['known_prefix']==k
              and b['known_hex']==a['known_hex'][:k*size*2],'direct initialized prefix survives resize')


def refresh_events(events):
    before=next(e for e in events if e['operation']=='material-refresh-before')
    after=next(e for e in events if e['operation']=='material-refresh-after')
    layouts=[e for e in events if e['operation'].startswith('material-layout-')]
    check(len(layouts)==2 and before['sequence']<layouts[0]['sequence']<layouts[1]['sequence']<after['sequence'],'layout inside refresh')
    old,new=before['state'],after['state']
    check(len(old['objects'])==len(new['objects']),'refresh stable object inventory')
    live={o['address']:o for o in old['objects']}
    updates=[e for e in events if e['operation']=='allocation-update' and e['object'] in live]
    expected=[(o['address'],kind) for o in old['objects'] if o['type']!=7 for kind in ['nodes','elements']]
    check([(e['object'],e['kind']) for e in updates]==expected,'active-only ordered container refresh')
    for e in updates:
        o=live[e['object']];count=o['node_allocation_count' if e['kind']=='nodes' else 'element_allocation_count']
        check(e['mode']=='refresh' and e['requested_count']==count
              and e['requested_arity']==(None if e['kind']=='nodes' else o['allocation_arity'])
              and layouts[1]['sequence']<e['sequence']<after['sequence'],'allocation uses stored count/arity after layout')
    for a,b in zip(old['objects'],new['objects']):refresh_object(a,b,new['components'])
    copies=[e for e in events if e['operation']=='material-copy-before']
    scratch=old['material_list']['scratch']
    enabled=all(scratch[k]!='0x0' for k in ['ids_pointer','ranks_pointer'])
    check([e['source']['address'] for e in copies]==([o['address'] for o in old['objects'] if o['type']!=7] if enabled else []),'temporary active object selection')
    for e in copies:
        check(before['sequence']<e['sequence']<layouts[0]['sequence'],'copy before root layout')
        check(e['components']==old['components'] and e['flags']==[1,1,1,1,0,1] and e['root']==old['address'],'copy old table/flags/root')
        serial=e['copy_serial']
        end=next(x for x in events if x['operation']=='material-copy-after' and x['copy_serial']==serial)
        destroy=next(x for x in events if x['operation']=='material-copy-destroy-before' and x['copy_serial']==serial)
        destroyed=next(x for x in events if x['operation']=='material-copy-destroy-after' and x['copy_serial']==serial)
        check(e['sequence']<end['sequence']<layouts[0]['sequence']<destroy['sequence']<destroyed['sequence']<after['sequence'],'copy lifetime ordering')
        check(e['source']==end['source'],'temporary copy preserves complete owned source snapshot')
        check(e['destination']['address']==end['destination']['address']==destroy['destination']['address']==destroyed['address']
              and e['destination']['address']!=e['source']['address'],'separate stable temporary identity')
        dst=end['destination'];src=e['source']
        copied_updates=[x for x in events if x['operation']=='allocation-update'
                        and x['object']==dst['address'] and e['sequence']<x['sequence']<end['sequence']]
        check([x['kind'] for x in copied_updates]==(['nodes'] if src['nodes']>0 else [])+['elements'],
              'temporary allocation call selection')
        for x in copied_updates:
            check(x['mode']=='refresh' and x['requested_count']==src['nodes' if x['kind']=='nodes' else 'elements']
                  and x['requested_arity']==(None if x['kind']=='nodes' else src['arity']),
                  'temporary allocation uses source header count/selected arity')
        check(dst['owner']==e['root'] and dst['id']==0 and destroy['destination']['id']==src['id'],'temporary ID assigned after copy return')
        for key in ['type','mesh_flag','nodes','elements','arity','flag_0x3990','boundary_total']:
            check(dst[key]==src[key],'temporary selected value copy')
        check(dst['name_hex']==src['name'].encode('ascii').hex(),'temporary name copy')
        check(dst['geometry_payload_omitted'] is True,'temporary geometry read boundary')
        check(dst['spatial_geometry_pointer']!=src['geometry']['slot0']
              and dst['alternate_geometry_handle']!=src['geometry_segments']['handle'],
              'separate temporary geometry owners')
        for a,b in zip(src['descriptors'],dst['descriptors']):
            check(a['address']!=b['address'] and a['id']==b['id'] and a['values_hex']==b['values_hex'],'deep descriptor values')
            if a['length'] and b['length']:check(a['storage_pointer']!=b['storage_pointer'],'deep descriptor allocation')
        for name,b in dst['buffer_metadata'].items():
            a=src['buffer_metadata'][name]
            if a['allocated_items'] and b['allocated_items']:check(a['pointer']!=b['pointer'],'deep direct allocation')
    return len(copies),len(updates)

def validate(evidence,inputs,rows):
    cap=evidence['normalized_result'];messages=cap['messages']
    check(len(cap['scripts'])==1 and all(m['type']=='send' and m['script_id']==cap['scripts'][0]['script_id'] for m in messages),'one script without runtime errors')
    check(messages[0]['payload']['operation']=='hooks-installed' and messages[0]['payload']['pid']==cap['pid'],'hooks identity')
    events=[m['payload'] for m in messages[1:]]
    check([e['sequence'] for e in events]==list(range(1,len(events)+1)),'continuous sequence')
    check(not any(e['operation']=='probe-error' for e in events),'no probe errors')
    groups=[];active=None
    for event in events:
        if 'state' in event:
            s=event['state']
            if 'dimension' in s:root_shape(s,rows)
            elif 'records' in s:material_shape(s)
        if 'material_list' in event:material_shape(event['material_list'])
        if event.get('record') is not None:record_shape(event['record'])
        if event['operation'] in ['material-copy-before','material-copy-after']:
            object_shape(event['source'],rows);object_shape(event['destination'],rows,True)
        if event['operation']=='material-copy-destroy-before':object_shape(event['destination'],rows,True)
        if event['operation']=='import-before':
            check(active is None,'non-nested import');active=[event]
        else:
            check(active is not None and event['file']==active[0]['file'],'event import owner');active.append(event)
            if event['operation']=='import-after':groups.append(active);active=None
    check(active is None and [g[0]['file'] for g in groups]==list(inputs),'complete import order')
    previous=None;results=[];totals=Counter();identities={};dimension=None
    for group in groups:
        first,last=group[0],group[-1];old,new=first['state'],last['state'];data=inputs[first['file']]
        if previous is not None:check(previous==old,'between-import continuity')
        else:check(old['material_count']==old['object_count']==0,'fresh empty initial lists')
        previous=new
        frames=[];handlers=[];clears=[];destroyed=[];last_token=None;unknown_tokens=[]
        for event in group[1:-1]:
            op=event['operation']
            if op=='stream-scan':
                if event['format']=='%39s':verify_token(event,data);last_token=event
                else:object_selector(event,data)
            if op=='keyword-resolved':
                check(last_token is not None and event['token']==last_token['value'],'token resolution provenance')
                if event['category']==0:
                    check(event['id']==305,'unknown token sentinel')
                    unknown_tokens.append({'token':event['token'],'sequence':event['sequence']})
                    totals['unknown_tokens']+=1
            if op=='handler-before':
                check(not frames,'non-nested handler');frames=[event]
            elif frames:
                frames.append(event)
            if op=='handler-after':
                check(frames and frames[0]['id']==event['id'] and frames[0]['category']==event['category'],'handler pair')
                before=frames[0]['state'];after=event['state']
                if dimension!=before['dimension']:
                    dimension=before['dimension'];identities={}
                for o in before['objects']+after['objects']:
                    identity=(o['address'],tuple(d['address'] for d in o['descriptors']))
                    check(identities.setdefault(o['id'],identity)==identity,'object generation identity')
                if event['category']==4:
                    check(event['id']==92,'selected material handler')
                    summary=material_handler(material_projection(frames),data)
                    allowed={'material_list','material_count','components','objects'}
                    check(all(before[k]==after[k] for k in before if k not in allowed),'material handler unrelated root state')
                    if summary['created']:
                        nc,nu=refresh_events(frames);totals['temporary_copies']+=nc;totals['container_refreshes']+=nu
                        totals['created_materials']+=1
                        if summary['status']==2:totals['created_name_failures']+=1
                        if before['objects']:totals['materials_after_objects']+=1
                    else:
                        check(before['objects']==after['objects'],'rename/EOF leaves objects exact')
                        totals['renames_or_selector_eof']+=1
                    handlers.append(summary)
                elif event['category']==1:
                    check(event['id'] in [28,34,38,48,49,50],'previously recovered control handler')
                    check(before['material_list']==after['material_list'] and before['material_count']==after['material_count'],
                          'control handler preserves materials')
                    totals['control_handlers']+=1
                    control_refreshes=[e for e in frames if e['operation']=='material-refresh-before']
                    if control_refreshes:
                        check(len(control_refreshes)==1 and event['id'] in [28,34]
                              and control_refreshes[0]['option']==0,'configuration preserving refresh option')
                        nc,nu=refresh_events(frames)
                        check(nc==0,'configuration refresh performs no material copy')
                        totals['configuration_container_refreshes']+=nu
                    handlers.append({'id':event['id'],'category':1,'status':event['status'],'created':False})
                else:
                    check(event['category']==2 and event['id'] in [144,145,153,158,159,173],'selected object handler')
                    check(before['material_list']==after['material_list'] and before['material_count']==after['material_count'],'object handler preserves materials')
                    selected=[e for e in frames if e['operation']=='object-selected']
                    check(len(selected)==1 and selected[0]['found'] and selected[0]['requested']>0,'single valid object selection')
                    target=selected[0]['requested']
                    if event['id']==173:
                        if event['status']==0:
                            totals['successful_connectivity_'+str(before['dimension'])+'d']+=1
                            connected=after['objects'][target-1]
                            check(connected['mesh_flag']==1 and connected['elements']==connected['element_allocation_count']
                                  and connected['allocation_arity']==connected['arity']
                                  and len(connected['connectivity'])==connected['elements']*connected['arity'],
                                  'successful connectivity allocated extent')
                            totals['successful_connectivity_'+str(before['dimension'])+'d_arity'+str(connected['arity'])]+=1
                        else:
                            check(event['status']==2 and before==after,'failed connectivity handler preserves selected state')
                            totals['failed_connectivity_handlers']+=1
                    for a,b in zip(before['objects'],after['objects']):
                        if a['id']!=target:check(a==b,'unselected existing object unchanged')
                    if len(after['objects'])>len(before['objects']):
                        check(len(after['objects'])==target,'placeholder creation reaches requested ID')
                        for o in after['objects'][len(before['objects']):]:
                            if o['id']!=target:
                                check(o['type']==7 and o['nodes']==o['elements']==o['mesh_flag']==0,'inactive placeholder defaults')
                        if before['material_count']:totals['objects_after_materials']+=1
                    handlers.append({'id':event['id'],'category':2,'status':event['status'],'created':False})
                totals['handlers']+=1;frames=[]
            if op=='material-clear-before':clears.append({'before':event,'records':[]})
            elif op=='material-destroy-before':
                check(clears,'record destruction inside clear');clears[-1]['records'].append(event['record']);destroyed.append(event['record']['address'])
            elif op=='material-destroy-after':check(destroyed and destroyed.pop()==event['address'],'paired material destructor')
            elif op=='material-clear-after':
                check(clears,'paired list clear');frame=clears.pop()
                check(frame['records']==frame['before']['state']['records'] and event['state']['records']==[]
                      and event['state']['count']==0,'insertion-order material destruction')
        check(not frames and not clears and not destroyed,'complete handler/lifecycle frames')
        finals=[e for e in group if e['operation'] in ['finalize-before','finalize-after']]
        check(len(finals)==2 and finals[0]['operation']=='finalize-before' and finals[1]['operation']=='finalize-after'
              and finals[0]['finalizer_serial']==finals[1]['finalizer_serial'],'single paired finalizer')
        pre,post=finals[0]['state'],finals[1]['state']
        check(post==new,'finalizer return equals import state')
        stages=[e for e in group if e['operation'] in ['stage-before','stage-after']]
        check([(e['operation'],e['stage']) for e in stages]==[
            ('stage-before','control'),('stage-after','control'),('stage-before','materials'),
            ('stage-after','materials'),('stage-before','objects'),('stage-after','objects')],'finalization stage order')
        check(all(finals[0]['sequence']<e['sequence']<finals[1]['sequence']
              and e['root']==pre['address'] and e['finalizer_serial']==finals[0]['finalizer_serial'] for e in stages),'finalization stage interval/owner')
        check(pre['material_list']==post['material_list'] and all(e['material_list']==pre['material_list'] for e in stages),'default material finalization no-op')
        check({k:v for k,v in pre.items() if k!='objects'}=={k:v for k,v in post.items() if k!='objects'},'selected finalizer root stability')
        for a,b in zip(pre['objects'],post['objects']):
            check({k:v for k,v in a.items() if k!='geometry'}=={k:v for k,v in b.items() if k!='geometry'},'finalizer changes only selected geometry')
        object_stages=[e for e in group if e['operation']=='object-stage-before']
        check([(e['object'],e['id']) for e in object_stages]==[(o['address'],o['id']) for o in pre['objects'] if o['type']!=7],'active-only object finalization')
        changed=old['dimension']!=new['dimension']
        if changed:
            check(any(e['operation']=='material-clear-before' for e in group),'dimension reset clears materials')
            first_handler=next((e for e in group if e['operation']=='handler-before'),finals[0])
            check(first_handler['state']['object_count']==first_handler['state']['material_count']==0,'reset both lists before handlers')
            totals['dimension_resets']+=1
            if not handlers:identities={};dimension=new['dimension']
        statuses=[h['status'] for h in handlers if h['status']]
        check(last['status']==(statuses[-1] if statuses else 0),'last handler failure status')
        results.append({'file':first['file'],'status':last['status'],'handlers':handlers,
                        'unknown_tokens':unknown_tokens,
                        'dimension':new['dimension'],'object_ids':[o['id'] for o in new['objects']],
                        'material_ids':[r['id'] for r in new['material_list']['records']],
                        'sequence_range':[first['sequence'],last['sequence']]})
    check(totals['materials_after_objects']>0 and totals['objects_after_materials']>0
          and totals['temporary_copies']>0 and totals['created_name_failures']>0,'cross-domain fixture coverage')
    return {'imports':results,'counts':dict(totals),'event_count':len(events)}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--capture',type=Path,default=ROOT/'analysis/material-object-runtime-training-v2.json')
    parser.add_argument('--run-record',type=Path,default=ROOT/'analysis/oracle/material-object-training-v2/run.json')
    parser.add_argument('--manifest',type=Path,default=ROOT/'analysis/material-object-training-v2-cases.json')
    parser.add_argument('--freeze',type=Path,default=ROOT/'analysis/material-object-candidate-freeze.json')
    parser.add_argument('--input-freeze',type=Path,default=ROOT/'analysis/material-object-v2-input-freeze.json')
    parser.add_argument('--output',type=Path,default=ROOT/'analysis/material-object-observations.json')
    args=parser.parse_args()
    evidence,run,manifest,freeze=map(read,[args.capture,args.run_record,args.manifest,args.freeze])
    directory=args.run_record.resolve().parent
    inputs=provenance(evidence,run,manifest,freeze,directory)
    rows=descriptor_map(read(ROOT/'analysis/mesh-array-contract.json'))
    result=validate(evidence,inputs,rows);rejected=[]
    def reject(label,mutate):
        damaged=deepcopy(evidence);events=[m['payload'] for m in damaged['normalized_result']['messages']]
        mutate(events)
        try:validate(damaged,inputs,rows)
        except (ValueError,KeyError,IndexError,TypeError,struct.error):rejected.append(label)
        else:raise ValueError('tamper accepted: '+label)
    def pick(events,op,predicate=lambda e:True):return next(e for e in events if e['operation']==op and predicate(e))
    reject('source copy mutation',lambda es:pick(es,'material-copy-after')['source'].update(boundary_total=99))
    reject('temporary aliases source',lambda es:pick(es,'material-copy-after')['destination'].update(address=pick(es,'material-copy-after')['source']['address']))
    reject('temporary byte flags',lambda es:pick(es,'material-copy-before')['flags'].__setitem__(4,1))
    reject('copy old components',lambda es:pick(es,'material-copy-before')['components'].__setitem__(1,77))
    reject('temporary descriptor deep allocation',lambda es:pick(es,'material-copy-after')['destination']['descriptors'][0].update(address=pick(es,'material-copy-after')['source']['descriptors'][0]['address']))
    reject('copy destructor identity',lambda es:pick(es,'material-copy-destroy-after').update(address='0x1'))
    reject('material refresh option',lambda es:pick(es,'material-refresh-before').update(option=0))
    reject('material scratch rank',lambda es:pick(es,'material-scratch-after',lambda e:e['mode']==0 and e['state']['scratch']['count']>0)['state']['scratch']['ranks'].__setitem__(0,8))
    reject('material name bytes',lambda es:pick(es,'material-name')['name'].update(name_hex='58',name_length=1))
    reject('material helper status',lambda es:pick(es,'material-name').update(status=1))
    reject('material selector bytes',lambda es:pick(es,'material-selector',lambda e:e['conversions']==1).update(output_hex='ffffffff'))
    reject('material count',lambda es:pick(es,'handler-after',lambda e:e['category']==4)['state']['material_list'].update(count=0))
    reject('refresh requested count',lambda es:pick(es,'allocation-update',lambda e:e['mode']=='refresh').update(requested_count=99))
    reject('descriptor storage reference',lambda es:pick(es,'material-copy-before')['source']['descriptors'][0].update(storage_references=2))
    reject('known prefix',lambda es:pick(es,'material-copy-before')['source']['buffer_metadata']['coordinates'].update(known_prefix=99))
    reject('descriptor raw payload',lambda es:pick(es,'material-copy-before')['source']['descriptors'][0].update(values_hex='ff'))
    reject('root layout table',lambda es:pick(es,'material-layout-after')['state']['components'].__setitem__(1,99))
    reject('inactive selection',lambda es:pick(es,'material-copy-before')['source'].update(type=7))
    reject('object material interference',lambda es:pick(es,'handler-after',lambda e:e['category']==2 and e['state']['material_count']>0)['state']['material_list']['records'][0].update(id=99))
    reject('material finalizer mutation',lambda es:pick(es,'stage-after',lambda e:e['stage']=='materials' and e['material_count']>0)['material_list']['records'][0].update(id=99))
    reject('event sequence',lambda es:pick(es,'material-copy-after').update(sequence=0))
    reject('probe error',lambda es:pick(es,'material-copy-after').update(operation='probe-error'))
    original_events=[m['payload'] for m in evidence['normalized_result']['messages']]
    if any(e['operation']=='keyword-resolved' and e['category']==0 for e in original_events):
        reject('unknown token sentinel',lambda es:pick(es,'keyword-resolved',lambda e:e['category']==0).update(id=92))
    if any(e['operation']=='handler-after' and e['id']==173 and e['status']==2 for e in original_events):
        reject('failed connectivity mutation',lambda es:pick(es,'handler-after',lambda e:e['id']==173 and e['status']==2)['state']['objects'][0].update(mesh_flag=99))
        reject('failed connectivity status',lambda es:pick(es,'handler-after',lambda e:e['id']==173 and e['status']==2).update(status=3))
    if any(e['operation']=='material-refresh-before' and e['option']==0 for e in original_events):
        reject('configuration refresh option',lambda es:pick(es,'material-refresh-before',lambda e:e['option']==0).update(option=1))
    for field,value in [('pid',-1),('source_sha256','0'*64),('messages_truncated',True)]:
        damaged=deepcopy(evidence);damaged['normalized_result'][field]=value
        try:provenance(damaged,run,manifest,freeze,directory)
        except ValueError:rejected.append('provenance '+field)
        else:raise ValueError('provenance tamper accepted: '+field)
    damaged=deepcopy(manifest);damaged['cases'][0]['sha256']='0'*64
    try:provenance(evidence,run,damaged,freeze,directory)
    except ValueError:rejected.append('fixture hash')
    else:raise ValueError('fixture tamper accepted')
    if args.input_freeze:
        input_freeze=read(args.input_freeze);verify_input_freeze(input_freeze,manifest,args.manifest,args.freeze)
        for key in ['candidate_manifest','fixture_manifest','inputs']:
            damaged=deepcopy(input_freeze)
            (damaged[key][0] if key=='inputs' else damaged[key])['sha256']='0'*64
            try:verify_input_freeze(damaged,manifest,args.manifest,args.freeze)
            except ValueError:rejected.append('input freeze '+key)
            else:raise ValueError('input freeze tamper accepted: '+key)
    paths=[args.capture,args.run_record,args.manifest,args.freeze,Path(__file__),PROBE,CONTRACT,
           ROOT/'analysis/mesh-array-contract.json',ROOT/'tools/verify_material_import_observations.py',
           ROOT/'tools/verify_mesh_refresh_observations.py',ROOT/'tools/verify_mesh_array_observations.py',
           ROOT/'tools/verify_geometry_segments_observations.py',ROOT/'tools/verify_boundary_sort_observations.py',
           ROOT/'tools/verify_step_distance_observations.py',ROOT/'tools/verify_time_controls_observations.py',
           ROOT/'tools/capture_oracle.py']
    if args.input_freeze:paths.append(args.input_freeze)
    report={'status':'PASS','scope':'Original-only default material/object interaction, owned source copies, deep destination storage, active-only refresh and lifecycle.',
            'evidence_id':evidence['evidence_id'],'pid':run['pid'],'result':result,'tampering_rejected':rejected,
            'bindings':[binding(p) for p in paths],
            'candidate_current':[{'path':f['path'],'matches_freeze':digest(ROOT/f['path'])==f['sha256']} for f in freeze['files']],
            'limitations':['No replacement execution. No scientific material property or nonempty reference-graph claim.',
                           'PASS denotes valid observations, not successful application imports. Training01/05/07 omit3D ELMCON arity: status2 and subsequent unknown token are retained; these do not establish successful3D connectivity coverage.',
                           'Temporary geometry payload is intentionally omitted by probe; source geometry and destination pointer metadata are retained.',
                           'Owned storage refcount1 is asserted; external/shared native aliases are outside this validation.',
                           'Object handlers are checked for material independence and identity/lifecycle; their numerical parser/geometry algorithms have separate milestone verifiers.',
                           'Previously recovered scalar/configuration handlers are checked for material independence and snapshot consistency here; this report adds no independent arithmetic/parser claim for them.',
                           'Allocation failures, corrupt counts and undefined selector matching failure remain outside the selected finite bounded fixtures.']}
    args.output.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(f"PASS {len(inputs)} imports; {result['counts']['temporary_copies']} temporary copies; {len(rejected)} tamper checks")


if __name__=='__main__':main()


