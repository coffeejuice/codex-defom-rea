# Save-current test comparison optimization

The recorded `profile-v1-ctest.log` times test 136 (`m2m.s.save_current`) at 4.89 s wall time for the process. Its per-case instrumentation reports comparison phases between 1.06 ms and 14.66 ms; the largest are COG7 (13.865 ms) and COG7P1 (14.662 ms). Loading/saving dominate most case totals, so the change here is intentionally narrow.

`runCase()` previously called `guiStep2(ob)` to find the oracle `.moproj` timestamp and classify step-2 files, then independently iterated the oracle directory to retrieve that same timestamp. `guiStep2` now optionally returns the timestamp it already read. The classification threshold remains the identical `.moproj` `last_write_time`, and all file membership and byte comparisons remain active. This removes one top-level directory enumeration and timestamp lookup per compared case.

Validation is static only in this turn: no build or test execution was performed. The existing profile is baseline evidence, not a before/after measurement; no speedup magnitude is claimed. Run the scoped save-current test with `M2M_S_TIMING` enabled when test execution is authorized to measure the effect and confirm coverage.
