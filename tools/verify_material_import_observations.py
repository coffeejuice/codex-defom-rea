"""Original-only verification of bounded MTNAME name/default material lifecycle."""
import argparse
from collections import Counter
from copy import deepcopy
import json
from pathlib import Path
import re
import struct

from capture_oracle import digest
from verify_boundary_sort_observations import ROOT, EXE_SHA, binding, check, read, verify_files
from verify_step_distance_observations import line_read
from verify_time_controls_observations import stream_position

def provenance(evidence,run,manifest,freeze,directory):
    c=evidence['normalized_result'];p=evidence['parameters']
    check(run['status']=='completed'and run['exit_code']==0 and Path(run['cwd'])==directory,'completed native run')
    check(Path(run['executable'])==ROOT/'source/DEF_PRE_64.exe'
          and digest(Path(run['executable']))==run['executable_sha256']==EXE_SHA,'original target hash')
    check(c['pid']==p['pid']==run['pid']and c['session_id']==p['session_id']
          and c['target']==f"DEF_PRE_64.exe (pid {run['pid']})",'process/session identity')
    check(c['mode']==p['mode']=='attach'and c['device_id']==p['device_id']=='local'
          and c['state']=='detached'and c['source_kind']=='file'
          and not c['messages_truncated']and c['cleanup_error']is None,'complete passive capture')
    verify_files(run,directory)
    check(sum((directory/n).stat().st_size for n in ['stdout.bin','stderr.bin'])<run['max_output_bytes'],
          'transcript below output truncation bound')
    pid=read(directory.with_suffix('.pid.json'))
    check(all(pid[k]==run[k]for k in ['pid','cwd','executable','executable_sha256','ready_gate']), 'harness PID record')
    check(pid['inputs']==[{k:x[k]for k in ['name','source','copy','sha256_before']}for x in run['inputs']],
          'PID fixture inventory')
    script=Path(c['source_path']);template=ROOT/'tools/observe_material_import.js';gate=directory.with_suffix('.ready')
    check(script in [directory.with_suffix('.js'),directory.with_name(directory.name+'-probe.js')]
          and digest(script)==c['source_sha256']and Path(run['ready_gate'])==gate,'probe identity/gate')
    check(script.read_text(encoding='utf-8-sig')==template.read_text(encoding='utf-8-sig').replace(
          '__READY_GATE__',gate.as_posix()),'exact passive probe template')
    check(manifest['probe']['path']=='tools/observe_material_import.js'
          and manifest['probe']['sha256']==digest(template),'manifest probe binding')
    names=[Path(x['path']).name for x in manifest['cases']]
    check(0<len(names)<=40 and len(names)==len(set(names)) and names==run['ordered_imports']==[x['name']for x in run['inputs']],
          'ordered bounded cases')
    inputs={}
    for case,item in zip(manifest['cases'],run['inputs']):
        source=ROOT/case['path'];copy=directory/item['name']
        check(Path(item['source'])==source and Path(item['copy'])==copy
              and digest(source)==digest(copy)==case['sha256']==item['sha256_before']
              ==item['sha256_after']==item['source_sha256_after'],'fixture source/copy hashes')
        data=source.read_bytes().replace(b'\r\n',b'\n')
        check(data.isascii()and len(data)<4096 and b'\0'not in data and b'\x1a'not in data,'bounded text domain')
        inputs[item['name']]=data
    check(run['stdin']=='\n2\n'+''.join('1\n'+n+'\n\n'for n in names)+'E\nE\nY\n','menu import protocol')
    check(freeze['files']and len({f['path']for f in freeze['files']})==len(freeze['files']),'unique candidate inventory')
    for f in freeze['files']:check(digest(ROOT/f['archive'])==f['sha256'],'frozen archive identity')
    check(freeze['inputs'] and len({f['path']for f in freeze['inputs']})==len(freeze['inputs']),
          'frozen input inventory')
    for f in freeze['inputs']:
        check(digest(ROOT/f['archive'])==f['sha256'],'frozen input archive identity')
    contract=read(ROOT/'analysis/material-import-next-contract.json')
    for b in contract['source_bindings']:
        check(digest(ROOT/b['path'])==b['sha256'],'native static source binding')
    if 'contract' in manifest:
        check(manifest['contract']==binding(ROOT/'analysis/material-import-next-contract.json'),
              'manifest material contract binding')
    return inputs


