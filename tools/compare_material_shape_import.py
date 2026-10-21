"""Compare the selected material shape-property import profile with retained original evidence.

Executes only replacement replay drivers. Captured names, lists, counts, defaults,
root tables and reader calls are the oracle; no original process is launched.
"""
import argparse
from collections import Counter
import json
import math
from pathlib import Path
import struct
import subprocess

from capture_oracle import digest
from compare_keyword_import import ROOT, EXE_SHA, require, read, binding
from compare_mesh_refresh import compare_values as differences_at, values_from_hex
from copy import deepcopy
from compare_geometry_segments import (decode_raw_controls, normalize_table,
    RootIdentity as GeometryRootIdentity, compare_helpers as compare_geometry_helpers,
    compare_segment_scans, normalize_geometry, normalize_segments)
from verify_mesh_array_observations import descriptor_map

def verify_candidate(driver, reader_driver, enforce, path):
    path = path.resolve(strict=True)
    freeze = read(path)
    current_files = []
    for item in freeze["files"]:
        require(digest(ROOT / item["archive"]) == item["sha256"],
                "frozen archive changed: " + item["archive"])
        current = ROOT / item["path"]
        current_sha = digest(current) if current.is_file() else None
        current_files.append({"path": item["path"], "sha256": current_sha,
                              "matches_freeze": current_sha == item["sha256"]})
    binaries = {Path(item["path"]).name: item["sha256"] for item in freeze["files"] if item["path"].endswith(".exe")}
    driver_matches = digest(driver) == binaries[driver.name]
    reader_matches = digest(reader_driver) == binaries[reader_driver.name]
    current_matches = all(item["matches_freeze"] for item in current_files)
    if enforce:
        require(current_matches and driver_matches and reader_matches, "candidate changed under frozen enforcement")
    return {"manifest": binding(path), "created_utc": freeze["created_utc"],
            "stage": freeze["stage"], "verified_archives": freeze["files"],
            "current_files": current_files, "current_files_match_freeze": current_matches,
            "driver_matches_freeze": driver_matches, "reader_matches_freeze": reader_matches, "frozen_candidate_required": enforce}


def provenance(path, run_path, fixtures, template):
    evidence, run = read(path), read(run_path)
    cap, params = evidence["normalized_result"], evidence["parameters"]
    directory = run_path.parent.resolve()
    require(Path(run["cwd"]).resolve() == directory, "run directory mismatch")
    require(run["status"] == "completed" and run["exit_code"] == 0,
            "original run did not complete normally")
    executable = ROOT / "source/DEF_PRE_64.exe"
    require(Path(run["executable"]).resolve() == executable.resolve()
            and run["executable_sha256"] == digest(executable) == EXE_SHA,
            "original executable identity mismatch")
    require(cap["pid"] == params["pid"] == run["pid"]
            and cap["target"] == f"DEF_PRE_64.exe (pid {run['pid']})"
            and cap["session_id"] == params["session_id"], "process/session mismatch")
    require(cap["state"] == "detached" and cap["mode"] == params["mode"] == "attach"
            and cap["device_id"] == params["device_id"] == "local"
            and cap["source_kind"] == "file", "capture lifecycle mismatch")
    require(cap["messages_truncated"] is False and cap["cleanup_error"] is None,
            "incomplete capture")
    script = Path(cap["source_path"]).resolve()
    require(script in (directory.with_suffix(".js"),
                       directory.with_name(directory.name + "-probe.js"))
            and digest(script) == cap["source_sha256"], "probe path/hash mismatch")
    gate = Path(run["ready_gate"]).resolve()
    require(gate == directory.with_suffix(".ready"), "gate path mismatch")
    require(script.read_text(encoding="utf-8-sig")
            == template.read_text(encoding="utf-8-sig").replace("__READY_GATE__", gate.as_posix()),
            "retained probe differs from passive template")
    pid_record = read(directory.with_suffix(".pid.json"))
    require(pid_record["pid"] == run["pid"]
            and pid_record["executable_sha256"] == EXE_SHA, "PID record mismatch")
    for item in run["files"]:
        output = directory / item["name"]
        require(output.parent.resolve() == directory and output.is_file()
                and output.stat().st_size == item["bytes"] and digest(output) == item["sha256"],
                "run output hash/length mismatch: " + item["name"])
    require(sum((directory / name).stat().st_size for name in ("stdout.bin", "stderr.bin"))
            < run["max_output_bytes"], "run output exceeded its bound")
    require((directory / "stdin.bin").read_bytes() == run["stdin"].encode("utf-8"),
            "stdin transcript mismatch")
    require([item["name"] for item in run["inputs"]] == run["ordered_imports"],
            "run input order mismatch")
    source_files, fixture_bindings = {}, []
    for item in run["inputs"]:
        source = (fixtures / item["name"] if fixtures else Path(item["source"])).resolve()
        copy = directory / item["name"]
        require(Path(item["source"]).resolve() == source and Path(item["copy"]).resolve() == copy,
                "input source/copy path mismatch")
        require(digest(source) == digest(copy) == item["sha256_before"]
                == item["sha256_after"] == item["source_sha256_after"], "input hash mismatch")
        source_files[item["name"]] = source
        fixture_bindings.append(binding(source))
    messages, scripts = cap["messages"], cap["scripts"]
    require(len(scripts) == 1 and all(m["type"] == "send"
            and m["script_id"] == scripts[0]["script_id"] for m in messages), "probe message error")
    hook = messages[0]["payload"]
    require(hook["operation"] == "hooks-installed" and hook["pid"] == run["pid"],
            "hook identity mismatch")
    events = [message["payload"] for message in messages[1:]]
    require([event["sequence"] for event in events] == list(range(1, len(events) + 1)),
            "event sequence gaps")
    require(not any(event["operation"] == "probe-error" for event in events), "probe failure")
    return evidence, run, events, source_files, {
        "capture": binding(path), "run_record": binding(run_path),
        "evidence_id": evidence["evidence_id"], "pid": run["pid"],
        "probe": binding(script), "probe_template": binding(template),
        "pid_record": binding(directory.with_suffix(".pid.json")),
        "fixtures": fixture_bindings, "executable_sha256": EXE_SHA,
    }


MATERIAL_FIELDS = ('id','name_hex','field_0x4','field_0x8','field_0x17e8',
    'mode_0x1cc8','reference_count_0x1ccc','reference_ids','field_0x1e40',
    'field_0x2150','field_0x2154','field_0x2158','field_0x215c')


def material_list(value, stable=True):
    records=value['records'];count=len(records)
    require(value['count']==count<=32 and len({r['address']for r in records})==count,
            'bounded material list/count/unique addresses')
    if stable:require(value['root_count']==count,'root/list material count mismatch')
    require(value['head']==(records[0]['address']if count else '0x0')
            and value['tail']==(records[-1]['address']if count else '0x0'), 'material endpoints')
    require(int(value['address'],16)==int(value['owner'],16)+0x1328,'material list owner')
    require(len({r['id']for r in records})==count,'unique material IDs')
    for index,record in enumerate(records):
        require(int(record['address'],16)>0 and record['owner']==value['owner']
                and record['next']==(records[index+1]['address']if index+1<count else '0x0'),
                'material record linkage/ownership')
        name=bytes.fromhex(record['name_hex'])
        require(name.isascii() and b'\0'not in name and len(name)==record['name_length']<=64
                and record['name_terminator_hex']=='00','bounded terminated material name')
        allocation=record['reference_allocation']
        capacity=allocation['capacity']; pointer=record['reference_pointer_0x1cd0']
        require(allocation['pointer']==pointer and type(capacity)is int and 0<=capacity<=128,
                'bounded physical reference allocation')
        require((pointer!='0x0')==(capacity>0),'reference pointer/physical capacity')
        require(pointer=='0x0' or allocation['allocation_observed'],
                'nonnull reference extent established by allocation observation')
        values=values_from_hex(allocation['values_hex'],True)
        require(len(values)==capacity and values==allocation['values'], 'reference allocation raw int32 values')
        logical_count=record['reference_count_0x1ccc']
        require(type(logical_count)is int and -(2**31)<=logical_count<2**31,
                'signed logical reference count')
        require(max(logical_count,0)<=capacity and record['reference_ids']==values[:max(logical_count,0)],
                'logical reference prefix within independently observed allocation')
        require(record['mode_0x1cc8']in (0,1,2),'selected material mode domain')
        for key in MATERIAL_FIELDS:
            if key not in ('id','name_hex','reference_ids','mode_0x1cc8','reference_count_0x1ccc',
                           'field_0x2150','field_0x2154','field_0x2158','field_0x215c'):
                require(record[key]==(1 if key in ('field_0x2158','field_0x215c')else 0),
                        'selected default material field '+key)
    scratch=value['scratch']
    if scratch['allocation_observed']:
        require(0<=scratch['count']<=32 and len(scratch['ids'])==len(scratch['ranks'])==scratch['count'],
                'observed scratch extent')
        require(scratch['ranks']==[sum(other<identifier for other in scratch['ids'])
                                   for identifier in scratch['ids']], 'old material rank map')
    else:
        require(scratch['ids_pointer']==scratch['ranks_pointer']=='0x0'
                and scratch['ids']is None and scratch['ranks']is None,'cleared scratch metadata')
    result=[]
    for record in records:
        selected={key:record[key]for key in MATERIAL_FIELDS}
        allocation=record['reference_allocation']
        selected['reference_allocation']={'allocated':allocation['pointer']!='0x0',
            'capacity':allocation['capacity'],'values':allocation['values']}
        result.append(selected)
    return result


def normalize_user_fields(native):
    output={}
    for kind,field,prefix in [('nodes','user_node_width_0x3398','node'),('elements','user_element_width_0x33a8','element')]:
        record=native['user_field_metadata'][kind]
        require(record['allocation_observed'] and record['capacity']is not None,'user metadata physical allocation observed')
        count=native[field];require(record['logical_count']==count and 0<=count<=8,'user metadata logical width')
        names=record['names'];require(len(names)==record['capacity']and record['capacity']==count,'stable user metadata capacity/count')
        require(len({name['pointer']for name in names})==len(names),'unique user metadata record allocations')
        for name in names:
            raw=bytes.fromhex(name['bytes_hex']);require(len(raw)==64 and 0 in raw,'initialized64byte metadata record')
            require(raw.split(b'\0',1)[0].hex()==name['name_hex'],'user metadata name/fullrecord identity')
        output[prefix+'_count']=count;output[prefix+'_records_hex']=[name['bytes_hex']for name in names]
    return output


def custom_vector(record,integers=False):
    size=4 if integers else 8
    begin,end,capacity=(int(record[key],16)for key in ('pointer','end','capacity_end'))
    require(end-begin==record['physical_items']*size and capacity-begin==record['capacity_items']*size
        and 0<=record['physical_items']<=record['capacity_items']<=128,'custom vector native physical boundaries')
    require(int(record['block'],16)>0 and record['references']>=1,'custom vector live COW block')
    values=column_values(record,integers);require(len(values)==record['physical_items'],'custom vector initialized extent')
    return values


def custom_fields(store,object_address,root_address):
    entries=store['entries'];require(store['count']==len(entries)<=4,'custom collection count')
    require(store['object']==object_address and store['root']==root_address,'custom collection object/root owners')
    require(int(store['container'],16)==int(object_address,16)+0xb58 and store['references']>=1,'custom collection offset/COW owner')
    require(len({entry['custom_id']for entry in entries})==len(entries),'exact unique signed custom IDs')
    previous=store['sentinel'];result=[]
    for index,entry in enumerate(entries):
        require(entry['previous_node']==previous and entry['next_node']==(entries[index+1]['node']if index+1<len(entries)else store['sentinel']),
            'custom linked list order')
        previous=entry['node'];values=custom_vector(entry['storage'])
        require(entry['id']==78 and all(type(value)is int and value>=0 for value in entry['shape'].values()),'custom statevar ID and boundedshape')
        require(len(values)==entry['shape']['count']*entry['shape']['width']*entry['shape']['multiplicity'],'custom shape physical length')
        result.append({key:entry[key]for key in ('custom_id','id','kind','shape','previous')})
        result[-1]['values_hex']=entry['storage']['values_hex']
    return result


def custom_dynamic_arrays(store,object_address):
    entries=store['entries'];require(store['count']==len(entries)<=4,'dynamic collection count')
    require(int(store['container'],16)==int(object_address,16)+0xba8,'dynamic collection offset')
    require(int(store['head'],16)==(int(entries[0]['address'],16)if entries else 0)
        and int(store['tail'],16)==(int(entries[-1]['address'],16)if entries else 0),'dynamic head/tail pointers')
    result=[]
    for index,entry in enumerate(entries):
        require(entry['index']==index+1 and int(entry['next'],16)==(int(entries[index+1]['address'],16)if index+1<len(entries)else 0),
            'dynamic sequential positions/links')
        custom_vector(entry['real_storage']);custom_vector(entry['integer_storage'],True)
        require(entry['integer_0x1c']in (0,1),'dynamic typed discriminator byte')
        result.append({key:entry[key]for key in ('index','type_0x18','integer_0x1c','discriminator_0x20','count_0x24','width_0x28')})
        result[-1].update(mode_0x0c=entry['source_0x0c'],real_values_hex=entry['real_storage']['values_hex'],
            integer_values_hex=entry['integer_storage']['values_hex'])
    return result


class RootIdentity:
    """Combine validated material and geometry projections of persistent roots.

    GeometryRootIdentity's historical empty-material guard is bypassed only in a
    detached validation view. Actual material counts/lists are checked below and
    restored in the expected snapshot; object/control bytes remain untouched.
    Never normalize temporary copy destinations through this persistent tracker.
    """
    def __init__(self):
        self.geometry=GeometryRootIdentity(descriptor_map(read(ROOT/'analysis/mesh-array-contract.json')))
        self.materials={}
        self.custom_identity={}

    def normalize(self,state):
        view=deepcopy(state)
        def geometry_view(root):
            root['material_count']=0
            if root['linked_state']is not None:geometry_view(root['linked_state'])
        geometry_view(view)
        normalized=self.geometry.normalize(view)
        def attach(native,output,generation,linked_output):
            selected=material_list(native['material_list'])
            require(native['material_count']==len(selected),'root material count')
            key=(native['address'],generation)
            tracked=self.materials.setdefault(key,{})
            for record in native['material_list']['records']:
                require(tracked.setdefault(record['id'],record['address'])==record['address'],
                        'persistent material identity within generation')
            output.update(material_count=native['material_count'],materials=selected,user_fields=normalize_user_fields(native),
                custom_last_id_0x33b8=native['custom_last_id_0x33b8'])
            for source,obj in zip(native['objects'],output['objects']):
                require(type(source['material_group_0x3980'])is int,'captured material-group scalar')
                obj['material_group_0x3980']=source['material_group_0x3980']
                for key in ('saved_shape_width_0xadc','saved_texture_width_0xae0'):
                    require(type(source[key])is int and source[key]>=0,'captured saved material shape width')
                    obj[key]=source[key]
                obj['custom_fields']=custom_fields(source['custom_fields'],source['address'],native['address'])
                obj['dynamic_arrays']=custom_dynamic_arrays(source['dynamic_arrays'],source['address'])
                identity_key=(native['address'],generation,source['address'])
                previous=self.custom_identity.setdefault(identity_key,{})
                now={('custom',entry['custom_id']):entry['address']for entry in source['custom_fields']['entries']}
                now.update({('dynamic',entry['index']):entry['address']for entry in source['dynamic_arrays']['entries']})
                for entry,address in now.items():
                    if entry in previous:require(previous[entry]==address,'persistent custom record identity')
                self.custom_identity[identity_key]=now

            if native['linked_state']is not None:
                linked_generation=self.geometry.roots[native['linked_state']['address']]['generation']
                attach(native['linked_state'],linked_output,linked_generation,None)
        attach(state,normalized['state'],normalized['generation'],normalized['linked_state'])
        return normalized


def logical(position,data):
    if position['cursor']==0 and position['count']==0 and position['buffer_hex']=='':return len(data)
    if position['buffer_hex']is None:
        require(position['cursor']==0,'uninitialized stream cursor');return 0
    require(bytes.fromhex(position['buffer_hex'])==data,'bounded complete stream buffer')
    require(0<=position['cursor']<=len(data),'bounded stream cursor')
    return position['cursor']


