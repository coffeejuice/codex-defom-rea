# CP267 converter Save-I/O leaf audit

## Disposition

No additional small Save-I/O implementation leaf is ready for promotion from the retained evidence. The closest uncovered source-level candidate is the whole-model KEY writer in `source/apps/moproj2mst/src/engine/save_current.cpp` (around line 709), but its current explicit refusal is justified: deferred DB-step temporary ownership and unchecked `fopen` behavior at native `0x140895350` remain unrecovered. Implementing around either unknown would invent caller-visible I/O behavior. Keep the refusal.

This is an exact evidence blocker, not a claim that the KEY route is unreachable. The native function is reached through `saveCurrentOperation` in the public Save sequence. What is missing is a source-closed contract for the temporary DB-step object lifetime and the failed-open branch, plus retained original output evidence suitable for an actual-caller regression. The current refusal is not an opportunity to add an isolated formatter test and call the route recovered.

## Scope reconciliation

- The current public Save path is `engine::saveProject` → `saveCurrent` → `saveCurrentOperation`, followed by the `.moproj` writer. `save_project.cpp` still guards the last-operation warning-modal path, so that distinct unresolved gate cannot be bypassed by testing a private writer alone.
- `save_current.cpp` contains explicit refusals for whole-model KEY temporary ownership/open failure, SimCtrl PPV ordering/owner/codec, DOE Output owner/modal/enumeration, and Report's `vt+0x80` call. Each refusal depends on missing owner, ordering, or failure-path evidence; none is a source-evidenced small serializer leaf.
- The Boolean step-4 side-file dispatch, SPRING page I/O, SW2SP, Type0, and DEF_CVT3 route are previously completed or explicitly excluded; do not repeat them.
- Generic deformdb codecs/writers and existing MDT/XML helpers are not substitutes for proving that a specific writer is reached with the claimed owner and ordering during the converter's public Save.

## Exact next evidence for the KEY candidate

Before implementation, recover the native `0x140895350` call contract at the Save occurrence: the DB-step temporary's allocation/initialization, ownership transfer or release, KEY writer call order, `fopen` result handling, and externally visible partial-file effects for both success and failure. Preserve original before/after file evidence with hashes. Then the exclusive implementation surface is `source/apps/moproj2mst/src/engine/save_current.cpp` plus a new or existing test under `source/apps/moproj2mst/tests/engine/` that calls the actual `engine::saveProject`/public ABI Save path with the qualified owner and checks the retained KEY output and failure effects. The test must not reduce to a direct helper invocation.

Until that evidence exists, the exact blocker is missing native temporary-owner and unchecked-open branch semantics; there is no justified code change or actual-caller expected result to test.

## Files reviewed

- `IMPLEMENTATION_PLAN.md` (checkpoint history and current CP267 direction)
- `analysis/resume263-operation-dispatch-next-leaf.md` (completed/refused operation-dispatch reconciliation)
- `source/apps/moproj2mst/src/engine/save_project.cpp`
- `source/apps/moproj2mst/src/engine/save_current.cpp`
- `source/apps/moproj2mst/include/moproj2mst.h` (public adapter refusal contract)

Read-only source/evidence review. Only this analysis note was written; no source, tests, shared state, build, GUI, original process, or mutable REA session was touched. No tests were run.
