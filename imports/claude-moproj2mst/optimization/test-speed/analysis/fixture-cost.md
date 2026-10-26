# Save-folder fixture cost audit

## What the retained evidence establishes

The latest full launcher run contains 98 Save-folder cases. The case-level JUnit time is 18.5023 seconds. The runner hashes four fixture roots containing 16,728 files and 1,867,669,827 bytes before and after CTest. One read-only warm-cache call to the original serial snapshot took 20.681 seconds. A separate complete launcher run took 58.598 seconds, with 18.563 seconds in CTest and 40.035 seconds outside it. That remainder includes both hashes, inventory, receipt/report work and other launcher overhead; it is not a hash-only measurement.

The fixture-hash engine report gives only a synthetic corpus benchmark, not a full-root speed result. It measured 385 files / 103,022,592 bytes with repeated overlapping roots. Four workers had a 0.357785-second median against the serial implementation's 0.563702 seconds in that synthetic setup. This does not establish benefit on the real 1.87 GB tree.

## Per-case size evidence and gaps

Read-only directory metadata gives 56 current A corpus directories (2,114 files / 311,285,240 bytes), 34 B input corpus directories (1,079 files / 66,781,512 bytes), and 403 directories in the retained oracle-B evidence tree (14,440 files / 1,464,352,698 bytes). The oracle tree includes `_before` and `_diff` evidence, so its totals are not equivalent to the sweep's working-copy input/output traffic.

Case manifest columns rank two large historical records at 838,639,889 and 838,142,852 bytes, each around 3,900 files. These project directories are absent from the current A corpus inventory. The same `copied_files` / `copied_bytes` totals appeared in both inspected manifests, so they cannot safely be treated as current per-case sweep inputs. Among extant named project trees the largest inspected cases are the Inc718 projects at about 25.7 MB / 41–45 files and the MO2 Extrusion/Boolean cases at about 12 MB / 130 files.

The retained test logs do not record the copied bytes, produced output bytes or copy/convert/compare phase duration for each case. The `folder-profile.md` specification describes those counters but also records that the instrumented checkpoint was not built or run. Therefore exact per-case I/O totals and output-size rankings remain unmeasured; diagnostic diff counts are not output sizes.

## Likely I/O pressure

The 98-case sweep is a single serial CTest program, so increasing CTest `-j` cannot shorten its critical path. Per case, the harness recursively copies a project, converts it, then enumerates and reads comparison files. Concurrent full-root hashing reads 1.87 GB twice; overlap with copy/compare can contend for disk, cache, and antivirus bandwidth. No retained timing isolates how much of the 18.5 seconds is copy versus conversion or comparison, so those are hypotheses for instrumented measurement rather than findings.

## Benchmark matrix

Keep the current serial sweep as the control. After a deterministic isolated-worker implementation is available, compare case concurrency at 1, 2, 4, 8 and 10 workers, and fixture-hash concurrency at 1, 2, 4 and 8 workers. Measure warm and cold cache separately, and compare the actual full-launcher sequence with non-overlapped phases. Use at least five paired repetitions per supported configuration, alternating order.

Capture full launcher wall time and separate inventory, before hash, CTest total, per-case copy/conversion/comparison/cleanup, after hash, and report phases. Preserve complete fixture manifests, the 98-case membership and order, raw and adjusted comparisons, skips/refusals, outputs, ABI tests and regression results. Report medians and spreads; do not accept a speed result that changes those outputs or silently omits full integrity checks.

The machine-readable measurements, metadata and qualifications are in [fixture-cost.json](fixture-cost.json). No evidence files were modified or hashed for this audit.
