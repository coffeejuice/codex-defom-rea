"""Independent original-only DIEGEO evidence checks; never execute either binary."""
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
from verify_step_distance_observations import typed_row
from verify_time_controls_observations import stream_position


def packed(values, integer=False):
    return struct.pack('<'+('i' if integer else 'd')*len(values), *values).hex()


def unpack(raw, integer=False):
    b=bytes.fromhex(raw); width=4 if integer else 8
    check(len(b)%width==0, 'raw payload alignment')
    return list(struct.unpack('<'+('i' if integer else 'd')*(len(b)//width),b))


def provenance(evidence, run, manifest, freeze, directory, expected_count):
    c=evidence['normalized_result']; p=evidence['parameters']
    check(run['status']=='completed' and run['exit_code']==0 and Path(run['cwd'])==directory,
          'harness completion/directory')
    check(Path(run['executable'])==ROOT/'source/DEF_PRE_64.exe'
          and digest(Path(run['executable']))==run['executable_sha256']==EXE_SHA, 'target hash')
    verify_files(run,directory)
    check(sum((directory/n).stat().st_size for n in ['stdout.bin','stderr.bin'])<run['max_output_bytes'],
          'transcript not at truncation bound')
    check(c['pid']==p['pid']==run['pid'] and c['session_id']==p['session_id']
          and c['target']==f"DEF_PRE_64.exe (pid {run['pid']})", 'capture process/session')
    check(c['mode']==p['mode']=='attach' and c['device_id']==p['device_id']=='local'
          and c['state']=='detached' and c['source_kind']=='file'
          and c['messages_truncated'] is False and c['cleanup_error'] is None, 'capture completion')
    script=Path(c['source_path']); template=ROOT/'tools/observe_geometry_import.js'
    gate=directory.with_suffix('.ready')
    check(script in [directory.with_suffix('.js'),directory.with_name(directory.name+'-probe.js')]
          and digest(script)==c['source_sha256'] and Path(run['ready_gate'])==gate,'probe source/gate')
    check(script.read_text(encoding='utf-8-sig')==template.read_text(encoding='utf-8-sig').replace(
          '__READY_GATE__',gate.as_posix()),'probe exact passive source')
    check(manifest['probe']['path']=='tools/observe_geometry_import.js'
          and manifest['probe']['sha256']==digest(template),'manifest probe identity')
    pid=read(directory.with_suffix('.pid.json'))
    check(all(pid[k]==run[k]for k in ['pid','executable','executable_sha256','cwd','ready_gate']),
          'PID harness identity')
    check(pid['inputs']==[{k:x[k]for k in ['name','source','copy','sha256_before']}for x in run['inputs']],
          'PID fixture identity')
    names=[Path(x['path']).name for x in manifest['cases']]
    check(len(names)==len(set(names))==expected_count and names==run['ordered_imports']
          ==[x['name']for x in run['inputs']], 'fixture order/count')
    inputs={}
    for case,item in zip(manifest['cases'],run['inputs']):
        source=ROOT/case['path']; copy=directory/item['name']
        check(Path(item['source'])==source and Path(item['copy'])==copy
              and digest(source)==digest(copy)==case['sha256']==item['sha256_before']
              ==item['sha256_after']==item['source_sha256_after'],'fixture hash/copy binding')
        data=source.read_bytes().replace(b'\r\n',b'\n')
        check(len(data)<4096 and data.isascii() and b'\0'not in data and b'\x1a'not in data,
              'bounded fixture text')
        inputs[item['name']]=data
    expected='\n2\n'+''.join('1\n'+name+'\n\n'for name in names)+'E\nE\nY\n'
    check(run['stdin']==expected,'ordered menu protocol')
    check(len(freeze['files'])==25 and len({x['path']for x in freeze['files']})==25,'freeze inventory')
    for f in freeze['files']: check(digest(ROOT/f['archive'])==f['sha256'],'frozen archive integrity')
    audit=read(ROOT/'analysis/geometry-finalize-static-audit.json')
    for item in audit['evidence_bindings']:
        check(digest(ROOT/item['path'])==item['sha256'],'static evidence binding')
    return inputs


def shape(g):
    check(int(g['address'],16)>0 and g['vtable']=='0x140548d18','geometry identity/vtable')
    for key,integer,expected in [('coordinates',False,3*g['vertices']),('faces',True,4*g['face_count']),
                                  ('face_vector_0x68',True,g['face_count'])]:
        v=g[key]; width=4 if integer else 8
        begin,end,cap=(int(v[k],16)for k in ['begin','end','capacity_end'])
        check(v['length']==expected==len(v['values']) and 0<=expected<=512
              and v['capacity']>=expected and end-begin==expected*width
              and cap-begin==v['capacity']*width and int(v['owner'],16)>0
              and v['references']==1,'geometry vector extents/private ownership')
        check((begin==0)==(v['capacity']==0) and len(v['raw_hex'])==expected*width*2
              and unpack(v['raw_hex'],integer)==v['values'],'geometry vector raw payload')
        if not integer:check(all(math.isfinite(x)for x in v['values']),'finite geometry domain')
    v=g['classification']; n=g['face_count']; pointer=int(v['pointer'],16)
    check(v['length']==n and len(v['values'])==n and len(v['raw_hex'])==8*n
          and unpack(v['raw_hex'],True)==v['values'],'classification raw extent')
    check((pointer==0)==(n==0) and (n==0 or v['allocation_observed']), 'classification allocation proof')
    p=g['plane_list']; nodes=p['planes']
    check(len(nodes)==p['count']<=32 and len({x['address']for x in nodes})==len(nodes), 'plane list bounds/uniqueness')
    check(p['head']==(nodes[0]['address']if nodes else'0x0')
          and p['tail']==(nodes[-1]['address']if nodes else'0x0'),'plane head/tail')
    for i,node in enumerate(nodes):
        check(int(node['address'],16)>0 and node['next']==(nodes[i+1]['address']if i+1<len(nodes)else'0x0'),
              'plane next-chain')
        for field in ['point','normal']:
            check(len(node[field])==3 and len(node[field+'_hex'])==48
                  and unpack(node[field+'_hex'])==node[field]
                  and all(math.isfinite(x)for x in node[field]),'plane raw triplet')


def geometry_shapes(value):
    if isinstance(value,dict):
        if {'coordinates','faces','classification','face_vector_0x68','plane_list'}<=value.keys():shape(value)
        for item in value.values():geometry_shapes(item)
    elif isinstance(value,list):
        for item in value:geometry_shapes(item)


def square(v):return (v[0]*v[0]+v[1]*v[1])+v[2]*v[2]


def normal(points):
    a,b,c=points;u=[c[i]-b[i]for i in range(3)];v=[a[i]-b[i]for i in range(3)]
    n=[u[1]*v[2]-u[2]*v[1],u[2]*v[0]-u[0]*v[2],u[0]*v[1]-u[1]*v[0]]
    length=math.sqrt(square(n))
    return [x/length for x in n]if length!=0 else n


def finalization(before,after,observations):
    check(before['object']==after['object'] and before['option']==after['option']==0
          and before['type']==after['type'] and before['flag_0x3990']==after['flag_0x3990'],
          'object finalizer identity/gate')
    a=before['geometry'];b=after['geometry']
    check(all(a[k]==b[k]for k in ['enabled_0x3990','flag_0x3994','slot0','snapshot_completeness']),
          'finalizer geometry identity/flags')
    if not before['flag_0x3990']:
        check(a==b and not observations,'disabled finalizer unchanged');return
    g,h=a['data'],b['data'];check(g is not None and h is not None,'enabled spatial geometry')
    for k in ['address','vtable','vertices','face_count','coordinates','faces','face_vector_0x68']:
        check(g[k]==h[k],'finalizer input payload preserved')
    coords=[unpack(g['coordinates']['raw_hex'][i:i+48])for i in range(0,len(g['coordinates']['raw_hex']),48)]
    lo=[1e30]*3;hi=[-1e30]*3
    for row in coords:
        for i,x in enumerate(row):
            if x<=lo[i]:lo[i]=x
            if hi[i]<=x:hi[i]=x
    total=0.0
    for i in range(3):total=total+(hi[i]-lo[i])*(hi[i]-lo[i])
    scale=math.sqrt(total)if coords and g['face_count']else 0.0
    expected=[];classes=list(g['classification']['values']);norm_index=0
    for face in range(g['face_count']):
        ids=g['faces']['values'][face*4:face*4+4]
        if ids[3]>=1:continue
        check(all(1<=i<=len(coords)for i in ids[:3]),'eligible valid coordinate indices')
        pts=[coords[i-1]for i in ids[:3]];n=normal(pts)
        check(norm_index<len(observations),'missing normal helper event')
        e=observations[norm_index];norm_index+=1
        check(e['points_hex']==[packed(x)for x in pts]and e['normal_hex']==packed(n),
              'normal helper exact point bytes/floating order')
        if math.sqrt(square(n))<1e-6:continue
        winner=None
        for plane in expected:
            d=[n[i]-plane['normal'][i]for i in range(3)]
            dot=((plane['normal'][0]*(pts[0][0]-plane['point'][0])+
                  plane['normal'][1]*(pts[0][1]-plane['point'][1]))+
                 plane['normal'][2]*(pts[0][2]-plane['point'][2]))
            if square(d)<0.01 and abs(dot)<0.001*scale:winner=plane;break
        if winner is None:
            winner={'id':len(expected)+1,'point':pts[0],'normal':n};expected.append(winner)
        classes[face]=winner['id']+600
    check(norm_index==len(observations),'extra normal helper event')
    check(len(expected)==h['plane_list']['count'],'cleared/rebuilt plane count')
    for x,y in zip(expected,h['plane_list']['planes']):
        check(x['id']==y['id']and packed(x['point'])==y['point_hex']
              and packed(x['normal'])==y['normal_hex'],'first-match plane order/origin/normal')
    check(classes==h['classification']['values'] and packed(classes,True)==h['classification']['raw_hex'],
          'classification mapping and skipped-face preservation')


def validate(evidence,inputs):
    c=evidence['normalized_result'];messages=c['messages'];script=c['scripts'][0]['script_id']
    check(len(c['scripts'])==1 and all(m['type']=='send'and m['script_id']==script for m in messages),
          'single clean probe script')
    check(messages[0]['payload']=={'operation':'hooks-installed','base':'0x140000000','pid':c['pid']},'hooks identity')
    ev=[m['payload']for m in messages[1:]]
    check([e['sequence']for e in ev]==list(range(1,len(ev)+1)),'complete sequence')
    check(all(e['operation']!='probe-error'for e in ev),'probe-error absent')
    groups=[];active=None;stack=[];stages=[];summary=[];last_import_state=None;coverage=Counter()
    for e in ev:
        op=e['operation'];geometry_shapes(e)
        if op=='import-before':
            check(active is None and not stack,'import nesting');active=e['file'];groups.append(active)
            if last_import_state is not None:check(e['state']==last_import_state,'tracked state persists between imports')
            data=inputs[active];stream_end=0;seen=False;handler=None;replacement=None;object_before=None
            normal_events=[];handler_rows=[];expected_geometry=None;coordrow=facerow=0;statuses=[];stages=[];classification_alloc=None
        check(active==e['file'],'event belongs to active import')
        if op.endswith('-before')and op in ['flags-reset-before','handler-before','finalize-before','stage-before','object-stage-before','geometry-replace-before','geometry-flag-before']:
            stack.append((op[:-7],e))
        elif op.endswith('-after')and op in ['flags-reset-after','handler-after','finalize-after','stage-after','object-stage-after','geometry-replace-after','geometry-flag-after']:
            check(stack and stack[-1][0]==op[:-6],'ordered before/after nesting')
            _,paired=stack.pop()
            if op.startswith('stage-'):check(paired['stage']==e['stage'],'stage pairing')
        if op=='stream-scan':stream_end,_=held_out_scan(e,data,stream_end,seen);seen=True
        if op=='handler-before':
            handler=e;handler_rows=[];expected_geometry=None;coordrow=facerow=0;geometry_address=None;flag_writes=[]
        if op=='geometry-row':
            check(handler is not None and handler['id']==158,'geometry rows within handler158')
            types={'geometry-header3':['int']*3,'geometry-coord3':['int']+['real']*3,'geometry-face5':['int']*5}[e['kind']]
            pos=stream_position(e['before'],data);check(pos>=stream_end,'row stream monotonic')
            stream_end=typed_row(e,data,pos,e['before'],types);seen=True;handler_rows.append(e)
            if e['kind']=='geometry-header3':
                check(e['optional_conversion_count']==(e['scans'][0]['conversions']if e['scans']else 0),
                      'header conversion-count output')
            elif expected_geometry is not None:
                key,index,width=('coordinates',coordrow,3)if e['kind']=='geometry-coord3'else('faces',facerow,4)
                start=index*width;check(start+width<=len(expected_geometry[key]),'row in allocated extent')
                expected_geometry[key][start:start+width]=unpack(''.join(e['output_hex'][1:]),key=='faces')
                if key=='coordinates':coordrow+=1
                else:facerow+=1
        if op=='geometry-replace-before':
            replacement=e;check(handler_rows and handler_rows[0]['status']==0,'replace after successful header')
            check([e['requested_vertices'],e['requested_faces']]==handler_rows[0]['values'][1:],'header allocation args')
            if handler['state']['object_count']==0:
                check(e['state']['vertices']==e['state']['face_count']==e['state']['plane_list']['count']==0,
                      'fresh object geometry empty before first replacement')
        if op=='geometry-classification-allocated':
            check(replacement and e['geometry']==replacement['geometry'],'classification inside replacement')
            expected=max(0,replacement['requested_faces'])or replacement['state']['face_count']
            check(e['count']==expected and(int(e['pointer'],16)>0 if expected else True),'classification allocation extent')
            classification_alloc=e
        if op=='geometry-replace-after':
            a=paired['state'];b=e['state'];check(a['address']==b['address']==e['geometry'],'geometry allocation identity')
            geometry_address=e['geometry']
            check(classification_alloc and classification_alloc['pointer']==b['classification']['pointer'],
                  'allocation pointer equals observed classification buffer')
            classification_alloc=None
            expected_geometry={}
            for key,countkey,width,requested in [('coordinates','vertices',3,paired['requested_vertices']),
                       ('faces','face_count',4,paired['requested_faces']),('face_vector_0x68','face_count',1,b['face_count'])]:
                count=requested if requested>0 else a[countkey]
                check(a[key]['owner']==b[key]['owner'],'private COW owner identity across resize')
                old=unpack(a[key]['raw_hex'],key!='coordinates');expected=(old+[0]*(count*width))[:count*width]
                check(b[countkey]==count and b[key]['values']==expected,'resize prefix/zero-growth/nonpositive preservation')
                check(b[key]['raw_hex']==packed(expected,key!='coordinates'),'resize preserves exact prefix bytes')
                expected_geometry[key]=expected
            check(b['classification']['values']==[0]*b['face_count'] and a['plane_list']==b['plane_list'],
                  'fresh classifications and retained plane list at replacement')
            expected_geometry['classification']=b['classification']['values'];replacement=None
        if op=='geometry-flag-after':
            a=deepcopy(paired['geometry']);field=e['field']
            check(e['field']in ['enabled_0x3990','flag_0x3994']and paired['field']==e['field']and e['value']==paired['value']==1,
                  'geometry setter field/value')
            a[field]=1;check(a==e['geometry'],'geometry setter only selected field')
            flag_writes.append(field)
        if op=='handler-after':
            check(handler and e['id']==handler['id'],'handler identity')
            statuses.append(e['status'])
            if e['id']==158:
                failed=any(x['status']for x in handler_rows)
                check(e['status']==(2 if failed else 0),'geometry row failure propagates')
                check(flag_writes==([]if failed else['flag_0x3994','enabled_0x3990']),
                      'success flag order and failure preserves flags')
                if expected_geometry is not None:
                    selected=[x for x in e['state']['objects']if x['geometry']['data']and x['geometry']['data']['address']==geometry_address]
                    # Geometry address comes from the allocation event, not replacement output.
                    check(len(selected)==1,'selected geometry object');g=selected[0]['geometry']['data']
                    for key in ['coordinates','faces','face_vector_0x68','classification']:
                        check(g[key]['values']==expected_geometry[key],'row prefix/partial mutation applied in order')
                        check(g[key]['raw_hex']==packed(expected_geometry[key],key!='coordinates'),
                              'row exact written bytes including signed zero')
                    if not failed:
                        check(coordrow==g['vertices'] and facerow==(g['face_count']if handler_rows[0]['values'][0]else 0),
                              'complete successful geometry row count')
                old={x['id']:x['geometry']for x in handler['state']['objects']}
                for obj in e['state']['objects']:
                    if obj['id']in old:
                        for flag in ['enabled_0x3990','flag_0x3994']:
                            check(obj['geometry'][flag]==(old[obj['id']][flag]if failed else 1),
                                  'geometry flags preserved on failed row')
                        if not expected_geometry:check(obj['geometry']==old[obj['id']],'failed header leaves geometry unchanged')
            elif e['id']==145:
                pos=stream_position(handler['stream'],data)
                selector=re.match(rb'\s*(\d+)',data[pos:])
                check(selector is not None,'selected OBJTYP selector syntax')
                identifier=int(selector.group(1))
                remainder=data[pos+selector.end():].split(b'\n',1)[0]
                if not remainder.strip():
                    check(e['status']==2 and e['state']==handler['state'],
                          'blank same-line OBJTYP pair fails without deactivation')
                    coverage['OBJTYP_blank_pair_failure']+=1
                    handler=None
                    continue
                match=re.fullmatch(rb'\s*([+-]?\d+)\s+([+-]?\d+)\s*',remainder)
                check(match is not None,'selected OBJTYP same-line pair syntax')
                code,secondary=map(int,match.groups())
                check(e['status']==0,'valid same-line OBJTYP pair succeeds')
                old=next(x for x in handler['state']['objects']if x['id']==identifier)
                obj=next(x for x in e['state']['objects']if x['id']==identifier)
                check(obj['secondary']==secondary,'OBJTYP secondary survives geometry cleanup')
                if code==99:
                    coverage['geometry_deactivation']+=1
                    check(obj['type']==7 and obj['geometry']['enabled_0x3990']==0
                          and obj['geometry']['flag_0x3994']==old['geometry']['flag_0x3994'],
                          'deactivation clears enable and preserves3994')
                    g=obj['geometry']['data'];check(g['vertices']==g['face_count']==g['plane_list']['count']==0,
                          'deactivation clears geometry arrays and planes')
                elif code==2:
                    coverage['geometry_reactivation']+=1
                    check(obj['type']==1 and obj['geometry']==old['geometry'],'reactivation preserves cleared geometry')
                else:raise ValueError('OBJTYP outside selected held-out scope')
            handler=None
        if op=='stage-before':stages.append(e['stage'])
        if op=='object-stage-before':object_before=e;normal_events=[]
        if op=='geometry-normal':check(object_before is not None,'normal inside finalizer');normal_events.append(e)
        if op=='object-stage-after':finalization(object_before,e,normal_events);object_before=None
        if op=='import-after':
            check(not stack and handler is None and stages==['control','materials','objects'],'complete finalization stages')
            check(e['status']==(2 if 2 in statuses else 0),'import status aggregation')
            if data.strip()==b'* DEFORM-2D':
                check(e['state']['dimension']==2 and e['state']['object_count']==0
                      and e['state']['objects']==[] and e['state']['linked_state']['dimension']==3,
                      'header-only dimension reset clears spatial objects')
            summary.append({'file':active,'status':e['status'],'geometry':[x['geometry']for x in e['state']['objects']]})
            last_import_state=e['state']
            active=None
    check(active is None and groups==list(inputs),'complete ordered imports')
    return {'events':len(ev),'operations':dict(Counter(x['operation']for x in ev)),
            'lifecycle_coverage':dict(coverage),'imports':summary}


def main():
    parser=argparse.ArgumentParser();group=parser.add_mutually_exclusive_group()
    group.add_argument('--held-out',action='store_true');group.add_argument('--lifecycle',action='store_true');args=parser.parse_args()
    suffix='lifecycle'if args.lifecycle else'held-out'if args.held_out else'training'
    capture=ROOT/f'analysis/geometry-import-runtime-{suffix}.json'
    directory=ROOT/f'analysis/oracle/geometry-import-{suffix}'
    manifest=ROOT/(f'analysis/geometry-import-{suffix}-cases.json'if args.held_out or args.lifecycle
                   else'tests/fixtures/geometry_import/cases.json')
    freeze=ROOT/'analysis/geometry-import-candidate-freeze.json'
    evidence,run,cases,frozen=map(read,[capture,directory/'run.json',manifest,freeze])
    inputs=provenance(evidence,run,cases,frozen,directory,len(cases['cases']))
    result=validate(evidence,inputs);checks=[]
    for field,value in [('source_sha256','0'*64),('pid',-1),('messages_truncated',True)]:
        copy=deepcopy(evidence);copy['normalized_result'][field]=value
        try:provenance(copy,run,cases,frozen,directory,len(cases['cases']))
        except ValueError:checks.append('provenance:'+field)
        else:raise ValueError('provenance tamper not rejected '+field)
    damaged=deepcopy(cases);damaged['cases'][0]['sha256']='0'*64
    try:provenance(evidence,run,damaged,frozen,directory,len(cases['cases']))
    except ValueError:checks.append('provenance:fixture hash')
    else:raise ValueError('fixture tamper not rejected')
    events=evidence['normalized_result']['messages']
    for op,key,value in [('geometry-normal','normal_hex','00'*24),('geometry-row','output_hex',['00000000']*3),
                          ('geometry-replace-before','requested_faces',99),('geometry-flag-after','value',0),
                          ('import-after','status',99),('stage-before','stage','wrong')]:
        copy=deepcopy(evidence);target=next(m['payload']for m in copy['normalized_result']['messages']if m['payload']['operation']==op)
        target[key]=value
        try:validate(copy,inputs)
        except (ValueError,KeyError,IndexError,TypeError):checks.append(op+':'+key)
        else:raise ValueError('tamper not rejected '+op+':'+key)
    for field in ['classification','plane_list','coordinates']:
        copy=deepcopy(evidence);target=next(m['payload']['geometry']['data']for m in copy['normalized_result']['messages']
            if m['payload']['operation']=='object-stage-after'and m['payload']['geometry']['data']['face_count']>0)
        if field=='classification':
            target[field]['values'][0]+=1
            target[field]['raw_hex']=packed(target[field]['values'],True)
        elif field=='plane_list':target[field]['head']='0x0'
        else:target[field]['end']='0x0'
        try:validate(copy,inputs)
        except ValueError:checks.append(field)
        else:raise ValueError('tamper not rejected '+field)
    copy=deepcopy(evidence)
    target=next(m['payload']['geometry']['data']['plane_list']['planes'][0]
        for m in copy['normalized_result']['messages']if m['payload']['operation']=='object-stage-after'
        and m['payload']['geometry']['data']['plane_list']['count']>0)
    target['normal'][2]+=0.25;target['normal_hex']=packed(target['normal'])
    try:validate(copy,inputs)
    except ValueError:checks.append('plane coherent raw/value corruption')
    else:raise ValueError('coherent plane corruption not rejected')
    current=[{'path':f['path'],'matches_freeze':digest(ROOT/f['path'])==f['sha256']}for f in frozen['files']]
    report={'status':'PASS','scope':'Original provenance, finite3D geometry buffer/row mutation, plane grouping and selected event nesting; no replacement execution.',
        'evidence_id':evidence['evidence_id'],'pid':run['pid'],'result':result,'tampering_rejected':checks,
        'candidate_current':current,'limitations':['No full mesh/descriptor or complete application model validation.',
        'Finite arithmetic/default FP environment; excludes NaNs, overflow, malformed eligible indices, shared buffers and OOM.',
        'Original plane addresses retained as observations only; not portable expectations.'],
        'bindings':[binding(p)for p in [capture,directory/'run.json',directory.with_suffix('.pid.json'),manifest,freeze,
            Path(evidence['normalized_result']['source_path']),ROOT/'tools/observe_geometry_import.js',Path(__file__),
            ROOT/'tools/verify_step_distance_observations.py',ROOT/'tools/verify_time_controls_observations.py',
            ROOT/'tools/verify_keyword_import_observations.py',ROOT/'tools/verify_boundary_sort_observations.py',
            ROOT/'tools/capture_oracle.py',ROOT/'analysis/geometry-finalize-static-audit.json']]}
    if args.held_out:
        report['research_corrections']=[{
            'files':['11_deactivate.KEY','12_reactivate.KEY'],
            'original_intent':'Exercise object deactivation and reactivation after geometry import.',
            'authoring_error':'Recovery inventory agent placed the OBJTYP type/secondary pair on the next line, assuming whitespace scanning across lines. The handler reads the remainder of the selector line via fgets.',
            'observation':'Both blank pair lines return handler/import status2; subsequent99/2 tokens resolve unknown keyword305/category0. Existing geometry remains enabled and finalizes again; deactivation/reactivation were not exercised.',
            'preservation':'Original fixtures, predeclared manifest and capture remain unchanged.',
            'followup':'Separate geometry_import/lifecycle inputs use OBJTYP 1 99 37 and OBJTYP 1 2 43 on one line; lifecycle coverage requires that separate original capture.'}]
    output=ROOT/f'analysis/geometry-import-{suffix}-observations.json';output.write_text(json.dumps(report,indent=2)+'\n')
    print(f"PASS {len(inputs)} imports; {result['events']} events; {len(checks)} tamper rejections")


if __name__=='__main__':main()
