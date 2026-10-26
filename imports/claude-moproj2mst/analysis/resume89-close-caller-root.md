# CP89: Tree caller arguments close; accepted public root does not

The native caller's QGenericArgument production is now source-closed: it creates one borrowed bool(false) argument and nine null arguments using a two-pointer leaf constructor, then synchronously calls the notifier. This removes a genuine local provider uncertainty from CP88. It does **not** authorize connecting CP88 to the public conversion path: that path has no source-qualified normal-Close invocation, shared native busy/main state producer or completed process5 can-close question on the same opened Project. No helper-only implementation is proposed.

Only this report was written. No source/build/test/GUI/original execution/mutable REA activity occurred. Evidence: current source, retained CP57/58/59/60/77/78/80/88 contracts, bounded offline PE/import/export/disassembly. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. No reconstructed executable/deployment was inspected or changed.

## Caller argument production, exactly

Original IAT141e04168 resolves through QtCore import OriginalFirstThunk RVA350c768, FirstThunk RVA1e03b60, zero-based index193, import-name RVA351659a, to `??0QGenericArgument@@QEAA@PEBDPEBX@Z`. The QtCore export has RVA17f0 and, using the DLL's actual ImageBase67000000, VA **670017f0**. Do not confuse this with671017f0.

The constructor is precisely670017f0..670017fb, end exclusive:

```text
670017f0  [this+0] = r8     borrowed data pointer
670017f3  [this+8] = rdx    borrowed type-name pointer
670017f7  rax = this
670017fa  return
```

There is no allocation, refcount, callback, argument conversion, event pump or transitive provider in this constructor. It does not copy the pointed Boolean or own either pointer. The wrapper025080..0250b2 only saves/forwards its three arguments to that imported constructor at0250a2 and returns its own this address.

At2caa63 the true busy setter writes its local Boolean false. It passes the local byte address and static type string1422d306c=`bool` to025080 at2caa82, then copies the16-byte result into its final argument slot. The calls at2caaba,2caadc,2cab01,2cab26,2cab4b,2cab70,2cab95,2cabba and2cabdf each pass rdx=0,r8=0 to QGenericArgument, producing **nine empty null/null arguments**. The subsequent fixed16-byte copies pass these ten borrowed views to2cf3c0.

At2cac86 the key is static1422d3078=`2signalTreeEnableSelection()`,2cac8d selects notifier singleton143d76700, and2cac94 calls2cf3c0 synchronously. The local Boolean, fixed arguments and type/key strings remain valid during that call. On return the caller jumps2cac99 ->2caef8 and returns; there is no destructor/provider call for these trivial argument views in that epilogue. The true-Boolean later branch uses the same constructor semantics but is a separate occurrence.

A faithful synchronous caller can therefore build the false byte plus ten borrowed views on its own call frame without an allocation or invented Qt event. A resumed staged continuation must not retain pointers into a departed call frame: recreate the views around the call, or use a same-attempt value owner whose lifetime spans it. Do not cache a raw address to a diagnostic fixture Boolean or turn QGenericArgument into a shared owner. No generic arbitrary Qt method implementation is required to establish this narrow constructor.

CP88's guard before2cf41f remains correct for the **unsuppressed callee**: key QString construction happens after the actual flag90 store and is not QGenericArgument construction. Resolving the borrowed argument constructor does not resolve that later allocating/refcounted string provider, map lookup, Tree receiver population or callbacks.

## Actual root source order and missing same-instance producer

Native0299a0 requires this order before the argument block:

1.0299cb ->2ca920(show=true) reads global state143d766b8+9. Nonzero is busy; zero calls the current main getter and, for nonnull main, virtual+300. A true result enters busy-warning effects/modal before public false return. No immediate busy-false shortcut is source-qualified.
2.Only unbusy reaches0299e6 ->c9b8c0/c7a100, scanning the actual manager's owning children through can-close virtual+68. Main+50 concrete-manager identity and selected child ownership are separate from the pure can-close predicate.
3.For actual Mo child+19c0=null, the child permits closure without question. For the successful .moproj lifecycle, the same field is nonnull and process5 selects the Close question; the shipped qualified default is Yes, whose branch performs **nested Save**, with source-qualified modal dispatch and warning handling. Prior explicit Save is not a substitute for that new Save invocation.
4.Only all-allowed reaches029a54 ->2ca9f0(true), writing busy+9 and+A/+B on its shared process owner before producing the false Tree arguments and entering the actual notifier.
5.Only after actual setter return does029a61 run null Tree/Property/Scene refresh, then029a76 manager cleanup. CP88 does not skip these or the later Report/OPI/native lifetime phases.

Current candidate identities close some links: Session owns the actual ModelManager, prepareProjectModel creates/registers the actual Mo DbHandle, commitProjectModelConstruction assigns its moProject to the actual Project, and commitProjectModelSelection selects it. `sessionNotificationStateOwner()` is the shared notifier projection that CP88 mutates. These are actual candidate owners, not evidence of the missing native root stages.

The first missing producer is **current mutable native busy/main state for this same Close attempt**, spanning intervening Open/Save/notifications. `CloseEntryBusyObservations` carries supplied facts only; `CloseBusyGlobalOwner` owns an explicitly isolated copy of supplied bytes. Neither is a Session-owned production binding to143d766b8. The current Session has no actual native-main field/current virtual+300 result producer; MainViewState and widget-runtime startup observations do not supply main+480's current QStackedWidget index or main+270. CP77's factory/backlink projection and CP80's candidate constructor receipt explicitly retain native main+50 publication/readiness guards.

