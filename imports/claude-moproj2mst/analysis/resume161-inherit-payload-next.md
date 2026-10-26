# CP161 RINHERIT: document payload producers and the remaining occurrence witness

The immutable PE now identifies concrete producers for all four SfSessionManager payload roles: guarded +20 is an SfItemModel, raw +30 is an SfInteractivePost, +40 contains SfPartInfo pointers, and +48 points to a separately allocated QDomDocument. The +48 allocation occurs inside the Report document constructor before publication to SUB+30. Constructor null stores therefore cannot justify an empty-document cleanup shortcut. These are conditional static producer/destructor paths; they do not establish the actual accepted-Close occurrence or a source-owned Report document.

Only this report was written. No source edit, build, test, GUI/app-control acquisition, original execution, live REA retry, database opening/mutation or deployment occurred. I applied the reverse-engineer-anything workflow using retained qualifications and section-aware offline PE inspection. No new REA Evidence ID is claimed. CP160's established manager/Project/SubProject deletion chain was not retraced.

## Guarded document+20: actual model producer

Complete accessor **140643540** reads its incoming boolean. False goes directly to the final guard read. True checks the current document+20 guard; if null, it allocates 0x50 bytes, invokes **140d3d590** on the allocation, then calls **140cbc920(document+20, returnedPointer)** at 1406435bb. It returns a fresh guard read at 1406435c9. It does not substitute its newly allocated pointer for the returned current guard. The allocation-null arm supplies null to changeGuard; this report does not assert that allocation failure is observed in normal operation.

Constructor 140d3d590 installs table **142a0f0b8** at 140d3d5bc. Its table-8 COL is 1430f2808, with TypeDescriptor 143591440 naming **`.?AVSfItemModel@@`**. Table+18 is **140d41b40**. The constructor invokes imported QStandardItemModel(QObject*) with null parent at 140d3d5a9, initializes its +10 helper, allocates a selection model, stores the latter at model+48, registers its itemChanged connection, and writes model+40=0. Its selected destructor **140d3d680** conditionally invokes the selection model's deleting virtual, then clears that raw slot, destroys +10 and calls the imported QStandardItemModel destructor. These are substantive transitive cleanup effects, not a QString-only document payload.

There is a concrete Report-side attachment route. In **1412b0ca0**, the early SUB check and mode predicate precede **1412b0f70**. The complete latter helper obtains its current SUB through 1412b1010 and calls **1400b5ad0(SUB)** at 1412b0f99. If the returned document is nonnull, 1412b0ca0 calls **140643540(document,true)** at **1412b0d08**. Thus the model can be produced on the same document returned from that invocation's actual SUB+30 accessor; it is not merely an unrelated class with a matching name. Fresh current reads and callback returns still matter. This static edge does not prove that any particular Open reached it or that its document survived to Close.

The separate complete **140643510** simply forwards its incoming pointer through 140cbc920(document+20,pointer). The bounded rel32/RIP scan found no direct reference candidate to that setter; this is a search limitation, not proof that rebinding is impossible.

## Raw document+30: interactive-post producer and default-flag qualification

Complete lazy accessor **140645320** tests raw document+30. On null it allocates 0x20 bytes and invokes **14065b260(new, current document+48)**. After construction returns, it stores the result at **14064538c**, calls **1406453c0(document, current document+30)**, and finally rereads/returns document+30. Existing nonnull state skips construction. Allocation, constructor, publication, callback/helper and final read are distinct boundaries.

Constructor 14065b260 calls QObject with null parent, installs **1424f2e18**, stores its incoming document+48 pointer at payload+18, allocates an inner 0x40 object through **14064f420(document+48,null)**, and publishes the latter through the payload's own +10 guard. The table COL 1430e62a8 / TypeDescriptor 143795788 names **`.?AVSfInteractivePost@@`**; table+18 is **14065b740**. That deleting wrapper invokes **14065b320** before scalar-free on flag&1. The ordinary destructor first calls **14065b410**; this obtains its current +10 target via 14065b3e0 and calls **14064f8e0** on it. Only afterward does the destructor perform its first/second guarded reads, conditional deleting virtual, changeGuard(null), removeGuard and QObject base destruction. The inner session cleanup is a real predecessor to the outer raw-slot clear in 140643400.

