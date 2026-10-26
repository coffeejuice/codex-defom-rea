# CP125: independent grouped-preflight source audit

No probable compile/link issue was found in the frozen editor Spring off/empty tests, new DOE notifier-prefix test or final bounded SPRING2 hexadecimal reader test. Root was notified before its grouped preflight that this audit was not a build blocker. This is static source review, not a compile or runtime pass.

## Checks against actual declarations and targets

- The DOE test's quoted includes resolve to existing actual target include roots. Its Session notifier/table accessors and DoeTableOwnerState `record`, `report`, `requireRefreshDelivery` and `releaseAdmissionScope` methods are public. The notifier/table report definitions are complete at use. `SampleState::tables` is an array of nested standard vectors of double, so its equality checks do not require a missing custom comparison. Journal comparisons use existing optional/string types. No private Session storage-release API is called.
- DOE uses existing qt::String paths and RecordingStudyIo interfaces; it adds no unsupported qt::String→filesystem::path cast. The production cpp explicitly includes the notifier's complete definition. The test's DiagramViews implementation follows the actual existing interface and links through m2m_core.
- The editor test retains its existing native ProjectFiles fixture and supported `qt::String(root.u16string())` conversion. New SpringWriteTrace behavior uses existing public Extra3DIo virtual overrides and mst::OutputDevice callbacks. New cases call the already declared page Apply API and supported state fields; no additional header, link target or access bypass is required.
- The final hexadecimal change stays within the existing native reader and its existing test/Rig. It introduces no new include, Session API, path conversion or CMake target. Its final source/test hashes supersede the earlier signed-only receipt hashes.
- `CMakeLists.txt` already discovers `tests/<packet>/test_*.cpp`, assigns the target from packet/topic, and links ordinary tests to m2m_core/testkit. All reviewed production cpp files were already in the recursive core source glob. No missing explicit test registration or new implementation TU/link dependency was identified.

Exact focused target names:

| Test | Target | CTest | Cases |
| --- | --- | --- | ---: |
| New DOE prefix | `m2m_doe_refresh_notifier_prefix` | `m2m.doe.refresh_notifier_prefix` |5 |
| Existing private editor/page | `m2m_s_spring_page_io_roundtrip` | `m2m.s.spring_page_io_roundtrip` |19 |
| Existing Spring reader | `m2m_mdt_simcontrol300_spring_reader` | `m2m.mdt.simcontrol300_spring_reader` |2 |

The existing DOE table-owner assertion amendment is root-owned and noted by the source owner. It is a runtime-expectation coordination item, not a newly discovered compile/link defect; this review does not alter it or substitute for its focused execution.

## Automated static preflight and binding

One read-only invocation passed for all three test files:

```text
python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_refresh_notifier_prefix.cpp source/apps/moproj2mst/tests/s/test_spring_page_io_roundtrip.cpp source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp
```

It reported no issues for Testkit entrypoints, supported Release-active assertion macros or direct quoted includes against actual roots. The three files have unique TEST_CASE names. Source hashes read at audit time match the final worker receipts:

| File under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `src/doe/table_owner_state.cpp` | `aad5eb5382edf7df98add74576ffab8b89738bbff0cd0e6ccc2427c113ee466e` |
| `tests/doe/test_refresh_notifier_prefix.cpp` | `8c1789ff7e61e3c47e52df121c686c5ae739858986cc650bbeebca941cdb1b04` |
| `src/engine/simcontrol300_io.cpp` | `720828fd1a77cef0e3b6fbb7201c40be86fb847fe603f2e2f638ef8075045ee0` |
| `tests/s/test_spring_page_io_roundtrip.cpp` | `7e652d40e4e9463391a469fb61e0552d6110764cc087a1866ab08b7bc24a2df2` |
| `src/engine/simcontrol300_native_io.cpp` | `4caddeb4afaf65e64dcc8c879f2e6301a08023b40a6125f7b5165012ba1293a0` |
| `tests/mdt/test_simcontrol300_spring_reader.cpp` | `73a2faceeb196b9e6b131c12179057dc0a7a502ec7b756f77ebce4d361923e7d` |

Scope is the new compile/link failure classes requested after CP122's include/access/path-conversion rework. This report does not prove all templates instantiate, MSVC compiles/links, fixtures behave, or native semantics match. Root owns grouped build, focused execution, full source-stable suite and archive binding. No source/test/CMake/build file, fixture, GUI or REA state was modified. Only this report was written; token usage was unavailable.