def retained_imports(events,order,source_files):
    identities=RootIdentity();imports=[];current=None;last=None;handler=None;stack=[]
    material_pairs={'material-append','material-scratch','material-refresh','material-layout',
                    'material-clear','material-destroy','material-copy','material-copy-destroy',
                    'material-reference-free','material-reference-finalize'}
    allowed={'import-before','import-after','keyword-resolved','handler-before','handler-after',
             'finalize-before','finalize-after','stage-before','stage-after','flags-reset-before',
             'flags-reset-after','stream-scan','material-selector','material-name','material-lookup','material-reference-count','material-reference-scan',
             'material-reference-allocate','material-reference-lookup-before','material-reference-feature'}
    allowed.update(pair+'-'+side for pair in material_pairs for side in ('before','after'))
    allowed.update(('column-wrapper-before','column-wrapper-after','column-map-after',
        'column-reader-before','column-reader-after','column-rows-before','column-rows-after',
        'column-scan','column-line','group-reader-before','group-reader-after',
        'group-rows-before','group-rows-after','group-postprocess-before','group-postprocess-after'))
    allowed.update(('object-selected','allocation-update','allocation-free','object-stage-before',
        'object-stage-after','geometry-row','geometry-flag-before','geometry-flag-after','geometry-access',
        'geometry-replace-before','geometry-replace-after','geometry-classification-allocated','geometry-normal',
        'segment-reader-before','segment-reader-after','segment-scan','segment-allocated','segment-clear-before',
        'segment-clear-after','segment-matrix-before','segment-matrix-after','real-row','mixed-row',
        'header-row','selection-row','step-object-lookup'))
    allowed.update(('field-reader-before','field-reader-after','field-rows-before','field-rows-after',
        'field-scan','field-line','field-postprocess'))
    allowed.update(('dynamic-header','dynamic-allocation','dynamic-free','dynamic-scan','dynamic-line','dynamic-property-guard'))
    allowed.update('dynamic-'+kind+'-'+side for kind in ('reader','rows','getter','width','metadata','metadata-clear','property')for side in ('before','after'))
    allowed.update(('custom-header','custom-token-header','custom-token-access','custom-token-integer','custom-token-real-input',
        'custom-lookup','custom-created','custom-dynamic-cast','custom-scan','custom-line'))
    allowed.update('custom-'+kind+'-'+side for kind in ('configure','default','rows','root-update','dynamic-lookup',
        'dynamic-configure','dynamic-default','typed-row','store-destroy','dynamic-clear')for side in ('before','after'))
    allowed.update(('material-shape-producer-header','material-shape-header-scan','material-shape-scan'))
    allowed.update('material-shape-'+kind+'-'+side for kind in ('query','reader','rows','storage')for side in ('before','after'))
    supported={(92,4),(116,4),(127,4),(128,4),(210,2),(211,2),(28,1),(34,1),(38,1),(48,1),(49,1),(50,1),
               (144,2),(145,2),(153,2),(158,2),(159,2),(173,2),(174,2),(205,2),(216,2),(305,0)}
    supported.update((identifier,2)for identifier in field_definitions())
    supported.update((identifier,2)for identifier in (170,196,197,254,255,263,198,266))
    for event in events:
        op=event['operation']
        require(op in allowed,'event outside selected material capture scope: '+op)
        if op=='import-before':
            require(current is None,'nested import')
            normalized=identities.normalize(event['state'])
            require(last is None or normalized==last,'state changed between imports')
            current={'file':event['file'],'before':normalized,'start':event['sequence'],
                     'tokens':[],'handlers':[],'observations':[],'stages':[],'resets':[],
                     'rows':[],'header_rows':[],'mixed_rows':[],'geometry_rows':[],'segment_events':[]}
        require(current is not None and event['file']==current['file'],'event outside import')
        data=source_files[event['file']].read_bytes().replace(b'\r\n',b'\n')
        current['observations'].append(event)
        if op.endswith(('-before','-after'))and op.rsplit('-',1)[0]in material_pairs:
            pair=op.rsplit('-',1)[0]
            if op.endswith('-before'):stack.append((pair,event))
            else:
                require(stack and stack[-1][0]==pair,'material lifecycle event nesting')
                before=stack.pop()[1]
                if pair=='material-scratch':require(before['mode']==event['mode'],'scratch paired mode')
                if pair=='material-refresh':
                    require(handler is not None and handler['entry']['id']in (92,116,127,128,28,34,196,197),'selected refresh caller')
                    option=1 if handler['entry']['id']in (92,116,127,128) else 0
                    require(before['option']==event['option']==option,'refresh option by MTNAME/configuration caller')
        if op=='keyword-resolved':
            require((event['id'],event['category'])in supported,'selected material/object keyword scope')
            current['tokens'].append({'token':event['token'],'id':event['id'],'category':event['category'],
                                      'handler_status':None,'unknown_warning':event['id']==305 and event['token']!='SRFNRG'})
        elif op=='handler-before':
            require(handler is None and (event['id'],event['category'])in supported,'selected handler entry')
            handler={'entry':event,'before':identities.normalize(event['state']),'events':[]}
        elif op=='handler-after':
            require(handler is not None and event['id']==handler['entry']['id']
                    and event['category']==handler['entry']['category'],'paired handler return')
            require(not stack,'unclosed material operation at handler return')
            normalized=identities.normalize(event['state']);state=normalized['state']
            current['tokens'][-1]['handler_status']=event['status']
            snapshot={key:state[key]for key in ('dimension','finalization_controls','time_table',
                'import_flag_0x2f88','import_flag_0x2f89','material_count','materials','components')}
            snapshot.update(id=event['id'],category=event['category'],status=event['status'],
                            cursor=logical(event['stream'],data),state=state,linked_state=normalized['linked_state'])
            handler.update(after=normalized,snapshot=snapshot,exit=event)
            if event['id']==92:verify_handler(handler)
            elif event['id']==116:verify_reference_handler(handler)
            elif event['id']in (127,128):verify_shape_producer(handler)
            current['handlers'].append(handler);handler=None
        elif op=='finalize-before':
            require(handler is None and 'parser_state'not in current,'finalizer entry')
            current['parser_state']=identities.normalize(event['state'])
            current['finalizer_serial']=event['finalizer_serial']
        elif op=='finalize-after':
            require('parser_state'in current and event['finalizer_serial']==current['finalizer_serial'],
                    'paired finalizer')
            current['finalized']=identities.normalize(event['state'])
        elif op in ('stage-before','stage-after','object-stage-before','object-stage-after'):
            require('parser_state'in current and 'finalized'not in current,'stage inside finalizer')
            current['stages'].append((op,event.get('stage',event.get('id'))))
            if not op.startswith('object-'):material_list(event['material_list'])
        elif op.startswith('flags-reset-'):current['resets'].append(event)
        elif op=='import-after':
            require(handler is None and not stack,'complete handler/lifecycle nesting')
            after=identities.normalize(event['state']);require(after==current['finalized'],'import final state')
            stages=[(op,stage)for stage in ('control','materials')for op in ('stage-before','stage-after')]
            stages.append(('stage-before','objects'))
            for obj in current['parser_state']['state']['objects']:
                if obj['type']!=7:stages.extend([('object-stage-before',obj['id']),('object-stage-after',obj['id'])])
            stages.append(('stage-after','objects'))
            require(current['stages']==stages,'ordered finalizer stages and eligible objects')
            require([e['operation']for e in current['resets']]==['flags-reset-before','flags-reset-after'],
                    'wrapper reset pairing')
            require(all(current['resets'][-1][key]==0 for key in ('import_flag_0x2f88','import_flag_0x2f89')),
                    'wrapper resets both flags')
            current.update(status=event['status'],end=event['sequence'])
            imports.append(current);last=after;current=None
        if current is not None:
            row_group={'real-row':'rows','mixed-row':'mixed_rows','header-row':'header_rows',
                       'selection-row':'header_rows','geometry-row':'geometry_rows'}.get(op)
            if row_group:
                event['logical_before']=logical(event['before'],data)
                current[row_group].append(event)
            if op.startswith('segment-'):current['segment_events'].append(event)
        if handler is not None and op not in ('handler-before','handler-after'):
            handler['events'].append(event)
    require(current is None and [x['file']for x in imports]==order,'complete ordered imports')
    return imports


def verify_handler(handler):
    events=handler['events'];selectors=[e for e in events if e['operation']=='material-selector']
    require(len(selectors)==1,'one material selector')
    selector=selectors[0];require(selector['conversions']in (-1,1),'uninitialized matching-failure selector excluded')
    names=[e for e in events if e['operation']=='material-name']
    old=handler['before']['state']['materials'];new=handler['after']['state']['materials']
    if selector['conversions']==-1:
        require(not names and old==new and handler['exit']['status']==2,'selector EOF has no material mutation')
        require(not any(e['operation'].startswith(('material-append','material-refresh'))for e in events),
                'selector EOF precedes material creation');return
    selected=selector['value'];existing=next((r for r in old if r['id']==selected),None)
    all_lookups=[e for e in events if e['operation']=='material-lookup']
    refresh_before=next((e for e in events if e['operation']=='material-refresh-before'),None)
    refresh_after=next((e for e in events if e['operation']=='material-refresh-after'),None)
    lookups=[e for e in all_lookups if refresh_before is None or e['sequence']<refresh_before['sequence']]
    require([e['requested']for e in lookups]==[selected]+([]if existing else[r['id']for r in old]),
            'selector lookup then old-ID rank-map lookups before refresh')
    for lookup in all_lookups[len(lookups):]:
        require(refresh_before is not None and refresh_after is not None
                and refresh_before['sequence']<lookup['sequence']<refresh_after['sequence'],
                'additional MTNAME lookup belongs to reference-aware layout/remap refresh')
        records=refresh_before['state']['material_list']['records']
        resolved=next((record for record in records if record['id']==lookup['requested']),None)
        require(lookup['result']==('0x0'if resolved is None else resolved['address']),
                'reference-aware refresh lookup matches current exact-ID list')
    native_old=handler['entry']['state']['material_list']['records']
    for event,record in zip(lookups[1:],native_old):
        require(event['result']==record['address'],'rank-map lookup retains existing record identity')
    require((lookups[0]['result']=='0x0')==(existing is None),'lookup presence matches prior list')
    native_target=next(r for r in handler['exit']['state']['material_list']['records']if r['id']==selected)
    if existing:
        require(lookups[0]['result']==native_target['address'],'existing material address reused')
    require(len(names)==1,'one name helper after valid selector')
    require(int(names[0]['destination'],16)==int(native_target['address'],16)+12,'name destination material+12')
    require(handler['exit']['status']==(0 if names[0]['status']==0 else 2),'name status remapping')
    require([r['id']for r in new]==[r['id']for r in old]+([]if existing else[selected]),
            'exact-ID append without placeholders')
    target=next(r for r in new if r['id']==selected)
    require(target['name_hex']==names[0]['name']['name_hex'],'name helper result reaches material')
    require(names[0]['previous_name']['name_hex']==(existing['name_hex']if existing else ''),
            'name helper previous contents')
    for record in old:
        if record['id']!=selected:require(record in new,'unselected material retained')
    operations=[e['operation']for e in events]
    for op in ('material-append-before','material-append-after','material-refresh-before',
               'material-refresh-after','material-layout-before','material-layout-after'):
        require(operations.count(op)==(0 if existing else 1),'creation versus rename '+op)
    if not existing:
        require(operations.index('material-append-after')<operations.index('material-refresh-before')
                <operations.index('material-refresh-after')<operations.index('material-name'),
                'creation/refresh precedes name read')
        scratches=[e for e in events if e['operation']=='material-scratch-after']
        require([e['mode']for e in scratches]==[0,1],'scratch snapshot then release')
        require(scratches[0]['state']['scratch']['ids']==[r['id']for r in old], 'scratch old IDs')
        for event in scratches:material_list(event['state'])
        appended=next(e for e in events if e['operation']=='material-append-after')
        require(appended['result']==native_target['address']and appended['state']['count']==len(old)+1
                and appended['state']['root_count']==len(old),'append precedes separate root-count store')
        fresh=appended['state']['records'][-1]
        require(fresh['id']==0 and fresh['name_hex']=='','constructor defaults precede selector/name assignment')
        refreshed=next(e for e in events if e['operation']=='material-refresh-after')['state']
        require(refreshed['material_count']==len(new)and refreshed['components']==handler['after']['state']['components'],
                'creation refresh result persists through name read')
    handler['creation']=existing is None


def verify_reference_handler(handler):
    events=handler['events'];old=handler['before']['state']['materials'];new=handler['after']['state']['materials']
    selectors=[e for e in events if e['operation']=='material-selector']
    require(len(selectors)==1 and selectors[0]['conversions']in (-1,1),'one initialized MSTMTR selector')
    rows=[e for e in events if e['operation']=='material-reference-count']
    scans=[e for e in events if e['operation']=='material-reference-scan']
    allocations=[e for e in events if e['operation']=='material-reference-allocate']
    selector=selectors[0]
    if selector['conversions']==-1:
        require(not rows and not scans and not allocations and old==new and handler['exit']['status']==2,
                'MSTMTR selector EOF precedes mutation')
        return
    selected=selector['value'];existing=next((r for r in old if r['id']==selected),None)
    require([r['id']for r in new]==[r['id']for r in old]+([]if existing else[selected]),
            'MSTMTR exact selector creation order')
    handler['creation']=existing is None
    target=next(r for r in new if r['id']==selected)
    native=next(r for r in handler['exit']['state']['material_list']['records']if r['id']==selected)
    require(len(rows)==1 and rows[0]['material']==native['address'],'one count helper writes selected material')
    row=rows[0];prior=row['previous']
    require(row['output_hex']==struct.pack('<i',row['value']).hex() and target['reference_count_0x1ccc']==row['value'],
            'direct count helper output retained')
    operations=[e['operation']for e in events]
    for operation in ('material-append-before','material-append-after','material-refresh-before','material-refresh-after'):
        require(operations.count(operation)==int(existing is None),'MSTMTR creation versus reuse '+operation)
    if existing is None:
        require(operations.index('material-append-after')<operations.index('material-refresh-before')
                <operations.index('material-refresh-after')<operations.index('material-reference-count'),
                'MSTMTR creation refresh before direct count read')
    require(target['name_hex']==prior['name_hex'],'MSTMTR preserves selected name')
    if row['status']!=0:
        require(handler['exit']['status']==2 and not allocations and not scans,
                'failed count returns before mode/allocation stage')
        require(target['mode_0x1cc8']==prior['mode_0x1cc8']
                and target['reference_allocation']['values']==prior['reference_allocation']['values']
                and native['reference_pointer_0x1cd0']==prior['reference_pointer_0x1cd0'],
                'failed count retains previous mode/physical allocation')
    elif row['value']>0:
        count=row['value']
        require(len(allocations)==1 and allocations[0]['material']==native['address']
                and allocations[0]['capacity']==count and allocations[0]['width']==4
                and allocations[0]['pointer']==native['reference_pointer_0x1cd0']
                and allocations[0]['values']==[0]*count
                and allocations[0]['values_hex']=='00000000'*count,'positive count calloc extent/defaults')
        require(target['mode_0x1cc8']==1 and 1<=len(scans)<=count,'positive count mode and bounded scan attempts')
        values=[];failed=False;previous_value=None
        for scan in scans:
            require(not failed and scan['material']==native['address'] and scan['format']=='%Flf'
                    and scan['conversions']in (-1,0,1),'reference scalar conversion status')
            if scan['conversions']==-1:
                failed=True
            else:
                if scan['conversions']==1:
                    value=struct.unpack('<d',bytes.fromhex(scan['output_hex']))[0]
                    previous_value=value
                else:
                    require(previous_value is not None and scan['output_hex']is None,
                            'matching failure only after initialized same-handler scalar')
                    value=previous_value
                require(math.isfinite(value) and -(2**31)<=math.trunc(value)<2**31,
                        'finite int32 truncation domain')
                values.append(math.trunc(value))
        require(failed or len(scans)==count,'all positive references read unless EOF')
        require(handler['exit']['status']==(2 if failed else 0),'reference EOF status')
        require(target['reference_ids']==values+[0]*(count-len(values)),
                'converted prefix and untouched calloc suffix')
    else:
        require(not allocations and not scans and handler['exit']['status']==0,'nonpositive count no allocation/scans')
        require(target['reference_allocation']['values']==prior['reference_allocation']['values']
                and native['reference_pointer_0x1cd0']==prior['reference_pointer_0x1cd0'],
                'nonpositive count retains complete old allocation')
        clear=prior['mode_0x1cc8']==1 and prior['reference_pointer_0x1cd0']!='0x0' and prior['reference_count_0x1ccc']>0
        require(target['mode_0x1cc8']==(0 if clear else prior['mode_0x1cc8']),
                'nonpositive count clear guard')
        if clear:
            for identifier in prior['reference_ids']:
                resolved=next((record for record in new if record['id']==identifier),None)
                if resolved is not None:require(resolved['mode_0x1cc8']==0,'old reference target cleared immediately')


