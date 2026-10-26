# CP103: minimal ApplicationState owner and Close cursor

Normal Close needs one shared application-state owner, with source-ordered writers and a current-main link, rather than more supplied busy observations. Keep its lifetime cursor separate from each Open/Save/Close invocation. Pure stores and argument production are implementable now as bounded transitions; constructor reach, current Qt stack state and modal dispatch still require actual producers. This design does not authorize public Close success or a new helper-only implementation lane.

Read-only design; only this report was written. No REA retry/session, GUI, original execution, source edit or build occurred. Primary qualification: `resume101-close-live-owner-chain.md`, SHA-256 `8d893447af81cedd885b112609f7a7de709c1e2ab9b9f54a456746c4e2c7f793`, and `resume101-close-pe-writer-chain.json`, SHA-256 `38ae40f0a8d56641940bfd3af1e2b31ab7ef43acb484f420a10b8f930534c9a1`. Immutable original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. CP101's REA session remains closed; no new REA Evidence ID exists. No reconstructed executable/deployment was inspected or changed.

## One actual owner, with explicit knowledge

Introduce an internal `engine::ApplicationState` in the **fresh-application runtime root** used by the real conversion driver and Session. It survives Project/model teardown; dropping/replacing a Project must not recreate it. For the selected fresh-original, one-project scope, one runtime can correspond to one conversion invocation. It must be explicitly identified as that fresh-application profile, not silently reset whenever a Session/Project appears or installed as a new C++ process-static singleton. Future multi-project support must reuse the application owner instead of borrowing fresh defaults again.

The Session retains/borrows this same owner and the same existing ModelManager; Open, explicit Save, nested Save and Close receive it internally. `SessionNotificationState` stays the separate existing notifier143d76700. ApplicationState represents143d766b8 plus its associated busy-warning counter143d91230; it does not own or copy the notifier, main window, Project, settings/MoInputMode, remesh state or status widget.

Minimum state:

| Component | Representation / rule |
| --- | --- |
| busy+9, fields+A/+B | Separate known/unknown raw bytes; busy reads nonzero, setters write source byte values. Do not normalize every stored byte into a policy Boolean. |
| warning counter143d91230 | Known/unknown DWORD, separate from singleton offsets. True setter preserves it even when unknown. False setter writes known0 before main lookup. |
| process type+0xc | Known/unknown DWORD for the actual application owner. Constructor reset and mode-0 refresh are separate transitions; it is not MoInputMode or a guessed constant5. |
| current main+0 | Unknown / known null / canonical primary target. A nonnull target carries the canonical window allocation generation/role and a nonowning link. Empty/expired C++ weak metadata is unavailable, not native null. |
| constructor byte+18 | Retain its source reset if implementing the initializer prefix; no broader semantic meaning or late reset is inferred. |
| owner scope | C++ progress/lifetime identity distinct from native construction/alive/dead phases. No historical dereferenceable pointer accessor. |

The canonical primary-window root owns its actual window record/allocation and main+50 manager field. ApplicationState's native raw pointer does **not** extend the main lifetime. main+270, saved+274 and main+480 stack state belong to that window, not ApplicationState. main+268 is another stack owner. Bind all primary aliases to the existing canonical role/allocation generation; do not substitute MainViewState or create another window solely to satisfy Close.

Current `src/doe/application_state_projection.hpp` owns a standalone observed process-type/DBLIST prototype. It is not a production state provider. Before actual process-type integration, turn that interface into a facade over this same state or migrate its one storage location; do not maintain two independently mutable +0xc fields. Its unresolved DBLIST+1c remains unknown and is not reset as a side effect of this Close work.

## Lifetime cursor and per-invocation cursors

Application/main lifetime cursor, minimal meaningful boundaries:

