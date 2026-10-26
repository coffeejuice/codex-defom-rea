# CP149: stock raw0x4a compression closes; the pre-publication callback interval does not

New bounded PE work qualifies postEvent's synchronous compression branch for the original SfApplication profile. Raw0x4a is not compressed or deleted by that selected method. This narrows one callback boundary inside post(E,H), but does not prove when H is delivered or which metaobject cache/table E has then. The interval after post returns still contains the base setup's direct notification before Boolean table publication.

Only this report was written. Retained immutable EXE/QtCore/QtGui bytes and bounded DUMPBIN14.44.35228.0 views were used. No source/build/tests, GUI/app control, original process or mutable REA/import occurred.

## Source-qualified compression target

The QWidget constructor's two-argument postEvent import resolves to **6711bfd0**, which sets R8d=0 (priority0) and tail-jumps6711ae90. The latter calls application virtual+68 at **6711af93** only when receiver-private+24 is nonzero and application singleton672f4220 is non-null. Other source branches can delete H or return; posting success is not inferred here.

The EXE imports QApplication::compressEvent at141e0bf68 through six-byte thunk14013873c. That thunk occupies **table1420edeb8+68**. The table's MSVC RTTI complete-object locator1430d3f70 identifies **SfApplication**; constructor1400021c0 installs the table through the LEA at21fd and following store. Thus on the explicitly qualified normal SfApplication owner/table profile, this callback is the stock QApplication method, not an unspecified application override. Actual application identity/lifetime at the selected E post still must be bound rather than copied from another run.

QApplication::compressEvent6501e3e0 tests its handled types4d,48,4c,0e,0d,59,c9,53. Raw0x4a matches none and tail-jumps imported QCoreApplication::compressEvent at6501e48a. Core67119880 reads raw type and handles timer1/deferred-delete34/quit14; **raw0x4a reaches67119963 and returns false**, without queue scan, event deletion or another callback. This is a source-closed conditional result, not a dequeue or handler occurrence.

After compression false (or skipped compression), postEvent continues its actual queue append/count/posted-bit/unlock/wake work. Its selected body has no direct notifyInternal call, but contains the dispatcher virtual+98 wake boundary at6711b021. CP52's Qt Win32 wake prefix qualifies the serial/CAS/PostMessage ordering conditionally; an unbound dispatcher/profile or missing enqueue/wake return cannot be treated as completed. The compression result alone cannot finish constructor PostPending or manufacture successful enqueue/delivery.

## After post return, before Boolean table publication

CP145's selected source order remains: factory's0x50 allocation/Boolean ctor ->QWidget base/init ->post(E,H)6506e857 ->post-return private+c8 store and Create destruction ->native base setup ->E+38 guard binding130857 ->derived table142cd9b18 store11e24b7. H's allocation-null branch remains real; normal E constructor return does not alone identify a live H.

Two additional base helpers were inspected:377b20(null) binds E+28 through cbc920, and377b80(null) is a scalar E+30 store/return. Neither helper itself is an event-loop poll. However base setup130830 later invokes custom `signalChangeActiveStepData()` at**130ad1**, while table1421a8938 is still active; Boolean publication has not happened. That custom registry/direct invocation has actual receiver/callback population dependencies. The bounded source does not prove that no nested event processing occurs in every recipient. Therefore do not extend “no direct notify in postEvent” into an unconditional no-delivery claim over the entire pre-publication interval.

At a pre-publication H dispatch, the same allocation E can have a QWidget/base table and a corresponding current metaobject, rather than completed Boolean table142cd9b18/static142cde640. A later table change can make the cached widget-private+d0 differ again. CP143's Boolean-profile prefix must not accept an earlier base-profile handler by relabeling its table. Source ordering does not establish whether that earlier handler happened.

The separate direct QWidget::setVisible(true) ->ensurePolished call at65070547 from CP145 also remains relevant: later visibility work may alter the cache independently of queued H. The cache cannot be derived solely from H's posting or derived construction. Retain its actual value at65064f79 for the selected owner/occurrence.

## One source-ready private slice

An isolated raw0x4a compression projection can accept explicitly qualified SfApplication/table+68 and actual H/type4a inputs, then report **CompressionFalse** for this source route. Unknown owner/target/type remains guarded; other event types are outside this slice. It performs no queue mutation, event destruction, wake, receiver callback, meta-cache store, native normal-return observation or OPI/Apply effect. A focused synthetic test can check raw4a false versus unknown/wrong-profile inputs without implying posting succeeded. No helper files were created here.

The more useful current cache plan stays conditional on an admitted handler, as CP143 requires. No public scheduling/guard change is source-ready solely from this compression leaf.

## Exact remaining original witness

To distinguish delivery before/after publication, retain one same-thread outer Boolean constructor frame (E and raw incoming OPI argument identities), same H construction/post record, and the **notifyInternal caller-return QtCore RVA11b32f** ->same-E/H raw4a handler6506cc2d frame, alongside source13xx base/derived table transition11e24b7 and actual cache/metaobject comparison inputs. Record whether that handler nests under compression/wake, the130ad1 callback, another poll, or occurs after constructor return; do not infer the interleaving from time or a queue count. The zero-timer child caller RVA14231a is a different source.

At the post site, **E+38 is not yet a constructed/bound operation guard** on this source order. A passive probe must retain the outer constructor's raw argument identity and defer guard dereference until its source initialization/binding boundaries. Likewise do not require the Boolean vtable or read UI fields before their publication. A normal H notify return precedes event deletion6711b33e; its later reuse must not extend the original event lifetime.

To claim first-Open Boolean object creation, additionally bind the complete Open span and actual current-widget UpdateData ->E Apply11e2870 ->E+38's resolved OPI O ->normal add return295a/2bcb. The compression/no-inline-notify source facts, direct-polish possibility and private prefixes supply none of that occurrence. Existing retained startup/Forming captures are not the missing Boolean frame.

## Bounded identities

Use CP145's original module hashes and verified DUMPBIN path. Selected code-byte SHA256 values, end exclusive:

| Range | SHA256 |
| --- | --- |
|EXE1400021c0..140002241|d6cb9fc2436200145231c4162d6e7abb547cf53df0c907baee4c88d461598765|
|QtCore6711ae90..6711b039|3e9d9e08c0592464f8f1abc87e14914a11053529346439d712e19d6e356b854c|
|QtGui6501e3e0..6501e494, selected unwind region|109a7e42d8fe4edfc9ec39c5cae71c464a6340a928df2fd738316aaeb59c8939|
|QtCore67119880..6711996b|a1d859d028e66b85857e2347ce46fec8e9066fd3de476cd0bb1edee513185e79|
|EXE140377b20..140377b46|f2fee0ba1f33bc2f03414472c4e9cad29bf4ec3d3571cff991f19fe7c481cb35|

The two-argument postEvent logical thunk is6711bfd0..6711bfd8; adjacent padding is excluded from that claim. The377b80 scalar leaf ends at377b95; later bytes in its bounded view are padding. Static source coverage remains separate from a native schedule receipt.