def verify_reference_lifecycle(events):
    live={};pending_free={};finalizer=None;finalizations=[];features=[]
    for event in events:
        op=event['operation']
        if op=='material-reference-allocate':
            pointer=event['pointer'];capacity=event['capacity']
            require(pointer!='0x0' and pointer not in live and 0<capacity<=128 and event['width']==4
                    and event['call_return_rva']==0x3883ee,'reference allocation identity/callsite')
            require(event['values']==[0]*capacity and event['values_hex']=='00000000'*capacity,
                    'observed calloc initializes all reference storage')
            live[pointer]=event
        elif op=='material-reference-free-before':
            pointer=event['pointer'];allocation=live.pop(pointer,None)
            require(allocation is not None and pointer not in pending_free,'free known live reference allocation')
            require(all(event[key]==allocation[key]for key in ('serial','capacity','material')),
                    'reference free preserves allocation identity')
            require(values_from_hex(event['values_hex'],True)==event['values']
                    and len(event['values'])==event['capacity'],'free observes bounded reference bytes')
            pending_free[pointer]=event
        elif op=='material-reference-free-after':
            before=pending_free.pop(event['pointer'],None)
            require(before is not None and all(event[key]==before[key]for key in
                ('serial','capacity','material','call_return_rva')),'paired reference release')
        elif op=='material-reference-feature':
            records=material_list(event['state'])
            require(event['feature']in (49,50,51,52) and event['result']==int(
                event['control_0x85c']!=0 or any(r['mode_0x1cc8']==1 for r in records)),
                'observed material feature query')
            features.append({key:event[key]for key in ('file','sequence','feature','result','control_0x85c')})
        elif op=='material-reference-finalize-before':
            require(finalizer is None,'non-nested material reference finalizer')
            original=material_list(event['state']);working=deepcopy(original);expected_lookups=[]
            for source in working:
                if source['mode_0x1cc8']!=1:continue
                index=0
                while index<source['reference_count_0x1ccc']:
                    identifier=source['reference_allocation']['values'][index]
                    expected_lookups.append({'id':identifier,'before':deepcopy(working)})
                    target=next((r for r in working if r['id']==identifier),None)
                    if target is not None and target['mode_0x1cc8']!=2:
                        if target['reference_count_0x1ccc']>0:
                            target['reference_count_0x1ccc']=0;target['reference_ids']=[]
                            target['reference_allocation']={'allocated':False,'capacity':0,'values':[]}
                        target['mode_0x1cc8']=2
                    index+=1
            finalizer={'file':event['file'],'start':event['sequence'],'before':original,
                       'expected':working,'lookups':expected_lookups,'observed':[]}
        elif op=='material-reference-lookup-before':
            require(finalizer is not None,'reference lookup inside finalizer')
            index=len(finalizer['observed']);require(index<len(finalizer['lookups']),'extra reference lookup')
            expected=finalizer['lookups'][index]
            require(event['requested']==expected['id'] and material_list(event['state'])==expected['before'],
                    'insertion-order immediate reference mutation before each lookup')
            finalizer['observed'].append(event['requested'])
        elif op=='material-reference-finalize-after':
            require(finalizer is not None and len(finalizer['observed'])==len(finalizer['lookups']),
                    'complete reference lookup iteration')
            after=material_list(event['state'])
            require(after==finalizer['expected'],'material finalizer immediate mutation and retained allocation contract')
            finalizations.append({'file':event['file'],'sequence_range':[finalizer['start'],event['sequence']],
                'before':finalizer['before'],'after':after,'lookup_ids':finalizer['observed']})
            finalizer=None
    require(not pending_free and finalizer is None,'complete reference free/finalizer events')
    return {'finalizations':finalizations,'features':features,'allocation_count':sum(
        event['operation']=='material-reference-allocate'for event in events),'remaining_observed_allocations':len(live),
        'scope':'Original allocation provenance, ordered mutable lookup semantics and query49..52; expected replay state remains captured original snapshots.'}


def compare_reference_readers(events,source_files,driver):
    commands=[];expected=[];metadata=[];prior_hex=None
    for event in events:
        op=event['operation']
        if op not in ('material-reference-count','material-reference-scan'):continue
        data=source_files[event['file']].read_bytes().replace(b'\r\n',b'\n')
        start,end=logical(event['before'],data),logical(event['after'],data)
        require(start<=end,'monotonic reference reader cursor')
        lines=[];previous='-'
        if op=='material-reference-count':
            prior_hex=None
            require(event['skip']==0 and len(event['lines'])==1,'count helper one line and no blank skipping')
            line=event['lines'][0];raw=b'' if line['eof']else bytes.fromhex(line['hex'])
            require(line['limit']==1024 and raw==data[start:end],'count helper exact fixture bytes')
            require(len(event['scans'])==(0 if line['eof']else 1),'count scan attempt count')
            if event['scans']:
                scan=event['scans'][0]
                require(scan['format']=='%i' and bytes.fromhex(scan['input_hex'])==raw.replace(b',',b' '),
                        'count scan format/comma normalization')
            kind='int1'
            wanted={'kind':kind,'status':event['status'],
                'conversions':event['scans'][0]['conversions']if event['scans']else -1,'values':[event['value']]}
            lines=[{'offset':start,'cursor':end,'hex':raw.hex(),'status':int(line['eof'])}]
        else:
            require(event['format']=='%Flf' and event['conversions']in (-1,0,1),'bounded reference scalar scan')
            kind='streamreal';previous=prior_hex or '-'
            wanted={'kind':kind,'status':int(event['conversions']==-1),'conversions':event['conversions']}
            if event['conversions']==1:
                value=struct.unpack('<d',bytes.fromhex(event['output_hex']))[0]
                require(math.isfinite(value) and value==event['value'],'raw successful scalar value')
                wanted['values']=[value];prior_hex=event['output_hex']
            else:
                require(event['output_hex']is None and event['value']is None,'failed scan has no newly assigned output')
                if event['conversions']==0:
                    require(prior_hex is not None,'matching failure requires prior successful same-handler scalar')
                    wanted['values']=[struct.unpack('<d',bytes.fromhex(prior_hex))[0]]
        wanted.update(cursor=end,consumed_hex=data[start:end].hex(),lines=lines)
        commands.append(f'{kind} 0 {start} {data.hex()or"-"} {previous}')
        expected.append(wanted);metadata.append({'file':event['file'],'sequence':event['sequence'],'operation':op})
    actual=replay(driver,commands);require(len(actual)==len(expected),'reference reader replay count')
    rows=[]
    for got,wanted,meta in zip(actual,expected,metadata):
        differences=[];differences_at(got,wanted,'reference_reader',differences)
        rows.append({**meta,'original':wanted,'differences':differences,'matched':not differences})
    return {'status':'pass'if all(row['matched']for row in rows)else'fail','calls':rows,'count':len(rows),
        'scope':'Count helper bytes/conversions/direct outputs and scalar successful raw outputs; later conversion0 uses proven same-handler prior assignment. EOF uninitialized native local is deliberately excluded.'}


def replay(driver,commands,expected_stderr=""):
    process=subprocess.run([str(driver)],input='\n'.join(commands)+'\n',text=True,
        capture_output=True,timeout=30,creationflags=subprocess.CREATE_NO_WINDOW)
    require(process.returncode==0 and process.stderr==expected_stderr,'replacement replay stderr/status differs: '+process.stderr)
    return [json.loads(line,parse_int=lambda text:-0.0 if text=='-0'else int(text))
            for line in process.stdout.splitlines()]


def copy_object_projection(obj,temporary):
    """Validate captured extents without asserting temporary/source equivalence."""
    descriptors=obj['descriptors'];rows=descriptor_map(read(ROOT/'analysis/mesh-array-contract.json'))
    require(len(descriptors)==len(rows)==53,'complete copy descriptor inventory')
    require(len({d['address']for d in descriptors})==53,'unique copy descriptors')
    normalized=[]
    for descriptor,row in zip(descriptors,rows):
        require(descriptor['id']==row['state_id']
                and descriptor['slot_offset']==int(row['slot_relative_to_object'],16),'copy descriptor registration')
        n=descriptor['length']
        require(n==descriptor['count']*descriptor['width']*descriptor['multiplicity']
                and 0<=n<=512 and descriptor['storage_references']>=1,'copy descriptor extent/ownership')
        values=values_from_hex(descriptor['values_hex'])
        require(len(values)==n and all(math.isfinite(x)for x in values),'copy descriptor finite raw values')
        normalized.append({key:descriptor[key]for key in ('id','slot_offset','count','width','multiplicity',
             'old_count','old_width','old_multiplicity','length','values_hex')})
    buffers={}
    for name,buffer in obj['buffer_metadata'].items():
        extent,prefix=buffer['allocated_items'],buffer['known_prefix']
        require(type(extent)is int and type(prefix)is int and 0<=prefix<=extent<=512
                and buffer['unknown_items']==extent-prefix,'copy direct allocation extent/prefix')
        values=values_from_hex(buffer['known_hex'],name!='coordinates')
        require(len(values)==prefix and all(math.isfinite(x)for x in values),'copy direct known bytes')
        buffers[name]={key:buffer[key]for key in ('allocated_items','known_prefix','unknown_items','known_hex')}
    projection={key:obj[key]for key in ('id','type','mesh_flag','nodes','elements','arity',
        'node_allocation_count','element_allocation_count','allocation_arity','flag_0x3990','material_group_0x3980')}
    projection.update(descriptors=normalized,buffers=buffers)
    if temporary:
        require(obj['geometry_payload_omitted']is True,'temporary geometry scope marker')
        projection.update({key:obj[key]for key in ('name_hex','flag_0x3994','boundary_group_count','boundary_total')})
    else:
        projection.update(name_hex=obj['name'].encode('ascii').hex(),stroke_hex=obj['stroke_hex'],
                          secondary=obj['secondary'],element_kind=obj['element_kind'],
                          geometry=normalize_geometry(obj['geometry']),
                          geometry_segments=normalize_segments(obj['geometry_segments']))
    return projection


def verify_temporary_copies(events):
    refreshes=[];active=[];copies={};finished=[]
    for event in events:
        op=event['operation']
        if op=='material-refresh-before':
            active.append({'before':event,'copies':[]})
        elif op=='material-copy-before':
            require(active,'copy outside material refresh')
            context=active[-1];root=context['before']['state'];source=event['source'];dest=event['destination']
            persistent={obj['address']:obj for obj in root['objects']}
            require(event['root']==root['address']and source['address']in persistent
                    and source['type']!=7 and dest['address']not in persistent,
                    'copy active persistent source and separate temporary destination')
            require(dest['owner']==root['address']and int(dest['address'],16)>0,'temporary owner root')
            require(len(event['flags'])==6 and all(flag in (0,1)for flag in event['flags'])
                    and event['components']==root['components'],'copy flags and old component table')
            serial=event['copy_serial'];require(serial not in copies,'unique copy serial')
            copies[serial]={'before':event,'source_projection':copy_object_projection(source,False),
                            'temporary_before':copy_object_projection(dest,True)}
            context['copies'].append(serial)
        elif op=='material-copy-after':
            copy=copies[event['copy_serial']];before=copy['before']
            require('after'not in copy and event['root']==before['root']and event['flags']==before['flags']
                    and event['components']==before['components']
                    and event['source']['address']==before['source']['address']
                    and event['destination']['address']==before['destination']['address'],'paired copy identity')
            require(copy_object_projection(event['source'],False)==copy['source_projection'],
                    'copy preserves selected persistent source values/history')
            copy['temporary_after']=copy_object_projection(event['destination'],True);copy['after']=event
        elif op=='material-copy-destroy-before':
            copy=copies[event['copy_serial']];require('after'in copy and 'destroy_before'not in copy,'copy destruction timing')
            require(event['destination']['address']==copy['before']['destination']['address']
                    and event['destination']['id']==copy['before']['source']['id'],
                    'caller assigns source ID before temporary destruction')
            copy['temporary_destroy']=copy_object_projection(event['destination'],True);copy['destroy_before']=event
        elif op=='material-copy-destroy-after':
            copy=copies[event['copy_serial']]
            require('destroy_before'in copy and 'destroy_after'not in copy
                    and event['address']==copy['before']['destination']['address'],'paired copy destruction address')
            copy['destroy_after']=event
            finished.append({'file':event['file'],'copy_serial':event['copy_serial'],
                'source_id':copy['before']['source']['id'],'flags':copy['before']['flags'],
                'sequence_range':[copy['before']['sequence'],event['sequence']],
                'persistent_source_unchanged':True,'temporary_before':copy['temporary_before'],
                'temporary_after':copy['temporary_after'],'temporary_destroy':copy['temporary_destroy']})
        elif op=='material-refresh-after':
            require(active,'refresh return without entry');context=active.pop()
            require(all('destroy_after'in copies[serial]for serial in context['copies']),
                    'temporary copies destroyed before refresh returns')
            refreshes.append({'file':event['file'],'sequence_range':[context['before']['sequence'],event['sequence']],
                              'copy_serials':context['copies']})
    require(not active and len(finished)==len(copies),'complete temporary lifetimes')
    return {'copies':finished,'count':len(finished),'refreshes':refreshes,
            'scope':'Temporary destination metadata/known bytes validated separately; no replacement temporary allocations are required.'}


def compare_readers(events,source_files,driver):
    commands=[];expected=[];metadata=[]
    for event in events:
        op=event['operation']
        if op not in ('material-name','material-selector','stream-scan'):continue
        data=source_files[event['file']].read_bytes().replace(b'\r\n',b'\n')
        start,end=logical(event['before'],data),logical(event['after'],data)
        require(start<=end,'monotonic reader cursor')
        previous='-';lines=[]
        if op=='material-name':
            require(event['capacity']==65 and not event['scans'],'material name helper capacity/no scan')
            kind='materialname';previous=event['previous_name']['name_hex']or'-';cursor=start
            for line in event['lines']:
                require(line['limit']==1024,'material name line capacity')
                raw=bytes.fromhex(line['hex'])if not line['eof']else b''
                lines.append({'offset':cursor,'cursor':cursor+len(raw),'hex':raw.hex(),'status':int(line['eof'])})
                cursor+=len(raw)
            require(cursor==end,'name fgets bytes account for cursor')
            wanted={'kind':kind,'status':event['status'],'conversions':None,'values':[],
                    'name_hex':event['name']['name_hex']}
        else:
            require(event['format']in ('%i','%39s'),'selected stream helper format')
            kind='selector'if event['format']=='%i'else'token'
            if kind=='selector':
                require(event['conversions']in (-1,1),'selector matching failure excluded')
                wanted={'kind':kind,'status':int(event['conversions']==-1),'conversions':event['conversions'],
                        'values':[event['value']if event['conversions']==1 else 0]}
                if op=='material-selector'and event['conversions']==1:
                    require(event['output_hex']==struct.pack('<i',event['value']).hex(),'raw selector bytes')
            else:
                wanted={'kind':kind,'status':int(event['conversions']==-1),'conversions':None,
                        'values':[],'token_hex':(event['value']or'').encode('ascii').hex()}
        wanted.update(cursor=end,consumed_hex=data[start:end].hex(),lines=lines)
        commands.append(f'{kind} 0 {start} {data.hex()or"-"} {previous}')
        expected.append(wanted);metadata.append({'file':event['file'],'sequence':event['sequence'],'operation':op})
    actual=replay(driver,commands);require(len(actual)==len(expected),'reader replay result count')
    rows=[]
    for got,wanted,meta in zip(actual,expected,metadata):
        differences=[];differences_at(got,wanted,'reader',differences)
        rows.append({**meta,'original':wanted,'differences':differences,'matched':not differences})
    return {'status':'pass'if all(row['matched']for row in rows)else'fail','calls':rows,'count':len(rows)}


def column_values(event, integers=False):
    values=values_from_hex(event['values_hex'],integers)
    # Frida JSON.stringify renders a numeric -0 as0. Only this redundant
    # numeric view may lose the sign; raw bytes remain the comparison oracle.
    require(len(event['values'])==len(values),'column numeric/raw extent')
    differences=[]
    for index,(observed,value) in enumerate(zip(event['values'],values)):
        if not integers and type(observed)in (int,float) and observed==value==0:continue
        differences_at(observed,value,f'captured_values[{index}]',differences)
    require(not differences and all(math.isfinite(value)for value in values),'column buffer numeric/raw identity')
    return values


def column_pairs(events):
    names={'column-reader','column-rows','group-reader','group-rows'}
    stack=[];pairs={}
    for event in events:
        op=event['operation'];pair=op.rsplit('-',1)[0]
        if pair in names and op.endswith('-before'):
            serial=event['serial'];require(serial not in pairs,'unique column call serial')
            if 'parent_serial'in event:
                require(stack and event['parent_serial']==stack[-1]['before']['serial'],'nested detailed row call')
            else:require(not stack,'non-nested top column helper')
            call={'before':event,'events':[]};pairs[serial]=call;stack.append(call)
        elif pair in names and op.endswith('-after'):
            require(stack and stack[-1]['before']['serial']==event['serial']
                    and stack[-1]['before']['operation']==pair+'-before','paired column helper exit')
            call=stack.pop();call['after']=event
            for key in ('kind','element_count','input_width','width','mode','declared_rows','explicit_width',
                        'object','parent_serial','wrapper_serial'):
                if key in call['before']:require(event[key]==call['before'][key],'stable column helper argument '+key)
        elif op in ('column-scan','column-line'):
            require(stack and event['reader_serial']==stack[-1]['before']['serial'],'scalar/line in innermost column helper')
            stack[-1]['events'].append(event)
    require(not stack and all('after'in call for call in pairs.values()),'complete column helper nesting')
    return pairs


def simulate_observed_rows(call, integers):
    before,after=call['before'],call['after'];width=before.get('input_width',before.get('width'))
    explicit=before.get('explicit_width',0);row_width=explicit or width
    declared=before['declared_rows'];count=before['element_count']
    require(0<=declared<=8 and 0<=row_width<=10 and 0<=count<=8,'bounded observed row reader')
    values=column_values(before,integers);require(len(values)==count*width,'detailed row scratch extent')
    implicit=integers or before['kind']=='implicit'
    buffer=[] if implicit and declared<1 else [0 if integers else 0.0]*row_width
    scans=[event for event in call['events']if event['operation']=='column-scan']
    lines=[event for event in call['events']if event['operation']=='column-line']
    row_id=None;scan_index=0;completed=0;failed=False
    for _ in range(declared):
        for slot in range(row_width+1):
            require(scan_index<len(scans),'missing detailed scalar scan')
            scan=scans[scan_index];scan_index+=1
            integer=integers or slot==0
            require(scan['format']==('%i'if integer else'%Flf')and scan['conversions']in (-1,0,1),
                    'detailed scalar format/status')
            if scan['conversions']!=1:
                require(scan['output_hex']is None and scan['value']is None,'no invented failed scalar output')
                if scan['conversions']==-1:failed=True;break
                if slot==0:require(row_id is not None,'first failed row ID outside initialized domain')
            else:
                raw=bytes.fromhex(scan['output_hex']);require(len(raw)==(4 if integer else 8),'scalar raw width')
                value=struct.unpack('<i'if integer else'<d',raw)[0]
                require(math.isfinite(value)and value==scan['value'],'successful scalar raw value')
                if slot==0:row_id=value
                else:buffer[slot-1]=value
        if failed:break
        require(row_id is not None,'initialized row ID after completed record')
        if 1<=row_id<=count:
            base=(row_id-1)*width
            values[base:base+min(width,row_width)]=buffer[:min(width,row_width)]
        completed+=1
    require(scan_index==len(scans),'no excess scalar scans')
    require(len(lines)==int(implicit and declared>0 and not failed),'exact trailing fgets branch')
    status=int(failed or bool(lines and lines[0]['eof']))
    require(after['status']==status,'detailed reader failure timing')
    differences=[];differences_at(column_values(after,integers),values,'row_scratch',differences)
    require(not differences,'detailed reader complete-row writes and reused partial buffer')
    return {'row_buffer':buffer,'completed_rows':completed,'row_id':row_id,
            'row_id_initialized':row_id is not None,'declared_rows':declared}


