"""Independently verify retained original DTTYPE imports; neither executable is run."""
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

DIRECTORY=ROOT/"analysis/oracle/time-table-evaluation"
CAPTURE=ROOT/"analysis/time-table-runtime-evaluation.json"
MANIFEST=ROOT/"analysis/time-table-evaluation-cases.json"
FREEZE=ROOT/"analysis/time-table-candidate-freeze.json"
HELD_OUT=False
IDS={"DTTYPE":51,"DSMAX":48,"DTMAX":50,"GEOTYP":28,"STNOUT":34,
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
          and len(order) == len(set(order)) == (11 if HELD_OUT else 14), "fixture order")
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
    cap = evidence["normalized_result"]
    script, gate = DIRECTORY.with_name(DIRECTORY.name + "-probe.js"), DIRECTORY.with_suffix(".ready")
    template = ROOT / "tools/observe_time_table.js"
    check(Path(cap["source_path"]) == script and digest(script) == cap["source_sha256"]
          and Path(run["ready_gate"]) == gate
          and manifest["probe"]["path"] == "tools/observe_time_table.js"
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
    check(s['dimension']in(2,3) and s['object_count']==len(s['objects'])<=(3 if HELD_OUT else 0),
          'empty root fixture scope')
    check(s['alternate_enabled']==s['suppress_boundaries']==0
          and len(s['components'])==79 and s['components'][0]==s['dimension']
          and all(type(v)is int and v>=0 for v in s['components'])
          and len(s['output_flags'])==7 and all(type(v)is int for v in s['output_flags'])
          and type(s['geometry_mode'])is int,
          'root baseline profile')
    check(s['node8_default']==(20 if s['process_units_raw']==0 else 68),'global unit default')
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


def verify(evidence,run,manifest,inputs):
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
    imports=[];headers=[];scans=[];tables=[];reals=[];transitions=[];phase=None
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
            last=deepcopy(expected);handler={'id':e['id'],'before':deepcopy(last),'row':None,'reset':False,'read':False,'selected':False}
            active['handlers']+=1
        elif op=='object-selected':
            check(HELD_OUT and handler is not None and handler['id']>=144
                  and not handler['selected'] and last_scan['format']=='%i'
                  and e['requested']==last_scan['value'] and e['found']is True,'object selection pairing')
            handler['selected']=True
        elif op in('allocation-update','allocation-free'):
            check(HELD_OUT and handler is not None and handler['id']in(28,34,159,173)
                  and e['kind']in('nodes','elements') and int(e['object'],16)>0,'setup allocation context')
            # Setup mesh values are retained as original evidence, not recomputed here.
            # Scalar/table handlers are required to preserve the entire resulting state.
            for b in e['metadata'].values():
                n,k=b['allocated_items'],b['known_prefix']
                check(type(n)is int and type(k)is int and 0<=k<=n<=512
                      and b['unknown_items']==n-k and len(b['values'])==n
                      and b['values'][k:]==[None]*(n-k),'allocation observed extent/masking')
        elif op=='header-row':
            check(handler is not None and handler['id']==51 and handler['row']is None,'header helper context')
            pos=header_read(e,data,pos,stream);stream=e['after'];handler['row']=e
            headers.append(deepcopy(e));active['headers']+=1
        elif op=='real-row':
            check(handler is not None and handler['id']in(48,50)and handler['row']is None,'scalar helper context')
            pos,result=verify_real(e,data,pos,{'after':stream},handler['id'])
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
                  and (e['stream']==stream and handler['row']is not None if e['id']in(48,50,51)else HELD_OUT)
                  and reset is None and reader is None,'handler exit pairing')
            expected=deepcopy(handler['before']);c,h=expected['finalization_controls'],expected['time_controls_hex'];row=handler['row']
            if e['id']==51:
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
            else:
                check(e['status']==0 and row is None and not handler['reset']and not handler['read']
                      and handler['selected']==(e['id']>=144),'setup handler status/selection')
                check(view(e['state'])==view(expected),'setup preserves selected table and scalar controls')
                check(e['state']['linked_state']==expected['linked_state'],'setup preserves linked root')
                check(stream_position(e['stream'],data)>=pos,'setup stream monotonicity')
                pos=stream_position(e['stream'],data);stream=e['stream'];expected=deepcopy(e['state'])
            check(e['state']==expected,'handler exact captured state effects and preservation')
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
    check(active is None and len(imports)==len(manifest['cases'])==(11 if HELD_OUT else 14),'complete import inventory')
    return {'imports':imports,'header_rows':headers,'table_scans':scans,'table_readers':tables,
            'real_rows':reals,'dimension_transitions':transitions,'operation_counts':dict(Counter(e['operation']for e in events))}

