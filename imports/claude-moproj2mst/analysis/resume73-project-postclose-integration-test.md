# CP73: paired project post-close status integration test

## Test added

Added `save_project_reported_postclose_error_continues_properties_mst_and_remesh` to `tests/s/test_save_io.cpp`. It runs two fresh default Boolean `Rig` copies from `133_B_DBSTEP_BOOL3_before`, with identical remesh fallback input and fresh sidefile/MST paths:

- control mode delegates the post-close status to the real `mst::FileDevice::error()`;
- injected mode returns the explicitly declared status `7` after the same real FileDevice open, serialization, close callback and close.

The test-only private factory/device forwards all real `FileDevice` writes, flushes, open state and close. It forwards the lower device's `aboutToClose` event to the wrapped `ProjectOutputDevice` while the lower handle is still open, allowing the real stream callback to flush. Factory-owned observation survives device destruction and records the normalized project path, actual open, nonempty serialization writes, callback/open state, close return, actual error, one post-close error query, and whether BWZ/INI/MST/remesh outputs were absent at that query. The injected value is a scripted post-close status observation, not a physical failure.

## Assertions

The paired case checks exactly one factory call per conversion, trace order `factory/open → aboutToClose → close returned → error query`, one error query after close, and real device error zero in both runs. At that query, step-4 Boolean outputs, MST, and remesh output do not yet exist.

After each conversion it checks the Boolean BWZ and INI signatures, MST completion marker, and remesh artifact generated from the fallback input with method 4 and filter distance 7. It compares the BWZ, INI, MST, and remesh bytes between control and injected runs. It checks `SaveCallState` for project attempted/returned and control success versus injected false, plus successful MST attempted/returned/result in both runs. The control returns `M2M_OK`; the injected case returns `M2M_E_WRITE` with a project-write diagnostic after later outputs, and `ConversionDetails.returnedCode` agrees.

The project output bytes are intentionally not compared or assigned an expected prefix/empty result. The real delegated FileDevice may write the full project while the test service reports error 7; this case validates orchestration and status mapping only.

## Static preflight and boundary

`git diff --check` passed for the owned test file. Both quoted private headers resolve relative to the test path. No raw C `assert` was introduced. The test uses a per-call borrowed factory, and it does not modify public headers, C ABI, CMake, production source, or default writer behavior. The root owns build and test validation; no build or CTest was run here. No original runtime or mutable REA was used.

## Freeze hash

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/tests/s/test_save_io.cpp` | `c6a2f5c36c928c151cce9575c144f0588e337b64d00291b361b03289d5a75997` |
