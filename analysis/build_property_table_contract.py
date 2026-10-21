"""Build the source-bound selected property-table research contract."""
import datetime
import hashlib
import json
from pathlib import Path
import re

def bind(p):
    p=Path(p);d={'path':p.as_posix(),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()}
    if p.suffix=='.json':
        e=json.loads(p.read_text())
        if 'evidence_id'in e:d['evidence_id']=e['evidence_id']
    return d

audit=json.loads(Path('analysis/property-table-byte-audit.json').read_text())
cases=[]
for c in audit['cases']:
    id=c['id'];q={k:v for k,v in c.items()if k!='instructions'}
    q['initial_type']=93 if id==131 else 182 if id==133 else 0
    q['auxiliary_flag']=int(id in(97,99))
    q['auxiliary_destination']='material+0x7d8 double'if id==97 else 'zero-initialized local double; truncate/store material+0x8d0 even failure'if id==99 else None
    q['postprocess']='if rank==2 force selector1(table+0x14)=20 even failure'if id==121 else '413de0 first writes material+0x20d0; only positive value invokes table reader; second status replaces first'if id==126 else 'none'
    cases.append(q)
s=json.loads(Path('analysis/configuration-0x14012d2a0.json').read_text())['normalized_result']['pseudocode'].replace('\r','')
s=s.split('switch(param_2) {',1)[1].split('\n  }\n',1)[0]
labels=[];profiles={}
for m in re.finditer(r'case (0x[0-9a-f]+|\d+):(.*?)(?=\n  case |\Z)',s,re.S):
    labels.append(int(m[1],0))
    writes=[{'offset':hex(int(a,0)*4),'int32_value':int(b,0)}for a,b in re.findall(r'param_1\[(0x[0-9a-f]+|\d+)\] = (0x[0-9a-f]+|\d+);',m[2])]
    if writes:
        for label in labels:profiles[str(label)]=writes
        labels=[]
