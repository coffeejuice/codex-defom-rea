import pathlib,json,hashlib,shutil,copy,datetime
ROOT=pathlib.Path(r'C:\Users\alext\.codex\worktrees\chat-b\codex-deform-rea')
A=ROOT/'analysis';F=A/'chat-b-roundtrip-fixtures';B=A/'chat-b-keyword-metadata-before'
hashf=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert not B.exists(), 'Before snapshot exists; refuse overwrite'
assert hashf(F/'manifest.json')=='39856cc53aa3b4cf2871307127a7c1fd9991a214bbaa010e885f5d54a281bc42'
old=json.loads((F/'manifest.json').read_text());before=[]
for p in [*F.glob('*.KEY'),F/'manifest.json',A/'chat-b-roundtrip-blockers.json']:
 rel=p.relative_to(A);dst=B/rel;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,dst);before.append({'path':str(p),'snapshot':str(dst),'sha256':hashf(p),'bytes':p.stat().st_size})
B.mkdir(exist_ok=True);(B/'receipt.json').write_text(json.dumps({'files':before,'purpose':'Immutable before revision R1-R5'},indent=2)+'\n')
def write(name,text):
 (F/name).write_bytes(text.replace('\n','\r\n').encode('ascii'))
D={'title':'SIMULATION','operation_name':'OPERATION 1','simulation_name':'SIMULATION 1','cursim':[1,0,0]}
P={'title':'Primary title sentinel','operation_name':'Primary operation sentinel','simulation_name':'Primary simulation sentinel','cursim':[19,28,37]}
L={'title':'Linked title sentinel','operation_name':'Linked operation sentinel','simulation_name':'Linked simulation sentinel','cursim':[46,55,64]}
def seed(st):return 'TITLE\n'+st['title']+'\nOPRNAM\n'+st['operation_name']+'\nSIMNAM\n'+st['simulation_name']+'\nCURSIM '+' '.join(map(str,st['cursim']))+'\n'
write('setup-primary-metadata.KEY',seed(P));write('setup-linked-metadata.KEY',seed(L))
write('12-dimension-resets-metadata.KEY','* DEFORM-2D V11.0 KEYWORD FILE\n')
write('13-first-header-wins.KEY','* ordinary comment\n* DEFORM-3D V11.0 KEYWORD FILE\n* DEFORM-2D V11.0 KEYWORD FILE\n')
write('16-same-dimension-preserves.KEY','* DEFORM-3D V11.0 KEYWORD FILE\n')
write('17-new-import-header-opportunity.KEY','* DEFORM-2D V11.0 KEYWORD FILE\n')
write('09-cursim-partial.KEY','CURSIM 07,0x5,0\nCURSIM 7 bad 9\nCURSIM 9 8 7\nCURSIM junk\nCURSIM 9 8 7\nCURSIM 7 5 bad\nCURSIM 010,0x10,-3\n')
capacity=[];capacityrows=[]
for kw,m,char in [('TITLE',80,'T'),('OPRNAM',64,'O'),('SIMNAM',80,'S')]:
 for mode,n,last in [('maximum_minus1',m-1,'x'),('maximum',m,'y'),('maximum_plus1',m+1,'z'),('truncate_before_trim',m+1,'!')]:
  if mode=='maximum_plus1':payload=char*(m-1)+'z'+'!'
  elif mode=='truncate_before_trim':payload=char*(m-1)+' '+'!'
  else:payload=char*(n-1)+last
  capacity.extend([kw,payload]);capacityrows.append({'keyword':kw,'capacity_including_NUL':m+1,'payload_length':len(payload),'mode':mode,'expected_value':payload[:m].rstrip(' \r\n\0')})
capacity.append('CURSIM 31 32 33');write('07-capacity-boundaries.KEY','\n'.join(capacity)+'\n')
cases=copy.deepcopy(old['cases'])
for name in ['16-same-dimension-preserves.KEY','17-new-import-header-opportunity.KEY']:
 c=copy.deepcopy(cases[11]);c['file']=name;cases.append(c)
