# CP275 Save owner next-leaf assessment

## Decision

No new finite, behavior-neutral Save owner-command migration is qualified after CP272. The next Save acceptance work is blocked on a fresh same-run original RIO failure witness, not on another candidate-side alias refactor. Do not duplicate `storeSimNames`, CP247 equipment ordering, or CP256 MultiBlow PDIE work.

## Evidence for the boundary

- CP233 reviewed the remaining compact-looking Save candidates and found that `captureKey`/`writeTaskKey` own callback timing, file effects and writer failure results; `saveInterObject` can mutate contacts before opening its file; and handing a native writer a copied value does not replace its borrowed step. Its identified seam was a fresh original Open → Save → Close witness, not a source edit.
- CP247 already split Forming equipment creation into a prefix and final append while preserving the two manager-path lookups, ID commits and KEY attempt order. Its tests and source review establish only that qualified successful-add segment; native manager-add failure and writer failure remain unresolved.
- CP256 already moved MultiBlow's PDIE store behind a narrow model command, retaining the two-control-admission successful path. Its review explicitly limits equivalence to the synchronous production provider/stable storage profile.
- CP272 consumed the distinct remaining finite tuple: first named `simEntries` match plus operation name/flow/simulation name/number is handed to one `GuiModel` command. The caller and command ordering are source-reviewed, with actual MultiBlow Save tests. Repeating that leaf would add no ownership coverage.

Current `save_current.cpp` confirms the remaining candidates are materially larger boundaries. `saveSimCtrl` passes the borrowed step into the native SimCtrl KEY writer, then conditionally writes PPV; on failed native open it skips model-flag mutation and block writers. `saveInterObject` can rebuild contacts and update flags before target derivation/open. `syncOperationFiles` sequences SimCtrl, interobject, object files, mesh settings, BCC, object properties, equipment and materials, then restart output. `saveCurrentOperation` adds property projection, class save-name dispatch, operation-file dispatch and after-file stages around that sequence. Extracting one of these as an owner command would either move a multi-field native serialization boundary or alter observable ordering/failure behavior.

Retained original static evidence supports these as live ordering constraints: `evidence/gui_mo/out_scope/opsave.c` shows `0x140057370` dispatching `0x1400597d0` then `0x14005a8a0`; `opsave4.c` retains the SimCtrl and interobject helpers; and `source/docs/re/moproj2mst/s/spec.md` sections 1–4 records their Save ordering and contact rebuild. This is static evidence only, not a new original runtime witness.

## Exact blocker and next evidence

`analysis/resume214-rio-failure-order.md` and `analysis/resume273-save-failure-next-leaf.md` identify the unmet evidence: one fresh owned original run must bind successful Open, enabled remesh, Save-time CWD/path, pre-open write denial, failed native open/first null `fprintf`, same-module `errno`, invalid-parameter handler/filter/termination path, external process result, and same-run output manifest. Candidate denial permutations already exist and do not establish that original terminal route. The root recovery status says supported GUI app-control lacks a release operation plus verifiable receipt; the subtree's control-cleanup instruction therefore bars reacquiring a GUI session until that capability exists. This is the concrete blocker to the next Save acceptance step.

Existing focused tests (`tests/s/test_save_io.cpp` and `tests/s/test_save_current.cpp`) cover candidate writer ordering and selected Save segments, but none can establish the missing original process outcome. No new focused seam is justified until the original witness identifies a distinct bounded behavior to implement. RACCEPT and fresh Open → Save → Close acceptance remain open; no historical SAME/DIFF classification changes.

## Scope

Read-only source and retained-static review. No source/tests, build, original process, GUI, REA state or shared recovery state were changed. This report is the sole write.
