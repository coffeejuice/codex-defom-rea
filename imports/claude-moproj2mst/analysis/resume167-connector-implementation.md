# CP167 private+0x40 implementation

Implemented the qualified QtCore cut `[0x1311b5, 0x1311bb)` on the retained Report's same `SparseQObjectPrivate` object. The new `private40` field is an optional `uint64_t`, so it starts unknown. At `BeforePrivateZeroFields`, the factory validates the exact pending Report/private identity, sizes and address points, incomplete QObject/backlinks, committed CP165 QList and CP166 QString bindings against the allocation store's associated live root. It then records only `private40 = 0` and advances to `BeforePrivateZero48`. Reentry refuses at `0x1311bb` without repeating the field write or symbolic deltas.

Focused tests cover the single zero and untouched owner/deltas, repeated same-key and different-key calls, rejection of an aliasing foreign root without writing, and the positive `ConversionSession` pairing. Existing QtCore field observations at +0x48/+0x50/+0x58 do not exist in the model and remain unknown. No TaskInfo, task completion, backlinks, or later constructor state is introduced.

Testkit preflight: test file already has the `TEST_CASE` entry point and uses supported `CHECK`/`REQUIRE` macros; no C `assert` was added. New standard-library uses are `std::optional`, `std::uint64_t`, and existing `shared_ptr` facilities. `test_doe_task_factory.cpp` includes `m2m/doe_task_factory.hpp`, whose production include chain resolves `doe_task_runtime.hpp`; the test already includes the session/runtime headers used by these fixtures. The source review caught stale first-call expectations; those are repaired. No test/build command was run, per assignment.

Continuation check: `python admin/check_continuation.py` returned `CONTINUE`, 15 remaining items, no blocked items or errors. No shared plan/state, CMake, build, full test suite, GUI, live REA, or app-control session was touched. No app-control session was acquired, so there is no session to release.

## SHA-256

- `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp`: `25959ea6f2786d4bc3abf645da6751e95688f6c0fe721e28df045abe4230465`
- `source/apps/moproj2mst/src/doe/task_factory.cpp`: `b8d56166ae963390213d8348dcc01f9085500416378bd802da198c4e8f0fd37a`
- `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp`: `a03cad51410d191ac7be6d8a37353a11bcdd5fa36531cde19164a3cf2af454e3`







