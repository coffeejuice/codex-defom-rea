# CP81 controlled incremental build-graph experiment

## Purpose and isolation

The experiment used the immutable CP80 source archive (`build/wave3-checkpoint80-verified/source.zip`) extracted into a new scratch root. It did not alter `source/`, `build/candidate`, or the CP80 archive, and it did not run CTest or the original application. The separate build was `build/cp81-graph-experiment-vs/build`; the source tree was `build/cp81-graph-experiment-vs/source`.

The archive SHA-256 matched CP80 (`486a24bfd4ce58dc75694d98b8231bec5b09a04a80c2395c30644ac1f9aeced1`). Its extracted 3,355-file source fingerprint matched the frozen source identity before and after the experiment: `ec108f046e509be9dcf4181be5e1cd11303f3aa0b3a9b7906a5183dca3553c57`. The CMake cache SHA-256 remained `f9de6dd8dcc59fff8b9e09ebde55f43fc4eefc979ddd58823e6fe976bcd6d758` from configure through completion.

## Results

| Step | Wall time | Compile actions | Link actions | Result |
|---|---:|---:|---:|---|
| Configure | 3.105 s | 0 | 0 | Passed |
| Initial full build from empty scratch tree | 202.767 s | 1,074 | 267 | Passed |
| Full build with no source change | 0.140 s | 0 | 0 | Passed |
| Full build after touching one leaf test source's mtime | 3.043 s | 1 | 1 | Passed |

The initial build reported 1,344 compile/link actions plus three other numbered Ninja actions. It began from an empty generated build tree, so its 202.767 seconds are not comparable to CP79's 64.640-second post-preflight full-build phase, which ran against an already-populated candidate build tree.

The no-op full build left all 260 tracked `.exe`/`.dll` artifacts byte-identical to the initial full build. The subsequent touch changed only the modification time of `apps/moproj2mst/tests/mdt/test_viewer_box.cpp`; its content SHA-256 remained `e5e9e94da0009655d791f250ea17b389279176886d3990acb831af0928b867e0`, and the full source content fingerprint remained identical to CP80. Ninja then rebuilt exactly `test_viewer_box.cpp.obj` and linked `m2m_mdt_viewer_box.exe`. All 260 artifact paths remained present; only `bin/m2m_mdt_viewer_box.exe` changed SHA-256. The complete artifact maps before, after the no-op, and after the touch build are preserved in the scratch directory.

## Interpretation for CP79

CP79's full-build log showed 100 remaining object compilations and 213 link actions after its six-target preflight; the frozen CMake graph defines each normal test source as a separate executable linked to `m2m_core` and `testkit`. The preflight was therefore not a complete build: it prepared a narrow target subset, leaving the broad target graph to finish. The CP81 scratch experiment confirms the opposite boundary as well: once every target is current, a full build performs zero compile/link actions; one leaf-source timestamp invalidation causes only one object compile and one executable link.

These measurements validate an incremental path, not a direct reconstruction of the CP79 64.640-second phase. CP79's neighboring successful build receipts use different source content fingerprints, so those runs do not provide a matched source/cache control. No per-action durations were recorded in the CP79 build log, and this scratch experiment does not partition the 64.640 seconds among its 100 compiles and 213 links.

## Evidence and limits

Full build, no-op, and mtime-touch logs are `full-build-initial.log`, `full-build-noop.log`, and `full-build-after-mtime-touch.log`. The structured result is `receipt.json`; full output-artifact SHA maps are `artifacts-after-initial.json`, `artifacts-after-noop.json`, and `artifacts-after-touch-build.json` in the scratch directory.

A direct CMake configure attempt without the MSVC developer environment failed to find a C++ compiler. A second setup attempt used a build directory outside the project takeover's required `build/` child path and failed CMake's scratch-path guard. Both failures were confined to the scratch area. The successful run used the established `vcvars64.bat` environment and a fresh scratch root whose binary tree met the path guard.

The recorded `process_cpu_seconds` values are Python controller-process CPU time, not the child compiler/Ninja process CPU; they are not valid build CPU measurements and are intentionally excluded above. The experiment did not measure CPU utilization. No candidate source or build files were changed.
