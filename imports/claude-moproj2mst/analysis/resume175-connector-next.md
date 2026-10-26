# CP175 connector: second QList shared-null reference increment

## Decision and predecessor

The next smallest state-changing constructor cut after CP174's private `+0x70` binding is QtCore RVA **`[0x1311de,0x1311e6)`**, end exclusive. Its single `lock add dword ptr [QtCore+0x2f0a50],1` applies a **second** reference increment to the **same root-owned `QListData::shared_null` object** already bound at private `+0x18` and `+0x70`. Model exactly one additional symbolic `uint32_t +1` on that root record, set the separate second-reference marker, and stop at `BeforePrivateZero78` / RVA `0x1311e6`. Do not execute the following private `+0x78` store.

The inspected source contains CP174's distinct second weak binding, false `secondQListReferenceCommitted` marker and `BeforeSecondQListIncrement` stop. The parent reports a source-stable CP174 candidate and passing full suite; `analysis/resume174-propsfix-build.json` independently records a source-stable completed build/focused preflight for source-tree hash `23ae8e2b14258004bf5229bf8e83d940dd56b6c526358f004e2368ddba9ea1ec`. This static pass did not rerun those gates or establish an original-comparison result. CP175 implementation should start from the accepted, frozen CP174 pending Report/private frame at `0x1311de`, with private `+0x70` bound to the exact first QList root record and the second marker false.

## Exact shipped instruction and global identity

