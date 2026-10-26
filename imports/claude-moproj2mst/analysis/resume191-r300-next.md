# CP191 R300 next owner cut after equipment

## Decision

GO for `storeSimNames(Project&, Operation&)` in `source/apps/moproj2mst/src/engine/save_current.cpp:219-242`, with one direct fixture in `source/apps/moproj2mst/tests/s/test_save_current.cpp` (`m2m.s.save_current`). Its `Cls_1401e7b10* sc` is an embedded step-owned alias returned by `GuiModel::simCtl`, and remains acquired in the function body while four ordered stores run. Confine that acquisition, its existing null check and all four stores to one immediately invoked, nonstored void callback. This is a small remaining alias-owner migration, not a new raw `GuiModel::step` dereference or a lifetime-safety proof.

Assign only those two files after root releases the CP190 pair and reconciles its freeze. Root owns independent review, focused/full tests and freeze. No implementation is included here.

## Reconciliation against CP173 and CP190

CP173 is an inventory of earlier source, not a current todo list. Current source contains CP190's equipment callback and CP189's restart callback. Live SHA-256 values match the CP190 implementation release: `save_current.cpp` `b0d62452408864276dde7825265ced96c1e5fc45cc1fb10b8c047530e84704a0`; `test_save_io.cpp` `346f8454c0a9e4fd1f519d3824b63de99fdb971b70a2a36442f86b3d6e1592ab`. The selected test file `test_save_current.cpp` currently hashes to `37b8fa328c8605c31faff5559aa4ff8f7955f1431afbe7a1bfb8adc77a50b552`. This pass did not compare a CP190 archive or rerun integration.

Do not repeat `saveMaterials`, `saveInterObject`, contact flags, BCC, restart or equipment work. The equipment raw expression is deliberately inside the completed slot-local callback. The `syncOperationFiles` NSTART tail at lines 757-762 already confines its `sc` alias to an if-initializer and its body; it performs only `nstartGiven = 1` and the `sc->nstart` copy before that scope ends. Another immediate callback there would add no meaningful smaller lexical boundary. Cached model ownership remains open throughout sync, and must not be silently claimed closed.

`storeSimNames` is callable separately through internal declarations and `baseSaveNames`; FormingExpress also calls it from `save_l3.cpp`. It was not separately enumerated in CP173's direct `.step` table because its alias comes from `simCtl()`. It is a genuine step-owned alias-owner site to include in the ongoing source migration. `GuiModel::simCtl()` at gui_model.cpp:179-183 calls `requireGeometrySyncAvailable()` before returning `step ? &step->sim_ctrl : nullptr`. Preserve that existing accessor and guard.

## Exact boundary, order and null policy

Leave the prefix unchanged: active-subproject lookup; return if absent; snapshot `op.name`; compute flow number; build `mst::simEntries`; select the first matching `pos`; return on no match; reject an unnamed entry with the existing `notPorted`; acquire cached `m` with `opModel`.

At the existing `m ? m->simCtl() : nullptr` acquisition, immediately invoke a void lambda taking `Cls_1401e7b10* sc`. Inside it retain `if (!sc) return`, then preserve:

1. `strncpy(sc->oprnam, local8(name).c_str(), 0x40)`.
2. `sc->cursim[1] = no`.
3. `strncpy(sc->simnam, local8(first->name).c_str(), 0x50)`.
4. `sc->cursim[0] = first->sim`.

Keep locale conversion at each original store; do not preconvert before model/SimControl admission. Preserve exact strncpy counts, including the absence of a newly appended terminator. Do not replace `simCtl` with direct `&step->sim_ctrl`, `withOpStep`, another `opModel`, or `GuiModelAccess::withStep`: these change lookup/admission behavior. Do not move the unnamed-entry rejection below model acquisition. No pointer/reference may be returned or stored by the callback. `first` remains a pointer into the live local `all` vector; that non-model alias is valid through this synchronous callback and requires no new ownership mechanism.

