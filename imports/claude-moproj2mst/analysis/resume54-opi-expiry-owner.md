# Checkpoint54 OPI expiry and model-owner prerequisite

The guarded OpClass current and Props-first helper correctly keep OPI identity live before native QObject destruction. The next missing owner is native OPI+160: a guarded, lazily allocated **SfMoItemModel**, distinct from both the engine GuiModel cache at+38 and the outer operation manager. Add a qualified nullable cache/owner slot before implementing any native expiry. The first safe test is a non-allocating lookup of the source-initialized null+160 slot, leaving current live and the constructor path guarded.

This report is the only write. No source/build/test/GUI/mutable REA operation occurred. Static reads used retained original PE, fixed vtable/RTTI/IAT records and bounded DUMPBIN, not original execution. Reference DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Audit against current source

- Each Operation owns one stable OperationInfoIdentity. Actual OpClass.current is CurrentOperationBinding; writer/loader temporary binds use metadata snapshots. Composite destruction yields AdapterIdentityLifetime; explicit native expiry remains separate.
- OperationInfoCleanupProgress implements only native0512c0 null-Props skip. Nonnull Props remains guarded before deleting virtual+18; its pointer, ownership count and OPI identity are unchanged. After the null skip it stops at engine-model cleanup; it does not skip later state because modern containers look empty.
- Operation.engineModel is a shared allocation projection of native guarded+38. Operation has no+160 model state. The old comment groups both guards as unmodelled; the engine cache allocation exists, but its complete native guard/owner lifecycle remains partial.
- No real native OPI cleanup/QObject boundary is reached by the public converter. The current explicit commit seam is not a completed-prefix certificate. Public Close must remain guarded.

Native order remains Props0512c0 -> guarded engine cache05f980 -> object/position cleanup061180/061f70 -> recovery wrappers+20/+28 -> guarded item-model+160 -> OPI member destructors -> QObject clearGuards -> destroyed recipients -> outer item diagName/base. Implicit C++ Operation cleanup still differs from that order.

## Exact+160 identity and lifecycle

Newly checked bounded native evidence:

1. `054f40(OPI, create)` first tests its byte create flag. If false it skips allocation and returns guarded+160 through c1aef0 at054fd2. If true, it tests current guarded+160; nonnull also skips allocation. Only true+null allocates0x50 at054f75..7a, calls0c4890 at054f91, and assigns the returned pointer into the same guard through cbc920 at054fc1. Allocation-return-null is assigned as null; no invented safe error status is recovered here.
2. `0c4890` calls base d3d590, then installs table14215e7b8. Direct x64 RTTI resolution gives `.?AVSfMoItemModel@@` through complete-object-locator1430d68e8. Table+18 is deleting wrapper0c8180 ->0c48d0 ->base d3d680. This is a different object from SfMoOprManager and SfStepData/GuiModel.
3. Base constructor d3d590 calls QStandardItemModel(parent=null) at d3d5a9, initializes its+10 helper, allocates a selection model, stores that owned pointer at+48 (d3d62d), registers a connection with raw type3, and stores int+40=0 at d3d65c. The exact helper/connection/Qt construction payloads remain separate dependencies.
4. Base destructor d3d680 deletes nonnull owned selection-model+48 through virtual+18 at d3d6df, then stores+48=null at d3d6f7; subsequent helper/base cleanup continues. Do not represent nonnull SfMoItemModel destruction as deleting an empty vector or forgetting a guard.
5. In OPI cleanup050690, current+160 is read at05079e. Nonnull target is deleted via its virtual+18 at0507e5, then cbc920(null) at050809. Later OPI member destruction removes the+160 guard registration at050478. Deleting the target, assigning guard null and destroying the guard registration are separate boundaries.
6. Native defaults050550 explicitly assigns+160 null through cbc920 at05067b. This is a source initialization/reset event, not evidence that every arbitrary modern Operation instance reached it.

Important correction: **class change clears model contents, not the+160 cache identity**. `050c80` returns early when class key is equal. On a changed key it assigns the class string, calls054f40(false) at050cd4, and for nonnull result invokes IAT141e0b600 at050ceb, verified `QStandardItemModel::clear()`. There is no+160 delete or null assignment in that body. Preserve an allocated model/guard across class changes while performing actual clear effects. The historical spec phrase "cleared on class change" must not be interpreted as owner destruction.

## Exact model-owner additions to plan

The following are conceptual field names; root controls final interfaces. Avoid raw nullable pointer defaults that turn unavailable knowledge into source-proven null.

