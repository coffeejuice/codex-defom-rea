# CP105: Cogging.MDT write-open denial regression

Added a standalone candidate-only public Save case in `source/apps/moproj2mst/tests/s/test_save_cogging_open_failure.cpp`. It copies the `133_B_DBSTEP_BOOL3_before` oracle fixture into a unique `C:/PROBLEM/c105_<pid>_<tick>_<serial>` leaf and requires each generated absolute path to remain under 100 UTF-16 code units. It retains the existing Task00003 `Cogging.MDT` bytes and uses the existing private per-call project-output service. The project writer's about-to-close callback acquires a read-only handle with `FILE_SHARE_READ`; this is after conversion Open and denies later CRT write access to Cogging while leaving the already-read input usable. The handle is released before inspecting results, and the Rig destructor removes only its uniquely-created leaf after callback handles and conversion services are destroyed. Its parent must equal `C:/PROBLEM`, and every input/output fixture path is required to stay below 100 UTF-16 code units.

The fixture's Operation_List order has Boolean3D #2 before Cogging #3. Therefore Boolean BWZ/INI are asserted as earlier effects that remain; this test does not claim they demonstrate continuation after Cogging's failed open. The later main MST output is the continuation assertion after the property loop. Prior Cogging bytes must remain, project output must open/close and be queried once, and the candidate adapter must return `M2M_OK`. The `M2M_OK` assertion describes only candidate behavior; no original GUI status or fresh original runtime parity is claimed. Original static evidence in `analysis/save-semantics.md` records Cogging `fopen("wt")` failure returning 1 and the outer Save property loop ignoring the property result.

Static preflight: the test uses existing private per-call service interfaces and `tests/s/test_*.cpp` discovery, with no shared support or CMake edits. No build or test was run; root owns validation. No production, GUI, original-execution, or mutable REA action occurred.

Test source SHA-256: `17CF058FF65EC755C6932FF8374B1528ADEA8A199E2E3D4316D2A28883929CB2`.






