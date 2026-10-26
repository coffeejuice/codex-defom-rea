# CP174 connector: second QList address store at private `+0x70`

## Decision and prerequisite

The smallest next owner-bound state change after CP173's QVector atomic add is QtCore RVA **`[0x1311da,0x1311de)`**, end exclusive. Its one `mov [rbx+0x70],rcx` stores the **same `QListData::shared_null` address previously bound at private `+0x18`** into a distinct eight-byte field of the same retained `0x88`-byte QObjectPrivate allocation. Stop at `0x1311de`, before the second QList `lock add`. This cut introduces a second nonowning private binding and a cursor advance only. It does not add a QList reference, allocate another shared-null object, complete QObject, or publish a backlink.

This is conditional source guidance. The inspected converter source has CP173's symbolic QVector increment and `BeforeSecondQListBinding` stop, but its implementation report explicitly says no build, test execution, candidate freeze or original comparison was performed. A source-stable accepted CP173 frame and focused/checkpoint/original-comparison gates are prerequisites before calling CP174 implemented or accepted. On that frame, private `+0x68` is bound to the root QVector record and its first reference marker is committed; private `+0x70` remains unobserved.

## Shipped instruction and register proof

The selected application `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The shipped `QtCore4_64.dll` SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred base `0x67000000`; PE `.text` maps RVA `0x1000` to raw offset `0x400`. The selected four bytes are at raw offset `0x1305da`: `48894b70`, SHA-256 `cf9361937e40dec6249de41b8b89b228a8565a6e61a39e0d7f873dd4b4ba0a4d`.

| Preferred VA / RVA | PE bytes | Selected effect |
|---|---|---|
| `0x67131178` / `0x131178` | `48 8b d9` | `RBX` retains the incoming private pointer. |
| `0x67131185` / `0x131185` | `48 8d 0d c4 f8 1b 00` | `RCX = QtCore+0x2f0a50`, the QList shared-null address. |
| `0x6713118c` / `0x13118c` | `48 89 4b 18` | Earlier first binding at private `+0x18`. |
| `0x671311d2` / `0x1311d2` | `f0 83 05 de f9 1b 00 01` | Accepted CP173 predecessor increments the distinct QVector global. |
| `0x671311da` / `0x1311da` | `48 89 4b 70` | **This cut:** private `+0x70` receives the still-retained `RCX` address. |
| `0x671311de` / `0x1311de` | `f0 83 05 6a f8 1b 00 01` | Later second QList atomic add, outside this cut. Its target is again RVA `0x2f0a50`. |

The retained contiguous disassembly from RVA `0x131185` through `0x1311da` has no intervening write to `RCX` and no call; the address survives the QString, four zero stores and QVector binding/add. Direct PE export-table parsing names RVA `0x2f0a50` `?shared_null@QListData@@2UData@1@A`, ordinal 4511. This identifies the global for the shipped DLL, not a numeric live reference count. VAs are preferred-image addresses; persistent source qualification uses QtCore digest and RVAs. The disassembly is an exception-directory unwind region and supports this selected normal, non-null allocation path, not every path, a live occurrence, native failure cleanup or release.

## Owner-bound source contract

Add a distinct weak/nonowning `secondQListSharedNullBinding` for native private `+0x70`, initially empty, and a separate `secondQListReferenceCommitted` marker initially false for the later atomic cut. Do **not** add a second root `QListSharedNullState`: both private `+0x18` and `+0x70` refer to the one stable root record qualified by QtCore hash and RVA `0x2f0a50`. The root's existing symbolic QList delta may already include other accepted frames; no absolute or fixed delta is an admission precondition.

Before mutation, require the exact retained pending Report and allocation-time private identity, native task/private sizes `0xc0`/`0x88`, written address-point RVAs `0x23cac8`/`0x1a9d10`, exact `BeforeSecondQListBinding` phase / RVA `0x1311da`, all four private zeros at `+0x40/+0x48/+0x50/+0x58`, incomplete QObject/base and unavailable backlinks. Require a live factory root paired with the allocation store by control-block and pointee identity. Require private `+0x18` QList and `+0x30` QString bindings to match their qualified root records with first reference markers committed, and private `+0x68` to match the root's qualified QVector record with its reference marker committed. Require the new second QList binding empty and its marker false. Reject a wrong owner, private pointee, symbol qualification, binding, marker, predecessor observation, phase or completion state before any write.

On admission, set only the private `+0x70` weak binding to the **same pointer-identical root QList object** already bound at `+0x18`, then advance to `BeforeSecondQListIncrement` / RVA `0x1311de`. Keep the second marker false. Preserve all root symbolic deltas and any observed counts, all previous private fields/bindings, pending/private/root identities, task/prototype/cache counts and maps, and completion/backlink flags. No host register emulator is needed: the selected original instruction sequence proves the retained `RCX` value.

## Short focused acceptance

1. From a source-stable accepted CP173 frame, snapshot pending/private/root identities, both previous container bindings, their committed markers, three root deltas/observations, four private zeros, phase, sizes/address points, maps/counts and completion/backlink flags. One continuation must bind private `+0x70` to the **exact** root QList object already bound at `+0x18`, leave `secondQListReferenceCommitted == false`, and stop at `0x1311de`. No root delta or other captured state changes.
2. Same-key and different-key reentry each refuse at the new stop without a second binding or reference increment, prototype/task/cache growth or QObject completion. Inspect the retained frame after each refusal.
3. Minimum refusal matrix: absent/expired/foreign or aliasing-foreign root; wrong or expired first QList, QString, or QVector binding; uncommitted first QList/QString/QVector marker; malformed QList symbol qualification; premature second QList binding/marker; missing/nonzero predecessor zero; wrong address point/size/private identity or phase; and premature backlinks/completion. Every refusal keeps identities, three deltas, phase, fields, maps and counts fixed. Include the actual `ConversionSession` pairing as a positive case. The existing `ReportQObjectSharedRefLedger` does not substitute for the root identity or prove an absolute count.
4. Run focused `doe_task_factory` and root-owner tests, then the established source-stable checkpoint regression and original-comparison gates. This read-only pass ran no tests, build, original process, GUI, or live REA operation. A numeric absolute QList count would require a separate same-call live observation of `QtCore+0x2f0a50`; static PE data cannot provide it.

## SHA-256 receipts and scope

Paths are relative to this converter checkout. Source hashes identify a moving inspected snapshot, not a candidate freeze.

| File | SHA-256 |
|---|---|
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/wave3-closure-next-close-clear-selection-empty-hash-leaves-static.json` | `17fce2e9c1d96c6f9497659be2a4485b161a65ba0efb0bb05f5f467c22739cd6` |
| `analysis/resume172-connector-next.md` | `9dd44f012f11bf9143be1341b0a9eaee17353e074d02455ce097cc26bdd56913` |
| `analysis/resume173-connector-plan-review.md` | `aabda757ec326c3e7b7e95ece7f396a95c695b3442b474ea5db10c48b45e659f` |
| `analysis/resume173-connector-implementation.md` | `31934c35073e6c9e6514c4c1e860b8b2189674bf311035ea24d3643f314497b2` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `15ecf0c1d8e1fddbb6d6f1a0a3fd03997044cb46baa18baf9dfcf314ca6c3357` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `14ab160c5e04344d536438d6cb2b6d47f738eab0672443936ac886537b6e47b0` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `51849bde4e923f2af7eaa7c5dd8d758dde4d27e32c97b3c210be621cefa4dc7f` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `ad7451cadae54b43b0b628f14e2f2cace0ba403a248f846251a7d27c449eabd9` |

This pass changed only this note. It made no source, tests, shared plan/state, build, deployment, GUI-control, original-process or live REA change. No GUI-control session was acquired.
