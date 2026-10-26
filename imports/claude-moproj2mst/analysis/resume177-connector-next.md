# CP177 connector: final private-allocation qword zero at `+0x80`

## Decision and predecessor

The smallest next owner-bound constructor cut after CP176's `+0x78` zero observation is QtCore RVA **`[0x1311ea,0x1311f1)`**, end exclusive. Its one seven-byte instruction stores the already-zero `RDI` as a 64-bit value at `+0x80` of the **same** retained `0x88`-byte QObjectPrivate allocation. This covers its final eight bytes `+0x80..+0x87` and advances the instruction cursor only. Stop at `0x1311f1`, before the `EDX` version comparison and possible compatibility call. No constructor return, backlink, thread choice, counter, TaskInfo, task or cache follows from this one store.

The inspected source contains CP176's `private78 == 0` continuation and `BeforePrivateZero80` stop, but no modeled `private80`. `analysis/resume176-dual-build.json` records a completed source-stable build/focused preflight with all steps exiting zero for source-tree hash `abb80badb264d6cc60349d147054040f1821253e6af565066887709ddd0bdaba`; the parent reports the full suite also passed. This static pass did not rerun either gate or establish original-comparison acceptance. Begin CP177 only from an accepted CP176 retained Report/private frame at RVA `0x1311ea`.

## Exact PE bytes and register basis