```text
AwaitNativeMemberConstruction
  -> ApplicationInitializerEntry -> BeforeInitializerTail
  -> AwaitMainPrepublicationEffects -> MainPublished
  -> BeforeMain50Initialization -> AwaitConcreteManagerFactory
  -> CommandRootIdentityAvailable
  -> MainTeardownBefore118Free -> BeforeGlobalMainClear -> MainCleared
```

These are reached source boundaries, not Boolean permissions set by tests or qualification strings. CommandRootIdentityAvailable does not mean all current fields are known or every callback is implemented. Original QFlags/QString member construction precedes2c7f80; QCoreApplication/c993d0 and Qt/base effects precede c9957b main publication. A C++ ApplicationState constructor must not self-certify those phases. Private source producers advance the cursor; missing predecessors retain their guard. Early known-null main cannot be reused as the late Close lookup after unimplemented publication/Open effects.

Each real command has its own cursor, source occurrence, caller-main identity, weak application/manager generation and consumed locals. Close needs only these early phases initially:

```text
ReadBusyByte -> [ReadCurrentMain -> ReadStack480 -> optional Read270]
  -> BusyWarningEffects OR ScanActualModelCanClose
  -> [CloseQuestion -> NestedSave -> answer remap/callbacks]
  -> TrueBusyStores -> BorrowedTreeArguments -> TreeNotifierPrefix
  -> BeforeKeyQString OR suppressed return/next refresh boundary
```

Do not use one global Open/Save/Close enum or a blanket “Close active” exclusion: native Close's Yes branch nests Save, and source reentry must retain its actual semantics. A per-call cursor preserves its latched busy result/answer while another call changes shared fields. An epoch can diagnose changes; it must not roll them back, globally reject every later write, or replace fresh native reads with registration-time snapshots. Unrecovered reentrant paths stop at their source effects rather than receiving a new synchronization policy.

## Exact transitions implementable from source now

“Implementable” here means the primitive's body/order is closed **once its actual entry is reached**. It does not mean its current public producer is complete.

| Transition | Exact implementation and first remaining guard |
| --- | --- |
|2c7f80 initializer prefix | After actual member-prefix return: process+0xc=0 (2cb760); main+0=null (2c81f0); byte+18=0 (2cb900); busy+9=0; A=1; B=1. All six writes are source-closed, callback-free and ordered. Do not change counter. Stop before subsequent initializer/member work; do not publish a completed native constructor. |
|c9957b publication | After actual c99530 predecessors: assign that **same** primary this to current main; then main+50=null atc99588. Pointer assignment is closed; constructor entry/Qt predecessors and later factory01344c are separate. |
| true2ca9f0 | At the actual command's setter: busy+9=1; its show=false busy query short-circuits on the just-written byte; A=0 then B=0. Preserve counter, including unknown knowledge. No main query is needed. Stop at Tree argument/callee effects as they become unavailable. |
| false2ca9f0 | Write busy+9=0 and counter=0 **before** main lookup. Unknown main then guards with those writes retained and prior A/B intact. Current known-null main selects unbusy. Nonnull requires the actual window/stack; index2 is busy without reading+270, otherwise busy iff+270==1. Then A/B are both !busy, followed by the matching Tree Boolean. |
| Tree arguments | Construct false data byte, static `bool` type and nine empty borrowed argument views; QGenericArgument is two pointer stores. Keep data lifetime through synchronous delivery or the logical suspended frame; no escaped stack pointer or owner allocation is implied. |
| notifier entry | Use the existing actual Session notifier. Current suppression true returns without flag90 store; false writes flag90=false then guards before key QString2cf41f. CP88 implements this body. No empty Tree population is qualified. |
| main+270 stores | The0/1/2/restore-from+274 stores are known, but execute only after each actual stack/resource predecessor. In32600, state already0 still has a later tree effect, so no whole-function no-op shortcut. |
| process-type refresh |2cb780 DWORD store is closed; selection0/1/5 must follow the actual constructor or013d40's earlier process/UI work and current Project/open-success read. The existing observed-Boolean prototype is insufficient authority. |
|c99245 main clear | After derived/base teardown and main+118 free, write current main=null. It is an explicit raw-pointer clear, not weak expiry, QPointer invalidation or Project Close cleanup. |

