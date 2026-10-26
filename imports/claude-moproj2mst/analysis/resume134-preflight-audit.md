# CP134 static build preflight

No probable compile, link or test-registration blocker was found in the four frozen cuts. The static source helper passed for all five affected test executables. This is a source preflight receipt; root owns compilation, runtime checks, the full suite and the candidate archive. I changed only this report and ran no build, executable test, GUI, original target or mutable REA operation.

## Checked target set

| Cut | Target | CTest name | Valid unique Testkit cases |
| --- | --- | --- | ---: |
| Private title delivery | `m2m_mdt_simcontrol300_title_changed` | `m2m.mdt.simcontrol300_title_changed` | 8 |
| Exact two-row SPRING2 `018` | `m2m_mdt_simcontrol300_spring_reader` | `m2m.mdt.simcontrol300_spring_reader` | 2 |
| Initial geometry-frame retirement | `m2m_mdt_viewer_box` | `m2m.mdt.viewer_box` | 48 |
| Isolated sentinel free | `m2m_model_feature_crt_runtime` | `m2m.model.feature_crt_runtime` | 11 |
| Embedded context sentinel free | `m2m_model_feature_context_prefix` | `m2m.model.feature_context_prefix` | 2 |

`python admin/check_test_source_preflight.py` passed with these five explicit source paths. Independent scans found identifier-form, unique `TEST_CASE` arguments, one `main` per source, and no `REQUIRE_EQ` or C `assert` calls. The helper checks include closure but does not itself validate Testkit case-argument form; that additional check was performed separately.

The title test is new in the existing `mdt` packet. The existing `CONFIGURE_DEPENDS` `test_*.cpp` glob produces the target/name above, and the ordinary test branch links `m2m_core` and `testkit`. Root must let CMake regenerate; no manual target edit is needed. The other four targets already exist. No new production translation unit, external library or standalone packet-double dependency was introduced.

## Source checks

The root-owned `StagedEditor300::titleChanged(Editor300Binding, const qt::String&)` declaration matches the owner implementation. Against CP133's source archive, that header differs only by the new declaration and its comment; no class data member or existing signature changed. The implementation adds `<cstring>` for `std::memcpy`. The new test's defmodel layout include supplies the complete `DB_STEP_DATA(3)` type used by its replacement-control fixture. Its API calls, overrides and private-page types match current declarations. Its public-completion exclusion remains asserted.

The Spring helper's added Boolean parameter and its call agree. The new admission is `count == 2 && flagBytes == "018\r\n"`; it uses the existing reader/test target and adds no include, exported API or link dependency. The exact first-token continuation and later decimal read do not introduce a second parser implementation.

Geometry's completer is a private static member of the existing `GeometrySyncAccess` friend. `std::shared_ptr::owner_before`, `reset`, `GuiModelGuard::get`, and the complete `DB_STEP_DATA` type are available. The one completer call is inside the original synchronous `withStep` callback. Its new test include `<defmodel/llobj_ctor/llobj_ctor.hpp>` resolves to the existing typed helper already supplied by the linked core/defmodel closure. The new enum value and local completion flag require no shared interface edit. Semantic/native fidelity is separately reviewed in `resume134-geometry-retirement-source-review.md`; this receipt does not substitute for that review or runtime execution.

The sentinel-free method and pure zero-result helper have matching header declarations and source definitions. The existing runtime friend permits `FeatureMapProxyCall`'s retained-runtime access. `<windows.h>` already supplies `HANDLE`, `BOOL` and `HeapFree`; this TU already uses the Windows heap APIs, so the cut adds no new library linkage. Its report types use existing `<array>`, `<optional>`, `<memory>` and integer includes. The fixed effect array has ten slots and the success path records ten events; the declared zero path records five. The actual success and declared failure tests are explicitly distinguished. No Project header, C ABI or default Close integration is introduced by these files.

## Frozen source binding

Paths below are relative to `source/apps/moproj2mst`. All hashes were computed from the active frozen bytes and match the owners' releases.

| File | SHA-256 |
| --- | --- |
| `include/m2m/engine/simcontrol300_owner.hpp` | `34336317aba2d8ff934eadd97aefb01862eee40854ada44af4983cc5370ce097` |
| `src/engine/simcontrol300_owner.cpp` | `43a24def1e3bb5e1d9380e604c56dcc1d05f1c300c3cbfec110eeabf2553ec74` |
| `tests/mdt/test_simcontrol300_title_changed.cpp` | `5112bf42c01cf43f9fbbab24cb9acac753a47674f739696422ad64c87436a7e6` |
| `src/engine/simcontrol300_native_io.cpp` | `34ca7bb60bb44fcf3a0fa580bfff28a24d02c4b3f991b0b6086d136ee95b38d8` |
| `tests/mdt/test_simcontrol300_spring_reader.cpp` | `c444ac4773bf11c2347ff6272bcfc6e050108a6d77508d87f1b1d180932c2f9e` |
| `src/engine/viewer.cpp` | `c4897abb365516e9e3056c028b40ac26be4ea3df56c04e0aa0aa70fa4a7b1f61` |
| `tests/mdt/test_viewer_box.cpp` | `888f7e0b80e4a3c57c0316f4bfb46f4567a956239c88b3c658c583459198808b` |
| `src/model/feature_crt_runtime.hpp` | `b2fb04cb60a05a06188264456075a2b1f2700bc65087b388c011677a6be7ad54` |
| `src/model/feature_crt_runtime.cpp` | `c02fb82749f1cee9c552d4d2216856893c8cf8021f41cbfb25fd04317eb0a01d` |
| `tests/model/test_feature_crt_runtime.cpp` | `a52cfc0cc545036b28fbd3cde4d3d7900eacdf07f571b35c6767be1f5ce39c37` |
| `tests/model/test_feature_context_prefix.cpp` | `a46d9009174b04aa5919527b4d6226926f6866c5d8f3908673673db21bf5c1a5` |

The following active shared files were byte-identical to their entries in `build/wave3-checkpoint133-verified/source.zip` during this review:

| File | SHA-256 |
| --- | --- |
| `include/m2m/model.hpp` | `baf657f01683196d78a45cbb180273cfa8d10b57c0523110171fe4fe235e078d` |
| `CMakeLists.txt` | `2d4e0d5ef2addce9011df45db49304e127d20ffd929ee7177ac96916e1d89cb5` |
| `tests/p11/test_xcheck.cpp` | `0da6511f62388a36cc9ddec08b352577e54ea7af9cac434af91dca538e8d02a5` |

The expected include-driven rebuild comes from the editor owner header and private feature-runtime header consumers. No global `model.hpp` edit expands this checkpoint's invalidation. This audit measures no build duration and claims no elapsed-time saving or acceptance beyond the static checks above.
