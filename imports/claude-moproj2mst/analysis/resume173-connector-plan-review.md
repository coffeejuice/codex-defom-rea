# CP173 connector independent review of the QVector atomic proposal

## Decision

**Conditionally source-ready after CP171 is source-stable and accepted.** The proposed QtCore instruction cut `[0x1311d2,0x1311da)` is exactly one locked 32-bit add to the same exported `QVectorData::shared_null` global that CP171 bound at private `+0x68`. Its sole constructor-visible model effect is a once-only symbolic `uint32_t +1` on the associated root's QVector record, plus the private committed marker and cursor. Stopping at `0x1311da` excludes the next `mov [rbx+0x70],rcx` and later second QList increment. No absolute Qt reference count is established.

The inspected source now contains CP171's `QVectorSharedNullState`, nonowning private `qvectorSharedNullBinding`, false reference marker, and `BeforeQVectorSharedNullIncrement` stop. `analysis/resume172-connector-implementation.md` says its binding work had not been built or tested, and no source-stable checkpoint/original-comparison receipt was supplied to this review. Those are **acceptance prerequisites** for the proposed increment, not a native-byte blocker. The source is concurrently changing; the hashes below name only the inspected snapshot.

## Native and identity check

Direct PE byte mapping of shipped `QtCore4_64.dll` SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` confirms RVA `0x1311d2`, raw offset `0x1305d2`, bytes `f08305def91b0001`, SHA-256 `47c5aacbee9dc02c18624057b766d57c37e5b2ba3ad7ea54f4e8d3b2edd634b0`. The instruction ends at RVA `0x1311da`; its RIP-relative displacement `0x1bf9de` resolves to RVA `0x2f0bb8`. The retained disassembly labels this `lock add dword ptr [0x672f0bb8],1`. The PE export table names that RVA `?shared_null@QVectorData@@2U1@A` (ordinal 4514), independently of the converter's label. The global's static initialized first DWORD cannot establish its value during a live constructor.

The native chain is coherent: CP171's `lea` loads this symbol, then `mov` stores its address at private `+0x68`; CP173 increments that same symbol directly, rather than dereferencing the private field. Thus the private binding and root record must agree by object identity and module hash/RVA, but the increment itself changes the root's symbolic delta, not the bound pointer or private bytes. Unsigned 32-bit wrap is appropriate. The disassembly is from an exception-directory unwind region and supports the selected normal, non-null allocation path only; it does not prove a live occurrence, exceptional cleanup, or native release.

## Contract review and one source caution

The proposal's required guards cover the material failure modes: exact pending Report/private allocation and `BeforeQVectorSharedNullIncrement` phase; task/private sizes and address points; four known zero fields; incomplete/unbacklinked QObject; allocation store's exact associated live root by control block and pointee; pointer-identical qualified, already committed QList/QString bindings; pointer-identical qualified QVector `+0x68` binding with its own marker still false. All checks must precede root delta, marker, or cursor mutation. On success, only those three values change. A root delta can already be nonzero from other qualified frames; requiring zero would incorrectly bind the cut to one global history. The brush-gradient helper and caller-supplied ledger are not alternate owners of this constructor's symbol.

One implementation caution is visible in the **in-progress CP171 source**, not in the CP173 proposal: `task_factory.cpp` sets `vector = qualified ? qualified->vector : nullptr` and then checks `qualified->vector != vector`, which compares the same pointer to itself. It supplies no additional identity guard. CP173 must instead lock `privateState->qvectorSharedNullBinding` and compare it with `qualified->widgets->qvectorSharedNull()` (including control-block identity if aliasing shared ownership is possible), then check the root record's QtCore digest and RVA. The existing helper already validates the allocation store/root pairing and earlier QList/QString bindings. Fixing the redundant predecessor expression is appropriate during CP171 review, but the CP173 contract is sound if it uses the real bound-versus-root comparison.

The plan mentions a projected absolute count if a qualified live baseline exists. The inspected `QVectorSharedNullState` has optional baseline fields but no `projectedCount()` method or observation API. CP173 needs only the symbolic delta; it should leave those optional observations unchanged and avoid claiming a computed absolute count unless a separately qualified projection is implemented and tested. The marker and explicit phase are enough to prevent replay on the selected adapter path, provided the factory validates them before mutation.

## Focused acceptance assessment

The shortest useful positive test starts from an accepted CP171 frame and checks pointer identity for private `+0x68` versus the root record, false marker, and the three root deltas. One continuation must make QVector delta `+1` modulo `2^32`, set the marker, and stop at `0x1311da`, with QList/QString deltas, all bindings, zero fields, identities, maps/counts, completion and backlinks fixed. A near-wrap seeded delta tests the arithmetic without inventing a native absolute count. Same-key and different-key reentry must each refuse with no second increment.

The focused refusal set should mutate one guard at a time: missing/expired or aliasing-foreign root; wrong or expired private QVector binding; wrong symbol digest/RVA; false prior QList/QString marker or wrong prior binding; missing/nonzero predecessor zero; wrong phase/size/address point/private identity; already committed QVector marker; and premature backlink/completion. Each refusal must preserve the root's three deltas and retained frame. Include a real `ConversionSession` owner-pairing positive case. Existing source has no modeled private `+0x70` field; assert the stop cursor and absence of a second QList delta or newly published binding, then keep `+0x70` unavailable for the subsequent cut. Run focused factory/root-owner tests and the source-stable checkpoint/original-comparison gates before accepting implementation. This review ran no tests, build, GUI, original process, or live REA operation.

## SHA-256 receipts

Paths are relative to this converter checkout. Source/report receipts are an inspected, moving snapshot.

| File | SHA-256 |
|---|---|
| `analysis/resume171-connector-next.md` | `6da0b4fedef7ae3fa9de4528aaa331d870b04dcd2b7f650e438f4935055865c7` |
| `analysis/resume171-connector-plan-review.md` | `3b3626e883227c8166803672380483fbc4515c738ec9cead3e444713efc4c1da` |
| `analysis/resume172-connector-next.md` | `9dd44f012f11bf9143be1341b0a9eaee17353e074d02455ce097cc26bdd56913` |
| `analysis/resume172-connector-implementation.md` | `b4b99f11c140cd8f0ea3bba08abfeb295ec40b8f68554e93e96e5d78e73d7344` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `31a1c9c52418a6ea775b2d5822feb13be170c3e54e98a632698b72c80a0b28b1` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `5205614842c1bbee1718a79222f79e1ba9282a83cf19e5adf0f5ccdd78e45aff` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `4b9d90bff29f08e988afa7db44e2ed95f35361796ef7cd77003f989c27935dad` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `fe3d27f63d318212cf44b4829d89592697e15a28e1fc1d04038f926a48af6527` |

This pass changed only this review note. No source, tests, shared state, build, GUI control, or live mutable REA database changed. No GUI-control session was acquired.
