# CP148 focused build and CTest invocation

The current candidate inventory contains the requested CTest names, and the CMake loop derives both executable targets and CTest names from `tests/<packet>/test_<topic>.cpp`:

| Test file | Focused build target | Exact CTest name |
|---|---|---|
| `tests/s/test_spring_page_io_roundtrip.cpp` | `m2m_s_spring_page_io_roundtrip` | `m2m.s.spring_page_io_roundtrip` |
| `tests/s/test_session_cogging_save_geometry.cpp` | `m2m_s_session_cogging_save_geometry` | `m2m.s.session_cogging_save_geometry` |
| `tests/mdt/test_doe_output_data.cpp` | `m2m_mdt_doe_output_data` | `m2m.mdt.doe_output_data` |

Run the grouped flow with the three focused targets and exact runtime names:

```text
python -B admin/build_candidate.py --label cp148-focused --then-full --preflight-target m2m_s_spring_page_io_roundtrip --preflight-target m2m_s_session_cogging_save_geometry --preflight-target m2m_mdt_doe_output_data --preflight-ctest m2m.s.spring_page_io_roundtrip --preflight-ctest m2m.s.session_cogging_save_geometry --preflight-ctest m2m.mdt.doe_output_data
```

CTest discovery is automatic: CMake uses `file(GLOB ... CONFIGURE_DEPENDS "${dir}/test_*.cpp")`, creates `m2m_<packet>_<topic>` for each source, and registers `m2m.<packet>.<topic>`. A newly added `test_<topic>.cpp` under an already-discovered packet is picked up on the build's automatic CMake regeneration. No CMake edit is needed for this test. The focused compilation runs before CTest, so the executable is built before invocation.

The builder's runtime gate runs only for explicitly supplied `--preflight-ctest` values. It has no test-selection or build side effect when absent. For this invocation, the focused tests may have their documented scratch effects; the same authorized `C:\PROBLEM` scratch permission required by the full suite is required here. A named test access-denied result must fail the focused gate and prevent the full build. This inspection used the read-only `ctest --show-only=json-v1` inventory and source inspection; no build or CTest test command was run.
