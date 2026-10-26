# CP193 R300 next genuine alias owner after htSubTypeDefaults

## Decision and frozen binding

GO for `furnacePostLoad` at `source/apps/moproj2mst/src/engine/loader_l2.cpp:322-335`, paired only with `source/apps/moproj2mst/tests/mdt/test_loader_l2.cpp` (`m2m.mdt.loader_l2`, target `m2m_mdt_loader_l2`, confirmed in `build/candidate/CTestTestfile.cmake:321`). The local `Cls_1401e7b10* sc` at line 327 remains a genuine step-owned alias across the furnace flag store, curve-count branch and curve setup/default calls. This is smaller than the neighboring heat-transfer postload owner, equipmentSync, L5 scratch/actual-step processing, or L6 model-copy/save routes.

Both owner files were compared byte-for-byte with `build/wave3-checkpoint192-verified/source.zip`, whose entries omit `source/`; both match. SHA-256:

- `loader_l2.cpp`: `8832bee8a682e4bf20a5d37378c81780a5ac9cf4341b6b147b5d49df18c7528b`.
- `test_loader_l2.cpp`: `6a4745a4737e124448b66881d9036d9eba0c8381df3a31848831ca39e596831e`.

CP192's htSubTypeDefaults callback and released test are present. Do not edit their scopes or reopen their explicitly deferred arbitrary-subtype/null tests. Root owns independent review, builds, focused/full checks and freeze.

## Minimal source boundary

Keep `GuiModel* m = opModel(p, op); if (!m) return;` outside and unchanged. Replace only the `sc` declaration and its consuming statements with one immediately invoked, nonstored void lambda taking `Cls_1401e7b10* sc`, invoked once with the original `m->simCtl()` expression. It needs no captures. Inside, retain this exact order:

1. `sc->furnace_on = 1`.
2. Test `sc->furnace.curve_count() == 0`.
3. Only in that branch, initialize `const std::int32_t n = 0x16`, call `sc->furnace.setup_curves(&n)`, then `sc->furnace.set_defaults()`.

Keep `(void)furnaceProps(c.propsOf(nullptr));` after the callback. This unused dynamic cast does not consume `sc`; keeping it outside gives the alias its smallest complete scope and preserves the original order. Do not retain/return/store the callback or any step/curve alias from it. Do not turn the whole function into a wrapper around a new pointer-return helper.

Do not substitute withOpStep, withPropsStep, GuiModelAccess::withStep, or direct `&m->step->sim_ctrl`: they change acquisition or admission. The existing direct `m->simCtl()` performs `requireGeometrySyncAvailable()` before testing nullable step (`gui_model.cpp:179-183`). Thus geometry refusal remains before the flag store, curve effects and trailing props lookup, even when step is null. Ordinary null step returns a null control from simCtl and the present owner dereferences it without a null check. Preserve that existing policy in this lexical cut; do not introduce an early return, exception adapter, or detached-step execution test. The model-null early return stays separate and unchanged. These are reconstruction source facts, not new native null-policy evidence.

This cut confines one alias. It does not pin m, prevent arbitrary reentry/owner replacement, invalidate aliases already handed out, close public GuiModel::step, or finish the parked Stopping barrier.

## Reachable same-owner fixture

Add `furnace_postload_step_scope` in the existing L2 test file. Reuse `heat1_as_furnace` at lines 187-215: open configured `08_HEAT1/HEAT1.moproj`, obtain operation 1, set classKey SfOprHTFurnace, clear opClass, create htFurnaceProps(1), bind `props->owner = op`, then call the existing load helper once. Retain ProjectScope, operation, cached model, step, control and property owners throughout.

Use the already public slot `mst::g_loaderSlotsL2.furnacePostLoad` from `m2m/mst/loader_l2.hpp:20`; it is installed by loader_l2.cpp:432 and used by the class override in family.cpp:103. Obtain `opBase(*op)`, require it and the slot, and set `cls->current = op` before direct slot calls. No forward declaration, production header change, exported test seam, substitute props owner or new namespace boundary is needed. Like htSubTypeDefaults, furnacePostLoad is inside the loader file's anonymous namespace; its slot is the correct existing invocation seam. Direct slot invocation avoids a later full-load/property-copy step obscuring the branch under test.

Exercise two branches with explicit independent assertions:

