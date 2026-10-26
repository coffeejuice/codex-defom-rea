# CP175 independent source review

## Verdict

**Accepted for build and executed validation; no blocking source defect found.** The released factory/header/tests implement the planned single second QList event and retain the boundary before private `+0x78`. This is source review, not candidate freeze, compilation, executed test or original-comparison acceptance.

## Implementation check

The exact native bytes and export target were independently verified during this review sequence: shipped QtCore DLL SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, raw `0x1305de`, bytes `f083056af81b0001`, slice SHA-256 `42761f1f0be20d86fa8ecaaa4a6cfad56f59f6c3f58031e1535c02381ab7fcb9`. Next RIP RVA `0x1311e6` plus signed displacement `0x1bf86a` resolves to `0x2f0a50`; direct export parsing identifies `?shared_null@QListData@@2UData@1@A`, ordinal 4511. Next bytes `48897b78` are excluded.

The new exact-phase branch at `task_factory.cpp:336` reuses retained Report/private/root qualification. That qualifier establishes the store/root control block and pointee pairing, pending ordinal/pointee, allocation-time private identity, task/private sizes and written address points, canonical first QList and QString root bindings and committed markers, and incomplete base/backlinks. The branch additionally validates root-qualified committed QVector, optional QVector RVA, four zeros, a live second QList binding equal in pointee and control block to the canonical first-list root, and a false second marker. Empty, expired, foreign and wrong-owner second bindings all fail before mutation.

Only after qualification, the existing noexcept unsigned root commit adds one modulo 2^32, then the second marker becomes true and phase becomes `BeforePrivateZero78`. Reentry at that phase reaches only the `0x1311e6` diagnostic. The sparse header adds the phase but no private78 field or observation. The source has no extra root QList object, fixed delta prerequisite, observation rewrite or second-path increment. Prior bindings, QString/QVector deltas, counts/maps and completion/backlinks are untouched.

## Test and migration check

`test_doe_task_factory.cpp:2178` reconstructs CP174 with both bindings present and second marker false. Its corruption matrix covers first and second binding failures including empty/expired/foreign/aliasing/wrong-control-block second binding, root expiry, earlier markers, malformed reachable QVector qualification, zeros, sizes/address points/private identity/phase, and completion/backlinks. Refusal snapshots cover all three root deltas, available observations and qualification, bindings and markers, phase, private fields, identities and counts/cache sizes. The replaced-private case snapshots through the replacement and does not subsequently dereference its old `state` reference.

The owner matrix at line 2356 independently supplies a retained preincrement frame and rejects absent pairing, foreign pairing and both aliasing directions without an increment. The actual ConversionSession positive at line 2398 tests delta 17 to 18 with unknown projection and delta `0xffffffff` to zero with supplied observation 42 at `0xfffffffe`. The expected projection 44 is correct modulo arithmetic; baseline and qualification remain unchanged. Repeated same-key and different-key calls retain exactly one additional event, true second marker, both same-root bindings, fixed QString/QVector deltas, identities, zeros, sizes and incomplete state.

Migrated ordinary stops use `0x1311e6` / `BeforePrivateZero78`. Inspected total-history expectations correctly distinguish one fresh frame's QList delta 2, two frames' delta 4, and deliberately successful older-cut replay's additional event. CP173 wrap/history replay expects QList delta +1 while still requiring exactly its intended QVector change. CP174 history replay clears the second marker and allows the newly followed CP175 event. Earlier-cut rewinds clear the second binding/marker; intentional negative-seam assertions preserve pre-call deltas and earlier refusal diagnostics.

The immutable QList module/RVA fields cannot express a malformed optional QList qualifier, so the implementation correctly tests reachable binding/owner corruption rather than inventing one. These authored checks remain to be executed by the parent.

## Exact snapshot SHA-256

| File | SHA-256 |
|---|---|
| `analysis/resume175-connector-next.md` | `8dbf332bc5b03808aea1f3670b9e797a72a2fd99e254361b35bb3304c083df51` |
| `analysis/resume175-connector-plan-review.md` | `a1b3b92406f299f002953f34a08270e9b4a67b73fe442a4f0174c61d470c6790` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `e6c073e8d01733d1a62fff9a0eefaeb630416ec2b1ff6cecaf2706ecbf17953c` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `6ac6da3fc895808cd82003352f15fc5a9335caa9e329ed9a73aa7e38e38581ed` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `c1dff37a4afc583e79a901aee8e5b678b8ab69756d11c0634c66a56c8f43ee5c` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `51849bde4e923f2af7eaa7c5dd8d758dde4d27e32c97b3c210be621cefa4dc7f` |

Only this report was written. No source/test edit, build, executed tests, GUI session, live REA, deployment or shared recovery-state edit occurred. Static evidence does not establish live absolute counts, native scheduling, exception/release paths or full QObject completion.
