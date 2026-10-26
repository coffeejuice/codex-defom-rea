# CP85 folder-helper IPC incident audit

## Purpose

Explain the first CP85 `m2m.s.save_folder` failure at helper iteration 38, compare it with the exact retry, and give a bounded qualification for the checkpoint record. This is a read-only log/source audit; no source, build, or candidate files were changed.

## Run-to-run evidence

| Run ID | Result | Tests | Folder test | Source/binary binding | Fixture change |
|---|---|---:|---|---|---|
| `1792894740181267300` | `failed-or-incomplete`; CTest/launcher exit 8 | 248 registered; 1 failed | Failed after 5.969 s; helper stopped before result | Source `48d7798c…`, 3,367 files, stable; 245 binary entries identical before/after | None |
| `1792894832534113100` | `completed-with-recorded-gaps`; exit 0 | 248 registered; 0 failed/skipped/incomplete | Passed after 13.2362 s, with the diagnostic differences recorded | Same source hash/count; same 245 binary entries, identical before/after | None |

Both runs used the same threaded four-worker, parent-cache SHA-256 mode. The retry preserved the suite inventory and completed the full Save-folder sweep. Its retained summary is 49 corpus cases (48 identical, 1 with file differences) and 49 oracle-B cases (45 identical, 3 with declared view profiles, 29 file differences); the first run failed before any complete pair of summary tallies was available.

## Bounded IPC inspection of the failed attempt

The first run's helper directory is `build/candidate/t/ab244c6e/m2m_folder_pipeline_35168`. It contains a complete `job.bin` of 27,072 bytes: schema `M2M_FOLDER_POST_JOB_MINIMAL_1`, 98 case records, exact EOF. Every retained `.done` file for ordinals 1–37 parses completely; each echoes the exact job bytes and its matching ordinal. The last result is ordinal 37 (`A:RS_41_NewProject_1`).

The directory has no `.done` for ordinal 38. It does have complete 55-byte `.ready` frames for ordinals 38 and 39. Both parse as `M2M_FOLDER_POST_READY_MINIMAL_1`, operation `compare=1`, with no trailing bytes. Ordinal 38 names `A:RS_42_vdie`; ordinal 39 names `A:RS_43_vdie2`. The helper diagnostic is exactly `Folder post helper infrastructure failure: Truncated folder IPC`. Coordinator stdout/stderr captures are empty; its case capture ends with the valid ordinal-37 `SAME` line. The CTest output reports `Folder pipeline helper exited before result` and the output parser correctly marks the folder output incomplete.

The failure therefore occurred after the helper had parsed the complete job and returned complete results through case 37, but before it published result 38. The persisted `.ready` 38 frame is valid now. It cannot establish the exact bytes or stream state the helper saw at the instant of failure. A successful retry's temporary IPC directory is removed by the normal success path, so no second raw wire capture remains for a frame-by-frame comparison.

## Source-supported interpretation

In the helper source, `getNumber()` and `getString()` throw `Truncated folder IPC` when a stream read fails. The worker constructs an `ifstream` for each `.ready` file and immediately parses it without first distinguishing an open failure from a short read. The worker catches either exception, writes the generic diagnostic, and returns status 2. The parent publishes each ready frame to a temporary file, closes/checks it, and renames it to the final name; the mailbox path is unique to the current test process. These facts make a deterministic partial-publication race less likely, but they do not rule out a transient open/read problem or a timing-dependent file interaction.

The evidence does **not** prove an infrastructure-only transient. The error text collapses open/read/truncation into one message, the failed run did not retain a per-frame stream error or worker exit-code receipt, and the retry discarded its IPC files. The best-supported classification is a single helper-side IPC read failure of unknown cause. It is consistent with a transient filesystem/open/read event; it is not enough to exonerate a timing-sensitive code race.

## Freeze qualification

Preserve both run receipts and the failed IPC directory. Record the CP85 full-suite result as: first attempt incomplete due helper IPC failure at iteration 38; exact retry on the same stable source and 245 unchanged binaries passed all 248 registered tests with the documented folder differences; fixture snapshots were unchanged in both attempts. The retry is valid evidence that the case is repeatable on the same candidate, but it does not retroactively change the first run's failed status or prove the flake is infrastructure-only. Any checkpoint gate that requires one uninterrupted passing run should use the retry receipt and keep the failed run as an explicit qualification, rather than silently replacing or deleting it.

No source fix is proposed in this audit. If the incident recurs, preserve the helper directory and add narrowly scoped diagnostics for the failing ready path and stream state before deciding whether to retry or patch; do not weaken the IPC frame checks.