The selected `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Shipped `QtCore4_64.dll` SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred base `0x67000000`. Its PE `.text` maps RVA `0x1000` to raw offset `0x400`. The selected seven bytes at raw offset `0x1305ea` are `4889bb80000000`, SHA-256 `e2b92a04f965c226abcbf1dea64eefb3fe5989b07ac0c8e02d38b365f421faf5`.

| Preferred VA / RVA | PE bytes | Selected effect |
|---|---|---|
| `0x67131178` / `0x131178` | `48 8b d9` | Earlier `RBX =` incoming private pointer. |
| `0x671311b5` / `0x1311b5` | `33 ff` | Earlier `xor edi,edi` sets full `RDI = 0`. |
| `0x671311e6` / `0x1311e6` | `48 89 7b 78` | Accepted CP176 predecessor writes zero at private `+0x78`. |
| `0x671311ea` / `0x1311ea` | `48 89 bb 80 00 00 00` | **This cut:** store eight zero bytes at private `+0x80`. |
| `0x671311f1` / `0x1311f1` | `81 fa 04 08 04 00` | Later `cmp edx,0x40804`, outside this cut. |
| `0x671311f7` / `0x1311f7` | `74 12` | Later conditional branch, also outside this cut. |

The retained contiguous disassembly shows no intervening write to `RBX` or `RDI`, and no call, between their definitions and this store. `+0x80..+0x87` fits exactly inside the `0x88` allocation. The following comparison is a new instruction with branch-dependent behavior; this report does not infer its outcome merely from the earlier constructor chain. These VAs are preferred-image display values; durable source qualification uses QtCore digest and RVAs. The disassembly is an exception-directory unwind region, so the proof covers the selected normal, non-null private-allocation path, not every logical/exceptional path, a live occurrence, native cleanup or full QObject construction.

## Owner-bound source contract

Add `std::optional<std::uint64_t> private80` to `PendingTaskPrototype::SparseQObjectPrivate`, initially disengaged. A disengaged optional means the allocated bytes are still unknown; do not default them to an observed zero. Keep it on the same sparse private pointee owned by the pending Report in `TaskAllocationStore`.

Before mutation, require the exact retained pending Report ordinal/pointee and allocation-time private identity; native task/private sizes `0xc0`/`0x88`; written task/private address-point RVAs `0x23cac8`/`0x1a9d10`; exact `BeforePrivateZero80` phase / RVA `0x1311ea`; observed `private40 == private48 == private50 == private58 == private78 == 0`; and `private80` unobserved. Require the factory's live `WidgetRuntimeState` to be the allocation store's associated root by both control-block and pointee identity. Private first/second QList bindings at `+0x18/+0x70` must lock to the **same** qualified root QList record (QtCore digest, RVA `0x2f0a50`), with both reference markers true. Private QString `+0x30` and QVector `+0x68` bindings must lock to their qualified root records (RVAs `0x2f0b30` and `0x2f0bb8`) with committed markers. QObject/base must remain incomplete and task/private backlinks unavailable. Reject any malformed owner, identity, phase, predecessor observation, binding, marker or completion state before changing either field or phase. Do not demand absolute Qt shared-null counts or replay prior reference events.

On admission, set only `private80 = std::uint64_t{0}` and advance once to `BeforeQtVersionCheck` / RVA `0x1311f1`. Preserve all earlier sparse observations, private/root/binding identities, three symbolic root deltas and any observed baselines, task/prototype/cache counts and maps, completion/backlink flags. The native private allocation is now observed as zero at six listed qword offsets, not proven fully initialized at every offset. Leave the `EDX` comparison, conditional compatibility path, subsequent private fields, QObject return and later task effects for separate cuts. A host register emulator is unnecessary for this one store because the selected instruction chain establishes `RDI=0` and unchanged `RBX=private`.

## Short focused acceptance

1. From a source-stable accepted CP176 frame, snapshot pending/private/root identities, sizes/address points, five observed zeros, `private80` unknown, all container bindings/markers, three root deltas/observations, phase, maps/counts and completion/backlink flags. One continuation must change only `private80` to zero and phase to `BeforeQtVersionCheck` / `0x1311f1`. All reference deltas and bindings remain fixed; no version result or compatibility-call effect is published.
2. Same-key and different-key reentry must each refuse at the new stop without replaying any store or shared reference increment, growing task/prototype/cache state, or completing QObject. Inspect the retained frame after each refusal.
3. Minimum refusal matrix: absent/expired/foreign or aliasing-foreign root; wrong or expired first/second QList, QString or QVector binding; uncommitted marker; mismatched first/second QList identity; wrong task/private address-point value or written flag, allocation size, retained Report/private identity or phase; missing/nonzero earlier zero including `private78`; prematurely observed `private80` (zero and nonzero cases); and premature backlink/completion. Each refusal must preserve all fields, bindings/markers, phase, three deltas/observations, counts and maps. Include actual `ConversionSession` pairing as a positive case.
4. Run focused `doe_task_factory` and root-owner tests, then the established source-stable checkpoint regression and original-comparison gates. This read-only pass ran no test, build, original process, GUI or live REA operation. The subsequent Qt version check and possible call need their own owner/branch qualification.

## SHA-256 receipts and scope

Paths are relative to this converter checkout. Source hashes identify an inspected snapshot, not a new candidate freeze.

| File | SHA-256 |
|---|---|
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/resume176-connector-next.md` | `48b3b73a64383c9d1e97f9791c55249116b02264a7117acb969d9eb086a16721` |
| `analysis/resume176-connector-plan-review.md` | `4e6a645de7335a1df618e3c1353e92b33f30353e0eacbe17ced920a67ff7cd0f` |
| `analysis/resume176-connector-implementation.md` | `66017a84fbfce2801abb53dccc8cfc6e86773f959feb3ba0307479c559c9c4b9` |
| `analysis/resume176-connector-source-review.md` | `79bf51f68ccf7b904b547fe5cc52c2ee001fde649b16d60dabe85a51c4021814` |
| `analysis/resume176-dual-build.json` | `d7b09a9ebe6475200e15e97c0f091ed42f52bff67e9528f52ff3539f22a4f5bf` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `285cad32db28596de87cf453f45af35e17ddb0edb1593c486518842947870622` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `576c994815adabc6932ddfaefd465d7e322b9c94a92d777a61f790295e0b37fa` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `51849bde4e923f2af7eaa7c5dd8d758dde4d27e32c97b3c210be621cefa4dc7f` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `576c6012b4180de84c6bc40bd3a0f21ba451d63916845fa29fb302cd4d828bf0` |

This pass changed only this note. It made no source, tests, shared plan/state, build, deployment, GUI-control, original-process or live REA change. No GUI-control session was acquired.
