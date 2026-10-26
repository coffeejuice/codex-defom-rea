# CP144: read-only new-C++ binding preflight

## Tool and focused self-test

Added `admin/check_new_cpp_bindings.py` with a focused 12-case Python self-test at `admin/tests/test_check_new_cpp_bindings.py`. It accepts explicit new/changed `.cpp` inputs and an optional project root or include-root list. The default include roots are the converter's `include`, `src`, and port `include` directories.

For `test_*.cpp`, it requires a `main`/`wmain`; when Testkit markers are present, the entrypoint must call `testkit::run_all()`. Plain legacy test harnesses with their own `main` are not mislabeled as Testkit tests. It checks quoted/angle `m2m/...` includes against the configured roots. For packet-local standalone source lists, it inspects each `// ctest-standalone:` directive and each explicit `add_executable(...)` in the packet `CMakeLists.txt`; if that list compiles `src/mst/emit_b_doe.cpp` without `src/doe/item_owner.cpp`, it reports a missing binding. A target that links `m2m_core` is treated as non-standalone.

Validation: `python -B -m unittest admin.tests.test_check_new_cpp_bindings -q` passed 12/12 in 0.061 s; AST parse passed. No compiler, configure, build, CTest, GUI, or mutable REA operation was run.

## CP142/143 snapshot measurement and finding

Seven in-process read-only scans of the two DOE owner tests, the two P09 emitter tests, and `tests/p09/CMakeLists.txt` measured **18.381 ms median** (18.110–20.121 ms). Their five-file input binding was SHA-256 `f4050bb6a306fa71b12b165c2044af3daa1b311907afbaabbd9fc60110171843` and was identical before/after. The checker produced two concrete findings: both explicit `add_executable` targets in `tests/p09/CMakeLists.txt` compile `emit_b_doe.cpp` but omit `item_owner.cpp`. The main-build `ctest-standalone` directives in `test_emit_b.cpp` and `test_keywords.cpp` already include that source, so the separate P09 subproject CMake targets remain a distinct linkage route. I did not edit either CMake file.

This demonstrates how a binding preflight could have supplemented CP142's focused target list: the reported CP142 repair3 focused build took 3.297 s and passed, but its target list omitted `m2m_p09_keywords`; the subsequent full-build step failed after 75.203 s at that target with LNK2019 for `snapshotItems` and `itemSnapshotText`. The preflight would have flagged the separate packet CMake source lists before invoking the full build. **75.203 s is the measured failed full-build duration, not a measured saving**: an expanded compile/link preflight for the second route has no separate timed receipt, and the failed run's compiled outputs were reused by the later build. The CP142 failed focused preflight durations (13.766 s and 2.687 s) plus the failed full step total 91.656 s of observed unsuccessful build phases; that sum is not a causal estimate of avoided time.

The checker would also report CP142's invalid `#include "m2m/mst/p17.hpp"` if the changed source file were passed and the configured roots lacked that header. It cannot determine that `CHECK_EQ` tries to stream a non-streamable `qt::String`, nor detect undeclared C++ names such as `doeControlDir`/`classInit` when all includes resolve. Those require focused compilation. Thus the low-risk sequence is: run this checker on the explicit changed/new tests and implementation files, then build the complete affected target set (including every standalone source-list route), then retain the existing full build and complete bound regression. The tool is an early diagnostic; it does not replace any compile, link, binding, or acceptance gate.

## False-positive and detection limits

- A file named `test_*.cpp` with a custom harness is checked for a `main` but is not forced to use Testkit unless Testkit include/macros appear. Unusual Testkit wrappers that use none of the recognized markers could evade the `run_all` check.
- `m2m` include resolution is lexical and depends on the caller supplying the real configured include roots. Generated includes, macro-expanded include paths, `#if` reachability, case-sensitive filesystem differences, and non-`m2m/` local includes are outside the rule.
- Standalone detection recognizes direct `src/mst/emit_b_doe.cpp` / `src/doe/item_owner.cpp` spellings in source directives and explicit CMake executable source lists. It does not expand CMake variables or follow object libraries, `target_sources`, custom commands, or other helper abstractions. It may miss an indirect omission; compilation remains authoritative.
- The current CP142/143 scan is intentionally not all-clean because it identifies two unresolved direct P09 subproject source-list bindings. The tool does not patch them.

## File identities

| File | SHA-256 |
|---|---|
| `admin/check_new_cpp_bindings.py` | `7419DD2842698B18075E7FDE151ACA1306CE33B18F52BE9D0EB7BFC0BE95C535` |
| `admin/tests/test_check_new_cpp_bindings.py` | `6E274FD9E51209B1558ADFBBAA31C62A68B15654CAFADAF4117404C41E645DDC` |

This receipt and the two new admin files are the only writes for CP144. Root decides whether and how to adopt the preflight.
