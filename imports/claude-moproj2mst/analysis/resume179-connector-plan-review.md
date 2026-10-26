# CP179 connector independent plan review

## Decision

**Source-ready from accepted frozen CP178; no blocking plan defect found.** The proposed cut `[0x13120b,0x13120f)` records only the private allocation's `+0x08` qword zero, then stops before private `+0x10`. This is distinct from the later task `+0x08` private-pointer publication and must not enable either backlink. Current source/test receipts match the reviewed CP178 release; parent supplies accepted/frozen status. This review does not rerun acceptance or original comparison.

## Native and predecessor check

Directly read raw `0x13060b` in shipped QtCore: `48897b08`, SHA-256 `0ff59178935f4e42602274d216887da3888a7055509d5c9caff6c34bf579ed7e`. It encodes `mov qword ptr [rbx+8],rdi`. Next bytes `48897b10` are the separate private `+0x10` store. Retained initializer disassembly establishes RBX=private at RVA `0x131178` and full RDI=zero via `xor edi,edi` at `0x1311b5`. CP178's qualified equality branch skips the mismatch call, leaving both registers unchanged through the selected store. Bytes `+0x08..+0x0f` lie within `0x88`.

The public constructor's later RVA `0x131425` uses its own RBX task receiver to publish the returned private pointer after initializer return. CP179's initializer RBX is the private receiver. These are separate objects and stages; private08 zero does not imply task-private or private-task backlink availability. Native evidence is static, module/RVA-qualified selected non-null/equal-path evidence, not live occurrence, mismatch-call semantics, cleanup or complete QObject construction.

## Required source guards and effect

Add initially disengaged `optional<uint64_t> private08` to the retained sparse private state. Admit only exact `BeforePrivateZero08`, with canonical retained version provenance `{QtCore digest, 0x40804, load 0x13140f, call 0x131417}`. Do not rely on phase alone after callers can corrupt or remove provenance. Reuse existing Report/private/root qualification, then require both canonical QList pointee/control-block bindings and committed markers, canonical committed QString/QVector, six prior observed zeros, private08 unknown, exact sizes/written address points/allocation-time identity, and incomplete base/backlinks. Validate every condition before writes.

Success changes only private08 to zero and phase to `BeforePrivateZero10` / `0x13120f`. No delta, observation baseline, binding/marker, map/cache/count, backlink, completion or version-provenance change belongs here. Reject both engaged zero and engaged nonzero private08. Leave private10 unknown and mismatch fallthrough unmodeled.

## Focused validation and migration hazards

Actual ConversionSession positive coverage should reconstruct CP178 by setting `BeforePrivateZero08`, retaining canonical provenance and six zeros, and clearing private08. Snapshot complete state and show only the optional/cursor change. Exercise independent zero/nonzero/near-wrap history and supplied/unknown observations without changing deltas or projections. Repeated same-key and different-key refusal must preserve the terminal frame.

Extend the retained-frame refusal matrix with premature private08 zero/nonzero; missing/wrong version argument, module, argument-load RVA and call RVA; missing/nonzero each earlier zero; owner absence/expiry/foreign/both aliasing directions; first/second QList mismatch or wrong control block; prior binding/marker faults; Report/private identity, size, address point/written flag and phase faults; and premature completion/backlinks. Snapshot provenance, all sparse fields, weak ownership identities, markers, three deltas/available observations, maps/counts and cursor around refusal. No failure should execute the skipped mismatch call or auto-repair provenance.

Every earlier successful rewind from the new terminal frame must clear private08 as well as whichever later sparse fields it already clears. Existing CP178 equal-decision fixtures currently rewind only phase; they must also clear private08 so the newly followed CP179 store can admit. Phase alone encodes the equal decision, so no extra branch marker reset is needed. Migrate ordinary stop assertions to `0x13120f` / `BeforePrivateZero10`, retain earlier intentional refusal diagnostics and all existing delta arithmetic. Add fresh/type checks for unknown optional<uint64_t>. Do not introduce private10 or enable task backlinks to satisfy new-stop tests.

Focused/root-owner, source-stable checkpoint regression and original-comparison gates remain implementation acceptance work.

## Exact SHA-256 receipts

| File | SHA-256 |
|---|---|
| `analysis/resume179-connector-next.md` | `10e55a3a08500371bb824c095928ff58db3a2f5917f30f14ba7a0c887efa6036` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `4ffa7e3e5b4f147cd4abb1643c4f8ab0f9e89c0d65b1da3d63314cf00b6204dc` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `d125ec4c0c45f6d3ec7da7ae31f97971f27b9b335762020f37aa58a3574945aa` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `91919dc54f34e6d1566eb0703004444b2fcb29324d53e027772a2c9e9559fc0f` |

Only this report was written. No source/test edits, build, executed tests, GUI control, live REA, deployment or shared recovery-state changes occurred.
