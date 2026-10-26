# CP176 connector independent plan review

## Decision

**Source-ready from accepted CP175; no blocking plan defect found.** The proposed cut `[0x1311e6,0x1311ea)` records precisely one eight-byte zero observation at private `+0x78` on the retained private allocation and advances to the independent `+0x80` store. The inspected factory/header/tests match the CP175 snapshot receipts below. This review does not rerun CP175 acceptance, build, regression or original-comparison gates.

## Native and sparse-order check

Directly read shipped QtCore DLL SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Raw `0x1305e6` contains four bytes `48897b78`, SHA-256 `89e81f6e18a602e8dadf2420153a73d497e90b06d5f96b5c12209fc6ea2be71d`, encoding `mov qword ptr [rbx+0x78],rdi`. The next bytes are `4889bb80000000`, the separate private `+0x80` store. The retained contiguous disassembly establishes `RBX = incoming private` at RVA `0x131178` and `xor edi,edi` at `0x1311b5`; the latter clears all of RDI under x64 register semantics. There is no intervening call or overwrite of either register before the selected store. Bytes `+0x78..+0x7f` lie within the retained `0x88` allocation.

The source ends CP175 with `secondQListReferenceCommitted = true` and `BeforePrivateZero78`, with no private78 observation. Therefore an initially disengaged `optional<uint64_t>` accurately distinguishes unknown native bytes from this newly observed zero. No private80 observation is justified. Static evidence qualifies the selected normal non-null path; it does not establish a live occurrence, exception/release behavior or full QObject completion.

## Admission and state effect

Add a dedicated exact `BeforePrivateZero78` branch. Reuse retained-prefix qualification for the allocation-store/root ownership and pointee pair, pending Report ordinal/pointer, allocation-time private identity, task/private sizes and written address points, first QList and QString qualified root bindings and markers, and incomplete base/backlinks. Independently require the canonical committed QVector binding and its optional qualification, all four earlier zero observations, and the same root QList record at both private bindings with control-block and pointee identity. Both QList markers must be true. Require private78 disengaged: both an already observed zero and a nonzero value must refuse.

Perform every check before writing private78 or advancing. Success changes only private78 to uint64 zero and phase to `BeforePrivateZero80` / `0x1311ea`. Root deltas and optional observation metadata remain fixed, including nonzero history and wrap states; no counter prerequisite or reference-commit call belongs here. Matching raw pointers without matching control blocks is insufficient. Reentry at the new stop must reach only a diagnostic and cannot replay an earlier increment.

## Focused test hazards and acceptance

Use an actual ConversionSession positive and independently reconstitute CP175 by retaining both QList bindings and true markers, setting phase `BeforePrivateZero78` and clearing private78. Snapshot identities, bindings/ownership, markers, three deltas, available observations/qualification, earlier zeros, sizes/address points, counts/cache state and completion/backlinks. Require only the new zero and cursor change; repeat same-key and different-key refusal with preserved state.

Every earlier-cut rewind fixture that starts from the new terminal frame must clear private78 before replaying, in addition to its existing binding/marker resets. Otherwise a valid older replay will reach CP176 with a premature observation. Migrate ordinary stop/phase expectations to `0x1311ea` / `BeforePrivateZero80`, while preserving intentional earlier-seam diagnostic checks. Unlike CP175 migration, this cut adds no reference delta: existing correct total-history arithmetic should remain unchanged.

Refusal coverage should include missing/expired/foreign root, both root aliasing directions, first/second QList mismatch and wrong control block, expired prior bindings, each uncommitted marker (especially the now-required true second marker), malformed reachable QVector qualifier, missing/nonzero earlier zeros, private78 already zero and nonzero, wrong identity/size/address point/written flag/phase, and premature completion/backlinks. Preserve snapshots through refusal; replaced-private fixtures must not dereference their destroyed old private reference. QList symbol fields remain immutable, so do not invent a mutable qualifier just to populate the matrix.

Focused factory/root-owner tests, source-stable regression and original-comparison gates remain implementation acceptance work. The inspected plan correctly keeps private80, version branching and subsequent QObject work outside this cut.

## SHA-256 receipts

| File | SHA-256 |
|---|---|
| `analysis/resume176-connector-next.md` | `48b3b73a64383c9d1e97f9791c55249116b02264a7117acb969d9eb086a16721` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `e6c073e8d01733d1a62fff9a0eefaeb630416ec2b1ff6cecaf2706ecbf17953c` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `6ac6da3fc895808cd82003352f15fc5a9335caa9e329ed9a73aa7e38e38581ed` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `c1dff37a4afc583e79a901aee8e5b678b8ab69756d11c0634c66a56c8f43ee5c` |

Only this report was written. No source/test edits, build, executed tests, GUI control, live REA, deployment or shared recovery-state changes occurred.
