"""Independently verify retained original PDIE/STROKE/DSMAX_NSTEP imports; neither executable is run."""
from collections import Counter
from copy import deepcopy
import json
import argparse
import math
from pathlib import Path
import re
import struct
from capture_oracle import digest
from verify_boundary_sort_observations import ROOT, EXE_SHA, binding, check, read, verify_files
from verify_keyword_import_observations import held_out_scan
from verify_time_controls_observations import verify_real, stream_position, FLOAT_KEYS, ZERO, NEG_ZERO
from verify_root_layout_observations import header_dimension
from verify_mesh_array_observations import descriptor_map
from verify_mesh_refresh_observations import SLOTS

DIRECTORY=ROOT/"analysis/oracle/step-distance-evaluation"
CAPTURE=ROOT/"analysis/step-distance-runtime-evaluation.json"
MANIFEST=ROOT/"analysis/step-distance-evaluation-cases.json"
FREEZE=ROOT/"analysis/step-distance-candidate-freeze.json"
HELD_OUT=True
EXTRA_RUN=False
EXPECTED_IMPORTS=17
IDS={"DTTYPE":51,"DSMAX":48,"DTMAX":50,"PDIE":38,"DSMAX_NSTEP":49,"STROKE":153,"GEOTYP":28,"STNOUT":34,
     "OBJNAM":144,"OBJTYP":145,"RZ":159,"ELMCON":173}
VIEW_KEYS=("finalization_controls","time_table","time_controls_hex","import_flag_0x2f88",
           "import_flag_0x2f89","nstart_option_0x1740","material_count")

def provenance(evidence, run, manifest, freeze):
    check(run["status"] == "completed" and run["exit_code"] == 0
          and Path(run["cwd"]) == DIRECTORY, "run completion/directory")
    check(Path(run["executable"]) == ROOT / "source/DEF_PRE_64.exe"
          and digest(Path(run["executable"])) == run["executable_sha256"] == EXE_SHA,
          "original identity")
    verify_files(run, DIRECTORY)
    check(sum((DIRECTORY / n).stat().st_size for n in ("stdout.bin", "stderr.bin"))
          < run["max_output_bytes"], "output bound")
    order = [Path(c["path"]).name for c in manifest["cases"]]
    check(order == run["ordered_imports"] == [x["name"] for x in run["inputs"]]
          and len(order) == len(set(order)) == EXPECTED_IMPORTS, "fixture order")
    inputs = {}
    for case, item in zip(manifest["cases"], run["inputs"]):
        source, copy = ROOT / case["path"], DIRECTORY / item["name"]
        check(Path(item["source"]) == source and Path(item["copy"]) == copy, "fixture paths")
        check(digest(source) == digest(copy) == case["sha256"] == item["sha256_before"]
              == item["sha256_after"] == item["source_sha256_after"], "fixture hashes")
        data = source.read_bytes().replace(b"\r\n", b"\n")
        check(len(data) < 4096 and data.isascii() and b"\0" not in data and b"\x1a" not in data,
              "fixture stream scope")
        inputs[item["name"]] = data
    if not EXTRA_RUN:
        receipt=read(ROOT/'analysis/step-distance-runtime-transport-receipt.json')
        check(receipt['error']['code']=='resource_constraint'
              and receipt['error']['details']['reported_limits']['evidence_id']==evidence['evidence_id'],
              'transport receipt retained evidence identity')
        bundle=read(ROOT/'analysis/oracle/step-distance-transport-evidence.json')
        records=[r for r in bundle['records']if r.get('evidence_id')==evidence['evidence_id']]
        check(records==[evidence],'full export record equals extracted capture')
    cap = evidence["normalized_result"]
    script, gate = DIRECTORY.with_name(DIRECTORY.name + "-probe.js"), DIRECTORY.with_suffix(".ready")
    template = ROOT / "tools/observe_step_distance.js"
    check(Path(cap["source_path"]) == script and digest(script) == cap["source_sha256"]
          and Path(run["ready_gate"]) == gate
          and manifest["probe"]["path"] == "tools/observe_step_distance.js"
          and digest(template) == manifest["probe"]["sha256"], "probe binding")
    check(script.read_text(encoding="utf-8-sig") ==
          template.read_text(encoding="utf-8-sig").replace("__READY_GATE__", gate.as_posix()),
          "probe differs from passive template")
    pid = read(DIRECTORY.with_suffix(".pid.json"))
    check(pid["pid"] == run["pid"] and pid["executable_sha256"] == EXE_SHA
          and pid["executable"] == run["executable"] and pid["cwd"] == run["cwd"]
          and pid["ready_gate"] == run["ready_gate"], "harness PID binding")
    check(pid["inputs"] == [{k: x[k] for k in ("name", "source", "copy", "sha256_before")}
                             for x in run["inputs"]], "harness fixture binding")
    expected_stdin = "\n2\n" + "".join("1\n" + name + "\n\n" for name in order) + "E\nE\nY\n"
    check(run["stdin"] == expected_stdin, "harness ordered input protocol")
    check(len(freeze["files"]) == 23 and len({f["path"] for f in freeze["files"]}) == 23,
          "freeze inventory")
    for item in freeze["files"]:
        check(digest(ROOT / item["archive"]) == item["sha256"], "frozen candidate archive")
    return inputs


def real_bytes(values):
    return ''.join(struct.pack('<d',v).hex()for v in values)


def table_shape(t):
    check(t['type_0x00']==128 and t['rank_0x04']in(0,1) and t['values_per_point_0x40']==1,
          'selected type/rank/width')
    check(len(t['selectors_0x10'])==len(t['counts_0x1c'])==len(t['axes'])==3
          and t['selectors_0x10'][1:]==t['counts_0x1c'][1:]==[0,0]
          and -4<=t['counts_0x1c'][0]<=(3 if HELD_OUT else 2),'selected axis metadata')
    check(t['flag_0x50']==0 and t['mode_0x58']in(0,1) and type(t['preserved_0x54'])is int
          and len(bytes.fromhex(t['preserved_0x60_hex']))==8,'table flags/preserved fields')
    check(t['constant_0x08']==0 and t['raw_hex']['constant']==ZERO,'constant selected scope')
    check(len(t['pointers']['axes'])==len(t['raw_hex']['axes'])==3,'axis pointer/raw inventory')
    for values,raw,pointer in zip(t['axes']+[t['dependent']],
            t['raw_hex']['axes']+[t['raw_hex']['dependent']],
            t['pointers']['axes']+[t['pointers']['dependent']]):
        check(len(values)<=(3 if HELD_OUT else 2) and len(raw)==len(values)*16
              and all(math.isfinite(v)for v in values),'table payload bounds')
        check([struct.unpack('<d',bytes.fromhex(raw[i:i+16]))[0]for i in range(0,len(raw),16)]==values,
              'table raw payload agrees with JSON values')
        check((int(pointer,16)==0)==(len(values)==0),'table payload pointer/null relationship')
    live=[int(p,16)for p in t['pointers']['axes']+[t['pointers']['dependent']]if int(p,16)]
    check(len(live)==len(set(live)),'selected owned table arrays alias')


def view(s):
    return {'root':s.get('address',s.get('root')),**{k:deepcopy(s[k])for k in VIEW_KEYS}}


def check_view(s):
    check(type(s['finalization_controls']['control_unit_0x84c'])is int,'root84c observation type')
    if not EXTRA_RUN:check(s['finalization_controls']['control_unit_0x84c']==0,'predeclared root84c context')
    check(type(s['finalization_controls']['selected_object_0x8b8'])is int
          and len(bytes.fromhex(s['finalization_controls']['pdie_alias_0x1100_hex']))==8,'selection and alias shape')
    table_shape(s['time_table'])
    c,t=s['finalization_controls'],s['time_table']
    check(int(t['address'],16)==int(s.get('address',s.get('root')),16)+0x938,'table root-relative address')
    check(c['control_0x93c']==t['rank_0x04'] and c['mode_0x990']==t['mode_0x58'],
          'rank and mode root aliases')
    check(c['step_number_0x8b0']==c['saved_step_0x744']==-1
          and s['material_count']==s['nstart_option_0x1740']==s['import_flag_0x2f88']==s['import_flag_0x2f89']==0,
          'selected finalization profile')
    check(set(s['time_controls_hex'])==set(FLOAT_KEYS),'scalar byte inventory')
    for key in FLOAT_KEYS:
        raw=bytes.fromhex(s['time_controls_hex'][key])
        check(len(raw)==8 and math.isfinite(c[key]) and struct.unpack('<d',raw)[0]==c[key],
              'scalar raw value')


