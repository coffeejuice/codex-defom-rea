# CP80 fixture SHA read-chunk benchmark

## Purpose

Measure a 4 MiB `read()` buffer against the current 1 MiB buffer in a separate Python process over the exact four CP78 fixture roots. The experiment changes no fixture file, test script, candidate build, or source file.

## Method and integrity checks

The run used the CP78 inventory's exact four fixture roots, SHA-256, four worker threads, the same `parent-cache` path enumeration, and no persistent content cache. A benchmark-only in-memory replacement for the helper's per-file reader changed only the chunk size. The call still hashed every byte and retained the per-file size and nanosecond modification time. Order was 1 MiB, 4 MiB, 4 MiB, 1 MiB, 1 MiB, 4 MiB. Neither OS cache nor Python cache was cleared.

Every pass produced a full manifest for 16,728 entries / 1,867,669,827 bytes. All six ordered manifest digests were identical: `5266f5101fb6a1b44793f84f8232678ee102b4284c45c70b6d04b7ade206c4d9`. The manifest included the relative path keys, path order, file byte sizes, `mtime_ns` values and file SHA-256 hashes. The first 1 MiB manifest also compared equal, including key order, to both the CP78 `fixtures-before.json` and `fixtures-after.json` snapshots.

## Results

| Chunk size | Trial wall (s) | Median wall (s) | Trial process CPU (s) | Median process CPU (s) |
|---|---|---:|---|---:|
| 1 MiB | 8.670, 6.757, 6.662 | 6.757 | 17.453, 13.766, 13.219 | 13.766 |
| 4 MiB | 15.209, 14.126, 10.175 | 14.126 | 34.734, 34.297, 23.031 | 34.297 |

In this run, the 4 MiB median wall time was 2.09x the 1 MiB median; its process CPU median was 2.49x. This did not improve fixture hashing and is grounds to reject the larger-chunk candidate. No full launcher was run.

## Limits and raw evidence

This is a small ordered experiment with uncontrolled filesystem-cache state and visible timing variation; it does not establish that larger reads are intrinsically slower on every machine. The repeated 4 MiB runs were all slower than the 1 MiB median, including the final warm pass, so there is no evidence to justify integrating the change. Keep the current 1 MiB implementation. Further tuning should be separately proposed and measured before adoption.

The complete six manifests and receipt are preserved in `C:\Users\alext\AppData\Local\Temp\cp80-fixture-hash-wuv811_9`. The structured raw receipt is `receipt.json`; each `trial-*-manifest.json` contains the full ordered map. The experiment did not access or modify the candidate build, test scripts, or fixture contents.
