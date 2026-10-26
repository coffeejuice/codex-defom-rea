"""Immutable PE ranges for CP163 actual FRCROT owner/file integration audit."""
import hashlib
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT/'tools'))
from doe_static_pe import PE
p = PE()
def span(a,b):
    raw = p.data[p.off(a):p.off(b-1)+1]
    return dict(start=hex(a), end_exclusive=hex(b), file_start=hex(p.off(a)),
                bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())
ranges = {
    'reader_function_discriminator':(0x14090f7db,0x14090f835),
    'reader_function_entry_number':(0x14090fa83,0x14090fb56),
    'reader_entity_id_append':(0x140910487,0x1409104e5),
    'reader_actual_owner_arguments_refetch_flags':(0x140910900,0x140910a67),
    'reader_three_text_stores':(0x140910a67,0x140910b6f),
    'reader_function_entity_selector':(0x140910b6f,0x140910c58),
    'reader_function_call_and_commit_gate':(0x14091100a,0x1409111c4),
    'file_parser_complete_unwind_region':(0x140916530,0x140917055),
    'file_first_write_parse_gate':(0x140916570,0x1409167b0),
    'save_reconciliation_complete':(0x1400d2b60,0x1400d31f3),
    'save_reconciliation_call':(0x1400d30ba,0x1400d30d7),
    'flags_constructor_leaf_through_RET':(0x140004a60,0x140004a7a),
    'flags_or_wrapper':(0x140031680,0x1400316bc),
    'flags_or_implementation':(0x1400318d0,0x140031904),
    'scan_wrapper':(0x141472ac8,0x141472afd),
    'scan_file_owner_validation_callback':(0x14147295c,0x141472ac6),
    'scan_callback_body':(0x141480128,0x1414812a8),
    'nominal_setter':(0x140921980,0x140921ad3),
    'upper_setter':(0x140921ae0,0x140921c33),
    'lower_setter':(0x140921c40,0x140921d93),
}
literals=[]
for a in [0x1427bba28,0x1427bba38,0x1427bba44,0x1427bba50,
          0x1427bba5c,0x1427bba68,0x1427bba74,0x1427bb924,0x1427bb98c]:
    o=p.off(a); n=p.data.index(0,o)-o
    literals.append({**span(a,a+n+1),'ascii':p.data[o:o+n].decode('ascii')})
def ascii_rva(rva):
    o=p.off(p.base+rva)
    return p.data[o:p.data.index(0,o)].decode('ascii')
imports={}
at=p.off(p.base+p.u32(p.optional+120))
while True:
    original,_,_,name,first=struct.unpack_from('<IIIII',p.data,at)
    if not any((original,name,first)):break
    i=0
    while True:
        thunk=p.u64(p.off(p.base+(original or first))+i*8)
        if not thunk:break
        imports[p.base+first+i*8]=dict(dll=ascii_rva(name),name='ordinal' if thunk>>63 else ascii_rva(thunk+2))
        i+=1
    at+=20
selected_iat=[0x141e04480,0x141e03fd8,0x141e03fe0,0x141e03dd8,0x141e03df0,
              0x141e04018,0x141e03df8,0x141e04020,0x141e04ee0,0x141e04f40]
artifacts=[]
for path in sorted((ROOT/'analysis').glob('resume163-frcrot-*.asm')):
    artifacts.append(dict(path=str(path.relative_to(ROOT)),bytes=path.stat().st_size,
                          sha256=hashlib.sha256(path.read_bytes()).hexdigest()))
for name in ['resume50-doe-variable-body-pe-compact.asm','resume50-doe-variable-body-pe.asm',
             'resume50-doe-variable-body-pe-receipt.json','resume146-doe-key-construction.md']:
    path=ROOT/'analysis'/name
    artifacts.append(dict(path=str(path.relative_to(ROOT)),bytes=path.stat().st_size,
                          sha256=hashlib.sha256(path.read_bytes()).hexdigest()))
sources={}
for name in ['keyword_reader_prefix.cpp','keyword_constructor_prefix.cpp','keyword_constructor_record.hpp',
             'frcrot_table_prefix.hpp','frcrot_table_prefix.cpp','doe_study.cpp','activation.cpp']:
    path=ROOT/'source/apps/moproj2mst/src/doe'/name
    sources[str(path.relative_to(ROOT))]=hashlib.sha256(path.read_bytes()).hexdigest()
out=dict(binary='evidence/original-binaries/DEF_GUI_MO_64.exe',binary_sha256=hashlib.sha256(p.data).hexdigest(),
         ranges={k:span(*v) for k,v in ranges.items()},literals=literals,
         iat={hex(a):imports.get(a) for a in selected_iat},artifacts=artifacts,sources=sources,
         reference_candidates=p.references([0x140916530,0x1400d2b60]),
         qualification='Immutable file ranges plus existing retained reader instructions and bounded dumpbin output. No original/runtime execution, GUI, live REA or mutable database. Direct-reference byte candidates were checked against selected function instructions; scan does not exclude indirect calls. Initial flags capture stopped because 0x140004a60 has no unwind record; explicit leaf capture is interpreted only through its RET at0x140004a79, excluding subsequent partial function. Scalar/function branch and file gates are conditional static behavior, not a witnessed complete FRCROT instance or scanner outcome.')
path=ROOT/'analysis/resume163-frcrot-integration-evidence.json'
path.write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps(dict(receipt=str(path.relative_to(ROOT)),ranges=len(ranges),artifacts=len(artifacts))))
