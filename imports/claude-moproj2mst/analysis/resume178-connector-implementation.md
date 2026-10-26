# CP178 connector implementation receipt

Implemented only the qualified QtCore version decision at RVA `[0x1311f1,0x1311f9)`. `BeforePrivateZero08` records the equal branch at `0x1311f7` taken to `0x13120b`; the private `+0x08` store remains outside this implementation. No private `+0x08` observation or value is materialized.

`QObjectPrefix` now retains an optional `PrivateInitializerVersion` record containing the module digest, incoming argument, public caller's argument-load RVA and call RVA. A fresh standalone frame has no record. The selected non-null private-allocation path supplies `0x40804` from public QObject RVA `0x13140f`, call RVA `0x131417`. This is retained caller evidence, rather than a value inferred from the comparison constant. Retained unqualified, foreign-module, foreign-caller and mismatched-argument frames refuse before advancing; mismatch fallthrough is not modeled.

Admission checks the same pending Report allocation and private pointee, exact allocation sizes and address points/written flags, exact CP177 phase, all six predecessor zero observations, all four bindings with their committed markers, root pointee and control-block identity, qualified QVector symbol, and absent completion/backlinks. The branch changes only phase. Root symbolic deltas, root observations, private fields, maps and allocation counts remain untouched.

## Changed source and tests

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `4ffa7e3e5b4f147cd4abb1643c4f8ab0f9e89c0d65b1da3d63314cf00b6204dc` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `d125ec4c0c45f6d3ec7da7ae31f97971f27b9b335762020f37aa58a3574945aa` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `91919dc54f34e6d1566eb0703004444b2fcb29324d53e027772a2c9e9559fc0f` |

Existing factory tests' terminal stop expectations now use `BeforePrivateZero08` / `0x13120b`. Added tests cover a fresh unqualified frame, actual `ConversionSession` owner pairing, the isolated equal decision with supplied sparse/symbolic history and optional count observations, same/different-key reentry, four unpaired/foreign/alias root-owner variants, and 57 malformed retained predecessor variants. The latter include missing/nonzero six-zero observations, malformed allocation/root/binding/marker/phase/completion state, and missing/wrong caller/version provenance. Refusal tests retain snapshots and verify no mutation; the `0x40803` case remains before the unmodeled mismatch path.

## Static checks and limitations

A direct read of `evidence/original-binaries/QtCore4_64.dll` verified its SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, compare/JE bytes `81fa040804007412`, caller bytes `ba04080400488bc8e844fdffff`, and branch arithmetic `0x1311f7 + 2 + 0x12 == 0x13120b`. Cut and caller hashes match the next-cut note. Static source assertions verified admission before the new phase assignment, absence of a private `+0x08` store, and equality of the 57 declared refusal variants and switch cases. Tests also contain a compile-time branch-target arithmetic assertion and optional-provenance type assertion. Direct source whitespace checks passed.

Source qualification follows `analysis/resume178-connector-next.md`: retained disassemblies are unwind regions, not proven complete logical functions; digest/RVAs qualify this selected non-null caller path. This static implementation does not establish a live occurrence, generic Qt caller behavior, mismatch compatibility-call semantics, cleanup, QObject completion, or subsequent effects.

No build, compiled test, full-suite gate, freeze, original comparison, deployment, GUI-control session, original process, live mutable REA operation, or shared plan/state edit was performed by this worker. Acceptance gates remain for the coordinator. The imported source paths are not tracked Git files in the enclosing repository, so an empty `git diff --check` result was not treated as a source-diff validation receipt. Only the three assigned source/test files and this receipt were edited. File ownership is released to the coordinator.
