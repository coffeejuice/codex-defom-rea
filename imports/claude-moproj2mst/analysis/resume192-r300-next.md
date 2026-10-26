# CP192 R300 next alias owner after storeSimNames

## Decision and source binding

GO for `htSubTypeDefaults(Project&, HeatTransferProps&)` at `source/apps/moproj2mst/src/engine/loader_l2.cpp:116-148`, paired with `source/apps/moproj2mst/tests/mdt/test_loader_l2.cpp` (`m2m.mdt.loader_l2`, target `m2m_mdt_loader_l2`). The function retains the step-owned SimControl alias from `opSimCtl` across subtype selection and the nested `propsSetHeatingTime` call, then writes TMAX and ENVTMP. This is a genuine remaining alias owner. Confine its acquisition and entire consuming tail to one immediately invoked, nonstored void callback; leave the owner lookup and absent-owner return outside.

Both proposed owner files match their entries byte-for-byte in `build/wave3-checkpoint191-verified/source.zip`. Current SHA-256:

- `loader_l2.cpp`: `227fb4ee444716aabca6f0d62757f015eb29ce2105704b30a682e9e59b45da5e`.
- `test_loader_l2.cpp`: `aed6f77b4414123a883e01c494428cc419e2ccc3a1df9d81cc4d2086ec88a950`.

Current `save_current.cpp` hashes to CP191's released `62d4f2a827a5fc281692e0b5d8199a549a27594585018fed77ac3d399a46c455`; its storeSimNames callback is already present. This proposal does not reopen that pair. Root owns independent review, focused/full checks and the next freeze.

## CP173 reconciliation

CP173 is a historical inventory, not an executable remaining-work list. `opStep` has already become `withOpStep`; `withPropsStep` and the props-sync readers/heating-time helper already confine their own aliases. Save-current materials/interobject/contact/BCC/restart/equipment/storeSimNames and the scoped L3/L6 saves must not be repeated. In particular, sync's NSTART if-initializer, the one-store TRANS setters in loader_l2, scene's nested SimControl if-initializers, and formingSimControl300Open's final two-line alias/use are already tight scopes; adding callbacks to those would provide no useful owner cut.

The selected L2 alias was not individually enumerated by CP173's direct `.step` search because it is acquired through `opSimCtl`. `opSimCtl` at loader_l2.cpp:55-59 still returns `m ? m->simCtl() : nullptr`; its general pointer-return seam remains open. This change narrows one consumer without claiming that helper, public step storage, viewer/geometry access, aggregate model ownership or the parked Stopping barrier is closed. Larger remaining examples include equipmentSync, FormingExpress save-name processing, L5 scratch/actual-step work and cogging placement; they have more ownership/effect branches than this cut.

## Exact boundary, ordering, null and geometry policy

Keep `Operation* owner = pr.owner; if (!owner) return;` unchanged. Invoke a void lambda taking `Cls_1401e7b10* sc` with the existing single `opSimCtl(p, *owner)` expression. Put the current null return and every statement through `sc->envtmp.value = env` inside it. Do not return or store a pointer/reference or retain the callback. Capture only the live references/owner needed by this synchronous invocation.

Preserve the exact sequence:

1. Owner admission, then one `opSimCtl` lookup and its null return; only afterward snapshot `owner->subType`, construct the default label and read `sc->unit`.
2. Default time 3600 and ambient 20 for unit 1, otherwise 68. Test case-insensitive contains in order: Transfer through air (15), Rest on die (4), Dwell on die (4). Only the final else changes ambient to 1200 for unit 1, otherwise 2250. Preserve label assignments and the existing UI-only `(void)label`.
3. Call `propsSetHeatingTime(p, pr, t)` before any of the three SimControl stores. Keep its separate owner lookup and `withPropsStep` behavior; do not fold it into the borrowed SimControl, precompute text, or remove that nested lookup.
4. Store `sc->tmax = t`, then `sc->envtmp.ndim = 0`, then `sc->envtmp.value = env`.

