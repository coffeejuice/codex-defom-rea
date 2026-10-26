# CP60: false busy setter and tree-selection tail

The false setter has a closed conditional prefix suitable for an isolated staged helper. It first writes busy=false and clears the warning counter, then derives the requested enable state from the current global main window. The tree's previous selection mode does not choose that branch. Actual tree mode changes only if the subsequent notifier reaches a live registered tree receiver.

This report is the sole write. No source/shared/CMake edits, builds, executable tests, GUI, target execution or mutable REA operations occurred. Method: existing qualified captures plus bounded offline PE/DUMPBIN. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; retained QtGui4_64.dll SHA-256 reverified `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`. DUMPBIN14.44.35228.0 is installed under MSVC14.44.35207. Static body closure is not current runtime ownership/population evidence.

## Exact source order

The normal accepted Close tail first emits `2signalDeleteWidgetInWidgetArea()` through2cf3c0 at029c8d, then invokes2ca9f0(false) at029c9b. The preceding notification must return before this false-setter occurrence is reached. This occurrence is distinct from Open's reset and the busy-warning checkbox reset. After the setter,029ce4 calls2d0bd0 with literal `All projects have been closed.` and public Close only loads true at029cf8. The status/reporting call is outside this helper and has not been expanded here; setter completion is not public Close completion.

Inside2ca9f0(false):

1.2caa12 writes global143d766b8+9=0.
2.2caa21 writes counter143d91230=0. This occurs before any main-window query, even if its identity/results remain unknown to the adapter.
3.2caa35 calls2ca920(show=false). Because+9 was just cleared, this normal serialized path queries2ca840. The show=false argument prevents2caf10's modal warning even if the computed busy result is true.
4.2ca840 obtains the actual global main-window pointer through pure2c8090. Null returns false. Nonnull calls that object's virtual+300. A null stacked-widget pointer inside a nonnull main window is not this null-main branch; the native getter dereferences it.
5.For the qualified main table1420fb108, virtual+300 resolves01daf0. It reads QStackedWidget::currentIndex(main+480), and if the result equals2 immediately returns true. Otherwise it calls0327c0, which is now fully bounded: it returns whether the dword at main+270 equals1. It has no calls. Values0,2,negative or any other value are false; this is equality to1, not a nonzero test.
6.If the latched busy result is true,2caa4f/2caa5e write global+A=0 then+B=0 and2cac94 emits TreeEnableSelection(false). If false,2caca8/2cacb7 write+A=1 then+B=1 and2caef3 emits TreeEnableSelection(true).

The main busy predicate is therefore `stackedCurrentIndex == 2 || mainField270 == 1`, with the native short circuit and current object identities preserved. In particular, index2 needs no+270 observation. An unknown current index cannot be skipped merely because a supplied+270 value would make a mathematical OR true: the native first getter still occurs and its object must exist. A different main vtable requires its actual virtual behavior, not this profile by assumption.

| Current qualified main facts after the first two stores | +9 | Counter | +A/+B request | Pending tree Boolean |
| --- | --- | --- | --- | --- |
| Null global main window | 0 | 0 | 1/1 | true |
| Nonnull, current index2 | 0 | 0 | 0/0 | false |
| Nonnull, index other than2, main+270=1 | 0 | 0 | 0/0 | false |
| Nonnull, index other than2, main+270 other than1 | 0 | 0 | 1/1 | true |
| Main pointer/profile/getter facts unavailable | 0 | 0 | retain previous+A/+B until branch known | no dispatch yet |

No tree selection-mode input is read by this predicate. MainView/engineMutex/MoInputMode/process type, a new Session, an empty ModelManager or a null current Project does not establish these main-window fields.

## Closed Qt leaf and the remaining dispatch boundary

The CP59 notifier qualification remains controlling:2cf3c0 first tests notifier+141; suppressed returns before flag90/map/receiver work. Unsuppressed resets+90 before map lookup/default insertion and snapshots the ordered list. Expired entries are pruned only from its local snapshot. Live entries use DirectConnection1 invokeMethod with the supplied Boolean; the dispatcher does not branch on invokeMethod's returned Boolean. The positive tree registration atd39b9c prevents an invented empty population for this key.

The retained metadata receipt `wave3-boolean-external-callback-contract.json` identifies `slotEnableSelection(bool)` as local method6 ->d3c0c0, separately from input-mode/editor local method1. False/true dispatches QAbstractItemView::setSelectionMode with0/1 respectively. PE export lookup now closes that imported Qt body at654a4e30..654a4e3b: load tree QObject private pointer at+8, store the input integer at private+2b0, return. There is no old-value equality check, selected-index clear, event dispatch, signal, virtual call or further cleanup in that body. The getter immediately afterward at654a4e40 reads the same field.

