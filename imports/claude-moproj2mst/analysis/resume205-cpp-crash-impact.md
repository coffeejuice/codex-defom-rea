# Impact of the B98 command-line Open crash on the converter

## Finding

This capture is relevant to the converter's stated lifecycle and should not be dismissed as out of scope. It launches `DEF_GUI_MO_64.exe -m open -f C:\PROBLEM\p202a\Gear_blank.moproj` (image SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`). The input is a `.moproj`, and Open is the first stage of the converter's documented Open > Save > Close lifecycle. The same project and Open behavior therefore belong in the converter's compatibility evaluation if this B98 project is among supported inputs. Since the reconstruction requirement includes confirmed bugs, a confirmed crash on this supported path may require matching the process-level failure (or another evaluator-visible equivalent), rather than returning a convenient parse/unsupported code.

The evidence establishes one complete crash capture for the exact target image, fixture digest, and launch profile: an access violation in `QtCore4_64.dll!QListData::size`, read address `0x58`, followed by exit `3221225477` (`0xC0000005`). The fixture digest is unchanged. This is enough to put the behavior on the compatibility ledger, but one capture alone does not prove repeatability across fresh launches, wider B98 inputs, or the converter's full Open > Save sequence. Nor does it establish the cause. Do not label it a null-owner bug while the C++-side static owner audit is pending.

The original command requests Open only; public conversion with `M2M_FLAG_WRITE_SAVE` continues through activation and Save. The crash occurs during the shared Open stage, so a matching converter result cannot be assumed solely from the later Save behavior. Establish that this exact fixture traverses the corresponding reconstructed Open path before encoding crash behavior.

## What the public converter does today

The public entry points `m2m_convert_w` and `m2m_convert_utf8` call the conversion driver and return its integer result. With `M2M_FLAG_WRITE_SAVE`, the driver treats the path as already selected and follows `openProject`, operation activation, then Save. Unlike the legacy non-`WRITE_SAVE` profile, it skips the initial readable-file preflight; failed reads can therefore continue through the recovered Open tail and reach the later Save decision. A failed `saveCurrent` is translated to `M2M_E_PARSE` (`Error on reading project file!`). The C ABI catches `m2m::Error` and other C++ exceptions, writes a bounded diagnostic, and returns an error code. It has no documented crash result.

For ordinary C++ errors the current API maps to an integer result. A native access violation is not a C++ exception and is not guaranteed to be caught; therefore the implementation may already terminate abnormally if the corresponding defect exists. Source inspection does not determine the actual result for `Gear_blank.moproj`. That requires a converter-only run against a disposable copy, isolated in a child process, recording return code/message, process exit status, and file effects. If the converter currently returns normally, that is a behavioral mismatch candidate, not proof that it should be preserved as the desired result.

## Existing coverage

- `tests/g0/test_abi_open_failure.cpp` covers public C ABI `WRITE_SAVE` Open failures: missing project reaching the later Save rejection (`M2M_E_PARSE`), remesh reader failure (`M2M_E_UNSUPPORTED`), sharing-denied input, and rejected suffix behavior through UTF-8.
- `tests/mdt/test_conversion_session.cpp` covers internal `openProject` acceptance, failed XML reads, partial remesh state, and session/owner lifetime. Those are synthetic lifecycle fixtures, not the B98 Gear project or an end-to-end crash comparison.
- `tests/g0/test_abi.cpp` covers basic ABI argument/error mapping and a corpus project, where unsupported is accepted while writer seams remain unported.

These tests establish neighboring error contracts but do not validate this exact Gear project or its crash behavior.

## Focused validation and integration proposal

1. Keep the capture as a fixture-specific observation with target SHA-256, argv, unchanged fixture digest, exception/read address, and exit code. Treat repeatability and cause as open qualifications; do not assert a null-owner cause.
2. Run only the reconstructed converter on a disposable copy of `Gear_blank.moproj`, in a child process, with the public `M2M_FLAG_WRITE_SAVE` entry point and explicit output path. Record ABI result/message, child exit status (including abnormal termination), output existence/hash, and hashes of touched project-side files. Do not run the original as part of this converter audit.
3. Compare this result with the captured original Open behavior and the confirmed input scope. If the crash is confirmed for the supported reconstructed Open path, preserve the observable process-level failure or a deliberately specified equivalent at the public boundary; do not silently normalize it to `M2M_E_PARSE` or `M2M_E_UNSUPPORTED`. Avoid implementing it as an accidental invalid dereference. If the B98 path is outside the converter's accepted domain or the launch profile does not correspond to its contract, record that evidence and keep the crash in the broader GUI/CLI reconstruction scope.
4. Add an integration regression for the established behavior. Run the case in a worker process so a fault is contained. Assert the chosen observable outcome and all input/output side effects. If successful conversion is the expected behavior for a different profile, compare MST and `WRITE_SAVE` effects against independently retained qualified artifacts; do not conflate that case with the captured `-m open` crash.

