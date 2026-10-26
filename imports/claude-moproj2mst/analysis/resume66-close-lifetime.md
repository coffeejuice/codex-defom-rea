# CP66: status-item, deferred widget deletion and process lifetime

One-project Close does not make layout-item destruction, deferred status-widget destruction and process-global reset the same boundary. A valid existing QStatusBar getter returns directly without replacement/deleteLater. A replacement setter can delete the old **item** synchronously while leaving its widget alive until a separately eligible DeferredDelete delivery. Neither operation establishes a reset of the process-global MessageArea QString.

This report is the sole write. CP65 source remained frozen. No source/build/tests/GUI, target execution or mutable REA operation occurred. Method: retained captures/current source and bounded offline PE/DUMPBIN views, with selected new evidence retained inline below. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`; QtCore4_64.dll `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. DUMPBIN14.44.35228.0 from MSVC14.44.35207 was used. These are static conditional contracts, not a fresh original one-project runtime trace.

## What normal Close can actually reach

After the false-busy setter, normal029ce4 calls the status helper. CP62/63 qualify the fresh global-main lookup and QMainWindow::statusBar. CP65 advances only qualified QWidgetItemV2's raw widget+10 read and the cast-null leaf. Nonnull still stops before QObject::metaObject/inheritance checks; no native QStatusBar has been established by that candidate prefix.

If the native getter finds an existing valid QStatusBar, it returns it directly and calls showMessage on that same result. It does not call setStatusBar or deleteLater on this branch. If lookup returns null, it constructs a new bar and later calls QMainWindow::setStatusBar. That setter re-reads the current bar: only an existing **different** nonnull result selects hide plus deleteLater. With a stable null lookup throughout construction, this replacement-deleteLater branch is skipped. Constructor/parent/event reentry could change that lookup, so stability cannot be inferred from the first null observation alone.

Which conditional route occurs in the selected fresh original Open→Save→Close remains unobserved. Known-null allocation fixtures and known-nonnull item fixtures establish candidate guard contracts, not an observed late status owner. Earlier DeleteWidgetInWidgetArea delivery, widget construction/callbacks and status ownership still need their own source-bound completion.

## The old QWidgetItemV2 is not the old QWidget

Layout setter653cc1f0 calls addChildWidget for the replacement, invokes the old layout item's deleting virtual with flag1, then constructs/stores a replacement item. The layout's old+4f8 pointer is not cleared before that deleting call. The old widget's deleteLater request, when selected, was made earlier by QMainWindow::setStatusBar.

Fixed item table656a58c8 slot0 targets65049860. Its flag1 scalar branch selects650498b1: install the V2 vtable, read widget+10, and if nonnull inspect widget private(+8)+c0. Only when that cache equals this exact item does it store null. It then installs base QLayoutItem vtable656a5258 and frees the item allocation when flag&1. It never invokes the widget's deleting virtual. Exported nondeleting QWidgetItemV2 destructor65049580 has the same cache rule and base transition; QWidgetItem::~QWidgetItem65043e90 forwards to65048320, which only installs the base table and returns.

Thus a diagnostic weak widget lease expiring is not a native null field, and dropping an item shared_ptr is not evidence that its QWidget died. Conversely, a widget may retain a different current item cache; item destruction must preserve that unrelated pointer. Cache identity/ownership needs to be explicit before implementing this destructor prefix. Current WidgetState has no source-qualified representation for native QWidgetPrivate+c0's QWidgetItem cache.

Selected bounded instructions:

```text
6504986f  test dl,2; flag1 selects scalar650498b1
650498b1  install QWidgetItemV2 table
650498bb  load item.widget(+10)
650498c4  load widget.private(+8)
650498c8  compare private+c0 with this item
650498d1  matching cache only: store private+c0=null
650498dc  install QLayoutItem base table
650498e6  test flag&1
650498ee  free item allocation; no widget-delete call
```

The deleting wrapper has chained unwind fragments; its first65049860..65049884 record is only a prologue/array-branch fragment. The scalar branch, not a guessed whole-function boundary, supports the flag1 claim. No array-deletion behavior is generalized here.

## deleteLater posts; event delivery deletes

