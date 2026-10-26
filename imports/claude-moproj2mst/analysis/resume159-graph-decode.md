# CP159: immutable-PE populated-cell writer decode

## Result

The missing receiver-preserving insertion edge is now established. `0x140d78490` is a real model rebuilder on the exact `SfObjectManager` returned by `0x140c90210`. It clears that manager, installs headers, obtains its invisible root item, inserts a group/name item beneath that root, and delegates per-object row construction to `0x140d79750`. That delegate constructs checkable `QStandardItem`s, initializes their check states from view-state bytes, and appends the row list beneath the supplied group item. This supersedes CP156's statement that no populated-cell writer is proven.

This pass does **not** close the complete source-ready graph contract. In particular, native object filtering/name-item constructors and the selection/notification tail have not been fully translated or checked against the current source adapter. Keep the populated-cell refusal pending the narrower witnesses below. No source behavior change is proposed by this report.

The derived callback dispatch also became concrete: `0x140ddb2a0 -> 0x140ddb190`, local method zero, invokes `0x140d7b4a0(this, *args[1])`. The independent callback lane reviews its identity and side-effect order in `analysis/resume159-graph-callback.md`; this writer report does not import the old, unsupported `0x140d3d8c0` base-slot identification. Independent inspection of the actual base meta-dispatch instead identifies `0x140d41ad0` through `0x140d48ba0 -> 0x140d48a80`.

## Evidence and completion

Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, freshly verified by read-only PE parsing. Image base is `0x140000000`. This is the original DEF_GUI_MO evidence target, not a reconstructed executable.

The installed Visual Studio decoder, absent from PATH but located by the existing `admin/capture_pe_ranges.py`, completed the three requested unwind-region captures with exit code zero. Its banner is `Microsoft (R) COFF/PE Dumper Version 14.44.35228.0`; exact executable path, tool SHA-256, raw range hashes, output hashes, and resolved import identities are retained in `analysis/resume159-graph-evidence.json`. The initial producer receipt is `analysis/resume159-graph-static.json`. Every `.asm` is an immutable-target `/disasm:bytes` capture, not target execution or a decompilation.

Two optional multi-entry follow-ups stopped after successfully producing earlier body captures because a later entry lacked a `.pdata` record (`d41b30`, then `1411eebd0`). Both leaves were subsequently captured successfully using explicit bounds. Their interpreted spans end at decoded terminal RETs. Bytes beyond those RETs in the broader capture include neighboring functions and truncated final instructions and are **not** included in the logical-body claims or hashes below. All other rows use `.pdata` region bounds; these bounds do not themselves prove absence of separately located cold/exception paths.

| VA span (end exclusive) | File byte span (end exclusive) | Machine-byte SHA-256 |
|---|---|---|
| `0x140c90210..0x140c90293` | `0xc8f610..0xc8f693` | `45a2c086a9e0b063b4f52b0e614b2764bc29f7f7596cfcb6264bc12b953a8a72` |
| `0x140d78220..0x140d7848a` | `0xd77620..0xd7788a` | `a05150b977986ca86d1da8062fdc8872c5d15d2ee734877ec98acc40704e0335` |
| `0x140d78490..0x140d7974a` | `0xd77890..0xd78b4a` | `65b0218c4bbd3e131011f569523ad2f9b730f6a2b0c9d5af118601038f47957a` |
| `0x140d7b4a0..0x140d7b76e` | `0xd7a8a0..0xd7ab6e` | `f30ddf3d66bed080039c1fecd8b050cc423b39e6dd744f0fc8d48359f897d69a` |
| `0x140d3fc70..0x140d3fe61` | `0xd3f070..0xd3f261` | `bab6b8ddaaaf47c2e853a5a6dff8bbd57011bcb727c6b6ed845fe8730266dbe6` |
| `0x140d79750..0x140d7a757` | `0xd78b50..0xd79b57` | `c24049e22fa36e8ef9d2b5aa851754ff0e48e6b6cce2f5ded0ebff003b98f6b5` |
| `0x140d41b30..0x140d41b3e` | `0xd40f30..0xd40f3e` | `daf3f9da457377ae1428e2942629528d8826acc05f67cb3cbe882f112efa6a3c` |
| `0x140ddb2a0..0x140ddb31a` | `0xdda6a0..0xdda71a` | `7c3a60d257ca8b7afeb4061cfe660245111842c3849dc4751cb532442e1c555a` |
| `0x140ddb190..0x140ddb1e9` | `0xdda590..0xdda5e9` | `b0ed217a0ae721ba0cf98d990a593d617b0e18d379d5155c31fe10cc383f04bd` |
| `0x140d7ad70..0x140d7ae54` | `0xd7a170..0xd7a254` | `9020184e09c81e495904e44952b8d348292f833b41007aa69f3eda97945dca0d` |
| `0x1411eebd0..0x1411eec02` | `0x11edfd0..0x11ee002` | `edb5260a34457e4028f2be93195b2264da1bb834eb3727aa42f744772cef3d19` |

The receipt also binds the item vtable bytes `[0x14210eb18,0x14210eb40)` at file range `[0x210d318,0x210d340)`, SHA-256 `ed30b7b4327d328af5d9e63179666dce13773fd16626923b5671bb05de7613bb`. Its slot `+0x10` points to `0x140138a2a`, bytes `ff25782dcd01`, an import jump through `0x141e0b7a8` to `QtGui4_64.dll!QStandardItem::setData(QVariant const&,int)`. Slot `+8` similarly jumps through `0x141e0b7b0` to `QStandardItem::data(int) const`.

## Receiver and insertion proof

