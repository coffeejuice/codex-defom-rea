# CP110–112 efficiency review

Across the three bound checkpoints, full CTest time ranges from 12.203 to 13.062 seconds and the complete launcher from 29.704 to 31.375 seconds. No measured 10× improvement appears. The analyzer reports worker overlap as unavailable for all three: existing receipts do not include worker event intervals or dependency edges.

| Checkpoint | Successful build timing relevant to bound test | Configure | Focused preflight | Full build step | Build receipt elapsed | CTest | Launcher |
|---|---|---:|---:|---:|---:|---:|---:|
| CP110 | Repair 1, same source as tests | 2.078 s | 3.203 s | 0.813 s | 11.929 s | 12.203 s | 30.859 s |
| CP111 | Repair 1 full build; repair 4 is the bound incremental build | 2.156 s | 2.891 s | 0.797 s (repair 4) | 11.779 s | 12.969 s | 31.375 s |
| CP112 | Grouped full build | 2.125 s | 2.000 s | 22.234 s | 32.371 s | 13.062 s | 29.704 s |

Successful full-build context: CP110’s original grouped build took 32.616 s (2.172 configure, 2.000 preflight, 22.515 full build). CP111’s first grouped attempt failed in the full-build step after 2.797 s (exit 2); repair 1 completed a full build in 35.589 s (24.500 full-build step), followed by successful incremental repairs 2–4 (13.919, 13.298, and 11.779 s). CP112’s grouped build completed in 32.371 s. Do not compare CP111’s 0.797-second incremental build step with a full build as if it were a configuration speedup.

Suite counts grew from 293 to 298 to 300 registered tests; clean passes grew from 286 to 291 to 293. Each checkpoint retains six logged-gap tests, one logged-difference test, and zero failed, skipped, or incomplete-output tests. The validation records report no changed diagnostics and `objective_complete: false`. These are separate source snapshots, not repeated performance trials of an identical executable; the build and test receipts bind matching frozen source hashes within each checkpoint. Token usage is unavailable.

All three launcher receipts use the same fixture hashing settings: four threaded workers, SHA-256, parent-cache path resolution, caching disabled. Each before/after manifest covers 16,728 files and 1,867,669,827 bytes. Measured before/after fixture hashing was 6.500/7.125 s at CP110, 6.687/6.968 s at CP111, and 6.250/5.641 s at CP112. Source/binary binding took 1.813/2.250 s, 1.844/1.844 s, and 1.891/1.844 s respectively. The source tree remained stable at 3,429, 3,439, and 3,445 files; the respective test runs bound 290, 295, and 297 binaries. Fixture changes were empty.

## Telemetry result

The new receipt analyzer was run against each matched build/test/timing set and wrote machine summaries to `resume112-cp110-critical-path-telemetry.json`, `resume112-cp111-critical-path-telemetry.json`, and `resume112-cp112-critical-path-telemetry.json`. All three return `worker_overlap.status: unavailable` because no same-clock agent/worker event receipt exists. Thus current data cannot establish whether parallel agent execution shortened a critical path. Build and launcher phase timings alone do not reveal worker overlap, wait time, or task dependencies.

The safe next measurement is for the coordinator to emit the analyzer’s checkpoint- and source-hash-bound event schema during one future run, including worker start/end times and dependency IDs on a single monotonic clock. Retain the exact fixture manifests, full test inventory, source/binary binding, and acceptance gates. Until those events exist, make no agent-orchestration causal claim and no configuration change based on these receipts.
