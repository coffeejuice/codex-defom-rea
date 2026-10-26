# CP116: zero-layout timer delivery and return boundary

The zero-delay request has a distinct dispatcher route: a posted **0x9a event targets the dispatcher**, which later looks up its timer record and sends a normal timer event to that record's tree-view object. The tree's matching layout branch can stop and return without layout, or perform Qt layout and optional scrolling. Its virtual+1c0 is **QTreeView::scrollTo**, not Boolean editor Apply. No unconditional same-owner Boolean creation follows from requesting or receiving this timer.

Only this report was written. Bounded DUMPBIN14.44.35228.0 reads ran in memory against retained local PE files. No original launch, GUI/control, REA import/database, source edits, build or tests occurred. Earlier CP112/114 captures were read without modification.

## Registration does not establish delivery

CP114's654a4f54 calls QBasicTimer::start67117ea0 for timer storage at tree-private+3a4, interval0, receiver=q_ptr. The source chain is:

1. QBasicTimer clears its stored ID at67117ed7, calls QObject::startTimer6712b1e0 at7ee7, and stores the returned ID at**7eec**. QObject reads that receiver's actual thread-data dispatcher at6712b207/20b; no dispatcher returns0.
2. QAbstractEventDispatcher::registerTimer6710c5a0 allocates an ID atc5bc, calls dispatcher virtual+78 at**c5d2**, and returns that allocated ID. Its return alone is not proof that the void native registration accepted the receiver/thread or populated a live record.
3. QEventDispatcherWin32::registerTimer67143410 rejects invalid ID/negative interval/null receiver and compares receiver, dispatcher and current threads at43464/4347d. Its normal record contains dispatcher+0, ID+8, interval+0c, actual receiver **+10**, callback marker+18 and platform handle+1c.
4. It calls private registration helper67141460 at**434c6 only if dispatcher-private+98 is nonnull**. Otherwise it still proceeds with record containers; do not infer that the initial zero event was posted at this callsite. The successful helper call precedes the normal append/hash insertion at434d7/434f0.
5. In67141460 interval==0 at414dc/414de allocates an event, writes type**0x9a** at4150f, and calls postEvent6711bfd0 at**4151d**, receiver=`[dispatcher-private+8]`, the dispatcher's QObject. The nonzero branches instead use other timer facilities. This zero path does not itself send a timer event to the tree or call Apply.

Actual dispatcher, thread equality, private+98 value, accepted record, returned ID and posted-event dequeue are absent for the selected Boolean first-Open instance. A zero interval is neither an observed deadline nor an instruction to invent HWND/postedWake values. The existing constructor raw0x4a path is separate.

## The callback has a record lookup and a second lookup after return

QEventDispatcherWin32::event67142240 handles event type0x9a at42273. It looks up the event's ID in dispatcher-private+C0 and requires a nonnull record at422db. It writes record+18=1 at422e4, constructs a normal QTimerEvent with the same ID at422f0, and, if the application singleton exists, calls **notifyInternal67119570 at67142315**, with **receiver=record+10**.

Thus the posted event's receiver is dispatcher Q, while the synchronous child notification's receiver is tree T. Neither is the current Boolean editor E or the widget-area impl. Normal QObject timer dispatch invokes receiver virtual+30 at6712cb49, subject to the actual application/event/filter route. On the qualified SfTreeView table142a0b158, +20 is QAbstractItemView::event654abd50 and +30 resolves through138c88 to **QTreeView::timerEvent654ea420**.

After notifyInternal returns, dispatcher event handling **looks up the ID again** at4231a..42370. Only a remaining record with interval0 and marker+18 nonzero gets another0x9a post at423b9; it then clears the marker. A removed record does not get this repost. This is callback-sensitive: an ID is not a lifetime/generation identity, and the marker is not evidence that nested callbacks are impossible. Successful stop/unregistration must be related to this same dispatcher record rather than presumed from a logical candidate ID.

## Matching the actual layout timer

QTreeView::timerEvent performs its own timer tests, then tail-calls **QAbstractItemView::timerEvent654a8820 at654ea701**. The base handler tests other timer storages before reaching private+3a4. The selected layout suffix is exactly:

| Source predicate | Effect |
| --- | --- |
|Incoming QTimerEvent ID equals QBasicTimer(private+3a4).timerId at**654a8956**; all earlier selected cases did not take their return|Stop that timer at**8965**. A received timer event with another ID does not admit this suffix.|
|`((u32(*(T+20)+8) >>15) &1)==0`, tested at**8978**|Return at8a6d..8a7f without calling layout. The stop already occurred; this path does **not** execute the pending-byte clear at898b.|
|The same widget-data bit is1|Stop again at8985, clear layout-pending private+3a2 at**898b**, then call T virtual+1f0 at**8998**.|
|After layout, current index has row>=0, column>=0, nonnull model, and raw private+2f8==3 at**89ac..89c9**|Call T virtual+1c0 at**89d9**, with the existing index and hint0. Otherwise skip this call. No initial row-zero selection is synthesized here.|