def state_shape(s,linked=False):
    check_view(s)
    check(s['control_unit_0x84c']==s['finalization_controls']['control_unit_0x84c'],'root unit observation consistency')
    check(s['dimension']in(2,3) and s['object_count']==len(s['objects'])<=(3 if HELD_OUT else 0),
          'empty root fixture scope')
    check(s['alternate_enabled']==s['suppress_boundaries']==0
          and len(s['components'])==79 and s['components'][0]==s['dimension']
          and all(type(v)is int and v>=0 for v in s['components'])
          and len(s['output_flags'])==7 and all(type(v)is int for v in s['output_flags'])
          and type(s['geometry_mode'])is int,
          'root baseline profile')
    check(s['node8_default']==(20 if s['process_units_raw']==0 else 68),'global unit default')
    for obj in s['objects']:empty_object(obj,s)
    if linked:check(s['linked_state']is None and not s['objects'],'bounded linked recursion')
    else:
        check(s['linked_state']is not None and s['address']!=s['linked_state']['address'],
              'distinct linked root')
        state_shape(s['linked_state'],True)


def blank_table(old,mode=0,preserve=True):
    t=deepcopy(old)
    t.update(type_0x00=128,rank_0x04=0,constant_0x08=0,selectors_0x10=[0,0,0],counts_0x1c=[0,0,0],
             values_per_point_0x40=1,flag_0x50=0,mode_0x58=mode,axes=[[],[],[]],dependent=[],
             raw_hex={'constant':ZERO,'axes':['','',''],'dependent':''},
             pointers={'axes':['0x0']*3,'dependent':'0x0'})
    if not preserve:t.update(preserved_0x54=0,preserved_0x60_hex=ZERO)
    return t


def normalize_mode(s):
    if s['finalization_controls']['control_0x93c']==0:
        mode=int(s['finalization_controls'][FLOAT_KEYS[0]]<=0)
        s['finalization_controls']['mode_0x990']=s['time_table']['mode_0x58']=mode


def line_read(data,pos):
    end=data.find(b'\n',pos)
    end=min(len(data),pos+1023,end+1 if end>=0 else len(data))
    line=data[pos:end];eof=pos==len(data)
    after={'cursor':end,'count':len(data)-end,'buffer_hex':data.hex()}
    if eof or(end==len(data)and not line.endswith(b'\n')):
        after={'cursor':0,'count':0,'buffer_hex':''}
    return line,end,eof,after


def header_read(e,data,pos,stream):
    line,end,eof,after=line_read(data,pos)
    check(e['kind']=='int2' and e['before']==stream and e['after']==after,
          'header helper identity and FILE progression')
    check(e['lines']==[{'limit':1024,'eof':eof,'hex':None if eof else line.hex()}],
          'header exact fgets bytes/EOF')
    values,cursor,conv=[0,0],0,0
    data_line=line.replace(b',',b' ')
    for i in range(2):
        cursor+=len(data_line[cursor:])-len(data_line[cursor:].lstrip(b' \t\r\n\v\f'))
        match=re.match(rb'[+-]?(?:0[xX][0-9a-fA-F]+|0[0-7]*|[1-9][0-9]*)',data_line[cursor:])
        if not match:
            if i==0 and cursor==len(data_line):conv=-1
            break
        word=match.group();unsigned=word.lstrip(b'+-')
        radix=16 if unsigned.lower().startswith(b'0x')else 8 if unsigned.startswith(b'0')else 10
        values[i]=int(word,radix);conv+=1;cursor+=len(word)
    expected=[]if eof else[{'input_hex':data_line.hex(),'format':'%i %i','conversions':conv}]
    check(e['scans']==expected and e['status']==int(conv==-1) and e['values']==values
          and e['output_hex']==[struct.pack('<i',v).hex()for v in values],
          'header conversion/zero/partial/status')
    return end


def table_scan(e,data,pos,stream):
    check(e['format']=='%Flf' and e['before']==stream and stream_position(e['before'],data)==pos,
          'table scanner format/continuity')
    cursor=pos+len(data[pos:])-len(data[pos:].lstrip(b' \t\r\n\v\f'))
    match=re.match(rb'[+-]?(?:[0-9]+(?:\.[0-9]*)?|\.[0-9]+)',data[cursor:])
    conv,value=0,0.0
    if match:
        word=match.group();cursor+=len(word)
        exponent=re.match(rb'[eE][+-]?[0-9]*',data[cursor:])
        if exponent:
            suffix=exponent.group();cursor+=len(suffix)
            if re.search(rb'[0-9]',suffix):word+=suffix
        value=float(word);conv=1
    else:
        cursor+=len(re.match(rb'[+-]?\.?',data[cursor:]).group())
        if cursor==len(data):conv=-1
    after={'cursor':cursor,'count':len(data)-cursor,'buffer_hex':data.hex()}
    if cursor==len(data):after={'cursor':0,'count':0,'buffer_hex':''}
    check(e['after']==after and e['conversions']==conv and e['value']==value
          and e['previous_output_hex']==ZERO and e['output_hex']==struct.pack('<d',value).hex(),
          'table scanner fixture prefix/conversion/zero/raw output')
    return cursor



MAPPINGS=None

def empty_object(o,s):
    if EXTRA_RUN and(o['nodes']or o['elements']):
        populated_shape(o,s);return
    check(o['dimension']==s['dimension']and o['components']==s['components']
          and o['id']>=1 and o['type']in(0,1,2,3,4,6,7),'object identity/profile')
    check(o['element_kind']==(2 if s['dimension']==3 else 0)and o['arity']==4
          and o['allocation_arity']==0,'empty object connectivity profile')
    check(all(o[k]==0 for k in('flag_0x3990','mesh_flag','nodes','elements','node_allocation_count',
                              'element_allocation_count','boundary_total'))
          and o['boundary_groups']==[]and o['coordinates']==[]and o['connectivity']==[],
          'selected empty object scope')
    check(len(o['stroke_0x22d0'])==3 and len(o['stroke_hex'])==48
          and [struct.unpack('<d',bytes.fromhex(o['stroke_hex'][i:i+16]))[0]for i in range(0,48,16)]
              ==o['stroke_0x22d0']and all(math.isfinite(v)for v in o['stroke_0x22d0']),
          'STROKE binary64 triplet')
    check(all(v==[]for v in o['direct'].values())and all(v==0 for v in o['initialized_prefixes'].values()),
          'empty direct buffers')
    check(set(o['buffer_metadata'])==set(SLOTS),'direct slot inventory')
    for name,b in o['buffer_metadata'].items():
        check(int(b['slot'],16)==int(o['address'],16)+SLOTS[name]and int(b['pointer'],16)==0
              and b['integers']==(name!='coordinates')and b['allocated_items']==b['known_prefix']==b['unknown_items']==0
              and b['extent_source']=='null-pointer'and b['values']==[]and b['known_hex']=='','empty direct slot')
    check(len(o['descriptors'])==len(MAPPINGS)==53,'descriptor inventory')
    addresses=set()
    for d,row in zip(o['descriptors'],MAPPINGS):
        check(d['id']==row['state_id']and d['slot_offset']==int(row['slot_relative_to_object'],16)
              and d['vtable_rva']==int(row['vtable_address'],16)-0x140000000,'descriptor static identity')
        check(d['address']not in addresses and int(d['address'],16)>0,'descriptor address unique')
        addresses.add(d['address'])
        check(d['count']==d['old_count']==d['length']==0
              and d['width']==d['old_width']==d['multiplicity']==d['old_multiplicity']==d['storage_references']==1
              and d['values']is None and d['values_hex']==''
              and int(d['bound_pointer'],16)==int(d['storage_pointer'],16)==0,'empty descriptor/history')