| Addition/location | Native meaning and required invariant |
| --- | --- |
| `Operation::cachedItemModel160` typed slot | Qualified guard result for native+160: Unknown, KnownNull, or KnownLive. A typed nullable binding/optional observation can represent this without a generic bool completion claim. |
| `MoItemModelBinding::guard` | Nonowning native QObject-style target identity, allowing external target destruction to null the observation. Rebinding must not create a second logical model for the same native pointer. |
| `MoItemModelBinding::allocationOwner` | Adapter storage retention for the same lazily allocated SfMoItemModel, with explicit native deletion stage independent of external shared diagnostic leases. It must not be a duplicate QObject/model. |
| `MoItemModelState::selectionModelOwner48` | Actual owned selection-model identity deleted before helper/base cleanup. Absence must be qualified; do not infer null from an unimplemented constructor. |
| `MoItemModelState::helper10` and int+40 | Construction/destruction state of the real helper and zero-initialized native int; retain constructor stage/qualification while unresolved. Do not call the whole model constructed just from an address binding. |
| Existing+38 cache plus a source-qualified native guard/owner observation | Reuse the current GuiModel identity/allocation; add missing guard/native-lifetime qualification rather than another engine model. Shared allocation liveness alone does not establish native receiver liveness. |
| OPI cleanup progress stages | Later distinguish engine-cache deletion, object/position payloads, recovery-wrapper deletion, item-model deletion, guard deregistration, OPI members, QObject expiry and admitted notifications. Never skip from Props-null to completed expiry. |

Native ownership for+38 has a specific provenance trap: `engine::opModel` first asks class virtual+e0. Cycle can return the first member's existing model. Such a borrowed classModel result must NOT be adopted as this OPI's owned+38 cache or deleted twice. Only the actual null-cache fallback branch assigning `op.engineModel = createModel(...)` corresponds to this OPI cache owner. `05f980` deletes the guarded cache, not every model returned by opModel.

A future unified owner record may group+38 and+160 metadata, but it must preserve these distinct types and source provenance. Keep current shared allocation APIs working until root audits all callers. Do not enable model deletion merely from reference counts or a receipt string.

## First safe isolated test/increment

Implement a typed+160 cache slot with an initial Unknown state and a source-specific defaults commit, bound to the actual OperationInfoIdentity. The actual native constructor/default event can commit KnownNull; arbitrary new slot absence remains Unknown. Non-creating lookup corresponding to054f40(false) may return the qualified cached null without allocation. Creating lookup with KnownNull must stop before the unrecovered0c4890/Qt construction effects, retaining the same KnownNull cache and live OPI current guard.

The first test should:

1. Create actual Operation and OpClass current binding; verify current live.
2. Show Unknown cache lookup refuses rather than treating absent storage as null.
3. Commit only the declared native defaults+160=null initialization event (or use the actual `initOperation` call site after root wires it); non-creating lookup returns null and does not call any factory, clear rows, dispatch events or change current.
4. Request create=true; the unavailable constructor path refuses/stages at that boundary without installing a fake model or changing KnownNull.
5. Verify Props-first cleanup outcome and progress are unchanged, current still names this OPI, and no QObjectBoundary expiry is published.

This is the smallest source-closed owner addition. It does not test a populated model or native Close. Follow-on tests with explicitly qualified live model state must prove repeated get preserves identity, equal class key performs no clear, changed class key clears rows through the real effect while preserving cache identity, and destructor ordering deletes target before guard-null/deregistration. Those need actual model/selection/Qt owners and effects.

Suggested root-owned source boundaries when assigned: a new focused typed cache header/source/test; Operation owner field/default initialization sites; existing class-change path only after its effects are recoverable; later native OPI cleanup continuation. No need to change OpClass.current or the public Close gate for this first cache observation increment.

## Bounded reproducibility receipts

DUMPBIN `/disasm:bytes /range:<start>,<end>` was run against the retained hash-verified binary. Direct PE resolution checked table14215e7b8[-8] ->COL1430d68e8 ->type descriptor and table+18. Existing IAT inventory binds141e0b600 to QStandardItemModel::clear. Newly inspected byte ranges are hashed by their exact unwind records (end exclusive); an unwind region is not proof of transitive closure:

| Start..end | SHA-256 |
| --- | --- |
|140054f40..140054fdc|2d5dae26240126418419a455d9462f2f66444ac69afed9dfe7e1c9f9dddba297|
|1400c4890..1400c48bc|76a4572c591e44f69a13026fdf8f64acdc19838f4fab93a555621994493f198d|
|140050c80..140050cf6|ac30afb3e5ce5bb2ce71f377007a97a79bb3135aeb1e7c8699b46143a24fe71d|

No reconstructed executable build/deployment occurred in this lane.
