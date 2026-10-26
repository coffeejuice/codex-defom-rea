# Resume 48: deterministic parent-helper failure experiment

## Result

The one-case parent/helper failure fixture passed all contract assertions using the existing checkpoint 47 executable. This demonstrates the parent’s helper-failure propagation and observable stdout/stderr restoration for this specific comparator exception. It does not test a still-live child being terminated by the parent, truncated parent-side result parsing, broad helper fault coverage, or converter parity.

## Pinned inputs and invocation

- Executable: `build/candidate/bin/m2m_s_save_folder.exe`
- Executable SHA-256 before and after: `55bd1352512665eb7e841fc85385b4d15ae4fb022e6e9b956e4e841a14f37722`
- Parent PID: `26128`; exit code: `1`; elapsed: `88 ms`; timeout: false; timeout cleanup: not used.
- Invocation: executable with no arguments, hidden/no shell, 20-second wait bound. `M2M_FOLDER_SWEEP=1`, `M2M_FOLDER_PIPELINE=1`; copied corpus, oracle, `M2M_TEST_SCRATCH`, `TEMP`, `TMP`, and `TMPDIR` were rooted in the unique run directory. The focused CTest reference/library variables (`DEF_PORT_REPO`, `M2M_REFERENCE_EXE`, `M2M_CLI`, `M2M_DB_FIXTURE`) were retained/set to the checkout’s existing paths. Prefetch, worker sharding, raw-receipt and targeted-capture variables were unset. No original application or REA process was launched.
- The accepted source fixture `evidence/gui_mo/s1/corpus/11_THEXP` was copied unchanged: all 15 files / 521,873 bytes matched source hashes before and after. The only oracle entry was an owned regular file `oracle/R11`, SHA-256 `055aa4af8b9d58488a805b418f4dba82fcd939c37037855106f1d0efb49d45f6`.

## Observed result

The retained job manifest parses exactly as `M2M_FOLDER_POST_JOB_MINIMAL_1`, one case, ordinal 1, name `B:R11`, expected path equal to the owned regular `oracle/R11`, and work path equal to the owned `m2m_folder_sweep_26128/1` directory. The helper received `1.ready` with compare=1. Conversion completed and its work leaf remains available.

The helper wrote a filesystem diagnostic from `recursive_directory_iterator` while trying to enumerate the regular expected file, then exited without publishing `1.done`. The parent observed its exact helper process handle signaled, reported `Folder pipeline helper exited before result`, restored the captured outer streams, emitted `[FAIL] save_folder_sweep` and `1 of 1 cases failed`, and exited 1. No successful section summary, `SAME`/`DIFF` comparison row, or `1.done` was present. The case stdout and coordinator stdout/stderr capture files are empty. The worker PID/exit code was not separately recorded; parent-observed process signaling and source-level wait/close are evidence, not an independent child exit-code receipt.

Validation assertions passed: executable pinning, no timeout, expected parent exit and outer streams, exact single-case manifest/request, expected regular-file location, absent result/success summary, matching worker failure, unchanged input copy/oracle, and retained converted case data. No raw output was normalized.

## Raw evidence

All artifacts are retained under:

`build/pipeline-parent48-onecase-96c6cf22099348ce8a3b1260558a0906/`

- `launcher.stdout.bin`: 47 bytes, SHA-256 `98fd0706b22e62a77598a909f4b2eedb0bcd095ff1ecda5e734bc847f53a8633`.
- `launcher.stderr.bin`: 260 bytes, SHA-256 `3bcde88838eca47912a9a0e9efcf64925b7c27d14e48d653a55855b86226ba09`.
- `scratch/m2m_folder_pipeline_26128/job.bin`: 388 bytes, SHA-256 `eb1f4ac3fb2b67eb576a682c77a4acfd2857f02696b8e58e1809baa9b5496358`.
- `scratch/m2m_folder_pipeline_26128/1.ready`: 55 bytes, SHA-256 `5bd2522385b5af7fdc6e751a9594ca37ee62600ebcc06b562b6ca26cf716c9d7`.
- `scratch/m2m_folder_pipeline_26128/worker.stderr`: 281 bytes, SHA-256 `734a3566cf66ec5930081c5a373b1b1b9fb0d4ede6f66a8960386892da16be13`.
- `case.stdout`, `coordinator.stdout`, and `coordinator.stderr`: empty files, each SHA-256 `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855`.
- `input-manifest-before.json`: SHA-256 `185f96808e648dc8b319aec4c66092e09803c36d22498b4f1a2c9353d3422e1c`.
- `output-manifest-after.json`: SHA-256 `0978090a5d912fd03a7579cf654f534ec0b0414c9f508272280752423919558d`.
- `process-receipt.json`: SHA-256 `f686f1da66f99aa6af193dd579ab134249dd7023824ce4f99b5cd5100c896a83`.
- `validation.json`: SHA-256 `e651671ba65c3bceabb9d9957531c085f8b4bfd37797214bc1c08b403e4e10a5`.

A first fixture-preparation attempt stopped before process launch because a PowerShell literal wildcard was used for copying. Its empty/partial, unlaunched scratch is preserved separately at `build/pipeline-parent48-onecase-8fd76eb660244d96a16e60ac04f49071/`; it is not part of the measured run or result. The corrected attempt used explicit source-child enumeration and a fresh run directory. No injection race, source change, or post-run cleanup was performed.