assert not labels and len(profiles)==124
c={
 'schema_version':1,'status':'selected_13_material_family_static_chain_closed_runtime_pending',
 'created_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),
 'target':bind('source/DEF_PRE_64.exe'),
 'scope':'Shared41b270 material header family; full41a800 rank0..3 stream behavior and table lifecycle. Scientific meanings/consumers are not inferred.',
 'material_dispatch':{
  'function':'0x140384a50','cases':cases,
  'selection':'Same category4 signed32 exact material ID selector as MTNAME. EOF returns2; conversion0 leaves native local uninitialized and is outside selected domain. Missing material append/count/refresh(option1) precedes payload; existing material does not refresh.',
  'status':'Common epilogue returns0 if localstatus0 else2. No dimension gate or post-property layout refresh.',
  'construction':'28c6f0 calls12d050 for all13 destinations. 28cdc0 overrides only ID131 table+17f0 to type93 and ID133+1878 to type182 among these13. Do not claim later28cdc0-only calls reset the other11 tables.',
  'auxiliary_defaults':{'0x7d8':'double+0','0x8d0':'int32zero','0x20d0':'int32zero'}},
 'header_41b270':{
  'abi':'RCX FILE*, RDX table*, R8D optional-output gate, R9 double* optional output.',
  'order':['Zero local selectorint32 and3 local doubles.','fgets1024; null returns1 without table/output mutation.','Comma-to-space; sscanf %i %Flf %Flf %Flf. Only conversions==-1 returns1 preserving old table.','All other conversioncounts including0 call12fc70(table,0) before applying selector fields.','Apply optional-output rules; call41a800; return its status.'],
  'selectors':{
   '0':{'rank':0,'constant':'value1'},
   '1':{'rank':1,'selectors':[3],'counts':['trunc(value1)']},
   '2':{'rank':1,'selectors':[9],'counts':['trunc(value1)']},
   '3':{'rank':1,'selectors':[12],'counts':['trunc(value1)']},
   '4':{'rank':2,'selectors':[3,12],'counts':['trunc(value1)','trunc(value2)']},
   '5':{'rank':2,'selectors':[3,8],'counts':['trunc(value1)','trunc(value2)']},
   '6':{'rank':1,'selectors':[8],'counts':['trunc(value1)']},
   '7':{'rank':2,'selectors':[3,4],'counts':['trunc(value1)','trunc(value2)']},
   'other':'No additional writes: reset type0/rank0/width1/constant0; consumes no payload.'},
  'auxiliary':'Only gate>=0 and nonnulloutput: selectors0..3 with conversions>=3 write value2; selector4 with conversions>=4 writes value3. All other selectors preserve output. EXPAND passes persistent double, HEATCP passes local initialized0 then truncates/stores it even headerfailure.',
  'conversion':'Counts use CVTTSD2SI. Checked finite representable truncation is the selected numeric domain; nonfinite/out-of-range native indefinite conversion is not runtime validated here.',
  'reachability':'This13-family resets type0/width1 and reaches ranks0..2 only. Shared rank3/vector behavior is separately recovered, not tested by this family.'},
 'table_layout':{
  'stride':'0x78 in embedded material arrays; not proof everybyte initialized.',
  'fields':{'0x00':'int32 type','0x04':'int32 rank','0x08':'double constant','0x10':'3xint32 selectors','0x1c':'3xint32 counts','0x28':'3xpointers<double> axes','0x40':'int32 values_per_point','0x48':'dependent double pointer','0x50':'int32 flag','0x54':'int32 opaque metadata','0x58':'int32 mode','0x60':'8opaque bytes','0x68':'optional owned cache/object pointer'},
  'unwritten':['0x44..0x47','0x5c..0x5f','0x70..0x77'],
  'constructor_12d050':'Zero58/selectors/counts/all4arraypointers/68, then12d2a0(type0). Fresh result type0/rank0/constant+0/width1/flags50,54,58zero/60eightzero/arraysnull/68null.'},
 'lifecycle':{
  'reset_12d2a0':'Call12f480; zero60eightbytes; type=argument; zero08eight,54,rank,50,58,3selectors/counts; width1. Apply type-specific ordered int32 writes below. Call12fad0 except types101,102,103,81. Does not write68 or padding.',
  'type_specific_int32_writes':profiles,
  'unlisted_type':'No switch writes; use base reset values and allocation.',
  'preserving_reset_12fc70':'Save54int32 and60eightbytes;12d2a0(type);restoreboth. Mode58 is reset,68 unchanged.',
  'clear_12f480':'Any axiscount==0 with corresponding nonnullpointer suppresses ALL frees. Otherwise free positive-count axis allocations and nonnull dependent allocation. Finally null all4arraypointers; counts/metadata retained. Negative-count axis pointer is not freed. Borrowed/alias domain remains outside owning reconstruction.',
  'allocate_12fad0':'Clear12f480, clamp all3 negativecounts0 and width<1 to1 via12fc00. For indices0..rank-1 calloc(count,8) ifcount>0; memsetzero again ifnonnull. If12ff20>0 calloc dependent(total,8), zeroagain ifnonnull. No OOM result or downstream null-write protection.',
  'destroy_12f410':'Clear12f480; if68nonnull call13c170(pointer,1). Selected constructors/imports leave68null; arbitrary cache destructor not recovered.',
  'dependent_count_12ff20':'Rank0 returnswidth if width>=2 OR type in181,167,168,170,171; else0. Nonzero rank multiplies width by count[i] using sequential32-bit IMUL then clamps finalnegative result0. Reject overflowing products in bounded reconstruction rather than silently use an unbounded mathematical product.',
  'distinct_read_predicate':'Rank0width1 specialtypes can allocate one dependent item, yet41a800 consumes nothing because its outer gate is rank!=0 OR width>1.'},
 'payload_41a800':{
  'abi':'RCX FILE*,RDX table*. The zero third argument from41b270 is unused by proven body.',
  'entry':'rank0 andwidth<=1 return0 beforeallocation/read/discard. Otherwise12fad0 reallocates and zeroes storage everycall.',
  'rank0':'Read width values directly to dependent through fscanf%Flf; only-1 aborts1, retaining assignedprefix and zero remainder.',
  'rank1':'For each point read axis0[point], then width dependent values atpoint*width+column. Allwritesdirect; conversion0 preserves destination, loopcontinues. EOF aborts1 with partialwrites retained.',
  'rank2_or3':'Read each axis inaxisindexorder/itemsascending directly into zeroed arrays. Then malloc-backed new scratch(total*8), uninitialized. Read dependent values into scratch; onlyafter every scan completes memcpywholebuffer todependent, thenfree scratch. AxisEOF leaves partialaxes/dependentzeros. ScratchEOF leaves completeaxes/dependentzeros and leaks scratch. Conversion0 can leave uninitialized bytes latercopied; exclude that domain or represent validity explicitly, never fabricatezero.',
  'zero_product':'Positive-length axes still read even when another zero axis makes total0. Scratchallocation0/memcpy0/free occur, no finaldiscard.',
  'ordering':'No sort/transpose/interpolation; dependent storage follows scalar scanner order.',
  'final_line':'After completedreadpath iff12ff20>0 doone fgets1024. Its return is ignored; terminalEOF does not turn success into failure.',
  'other_rank':'Nonzero rank outside1..3 skips specialized scans afterallocation; rank>3 can access outside3axis metadata and is excluded.'},
 'relation_table_defaults':{
  'source':'333f60 constructs12 through12d050 thenchanges8 types. No new relation scientific dispatch is claimed.',
  'offset_types':{'0x78':44,'0xf8':55,'0x180':179,'0x1f8':48,'0x2c8':50,'0x3f8':54,'0x478':0,'0x4f0':0,'0x568':0,'0x5e8':0,'0x678':109,'0x6f0':110},
  'rank3':'44/55 rank3,width2,selectors12/6/3. 48 rank3,width2,selectors12/15/16. Allcounts0 andarraysnull initially.'},
 'byte_audit':bind('analysis/property-table-byte-audit.json'),
 'limitations':['Static only; no original execution or equivalence claim.','No OOM, overflow, nonfinite/out-of-range conversions, unsafe rank>3, borrowed allocations or arbitrary cache68 ownership claim.','Native temporary scratch leaks on EOF; resource behavior remains distinct from owned C++ cleanup.','Metadata54/60 and scientific selector/value meanings remain unnamed.']}
