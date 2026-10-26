# Resume 47: writer-specific failure target — native project Save

## Selected target

Target the reachable native `.moproj` writer in `source/apps/moproj2mst/src/moproj/write.cpp`: `saveProjectFileNative` → `saveProjectFileBody`. Add one project-writer-specific write/flush failure regression, using an already-open destination and an injected short/failed flush. This is a real gap after reconciling the existing failure cases: Save I/O tests cover project destination open denial (`save_project_open_failure_preserves_project_but_continues_boolean_and_mst`, `project_native_open_failure_precedes_dom_mutation`) and successful direct write without delete sharing (`save_project_direct_write_does_not_require_delete_sharing`), but no project writer test injects a write/flush failure after successful open or asserts the resulting project bytes and later Save continuation.

## Original-side qualification

The original source is offline static evidence only; no original application or Qt runtime failure was injected for this target.

- `analysis/wave3-project-close-status-contract.json` binds the caller to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, function `0x1400983c0`, and retains call order: set UTF-8 codec, serialize DOM, insert the string into QTextStream, call `QFile::close`, then query `QFile::error`.
- The receipt states the project writer returns false on failed open or nonzero QFile error after Close, and does not inspect QTextStream status. Top-level Save continues subsequent property/MST stages when project writing returns false.
- `analysis/wave3-file-device-error-contract.json` and its linked QtCore disassembly receipts bind the device mechanics to QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`: QFile buffer failures and QTextStream sticky status are separate; Close saves its initial QFile flush result before `aboutToClose`, and a successful engine close can clear QFile error even when the later text callback failed. Therefore the writer result cannot be predicted from QTextStream failure alone.

These receipts establish ordering and status distinctions, not the exact original on-disk residue for a given short-write/OS failure. Do not claim a particular partial XML prefix as original behavior until a deterministic original-equivalent result is qualified.

## Candidate coverage reconciliation

- `source/apps/moproj2mst/tests/s/test_save_io.cpp` has project open-denial, successful direct-write/share-mode, and pre-open-DOM-mutation cases at the named test cases above. It has no project writer stream-failure/partial-file case.
- The same file *does* have writer-specific stream-failure coverage for Forming wizard, Distortion INI and DieStress XML; those do not close the project-writer gap because their writer status/flush contracts differ.
- `m2m.mdt.file_device` has lock-driven generic FileDevice stream/Close failure cases (listed by `analysis/wave3-file-device-error-contract.json`). They test the device mechanism, not `saveProjectFileNative`'s QTextStream callback, post-Close `QFile.error` decision, project DOM effects, or top-level continuation.
- `m2m.s.save_io` currently has nine focused cases per `analysis/wave3-project-save-validation.json`; its pass receipt is not evidence that a project writer flush failure was exercised.

## Smallest regression fixture

Extend the existing project Save I/O fixture with a single parameterized failure after successful native open:

1. Start from the existing disposable project rig and preserve the original `.moproj` bytes. Ensure the project serialization exceeds the 16 KiB QFile buffer or explicitly drive a flush boundary; otherwise the case may only test buffering.
2. Open a second share-compatible handle to the project destination and lock the relevant byte range before `saveProjectFileNative` writes. This provides a deterministic candidate-side failed flush without relying on disk-full state. Keep the test confined to the owned scratch project.
3. Assert separately: (a) Save reaches and completes later permitted Boolean/INI/MST stages, (b) project DOM mutation is retained or not according to the pre-open gate already covered, (c) project writer's result follows post-Close QFile error rather than QTextStream status, and (d) exact candidate file bytes after unlock/close are captured. Do not assert that this candidate residue is original-equivalent until it is compared with a source-backed oracle.
4. Include a companion subcase that unlocks before QFile Close's recovery opportunity. This distinguishes an unrecovered close/error state from a recovered byte-buffer failure, because the Qt receipt shows Close may clear QFile error while QTextStream status remains sticky.

The irreducible target is the project writer's close callback plus post-Close error sample. If the current project file is below the QFile buffer size, enlarge only the disposable fixture or use an injected FileDevice seam so the failure is known to occur after successful open. Keep stream status, QFile error, project-writer bool, outer Save result, and output residue as separate observations.

## Limits

This is a focused prerequisite proposal, not a source change or a claim of validated original behavior. The receipts do not establish positive short-write residue or CloseHandle failure effects. Any final fixture must preserve those as unresolved until candidate behavior is independently compared against an original-qualified outcome.
