# CP67: next active DOE output branch after InputTable refresh

## Finding

The earliest post-InputTable DOE output is Type2's **current-sample `DEF_DOE.PAR` write**, followed by the three per-design sample files. Its serialization and file effects are independent of the GUI receiver once the preceding `signalDoeTableRefresh()` call has actually returned. The current public candidate cannot reach it: `write.cpp::subProjectElement` calls the XML prefix, then throws at the unconditional refresh guard. The guard must remain until the real table/timer/UI receiver path is represented. Consequently, a helper-only sample test could validate formatting, but would not be a public Save regression or justify advancing this guard.

## Native order and owners

The controlling original Save arm is `0x140099040` (original PE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; bounded call-order evidence is summarized in `resume63-doe-public-save-order.md` and `resume64-doe-save-effects.md`). For a non-null active-SUB manager cached at SUB `+0x20`, the order is:

1. Append `<SfDoeStudy name=manager.name>` to the pending `Sub_Project` (`0x1400d29d0`, then DOM append).
2. Save `DoeInputTable.XML` (`0x1400d3200` -> reconciliation `0x1400d2b60` -> writer `0x14090a550`). The writer performs empty-variable cleanup before QFile open, serializes/closes/checks error, and the wrapper unconditionally emits `signalDoeTableRefresh()` at `0x1400d369d` before returning the saved writer Boolean.
3. After the refresh callback returns, save the current mode to `Control/DEF_DOE.PAR` through `0x1400d75a0` (call at `0x1400997b2`). This is the first post-refresh output branch. It uses the arm's cached manager, selected mode, and that mode's table/invalid state; invalid state skips the file. A mode outside `[0,2]` reaches the original unsupported boundary.
4. Call `0x1400d7790` at `0x140099a77`, `0x140099d7c`, and `0x14009a081` with modes 0, 1, and 2 respectively, targeting `Samples_LHS.par`, `Samples_FF.par`, and `Samples_USR.par`. These are conditional on each mode's validity. Ordinary helper Boolean results do not cancel later calls.
5. Append common `Subproject_DbName`; only after the helper returns does the parent replace/append the old `Sub_Project` node and stream/close the already-open `.moproj`.

The sample writer has no GUI object in its own contract. Candidate `doe_study.cpp::saveCurrentSamples` validates the selected mode, builds `DoeStudies/<study>/Control/DEF_DOE.PAR`, and delegates to `saveSampleTable`; that helper skips invalid modes and otherwise serializes the mode header and rows into `FileEffects`. However, `rg` finds only helper declarations/definitions and unit-test uses, no production call site. `FileEffects` is also not evidence of native immediate writes or of the public Save's ordered output receipt.

## Smallest independent reconstruction boundary

The narrow reusable action is the existing sample serialization helper contract, kept isolated: test `saveCurrentSamples`/`saveSampleTable` for valid selected mode, invalid-mode skip, empty-but-valid mode header, row formatting and out-of-range mode refusal. This needs no GUI receiver and should remain a helper-level test. It is not a reason to wire any of the four writer calls into `subProjectElement` before the refresh receiver exists. A later integrated release must cache and validate the same actual Study/manager for the full call sequence, preserve XML effects if refresh refuses, and append each later file effect at its native position.

There is no later variable-finalization branch independently reachable in this public sequence: the nonempty-variable reconciliation/serializer guards precede XML output, while the current public path stops at refresh. Type3 optimization is a separate arm, not the next Type2 continuation: it requires the SUB `+0x28` optimization owner and `+0x20` manager, then appends a study element, derives `.OPT`, saves InputTable, and calls optimization writer `0x140907a10` (`0x14009a3ec`). Its input/notification ownership is not closed either, and the current Type3 guard must stay separate.

## Candidate guard and next action

Current source `src/moproj/write.cpp` explicitly throws `xml.guardMessage()` after the immediate XML prefix. That prefix's documented next boundary is actual `DoeTableRefresh` delivery, not the XML Boolean. Both ordinary XML success and failure take the same refresh path; after a future successful native receiver return, the first next phase is `DEF_DOE.PAR`, then LHS/FF/USR in order. Until then, do not add public sample calls or claim active Type2 Save completion. Root should first provide the source-owned table/timer/UI lifecycle and refresh-return boundary; then integrate the cached-manager sample sequence at the existing shared Save callsite with immediate effects and an ordered result receipt.

This is a read-only source/evidence audit. No production/test files, build, GUI, original runtime or mutable REA state changed. Static claims above are qualified by retained native evidence and candidate-source inspection; no fresh-original acceptance or complete active DOE Save is established.
