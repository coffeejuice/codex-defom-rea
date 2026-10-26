# Resume 223: ObjectKey incarnation contract

## Purpose

Define the invalidation boundary needed before viewer/editor consumers can replace retained `LLOBJ_DATA*` aliases with opaque `ObjectKey` values. This is a read-only design note. CP222's implemented `StepIdentity` batch remains observation-only; it does not authorize dereference or make legacy public storage safe.

## Evidence reviewed

- `analysis/resume222-editor-closure-architecture.md` (CP222): proposed private model storage, identity contract, dependency sequence, and distinct source-closure/original-witness gates.
- `analysis/resume222-step-identity-review.md` and `analysis/resume222-step-identity-implementation.md`: current `StepIdentity` behavior and explicit legacy bypasses.
- `analysis/resume222-editor-alias-ledger.md`, `resume222-loader-alias-ledger.md`, `resume222-viewer-alias-ledger.md`, `resume222-property-alias-ledger.md`, and `resume222-save-alias-ledger.md`: bounded raw-alias inventories and intended owner-operation destinations.
- Current CP222 focus receipt: `analysis/cp222-step-identity-focus-build.json`.

The alias ledgers are bounded inventories, not proof that every production consumer or native callee has been found. CP222 source/tests qualify only the implemented step observation path; no ObjectKey implementation, build, test, or original execution is established by those receipts.

## Contract: what an ObjectKey names

An `ObjectKey` is an opaque, copyable identity value. Its equality identity must include the owning model control-block identity, the current step/storage generation, and a per-object incarnation. It may retain a native object number or sparse ordinal as a lookup hint, but that number/ordinal alone is never identity. Construction is private to the owning model/access implementation. It has no pointer/address conversion, native reference, generic resolver, or API that returns `LLOBJ_DATA*`.

A consuming named model operation checks, in order, the owner, current storage generation, and object incarnation against live owner metadata; only after admission may it resolve a local native pointer for the finite synchronous operation. Stale or foreign keys must fail before native dereference and before the operation's first effect. Results that outlive the operation are copied values. No callback signature may make the pointer capturable by a caller.

## Required invalidation events

| Event | Invalidation point and scope | Required behavior |
|---|---|---|
| Model construction | Establish a fresh owner identity and initial storage generation; issue no key until an actual object binding exists. | A model allocated at a recycled address cannot accept keys from the prior model. |
| Step reset/clear (`step_clear`) | Advance the storage generation immediately before the first destructive reset effect, following CP222's ordering rule. | All keys from the prior step generation become stale, including when the native aggregate/object happens to retain the same address. Keep the new generation if later reset work throws. Preserve null-step no-op behavior. |
| Whole-step replacement or KEY/DB reload | Advance the storage generation at the first destructive effect that can invalidate old object storage, before releasing, clearing, or replacing that storage. If the loader's first effect is staged/non-destructive, invalidate immediately before the first actual destructive operation, not when parsing begins. | Every key for the previous step is stale even if allocator reuse produces identical object addresses/numbers. Preserve loader order and failure prefix; do not roll back committed invalidation. |
| Object removal/destruction | Retire that object's incarnation immediately before its native object is removed/destroyed. | Its key is stale before destruction. Other object keys may remain current only if the operation preserves their storage and the native behavior requires that stability. |
| Object recreation, including same native number/slot | Assign a fresh incarnation before publishing the created object to later converter operations. | The recreated object's key differs from the retired key even when number, ordinal, or address repeats. |
| Bulk object-list replacement/clear | Retire every affected incarnation before the first operation that invalidates the old list/object storage. | No old list member key is accepted after replacement. If the owning step generation is also replaced, step-generation invalidation subsumes these object retirements. |
| Model destruction | Retire the owner observation at destructor body entry, consistent with CP222's step token contract. | All keys fail currentness without keeping the model/native aggregate alive. |
| Numeric step selection/update or read-only query | No invalidation solely because the numeric simulation step changes or a query runs. | Do not conflate frame/result selection with object storage incarnation. Rebind only at a source-qualified binding site when a different object is actually selected. |
| Ordinary in-place field mutation | No invalidation solely because an object field changes, unless that operation destroys/replaces the object storage. | Keys identify incarnation, not a snapshot of mutable contents. Named commands still perform current admission. |

A registry must not use wraparound to revive an old incarnation. If an incarnation or storage generation is exhausted, reject before its corresponding destructive effect. Do not add a production-visible mutation hook solely to force this branch in tests.