def main():
    global HELD_OUT,DIRECTORY,CAPTURE,MANIFEST
    parser=argparse.ArgumentParser();parser.add_argument('--held-out',action='store_true');args=parser.parse_args()
    HELD_OUT=args.held_out
    if HELD_OUT:
        DIRECTORY=ROOT/'analysis/oracle/time-table-held-out'
        CAPTURE=ROOT/'analysis/time-table-runtime-held-out.json'
        MANIFEST=ROOT/'analysis/time-table-held-out-cases.json'
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
    reject('target PID',lambda e,r,m,f:e['normalized_result'].update(pid=1))
    reject('truncated messages',lambda e,r,m,f:e['normalized_result'].update(messages_truncated=True))
    reject('sequence gap',lambda e,r,m,f:pick(e,'table-scan').update(sequence=0))
    reject('probe error',lambda e,r,m,f:e['normalized_result']['messages'][3].update(type='error'))
    reject('header partial values',lambda e,r,m,f:pick(e,'header-row',lambda p:p['values'][1]==0).update(values=[1,2]))
    reject('header raw bytes',lambda e,r,m,f:pick(e,'header-row')['output_hex'].__setitem__(0,'00000000'))
    if not HELD_OUT:reject('header zero conversion status',lambda e,r,m,f:pick(e,'header-row',lambda p:p['scans']and p['scans'][0]['conversions']==0).update(status=1))
    reject('reset preserves field54',lambda e,r,m,f:pick(e,'table-reset-after')['table'].update(preserved_0x54=1))
    reject('reset preserves field60',lambda e,r,m,f:pick(e,'table-reset-after')['table'].update(preserved_0x60_hex='0100000000000000'))
    reject('reset incorrectly preserves mode',lambda e,r,m,f:pick(e,'table-reset-after')['table'].update(mode_0x58=1))
    reject('selector changes rank',lambda e,r,m,f:pick(e,'table-reader-before')['table'].update(rank_0x04=0))
    reject('unclamped negative count',lambda e,r,m,f:pick(e,'table-reader-after',lambda p:p['table']['counts_0x1c'][0]==0)['table']['counts_0x1c'].__setitem__(0,-1))
    reject('rank alias',lambda e,r,m,f:pick(e,'handler-after',lambda p:p['id']==51)['state']['finalization_controls'].update(control_0x93c=0))
    reject('mode alias',lambda e,r,m,f:pick(e,'handler-after',lambda p:p['id']==51)['state']['finalization_controls'].update(mode_0x990=1))
    reject('scan previous zero',lambda e,r,m,f:pick(e,'table-scan').update(previous_output_hex='000000000000f03f'))
    reject('matching failure accepted as EOF',lambda e,r,m,f:pick(e,'table-scan',lambda p:p['conversions']==0).update(conversions=-1))
    reject('partial scan output raw',lambda e,r,m,f:pick(e,'table-scan',lambda p:p['conversions']==-1).update(output_hex='000000000000f03f',value=1))
    reject('negative zero sign',lambda e,r,m,f:pick(e,'table-scan',lambda p:p['output_hex']==NEG_ZERO).update(output_hex=ZERO))
    reject('stream cursor',lambda e,r,m,f:pick(e,'table-scan')['after'].update(cursor=99))
    reject('discard bytes',lambda e,r,m,f:pick(e,'table-discard').update(hex='20'))
    reject('discard EOF result',lambda e,r,m,f:pick(e,'table-discard',lambda p:p['eof']).update(eof=False))
    def partial_array(e,r,m,f):
        t=pick(e,'table-reader-after',lambda p:p['status']==1)['table']
        t['dependent'][-1]=1;t['raw_hex']['dependent']=t['raw_hex']['dependent'][:-16]+'000000000000f03f'
    reject('consistent fabricated missing dependent value',partial_array)
    reject('array pointer aliases',lambda e,r,m,f:pick(e,'table-reader-after',lambda p:bool(p['table']['dependent']))['table']['pointers'].update(dependent='0x0'))
    reject('reader failure status',lambda e,r,m,f:pick(e,'table-reader-after',lambda p:p['status']==1).update(status=0))
    reject('finalizer stage order',lambda e,r,m,f:pick(e,'stage-before').update(stage='objects'))
    reject('finalizer ignores pinned rank mode',lambda e,r,m,f:pick(e,'stage-after',lambda p:p['stage']=='control'and p['finalization_controls']['control_0x93c']==1)['finalization_controls'].update(mode_0x990=1))
    reject('import aggregate error lost',lambda e,r,m,f:pick(e,'import-after',lambda p:p['status']==2).update(status=0))
    def drop_discard(e,r,m,f):
        ms=e['normalized_result']['messages'];i=next(i for i,x in enumerate(ms)if x['payload']['operation']=='table-discard');ms.pop(i)
        for i,x in enumerate(ms[1:],1):x['payload']['sequence']=i
    reject('removed discard with repaired sequence',drop_discard)
    if HELD_OUT:
        reject('table handler changes captured mesh',lambda e,r,m,f:pick(e,'handler-after',
            lambda p:p['id']==51 and bool(p['state']['objects']))['state']['objects'][0].update(mesh_flag=0))
        reject('type7 object incorrectly finalized',lambda e,r,m,f:pick(e,'object-stage-before').update(type=7))
        def drop_visit(e,r,m,f):
            ms=e['normalized_result']['messages']
            for op in('object-stage-before','object-stage-after'):
                i=next(i for i,x in enumerate(ms)if x['payload']['operation']==op);ms.pop(i)
            for i,x in enumerate(ms[1:],1):x['payload']['sequence']=i
        reject('missing eligible object visit with repaired sequence',drop_visit)
        def unknown(e,r,m,f):
            event=pick(e,'allocation-update',lambda p:any(b['unknown_items']for b in p['metadata'].values()))
            b=next(b for b in event['metadata'].values()if b['unknown_items']);b['values'][b['known_prefix']]=0
        reject('unknown direct tail fabricated as zero',unknown)
    reject('fixture hash',lambda e,r,m,f:m['cases'][0].update(sha256='0'*64),True)
    reject('original hash',lambda e,r,m,f:r.update(executable_sha256='0'*64),True)
    reject('probe hash',lambda e,r,m,f:e['normalized_result'].update(source_sha256='0'*64),True)
    reject('input order',lambda e,r,m,f:r['ordered_imports'].reverse(),True)
    reject('output hash',lambda e,r,m,f:r['files'][0].update(sha256='0'*64),True)
    reject('freeze archive hash',lambda e,r,m,f:f['files'][0].update(sha256='0'*64),True)
    deps=['tools/capture_oracle.py','tools/verify_boundary_sort_observations.py',
          'tools/verify_keyword_import_observations.py','tools/verify_time_controls_observations.py',
          'tools/verify_root_layout_observations.py','analysis/time-table-static-audit.json',
          'analysis/time-table-0x14012fc70.json','analysis/time-table-0x14041a800.json',
          'analysis/configuration-0x14012d2a0.json','analysis/configuration-0x14012fad0.json',
          'analysis/configuration-0x14012f480.json','analysis/configuration-0x14012fc00.json',
          'analysis/configuration-0x14012ff20.json','analysis/mesh-refresh-controls-pseudocode.json']
    report={'status':'pass','scope':__doc__,'capture':binding(CAPTURE),'evidence_id':evidence['evidence_id'],
        'run':binding(DIRECTORY/'run.json'),'pid':run['pid'],'manifest':binding(MANIFEST),
        'probe':binding(DIRECTORY.with_name(DIRECTORY.name+'-probe.js')),
        'probe_template':binding(ROOT/'tools/observe_time_table.js'),
        'pid_record':binding(DIRECTORY.with_suffix('.pid.json')),'harness':binding(ROOT/'tools/capture_keywords.py'),
        'verifier':binding(Path(__file__)),'dependencies':[binding(ROOT/p)for p in deps],
        'candidate_freeze':{'manifest':binding(FREEZE),'declared_created_utc':freeze['created_utc'],
            'verified_archives':len(freeze['files']),'current_matches':[{'path':f['path'],
                'matches':(ROOT/f['path']).is_file()and digest(ROOT/f['path'])==f['sha256']}for f in freeze['files']]},
        **result,'tampering_checks':mutations,
        'findings':[
            'Fixture originals/copies, passive probe substitution, original executable, PID, input/output transcripts and frozen candidate archives verified.',
            'Header helper output is independently interpreted from exact line bytes, including zero, partial, blank and physical EOF conversions.',
            'Successful headers reset type0x80 and restore fields54/60. All nonzero selectors create rank1; selectors3/4 set mode1; other nonzero selectors set mode0.',
            'Rank and mode are exact aliases of root93c and root990 in every captured view. Scalar mutual clearing and rank-gated mode normalization agree with static evidence.',
            'Body reads consume original stream bytes without comma replacement. Matching failure0 continues and leaves allocated zeros; EOF-1 exits with partial arrays retained and no discard.',
            'All table arrays are reconstructed from their ordered captured original scanner outputs and zero-filled missing positions, with independent fixture numeric checks and raw signed-zero validation.',
            'The final line discard occurs only after completed positive-count loops; its EOF failure does not fail a completed table.',
            'Finalization always visits control/material/object stages in order; rank1 pins mode, while rank0 permits normalization. Late first dimension header resets the captured primary/linked profiles.'],
        'limitations':[
            'No original or replacement executable runs. No replacement-generated expected values are used.',
            'This initial evaluation uses bounded rank0/1, at most two table rows, empty object/material roots and default zero preserved54/60. Runtime preservation of nonzero opaque preserved fields is not exercised; the copy contract is static.',
            'Binary64 comparisons cover finite fixture literals, not arbitrary rounding, range, locale, nonfinite forms, rank2/3 tables, interpolation or solver consumers.',
            'Positive table allocations succeeded. Failure, borrowed/aliased ownership and the unusual cleanup guard are not dynamically exercised.',
            'Full equality applies to emitted fields only. No claim is made about unobserved cleanup or scientific behavior.',
            'Frozen archive hashes are verified and current matches reported separately. Freeze chronology is declared metadata, not a trusted timestamp; harness source hash was not retained at launch.']}
    if HELD_OUT:
        report['findings'][-1]='Finalization visits control/material/object stages in order, with rank1 pinning mode and rank0 permitting normalization. The compound file changes to 2D, creates a planar mesh and placeholder/empty objects, and all selected table/scalar handlers and finalization preserve their complete captured state.'
        report['limitations'][1]='Eleven additional files, bounded rank0/1 with at most three rows. Final compound file includes reused planar setup. Setup mesh construction is not independently recomputed here; table/scalar handlers and finalizers must preserve its complete captured snapshot. Default zero preserved54/60 only.'
        report['limitations'].append('Setup allocation events are checked for context and known/unknown extent shape only. Descriptor defaults, boundary geometry and refresh preservation are covered by separate comparison; no independent full mesh validation is claimed here.')
    out=ROOT/('analysis/time-table-held-out-observations.json'if HELD_OUT else 'analysis/time-table-observations.json')
    out.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(f"PASS: {len(result['imports'])} imports; {len(result['table_readers'])} table readers; {len(result['table_scans'])} body scans; {len(mutations)} tamper checks")


if __name__=='__main__':main()