paths=['analysis/property-table-0x14041b270.json','analysis/property-table-41b270-instructions.json','analysis/property-table-byte-audit.json','analysis/audit_property_table_bytes.py','analysis/property-table-material-dispatch-dumpbin.txt','analysis/property-table-readers-dumpbin.txt','analysis/time-table-0x14041a800.json','analysis/relation-import-0x14012d050.json','analysis/configuration-0x14012d2a0.json','analysis/time-table-0x14012fc70.json','analysis/configuration-0x14012f480.json','analysis/configuration-0x14012fad0.json','analysis/configuration-0x14012fc00.json','analysis/configuration-0x14012ff20.json','analysis/property-table-0x14012f410.json','analysis/property-table-0x14047780c.json','analysis/property-table-0x1404797d8.json','analysis/material-import-0x14028c6f0.json','analysis/material-import-0x14028cdc0.json','analysis/material-import-0x14028cb70.json','analysis/material-import-0x14028d030.json','analysis/material-import-0x140384a50.json','analysis/volume-width-0x140333f60.json','analysis/volume-width-0x140334490.json','analysis/keyword-catalog.json',__file__]
paths += [str(p)for p in Path('analysis').glob('property-table-bytes-*.json')]
c['source_bindings']=[bind(p)for p in paths]
out=Path('analysis/property-table-static-contract.json');out.write_text(json.dumps(c,indent=2)+'\n');print(bind(out))