Reader/database/geometry identities remain separate dimensions. A model generation cannot replace `DbHandle` owner, reader incarnation, directory receipt/version, frame identity, or sealed evidence receipt checks. Historical address tokens in geometry reports remain diagnostic values only and never become key material or later dereference authority.

## Smallest first ObjectKey implementation batch

The smallest useful batch is a model-owned identity registry and lifecycle transitions, without migrating any consumer to raw native access:

1. Add `ObjectKey` with private construction and value-only equality. Bind it to the existing weak model lifetime identity, step generation, stable object lookup discriminator, and monotonically fresh per-object incarnation. Keep step storage/getters unchanged only as explicitly transitional legacy surface; do not add a key-to-pointer resolver or pointer-bearing callback.
2. Add private issue/retire/retire-all metadata operations used only by `GuiModel` lifecycle code. Issue a key only for an object that has been established in the model's current live step. Retire keys at the event boundaries above. Reset/reload generation changes invalidate the whole key namespace without walking possibly-destroying native objects.
3. Expose only non-dereferencing observations needed to test identity (`ObjectKey` equality/currentness against its owning model, or a model-issued key paired with an admitted value query that returns copied data). Do not allow callers to use currentness as admission authority for a later unrelated operation.
4. Keep this batch confined to the model header/implementation, a private identity helper if required, and one focused Testkit file plus root-owned registration. Loader/viewer/editor migrations wait for the API/event matrix to freeze; only root assigns the test and integrates/builds.

If a source audit shows that exact create/remove/reload boundaries cannot yet be instrumented without altering native order or silently missing a mutation route, split the first batch further: land key value/registry semantics with a pure private transition helper and test it, but do not claim live-model incarnation coverage until every relevant owner mutation route is instrumented. The still-public `step` permits direct replacement/reset to bypass metadata, as CP222 already documents; therefore no new key observation can certify storage after that legacy bypass.

## Focused tests for that batch

Use Testkit `CHECK_*`/`REQUIRE` checks, not `assert`. The first focused test set should establish:

- Ordinary callers cannot construct a bound key, convert one to an address/pointer, or obtain a native reference/resolver through it; default/inert keys cannot match issued keys.
- Same owner + same live object binding yields equal keys; foreign model keys are refused.
- Reset invalidates a key despite unchanged native step/object address and preserves null-step no-op semantics.
- Object removal then recreation with the same number/ordinal (and, if a controlled allocator fixture can establish it without production hooks, same address) yields a distinct key; the removed key is refused before effects.
- Bulk reload/replacement invalidates every old key even if the loader fails after the first destructive effect; the advanced generation is not rolled back.
- Destroyed owner and same-address reconstructed model do not accept prior keys, while retaining a key does not extend model lifetime.
- Numeric simulation-step changes and read-only queries do not spuriously reissue/rebind an object key.
- Sparse object positions remain distinct from native object-number lookup; do not collapse the two lookup modes.

These tests qualify identity metadata only. They do not qualify all loaders, every remove/recreate route, viewer retention, cache1 admission, original behavior, or whole-program alias closure. Subsequent loader and viewer batches need route-specific tests for call/effect order, failure prefix, stale key refusal before dereference, and preserved output/state behavior.

## Original-evidence limits and gates

CP203/214/218 and CP222 reports support the source-side ownership problem and the proposed identity rule. The current CP222 review identifies a test that deliberately demonstrates public `step.reset()` can leave the CP222 token current; this confirms that step identity is not yet a live-storage certificate under legacy mutation. Existing static native instruction/call evidence can preserve the ordering of known reset/loader effects, but does not prove an original execution performed a particular ObjectKey event or that an original key-like identity existed.

No ObjectKey semantics can be attributed to the original application. Any report of a reconstructed DEF_PRE_64 instance must separately state its verified version and distinguish source, build, and deployed versions; this design has no such build/deployment. Source closure still requires the full per-symbol/call-site alias ledger, compile-negative API checks, focused and integration tests, and a source-stable candidate freeze. R300 selected-length/cache1 continuation additionally remains blocked on the independent same-occurrence original witness described in CP222. GUI-control work remains behind the existing verified-release lifecycle gate; this design neither starts nor authorizes a GUI session.

## Scope

Read-only design work based on the named CP222 architecture/review and model, engine/editor, loader, viewer, property, and Save alias ledgers. No production or test files, builds, executables, deployments, original projects/processes, GUI control, or mutable REA state were changed. The only intended write is this report.
