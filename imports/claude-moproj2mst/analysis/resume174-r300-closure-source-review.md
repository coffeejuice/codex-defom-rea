# CP174 R300 `propsStep` closure source review

## Verdict

**Source cut conditionally accepted; focused test blocked.** The four engine files remove the `propsStep` raw `DB_STEP_DATA*` return and preserve the inspected nullable, lookup and effect order. The new `tests/moproj/test_props_l2.cpp` case cannot pass as written: it assigns a stack probe to `owner.opClass`, while the reached `opModel -> classOf -> opBase` path reads `owner.project->registry[owner.classKey]` and ignores that cached field. `owner.project` is still null, so the probe receives zero lookups and the asserted count of five fails. Bind the operation to `p`, set its class key, and place an owned probe in `p.registry`; then re-review the revised test hash before build acceptance.

Reviewed source/test SHA-256 values (matching `analysis/resume174-r300-closure-implementation.md`):

| File under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `src/engine/props_sync.hpp` | `36e7a55781966e4f41d2f6b33e6b8b19c9f99ade61c6623d2dbdbac02f3d05f6` |
| `src/engine/props_sync.cpp` | `125cb1f22b4328082539a4311955c3626b1fa14886cfc04332b2d05bf466e69a` |
| `src/engine/loader_l2.cpp` | `227fb4ee444716aabca6f0d62757f015eb29ce2105704b30a682e9e59b45da5e` |
| `src/engine/loader_l3.cpp` | `c634eba9bdffbb6386938fe442bbd129682511bc40ba3e929792ada24a0ab766` |
| `tests/moproj/test_props_l2.cpp` | `3fa58bc62283b4710876f502e70a28eb593621432e82521d5124ab8b7b7dc2a7` |

## Source findings

The `withPropsStep` template has the required direct includes, restricts callback results to void/bool, invokes once with null when `pr.owner` is absent, and otherwise performs one `opModel` lookup before invoking once with the model's nullable step. It adds no geometry gate. A search of active `source/apps/moproj2mst/{src,include}` found no remaining `propsStep(` declaration or call. The type constraint prevents directly returning a borrowed pointer or reference, though it cannot by itself prevent a callback from storing one. None of the seven migrated call sites stores the step address.

`withReaderSimCtl` invokes an explicitly supplied `sc` directly and performs no owner lookup. On a null `sc`, its `withPropsStep` callback uses `&s->sim_ctrl` only within the nested synchronous callback and returns a bool. The four property readers still create `B(pr)` only after a nonnull control and make their prior stores in the same order. `propsSetHeatingTime` retains its exact unsupported error before creating `B(pr)` on an absent step.

The three L2 sites retain their distinct null returns. `furnacePropsLoad` still does an outer step lookup, then calls `furnaceEnvTemp` for its second independent lookup and curve/diagram effects, then copies from the *outer* step and invokes `opSimCtl` for the later property fill. The nested lookup was not coalesced. `furnaceSetModel` calls `furnace_model_copy`, whose inspected body rebuilds destination curves, copies each DYNARY and copies the name, without retaining the source furnace address. `furnaceToEngine` still acquires the step before local8 conversion, name-bound check, writes, curve update and diagram signal. Both L3 props loaders still perform their four property fills before a bool presence probe and only then perform `propsModel`/movement work. These are source-order findings; no dynamic callback/reentry or runtime behavior was observed.

The four engine files contain no selected-length or cache1 write. This access refactor does not install the Stopping barrier or advance CP163's after-A cache0 refusal. It changes no documented C export. Public `GuiModel::step`, `opModel`, `opSimCtl` and object/SimControl raw returns remain open for later closure, so this patch is not an R300 barrier acceptance claim.

## Test blocker and acceptance

The test's `StepLookupProbe::classModel` counter would be a valid observation if the class were registered. The present setup creates `Operation owner`, sets `pr.owner = &owner`, then sets only `owner.opClass = &probe`. `opBase` returns null when `owner.project` is null; its implementation then searches `project->registry` by `owner.classKey`, not `opClass`. Thus the later `checkMissing()` cannot increment `probe.lookups`, and `CHECK_EQ(probe.lookups, 5)` is contradicted by source. The proposed repair needs a `std::unique_ptr<StepLookupProbe>` in `p.registry`, a matching `owner.classKey`, `owner.project = &p`, and a retained nonowning pointer to the owned probe for assertions. Preserve the earlier unregistered-owner case before this binding. Do not use the stack probe as the registry owner.

After repair, review the new test hash and check that the null-step model is reached five times by the four fallback readers and heating setter, while the four explicit-control calls leave that count unchanged. The source-only test preflight is otherwise plausible: the new case uses existing Testkit `TEST_CASE`, `CHECK`, `CHECK_EQ` and `REQUIRE`; the relative `props_sync.hpp` include resolves; compile-time constraints cover void/bool versus pointer/reference returns. It does not include a dynamic assertion for the furnace's nested lookup count, so that order remains source-reviewed and should be exercised by the existing positive L2 furnace case plus a focused counter case if root needs runtime proof.

Root should compile the changed core and the `m2m.moproj.props_l2` target after the test repair, then run `m2m.moproj.props_l2`, `m2m.mdt.loader_l2`, `m2m.mdt.engine_l3`, `m2m.s.save_current`, `m2m.mdt.simcontrol300_stopping_cached`, and `m2m.g0.abi` under source-bound hashes. Check the new null/explicit-control case actually executes, and retain the complete source-stable suite/freeze gate before releasing successor source work. If the repaired source or tests fail, revert the four-file accessor migration and its test together to the frozen CP173 version, restoring `propsStep` and its seven callers as one unit. Cache0/A/refusal require no rollback action.

No build, CTest, GUI/app control, original execution, live REA, mutable analysis DB, other source, or deployment was touched in this review. The retained original is DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; the static native zero branch does not establish public Open/Save reachability or reader-tail completion.

## Re-review of repaired focused fixture

The test owner repaired only `tests/moproj/test_props_l2.cpp`; its current SHA-256 is `9b2ca7f9e2cbcc1cf4ebc0830dfe662e1b9ba5bb7a268cc7331e2202a0df46bf`. The earlier `3fa58b...` hash and blocker above describe the superseded fixture, not the current build candidate. The new setup gives `owner.project = &p`, sets `owner.classKey` to the probe key, and inserts a `std::unique_ptr<StepLookupProbe>` into `p.registry` before resetting its model step. `opBase` can now resolve that exact owned probe. The retained reference remains valid after moving the unique pointer because the object itself stays at the same address. Four fallback readers and heating setter each reach `classModel` once, making the expected count five; explicit `sc` readers bypass `withPropsStep`, so the count remains five. The unregistered-owner case still executes before registry binding.

**Updated verdict: source and focused fixture accepted for a source-bound compile and focused tests.** The prior test-binding blocker is resolved. This is a static review only; the compiler and CTest have not yet confirmed the candidate. The source hashes in the table above remain unchanged, and the integration targets and original-evidence limits remain as stated.
