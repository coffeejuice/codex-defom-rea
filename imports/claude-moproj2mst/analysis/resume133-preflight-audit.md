# CP133: four-cut static preflight audit

No probable compile, link or registration blocker was found in the frozen NSTART, Type3 tokenizer, exact two-row SPRING2 hex-failure and empty-map-clear cuts. Five test files pass source preflight plus separate unique identifier-form TEST_CASE checks. Root was notified before the broad build; this report does not claim compiled/runtime or native acceptance.

## Interface and target checks

NSTART's root-supplied shared declarations match the new fillStepCount overload and StagedEditor300::toggleNstart definition. The existing two-argument fill remains declared; calls with the prior checkbox use the explicit three-argument overload. The new test uses public owner/binding/page methods, actual complete GuiModel/step types, standard registry/unique_ptr operations and existing Extra3DIo/mst::OutputDevice overrides. No private Session API or unsupported path cast is added. The new test is in an existing mdt packet and is an ordinary m2m_core/testkit target.

Type3's tokenizer header/definition/test signatures consistently use qt::String/vector<qt::String>. String::u16 returns the const u16string reference used by the implementation; substr results match the declared Qt-string constructor. UTF-16 surrogate fixtures use integer code units rather than invalid universal-character escapes. Identifier-form tests and supported assertions remain valid. Existing core/test globs cover the cpp/test; no Qt regex library or file parser dependency is introduced by this value helper.

SPRING2's added exact-profile argument and caller arity agree. It retains existing Rig/IO/path interfaces and target. Full companion equality/count-two controls the retry permission; no new header or external symbol is required. This source review does not itself prove the stream-state/native retry interpretation.

FeatureMapProxyCall::resumeEmptyMapCleanupPrefix is declared publicly in the existing private runtime header and defined in its existing cpp. New report/stage/effect fields are complete at test use. The private storage/runtime access follows existing friendship and source types; fixed14-effect arrays contain ordinary enum values. Existing standalone and context executables remain separate, so no new source registration, external library or cross-executable symbol is required. Metadata/lifetime/native-clear semantics remain subject to the independent source review and root runtime tests.

| Build target | CTest name | Cases |
| --- | --- | ---: |
| `m2m_mdt_simcontrol300_nstart_toggle` | `m2m.mdt.simcontrol300_nstart_toggle` |8 |
| `m2m_doe_type3_optimizer_scalar_prefix` | `m2m.doe.type3_optimizer_scalar_prefix` |16 |
| `m2m_mdt_simcontrol300_spring_reader` | `m2m.mdt.simcontrol300_spring_reader` |2 |
| `m2m_model_feature_crt_runtime` | `m2m.model.feature_crt_runtime` |9 |
| `m2m_model_feature_context_prefix` | `m2m.model.feature_context_prefix` |1 |

CMake's existing CONFIGURE_DEPENDS packet glob supplies the new NSTART target after configure. No manual registration is missing. A direct comparison against CP132 source.zip confirms model.hpp, CMakeLists.txt and p11 textual-source code remain byte-identical; the prior standalone cache implementation wiring is intact. The changed editor headers are root-owned declarations, not an uncoordinated worker edit. Their exact read identities are included below.

## Static checks and source identities

One read-only `admin/check_test_source_preflight.py` invocation passed for all five tests: actual quoted include roots, Testkit entrypoint and supported Release-active macros. A separate scan confirmed8/16/2/9/1 unique valid identifier arguments. No compiler, linker or CTest/test executable ran in this audit.

Paths below are under `source/apps/moproj2mst`:

| File | SHA-256 |
| --- | --- |
| `include/m2m/engine/simcontrol300.hpp` | `6cf4f9b8d444839f39a9ac3cb831f6c0f313fb0763e8aeac3d16963db12d67ed` |
| `include/m2m/engine/simcontrol300_owner.hpp` | `3865aaf7c4670cb63e5c88d91c7a4a8605235f6f888675d9bd44e7ac5b62b364` |
| `src/engine/simcontrol300.cpp` | `04b62ac1da494473cb26e1dcb3d1ae93ad087cf3c3fa4409f37e8ee6336f9a80` |
| `src/engine/simcontrol300_owner.cpp` | `65af624f02b7a322a041d0f114bdf2bfba1aa5d22a9da538c20dd061c4378e8c` |
| `tests/mdt/test_simcontrol300_nstart_toggle.cpp` | `0d98957da0495ca7782d47292e3e776c37973beb6d176e884cab1d04d5fda0ed` |
| `src/ops/type3_optimizer_scalar_state.hpp` | `9a1aabdf47f1332643e9d09537ba8483f5f80891f54e9bda454c81d3d5f7b946` |
| `src/ops/type3_optimizer_scalar_state.cpp` | `715033d53f0b265c49e6fb075f9c32bc4317bb4787a2c40d4bfd5f2a317e7eb6` |
| `tests/doe/test_type3_optimizer_scalar_prefix.cpp` | `793c22aee7c6ceeabbcd6fd6fd54b19b3129b0230fa84b64e2b32ddc483792f2` |
| `src/engine/simcontrol300_native_io.cpp` | `0b4832377e1eb03a5c68aeef15a4519f1d9844d1193db115857d1ba087a79b8b` |
| `tests/mdt/test_simcontrol300_spring_reader.cpp` | `c9a4b3fa1cf51ec77b0c02eb21b942d0a7826c450d419e2ef0e535922c1496d5` |
| `src/model/feature_crt_runtime.hpp` | `1164febe2b9befdeb49c39bde91ed5fc52c2f4fef50fc487a9d1c09604838a73` |
| `src/model/feature_crt_runtime.cpp` | `7f26fa734a47931b29e8829133e8e1e3ac4b0d4c038a36f9b46a5b77ccfc00f3` |
| `tests/model/test_feature_crt_runtime.cpp` | `591f5d0b4f2e5cc0897ed34266e7af7ae26a18033c7a2532db30bdadebc19435` |
| `tests/model/test_feature_context_prefix.cpp` | `153c8f94cd252148451288b3ec27a59bc003a11703faa4996b1dd2ded893864b` |

These identities match the released cuts at the static read boundary; root owns final source/build/test/archive binding. No source/test/CMake/build/fixture edit, executable launch, GUI or mutable REA operation occurred. Only this report was written. Static preflight cannot replace runtime checks or native-evidence qualification. Token usage was unavailable.