QObject::deleteLater67129b10 allocates a0x18 QEvent, constructs raw type0x34, and tail-calls default-priority postEvent. It contains no receiver destructor call. In postEvent6711ae90's admitted enqueue branch, the event is placed on the receiver's thread queue, its posted flag/count are updated and a dispatcher wake is requested when available. Null receiver/threadData and the application compression virtual have earlier branches; a deleteLater call is not proof that a new distinct queue entry was inserted. Actual compression/provider behavior remains a dependency. For raw34 posted on the current thread,6711afbe..afc2 copies threadData+1c into event+8 as its posting loop level.

QCoreApplicationPrivate::sendPostedEvents6711b040 considers that level before delivery. For a matching raw34 event, the inspected condition permits delivery when stored level is greater than current, or stored level is0 while current is positive, or the request explicitly filters raw34 and stored equals current. Otherwise it skips/requeues according to the request/receiver case. Equal levels under an all-event request do not by themselves establish delivery. Queue population, current thread/loop level, request filter, application instance and the actual pump interval remain required observations.

On an admitted event, selected delivery code clears posted state, decrements the receiver's posted count and tombstones the record before releasing any held queue lock and calling67119570's notification path. After that returns, the event object's deleting virtual is called. This latter event deletion is separate from deleting its receiver.

For the actual Qt QStatusBar handler profile, raw34 bypasses its special0x4c/0x48/0x47 handlers and delegates to QWidget::event. The QWidget selector tables choose default QObject forwarding for raw34, including through its preliminary state-filter table. QObject::event's raw34 table entry selects6712cb70 and invokes the receiver's deleting virtual+18 with flag1 at6712cb84. Fixed QStatusBar table6570be88+18 is65407680; its scalar branch654076d1 calls QStatusBar destructor654075a0 then conditionally frees allocation.

This closes the selected unfiltered class-dispatch route. It does not establish that this event was posted/due/delivered in the actual Close interval, that application/object filters did not consume it, or that a different receiver subclass uses the same handler. Do not add an eager flush or direct destructor as a reconstruction shortcut.

Selected source bindings:

```text
67129b36  QEvent raw type=34h
67129b52  tail postEvent(receiver,event)
6711afbe  load threadData.loopLevel(+1c)
6711afc2  store level in event+8
6711b21a..6711b245  raw34 loop-level/request-filter eligibility
6711b29d  decrement receiver posted count
6711b2a0  tombstone queue record event slot
6711b32a  notification path; later6711b33e deletes event object
6712cb84  QObject raw34 handler deletes receiver through virtual+18(flag1)
```

The QWidget raw34 selector bytes are1 at6d78c[0x34-2], selecting6506c9ba, then52 at6d924[0x34-2], selecting6506d767 -> imported QObject::event. QObject's byte12cf08[0x34-1]=3 selects its12ceec entry6712cb70. These are decoded data tables, not instructions.

`resume47-boolean-cleanup-contract.md` remains the qualification for QObjectPrivate/removePostedEvents ordering. Receiver cleanup removes posted events and eventually deletes those event objects; it does not deliver DeferredDelete or delete the receiver as a substitute. Actual QObject guard invalidation/destruction signals belong to the receiver's proper base-destructor boundary, not the deleteLater posting call.

## Status payload versus application/process owners

QStatusBar destructor654075a0 first processes its status-private+1e0 item-descriptor list, freeing removed descriptor allocations, then tail-calls QWidget destructor6506e890. Complete QWidget/private/child/destruction callback effects remain separate dependencies. This is not the V2 layout-item destructor, and descriptor-pointer frees must not be relabeled child-widget deletion.

The status timer at status-private+1f8 is its own QObject child/current raw field. Fixed Close showMessage(timeout0) can delete it synchronously before nulling+1f8; a status-bar deletion reaches broader QWidget/QObject-owned cleanup. Neither mechanism licenses clearing the Session diagram timer queue or destroying its dispatcher. Relative descendants/guard/callback order must be recovered for the actual owner, not inferred from an external shared_ptr lease.

The MessageArea global last-message QString1446b3bb0/initialization word1446b3bb8 is independent of the per-status-bar message field and any particular receiver. CP63's handler updates it only after append returns. Receiver destruction or normal one-project Close provides no direct reset instruction in this selected contract. The callback141dfb720 destroys the QString through its imported destructor; it does not reset the initialization word, and actual CRT exit delivery is not established by normal Close. CP64's process-cache helper correctly guards every reset request.

