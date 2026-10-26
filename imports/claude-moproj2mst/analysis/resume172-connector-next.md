# CP172 connector: first QVector shared-null reference increment

## Decision and prerequisite

The next smallest state-changing constructor cut after the CP171 private `+0x68` binding is **QtCore RVA `[0x1311d2, 0x1311da)`**, end exclusive. It consists of one `lock add dword ptr [QtCore+0x2f0bb8],1`. On the selected normal, non-null private-allocation path, account for exactly one symbolic `uint32_t +1` on the **same root-owned `QVectorData::shared_null` identity already bound at private `+0x68`**. Advance to `BeforeSecondQListBinding` / RVA `0x1311da`. Do not write private `+0x70` or add a second QList reference in this cut.

This is conditional source guidance. The inspected converter source still stopped at `BeforeQVectorSharedNull` / RVA `0x1311c7` when hashed below; it had no CP171 root QVector record or private `+0x68` binding yet. A source-stable accepted CP171 frame and its focused/checkpoint/original-comparison gates are prerequisites. A direct PE export-table parse and the retained export map name RVA `0x2f0bb8` as `?shared_null@QVectorData@@2U1@A`, ordinal 4514. That closes the targeted global's identity for this binary, but the new source root record must be checked against it. No live count is available; the PE-initialized first DWORD `1` cannot serve as a runtime baseline.

## Exact original instruction

