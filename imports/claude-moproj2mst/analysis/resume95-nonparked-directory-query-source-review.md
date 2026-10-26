# CP95 independent review: nonparked directory query borrow

**Result: no source blocker found.** The ordinary high-record query is wrapped in a synchronous void `withStep` callback after all owner, receipt, bounds, and record-version guards.

## Findings

At `viewer.cpp` lines 617–656, the record-version `< 6000` branch prepares and installs the frame, makes its query, records post-version/stage, and returns. That branch is unchanged and does not invoke the new callback after installation. The selected ordinary path initializes the same four output locals and then invokes `step_dir_sizes` exactly once through a void callback. Its seven arguments preserve the identities and order: `nativeStep`, captured `reader`, captured `index`, and addresses of `hsize`, `marSize`, `arraySize`, and `version`. The native `uint64` return remains explicitly ignored. The callback ends before the unchanged `HighVersionDbLoaderPending` return.

The strong `dbOwner` lease remains in scope across the call; its reader was checked against the model DB and actual step DB before receipt validation. The preceding receipt-current, signed index, physical allocation extent, and actual record-version checks remain before the call. The callback captures no step pointer beyond its parameter and neither stores nor returns it. `withStep` performs the pending-barrier preflight before borrowing, so an already parked model cannot enter this nonparked query path.

The existing `geometry_nonnull_directory_query_keeps_high_versions_pending_with_actual_owner` regression exercises the actual-record version thresholds and the conditional step-version write while requiring a pending result; the fixture matrix includes indices and null/non-null connectivity. Existing owner, receipt, bounds, reload, and parked-path cases cover the preceding guards and excluded low-record path. No duplicate test or helper/API change is indicated.

## Conclusion

No change to native argument/output identity, owner lease, guard ordering, ignored return, pending outcome, or low-record frame sequencing was found. Current `viewer.cpp` matches the CP95 final SHA-256 `7274e7772e6dc0c6dbf02a9a0811ee77e65bf9b683e474831cab54f3f9797f75`. Root owns build and test acceptance; I did not run them.