1. The retained caller bodies in `evidence/gui_mo/s1/mdt/DB/d3.c:421-477` call `manager = c90210(owner)` and dispatch `manager->vtable[0x140](manager, owner)`. These are historical static decompiles, not a fresh runtime observation. CP143's vtable inventory binds that slot to `d78220`.
2. Fresh `c90210` instructions read `owner+0x230`, lazily allocate/construct via `d78100`, store that result to `owner+0x230`, and return the same pointer. Fresh `d78220` stores its RDX owner/model argument to `this+0x50` at `d78243`. The decoded leaf `d41b30` returns the integer at `this+0x40`; value 1 calls `d78490(this)`, value 2 calls `d7a9f0(this,0)`. Mode 2 remains outside this cut.
3. `d78490` saves its incoming RCX at stack `+0x3f0`. At `d784d9`, it calls imported `QStandardItemModel::clear` on that saved pointer. `d78700` calls imported `setHorizontalHeaderLabels` on the same pointer; the repeated header-item calls use that same manager too.
4. `d79384` calls imported `invisibleRootItem()` with RCX equal to that manager and saves RAX as `root` at stack `+0x68`. At `d793b1`, arguments are `d3fc70(manager, manager->field_50, root)`. `d3fc70` preserves R8, allocates/constructs a 0x30-byte item through `d4bed0`, and at `d3fd29` calls the PE import `QStandardItem::appendRow(QStandardItem*)` with RCX equal to the original R8 root and RDX equal to the constructed item. Its return value is that new group item.
5. `d793d4` then calls `d79750(manager, manager->field_50, group)`. At `d79bfc` (null-view row) and `d7a34e` (populated row), imported `QStandardItem::appendRow(QList<QStandardItem*> const&)` receives the preserved incoming R8 group and the accumulated row list. Thus both insertion stages preserve the exact manager/root/group relationship; this is stronger than a loose call-address or class-name match.

Conditional `ROIs` and `Coupons` groups use the same root and the same `d3fc70`/`d79750` sequence at `d794b8/d794d7` and `d79588/d795a7`. Their feature predicates and returned owner objects require their own retained function qualifications before reproducing those branches.

## Row, column and value sources

`d79750` obtains a count from its incoming model at `c7c1a0`, iterates ordinal `i=0..count-1`, and asks `c82d50(model,&(i+1))` for each candidate. It skips null candidates, candidates failing `1411f1b80`, and candidates whose `1411f19f0` result is null. These three filtering calls are observed; their semantic names are not established by this pass.

The returned data object's integer at `+8` supplies the native object ID. Its addresses `+8` and `+0xc` feed the name-item constructor `d4c050`. `1411f1af0` supplies text for `d4eb00`. The first row entry is that constructed name item; optional child name items are inserted beneath it after predicates `140834f40` / `140834ee0`. Their constructors are not decoded here, so allocation and append are established without inventing their full payload/flag semantics.

`1411f1a10(candidate)` supplies the view-state pointer. If null, the name-only list is appended immediately. Otherwise plain `QStandardItem` construction is bound directly to import `0x141e0b670`, followed by explicit assignment of vtable `0x14210eb18`. The list next receives selectors 0, 1, 2, 3, with selector 4 included if the `c7c0a0` or `d0c920` model predicates enabled it and selector 5 included only under `c7c0a0`. These are row-list positions after the name entry; conditionally omitted entries must not be replaced by invented placeholder columns.

For enabled cells, imported `setCheckable(true)` precedes imported `setCheckState(state)`. Cells for selectors 2 and 3 have their own `1411f1bc0` / `1411f1c00` predicates; selector 5 has `1411fd560`. If these predicates fail the code still appends a plain item but calls `setCheckable(false)` and skips its role/check-state initialization. Selectors 0 and 1 use the unconditional checkable path. The selector-4 branch is enabled by model conditions as above.

Decoded `1411eebd0(view,j)` returns zero for `j<0`; otherwise it reads `view[8+j]`, returning 2 for a nonzero byte and 0 for zero. The result feeds `QStandardItem::setCheckState` directly. This is an exact byte-to-state rule, not an assumption that arbitrary Boolean fields or serialized display options use the same layout.

At `d7ad70`, the enabled plain items receive `QStandardItem::setData` in this order: role `0x21` carries a QString returned by `c91310(model)`, role `0x22` carries the native object ID, role `0x23` carries the selector. The name item receives selector `-1`, but its subclass vtable and overrides still need independent qualification. Each metadata helper call destroys its by-value QString argument. The role reader/writer callback lane independently checks the downstream meaning of these roles.

## Remaining source-ready witness

The next task no longer needs to retry `d78490` or ask whether it inserts items. Reuse the completed captures and narrow the source contract to:

- The concrete name-item constructor `d4c050` and its vtable/`setData` behavior, plus `d4bed0` for the parent group. These determine whether the already-observed writes have additional subclass effects.
- The candidate count/lookup/filter chain `c7c1a0 -> c82d50 -> 1411f1b80/1411f19f0`, and the exact view pointer `1411f1a10`. A bounded populated example must bind these objects to the converter's current owner/view representation.
- Actual derived/base callback effects and order from `analysis/resume159-graph-callback.md`, followed only by its unresolved concrete delegates. Do not retain the earlier mistaken `d3d8c0` base-slot label.
- Reachable post-population selection/notification effects: `d7b190`, `d7acb0`, `d7abe0`, `d7ae60`, and the global-observer calls in the `d79750` tail, using branch predicates to choose the smallest necessary subset. This pass establishes call sites and input flow, not their full behavior.

No original process, live REA session, GUI session, mutable analysis database, C++ source, build, test executable, deployment, or existing evidence artifact was modified. New files are restricted to these analysis receipts, script, captures and report.
