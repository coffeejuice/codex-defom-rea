# CP159: independent SfObjectManager callback and role contract

## Result and limit

The derived item-change callback is now concretely bound: `SfObjectManager::qt_metacall` at `0x140ddb2a0` delegates local method ID 0 through `0x140ddb190` to `0x140d7b4a0`, preserving the same manager receiver and passing `*args[1]` as the changed item. Its role lookup and state-write order are recoverable from complete bounded native instruction regions. This closes the missing derived callback identity from CP143/149/156, but does **not** admit a populated RGRAPH property-table path: a QStandardItemModel callback alone does not establish the QTableWidget/view owner, generation, refresh boundary, or row mapping required by CP131.

There is also a material correction to CP143/149: `0x140d3d8c0` is not the base item-change slot selected by the recovered SfItemModel meta-call. The actual local ID 0 route is `0x140d48ba0 -> 0x140d48a80 -> 0x140d41ad0`. That last body only constructs and destroys `indexFromItem(this, item)`. The separate `d3d8c0` notifier body must not be applied as this slot's state effects without a different concrete edge.

## Identity and evidence

Original reference PE: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. This is immutable original DEF_GUI_MO evidence, not a reconstructed executable. No original program was executed.

`analysis/resume159-graphcb-verification.json` independently maps PE32+ sections/imports/unwind ranges and records raw machine-byte and retained-assembly hashes for 18 ranges. New captures use `admin/capture_pe_ranges.py` with dumpbin 14.44.35228.0; all capture commands completed successfully. Captures are unwind regions ending in RET, not a guarantee of unrelated cold/exception-path coverage. The graph_decode worker's separately owned `analysis/resume159-graph-evidence.json` binds the plain item vtable `0x14210eb18`: +8 forwards to imported QStandardItem::data, +0x10 to QStandardItem::setData. Its ctor assignment is retained at `0x140d79c3a/0x140d79c41`. I read that receipt without editing it or repeating the d78490 writer investigation.

| Entry | End exclusive | Machine bytes | Raw machine SHA-256 |
|---|---|---:|---|
| `0x140d41ad0` | `0x140d41b03` | 51 | `18bb5e6785c3a8cd19c3f0264b74f856c274c3b8e2eb58b7251981b91f93086b` |
| `0x140d7ae60` | `0x140d7b188` | 808 | `f2635621c340cb896306671ef86379a44d46587414026131267654c0b1844cc7` |
| `0x140cff450` | `0x140cff4bb` | 107 | `6bd6b6912e66bd41abca13ec99f77dc3b071672f38561e635f34aadae9d34be7` |
| `0x1411eec10` | `0x1411eec66` | 86 | `f9d382ca8db480b7eb97dd2728558319ffaee3022fe4d59611a2776d76f617df` |
| `0x140d7b4a0` | `0x140d7b76e` | 718 | `f30ddf3d66bed080039c1fecd8b050cc423b39e6dd744f0fc8d48359f897d69a` |
| `0x140ddb2a0` | `0x140ddb31a` | 122 | `7c3a60d257ca8b7afeb4061cfe660245111842c3849dc4751cb532442e1c555a` |
| `0x140ddb190` | `0x140ddb1e9` | 89 | `b0ed217a0ae721ba0cf98d990a593d617b0e18d379d5155c31fe10cc383f04bd` |

The independent receipt also identifies imports by original PE IAT, including `indexFromItem` at `0x141e0b6b0`, `itemFromIndex` at `0x141e05bc8`, `checkState` at `0x141e0b6b8`, `setCheckState` at `0x141e09240`, QVariant::toInt at `0x141e04658`, and QModelIndex row/column/sibling. These are byte/import identities, not names inferred from port source.

## Receiver and role lookup

`c90210` allocates a 0x60-byte manager only when the owner object's +0x230 is null, calls `d78100(newManager, owner)`, stores it at owner+0x230, and returns that pointer. Constructor `d78100` stores the same owner at manager+0x50. Thus the callback's `this+0x50` is the concrete constructor-supplied owner, not an inferred graph owner. The base constructor `d3d590` registers `itemChanged(QStandardItem *) -> slotItemChanged(QStandardItem *)` first; the derived constructor registers the same textual pair later, each with raw connection-type argument 3. The base/derived meta-call bodies prove their respective method-ID routes. Native registration order does not, by itself, prove event-delivery timing or callback interleaving; no such claim is made here.

