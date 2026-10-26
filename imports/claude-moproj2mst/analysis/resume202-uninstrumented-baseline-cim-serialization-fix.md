# Resume 202 baseline CIM serialization fix

The retained one-attempt baseline at `resume202-uninstrumented-baseline-2026-10-26T04-59-10-181Z` observed PID 37824 exit naturally with code `0xC0000005`. The owned helper reported verified exit and closed handles. The runner recorded unchanged executable/source/disposable hashes, but its process-absence check failed to parse PowerShell's plain text `ok absent` response, so the overall attempt correctly remained incomplete. The coordinator independently verified the PID was absent with CIM/Get-Process and REA; this report does not substitute that external check for the runner's failed receipt.

The runner now wraps the PowerShell query result in `ConvertTo-Json -InputObject ... -Compress`, producing a strict JSON object for zero rows, a present process, or a query error. The zero-row case is accepted only as `{ok:true, absent:true}`; a present row must carry the target PID and complete identity fields and remains classified as present. Query errors and legacy plain-text output fail closed.

Verification performed without launching the original:

- `node --check imports/claude-moproj2mst/admin/original_uninstrumented_baseline.mjs` passed.
- `node .../original_uninstrumented_baseline.mjs --self-test-cim` passed four mocked cases: zero rows, present PID row, query error, and rejection of legacy text. The mocked envelopes are serialized by Windows PowerShell `ConvertTo-Json`.
- Direct runner invocation stopped at `RUN_UNINSTRUMENTED_BASELINE = false` as expected.

No new original-process run was made. The failed absence receipt from the retained run remains unchanged; the repaired parser is ready for a future authorized attempt only after the helper ownership gate is clear.