Do not replace acquisition with `withPropsStep`, `withOpStep`, direct `&step->sim_ctrl` or `GuiModelAccess::withStep`; those alter the existing accessor/guard path. `GuiModel::simCtl` at gui_model.cpp:179-183 calls `requireGeometrySyncAvailable` before testing nullable step and returning its embedded control. Thus a geometry barrier refuses before subtype/time/property effects, even if the step is null. An ordinary null step returns from this direct helper before propsSetHeatingTime, so it must not acquire that helper's null-model refusal. The no-owner case must not create a model. Existing cached-model and owner behavior stays unchanged.

This lexical cut does not pin the model, prevent reentry or destructive owner replacement, invalidate prior aliases, or make the nested property setter safe against arbitrary owner mutation. Preserve those limits explicitly.

## Direct fixture

Add `heat_transfer_subtype_defaults_step_scope` to the existing L2 test file. The function has external linkage but no current header declaration; add only a matching test-local forward declaration in `namespace m2m::engine`, without changing production headers/API. Existing Testkit uses `TEST_CASE`, `REQUIRE`, `CHECK`, `CHECK_EQ`; do not invent REQUIRE_EQ or C assert.

Reuse `openCorpus`, `load`, ProjectScope and configured `08_HEAT1/HEAT1.moproj`. Obtain operation ID 1, require HeatTransferProps and its owner identity, load the actual model once, and require cached model, step and SimControl. Keep the project and all owners alive. Call `engine::htSubTypeDefaults` directly; do not use full load as the assertion trigger because postload can choose/overwrite subtype and add-object refusals are outside this cut.

For each row, set an explicit short subtype, unit and distinct TMAX/ENVTMP/heatingTime sentinels; snapshot model/step/control identity and an unrelated control field such as NSTEP. Check TMAX, ndim zero, ambient, property heating-time text, unchanged unrelated field and all identities. Exercise Transfer, Rest, Dwell and unmatched/furnace at unit 1 (15/4/4/3600; ambient 20/20/20/1200), repeat under a non-1 unit (ambient 68/68/68/2250), and use mixed-case strings. An overlap string containing all three recognized phrases must select Transfer; another containing Rest and Dwell must select Rest. Independently bind expected time and ambient per row. The existing SI fixture establishes `15 sec`; use retained time-unit table/source for the other expected text spellings rather than deriving expected text by calling the helper under test. No files need saving.

After a positive call, retain the step in a local RAII restoration guard, reseed its control and the property text, call only this direct helper, and require no exception or sentinel changes. `opModel` must keep the cached model while `simCtl` returns null; restore before any other operation and verify identity. Separately use a standalone HeatTransferProps with null owner and a sentinel CommonBlock: direct call must leave it untouched and must not change the operation's cache. No detached full-load/save fixture is admitted.

Source review, not identity checks, proves the callback is synchronous/nonstored and the stores remain after propsSetHeatingTime. The registered candidate CTest metadata contains `m2m.mdt.loader_l2`; root should run that focused target and the required source-stable full regression, including existing props-sync coverage, then freeze. No tests were executed by this audit.

## Native qualification and limits

Read `source/docs/re/moproj2mst/mdt/L2/spec.md` introduction and section 2. It identifies static analysis of `DEF_GUI_MO_64.exe`, historical annotated `deform-rea-work/gui_mo/s1/mdt/L2/d0a.c` through `d5a.c` and `ht_post.asm`, Ghidra copy gproj_L2. Its `0x141358150` entry retains subtype priority/constants, heating text through `0x1405fcb90`, the dropped UI label `0x1405fd090`, then TMAX/ENVTMP mode/value. `ledger.tsv` binds this address to the L2 subtype-default helper. Current source's `@src` annotation and existing corpus08 first-setup test independently identify this route; the latter is reconstructed mechanics coverage, not a new native observation.

The named original L2 decompile/asm files were not found in the converter's file inventory. Consequently this is retained annotated native evidence plus current source, not newly inspected exact native assembly, fresh original runtime evidence or byte-equivalence proof. The null/geometry policy described above is established by current reconstruction; do not silently ascribe it to the native caller. Source-ready lexical confinement needs no new original capture. A stronger claim about original null behavior, callback reentry, same-occurrence reader continuation or public GUI Open/Save equivalence remains evidence-blocked and outside this pair.

This read-only pass wrote only `analysis/resume192-r300-next.md`. No source/test/build, executable/deployment, GUI/app-control, live REA, mutable analysis DB or shared plan/state was changed or run.
