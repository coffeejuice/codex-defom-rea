# CP174 R300 props-step closure implementation

Status: source-only implementation complete; exclusive ownership released to root integration. No build, test execution, candidate freeze, GUI session, live REA mutation, deployment, or shared recovery-state edit was performed by this owner. Compilation and runtime results remain pending root verification.

## Changed files and SHA-256

- `source/apps/moproj2mst/src/engine/props_sync.hpp`: `36e7a55781966e4f41d2f6b33e6b8b19c9f99ade61c6623d2dbdbac02f3d05f6`
- `source/apps/moproj2mst/src/engine/props_sync.cpp`: `125cb1f22b4328082539a4311955c3626b1fa14886cfc04332b2d05bf466e69a`
- `source/apps/moproj2mst/src/engine/loader_l2.cpp`: `227fb4ee444716aabca6f0d62757f015eb29ce2105704b30a682e9e59b45da5e`
- `source/apps/moproj2mst/src/engine/loader_l3.cpp`: `c634eba9bdffbb6386938fe442bbd129682511bc40ba3e929792ada24a0ab766`
- `source/apps/moproj2mst/tests/moproj/test_props_l2.cpp`: `9b2ca7f9e2cbcc1cf4ebc0830dfe662e1b9ba5bb7a268cc7331e2202a0df46bf`

The requested props-test path was corrected from nonexistent `tests/mdt/test_props_l2.cpp` to existing `source/apps/moproj2mst/tests/moproj/test_props_l2.cpp`; root was notified before modification. No L3 test or DOE-owned source/test was changed.

## Source coverage and preservation

`withPropsStep` constrains callback results to exactly void/bool, short-circuits an absent owner before `opModel`, and invokes once with the same nullable step. It deliberately adds no geometry gate or lifetime guarantee. All seven former acquisitions are migrated: props-sync reader fallback and heating-time setter; furnace environment read, props load and save; L3 forming and FE presence checks. Search of active `src` and `include` found no `propsStep(` declaration, definition or invocation. A Python source assertion verified this and the seven calls plus helper definition.

All four props-sync readers use a bool-only scoped `withReaderSimCtl`. Explicit caller `sc` is invoked directly without a model lookup. The owner-derived `&s->sim_ctrl` is passed immediately to the reader callback and never returned or captured into storage. `B(pr)` allocations and stores remain in the same order; heating-time absence throws the identical unsupported code/text before `B(pr)` mutation.

Furnace props loading retains the original sequence: outer owner lookup, null check, nested `furnaceEnvTemp` owner lookup and curve/diagram effects, copy using the original outer step, then the `opSimCtl` lookup for furnace data. Neither lookup is coalesced. `furnaceSetModel` calls `def::cmd::furnace_model_copy`; inspection of `source/port/defcmd/src/p8_08/model_copy.cpp` lines 142–153 shows clear/setup of destination curves, dynary copies and name strcpy, with no retained source furnace/step address. Furnace writer keeps null return, local8/name bound refusal, strcpy, curve update and diagram event order. No FileEffects logic changed. L3 still performs four property fills before the presence probe and only then its second propsModel/movement lookup.

Every migrated callback was inspected for pointer escape: step, furnace, curve and sim-control pointers remain in synchronous scopes; returned values are void/bool. Captures reference existing p/pr only; no callback is queued or stored. Callback return constraints cannot prevent arbitrary future capture side effects, so this remains a reviewed borrow seam, not a concurrency/ownership proof.

## Added tests and preflight

The existing props Testkit target now has compile-time call-expression constraints accepting void/bool and rejecting step pointer/reference, SimControl pointer, int and wrong-arity callbacks; return types are checked explicitly. The added runtime case covers exactly-once void/bool null callbacks, absent owner, unregistered owner/no model, present class model/no step, all four absent-control readers, unchanged absent common block, identical heating-time refusal, and all four explicit-control readers with expected values. A classModel counter verifies the null-step path makes five lookups (four readers and heating setter), and explicit sc leaves that count unchanged.

Preflight inspected existing Testkit main, supported CHECK/CHECK_EQ/REQUIRE macros, complete SimControl/step types, default SimControl constructor, GuiModel null-db constructor and null-step destructor behavior. New quoted `../../src/engine/props_sync.hpp` resolves relative to the existing test source. Its m2m includes resolve through the target's public include directory; defmodel types flow through m2m_core PUBLIC dependencies. No C assert or unsupported assertion macro added. These checks do not substitute for compilation.

No dynamic furnace lookup-order probe was added by this owner; source order is preserved as described. Existing L2 positive furnace and L3 property coverage remain in their original targets, and root owns focused group execution including the changed moproj props_l2 target. Do not present this source-only receipt as proof that furnace ordering or nullable branches executed.

## Remaining aliases and integration boundary

GuiModel::step remains public. opModel, opSimCtl, GuiModel::simCtl/object/objectAt and movement/object/aggregate pointers remain independent alias routes requiring later owner/lifetime work. No private-member migration, cache1, selected-length read, Stopping barrier, CP163 cache0/A/after-A refusal, exported header or C ABI signature changed. No new original runtime behavior is claimed by this mechanical source migration.

## Integration fixture repair

Root's first focused run found the new props test's lookup counter was zero rather than five. The fixture had set the cached `owner.opClass`, while `opBase` reads `owner.project->registry[owner.classKey]` (src/model/registry_rescue.cpp:63–68). Repaired only the test: a unique_ptr-owned StepLookupProbe is inserted in p.registry, owner.project and owner.classKey bind to that registry entry, and counters/model access use a reference to the owned instance. Production code is unchanged. Source assertions verify the binding and the absence of the old cached-pointer assignment; active src/include still contain no raw propsStep call. No build/test run was performed by this owner for the repair; root rerun remains required. Test ownership released again after this update.