DEFAULT_FIELDS = {
    'field_0x4':0,'field_0x8':0,'field_0x17e8':0,'mode_0x1cc8':0,
    'reference_count_0x1ccc':0,'field_0x1e40':0,'field_0x2150':0,'field_0x2154':0,
    'field_0x2158':1,'field_0x215c':1,
}


def name_shape(n):
    raw=bytes.fromhex(n['name_hex'])
    check(len(raw)==n['name_length']<=64 and raw.isascii() and b'\0' not in raw,
          'bounded material name bytes')
    check(n['name_terminator_hex']=='00','material name terminator')
    return raw


def record_shape(record):
    check(type(record['id']) is int and -(2**31)<=record['id']<2**31,'material signed ID')
    check(int(record['address'],16)>0 and int(record['owner'],16)>0,'material record ownership')
    check(all(record[k]==v for k,v in DEFAULT_FIELDS.items()),'default material selected fields')
    check(record['reference_pointer_0x1cd0']=='0x0' and record['reference_ids']==[],
          'name-only material references empty')
    name_shape(record)


def semantic(record):
    return {key:record[key] for key in ['id','name_hex','name_length','name_terminator_hex',
                                       *DEFAULT_FIELDS,'reference_ids']}


def default_record(identifier):
    return {'id':identifier,'name_hex':'','name_length':0,'name_terminator_hex':'00',
            **DEFAULT_FIELDS,'reference_ids':[]}


def material_shape(state):
    records=state['records'];count=state['count']
    check(type(count)is int and 0<=count<=16 and len(records)==count,'material list count')
    check(int(state['address'],16)>0 and int(state['owner'],16)>0,'material list owner')
    check(state['head']==(records[0]['address'] if records else '0x0')
          and state['tail']==(records[-1]['address'] if records else '0x0'),'material list head/tail')
    check(len({r['address'] for r in records})==count,'material unique record addresses')
    for i,record in enumerate(records):
        record_shape(record)
        check(record['owner']==state['owner']
              and record['next']==(records[i+1]['address'] if i+1<count else '0x0'),
              'material linked owner/next')
    scratch=state['scratch']
    if scratch['allocation_observed']:
        n=scratch['count']
        check(type(n)is int and 0<=n<=16 and len(scratch['ids'])==len(scratch['ranks'])==n,
              'scratch allocation extent')
        check(n==0 or all(int(scratch[k],16)>0 for k in ['ids_pointer','ranks_pointer']),
              'positive scratch pointer')
    else:
        check(scratch['ids_pointer']==scratch['ranks_pointer']=='0x0'
              and scratch['count'] is None and scratch['ids'] is None and scratch['ranks'] is None,
              'unallocated scratch metadata')


def root_shape(state):
    check(state['dimension'] in [2,3] and state['object_count']==0 and state['objects']==[],
          'zero-object material profile')
    check(len(state['components'])==79 and state['components'][0]==state['dimension'],
          'root component dimension')
    material_shape(state['material_list'])
    check(state['material_list']['owner']==state['address']
          and state['material_list']['root_count']==state['material_count']
          ==state['material_list']['count'],'stable root material count')
    if state['linked_state'] is not None:root_shape(state['linked_state'])


def validate_shapes(event):
    if 'state' in event:
        state=event['state']
        if 'dimension' in state:root_shape(state)
        elif 'records' in state:material_shape(state)
    if 'material_list' in event:material_shape(event['material_list'])
    if event.get('record') is not None:record_shape(event['record'])