Owner/scope validity is checked before effects. **Value** prerequisites are checked at their actual read, not as a blanket “all state known” preflight. In particular, requiring current-main knowledge before false's busy/counter stores or requiring counter knowledge before true would move the guard earlier than native code.

## What remains recoverable statically, and what needs an occurrence witness

Still-recoverable static producer work includes native member/Qt/base constructor phases, the actual main+50 concrete manager factory, main stack/page construction and its setters, and the reached bodies of main+270 writers. These do not intrinsically require a runtime sample for each store, but must operate on real canonical owners in the reconstructed command path. CP101 located their identities and mutation sites; it did not implement their effects.

Live Close state can come either from a complete executable source producer/writer chain on those owners, or a source-qualified same-instance original occurrence trace. A label or copied structure is neither. Current stack index/main mode cannot be inferred from startup, excluding explicit simulation running, or the final Save reset: native Save status, Tree delivery, processEvents and other synchronous receivers can intervene.

Modal dispatch is the remaining runtime-sensitive blocker in the successful Project route. The shipped-style unchanged-focus profile statically selects default Yes; it does not select due timer IDs, zero delivery, callback state or nested Save completion. Keep the real Close question schedule, nested Save's own dispatch/warning intervals and integer-result remap in the same command cursor. Reusing the earlier explicit Save result would omit a second real invocation. None of CP101's failed REA queries supplies that witness, and no new capture is authorized here.

Later null refresh, nonempty manager/Project/Report/OPI destruction and status effects remain their own guards. Source-proven primitive stores must not be promoted to full accepted Close or Session-destruction authority.

## Minimal source boundaries and integration test

When root authorizes source work, the smallest ownership change is NEW `include/m2m/engine/application_state.hpp`, `src/engine/application_state.cpp`, and a focused test, plus **reserved** conversion_session.hpp/.cpp binding to one actual runtime owner. Keep the C ABI unchanged. A field-only sidecar with no command read/write callsites would leave the exact problem unresolved.

The subsequent shared integration must reserve core/convert.cpp and existing Close entry/busy/false/notifier files. Replace production supplied copies with reads/writes of this owner; retain legacy declared-fixture helpers as diagnostics if needed. CP88's current prerequisite accepts the copied CloseBusyGlobalOwner, so production needs a typed same-owner reached-setter/caller cursor or a shared internal primitive; do not manufacture that copied prerequisite. The caller cursor records completed argument production separately from CP88's deliberately non-authoritative result flags. No arbitrary “mark constructor/modal/Close complete” API is acceptable.

The public seam is the existing run's same Session, after the selected complete Save invocation/effects and before adapter destruction. It is ready only when its actual application/main and can-close producers can reach it. Adding an always-unavailable receipt or invoking CP88 directly from cleanup is not public behavior advancement. Initial ownership code may be staged behind these gates; public insertion/activation remains root-owned.

Tests should first verify Unknown is never null/false; exact constructor-prefix writes and untouched counter; true with unknown counter/main; false partial stores before missing main; index2 short-circuit versus non2 reading+270; generation/address-reuse refusal; and main clear only at native teardown. Then exercise **real Open/Save/Close command callsites on one owner**, not direct helper calls: explicit and nested Save share state, latched busy rejection survives warning mutations, callbacks affect later fresh reads, actual notifier flag90 changes before its QString guard, and prior file effects survive refusal. Preserve public statuses until that integration is deliberately introduced; declared schedules remain tests of reconstruction ordering, not original accepted timing.

The next useful task is actual owner plus source-producer/callsite binding, with the first remaining constructor/current-state guard explicit. It is not another detached observation class, and no public Close release follows from this design alone.
