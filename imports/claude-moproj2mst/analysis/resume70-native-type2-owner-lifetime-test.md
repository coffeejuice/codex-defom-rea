# CP70 native Type2 observer owner-lifetime test

Added one bounded test-only native lifecycle case to source/apps/moproj2mst/tests/doe/test_type2_native_open_guard.cpp after exclusive ownership was assigned. Existing CP70 observer-case behavior is retained. The source is frozen and released to root for build/runtime validation.

Owned test SHA-256: 625bcdfbe5f5c2efbd970b5694dd6ba3ff1addf074d3e2d9222c0830bf79f210; 384 lines, two TEST_CASE declarations, one Testkit main. No production API, shared source, CMake/admin, GUI, mutable REA or original binary was changed. No build or executable test was run.

## Added behavior

New case: type2_native_journal_survives_refresh_guard_and_actual_session_teardown.

The case copies the retained 120_B_DOE project into a fresh contained scratch directory and verifies the complete source/copy manifest. It pins the project/XML hashes, uses the existing actual ConversionSession/NativeStudyIo setup, loads the empty-variable/zero-constraint Study and deletes the copied XML before invoking native Save. The current native observation sink is supplied through external FileEffects. The exact expected current guard is M2M_E_UNSUPPORTED with DOE table owner: main-window +458 is unavailable.

Before leaving the Session scope it verifies actual journal identity, one native opened/closed attempt, errorAfterClose 0, a normal XML success without stream failure and reached refresh boundary. Only the const journal owner, caller FileEffects and weak StudyIo escape. After actual Session/Project teardown it requires weak StudyIo expiry and continued readable journal identity/outcome/last-error snapshot, unchanged event count/tail and no recording-store/raw-byte/snapshot fabrication.

Physical XML is independently read after teardown: 249 bytes, exact retained bytes and SHA-256 1777bb9948fb2d6f55411f9dcfdc4a7c9196230086c0f86720255d03e76571e0. Main .moproj remains a regular zero-byte file. Source fixture manifest remains unchanged. The test leaves diagnostic scratch for root inspection and does not infer durable contents from journal counts.

Added explicit memory and m2m/qt4path.hpp includes for the new shared/weak pointers and cleanPath path comparison; no reliance on incidental header declarations. The post-teardown event vector has a REQUIRE-nonempty guard before back(), so a failed preservation check cannot trigger an unchecked empty-vector access.

## Static preflight and runtime gate

Passed source-only checks: balanced parentheses/brackets/braces after stripping comments/strings; all quoted includes resolve; exactly two cases and one main; no raw C assert; no trailing whitespace; git diff --check. These are not compilation or runtime acceptance.

Root target is m2m_doe_type2_native_open_guard; CTest m2m.doe.type2_native_open_guard should execute two cases after the new build. Root must retain focused/full results and the exact source/executable/archive binding. This change adds one case without adding a CTest registration; total successor registration depends on the other lanes. Do not reuse a prior one-case receipt as proof.

## Limits

This tests candidate internal native Save and actual candidate Session/I/O destruction. It does not claim public C ABI lifetime, original Qt/Windows failure parity, completed Type2/public Save, native refresh delivery, every OS handle release or general Close/global reset. It inherits configured scratch containment and does not assert short-path conformity. Any failed runtime observation requires investigation rather than relaxation of native behavior or replacement with transactional cleanup.
