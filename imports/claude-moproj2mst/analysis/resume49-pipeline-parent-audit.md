# Checkpoint 49: independent parent-helper failure audit

The observed regular-file oracle test exercises the intended parent failure path. No behavioral discrepancy was found between the retained artifacts, the checkpoint48 contract and the current minimal pipeline source. It provides parent runtime failure-propagation evidence beyond the standalone worker suite. Exact helper exit-code and kernel-handle cleanup remain limited claims because this attempt did not independently record the helper PID/exit event.

This audit only read existing source and artifacts and wrote this report. It did not run a process, inject a fault, build, alter a fixture/source or touch REA/GUI state. Coordinated the artifact selection and findings with `test_speed_migration`.

## Evidence identity

Run directory: `build/pipeline-parent48-onecase-96c6cf22099348ce8a3b1260558a0906`.

- `process-receipt.json` SHA-256: `f686f1da66f99aa6af193dd579ab134249dd7023824ce4f99b5cd5100c896a83`.
- `validation.json` SHA-256: `e651671ba65c3bceabb9d9957531c085f8b4bfd37797214bc1c08b403e4e10a5`.
- Recorded test executable: `build/candidate/bin/m2m_s_save_folder.exe`, SHA-256 `55bd1352512665eb7e841fc85385b4d15ae4fb022e6e9b956e4e841a14f37722`.
- Current unchanged test source: `source/apps/moproj2mst/tests/s/test_save_folder.cpp`, SHA-256 `38c6fb80a3bc30a9cf15b49233a88e83f33a8dd4c4b1da5570254dd33679ad83`.
- Earlier direct-worker evidence: `build/pipeline-faults-48a/receipt.json`, 11/11 accepted, using the same recorded executable/source hashes. That suite's W9 establishes the regular-file expected-path failure inside the worker; its W10 covers successful worker comparison and leaf cleanup.

## Independent checks

Recomputed and verified all 37 file hashes in the parent run's post-run manifests and all three directory manifests. The copied input manifest equals the pre-run copy manifest and the recorded source-fixture manifest. The regular oracle-file manifest is unchanged. The corpus contains exactly full `11_THEXP` and empty `19_XY_H01`/`52_BEND_2D` placeholders, so the fixed-resave discovery precondition was satisfied without queuing extra cases.

Parsed `job.bin` independently, with exact length framing and EOF: minimal job schema, count 1, ordinal 1, name `B:R11`, expected path equal to the owned regular `oracle/R11` file, and work path equal to the retained `scratch/m2m_folder_sweep_26128/1` directory. Parsed `1.ready` independently: minimal ready schema, ordinal 1, compare operation 1, exact EOF. There are no done results.

Verified launcher stream lengths and hashes against the process receipt and validation receipt. Outer stdout is exactly:

```
[FAIL] save_folder_sweep
1 of 1 cases failed
```

The actual file contains Windows CRLF. Outer stderr contains `Folder pipeline helper exited before result` and the owned IPC path. Worker stderr contains the recursive-directory-iterator invalid-directory exception for the regular oracle path. The helper case.stdout and both coordinator captures are empty. The converted work leaf remains. These observations match the intended stage of failure rather than merely accepting any nonzero exit.

The recorded parent PID is 26128, exit code 1, elapsed time 88 ms, with no timeout and no driver timeout cleanup. The elapsed value describes this attempt only; it is not a benchmark or speed claim.

## Why this follows the code

`runCase` checks expected-path existence during discovery (274–277), so a regular file is accepted as an oracle. The parent converts normally and publishes compare=1 only after `M2M_OK` (305–317); the retained ready message therefore establishes that this attempt reached successful conversion rather than copy skip/refusal.

The helper invokes `compareCase` (484). `files(expected)` fails during recursive enumeration before diagnostic rows, work removal or done-result publication (209–265). Its catch writes worker.stderr and returns 2 (507–510). The parent has exactly one pending case, drains it, and waits on that case's missing result (683–722). `waitMailbox` sees its exact child handle signaled and throws the observed helper-exited-before-result exception (433–443).

Exception unwinding restores stdout/stderr through the capture destructors; testkit then prints the failure row/diagnostic to the outer launcher's streams and returns 1. The observed outer streams establish usable restoration for this path. IPC/work retention is expected because normal replay and cleanup were not reached. The source wrapper waits/closes the child handle during unwinding; its termination branch is conditional on a still-running process and is not needed for the already-signaled helper in this test.

## Exact discrepancy and remaining limits

No mismatch was observed in the target failure seam. The earlier proposed outcome is now supported at runtime for this built executable: one normal parent conversion, one helper comparator failure, a parent test failure, observable stream restoration and retained diagnostic scratch.

The statement “helper exited 2” is a source-supported inference corroborated by its worker diagnostic and the standalone W9 run, not a separately captured exit code for this specific parent attempt. Likewise, this attempt does not independently observe the helper PID, prove every CloseHandle call, or test the wrapper's terminate-if-live branch. Those should not be reported as measured results. Parent-reader truncation, malformed done identity/tally, forced launch failure, capture-restoration failure and timeout behavior remain outside this fixture. No additional experiment is required to accept this bounded helper-failure propagation test, and it does not establish full parent-fault coverage or converter parity.
