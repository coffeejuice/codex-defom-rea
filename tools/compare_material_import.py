"""Compare the selected MTNAME material profile with retained original evidence.

Executes only replacement replay drivers. Captured names, lists, counts, defaults,
root tables and reader calls are the oracle; no original process is launched.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import struct
import subprocess

from capture_oracle import digest
from compare_keyword_import import ROOT, EXE_SHA, require, read, binding
from compare_mesh_refresh import compare_values as differences_at
from compare_geometry_segments import decode_raw_controls, normalize_table

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


def provenance(path, run_path, fixtures):
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
    template = ROOT / "tools/observe_material_import.js"
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
    require(value['count']==count<=4 and len({r['address']for r in records})==count,
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
        require(record['reference_pointer_0x1cd0']=='0x0' and record['reference_ids']==[],
                'selected reference storage defaults')
        for key in MATERIAL_FIELDS:
            if key not in ('id','name_hex','reference_ids'):
                require(record[key]==(1 if key in ('field_0x2158','field_0x215c')else 0),
                        'selected default material field '+key)
    scratch=value['scratch']
    if scratch['allocation_observed']:
        require(0<=scratch['count']<=4 and len(scratch['ids'])==len(scratch['ranks'])==scratch['count'],
                'observed scratch extent')
        require(scratch['ranks']==[sum(other<identifier for other in scratch['ids'])
                                   for identifier in scratch['ids']], 'old material rank map')
    else:
        require(scratch['ids_pointer']==scratch['ranks_pointer']=='0x0'
                and scratch['ids']is None and scratch['ranks']is None,'cleared scratch metadata')
    return [{key:r[key]for key in MATERIAL_FIELDS}for r in records]


class RootIdentity:
    def __init__(self):self.roots={}

    def normalize(self,state):
        address,dimension=state['address'],state['dimension']
        require(int(address,16)>0 and dimension in (2,3),'root identity/dimension')
        track=self.roots.setdefault(address,{'dimension':dimension,'generation':0,'materials':{}})
        if dimension!=track['dimension']:
            track.update(dimension=dimension,generation=track['generation']+1,materials={})
        require(state['object_count']==0 and state['objects']==[], 'zero-object material milestone')
        require(state['alternate_enabled']==0 and state['nstart_option_0x1740']==0,
                'normal wrapper/default NSTART option')
        require(state['node8_default']==(20 if state['process_units_raw']==0 else 68), 'node default units')
        require(len(state['components'])==79 and state['components'][0]==dimension,'root component extent')
        selected=material_list(state['material_list'])
        require(state['material_count']==len(selected),'root selected material count')
        for record in state['material_list']['records']:
            require(track['materials'].setdefault(record['id'],record['address'])==record['address'],
                    'material identity changed within dimension generation')
        output={key:state[key]for key in ('dimension','suppress_boundaries','alternate_enabled',
            'node8_default','components','geometry_mode','output_flags','finalization_controls',
            'import_flag_0x2f88','import_flag_0x2f89','object_count','objects','material_count')}
        output.update(materials=selected,time_table=normalize_table(state['time_table']))
        linked=self.normalize(state['linked_state'])if state['linked_state']is not None else None
        return {'state':output,'generation':track['generation'],
                'linked_state':linked['state']if linked else None,
                'linked_generation':linked['generation']if linked else None}


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
                    'material-clear','material-destroy'}
    allowed={'import-before','import-after','keyword-resolved','handler-before','handler-after',
             'finalize-before','finalize-after','stage-before','stage-after','flags-reset-before',
             'flags-reset-after','stream-scan','material-selector','material-name','material-lookup'}
    allowed.update(pair+'-'+side for pair in material_pairs for side in ('before','after'))
    for event in events:
        op=event['operation']
        require(op in allowed,'event outside selected material capture scope: '+op)
        if op=='import-before':
            require(current is None,'nested import')
            normalized=identities.normalize(event['state'])
            require(last is None or normalized==last,'state changed between imports')
            current={'file':event['file'],'before':normalized,'start':event['sequence'],
                     'tokens':[],'handlers':[],'observations':[],'stages':[],'resets':[]}
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
                if pair=='material-refresh':require(before['option']==event['option']==1,'refresh option')
        if op=='keyword-resolved':
            require(event['id']==92 and event['category']==4,'selected MTNAME-only keyword scope')
            current['tokens'].append({'token':event['token'],'id':92,'category':4,
                                      'handler_status':None,'unknown_warning':False})
        elif op=='handler-before':
            require(handler is None and event['id']==92 and event['category']==4,'MTNAME entry')
            handler={'entry':event,'before':identities.normalize(event['state']),'events':[]}
        elif op=='handler-after':
            require(handler is not None and event['id']==92 and event['category']==4,'MTNAME return')
            require(not stack,'unclosed material operation at handler return')
            normalized=identities.normalize(event['state']);state=normalized['state']
            current['tokens'][-1]['handler_status']=event['status']
            snapshot={key:state[key]for key in ('dimension','finalization_controls','time_table',
                'import_flag_0x2f88','import_flag_0x2f89','material_count','materials','components')}
            snapshot.update(id=92,category=4,status=event['status'],cursor=logical(event['stream'],data),
                            object_strokes=[],object_geometries=[],object_segments=[])
            handler.update(after=normalized,snapshot=snapshot,exit=event)
            verify_handler(handler)
            current['handlers'].append(handler);handler=None
        elif op=='finalize-before':
            require(handler is None and 'parser_state'not in current,'finalizer entry')
            current['parser_state']=identities.normalize(event['state'])
            current['finalizer_serial']=event['finalizer_serial']
        elif op=='finalize-after':
            require('parser_state'in current and event['finalizer_serial']==current['finalizer_serial'],
                    'paired finalizer')
            current['finalized']=identities.normalize(event['state'])
        elif op in ('stage-before','stage-after'):
            require('parser_state'in current and 'finalized'not in current,'stage inside finalizer')
            current['stages'].append((op,event['stage']))
            material_list(event['material_list'])
        elif op.startswith('flags-reset-'):current['resets'].append(event)
        elif op=='import-after':
            require(handler is None and not stack,'complete handler/lifecycle nesting')
            after=identities.normalize(event['state']);require(after==current['finalized'],'import final state')
            require(current['stages']==[(op,stage)for stage in ('control','materials','objects')
                for op in ('stage-before','stage-after')], 'ordered finalizer stages')
            require([e['operation']for e in current['resets']]==['flags-reset-before','flags-reset-after'],
                    'wrapper reset pairing')
            require(all(current['resets'][-1][key]==0 for key in ('import_flag_0x2f88','import_flag_0x2f89')),
                    'wrapper resets both flags')
            require(current['parser_state']['state']['materials']==after['state']['materials'],
                    'default-material finalizer preserves selected records')
            current.update(status=event['status'],end=event['sequence'])
            imports.append(current);last=after;current=None
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
    lookups=[e for e in events if e['operation']=='material-lookup']
    require([e['requested']for e in lookups]==[selected]+([]if existing else[r['id']for r in old]),
            'selector lookup then old-ID rank-map lookups')
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


def replay(driver,commands):
    process=subprocess.run([str(driver)],input='\n'.join(commands)+'\n',text=True,
        capture_output=True,timeout=30,creationflags=subprocess.CREATE_NO_WINDOW)
    require(process.returncode==0 and not process.stderr,'replacement replay failed: '+process.stderr)
    return [json.loads(line,parse_int=lambda text:-0.0 if text=='-0'else int(text))
            for line in process.stdout.splitlines()]


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


def compare(args):
    driver=args.driver.resolve(strict=True);reader=args.reader_driver.resolve(strict=True)
    candidate=verify_candidate(driver,reader,args.require_frozen_candidate,args.freeze)
    freeze=read(args.freeze)
    for entry in freeze.get('inputs',[]):
        require(digest(ROOT/entry['archive'])==entry['sha256'],'frozen input archive identity')
    evidence,run,events,source_files,sources=provenance(args.capture.resolve(),args.run_record.resolve(),None)
    manifest=read(args.cases)
    require([Path(c['path']).name for c in manifest['cases']]==run['ordered_imports'],'predeclared fixture order')
    require(manifest['probe']==binding(ROOT/'tools/observe_material_import.js'),'predeclared probe binding')
    for case in manifest['cases']:
        path=ROOT/case['path'];require(path.resolve()==source_files[path.name]and digest(path)==case['sha256'],
                                     'predeclared fixture identity')
    input_freeze_path=args.input_freeze
    if input_freeze_path is None and args.cases.name=='material-import-held-out-cases.json':
        input_freeze_path=ROOT/'analysis/material-import-held-out-input-freeze.json'
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
        input_freeze_binding=binding(input_freeze_path)
    decode_raw_controls(events);imports=retained_imports(events,run['ordered_imports'],source_files)
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
    actual=replay(driver,commands);require(len(actual)==len(imports),'import replay count')
    results=[]
    for got,item,data in zip(actual,imports,translated):
        differences=[]
        wanted={'outcome':'parsed','status':item['status'],'cursor':len(data),'reached_eof':True,
                'finalization_pending':False,'unsupported_reason':'','events':item['tokens']}
        differences_at(got.get('result'),wanted,'result',differences)
        differences_at(got.get('before_state'),item['before']['state'],'before_state',differences)
        differences_at(got.get('before_linked_state'),item['before']['linked_state'],'before_linked_state',differences)
        for key,value in item['finalized'].items():differences_at(got.get(key),value,key,differences)
        differences_at(got.get('handler_states'),[h['snapshot']for h in item['handlers']],'handler_states',differences)
        differences_at(got.get('stable_material_identity'),True,'stable_material_identity',differences)
        results.append({'file':item['file'],'matched':not differences,'differences':differences,
            'status':item['status'],'tokens':len(item['tokens']),'sequence_range':[item['start'],item['end']],
            'before':item['before'],'after':item['finalized'],
            'handlers':[{'snapshot':h['snapshot'],'creation':h.get('creation'),
                         'observed_operations':[e['operation']for e in h['events']]}for h in item['handlers']]})
    readers=compare_readers(events,source_files,reader)
    return {'status':'pass'if all(r['matched']for r in results)and readers['status']=='pass'else'fail',
        'provenance':sources,'candidate':candidate,'cases':binding(args.cases),'input_freeze':input_freeze_binding,
        'driver':binding(driver),'reader_driver':binding(reader),'comparator':binding(Path(__file__)),
        'dependencies':[binding(ROOT/'tools'/name)for name in ('compare_geometry_segments.py',
            'compare_keyword_import.py','compare_mesh_refresh.py','capture_oracle.py')],
        'imports':results,'tokens':sum(r['tokens']for r in results),'reader_comparison':readers,
        'original_operations':dict(Counter(e['operation']for e in events)),
        'limitations':['Selected name-only material defaults, zero objects, normal wrapper, finite ASCII fixtures only.',
            'Native list links, owner pointers, scratch ranks and record identities are checked within each generation; native addresses and allocation capacities are not replacement ABI requirements.',
            'Original creation refresh ordering is validated; replacement comparison observes resulting component tables and records, not an internal refresh callback.',
            'Full material property tables, shared ownership, allocation failures, nonempty reference graphs, active-object remapping and selector conversion0 are outside this milestone.',
            'Primary and linked roots are compared before and after each import; immediate handler snapshots cover the primary root.',
            'Only replacement replay drivers execute; expected state is normalized from captured original observations.']}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--driver',type=Path,default=ROOT/'build/Release/keyword_import_replay.exe')
    parser.add_argument('--reader-driver',type=Path,default=ROOT/'build/Release/keyword_reader_replay.exe')
    parser.add_argument('--capture',type=Path,default=ROOT/'analysis/material-import-runtime-training.json')
    parser.add_argument('--run-record',type=Path,default=ROOT/'analysis/oracle/material-import-training/run.json')
    parser.add_argument('--cases',type=Path,default=ROOT/'tests/fixtures/material_import/cases.json')
    parser.add_argument('--freeze',type=Path,default=ROOT/'analysis/material-import-candidate-freeze.json')
    parser.add_argument('--input-freeze',type=Path)
    parser.add_argument('--report',type=Path)
    parser.add_argument('--require-frozen-candidate',action='store_true')
    args=parser.parse_args();report=compare(args)
    if args.report:args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(f"{report['status'].upper()}: {len(report['imports'])} imports, {report['tokens']} tokens, "
          f"{report['reader_comparison']['count']} reader calls")
    return 0 if report['status']=='pass'else 1


if __name__=='__main__':raise SystemExit(main())

