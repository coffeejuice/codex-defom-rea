import argparse, collections, hashlib, json, pathlib, re, struct

TARGET='f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
def sha(b): return hashlib.sha256(b).hexdigest()
def readj(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def binding(p): return {'path':str(p.resolve()),'bytes':p.stat().st_size,'sha256':sha(p.read_bytes())}
def writej(p,v): p.write_text(json.dumps(v,indent=2,ensure_ascii=True)+'\n',encoding='utf-8')

ap=argparse.ArgumentParser(description='Read-only retained evidence audit; writes only Chat B derived analysis files. Does not execute the reference or use REA.')
ap.add_argument('--evidence-root',type=pathlib.Path,default=pathlib.Path('C:/Users/alext/Projects/codex-deform-rea/analysis'))
ap.add_argument('--output-dir',type=pathlib.Path,default=pathlib.Path(__file__).resolve().parent)
ap.add_argument('--skip-snapshot-search',action='store_true')
a=ap.parse_args();p=a.evidence_root;out=a.output_dir
checks=[]; sources={}
def source(rel):
 f=p/rel; sources[rel]=binding(f);return f
runs=[]
for dirname in ['rea-step25','export-step25-v2']:
 base=p/'oracle/interactive'/dirname;run=readj(source('oracle/interactive/'+dirname+'/run.json'))
 assert run['executable_sha256']==TARGET and run['status']=='completed' and run['exit_code']==0
 verified=[]
 for f in run['files']:
  path=source('oracle/interactive/'+dirname+'/'+f['name']);data=path.read_bytes()
  assert len(data)==f['bytes'] and sha(data)==f['sha256'];verified.append(f['name'])
 assert (base/'stdin.bin').read_bytes()==run['stdin'].encode('ascii')
 assert run['input_copy_sha256_before']==run['input_copy_sha256_after']==sha((base/'input.db').read_bytes())
 runs.append({'directory':str(base),'manifest_matches':verified,'executable_sha256_claim':TARGET,'exit_code':0,'input_unchanged_per_manifest':True})
checks.append('Both manifests agree with all ten retained output/input files; no original process was launched.')
raw=(p/'oracle/interactive/rea-step25/output.key').read_bytes();other=(p/'oracle/interactive/export-step25-v2/output.key').read_bytes();assert raw==other
assert (p/'oracle/interactive/rea-step25/stdout.bin').read_bytes()==(p/'oracle/interactive/export-step25-v2/stdout.bin').read_bytes()
lines=raw.decode('ascii').splitlines();rawlines=raw.splitlines(keepends=True);starts=[];cur=0
for line in rawlines: starts.append(cur);cur+=len(line)
cat=readj(source('keyword-catalog.json'));catalog={r['name']:r for r in cat['rows']}
sections=[];blocks=[];unclassified=[];section=None
for i,line in enumerate(lines):
 if line.startswith('*') and line.strip('* '):
  section={'title':line.strip('* '),'line_1based':i+1,'byte_offset_0based':starts[i],'keyword_order':[]};sections.append(section)
 token=line.split()[0] if line.split() else ''
 if re.match(r'^[A-Z]',line) and token in catalog:
  r=catalog[token];blocks.append({'ordinal_1based':len(blocks)+1,'keyword':token,'keyword_id':r['id'],'category':r['category'],'section':section['title'] if section else None,'header_line_1based':i+1,'byte_offset_0based':starts[i],'header_ascii':line,'header_tokens':line.split()[1:]})
  if section: section['keyword_order'].append(token)
 elif re.match(r'^[A-Z]',line): unclassified.append({'line_1based':i+1,'text':line})
# Spans end before any next keyword or asterisk comment line. This is a lexical artifact index, not a universal KEY parser.
for idx,b in enumerate(blocks):
 lo=b['header_line_1based']-1; hi=blocks[idx+1]['header_line_1based']-1 if idx+1<len(blocks) else len(lines)
 hi=next((i for i in range(lo+1,hi) if lines[i].startswith('*')),hi)
 end=starts[hi] if hi<len(lines) else len(raw)
 b.update({'last_line_1based':hi,'end_byte_exclusive':end,'span_sha256':sha(raw[starts[lo]:end]),'payload_line_count':hi-lo-1,'payload_first_ascii':lines[lo+1] if hi>lo+1 else None})
counts=collections.Counter(b['keyword'] for b in blocks);present=set(counts)
# Ensure every non-keyword alphabetic line is accounted for as one of the observed name/title payloads.
namekw={'TITLE','OPRNAM','SIMNAM','MTNAME','OBJNAM','PRSNAM'}
for u in unclassified: assert any(b['keyword'] in namekw and b['header_line_1based']+1==u['line_1based'] for b in blocks),u
floats=[m.group() for m in re.finditer(rb'(?<![A-Za-z0-9_])[+-]?\d\.\d+E[+-]\d+',raw)]
formats=collections.Counter((len(x.split(b'.')[1].split(b'E')[0]),len(x.split(b'E')[1])-1) for x in floats)
objref=readj(source('object-key-reference.json'));dbraw=source('oracle/cpp-step25.reals.bin').read_bytes();dbint=source('oracle/cpp-step25.integers.bin').read_bytes();source('oracle/cpp-step25.header.bin')
for kind in ['reals','integers','header']:
 f=objref['sources'][kind]; data=(p.parent/f['path']).read_bytes();assert sha(data)==f['sha256'] and len(data)==f['size']
objects=[];example=None
for o in objref['objects']:
 rz=o['rz_payload_check'];mapping=o['buffer_observations']['rz_complete_sequence'];rows=[l.split() for l in lines[rz['first_line']-1:rz['last_line']]]
 assert [int(r[0]) for r in rows]==list(range(1,o['node_count']+1)) and all(len(r)==4 for r in rows)
 tokens=[x for row in rows for x in row[1:]];vals=[float(x) for x in tokens];db=struct.unpack_from('<'+'d'*len(vals),dbraw,8*mapping['zero_based_real_index']);errs=[abs(x-y) for x,y in zip(vals,db)]
 eq=sum(struct.pack('<d',x)==struct.pack('<d',y) for x,y in zip(vals,db));tol=all(abs(x-y)<=max(1e-9,abs(y)*6e-11) for x,y in zip(vals,db));assert tol
 if example is None:
  for i,(x,y) in enumerate(zip(vals,db)):
   if x!=y:example={'object_id':o['id'],'component_0based':i,'text':tokens[i],'parsed_binary64_le_hex':struct.pack('<d',x).hex(),'retained_db_binary64_le_hex':struct.pack('<d',y).hex(),'retained_db_value':y};break
 ec=o['elmcon_payload_check'];em=o['buffer_observations']['elmcon_complete_sequence'];erows=[l.split() for l in lines[ec['first_line']-1:ec['last_line']]]
 assert [int(r[0]) for r in erows]==list(range(1,o['element_count']+1)) and all(len(r)==5 for r in erows)
 valsi=[int(x) for row in erows for x in row[1:]];dbi=struct.unpack_from('<'+'i'*len(valsi),dbint,4*em['zero_based_integer_index']);assert all(x+em['node_index_shift']==y for x,y in zip(valsi,dbi))
 objects.append({'id':o['id'],'name':o['name'],'KEY_type_code':o['type_code'],'node_count':o['node_count'],'element_count':o['element_count'],'arity':o['nodes_per_element'],'coordinate_components':len(vals),'coordinates_binary64_equal':eq,'coordinate_max_abs_error':max(errs),'reference_tolerance_pass':tol,'connectivity_integer_count':len(valsi),'connectivity_global_minus_shift_exact':True,'node_index_shift':em['node_index_shift'],'mapping_basis':'object-key-reference.json fixture-only sequences; not a general DB writer offset contract'})
contacts=[b for b in blocks if b['keyword']=='CNTACT'];pairs=[tuple(map(int,b['header_tokens'][:2])) for b in contacts];assert pairs==[(i,j) for i in range(1,8) for j in range(i,8)]
nonzero=[b for b in contacts if int(b['header_tokens'][2])!=0]
assert all(counts[n]==len(nonzero) for n in ['FRCFAC','IHTCOF','SEPRES','INTRST','CNTMTH'])
bykeyword=[{'keyword':n,'keyword_id':catalog[n]['id'],'category':catalog[n]['category'],'occurrences':counts[n],'first_line_1based':next(b['header_line_1based'] for b in blocks if b['keyword']==n)} for n in counts]
index={'schema_version':1,'status':'observed_artifact_index_not_universal_writer_grammar','source':sources['oracle/interactive/rea-step25/output.key'],'same_bytes_second_capture':sources['oracle/interactive/export-step25-v2/output.key'],'index_method':'Exact catalog token in column 1; all other alphabetic first-column lines checked as next-line name/title payload. Span ends before next keyword/comment. Full byte offsets and span hashes permit independent verification.','sections':sections,'keyword_summary':bykeyword,'catalog_keywords_absent':sorted(set(catalog)-present),'non_keyword_alphabetic_payload_lines':unclassified,'blocks':blocks}
writej(out/'chat-b-keyword-export-observed-index.json',index)
audit={'schema_version':1,'status':'retained_evidence_checks_passed','target_sha256':TARGET,'runs':runs,'repeat_export_exact_bytes':True,'repeat_stdout_exact_bytes':True,'artifact':{'bytes':len(raw),'lines':len(lines),'crlf_count':raw.count(b'\r\n'),'lf_count':raw.count(b'\n'),'ends_crlf':raw.endswith(b'\r\n'),'high_bytes':sum(x>127 for x in raw),'keyword_occurrences':len(blocks),'distinct_keywords':len(present),'section_count':len(sections),'scientific_numeric_tokens':len(floats),'fraction_exponent_digit_histogram':[{'fraction_digits':k[0],'exponent_digits':k[1],'count':v} for k,v in formats.items()]},'objects':objects,'coordinate_precision':{'components':sum(x['coordinate_components'] for x in objects),'binary64_equal':sum(x['coordinates_binary64_equal'] for x in objects),'binary64_different':sum(x['coordinate_components']-x['coordinates_binary64_equal'] for x in objects),'example':example,'interpretation':'Loss demonstrated relative to retained decoded DB coordinates, corroborating the historical fixture comparison; this is not a new observation of writer input registers or a KEY reimport experiment.'},'contacts':{'pairs':pairs,'nonzero_pair_count':len(nonzero),'nonzero_pairs':[list(map(int,b['header_tokens'])) for b in nonzero]},'sources':sources,'checks':checks}
if not a.skip_snapshot_search:
 scans=[];hits=[]
 for f in sorted((p/'oracle').glob('*snapshot.json')):
  j=readj(f); entries=j.get('entries',[]);requests=[e for e in entries if e.get('parameters',{}).get('procedure')=='0x14000ac60'];records=j.get('evidence_bundle',{}).get('records',[]);rqs=[r for r in records if r.get('parameters',{}).get('procedure')=='0x14000ac60'];rec={'path':str(f),'bytes':f.stat().st_size,'sha256':sha(f.read_bytes()),'entries':len(entries),'records':len(records),'exact_seed_entry_count':len(requests),'exact_seed_record_count':len(rqs)};scans.append(rec)
  if requests or rqs:hits.append({'path':str(f),'operations':[e.get('operation') for e in requests+rqs]})
  del j,entries,records
 audit['retained_snapshot_search']={'scope':'All analysis/oracle/*snapshot.json files existing at this run; exact procedure-parameter search only. Metadata/call references do not constitute the seed body. No live provider queries.','files':scans,'exact_seed_hits':hits}
writej(out/'chat-b-keyword-export-evidence-audit.json',audit)
print(json.dumps({'artifact':audit['artifact'],'precision':audit['coordinate_precision'],'seed_hits':audit.get('retained_snapshot_search',{}).get('exact_seed_hits'),'outputs':['chat-b-keyword-export-observed-index.json','chat-b-keyword-export-evidence-audit.json']},indent=2))