def verify_selector(event,data):
    check(event['id']==92 and event['format']=='%i','MTNAME selector identity')
    start=stream_position(event['before'],data);pos=start
    while pos<len(data) and data[pos] in b' \t\r\n\v\f':pos+=1
    if pos==len(data):
        check(event['conversions']==-1 and event['value'] is None and event['output_hex'] is None,
              'selector EOF leaves destination unobserved')
        check(event['after']=={'cursor':0,'count':0,'buffer_hex':''},'selector EOF reset')
        return None
    match=re.match(rb'[+-]?(?:0[xX][0-9a-fA-F]+|0[0-7]*|[1-9][0-9]*)',data[pos:])
    check(match is not None,'selector matching failure outside deterministic profile')
    token=match[0];unsigned=token.lstrip(b'+-')
    base=16 if unsigned.lower().startswith(b'0x') else 8 if unsigned.startswith(b'0') else 10
    value=int(token,base);end=pos+len(token)
    check(-(2**31)<=value<2**31 and event['conversions']==1 and event['value']==value
          and event['output_hex']==struct.pack('<i',value).hex(),'selector exact numeric prefix')
    check(event['after']=={'cursor':end,'count':len(data)-end,'buffer_hex':data.hex()}
          or end==len(data) and event['after']=={'cursor':0,'count':0,'buffer_hex':''},
          'selector cursor/remaining bytes')
    return value


def verify_name(event,data,previous):
    check(event['capacity']==65 and event['scans']==[],'material name helper capacity/no sscanf')
    check(name_shape(event['previous_name'])==bytes.fromhex(previous['name_hex']),
          'name prior bytes')
    position=stream_position(event['before'],data)
    expected_lines=[];status=0;value=bytes.fromhex(previous['name_hex'])
    for attempt in range(2):
        line,position,eof,after=line_read(data,position)
        expected_lines.append({'limit':1024,'eof':eof,'hex':None if eof else line.hex()})
        if eof:status=1;break
        if attempt==1:value=line.split(b'\n',1)[0][:64].rstrip(b'\0 \n\r')
    check(event['lines']==expected_lines and event['after']==after,'exact two-line name consumption')
    check(event['status']==status and name_shape(event['name'])==value,
          'name trim/truncate/failure-preservation result')
    return value,status