def compare_column_readers(events,source_files,driver):
    pairs=column_pairs(events);commands=[];expected=[];metadata=[]
    for serial,call in pairs.items():
        before,after=call['before'],call['after']
        if 'parent_serial'in before:continue
        integers=before['kind']=='integercolumns';kind='integercolumns'if integers else'materialcolumns'
        count=before['element_count'];width=before.get('input_width',before.get('width'))
        require(0<=count<=4 and 0<=width<=8,'bounded top column helper')
        original=column_values(before,integers);result=column_values(after,integers)
        require(len(original)==len(result)==count*width,'top column buffer extent')
        if not integers:require(all(struct.pack('<d',value)==bytes(8)for value in original),'floating wrapper scratch calloc zeros')
        data=source_files[before['file']].read_bytes().replace(b'\r\n',b'\n')
        start,end=logical(before['stream'],data),logical(after['stream'],data)
        require(start<=end,'top column helper monotonic stream')
        descendants={serial}
        while True:
            extended=descendants|{number for number,item in pairs.items()if item['before'].get('parent_serial')in descendants}
            if extended==descendants:break
            descendants=extended
        line_events=sorted([event for number in descendants for event in pairs[number]['events']
            if event['operation']=='column-line'],key=lambda event:event['sequence'])
        require(line_events and line_events[0]['reader_serial']==serial,'top header fgets first')
        lines=[]
        for event in line_events:
            begin,finish=logical(event['before'],data),logical(event['after'],data)
            raw=b''if event['eof']else bytes.fromhex(event['hex'])
            require(event['limit']==1024 and raw==data[begin:finish] and finish-begin<=1023,'column fgets exact fixture bytes')
            if event['eof']:require(begin==finish==len(data),'fgets EOF location')
            lines.append({'offset':begin,'cursor':finish,'hex':raw.hex(),'status':int(event['eof'])})
        header=line_events[0];scans=after['scans']
        require(len(scans)==(0 if header['eof']else 1),'one column header sscanf when line exists')
        conversions=scans[0]['conversions']if scans else -1
        if scans:
            require(scans[0]['format']==('%i %i'if integers else'%i %i %Flf')
                    and scans[0]['input_hex']==header['hex'],'column header exact format with no comma replacement')
        wanted={'kind':kind,'status':after['status'],'cursor':end,'consumed_hex':data[start:end].hex(),
                'conversions':conversions,'values':result,'lines':lines,
                'default_assigned':conversions>=(2 if integers else 3)}
        children=[pairs[number]for number in descendants if number!=serial]
        if conversions<1:
            require(not children and after['status']==1,'header failure precedes detailed records')
            require(integers is False or conversions==-1,'integer header matching failure is undefined')
            require(result==original,'header failure leaves scratch/destination unchanged')
            wanted.update(declared_rows=0,completed_rows=0,row_id=None,row_id_initialized=False,row_buffer=[])
            if not integers:wanted['explicit_width']=0
        else:
            roots=[item for item in children if item['before']['parent_serial']==serial]
            require(len(roots)==1,'one detailed reader after assigned header count')
            direct=roots[0]['before'];wanted['declared_rows']=direct['declared_rows']
            if not integers:wanted['explicit_width']=direct['explicit_width']
            leaf=[item for item in children if not any(other['before']['parent_serial']==item['before']['serial']for other in children)]
            require(len(leaf)==1,'one deepest row reader')
            wanted.update(simulate_observed_rows(leaf[0],integers))
            for child in children:
                require(child['after']['status']==after['status'],'delegated detailed status propagation')
                require(column_values(child['after'],integers)==result,'detailed scratch reaches top helper')
            initial_rows=column_values(direct,integers)
            if wanted['default_assigned']:
                if initial_rows:
                    wanted['default_value']=initial_rows[0]
                    require(all(struct.pack('<i'if integers else'<d',value)==struct.pack('<i'if integers else'<d',initial_rows[0])for value in initial_rows),
                            'header scalar fills every destination item')
            else:require(initial_rows==original,'missing default leaves original scratch/destination')
        parameter=f'{count}:{width}'+(':'+before['values_hex']if integers else'')
        if integers and not before['values_hex']:parameter+='-'
        commands.append(f'{kind} 0 {start} {data.hex()or"-"} {parameter}')
        expected.append(wanted);metadata.append({'file':before['file'],'serial':serial,'sequence_range':[before['sequence'],after['sequence']]})
    actual=replay(driver,commands);require(len(actual)==len(expected),'column helper replay count')
    rows=[]
    for got,wanted,meta in zip(actual,expected,metadata):
        differences=[];differences_at(got,wanted,'column_reader',differences)
        rows.append({**meta,'original':wanted,'differences':differences,'matched':not differences})
    return {'status':'pass'if all(row['matched']for row in rows)else'fail','count':len(rows),'calls':rows,
        'scope':'Full helper scratch/destination, captured scalar assignments, partial complete-row writes, reused row buffer, byte cursor and all fgets attempts.'}


def verify_column_commits(imports):
    producers=[];groups=[]
    for item in imports:
        for handler in item['handlers']:
            identifier=handler['entry']['id'];events=handler['events']
            if identifier not in (174,205,216):continue
            if identifier==174:
                entries=[event for event in events if event['operation']=='group-reader-before']
                exits=[event for event in events if event['operation']=='group-reader-after']
                pre=[event for event in events if event['operation']=='group-postprocess-before']
                post=[event for event in events if event['operation']=='group-postprocess-after']
                require(len(entries)==len(exits)==len(pre)==len(post)==1,'one group read followed by unconditional postprocess')
                entry,exit,before,after=entries[0],exits[0],pre[0],post[0]
                require(entry['object']==exit['object']==before['object']==after['object']
                        and entry['sequence']<exit['sequence']<before['sequence']<after['sequence'],
                        'group read/postprocess identity and ordering')
                require(handler['exit']['status']==(0 if exit['status']==0 else 2),'group reader status remapping')
                require(before['direct61']==after['direct61'],'group postprocess preserves direct integer buffer')
                values=values_from_hex(before['direct61']['known_hex'],True)
                require(before['direct61']['known_prefix']>=before['element_count'],'known group postprocess extent')
                selected=0
                if before['mesh_flag'] and before['direct61']['pointer']!='0x0':
                    counts=Counter(values[:before['element_count']])
                    if counts:selected=max(counts,key=counts.get)
                expected=selected or before['material_group_0x3980']
                require(after['material_group_0x3980']==expected,'group modal count with first-seen tie and zero retention')
                native=next(obj for obj in handler['exit']['state']['objects']if obj['address']==after['object'])
                require(native['material_group_0x3980']==expected,'group scalar reaches handler state')
                require(column_values(exit,True)==values[:exit['element_count']*exit['width']],
                        'group helper mutations reach postprocess')
                groups.append({'file':item['file'],'sequence_range':[entry['sequence'],after['sequence']],
                    'status':exit['status'],'before_values':column_values(entry,True),'after_values':column_values(exit,True),
                    'before_group':before['material_group_0x3980'],'after_group':expected})
                continue
            entries=[event for event in events if event['operation']=='column-wrapper-before']
            exits=[event for event in events if event['operation']=='column-wrapper-after']
            maps=[event for event in events if event['operation']=='column-map-after']
            readers=[event for event in events if event['operation']=='column-reader-after']
            require(len(entries)==len(exits)==len(maps)==len(readers)==1,'one producer wrapper/map/reader')
            entry,exit,mapping,reader=entries[0],exits[0],maps[0],readers[0]
            require(entry['serial']==exit['serial']==mapping['wrapper_serial']==reader['wrapper_serial']
                    and entry['object']==exit['object']==mapping['object']
                    and entry['root']==exit['root']==mapping['root']
                    and entry['destination']==exit['destination'], 'producer wrapper argument identity')
            require(entry['sequence']<mapping['sequence']<reader['sequence']<exit['sequence'],
                    'mapping then scratch reader then wrapper commit')
            materials=material_list(mapping['materials']);count=len(materials)
            require(mapping['mesh_flag']!=0 and mapping['material_count']==count,'live mesh mapping guard')
            rank={record['id']:position for position,record in enumerate(sorted(materials,key=lambda record:record['id']))}
            marks=[0]*count;last_id=0
            direct=mapping['direct61'];ids=values_from_hex(direct['known_hex'],True)
            require(direct['known_prefix']>=entry['element_count'],'initialized mapping element IDs')
            for material_id in ids[:entry['element_count']]:
                if material_id==last_id:continue
                last_id=material_id;position=rank.get(material_id)
                if position is None or marks[position]!=0:continue
                record=next(record for record in materials if record['id']==material_id)
                if record['mode_0x1cc8']==1:
                    marks[position]=1
                    for target in record['reference_ids']:
                        if target in rank:marks[rank[target]]=2
            columns=[];input_width=0
            for mark in marks:
                columns.append(input_width if mark==2 else -1)
                if mark==2:input_width+=1
            require(mapping['map']==values_from_hex(mapping['map_hex'],True)==columns
                    and mapping['input_width']==reader['input_width']==input_width,
                    'material map follows ordered element IDs and sorted target ranks')
            require(entry['element_count']==exit['element_count']==reader['element_count']
                    and entry['destination_width']==exit['destination_width']<=count,
                    'producer scratch and destination extents')
            require(exit['status']==reader['status']and handler['exit']['status']==(0 if exit['status']==0 else 2),
                    'scratch status reaches wrapper then handler')
            original=column_values(entry);wanted=list(original);scratch=column_values(reader)
            width=entry['destination_width'];rows=entry['element_count']
            require(len(original)==rows*width and len(scratch)==rows*input_width,'producer complete buffers')
            if reader['status']==0:
                for row in range(rows):
                    for column in range(width):
                        if columns[column]>=0:wanted[row*width+column]=scratch[row*input_width+columns[column]]
            differences=[];differences_at(column_values(exit),wanted,'producer_commit',differences)
            require(not differences,'producer success-only mapped commit and unmapped value retention')
            native=next(obj for obj in handler['exit']['state']['objects']if obj['address']==entry['object'])
            state_id=49 if identifier==205 else 50
            descriptor=next(d for d in native['descriptors']if d['id']==state_id)
            require(descriptor['bound_pointer']==entry['destination']and descriptor['width']==width
                    and descriptor['values_hex']==exit['values_hex'],'producer destination reaches descriptor state')
            producers.append({'file':item['file'],'id':identifier,'sequence_range':[entry['sequence'],exit['sequence']],
                'map':columns,'input_width':input_width,'status':exit['status'],'before':original,'scratch':scratch,'after':wanted})
    return {'producers':producers,'groups':groups,
        'scope':'Original-only ordered mapping, modal group postprocess, exact mapped commits, failure retention, and descriptor destination binding.'}


def verify_nonzero_column_remaps(events):
    active=[];results=[]
    for event in events:
        if event['operation']=='material-refresh-before':active.append(event)
        elif event['operation']=='material-refresh-after':
            require(active,'paired material refresh for column preservation');before=active.pop()
            if before['option']!=1:continue
            old=before['state'];new=event['state'];old_width=old['components'][49];width=new['components'][49]
            records=material_list(new['material_list'])
            enabled=any(record['mode_0x1cc8']==1 for record in records)
            scratch=old['material_list']['scratch'];ids=scratch['ids'];ranks=scratch['ranks']
            if not enabled or ids is None or ranks is None:continue
            require(old_width<=len(ids)==len(ranks),'remap old material ranks cover prior columns')
            new_rank={record['id']:position for position,record in enumerate(sorted(records,key=lambda record:record['id']))}
            for original in old['objects']:
                if original['type']==7:continue
                target=next(obj for obj in new['objects']if obj['address']==original['address'])
                count=original['elements']
                for state_id in (49,50):
                    source_descriptor=next(d for d in original['descriptors']if d['id']==state_id)
                    target_descriptor=next(d for d in target['descriptors']if d['id']==state_id)
                    values=values_from_hex(source_descriptor['values_hex']);after=values_from_hex(target_descriptor['values_hex'])
                    require(source_descriptor['width']==old_width and target_descriptor['width']==width,
                            'selected remap descriptor widths')
                    wanted=[0.0]*(count*width)
                    require(len(values)==count*old_width and len(after)==count*width,'remap descriptor extents')
                    for index in range(old_width):
                        material_id=ids[index]
                        if material_id not in new_rank:continue
                        for row in range(count):wanted[row*width+new_rank[material_id]]=values[row*old_width+ranks[index]]
                    differences=[];differences_at(after,wanted,'native_column_remap',differences)
                    require(not differences,'material append remaps old rank columns with zero new slots')
                    results.append({'file':event['file'],'object_id':original['id'],'state_id':state_id,
                        'sequence_range':[before['sequence'],event['sequence']],'old_width':old_width,'width':width,
                        'before':values,'after':after,'nonzero_source':any(value!=0 for value in values)})
    require(not active,'complete material refreshes')
    return {'checks':results,'nonzero_source_checks':sum(row['nonzero_source']for row in results),
        'scope':'Retained original source/destination descriptor49/50 raw values and saved rank arrays; no replacement-generated expected values.'}


def field_definitions():
    inventory=read(ROOT/'analysis/array-keyword-next-inventory.json')
    definitions={}
    for item in inventory['keywords']:
        if not item['next_batch']:continue
        require(len(item['calls'])==1,'one selected field route')
        call=item['calls'][0]
        definitions[item['id']]={'keyword':item['keyword'],'slot':int(call['bound_data_slot'],16),
            'component':call['component_index'],'domain':call['count_domain'],
            'mode':call['reader_mode'],'integers':call['helper']=='0x140419d40'}
    require(len(definitions)==25,'static selected field inventory size')
    return definitions


def field_pairs(events):
    converted=[]
    for event in events:
        op=event['operation']
        if op not in ('field-reader-before','field-reader-after','field-rows-before','field-rows-after',
                      'field-scan','field-line'):continue
        item=dict(event)
        item['operation']=op.replace('field-reader','column-reader').replace('field-rows','column-rows').replace('field-scan','column-scan').replace('field-line','column-line')
        if op.startswith('field-reader'):item['kind']='integercolumns'if item['integers']else'realcolumns'
        converted.append(item)
    return column_pairs(converted)