**140646370** is a second concrete producer: if a copied document+58 QDomNode is nonnull, it allocates/constructs the same payload using current document+48 and writes document+30 at **140646415**, without a preceding raw-slot-null check in that branch. It then invokes 14065b540 and eventually calls the lazy accessor even on its null-node arm. This body must not be collapsed into an allocate-only-if-null rule. The bounded direct-reference scan found no rel32/RIP candidate to 140646370; no actual Report call occurrence is claimed.

Save's **140645030** reaches 140645320 at **1406450ef only when document+2a is true**, after path work and 14064a830 return. For a newly constructed Report document, 1400b5ad0 explicitly passes fifth constructor argument 0 at 1400b5b4c; 140643100 stores that argument in +2a at 1406431d2. Consequently the Save branch alone is not proof of Report payload+30 creation. Other calls exist: the complete prefix of **1406459d0** calls 140645320 unconditionally at 1406459fc, and 140645440 conditionally reaches 1406459d0 at 14064599b. Their admitted incoming paths/current state are not established as this Report occurrence. Retain unknown current +30 rather than inventing either populated or permanently-null state.

## Document+40 and +48: coupled part/DOM owners

Complete **140646a60** allocates 0x58 and calls **140658690(new, document, current document+48, document+28, document+29)** at 140646acb. It performs ID work, calls 14064bf50 then appends through 14172d7e0, or calls 14064bec0 then inserts through 14140df40, into this document's +40 list. Later it notifies through 1402cf3c0 before returning the part. The inserted pointer is the constructor result, not a fresh independent owner. The constructor installs **1424ebf08**, COL 1430e61a0 / TypeDescriptor 1437920b0 naming **`.?AVSfPartInfo@@`**; table+18 is **14065af80**. Its +30 guard is assigned the incoming document, +50 receives the incoming QDomDocument, and +28 receives an inner object constructed by 14064f420. These aliases preserve the relation to D; they do not prove liveness later.

The cleanup loop **140646e90** uses 140004ae0 (imported QListData::size) once for its entry test, then 140307410 (imported QListData::isEmpty) on each iteration. **140c51720** obtains the selected pointer through 140c517d0, saves it, calls **1413df290** on the same list, and returns the saved pointer. The latter obtains a begin iterator through 140c51800 and calls **140066f10**, whose **140066f42** calls imported **QListData::erase**. Removal therefore precedes **1406589a0(entry)** at 140646edc and the subsequent conditional deleting virtual. Provider detach/erase internals are not reconstructed here.

**1406589a0** reads entry+28 through **140658900**, then immediately calls **14064f8e0** on the returned pointer. It occurs before the loop's entry-null test. Do not move that test before the helper or replace the helper with entry deletion. The part's ordinary destructor 1406587f0 calls 1406589a0 again before its own inner deletion. The existing retained ownership qualification identifies the inner list as pointer cells and preserves separately owned SessionRecord/Task allocations; clearing a part list is not permission to delete those retained records. That prior implementation remains a DOE projection, not an actual Report QObject owner.

For **+48**, complete **1406433a0** stores zero, then immediately calls **14064b890**. The latter allocates 8 bytes, invokes IAT **141e0c570 = QtXml4_64!QDomDocument::QDomDocument(QString const&)**, publishes the result to document+48 at **14064b913**, destroys its temporary QString and calls **14064b9d0(document)** before returning. This occurs before constructor notifier registrations and before external SUB+30 publication. An unconditionally null post-construction +48 is contradicted by the ordinary successful allocation/constructor path.

