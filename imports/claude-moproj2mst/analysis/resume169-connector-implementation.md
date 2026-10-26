# CP169 connector implementation receipt

Implemented the selected QtCore normal-path store at RVA `0x1311bf` through exclusive end `0x1311c3` in the converter source. The retained sparse QObjectPrivate state now represents `+0x50` as initially unknown, admits the zero only from the `BeforePrivateZero50` phase after rechecking CP168 zero fields, pending Report/private identity, allocation sizes/address points, matching root ownership, pointer-identical qualified QList/QString bindings and committed reference markers, and incomplete/backlink state. Success changes only `private50` and phase (`BeforePrivateZero58`); the next `+0x58` bytes remain unknown. CP166/CP167 predecessor guards also reject premature `private50` observations before their `+0x40`/`+0x48` writes.

Added a focused CP169 test for one zero transition, same-key and different-key reentry refusal, stable owner/delta/count/completion state, and prewrite refusal for missing/nonzero predecessor fields and premature `+0x50`.

## Boundary audit

Audited every first-call boundary assertion in `test_doe_task_factory.cpp`: full factory calls now expect the CP169 refusal at `RVA 0x1311c3`, and immediate state assertions expect `BeforePrivateZero58` with `private50 == 0`. The CP168-specific transition test explicitly reconstructs its accepted boundary by disengaging `private50` and setting `BeforePrivateZero50`; earlier CP166/CP167 fault tests similarly reconstruct their intended phases. Added actual `ConversionSession` CP169 state/reentry assertions. The CP168 predecessor refusal matrix now covers premature `+0x50` before `+0x40` and before `+0x48`, capturing all field observations and confirming no mutation. No old `0x1311bf` expectation remains in the test file. The test target was not built or run per task instructions.

Following the independent source review, added a separate CP169-phase refusal matrix for nonzero `private40`, missing `private48`, prematurely observed zero `private50`, expired root, aliasing-foreign QList binding, foreign QString binding, both uncommitted markers, wrong task/private address points, replaced private identity, wrong phase, completion, and both backlinks. Each case snapshots and compares phase, private optionals, binding identities, commit markers, address points, completion/backlink flags, retained owner identity, root and foreign deltas, cache counts, and prototype/task counts. Symbol RVA/hash qualifications are static constants on the shared-null record types, so the test challenges binding identity with a foreign record instead of mutating qualification constants. Root loss is exercised by expiring the runtime after reaching the CP169 seam while retaining reference nodes for delta snapshots. The target remains unbuilt and unrun.

## Hashes and limits

SHA-256 after edits:

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `622e5a5e933cc897b5eb24c0a83ed5584de06170ad8dce8752c1503f2e8d9a7a` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `231bf736b2d8842063974aefbdf8f4818832dd2f0297332be55342bed05494d` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `f85fcc46dd0df2ebea9fbf1110866c3674b329b9cda97e1c6e26751056e66cc2` |

`git diff --check` completed without output. No build, test, shared plan/state edit, GUI-control session, or live REA session was performed. There is no GUI-control release receipt because no GUI-control session was acquired.
