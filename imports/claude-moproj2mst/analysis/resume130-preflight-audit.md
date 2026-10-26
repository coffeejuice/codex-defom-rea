# CP130: meshing, feature sentinel and SPRING2 preflight

No probable compile, link or test-registration issue was found in the three frozen cuts. All tests pass the existing static source preflight and a separate unique-identifier TEST_CASE check. Root was notified before broad build completion; this report is not a compiled/runtime acceptance claim.

## Declaration and build checks

The new meshing test includes actual app headers plus existing defmodel LLOBJ/DB-step/deletion headers. GuiModel::object/objectWrap are public with the used signatures; object_list_remove returns bool as required by REQUIRE. The fixture has complete step/object types at field access. Shared MeshingPage fields, event/report enums and transition/Apply declarations match the production definitions and test calls. Root's simcontrol300.hpp/lifecycle hashes match the worker's read-time dependency identities. No missing String alias, private lifecycle access, unsupported path conversion or string-literal TEST_CASE argument was found.

The meshing test is an ordinary `tests/mdt/test_*.cpp` target, not standalone. Existing CMake packet discovery registers it after configure as `m2m_mdt_simcontrol300_meshing_selection` / `m2m.mdt.simcontrol300_meshing_selection`, with m2m_core/testkit and inherited port include/link dependencies. It needs no manual CMake entry. The production meshing cpp and root lifecycle cpp were already core sources.

The sentinel API adds its declaration/definition in the existing private CRT header/cpp and uses the existing runtime test target. New report fields/types are complete at test use; resumeSentinel is public with matching signature. Its private runtime/storage access follows existing friendship. Placement new, standard-layout/trivial-destructor/offset checks and std::byte use existing explicit/type-header dependencies; no new external library or implementation TU is required. The fixed20-effect success trace matches its declared capacity by source count. This does not establish runtime allocation/retention behavior or native constructor acceptance.

SPRING2 retains the existing reader/Rig/source target and supported path conversion. Its exact token branch introduces no new header, access or linker dependency. Final source/test bytes match the frozen token receipt.

| Build target | CTest name | Cases |
| --- | --- | ---: |
| `m2m_mdt_simcontrol300_meshing_selection` | `m2m.mdt.simcontrol300_meshing_selection` |8 |
| `m2m_model_feature_crt_runtime` | `m2m.model.feature_crt_runtime` |9 |
| `m2m_mdt_simcontrol300_spring_reader` | `m2m.mdt.simcontrol300_spring_reader` |2 |

Existing standalone cache-source and p11 linkage repairs remain present in CMake/source; this lane introduces no new dependency into those targets.

## Static checks and exact source binding

`python admin/check_test_source_preflight.py` passed for all three test paths in one final invocation. It resolved direct quoted includes against actual app/port/Testkit roots, recognized main/run_all and supported Release-active assertions. A separate scan confirmed8/9/2 unique valid identifier TEST_CASE arguments, addressing the macro-argument blind spot found in CP129. No compiler, CTest or test executable was run.

Hashes below are under `source/apps/moproj2mst` and match worker freezes/shared root dependencies at the read boundary:

| File | SHA-256 |
| --- | --- |
| `src/engine/simcontrol300.cpp` | `a657daa78e1a7b9c3618e37422cf1d8e29a7d28f368fcdcbdae16bc166a9bccb` |
| `include/m2m/engine/simcontrol300.hpp` | `f014239ace1dc9633dec3033aa3744d177f188c4fd8950196ed6b4b3b234fea7` |
| `src/engine/simcontrol300_lifecycle.cpp` | `245fe59467e0852f6a34c540a975dcf5fe7e564922f80cad3a00e6cfdb3d6854` |
| `tests/mdt/test_simcontrol300_meshing_selection.cpp` | `6b7226249296e8e89b70bdcdf5a996dd58fe8ca58b7043241a9c95409e57e6be` |
| `src/model/feature_crt_runtime.hpp` | `053ee825f92374cd7a61b88d83381b2ebefb3fcc2bdcb5aca657c998b8b72903` |
| `src/model/feature_crt_runtime.cpp` | `cc9c9a72ca0302148615ba51f384a51a291f40bedabda218b6a51dfe9f43e3d7` |
| `tests/model/test_feature_crt_runtime.cpp` | `c26ca7daa2e91770dcc6526e4bb0be885b5c9f55ebbceca08f6c96a971f441ac` |
| `src/engine/simcontrol300_native_io.cpp` | `4035164d5a420aa1e892e3d6b39c940eaee1d9d5f83134f6c76009a2289da613` |
| `tests/mdt/test_simcontrol300_spring_reader.cpp` | `0c9183b5ee38504295ffdadd62f4b4ea6f0a3af855f0f0f426751c64312da171` |

Worker receipts: `analysis/resume130-editor-meshing-selection.md`, `analysis/resume130-feature-sentinel.md`, `analysis/resume130-spring-token.md`. Root owns actual compile/link, focused execution, complete source-stable suite, fixture/executable binding and archive. Static checks cannot establish template instantiation, positive runtime effects or native fidelity.

Only this report was written. No source/test/CMake/build/fixture edits, compilation, executable launch, GUI or mutable REA operation occurred. Token usage was unavailable.