def compare_field_readers(events,source_files,driver):
    pairs=field_pairs(events);commands=[];wanted_rows=[];metadata=[]
    for serial,call in pairs.items():
        before,after=call['before'],call['after']
        if 'parent_serial'in before:continue
        integers=before['integers'];kind='integercolumns'if integers else'realcolumns'
        count,width=before['element_count'],before['width']
        require(0<=count<=8 and 0<=width<=10,'bounded field helper extent')
        original=column_values(before,integers);result=column_values(after,integers)
        require(len(original)==len(result)==count*width,'field helper exact destination extent')
        data=source_files[before['file']].read_bytes().replace(b'\r\n',b'\n')
        start,end=logical(before['stream'],data),logical(after['stream'],data)
        require(start<=end,'field helper monotonic cursor')
        descendants={serial}
        while True:
            expanded=descendants|{number for number,child in pairs.items()if child['before'].get('parent_serial')in descendants}
            if expanded==descendants:break
            descendants=expanded
        line_events=sorted([event for number in descendants for event in pairs[number]['events']
            if event['operation']=='column-line'],key=lambda event:event['sequence'])
        require(line_events and line_events[0]['reader_serial']==serial,'field header first fgets')
        lines=[]
        for event in line_events:
            begin,finish=logical(event['before'],data),logical(event['after'],data)
            raw=b''if event['eof']else bytes.fromhex(event['hex'])
            require(event['limit']==1024 and raw==data[begin:finish] and finish-begin<=1023,'field fgets fixture bytes')
            if event['eof']:require(begin==finish==len(data),'field fgets EOF position')
            lines.append({'offset':begin,'cursor':finish,'hex':raw.hex(),'status':int(event['eof'])})
        header=line_events[0];scans=after['scans']
        require(len(scans)==(0 if header['eof']else 1),'one field header sscanf')
        conversions=scans[0]['conversions']if scans else -1
        mode=before['mode']
        fmt='%i %i'if integers else('%i %i %Flf'if mode==1 else'%i'+' %Flf'*(10 if mode==6 else 1))
        if scans:require(scans[0]['format']==fmt and scans[0]['input_hex']==header['hex'],'field header exact format/input')
        default_count=max(0,conversions-(2 if not integers and mode==1 else 1))
        wanted={'kind':kind,'status':after['status'],'cursor':end,'consumed_hex':data[start:end].hex(),
            'conversions':conversions,'values':result,'lines':lines,'default_assigned':default_count>0}
        if not integers:wanted['values_hex']=after['values_hex']
        children=[pairs[number]for number in descendants if number!=serial]
        if conversions<1:
            require(not children and after['status']==1,'field header failure before row reader')
            require(not integers or conversions==-1,'undefined integer header matching failure excluded')
            require(after['values_hex']==before['values_hex'],'header failure unchanged destination')
            wanted.update(declared_rows=0,completed_rows=0,row_id=None,row_id_initialized=False,row_buffer=[])
            if not integers:wanted['explicit_width']=0
        else:
            roots=[child for child in children if child['before']['parent_serial']==serial]
            require(len(roots)==1,'one field detailed reader')
            direct=roots[0]['before'];wanted['declared_rows']=direct['declared_rows']
            if not integers:wanted['explicit_width']=direct['explicit_width']
            leaves=[child for child in children if not any(other['before']['parent_serial']==child['before']['serial']for other in children)]
            require(len(leaves)==1,'one deepest field row reader')
            wanted.update(simulate_observed_rows(leaves[0],integers))
            for child in children:
                require(child['after']['status']==after['status'] and child['after']['values_hex']==after['values_hex'],
                    'field delegated status and result propagation')
            if not default_count:require(direct['values_hex']==before['values_hex'],'absent field default preserves prior buffer')
            elif count*width:
                defaults=column_values(direct,integers)
                pattern=defaults[:width]
                require(all(struct.pack('<i'if integers else'<d',value)==struct.pack('<i'if integers else'<d',pattern[index%width])
                    for index,value in enumerate(defaults)),'field header repeated column default pattern')
                if default_count!=width:
                    require(all(value==pattern[0]for value in pattern),'partial field defaults broadcast first scalar')
                wanted['default_value']=pattern[0]
        parameter=f'{count}:{width}:'+(''if integers else f'{mode}:')+(before['values_hex']or'-')
        commands.append(f'{kind} 0 {start} {data.hex()or"-"} {parameter}')
        wanted_rows.append(wanted);metadata.append({'file':before['file'],'id':before['id'],'width':width,
            'rows':count,'mode':mode,'serial':serial,'sequence_range':[before['sequence'],after['sequence']]})
    actual=replay(driver,commands);require(len(actual)==len(wanted_rows),'field replay count')
    rows=[]
    for got,wanted,meta in zip(actual,wanted_rows,metadata):
        differences=[];differences_at(got,wanted,'field_reader',differences)
        rows.append({**meta,'original':wanted,'differences':differences,'matched':not differences})
    return {'status':'pass'if all(row['matched']for row in rows)else'fail','count':len(rows),'calls':rows,
        'scope':'Exact seeded destination/raw binary64, retained row buffer, scalar assignments, complete-row writes, status, cursor and fgets attempts.'}


def compare_field_scans(events,source_files,driver):
    commands=[];expected=[];metadata=[]
    for event in events:
        if event['operation']!='field-scan':continue
        data=source_files[event['file']].read_bytes().replace(b'\r\n',b'\n')
        start,end=logical(event['before'],data),logical(event['after'],data)
        integer=event['format']=='%i';require(integer or event['format']=='%Flf','field scalar format')
        conversions=event['conversions'];require(conversions in (-1,0,1),'field scalar scan result')
        require(start<=end,'field scalar monotonic cursor')
        value=0
        if conversions==1:
            raw=bytes.fromhex(event['output_hex']);require(len(raw)==(4 if integer else 8),'field scalar exact raw width')
            value=struct.unpack('<i'if integer else'<d',raw)[0]
            require(math.isfinite(value)and value==event['value'],'field scalar redundant view')
        else:require(event['output_hex']is None and event['value']is None,'field scalar no invented failed output')
        kind='selector'if integer else'streamreal'
        commands.append(f'{kind} 0 {start} {data.hex()or"-"} -')
        expected.append({'kind':kind,'status':int(conversions==-1),'conversions':conversions,
            'values':[value],'cursor':end,'consumed_hex':data[start:end].hex(),'lines':[]})
        metadata.append({'file':event['file'],'sequence':event['sequence'],'reader_serial':event['reader_serial']})
    actual=replay(driver,commands);require(len(actual)==len(expected),'field scalar replay count')
    rows=[]
    for got,wanted,meta in zip(actual,expected,metadata):
        differences=[];differences_at(got,wanted,'field_scalar',differences)
        rows.append({**meta,'original':wanted,'differences':differences,'matched':not differences})
    return {'status':'pass'if all(row['matched']for row in rows)else'fail','count':len(rows),'calls':rows,
        'scope':'Successful native scalar assignments compare raw bytes; failed scan conversion/cursor is checked using a zero replay seed, while retained native row-buffer values are checked by aggregate replay.'}


def verify_field_commits(imports):
    definitions=field_definitions();calls=[];postprocesses=[]
    for item in imports:
        for handler in item['handlers']:
            identifier=handler['entry']['id']
            if identifier not in definitions:continue
            entries=[event for event in handler['events']if event['operation']=='field-reader-before']
            exits=[event for event in handler['events']if event['operation']=='field-reader-after']
            require(len(entries)==len(exits)==1,'one generic field helper per selected handler')
            before,after=entries[0],exits[0];definition=definitions[identifier]
            for key,value in definition.items():require(before[key]==after[key]==value,'field static dispatch '+key)
            require(before['serial']==after['serial'] and before['object']==after['object'],'field helper identity')
            state=handler['exit']['state'];native=next(obj for obj in state['objects']if obj['address']==before['object'])
            require(before['element_count']==native[definition['domain']] and before['width']==state['components'][definition['component']],
                'field header count and root component width')
            require(handler['exit']['status']==(0 if after['status']==0 else 2),'field reader status reaches handler')
            expected_hex=after['values_hex'];post=[event for event in handler['events']if event['operation']=='field-postprocess']
            require(len(post)==int(identifier==162),'BCCDEF-only postprocess observation')
            if post:
                event=post[0];require(event['reader_serial']==after['serial']and event['object']==before['object']
                    and event['before']['values_hex']==expected_hex and event['dimension']==state['dimension'],
                    'BCCDEF postprocess after reader including failure')
                values=column_values(after,True)
                if state['dimension']==3:
                    require(before['width']==3,'BCCDEF spatial triple width')
                    for offset in range(0,len(values),3):
                        a,b,c=values[offset:offset+3]
                        if a==b==c and a<0:values[offset:offset+3]=[((a-200+2**31)%2**32)-2**31,0,0]
                expected_hex=struct.pack('<'+'i'*len(values),*values).hex()
                require(event['after']['values_hex']==expected_hex,'BCCDEF exact int32 subtraction/post-zero')
                column_values(event['after'],True)
                postprocesses.append({'file':item['file'],'dimension':state['dimension'],'reader_status':after['status'],
                    'before_hex':event['before']['values_hex'],'after_hex':expected_hex})
            extent=before['element_count']*before['width']
            if definition['integers']:
                buffer=native['buffer_metadata']['direct'+str(definition['component'])]
                require(buffer['known_prefix']>=extent and buffer['known_hex'][:extent*8]==expected_hex,
                    'integer field result retained in initialized direct buffer')
            else:
                descriptor=next(d for d in native['descriptors']if d['id']==definition['component'])
                require(descriptor['width']==before['width']and descriptor['length']>=extent
                    and descriptor['values_hex'][:extent*16]==expected_hex,'real field result retained in descriptor')
            calls.append({'file':item['file'],'id':identifier,**definition,'dimension':state['dimension'],
                'width':before['width'],'rows':before['element_count'],'status':after['status'],
                'before_hex':before['values_hex'],'after_reader_hex':after['values_hex'],'after_handler_hex':expected_hex})
    return {'calls':calls,'postprocesses':postprocesses,'ids_observed':sorted({c['id']for c in calls}),
        'unobserved_selected_ids':sorted(set(definitions)-{c['id']for c in calls}),
        'zero_width_calls':sum(c['width']==0 for c in calls),
        'dimension_counts':dict(Counter(c['dimension']for c in calls)),
        'dimensions_by_id':{str(identifier):sorted({c['dimension']for c in calls if c['id']==identifier})for identifier in definitions},
        'static_inventory':binding(ROOT/'analysis/array-keyword-next-inventory.json')}


def dynamic_pairs(events):
    pairs={};stack=[]
    for event in events:
        op=event['operation']
        if op in ('dynamic-reader-before','dynamic-rows-before'):
            serial=event['serial'];require(serial not in pairs,'unique dynamic reader serial')
            require(event['parent_serial']==(stack[-1]['before']['serial']if stack else None),'dynamic row parent nesting')
            call={'before':event,'events':[]};pairs[serial]=call;stack.append(call)
        elif op in ('dynamic-reader-after','dynamic-rows-after'):
            require(stack and stack[-1]['before']['serial']==event['serial'],'dynamic reader exit nesting')
            call=stack.pop();call['after']=event
            for key in ('id','object','component','domain','mode','count','width_before','declared_rows','explicit_width','buffer','physical_items'):
                require(call['before'][key]==event[key],'stable dynamic reader argument '+key)
        elif op in ('dynamic-scan','dynamic-line'):
            require(stack and stack[-1]['before']['serial']==event['reader_serial'],'dynamic scalar/line innermost reader')
            stack[-1]['events'].append(event)
    require(not stack and all('after'in call for call in pairs.values()),'complete dynamic helper pairs')
    return pairs


def dynamic_buffer(event):
    values=column_values(event)
    require(len(values)==event['physical_items']<=128,'dynamic physical storage extent')
    logical=event['count']*event['width_current']
    require(event['logical_items']==logical and event['logical_extent_valid']==(0<=logical<=len(values)),
        'dynamic independent logical extent')
    if event['logical_extent_valid']:
        require(event['logical_values_hex']==event['values_hex'][:logical*16],'dynamic logical prefix raw view')
    else:require(event['logical_values_hex']is None,'invalid dynamic logical extent not read')
    require(event['extent_source']in ('registered-descriptor-begin-end','observed-calloc'),'dynamic extent provenance')
    if event['extent_source']=='registered-descriptor-begin-end':
        d=event['allocation'];require(d['length']==len(values)and d['storage_pointer']==event['buffer'],
            'dynamic descriptor actual physical extent')
    else:require(event['allocation']['bytes']==len(values)*8 and event['allocation']['freed']is False,'dynamic live scratch allocation')
    return values


def dynamic_line(event,data):
    begin,end=logical(event['before'],data),logical(event['after'],data)
    raw=b''if event['eof']else bytes.fromhex(event['hex'])
    require(event['limit']==1024 and begin<=end and raw==data[begin:end]and len(raw)<=1023,'dynamic fgets exactbytes')
    if event['eof']:require(begin==end==len(data),'dynamic fgets EOF location')
    return {'offset':begin,'cursor':end,'hex':raw.hex(),'status':int(event['eof'])}


def compare_dynamic_readers(events,source_files,driver):
    pairs=dynamic_pairs(events);commands=[];expected=[];metadata=[]
    for serial,call in pairs.items():
        before,after=call['before'],call['after'];dynamic_buffer(before);dynamic_buffer(after)
        if before['parent_serial']is not None:continue
        require(before['logical_extent_valid']and after['logical_extent_valid'],'bounded dynamic accessed extents')
        data=source_files[before['file']].read_bytes().replace(b'\r\n',b'\n')
        start,end=logical(before['stream'],data),logical(after['stream'],data)
        require(start<=end,'dynamic helper monotonic stream')
        descendants={serial}
        while True:
            extended=descendants|{n for n,c in pairs.items()if c['before']['parent_serial']in descendants}
            if extended==descendants:break
            descendants=extended
        lines=sorted([event for n in descendants for event in pairs[n]['events']if event['operation']=='dynamic-line'],key=lambda e:e['sequence'])
        rowcount,width=before['count'],before['width_before']
        raw_seed=before['values_hex']or'-'
        aggregate=before['operation']=='dynamic-reader-before'
        if aggregate:
            require(before['mode']in (0,2),'dynamic recovered aggregate mode')
            if before['mode']==2:
                kind='mode2columns';parameter=f'{rowcount}:{width}:{raw_seed}'
            else:
                kind='realcolumns';parameter=f'{rowcount}:{width}:0:'+(before['logical_values_hex']or'-')
                require(before['width_current']==after['width_current'],'mode0 preserves referenced width')
                require(before['values_hex'][rowcount*width*16:]==after['values_hex'][rowcount*width*16:],
                    'mode0 untouched physical tail')
            scans=after['scans'];require(len(scans)==int(bool(lines and not lines[0]['eof'])),'one dynamic header sscanf')
            conversions=scans[0]['conversions']if scans else -1
            if scans:
                fmt='%i %Flf'if before['mode']==0 else'%i %i'+' %Flf'*10
                require(scans[0]['format']==fmt and scans[0]['input_hex']==lines[0]['hex'],'dynamic header exact format/input')
        else:
            require(before['kind']=='implicit','top standalone4199a0 only')
            kind='realrows';parameter=f'{rowcount}:{width}:{before["declared_rows"]}:{raw_seed}';conversions=-1
        wanted={'kind':kind,'status':after['status'],'conversions':conversions,'cursor':end,
            'consumed_hex':data[start:end].hex(),'lines':[dynamic_line(e,data)for e in lines]}
        if kind in ('realrows','mode2columns'):
            wanted.update(storage_hex=after['values_hex'],width_after=after['width_current'])
        else:wanted.update(values_hex=after['logical_values_hex'])
        leaves=[pairs[n]for n in descendants if pairs[n]['before']['operation']=='dynamic-rows-before'
                and not any(c['before']['parent_serial']==n for c in pairs.values())]
        if leaves:
            require(len(leaves)==1,'single dynamic leaf row reader')
            leaf=leaves[0];entry=leaf['before'];exit=leaf['after']
            adapted={'before':{**entry,'element_count':entry['count'],'width':entry['width_current'],
                'values':values_from_hex(entry['logical_values_hex']),'values_hex':entry['logical_values_hex']},
                'after':{**exit,'values':values_from_hex(exit['logical_values_hex']),'values_hex':exit['logical_values_hex']},
                'events':[{**e,'operation':e['operation'].replace('dynamic-','column-')}for e in leaf['events']]}
            wanted.update(simulate_observed_rows(adapted,False))
            if aggregate:wanted['explicit_width']=next((pairs[n]['before']['explicit_width']for n in descendants
                if pairs[n]['before']['parent_serial']==serial),0)
        commands.append(f'{kind} 0 {start} {data.hex()or"-"} {parameter}')
        expected.append(wanted);metadata.append({'file':before['file'],'id':before['id'],'serial':serial,
            'physical_items':before['physical_items'],'width_before':width,'width_after':after['width_current'],
            'sequence_range':[before['sequence'],after['sequence']]})
    for event in events:
        if event['operation']!='dynamic-header':continue
        data=source_files[event['file']].read_bytes().replace(b'\r\n',b'\n')
        start,end=logical(event['before'],data),logical(event['after'],data)
        values=[struct.unpack(fmt,bytes.fromhex(raw))[0]for fmt,raw in zip(('<i','<d','<i'),event['output_hex'])]
        require(values==event['values'],'mixed header raw/numeric assignments')
        lines=[];cursor=start
        for line in event['lines']:
            raw=b''if line['eof']else bytes.fromhex(line['hex'])
            require(raw==data[cursor:cursor+len(raw)]and line['limit']==1024,'mixed header fgets')
            lines.append({'offset':cursor,'cursor':cursor+len(raw),'hex':raw.hex(),'status':int(line['eof'])});cursor+=len(raw)
        require(cursor==end,'mixed header cursor')
        for scan in event['scans']:require(scan['format']=='%i %Flf %i','mixed header scan order')
        commands.append(f'intrealint 0 {start} {data.hex()or"-"} -')
        expected.append({'kind':'intrealint','status':event['status'],'conversions':event['conversions'],
            'values':values,'cursor':end,'consumed_hex':data[start:end].hex(),'lines':lines})
        metadata.append({'file':event['file'],'id':event['id'],'serial':event['serial'],'sequence':event['sequence']})
    actual=replay(driver,commands);require(len(actual)==len(expected),'dynamic reader replay count')
    rows=[]
    for got,wanted,meta in zip(actual,expected,metadata):
        differences=[];differences_at(got,wanted,'dynamic_reader',differences)
        rows.append({**meta,'original':wanted,'differences':differences,'matched':not differences})
    return {'status':'pass'if all(r['matched']for r in rows)else'fail','count':len(rows),'calls':rows,
        'scope':'Native-before physical seed only; exact output storage/tails, live width mutation, mixed header/raw rows/aggregate status, reused rows and cursors. No native-after values seed replacement.'}


