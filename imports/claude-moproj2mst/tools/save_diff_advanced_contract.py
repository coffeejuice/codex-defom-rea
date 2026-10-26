"""Retain bounded advanced-page recovery facts and the next guarded increment."""
import hashlib
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
files=[
'analysis/wave2-save-diffs-300-fill-140ec5bc0.c',
'analysis/wave2-save-diffs-300-apply-140ec7500.c',
'analysis/wave2-save-diffs-300-advanced-fill-complete.asm',
'analysis/wave2-save-diffs-300-advanced-time-callback-complete.asm',
'analysis/wave2-save-diffs-300-advanced-time-getter.asm',
'analysis/wave2-save-diffs-300-advanced-motion-predicate.asm',
'analysis/wave2-save-diffs-300-advanced-twin-comparison.json',
'analysis/wave2-save-diffs-300-advanced-values.json',
'analysis/wave2-save-diffs-300-support-140eb26f0.c',
'analysis/wave2-save-diffs-300-setupui.c',
'analysis/wave2-doe-sim300-advanced-time-apply-decompile.json',
'analysis/wave2-save-diffs-300-support-140ed97c0.c',
'analysis/wave2-save-diffs-300-support-140ecfab0.c',
'analysis/wave2-save-diffs-300-support-140ecfb90.c',
'analysis/wave2-save-diffs-300-support-140ecfc30.c',
'analysis/wave2-save-diffs-300-support-140ecfcd0.c',
'analysis/wave2-save-diffs-300-support-140ecfd70.c',
'analysis/wave2-save-diffs-300-support-140ecfe10.c',
'analysis/wave2-save-diffs-300-support-140eca080.c',
'analysis/wave2-save-diffs-300-support-140ed5dd0.c',
'source/port/defmodel/src/material/state_remap.cpp']
d={
'status':'read-only recovery; production and tests remain frozen for root combined build',
'original_exe_sha256':'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc',
'qualification':'Static recovery of a bounded initial-editor branch. No original application executed; no public lifecycle acceptance claim. Native instruction-shape matches preserve paired external target uncertainty and are not whole-call-graph equivalence proof.',
'capture':{'functions':['140874630','14082a360','1408bfe60'],'elapsed_seconds':16.905,'tokens':'unavailable','read_only_private_Ghidra':True,'lock_released':True},
'fill':{
'numeric_text':'TNOW1/2 and GEOERR1..4*100 use original QString::sprintf %.6g; supported KnownC path uses recovered qt::number g6 seam',
'radios':'TNOW3 accepts0/1; ELMNOD first3 accept0/2/3; invalid source radios excluded from first branch',
'user_strings':'Raw140ec60dd loads SC+688+i*51 into R8 for original sprintf %s at140ec60f1. Decompiler omitted this variadic argument. Ten bounded printable-ASCII strings can be snapshotted exactly.',
'output_flags':'Constructor140eb26f0 maps slot0x14 to setupUi+1298; setupUi140f10e23 setsChecked(1), disabled. Fill skips i==4. Other six flags use model!=0; all seven are written from checkbox booleans at Save regardless of enabled state.',
'secondary_die':'Leading none; iterate model object positions, require object and kind!=7 and (mesh number+87c!=0 or kind5). Remember matching stored PDIE2 number or0 when missing.',
'frequency':'Fill ensures rows>=6/cols>=1/ncomp>=6 and may allocate, then sets six combo/text controls from first column after CVTTSD2SI conversion. Combo callbacks directly write0, or first-row-1, but positive-number combos only enable fields. First bounded profile can require existing6x1/ncomp6/value allocation and exact int32-valued entries (no negative zero); no allocation branch needed. Initial positive integers/zero, plus optional first-row-1 preserve table bytes. Save advanced does not directly reapply remembered frequency text. Edited controls and other frequency shapes remain excluded.'},
'apply_order':[
'Validate TNOW1; compare absolute difference against1e-6; assign. Invalid only stops this page.',
'Validate TNOW2; compare against1e-6; assign; assign TNOW3 radio.',
'Always1408bfe60(step), then call again if either time changed. It selects TNOW1 if TNOW3==0 else TNOW2; scans valid objects; only movementkind6 with nonempty curve enters140874630. First branch excludes valid movementkind6 objects, so both calls are proven no-ops.',
'Validate/store GEOERR1..4 sequentially as parsed/100. Earlier time writes and callbacks survive later Invalid.',
'Copy ten remembered user strings to SC, then seven checkbox values (index4 remains1), ELMNOD first3, remembered PDIE2.',
'Call14082a360(step,0), corresponding to existing native step_remap_material_state(step,false), after all preceding page stores. It rebuilds state-variable widths and remaps each valid object array; this is not a UI-only callback.'
],
'proposed_fields':['array<ValidatedText,2> time','int timeRadio','array<ValidatedText,4> geometryErrorPercent','array<qt::String,10> userStrings','array<bool,7> outputFlags','array<int,3> elementNodeOptions','int secondaryDieNumber','array<int,6> frequencyComboIndices plus remembered initial text when represented'],
'proposed_guards':['dimension3 and explicit KnownC locale','finite TNOW values and finite GEOERR*100 products','valid source radios, bounded printable ASCII user strings','first bounded FRQNCY profile described above','no valid movementkind6 objects; recheck before Apply callbacks if source model changed','positive unique eligible object numbers for unambiguous displayed PDIE2 labels'],
'next_tests':['Fill then mutate source model before Apply: remembered times, GEOERR, user strings, flags and PDIE2 must win; frequency source changes must remain','Invalid TNOW2 retains TNOW1 only; Invalid GEOERR2 retains times and GEOERR1 but leaves later strings/flags/remap untouched','STNOUT index4 source0 becomes1; other nonzero source flags become1','Nontrivial valid object arrays exercise final native state-layout remap after ELMNOD changes','Guard excludes movementkind6 without inventing an approximation'],
'composition_remaining':['extra3D session reader/writer140ec8110/140ec9040 and constructors/default session state','eca080 tail includes ed5dd0: rewrites recovery lists then re-applies advanced page; guard/source ordering must be composed','original feature/license binding for pressure population and general process enable states','project Open/remesh file binding and persistent per-call snapshot ownership','remaining Save tail lifecycle effects and original-first fresh-run acceptance'],
'evidence':[{'path':p,'sha256':hashlib.sha256((ROOT/p).read_bytes()).hexdigest()} for p in files]
}
out=ROOT/'analysis/wave2-save-diffs-300-advanced-contract.json'
out.write_text(json.dumps(d,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'output':str(out),'sha256':hashlib.sha256(out.read_bytes()).hexdigest(),'evidence_files':len(files)}))