def populated_shape(o,s):
    check(o['dimension']==s['dimension']and o['components']==s['components']
          and o['nodes']>=0 and o['elements']>=0 and o['arity']==4,'populated object shape')
    check(len(o['stroke_0x22d0'])==3 and len(o['stroke_hex'])==48
          and [struct.unpack('<d',bytes.fromhex(o['stroke_hex'][i:i+16]))[0]for i in range(0,48,16)]
              ==o['stroke_0x22d0'],'populated STROKE raw values')
    check(set(o['buffer_metadata'])==set(SLOTS),'populated buffer inventory')
    for name,b in o['buffer_metadata'].items():
        n,k=b['allocated_items'],b['known_prefix'];integer=name!='coordinates'
        check(int(b['slot'],16)==int(o['address'],16)+SLOTS[name]and b['integers']==integer
              and type(n)is int and type(k)is int and 0<=k<=n<=512
              and b['unknown_items']==n-k and len(b['values'])==n and b['values'][k:]==[None]*(n-k),
              'populated extent and unknown masking')
        width=4 if integer else 8;raw=bytes.fromhex(b['known_hex'])
        check(len(raw)==k*width and list(struct.unpack('<'+('i'if integer else 'd')*k,raw))==b['values'][:k],
              'populated initialized raw prefix')
        check((int(b['pointer'],16)==0)==(n==0),'populated pointer/null extent')
        if name.startswith('direct'):check(o['direct'][name[6:]]==b['values'],'direct view equals tracked buffer')
        else:check(o[name]==b['values'],'primary view equals tracked buffer')
    check(len(o['descriptors'])==53,'populated descriptor inventory')
    addresses=set()
    for d,row in zip(o['descriptors'],MAPPINGS):
        check(d['id']==row['state_id']and d['slot_offset']==int(row['slot_relative_to_object'],16)
              and d['vtable_rva']==int(row['vtable_address'],16)-0x140000000
              and int(d['address'],16)>0 and d['address']not in addresses,'populated descriptor mapping')
        addresses.add(d['address']);n=d['length']
        check(all(type(d[k])is int and d[k]>=0 for k in('count','width','multiplicity','old_count','old_width','old_multiplicity','length'))
              and n==d['count']*d['width']*d['multiplicity']and n<=512,'populated descriptor dimensions')
        if n:
            check(len(d['values'])==n and len(d['values_hex'])==n*16
                  and list(struct.unpack('<'+'d'*n,bytes.fromhex(d['values_hex'])))==d['values']
                  and int(d['storage_pointer'],16)>0 and d['bound_pointer']==d['storage_pointer'],
                  'populated descriptor raw values and pointer binding')
        else:check(d['values']is None and d['values_hex']=='','empty descriptor values')

def typed_row(e,data,pos,stream,types):
    line,end,eof,after=line_read(data,pos)
    check(e['before']==stream and e['after']==after
          and e['lines']==[{'limit':1024,'eof':eof,'hex':None if eof else line.hex()}],
          'typed row exact line and FILE progression')
    text=line.replace(b',',b' ');cursor=0;conv=0;values=[0]*len(types)
    for i,kind in enumerate(types):
        cursor+=len(text[cursor:])-len(text[cursor:].lstrip(b' \t\r\n\v\f'))
        if kind=='real':
            match=re.match(rb'[+-]?(?:[0-9]+(?:\.[0-9]*)?|\.[0-9]+)',text[cursor:])
            if not match:
                cursor+=len(re.match(rb'[+-]?\.?',text[cursor:]).group())
                if i==0 and cursor==len(text):conv=-1
                break
            word=match.group();cursor+=len(word)
            exponent=re.match(rb'[eE][+-]?[0-9]*',text[cursor:])
            if exponent:
                suffix=exponent.group();cursor+=len(suffix)
                if re.search(rb'[0-9]',suffix):word+=suffix
            values[i]=float(word)
        else:
            # A bare hexadecimal prefix at physical string exhaustion is not
            # an integer assignment. Prior successful real conversion remains.
            if re.fullmatch(rb'[+-]?0[xX]',text[cursor:]):
                cursor=len(text)
                if i==0:conv=-1
                break
            match=re.match(rb'[+-]?(?:0[xX][0-9a-fA-F]+|0[0-7]*|[1-9][0-9]*)',text[cursor:])
            if not match:
                if i==0 and cursor==len(text):conv=-1
                break
            word=match.group();cursor+=len(word);unsigned=word.lstrip(b'+-')
            radix=16 if unsigned.lower().startswith(b'0x')else 8 if unsigned.startswith(b'0')else 10
            values[i]=int(word,radix)
        conv+=1
    raw=[struct.pack('<d'if t=='real'else '<i',v).hex()for t,v in zip(types,values)]
    fmt=' '.join('%Flf'if t=='real'else '%i'for t in types)
    check(e['values']==values and e['output_hex']==raw and e['status']==int(conv==-1)
          and e['scans']==([]if eof else[{'input_hex':text.hex(),'format':fmt,'conversions':conv}]),
          'typed row conversions/partial zeroing/raw outputs')
    return end


def object_change(before,actual,selected,id,row,data,pos):
    expected=deepcopy(before);old_count=before['object_count']
    check(1<=selected<=3 and actual['object_count']==max(old_count,selected)
          and [o['id']for o in actual['objects']]==list(range(1,actual['object_count']+1)),
          'object selection creates consecutive placeholders')
    expected['object_count']=actual['object_count']
    for n in range(old_count,actual['object_count']):
        obj=deepcopy(actual['objects'][n])
        check(obj['secondary']==0,'new object secondary baseline')
        # New identities come from the original; their descriptor/empty-storage
        # fields are checked against the retained static contract in empty_object.
        obj['stroke_0x22d0']=[0,0,0];obj['stroke_hex']=ZERO*3;obj['name']=''
        obj['type']=0 if n+1==selected else 7
        expected['objects'].append(obj)
    obj=expected['objects'][selected-1]
    if id==153:
        count=before['dimension']
        obj['stroke_0x22d0'][:count]=row['values']
        obj['stroke_hex']=''.join(row['output_hex'])+obj['stroke_hex'][count*16:]
    elif id==144:
        _,name_start,_,_=line_read(data,pos)
        line,_,eof,_=line_read(data,name_start);check(not eof,'name fixture has value line')
        obj['name']=line.split(b'\n',1)[0][:80].rstrip(b' \r\n\0').decode('ascii')
    else:
        from verify_configuration_import_observations import line_integers
        values,status,_=line_integers(data,pos,2);check(status==0,'type fixture succeeds')
        code,secondary=values;obj['secondary']=secondary
        mapped={1:0,2:1,3:2,4:3,5:4,7:6}
        if code in mapped:obj['type']=mapped[code]
        elif code!=6:obj['type']=7
        if obj['type']==7:obj['name']=''
        # All selected objects have empty mesh buffers: deactivation preserves
        # descriptor history and triplet; the emitted free events are separate.
    return expected

