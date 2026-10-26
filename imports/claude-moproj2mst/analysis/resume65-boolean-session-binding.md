# CP65: actual Session binding for Boolean UpdateData

The CP64 synchronous suffix cannot yet be attached to an actual reached widget-area receiver in the current Session. The Session owns the notification state and diagram events, but it has no `SfWidgetArea`/`SfWidgetAreaImpl` component, no UpdateData receiver list and no separately lived current Boolean widget owner. Reusing a diagram receiver, MainView, class registry or model owner would invent an identity relationship. No source or test change is justified in this exclusive Boolean lane; CP64's isolated helper and public fresh-Open/PostPending guards remain unchanged.

This investigation adds this report and two bounded original-PE capture receipts with their two `.asm` files. It uses no original execution, GUI, mutable REA, build, CTest or test executable. Root owns integration/builds.

## Reached candidate paths and the exact missing ownership

| Candidate source boundary | What exists | What it does not establish |
| --- | --- | --- |
| `src/core/convert.cpp:182` | The real conversion constructs the default `DiagramEffectDriver`, then `ConversionSession` at183. Full Save stores `ctx.session`; narrower adapter profiles do not. | The default driver explicitly refuses fit/click effects. A timer registration or trace binding does not create a widget-area receiver. |
| `src/engine/conversion_session.cpp:144` | Session owns DiagramLifecycleEvents, a diagram receiver, shared `SessionNotificationState`, WidgetRuntime, MainView/ViewManager, class registry lease, ModelManager and Project storage. | No widget-area/impl allocation, registration or current-widget ownership member exists in its actual header/constructor. The diagram receiver is a distinct logical scheduler identity, not the native UpdateData receiver. |
| `ConversionSession::dispatchBooleanTraceAt`, line337 | Validates the actual trace owner and calls existing dispatchAt. | It certifies selected timer delivery admission, not widget-area receiver population. |
| `ConversionSession::modelUpdated`, line372 | Calls fit first, then resolves the current bound model/item and delegates click effects to the explicit driver. | It does not perform the whole native click receiver list or attach a Boolean editor. Default driver remains unsupported. |
| `ConversionSession::applyBooleanSelection`, line390 | Validates current subproject/item, resolves actual operation and calls `reselectBooleanObjectEditor` at404. | This existing bounded engine leaf directly applies/refills the class-owned Boolean snapshot. Its documented outer driver admission is not an actual Session widget-area owner, native receiver registration or notifier prefix witness. Calling CP64 here with made-up owner aliases would not close those gaps. |
| `src/engine/save_current.cpp:790` | The existing base Save projection calls `ob->updateDataInWidgetArea` directly, then propsLoad on true, otherwise propsToEngine; outer propsToEngine follows. | This compatibility projection does not traverse the Session notifier/area. It cannot provide native receiver/emission admission just because it reaches the same Apply hook. Shared Save is outside this lane. |
| `src/ops/boolean.cpp:63..77` | `BooleanClass` owns private value `BooleanObjectEditor editor_`; Fill and Apply delegate to the real loader hooks. | The value has only `shown`, Workpiece text and Cutter text (`mst/loader_l5.hpp:37`). It has no native widget lifetime, current-area guard, Qt parent, registration identity or replacement epoch. |
| `src/engine/boolean_editor.cpp:23` | Fill resets that private selection snapshot with `ed = BooleanObjectEditor{}`, then populates it. | Its containing C++ address/class-registry lifetime remains the same. This is not the separate old/new QWidget lifetime used by native replacement/deferred deletion. |
| `doe_session_notifications.hpp:47..60` | The actual shared notifier has suppression/flag90, empty-key tracking, task-factory and graph registrations, and a typed ViewManager registry. | The ViewManager map represents the four recovered ViewManager keys. There is no admitted `2signalUpdateDataInWidgetArea()` population. Missing storage must not be interpreted as an empty native receiver list. |
| `ConversionSession::beginEditor300Factory`, line237 | Retains a specific Forming300 staged factory under Session and binds its WidgetRuntime at the original factory prefix. | Its source explicitly guards replacement on missing widget-area leave/deferred-deletion order. It is not a Boolean editor or attached current-area owner and cannot be borrowed as one. |

The class registry uses `map<String,unique_ptr<OpClass>>` (`engine/mo_model_slots.hpp:19..28`) and the Session retains the registry across Project teardown. Aliasing that registry's shared control block to a Boolean class or private snapshot would extend the registry lifetime, not model current-widget replacement/destruction. CP64's exact control-block checks deliberately cannot authenticate such an invented native mapping.

## New original construction boundary

