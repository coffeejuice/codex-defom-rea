# CP81: original Report document release path and Close boundary

## Finding

The original PE closes the Report document's release path inside SubProject cleanup. The `SubProject+0x30` slot is a QPointer; when SubProject cleanup runs and the guard is still nonnull, it explicitly invokes the pointed Report document's deleting virtual (`vt+0x18`, flag 1), then stores null. Project teardown iterates `Project+0x58` subprojects and calls their deleting wrapper, so this cleanup is nested in Project destruction. This is the first source-level evidence that SubProject actively releases the Report document, rather than merely observing a main-window-owned object.

The retained accepted normal-Close trace still does not bind its entry to this Project/SubProject destructor chain. The Release path is recovered; whether accepted one-project Close invokes it before main-window teardown remains unproven. Preserve the public Report guard and do not implement adapter cleanup from this receipt alone.

## Bounded PE disassembly

All disassembly below uses the immutable `evidence/original-binaries/DEF_GUI_MO_64.exe` (SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`) and the verified Microsoft DUMPBIN 14.44.35207 executable at the path in `analysis/resume81-native-analysis-tooling-inventory.md` (tool SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`). Ranges were bounded to function RETs or the exact relevant instructions.

The SubProject cleanup body is `0x1400b58d0..0x1400b59dd`. It reads QPointer fields at `+0x48` and `+0x30`. For nonnull `+0x48`, it calls the pointed object's deleting virtual `+0x18` with `edx=1`, then clears that guard. At `0x1400b5954` it calls helper `0x1400b5bb0(this)`. That helper (`0x1400b5bb0..0x1400b5c2a`) reads `this+0x30`; if nonnull, calls `(*docVtable+0x18)(doc,1)`, then calls `0x140cbc920(this+0x30,null)`. Returning to the SubProject cleanup, it deletes the raw model at `+0x00` with the same deleting-virtual/flag pattern, then clears the `+0x48` guard, destroys the `+0x40` list and `+0x38` QString, and clears the `+0x30` guard registration. Thus the explicit Report document deletion is before the SubProject model and field teardown. The SubProject field table in `source/docs/re/moproj2mst/s1-01/spec.md` independently labels `+0x30` as QPointer.

The SubProject deleting wrapper `0x1400ac330..0x1400ac365` calls `0x1400b58d0`, then performs scalar deletion when `flag&1`. Project list cleanup is `0x1400a80e0..0x1400a8189`: it gets the count from `0x1400a7360`, loops indices, obtains `Project+0x58[i]` through `0x1400a74f0`, and calls `0x1400ac330(sub,1)` for each nonnull entry. It then destroys the QList at `Project+0x58`.

Project destruction is `0x14008a3c0..0x14008a465`. At `0x14008a403` it calls the subproject-list cleanup `0x1400a80e0`, then destroys the Project `+0x60` member, `+0x58` list, QString fields at `+0x30/+0x28/+0x18`, and the trailing Project context. Project deleting wrapper `0x1400453e0..0x140045415` calls `0x14008a3c0` and frees the allocation when `flag&1`.

The main-window parent route is separate: Report document accessor `0x1400b5ad0` passes the main-window object to `0x140643100`, as already qualified in the retained constructor capture. If main-window destruction has already invalidated the QPointer, the SubProject cleanup's null check skips the explicit document delete. The static code therefore qualifies SubProject-driven deletion while the guard is live and a null-guard skip after prior invalidation. It does **not** establish which of those states occurs first in accepted normal Close, or the QObject child-deletion callback order for this particular instance.

## Static call path and normal-Close gap

A bounded direct-call scan of original `.text` (RVA `0x1000`, raw offset `0x400`, raw size `0x1e01400`) found:

