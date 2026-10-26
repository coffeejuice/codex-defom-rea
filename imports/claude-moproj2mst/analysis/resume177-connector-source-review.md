# CP177 connector independent source review

## Verdict

**Accepted for build and executed validation; no blocking source defect found.** The released snapshot implements precisely the final private `+0x80` qword zero observation and stops before Qt version comparison. The three source/test hashes match the implementation release. This review does not establish compilation, executed tests, candidate freeze or original-comparison acceptance.

## Implementation and native boundary

`task_factory.cpp:378` admits only `BeforePrivateZero80`. Retained-prefix qualification establishes the Report ordinal/pointee, allocation-time private identity, task/private sizes and written address points, live allocation-store/root ownership and pointee pairing, canonical committed first QList and QString bindings, and incomplete QObject/base with unavailable backlinks. The new branch separately requires canonical committed QVector with optional qualification, four earlier zeros, second same-root QList pointee/control block and true marker, private78 observed zero and private80 unknown.

All checks precede the only two writes: private80 becomes `uint64_t{0}` and phase becomes `BeforeQtVersionCheck`. No delta, binding, allocation, map/cache, completion or backlink changes. A false/expired binding, foreign pointee or wrong control block refuses. An already observed private80 refuses whether zero or nonzero. Reentry at the final phase reaches the `0x1311f1` diagnostic without reference replay or later effects. The header declares initially disengaged optional<uint64_t>; no version result or compatibility-call model is added.

The independently verified native store in this review sequence is QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, raw `0x1305ea`, bytes `4889bb80000000`, slice SHA-256 `e2b92a04f965c226abcbf1dea64eefb3fe5989b07ac0c8e02d38b365f421faf5`. It writes `+0x80..+0x87`, exactly the final qword of `0x88`. Retained disassembly establishes unchanged RBX private pointer and RDI zero through the store. Following `81fa04080400` is the excluded compare at RVA `0x1311f1`. Sparse earlier bytes remain unknown unless separately observed; final-qword publication does not establish full initialization, constructor return, branch outcome or cleanup.

## Authored validation and migration

Independently counted 52 CP177 corruption cases at `test_doe_task_factory.cpp:2768`. They include missing/nonzero private78, premature private80 both zero/nonzero, missing/nonzero four earlier zeros, owner/binding expiry/foreign/aliasing/wrong-control-block cases, uncommitted markers, malformed reachable QVector qualification, identity/size/address/written flag/phase faults, and completion/backlinks. Refusal snapshots preserve private78/private80, earlier fields, identities, weak ownership identities even for expired bindings, markers, three deltas, available list/vector observation metadata, counts and caches. Replaced-private checks snapshot the replacement and do not dereference the destroyed original afterward.

The owner test rejects absent/foreign pairing and both aliasing directions at an independently supplied predecessor. Actual ConversionSession coverage at line 3016 tests delta 17 and near-wrap supplied observation history without changing any delta or projection; baseline 42 at delta `0xfffffffe` with current `0xffffffff` correctly remains projected 43. Same-key and different-key reentry preserve bindings, markers, private78 and all counts/incomplete flags. A separate sparse supplied predecessor at line 3093 proves zero symbolic deltas and absent absolute observations are admissible without manufacturing counts or replaying events. Fresh/type checks establish unknown optional<uint64_t>.

Ordinary terminal assertions migrate to `0x1311f1` / `BeforeQtVersionCheck`, with zero old `RVA 0x1311ea` terminal strings. Earlier replay fixtures clear private80 beside private78; the sole private78 reset without private80 reset is the intentional Missing78 corruption case, after private80 was already cleared. CP177 predecessor fixtures retain private78 zero. Prior QList/QVector arithmetic stays unchanged because this cut adds no event. No later comparison, conditional branch or compatibility-call effect is published.

## Exact snapshot SHA-256

| File | SHA-256 |
|---|---|
| `analysis/resume177-connector-next.md` | `e8ac9bd94385960a97ae28d9554c2dd59a783333b4827174c3d88212be13ea42` |
| `analysis/resume177-connector-plan-review.md` | `7162222c31ecd5e2e6621a7f5dc7394bd6eff471ab6c761faff1467185af6a70` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `71e8cb455a5e808144e19e1b1f2ced12783080722d9fcab93b33e8bdd2c21b90` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `6b5a4c207fc96d863a6463de06c536bda7cef9c934f304f3cc5ae178545d6bcb` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `df030fe660d7ff8f5268e47201118795c43fe8695b7c201c8c2d8be32b8862af` |

Only this report was written. No source/test edit, build, executed tests, GUI control, live REA, deployment or shared recovery-state change occurred. Executed acceptance gates remain with the parent; static evidence qualifies only the selected normal non-null path.
