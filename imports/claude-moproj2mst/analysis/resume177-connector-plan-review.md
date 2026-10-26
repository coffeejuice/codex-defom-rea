# CP177 connector independent plan review

## Decision

**Source-ready from accepted frozen CP176; no blocking plan defect found.** The proposed cut `[0x1311ea,0x1311f1)` is precisely the final qword zero store in the retained `0x88`-byte private allocation. It stops before the version comparison and makes no claim about branch outcome, compatibility-call effects, complete private initialization or QObject return. Source receipts match the CP176 release reviewed previously; parent supplies frozen/full-suite acceptance. This pass does not rerun those gates or establish original-comparison acceptance.

## Independent native check

Directly read QtCore DLL SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Raw `0x1305ea` contains seven bytes `4889bb80000000`, SHA-256 `e2b92a04f965c226abcbf1dea64eefb3fe5989b07ac0c8e02d38b365f421faf5`, encoding `mov qword ptr [rbx+0x80],rdi`. The next six bytes `81fa04080400` encode the excluded `cmp edx,0x40804` at RVA `0x1311f1`. Retained contiguous disassembly, checked during this sequence, establishes RBX as the private pointer at `0x131178` and full RDI zero via `xor edi,edi` at `0x1311b5`, with no intervening register overwrite/call through this store. Bytes `+0x80..+0x87` fit exactly within allocation length `0x88`.

The last qword being observed does not mean all earlier private bytes are initialized: the sparse representation proves only the enumerated fields. The subsequent compare/branch/call remains a separate qualification problem. Native evidence is the selected normal non-null path in an unwind-region capture, not live counts, exceptional paths, cleanup or full construction.

## Required owner and sparse contract

Use a disengaged `optional<uint64_t> private80`, an exact `BeforePrivateZero80` admission branch and a new `BeforeQtVersionCheck` terminal phase. Reuse the retained Report/private/root qualifier, then validate committed root-qualified QVector and optional RVA, the same canonical first/second QList pointee and control block with both markers true, and all five prior zero observations including private78. Require private80 disengaged and incomplete base/backlinks. All checks precede either write. Success changes only private80 to zero and the cursor to `0x1311f1`.

Maintain exact store/root pointee and ownership pairing, retained ordinal/Report identity, allocation-time private identity, sizes and written address points. Empty/expired bindings, raw-pointer equality with a different control block, or shared ownership with a foreign pointee must refuse. No fixed delta, absolute native count, reference-commit call or version-input guess belongs in this cut. All three deltas and observation metadata, prior fields/bindings/markers, counts/maps and completion state stay fixed.

## Focused tests and migration hazards

Actual ConversionSession positive coverage should reconstruct CP176 by retaining both committed QList bindings and private78 zero, clearing private80, and setting `BeforePrivateZero80`. Snapshot full retained state and require only private80/phase change, then same-key and different-key reentry without any reference replay or growth. Include independent nonzero/near-wrap root history and supplied/unknown observations to show this store changes no counter or projection.

The refusal matrix should include missing/nonzero private78 in addition to the four earlier zeros; private80 already zero **and** nonzero; both root aliasing directions; first/second QList mismatch, missing/expired/foreign/wrong-control-block bindings; each false marker; malformed reachable QVector qualifier; bad size/address/written flag/Report/private identity/phase; and completion/backlinks. Verify no mutation of private78/private80, all prior state, three deltas/observations, bindings/markers, counts/maps and phase. Do not fabricate a mutable QList symbol qualifier absent from its immutable type.

Every earlier successful rewind from the new terminal state must now clear private80, while earlier CP176 replays clear private78 too. Preserve the observed private78 zero for a direct CP177 predecessor. Migrate ordinary stop expectations to `0x1311f1` / `BeforeQtVersionCheck`, retaining intentional earlier refusal diagnostics and existing delta arithmetic: CP177 adds no reference event. Replacement-private tests must snapshot through the replacement and avoid dangling old references. Fresh/type checks should establish initially unknown `optional<uint64_t>` and there should be no version-result or call-effect field published at this stop.

Focused, source-stable checkpoint regression and original-comparison gates remain implementation acceptance work.

## Exact SHA-256 receipts

| File | SHA-256 |
|---|---|
| `analysis/resume177-connector-next.md` | `e8ac9bd94385960a97ae28d9554c2dd59a783333b4827174c3d88212be13ea42` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `576c994815adabc6932ddfaefd465d7e322b9c94a92d777a61f790295e0b37fa` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `285cad32db28596de87cf453f45af35e17ddb0edb1593c486518842947870622` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `576c6012b4180de84c6bc40bd3a0f21ba451d63916845fa29fb302cd4d828bf0` |

Only this report was written. No source/test edits, build, executed tests, GUI control, live REA, deployment or shared recovery-state changes occurred.
