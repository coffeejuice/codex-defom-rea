# CP292 CurveItem width owner and initial-bind chronology

The first successful ordinary 2D public bind has production-owned binary32 width `0x3d0f5c29`, exactly promoted to double as `0.03500000014901161`. Public bind destroys and recreates CurveItems, so prior width configuration cannot survive that operation. This supports an owned float initialized with `0.035f`; it does not close the render transform or public color contract.

## Evidence and qualifications

Read-only DUMPBIN 14.44 bounded disassembly used retained original DEF_GUI_MO_64.exe v11.0.2 at `evidence/original-binaries/DEF_GUI_MO_64.exe`, independently rehashed SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Tool: `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`, flags `/nologo /disasm:bytes /range:start,end`. Version qualification is historical CP257/262/274 and CP286/287, not newly recovered Windows metadata. PE-section mapping independently decoded constants and range hashes.

Retained Ghidra-style exports provided interpretation, not original C++ source or runtime observations: `evidence/gui_mo/s1/mdt/V/v0.c` SHA-256 `e41a4f1aa3fa5f13fb78881c01c6d1e833fd134b1748e027a9b9c2dab2881f10`; `v1.c` `b343633d23fa2283e259a36e7db535c83501fda73c8556be79b6fe83892814e7`; `v8.c` `080aa7aa18b0a1663c1d0701c14a93e905d303e6ebd632b2cc5226634c7e55b6`. Key effects and instruction boundaries below were checked against immutable PE bytes. Direct-CALL/LEA scans using `tools/save_diff_pe_xrefs.py` were discovery hints followed by disassembly, not proof of complete indirect-xref coverage.

No original process, GUI control, Frida, mutable REA database, source change, build or test was used. Recovery/gui status tools were read-only; the GUI release gate remains blocked. This report concerns the converter and original GUI_MO target, not reconstructed DEF_PRE.

## Destruction, recreation and first consumption

Every nonnull call through public binder `0x1411f17f0` calls `0x1411f0b50(view,0)` at `0x1411f184a`, before object assignment at `0x1411f1859` and 2D dispatch at `0x1411f1873`. The null-object branch separately performs full unbind and returns. This rule applies to that concrete entry, not arbitrary internal row-binder callers.

Unbind unconditionally calls curve cleanup `0x1411f0c30` at `0x1411f0baa`. Cleanup visits View+0x28 owners, invokes nonnull owners' virtual deleting destructors at `0x1411f0e22`, then clears View+0x28 through `0x14020db90` at `0x1411f0e5a`. Normal completed unbind therefore discards old curve ownership. Destructor/allocator exceptions are not successful cleanup.

The 2D binder obtains index i through lazy getter `0x1411f3560` at `0x1411f1e29`. Getter allocates 0x1c0 bytes and invokes constructor `0x1412671d0` at `0x1411f35b3`. Allocation failure appends null, and the binder skips null items. For normal nonnull allocation, constructor calls secondary initializer `0x14126ed70(curve+0x130)` at `0x1412671fe`, then installs the curve secondary vtable at `0x14126721f`. Width belongs to the curve: secondary+0x48 = curve+0x178.

Secondary initializer calls `0x14126ee00`. At `0x14126ee40` it loads `.rdata` address `0x142d36788`, bytes `29 5c 0f 3d`, binary32 `0x3d0f5c29`; at `0x14126ee48` it stores this float to secondary+0x48. At `0x14126ee53` it writes color flag secondary+0x4c=0. The widened float is exactly `0.03500000014901161`, not a directly represented double 0.035.

After getter return, `bind2D` directly calls row binder `0x141267410` at `0x1411f1e51`; no width setter interposes in this sequence. Row binder resets geometry/selection state, stores geometry, builds rows, calls primary rectangle virtual+0x48 at `0x1412674fa`, then calls the secondary render-builder entry at `0x14126751d`. Its inspected body does not reset width or invoke its setter. Bounded reset/selection helper inspection at `0x1412688c0`, `0x1412688e0`, `0x141269c50`, `0x141269ca0` and `0x141269d20` concerns count/rows or separate curve+0x1b8 selection state. Constructor row reset also leaves width intact.

Render builder `0x14126a190` returns immediately when signed count secondary+0x58 is zero. Otherwise `0x14126a1c6` loads width bits at secondary+0x48, `0x14126a1cb` promotes binary32 to double, and `0x14126a319` passes it in XMM2 to `0x1417d0850`. The builder's incoming double argument is not width. Direct internal row rebinding preserves an existing configured width, unlike public bind's owner replacement.

## Setter and external configuration

Setter `0x14126f380` only stores its float argument to secondary+0x48 at `0x14126f396`, then returns: no local rebuild or color callback. Getter `0x14126f3b0` reads that float. View-width helper `0x1411f3040(view,float)` invokes the setter for curve items at `0x1411f30ab`, another representation at `0x1411f30ce`, and item subobjects at `0x1411f311a`; its curve arm gets items through the same lazy getter. It stores no View-level width for replay during public bind.

Three direct external callers were instruction-qualified at `0x14019e28c`, `0x1401b3ba1`, and `0x14039e6c8`. All load `.rdata` `0x1421ebb14`, bytes `0a d7 23 3c`, binary32 `0x3c23d70a`, exact promoted value `0.009999999776482582`. Their enclosing controller bodies were not completely recovered; they are not claimed as executed ordinary-fixture occurrences. This setting affects existing/lazy owners and potentially later internal rebuilds; it does not survive public bind's owner replacement.

## Color order and remaining public guard

Initial render builder later calls `0x14126a620` before the outer binder's post-bind `0x1411f28f0`. Its ordinary fresh-owner flag starts at zero. CP286's post-bind color setter later assigns QColor and sets the flag to one before its own callback: these are separate occurrences. Static order does not recover live slot-12 palette value, nor prove absence of exceptional, concurrent or reentrant interventions.

A production float initialized with `0.035f` can now be represented without inventing a parameter. It must survive direct row reset/rebind and die with CurveItem ownership. A remembered View width, default 0.01 or direct double 0.035 changes the recovered contract. The public render/color guards must remain: CP287 transform failure can leave displaced scratch uninitialized while the caller ignores EAX; neither zero-fill nor early-return is justified. Separate index/render vector ownership and allocation/copy-on-write partial effects remain unresolved.

The smallest missing same-occurrence runtime witness binds actual ordinary View/object and newly created CurveItem identity to width bits at initial `0x14126a190`/`0x1417d0850`, then binds that render occurrence's callback owner/flag, returned slot-12 palette object and consumed color components. Static lifetime establishes the ordinary default contract; it is not a fresh Open-to-Save execution observation.

## PE-byte receipts

These are hashes of half-open PE-section-mapped raw bytes, not disassembly text.

| Range | SHA-256 |
|---|---|
| `[0x14126ee00,0x14126ee5c)` | `28970e988658302462fe07fd531ac0a9114bcdb6d703dbb82786cf5141cd5992` |
| `[0x14126f380,0x14126f39c)` | `9513058a98a9210a998944c2267daa562214b55f540235ffd054a988d474f0cc` |
| `[0x1411f3040,0x1411f3126)` | `8c304f67248330dcb6087cd8143cd18c29aac63c3db41c37c93ff4dcc621c8bd` |
| `[0x141267410,0x141267524)` | `598c59e88ba406672ce365a56e2e6e3e95c69d70598b2702a9dd8c0194734d26` |
| `[0x1411f0c30,0x1411f0e67)` | `044830b271e58ddb1f78535ca2e8b575adce177a29f6529b1b3491fbca7a6241` |
