# CP176 connector: private `+0x78` zero store

## Decision and prerequisite

The smallest next owner-bound constructor cut after CP175's second QList atomic add is QtCore RVA **`[0x1311e6,0x1311ea)`**, end exclusive. Its sole instruction stores the already-zero `RDI` as a 64-bit value at `+0x78` of the *same* retained `0x88`-byte QObjectPrivate allocation. Record one sparse field observation, then stop before the separate private `+0x80` store at `0x1311ea`. This cut changes no QList/QString/QVector reference delta, binding, backlink, counter, task or cache.

The inspected converter source has CP175's `secondQListReferenceCommitted` marker and `BeforePrivateZero78` stop but no private `+0x78` observation. `analysis/resume175-dual-build.json` records a completed, source-stable focused build/preflight with all seven named focused cases exiting zero for source-tree hash `b23ecdbd1011128a857cb7cf5ee6b37a838ee78f7e4af72b6221572ff728680e`. This pass did not rerun it. Begin implementation from an accepted CP175 pending Report/private frame at RVA `0x1311e6`; checkpoint regression and original-comparison acceptance remain separate gates.

## Exact original bytes and register qualification

The selected `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Shipped `QtCore4_64.dll` SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred base `0x67000000`. Its PE `.text` maps RVA `0x1000` to raw offset `0x400`. This cut starts at raw offset `0x1305e6`, with bytes `48897b78` and SHA-256 `89e81f6e18a602e8dadf2420153a73d497e90b06d5f96b5c12209fc6ea2be71d`.

| Preferred VA / RVA | PE bytes | Selected effect |
|---|---|---|
| `0x67131178` / `0x131178` | `48 8b d9` | Earlier `RBX =` incoming private pointer. |
| `0x671311b5` / `0x1311b5` | `33 ff` | Earlier `xor edi,edi` establishes full `RDI = 0`. |
| `0x671311de` / `0x1311de` | `f0 83 05 6a f8 1b 00 01` | Accepted CP175 predecessor adds the second QList reference. |
| `0x671311e6` / `0x1311e6` | `48 89 7b 78` | **This cut:** store eight zero bytes at private `+0x78`. |
| `0x671311ea` / `0x1311ea` | `48 89 bb 80 00 00 00` | Later independent 64-bit zero store at private `+0x80`; outside this cut. |

The retained contiguous private-initializer disassembly has no intervening write to `RBX` or `RDI`, and no call, between those register definitions and `0x1311e6`. The selected store reaches bytes `+0x78..+0x7f` within the `0x88` allocation. The next `+0x80` store occupies the final eight bytes and is not implied by this cut. VAs are preferred-image display addresses; use module SHA-256 and RVAs for durable qualifications. This disassembly covers an exception-directory unwind region and proves only the selected normal, non-null allocation path, not every logical/exceptional path, a live occurrence, native cleanup or full QObject completion.

## Owner-bound source contract

Add `std::optional<std::uint64_t> private78` to `PendingTaskPrototype::SparseQObjectPrivate`, initially disengaged. Disengaged means allocated native bytes are unknown, not observed zero. Retain it on the same private pointee of the same pending Report in `TaskAllocationStore`. The phase `BeforePrivateZero78` is an instruction cursor, not QObject completion.

Before mutation, require the exact retained pending Report ordinal and private allocation-time identity, task/private native sizes `0xc0`/`0x88`, written task/private address-point RVAs `0x23cac8`/`0x1a9d10`, exact `BeforePrivateZero78` / RVA `0x1311e6`, and observed `private40 == private48 == private50 == private58 == 0` while `private78` is unobserved. Require a live factory `WidgetRuntimeState` owned by the allocation store with both control-block and pointee identity. Private `+0x18` first QList and `+0x70` second QList bindings must lock to the **same** qualified root QList record (QtCore digest, RVA `0x2f0a50`) with both reference markers true. The QString `+0x30` and QVector `+0x68` bindings must likewise lock to their qualified root records (RVAs `0x2f0b30` and `0x2f0bb8`) with committed markers. Require incomplete QObject/base and unavailable task/private backlinks. Reject malformed owner, identity, phase, predecessor, binding, marker or premature-completion state before writing a field or cursor. Do not require fixed or numeric absolute shared-null counts; prior root deltas are committed observations, not new events.

On admission, set only `private78 = std::uint64_t{0}` and advance once to `BeforePrivateZero80` / RVA `0x1311ea`. Keep all prior sparse observations, three root records and deltas, optional absolute observations, pending/private/root identities, task/prototype/cache counts and maps, completion flags and backlinks unchanged. Private `+0x80` and other uninitialized bytes remain unknown. No host register emulator is needed for this one store because the qualified predecessor establishes unchanged `RDI = 0` and `RBX = private`.

## Short focused acceptance

1. From a source-stable accepted CP175 frame, capture pending/private/root identities, allocation sizes/address points, four earlier zero optionals, `private78` unknown, all four container bindings and markers, three root deltas/observations, phase, maps/counts and completion/backlinks. One continuation must yield only `private78 == 0` and `BeforePrivateZero80` / `0x1311ea`. The root deltas and both QList bindings remain unchanged; private `+0x80` is still unobserved.
2. Same-key and different-key reentry after the stop must refuse without replaying the store or any prior reference increment, growing task/prototype/cache state, or completing QObject. Inspect the retained frame after each refusal.
3. Minimum refusal matrix: absent/expired/foreign or aliasing-foreign root; wrong or expired QList/QString/QVector bindings or uncommitted markers; mismatch between first and second QList identity; wrong task/private address point or written flag, allocation size, retained Report/private identity or phase; missing/nonzero earlier zero; prematurely observed `private78`; and premature backlink/completion. Each refusal must preserve fields, bindings, markers, phase, all root deltas/observations, counts and maps. Include the actual `ConversionSession` owner pairing as a positive case. A malformed optional QList RVA is not a source-representable fixture because the root QList symbol is an immutable constant; exercise reachable owner/binding faults instead.
4. Run focused `doe_task_factory` and root-owner tests, then the established source-stable checkpoint regression and original-comparison gates. This read-only pass ran no tests, build, original process, GUI or live REA operation. A later `+0x80` store, version branch, QObject completion and native release require separate cuts/evidence.

## SHA-256 receipts and scope

Paths are relative to this converter checkout. Source hashes identify an inspected snapshot, not a new candidate freeze.

| File | SHA-256 |
|---|---|
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/resume175-connector-next.md` | `8dbf332bc5b03808aea1f3670b9e797a72a2fd99e254361b35bb3304c083df51` |
| `analysis/resume175-connector-plan-review.md` | `a1b3b92406f299f002953f34a08270e9b4a67b73fe442a4f0174c61d470c6790` |
| `analysis/resume175-connector-implementation.md` | `a7c74be2c02212c14058aaa7b30355c14b5e8e828a5c3129d0491dfee1e44c5c` |
| `analysis/resume175-connector-source-review.md` | `ad578325670c4ccda520abd9527819a119d2527b6ebb03533d118ca46499eec6` |
| `analysis/resume175-dual-build.json` | `4ba50a2e40e2c06ad3b63a949b19ea5afad0a57eee754a275832e977c4677fcf` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `6ac6da3fc895808cd82003352f15fc5a9335caa9e329ed9a73aa7e38e38581ed` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `e6c073e8d01733d1a62fff9a0eefaeb630416ec2b1ff6cecaf2706ecbf17953c` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `51849bde4e923f2af7eaa7c5dd8d758dde4d27e32c97b3c210be621cefa4dc7f` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `c1dff37a4afc583e79a901aee8e5b678b8ab69756d11c0634c66a56c8f43ee5c` |

This pass changed only this note. It made no source, tests, shared plan/state, build, deployment, GUI-control, original-process or live REA change. No GUI-control session was acquired.