ShowMsgInArea may have no live receiver after earlier widget-area work, or a live receiver may suppress an equal message. Therefore even the final cache **value** cannot be inferred from the Close literal without qualified delivery/population. Preserve process-global ownership; do not create/clear a cache per receiver or Project. The application busy/counter resets and earlier application process-type5 route are separately source-qualified in CP60/62, not a blanket engine/notifier/remesh/global reset.

Current ConversionSession destruction explicitly releases adapter Project storage, diagram receiver/dispatcher, MainView and adapter task scopes. `releaseProjectAfterCloseEffects` only releases Project storage and deliberately leaves receiver/dispatcher/pending timers alive. Neither operation replays original normal Close. A root coordinator must not use Session destruction to manufacture status-widget/process exit semantics; the selected original0299a0/031f10 path returns before any direct application teardown call.

## Smallest next contract and validation

The smallest closed independent destructor increment would be an explicit QWidgetItemV2 scalar cleanup prefix bound to the same item and actual widget/private-cache owner: install destructor V2 class, inspect widget+10, clear private+c0 only when it equals this item, then install base class and guard actual heap free. Missing widget/cache knowledge must retain the earlier native phase. Keep the caller layout slot visible until its own later replacement store. This requires a new actual QWidgetPrivate item-cache field/identity seam; current status/WidgetState owners do not supply it. No source is changed here.

Focused future tests should distinguish null widget, unknown cache, matching cache and a different live item cache; preserve widget identity/liveness while freeing only the item; retain the old visible layout slot during a deleting effect; and prevent repeated completed cache writes on retry. A separate deferred-delete test must prove post-only liveness, loop-level eligibility, queue mutation before dispatch, event versus receiver deletion, filters and actual receiver QObject expiry. It must not synthesize a queue drain at Close return. Process-cache tests should retain one owner across receiver/Project teardown and continue guarding native reset. Fresh original one-project lifecycle acceptance remains open.

## Bounded evidence receipts

New raw views were retained inline in this report; no new ASM files were written. Reproduce with `dumpbin.exe /nologo /disasm /range:<start>,<end-minus-one>` against the indicated original binary. Existing CP62/63/65 captures supply enclosing status/parent/timer/Close bodies.

| Region, end-exclusive | SHA-256 | Qualification |
| --- | --- | --- |
| QtGui65049580..650495c1 | 0f76af96647e3cd017a7aab611ea5e2600bca0b3e4f3aaf47fcad71a010dc305 | leaf destructor, decoded returns |
| QtGui65048320..6504832b | 934de61d39cad906fcac15800363b8662e78123e1266338f484b66e8bec214fd | base leaf |
| QtCore67129b10..67129b57 | a48ab202830783c9040efda804760cd197a8e83b4150f331754afacee68fda5c | deleteLater unwind region |
| QtCore6711ae90..6711b039 | 3e9d9e08c0592464f8f1abc87e14914a11053529346439d712e19d6e356b854c | postEvent unwind region |
| QtCore6711b040..6711b4fe | 0e2b4a74fe05cc290433f6f0aeaab69a5ea93e5f35f3b1011bfd91c2effa7c19 | sendPostedEvents region, selected branch only |
| QtCore6712caf0..6712cf4f | 38ef1099718079f5089b1f300882986342f98d5a78f4f8e92af6d55a2102b4d9 | QObject event plus dispatch data |
| QtGui654072e0..65407593 | 1b4b8feb9e8af354ddb495a92890e058634f609be1a885aed9937fe1501d6da5 | QStatusBar event |
| QtGui6506c950..6506d9e9 | b73ef66751097ecb85253e5e516888a27fb0c220980593848ecceb4a7d29bba2 | QWidget event region includes selector data |
| QtGui654075a0..6540767d | 6b21a8d8852a6840c9ed65c70618ba0139b2302fff3063caba62da5d44a117d4 | status-bar payload destructor |

Bounds do not prove whole transitive call-graph closure. Deleting wrappers have chained fragments; source claims above use flag1 scalar branch targets, not just the first unwind fragment. No source/test/runtime parity claim is made from these hashes.