The exact evidence needed is either (a) a source-closed actual process/main owner with construction and every relevant intervening writer/effect represented, whose current fields are read by both entry and setter, or (b) a valid same-instance source occurrence witness covering those original fields/current main at this entry, bound to the actual manager/Project and consumed with its actual effect sequence. A fresh-process label, PE-zero default reused after Save, engine mutex, current Project, prior saved flag or generic C++ owner token is insufficient. A nonblank qualification is not capture authentication.

Even after that first producer, successful Open retains the process5 same-Project modal/nested-Save dependency. The default-answer policy qualifies the selected button for the recovered style/focus; it does not qualify zero timer delivery, receiver effects or completion of nested Save. Its result must flow through the original answer remap before manager acceptance. A null moProject failure seam is source-closed but cannot be substituted for a successfully opened/committed Project.

## Why no public caller is presently source-ready

`src/core/convert.cpp::run` constructs its Session and, for WRITE_SAVE, prepares/commits/selects the model and executes Open/Save. Its tail collects/applies existing file effects, sets the result message and exits the scope. There is **no call to native normal Close or a candidate normal-Close coordinator**. Session destruction then performs expressly labeled adapter cleanup: releaseProjectStorage, receiver/dispatcher teardown and owner-scope release. It is not0299a0 and cannot be reclassified as accepted Close.

The existing C ABI is conversion-shaped, with no separate Close export. Internal ConversionCallServices currently has only ProjectOutputFactory; ConversionDetails has Save/XML observations and no Close producer. OriginalSettings carries settings, not same-attempt main/busy/modal effect authority. Therefore there is no existing public input or actual producer that can honestly populate the missing state and advance CP58 ->true setter ->CP88. Supplying fixed unbusy facts inside convert.cpp would invent that producer. Calling CP88 from Scope cleanup would reverse the source order and execute it after/beside unrelated adapter teardown. Calling another isolated wrapper from tests would not close this public gap.

The eventual public integration location is the same live `run` Session **after the selected original Save invocation and its required file/stream effects return, before Session destruction**, with its actual Project/ModelManager/notifier unchanged. The exact point must account for the selected native Save branch and deferred adapter effects; it cannot be selected merely from the last line of run. A new normal-Close coordinator there must consume actual busy/main and can-close producers and share process state with its setter. It may then construct the now-qualified stack arguments and invoke CP88 using the same notifier. It must stop at the first next effect (e.g.2cf41f) while retaining committed state and prior file writes. This is a conditional integration design, not a currently executable task authorization.

Do not count an always-unavailable public diagnostic, a new receipt field, or another helper-only argument adapter as advancement of original Close behavior. The next productive work is the missing actual owner/writer/root witness, not more copies of declared facts.

## Focused integrated validation once the producer is closed

Drive the actual WRITE_SAVE conversion entry or the existing source-bound run path with one real opened/committed Project; tests must not call CP88 directly to manufacture the observation. Verify the same Session/manager/model/notifier identities through Open, explicit Save, busy entry, modal/nested Save and true setter. With a qualified accepted path, actual suppression=false must expose flag90=false before the subsequent key-string refusal, retaining already committed Save/file effects without rollback. A suppressed case must preserve flag90 and reach the next caller phase only after the real argument block and setter return.

Negative integration cases must stop **before busy stores** for unavailable main/busy state, and before acceptance for unresolved process5 modal effects; unavailable data is not zero/false/no-delivery. A prepared-but-uncommitted null-Project failure case is separate from successful Open. Verify the existing C ABI status/message behavior is intentionally defined for each guarded partial path before changing convert.cpp. Held-out native one-project acceptance remains distinct from a declared fixture schedule.

No source files or helper implementation are proposed in this lane until those owner/producer conditions are met.

## Byte receipts

DUMPBIN14.44.35228.0, section-aware PE import/export lookup, exact **end-exclusive** byte ranges:

| Image / region | SHA-256 |
| --- | --- |
| DEF_GUI025080..0250b2 typed forwarding wrapper | `84afcbe4515c24193732264e38a666409c8be07224991f2381bdd915e38d144c` |
| QtCore670017f0..670017fb constructor, excluding padding | `65d3a94c25b7067d6dd42c5ab3e7694104797881953021e8a64e5de2435c60f2` |
| DEF_GUI2caa63..2cac99 full false argument block and call | `82cdb65b87b7c86c06d03817d397a48fa9db0c04abe26586758c269a34605ebb` |
| DEF_GUI0299a0..029a59 root through true setter call | `a3dbb6f4fcc95d4363af9c6d32c52769129f9bfafdef572ffbcc0390c7430744` |

Retained source qualifications: `resume57-close-staged-owner.md`, `resume58-close-entry-prefix.md`, `resume59-close-next.md`, `resume60-close-tail-next.md`, `resume77-close-main-wrapper.md`, `resume80-session-manager-producer.md`, `resume88-close-tree-prefix-implementation.md`. Current source audit: core/convert.cpp, core/conversion_call_services.hpp, convert_details.hpp, engine/conversion_session.hpp/.cpp, close_entry_prefix.hpp/.cpp, close_busy_prefix.hpp/.cpp and close_dialog_answer.cpp. No source, GUI or mutable REA state changed.
