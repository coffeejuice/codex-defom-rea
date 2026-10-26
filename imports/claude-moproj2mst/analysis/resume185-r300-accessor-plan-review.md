# CP185 R300 native accessor plan independent review

**PASS for the bounded implementation plan; no blocking defect found.** This approves restoring repeated model/step resolution on the supported stable-owner path. It does not approve a public null-model fault-equivalence claim or a claim of reentrant lifetime safety.

Reviewed `analysis/resume185-r300-native-accessor-plan.md` against retained evidence and current source. Used the reverse-engineer-anything skill for evidence qualification. Only this review file was written; no source/test changes, builds, GUI sessions, original executions, or REA database operations occurred. Root recovery status and GUI queue were read; their unsupported-release gate remains unchanged and is unrelated to these static reads.

## Native sequence and counts

Independently rehashed original `evidence/original-binaries/DEF_GUI_MO_64.exe` (`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`), assembly `analysis/wave3-native-key-140058cc0.asm` (`e66f7860e75c9377f5de55821c2f090b591da06be38f12bbe8b6091afb80d896`), and current `source/apps/moproj2mst/src/engine/save_current.cpp` (`36b3b12693fd5c2b3d4f4bde9ec486a9b673f7575da4b6d94903de19ceb0cf8b`). These match the plan. Read Packet S qualification at `source/docs/re/moproj2mst/s/spec.md:3–10`, L0 qualification at `evidence/gui_mo/s1/mdt/L0/spec_A.md:3–10`, and the wrapper extraction qualification: runtime unwind region, not proof of a whole logical procedure. The assembly contains the relevant entry, loop, and normal return; it is static evidence rather than an executed trace.

The assembly establishes manager resolution once (`0x140058ce6`), initial operation-model/count resolution (`0x140058cf8/0x140058d00`), position-time operation-step resolution (`0x140058d34`), null/type-2 filtering, number copy, default path/local-byte conversion, writer-time model/step resolution (`0x140058dc6/0x140058dce`), and ignored writer return. `L0/A/b0.c` and `s1/mdt/cur_d4.c` establish that the position-time operation-step accessor calls the operation-model accessor. Thus `1 + visited positions + eligible positions` operation-model calls is correct for a normal traversal. Do not confuse those calls with model-holder reads inside the count accessor.

`out_s/s1c.c:780–792` explicitly returns zero when the first count-time step getter returns null, otherwise calls it again before reading count. The plan correctly retains the detached-step test and distinguishes that branch from a later null step after a positive saved count. Packet S's shorthand does not override this body.

## Source and test feasibility

Current `save_current.cpp:639–654` caches one model/step, so the structural difference exists. `src/engine/loader.cpp:168–180` dispatches the virtual `classModel` on every `opModel` call; `include/m2m/mst/writer.hpp:204` exposes the override. `tests/s/test_save_dispatch.cpp:12–74` demonstrates class ownership/registry setup. A subclass returning owned A/B models can therefore count and script the helper's lookups without a new production callback. Returning null from that override falls through to `op.engineModel`/creation; any future null-wrapper test must account for that fallback.

The proposed A traversal/B writer test distinguishes writer reacquisition. Exact ordered paths plus distinguishable serialized payload are necessary; call counts alone are insufficient. Native `L0/A/a0a.c:419` resolves the material by number before fopen. Current `saveMaterialKey:621–634` does likewise for `nativeTaskKeys`, before the callback/open in `writeTaskKey`. A missing writer-time number can preserve a sentinel and skip the callback while a later position proceeds. Filtering remains a traversal-time decision; writer-time type filtering would be an invented behavior.

**Nonblocking test refinement:** the same-count/different-order fallback proves traversal reacquisition but cannot independently distinguish a saved count from a refreshed count. Prefer A with two valid nodes and B with three valid nodes, with both count fields consistent. Switch traversal to B and require exactly two positions/no third path. This proves the original count snapshot without malformed list metadata. The existing nth helper (`source/port/defmodel/src/mesh2d/material.cpp:126`) follows links; `material_list_add` updates list count, while fixtures must also maintain step `n_materials`.

Copying number/filter result before path generation avoids carrying a traversal borrow into the next resolution. It does not protect writer-time material/step pointers against destruction inside `beforeNativeKeyWrite`; the callback runs after number lookup. Keep both model/step owners alive throughout synthetic tests, and retain this limitation. Lexical scope is not an ownership guarantee.

## Precise owners and acceptance boundary

- Implementation owner: `source/apps/moproj2mst/src/engine/save_current.cpp`, specifically `saveMaterials`; retain existing writer and path contracts.
- Test owner: `source/apps/moproj2mst/tests/s/test_save_current.cpp`; preserve detached-step coverage and add scripted subclass tests there. `test_save_dispatch.cpp` supplies the seam example and needs no change solely for this work.
- Read-only contract references: `src/engine/loader.cpp`, `include/m2m/mst/writer.hpp`, and the material-list helpers above. No production seam or ownership redesign is justified by this plan.
- Root owns focused `m2m.s.save_current` validation, required grouped/full checks, evidence freeze, and any build decision. No tests were run in this review.

Initial null-step zero count, snapshot order, repeated lookups, filtering, path/number selection, and ignored writer status are supported. Public input-driven replacement/null-wrapper reachability, native fault equivalence, and destructive callback/reentry safety remain unknown. The plan states these limits accurately. Later null guards may remain a qualified policy divergence; do not replace them with deliberate undefined behavior or claim them as native failure recovery.
