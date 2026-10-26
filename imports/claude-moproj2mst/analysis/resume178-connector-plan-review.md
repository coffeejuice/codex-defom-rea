# CP178 connector independent plan review

## Decision

**Source-ready for the qualified successful-allocation caller path; no blocking plan defect found.** The comparison and taken equal branch are supported by an explicit caller argument, not inferred from the comparison constant. CP177 source receipts match the frozen release reviewed previously. Parent supplies CP177 accepted/frozen status; this read-only review does not rerun focused/full acceptance or original comparison.

## Independent PE and value-chain check

Direct PE reads confirm caller RVA `0x13140f` bytes `ba04080400488bc8e844fdffff` (SHA-256 `403861a57d687e136473a558d18ad834f70c04b368f904f3107d19f3a117579a`). They set EDX to `0x40804`, move the allocated private pointer into RCX and call: decoding the relative call at `0x131417` gives target RVA `0x131160`. Retained public-constructor disassembly shows allocation length `0x88`, a null test and branch bypassing the initializer before this argument load. Thus the proof is explicitly scoped to the non-null path.

Private-initializer RVA `0x1311f1` bytes `81fa040804007412` hash to `0255c14fa893d0c8f2b08512b47e3d4672cde7559f99731ab3f7f983cfad95aa`. The six-byte compare followed by `74 12` gives target `0x1311f9 + 0x12 = 0x13120b`. Retained contiguous initializer disassembly has no EDX write or intervening call before the compare, so volatility does not break this specific chain. The earlier lock-add flags are overwritten by the compare.

Mismatch bytes `[0x1311f9,0x13120b)` hash to `75c3134a6aaad6da6a2364f2b94afb68816cacaf6c35c7260ddeefe4481346d6`; they are skipped on the selected equality path. At the target, `48897b08` is the excluded private `+0x08` store. Source must not infer mismatch-call semantics or publish any private08 observation. Evidence is qualified by QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and RVAs, not a preferred loaded base. The unwind-region evidence proves only this selected static path, not all callers, live occurrence, exceptional cleanup or QObject completion.

## Retained source contract

CP177 has no retained version argument/result yet. Introduce the qualified optional argument and caller provenance when constructing the selected non-null frame, with canonical module qualification and argument-load/call RVAs. Keep it on the same retained frame. Do not repair a missing or foreign qualifier on a later CP178 continuation: that must refuse. A mutable representation should permit testing absent/wrong provenance; immutable qualifiers should still have explicit canonical checks where applicable. No general register emulator is needed.

Use an exact `BeforeQtVersionCheck` admission branch and validate the retained Report/private allocation identity, store/root pointee and control block, sizes/written address points, six zero observations, all four root bindings and committed markers, and incomplete base/backlinks before mutation. Validate both QList bindings against the same canonical root object with ownership identity. Require the retained argument `0x40804` and its selected caller qualification; a mismatch or unsupported caller refuses without executing or simulating the fallthrough.

On success only the branch decision/cursor changes to `BeforePrivateZero08` / `0x13120b`. The phase may itself encode taken equality if the qualified argument remains retained and immutable during the transition. No private field, delta, observation baseline, allocation/map/cache count, completion/backlink or compatibility-call state changes. Reentry at that phase must stop without replaying the decision.

## Focused tests and migration hazards

Use actual ConversionSession pairing plus a direct CP177 predecessor with the canonical argument/provenance and six zeros. Snapshot full state including argument/provenance, binding ownership, deltas/available observations, identities/counts and completion. Require only the decision/cursor change, private08 unknown and unchanged nonzero/near-wrap histories. Repeated same/different-key calls must retain the new state.

Refusal coverage must distinguish missing argument, wrong argument `0x40803`, missing/wrong argument-load RVA, wrong call RVA and wrong module qualification where representable. None may take the mismatch fallthrough or mutate the frame. Carry forward owner/root/binding/control-block/marker, identity/size/address/written flag, six missing/nonzero zeros, phase and completion/backlink refusals with complete no-mutation snapshots. Do not let a default comparison constant make wrong-provenance tests pass.

Migrate ordinary stop assertions to `0x13120b` / `BeforePrivateZero08`, retaining earlier intentional diagnostics and all delta arithmetic. Earlier replay fixtures must reset any explicit branch-result marker if one is introduced; phase alone avoids redundant result state. Existing valid constructed frames should retain their canonical caller evidence through rewinds. Tests manually supplying a predecessor must provide that evidence explicitly rather than relying on CP178 to invent it. Do not add private08 zero to make the new stop appear more complete.

Focused/root-owner, source-stable checkpoint regression and original-comparison gates remain implementation acceptance work.

## Exact source receipts

| File | SHA-256 |
|---|---|
| `analysis/resume178-connector-next.md` | `e1ef3c91a288c89a45dcca54c9e5164464fdba0566f546ec82a9c36841d834d5` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `71e8cb455a5e808144e19e1b1f2ced12783080722d9fcab93b33e8bdd2c21b90` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `6b5a4c207fc96d863a6463de06c536bda7cef9c934f304f3cc5ae178545d6bcb` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `df030fe660d7ff8f5268e47201118795c43fe8695b7c201c8c2d8be32b8862af` |

Only this report was written. No source/test edits, build, executed tests, GUI control, live REA, deployment or shared recovery-state changes occurred.