- Empty: call `sc->furnace.clear_curves()`, require count zero, seed furnace_on to zero and an unrelated scalar such as NSTEP to 37, then call only the same-owner furnacePostLoad slot. Require furnace_on 1, curve_count exactly 22 (0x16), a nonnull first curve and its documented default shape. The inspected `source/port/defmodel/include/defmodel/members/furnace.hpp:12` table gives first curve ndim 1, n[0] 2 and ncomp 6; use those independent literals for a default-shape check. Count 22 independently follows the fixed 21 types 0x8e..0xa2 plus one extra type 0xa3; do not call setup_curves/set_defaults to compute the expected result.
- Populated: use the created list, retain its first curve identity and count, seed first->value and first->u54 with distinct sentinels (existing lifecycle tests use these fields), seed furnace_on zero again, call the same slot and require furnace_on 1, unchanged count, same first-curve identity and unchanged sentinel fields. This proves the nonempty branch does not rebuild or reset existing curves. Repeat the nonempty call to check its stable branch mechanics if useful; do not broaden to a detached/null model.

In both branches assert cached model/step/control identities, property owner identity, unchanged NSTEP and unchanged FileEffects collection sizes. Snapshot sizes after initial load rather than assuming that unrelated setup effects are empty. Keep the furnace property copy's curve count/name unchanged during direct slot invocation: this slot changes the engine furnace, while its trailing dynamic cast is unused. Curve identity/sentinel assertions apply only to the populated branch; clear_curves deliberately destroys prior curve owners. No assertion should dereference a curve alias retained before clear/setup.

Testkit supports TEST_CASE, REQUIRE, CHECK and CHECK_EQ. Existing includes cover the furnace and step types; add a direct dynary header only if required by first-curve field access. Static review, rather than model identity assertions, establishes the immediate callback is nonstored and all its step-owned aliases end there. Root should run the registered L2 focused test and the source-stable required full regression; this audit executed no tests.

## CP173 reconciliation

CP173's direct `.step` inventory is historical. Current opStep consumers have been migrated to withOpStep; withPropsStep and props-sync scopes, material/interobject/contact/BCC/restart/equipment/storeSimNames save scopes, and L3/L6 saves must not be repeated. CP192's opSimCtl-derived htSubTypeDefaults owner is now scoped. The two one-store TRANS setters in loader_l2, scene's nested SimControl if-initializers, sync NSTART if-initializer, and formingSimControl300Open's final tiny alias/use do not justify another callback-only cut.

The selected furnace alias was not separately listed by the original direct-token inventory because it comes through simCtl. It is still unscoped in frozen CP192 and has real conditional consuming work. General opSimCtl at loader_l2.cpp:55-59 still returns a raw alias; this function instead calls m->simCtl directly. Neither pointer-return seam is closed by this change. Neighboring htPostLoad's alias survives subtype selection, object admission, nested htSubTypeDefaults and conditional STNOUT/ELMNOD copy; equipmentSync retains both control and object aliases through file/project effects. Those are larger future cuts, not claimed closed by this selection.

## Native evidence and limits

Read `source/docs/re/moproj2mst/mdt/L2/spec.md` introduction and section 2. It qualifies its evidence as static analysis of DEF_GUI_MO_64.exe, historically annotated `deform-rea-work/gui_mo/s1/mdt/L2/d0a.c` through `d5a.c`, ht_post.asm, and Ghidra copy gproj_L2. Its `0x140c65830` description explicitly records furnace flag 1, only-empty curve setup with 0x16, defaults afterward, unused props dynamic_cast, and no SC null check. `ledger.tsv:13` binds that address to this source owner. The table binds it to HTFurnace vt+0xc0. Current @src annotations agree. The separate furnace member source at `source/port/defmodel/src/members/furnace.cpp:52-74` explains the resulting 22 count and clear/setup behavior; it is reconstructed source, not a fresh original trace.

The named historical original L2 decompile/asm files are absent from the converter file inventory. Therefore the native basis is retained annotated evidence, not newly inspected exact assembly, a live native observation, byte equivalence, or public Open/Save proof. The existing class-substitution corpus fixture is reconstructed route coverage; the L2 spec explicitly says no corpus operation is natively HTFurnace. Source-ready lexical confinement needs no new original capture. Stronger null/reentry/runtime-equivalence claims remain outside this pair.

This pass wrote only this report. No source/test/build, executable/deployment, GUI/app-control, live REA or mutable analysis DB, shared plan or shared state was changed or run.
