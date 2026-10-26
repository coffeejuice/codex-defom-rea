# CP190 connector independent source review

Pass for the bounded source change. No blocking source defect found. Compilation and test execution remain integration gates; this review did not run either.

Reviewed `resume190-connector-next.md`, `resume190-connector-plan-review.md`, `resume190-connector-implementation.md`, the factory implementation, both runtime/factory headers, snapshot helpers and CP190 tests. Read `resume163-connector-qobject-static.json` before interpreting its disassembly: the capture is an unwind region, not complete logical-function or runtime proof. The retained listing at lines 34–37 agrees with the selected instruction and later ordering.

## Instruction and state

`task_factory.cpp:744–753` implements exactly public task+8 receiving the qualified RAX/private identity at `[0x131425,0x131429)`. `taskPrivateIdentity`, its marker and successor phase are nonthrowing assignments after qualification. `doe_task_runtime.hpp:197` derives the successor RIP as `0x131429`. Neither R12 copy, caller spill nor private+8 reverse publication is represented or executed. Sparse `private08` remains qualified zero; reverse-link, QObject completion, task/cache availability remain unavailable.

The early publication guard at factory lines 196–200 rejects any token or marker before the terminal, and admits only the allocation-bound value with its marker at the terminal. `qualifyRetainedZeroPrefix` preserves that phase-sensitive rule and rejects reverse publication and completion. Terminal admission is included in returned, RDI-restored and XOR-completed checks. Before the store and on terminal reentry, the flags loop revalidates public/private allocation sizes and identity, Report ordinal, address points, live widget root, shared-null bindings/control blocks, initializer version/module provenance, sparse zeros, symbolic flags, RAX/RBX/RDI and saved stack/CALL/RET lineage. Saved allocation owners require both pointer and ownership-control-block equality; alias owners do not pass. `TaskAllocationStore::ownsWidgetRuntime` likewise checks ownership and pointer identity.

Terminal ordinal 12 is validated before stopping, even with a smaller budget. The explicit terminal stop prevents indexing `flagsPhases[ordinal+1]` beyond its end. Production default is 12; historical fixtures explicitly pin 11. The maximum guard rejects 13 and maximum `size_t` before allocation or retained-prefix writes.

## Tests and report agreement

The five tests beginning at `test_doe_task_factory.cpp:6246` cover 101 corruption variants at each of predecessor and terminal, all three task keys, all budgets 0–12, a factory using the unmodified production default, every intermediate budget, premature publication, and expired widget-root refusal. Corruptions include value/marker combinations, phase-only rewind, invalid phase, allocation owner/control-block, ordinal, module/private identity, saved registers, flags, stack lineage, reverse-link and completion damage. `frameValues` includes the publication value, and full snapshots preserve reference/allocation counts and ownership provenance after refusals.

The publication test consistently clears token, marker and phase for its exact predecessor comparison, restores all three for terminal comparison, then tests a consistent rewind and resume. It explicitly retains `private08=0`; the full comparison establishes preservation of other represented observations. Invalid-stop tests use 13 and maximum `size_t` before allocation and at a retained prefix. These source observations support the implementation report's authored-test claims; they do not establish that the tests compile or pass.

Scanned the remaining `source/apps/moproj2mst/tests` files for factory boundary diagnostics and CP189/public-store wording. No stale default diagnostic expectation was found outside the focused factory test. The historical boundary assertions in the focused file are deliberately pinned to budget 11; the new default-specific case expects `0x131429`.

## Reviewed identities and boundary

SHA-256 values match the implementation report:

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `1d6497d084683e9ffd07e97cbcb7ba3453b20c97242651e0123a93bc340b6be1` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `67d824a237fda98618323684a7dee82d9ff0f80bfefc753449c7ecba9cc60ae3` |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `4a571e2edc2f66c7f88e30f0d10b3bd9324bfd29b30c96524486f5500eb32576` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `564f47970236a40d69a174be2e1fdcc1565ba6e55e60340ef0f924e1ae43a238` |

Only this review file was written. No source changes, builds, test execution, GUI, REA database, deployment or Git transport occurred. The evidence supports this local symbolic store and ordering; it does not establish successful native construction, concrete native pointers, prior task+8 contents, fault behavior or concurrent visibility.
