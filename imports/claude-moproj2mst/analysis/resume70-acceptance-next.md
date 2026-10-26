# CP70 acceptance next: native observation lifetime across guarded Save and teardown

The highest-priority headless acceptance slice selected from checkpoint69 is native Type2 journal lifetime across the actual guarded Save and destruction of its actual candidate Session/I/O. Existing recording tests retain journals through guards or recording teardown; the frozen native test checked physical XML/main-file effects but did not retain a journal or weak I/O owner across Session destruction. This can be isolated without a fresh GUI and without widening public behavior.

## Evidence inspected

Checkpoint69 frozen receipt: build/wave3-checkpoint69-verified/receipt.json. Source tree is 0bbbc04f787a59d07e9c8527641a29ba29816fde4674599123d42fd881f4e6a6, 3292 files; source ZIP byte hash e05f469f07894a34c3bd30f4fc9f39b95832683accb6c6d8727f7d4001386c64 was independently checked. The selected build/source/test boundaries agree and execution_binding is stable. Bound full run is build/test-runs/1792883234316051000, 214 registered, 207 clean passes plus six gap suites/one diagnostic suite, 93 SAME/5 DIFF and incomplete objective.

Inspected frozen tests show:

- test_type2_native_open_guard.cpp (SHA-256 ee7077f202121e03cc71c7576b72f2dfddb88cc2aa604275596c0f75e8db3a73) contains no journal/weak-owner lifetime assertions.
- test_type2_xml_prefix.cpp's escaped-journal lifetime case uses its default recording Rig. Its native component case only checks ordinary file output and absence of fabricated recording-byte snapshots.
- test_type2_save_node_prefix.cpp's caller-owned SaveEffects cases are recording-sink guard tests.
- NormalWriterReturn and RefreshBoundaryReached observations are available; they represent returned XML outcome and reached pre-dispatch boundary, not actual timer/UI delivery.

## Exact executable proposal and expected proof

Extend the existing tests/doe/test_type2_native_open_guard.cpp with one new case named type2_native_journal_survives_refresh_guard_and_actual_session_teardown. Keep its existing native test. No new CMake target is required: executable m2m_doe_type2_native_open_guard, CTest m2m.doe.type2_native_open_guard.

Use a fresh disposable copy of 120_B_DOE, pin its project/XML hashes, load the empty-variable/zero-constraint Study, then remove the copied InputTable XML before Save. Retain only a const journal owner, caller FileEffects and weak StudyIo outside an inner scope that owns the real ConversionSession/Project/NativeStudyIo. Execute native Save, assert the current exact refresh-owner guard, inspect the returned XML/refresh observations, then leave the scope and require weak I/O expiration while journal values and physical partial artifacts remain.

Required observations are one actual native attempt, observed open/close/error0, normal writer success without stream failure, reached refresh boundary, no recording-memory byte fields, same journal identity, and no appended/deleted observations at Session teardown. Independently read the regenerated 249-byte physical XML after teardown and compare its bytes/hash to retained golden 1777bb9948fb2d6f55411f9dcfdc4a7c9196230086c0f86720255d03e76571e0; require the main .moproj remains 0 bytes and retained source manifest unchanged.

The root subsequently authorized this test implementation after the original source owner released the file. It is now authored and statically checked, not compiled/run. Current test-file SHA-256 is 625bcdfbe5f5c2efbd970b5694dd6ba3ff1addf074d3e2d9222c0830bf79f210. It uses the separately released CP70 internal native observation sink; this lane changed no production API. See analysis/resume70-native-type2-owner-lifetime-test.md for the implementation handoff.

Root-owned commands after all relevant source owners are frozen:

```powershell
python admin/build_candidate.py --label resume70-native-type2-lifetime-preflight --target m2m_doe_type2_native_open_guard
ctest --test-dir build/candidate --output-on-failure -R '^m2m\.doe\.type2_native_open_guard$'
```

Expect two active Testkit cases in that single CTest entry. Do not run the old frozen checkpoint69 executable and interpret its one-case result as this test. Retain the new source/binary hashes and actual case summary; then complete the normal exact full-suite/fixture-hash/archive gate. The current configured scratch policy is inherited; short-path conformity must be resolved/verified by root, not assumed from these source checks.

## Qualification

This is an actual candidate native-writer/Session lifetime test, not original Windows/Qt failure parity. It does not expose a C ABI observation handle, prove public conversion diagnostics survive return, release the refresh guard, dispatch native timers/UI, complete Type2 Save or establish every Session/global cleanup effect. A weak StudyIo expiry proves this concrete owner is not retained by the escaped journal; it does not prove all OS handles or unrelated native owners are gone. Physical effects remain independently read, never inferred from stream counts.

If the future runtime assertion fails, preserve the evidence and trace ownership/lifetime before changing expectations; do not weaken the guard or invent cleanup. No GUI/original process or build/test was executed by this worker, and no passing acceptance is claimed yet.
