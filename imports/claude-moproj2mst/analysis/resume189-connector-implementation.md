# CP189 connector implementation

The bounded native factory prefix now executes the normal non-null caller jump and stops at QtCore RVA `0x131425`, immediately before the public `task+8=private` store. The implementation adds phase `BeforeTaskPrivateBacklink`; `returnBoundaryRva()` derives the new cursor directly from the phase. There is no separately mutable RIP or branch predicate.

## Qualified evidence

Read `analysis/resume189-connector-next.md`, `analysis/resume189-connector-plan-review.md`, `analysis/resume163-connector-qobject-static.json`, and the bounded caller listing. The retained QtCore SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The capture is an unwind-region listing, not complete logical-function or runtime proof. Bytes `EB 05` at `[0x13141e,0x131420)` give target `0x131420 + 5 = 0x131425`. The null-arm XOR/MOV at `0x131420`/`0x131422` are skipped. Neither `48 89 43 08` at `0x131425` nor `48 89 58 08` at `0x131431` is executed.

## Source changes

- `source/apps/moproj2mst/src/doe/task_factory.cpp`: admits the terminal in the existing return/RBX/RSI/flags/stack guards, extends the instruction table through ordinal 11, and executes ordinal 10 as a phase-only jump. Retained owner/control block, pending Report ordinal, module identity, allocation identities, saved-register and return-slot qualifications remain required. Terminal ordinal 11 always stops, even when reentered with a smaller diagnostic budget, so no fallthrough can replay the jump or index past the phase table.
- `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp`: new terminal phase and derived `0x131425` cursor; earlier cursor values remain intact.
- `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp`: default/maximum diagnostic stop moves from 10 to 11; invalid ordinal 12 still refuses before allocation.
- `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp`: default-terminal expectations move to CP189 while explicit CP188 budget-10 tests remain intact. New cases compare the complete CP188 predecessor snapshot after reverting only phase; preserve zero RSI/XOR flags, stack, return lineage, sparse memory, allocation/reference/cache observations, and both unavailable backlink markers; cover terminal reentry at all budgets 0..11; cover all intermediate instruction stops and resumed completion; and exercise 81 corruption variants at both predecessor and terminal without mutation.

## Validation and release

Static preflight passed: all six quoted includes resolve against the existing source/include directories or test directory; the existing Testkit main is present; 111 test case names are unique; no C `assert` or unsupported `REQUIRE_EQ` appears. The new tests use existing `CHECK`/`REQUIRE` and snapshot helpers. No new include or fixture filesystem path was introduced.

No build, test execution, full-suite run, GUI, REA database, deployment, or shared coordination-state operation was performed in this lane. Compilation and focused/group integration tests remain root-owned gates. This report makes no runtime, parent behavior, constructor-completion or full recovery claim. The four exclusive source/test files and this report are released to root for integration.
