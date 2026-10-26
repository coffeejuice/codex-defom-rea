# CP69: acquiring the actual status-widget private/cache owner

The existing status prefix cannot yet supply a native QWidgetPrivate cache owner. ConversionSession has no status-owner field or acquisition call, and the isolated CloseStatusMainOwner projection stops before establishing that its nonnull widget is a QStatusBar. A new Session-held metadata object would retain diagnostics; its creation would not establish the missing private allocation, constructor progress or lifetime.

The earliest native structural binding is precise: QObject's protected constructor writes public+8=private at6712e655 and private+8=public at6712e661. That binding precedes thread retention, QObject accounting, QWidget initialization, QStatusBar public class installation and constructor return. QWidgetPrivate's earlier+c0=null constructor store is a distinct event, not a null value that can be inferred when Close later obtains an existing status widget.

This report is the only write. No source, build, test execution, GUI, original execution or mutable REA operation occurred. CP65 source stayed untouched. Method: current source, retained CP63/65/66/67 evidence and bounded offline PE/import/export/DUMPBIN analysis. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`; QtCore4_64.dll `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. No reconstructed executable or deployment was inspected or changed.

## What current Session state actually binds

Source paths below are relative to `source/apps/moproj2mst/`.

| Current owner/API | Established state | Missing status/private fact |
| --- | --- | --- |
| `ConversionSession` constructor, `src/engine/conversion_session.cpp:144`; accessors in `include/m2m/engine/conversion_session.hpp` | Owns its actual ModelManager, MainViewState and WidgetRuntimeState; partially constructs view-manager/splitter state. | No QMainWindow status/layout/private allocation, QStatusBar construction or status lookup binding is made. There is no CloseStatusMainOwner member/accessor. |
| `CloseStatusMainOwner::create`, `src/engine/close_status_owner_prefix.cpp` | Diagnostic projection associated with the Session address and its actual live ModelManager. Explicit layout/item observations record candidate identities. | Not installed in Session; not native main construction or capture authentication. Its nonblank qualification is caller provenance. |
| `CloseStatusItemOwner` / `CloseStatusOwnerPrefixProgress`, same helper/header | Same consumed candidate item and its raw+10 widget identity survive retries. Qualified V2 virtual+60 is a pure field load. Unknown, observed null and unavailable nonnull stay distinct. | No private pointer, private allocation phase, QStatusBar type or destructor provider owner. |
| `WidgetState`, `include/m2m/engine/view_manager.hpp:345` | Actual candidate component identity, runtime, public native-liveness flag, constructor/event/geometry fields. | No separate QWidgetPrivate allocation identity, lifecycle or+c0 cache field. Runtime membership alone does not establish QStatusBar type. |
| `QObjectConstructionRecord`, same header:320 | Explicitly a per-object counter-effect record. | Its source comment excludes standing in for construction, thread/parent binding or destruction. It is not a private allocation owner. |
| `EditorBaseConstruction::resume`, `src/engine/editor_base.cpp:21` | Stages selected QWidgetPrivate/QWidget effects on the factory's same actual editor WidgetState; emits `PrivateAllocation`/`QObjectPrivateConstructed`/`WidgetPrivatePrefix` effects. | No independently identified native private allocation or+c0 cell. This editor occurrence cannot be borrowed as a status-widget constructor/lifetime. |

`test_close_status_owner_prefix.cpp` deliberately passes `session.mainViewOwner()->widgetState()` as a fixture's item widget in `status_owner_actual_v2_nonnull_widget_guards_before_qobject_metaobject`. The result is `WidgetMetaObjectUnavailable`, not a QStatusBar result. The test explicitly says a QWidget owner is not proof of QStatusBar. It is evidence of the guard's scope, not an existing status-bar binding.

The nonnull helper currently ends at6711ddeb, before QObject::metaObject and the QStatusBar inheritance walk. A foreign runtime or dead/unavailable weak target stops even earlier. Neither stop can be promoted to private allocation acquisition by attaching a new metadata holder.

## Native acquisition and the first bound state

The normal existing-bar chain is: native global main -> main.public+8 private -> private+1e0 layout -> layout+4f8 item -> qualified item virtual+60 -> item+10 widget -> QMetaObject::cast against QStatusBar's static metaobject. CP63/65 qualify the actual addresses and branch conditions. The item slot owns item storage; its widget pointer is nonowning. A successful cast of the actual surviving widget is required before calling QStatusBar::showMessage. That method's private load addresses that same returned widget, not Session::MainViewState.

The lazy branch is different: null slot or null/cast-failed result reaches the outer allocation at653ca63f. Source-qualified fresh QStatusBar construction supplies this sequence:

1. At65406d70 allocate0x210 bytes for the **private** object. Public `this` is a separate existing constructor receiver. Allocation failure/exception behavior is not replaced with a convenient live owner.
2. At65406d84 call QStatusBarPrivate65406880 on that allocation. Its base call6540689b invokes QWidgetPrivate(int)6505e780. At6505e7e3 the base writes private+c0=null. This is the earliest qualified KnownNull event for that allocation; no public QWidget d-pointer binding has happened yet. Later palette/font/locale and constructor effects remain ordered dependencies.
3. QStatusBarPrivate installs its dynamic table6570bd18 and constructs its instance list/QString. The protected QWidget constructor65070f00 receives the same private reference at65406da0.
4. QWidget65070f27 calls import65696a30. The import names `QObject::QObject(QObjectPrivate&,QObject*)`, resolved by the QtCore export to6712e630. QWidget passes a null QObject parent at65070f24; the QStatusBar main-parent argument is retained separately for QWidgetPrivate::init later. Do not commit parent ownership at the early QObject binding stores.
5. QObject6712e655 stores the incoming actual private at public+8. At6712e661 it stores this same public receiver at private+8. Immediately after the second store, the reciprocal alias is bound, while the constructor remains incomplete. Thread-data selection/store/retention follows at6712e665..e685; accounting follows at6712e6e0. These later effects are not authorized by merely recording the alias pair.
6. After QObject returns, QWidget installs its own public/paint-device classes, loads the same public+8 at65070f5b, and calls QWidgetPrivate::init6506e4d0. Only after QWidget returns does QStatusBar install its public class at65406dad and continue its timer/layout/child setup. Its original constructor must return, setStatusBar must run, and the getter must re-read its actual layout item. The source chain is not replaced by returning a fabricated status widget directly from the allocation branch.

The earliest **native bound** state is therefore a phase such as `QObjectBaseAliasesBound`, with one actual private allocation linked to one public widget and all later phases pending. It is not `QStatusBarConstructed`, parented, registered, callable as QStatusBar, or destroyed. The KnownNull cache store belongs to the earlier6505e7e3 event. An implementation beginning at the alias-binding point must either retain that exact same allocation's tracked constructor history or keep the current cache value Unknown. A late projection cannot replay a fresh constructor store to erase intervening state.

For an already existing status bar, acquisition starts after the actual cast/lookup, not from the fresh constructor. Its private identity can be bound from the same live widget's public+8 at a qualified read. The current+c0 value remains Unknown until a same-occurrence read or a complete tracked history establishes it. QWidgetItemV265049543..65049555 publishes itself only when that cell is null; another item is preserved. QWidgetPrivate's copy constructor65011736/6501173d copies+c0 rather than applying the fresh constructor default. Neither late Close nor an existing candidate public widget qualifies a fresh-null cache.

## Minimal root-owned integration boundary

The next production seam is acquisition/state representation, not destructor execution. Root should keep the current public Close/status guards and introduce the following relationship only when its actual producer is available:

1. Session may retain its single actual native-main projection for diagnostic continuity, separate from MainViewState. Missing native main/layout/status links stay Unknown. Storing the projection does not claim original main construction.
2. Use the same actual item and WidgetState already consumed by the status lookup. Bind QStatusBar type only after the qualified dynamic metaobject/cast effects return; until then retain the existing6711ddeb guard. Known-null/cast-null still goes to the allocation guard.
3. Associate one private-allocation record with that same public widget. Its canonical allocation identity, public backlink and explicit native lifecycle/constructor phase must be independently represented. Do not use the Session, widget C++ address, `shared_ptr` control block or a newly allocated metadata address as evidence of an original private allocation. A reconstructed logical allocation requires the real constructor path to create/bind its identity; an observed existing allocation requires source-point evidence of the actual alias.
4. Put the Unknown/KnownNull/KnownItem+c0 cell on that private record. Cache identity is nonowning and must resolve to the same common QWidgetItemV2 allocation used by its actual layout and raw widget field. Current `CloseStatusItemOwner` is layout-local diagnostic storage; it cannot name every possible widget cache target or prove those items are allocated/live. An unresolved nonnull alias must remain nonnull/unavailable, not become null.
5. Record the fresh constructor null store, V2 publish-if-empty and destructor backlink writes as separate qualified events on this common graph. Do not initialize late-bound private state to null. Freeze consumed cache/item identities per call-frame progress so retries cannot silently act on a replacement.
6. Keep public QObject destruction, private destructor entry, private payload completion and allocation free as separate transitions. CP67's base-private prefix is admitted only on the actual private/provider path after preceding QStatusBarPrivate instance QString/list cleanup. A still-held C++ diagnostic lease is not native memory survival; `WidgetState::nativeOwnerAlive()==false` is not proof that private memory is already freed. Conversely, a live public widget flag does not certify private/provider entry or cache target lifetime.