The shipped `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The shipped `QtCore4_64.dll` SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred base `0x67000000`. PE `.text` maps RVA `0x1000` to raw offset `0x400`, giving raw offset `0x1305d2` for this instruction. These addresses are preferred-image VAs; retain the module digest and RVAs in the model.

| Preferred VA / RVA | PE bytes | Effect |
|---|---|---|
| `0x671311c7` / `0x1311c7` | `48 8d 05 ea f9 1b 00` | CP171 predecessor loads the QVector shared-null address. |
| `0x671311ce` / `0x1311ce` | `48 89 43 68` | CP171 predecessor stores that address at private `+0x68`. |
| `0x671311d2` / `0x1311d2` | `f0 83 05 de f9 1b 00 01` | **This cut:** atomically add one to the symbol's 32-bit reference DWORD. RIP after the instruction is RVA `0x1311da`; signed displacement `0x1bf9de` resolves to `0x2f0bb8`. |
| `0x671311da` / `0x1311da` | `48 89 4b 70` | Next private `+0x70` write, outside this cut. |
| `0x671311de` / `0x1311de` | `f0 83 05 6a f8 1b 00 01` | Later second QList shared-null increment, outside this cut. |

The eight selected bytes `f08305def91b0001` have SHA-256 `47c5aacbee9dc02c18624057b766d57c37e5b2ba3ad7ea54f4e8d3b2edd634b0`. The `lock add` does not store its result in a register; represent a symbolic delta modulo `2^32`, not an absolute native count or a host atomic object. The retained `0x67131160..0x67131240` disassembly covers an exception-directory unwind region, not every logical or exceptional path. This qualification does not establish a live constructor occurrence, native ref-release, or cleanup.

## Owner-bound source contract

Use the *same* pending Report and sparse QObjectPrivate allocation retained by `TaskAllocationStore`; the CP171 private `+0x68` binding must lock to the stable QVector record of the allocation store's associated `WidgetRuntimeState`. Keep the root record qualified by QtCore SHA-256 above and RVA `0x2f0bb8`. Its symbolic reference delta may already be nonzero from other frames on that root; admission must not require a globally zero delta or invent an absolute baseline. The private frame needs a once-only `qvectorSharedNullReferenceCommitted` marker, false before this cut.

Before changing state, require the exact retained pending Report and unchanged private pointee, task/private allocation sizes `0xc0`/`0x88`, written address-point RVAs `0x23cac8`/`0x1a9d10`, exact `BeforeQVectorSharedNullIncrement` cursor, and private `+0x40`, `+0x48`, `+0x50`, `+0x58` all observed zero. Require incomplete QObject/base and unavailable task/private backlinks. The factory's live root must match the allocation store's associated root by both control block and pointee (`ownsWidgetRuntime`). Require pointer-identical, qualified, previously committed QList `+0x18` / RVA `0x2f0a50` and QString `+0x30` / RVA `0x2f0b30` bindings. Require private `+0x68` to lock to the pointer-identical qualified root QVector record, with its marker false. Reject a missing, expired, foreign, aliasing-foreign, or malformed root or binding, bad phase, wrong predecessor observation, already committed marker, or premature completion **before** incrementing anything.

On admission, increment only that root QVector record's `uint32_t` symbolic delta once, set the private marker true, and advance the cursor to `BeforeSecondQListBinding` / RVA `0x1311da`. Preserve the root and private identities, private `+0x68` binding, four zero fields, QList/QString deltas, observed absolute baselines, task/prototype/cache counts and maps, and completion/backlink flags. If the root has a qualified absolute observation, its projected count advances by one modulo `2^32`; otherwise it stays unknown. Do not reuse the isolated brush-gradient shared-null helper or the inert `ReportQObjectSharedRefLedger` as the constructor's process owner. Their optional future integration requires an explicit canonical-symbol reconciliation.

## Short focused acceptance

1. From an accepted CP171 frame, snapshot the pending/private/root pointees, all three shared-null object identities and deltas, any observed baselines, four private zeros, phase, counts/maps, and completion/backlink flags. One continuation must change only QVector delta by one modulo `2^32`, its private committed marker to true, and phase to RVA `0x1311da`. Private `+0x70` remains unobserved, and QList's second delta remains absent.
2. Same-key and different-key reentry at the new stop must each refuse without another QVector add, owner rebind, task/prototype/cache growth, or QObject completion. Check the retained frame after each refusal.
3. Minimum guard matrix: absent/expired/foreign or aliasing-foreign root; stale/wrong QVector `+0x68` binding or symbol qualification; false earlier QList/QString marker or wrong binding; false predecessor zero or address-point/private identity; already true QVector marker; wrong phase; and premature backlink/completion. Each refusal preserves the three root deltas, bindings, phase, counts and maps. Include the actual `ConversionSession` root pairing as a positive case. A seeded near-wrap `uint32_t` delta should advance from `0xffffffff` to `0` without requiring a numeric native count.
4. Run focused `doe_task_factory` and root-owner tests, then the established source-stable checkpoint build/regression and original-comparison gates. This read-only pass ran no tests, build, original process, GUI, or live REA operation. A numeric absolute Qt count would require a same-call live pre/post observation of the actual `QtCore+0x2f0bb8` DWORD around this instruction.

## SHA-256 receipts and scope

Paths are relative to this converter checkout. Source hashes identify the inspected, concurrently changing snapshot; they are not a candidate freeze.

| File | SHA-256 |
|---|---|
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/wave3-closure-next-close-clear-selection-empty-hash-leaves-static.json` | `17fce2e9c1d96c6f9497659be2a4485b161a65ba0efb0bb05f5f467c22739cd6` |
| `analysis/resume171-connector-next.md` | `6da0b4fedef7ae3fa9de4528aaa331d870b04dcd2b7f650e438f4935055865c7` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `2aa6dfaeb664511d5987c02e35837ec542f3141d377829a0c2a827e55a09cf9c` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `a3e61c964c57e6733497fd3c395f303081455aa213ca2dc47d3e54d7fb307f77` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `a33b4b05e059b7313efb196fb34967c20196399b87226c4ff431c1c2106c1d88` |
| `source/apps/moproj2mst/src/engine/view_manager.cpp` | `d1ac4321eef560232771f175c1d2bBDC8e7d7fa18a51269c4c060d950d92c382` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `7ca7063147487ac60e5831231e1fd39e195a89fe29e196adde2287a4518b280e` |
| `source/apps/moproj2mst/src/engine/report_qobject_ledger.cpp` | `d95b70c3c93585997a81aa4492d2f51e6bf8d26ce4a7880f07fadc9e6851e897` |
| `source/apps/moproj2mst/include/m2m/engine/brush_gradient_stops.hpp` | `adc6f78674c0706586a8d9dd442f6c42a41ca8adc4f4eb5e0dff3b3e3524c8f2` |

This pass changed only this report. It made no source, tests, shared plan/state, build, deployment, GUI-control, original-process, or live REA change. No GUI-control session was acquired.
