# CP105 isolated terminal harness safety implementation

The five approved source files are frozen for root build/validation. This implements a candidate-only self-test supervisor/child executable. It does not call or wire conversion, remesh Save, the public C ABI, normal Close or the original application. No handler profile or original terminal branch is enabled.

The existing CMake glob discovers target `m2m_mdt_terminal_harness_safety`, CTest name `m2m.mdt.terminal_harness_safety`, with11 Release-active Testkit cases and16 explicit adverse/control child modes. Testkit runs the parent; only a nondefault child role with the literal termination opt-in, pipe handle and nonce enters child mode. The private supervisor additionally requires an explicit opt-in, the exact own executable identity, matching expected SHA-256, bounded settings and a new canonical scratch leaf. The expected test image hash is measured from the running self image; root's normal pre/post test executable/source receipts must supply the independent build binding.

## Implemented safety behavior

The protocol uses40-byte explicitly little-endian records with magic/version, nonce, kind, unique SelfTestEndpoint site, status/error/detail and checksum. The collector incrementally buffers one partial frame, limits record count, rejects stale/wrong/malformed/conflicting/final-state data and validates EOF completeness. SelfTestTerminal is separate from a real remesh source occurrence. Timeout and all supervisor stop reasons dominate terminal-looking exit status; a boundary record alone is only reach evidence.

The supervisor launches only its own hashed image, suspended and with an explicit restricted handle list. It assigns a kill-on-close job before resume. Protocol input is single-reader: PeekNamedPipe establishes available byte count and ReadFile requests no more than that count, in bounded256-byte chunks, checking one monotonic deadline before every further read. There is no blocking pipe drain or reader-thread join. stdout/stderr use ordinary files rather than potentially full capture pipes; sizes are checked against the configured cap. Run deadline is50..20000ms, default4000; deliberate stall tests use1000ms. Cleanup waits at most2000ms and never clears the earlier failure state. The image is rehashed after stop.

The direct fixed self-test endpoint stops only the child withC0000417. It has no original context/debugger/filter/default-handler prelude. An adverse backend is deliberately ordinary-returning, without a false noreturn promise; after its return the child reports BackendFailure and stays stopped for the supervisor. Publication failure likewise cannot return to a successful event. Only the controlled infinite stopped-worker function and fixed endpoint are noreturn. No exception marker, exit/abort, host null-FILE printf or production callback is used.

## Test coverage and diagnostics

The11 cases cover fragmented valid protocol; stale nonce/wrong site/kind/version/checksum/truncation/conflict; required opt-in/image hash/scratch containment; normal return and bounded file output; actual fixed child termination/no helper-scope unwind; malformed/flood/same-code-without-boundary and boundary-with-normal-exit; half-frame and complete-boundary stalls; backend-return/publication failure; capture flood; unrelated inheritable parent event exclusion; and a forced failure before job assignment, requiring the suspended child to stop before worker code.

The sticky classifier is also checked against a terminal-looking status with Timeout/Protocol already selected. Self-test helper RAII markers distinguish ordinary helper-scope destruction from the direct endpoint; they do not measure ConversionSession destruction or accepted Close. The inherited-event test verifies the unrelated parent's event remains unsignaled after the child attempts its supplied numeric handle. All child handles are owned; no process-name killing or unrelated process management exists.

All fixtures/captures/diagnostics resolve beneath M2M_TEST_SCRATCH and this self image's checkout `build` ancestor. No C:\PROBLEM path is used by this harness; it creates no DEFORM project. Each run uses an exclusive new leaf. On all-pass, one bounded `terminal-harness-last-success.txt` aggregate remains in checkout scratch and the checked, exclusively owned case root is removed. Unexpected Testkit failure preserves the full case directories/receipts and reports that path. Individual receipts record case, executable/expected and pre/post hashes, status/disposition, stop reason, process ownership/cleanup, transcript flags, bytes, deadline and measured elapsed time. Aggregate copy is bounded8KiB per receipt/256KiB total; image hashing is bounded64MiB. These are local regular-file operations, not a hard real-time guarantee against OS/storage failure.

## Static preflight and release

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/mdt/test_terminal_harness_safety.cpp` returned ok with no issues. Direct checks found resolvable quoted includes, supported CHECK/CHECK_EQ/REQUIRE only, explicit Testkit main, balanced delimiters and no trailing whitespace. No C assert, REQUIRE_EQ, shell execution, converter/remesh dispatch or public ABI changes were introduced. SHA-256 uses the same BCryptHash/SHA256 pseudo-handle pattern already present in existing tests, with a source-local bcrypt library directive and no shared CMake edit.

This is static preflight only. No build, test, child process, GUI, original execution or REA action occurred. Root owns the first compile, focused runtime checks, repairs if needed, complete launcher and source-stable freeze. Successful source preflight does not establish Win32 job/pipe timing, dynamic no-unwind behavior or any native behavioral acceptance. The entire CP104 real remesh vertical slice and legacy wide/UTF8 compatibility cases remain pending; this safety slice does not replace them.

Frozen SHA-256 values (relative to `source/apps/moproj2mst`):

| File | SHA-256 |
| --- | --- |
| `tests/mdt/test_terminal_harness_safety.cpp` | `73bff8feaf3f46afa32433120ee7118d8dcaa0479842710f059b2e22ef371ca5` |
| `src/engine/terminal_harness_protocol.hpp` | `a8dbc859a471f1463029e2821a471f287a1148cf93faf6d9a63601c8745aa244` |
| `src/engine/terminal_harness_protocol.cpp` | `520c3b3e8339732b3746ae19c60ae76a6fcc01c4c2f11188ab18168b4c070913` |
| `src/engine/terminal_harness_supervisor.hpp` | `55d60064fb86593f6624801ed1ccb52233c8940a050de5a356d3942dcd7f17a9` |
| `src/engine/terminal_harness_supervisor.cpp` | `76cc361d81777ca7fae9d64c879798e9a33eba24a31cb6285e275d441bc68767` |