setupseq=[{'target':'primary','file':'setup-primary-metadata.KEY'},{'target':'linked','file':'setup-linked-metadata.KEY'}]
for c in cases:
 c['execution_status']='NOT_RUN';c['runtime_oracle']=None;c['setup_imports']=[]
 c['initial_metadata']={'primary':copy.deepcopy(D),'linked':copy.deepcopy(D)}
 if c['file'] in ['07-capacity-boundaries.KEY','12-dimension-resets-metadata.KEY','13-first-header-wins.KEY','16-same-dimension-preserves.KEY','17-new-import-header-opportunity.KEY']:
  c['setup_imports']=copy.deepcopy(setupseq);c['initial_metadata']={'primary':copy.deepcopy(P),'linked':copy.deepcopy(L)}
  c['initial_state']='Initialize primary dimension3 and linked dimension2 separately; link without changing these dimensions; import listed seeds into the named root directly. Ordinary metadata dispatch always targets that import root; no alternate-root keyword routing.'
 if c['file'].startswith('17-'):c['setup_imports'].append({'target':'primary','file':'13-first-header-wins.KEY'})
 if c['file'].startswith('07-'):c['capacity_assertions']=capacityrows
# Only explicitly specified valid bounded numeric strings are admitted to this annotation generator.
ints={'07 0x5 0':([7,5,0],3,0),'7 bad 9':([7,0,0],1,0),'9 8 7':([9,8,7],3,0),'junk':([0,0,0],0,0),'7 5 bad':([7,5,0],2,0),'010 0x10 -3':([8,16,-3],3,0),'7 5 0':([7,5,0],3,0),'31 32 33':([31,32,33],3,0),'19 28 37':([19,28,37],3,0),'46 55 64':([46,55,64],3,0),'':([0,0,0],-1,1)}
import re
field={'TITLE':('title',81),'OPRNAM':('operation_name',65),'SIMNAM':('simulation_name',81)}
def annotate(c):
 raw=(F/c['file']).read_bytes();data=raw.replace(b'\r\n',b'\n');assert b'\r' not in data and b'\0' not in data
 # Boundary mapping is lexical CRLF translation, never an asserted CRT ftell.
 physical=[0];i=0
 while i<len(raw):
  i+=2 if raw[i:i+2]==b'\r\n' else 1;physical.append(i)
 assert len(physical)==len(data)+1
 pos=0;reads=[];events=[];states=copy.deepcopy(c['initial_metadata']);dims={'primary':3,'linked':2};header_seen=False;sticky=0
 def line(stage):
  nonlocal pos
  start=pos
  if pos==len(data):status=1
  else:
   end=data.find(b'\n',pos);pos=min(len(data),pos+1023) if end<0 else min(end+1,pos+1023);status=0
  row={'stage':stage,'translated_offset':start,'translated_cursor':pos,'physical_source_offset':physical[start],'physical_source_cursor':physical[pos],'status':status,'bytes_hex':data[start:pos].hex()};reads.append(row);return data[start:pos],status
 while True:
  while pos<len(data) and data[pos] in b' \t\n\r\v\f':pos+=1
  if pos==len(data):break
  start=pos
  while pos<len(data) and data[pos] not in b' \t\n\r\v\f' and pos-start<39:pos+=1
  token=data[start:pos].decode();entry=pos;firstread=len(reads);beforestate=copy.deepcopy(states)
  event={'token':token,'offset':start,'handler_entry_cursor':entry,'physical_source_token_offset':physical[start],'physical_source_handler_entry_cursor':physical[entry],'metadata_before':beforestate}
  if token.startswith('*'):
   remainder,status=line('comment_or_header_remainder');match=re.search(rb'DEFORM-([23])D',remainder);applied=False
   if match and not header_seen:
    dimension=int(match.group(1));header_seen=True;applied=True
    for who,dim in [('primary',dimension),('linked',5-dimension)]:
     if dims[who]!=dim:states[who]=copy.deepcopy(D);dims[who]=dim
   event.update(kind='header_or_comment',recognized_dimension=int(match.group(1)) if match else None,applied=applied,observer_callback=False)
  elif token in field:
   key,cap=field[token];_,status=line('discard_inline_remainder')
   if status==0:
    payload,status=line('string_payload')
    if status==0:states['primary'][key]=payload.split(b'\n',1)[0][:cap-1].rstrip(b' \r\n\0').decode()
   handler=0 if status==0 else 2;sticky=handler or sticky
   event.update(kind='metadata_handler',helper='0x140413ab0',helper_status=status,handler_status=handler,capacity_including_NUL=cap,observer_callback=True)
  elif token=='CURSIM':
   states['primary']['cursim']=[0,0,0];event['metadata_after_zeroing_before_line_read']=copy.deepcopy(states)
   payload,status=line('integer_triple_remainder');key=' '.join(payload.decode().replace(',',' ').split());assert key in ints,(c['file'],key)
   vals,conversions,helper=ints[key];states['primary']['cursim']=copy.deepcopy(vals);handler=helper*2;sticky=handler or sticky
   event.update(kind='metadata_handler',helper='0x140413fb0',helper_status=helper,handler_status=handler,scan_conversions=None if status else conversions,observer_callback=True)
  else:
   line('unknown_remainder');event.update(kind='unknown',observer_callback=False,unknown_warning=True)
  event.update(cursor=pos,physical_source_cursor=physical[pos],line_read_attempts=len(reads)-firstread,line_reads=copy.deepcopy(reads[firstread:]),cumulative_line_reads=len(reads),metadata_after=copy.deepcopy(states),dimensions_after=copy.deepcopy(dims),parser_status_after_event=sticky)
  nxt=pos
  while nxt<len(data) and data[nxt] in b' \t\n\r\v\f':nxt+=1
  event['next_token_offset_or_eof']=nxt;events.append(event)
 c['bytes']=len(raw);c['sha256']=hashf(F/c['file'])
 c['trace']={'coordinate_system':'Zero-based byte boundaries in explicit CRLF-to-LF translated input; physical_source fields refer to original file boundaries, not native ftell.','physical_bytes':len(raw),'translated_bytes':len(data),'translated_sha256':hashlib.sha256(data).hexdigest(),'events':events,'total_line_read_attempts':len(reads),'expected_parser_cursor':pos,'expected_reached_eof':True,'capture_status':'PROPOSED_NOT_RUN'}
 c['expected_after_future_metadata_implementation']={**copy.deepcopy(states['primary']),'linked_metadata':copy.deepcopy(states['linked']),'primary_dimension':dims['primary'],'linked_dimension_if_present':dims['linked'],'handler_statuses':[e['handler_status'] for e in events if 'handler_status' in e],'parser_final_status':sticky,'cursim_after_each':[e['metadata_after']['primary']['cursim'] for e in events if e['token']=='CURSIM']}
 c['adjacent_field_assertion']='At each metadata handler, compare all four primary fields and all four linked fields; only the selected primary destination may change. At header boundaries compare every field on both roots.'
 return c