This preserves existing no-model/no-step behavior through `simCtl`: null means no stores. The geometry guard still precedes the accessor's null-step decision. A prior `m` check is not a lifetime pin. Static confinement narrows where the alias can be used; it does not enforce non-reentry, private step storage, the parked Stopping gate or arbitrary owner replacement safety.

## Focused direct fixture and reachability

Add `store_sim_names_step_scope` in `tests/s/test_save_current.cpp`, reusing the setup pattern of `save_interobject_step_scope` at line 506: configured corpus `46_MO2_Boolean_Oprations/MO2_Boolean_Oprations.moproj`, initialize/open project, activate subproject 0, operation ID 2, require class `SfOprHeatTransferExpress2D`, install loader slots, select its class/current operation and load the actual model. Keep every owner alive and mutation in the disposable in-memory project; no save/current/project/native file effects are required. Call `engine::storeSimNames` directly through the existing engine_internal include.

The entry path is inspectable: `src/model/simcount.cpp:264` traverses MST order and dispatches `simList`; the base `OpClass::simList` at lines 56-59 delegates to `simListOne`, which marks an operation-derived name as named. Before asserting output, independently compute `flowNumber`, `mst::simEntries` and its first matching entry, require that entry exists and is named, and bind expected scalar/string values. Do not let an absent entry make the test vacuously pass. This report establishes source reachability, not an executed fixture result.

Seed OPRNAM, SIMNAM and both CURSIM elements with distinct sentinels. Call the helper; assert the four resulting fields against the name, flow number, first entry name and simulation number, plus unchanged model/step identity. A repeat call after changing the operation's short ASCII name verifies the helper rebuilds entry data rather than using stale text. Use short names for this mechanics probe; changing strncpy semantics or adding Unicode/long-name behavior is outside this cut.

A detached-step negative is admitted for this direct helper, unlike full sync: after confirming a matching named entry, temporarily move the step into a local RAII restoration guard, call `storeSimNames`, require no exception and unchanged sentinels in the retained step, then restore and verify identity. `opModel` retains the cached model; `simCtl` explicitly returns null when that step is absent. Do not invoke full `syncOperationFiles` while detached. A no-match negative can use a separate disposable project state with all produced sim positions deliberately mapped away from this flow number (`doeSimPosSet`/`doeSimPos`), but is optional if it needs unrelated fixture plumbing; source review must retain the early return regardless.

Preflight existing Testkit entry/includes/macros, then root confirms the registered `m2m.s.save_current` target; shared-source `m2m.s.save_io` remains appropriate regression coverage. No test was run here. Runtime field/identity checks establish reconstructed helper mechanics; static review establishes nonescape.

## Native basis and limits

Read Packet S `source/docs/re/moproj2mst/s/spec.md` section 1. It retains `0x140054540` as active-subproject/name/chain-number/list selection, first-entry admission, then OPRNAM (SC+0x51, 0x40), CURSIM[1] (+0xe8), SIMNAM (+0x92, 0x50) and CURSIM[0] (+0xe4), with no writes for an empty list or absent SimControl. The source function is annotated with that address. The caller table places base vt+0xd8 after properties and before vt+0xe8 sync; FormingExpress has its separate retained caller.

This is retained annotated source evidence. Packet S names historical `deform-rea-work/gui_mo/out_s/s1c.c` through `s9c.c`; the checked s1c path is absent here, consistent with the prior qualified reviews. No exact native assembly, new runtime trace, native-byte comparison or public GUI Save-equivalence evidence was collected. The existing unnamed-entry refusal is reconstructed policy and must stay unchanged, not be presented as a new native claim.

The larger CP173 closure remains unresolved: public step access, raw accessor return capability, geometry/viewer/Stopping aliases and aggregate ownership/admission still need their own cuts and evidence. This proposal neither installs nor validates the R300 parked barrier.

This read-only audit wrote only `analysis/resume191-r300-next.md`. No source/test/build, executable/deployment, GUI/app-control session, live REA or mutable analysis DB was changed or run.
