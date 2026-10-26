# CP174 connector independent plan review: private `+0x70` QList binding

## Decision

**Conditionally source-ready after CP173 is source-stable and accepted.** The proposed QtCore cut `[0x1311da,0x1311de)` is one four-byte store, `mov qword ptr [rbx+0x70],rcx`. On the selected retained private-initializer path, `RBX` is the same `0x88`-byte private allocation and `RCX` still identifies the exported `QListData::shared_null` address already stored at private `+0x18`. The proposal correctly adds a distinct private `+0x70` binding to the **same** root-owned QList record and advances the cursor. It stops before the second QList atomic add at `0x1311de`; no root delta or numeric count changes in CP174.

The inspected CP173 source has `BeforeSecondQListBinding` after one committed QVector symbolic increment and has no modeled private `+0x70` field yet. Its release report states that it ran no compilation or tests. This review did not find a source-stable CP173 focused/checkpoint/original-comparison acceptance receipt, so native byte proof and source shape do not make CP174 unconditionally implementable. The predecessor acceptance gate remains explicit.

## Native and root identity check

Direct PE bytes in shipped `QtCore4_64.dll` SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` at raw offset `0x1305da` are `48894b70`, SHA-256 `cf9361937e40dec6249de41b8b89b228a8565a6e61a39e0d7f873dd4b4ba0a4d`. The instruction spans RVA `0x1311da..0x1311de` at preferred base `0x67000000`. The retained disassembly shows `mov rbx,rcx` at `0x131178`, then `lea rcx,[QtCore+0x2f0a50]` at `0x131185`; from that load to the `+0x70` store it shows no instruction that writes `RCX` and no intervening call. The `lock add` at `0x1311d2` targets QVector, not `RCX`. Direct PE export parsing names RVA `0x2f0a50` `?shared_null@QListData@@2UData@1@A`, ordinal 4511. Thus no new QList symbol identity or host register emulator is needed for this cut.

The present `WidgetRuntimeState` owns one stable `QListSharedNullState` with QtCore hash/RVA qualification, optional absolute observation and symbolic delta. Its first private `+0x18` binding is a weak reference to that root record. The plan's new weak `secondQListSharedNullBinding` must lock to the pointer-identical **and control-block-identical** root record and first binding; an aliasing shared pointer to a foreign pointee must not pass. The second marker starts false and remains false after the store. This correctly separates pointer publication from the next instruction's reference increment. Neither the inert caller-supplied ledger nor a copied root record establishes the constructor owner.

## Admission, state effect, and test assessment

The proposed admission covers the necessary CP173 frame: exact retained Report/private identity, `0xc0`/`0x88` sizes, written task/private address-point RVAs `0x23cac8`/`0x1a9d10`, exact `BeforeSecondQListBinding` cursor, four observed zero fields, incomplete QObject and unavailable backlinks. The factory's root must be the allocation store's associated live `WidgetRuntimeState` by control block and pointee. Private `+0x18` QList, `+0x30` QString and `+0x68` QVector bindings must each match their qualified root records with their prior reference markers true. The new second QList binding must be unobserved and its marker false. Validate all of that before binding or advancing. The root QList symbolic delta may be nonzero from prior qualified frames; requiring a fixed count would be unsound. A malformed optional QList symbol qualifier, when present, must not override the canonical RVA `0x2f0a50`.

On admission, only `secondQListSharedNullBinding` and phase `BeforeSecondQListIncrement` / RVA `0x1311de` change. The first and second private QList bindings then refer to the same root object while the root QList delta stays unchanged. QString and QVector deltas, all observed absolute baselines, identities, maps/counts, zero fields, completion/backlinks and the new false marker remain fixed. This is a sound sparse representation of one native pointer store; it does not imply all other allocated private bytes are initialized.

The shortest focused positive check should start from an accepted CP173 frame, snapshot private/root identities, three symbol identities/deltas, first QList/QString/QVector bindings and markers, four zeros, phase, maps/counts and completion flags, then show only the second binding and cursor change. Same-key and different-key reentry must each refuse with no rebind or second QList increment. Refusal cases should cover absent/expired/foreign or aliasing-foreign root, wrong or expired prior bindings, uncommitted prior markers, malformed QList qualification, premature second binding/marker, bad predecessor zero/size/address-point/private identity/phase, and premature backlink/completion, with unchanged snapshots. Include the actual `ConversionSession` pairing. Migrate ordinary constructor stop expectations to `0x1311de`; tests that intentionally rewind a later frame must clear the new second binding/marker before replaying earlier cuts. Since current source lacks a `+0x70` observation, the CP174 change should make the new weak field directly inspectable and keep the second reference marker false.

The retained disassembly is an exception-directory unwind region. It qualifies the selected normal, non-null allocation path, not all exceptional paths, a live occurrence, release, or full QObject completion. Focused factory/root-owner tests, a source-stable checkpoint regression and original-comparison gate remain necessary. This review ran no build, test, GUI, original process, or live REA operation.

## SHA-256 receipts

Paths are relative to this converter checkout. Source hashes identify the released inspected snapshot, not an accepted candidate freeze.

| File | SHA-256 |
|---|---|
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/resume174-connector-next.md` | `0c043931e20ca42f950437a5bac425d5b967524642be9ad3b9ebb7f7900cef56` |
| `analysis/resume173-connector-implementation.md` | `31934c35073e6c9e6514c4c1e860b8b2189674bf311035ea24d3643f314497b2` |
| `analysis/resume173-connector-preflight-review.md` | `f2ba9bc9a1578684264084a302cdca1c2f6f0124246d374ddc98e619f4509d8e` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `15ecf0c1d8e1fddbb6d6f1a0a3fd03997044cb46baa18baf9dfcf314ca6c3357` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `14ab160c5e04344d536438d6cb2b6d47f738eab0672443936ac886537b6e47b0` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `51849bde4e923f2af7eaa7c5dd8d758dde4d27e32c97b3c210be621cefa4dc7f` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `ad7451cadae54b43b0b628f14e2f2cace0ba403a248f846251a7d27c449eabd9` |

This pass changed only this review note. It made no source, tests, shared plan/state, build, deployment, GUI-control, original-process or live REA change. No GUI-control session was acquired.