cases=[annotate(c) for c in cases]
manifest=copy.deepcopy(old);manifest['schema_version']=2;manifest['cases']=cases
manifest['execution']='NOT_RUN: generator computes source-derived lexical annotations only. No candidate, original, replay, compilation or compatibility test executed. Future import_translated uses explicit CRLF->LF bytes and request_finalization=false. R6 remains a separately authorized runtime acceptance gate.'
manifest['setup_files']=[{'file':name,'bytes':(F/name).stat().st_size,'sha256':hashf(F/name),'status':'NOT_RUN','target':who,'expected_metadata':st} for name,who,st in [('setup-primary-metadata.KEY','primary',P),('setup-linked-metadata.KEY','linked',L)]]
manifest['setup_protocol']='For each case start fresh roots with initialized simulation profiles, dimensions3/2, then link without dimension change. Apply setup_imports in listed order to the named root as independent imports with finalization=false. Each import has fresh first-header recognition. Linked seed is a direct import into that root, not alternate-root dispatch. Do not count setup events in action offsets/read counts.'
manifest['observation_plan']={'handlers':'After handler return use HandlerObserver to compare offset,cursor,status and all metadata; in CURSIM failure case15 assert zeros before later TITLE event.','helper_reads':'Use separate future helper-level calls with KeywordReader(data,handler_entry_cursor), public name/material_name/integer_triple, and line_reads() to compare attempt count, status, offsets/cursors and bytes. Current HandlerObserver does not expose reader internals.','headers':'Comments/headers have no HandlerObserver callback. Final both-root state is asserted for header-only actions12/13/16/17. Trace header snapshots are source-derived future probe targets, not claims of current callback availability.','native':'Native captures need separate authorization and record file mode/CRT/locale; translated cursor fields are not promises about native ftell.'}
manifest['review_source']={'path':r'C:\Users\alext\.codex\worktrees\chat-c-recovery\codex-deform-rea\analysis\chat-c-keyword-metadata-review.json','sha256':'356a172d932418578c93bf9343daf1ad7282060651364e9ad9b8ec324597971c'}
(F/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
blockpath=A/'chat-b-roundtrip-blockers.json';block=json.loads(blockpath.read_text());plan=block['recommended_next_implementation_slice']
plan['reuse']='Reuse existing public KeywordReader::integer_triple() for CURSIM; store all three zero-initialized result values even on failure. Reuse name(previous) for TITLE/SIMNAM (capacity81) and material_name(previous) for OPRNAM (capacity65); these are text readers and do not mutate object/material records. No new generic capacity API is required.'
plan['existing_api_anchors']=[{'file':'src/recovered/keyword_reader.cpp','line':236,'api':'integer_triple()'},{'file':'src/recovered/keyword_reader.cpp','line':839,'api':'name(previous)'},{'file':'src/recovered/keyword_reader.cpp','line':843,'api':'material_name(previous)'}]
plan['authorization']='Planning only: preserve import_translated signature, profile guard, current callers and helper semantics. Shared-model/production changes and runtime work require separate authorization. R6 acceptance gate remains NOT_RUN.'
plan['capture_plan']=manifest['observation_plan'];block['fixture_package'].update(case_count=len(cases),total_key_bytes=sum(c['bytes'] for c in cases),setup_file_count=2,total_setup_bytes=sum(x['bytes'] for x in manifest['setup_files']))
block['fixture_package']['manifest']={'path':str(F/'manifest.json'),'bytes':(F/'manifest.json').stat().st_size,'sha256':hashf(F/'manifest.json')}
blockpath.write_text(json.dumps(block,indent=2)+'\n')
# Static artifact checks only; no replacement or original execution.
manifest['purpose']='Source-bound metadata fixture candidates:17 actions,2 setup files; runtime NOT_RUN.'
for c in cases:
 c['current_replacement']='NOT_RUN; at d546058 the four metadata handlers/storage are unimplemented. Header-only actions still require unsupported metadata setup; no passing test claim.'
 if c['file'].startswith(('12-','13-','16-','17-')):c['evidence_refs']=['parser','dimension_setter','root_init','controls_init','defaults','metadata_extract','string_helper','integer3_helper']
(F/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
block['fixture_package']['manifest']={'path':str(F/'manifest.json'),'bytes':(F/'manifest.json').stat().st_size,'sha256':hashf(F/'manifest.json')};blockpath.write_text(json.dumps(block,indent=2)+'\n')
by={c['file']:c for c in cases}
for c in cases:
 data=(F/c['file']).read_bytes().replace(b'\r\n',b'\n');assert c['trace']['expected_parser_cursor']==len(data)
 for e in c['trace']['events']:
  assert data[e['offset']:e['handler_entry_cursor']].decode()==e['token']
  assert e['line_read_attempts']==len(e['line_reads'])
  for lr in e['line_reads']:assert data[lr['translated_offset']:lr['translated_cursor']].hex()==lr['bytes_hex']
  if e['kind']=='metadata_handler':
   assert e['metadata_after']['linked']==e['metadata_before']['linked']
   target='cursim' if e['token']=='CURSIM' else field[e['token']][0]
   for k in D:
    if k!=target:assert e['metadata_after']['primary'][k]==e['metadata_before']['primary'][k]
for name in ['12-dimension-resets-metadata.KEY','17-new-import-header-opportunity.KEY']:
 q=by[name]['expected_after_future_metadata_implementation'];assert {k:q[k] for k in D}==D and q['linked_metadata']==D and q['primary_dimension']==2 and q['linked_dimension_if_present']==3
for name in ['13-first-header-wins.KEY','16-same-dimension-preserves.KEY']:
 q=by[name]['expected_after_future_metadata_implementation'];assert {k:q[k] for k in D}==P and q['linked_metadata']==L and q['primary_dimension']==3 and q['linked_dimension_if_present']==2
assert by['17-new-import-header-opportunity.KEY']['setup_imports'][-1]['file']=='13-first-header-wins.KEY'
ev=by['09-cursim-partial.KEY']['trace']['events'];assert [(e['scan_conversions'],e['metadata_after']['primary']['cursim']) for e in ev][-4:]==[(0,[0,0,0]),(3,[9,8,7]),(2,[7,5,0]),(3,[8,16,-3])]
for name,n,failed in [('04-title-first-read-eof.KEY',1,'discard_inline_remainder'),('05-title-second-read-eof.KEY',2,'string_payload')]:
 e=by[name]['trace']['events'][-1];assert e['line_read_attempts']==n and e['line_reads'][-1]['stage']==failed and e['line_reads'][-1]['status']==1 and e['metadata_after']['primary']['title']=='Prior'
ev=by['15-sticky-read-error.KEY']['trace']['events'];assert ev[1]['metadata_after']['primary']['cursim']==[0,0,0] and ev[1]['parser_status_after_event']==2 and ev[2]['metadata_before']['primary']['cursim']==[0,0,0] and ev[2]['parser_status_after_event']==2
for row,e in zip(capacityrows,by['07-capacity-boundaries.KEY']['trace']['events']):
 key,cap=field[row['keyword']];assert e['metadata_after']['primary'][key]==row['expected_value'];assert e['line_read_attempts']==2;assert len(e['line_reads'][1]['bytes_hex'])//2==row['payload_length']+1
checks=['Actual translated slices match event/read coordinates','All non-target metadata retained per handler','Both-root changed/same/later/new-import header expectations','CURSIM0/1/2/3 conversions and octal/hex vectors','12 string capacity vectors and adjacent fields','First-read EOF1 versus second-read EOF2 attempts','Sticky failure zeroing before later TITLE','All17 actions NOT_RUN/null runtime oracle']
changed=[];unchanged=[]
for row in before:
 p=pathlib.Path(row['path']);assert hashf(pathlib.Path(row['snapshot']))==row['sha256'];after=hashf(p)
 if after!=row['sha256']:changed.append({'path':str(p),'before_sha256':row['sha256'],'after_sha256':after,'before_bytes':row['bytes'],'after_bytes':p.stat().st_size})
 else:unchanged.append({'path':str(p),'sha256':after})
assert {pathlib.Path(x['path']).name for x in changed}=={'07-capacity-boundaries.KEY','09-cursim-partial.KEY','12-dimension-resets-metadata.KEY','13-first-header-wins.KEY','manifest.json','chat-b-roundtrip-blockers.json'}
added=[{'path':str(F/name),'sha256':hashf(F/name),'bytes':(F/name).stat().st_size} for name in ['setup-primary-metadata.KEY','setup-linked-metadata.KEY','16-same-dimension-preserves.KEY','17-new-import-header-opportunity.KEY']]
assert hashf(pathlib.Path(manifest['review_source']['path']))==manifest['review_source']['sha256']
revisions=[
 {'id':'R1','status':'CORRECTED_STATIC','detail':'Cases12/13/16/17 seed distinct nondefaults in all four fields on both roots via direct-root setup imports. Changed dimension resets both; same/later headers retain both; case17 runs after13 in a new import and applies its first header. No alternate-root routing.'},
 {'id':'R2','status':'CORRECTED_STATIC','detail':'Case09 adds junk0 conversions,7 5 bad2 conversions,010/0x10/-3=>8/16/-3, with nonzero reseeds. Existing EOF10 and whitespace failure15 retained. One remainder-line read and cursor asserted.'},
 {'id':'R3','status':'CORRECTED_STATIC','detail':'Case07 covers maximum-1/maximum/maximum+1 for all3 fields with distinct final bytes, plus boundary space/outside nonspace to prove truncate-before-trim. Full line consumption, next token cursor and non-target fields asserted; final CURSIM sentinel.'},
 {'id':'R4','status':'CORRECTED_STATIC','detail':'All17 cases now pin physical/translated hashes and offsets, token/entry/return/next cursors, read-stage bytes/status/counts and full state.04/05 distinguish failed stages;03/08 payload consumption traced;15 zeroing precedes TITLE. Header callbacks are not claimed; helper line_reads requires separate helper-level tests.'},
 {'id':'R5','status':'CORRECTED_PLAN_ONLY','detail':'Reuse existing integer_triple(), name(previous)81 and material_name(previous)65. No required generic API. Preserve current helpers/callers, loader signature and profile guard. No production changes.'},
 {'id':'R6','status':'OUTSTANDING_NOT_AUTHORIZED_NOT_RUN','detail':'Replacement replay/native comparison remain separately authorized runtime gates; no compatibility or finalization claim.'}]
r={'schema_version':1,'task':'fix-keyword-metadata-fixtures-1','status':'CORRECTED_PENDING_C_INDEPENDENT_RECHECK','base_commit':block['base_commit'],'review':manifest['review_source'],'before_receipt':{'path':str(B/'receipt.json'),'sha256':hashf(B/'receipt.json'),'preserved_files':len(before)},'changed_files':changed,'unchanged_prior_files':unchanged,'added_files':added,'generator':{'path':str(A/'chat-b-build-keyword-metadata-fixtures.py'),'sha256':hashf(A/'chat-b-build-keyword-metadata-fixtures.py')},'revisions':revisions,'validation':{'result':'PASS_STATIC_ARTIFACT_CHECKS_ONLY','checks':checks,'action_cases':17,'setup_files':2,'original_runs':0,'candidate_runs':0,'note':'Bounded source-derived annotation generation is not an independent native oracle.'},'limits':['No implementation/shared-model edit authorization inferred.','Malformed headers, embedded NUL, oversized fgets lines, out-of-range integers and native file-mode details remain excluded.','Header and linked-root setup require initialized simulation profiles; assert both-root prestate.','Earlier delivery-validation hashes describe the preserved original package; this correction supplies updated identities.'],'tooling_adoption':{'evidence_index':'DEFERRED','reason':'Known files sufficient; no index or benchmark needed.'},'resource_release':{'rea_session':'none','runtime_process':'none'},'next_action':'C independently rechecks the revised package; stop pending coordinator assignment.'}
out=A/'chat-b-keyword-metadata-correction.json';out.write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({'correction':str(out),'sha256':hashf(out),'manifest_sha256':hashf(F/'manifest.json'),'blockers_sha256':hashf(blockpath),'changed_files':len(changed),'added_KEY_files':len(added)}))
