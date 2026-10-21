"""Verify original alternate-geometry evidence independently of the C++ replay."""
import argparse
from collections import Counter
from copy import deepcopy
import json
import math
from pathlib import Path
import re
import struct

from capture_oracle import digest
from verify_boundary_sort_observations import ROOT, EXE_SHA, binding, check, read, verify_files
from verify_keyword_import_observations import held_out_scan
from verify_step_distance_observations import line_read
from verify_time_controls_observations import stream_position
from verify_geometry_import_observations import finalization as direct_finalization, shape as direct_shape

IDENTITY=[1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.]
ANGLE=0.017453292519943295


def raw(values,integer=False):return struct.pack('<'+('i'if integer else'd')*len(values),*values).hex()
def values(h,integer=False):
    b=bytes.fromhex(h);w=4 if integer else 8
    check(len(b)%w==0,'aligned raw values')
    return list(struct.unpack('<'+('i'if integer else'd')*(len(b)//w),b))


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
    script=Path(c['source_path']);template=ROOT/'tools/observe_geometry_segments.js';gate=directory.with_suffix('.ready')
    check(script in [directory.with_suffix('.js'),directory.with_name(directory.name+'-probe.js')]
          and digest(script)==c['source_sha256']and Path(run['ready_gate'])==gate,'probe identity/gate')
    check(script.read_text(encoding='utf-8-sig')==template.read_text(encoding='utf-8-sig').replace(
          '__READY_GATE__',gate.as_posix()),'exact passive probe template')
    check(manifest['probe']['path']=='tools/observe_geometry_segments.js'
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
    static=read(ROOT/'analysis/geometry-format-static-audit.json')
    for b in static['sources']:check(digest(ROOT/b['path'])==b['sha256'],'native static source binding')
    return inputs


def payload(p):
    check(p['format']in [0,1,2]and 0<=p['count']<=16,'bounded segment format/count')
    width={0:0,1:3,2:6}[p['format']];n=p['count']*width
    check(p['vtable_rva']==0x537a38 and int(p['address'],16)>0 and p['width']==width,'segment class/width')
    check(p['auxiliary_pointer']=='0x0'and p['flag_0x28']==0,'selected auxiliary/flag defaults')
    check(len(p['values'])==n and len(p['values_hex'])==16*n and values(p['values_hex'])==p['values']
          and all(math.isfinite(v)for v in p['values']),'segment payload raw bytes')
    check((int(p['data_pointer'],16)==0)==(n==0)and(n==0 or p['allocation_observed']),
          'segment allocation extent proof')


def container(c):
    check(len(c['matrix'])==16 and len(c['matrix_hex'])==256 and values(c['matrix_hex'])==c['matrix']
          and all(math.isfinite(v)for v in c['matrix'])and 0<=c['flag_0xb8']<=255,'container matrix/byte flag')
    check(c['list']['count']==len(c['segments'])<=32 and int(c['list']['sentinel'],16)>0
          and c['list']['references']>=1 and int(c['list']['storage'],16)>0,'native segment list metadata')
    for field in ['node','wrapper','address']:
        check(len({s[field]for s in c['segments']})==len(c['segments'])
              and all(int(s[field],16)>0 for s in c['segments']),'unique segment identities')
    check([s['id']for s in c['segments']]==list(range(1,len(c['segments'])+1)),'ordered wrapper IDs')
    for s in c['segments']:payload(s)


def shapes(x):
    if isinstance(x,dict):
        if {'segments','matrix_hex','flag_0xb8'}<=x.keys():container(x)
        elif {'format','width','values_hex','auxiliary_pointer'}<=x.keys():payload(x)
        if {'handle','containers','list'}<=x.keys():
            check(int(x['handle'],16)>0 and x['list']['count']==len(x['containers'])==1,
                  'selected first alternate container')
        for y in x.values():shapes(y)
    elif isinstance(x,list):
        for y in x:shapes(y)


def scan(e,data,pos):
    check(stream_position(e['before'],data)==pos,'segment scan exact continuity')
    if e['format']=='%i':
        converted=e['conversions']==1
        proxy=deepcopy(e);proxy['value']=e['value']if converted else 0
        end,_=held_out_scan(proxy,data,pos,True)
        check(e['output_hex']==(raw([e['value']],True)if converted else None)
              and(converted or e['value']is None),'integer successful output only')
        return end
    check(e['format']=='%Flf','segment scanner format')
    start=pos+len(data[pos:])-len(data[pos:].lstrip(b' \t\r\n\v\f'));end=start
    match=re.match(rb'[+-]?(?:\d+(?:\.\d*)?|\.\d+)',data[start:]);value=None;conv=0
    if match:
        word=match.group();end+=len(word);exponent=re.match(rb'[eE][+-]?\d*',data[end:])
        if exponent:
            tail=exponent.group();end+=len(tail)
            if re.search(rb'\d',tail):word+=tail
        value=float(word);check(math.isfinite(value),'finite segment scalar');conv=1
    else:
        end+=len(re.match(rb'[+-]?\.?',data[end:]).group())
        if end==len(data):conv=-1
    after={'cursor':end,'count':len(data)-end,'buffer_hex':data.hex()}
    if end==len(data):after={'cursor':0,'count':0,'buffer_hex':''}
    check(e['after']==after and e['conversions']==conv and e['value']==value
          and e['output_hex']==(raw([value])if conv==1 else None),'stream decimal grammar/EOF/raw output')
    check(int(e['destination'],16)>0,'scalar destination address')
    return end


def line_row(e,data,discarded=False):
    kinds={'geometry-header2':['int']*2,'geometry-header3':['int']*3,
           'geometry-coord3':['int']+['real']*3,'geometry-matrix4':['int']+['real']*4,
           'geometry-face5':['int']*5}
    types=kinds[e['kind']];pos=stream_position(e['before'],data);line,end,eof,after=line_read(data,pos)
    check(e['after']==after and e['lines']==[{'limit':1024,'eof':eof,'hex':None if eof else line.hex()}],
          'row exact fgets consumption')
    text=line.replace(b',',b' ');cursor=0;conv=0;out=[0]*len(types)
    for i,t in enumerate(types):
        cursor+=len(text[cursor:])-len(text[cursor:].lstrip(b' \t\r\n\v\f'))
        pattern=rb'[+-]?(?:0[xX][0-9a-fA-F]+|0[0-7]*|[1-9]\d*)'if t=='int'else rb'[+-]?(?:\d+(?:\.\d*)?|\.\d+)'
        m=re.match(pattern,text[cursor:])
        if not m:
            if i==0 and cursor==len(text):conv=-1
            break
        word=m.group();cursor+=len(word)
        if t=='real':
            tail=re.match(rb'[eE][+-]?\d*',text[cursor:])
            if tail:
                cursor+=len(tail.group())
                if re.search(rb'\d',tail.group()):word+=tail.group()
            out[i]=float(word)
        else:
            unsigned=word.lstrip(b'+-');base=16 if unsigned.lower().startswith(b'0x')else 8 if unsigned.startswith(b'0')else 10
            out[i]=int(word,base)
        conv+=1
    fmt=' '.join('%i'if t=='int'else'%Flf'for t in types)
    check(e['scans']==([]if eof else[{'input_hex':text.hex(),'format':fmt,'conversions':conv}])
          and e['status']==int(conv==-1),'line conversion count/status')
    if e['kind']=='geometry-coord3'and discarded:
        check(len(set(e['destinations'][1:]))==1,'discarded prefix real destinations alias')
        out[1:]=[out[max(1,conv-1)]if conv>=2 else 0.0]*3
    if e['kind']=='geometry-matrix4':
        pointers=[int(p,16)for p in e['destinations'][1:]]
        check(pointers==list(range(pointers[0],pointers[0]+32,8)),'matrix row destination order')
    check(e['values']==out and e['output_hex']==[raw([v],t=='int')for v,t in zip(out,types)],
          'line zeroing/partial writes/alias/raw outputs')
    if e['kind']=='geometry-header3':check(e['optional_conversion_count']==(0 if eof else conv),'header conversion count output')
    return end,out


def semantic(c):
    return [{'address':s['address'],'id':s['id'],'format':s['format'],'count':s['count'],
             'values':values(s['values_hex']),'flag_0x28':s['flag_0x28']}for s in c['segments']]


def segment_reader(before,after,events,data):
    format,count=before['first'],before['second'];check(format in [1,2]and 0<=count<=16,'safe format/row count')
    a=before['state']['containers'][0];b=after['state']['containers'][0]
    check(a['flag_0xb8']==b['flag_0xb8']==1,'completed container flag')
    expected=semantic(a);width=3 if format==1 else 6;completed=0;group=[];row=[];label=None;index=0;pending=None;failure=None
    pos=stream_position(before['stream'],data);scan_count=0;allocations=0;label_address=None;temporary=None
    for e in events:
        if e['operation']=='segment-scan':
            check(pending is None and failure is None and completed<count,'scan while active row group')
            check(e['reader_serial']==before['reader_serial'],'scan belongs to reader')
            check(e['format']==('%i'if label is None else'%Flf'),'label/value scan order')
            address=int(e['destination'],16);check(address>0,'scanner output address')
            if label is None:
                if label_address is None:label_address=address
                check(address==label_address,'label scratch destination persists')
            else:
                offset=(len(group)+len(row))*8
                if temporary is None:temporary=address-offset
                check(address==temporary+offset and 0<=offset<count*width*8,
                      'temporary scalar destination stays inside allocated group buffer')
            pos=scan(e,data,pos);scan_count+=1
            if e['conversions']!=1:failure=2 if e['conversions']==-1 else 3;continue
            if label is None:label=e['value'];row=[]
            else:
                row.append(values(e['output_hex'])[0])
                if len(row)==width:
                    group.extend(row);separator=label==-1
                    if separator or completed==count-1:
                        kept=group[:-width]if separator else group
                        pending={'format':format,'count':len(kept)//width,'values':kept,'flag_0x28':0}
                    if not separator:completed+=1
                    label=None;row=[]
        elif e['operation']=='segment-allocated':
            check(pending is not None,'allocation only after complete group')
            p=e['segment'];allocations+=1
            check(p['format']==format and p['count']==pending['count']and p['values_hex']=='00'*(pending['count']*width*8),
                  'allocation zeroes complete destination before copy')
            if index<len(expected):check(expected[index]['address']==p['address'],'overwrite retains payload object')
            pending['address']=p['address'];pending['id']=index+1
            if index<len(expected):expected[index]=pending
            else:expected.append(pending)
            index+=1;pending=None;group=[]
    check(pending is None and(failure is not None or completed==count),'reader group completion')
    status=failure or 0;check(after['status']==status and stream_position(after['stream'],data)==pos,'reader status/end cursor')
    if status==0 and format==2:
        for s in expected:
            check(len(s['values'])>=6*s['count'],'native safe stride6 retained payload')
            for i in range(s['count']):s['values'][6*i+5]*=ANGLE
    actual=semantic(b)
    check(len(expected)==len(actual),'retained/created segment count')
    for x,y in zip(expected,actual):
        check(all(x[k]==y[k]for k in ['address','id','format','count','flag_0x28'])
              and raw(x['values'])==raw(y['values']),'committed groups/trailing retention/angle exact bytes')
    check(a['matrix_hex']==b['matrix_hex']and a['address']==b['address'],'segment helper preserves matrix/container')
    return {'kind':'segments','format':format,'requested_rows':count,'status':status,'scans':scan_count,'allocations':allocations}


def transformed_reader(before,after,events):
    total,prefix=before['first'],before['second'];a=before['state']['containers'][0];b=after['state']['containers'][0]
    rows=[e for e in events if e['operation']=='geometry-row'];clears=[e for e in events if e['operation']=='segment-clear-after']
    matrices=[e for e in events if e['operation']=='segment-matrix-after']
    tails=[e for e in events if e['operation']=='segment-reader-after'and e['kind']=='segments']
    valid=0<prefix<total
    prefixrows=[e for e in rows if e['kind']=='geometry-coord3'];matrixrows=[e for e in rows if e['kind']=='geometry-matrix4']
    if not valid:
        check(after['status']==2 and before['state']==after['state']and not rows and not clears and not tails,
              'invalid transformed prefix fails without mutation')
    elif prefix in [2,6]and any(e['status']for e in prefixrows):
        check(1<=len(prefixrows)<=2 and prefixrows[-1]['status']==1 and after['status']==2
              and before['state']==after['state']and not clears and not tails,'prefix error precedes clear')
    else:
        check(len(prefixrows)==(2 if prefix in [2,6]else 0)and len(clears)==1,'prefix consumption/clear order')
        if prefix in [4,6]and any(e['status']for e in matrixrows):
            check(len(matrixrows)<=4 and matrixrows[-1]['status']==1 and not matrices and not tails
                  and after['status']==2 and a['matrix_hex']==b['matrix_hex'],'failed matrix retains prior matrix')
            check(len(b['segments'])==1 and b['segments'][0]['format']==b['segments'][0]['count']==0,
                  'failed matrix leaves newly created empty segment')
        else:
            check(len(matrixrows)==(4 if prefix in [4,6]else 0)
                  and len(matrices)==(1 if prefix in [4,6]else 0)and len(tails)==1,'complete matrix/tail call order')
            check(after['status']==tails[0]['status']and after['state']==tails[0]['state'],'transformed forwards tail status/state')
            if matrixrows:
                check(b['matrix_hex']==''.join(''.join(e['output_hex'][1:])for e in matrixrows),
                      'row-major staged matrix commit bytes')
            else:check(a['matrix_hex']==b['matrix_hex'],'nonmatrix prefix retains transform')
    return {'kind':'transformed','total':total,'prefix':prefix,'status':after['status'],'prefix_rows':len(prefixrows),
            'matrix_rows':len(matrixrows),'matrix_commits':len(matrices)}


def validate(evidence,inputs):
    c=evidence['normalized_result'];messages=c['messages'];check(len(c['scripts'])==1,'one probe script')
    script=c['scripts'][0]['script_id']
    check(all(m['type']=='send'and m['script_id']==script for m in messages),'clean send-only probe messages')
    check(messages[0]['payload']=={'operation':'hooks-installed','base':'0x140000000','pid':c['pid']},'hook identity')
    events=[m['payload']for m in messages[1:]]
    check([e['sequence']for e in events]==list(range(1,len(events)+1)),'complete ordered event sequence')
    check(not any(e['operation']=='probe-error'for e in events),'no probe errors')
    frames=[];stack=[];active=None;files=[];last_state=None;readers=[];imports=[];coverage=Counter()
    paired_ops={'handler','flags-reset','finalize','stage','object-stage','segment-reader','segment-clear','segment-matrix',
                'geometry-replace','geometry-flag'}
    for e in events:
        op=e['operation'];shapes(e)
        if op=='import-before':
            check(active is None and not frames and not stack,'import boundary nesting')
            active=e['file'];files.append(active);data=inputs[active];position=0;seen=False;handler=None;stages=[];statuses=[];unknown=[];normals=[]
            if last_state is not None:check(e['state']==last_state,'state continuity between imports')
        check(e['file']==active,'event file identity')
        for frame in frames:frame['events'].append(e)
        if op.endswith('-before')and op[:-7]in paired_ops:stack.append((op[:-7],e))
        if op.endswith('-after')and op[:-6]in paired_ops:
            check(stack and stack[-1][0]==op[:-6],'nested operation pairs');_,paired=stack.pop()
            if op=='stage-after':check(paired['stage']==e['stage'],'finalizer stage identity')
        if op=='stream-scan':
            position,_=held_out_scan(e,data,position,seen);seen=True
            if handler is not None and e['format']=='%i'and handler['selector']is None:
                handler['selector']=e['value']
        elif op=='geometry-row':
            end,out=line_row(e,data,any(f['before']['kind']=='transformed'for f in frames))
            check(stream_position(e['before'],data)>=position,'line/stream monotonicity')
            position=end;seen=True
            if handler is not None:handler['rows'].append(e)
        elif op=='handler-before':handler={'before':e,'rows':[],'calls':[],'clears':[],'selector':None}
        elif op=='segment-reader-before':
            check(handler and handler['before']['id']==158,'alternate reader inside DIEGEO')
            check(stream_position(e['stream'],data)==position,'reader entry stream continuity')
            frame={'before':e,'events':[]};frames.append(frame);handler['calls'].append(e)
            if handler['before']['state']['object_count']==0:
                cc=e['state']['containers'][0]
                check(cc['matrix_hex']==raw(IDENTITY)and len(cc['segments'])==0,'fresh alternate container defaults')
        elif op=='segment-reader-after':
            check(frames and frames[-1]['before']['reader_serial']==e['reader_serial']
                  and frames[-1]['before']['kind']==e['kind'],'reader nested identity')
            frame=frames.pop();before=frame['before']
            result=segment_reader(before,e,frame['events'],data)if e['kind']=='segments'else transformed_reader(before,e,frame['events'])
            result.update(file=active,reader_serial=e['reader_serial']);readers.append(result)
            position=stream_position(e['stream'],data);handler['calls'].append(e)
        elif op=='segment-scan':
            check(frames and frames[-1]['before']['kind']=='segments','segment scan inside simple reader')
            # Complete scanner validation is done with its reader's private cursor.
            position=stream_position(e['after'],data);seen=True
        elif op=='segment-clear-after':
            a,b=paired['state'],e['state'];check(a['address']==b['address']and a['matrix_hex']==b['matrix_hex']
                and b['segments']==[]and b['list']['count']==0 and b['flag_0xb8']==1,'clear preserves transform and empties list')
            if handler:handler['clears'].append(e)
        elif op=='segment-matrix-after':
            a,b=paired['state'],e['state'];check(a['address']==b['address']and a['segments']==b['segments']
                and b['matrix_hex']==paired['replacement_hex']and b['matrix']==paired['replacement'],'matrix setter copy only')
        elif op=='keyword-resolved':
            if e['category']==0:unknown.append(e['token'])
        elif op=='handler-after':
            check(handler and e['id']==handler['before']['id'],'handler pairing');statuses.append(e['status'])
            if e['id']==158:
                header=handler['rows'][0];dimension=handler['before']['state']['dimension'];calls=handler['calls']
                last=[x for x in calls if x['operation']=='segment-reader-after']
                selector=handler['selector'];check(selector is not None,'DIEGEO selected object')
                obj=next(x for x in e['state']['objects']if x['id']==selector)
                old={x['id']:x for x in handler['before']['state']['objects']}
                fmt=header['values'][0]
                invalid=header['status']!=0 or((dimension==2 or header.get('optional_conversion_count')==2)
                         and fmt not in [1,2]and not(dimension==3 and fmt==3))
                direct=not invalid and dimension==3 and fmt!=3 and header['optional_conversion_count']!=2
                if invalid:
                    check(not last and e['status']==2,'invalid header/format fails without alternate helper')
                    if selector in old:
                        check(obj['geometry_segments']==old[selector]['geometry_segments']
                              and obj['geometry']==old[selector]['geometry'],'invalid header preserves both geometry forms')
                    coverage['rejected_header_or_format']+=1
                elif direct:
                    check(not last and e['status']==(2 if any(r['status']for r in handler['rows'])else 0),
                          'direct branch independent row status')
                    if selector in old:check(obj['geometry_segments']==old[selector]['geometry_segments'],
                                             'direct geometry preserves alternate container')
                    else:
                        cc=obj['geometry_segments']['containers'][0]
                        check(cc['segments']==[]and cc['matrix_hex']==raw(IDENTITY),'fresh alternate defaults beside direct geometry')
                    coverage['direct_spatial_coexistence']+=1
                else:
                    check(last,'valid alternate dispatch invokes reader')
                    if dimension==3 and fmt!=3:
                        check(header['optional_conversion_count']==2 and e['status']==0,
                              'spatial two-conversion dispatch ignores helper status')
                    else:check(e['status']==(2 if last[-1]['status']else 0),'planar/transformed dispatch remaps helper failure')
                    check(obj['geometry_segments']==last[-1]['state'],'handler alternate state equals returned reader state')
                    if selector in old:check(obj['geometry']==old[selector]['geometry'],'alternate import preserves direct spatial geometry')
                    coverage['alternate_handler']+=1
            elif e['id']==145:
                pos=stream_position(handler['before']['stream'],data)
                match=re.match(rb'\s*(\d+)([^\n]*)',data[pos:]);check(match is not None,'OBJTYP selected syntax')
                identifier=int(match.group(1));pair=re.fullmatch(rb'\s*([+-]?\d+)\s+([+-]?\d+)\s*',match.group(2))
                check(pair is not None and e['status']==0,'same-line valid OBJTYP pair')
                code,secondary=map(int,pair.groups());old=next((x for x in handler['before']['state']['objects']if x['id']==identifier),None)
                obj=next(x for x in e['state']['objects']if x['id']==identifier)
                kind=code-1 if 1<=code<=5 else 6 if code==7 else (old['type']if old else 0)if code==6 else 7
                check(obj['type']==kind and obj['secondary']==secondary,'OBJTYP type/secondary')
                if kind==7:
                    cc=obj['geometry_segments']['containers'][0]
                    check(cc['segments']==[]and cc['matrix_hex']==raw(IDENTITY)and cc['flag_0xb8']==1,
                          'deactivation reconstructs alternate identity defaults')
                    check(obj['geometry']['enabled_0x3990']==0 and obj['geometry']['flag_0x3994']==(old['geometry']['flag_0x3994']if old else 0),
                          'deactivation clears spatial enable and retains3994')
                    if obj['geometry']['data']is not None:
                        g=obj['geometry']['data'];check(g['vertices']==g['face_count']==g['plane_list']['count']==0,
                                                      'deactivation clears direct geometry')
                    coverage['actual_deactivation']+=1
                else:
                    if old:
                        check(obj['geometry_segments']==old['geometry_segments']and obj['geometry']==old['geometry'],
                              'reactivation/type assignment retains geometry')
                    else:
                        cc=obj['geometry_segments']['containers'][0]
                        check(cc['segments']==[]and cc['matrix_hex']==raw(IDENTITY)
                              and obj['geometry']['enabled_0x3990']==obj['geometry']['flag_0x3994']==0,
                              'new selected object default geometries')
                    coverage['type_without_cleanup']+=1
            handler=None
        elif op=='stage-before':stages.append(e['stage'])
        elif op=='object-stage-before':normals=[]
        elif op=='geometry-normal':normals.append(e)
        elif op=='object-stage-after':
            if e['geometry']['data']is not None:direct_shape(e['geometry']['data'])
            direct_finalization(paired,e,normals)
            coverage['direct_plane_finalization'if e['flag_0x3990']else'disabled_spatial_finalizer']+=1
        elif op=='import-after':
            check(not frames and not stack and handler is None and stages==['control','materials','objects'],
                  'completed import and ordered finalization')
            check(e['status']in [0,2],'bounded import status')
            if not unknown:check(e['status']==(2 if any(statuses)else 0),'import handler status accumulation')
            imports.append({'file':active,'status':e['status'],'unknown_tokens':unknown,
                            'alternate':[x['geometry_segments']for x in e['state']['objects']]})
            last_state=e['state'];active=None
    check(active is None and files==list(inputs),'complete bounded import sequence')
    return {'event_count':len(events),'operations':dict(Counter(e['operation']for e in events)),
            'coverage':dict(coverage),'readers':readers,'imports':imports}


def main():
    p=argparse.ArgumentParser()
    p.add_argument('--capture',type=Path,default=ROOT/'analysis/geometry-segments-runtime-training.json')
    p.add_argument('--run-record',type=Path,default=ROOT/'analysis/oracle/geometry-segments-training/run.json')
    p.add_argument('--manifest',type=Path,default=ROOT/'tests/fixtures/geometry_segments/cases.json')
    p.add_argument('--freeze',type=Path,default=ROOT/'analysis/geometry-segments-candidate-freeze.json')
    p.add_argument('--input-freeze',type=Path)
    p.add_argument('--output',type=Path,default=ROOT/'analysis/geometry-segments-audit-observations.json')
    a=p.parse_args();evidence,run,manifest,freeze=map(read,[a.capture,a.run_record,a.manifest,a.freeze])
    directory=a.run_record.resolve().parent;inputs=provenance(evidence,run,manifest,freeze,directory)
    result=validate(evidence,inputs);rejected=[]
    input_freeze_path=a.input_freeze
    if input_freeze_path is None and a.manifest.name=='geometry-segments-held-out-cases.json':
        input_freeze_path=ROOT/'analysis/geometry-segments-held-out-input-freeze.json'
    input_freeze=None
    if input_freeze_path is not None:
        input_freeze=read(input_freeze_path)
        def verify_input_freeze(record):
            for key,path in [('candidate_manifest',a.freeze),('fixture_manifest',a.manifest)]:
                ref=record[key]
                check((ROOT/ref['path']).resolve()==path.resolve() and digest(path)==ref['sha256'],
                      'held-out input freeze '+key+' binding')
            check(record['candidate_current_all_28_match'] is True,'held-out freeze candidate observation')
            check([(f['path'],f['sha256'])for f in record['inputs']]==
                  [(f['path'],f['sha256'])for f in manifest['cases']]+[
                      (manifest['probe']['path'],manifest['probe']['sha256'])], 'held-out frozen fixture/probe inventory')
            for f in record['inputs']:
                check(digest(ROOT/f['archive'])==digest(ROOT/f['path'])==f['sha256'],
                      'held-out input archive/current identity')
        verify_input_freeze(input_freeze)
        for key in ['candidate_manifest','fixture_manifest','inputs']:
            damaged=deepcopy(input_freeze)
            (damaged[key][0]if key=='inputs'else damaged[key])['sha256']='0'*64
            try:verify_input_freeze(damaged)
            except ValueError:rejected.append('held-out input freeze:'+key)
            else:raise ValueError('held-out input freeze tamper accepted:'+key)
    for field,value in [('pid',-1),('source_sha256','0'*64),('messages_truncated',True)]:
        damaged=deepcopy(evidence);damaged['normalized_result'][field]=value
        try:provenance(damaged,run,manifest,freeze,directory)
        except ValueError:rejected.append('provenance:'+field)
        else:raise ValueError('provenance tamper accepted:'+field)
    for label,which in [('fixture hash','manifest'),('frozen input hash','freeze')]:
        damaged_manifest,damaged_freeze=deepcopy(manifest),deepcopy(freeze)
        (damaged_manifest['cases']if which=='manifest'else damaged_freeze['inputs'])[0]['sha256']='0'*64
        try:provenance(evidence,run,damaged_manifest,damaged_freeze,directory)
        except ValueError:rejected.append('provenance:'+label)
        else:raise ValueError('provenance tamper accepted:'+label)
    def reject(label,mutate):
        damaged=deepcopy(evidence);events=[m['payload']for m in damaged['normalized_result']['messages']]
        mutate(events)
        try:validate(damaged,inputs)
        except (ValueError,KeyError,IndexError,TypeError):rejected.append(label)
        else:raise ValueError('event tamper accepted:'+label)
    def pick(events,op,condition=lambda e:True):return next(e for e in events if e['operation']==op and condition(e))
    reject('scanner output',lambda es:pick(es,'segment-scan',lambda e:e['conversions']==1).update(output_hex='00'*4))
    reject('scanner conversion',lambda es:pick(es,'segment-scan').update(conversions=-1))
    reject('temporary destination extent',lambda es:pick(es,'segment-scan',lambda e:e['format']=='%Flf').update(destination='0x1'))
    reject('reader error status',lambda es:pick(es,'segment-reader-after',lambda e:e['status']!=0).update(status=0))
    reject('matrix replacement',lambda es:pick(es,'segment-matrix-before').update(replacement_hex='00'*128))
    reject('matrix row partial bytes',lambda es:pick(es,'geometry-row',lambda e:e['kind']=='geometry-matrix4').update(output_hex=['00000000']*5))
    reject('aliased prefix destination',lambda es:pick(es,'geometry-row',lambda e:e['kind']=='geometry-coord3'
           and len(set(e['destinations'][1:]))==1)['destinations'].__setitem__(2,'0x1'))
    reject('allocated values not zero',lambda es:pick(es,'segment-allocated',lambda e:e['segment']['count']>0)['segment']['values'].__setitem__(0,3))
    def corrupt_values(es):
        c=pick(es,'segment-reader-after',lambda e:e['kind']=='segments'and e['status']==0
               and e['state']['containers'][0]['segments']and e['state']['containers'][0]['segments'][0]['count']>0)['state']['containers'][0]
        s=c['segments'][0];s['values'][0]+=1;s['values_hex']=raw(s['values'])
    reject('coherent committed payload corruption',corrupt_values)
    reject('clear resets matrix incorrectly',lambda es:pick(es,'segment-clear-after')['state'].update(matrix_hex='00'*128,matrix=[0]*16))
    reject('finalization order',lambda es:pick(es,'stage-before').update(stage='wrong'))
    paths=[a.capture,a.run_record,directory.with_suffix('.pid.json'),a.manifest,a.freeze,Path(evidence['normalized_result']['source_path']),
           ROOT/'tools/observe_geometry_segments.js',Path(__file__),ROOT/'analysis/geometry-format-static-audit.json',
           ROOT/'analysis/geometry-segments-audit-static.json',ROOT/'tools/verify_boundary_sort_observations.py',
           ROOT/'tools/verify_keyword_import_observations.py',ROOT/'tools/verify_step_distance_observations.py',
           ROOT/'tools/verify_time_controls_observations.py',ROOT/'tools/verify_geometry_import_observations.py',ROOT/'tools/capture_oracle.py']
    if input_freeze_path is not None:paths.append(input_freeze_path)
    report={'status':'PASS','scope':'Original-only finite alternate geometry provenance, scanners, group commits, zero allocations, trailing retention, angle conversion, transformed prefix/matrix semantics and dispatch statuses.',
        'pid':run['pid'],'evidence_id':evidence['evidence_id'],'result':result,'tampering_rejected':rejected,
        'candidate_current':[{'path':f['path'],'matches_freeze':digest(ROOT/f['path'])==f['sha256']}for f in freeze['files']],
        'frozen_input_current':[{'path':f['path'],'matches_freeze':digest(ROOT/f['path'])==f['sha256']}for f in freeze['inputs']],
        'additional_frozen_inputs':[]if input_freeze is None else [
            {'path':f['path'],'archive':f['archive'],'sha256':f['sha256'],'archive_and_current_verified':True}
            for f in input_freeze['inputs']],
        'limitations':['No replacement execution; this does not establish complete importer or scientific-model equivalence.',
          'Failed scanner destinations are uninitialized/omitted and no value is inferred.',
          'Finite fixtures only; native unsafe mixed-width tails, negative counts, resource failures and unbounded separator streams excluded.',
          'Native raw list-link traversal checks are probe-owned; verifier checks emitted metadata and identities, not unreported intermediate link bytes.',
          'Direct3D coexistence verifies scanner bytes, alternate-storage preservation and original plane helper results; direct replacement prefix semantics remain covered by the separate direct-geometry verifier/comparator.',
          'Unknown keyword tokens/statuses retained; no new general parser-error policy inferred.'],
        'bindings':[binding(x)for x in paths]}
    a.output.write_text(json.dumps(report,indent=2)+'\n')
    print(f"PASS {len(inputs)} imports; {result['event_count']} events; {len(result['readers'])} readers; {len(rejected)} tamper checks")


if __name__=='__main__':main()
