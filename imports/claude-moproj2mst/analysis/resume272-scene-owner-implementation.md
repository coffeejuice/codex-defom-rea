# CP272 canonical 2D Slice adaptation implementation receipt

Implemented the reviewed narrow gate in `source/apps/moproj2mst/src/engine/viewer.cpp::bind`, after partial unbind, object assignment, object-dimension binding and the pending-geometry return. Existing Slice adaptation is allowed to continue only when the actual owner equals the caller, has a present step, and has dimension exactly 2. Failed eligibility retains the existing unsupported error at that boundary. Eligible execution falls through to later bind work, matching native caller sequencing.

Updated `source/apps/moproj2mst/tests/mdt/test_viewer_slice.cpp` with actual-caller coverage: canonical 2D no-DB rebind succeeds repeatedly while collection pointer, view identity, constructor-only state, and runtime scratch remain unchanged. Canonical 3D no-DB rebind retains the existing refusal and preserves the same state. Checks use Testkit `REQUIRE`/`CHECK`; no C `assert` is used.

## Review correction and retest

The independent implementation review found that nonfatal pointer identity `CHECK`s could allow subsequent dereference after a collection replacement. Changed all three post-call collection identity and `viewIdentity` equality checks in both actual-caller tests to `REQUIRE`, so each stops before any collection dereference if identity changes.

Focused build: `python -B admin/build_candidate.py --label cp272-slice-scene-owner-require --target m2m_mdt_viewer_slice` succeeded (exit 0); receipt `analysis/cp272-slice-scene-owner-require-build.json`. Focused CTest `ctest --test-dir build/candidate -R "^m2m\\.mdt\\.viewer_slice$" --output-on-failure` passed 1/1. An earlier direct CMake invocation lacked MSVC standard include environment; the repository runner configured VS environment successfully.

No full suite/freeze, GUI interaction, or original execution was performed. Behavioral qualification remains retained static decompilation as scoped in the reviewed next-leaf design.

Final source SHA-256:
- `viewer.cpp`: 8C8E96BEB76BDDFBE74688C3AA605378C744542BB5EC45F8EB74F802F773AD25
- `test_viewer_slice.cpp`: BDBA0712693F0290187AD9DEF6AA52210917612053C1A44CC3930991A2A785F8
