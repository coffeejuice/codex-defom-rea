# CP86 mailbox open-retry implementation receipt

## Change

Updated only `source/apps/moproj2mst/tests/s/test_save_folder.cpp`. Both mailbox readers (`.ready` in the helper and `.done` in the coordinator) now use the same opener. It retries an unsuccessful `ifstream` open at most 10 times, sleeping 10 ms between attempts, then fails with `Cannot open folder IPC mailbox after 10 attempts: <path>`. A stream that opens is returned immediately; reads and all subsequent framing checks are never retried.

Ready/result parsing was factored into focused functions and retains the schema, exact job identity, ordinal, operation/terminal-state, tally, output-length, and EOF checks. The producer's close/check/rename publication is unchanged. The coordinator retains its existing refusal tally/output when the helper's result says `compare=0`.

Added a Release-active Testkit case in the same file. It covers two injected failed opens followed by a successful open of a valid ready frame; permanent denial with an exact bounded-attempt diagnostic; and opened-but-truncated ready and done frames. The latter both fail on the first open with `Truncated folder IPC`, demonstrating that malformed bytes are not retried. Its temporary files stay beneath the configured CTest scratch root and are removed with a scope guard.

## Static checks and hashes

- CP85 archived baseline file SHA-256: `38c6fb80a3bc30a9cf15b49233a88e83f33a8dd4c4b1da5570254dd33679ad83`
- Updated file SHA-256: `dd44cc93dce3ebabf12dbb9ec51ef6c54efda07b65377694c7a0861c1bd97ac0`
- Manual quoted-include resolution: 2 includes checked, 0 missing.
- Trailing-whitespace scan: none. `git diff --check` produced no diagnostics.
- `check_test_source_preflight.py` reports only `missing_int_main`; this file intentionally uses the Windows `wmain` Testkit entrypoint, present at its tail. It reports no unresolved quoted includes.

A post-review static correction replaced two stale `compare` references in the helper result writer with `parsedRequest.compare`; the worker source now uses the parsed request for both result framing and payload selection. The grouped preflight had found this compile blocker.\n\nNo build or test executable was run, per the assigned no-worker-build boundary. Functional validation remains pending root integration and a frozen-source build/run. The source and candidate executable were not otherwise changed by this task.