def verify_dynamic_lifecycle(imports):
    summaries=[];getters=[];widths=[];stacks={};pairs={}
    names={'dynamic-getter','dynamic-width','dynamic-metadata','dynamic-metadata-clear','dynamic-property'}
    for item in imports:
        for event in item['observations']:
            op=event['operation'];base=op.rsplit('-',1)[0]
            if base in names and op.endswith(('-before','-after')):
                serial=event['serial']
                if op.endswith('-before'):
                    require(serial not in pairs,'unique dynamic lifecycle serial')
                    pairs[serial]={'before':event};stacks.setdefault(base,[]).append(serial)
                elif op.endswith('-after'):
                    require(stacks.get(base)and stacks[base].pop()==serial,'paired dynamic lifecycle nesting')
                    pairs[serial]['after']=event
            if op=='dynamic-property-guard':require(event['value']==1,'retained property cleanup guard true')
        for handler in item['handlers']:
            identifier=handler['entry']['id']
            if identifier not in (170,196,197,254,255,263):continue
            events=handler['events'];headers=[e for e in events if e['operation']=='dynamic-header']
            readers=[e for e in events if e['operation']=='dynamic-reader-after']
            rawrows=[e for e in events if e['operation']=='dynamic-rows-after'and e['parent_serial']is None]
            before,after=handler['entry']['state'],handler['exit']['state'];dimension=after['dimension']
            selected=[e for e in events if e['operation']=='object-selected']
            require(selected,'dynamic selected-object observation')
            allgetters=[e for e in events if e['operation']=='dynamic-getter-after']
            widthcalls=[e for e in events if e['operation']=='dynamic-width-after']
            if identifier in (196,197)or(identifier==170 and dimension==2):
                require(len(headers)==1 and not readers,'dynamic mixed header route')
                header=headers[0]
                if header['status']:
                    require(not rawrows and not allgetters and not widthcalls and handler['exit']['status']==2,
                        'mixed header failure before destination/width/getter work')
                else:
                    require(len(rawrows)==1 and handler['exit']['status']==(0 if rawrows[0]['status']==0 else 2),
                        'raw row result reaches dynamic handler')
                    row=rawrows[0]
                    obj=next(o for o in after['objects']if o['address']==row['object'])
                    if identifier in (196,197):
                        component=17 if identifier==196 else 60
                        expected_width=header['values'][2]or(2 if identifier==197 else 0)
                        require(len(widthcalls)==1 and widthcalls[0]['requested']==expected_width,
                            'user header width setter and element zero coercion')
                        require(row['width_current']==after['components'][component]==expected_width,'user live width')
                        require(len(allgetters)==row['count']*expected_width+1 and all(g['component']==component for g in allgetters),
                            'user getter once per default scalar plus final access')
                    else:
                        require(not widthcalls and not allgetters,'planar temperature has no getter/width setter')
                        count=row['count'];values=dynamic_buffer(row)
                        d8=next(d for d in obj['descriptors']if d['id']==8)
                        d12=next(d for d in obj['descriptors']if d['id']==12)
                        if header['values'][2]==2:
                            require(row['extent_source']=='observed-calloc'and row['width_current']==2,'planar paired scratch extent')
                            a=[values[i*2+1]for i in range(count)];b=[values[i*2+1]-values[i*2]for i in range(count)]
                            require(all(math.isfinite(v)for v in b),'finite planar subtraction domain')
                        else:
                            require(row['extent_source']=='registered-descriptor-begin-end','planar scalar direct destination')
                            a=values[:count];b=[0.0]*count
                        require(d8['values_hex'][:count*16]==struct.pack('<'+'d'*count,*a).hex()
                            and d12['values_hex'][:count*16]==struct.pack('<'+'d'*count,*b).hex(),
                            'planar transformed temperature values including helper failure')
            elif identifier==263 and dimension==2:
                require(not readers and not rawrows and not headers and handler['exit']['status']==0,'planar DRMESH no payload read')
            elif identifier==255 and not readers:
                require(len(allgetters)==1 and int(allgetters[0]['pointer'],16)==0 and handler['exit']['status']==0,
                    'NWEAR null getter skips reader')
            else:
                require(len(readers)==1 and not headers and not rawrows,'dynamic aggregate route')
                reader=readers[0]
                require(handler['exit']['status']==(0 if reader['status']==0 else 2),'aggregate dynamic status')
                component={170:8,254:46,255:24,263:25}[identifier]
                require(reader['component']==component and reader['mode']==(2 if identifier==254 else 0),'dynamic component/mode dispatch')
                if identifier in (254,255):
                    require(len(allgetters)==1 and allgetters[0]['option']==0 and allgetters[0]['component']==component,
                        'selected dynamic getter with cleanup option0')
                if identifier==254:
                    require(reader['width_before']==(9 if dimension==3 else 1)
                        and reader['width_current']==after['components'][46],'MATAXI prewidth and root writeback')
                    obj=next(o for o in after['objects']if o['address']==reader['object'])
                    d=next(d for d in obj['descriptors']if d['id']==46)
                    require(d['values_hex']==reader['values_hex'],'MATAXI complete physical storage retained after parsed-width divergence')
            summaries.append({'file':item['file'],'id':identifier,'dimension':dimension,'status':handler['exit']['status'],
                'header_calls':len(headers),'aggregate_calls':len(readers),'raw_row_calls':len(rawrows),'getter_calls':len(allgetters),
                'width_setters':len(widthcalls)})
    require(all(not value for value in stacks.values())and all('after'in pair for pair in pairs.values()),'complete dynamic lifecycle pairs')
    for pair in pairs.values():
        before,after=pair['before'],pair['after'];kind=before['operation']
        if kind=='dynamic-getter-before':
            a,b=before['descriptor'],after['descriptor'];require(before['component']==after['component']==a['id']==b['id'],'getter component identity')
            require(a['address']==b['address']and after['pointer']==b['bound_pointer']==b['storage_pointer'],'stable getter descriptor and returned binding')
            av=values_from_hex(a['values_hex']);bv=values_from_hex(b['values_hex'])
            require(len(av)==a['length']and len(bv)==b['length'],'getter physical descriptor lengths')
            wanted=av[:len(bv)]+[0.0]*max(0,len(bv)-len(av))
            if len(av)!=len(bv)and b['id']==46 and b['width']==9:
                for row in range(b['count']):
                    for offset in (0,4,8):wanted[row*9+offset]=1.0
            require(struct.pack('<'+'d'*len(wanted),*wanted).hex()==b['values_hex'],'getter preserved linear prefix/zero append/conditional MATAXI diagonal')
            if b['count']>0:
                require(b['old_count']==a['count']and b['old_width']==a['width'],'getter descriptor previous shape update')
            getters.append({'file':before['file'],'component':a['id'],'before_shape':[a['count'],a['width'],a['old_count'],a['old_width']],
                'after_shape':[b['count'],b['width'],b['old_count'],b['old_width']],'before_hex':a['values_hex'],'after_hex':b['values_hex']})
        elif kind=='dynamic-width-before':
            field='user_node_width_0x3398'if before['rva']==0x1d21c0 else'user_element_width_0x33a8'
            require(before['requested']==after['requested']==after[field],'user setter requested count')
            widths.append({'file':before['file'],'field':field,'before':before[field],'after':after[field],
                'components_before':before['components'],'components_after':after['components']})
    return {'handlers':summaries,'getters':getters,'width_setters':widths,'ids_observed':sorted({x['id']for x in summaries}),
        'root_descriptor_width_conflated':False,'source_contract':binding(ROOT/'analysis/dynamic-field-static-contract.json')}


def custom_pairs(events):
    pairs={};stack=[]
    for event in events:
        op=event['operation']
        if op in ('custom-rows-before','custom-typed-row-before'):
            require(not stack and event['serial']not in pairs,'custom row calls not nested')
            call={'before':event,'events':[]};pairs[event['serial']]=call;stack.append(call)
        elif op in ('custom-rows-after','custom-typed-row-after'):
            require(stack and stack[-1]['before']['serial']==event['serial'],'custom reader pair')
            stack.pop()['after']=event
        elif op in ('custom-line','custom-scan'):
            require(stack and stack[-1]['before']['serial']==event['reader_serial'],'custom row scalar/line scope')
            stack[-1]['events'].append(event)
    require(not stack and all('after'in c for c in pairs.values()),'complete custom row calls')
    return pairs


def custom_lines(lines,data,start,end):
    output=[];cursor=start
    for line in lines:
        raw=b''if line['eof']else bytes.fromhex(line['hex'])
        require(line['limit']==1024 and len(raw)<=1023 and raw==data[cursor:cursor+len(raw)],'custom fgets fixture bytes')
        if line['eof']:require(cursor==len(data),'custom fgets EOF position')
        output.append({'offset':cursor,'cursor':cursor+len(raw),'hex':raw.hex(),'status':int(line['eof'])});cursor+=len(raw)
    require(cursor==end,'custom line-accounted cursor')
    return output


def compare_custom_readers(imports,events,source_files,driver):
    commands=[];expected=[];metadata=[];pairs=custom_pairs(events)
    for item in imports:
        for handler in item['handlers']:
            identifier=handler['entry']['id']
            if identifier not in (198,266):continue
            data=source_files[item['file']].read_bytes().replace(b'\r\n',b'\n');observations=handler['events']
            headers=[e for e in observations if e['operation']in ('custom-header','custom-token-header')]
            require(len(headers)==1,'one selected custom header')
            header=headers[0];start,end=logical(header['before'],data),logical(header['after'],data)
            wanted={'status':header['status'],'cursor':end,'consumed_hex':data[start:end].hex(),
                'lines':custom_lines(header['lines'],data,start,end)}
            if identifier==198:
                kind='customheader';formats=('<i','<i','<i','<d','<i')
                values=[struct.unpack(fmt,bytes.fromhex(raw))[0]for fmt,raw in zip(formats,header['output_hex'])]
                require(values==header['values'],'CUSVAR header raw/numeric view')
                scans=header['scans'];require(len(scans)==int(bool(header['lines']and not header['lines'][0]['eof'])),'CUSVAR header scanf count')
                if scans:require(scans[0]['format']=='%i %i %i %Flf %i','CUSVAR exact header format')
                wanted.update(values=values,conversions=scans[0]['conversions']if scans else-1)
            else:
                kind='dynamicheader';wanted['conversions']=None
                if header['status']:
                    wanted.update(token_count=0,tokens_hex=[],integer_values=[0]*6)
                    require(not any(e['operation']=='custom-token-access'for e in observations),'DYNARY EOF before tokenaccess')
                else:
                    tokens=[value or''for value in header['tokens_hex']];wanted.update(token_count=header['count'],tokens_hex=tokens)
                    require(header['limit']==10 and len(tokens)==header['count'],'DYNARY token count/limit')
                    accesses=[e for e in observations if e['operation']=='custom-token-access']
                    integer_calls=[e for e in observations if e['operation']=='custom-token-integer']
                    real_calls=[e for e in observations if e['operation']=='custom-token-real-input']
                    expected_indices=list(range(5+(header['count']>=6)+(header['count']>6)))
                    require([e['index']for e in accesses]==expected_indices,'DYNARY accessor order')
                    for access in accesses:
                        require(access['value_hex']==(tokens[access['index']]if access['index']<len(tokens)else''),'DYNARY missing token empty fallback')
                    require(len(integer_calls)==5+int(header['count']>=6),'DYNARY exact integer conversion calls')
                    require([e['input_hex']for e in integer_calls]==[e['value_hex']for e in accesses[:len(integer_calls)]],
                        'DYNARY integer converter inputs')
                    integers=[e['value']for e in integer_calls]
                    if len(integers)==5:integers.append(integers[3])
                    wanted['integer_values']=integers
                    require(len(real_calls)==int(header['count']>6),'DYNARY optional atof call')
                    if not real_calls:wanted['default_value']=0.0
                    else:
                        require(real_calls[0]['input_hex']==tokens[6],'DYNARY atof input token')
                        defaults=[e for e in observations if e['operation']=='custom-dynamic-default-after']
                        if defaults and defaults[0]['entry']['real_storage']['physical_items']:
                            wanted['default_value']=custom_vector(defaults[0]['entry']['real_storage'])[0]
            wanted['kind']=kind;commands.append(f'{kind} 0 {start} {data.hex()or"-"} -')
            expected.append(wanted);metadata.append({'file':item['file'],'id':identifier,'sequence':header['sequence']})
    for serial,call in pairs.items():
        before,after=call['before'],call['after'];data=source_files[before['file']].read_bytes().replace(b'\r\n',b'\n')
        if before['operation']=='custom-rows-before':
            require(all(before[key]==after[key]for key in ('payload','declared_rows','count','width')),'custom sparse argument stability')
            custom_vector(before['storage']);custom_vector(after['storage'])
            start,end=logical(before['stream'],data),logical(after['stream'],data)
            lineevents=[e for e in call['events']if e['operation']=='custom-line']
            lines=[dynamic_line(e,data)for e in lineevents]
            count,width=before['count'],before['width'];kind='realrows'
            parameter=f'{count}:{width}:{before["declared_rows"]}:'+(before['storage']['values_hex']or'-')
            wanted={'kind':kind,'status':after['status'],'conversions':-1,'cursor':end,'consumed_hex':data[start:end].hex(),
                'storage_hex':after['storage']['values_hex'],'width_after':width,'lines':lines}
            adapted={'before':{**before,'kind':'implicit','element_count':count,'explicit_width':0,
                'values_hex':before['logical_values_hex'],'values':values_from_hex(before['logical_values_hex'])},
                'after':{**after,'values_hex':after['logical_values_hex'],'values':values_from_hex(after['logical_values_hex'])},
                'events':[{**e,'operation':e['operation'].replace('custom-','column-')}for e in call['events']]}
            wanted.update(simulate_observed_rows(adapted,False))
        else:
            integers=before['integers'];width=before['width']
            require(all(before[key]==after[key]for key in ('payload','offset','width','kind','integers')),'custom sequential row identity')
            original=custom_vector(before['storage'],integers);result=custom_vector(after['storage'],integers)
            kind=('int'+str(width+1))if integers else('coord'+str(width));parameter='-'
            start,end=logical(after['before'],data),logical(after['after'],data)
            values=[struct.unpack('<i'if index==0 or integers else'<d',bytes.fromhex(raw))[0]for index,raw in enumerate(after['output_hex'])]
            require(values==after['values'],'typed row raw/numeric view')
            require(len(values)==width+1 and len(original)==len(result),'typed row output physical width')
            patched=original[:];offset=before['offset'];patched[offset:offset+width]=values[1:]
            require(struct.pack('<'+('i'if integers else'd')*len(patched),*patched).hex()==after['storage']['values_hex'],
                'sequential typed row updates exact offset independent of parsed label even failure')
            scans=after['scans'];wanted={'kind':kind,'status':after['status'],'conversions':scans[0]['conversions']if scans else-1,
                'values':values,'cursor':end,'consumed_hex':data[start:end].hex(),'lines':custom_lines(after['lines'],data,start,end)}
        commands.append(f'{kind} 0 {start} {data.hex()or"-"} {parameter}')
        expected.append(wanted);metadata.append({'file':before['file'],'serial':serial,'sequence_range':[before['sequence'],after['sequence']]})
    actual=replay(driver,commands);require(len(actual)==len(expected),'custom helper replay count')
    rows=[]
    for got,wanted,meta in zip(actual,expected,metadata):
        if wanted['kind']=='dynamicheader':got.update(integer_values=got['values'][:6],default_value=got['values'][6])
        differences=[];differences_at(got,wanted,'custom_reader',differences)
        rows.append({**meta,'original':wanted,'differences':differences,'matched':not differences})
    return {'status':'pass'if all(r['matched']for r in rows)else'fail','count':len(rows),'calls':rows,
        'scope':'CUSVAR header outputs;DYNARY tokens and observedinteger outputs, atof result only where original defaultfill exposes it; exact sequential rows including partialzero writes; sparse rows seeded with originalBEFORE physicalstorage. All final model state compared separately.'}


