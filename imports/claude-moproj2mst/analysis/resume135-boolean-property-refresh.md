# CP135: a separate postLoad property notification has a bounded private prefix

This pass examined a different source leg, rather than repeat the timer/item/style audit. After tree refresh returns, native views1400547b0 calls140054df0 at0547db. That leg emits SfNotifier's **signalRefreshProperty(SfBaseProperties*)**, not widget-area UpdateData. Its concrete registered receiver is SfPropertyEditor. The new source facts support an isolated notifier/slot ordering prefix, but do not identify that receiver with Boolean editor E or establish Apply/add during Open.

Only this receipt was written. Bounded immutable original PE/DUMPBIN reads were used; no GUI, original execution, mutable REA/import, source changes, tests or builds occurred.

## New source and owner chain

1. Accessor140051230 reads its receiver+10 at05123a and returns.140054df0 calls it at054dfe, forms result+10 when non-null (otherwise null), and calls notifier1402ce9c0 at054e3f with global143d76700. These mechanical pointer reads do not equate the payload with Boolean editor E or Apply's OPI.
2. Notifier2ce9c0 reads its actual owner+141 at2ce9d3. Nonzero returns without emission. Otherwise2ce9e8 calls3d09f0, which supplies SfNotifier metadata143d94cb0 and local signal index3 at3d0a16, then imported QMetaObject::activate at3d0a28. Metadata table14234dbd0/strings14234dcc0 identify index3 as signalRefreshProperty(SfBaseProperties*); local5 is the distinct RefreshTree signal.
3. The direct LEA carrier of string142e40210 (`2signalRefreshProperty(SfBaseProperties*)`) is1413726e4. At13726f2, initialization13726b0 calls QObject::connect with its actual this as receiver, slot142e401e8 (`1slotRefreshProperty(SfBaseProperties*)`) and raw connection type3.
4. This Qt4 binary normalizes that raw type: QObject::connect671315c0 zeros R13d at671315ee, compares input with3 at3162c, conditionally replaces it with zero at3162f, and stores it at31633. Do not label3 a blocking connection or infer immediate delivery from registration. The resulting Auto route still depends on actual thread/connection/lifetime state at emission.
5. Constructors1372360/13724b0 install public QObject table142e40978. Its qt_metacast1377c50 names **SfPropertyEditor** and ISfPropertyEditor. Its qt_metacall1377d40 selects local dispatcher13779d0; local0's table target1377a2c calls slot1376590 at1377a3d. This closes the class/slot identity independently of the Boolean widget's unrelated vtable.
6. Slot1376590 obtains its actual virtual-base interface and calls interface virtual0 at13765d4, passing the payload and R8d=0. On the inspected installed interface table142e40940, that target is adjusting thunk13779c0 ->1375950. Keep primary receiver, interface and private implementation as distinct identities; do not substitute literal offsets for arbitrary derived objects.

## Slot ordering and first unclosed callback

At1375950, the adjusted interface receiver's field-10 supplies private implementation D. It calls **[D+8]'s browser clear1413dd960 at1375979 before storing the incoming properties pointer** to D+18 at137598c. Thus no helper may commit the new payload at slot entry or before clear returns.

After that normal return/store, null payload branches at1375990/5998 directly to epilogue13759ec. Non-null payload obtains a property-list value through353770/0ac450, calls refresh1373e60 at13759dc and destroys its temporary at13759e7. Browser clear itself has calls/iteration; its callback effects are not represented as completed merely by naming the helper. Refresh1373e60 also has many downstream callbacks, starting with a live main-window getter2c8090 and its virtual+260 at1373eb9. Complete transitive absence of Apply through those bodies is not claimed.

This route supplies a different missing dependency from the tree timer: actual emission from the reached0547db leg with notifier suppression permitting it, a live SfPropertyEditor registration/thread-selected dispatch, and the actual slot/browser-clear return frame. Even such a frame would establish property refresh, not Boolean Apply. A positive Apply claim still needs a nested current-widget UpdateData ->Boolean E ->resolved OPI ->normal add return295a/2bcb before the same complete Open span ends. Existing retained Forming prefix is not that witness.

## Proposed source cut for the next checkpoint

The smallest safe implementation is an **isolated private property-refresh prefix**, with no Session/public Open/Save/Apply wiring. Proposed exclusive new files are include/m2m/engine/property_refresh_prefix.hpp, src/engine/property_refresh_prefix.cpp and tests/mdt/test_property_refresh_prefix.cpp; no files were created in this checkpoint.

Model two explicitly separate boundaries:

- Notifier admission: actual owner/lifetime, known suppression byte and admitted nullable payload produce either Suppressed or a local3 emission request. Unknown suppression/payload remains guarded. The request does not execute Qt, manufacture connection delivery or change an editor.
- Slot admission: require a separately admitted actual receiver/interface/private/browser owner mapping and delivered payload. Request browser clear first and suspend. Only a matching explicitly admitted normal clear completion permits this frame's D+18 payload write. Null then completes this prefix; non-null stops at a distinct property-list/refresh continuation before13759a4. Model no browser callbacks, successful renderer or Boolean effect by default.

Focused Release-active tests should cover unknown inputs, suppressed emission, emitted-but-undelivered request, no prefix-owned write while clear is pending, mismatched/expired receiver completion, normal clear-return-before-write, null completion, and non-null guarded continuation. Preserve callback/reentrancy as unrepresented unless its actual state/ordering is separately modeled; do not promise the global old value stayed unchanged during an external callback. A declared candidate prefix test is not an original timing witness.

This cut is source-ready as an isolated guarded model. It does not resolve actual first-Open timing. Root must assign exclusive files and review its interface before implementation; public PostPending and owner guards remain intact.

## Bounded reproduction

Use CP120's immutable EXE/QtCore hashes and verified DUMPBIN14.44.35228.0 path. Selected code-byte SHA256 values:

| Range, end exclusive | SHA256 |
| --- | --- |
|EXE140054df0..140054e49|1caa657f7bb3696e71c0a256d0781790650eda380d7353c17122deafa4a43fe0|
|EXE1402ce9c0..1402ce9f2|354c86084f50db93b100d881734e25b65be7deb00e8dea058089d63b3db8a083|
|EXE1403d09f0..1403d0a33|1314819a7fe45f3cf153355879a408c11bf29a00d16a95777859324342be7b5a|
|EXE1413726b0..141373452|f02dfd591ad7eed98a5698915955c5dcadbaa7891f03762d72a67d2271cae7d8|
|QtCore671315c0..67131ca6|8a62dc27d8c709f463fd33364cd47c06f1491c921ed6d900211e775bd18a966f|
|EXE141376590..1413765db|833e0edc80efe47f83f8418a1b6d7234e6a3cb5c92383bd0a755432eb3b66acd|
|EXE141375950..1413759f1|e24cf2a0fb96de0c00f4016f0b3a3a6b38e6be3fa6130e4d5ca43a72d6cb9cfb|
|EXE1413dd960..1413dd9ea|150280abead694318fa98d6d8aa573d3863a063b1d017fb2c1001a9b6d527edb|
|EXE141373e60..14137593c|d1ca7bbee4679f1c9b673f4afaffdcc300ca5606fcde9d0cfe652b5ceae99e12|