Role writer `d7ad70(this,item,ownerName,objectID,selector)` constructs variants and sets roles in this order: 0x21 from the QString ownerName, 0x22 from integer objectID, 0x23 from integer selector. All three calls preserve the item receiver. Reader `d7ae60(manager,index,&resolvedOwner,&selector)` first reads manager+0x40 via `d41b30`:

- Mode 1 obtains `itemFromIndex`; a null item returns 0. It reads 0x22 to integer ID, then (if requested) 0x23 to selector, then 0x21 to QString owner name.
- Its owner-name predicate-true branch returns the constructor owner through `resolvedOwner` and gets its object bound through `c7c220`. The predicate-false branch obtains `[constructorOwner+0x10]` through `c7ae00`; when non-null and an owner output is requested, `cfa040` resolves the named owner through its +0x1d8 collection, then gets that owner's bound. Failure keeps bound 0. The predicate calls `0041b50` with the byte loaded through Qt data import `0x141e04190`. This looks like an empty-name decision, but that interpretation remains an inference until the QString helper and imported byte are bound; the branch flow itself is established.
- It returns ID only if `ID > 0 && ID <= resolved bound`; otherwise it returns 0. The read selector can already have been written even if the ID fails that check.
- Mode 2 is a separate branch: `item(index.row(),0)` is dynamically cast and `d4ea70` supplies the returned ID if cast succeeds. It does not perform the mode-1 owner-role lookup. Other modes return 0. No RGRAPH meaning is assigned to mode 2 here.

## Derived callback state order

The concrete order in `d7b4a0` is:

1. Return immediately if manager+0x50 is null. Otherwise form `indexFromItem(manager, changedItem)`. Initialize resolved owner to null and selector to -1; call `d7ae60`. If lookup leaves owner null, substitute manager+0x50. Return after index destruction when returned ID <= 0.
2. Read the changed item's checkState, then call `c832d0(resolvedOwner,&ID)`. A null result returns after index destruction. `c832d0` validates positive/in-bound ID against resolvedOwner+0x20 and returns the element at ID-1; its later append loop is retained but is preceded by that bounds gate. Do not turn it into unconditional object creation.
3. If selector >= 0, call `1411eec10(objectState, selector, checkState)`. It writes byte `[state+8+selector] = (checkState == 2)` and then calls `1411eea80(state,&selector)`. That normalization forces byte +9 to 1 when selector 0 becomes true; when selector 1 becomes false it forces byte +8 to 0. Thus enabling selector 0 implies selector 1 enabled; disabling selector 1 implies selector 0 disabled. There is a lower-bound guard, no recovered upper-bound guard. Do not add an upper bound to the faithful contract based on this body.
4. Obtain `[constructorOwner+0x10]` through `c7ae00`, using the original constructor owner even if role resolution chose another owner. If this linked pointer is non-null and selector >= 0, call `cff450(linked, ID, selector, checkState)`. This happens after the first object's state normalization. `cff450` first calls eligibility predicate `d0c920`; only if true it obtains a linked owner through `cf9960` (+0x1b8 -> c1aef0), performs `c832d0(linkedOwner,&ID)`, then invokes the same state setter when that result is non-null. The complete eligibility predicate's downstream functions and the linked owner provider's internals are not fully closed by this pass.
5. Read the index row and column. For column 2, obtain sibling(row,1), convert that index to an item, read `1411eebd0(objectState,0)`, and call siblingItem.setCheckState(result). For column 1, do the symmetric update to column 2 using state selector 1. The getter maps zero byte to checkState 0 and nonzero to 2. Other columns skip this synchronization. The visible body has no sibling-item null check and no signal-blocking call around setCheckState. Consequently reentrant delivery cannot be erased by assumption.
6. Call `2ca890(0x143d766b8)` after any sibling update (or immediately after column handling for other columns), destroy the index, and return. The retained historical decompile calls this routine size 1; this pass preserves the call without inventing an observer effect.

## Remaining exact witness

A source implementation still needs the existing QTableWidget/view owner and generation boundary, correspondence of native manager rows/roles to that widget's View order, and the full refresh/finalize path's callback timing. The native callback contains owner resolution and linked-owner propagation; reducing it to a checkbox boolean write would discard concrete behavior. Conversely, admitting the public path solely because this callback is now identified would cross the unchanged CP131 ownership gate. Preserve the populated-cell refusal pending that owner/state bridge. The independent writer report owns d78490's receiver/insertion proof.

Only analysis artifacts were created. No source, build, tests, executable bytes, original fixtures, GUI session, live REA startup, or mutable analysis database were changed. Token measurements are unavailable; no speed/token-savings claim is made.
