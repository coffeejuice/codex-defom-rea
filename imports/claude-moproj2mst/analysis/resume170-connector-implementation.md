# CP170 connector implementation: QObjectPrivate +0x58 zero

Implemented the selected QtCore instruction cut `[0x1311c3, 0x1311c7)`. The retained Report private state now represents `private58` as initially disengaged; its admission requires the exact CP169 frame, retained allocation-time private identity, allocation-store-owned live root, qualified pointer-identical committed QList/QString root bindings, private/task allocation sizes and address points, zero `private40`/`private48`/`private50`, and incomplete/unbacklinked QObject state. The transition writes only `private58 = 0` and advances to `BeforeQVectorSharedNull`. Every earlier zero phase rejects premature `private58`. Reentry now stops at RVA `0x1311c7`, before the QVector shared-null load.

The focused Testkit test covers the one-time CP170 result, same-key/different-key refusal, and CP169 field/phase/completion/backlink fault refusal with a no-mutation snapshot. Following independent review, all first-call phase expectations now use `BeforeQVectorSharedNull`; CP168/CP169 seam fixtures clear and verify absent `private58`; the CP169 wrong-phase expectation matches the earlier CP168 guard. The CP170 prewrite matrix now starts each injection at `BeforePrivateZero58` with `private58` absent and includes missing/nonzero predecessor fields, absent and foreign factory roots, foreign and aliasing-foreign QList bindings, foreign QString binding, uncommitted markers, bad address points, replacement private identity, wrong allocation sizes, premature `private58`, base/frame completion, and backlinks. Its refusal snapshots include pending/private identities, fields, phase, bindings, commit markers, address points and sizes, owner/shared-root identities, deltas, allocation counts, cache counts and completion flags. The existing real `ConversionSession` owner test remains present.

The accepted CP169 prerequisite was found in `analysis/wave3-checkpoint169-validation.json`: focused `doe_session` 0/41 and `doe_task_factory` 0/38 failures; checkpoint regression 361 registered, 354 clean, 6 logged gaps, 1 logged difference, 0 incomplete, 0 skipped, 0 failed; no fixture changes. Its qualifications include that the historical comparison is not fresh original-session acceptance evidence and that this step does not close the larger recovery objective.

No build or tests were run for CP170 under this owner assignment; the corrections are static only. No shared plan/state/CMake, GUI, live REA, original process, deployment, or user application was touched. No app-control session was acquired, so no release operation was needed.

## SHA-256

- `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp`: `2aa6dfaeb664511d5987c02e35837ec542f3141d377829a0c2a827e55a09cf9c`
- `source/apps/moproj2mst/src/doe/task_factory.cpp`: `a3e61c964c57e6733497fd3c395f303081455aa213ca2dc47d3e54d7fb307f77`
- `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp`: `C2AB71115559CAEB2AEC200E17C031D7A0CC68453FCB330A6C0C0CE867C2A081`


## Follow-up fixture repairs after independent re-review

The CP170 matrix now uses a distinct retained frame for each injected malformed state. This avoids cross-case pointer lifetime issues; the replacement-private injection retains the original pointee until after its refusal snapshot. Root expiration is exercised with the original factory and its existing retained CP169 frame, at the `BeforePrivateZero58` seam with `private58` cleared, so it reaches the intended admission guard without allocating another Report. The small CP169 predecessor loop explicitly clears and snapshots `private58`. The stray undeclared `allocationIdentity` assertion reported by the grouped focused preflight was removed; only the matrix's locally captured identity remains. These are test-only edits. No build or tests were run in this follow-up.

Latest test SHA-256: `7ca7063147487ac60e5831231e1fd39e195a89fe29e196adde2287a4518b280e`.


The CP170 matrix root-binding locals were renamed `matrixList` and `matrixString` to avoid redeclaration with the primary one-time-store fixture. `git diff --check` passed; no build/test was run. Latest focused test SHA-256: `7ca7063147487ac60e5831231e1fd39e195a89fe29e196adde2287a4518b280e`.