No C++ or test files were changed, and no original process or binary REA operation was run for this audit.

## Appendix: safe converter-only B98 candidate run plan (not executed)

### Verified inputs and isolated-save fit

- Frozen candidate exists at `C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\build\wave3-checkpoint201-verified\moproj2mst.exe`. Its SHA-256 is `0B531BE96AF428C53031700EE62CFD193CEA73F879563499878E8DC367831FA3` (4,422,656 bytes), matching `build\wave3-checkpoint201-verified\receipt.json`. Use this exact frozen artifact; do not substitute `build\candidate\bin\moproj2mst.exe`.
- The complete preserved B98 fixture is `evidence\gui_mo\oracle_b\98_B_DS3F_before`, with 27 files totaling 2,719,651 bytes. Its `Gear_blank.moproj` SHA-256 is `E769FAF514B335D56FFBB22F2C76490D029F2714ADA9B26A75496FBD62ADE7FD`, matching the captured `C:\PROBLEM\p202a` project manifest. Copy this fixture into a fresh, disposable `C:\PROBLEM\b98cp201\project` tree; keep the evidence source and original crash capture untouched.
- `--isolated-save` preflight requirements are met by the proposed layout: absolute `.moproj` and output paths; `M2M_FLAG_WRITE_SAVE`; absolute existing `--deform-dir`; absolute receipt whose parent exists, lies outside the project tree, and does not alias input/output; output parent exists. The install directory `C:\Program Files\SFTC\DEFORM\v11.0.2` exists. The proposed receipt is a sibling of, not nested under, `project`.
- Important scope of that supervisor: it has a fixed 20,000 ms child deadline. It returns the adapter code only for a validated normal worker return; its special receipt/terminal path accepts the product's specific `0xC0000417` protocol boundary. A natural access violation during Open is not that validated terminal boundary, so it may yield supervisor rejection/no receipt rather than a crash-specific receipt. Thus `--isolated-save` contains and bounds this candidate run, but its receipt schema is not a generic record of arbitrary crashes. Preserve parent process exit code and stderr/stdout alongside any receipt. No original executable or GUI is invoked.

### Proposed layout and invocation

All generated paths below are under `C:\PROBLEM`, shorter than 100 characters. Before running, require the destination root not to exist; create it fresh, copy the entire `98_B_DS3F_before` tree into `project`, create `result` and `records`, and verify the copied manifest matches the source digest. Record sorted per-file relative path, byte length, and SHA-256 rows in `C:\PROBLEM\b98cp201\records\before.csv` and `after.csv`; the first covers the copied project tree and confirms the result file is absent, while the second covers both project and result trees. Put both manifests outside the converter's project tree. The explicit MST output is outside the copied input tree, and the isolated receipt is outside both the project tree and output file.

```powershell
& 'C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\build\wave3-checkpoint201-verified\moproj2mst.exe' 'C:\PROBLEM\b98cp201\project\Gear_blank.moproj' 'C:\PROBLEM\b98cp201\result\Gear_blank.MST' --write-save --deform-dir 'C:\Program Files\SFTC\DEFORM\v11.0.2' --isolated-save --isolated-receipt 'C:\PROBLEM\b98cp201\records\isolated-save-receipt.json'
$rc = $LASTEXITCODE
"converter_exit=$rc"
```

Create `result` and `records` before this call so all parents exist. The supervisor's own 20-second deadline is the operative run bound; if adding an outer watchdog, cap it at 30 seconds and record whether it fired separately from `$LASTEXITCODE`. A supervisor timeout, abnormal child status, or absent receipt is evidence to retain, not a reason to rerun automatically. The worker may write project-side effects under `project`; compare the before/after manifest and output explicitly. Keep output, receipt, both manifests, process result, and diagnostic streams together under the fresh root.

### Interpretation gate

This tests the frozen converter's full Open > Save lifecycle on a copy of the same B98 input, not the original's `-m open` profile. A normal public error code is not automatically equivalent to the original access violation. A child access violation would support a crash correspondence only after checking that it arose during the corresponding Open stage; a `65` supervisor rejection by itself is not enough to identify the worker's failure. Conversely, successful conversion would be a meaningful mismatch candidate to reconcile with the captured Open-only crash and static owner-chain analysis. Do not alter the frozen artifact or weaken the captured crash qualification based on this one converter run.