This refines CP59's pending Qt-slot effects: the known slot leaf itself is now source-closed. It does not establish actual receiver construction/liveness/order, other registered methods, or complete current notifier population. A suppressed notification leaves tree mode unchanged, even though+A/+B have already been written. Expired receivers likewise do not update their former tree. With a qualified live known tree receiver, requested false overwrites any old mode with0 and true overwrites it with1. Do not infer that setting mode0 also empties a selection collection.

The QStackedWidget getter is also closed: export65405af0 reads widget private+8, obtains its layout at private+208, and tail-jumps650501e0. That leaf reads layout private+8 and returns dword+0xc8. No callbacks are present in these bounded getter bodies. Actual main->stacked-widget->layout/private ownership and current field values remain observation/binding requirements.

## Current Session owner facts

The current Session exposes its actual ModelManager, shared SessionNotificationState, MainViewState and WidgetRuntimeState. None supplies the global main pointer/table1420fb108, that main object's+480 stacked widget or+270 dword, or the tree receiver's private selection mode. `MainViewState` has its own private/native-alive, manager-child, WidgetState and SplitView ownership; these do not authenticate the global main-window alias or implement the application busy virtual.

CP58 `CloseEntryBusyObservations` is bound to a Session and an earlier normal-entry occurrence. Its observations are not mutable process globals and cannot be reused unchanged at the late false-setter occurrence after notifications/destruction. CP59 `CloseBusyGlobalOwner` binds a declared field projection to an actual Session/manager identity, but supports only the true occurrence and retains its pending TreeEnableSelection guard. It has no authorized false continuation or production native-global binding. Reaching the false tail cannot bypass that earlier pending notification, null refreshes, manager/Project/OPI cleanup or DeleteWidgetInWidgetArea delivery.

## Smallest isolated helper and focused tests

The smallest next isolated implementation is a false-only staged owner/progress object using the existing explicit `CloseBusyNativeFields` value type, exact occurrence `0x140029c9b:0x1402ca9f0(false)`, and same-live-Session/manager identity checks. It must first commit+9=0/counter=0, then retain an `AwaitMainBusyFacts` stage if the late main binding is unavailable. A qualified null main permits the unbusy branch. A qualified same-main/table profile with current index and conditionally+270 permits the predicate above. It then commits+A followed by+B and retains `BeforeTreeEnableSelectionDispatch` with its latched Boolean. Retry must not repeat the first stores or re-query a completed/latched branch. Missing main facts after the first stores retain those stores and preserve prior+A/+B; a blanket preflight refusal before all writes would misrepresent native partial progress.

This isolated diagnostic owner may start from explicitly declared field values. Eventual lifecycle integration must share one actual mutable global owner with the true setter; copying CP59's fields into a second production false owner would lose callback mutations and violate identity. The private true-only owner currently exposes no writable integration seam, so any future shared-owner extension belongs to root after checkpoint freeze. Do not broaden the current source occurrence, synthesize tree registration or call the public Close path.

Focused tests should cover: initial nonzero bytes/counter become+9=0/counter=0 before an unknown-main guard; null-main branch; index2 short circuit with unavailable+270; non2/+270=1 busy; non2/+270=0,2,-1 unbusy; wrong-profile/missing-index refusal preserving+A/+B; retry without replay; immutable latched argument after later supplied facts differ; foreign/expired Session owner rejection; unchanged actual manager/Project/OPI/timers/notifier state. A separate isolated known-tree-slot test may now safely verify old mode2 overwritten by0/1 while selected indexes remain untouched, provided it is described as a declared receiver-state projection. Actual map dispatch remains guarded until qualified receiver ownership/population exists.

## Evidence receipts

Native bounds are end-exclusive.0327c0 is bounded by its unwind entry; the Qt getter/store leaf bounds terminate at decoded ret/tail jump and exclude adjacent procedures/padding. This audit's exploratory80-byte export windows are not function bounds and are not used as the hashes below.

| Native body/slice | SHA-256 |
| --- | --- |
| 14001daf0..14001db3c | 9fde859043f5266fd0d9107338d3d28a1dd5634080b8211f4ce2919e489249f4 |
| 1400327c0..1400327ef | 96132f3a2d0722d406ac97a470810eae94df865fcd9a50534df37cf6b07f280e |
| 1402ca9f0..1402caf02 | 97c943c72cc396f12d6abb4f1736d5bdae97332511b5a1fa1dab1d52a5617598 |
| QtGui65405af0..65405b00 | b2bbcd8ac2e54017789440e895172cccc119cb6b296e59a85ef3e1505ba2e3df |
| QtGui650501e0..650501eb | 65f5763c17c8c47b38ad4d608e06a7521f9dd999d85a85f7176a0d35d62ab640 |
| QtGui654a4e30..654a4e3b | b48a8afcba5fedab0459853a5b33a469e92fae667edbccdfd28d5a489ebf4540 |

Reproduce with bounded `dumpbin.exe /nologo /disasm /range:<start>,<end-minus-one>` against the retained indicated binary. Qt export identity and currentIndex/setSelectionMode call chains were checked directly; no mutable analysis database was opened. Full normal Close acceptance remains unresolved.