The selected application `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Shipped `QtCore4_64.dll` SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred base `0x67000000`. PE `.text` maps RVA `0x1000` to raw offset `0x400`; the selected instruction starts at raw offset `0x1305de`.

| Preferred VA / RVA | PE bytes | Selected effect |
|---|---|---|
| `0x67131185` / `0x131185` | `48 8d 0d c4 f8 1b 00` | Earlier `RCX = QtCore+0x2f0a50`. |
| `0x6713118c` / `0x13118c` | `48 89 4b 18` | First private QList binding at `+0x18`. |
| `0x67131190` / `0x131190` | `f0 83 05 b8 f8 1b 00 01` | First committed QList reference increment. |
| `0x671311da` / `0x1311da` | `48 89 4b 70` | CP174 second binding to the same global at private `+0x70`. |
| `0x671311de` / `0x1311de` | `f0 83 05 6a f8 1b 00 01` | **This cut:** atomic DWORD add by one. RIP after instruction is RVA `0x1311e6`; displacement `0x1bf86a` resolves to `0x2f0a50`. |
| `0x671311e6` / `0x1311e6` | `48 89 7b 78` | Next private `+0x78` zero store, outside this cut. |

The exact eight bytes `f083056af81b0001` have SHA-256 `42761f1f0be20d86fa8ecaaa4a6cfad56f59f6c3f58031e1535c02381ab7fcb9`. Direct PE export parsing and the retained export map name RVA `0x2f0a50` `?shared_null@QListData@@2UData@1@A`, ordinal 4511. The instruction's direct RIP-relative target, CP174's `+0x70` binding and the earlier `+0x18` binding all designate that one symbol. Preferred VAs are only display addresses; model identity with module digest and RVA. The static PE-initialized DWORD is not a live reference count. The retained disassembly is an exception-directory unwind region and qualifies this selected normal, non-null allocation path, not every logical/exceptional path, a live occurrence, native release or full QObject completion.

## Owner-bound source contract

Continue on the same `TaskAllocationStore`-retained pending Report and private allocation. The allocation store's associated `WidgetRuntimeState` owns the one canonical QList shared-null record; both private weak bindings at `+0x18` and `+0x70` must lock to that record with both pointee and shared-control-block identity. The first reference marker must already be true, the second false. The root QList symbolic delta may be nonzero from other qualified constructor frames; no fixed value or absolute count is an admission prerequisite. Reuse the existing `WidgetRuntimeState::commitQListSharedNullReference()` once on admission, not a new root record, counter or ledger observation.

Before mutation, require exact phase `BeforeSecondQListIncrement` / `0x1311de`; retained Report ordinal/pointee and allocation-time private identity; task/private native sizes `0xc0`/`0x88`; written address-point RVAs `0x23cac8`/`0x1a9d10`; four private zeros at `+0x40/+0x48/+0x50/+0x58`; incomplete QObject/base and unavailable backlinks. Require a live factory root owned by the allocation store by both control block and pointee. Require qualified and pointer-identical prior QString `+0x30` and QVector `+0x68` root bindings and committed markers, as well as both qualified QList bindings to the **same** root record. Require `secondQListReferenceCommitted == false`. Any missing/expired, foreign/aliasing, malformed, uncommitted or prematurely completed state must refuse **before** modifying the root delta, marker or cursor. The root's optional absolute QList count/observation qualification, if present, must remain unchanged.

On admission, increment the root QList `referenceDelta` once modulo `2^32`, set `secondQListReferenceCommitted = true`, and advance to `BeforePrivateZero78` / RVA `0x1311e6`. Preserve both QList bindings, QString and QVector deltas, root/private identities, four zero fields, any observed absolute baselines, task/prototype/cache counts and maps, and all completion/backlink flags. If a separately qualified absolute observation exists, `projectedCount()` advances by one; otherwise the projected count remains unknown. Private `+0x78`, `+0x80`, later version branch, backlinks, thread selection, QObject counter, TaskInfo, task and cache completion remain outside this cut. A `lock` prefix is native atomic semantics; this adapter records the event/order and wrap, not host inter-thread timing.

## Short focused acceptance

1. From a source-stable accepted CP174 frame, capture pending/private/root identities, both QList bindings and markers, three root symbol identities/deltas, optional QList absolute observation and projection, four zero fields, phase, sizes/address points, maps/counts and completion flags. One continuation must change only root QList delta by one modulo `2^32`, second marker to true, and phase to `0x1311e6`. The first marker stays true; both bindings remain to the same root object, and no private `+0x78` observation is published.
2. Same-key and different-key reentry must each refuse at `0x1311e6` without replaying either QList increment, adding a task/prototype/cache entry or completing QObject. Include a near-wrap `0xffffffff -> 0` root delta case and a nonzero-history case without claiming a live absolute baseline.
3. Minimum refusal matrix: missing/expired/foreign or aliasing-foreign root; missing, expired or wrong first/second QList binding or ownership; false first/second predecessor marker state; wrong QString/QVector binding or false marker; malformed symbol qualification; missing/nonzero private zero; wrong size/address point/private identity or phase; and premature backlink/completion. Capture three root deltas, observations, all bindings/markers, phase, fields, maps and counts before each refusal and require no change. Include actual `ConversionSession` owner pairing. A malformed optional QList qualifier is not available in the current immutable QList record; test reachable binding/owner faults rather than adding a fake mutable symbol field.
4. Run focused `doe_task_factory` and root-owner tests, then the established source-stable checkpoint regression and original-comparison gates. This read-only pass ran no test, build, original process, GUI or live REA operation. A numeric absolute native count would require a same-call live pre/post observation of the actual QtCore `+0x2f0a50` DWORD.

## SHA-256 receipts and scope

Paths are relative to this converter checkout. Source receipts identify an inspected snapshot, not a new candidate freeze.

| File | SHA-256 |
|---|---|
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/resume174-connector-next.md` | `0c043931e20ca42f950437a5bac425d5b967524642be9ad3b9ebb7f7900cef56` |
| `analysis/resume174-connector-plan-review.md` | `0d458f39eca04b2c7350b98f76146e31ec71c01d5db8b65fa54e22b4d960e813` |
| `analysis/resume174-connector-implementation.md` | `7ecbb0e07877db172a707fe886cd1def45a9c0293308ef515fcaecf1c73f3622` |
| `analysis/resume174-connector-source-review.md` | `e7641e9e98a6f634550f535ce57bdbfd6d8aad8012a5ea8f621d544132542a95` |
| `analysis/resume174-propsfix-build.json` | `404b2841b7e36a4c4658fc563f8e5bd68f8cd9eaa8973d56040bf2aa906c89c0` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `3d9cf855f0c06db319657b682b3442def787ffbedbaf39caf9354892355994a0` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `7703139852f2eec98d7da3485ffa0ea2df4b27047876ea3d56c9a9697110a7bb` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `51849bde4e923f2af7eaa7c5dd8d758dde4d27e32c97b3c210be621cefa4dc7f` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `e21a97c15fefc135b0e498272fd4b3e9beb8c476d019bd33943e97552eff1cc0` |

This pass changed only this note. It made no source, tests, shared plan/state, build, deployment, GUI-control, original-process or live REA change. No GUI-control session was acquired.