File boundaries for future coordinated implementation: root's Session ownership/accessors in `conversion_session.hpp/.cpp`; actual widget/private relationship in `view_manager.hpp` and its owner implementation; common item identity/lifetime rather than a status-only duplicate; and the isolated `close_status_owner_prefix.hpp/.cpp` progress adapter. The editor constructor cursor must reference its own actual private allocation if a common record is added, not construct a second editor/private graph. This report does not authorize a broad refactor or supply missing effects.

The smallest isolated testable addition is an unbound/Unknown private-allocation projection with explicit, non-authenticating observation APIs and a refusal at the existing cast/constructor boundary. Its creation must leave counters, parenting, allWidgets, cache contents, timers, pending events, Project/OPI and public Close behavior unchanged. Source-point observations should be distinguished from effect execution, as in the CP68 lifetime skeleton; observation methods must not become production shortcuts that manufacture a completed constructor/destructor.

Focused future tests should establish:

- A newly Session-held status projection still has no private allocation/cache binding; the real MainView widget does not become a status bar.
- Generic QWidget/nonstatus dynamic type, foreign runtime, expired target and missing actual private alias retain their guards.
- Reciprocal alias binding is distinct from constructor completion, QStatusBar public class, thread retention, counter changes and main parenting. Conflicting allocation/public aliases are rejected before mutation.
- A fresh6505e7e3 null event affects only the identified constructor allocation; late binding and copy paths remain Unknown or retain the copied alias.
- Same canonical item identity is shared by layout, item.widget and private.cache; other cached item, unavailable nonnull target and same-address/different-owner records are distinct cases.
- Public guard invalidation does not synthesize private-free/cache-null/item-widget-null; only the qualified exact producer changes a field. Session diagnostic release does not execute native destruction.
- Status-private member cleanup gates base-private entry; private cleanup affects only the actual cached item, and a retry keeps its consumed identity. Normal one-project Close cannot invoke this teardown merely because Close was accepted.

These are proposed tests; none were created or run. Successful isolated metadata tests would establish storage distinctions, not runtime acquisition, Qt lifecycle parity or an enabled public Close branch.

## Bounded evidence receipt and remaining uncertainty

New QObject binding evidence is retained inline here; no raw file was added. DUMPBIN14.44.35228.0 from MSVC14.44.35207 was run with `/nologo /disasm /range:0x6712e630,0x6712e6f2`. PE import/export resolution independently bound the named constructor. The region is one unwind-qualified direct function; transitive thread/parent providers are not claimed closed.

```text
QtGui IAT65696a30 -> QtCore4_64.dll ??0QObject@@IEAA@AEAVQObjectPrivate@@PEAV0@@Z
QtCore export ->6712e630; unwind end exclusive6712e6f3
65070f24 xor r8d,r8d             QObject parent null
65070f27 call [65696a30]         supplied actual private remains rdx
6712e655 mov [rcx+8],rdx         public.d_ptr = supplied private
6712e661 mov [rdx+8],rcx         private.q_ptr = same public
6712e67e mov [rsi+40],rax        later threadData store
6712e6e0 lock add [672f4344],1    later QObject count increment
```

| End-exclusive native region | Code SHA-256 | Qualification |
| --- | --- | --- |
| QtCore6712e630..6712e6f3 | `5dedc14332bbd1f8dcb40345f6134a21057d4c7be8b2b0598905802a195cd296` | Protected QObject constructor; direct reciprocal binding stores. |
| QtGui65070f00..65070f76 | `49494ec7912d906116e123409af0384da2429070851efbd696f75f5243308658` | Protected QWidget constructor; retained `wave3-closure-next-65070f00.asm`. |
| QtGui65406d50..65406da5 | `9966e5da1c5581ec0ef8477231fccb40d785c7665f86d7b88543db2dcadc4d5e` | Deliberate QStatusBar allocation/private-construction/QWidget-call prefix, not the whole constructor. |

CP67 supplies the code hashes for QWidgetPrivate int/copy constructors, V2 publication and both private/item teardown directions. CP63/65 retain status lookup/cast/showMessage and lazy constructor/setter boundaries. CP66 qualifies DeferredDelete posting/delivery separately. No existing status widget/private/cache runtime alias or current cache value was acquired in this lane. Native main binding, actual cast/constructor effects, common item population, intervening/reentrant mutations and actual destructor reachability remain required evidence. No new synchronous public Close effects or process-global cache reset are established.