def verify_handler(events,data):
    before=events[0];after=events[-1]
    check(before['operation']=='handler-before' and after['operation']=='handler-after'
          and before['id']==after['id']==92 and before['category']==after['category']==4,
          'paired MTNAME handler')
    selectors=[e for e in events if e['operation']=='material-selector']
    check(len(selectors)==1 and selectors[0]['before']==before['stream'],'single selector before mutation')
    selector=selectors[0];identifier=verify_selector(selector,data)
    prior=before['state']['material_list'];final=after['state']['material_list']
    expected=[semantic(r) for r in prior['records']]
    core=[e for e in events if e['operation'] in ('material-lookup','material-scratch-before',
        'material-scratch-after','material-append-before','material-append-after',
        'material-refresh-before','material-refresh-after','material-name')]
    if identifier is None:
        check(core==[] and after['status']==2 and final==prior,'EOF selector no material mutation')
        check(after['stream']==selector['after'],'EOF handler cursor')
        return {'id':None,'created':False,'status':2,'name_read':False}
    lookups=[e for e in core if e['operation']=='material-lookup']
    core=[e for e in core if e['operation']!='material-lookup' or e is lookups[0]]
    found=next((i for i,r in enumerate(expected) if r['id']==identifier),None)
    created=found is None
    operations=[e['operation'] for e in core]
    wanted=['material-lookup']+(['material-scratch-before','material-scratch-after',
        'material-append-before','material-append-after','material-refresh-before',
        'material-refresh-after','material-scratch-before','material-scratch-after'] if created else [])+['material-name']
    check(operations==wanted,'lookup append scratch refresh name ordering')
    lookup=core[0]
    check(lookup['list']==prior['address'] and lookup['requested']==identifier,'lookup request/list')
    if created:
        check(lookup['result']=='0x0' and lookup['record'] is None,'missing-ID exact lookup')
        map_before,map_after,append_before,append_after,refresh_before,refresh_after,clear_before,clear_after=core[1:9]
        check(map_before['mode']==map_after['mode']==0 and clear_before['mode']==clear_after['mode']==1,
              'scratch allocate/free modes')
        check([e['requested'] for e in lookups[1:]]==[r['id'] for r in prior['records']],
              'rank-map lookups in old insertion order')
        for e,r in zip(lookups[1:],prior['records']):
            check(map_before['sequence']<e['sequence']<map_after['sequence']
                  and e['record']==r and e['result']==r['address'] and e['list']==prior['address'],
                  'nested rank lookup identity/interval')
        ids=[r['id'] for r in expected];ranks=[sum(other<identifier for other in ids) for identifier in ids]
        scratch=map_after['state']['scratch']
        check(scratch['allocation_observed'] and scratch['count']==len(ids)
              and scratch['ids']==ids and scratch['ranks']==ranks,'old insertion IDs and sorted ranks')
        check(map_before['state']==prior,'scratch begins from handler list')
        check(append_before['state']==map_after['state'],'map established before append')
        appended=append_after['state']
        check(appended['count']==len(expected)+1 and appended['root_count']==prior['root_count']
              and [semantic(r) for r in appended['records']]==expected+[default_record(0)],
              'append default record before selector/count assignment')
        check(appended['records'][-1]['address']==append_after['result'],'append returned new tail')
        expected.append(default_record(identifier));found=len(expected)-1
        for refresh in [refresh_before,refresh_after]:
            check(refresh['option']==1 and [semantic(r) for r in refresh['state']['material_list']['records']]==expected,
                  'ID/count assignment and refresh precede name')
            check(refresh['state']['material_list']['scratch']==scratch,'refresh uses old allocation extents')
        expected_components=list(refresh_before['state']['components'])
        # Native1d0320 refresh-only writes1680/1684/16f4 from reset control
        # fields11c4/12b4/123c. These default-profile fields are zero; the
        # constructor layout leaves the three component slots at one.
        for index in (34,35,63):expected_components[index]=0
        check(expected_components==refresh_after['state']['components'],
              'default material refresh component transition')
        check(clear_before['state']==refresh_after['state']['material_list']
              and not clear_after['state']['scratch']['allocation_observed'], 'scratch freed after refresh')
        check([semantic(r) for r in clear_after['state']['records']]==expected,'scratch clear preserves records')
        layouts=[e for e in events if e['operation'].startswith('material-layout-')]
        check([e['operation'] for e in layouts]==['material-layout-before','material-layout-after']
              and refresh_before['sequence']<layouts[0]['sequence']<layouts[1]['sequence']<refresh_after['sequence'],
              'nested component refresh ordering')
        layout_after=deepcopy(layouts[0]['state']);layout_after['components']=expected_components
        check(layout_after==layouts[1]['state'],'zero-object layout changes only recovered default slots')
    else:
        check(len(lookups)==1,'rename only exact-ID lookup')
        check(lookup['result']==prior['records'][found]['address']
              and lookup['record']==prior['records'][found],'lookup reuses existing identity')
        check(not any(e['operation'].startswith('material-layout-') for e in events),
              'rename does not refresh components')
    name=core[-1]
    check(name['before']==selector['after'],'name follows selector without file consumption by refresh')
    check(name['destination']==hex(int(final['records'][found]['address'],16)+0xc),
          'name destination selected record')
    value,status=verify_name(name,data,expected[found])
    expected[found].update(name_hex=value.hex(),name_length=len(value))
    check([semantic(r) for r in final['records']]==expected,'handler complete material mutation')
    check(after['status']==(2 if status else 0) and after['stream']==name['after'],
          'name status mapping and handler cursor')
    for i,record in enumerate(prior['records']):
        check(record['address']==final['records'][i]['address'],'material stable identity')
    return {'id':identifier,'created':created,'status':after['status'],'name_read':True,
            'name_status':status,'name_hex':value.hex(),
            'component_changes':[] if not created else [{'index':i,'before':x,'after':y}
                for i,(x,y) in enumerate(zip(refresh_before['state']['components'],refresh_after['state']['components'])) if x!=y]}


