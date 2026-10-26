# CP111 isolated Save supervisor

## Implementation

Added `runIsolatedSave` in `source/apps/moproj2mst/src/cli/isolated_save_supervisor.cpp`.
It validates the opt-in flags, code page and absolute project/output/deform/receipt paths; rejects receipt aliases with either selected file; binds an `IsolatedJob` to the caller CWD; and asks the existing terminal harness to launch the same executable as ProductIsolatedSave / RemeshV2 / ChildCase::ProductIsolated. The supervisor uses a canonical OS temporary root and a unique direct child leaf. Its preparation lease writes `isolated-save.job` only in `bindNonce`, after the harness creates the cryptographic nonce and before child creation, and stays alive through child stop.

Normal adapter codes 0–7 are returned only for a two-frame RemeshV2 `Started` + `Returned` transcript and a stopped child with OS status 0, with no sticky supervisor failure. A validated natural C0000417 Boundary is committed with `commitIsolatedTerminalReceipt` before this CLI process attempts `TerminateProcess` with that status. If relay returns unexpectedly, the process parks. Timeouts map to 66; other untrusted/failed outcomes map to 65. A terminal receipt commit failure does not relay.

Added `tests/g0/test_cli_isolated_supervisor.cpp`. It routes the explicit product worker token to `isolatedSaveWorkerMain`, verifies receipt/output alias rejection before launch, and verifies a disposable missing-project request returns `M2M_E_PARSE` after normal supervised completion. This is expected for `M2M_FLAG_WRITE_SAVE`: `convert.cpp` skips the legacy `checkReadable` preflight in this mode and the selected-path project reader rejects the missing file. The observed adapter result 3 is therefore a successful protocol `Returned` value, not a supervisor error. The test does not invoke the parent terminal relay, and makes no claim about native default-handler prelude or native Close behavior. The worker remains qualified as the existing static fresh-default candidate endpoint; this change does not implement native context/debugger/filter setup.

## Validation

No build or test was run, per task boundary. `git diff --check` passed for the new source and test. The report and both files were frozen after static review; root owns integration/build validation.

## Frozen SHA-256

- `source/apps/moproj2mst/src/cli/isolated_save_supervisor.cpp`: `4a0d762076b273b58c5ba898c460bbcade1f9a6c0a4ea7fc11770e76c88bf872`
- `source/apps/moproj2mst/tests/g0/test_cli_isolated_supervisor.cpp`: `aa4b32b8a6d60bf73a06db5ffa9c79fc23e0f98e41d1b14c8cf2b3eb63121f89`