def verify(evidence,run,manifest,inputs):
    global MAPPINGS
    if MAPPINGS is None:MAPPINGS=descriptor_map(read(ROOT/'analysis/mesh-array-contract.json'))
    if not EXTRA_RUN:check(manifest['startup_context']['control_unit_0x84c']==0,'predeclared startup context')
    cap,params=evidence['normalized_result'],evidence['parameters'];pid=run['pid']
    check(cap['pid']==params['pid']==pid and cap['target']==f'DEF_PRE_64.exe (pid {pid})',
          'capture target PID')
    check(cap['session_id']==params['session_id'] and cap['state']=='detached'
          and cap['mode']==params['mode']=='attach' and cap['device_id']==params['device_id']=='local'
          and cap['source_kind']=='file' and cap['messages_truncated']is False and cap['cleanup_error']is None,
          'complete passive lifecycle')
    messages,scripts=cap['messages'],cap['scripts']
    check(len(scripts)==1 and all(m['type']=='send'and m['script_id']==scripts[0]['script_id']for m in messages),
          'capture message/error inventory')
    hook=messages[0]['payload']
    check(hook['operation']=='hooks-installed'and hook['pid']==pid and int(hook['base'],16)>0,'hooks identity')
    events=[m['payload']for m in messages[1:]]
    check([e['sequence']for e in events]==list(range(1,len(events)+1)),'complete sequence')
    active=last=handler=reader=reset=stage=None
    imports=[];headers=[];scans=[];tables=[];reals=[];transitions=[];step_calls=[];selection_calls=[];stroke_calls=[];phase=None
    roots={};final_serial=0;stream=None;token=None
    for e in events:
        op=e['operation'];name=e['file'];data=inputs[name]
        if 'state'in e:state_shape(e['state'])
        if 'table'in e:table_shape(e['table'])
        if 'finalization_controls'in e:check_view(e)
        if op=='import-before':
            check(active is None and name==run['ordered_imports'][len(imports)],'ordered imports')
            check(last is None or e['state']==last,'cross-import persistence')
            last=deepcopy(e['state']);active={'file':name,'status':0,'headers':0,'handlers':0}
            phase='reset-before';pos=0;stream=None;token=None;header_seen=False
            if not roots:
                check(last['finalization_controls']['selected_object_0x8b8']==0
                      and last['finalization_controls']['pdie_alias_0x1100_hex'][8:]=='00000000'
                      and last['linked_state']['finalization_controls']['pdie_alias_0x1100_hex'][8:]=='00000000',
                      'selected ID and PDIE integer zero reset')
            if not roots:
                roots={last['dimension']:deepcopy(last),last['linked_state']['dimension']:deepcopy(last['linked_state'])}
            continue
        check(active is not None and name==active['file'],'event import ownership')
        if op=='flags-reset-before':
            check(phase=='reset-before'and view(e)==view(last),'wrapper reset entry')
            phase='reset-after'
        elif op=='flags-reset-after':
            check(phase=='reset-after'and view(e)==view(last),'wrapper reset exit')
            phase='parse'
        elif op=='stream-scan':
            check(phase=='parse'and e['format']in('%39s','%i')
                  and (handler is None if e['format']=='%39s'else handler is not None and handler['id']>=144),
                  'token/selector scan context')
            start=len(data)if e['before']['buffer_hex']==''else e['before']['cursor']
            if start>pos:
                check(token=='*','unaccounted stream gap')
                check(start==line_read(data,pos)[1],'comment line skip')
            else:check(start==pos,'token stream continuity')
            pos,_=held_out_scan(e,data,pos,stream is not None);stream=e['after']
            if e['format']=='%39s':
                token=e['value']if e['conversions']==1 else None
                resolved=None
            last_scan=e
        elif op=='keyword-resolved':
            check(phase=='parse'and handler is None and e['token']==token and token in IDS
                  and e['id']==IDS[token]and e['category']==(2 if e['id']>=144 else 1),'resolved table/scalar keyword')
            resolved=e['id']
        elif op=='handler-before':
            check(phase=='parse'and handler is None and e['id']==resolved and e['category']==(2 if e['id']>=144 else 1)
                  and e['stream']==stream,'handler entry pairing/stream')
            dims=header_dimension(data[:pos]);expected=deepcopy(last)
            if dims and not header_seen:
                header_seen=True
                if dims[0]!=last['dimension']:
                    for current,wanted in [(expected,dims[0]),(expected['linked_state'],5-dims[0])]:
                        old_address=current['address'];old_link=current['linked_state']
                        base=deepcopy(roots[wanted]);base['address']=old_address;base['linked_state']=old_link
                        base['time_table']['address']=hex(int(old_address,16)+0x938)
                        current.clear();current.update(base)
                    transitions.append({'file':name,'sequence':e['sequence'],'from':last['dimension'],'to':dims[0]})
            check(e['state']==expected,'handler entry preservation or header reset')
            last=deepcopy(expected);handler={'id':e['id'],'before':deepcopy(last),'row':None,'reset':False,'read':False,'selected':False,'lookup':None}
            active['handlers']+=1
        elif op=='object-selected':
            check(HELD_OUT and handler is not None and handler['id']>=144
                  and not handler['selected'] and last_scan['format']=='%i'
                  and e['requested']==last_scan['value'] and e['found']is True,'object selection pairing')
            handler['selected']=True;handler['selected_id']=e['requested']
        elif op in('allocation-update','allocation-free'):
            if EXTRA_RUN and handler is not None and handler['id']in(28,34,159,173):
                check(e['kind']in('nodes','elements')and int(e['object'],16)>0,'populated setup allocation context')
            else:
                check(op=='allocation-free'and e['kind']in('nodes','elements')
                      and e['object']in[o['address']for o in last['objects']]
                      and ((handler is not None and handler['id']==145)or
                           (handler is None and phase=='parse'and token=='*'and not header_seen
                            and header_dimension(data)and header_dimension(data)[0]!=last['dimension'])),
                      'deactivation or dimension-reset free context')
            # Setup mesh values are retained as original evidence, not recomputed here.
            # Scalar/table handlers are required to preserve the entire resulting state.
            for b in e['metadata'].values():
                n,k=b['allocated_items'],b['known_prefix']
                check(type(n)is int and type(k)is int and 0<=k<=n<=512
                      and b['unknown_items']==n-k and len(b['values'])==n
                      and b['values'][k:]==[None]*(n-k),'allocation observed extent/masking')
                if op=='allocation-free':check(n==k==0 and int(b['pointer'],16)==0 and b['known_hex']=='','free leaves empty buffer')
        elif op=='selection-row':
            check(handler is not None and handler['id']==38 and handler['row']is None,'PDIE row context')
            pos=header_read(e,data,pos,stream);stream=e['after'];handler['row']=e
            headers.append(deepcopy(e))
        elif op=='mixed-row':
            check(handler is not None and handler['id']==49 and handler['row']is None and e['kind']=='realint','mixed row context')
            pos=typed_row(e,data,pos,stream,['real','int']);stream=e['after'];handler['row']=e
            reals.append(deepcopy(e))
        elif op=='step-object-lookup':
            check(handler is not None and handler['id']==49 and handler['row']is not None
                  and handler['lookup']is None and e['root']==last['address']
                  and e['requested']==last['finalization_controls']['selected_object_0x8b8'],'step lookup context')
            obj=next((o for o in last['objects']if o['id']==e['requested']),None)
            check(e['found']==(obj is not None) and e['object']==(obj['address']if obj else '0x0')
                  and e['id']==(obj['id']if obj else None)and e['type']==(obj['type']if obj else None)
                  and e['stroke_hex']==(obj['stroke_hex']if obj else None),'lookup exact ID/type/raw value')
            handler['lookup']=e
        elif op=='header-row':
            check(handler is not None and handler['id']==51 and handler['row']is None,'header helper context')
            pos=header_read(e,data,pos,stream);stream=e['after'];handler['row']=e
            headers.append(deepcopy(e));active['headers']+=1
        elif op=='real-row':
            check(handler is not None and handler['id']in(48,50,153)and handler['row']is None,'real helper context')
            if handler['id']==153:
                count=last['dimension'];check(e['kind']==('real3'if count==3 else 'real2'),'STROKE dimension helper')
                pos=typed_row(e,data,pos,stream,['real']*count);result=deepcopy(e)
            else:pos,result=verify_real(e,data,pos,{'after':stream},handler['id'])
            stream=e['after'];handler['row']=e;reals.append(result)
        elif op=='table-reset-before':
            check(handler is not None and handler['id']==51 and handler['row']is not None
                  and handler['row']['status']==0 and not handler['reset']and reset is None
                  and e['type']==128 and e['table']==last['time_table'],'table reset context/input')
            reset=e['table'];handler['reset']=True
        elif op=='table-reset-after':
            check(reset is not None and e['table']==blank_table(reset),'reset recipe and preserved 54/60')
            reset=None;handler['reset_table']=deepcopy(e['table'])
        elif op=='table-reader-before':
            check(handler is not None and handler['id']==51 and 'reset_table'in handler
                  and reader is None and not handler['read']and e['stream']==stream,'table reader nesting')
            expected=deepcopy(handler['reset_table']);selector,count=handler['row']['values']
            if selector:
                expected['rank_0x04']=1;expected['counts_0x1c'][0]=count
                expected['selectors_0x10'][0]=2 if selector in(2,4)else 1
                expected['mode_0x58']=int(selector in(3,4))
            else:expected['mode_0x58']=int(last['finalization_controls'][FLOAT_KEYS[0]]<=0)
            check(e['table']==expected,'selector/rank/count/mode mapping')
            reader={'before':deepcopy(expected),'scans':[],'discard':None};handler['read']=True
        elif op=='table-scan':
            check(reader is not None and reader['before']['rank_0x04']==1 and reader['discard']is None
                  and not any(x['conversions']==-1 for x in reader['scans'])
                  and len(reader['scans'])<max(0,reader['before']['counts_0x1c'][0])*2,'table scan bounded loop order')
            pos=table_scan(e,data,pos,stream);stream=e['after'];reader['scans'].append(e);scans.append(deepcopy(e))
        elif op=='table-discard':
            check(reader is not None and reader['discard']is None
                  and len(reader['scans'])==reader['before']['counts_0x1c'][0]*2>0
                  and all(x['conversions']!=-1 for x in reader['scans'])and e['before']==stream,
                  'discard follows successful positive table loop')
            line,end,eof,after=line_read(data,pos)
            check(e['limit']==1024 and e['eof']==eof and e['hex']==(None if eof else line.hex())
                  and e['after']==after,'discard exact bytes and EOF status independence')
            reader['discard']=e;stream=e['after'];pos=end
        elif op=='table-reader-after':
            check(reader is not None and e['stream']==stream,'table reader exit pairing')
            expected=deepcopy(reader['before']);n=max(0,expected['counts_0x1c'][0]);rank=expected['rank_0x04']
            expected['counts_0x1c'][0]=n
            failed=bool(reader['scans']and reader['scans'][-1]['conversions']==-1)
            check(e['status']==int(failed)and(failed or len(reader['scans'])==n*2)
                  and (reader['discard']is not None)==(n>0 and not failed),'table loop completion/failure/discard')
            if rank and n:
                values=[0.0]*(2*n);raw=[ZERO]*(2*n)
                for i,scan in enumerate(reader['scans']):values[i]=scan['value'];raw[i]=scan['output_hex']
                expected['axes'][0]=values[::2];expected['dependent']=values[1::2]
                expected['raw_hex']['axes'][0]=''.join(raw[::2]);expected['raw_hex']['dependent']=''.join(raw[1::2])
                expected['pointers']=deepcopy(e['table']['pointers'])
                check(int(expected['pointers']['axes'][0],16)>0 and int(expected['pointers']['dependent'],16)>0
                      and expected['pointers']['axes'][1:]==['0x0','0x0'],'rank one allocation shape')
            check(e['table']==expected,'table partial arrays/zero tail and raw interleaving')
            handler['read_after']=e;tables.append({'file':name,'sequence':e['sequence'],
                'selector':handler['row']['values'][0],'requested_count':handler['row']['values'][1],
                'status':e['status'],'scan_conversions':[x['conversions']for x in reader['scans']],
                'discard':reader['discard'],'table':deepcopy(e['table'])})
            reader=None
        elif op=='handler-after':
            check(handler is not None and e['id']==handler['id']and e['category']==(2 if e['id']>=144 else 1)
                  and (e['stream']==stream and handler['row']is not None if e['id']in(38,48,49,50,51,153)else HELD_OUT)
                  and reset is None and reader is None,'handler exit pairing')
            expected=deepcopy(handler['before']);c,h=expected['finalization_controls'],expected['time_controls_hex'];row=handler['row']
            if e['id']==38:
                check(e['status']==2*row['status']and not handler['read']and not handler['reset'],'PDIE status')
                c['selected_object_0x8b8']=row['values'][0]
                c['pdie_alias_0x1100_hex']=c['pdie_alias_0x1100_hex'][:8]+row['output_hex'][1]
            elif e['id']==49:
                check(e['status']==0 and handler['lookup']is not None and not handler['read']and not handler['reset'],
                      'DSMAX_NSTEP ignores helper status and always looks up')
                lookup=handler['lookup'];value,count=row['values']
                if lookup['found']and count!=0 and value!=0:
                    reference=struct.unpack('<d',bytes.fromhex(lookup['stroke_hex'][32:48]))[0]
                    check(math.isfinite(value)and math.isfinite(reference),'finite arithmetic scope')
                    difference=abs(value)-abs(reference);result=difference/float(count)
                    c[FLOAT_KEYS[0]]=result;h[FLOAT_KEYS[0]]=struct.pack('<d',result).hex()
            elif e['id']==153:
                check(e['status']==2*row['status']and handler['selected'],'STROKE status and object selection')
                expected=object_change(expected,e['state'],handler['selected_id'],153,row,data,pos)
            elif e['id']==51:
                if row['status']:
                    check(not handler['reset']and not handler['read']and e['status']==2,'failed header preserves table')
                else:
                    check(handler['read']and 'read_after'in handler and e['status']==2*handler['read_after']['status'],
                          'DTTYPE reader propagation')
                    expected['time_table']=deepcopy(handler['read_after']['table'])
                    c['control_0x93c']=expected['time_table']['rank_0x04'];c['mode_0x990']=expected['time_table']['mode_0x58']
            elif e['id']in(48,50):
                check(not handler['reset']and not handler['read']and e['status']==2*row['status'],'scalar helper propagation')
                if e['id']==48:
                    c[FLOAT_KEYS[0]]=row['values'][0];h[FLOAT_KEYS[0]]=row['output_hex'][0];positive=c[FLOAT_KEYS[0]]>0
                    if positive:c[FLOAT_KEYS[1]]=0;h[FLOAT_KEYS[1]]=ZERO
                else:
                    for key,value,raw in zip(FLOAT_KEYS[1:],row['values'],row['output_hex']):c[key]=value;h[key]=raw
                    positive=c[FLOAT_KEYS[1]]>0
                    if positive:c[FLOAT_KEYS[0]]=0;h[FLOAT_KEYS[0]]=ZERO
                if positive:normalize_mode(expected)
            elif e['id']in(144,145):
                check(handler['selected']and row is None and e['status']==0,'object setup success')
                expected=object_change(expected,e['state'],handler['selected_id'],e['id'],None,data,pos)
                pos=stream_position(e['stream'],data);stream=e['stream']
            else:
                check(e['status']==0 and row is None and not handler['reset']and not handler['read']
                      and handler['selected']==(e['id']>=144),'setup handler status/selection')
                check(view(e['state'])==view(expected),'setup preserves selected table and scalar controls')
                check(e['state']['linked_state']==expected['linked_state'],'setup preserves linked root')
                check(stream_position(e['stream'],data)>=pos,'setup stream monotonicity')
                pos=stream_position(e['stream'],data);stream=e['stream'];expected=deepcopy(e['state'])
            check(e['state']==expected,'handler exact captured state effects and preservation')
            if e['id']==49:
                step_calls.append({'file':name,'sequence':e['sequence'],'helper':deepcopy(row),
                    'lookup':deepcopy(handler['lookup']),'handler_status':e['status'],
                    'before_controls':handler['before']['finalization_controls'],
                    'after_controls':e['state']['finalization_controls'],
                    'before_raw':handler['before']['time_controls_hex'],'after_raw':e['state']['time_controls_hex']})
            elif e['id']==38:
                selection_calls.append({'file':name,'sequence':e['sequence'],'helper':deepcopy(row),
                    'handler_status':e['status'],'before_controls':handler['before']['finalization_controls'],
                    'after_controls':e['state']['finalization_controls']})
            elif e['id']==153:
                stroke_calls.append({'file':name,'sequence':e['sequence'],'dimension':e['state']['dimension'],
                    'object_id':handler['selected_id'],'helper':deepcopy(row),'handler_status':e['status'],
                    'after_stroke_hex':e['state']['objects'][handler['selected_id']-1]['stroke_hex'],
                    'object_type':e['state']['objects'][handler['selected_id']-1]['type']})
            if e['status']:active['status']=e['status']
            last=deepcopy(expected);handler=None
        elif op=='finalize-before':
            check(phase=='parse'and handler is None and last_scan['conversions']==-1 and pos==len(data)
                  and e['state']==last,'EOF finalization timing/state')
            final_serial+=1;check(e['finalizer_serial']==final_serial,'finalizer serial')
            phase='finalize';stage_index=0;stage=None
        elif op=='stage-before':
            check(phase=='finalize'and stage is None and stage_index<3
                  and e['stage']==['control','materials','objects'][stage_index]
                  and e['finalizer_serial']==final_serial and view(e)==view(last)
                  and e['option']==(0 if stage_index==2 else None)
                  and e['objects']==([{k:o[k]for k in('address','id','type','flag_0x3990')}for o in last['objects']]
                                        if stage_index==2 else None),'finalization stage order/input')
            stage=e['stage']
            visits=[o for o in last['objects']if o['type']!=7]if stage_index==2 else []
            visit_index=0;visit=None
        elif op in('object-stage-before','object-stage-after'):
            check(HELD_OUT and stage=='objects'and e['finalizer_serial']==final_serial
                  and e['root']==last['address']and e['option']==0 and visit_index<len(visits),
                  'object finalizer nesting')
            obj=visits[visit_index]
            check(e['object']==obj['address']and all(e[k]==obj[k]for k in('id','type','flag_0x3990'))
                  and e['flag_0x3990']==0,'eligible finalizer object guard')
            if op=='object-stage-before':
                check(visit is None,'overlapping object visits');visit=e['object']
            else:
                check(visit==e['object'],'object visit pairing');visit=None;visit_index+=1
        elif op=='stage-after':
            check(phase=='finalize'and stage==e['stage']and e['finalizer_serial']==final_serial
                  and e['option']==(0 if stage_index==2 else None)
                  and e['objects']==([{k:o[k]for k in('address','id','type','flag_0x3990')}for o in last['objects']]
                                        if stage_index==2 else None)
                  and visit is None and visit_index==len(visits),'stage exit pair/complete visits')
            expected=deepcopy(last)
            if stage=='control':normalize_mode(expected)
            check(view(e)==view(expected),'stage normalization rank guard and table preservation')
            last=expected;stage=None;stage_index+=1
        elif op=='finalize-after':
            check(phase=='finalize'and stage is None and stage_index==3
                  and e['finalizer_serial']==final_serial and e['state']==last,'finalizer exit exact snapshot')
            phase='import-after'
        elif op=='import-after':
            check(phase=='import-after'and e['state']==last and e['status']==active['status'],'import aggregate status')
            imports.append({**active,'final_table':deepcopy(last['time_table']),
                            'final_controls':deepcopy(last['finalization_controls'])});active=None
        else:raise ValueError('Unknown/probe error operation '+op)
    check(active is None and len(imports)==len(manifest['cases'])==EXPECTED_IMPORTS,'complete import inventory')
    return {'step_calls':step_calls,'selection_calls':selection_calls,'stroke_calls':stroke_calls,
            'imports':imports,'header_rows':headers,'table_scans':scans,'table_readers':tables,
            'real_rows':reals,'dimension_transitions':transitions,'operation_counts':dict(Counter(e['operation']for e in events))}

