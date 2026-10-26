# CP178 connector independent source review

## Verdict

**Accepted for build and executed validation; no blocking source defect found.** The released factory/header/tests retain caller-qualified version evidence and implement only the selected equality branch to RVA `0x13120b`. The three source hashes match the implementation release. Compilation, executed tests, freeze and original-comparison acceptance remain separate gates.

## Provenance, qualification and state effect

`QObjectPrefix::PrivateInitializerVersion` retains module digest, argument, argument-load RVA and call RVA in an optional owned by the retained frame. A standalone frame starts unqualified. Only fresh selected non-null private construction supplies `{canonical QtCore digest, 0x40804, 0x13140f, 0x131417}` after private allocation. Missing or corrupt evidence on an existing frame is not repaired.

The exact `BeforeQtVersionCheck` branch reuses retained Report/private/root identity qualification and validates canonical committed QVector and its optional RVA, all six zero observations, second same-root QList pointee/control block and true marker, and all four provenance fields. The reused qualifier establishes Report ordinal/pointee, allocation-time private identity, task/private sizes/written address points, store/root ownership and pointee equality, first QList/QString canonical committed bindings, and incomplete base/backlinks. Thus provenance is scoped to the qualified retained allocation/owner rather than a foreign frame with a matching numeric version alone.

A version other than `0x40804`, missing optional, wrong module/load/call RVA or invalid predecessor refuses before mutation. The only admitted write is phase `BeforePrivateZero08`, encoding the equality branch. No private08 field/store, delta, observation, binding, count/map, completion/backlink or mismatch-call effect is published. At the new phase, reentry reaches only the stop diagnostic.

Native evidence independently verified in this review sequence: canonical QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; caller bytes `ba04080400488bc8e844fdffff`, SHA-256 `403861a57d687e136473a558d18ad834f70c04b368f904f3107d19f3a117579a`; relative call resolves to private initializer RVA `0x131160`. Compare/JE bytes `81fa040804007412`, SHA-256 `0255c14fa893d0c8f2b08512b47e3d4672cde7559f99731ab3f7f983cfad95aa`; target arithmetic gives `0x1311f9 + 0x12 = 0x13120b`. The caller non-null branch explicitly supplies EDX, and retained initializer disassembly contains no intervening EDX write/call. Target bytes `48897b08` remain excluded. Static unwind-region evidence qualifies this selected path only, not all callers, live occurrence, exception/release behavior or QObject completion.

## Test and migration review

Independently counted 57 corruption variants at `test_doe_task_factory.cpp:3131`. Missing/wrong version, wrong module, wrong argument-load RVA and wrong call RVA are distinct cases; `0x40803` refuses at the retained comparison boundary rather than entering mismatch fallthrough. The matrix carries forward missing/nonzero six-zero observations, root/binding/control-block/expiry/marker errors, Report/private identity/size/address/written flag/phase faults and completion/backlinks. It snapshots provenance plus private/frame/root/binding/delta/observation/count state and checks no mutation. Four owner-pair cases cover absent, foreign and both aliasing directions.

The fresh-frame test verifies absent provenance. Actual ConversionSession positive coverage at line 3393 checks canonical provenance generated on fresh construction, rewinds only the phase to CP177, and supplies independent zero or `0xffffffff` histories with optional QList/QVector observations. It then verifies phase-only advancement, all six zeros/bindings/markers/ownership identities, unchanged three deltas and observation metadata, retained provenance/identities, counts/cache state and incomplete flags. Same-key and two different-key reentries refuse at the new stop without replay. Compile-time assertions cover optional provenance and branch-target arithmetic.

Ordinary final stop expectations migrate to `0x13120b` / `BeforePrivateZero08`; zero old `RVA 0x1311f1` terminal strings remain. Phase alone encodes the decision, so no redundant branch marker needs clearing in earlier rewind fixtures. Existing constructed frames retain the caller evidence, and prior sparse/counter arithmetic remains unchanged. There is no modeled private08 or later comparison-call effect.

## Exact SHA-256 receipts

| File | SHA-256 |
|---|---|
| `analysis/resume178-connector-next.md` | `e1ef3c91a288c89a45dcca54c9e5164464fdba0566f546ec82a9c36841d834d5` |
| `analysis/resume178-connector-plan-review.md` | `c4dc339d1150ab14d21bd71afbaecdccca6e7a585cb6c0284e9e4d7abf09bec1` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `4ffa7e3e5b4f147cd4abb1643c4f8ab0f9e89c0d65b1da3d63314cf00b6204dc` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `d125ec4c0c45f6d3ec7da7ae31f97971f27b9b335762020f37aa58a3574945aa` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `91919dc54f34e6d1566eb0703004444b2fcb29324d53e027772a2c9e9559fc0f` |

Only this report was written. No source/test edit, build, executed tests, GUI control, live REA, deployment or shared recovery-state change occurred. Required executed acceptance gates remain with the parent.
