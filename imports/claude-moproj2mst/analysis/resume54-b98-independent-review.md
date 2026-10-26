# Checkpoint 54: independent B98 raw-path review

## Finding

The raw-path qualification itself is strong: the test replaces one exact, counted candidate path string and then compares the entire resulting candidate byte vector to the historical MST. There is no broad slash/case/line-ending normalization or wildcard masking in that comparison. However, the current test violates the short disposable-project path constraint: at `source/apps/moproj2mst/tests/s/test_b98_kfread.cpp:49`, it roots the copied project at `m2m_test::scratchRoot()`, whose CMake default is `${CMAKE_BINARY_DIR}/t` (see `source/apps/moproj2mst/CMakeLists.txt:28-36`). It neither selects a short `C:\PROBLEM` runtime root nor checks the resulting project/key/output paths are below 100 characters. With the normal checkout build path, these generated paths can exceed the required length. This is a path-isolation/setup defect, not a reason to weaken the byte assertion. The test was not run here.

## Evidence and assertion audit

Read-only inspection of retained files independently confirms the historical `evidence/gui_mo/oracle_b/98_B_DS3F/Gear_blank.MST` is 2,756 bytes with SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`. The retained prior candidate output `build/b98-repro-checkpoint48/candidate.MST` is 2,786 bytes with SHA-256 `05e6dcfff2d3520b3f4495308a6ccbb65915eba5f33b66364ae3d26fd1e13159`. The old historical key path occurs once in the historical file, the old candidate scratch key path occurs once in the candidate, and substituting only the exact candidate path bytes with the historical path produces a 2,756-byte vector equal byte-for-byte to the historical MST (SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`). This verifies the prior retained pair; it is not a run of the newly added test or fresh original parity.

The test logic in `test_b98_kfread.cpp:43-45, 63-79` requires exactly one historical/candidate path occurrence, requires the candidate path to be the complete text line, and requires whole-file equality after only that path replacement. It checks the `KFREAD` marker count against the historical count at lines 66-67; the retained files have one each. Even if the candidate contains some other altered byte, path mapping leaves that byte untouched and the whole-file equality fails. The only masked bytes are the exact candidate path value at the asserted occurrence. The raw candidate string/vector is retained in memory unchanged; `pathQualified` is a separate comparison copy.

## Source and qualification

`source/apps/moproj2mst/src/ops/diestress.cpp:114-120` gets the active operation's subproject DB path. Lines 169-181 attribute the MST writer to original address `0x1402b5410`, replace `.DB` with `.KEY`, and emit the Diestress simulation's KFREAD record. Lines 107-112 attribute the `KFREAD` marker/path emission sequence to `0x1401344a0`. These source facts qualify the narrow candidate expectation. Historical B98 remains a retained oracle from the earlier `deform-rea-work` tree; neither the hashes nor the candidate-only test establish a new original-session observation.

## Integration note

For a compliant test setup, use a short owned disposable runtime root such as `C:\PROBLEM\b98r-<pid>-<tick>` (subject to the harness's existing configurable-root rules), and assert the copied `.moproj`, expected `.KEY`, and output `.MST` absolute paths are all shorter than 100 characters before conversion. Retain the exact counted-path replacement and complete raw-byte equality assertion. This review changed no code, fixture, build, GUI state, or REA data.