def verify_custom_lifecycle(imports):
    summaries=[];destroy=[]
    for item in imports:
        for event in item['observations']:
            if event['operation']=='custom-dynamic-clear-after':
                require(event['store']['count']==0 and not event['store']['entries'],'dynamic list clear observed empty')
                destroy.append({'file':item['file'],'operation':event['operation'],'object':event['object']})
            elif event['operation']=='custom-store-destroy-after':
                require(event['destroyed']is True,'custom collection destructor completion')
                destroy.append({'file':item['file'],'operation':event['operation'],'object':event['object']})
        for handler in item['handlers']:
            identifier=handler['entry']['id']
            if identifier not in (198,266):continue
            events=handler['events'];before=handler['entry']['state'];after=handler['exit']['state']
            def get(name):return [e for e in events if e['operation']==name]
            headers=get('custom-header'if identifier==198 else'custom-token-header');require(len(headers)==1,'one custom header perhandler')
            header=headers[0];rows=get('custom-rows-after');typed=get('custom-typed-row-after')
            if identifier==198:
                updates=get('custom-root-update-after');configs=get('custom-configure-after')
                if header['status']:
                    require(not configs and not rows and not updates and handler['exit']['status']==2,'CUSVAR header failure beforestoremutation')
                else:
                    require(len(configs)==len(rows)==1 and not typed,'CUSVAR configure/default/sparse row route')
                    configbefore=get('custom-configure-before');require(len(configbefore)==1,'CUSVAR config pairing')
                    prior=configbefore[0];configured=configs[0]['entry'];custom_id=header['values'][0]
                    require((prior['custom_id'],prior['kind'],prior['rows'],prior['width'])==tuple(header['values'][i]for i in (0,1,2,4)),
                        'CUSVAR native header configure arguments')
                    existing=[e for e in prior['store']['entries']if e['custom_id']==custom_id]
                    created=get('custom-created');require(len(created)==int(not existing),'CUSVAR exact-ID append or reuse')
                    baseline=existing[0]if existing else created[0]['entry']
                    require(configured['address']==baseline['address']and configured['custom_id']==custom_id and configured['kind']==prior['kind'],
                        'CUSVAR configured identity/kind')
                    obj=next(o for o in after['objects']if any(e['address']==configured['address']for e in o['custom_fields']['entries']))
                    expected_count={0:prior['rows'],1:obj['nodes'],2:obj['elements']}.get(prior['kind'],baseline['shape']['count'])
                    require(configured['shape']['count']==expected_count and configured['shape']['width']==prior['width']
                        and configured['previous']['width']==baseline['shape']['width'],'CUSVAR count/domain and width history')
                    if prior['kind']in (0,1,2):require(configured['previous']['count']==baseline['shape']['count'],'CUSVAR countsetterhistory')
                    else:require(configured['previous']['count']==baseline['previous']['count'],'CUSVAR unrecognizedkind retainscount history')
                    oldvalues=custom_vector(baseline['storage']);newvalues=custom_vector(configured['storage'])
                    resize=oldvalues[:len(newvalues)]+[0.0]*max(0,len(newvalues)-len(oldvalues))
                    require(struct.pack('<'+'d'*len(resize),*resize).hex()==configured['storage']['values_hex'],'CUSVAR configure preservesprefix andzeroappends')
                    defaults=get('custom-default-after');require(len(defaults)==1,'CUSVAR onephysical defaultfill')
                    n=defaults[0]['entry']['storage']['physical_items'];default_hex=header['output_hex'][3]
                    require(defaults[0]['entry']['storage']['values_hex']==default_hex*n,'CUSVAR wholephysical default fill')
                    require(rows[0]['payload']==configured['address']and rows[0]['declared_rows']==prior['rows']
                        and rows[0]['count']==configured['shape']['count']and rows[0]['width']==configured['shape']['width'],'CUSVAR sparsecounts fromentryshape')
                    require(handler['exit']['status']==(0 if rows[0]['status']==0 else 2),'CUSVAR row status maps2')
                    require(len(updates)==int(rows[0]['status']==0),'CUSVAR success-only rootsideeffect')
                    if updates:
                        require(updates[0]['value']==custom_id and int(updates[0]['target'],16)==int(after['address'],16)+0x33b8,
                            'CUSVAR exactrootlastIDtarget')
                        require(updates[0]['sequence']>rows[0]['sequence'],'CUSVAR lastID writes after successfulrows')
                expected_last=updates[0]['value']if updates else before['custom_last_id_0x33b8']
                require(after['custom_last_id_0x33b8']==expected_last,'CUSVAR lastID persists onfailure')
            else:
                lookups=get('custom-dynamic-lookup-after');configs=get('custom-dynamic-configure-after')
                require(after['custom_last_id_0x33b8']==before['custom_last_id_0x33b8'],'DYNARY doesnotwriteCUSVARlastID')
                if header['status']:
                    require(not lookups and not configs and not rows and not typed and handler['exit']['status']==2,'DYNARY headerEOF beforelookup')
                else:
                    require(len(lookups)==1,'DYNARY one positional lookup')
                    lookup=lookups[0];prior=get('custom-dynamic-lookup-before')[0];index=prior['index']
                    expected_count=max(prior['store']['count'],index)if index>0 else prior['store']['count']
                    require(lookup['store']['count']==expected_count,'DYNARY positional append placeholders')
                    expected_pointer=lookup['store']['entries'][min(max(index,1),expected_count)-1]['address']if expected_count else'0x0'
                    require(int(lookup['pointer'],16)==int(expected_pointer,16),'DYNARY nonpositive first existing lookup')
                    if not int(lookup['pointer'],16):require(not configs and not rows and not typed and handler['exit']['status']==0,'nullDYNARYselection no rows')
                    else:
                        require(len(configs)==1,'DYNARY configureselected payload')
                        priorconfig=get('custom-dynamic-configure-before')[0];a,b=priorconfig['entry'],configs[0]['entry'];requested=priorconfig['requested']
                        require(a['address']==b['address']==lookup['pointer']and requested['source']==b['source_0x0c']==1,'DYNARY mode normalization andstablepayload')
                        require((b['type_0x18'],b['count_0x24'],b['width_0x28'])==(requested['type'],requested['count'],requested['width']),
                            'DYNARY config selected headerfields')
                        expected_pair={1:(0,1),2:(1,2),3:(0,3)}.get(requested['type'],(a['integer_0x1c'],a['discriminator_0x20']))
                        require((b['integer_0x1c'],b['discriminator_0x20'])==expected_pair,'DYNARY unrecognizedtype preservesbyte/discriminator')
                        selected='integer_storage'if b['integer_0x1c']else'real_storage';inactive='real_storage'if b['integer_0x1c']else'integer_storage'
                        original=custom_vector(a[selected],bool(b['integer_0x1c']));observed=custom_vector(b[selected],bool(b['integer_0x1c']))
                        n=b['count_0x24']*b['width_0x28'];require(n==len(observed),'DYNARY selectedphysicalresizeextent')
                        wanted=original[:n]+[0]*max(0,n-len(original));fmt='i'if b['integer_0x1c']else'd'
                        require(struct.pack('<'+fmt*n,*wanted).hex()==b[selected]['values_hex']and a[inactive]['values_hex']==b[inactive]['values_hex'],
                            'DYNARY independent typedbuffers preserveinactive andselectedlinear prefix')
                        sparse=b['discriminator_0x20']==3 and b['integer_0x1c']==0
                        sequential=(b['discriminator_0x20']==1 and not b['integer_0x1c']and b['width_0x28']in (2,3))or(b['discriminator_0x20']==2 and b['integer_0x1c']and b['width_0x28']in (3,4,8))
                        if sparse:
                            require(len(rows)==1 and not typed and len(get('custom-dynamic-default-after'))==1,'DYNARY sparse default+rowsroute')
                            require(handler['exit']['status']==(0 if rows[0]['status']==0 else 2),'DYNARY sparsefailure status')
                        elif sequential:
                            require(not rows and not get('custom-dynamic-default-after'),'DYNARY sequential branch no sparse default')
                            expected=len(typed)if typed and typed[-1]['status']else b['count_0x24']
                            require(len(typed)==expected and len(typed)<=b['count_0x24']and all(e['status']==0 for e in typed[:-1]),'DYNARY sequential failurestops rows')
                            require([e['offset']for e in typed]==[i*b['width_0x28']for i in range(len(typed))],'DYNARY sequential storageoffset ignoreslabel')
                            require(handler['exit']['status']==(2 if typed and typed[-1]['status']else 0),'DYNARY sequential status')
                        else:require(not rows and not typed and handler['exit']['status']==0,'DYNARY inactive branch payloadnotconsumed')
            summaries.append({'file':item['file'],'id':identifier,'status':handler['exit']['status'],'header_status':header['status'],
                'sparse_calls':len(rows),'typed_rows':len(typed),'custom_last_id_before':before['custom_last_id_0x33b8'],
                'custom_last_id_after':after['custom_last_id_0x33b8']})
    return {'handlers':summaries,'destruction':destroy,'source_contract':binding(ROOT/'analysis/custom-field-static-contract.json'),
        'scope':'Exact signed custom IDs and positional dynamic IDs; stable selected record identity; configure/default/row/success-update order; both typedbuffer allocations; source-native branches/partialwrites and list destruction.'}


def verify_shape_producer(handler):
    events=handler['events'];identifier=handler['entry']['id']
    selectors=[e for e in events if e['operation']=='material-selector']
    headers=[e for e in events if e['operation']=='material-shape-producer-header']
    old=handler['before']['state']['materials'];new=handler['after']['state']['materials']
    require(len(selectors)==1 and selectors[0]['conversions']in (-1,1),'initialized material shape selector')
    if selectors[0]['conversions']==-1:
        require(not headers and old==new and handler['exit']['status']==2,'material shape selector EOF before mutation');return
    selected=selectors[0]['value'];existing=next((r for r in old if r['id']==selected),None)
    require([r['id']for r in new]==[r['id']for r in old]+([]if existing else[selected]),'material shape exact-ID append')
    require(len(headers)==1 and headers[0]['id']==identifier,'single property producer read')
    header=headers[0];target=next(r for r in new if r['id']==selected)
    native=next(r for r in handler['exit']['state']['material_list']['records']if r['id']==selected)
    require(header['material']==native['address'],'property selected material identity')
    fields=['field_0x2150']if identifier==127 else['field_0x2154','field_0x2158','field_0x215c']
    require(header['values']==[struct.unpack('<i',bytes.fromhex(h))[0]for h in header['output_hex']], 'property output raw int32 identity')
    require([target[k]for k in fields]==header['values'],'direct property partial outputs reach record')
    for key in MATERIAL_FIELDS:
        if key not in fields:require(header['before_material'][key]==header['after_material'][key]==native[key],'unselected property field retained '+key)
    require(handler['exit']['status']==(0 if header['status']==0 else 2),'property reader failure maps2')
    operations=[e['operation']for e in events]
    for op in ('material-append-before','material-append-after','material-refresh-before','material-refresh-after'):
        require(operations.count(op)==int(existing is None),'property creation refresh versus existing edit')
    if existing is None:
        require(operations.index('material-append-after')<operations.index('material-refresh-before')<operations.index('material-refresh-after')<operations.index('material-shape-producer-header'),'creation refresh precedes property parsing')
        refreshed=next(e for e in events if e['operation']=='material-refresh-after')['state']
        require(refreshed['components']==handler['exit']['state']['components'],'property mutation does not eagerly refresh table')
    else:require(handler['entry']['state']['components']==handler['exit']['state']['components'],'existing property edit leaves table stale')
    handler['creation']=existing is None


def shape_pairs(events):
    pairs={};stack=[]
    for event in events:
        op=event['operation']
        if op in ('material-shape-reader-before','material-shape-rows-before'):
            require(event['serial']not in pairs,'unique shape reader serial')
            if op.endswith('rows-before'):require(stack and stack[-1]['before']['serial']==event['parent_serial'],'shape row reader parent')
            else:require(not stack,'shape aggregate reader not nested')
            pair={'before':event,'events':[]};pairs[event['serial']]=pair;stack.append(pair)
        elif op in ('material-shape-reader-after','material-shape-rows-after'):
            require(stack and stack[-1]['before']['serial']==event['serial'],'shape reader return pairing');stack.pop()['after']=event
        elif op=='material-shape-scan':
            require(stack and stack[-1]['before']['serial']==event['reader_serial'],'shape scalar within detailed reader')
            stack[-1]['events'].append({**event,'operation':'column-scan'})
        elif op=='material-shape-header-scan':
            require(stack and stack[-1]['before']['serial']==event['reader_serial'],'shape header within aggregate')
            stack[-1]['events'].append(event)
    require(not stack and all('after'in p for p in pairs.values()),'complete shape readers')
    return pairs


def compare_shape_readers(events,source_files,driver):
    commands=[];expected=[];metadata=[];pairs=shape_pairs(events)
    for e in events:
        if e['operation']!='material-shape-producer-header':continue
        data=source_files[e['file']].read_bytes().replace(b'\r\n',b'\n');start,end=logical(e['before'],data),logical(e['after'],data)
        scans=e['scans'];wanted={'kind':e['kind'],'status':e['status'],'conversions':scans[0]['conversions']if scans else-1,
            'values':[struct.unpack('<i',bytes.fromhex(v))[0]for v in e['output_hex']], 'cursor':end,'consumed_hex':data[start:end].hex(),'lines':custom_lines(e['lines'],data,start,end)}
        commands.append(f'{e["kind"]} 0 {start} {data.hex()or"-"} -');expected.append(wanted);metadata.append({'file':e['file'],'sequence':e['sequence'],'id':e['id']})
    for serial,pair in pairs.items():
        a,b=pair['before'],pair['after'];data=source_files[a['file']].read_bytes().replace(b'\r\n',b'\n')
        require(all(a[k]==b[k]for k in ('id','component','object','count')),'shape reader stable identity/count')
        start,end=logical(a['stream'],data),logical(b['stream'],data)
        if a['operation']=='material-shape-reader-before':
            scans=[e for e in pair['events']if e['operation']=='material-shape-header-scan'];lines=b['lines']
            require(len(lines)==1 and len(scans)==int(not lines[0]['eof']),'shape exactly one header line/optional sscanf')
            raw=b''if lines[0]['eof']else bytes.fromhex(lines[0]['hex']);finish=start+len(raw)
            vals=[0,0,0,0.0];conversions=-1
            if scans:
                scan=scans[0];require(scan['format']=='%i %i %i %Flf' and scan['input_hex']==raw.hex(),'shape raw mixed header format/bytes')
                vals=[struct.unpack('<d'if i==3 else'<i',bytes.fromhex(h))[0]for i,h in enumerate(scan['output_hex'])];conversions=scan['conversions']
                require(vals==scan['values'],'shape header numeric/raw identity')
            wanted={'kind':'shapeheader','status':int(vals[2]<1 or conversions<1),'conversions':conversions,'values':vals,'cursor':finish,'consumed_hex':data[start:finish].hex(),'lines':custom_lines(lines,data,start,finish)}
            commands.append(f'shapeheader 0 {start} {data.hex()or"-"} -')
        else:
            require(a['width']==a['explicit_width']==b['width']==b['explicit_width']>0,'shape equal positive explicit width')
            count,width=a['count'],a['width'];ad,bd=a['descriptor'],b['descriptor']
            require(ad['address']==bd['address']and ad['id']==bd['id']==a['component'],'shape detailed descriptor identity')
            require(count*width<=ad['length']==bd['length'],'shape detailed physical destination extent')
            adapted={'before':{**a,'kind':'explicit','element_count':count,'values_hex':ad['values_hex'][:count*width*16],'values':values_from_hex(ad['values_hex'])[:count*width]},
                'after':{**b,'values_hex':bd['values_hex'][:count*width*16],'values':values_from_hex(bd['values_hex'])[:count*width]},'events':pair['events']}
            wanted={'kind':'explicitrows','status':b['status'],'conversions':-1,'cursor':end,'consumed_hex':data[start:end].hex(),
                'storage_hex':bd['values_hex'],'width_after':width,'explicit_width':width,'lines':[]}
            wanted.update(simulate_observed_rows(adapted,False))
            commands.append(f'explicitrows 0 {start} {data.hex()or"-"} {count}:{width}:{a["declared_rows"]}:{ad["values_hex"]or"-"}')
        expected.append(wanted);metadata.append({'file':a['file'],'id':a['id'],'serial':serial,'sequence_range':[a['sequence'],b['sequence']]})
    actual=replay(driver,commands);require(len(actual)==len(expected),'shape reader replay count');results=[]
    for got,wanted,meta in zip(actual,expected,metadata):
        differences=[];differences_at(got,wanted,'shape_reader',differences);results.append({**meta,'original':wanted,'matched':not differences,'differences':differences})
    return {'status':'pass'if all(r['matched']for r in results)else'fail','count':len(results),'calls':results,'scope':'Raw mode4 header, direct material integer headers and explicit rows seeded solely from captured native before-storage; exact status/cursor/partial row state.'}


