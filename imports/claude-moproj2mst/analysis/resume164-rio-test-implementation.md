# CP164 RIO test implementation receipt

Implemented the bounded candidate-only conjunction in `source/apps/moproj2mst/tests/s/test_save_remesh_failure_matrix.cpp`.

The new Testkit case copies the existing B133 fixture and combines (1) scripted project post-close error 7, (2) physical MST write-open denial established at `aboutToClose`, and (3) enabled remesh writer denial. It verifies the remesh write lock causes `ERROR_SHARING_VIOLATION` before conversion, checks the existing project-output-open/about-to-close/close/error-query sequence (the `project output open` event is emitted by `OutputFactory::open` during output Save, after conversion input Open and Save-current) and project/MST `SaveCallState`, retains the existing Boolean BWZ/INI effect assertions, checks MST and remesh sentinel bytes, and expects the unchanged public `M2M_E_UNSUPPORTED` guard message and returned code. The existing disabled-remesh test and terminal guard remain intact.

Preflight: target entry point exists (`main` calls `testkit::run_all`); all quoted includes are existing source-relative paths; Testkit defines `CHECK_EQ` and `REQUIRE`; no C `assert` is used. `git diff --check` passed. No build, test run, GUI, live REA, production source, CMake, or shared recovery state was touched.

## Hashes

- `source/apps/moproj2mst/tests/s/test_save_remesh_failure_matrix.cpp` (after wording correction): `3e76588bf5397f6fd9f448408601ca1bd66d0401856bc80aa52d393d8b4b823e`
- `analysis/resume164-rio-test-implementation.md` (receipt hash): `676f761734f316ac33d20ab7531a5558f948d6e3264fdbe58c37e7fc8d56f46d`
- `analysis/resume164-rio-next.md` (read-only input): `6250b6db12a70536d5f4fc3fa6d740ab34216d3d76b652cfa3ebbb3f7fb4fa97`

Implementation ownership is released to the integrating owner. The test remains unbuilt and unexecuted, as requested; integration should run the focused target and subsequent source-stable validation/freeze workflow.