def main():
    global EXTRA_RUN,EXPECTED_IMPORTS,DIRECTORY,CAPTURE,MANIFEST,FREEZE
    parser=argparse.ArgumentParser();parser.add_argument('--held-out',action='store_true');args=parser.parse_args()
    EXTRA_RUN=args.held_out
    if EXTRA_RUN:
        EXPECTED_IMPORTS=12
        DIRECTORY=ROOT/'analysis/oracle/step-distance-held-out'
        CAPTURE=ROOT/'analysis/step-distance-runtime-held-out.json'
        MANIFEST=ROOT/'analysis/step-distance-held-out-cases.json'
        FREEZE=ROOT/'analysis/step-distance-corrected-freeze.json'
    evidence,run,manifest,freeze=map(read,[CAPTURE,DIRECTORY/'run.json',MANIFEST,FREEZE])
    inputs=provenance(evidence,run,manifest,freeze);result=verify(evidence,run,manifest,inputs)
    mutations=[]
    def pick(e,op,predicate=lambda p:True):
        return next(m['payload']for m in e['normalized_result']['messages']
                    if m['payload']['operation']==op and predicate(m['payload']))
    def reject(name,mutate,prov=False):
        e,r,m,f=map(deepcopy,(evidence,run,manifest,freeze));mutate(e,r,m,f)
        try:
            if prov:provenance(e,r,m,f)
            else:verify(e,r,m,inputs)
        except (ValueError,AssertionError,KeyError,IndexError,TypeError,StopIteration)as ex:
            mutations.append({'case':name,'rejected':True,'reason':str(ex)});return
        raise AssertionError('Tampering accepted: '+name)
    if not EXTRA_RUN:
        reject('wrong process',lambda e,r,m,f:e['normalized_result'].update(pid=1))
        reject('truncated capture',lambda e,r,m,f:e['normalized_result'].update(messages_truncated=True))
        reject('sequence gap',lambda e,r,m,f:pick(e,'mixed-row').update(sequence=0))
        reject('probe error',lambda e,r,m,f:e['normalized_result']['messages'][2].update(type='error'))
        reject('undeclared root unit context',lambda e,r,m,f:pick(e,'import-before')['state']['finalization_controls'].update(control_unit_0x84c=2))
        reject('declared unit context altered',lambda e,r,m,f:m['startup_context'].update(control_unit_0x84c=2))
        reject('PDIE initializer high word nonzero',lambda e,r,m,f:pick(e,'import-before')['state']['finalization_controls'].update(pdie_alias_0x1100_hex='000000000000f03f'))
        reject('PDIE helper status',lambda e,r,m,f:pick(e,'selection-row',lambda p:p['status']==1).update(status=0))
        reject('PDIE raw integer output',lambda e,r,m,f:pick(e,'selection-row')['output_hex'].__setitem__(1,'00000000'))
        reject('PDIE selected ID mutation lost',lambda e,r,m,f:pick(e,'handler-after',lambda p:p['id']==38)['state']['finalization_controls'].update(selected_object_0x8b8=0))
        reject('PDIE unrelated adjacent byte changed',lambda e,r,m,f:pick(e,'handler-after',lambda p:p['id']==38)['state']['finalization_controls'].update(pdie_alias_0x1100_hex='010000000000f03f'))
        reject('PDIE independent high word not written',lambda e,r,m,f:pick(e,'handler-after',lambda p:p['id']==38)['state']['finalization_controls'].update(pdie_alias_0x1100_hex=ZERO))
        reject('mixed helper EOF status',lambda e,r,m,f:pick(e,'mixed-row',lambda p:p['status']==1).update(status=0))
        reject('mixed helper raw integer',lambda e,r,m,f:pick(e,'mixed-row')['output_hex'].__setitem__(1,'04000000'))
        reject('mixed helper consumed bytes',lambda e,r,m,f:pick(e,'mixed-row')['lines'][0].update(hex='203120320a'))
        reject('mixed scanner conversion count',lambda e,r,m,f:pick(e,'mixed-row')['scans'][0].update(conversions=0))
        reject('mixed EOF cursor',lambda e,r,m,f:pick(e,'mixed-row',lambda p:p['status']==1)['after'].update(cursor=1))
        reject('49 propagates ignored helper error',lambda e,r,m,f:pick(e,'handler-after',lambda p:p['id']==49 and p['file']=='09_mixed_eof.KEY').update(status=2))
        reject('lookup incorrectly filters type7',lambda e,r,m,f:pick(e,'step-object-lookup',lambda p:p['type']==7).update(found=False))
        reject('lookup fabricates missing object',lambda e,r,m,f:pick(e,'step-object-lookup',lambda p:not p['found']).update(found=True))
        reject('lookup requested ID',lambda e,r,m,f:pick(e,'step-object-lookup').update(requested=99))
        reject('lookup raw reference value',lambda e,r,m,f:pick(e,'step-object-lookup').update(stroke_hex=ZERO*3))
        def number(e,id,key,value,predicate=lambda p:True):
            q=pick(e,'handler-after',lambda p:p['id']==id and predicate(p))['state']
            q['finalization_controls'][key]=value;q['time_controls_hex'][key]=struct.pack('<d',value).hex()
        reject('consistent wrong result arithmetic',lambda e,r,m,f:number(e,49,FLOAT_KEYS[0],99.0))
        reject('49 incorrectly clears DTMAX',lambda e,r,m,f:number(e,49,FLOAT_KEYS[1],0.0))
        reject('49 incorrectly clears secondary DTMAX',lambda e,r,m,f:number(e,49,FLOAT_KEYS[2],0.0))
        def early_mode(e,r,m,f):
            q=pick(e,'handler-after',lambda p:p['id']==49)['state']
            q['finalization_controls']['mode_0x990']=0;q['time_table']['mode_0x58']=0
        reject('49 normalizes mode immediately',early_mode)
        reject('zero gate overwrites prior result',lambda e,r,m,f:number(e,49,FLOAT_KEYS[0],0.0,lambda p:p['file']=='03_zero_guards.KEY'))
        reject('negative denominator sign lost',lambda e,r,m,f:number(e,49,FLOAT_KEYS[0],2.0,lambda p:p['file']=='02_signed_arithmetic.KEY'))
        reject('STROKE dimension helper mismatch',lambda e,r,m,f:pick(e,'real-row',lambda p:p['kind']=='real3').update(kind='real2'))
        reject('STROKE helper output partial tail',lambda e,r,m,f:pick(e,'real-row',lambda p:p['kind']=='real3'and p['file']=='15_spatial_reset_partial.KEY')['output_hex'].__setitem__(2,'000000000000f03f'))
        def stroke(e,op,predicate,index,value):
            obj=next(o for o in pick(e,op,predicate)['state']['objects']if o['id']==1)
            obj['stroke_0x22d0'][index]=value
            obj['stroke_hex']=obj['stroke_hex'][:index*16]+struct.pack('<d',value).hex()+obj['stroke_hex'][(index+1)*16:]
        reject('2D STROKE writes third destination',lambda e,r,m,f:stroke(e,'handler-after',lambda p:p['id']==153 and p['state']['dimension']==2,2,7.0))
        reject('deactivation clears retained STROKE',lambda e,r,m,f:stroke(e,'handler-after',lambda p:p['id']==145,2,0.0))
        reject('STROKE reactivates type7 object',lambda e,r,m,f:pick(e,'handler-after',lambda p:p['id']==153 and p['file']=='06_deactivated_object.KEY')['state']['objects'][0].update(type=0))
        reject('descriptor mapping corrupted',lambda e,r,m,f:pick(e,'handler-after',lambda p:bool(p['state']['objects']))['state']['objects'][0]['descriptors'][0].update(id=99))
        reject('unobserved mesh population',lambda e,r,m,f:pick(e,'handler-after',lambda p:bool(p['state']['objects']))['state']['objects'][0].update(nodes=1))
        reject('finalizer stage ordering',lambda e,r,m,f:pick(e,'stage-before').update(stage='objects'))
        reject('inactive object visited by finalizer',lambda e,r,m,f:pick(e,'object-stage-before').update(type=7))
        def drop(e,r,m,f,ops):
            ms=e['normalized_result']['messages']
            for op in ops:
                i=next(i for i,x in enumerate(ms)if x['payload']['operation']==op);ms.pop(i)
            for i,x in enumerate(ms[1:],1):x['payload']['sequence']=i
        reject('missing lookup with repaired sequence',lambda e,r,m,f:drop(e,r,m,f,['step-object-lookup']))
        reject('missing eligible finalizer visit with repaired sequence',lambda e,r,m,f:drop(e,r,m,f,['object-stage-before','object-stage-after']))
        reject('error status erased by later successful49',lambda e,r,m,f:pick(e,'import-after',lambda p:p['file']=='11_pdie_blank.KEY').update(status=0))
        reject('fixture hash',lambda e,r,m,f:m['cases'][0].update(sha256='0'*64),True)
        reject('original hash',lambda e,r,m,f:r.update(executable_sha256='0'*64),True)
        reject('probe hash',lambda e,r,m,f:e['normalized_result'].update(source_sha256='0'*64),True)
        reject('input order',lambda e,r,m,f:r['ordered_imports'].reverse(),True)
        reject('transcript hash',lambda e,r,m,f:r['files'][0].update(sha256='0'*64),True)
        reject('frozen archive hash',lambda e,r,m,f:f['files'][0].update(sha256='0'*64),True)
        reject('export extraction altered',lambda e,r,m,f:e.update(confidence='fabricated'),True)
    else:
        reject('wrong process',lambda e,r,m,f:e['normalized_result'].update(pid=1))
        reject('truncated capture',lambda e,r,m,f:e['normalized_result'].update(messages_truncated=True))
        reject('sequence gap',lambda e,r,m,f:pick(e,'mixed-row').update(sequence=0))
        reject('PDIE nonzero default integer',lambda e,r,m,f:pick(e,'import-before')['state']['finalization_controls'].update(pdie_alias_0x1100_hex='0000000001000000'))
        reject('PDIE high word lost',lambda e,r,m,f:pick(e,'handler-after',lambda p:p['id']==38)['state']['finalization_controls'].update(pdie_alias_0x1100_hex=ZERO))
        reject('PDIE adjacent bytes mutated',lambda e,r,m,f:pick(e,'handler-after',lambda p:p['id']==38)['state']['finalization_controls'].update(pdie_alias_0x1100_hex='01000000ffffffff'))
        reject('PDIE selected ID',lambda e,r,m,f:pick(e,'handler-after',lambda p:p['id']==38)['state']['finalization_controls'].update(selected_object_0x8b8=0))
        reject('hex prefix falsely assigned',lambda e,r,m,f:pick(e,'mixed-row',lambda p:p['file']=='03_hex_prefix_eof.KEY')['scans'][0].update(conversions=2))
        reject('mixed signed count',lambda e,r,m,f:pick(e,'mixed-row')['output_hex'].__setitem__(1,'00000000'))
        reject('mixed exact input bytes',lambda e,r,m,f:pick(e,'mixed-row')['lines'][0].update(hex='203120320a'))
        reject('lookup filters inactive',lambda e,r,m,f:pick(e,'step-object-lookup',lambda p:p['type']==7).update(found=False))
        reject('lookup fabricates missing object',lambda e,r,m,f:pick(e,'step-object-lookup',lambda p:not p['found']).update(found=True))
        reject('lookup reference corruption',lambda e,r,m,f:pick(e,'step-object-lookup',lambda p:p['found']).update(stroke_hex=ZERO*3))
        def number(e,key,value,predicate):
            q=pick(e,'handler-after',lambda p:p['id']==49 and predicate(p))['state']
            q['finalization_controls'][key]=value;q['time_controls_hex'][key]=struct.pack('<d',value).hex()
        reject('finite result changed consistently',lambda e,r,m,f:number(e,FLOAT_KEYS[0],99.0,lambda p:True))
        reject('negative zero result sign lost',lambda e,r,m,f:number(e,FLOAT_KEYS[0],0.0,lambda p:p['file']=='06_negative_zero_result.KEY'))
        reject('49 clears existing DTMAX',lambda e,r,m,f:number(e,FLOAT_KEYS[1],0.0,lambda p:p['file']=='11_planar_reset.KEY'))
        reject('49 clears secondary DTMAX',lambda e,r,m,f:number(e,FLOAT_KEYS[2],0.0,lambda p:p['file']=='11_planar_reset.KEY'))
        def early_mode(e,r,m,f):
            q=pick(e,'handler-after',lambda p:p['id']==49 and p['file']=='11_planar_reset.KEY')['state']
            q['finalization_controls']['mode_0x990']=0;q['time_table']['mode_0x58']=0
        reject('49 premature normalization',early_mode)
        reject('failed STROKE helper accepted',lambda e,r,m,f:pick(e,'real-row',lambda p:p['file']=='08_stroke_sign_eof.KEY').update(status=0))
        def stroke(e,predicate,index,value):
            o=pick(e,'handler-after',predicate)['state']['objects'][0]
            o['stroke_0x22d0'][index]=value
            o['stroke_hex']=o['stroke_hex'][:index*16]+struct.pack('<d',value).hex()+o['stroke_hex'][(index+1)*16:]
        reject('deactivation erases retained stroke',lambda e,r,m,f:stroke(e,lambda p:p['id']==145 and p['file']=='09_inactive_retention.KEY',2,0.0))
        reject('planar stroke overwrites third component',lambda e,r,m,f:stroke(e,lambda p:p['id']==153 and p['state']['dimension']==2,2,1.0))
        reject('scalar changes captured mesh',lambda e,r,m,f:pick(e,'handler-after',lambda p:p['id']==49 and p['file']=='12_populated_mesh.KEY')['state']['objects'][0].update(mesh_flag=0))
        reject('descriptor identity changed',lambda e,r,m,f:pick(e,'handler-after',lambda p:p['id']==49 and p['file']=='12_populated_mesh.KEY')['state']['objects'][0]['descriptors'][0].update(id=99))
        def unknown(e,r,m,f):
            q=pick(e,'allocation-update',lambda p:any(b['unknown_items']for b in p['metadata'].values()))
            b=next(b for b in q['metadata'].values()if b['unknown_items']);b['values'][b['known_prefix']]=0
        reject('unknown growth tail fabricated',unknown)
        reject('stage order',lambda e,r,m,f:pick(e,'stage-before').update(stage='objects'))
        reject('type7 finalizer visit',lambda e,r,m,f:pick(e,'object-stage-before').update(type=7))
        def drop(e,r,m,f,ops):
            ms=e['normalized_result']['messages']
            for op in ops:
                i=next(i for i,x in enumerate(ms)if x['payload']['operation']==op);ms.pop(i)
            for i,x in enumerate(ms[1:],1):x['payload']['sequence']=i
        reject('missing lookup repaired sequence',lambda e,r,m,f:drop(e,r,m,f,['step-object-lookup']))
        reject('missing finalizer visit repaired sequence',lambda e,r,m,f:drop(e,r,m,f,['object-stage-before','object-stage-after']))
        reject('import failure lost',lambda e,r,m,f:pick(e,'import-after',lambda p:p['status']==2).update(status=0))
        reject('fixture hash',lambda e,r,m,f:m['cases'][0].update(sha256='0'*64),True)
        reject('original hash',lambda e,r,m,f:r.update(executable_sha256='0'*64),True)
        reject('probe hash',lambda e,r,m,f:e['normalized_result'].update(source_sha256='0'*64),True)
        reject('input order',lambda e,r,m,f:r['ordered_imports'].reverse(),True)
        reject('transcript hash',lambda e,r,m,f:r['files'][0].update(sha256='0'*64),True)
        reject('freeze archive hash',lambda e,r,m,f:f['files'][0].update(sha256='0'*64),True)
    deps=['tools/capture_oracle.py','tools/verify_boundary_sort_observations.py','tools/verify_keyword_import_observations.py',
          'tools/verify_time_controls_observations.py','tools/verify_root_layout_observations.py',
          'tools/verify_configuration_import_observations.py','tools/verify_mesh_array_observations.py',
          'tools/verify_mesh_refresh_observations.py','analysis/mesh-array-contract.json',
          'analysis/step-distance-state-audit.json','analysis/startup-0x1401ead60.json',
          'analysis/step-distance-0x14039aa90-assembly.json','analysis/step-distance-0x140416aa0.json',
          'analysis/step-distance-0x140414c20.json','analysis/step-distance-0x140484558.json',
          'analysis/step-distance-0x14018b500.json','analysis/step-distance-0x14018bc30.json',
          'analysis/step-distance-0x1401d6ea0.json','analysis/model-0x14017aaf0.json']
    report={'status':'pass','scope':__doc__,'capture':binding(CAPTURE),'evidence_id':evidence['evidence_id'],
        'pid':run['pid'],'run':binding(DIRECTORY/'run.json'),'manifest':binding(MANIFEST),
        'probe':binding(DIRECTORY.with_name(DIRECTORY.name+'-probe.js')),
        'probe_template':binding(ROOT/'tools/observe_step_distance.js'),'pid_record':binding(DIRECTORY.with_suffix('.pid.json')),
        'harness':binding(ROOT/'tools/capture_keywords.py'),'verifier':binding(Path(__file__)),
        'dependencies':[binding(ROOT/p)for p in deps],
        'export_recovery':{'transport_receipt':binding(ROOT/'analysis/step-distance-runtime-transport-receipt.json'),
            'retained_bundle':binding(ROOT/'analysis/oracle/step-distance-transport-evidence.json'),'matching_full_record':evidence['evidence_id'],
            'record_equal_to_extracted_capture':True,
            'qualification':'This stable retained export is bound by SHA256. Verification compares the complete matching record, not only its normalized messages, with the bound extracted capture.'},
        'candidate_freeze':{'manifest':binding(FREEZE),'declared_created_utc':freeze['created_utc'],
            'verified_archives':len(freeze['files']),'current_matches':[{'path':f['path'],
                'matches':(ROOT/f['path']).is_file()and digest(ROOT/f['path'])==f['sha256']}for f in freeze['files']]},
        **result,'tampering_checks':mutations,
        'findings':[
            'All17 original/copied fixtures, original executable, PID, passive probe template substitution, input/output transcripts and23 initial frozen archives verified. The exported full evidence record exactly equals the extracted capture.',
            'Every primary/linked root snapshot satisfies the historical predeclared UNIT0 context. This observation is not used to infer the PDIE reset value or to require UNIT as a model input.',
            'PDIE writes selected object ID and independent signed32 field1104, including zero/partial/failed reader mutations. Its write preserves the adjacent lower four bytes in the1100 probe window.',
            'STROKE assigns three spatial components or only two planar components from independent fixture parsing. Unassigned destinations zero; planar third component is preserved. Creation precedes value-row failure.',
            'Deactivation retains the captured STROKE triplet; STROKE can write an inactive object without reactivation. The step lookup returns matching IDs without excluding type7 objects or creating missing objects.',
            'All24 mixed helpers are independently checked against exact lines, comma replacement, legacy decimal/base0 integer conversions, statuses and raw outputs. DSMAX_NSTEP handler always returns0 even when its helper returns1.',
            'Finite calculation is checked as abs(input) minus abs(third STROKE), then division by the signed nonzero count. Failed gates preserve prior930. Successful49 writes neither clear920/928 nor normalize mode immediately.',
            'Finalization visits control/material/object stages in order, including after parser errors; it normalizes mode only when table rank permits and skips inactive object visits.',
            'Every emitted empty object descriptor mapping, pointer relationship, default shape/history and direct-buffer null extent is validated. Existing captured state is preserved except for the established selected mutations.'],
        'discovery':{'initial_static_error':'An incorrect0x1100-0x750 subtraction attributed relative3b0/unit-dependent double initialization to the PDIE neighbor window.',
            'rejection':'The verifier rejected the initial1.0 expectation against original initial snapshots before accepting the run.',
            'correct_static_proof':'0x750+0x9b4=0x1104;1ead60 explicitly zeroes that int32. Relative3b0 targetsrootB00, and the lower four1100 bytes have no established double meaning.',
            'historical_preservation':'Initial frozen C++ and failed comparison are retained by the parent. The corrected audit records the withdrawn claim; no reconstructed historical file or unverifiable old hash is fabricated.'},
        'limitations':[
            'No original or replacement executable runs; this independent Python verifier does not import or execute candidate C++.',
            'Seventeen small finite ASCII fixtures, empty mesh objects and materials, dimensions2/3, selected type0/type7 and rank0/1 tables only. This is not complete movement, geometry, DB or solver equivalence.',
            'Lower four bytes of the1100 probe window are opaque adjacent storage, not modeled double data. Runtime zero bytes do not establish a zero-initialization contract; PDIE preservation is checked without using them to seed replacement state.',
            'Native root84c startup origin remains unresolved. The historical UNIT0 fixture precondition holds, but correct PDIE1104 initialization is independent of units.',
            'Finite rounding is checked for these literals and operation order. Nonfinite CRT branches, general range/locale behavior and arbitrary numerical inputs remain outside scope.',
            'Exact triplet retention through deactivation is corroborated for empty objects here; populated mesh cleanup was established separately and is not newly exercised.',
            'Snapshot equality proves only emitted fields. Opaque neighboring storage and broader construction/subobject semantics remain partial.',
            'Archive integrity is verified; freeze chronology is declared metadata rather than trusted time. Current-file matches are informational, and launch-time harness source hash was not retained.']}
    if EXTRA_RUN:
        report.pop('export_recovery')
        report['findings'][0]='All12 held-out original/copied fixtures, executable, PID, passive probe, input/output transcripts and23 corrected frozen archives verified.'
        report['findings'][1]='No unit-value precondition is imposed on this held-out run. Independent integer1104 resets to zero; its lower neighboring bytes remain opaque.'
        report['findings'][5]='All15 mixed helpers are checked from exact line bytes, legacy decimal/base0 integers, conversion counts, statuses and raw outputs. A bare0x integer prefix at EOF leaves the prior successful real conversion intact and the integer unassigned.'
        report['findings'][-1]='Empty objects have validated constructor descriptor shapes; populated objects have descriptor identities/raw payloads and direct-buffer known-prefix/unknown-tail checks. The full captured mesh remains unchanged through PDIE, STROKE, DSMAX_NSTEP, other scalar/table handlers and finalizers.'
        report['findings'].append('The held-out negative-zero result is checked bitwise: equal absolute operands divided by a negative count yields raw0000000000000080. Inactive-object STROKE retention is exercised without rewriting STROKE after deactivation.')
        report['limitations'][1]='Twelve finite ASCII fixtures with dimensions2/3, empty objects and one reused planar mesh setup. Setup boundary topology and refresh semantics are not independently regenerated here; separate comparator coverage applies. Scalar preservation checks include every emitted mesh field.'
        report['limitations'][3]='Native root84c startup origin remains unresolved. No UNIT0 or other unit precondition is required for this run or for the proved1104 integer default.'
        report['limitations'][5]='Deactivation retention is directly exercised on an empty object. The populated mesh is then used to test scalar-field preservation, not populated cleanup.'
        report['discovery']['held_out_scanner_qualification']='Independent verifier grammar was extended for observed bare0x integer-prefix exhaustion; it is reported as validation of this captured edge, not a predeclared prediction.'
    out=ROOT/('analysis/step-distance-held-out-observations.json'if EXTRA_RUN else 'analysis/step-distance-observations.json')
    out.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(f"PASS: {len(result['imports'])} imports; {len(result['step_calls'])} mixed/lookup calls; {len(mutations)} tamper checks")


if __name__=='__main__':main()
