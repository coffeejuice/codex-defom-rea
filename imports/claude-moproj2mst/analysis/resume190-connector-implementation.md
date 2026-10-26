# CP190 connector implementation

Implemented the bounded public task+8 publication at QtCore RVA `[0x131425,0x131429)`, `48 89 43 08` / `mov [rbx+8],rax`. The new `BeforeCallerPrivateCopy` phase derives RIP `0x131429`; R12 copy, caller stack spill and private+8 reverse publication remain outside this cut.

## State and guards

`taskPrivateIdentity` is a value-bearing symbolic allocation token, not a native address. Its nonthrowing assignment, publication marker and successor phase commit after complete CP189 prefix qualification. A global phase-sensitive publication guard rejects premature value/marker combinations before any earlier prefix writes. The retained-zero qualifier admits exactly the new terminal's allocation-bound publication, keeps earlier cuts strict and still rejects reverse publication and completion. Returned/RDI-restored/XOR-completed admission includes the new terminal.

Existing ordinal, public/private allocation, address-point, live widget/shared-null, sparse zero, flags-expression, initializer provenance, owner/control-block, restored register, symbolic stack and CALL/RET lineage guards remain active. The new terminal is fully validated before the diagnostic stop regardless of a smaller requested budget. The instruction loop terminal is now ordinal12, with an explicit maximum guard preventing an `ordinal+1` access at the terminal. Ordinal13 and maximum size_t refuse before allocations and at retained prefixes.

Only the task+8 observation and phase change. Private+8 remains qualified zero. No allocation, reference delta, register/flags/stack effect, R12/spill observation, completion, TaskInfo or cache entry is added. Production default diagnostic budget advances to12. Historical tests explicitly pin budget11 to retain their exact predecessor contract; publication-damage expectations now identify the new early guard.

## Focused tests authored

Five CP190 Testkit cases add 202 predecessor/terminal corruption setups (101 damage variants at each cut), exact full-snapshot preservation comparison after consistently rewinding the publication value/marker/phase, all-budget/all-key terminal reentry, consistent rewind/resume, all intermediate diagnostic budgets, an unmodified default-budget instance, premature publication at every earlier instruction cut and before first QList binding, and expired-widget-root refusal at both cuts. Snapshot `frameValues` now includes the published token. Existing invalid-stop tests now use13 and maximum size_t.

Corruptions include missing/foreign published values and marker combinations, phase-only rewind, invalid phase, absent/foreign/alias-control-block owners, ordinal, module, private identity, initializer provenance, RAX/RBX/RDI, XOR flags, symbolic stack and return lineage, shared-null bindings, reverse publication, completion and sparse fields. Full snapshots preserve failed inputs and reference/allocation counts.

## Validation and boundary

Static preflight confirmed the existing Testkit entry point, supported CHECK/REQUIRE macros, resolvable quoted includes, and scopes for adjusted enum/damage diagnostics. No new quoted includes or C asserts were introduced. No build, executable/test execution, GUI, REA database, deployment, shared-state mutation or Git transport operation was performed. Root integration must compile and run the changed focused target and the source-stable acceptance group.

Read `resume190-connector-next.md`, `resume190-connector-plan-review.md` and the retained `resume163-connector-qobject-static.json` qualification. The underlying capture is an unwind region, not complete logical-function/runtime proof. Static evidence establishes the local instruction effect and ordering only; successful native construction, concrete addresses, prior task+8 bytes, fault behavior and concurrency visibility remain unestablished.

## Released files

| File | SHA-256 at release |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `1d6497d084683e9ffd07e97cbcb7ba3453b20c97242651e0123a93bc340b6be1` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `67d824a237fda98618323684a7dee82d9ff0f80bfefc753449c7ecba9cc60ae3` |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `4a571e2edc2f66c7f88e30f0d10b3bd9324bfd29b30c96524486f5500eb32576` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `564f47970236a40d69a174be2e1fdcc1565ba6e55e60340ef0f924e1ae43a238` |