def verify_token(event,data):
    check(event['format']=='%39s','material fixture token scanner format')
    before=event['before']
    pos=0 if before['buffer_hex'] is None else stream_position(before,data)
    while pos<len(data) and data[pos] in b' \t\n\r\v\f':pos+=1
    if pos==len(data):
        check(event['conversions']==-1 and event['value'] is None
              and event['after']=={'cursor':0,'count':0,'buffer_hex':''},'token EOF')
        return
    token=re.match(rb'\S{1,39}',data[pos:])[0];end=pos+len(token)
    check(event['conversions']==1 and event['value']==token.decode('ascii')
          and (event['after']=={'cursor':end,'count':len(data)-end,'buffer_hex':data.hex()}
               or end==len(data) and event['after']=={'cursor':0,'count':0,'buffer_hex':''}),
          'token bytes/cursor')


def validate(evidence,inputs):
    capture=evidence['normalized_result'];messages=capture['messages']
    check(len(capture['scripts'])==1 and all(m['type']=='send' and m['script_id']==capture['scripts'][0]['script_id']
          for m in messages),'single script no errors')
    check(messages[0]['payload']['operation']=='hooks-installed'
          and messages[0]['payload']['pid']==capture['pid'],'hook installation identity')
    events=[m['payload'] for m in messages[1:]]
    check([e['sequence'] for e in events]==list(range(1,len(events)+1)),'continuous event sequence')
    check(not any(e['operation']=='probe-error' for e in events),'no passive probe errors')
    groups=[];current=None
    for event in events:
        validate_shapes(event)
        if event['operation']=='import-before':
            check(current is None,'non-nested import')
            current=[event]
        else:
            check(current is not None and event['file']==current[0]['file'],'event import ownership')
            current.append(event)
            if event['operation']=='import-after':groups.append(current);current=None
    check(current is None and [g[0]['file'] for g in groups]==list(inputs),'complete ordered imports')
    previous=None;results=[]
    for group in groups:
        before,after=group[0],group[-1];data=inputs[before['file']]
        if previous is not None:check(before['state']==previous,'state continuity between imports')
        else:check(before['state']['material_count']==0,'fresh empty material state')
        previous=after['state']
        handlers=[];active=None;finals={};stages=[];clears=[];destroyed=[];last_token=None
        for event in group[1:-1]:
            op=event['operation']
            if active is not None:active.append(event)
            if op=='stream-scan':verify_token(event,data);last_token=event
            if op=='keyword-resolved':
                check(last_token and last_token['value']==event['token']=='MTNAME'
                      and event['id']==92 and event['category']==4,'MTNAME resolution')
            if op=='handler-before':
                check(active is None,'non-nested material handler');active=[event]
            elif op=='handler-after':
                check(active is not None,'paired material handler return')
                handlers.append(verify_handler(active,data));active=None
            elif op in ('finalize-before','finalize-after'):
                check(op not in finals,'single wrapper finalization');finals[op]=event
            elif op in ('stage-before','stage-after'):
                stages.append(event)
            elif op=='material-clear-before':
                clears.append({'before':event,'destroyed':[]})
            elif op=='material-destroy-before':
                check(clears,'destructor inside material list clear')
                clears[-1]['destroyed'].append(event['record'])
                destroyed.append(event)
            elif op=='material-destroy-after':
                check(destroyed and destroyed[-1]['record']['address']==event['address'],
                      'paired record destructor')
                destroyed.pop()
            elif op=='material-clear-after':
                check(clears,'paired material list clear')
                clear=clears.pop();old=clear['before']['state'];new=event['state']
                check(old['address']==new['address'] and clear['destroyed']==old['records'],
                      'destruction insertion order and exact records')
                check(new['records']==[] and new['count']==0,'material list cleared after destruction')
        check(active is None and not clears and not destroyed,'completed handlers/destruction')
        check(set(finals)=={'finalize-before','finalize-after'},'complete finalization')
        pre,post=finals['finalize-before'],finals['finalize-after']
        check(pre['sequence']<post['sequence'] and pre['finalizer_serial']==post['finalizer_serial'],
              'finalizer identity/order')
        check([(e['operation'],e['stage']) for e in stages]==[
            ('stage-before','control'),('stage-after','control'),('stage-before','materials'),
            ('stage-after','materials'),('stage-before','objects'),('stage-after','objects')],
            'control material object stage ordering')
        check(all(pre['sequence']<e['sequence']<post['sequence'] and e['root']==pre['state']['address']
              and e['finalizer_serial']==pre['finalizer_serial'] for e in stages),'stage ownership interval')
        check(pre['state']==post['state']==after['state'],'default material finalization no-op')
        check(all(e['material_list']==pre['state']['material_list'] for e in stages),
              'material finalization preserves selected default records')
        statuses=[h['status'] for h in handlers if h['status']!=0]
        check(after['status']==(statuses[-1] if statuses else 0),'last nonzero handler status')
        resets=[e for e in group if e['operation'].startswith('flags-reset-')]
        check([e['operation'] for e in resets]==['flags-reset-before','flags-reset-after']
              and resets[-1]['sequence']<pre['sequence'],'wrapper flags reset before parse/finalize')
        for e in resets:
            check(e['import_flag_0x2f88']==e['import_flag_0x2f89']==0,'MTNAME flags stay clear')
        # Fixture header is an input fact, not a replacement-generated expectation.
        headers=re.findall(rb'^\* DEFORM-([23])D(?:\r?\n|$)',data,re.M)
        dimension=int(headers[0]) if headers else before['state']['dimension']
        check(after['state']['dimension']==dimension,'header dimension request')
        changed=dimension!=before['state']['dimension']
        if changed:
            check(any(e['operation']=='material-clear-before' for e in group),
                  'dimension change invokes material destruction')
            first_handler=next((e for e in group if e['operation']=='handler-before'),pre)
            check(first_handler['state']['material_count']==0,'dimension reset empties material list before handlers')
        elif not handlers:
            check(before['state']['material_list']==after['state']['material_list'],
                  'same dimension preserves material list')
        check(before['state']['address']==after['state']['address'],'stable primary root identity')
        linked=after['state']['linked_state']
        if linked is not None:
            check(before['state']['linked_state'] is not None
                  and linked['address']==before['state']['linked_state']['address']
                  and linked['dimension']==5-dimension,'linked root identity/opposite dimension')
            check(linked['material_count']==0 and linked['material_list']['records']==[],
                  'normal dispatch linked root remains material-empty')
        results.append({'file':before['file'],'handlers':handlers,'status':after['status'],
            'before_dimension':before['state']['dimension'],'dimension':dimension,'changed_dimension':changed,
            'material_ids':[r['id'] for r in after['state']['material_list']['records']],
            'names_hex':[r['name_hex'] for r in after['state']['material_list']['records']],
            'refreshes':sum(e['operation']=='material-refresh-before' for e in group),
            'destructors':sum(e['operation']=='material-destroy-before' for e in group),
            'sequence_range':[before['sequence'],after['sequence']]})
    return {'imports':results,'event_count':len(events),
        'handler_count':sum(len(r['handlers']) for r in results),
        'created_count':sum(h['created'] for r in results for h in r['handlers'])}


