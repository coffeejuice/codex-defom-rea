# CP62 checkpoint boundary: CP61 timing and source binding

## Timing finding

The reported `12.265 s` is the CP61 CTest phase, not the full test run. In `analysis/test-relocation-results.json`, CTest completed 198 registered tests with exit code 0 in 12.265 s. Its paired `build/test-runs/1792877344044145300/timing.json` records 26.031 s for the test launcher: 0.922 s inventory, 6.328 s fixture-before hashing, 12.265 s CTest, 6.422 s fixture-after hashing, and 0.078 s report parsing. Thus most of launcher time is the two fixture hash passes, not tests.

This is not a timing drop against the immediately preceding full suite: CP60 recorded 12.125 s for 196 tests, so CP61 is 0.140 s slower while adding two tests. CP59 was 12.563 s for 194 tests. The available receipts establish a small run-to-run difference, not a speed improvement. The CP61 build receipt separately reports 29.176 s total (10.703 s configure and 15.860 s build), source fingerprint `60fe2cb33fa7863409e04b811a4cba46e13d647befea3089511a31bee327cca3`, 3,246 source files before and after, and `source_stable_during_build: true`.

## Snapshot binding and race boundary

`admin/build_candidate.py` fingerprints the full `source/` tree immediately before configure/build and again after collecting artifact hashes. A change during that interval makes `source_stable_during_build` false. The CP61 build receipt’s matching before/after hashes therefore attest that this build ran while its source tree was stable.

`admin/run_converter_tests.py` records the build directory and the CTest command/results, but the test receipt does not include a source-tree hash or hashes of the test executables at test time. It runs the shared `build/candidate` directory. Consequently the test receipt alone does not prove which source snapshot or executable set it tested if another worker can edit source and launch another build against that same build directory during/around CTest.

`admin/freeze_checkpoint.py` adds useful post-run guards: it requires a successful stable build, recomputes the current source-tree fingerprint and requires it to equal the build receipt, then hashes `moproj2mst.exe`, `moproj2mst.dll`, and `deformdb.dll` against that receipt. It also checks test success, fixtures, and diagnostic continuity. If CP62 edits remain present at freeze time, the current-source/build-hash comparison rejects the freeze. This catches ordinary source drift after CP61’s build, including edits made before freezing, but it is not a complete concurrent-worker lock or time-of-test attestation: source edits followed by restoration to identical contents, or concurrent reuse/rebuild of `build/candidate`, are not proven absent by the current test receipt. The three executable checks bind the shipped binaries; they do not bind every test executable used by CTest.

No CP61 freeze is evidenced here. The build and test receipts are present, but the task’s timing and snapshot audit does not execute or infer a freeze.

## Safe checkpoint handling

For a valid CP61 freeze, stop CP62 writes to `source/` and prevent any build from reusing `build/candidate` until the freeze has either succeeded or failed. Then run the existing freeze workflow against the CP61 build and test receipts. A mismatch against the build fingerprint is a hard stop: preserve the receipts and treat CP61 as unfrozen; do not bless the later tree under the old CP61 label.

If CP62 work must continue concurrently, first preserve the CP61 source snapshot in an isolated checkout/snapshot and give it a dedicated build directory. Build and test that same immutable snapshot, then freeze from it. For future automated overlap, strengthen the test receipt to record the source fingerprint and build artifact manifest actually present when CTest starts and ends, and make freeze require those fingerprints/manifests to match the successful build receipt. At minimum, verify all CTest executables—not only the three production binaries—against the build artifact manifest at test time. These are recommendations only; no source, script, test, or build files were changed for this audit.

## Evidence consulted

- `analysis/resume61-integrated-full-build.json`
- `analysis/test-relocation-results.json`
- `build/test-runs/1792877344044145300/timing.json`
- `analysis/resume60-integrated-full-tests.json`
- `analysis/resume59-integrated-full-tests.json`
- `admin/build_candidate.py`, `admin/run_converter_tests.py`, `admin/freeze_checkpoint.py`