def verify_shape_lifecycle(imports,events):
    pairs=shape_pairs(events);summaries=[];queries=[];query_stack=[];storage_stack=[];storage_pairs=[]
    for event in events:
        op=event['operation']
        if op=='material-shape-query-before':query_stack.append(event)
        elif op=='material-shape-query-after':
            require(query_stack,'material feature query return without entry');before=query_stack.pop()
            require(all(before[k]==event[k]for k in ('root','feature','output_pointer'))and event['feature']in (54,55),'material feature query identity')
            material_list(before['materials'])
            if event['enabled']and int(event['output_pointer'],16):require(type(event['output'])is int and event['output']>0,'enabled feature writes positive width')
            else:require(event['output']is None,'query output read only when assigned')
            queries.append({'file':event['file'],'feature':event['feature'],'enabled':event['enabled'],'output':event['output'],'sequence_range':[before['sequence'],event['sequence']]})
        elif op=='material-shape-storage-before':storage_stack.append(event)
        elif op=='material-shape-storage-after':
            require(storage_stack,'shape storage return without entry');before=storage_stack.pop()
            require(all(before[k]==event[k]for k in ('serial','object','component','kind','requested_width')),'shape storage paired identity')
            require(before['descriptor']['address']==event['descriptor']['address'],'shape storage descriptor identity')
            if event['kind']=='getter':require(event['pointer']==event['descriptor']['bound_pointer']==event['descriptor']['storage_pointer'],'shape getter returns rebound physical storage')
            storage_pairs.append((before,event))
    require(not query_stack and not storage_stack,'complete shape queries/storage calls')
    for item in imports:
        for handler in item['handlers']:
            identifier=handler['entry']['id']
            if identifier not in (210,211):continue
            obs=handler['events'];readers=[e for e in obs if e['operation']=='material-shape-reader-after'];require(len(readers)==1,'one shape consumer helper')
            final=readers[0];pair=pairs[final['serial']];before=pair['before'];component={210:54,211:55}[identifier]
            require(before['component']==final['component']==component,'shape component dispatch')
            require(final['completion_rva']=={210:0x39354f,211:0x3935a0}[identifier],'shape native caller continuation')
            require(handler['exit']['status']==(0 if final['status']==0 else 2),'shape helper status maps2')
            obj_before=next(o for o in handler['entry']['state']['objects']if o['address']==before['object'])
            obj_after=next(o for o in handler['exit']['state']['objects']if o['address']==before['object'])
            scans=[e for e in pair['events']if e['operation']=='material-shape-header-scan'];scan=scans[0]if scans else None
            vals=scan['values']if scan else[0,0,0,0.0];conversions=scan['conversions']if scan else-1
            children=[p for p in pairs.values()if p['before'].get('parent_serial')==before['serial']]
            stores=[(a,b)for a,b in storage_pairs if before['sequence']<a['sequence']<final['sequence']]
            valid=vals[2]>0 and conversions>=1
            require(final['root_width']==(vals[2]if valid else before['root_width']),'shape positive root width assignment before selector validation')
            require(handler['exit']['state']['components'][component]==final['root_width'],'shape root width retained at handler exit')
            if not valid or vals[1]!=0:
                require(final['status']==1 and not stores and not children,'shape rejected header/selector before allocation or rows')
                require(before['descriptor']==final['descriptor'],'shape rejected header preserves descriptor')
                for key in ('saved_shape_width_0xadc','saved_texture_width_0xae0'):require(before[key]==final[key],'rejected shape header retains saved widths')
            else:
                require(len(children)==1 and [a['kind']for a,b in stores]==['resize','getter'],'shape resize then getter then explicit rows')
                child=children[0];a,b=child['before'],child['after']
                require(a['declared_rows']==vals[0]and a['width']==a['explicit_width']==vals[2]and a['count']==before['count'],'shape detailed reader args from header/object')
                require(b['completion_rva']==0x41889c and b['status']==final['status'],'shape detailed continuation/status propagation')
                require(stores[0][0]['sequence']<stores[0][1]['sequence']<stores[1][0]['sequence']<stores[1][1]['sequence']<a['sequence'],'shape allocation/default/read order')
                getter=stores[1][1];initial=getter['descriptor']['values_hex'];extent=before['count']*vals[2]
                require(extent<=getter['descriptor']['length'],'shape initialized destination extent')
                wanted=(scan['output_hex'][3]*extent+initial[extent*16:])if conversions>3 else initial
                require(a['descriptor']['values_hex']==wanted,'shape assigned scalar broadcasts; missing default retains getter data')
                require(b['descriptor']['values_hex']==final['descriptor']['values_hex'],'partial explicit row mutations survive helper return')
                for key in ('saved_shape_width_0xadc','saved_texture_width_0xae0'):
                    selected=key==('saved_shape_width_0xadc'if component==54 else'saved_texture_width_0xae0')
                    wanted_saved=vals[2]if selected and obj_before['element_allocation_count']>0 else before[key]
                    require(final[key]==obj_after[key]==wanted_saved,'shape saved width updated only by positive-count selected resize')
            native=next(d for d in obj_after['descriptors']if d['id']==component)
            require(native==final['descriptor'],'shape final descriptor reaches handler snapshot')
            summaries.append({'file':item['file'],'id':identifier,'status':final['status'],'root_width_before':before['root_width'],'root_width_after':final['root_width'],'declared_rows':vals[0],'selector':vals[1],'header_conversions':conversions,'detailed_rows':len(children),'storage_calls':len(stores)})
    return {'consumers':summaries,'feature_queries':queries,'storage_calls':len(storage_pairs),'scope':'Original mutation order, direct-default writes, failed-header width divergence, saved container widths, allocation identity and exact row storage; full replacement snapshots compared separately.'}


def compare(args):
    driver=args.driver.resolve(strict=True);reader=args.reader_driver.resolve(strict=True)
    candidate=verify_candidate(driver,reader,args.require_frozen_candidate,args.freeze)
    freeze=read(args.freeze)
    for entry in freeze.get('inputs',[]):
        require(digest(ROOT/entry['archive'])==entry['sha256'],'frozen input archive identity')
    manifest=read(args.cases)
    for source in manifest.get('source_bindings',[]):
        require(digest(ROOT/source['path'])==source['sha256'],'fixture static-source binding')
    template=(ROOT/manifest['probe']['path']).resolve(strict=True)
    evidence,run,events,source_files,sources=provenance(args.capture.resolve(),args.run_record.resolve(),None,template)
    require([Path(c['path']).name for c in manifest['cases']]==run['ordered_imports'],'predeclared fixture order')
    require(all(manifest['probe'][key]==value for key,value in binding(template).items()),'predeclared probe binding')
    for case in manifest['cases']:
        path=ROOT/case['path'];require(path.resolve()==source_files[path.name]and digest(path)==case['sha256'],
                                     'predeclared fixture identity')
    input_freeze_path=args.input_freeze
    input_freeze_binding=None
    if input_freeze_path is not None:
        inputs_freeze=read(input_freeze_path)
        for key,path in [('candidate_manifest',args.freeze),('fixture_manifest',args.cases)]:
            require((ROOT/inputs_freeze[key]['path']).resolve()==path.resolve()
                    and digest(path)==inputs_freeze[key]['sha256'],'held-out freeze '+key+' binding')
            if 'archive'in inputs_freeze[key]:
                require(digest(ROOT/inputs_freeze[key]['archive'])==inputs_freeze[key]['sha256'],
                        'held-out archived '+key+' identity')
        require(inputs_freeze['candidate_current_all_match']is True
                and all(entry['matches_freeze']for entry in inputs_freeze['candidate_current']),
                'held-out freeze candidate observation')
        require([entry['path']for entry in inputs_freeze['candidate_current']]==
                [entry['path']for entry in freeze['files']],'held-out candidate inventory')
        require([(entry['path'],entry['sha256'])for entry in inputs_freeze['inputs']]==
                [(entry['path'],entry['sha256'])for entry in manifest['cases']]+[
                    (manifest['probe']['path'],manifest['probe']['sha256'])], 'held-out frozen input inventory')
        for entry in inputs_freeze['inputs']:
            require(digest(ROOT/entry['archive'])==digest(ROOT/entry['path'])==entry['sha256'],
                    'held-out input archive and current identity')
        related=list(inputs_freeze.get('native_source_bindings',[]))
        for key in ('original_input_manifest','repair_diagnostic'):
            if key in inputs_freeze:related.append(inputs_freeze[key])
        for entry in related:
            require(digest(ROOT/entry['path'])==entry['sha256'],'input freeze repair/source binding')
        input_freeze_binding=binding(input_freeze_path)
    decode_raw_controls(events);imports=retained_imports(events,run['ordered_imports'],source_files)
    temporary_copies=verify_temporary_copies(events)
    reference_lifecycle=verify_reference_lifecycle(events)
    column_commits=verify_column_commits(imports)
    column_remaps=verify_nonzero_column_remaps(events)
    field_commits=verify_field_commits(imports)
    dynamic_lifecycle=verify_dynamic_lifecycle(imports)
    shape_lifecycle=verify_shape_lifecycle(imports,events)
    custom_lifecycle=verify_custom_lifecycle(imports)
    warning_counts=Counter(t['token']for item in imports for t in item['tokens']if t['unknown_warning'])
    transcript=(args.run_record.parent/'stdout.bin').read_text(encoding='ascii')
    for token,count in warning_counts.items():
        require(transcript.count(f'WARNING: Keyword {token} not found. Skip ')==count,'unknown keyword warning transcript')
    initial=imports[0]['before'];root=initial['state']
    require(root['materials']==[]and initial['generation']==0,'empty initial material profile')
    commands=[f"initialized {root['dimension']} {root['node8_default']} {root['suppress_boundaries']}"]
    if initial['linked_state']is not None:
        require(initial['linked_state']['materials']==[]and initial['linked_generation']==0,'empty linked profile')
        commands.append(f"link {initial['linked_state']['dimension']}")
    translated=[]
    for item in imports:
        data=source_files[item['file']].read_bytes().replace(b'\r\n',b'\n')
        require(data.isascii()and b'\0'not in data and b'\x1a'not in data and len(data)<4096,'bounded ASCII text domain')
        translated.append(data);commands.append('import '+(data.hex()or'-'))
    native_stdout_path=args.run_record.resolve().parent/'stdout.bin'
    native_stdout=native_stdout_path.read_bytes().replace(b'\r\n',b'\n')
    native_stderr=(args.run_record.resolve().parent/'stderr.bin').read_bytes()
    require(not native_stderr,'original shape stderr is not empty')
    diagnostic=b'Error in TEXTURE_DATA::countNumODFs()\n'
    diagnostic_counts=Counter()
    for event in events:
        if event['operation']!='material-shape-query-before' or event['feature']!=55:continue
        records=event['materials']['records'];by_id={record['id']:record for record in records}
        for record in records:
            selected=[record]if record['mode_0x1cc8']==0 else[by_id[identifier]for identifier in record['reference_ids']if identifier in by_id]
            diagnostic_counts[event['file']]+=sum(r['field_0x2154']not in (0,1,2,3)for r in selected)
    diagnostic_count=sum(diagnostic_counts.values())
    require(native_stdout.count(diagnostic)==diagnostic_count,'native stdout diagnostic total matches per-import query55 traversal')
    actual=replay(driver,commands)
    require(len(actual)==len(imports),'import replay count')
    results=[]
    for got,item,data in zip(actual,imports,translated):
        differences=[]
        wanted={'outcome':'parsed','status':item['status'],'cursor':len(data),'reached_eof':True,
                'finalization_pending':False,'unsupported_reason':'','events':item['tokens']}
        differences_at(got.get('native_stdout_hex'),(diagnostic*diagnostic_counts[item['file']]).hex(),'native_stdout_hex',differences)
        differences_at(got.get('result'),wanted,'result',differences)
        differences_at(got.get('before_state'),item['before']['state'],'before_state',differences)
        differences_at(got.get('before_linked_state'),item['before']['linked_state'],'before_linked_state',differences)
        for key,value in item['finalized'].items():differences_at(got.get(key),value,key,differences)
        differences_at(got.get('handler_states'),[h['snapshot']for h in item['handlers']],'handler_states',differences)
        differences_at(got.get('stable_material_identity'),True,'stable_material_identity',differences)
        differences_at(got.get('stable_object_identity'),True,'stable_object_identity',differences)
        results.append({'file':item['file'],'matched':not differences,'differences':differences,
            'status':item['status'],'tokens':len(item['tokens']),'sequence_range':[item['start'],item['end']],
            'before':item['before'],'parser_state':item['parser_state'],'after':item['finalized'],
            'handlers':[{'snapshot':h['snapshot'],'original_before':h['before'],'creation':h.get('creation'),
                         'observed_operations':[e['operation']for e in h['events']]}for h in item['handlers']]})
    readers=compare_readers(events,source_files,reader)
    reference_readers=compare_reference_readers(events,source_files,reader)
    column_readers=compare_column_readers(events,source_files,reader)
    field_readers=compare_field_readers(events,source_files,reader)
    field_scans=compare_field_scans(events,source_files,reader)
    dynamic_readers=compare_dynamic_readers(events,source_files,reader)
    dynamic_scans=compare_field_scans([{**e,'operation':'field-scan'}for e in events if e['operation']=='dynamic-scan'],source_files,reader)
    custom_readers=compare_custom_readers(imports,events,source_files,reader)
    custom_scans=compare_field_scans([{**e,'operation':'field-scan'}for e in events if e['operation']=='custom-scan'],source_files,reader)
    shape_readers=compare_shape_readers(events,source_files,reader)
    shape_scans=compare_field_scans([{**e,'operation':'field-scan'}for e in events if e['operation']=='material-shape-scan'],source_files,reader)
    geometry_readers=compare_geometry_helpers(imports,source_files,reader)
    segment_readers=compare_segment_scans(imports,source_files,reader)
    return {'status':'pass'if all(r['matched']for r in results)and all(
            r['status']=='pass'for r in (readers,geometry_readers,segment_readers,reference_readers,column_readers,field_readers,field_scans,dynamic_readers,dynamic_scans,custom_readers,custom_scans,shape_readers,shape_scans))else'fail',
        'provenance':sources,'candidate':candidate,'cases':binding(args.cases),'input_freeze':input_freeze_binding,
        'driver':binding(driver),'reader_driver':binding(reader),'comparator':binding(Path(__file__)),
        'dependencies':[binding(ROOT/'tools'/name)for name in ('compare_geometry_segments.py',
            'compare_keyword_import.py','compare_mesh_refresh.py','compare_mesh_storage.py',
            'verify_mesh_array_observations.py','capture_oracle.py')],
        'imports':results,'tokens':sum(r['tokens']for r in results),'reader_comparison':readers,
        'geometry_reader_comparison':geometry_readers,'segment_reader_comparison':segment_readers,
        'reference_reader_comparison':reference_readers,'column_reader_comparison':column_readers,
        'shape_lifecycle':shape_lifecycle,'shape_reader_comparison':shape_readers,'shape_scalar_comparison':shape_scans,
        'custom_lifecycle':custom_lifecycle,'custom_reader_comparison':custom_readers,'custom_scalar_comparison':custom_scans,
        'dynamic_lifecycle':dynamic_lifecycle,'dynamic_reader_comparison':dynamic_readers,'dynamic_scalar_comparison':dynamic_scans,
        'field_reader_comparison':field_readers,'field_scalar_comparison':field_scans,'field_commits':field_commits,
        'temporary_copy_observations':temporary_copies,
        'reference_lifecycle':reference_lifecycle,'column_commits':column_commits,'column_remaps':column_remaps,
        'observed_unknown_warnings':dict(warning_counts),
        'descriptor_snapshots':sum(len(r['after']['state']['objects'])*53 for r in results),
        'coverage':{
            'predeclared_splits':dict(Counter(case.get('split','unspecified')for case in manifest['cases'])),
            'producer_calls':len(column_commits['producers']),'group_calls':len(column_commits['groups']),
            'nonzero_original_remap_checks':column_remaps['nonzero_source_checks'],
            'complete_post_handler_snapshots':sum(len(r['handlers'])for r in results),
            'failed_connectivity_handlers':[{'file':r['file'],'status':h['snapshot']['status'],
                'dimension':h['snapshot']['dimension']}for r in results for h in r['handlers']
                if h['snapshot']['id']==173 and h['snapshot']['status']!=0],
            'spatial_final_object_snapshots_with_elements':sum(obj['elements']>0 for r in results
                if r['after']['state']['dimension']==3 for obj in r['after']['state']['objects']),
            'planar_final_object_snapshots_with_elements':sum(obj['elements']>0 for r in results
                if r['after']['state']['dimension']==2 for obj in r['after']['state']['objects'])},
        'texture_diagnostic':{'literal_hex':diagnostic.hex(),'channel':'stdout','original_count':diagnostic_count,'per_import_original_counts':dict(diagnostic_counts),'original_stdout':binding(native_stdout_path),'comparison':'exact per-import native_stdout_hex in import differences; raw original transcript bound, CRLF normalized only for matching this literal','original_stderr_hex':native_stderr.hex()},
        'original_operations':dict(Counter(e['operation']for e in events)),
        'static_source_bindings':manifest.get('source_bindings',[]),
        'limitations':['SIZEMD127/TXTURE128 and SIZESH210/TXTODF211 plus previously recovered setup; bounded initialized physical allocations and finite ASCII fixtures only.',
            'Custom descriptor identities and linked-list ownership are checked in the captured owned-object domain. This does not establish behavior for externally shared custom collections or arbitrary copy-on-write aliasing.',
            'DYNARY atof input tokens are observed, but the probe does not directly capture its floating return. Reader default values are checked where native default-fill storage exposes them; all final typed storage is compared as exact raw bytes.',
            'Legacy decimal conversion outside the declared finite fixture slice, allocation failure, negative extents, first undefined row-ID conversion and native out-of-bounds writes remain unsupported.',
            'Native list links, owner pointers, scratch ranks and record identities are checked within each generation. Physical reference extents and retained values are compared; native pointer addresses are not replacement ABI requirements.',
            'Original creation refresh ordering is validated; replacement comparison observes resulting component tables and records, not an internal refresh callback.',
            'Full material property tables, externally shared ownership, allocation failures, unrecovered property features and selector conversion0 are outside this milestone.',
            'Primary and linked roots are compared before/after imports and after every selected handler. Native handler-entry snapshots are retained and checked for identity; replacement observer does not expose separate entry callbacks.',
            'Persistent root snapshots are normalized separately from native temporary copy traffic. Temporary addresses, descriptor histories and reference counts are not required to equal persistent source records.',
            'Known binary64 buffers compare exact bits; uninitialized grown direct-buffer tails stay null. Full modeled mesh, descriptors, selected geometry and materials are compared after handlers, not every internal native instruction.',
            'Fixture splits and observed IDs/dimensions are reported; unseen scanner inputs, undefined locals and unsafe native allocation extents are excluded.',
            'Only replacement replay drivers execute; expected state is normalized from captured original observations.']}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--driver',type=Path,default=ROOT/'build/Release/keyword_import_replay.exe')
    parser.add_argument('--reader-driver',type=Path,default=ROOT/'build/Release/keyword_reader_replay.exe')
    parser.add_argument('--capture',type=Path,default=ROOT/'analysis/material-shape-runtime-training.json')
    parser.add_argument('--run-record',type=Path,default=ROOT/'analysis/oracle/material-shape-training/run.json')
    parser.add_argument('--cases',type=Path,default=ROOT/'analysis/material-shape-evaluation-cases.json')
    parser.add_argument('--freeze',type=Path,default=ROOT/'analysis/material-shape-candidate-freeze.json')
    parser.add_argument('--input-freeze',type=Path)
    parser.add_argument('--report',type=Path)
    parser.add_argument('--require-frozen-candidate',action='store_true')
    args=parser.parse_args();report=compare(args)
    if args.report:args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(f"{report['status'].upper()}: {len(report['imports'])} imports, {report['tokens']} tokens, "
          f"{report['reader_comparison']['count']} reader calls")
    return 0 if report['status']=='pass'else 1


if __name__=='__main__':raise SystemExit(main())