def verify_input_freeze(record,manifest,manifest_path,candidate_path):
    check(record['candidate_manifest']==binding(candidate_path),'held-out candidate manifest binding')
    recorded_manifest=record['fixture_manifest']
    check({k:recorded_manifest[k] for k in ('path','sha256')}==binding(manifest_path)
          and digest(ROOT/recorded_manifest['archive'])==recorded_manifest['sha256'],
          'held-out fixture manifest binding/archive')
    check(record['candidate_current_all_match'] is True,'held-out pre-capture candidate identity')
    expected=[(item['path'],item['sha256']) for item in manifest['cases']]+[
        (manifest['probe']['path'],manifest['probe']['sha256'])]
    check([(item['path'],item['sha256']) for item in record['inputs']]==expected,
          'held-out archived input inventory')
    for item in record['inputs']:
        check(digest(ROOT/item['archive'])==digest(ROOT/item['path'])==item['sha256'],
              'held-out archive/current input identity')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--capture',type=Path,default=ROOT/'analysis/material-import-runtime-training.json')
    parser.add_argument('--run-record',type=Path,default=ROOT/'analysis/oracle/material-import-training/run.json')
    parser.add_argument('--manifest',type=Path,default=ROOT/'tests/fixtures/material_import/cases.json')
    parser.add_argument('--freeze',type=Path,default=ROOT/'analysis/material-import-candidate-freeze.json')
    parser.add_argument('--input-freeze',type=Path)
    parser.add_argument('--output',type=Path,default=ROOT/'analysis/material-import-observations.json')
    args=parser.parse_args()
    evidence,run,manifest,freeze=map(read,[args.capture,args.run_record,args.manifest,args.freeze])
    directory=args.run_record.resolve().parent
    inputs=provenance(evidence,run,manifest,freeze,directory)
    result=validate(evidence,inputs);rejected=[]
    input_freeze=None
    if args.input_freeze:
        input_freeze=read(args.input_freeze)
        verify_input_freeze(input_freeze,manifest,args.manifest,args.freeze)
        for key in ('candidate_manifest','fixture_manifest','inputs'):
            damaged=deepcopy(input_freeze)
            (damaged[key][0] if key=='inputs' else damaged[key])['sha256']='0'*64
            try:verify_input_freeze(damaged,manifest,args.manifest,args.freeze)
            except ValueError:rejected.append('held-out input freeze '+key)
            else:raise ValueError('held-out freeze tamper accepted '+key)
    def reject(label,mutate):
        damaged=deepcopy(evidence);events=[m['payload'] for m in damaged['normalized_result']['messages']]
        mutate(events)
        try:validate(damaged,inputs)
        except (ValueError,KeyError,IndexError,TypeError,struct.error):rejected.append(label)
        else:raise ValueError('tamper accepted: '+label)
    def pick(events,operation,predicate=lambda e:True):
        return next(e for e in events if e['operation']==operation and predicate(e))
    reject('record name bytes',lambda es:pick(es,'handler-after',lambda e:e['state']['material_count']>0)
           ['state']['material_list']['records'][0].update(name_hex='58',name_length=1))
    reject('record ID',lambda es:pick(es,'handler-after',lambda e:e['state']['material_count']>0)
           ['state']['material_list']['records'][0].update(id=1234))
    reject('list count',lambda es:pick(es,'handler-after')['state']['material_list'].update(count=16))
    reject('name terminator',lambda es:pick(es,'material-name')['name'].update(name_terminator_hex='41'))
    reject('name cursor',lambda es:pick(es,'material-name')['after'].update(cursor=0))
    reject('name line bytes',lambda es:pick(es,'material-name')['lines'][1].update(hex='580a'))
    def corrupt_selector(es):
        event=pick(es,'material-selector',lambda e:e['conversions']==1)
        raw=bytearray.fromhex(event['output_hex']);raw[0]^=1
        event['output_hex']=raw.hex()
    reject('selector bytes',corrupt_selector)
    reject('scratch rank',lambda es:pick(es,'material-scratch-after',lambda e:e['mode']==0 and e['state']['scratch']['count']>0)
           ['state']['scratch']['ranks'].__setitem__(0,7))
    reject('scratch extent',lambda es:pick(es,'material-scratch-after',lambda e:e['mode']==0 and e['state']['scratch']['count']>0)
           ['state']['scratch'].update(count=0))
    reject('refresh option',lambda es:pick(es,'material-refresh-before').update(option=0))
    reject('refresh components',lambda es:pick(es,'material-refresh-after')['state']['components'].__setitem__(1,17))
    reject('finalization ordering',lambda es:pick(es,'stage-before').update(stage='wrong'))
    reject('destructor record',lambda es:pick(es,'material-destroy-before')['record'].update(id=1234))
    def exchange_name_and_refresh(es):
        a=pick(es,'material-refresh-after');b=pick(es,'material-name')
        ai,bi=es.index(a),es.index(b);sa,sb=a['sequence'],b['sequence']
        es[ai],es[bi]=b,a;es[ai]['sequence']=sa;es[bi]['sequence']=sb
    reject('refresh/name ordering',exchange_name_and_refresh)
    for field,value in [('pid',-1),('source_sha256','0'*64),('messages_truncated',True)]:
        damaged=deepcopy(evidence);damaged['normalized_result'][field]=value
        try:provenance(damaged,run,manifest,freeze,directory)
        except ValueError:rejected.append('provenance '+field)
        else:raise ValueError('provenance tamper accepted '+field)
    damaged=deepcopy(manifest);damaged['cases'][0]['sha256']='0'*64
    try:provenance(evidence,run,damaged,freeze,directory)
    except ValueError:rejected.append('fixture identity')
    else:raise ValueError('fixture identity tamper accepted')
    paths=[args.capture,args.run_record,args.manifest,args.freeze,Path(__file__),
           ROOT/'tools/observe_material_import.js',ROOT/'analysis/material-import-next-contract.json',
           ROOT/'tools/verify_boundary_sort_observations.py',ROOT/'tools/verify_step_distance_observations.py',
           ROOT/'tools/verify_time_controls_observations.py',ROOT/'tools/capture_oracle.py']
    if args.input_freeze:paths.append(args.input_freeze)
    report={'status':'PASS','scope':'Original-only MTNAME selected default fields, exact names, sparse insertion, refresh ordering, scratch maps, reset and finalization in zero-object profile.',
        'evidence_id':evidence['evidence_id'],'pid':run['pid'],'result':result,'tampering_rejected':rejected,
        'bindings':[binding(path) for path in paths],
        'additional_input_archive':input_freeze,
        'candidate_current':[{'path':item['path'],'matches_freeze':digest(ROOT/item['path'])==item['sha256']} for item in freeze['files']],
        'limitations':['No replacement execution and no complete material-property/table equivalence claim.',
            'Selected default fields are checked; uninitialized name tail/padding and hidden property tables are not read.',
            'Object count remains zero; populated-object copying/material remapping and nonempty reference graphs are outside scope.',
            'Selector matching failure uses an uninitialized native local and is excluded; physical EOF is verified.',
            'Zero-size scratch pointer identity is not specified; no zero-extent bytes are read.',
            'The observed first-refresh changes at component indices34/35/63 are asserted for these reset profiles. Static1d0320 establishes their source offsets; the underlying root11c4/12b4/123c fields are not separately captured by this probe.']}
    args.output.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(f"PASS {len(inputs)} imports; {result['handler_count']} handlers; {len(rejected)} tamper checks")


if __name__=='__main__':main()
