# CP256 B121/group-6 next-leaf decision

No independently implementable production leaf is established for B121's TLOC group-6 branch or the coupled Add Study/Save transition. Keep the current refusals. The retained final files qualify output bytes, but do not identify the live variable edit, intermediate owners, or which action produced each write.

## Group-6 boundary

`source/apps/moproj2mst/src/doe/doe_study.cpp::keywordValues` (line 864) rejects keyword groups outside 1–5 before recording any `temp.KWD` write. `source/apps/moproj2mst/tests/mdt/test_doe_study.cpp::doe_temp_kwd_tloc_refuses` already tests id 8 and no writes. A bounded source search finds no production caller of `keywordValues`; its only call is this test. Wiring it into activation or Save would therefore require an additional, unproved owner/caller transition.

The retained original `evidence/gui_mo/s1/mdt/DOE/d9.c::FUN_140916530` writes the first supplied text, allocates DB_STEP_DATA, opens the file and dispatches only groups 1–5. The group-6 edge then compares `iStack_220` without an assignment on that edge. This is decompiler evidence of an unresolved status read, not a deterministic successful TLOC parse. The retained offline disassembly `analysis/resume163-frcrot-file-140916530.asm` provides the instruction-level counterpart; it is static evidence, not an execution receipt. Do not invent a group-6 parser, initialize that status to success, or generalize group-1–5 success behavior.

`evidence/gui_mo/s1/mdt/DOE/d1.c` supplies the upstream call shape: collect keyword/object/operation/text inputs, initialize three DYNARY pointers to null, invoke `0x140916530`, then update variable values only when upper/lower pointers are nonnull. Reproducing only the first write is insufficient: the actual caller, arguments, status edge, subsequent writes, handler result and caller mutations still need binding to B121. A file-only helper would have no currently supported caller and would not resolve variable serialization.

## Historical reconciliation and action boundary

CP218's B121 retained final state has eight missing paths (the seven Control files plus root `temp.KWD`, 13 bytes), plus a changed project. B121's PAR/input-table/MST.TEMP differ from B120. These are action-created studies, not ordinary converter Open/Save inputs. The historical workflow note `source/docs/re/moproj2mst/mdt/L6/doe.md` explicitly labels TLOC/op1/three samples; its timestamp/action narrative does not provide per-call attribution or the missing status value. Its old “to do” list is not an authoritative inventory of current implementation.

CP240 statically qualifies `triggered()` → `slotActionProjectAddDoe()` → guarded `0x14002AB20` → `0x1400A6A00`, including DOM/SUB append, current-index assignment, GUI shared-model ownership, activation and `0x1400A63C0(...,"SfOprDoeOutput",1)`. CP249 correctly keeps these coupled. Current `src/core/convert.cpp` selects existing index 0; `src/model/subproject.cpp::makeSubProject` owns an existing Type2 record. Neither supplies this GUI action. The current bounded TLOC reader/model helpers in `src/doe/keyword_reader_prefix.cpp`, `variable_prefix.cpp` and `doe_study.cpp` do not close nonempty notifier/backend ownership, group-6 extraction, general variable writing, or study creation. Existing candidate-only model-prefix tests cannot promote their projections to original caller equivalence.

## Exact missing witness and eventual test owner

One staged original occurrence must preserve process/project identity and ordered effects:

1. Start with a disposable retained before project; record DOM/SUB/current selection, DOE variable/sample owners and file manifest. Observe Add Study through the success predicate and normal/failing return of `0x1400A6A00` and initial output-operation creation, before Save.
2. In that same study, add/edit TLOC for operation 1 with three samples. Bind the actual variable-edit caller in `d1.c` to `0x140916530` with id/group, dimension, object/operation list and all three supplied strings. Capture first-file bytes/open result, the exact status storage/value and edge after group lookup, each reached later overwrite, all three output pointers, handler/copy results and the caller's resulting variable/sample state. Preserve failures and partial effects. A final 13-byte `temp.KWD` alone cannot distinguish first-write failure from later successful overwrites.
3. Save explicitly in the same process/project; retain ordered writes and exact project/input-table/PAR/MST.TEMP/INI bytes. This separates variable-edit and sample-generation effects from Save effects.

Only after that witness can a test be specified with justified expected behavior. The prospective extraction owner is `source/apps/moproj2mst/src/doe/doe_study.cpp`, declaration `include/m2m/doe_study.hpp`; the actual caller owner must be established from the trace rather than selected in advance. Extend `tests/mdt/test_doe_study.cpp` for the observed extraction branch and add a caller-level Testkit case through the observed production variable-edit/Save owner, asserting ordered partial/full file effects, pointer/result state and persisted variable/sample state. A direct helper test alone is insufficient. Add Study would separately need its explicit public action contract and production owner before a caller test can be written; injecting its state into conversion is unsupported.

No GUI or mutable REA access was used. No source, test, build, shared recovery state, original process, or evidence artifact was changed; this report is the sole output. This decision does not claim closure of DOE lifecycle or historical DIFFs.