- `0x1400b58d0` is directly called by deleting wrapper `0x1400ac342`.
- `0x1400b5bb0` is directly called by SubProject cleanup `0x1400b5954` and a second site at `0x1412ae046`; the latter's containing lifecycle is not classified in the retained Report excerpts.
- `0x1400ac330` has direct callers `0x1400a71fe` and Project list cleanup `0x1400a8160`.
- Project destructor `0x14008a3c0` is called by its deleting wrapper `0x1400453f2`.
- Project deleting wrapper `0x1400453e0` is directly called by `0x140044c46`, inside helper `0x140044bf0`.
- `0x140044bf0` has direct call sites `0x14001d126` and `0x1400440ce`. The first is in the retained OpenProject failure branch: after failed `0x14008a9b0`, it clears the data-root Project slot. The second is in bounded routine `0x1400440c0`, which invokes `0x140044bf0` and then `0x140045000`; its connection to accepted one-project Close is not established by the retained trace.

The retained normal-Close work at `0x1400299a0`/manager clear and the later `0x140029a83` Project lookup does not show a direct call to `0x140044bf0` or Project deleting wrapper. The bounded CP74 report expressly leaves nonempty child/Project/OPI/slot teardown unresolved. The `.text` scan found no direct relative-call reference beyond the call sites above and no raw 64-bit code-pointer literal for the Project delete/release addresses examined. This is a bounded static result: it does not exclude an indirect target, dynamically formed call, or external parent-driven QObject deletion.

Therefore, the accepted Close root remains the precise missing edge. To finish it, recover the owner/caller at `0x1400440c0`/`0x140044bf0` (or any indirect normal-Close target), prove whether accepted Close reaches Project deletion, and bind that order against main-window destruction for the same window/document lifecycle. Do not equate Open-failure cleanup or eventual application teardown with accepted project Close.

## Byte-hash receipts

Hashes cover exact inclusive code byte spans from the original PE, mapped using its `.text` RVA/raw offset correspondence:

| Function/span | Bytes | SHA-256 |
| --- | ---: | --- |
| SubProject cleanup `0x1400b58d0..0x1400b59dd` | `0x10e` | `2b35e06273c333eb3c9ee445daf5e8d27c2e54245c22236345e3bc2474beb3de` |
| Report QPointer delete helper `0x1400b5bb0..0x1400b5c2a` | `0x7b` | `2817658eebd73a75f2829881363c149b68bf4b31501a5123a9543e874806b1c9` |
| Project subproject-list cleanup `0x1400a80e0..0x1400a8189` | `0xaa` | `17d446ec2ab4fca352f9353858a184ae63a07c6d1f1e60df0c8931b89c1d9fa4` |
| Project destructor `0x14008a3c0..0x14008a465` | `0xa6` | `d33ef435aea6da822f0aa985431d82cba18f8a18374e724d0d76030e986614f9` |
| SubProject deleting wrapper `0x1400ac330..0x1400ac365` | `0x36` | `ed4d13f5c1eb45d0762a74fdc00fdd3cd65df6fb8e5fdc9c846023c2afeef5ab` |
| Project deleting wrapper `0x1400453e0..0x140045415` | `0x36` | `8b82e0d93a439f626d57dcb2a27fb558d5d3dbc994b15b441de8c13c24db49ed` |

The `0x140044bf0` callers and normal-Close boundary are supported by the bounded disassemblies above plus `analysis/resume74-close-next-owner.md` (SHA-256 `1f47984d52dcbff0a7db6bfdc632fe5c268c216a947e1e7a734284546aefa7d8`). Existing receipts identify the SubProject layout (`spec.md`, SHA-256 `779c3a01f3bff4cb496199507f43abb1ea7414b2346a21c018193299034e02da`) and Report lazy accessor (`evidence/gui_mo/s1/mdt/L7/d3.c`, SHA-256 `6f6f7205103782d7ae11c35bf2f33245433b60b6bf8b09dea29c2572bed19582`). Tool inventory report SHA-256 is `3ee03a987ea606bb412d87fb120e898d30fed15490f935bac88846b75f6924ae`.

No native REA session was opened. This was immutable PE disassembly and static byte/reference analysis only. No GUI, source/build/test, database, or runtime state was changed.
