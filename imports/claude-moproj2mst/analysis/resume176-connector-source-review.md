# CP176 connector independent source review

## Verdict

**Accepted for build and executed validation; no blocking source defect found.** The released factory/header/tests implement exactly private `+0x78` zero publication followed by the stop before private `+0x80`. Source hashes match the implementation release receipt. This is static source acceptance, not compilation, executed tests, freeze or original-comparison acceptance.

## Source and native boundary

The dedicated `BeforePrivateZero78` branch at `task_factory.cpp:356` reuses retained Report/private/root qualification and independently validates committed canonical QVector, its optional symbol qualification, four earlier zeros, the second same-root QList binding and true second marker, and disengaged private78. The shared qualifier supplies exact store/root control-block and pointee pairing, Report ordinal/pointer, allocation-time private identity, task/private sizes and written address points, committed first QList/QString bindings and canonical qualification, and incomplete base/backlinks. Both QList bindings therefore resolve to the same canonical root object with matching control blocks. Missing, expired, foreign or aliased bindings cannot admit the store.

All guards precede the only state writes: `private78 = uint64_t{0}` and phase `BeforePrivateZero80`. No reference commit, rebinding, allocation, cache update, backlink or completion occurs in this branch. The sparse header declares private78 as initially disengaged `optional<uint64_t>`; there is no private80 observation. The terminal diagnostic is RVA `0x1311ea`, so same/different-key reentry cannot replay CP175 or initialize private80.

Native evidence was independently checked in this review sequence: QtCore DLL SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, raw `0x1305e6`, bytes `48897b78`, slice SHA-256 `89e81f6e18a602e8dadf2420153a73d497e90b06d5f96b5c12209fc6ea2be71d`. Retained disassembly establishes RBX as the private pointer and RDI as zero without intervening overwrite/call. The store touches `+0x78..+0x7f` within `0x88`; following bytes `4889bb80000000` are the excluded private80 store. This qualifies the selected normal path, not live occurrence, exception/release behavior or complete QObject construction.

## Authored tests and migration

Independently counted 49 entries in the CP176 matrix at `test_doe_task_factory.cpp:2463`. They cover all four missing/nonzero predecessor zeros, private identity/size/address/written flags, root expiry, binding ownership/pointee/expiry faults, prior markers including the required committed second marker, malformed reachable QVector qualification, phase/completion/backlinks and prematurely observed private78. Refusals snapshot and preserve private78, other fields, identities, bindings/markers, three root deltas and available observations/qualification, counts and cache sizes. Replacement-private checks use the replacement afterward rather than dereferencing the destroyed original reference.

The four owner cases at line 2650 reject absent/foreign pairing and both aliasing directions from independently supplied predecessor state. Actual ConversionSession positive coverage at line 2694 reconstructs CP175 with private78 unknown and all markers true, then verifies only zero/phase publication and same/different-key reentry. Nonzero delta 17 and supplied observation history at delta `0xffffffff` remain unchanged. Projected count 43 remains correct for baseline 42 at `0xfffffffe`; unknown projection remains unknown without an observation. QList/QString/QVector deltas and bindings remain fixed. Fresh-state and compile-time checks establish optional<uint64_t> and disengagement.

Earlier successful rewinds clear private78 together with their existing binding/marker resets. Inspected phase assignments without an adjacent reset are intentional corruption cases, reset slightly later, or owner-refusal fixtures starting before CP176 was ever reached. Ordinary terminal assertions use `0x1311ea` / `BeforePrivateZero80`; zero old `RVA 0x1311e6` terminal strings remain in the test file. Prior correct QList/QVector history arithmetic is retained because this cut adds no increment.

One nonblocking coverage opportunity: premature private78 is tested with engaged zero, but not a separate engaged nonzero value. The source's `has_value()` guard rejects both identically; an additional nonzero fixture would make that plan-review recommendation explicit without changing the implementation verdict.

## Exact SHA-256 receipts

| File | SHA-256 |
|---|---|
| `analysis/resume176-connector-next.md` | `48b3b73a64383c9d1e97f9791c55249116b02264a7117acb969d9eb086a16721` |
| `analysis/resume176-connector-plan-review.md` | `4e6a645de7335a1df618e3c1353e92b33f30353e0eacbe17ced920a67ff7cd0f` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `576c994815adabc6932ddfaefd465d7e322b9c94a92d777a61f790295e0b37fa` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `285cad32db28596de87cf453f45af35e17ddb0edb1593c486518842947870622` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `576c6012b4180de84c6bc40bd3a0f21ba451d63916845fa29fb302cd4d828bf0` |

Only this report was written. No source/test edits, build, executed tests, GUI control, live REA, deployment or shared recovery-state changes occurred. Required executed gates remain with the parent.
