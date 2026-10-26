# CP164 connector repair: adapter root admission

## Outcome

The first QList shared-null event now requires the `WidgetRuntimeState` supplied to `TaskFactoryRuntime` to be the same shared owner explicitly associated with its `TaskAllocationStore`. `ConversionSession` makes that association from its actual store and widget members at the start of its constructor body, before factory use. A factory constructor cannot nominate a root. The store compares shared ownership identities, so a different root with the same QtCore module digest and symbol RVA is rejected, including a replacement allocated at a recycled host address.

At `BeforeFirstSharedQList`, the factory checks that association before assigning the private `+0x18` weak binding or committing the symbolic `uint32_t +1`. Missing, unpaired, expired, or foreign roots retain the pending Report/private identities and the CP163 cursor. The admitted path still binds the existing root-owned `QListData::shared_null` state, advances once to `BeforePrivateFinalAddressPoint`, and refuses before QtCore RVA `0x131198`. No absolute reference count is inferred.

## Focused source checks

The existing admitted test now checks same-key and different-key reentry separately, including binding, frame identity, cursor, and unchanged delta after each refusal. A new test calls a foreign factory first on an unpaired store and again after explicit owner association; both calls leave the pending private state unbound and both root deltas at zero. Another test constructs two actual `ConversionSession` instances, uses the second Session's widget root with the first Session's store on that factory's first call, and checks refusal before binding or increment. A positive actual-Session test checks the production store/widget pairing and the admitted one-delta path. These are source tests, not execution results.

The focused test has `TEST_CASE` entries, resolvable `m2m/engine/conversion_session.hpp` and Testkit includes, supported `CHECK`/`REQUIRE` assertions, and no C `assert`. A static trailing-whitespace check of the five changed files found none. No build or tests were run under this worker assignment. Root integration still owes focused tests and the checkpoint build/regression gate. No GUI or live REA session was used.

The retained original instruction window is unchanged: QtCore RVA `0x131185` loads the qualified shared-null symbol, `0x13118c` writes private `+0x18`, `0x131190` increments the shared reference, and `0x131198` begins the later private address-point change. QtCore SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; the end-exclusive `[0x131185, 0x131198)` byte window SHA-256 is `110efb63fd2d3c3005f668334e43abf219e8382a25575708272be433a53b4772`. This static evidence does not establish a live absolute count, native failure unwinding, or TaskInfo completion.

## Changed-source SHA-256 receipts

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `44a2616e575e489052d0d6db850dfb9ddb0e8cc192576950a318504a7fed0c54` |
| `source/apps/moproj2mst/src/doe/task_runtime.cpp` | `5f8cb8cdb27189f06155524642e504b544736f6db24e3fe0dda749b6c16810bb` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `3caf344a07b946c396d5b3c4d645e3eec698eb779799973da67fa0ec4b1e41f0` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `e75573b6264abd07afc082472f1d532e9a0467ff262a458e15bc9242f3cd9d7d` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `bac15e6b20def8846dadba8c9653046272a0576166b009d992705baba0626f41` |

## Release

Exclusive source ownership is released for the five changed files. `view_manager.cpp/.hpp`, `doe_task_factory.hpp`, shared plan/state, CMake, build outputs, and deployments were not edited. This is a source repair handoff, not a tested or frozen checkpoint.
