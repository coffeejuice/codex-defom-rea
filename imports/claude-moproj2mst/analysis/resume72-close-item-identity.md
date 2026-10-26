# CP72: private allocation, status item identity and destructor admission

The static constructor chain identifies a possible cache producer, but does not bind the actual late-Close cache target. A further native removal path confirms why the layout's current status slot cannot supply that missing identity: it can return an item and clear layout+4f8 without deleting the item or clearing private+c0. Conversely, layout destruction can delete a still-owned status item before a separate widget/private destruction. The item allocation must therefore have an identity/lifetime independent of whether that one layout slot currently holds it.

Stop at the unresolved edge `actual private+c0 -> canonical live item allocation/writable+10`. Neither a current status-item projection nor its later null slot establishes that edge. The earliest safe isolated destructor binding is the actual base-private entry/phase plus a consumed read of that allocation's cache cell; a nonnull target must resolve before65067701 can write through it. Public WidgetState liveness is not that private lifetime qualification.

Only this report was written. No source/build/test execution/GUI/original execution/mutable REA activity occurred. Method: retained CP65/67/69/70/71 evidence, current source and bounded offline PE/RTTI/DUMPBIN. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`. No reconstructed executable/deployment was inspected or changed.

## What the constructor establishes, conditionally

Let W be the actual status widget, P its private allocation, I the newly constructed item and L the actual status layout. These names describe one source occurrence, not invented candidate addresses.

1. QStatusBar65406d70 allocates P separately from public W. QWidgetPrivate(int)6505e7e3 writes P.cache=null at its own constructor event. Protected QObject6712e655/661 later binds W.public+8=P and P.public+8=W, before complete QWidget/QStatusBar construction. A late existing W does not acquire constructor-default cache state retroactively.
2. Selected status setter653cc1f0 allocates I and calls QWidgetItemV2650494b0 directly at653cc249; this direct path bypasses the general QLayout item-factory hook. I.widget=W is stored at650494da.
3.65049543/547 reads that actual I.widget and its actual public+8 P.6504954b reads P.cache. Only null executes65049555's P.cache=I store. A nonnull cache J is preserved, including when J differs from I.
4. Only after the constructor returns does653cc256 adopt I into L.status(+4f8). Constructor-frame retention precedes layout adoption. The borrowed cache link must not become an extra owning layout or item allocation.

The conditional equality `P.cache==I` is established at the publication store, with the preceding P.cache=null branch qualification and same actual P/W/I identities. It is not established merely by L.status=I or I.widget=W. Exported useSizeCache65049160 independently compares I.widget.private.cache against this exact I. CP71 records that complete leaf and all eight validated direct constructor xrefs.

At destruction, the relevant cache value must be current at that destructor occurrence. Even a previously proved publication does not close intervening writes, item deletion, transfer, copying or reentry. A qualified current P.cache read can replace the need to reconstruct its entire history, but still needs the actual target allocation/field owner for a nonnull value.

## Additional ownership boundaries recovered here

A bounded byte-displacement search in QtGui653c0000..653d0000 found+4f8 candidates. Disassembly from the containing unwind entries qualified the following branches; byte hits alone were not treated as instructions. This bounded search is not global field-xref completeness.

**Indexed retrieval/removal:** primary table657036e8 has+60 ->653cc140 and+68 ->653cf150. Its MSVC RTTI complete-object locator657c3ff8 names `.?AVQMainWindowLayout@@`. The retrieval method's selected final branch loads+4f8 and returns it only for the matching index. The removal method's selected final tail is:

```text
653cf30b  rax = [layout+4f8]
653cf312  test rax
653cf317  compare selected index
653cf31d  [layout+4f8] = 0       matching nonnull branch
653cf328  return retained rax
```

`r12` is set zero at653cf171. This selected tail contains no item deleting call, item.widget write or widget.private.cache write. The returned item therefore cannot be declared dead/null merely because the slot was cleared. Earlier subordinate indexed-removal/layout work in653cf150 has its own effects; the claim here is the directly qualified final status-item branch, not full transitive removal parity.

**Layout destruction:** the same RTTI-qualified primary table's deleting slot+18 targets653d0320. That provider calls653cfe30 and conditionally frees the layout allocation for flag&1. The destructor installs the same main-window-layout tables, runs prior subobject/item cleanup, then:

```text
653cfe96  rcx = [layout+4f8]
653cfe9d  test rcx
653cfea0  null skips
653cfea2  rax = [item.vtable]
653cfea5  edx = 1
653cfeaa  call item.vtable.slot0
653cfead  next layout member cleanup
```

It does not clear layout+4f8 before this selected deleting call. For the qualified actual V2 item profile, CP66's deleting destructor clears its widget.private.cache only if that cell equals this item, then frees only the item. It does not delete the QWidget. A different cached item is preserved. If the item was previously removed from the slot, this null slot does not recover where that item went or who later deletes it.

These two boundaries close concrete transfer/deletion distinctions. They do not prove which one is reached during accepted one-project Close, the actual main child destruction order, or which item/private allocation survives that lifecycle. Main-window/layout teardown cannot be inserted into Project Close based on these static destructor bodies.

## Earliest safe destructor binding and exact stop

For the isolated base-private prefix, use P's actual destructor occurrence, not a fresh global-main lookup or an assumed current status slot. CP70 qualifies QObject's late re-read of public+8 before private deleting dispatch and its null store only after the provider returns. The actual QStatusBarPrivate provider first destroys instance QString+1e8/list backing+1e0, then enters QWidgetPrivate::~QWidgetPrivate650676d0.

The isolated contract can start at this qualified actual P/base-private entry without requiring W's public guard to remain live. It must preserve the distinction between public destruction, P still allocated, private payload phases and eventual P free. A held C++ lease alone supplies none of those native phases.

1. Require the same canonical P allocation and a source-qualified entry/phase. Do not certify preceding status-private payloads by a standalone success Boolean.
2.650676f2 commits P's QWidgetPrivate base-class transition.650676f5 consumes P.cache once. If the cell is Unknown, stop here; do not roll back the class phase or substitute null.
3. Observed null skips the target write. Observed nonnull I/J must resolve to its actual still-allocated writable item+10 field, independent of L.status and independent of old item.widget knowledge. Unresolved/expired nonnull stops before65067701.
4.65067701 writes that consumed target's widget field null. No old-backlink equality test, item deletion or P.cache=null store is performed. Stop before65067709's remaining private extra/member work. Retries retain the consumed target; later slot/cache replacement cannot retarget the completed occurrence.

The first unresolved current-source edge is earlier than step1: WidgetState has no actual private allocation/phase owner or+c0 cell, and CloseStatusItemOwner has no common native item allocation/lifetime shared with such a cache. Current Session has no production status-owner acquisition; its nonnull prefix still guards before QObject::metaObject/cast. The static producer chain proves how an implementation must bind identities, not that candidate metadata currently has them.

The required minimal graph is:

```text
canonical W --qualified public d_ptr read/store--> canonical P allocation/phase
P.cache --consumed raw nonowning identity--> canonical I/J allocation
canonical I/J --same field storage--> writable widget+10
constructor frame or actual layout/caller --separate ownership role--> canonical I/J
```

A separate status/cache duplicate, a weak target expiring, a nonblank label, a native-alive flag copied from W, or a null L.status cannot supply a missing graph edge. Opaque nonnull cache identity can support a comparison/preserve branch, but cannot authorize a write through it. Do not widen the claim to a process-global cache: MessageArea's global QString and status-private instance fields remain separate.

## Smallest next action and validation

No further public Close/destructor effect is released here. The next root-owned implementation prerequisite is shared canonical item/private allocation storage with explicit Unknown cells and independent lifecycle/ownership roles. Bind at the actual constructor publication/adoption events or an actual current private/cache read. Keep the existing public status cast/constructor and prior-cleanup guards. If those owners/producer events remain unavailable, stop at that missing edge rather than expanding a status observation into native construction.

Focused validation should include the new removal case: a live cached item returned from L while L.status becomes known null; private.cache remains the same nonnull identity and the returned caller/frame retains the allocation. Contrast actual layout-owned item deletion, where only a matching private cache is cleared. Then test cached item different from current status item, unresolved nonnull target, public-dead/private-live phase, private prefix exact target write regardless of previous item.widget, retained private cache, and retry identity. These tests must distinguish diagnostic field observations from executed native effects. No tests were created or run in this lane.

## Receipts and remaining evidence

New bounded disassembly/metadata is retained inline above; no raw file was added. DUMPBIN14.44.35228.0 from MSVC14.44.35207 supplied views from the qualified entries. An exploratory slice beginning653cfe80 started mid-instruction and is excluded; claims use the later full-entry653cfe30..653cffc1 view. The RTTI pointer/type string and fixed table slots were decoded from PE data, not interpreted as machine code.

| Region, end exclusive | Code SHA-256 | Scope |
| --- | --- | --- |
| 653cc140..653cc1a5 | `608c32070f9c606d966884c856bedef4200e53d00815a524be320bd027699d34` | Indexed retrieval; selected status tail. |
| 653cf150..653cf33b | `a46932b241966f2e1f083072da2efdb7a4d83b96345f15712e9156fcdb5edb2b` | Indexed removal; selected status return/clear tail. |
| 653cfe30..653cffc1 | `7788d2045deb88a2c5f8d51ad3dbff3e429c9a65d73827219b44c30f3721e8d0` | Main-window-layout destructor direct body. |
| 653d0320..653d034f | `d61c7448611a45d693daf0b855f4c9b1ac9e59597720822f437d0cb2824cc129` | Deleting provider, excluding alignment padding. |

CP65/67/69/70/71 retain the item producer/cache uses and status/private lifecycle source qualifications. Missing evidence remains the actual late-Close W/P/L/I/J aliases and lifetimes, current cache value, selected producer/removal/deletion occurrence, intervening/reentrant changes and actual destructor reachability. The static paths do not close that final binding, and do not qualify normal-Close cache reset or blanket process cleanup.
