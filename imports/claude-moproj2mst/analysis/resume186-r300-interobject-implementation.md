# CP186 R300 InterObject implementation

`saveInterObject` now confines its nullable step borrow to one immediately invoked callback. The function still resolves `opModel(p, op)` exactly once, rejects a null model/step before all other work, optionally rebuilds system contacts, synchronizes contact flags, derives `interObjectPath`, then calls the unchanged inline `writeTaskKey` with argument 1 and the same file/stream serializers. It returns the original `r == 0` result. The callback returns only bool; the step pointer and writer callbacks are not stored or returned. Existing callback captures include the already resolved model by reference; no new accessor or owner is introduced.

## Source identity reconciliation

Exclusive ownership was assigned by root before this edit. The actual test path is `source/apps/moproj2mst/tests/s/test_save_current.cpp`; the proposal's shortened `tests/s/...` path refers to that app-relative file.

Before editing, both files byte-matched the CP185 frozen source archive:

| File | CP185 archive and pre-edit live SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/save_current.cpp` | `dfc90650f638389644301fca786b447a581f75ac749de444653eb0b8e5bedd54` |
| `source/apps/moproj2mst/tests/s/test_save_current.cpp` | `4bebfa62f75f939d3940f2da3627269ac11e53f475239b1944a7fa9995a62acf` |

`build/wave3-checkpoint185-verified/source.zip` was rehashed as `231987b3de08e9419db91886cb9e89acb84a68135802125f50cbd59aed315698`, matching its receipt; the receipt records source-tree hash `8a413389ce714958662797eb098d15766c82d170406db5619cac8aa9db27c791`. Thus `analysis/resume185-r300-next.md` describes the intended next function but predates CP185's integrated material-accessor changes. `analysis/resume186-r300-interobject-plan-review.md` observed that active edit sequence. Its last observed hashes agree with the frozen predecessor; no unresolved concurrent delta was present at this owner boundary. CP184/CP185 material fixtures and all other functions remain preserved.

## Direct helper fixture

Added `save_interobject_step_scope` to the existing `m2m.s.save_current` Testkit executable. It follows the existing operation-2 corpus load/activation/loader-slot/model-reset sequence and requires two real loaded objects. With system lubricant disabled for the writer probe, it preserves existing pairs; only if the list is empty it appends constructor-initialized kind-2 and reversed kind-1 mirror records using actual loaded object IDs. All pair kinds and both IDs are checked against the loaded object list and flag bounds before the helper call. These synthetic changes are confined to the discarded project.

The present-step probe requires success, exactly one write at `interObjectPath(op)`, nonempty captured KEY bytes, no deletes, expected INICTC flags, stable model/step identity and unchanged pair count. A noncopyable detached-step owner then exercises null refusal with `lubSystem` enabled, verifies no writes/deletes or flag/list mutation, and restores ownership on normal and exceptional exits. Restoration identity/count checks follow both paths. The guard also restores ownership when Testkit REQUIRE throws its abort exception.

These are helper mechanics assertions. They establish neither public Save admission/dispatch, native byte equivalence, public reentrant replacement reachability, cache1 publication, nor a model-wide parked barrier. The positive system-lubricant rebuild branch is preserved by source inspection, not newly exercised by this fixture.

## Validation and release

Static preflight passed: exactly one direct model lookup; null refusal before rebuild/flag/path/writer work; retained ordering, argument 1 and serializers; lexical step confinement; Testkit entry point; existing supported CHECK/CHECK_EQ/REQUIRE macros; no C assert or unsupported REQUIRE_EQ; quoted includes resolve through the source directory or target include directories. Newly added standard/defmodel includes resolve to the existing runtime headers. A frozen-archive diff confirms the production change is confined to `saveInterObject` and the test changes are the new helper declaration/includes/case.

No build or executable test was run by this owner, as assigned. Root must run `m2m.s.save_current` in the focused integration group, then required source-stable regression and freeze. No GUI/app control, original application, mutable REA database, deployment or shared state was touched. Retained native source qualification was read in `source/docs/re/moproj2mst/s/spec.md` sections 1 and 2 (native `0x14005a8a0`, writer `0x140896030`); no new runtime observation was acquired.

Released file hashes:

- Production: `35e58afda87b9beab10c39b74197dd6a0bde0c99627351a7e8db01df2212f3ab`
- Test: `5c965f9622c72e1970ea1ebdf9f7a466db2ade6584519741a6fe42549934410e`

Both exclusively assigned source files are released to root for integration/review. Implementation is complete; compiled/runtime acceptance remains pending root integration. Token measurement unavailable.
