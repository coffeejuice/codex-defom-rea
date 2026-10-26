# CP180 R300 hteSaveTmax implementation handoff

The one-function cut confines the raw step pointer and SimControl reference to an immediately invoked synchronous callback with explicit `void` return. `hteSaveTmax` still checks props, evaluates HeatingTime in its unit system, performs one `opModel` lookup, acquires the nullable step, returns on null and writes TMAX from TNOW1 or TNOW2 according to TNOW3. The callback captures only the evaluated scalar `t`; it stores no address, returns no address and schedules no work. No GuiModelAccess geometry refusal or `withOpStep` lookup was introduced.

Only `hteSaveTmax` changed in `source/apps/moproj2mst/src/engine/save_l3.cpp`. A static reverse-substitution reconstructed the exact pre-cut SHA-256, confirming the remaining production file is unchanged. No C ABI header, CP163 implementation, cache0/A logic or cache1 admission was edited.

The existing `air_convect_2d_active` copied-corpus fixture in `source/apps/moproj2mst/tests/mdt/test_engine_l3.cpp` now exercises both TNOW3 branches against the actual admitted cached model. It computes expected HeatingTime through the existing unit evaluator, uses distinct TNOW1/TNOW2 and old TMAX sentinels, and calls the function once with the actual cached step temporarily removed. The step is restored on normal and exceptional exits. Assertions check the cached model and step identities, unchanged null-step TMAX sentinel, and empty FileEffects. This is internal synthetic null-step coverage, not evidence that public Save reaches null step.

Static preflight confirmed the existing Testkit entry point and supported `CHECK_EQ`/`REQUIRE` macros. The new relative internal-header include resolves from the test directory; `<utility>` supplies `std::move`. Static inspection confirms the props/parse/lookup/callback order and a single production lookup. No artificial unit-parser hook or test-only production interface was added to test ordering: that property is statically audited rather than dynamically claimed.

Independent review caught an unqualified `DB_STEP_DATA` test declaration missed by the initial preflight. It is corrected to `engine::DB_STEP_DATA`, whose alias is declared in included `m2m/engine.hpp`. The added test statements were rechecked for name resolution: both engine functions are declared in the included internal header, `std::move` is qualified, and the remaining names are existing fixture locals or inferred local declarations. This correction has not been compiled or executed by this worker.

| File | Before SHA-256 | After SHA-256 |
| --- | --- | --- |
| `source/apps/moproj2mst/src/engine/save_l3.cpp` | `d63ac30c8df84a84aaab71890d8b2c9ca4595692801c77674ba51cf43af9668d` | `7c89166e6d2fede2b110578c332ecb690e62d94d180ae6d27be72c30759684a1` |
| `source/apps/moproj2mst/tests/mdt/test_engine_l3.cpp` | `14a0eb20d17918b92c684ce2b6236d97d536e83734b31d14688eb3e4f495fba4` | `c24892241818ed286c8cda68136cdae53dafa2d9e3e2a179dae1bb0c4a4982cb` |

Unchanged guard hashes: exported `include/moproj2mst.h` is `34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40`; `src/engine/simcontrol300.cpp` is `67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae` (paths relative to `source/apps/moproj2mst`).

No build, test execution, source freeze, GUI input, original execution, live REA operation, deployment or shared state update was performed by this worker. Compilation, execution of the expanded fixture, focused acceptance group, full source-stable suite and freeze remain for the integrating root. The callback narrows this local alias lifetime; public step/SimControl access and the aggregate parked-model gate remain unresolved. No cache1 evidence is added.

Ownership of the two source/test files is released to root with this report. The worker will make no further source edits unless reassigned.