The previously identified cleanup target **1402a2bc0** calls IAT **141e0c6c8 = QDomDocument::~QDomDocument** first, then scalar-frees if flag&1. Thus 14064be50's document+48 deletion is a typed DOM-owner release, after the payload/list predecessors. Native DOM backing/refcount effects remain separate from copying a serialized XML string.

## Smallest cut and exact next witness

No new public admission or source-ready native cleanup cut follows from these bytes. The first document cleanup operation remains the source-qualified QString+18 null assignment, followed by the current +20/+30 payloads, part loop, DOM owner and QObject teardown. Current candidate Report loading stops before constructing/adopting D. An opaque owner token, constructor defaults, or the existing DOE SessionDocument projection cannot supply these providers or certify accepted Close.

The missing witness is now narrower than “unknown payload classes”:

1. Bind actual P, current SUB S, S+30 D and D's accepted QObject parent across the Open/Save occurrence. At the Report model call 1412b0d08, bind that same D to the +20 returned guard and its current SfItemModel/selection-model identity; retain any rebind or death before deletion.
2. Record which admitted call, if any, reaches 140645320/140646370 for this D; its current +48 must be the constructor/reset-produced DOM owner used by the new SfInteractivePost and its inner session. The default +2a=0 rules out assuming Save's conditional call fired.
3. Bind D+40 entries, their +30 backguards/+50 DOM aliases and current inner sessions through actual list removal, cleanup and deleting dispatch. Bind D+48 to its current DOM allocation, including resets/replacements since construction.
4. At the already recovered accepted-Close chain's fresh SUB+30 reads, determine whether this same D is still live. If nonnull, the actual current vtable/payloads select the conditional paths above; if null, identify the earlier QObject destruction/invalidation. Order this against the actual parent's child deletion. Static construction input and static destructor reachability cannot choose the branch.

For another immutable-only pass, the smallest useful target is **Report's call into 1406459d0/140645320 and 140643540**, following the actual Report load/save dispatch and its current-document returns, plus 1406453c0's post-publication effects. Do not repeat CP143's manager chain or infer null state from a scan without a full producer boundary. A same-instance runtime witness remains an alternative only after the supported GUI-control release gate changes.

## Immutable receipts

