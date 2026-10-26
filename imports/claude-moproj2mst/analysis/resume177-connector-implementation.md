# CP177 connector implementation release

Implemented the bounded normal-path QtCore RVA `[0x1311ea,0x1311f1)` store on the retained Report QObject private allocation. `private80` is `std::optional<std::uint64_t>` and starts unknown. Admission checks the qualified retained owner/private identities, sizes/address points, root control block and pointee identity, first/second QList, QString and QVector bindings and committed markers, five earlier observed zeros including `private78`, unknown `private80`, and incomplete/backlink-unavailable state. Only `private80 = 0` and `BeforeQtVersionCheck` are published. The version comparison, branch, compatibility call and later constructor effects remain unavailable.

The CP176 qualification helper is reused; no symbolic reference event or absolute-count requirement is added. Same-key and different-key reentry stop at RVA `0x1311f1` without replay. Ordinary first-call stop and phase assertions were migrated; earlier sparse fixtures reset the new successor observation alongside `private78`.

## Focused coverage and static audit

Five CP177 cases cover fresh unknown/type assertion, a 52-case corruption/refusal matrix with pre/post snapshots, four unpaired/foreign/aliasing root forms, actual ConversionSession pairing with supplied observed and unobserved QList history, and a supplied sparse CP176 predecessor with all three root deltas zero and no absolute baseline. Refusals snapshot private fields, phase, completion/backlinks, address points and sizes, bindings including weak owner identity, committed markers, list/vector observations, all three root deltas, allocation identities, prototype/task counts and cache counts. QString exposes referenceDelta only; the static type audit checked its actual declaration and does not invent observation fields.

Static audit confirmed the optional uint64 type, declared phase names, all ordinary stop strings migrated, earlier fixture resets, five distinct CP177 test cases, and exactly one field store plus cursor advance in the new continuation. Shipped QtCore SHA-256 was independently recomputed as `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Bytes at raw offset `0x1305ea` are `4889bb80000000`; their SHA-256 is `e2b92a04f965c226abcbf1dea64eefb3fe5989b07ac0c8e02d38b365f421faf5`. The source qualification and exclusions are those in `analysis/resume177-connector-next.md`.

## File receipts

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `6b5a4c207fc96d863a6463de06c536bda7cef9c934f304f3cc5ae178545d6bcb` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `71e8cb455a5e808144e19e1b1f2ced12783080722d9fcab93b33e8bdd2c21b90` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `df030fe660d7ff8f5268e47201118795c43fe8695b7c201c8c2d8be32b8862af` |

This worker changed only the three exclusively owned source/test files and this implementation receipt. No compiler, build, executable test, candidate freeze, deployment, GUI control, original-process comparison, live mutable REA operation or shared recovery state was performed. Tests are authored but unexecuted; parent integration owns focused/full regression gates and source review. No GUI-control session was acquired. Ownership of the three files is released after this receipt.
