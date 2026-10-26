# CP142 independent ABI sequence review

## Result

No blocking correctness defect found in the frozen test source. It checks the public exported wide-path ABI in one process, requires exact `M2M_OK` for the mode-0 Save and exact `M2M_E_PARSE` for the subsequent missing-project call, and checks the documented parse-failure message, CWD stability, absent second MST, unchanged remesh input, and byte-identical first-call output snapshot.

The scratch layout is isolated: `scratchOutput()` validates the unique leaf against the configured scratch root before creation; the test refuses an existing leaf; the retained input is copied into `first`; and `second` is separately created for the failure call. The retained oracle is read-only. The `M2M_MO_INPUT_MODE` value is pinned to `0` and restored by RAII, and the code page/unit setters are checked before conversion.

## Comparator and receipt qualification

The candidate output tree is compared with the retained `RS_08_HEAT1` tree using the same exclusions and the same path relocation, banner, and `.moproj` `Expanded` treatment found in `tests/s/test_save_folder.cpp` (`files`, `relocated`, `bannerOnly`, and `maskView`'s identical regex). Raw unequal relative paths are recorded before those allowances; the normalized result must have no differing, missing, or extra comparable files. `saveSnapshot()` then keeps the first output's comparable bytes in memory and verifies that the later failure call did not change them.

The bound artifacts match the plan: the test SHA-256 is `ACB1FF092EFC3A529CBCC6C7004E8AABD5AFB1BD56119B2DB6E7DFDB64B0E21C`; input `HEAT1.moproj` is `E758AA107B6D8C849B1417F9F12177F70B282A4ECD6FA96D1231F6F748E30093`; retained output `HEAT1.MST` is `4245304B4CB3941B1A8FF10D21A714D1290A696ADF2834F0626FF5AE5896EEC7`. The comparator source hash is `DD44CC93DCE3EBABF12DBB9EC51EF6C54EFDA07B65377694C7A0861C1BD97AC0`.

The `.moproj` allowance is not newly invented here: its regex is exactly the existing comparator's `groupExpanded` expression, and is applied only to `.moproj` files. Thus this test inherits that comparator's established blind spot for the matched cycle/group prefix/attributes, while it does not add the DOE view masking or any additional file exclusions.

## Diagnostic limitation

For raw differences, the test prints the differing relative paths but not per-file hashes or byte-level diff receipts. The actual candidate files remain under the unique scratch leaf (the test does not clean them), so the raw bytes are recoverable after a run; a failure report alone will not identify the differing byte ranges. This is a diagnostics limitation, not a false-pass path: every non-identical comparable file must pass one of the narrowly copied comparator allowances or remains in `different` and fails the test.

No build or test was run in this review. A passing execution would qualify candidate ABI status/output and success-then-failure isolation against this retained mode-0 case; it would not constitute a fresh original run or prove other guarded workflows.