Original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Tool: DUMPBIN 14.44.35207.0 at the configured BuildTools path, SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`; section mapper tools/doe_static_pe.py SHA-256 `e949707799912f8079e58d60a2b02ca204412d381e9accba81cd52bd4ee03ca8`. All ranges below are half-open and hashes are mapped original bytes. Complete functions were read from entry through RET; selected prefixes do not claim whole-function closure. Import names use matching original-thunk/IAT indices.

| Range / selection | SHA-256 |
| --- | --- |
| 140643510..140643536 +20 setter | `b53ddccb0ad3cf3da3ce5d9e3322f38f60643b1b44b511cfca4812043a39b8c4` |
| 140643540..1406435d3 +20 producer | `8859c1e83da7467623e21fb5b054e969afb5be2c5d261a0891df81b60d5d6135` |
| 140d3d590..140d3d66e model constructor | `34b59bab3e6c8c0c0c0abaf5a4482fb187088e23df53f3561f359380da5376eb` |
| 140645320..1406453b1 +30 producer | `e1d95b774193176c5e292af8880c22488ae224cfbba34d684bf2c72187d6c884` |
| 14065b260..14065b312 interactive constructor | `c0e2abc19d6ad85e79d94e129b2b2ff45f1e3dbb3bc84c4ba1cb2dbc3a84c71f` |
| 14065b320..14065b3d8 interactive destructor | `659be546efa4d813e281064e8b895c71f070cab0de5bdae298094033a258d9bb` |
| 14065b410..14065b430 inner cleanup call | `43d5a4ed8f65381272d4e04e53a6d9376c268ae3cec73c64c288c4fab470e0ea` |
| 140646370..1406464c5 alternate +30 producer | `7ccf8f5b470131a009d3128f2c28b6911172ea9a27ae196faf8a96688c8c6a8b` |
| 1406433a0..1406433f5 initializer | `376871fe0b8bf6779d1e4ac1b38f8537f6c79788629cbd116866b4335c98f345` |
| 14064b890..14064b941 +48 producer | `b8bcabdd27f6ab7797b532dc24b9c8eb5f8ceb902040a8c0acdb490c7bbaf889` |
| 1402a2bc0..1402a2bf7 DOM deleting wrapper | `cc688a8afbbf1e074dd9934d3640f0f3d8854a45d51aad755a76b2b8cedb1554` |
| 140646a60..140646dc8 part producer | `8661df5b9ccb4dc9df5e61b8b6988033fe267d4a371a002a2d45f41578ddea07` |
| 140658690..1406587ea part constructor | `46d92c423121a24300bc08f6a0b0c5beee63fc5ba0cf9b4f9ce6af661c9426f3` |
| 1406589a0..1406589c0 part cleanup bridge | `35b864af1465c5c741fd10a22daf5066198658469372601caaba888a5a755840` |
| 140658900..14065891c inner guard read | `29b1f10a937c22e5e21098a155a06c09e4c7b9a7a7bf8ff741d1ff0dcba52e38` |
| 140c51720..140c5174f pointer take helper | `03ca6186c5a4b034e478f6c975dd3a1a1d6772f6aabbf9f2716208826d025a09` |
| 140c51800..140c5183a begin helper | `2846d74f5f51764f741dea79fee2763a8e33fa5c344a0633e2795337ffff4a14` |
| 140066f10..140066f5f erase helper | `22019470998b21a3b6298feb1b34d58c8b90d8668a9cc5757265d4182fde452d` |
| 1412b0ca0..1412b0d26 Report selected prefix | `c532604d2498d62fd4040d385fddbc5060739561742c1ca60971c0c76a6d1974` |
| 1412b0f70..1412b0fb9 Report document accessor | `b5e2bd6df3a5cfc68b8a3171aa2ef1e2decd032cd98b3aaa3efa12978aab6aa4` |
| 140645030..140645110 Save selected prefix | `1fba32e9be81b8435b42865f773e812e16e0bf759e7a7f0724eedeac714c99b5` |
| 1430f2808..1430f2820 SfItemModel COL | `861223eb2f5831f29a3a63e56b6ddf57f649870d4cb9f9c407ddc26b98912b0e` |
| 143591440..143591462 SfItemModel type descriptor | `e302558372fa732c28a3c00abf470f7ea92ee55534afc460dff759e37d7f92c6` |
| 1430e62a8..1430e62c0 SfInteractivePost COL | `0e5712b9ea9029b7587f367e36aebfb900b9a793db802fe07ac6402c7b2c4d6a` |
| 143795788..1437957b0 SfInteractivePost type descriptor | `1f79c77f4c83a537297958cfaaf3f4f90ee2b40823f0ba86fa89774f462f373f` |
| 1430e61a0..1430e61b8 SfPartInfo COL | `17e9dcfc32a19b5544ffc8b668445222400aa34e45b90f20c58789195a17d881` |
| 1437920b0..1437920d1 SfPartInfo type descriptor | `27b2775ff43f9183e96d60aa712b090ed45d72692d0d1f7281d9487b967c2576` |

Read qualifications: CP160 current SHA-256 `f80e719ca754ad29d55e75c08077949eef33a556ed46ec77668f320d3de1c99f`; all-session ownership implementation `82ff77bae784bfc93cd7611b84d8ee06b4d7fcfeba88f49cbb7f6d0ed97cc062`; record creation-order decompile `d56b406a1eb639641f5b9db36776e27c54fee60f6d5682a28414665e7efef342`; root-task-IO decompile `fe234e79d42ea999d4067d27bea3c99eb876f29321f87d9c122ecaa7e8e6f6b5`; inner-clear decompile `4d1299dca502deae183baaa249502fe5a28656fbe9000574fe615763f3017610`. The retained decompiles describe private snapshot analysis with unresolved indirect calls; they are hypotheses corroborated only for the bounded bytes above, not new runtime observations.