Immutable original `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. `analysis/resume65-boolean-area-owner-static.json` captures `140018460..1400184e3`; `analysis/resume65-boolean-area-caller-static.json` captures `140018790..140018ad5`, ends exclusive. Both include exact DUMPBIN commands, byte counts and hashes. These are unwind-region bounds, not full transitive callgraph proof.

The lazy helper `140018460` provides a concrete missing owner commit:

1. `140018477` reads its parent object's `+3c8`; nonnull skips allocation and returns the existing field.
2. Null allocates0x30 at `140018486`.
3. `140018498..1400184a2` passes r8b=1 and parent argument rdx=0 to actual area constructor `140d14d30`.
4. Only after the constructor returns does `1400184cb` store its result into parent `+3c8`. The allocation-failure path stores null. Do not replace that original branch with a newly invented failure policy.
5. `1400184d7` returns the cached field.

Selected caller `140018790` invokes this helper at `1400188af`, uses the area result in a Qt call at `1400188ba`, then invokes the helper again at `1400188c8` and uses the same cached result at `1400188df`. The second call does not require another area/impl registration when the first completed nonnull. Other constructor/caller effects in this body remain separate; this lane does not certify completion of all its widget operations.

Carrier discovery scanned encoded `E8 rel32` candidates in original executable file sections: one candidate to `140d14d30`, at `1400184a2`; two candidates to `140018460`, at `1400188af` and `1400188c8`. The subsequently retained disassembly independently confirms those instructions. This search only locates that instruction encoding, not all indirect/cross-module calls or all allocation paths. No inference about runtime execution count follows.

The retained area constructor `wave3-editor-area-constructor-140d14d30.asm` separately proves impl allocation/store to area `+28` and registration call `140d14ddc ->140d14e90`. CP63/64 bind the registered receiver to this impl, **not** to its parent object or the model. The actual UpdateData registration at `140d14f08` uses `area+28`, `2signalUpdateDataInWidgetArea()`, bare `slotUpdateData` after native normalization, and flag0's append/dedup policy.

For current-widget identity, the retained `wave3-editor-native-owner-contract.json` remains necessary: native factory `1405f4756` precedes ShowWidget emission `1405f499f`; area attach/replacement reaches `140dadc20`, while `140dae340` resolves impl current guard `+18` at invocation. Delete/replacement can retain an old widget pending deferred deletion. Losing current status therefore must revoke the old CP64 admission immediately even when that widget's allocation remains alive.

## Smallest source-qualified integration successor

Root/shared ownership is required before implementation; merely adding another Boolean-only wrapper would still have no reached owner. The minimal composition is:

1. Add a Session-owned staged area/impl component at the proven lazy construction boundary. Retain distinct parent/area/impl identities, actual QWidget/runtime prefix and native parentless-constructor effects. Commit parent `+3c8` only after the area constructor returns. Its constructor's unresolved native effects remain guards; do not eagerly fabricate a completed area in `ConversionSession` construction.
2. Project the original `140d14e90` registration into the **same** `SessionNotificationState` keyed population with its actual impl weak receiver and native-alive status. Missing population remains unavailable; no fake empty list and no ViewManager/diagram receiver substitution.
3. Give each actual factory-created Boolean editor a distinct candidate ownership component and native guard/lifetime projection. Current-area attach/replacement must update the impl `+18` projection, preserve old deferred-delete ownership, and release the prior CP64 admission. Exposing the private value's address or merely toggling `shown` is insufficient.
4. At an admitted `1405f4524` reselection or `1405f22af` Save emission, use the actual shared notifier suppression/flag90/key-list prefix. Preserve list snapshot/order and expired receivers; for the selected receiver, match actual current editor and invoke CP64's suffix synchronously. No event/timer should be registered for this direct suffix.
5. Keep the complete effect driver and first-Open/current-editor construction dependencies guarded. Neither the new area holder nor direct invocation proves an earlier initial selection or modelUpdated delivery. Gate the bridge at the actual reached source point, not at timer request or session allocation.

A focused bridge regression should use two real Session owners with colliding logical IDs and actual typed receiver/editor components; attempt cross-session admission and same-address/new-lifetime editor replacement, verify no Apply, then prove one admitted invocation mutates before its caller's Leave/refill or Save propsLoad. Revoke the old scope at replacement while its deferred-delete allocation remains live. These tests require the actual component/interface work above; the seven CP64 tests remain synthetic isolated contracts and must not be presented as this Session integration.

## Decision and frozen scope

No CP65 source/test files were modified and no tests/builds were run. CP64 source hashes remain the freeze published in `analysis/resume64-boolean-sync-implementation.md`. This is a concrete shared ownership dependency, not a request to relax admission or a reason to stop independent recovery work.

The public constructor still reports `Constructor raw0x4a posting requires the actual shared queue/dispatcher continuation`. First fresh-Open Boolean Apply, complete native event/filter population, native dispatcher values and nested callbacks retain their existing guards. A shared owner-construction/registration increment is the safe next implementation action; attaching to an unrelated existing owner is not.

## Inspected candidate source identities

| Source | SHA-256 at inspection |
| --- | --- |
| `include/m2m/engine/conversion_session.hpp` | `09feedab6ca1ac321deee04eecf5ff51bd6c1b2302c396ee4de0a3af5acfcebe` |
| `src/engine/conversion_session.cpp` | `fb54fd5f79dbbe6660c0cc0284015bff8d33a8ae9495bf88de8b985b7f55a8fd` |
| `include/m2m/doe_session_notifications.hpp` | `be68879bf7756a67b229662d5d2397f16d89668c3accf277c7a97b442c5ad625` |
| `src/ops/boolean.cpp` | `4d881b968ca13eb9e78fb57114b4e062358f870e2a075f7c9ce7740cecb2ab8e` |
| `include/m2m/mst/loader_l5.hpp` | `44cb678960d3f7d746fe29b26edd54b4361ae83408b4a153e08eadf828e4ab88` |
| `src/engine/boolean_editor.cpp` | `5aab83cb71d5eedc69ad1476c4e5ff2adc566687655694e59e3c3d949ff9a44f` |
| `src/engine/save_current.cpp` | `ba944dade590fd73cbe16c95a2a83d7f50868b2f1b05370bd477f7611d7f9812` |
| `src/core/convert.cpp` | `f333209669d87df730849bf2d7962b052a3d9c245ed3c328f268b1567cbd2555` |

All paths in this table are relative to `source/apps/moproj2mst`. Source structure is candidate evidence about the missing integration, not an original behavioral oracle.