On SfTreeView's source-qualified primary table, +1f0 ->138d3c/IAT141e06f50 ->**QTreeView::doItemsLayout654e8280**. Its +1c0 ->138d18/IAT141e06f80 ->**QTreeView::scrollTo654e9f50**. The identical slot offset on Boolean's different table means Apply; it must not be used to relabel this tree callback.

## What layout itself closes, and the remaining source frontier

QTreeView::doItemsLayout has state-dependent persisted-index cleanup (private+475), obtains the current root private+308, and calls installed Qt model virtual+78 at**654e84d0**. If that returns true it enters recursive layout helper **654e68d0 at654e84e3**. It then calls QAbstractItemView::doItemsLayout654a5120 at84eb and the header's layout virtual at84fb.

The fresh tree model producer already identified at140054a08 calls constructor**1400c4890**, which installs table**14215e7b8** atc48af. That table's +78 ->138a60/IAT141e0b750 ->**QStandardItemModel::hasChildren6552a180**. The bounded leaf through RET6552a1fd performs index/item/row/column pointer checks with no call instruction. This query itself is not an Apply. Actual continued ownership of this model/table remains necessary; do not equate it with engine GuiModel or OPI.

The next **source** frontier for a layout-generated application effect is the recursive model/item/delegate work in **654e68d0**, reached only when actual hasChildren is true, plus actual header/layout/geometry recipients. QAbstractItemView::doItemsLayout654a5120 itself stops private+3a4, clears pending+3a2, invokes geometry virtual+240, and updates the viewport. The inspected outer timer/layout bodies do not explicitly select the application's NavigateToFirst/UpdateData or Boolean Apply sites. This is not complete transitive absence through model queries, geometry, filters or reentrancy.

## First unresolved runtime edge and smallest next witness

For this route the first unresolved **runtime** step is a source-reached layout request on actual T followed by a live same-dispatcher record and **0x9a dequeue ->67142315 child notification before the selected Open completes**. Static posting/registration does not choose that interleaving. Even a delivered layout event supplies no OPI/editor relation by itself.

The smallest targeted passive observation is: identify T and its private+3a4 registration lifetime; retain the actual67142315 receiver/ID and enclosing Open span; record entry into **654a895e**, the widget-data branch at8978 and, if reached, layout call8998/return. This avoids inferring earlier timer-ID test outcomes from a count. For a positive **Boolean creation** claim, additionally retain any nested real widget-area UpdateData ->Boolean Apply1411e2870 on E, E+38 resolved OPI O, and normal add return1411e295a or1411e2bcb for that OPI. If no such nested frame is recorded, the result remains unobserved rather than a no-Apply claim. Timer-origin attribution requires the actual dispatcher-record lifetime, not equal numeric IDs alone.

Use the established Open boundary kinds: GUI handler actual return028514; selected CLI startup branch completion001e4e, with001c94/001cae labeled as earlier loader/postOpen returns. This report authorizes no original run. Public first-Open/PostPending guards remain necessary.

## Reproduction and bounded byte identity

Preferred-base binary hashes remain EXE `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, QtGui `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`, QtCore `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Use the verified DUMPBIN path from CP112/114 with `/disasm:bytes /range:<start>,<end-1>` on these retained files. Bounds below are unwind regions except the marked leaf candidate, whose terminal RETs were inspected; they are not whole-callgraph bounds.

| Binary / VA range (end exclusive) | Code-byte SHA256 |
| --- | --- |
|QtCore67117ea0..67117efe|26312198fbdaf82b0d11c3d3e2e5cbdac3ce85671df31042e1c395c1470f8753|
|QtCore6710c5a0..6710c5ed|86acc883d415c14b9a44d5ac8ca69c7a250e8f027d94f53801acec3bfd7de2e2|
|QtCore6714344f..671434fa|fe96db22619bf48b0303bd2fc228097fd71bb938696d2fa7b65a4a656ad5af99|
|QtCore67141460..6714155b|aad3da1c7d73fcfcbdbacf78c79c41cb5dfd4d81c7436a8e2185f5c6c85191f1|
|QtCore67142240..671423ff|cd82485f6a2cde37b2f58a79560e6f5b23f32a3e7bdf425a0ceb18cfad45c521|
|QtGui654ea420..654ea706|982ee043dd83c15622d1a1500b9d6afed12457249ac10eddbb61385e385daf9b|
|QtGui654a8820..654a8a80|cb693fb9ef0ed3fa2effac86c1d9eddacbefa0f1bcd0807397b8e01d0ba0f41b|
|QtGui654e8280..654e851b|7482515c36b0ba7ec5588f4be7ff242336f8d2889a849364c13d77b736bfc6cb|
|QtGui654a5120..654a5167|6b417f3ee2ffb528d05f0bcd3ba977213b57390b30d0e09be34363a05db28a76|
|QtGui6552a180..6552a200 (explicit128-byte leaf candidate)|5a5cfa674a8be2e0056221e80589d4d1c677ea3e35c7197ac3c780dfff7edd40|
|QtGui654e68d0..654e7161|c2655d02709066d12ead5ccf1d968730455cdec4cd4367312112c637f0adaeb3|
|EXE1400c4890..1400c48bc|76a4572c591e44f69a13026fdf8f64acdc19838f4fab93a555621994493f198d|

