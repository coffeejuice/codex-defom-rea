# CP288 installed-file identities

The three fixed installed files matched every CP283 path, size and SHA-256 pin during one read-only acquisition. The result is `ACQUIRED_STATIC_IDENTITIES_ONLY`; `original_run_ready` remains false and `runtimeLoadedModuleIdentity` remains unknown. This observation establishes installed byte identities during the held-handle acquisition interval. It does not establish loader resolution, process architecture, runtime module identities or original-run readiness.

The selected frozen converter baseline is `c515cd4028641ad2ea21a8fde1b566f01a65f5aa`. Source qualification is `analysis/resume283-color-dependency-preflight.md` and its JSON receipt. The CP283 Qt file-version observations remain prior evidence; this collector does not query versions or infer new version metadata.

## Acquisition method and evidence

`admin/color_installed_file_identity.py` has no path-selection CLI. It opens only the three fixed installed paths using `CreateFileW`, `GENERIC_READ`, `FILE_SHARE_READ` only, `OPEN_EXISTING` and `FILE_FLAG_OPEN_REPARSE_POINT`. Each open is recorded before metadata or content processing. Non-disk, directory and reparse attributes are rejected. `GetFileInformationByHandle` and `GetFinalPathNameByHandleW` bind the exact normalized DOS final path, volume serial, file index, last-write FILETIME, attributes and size before/after `ReadFile` SHA-256 hashing on that same handle. All three handles remain open for the final collective metadata/path comparison. Every opened handle receives a `CloseHandle` attempt in `finally`, including failure paths; any reported close failure prevents success. No partial file-success list is published on failure.

Root independently reran the fixed read-only acquisition. `analysis/cp288-installed-file-identities.json` records 2026-10-27T00:50:03.769301+00:00 through 2026-10-27T00:50:03.821605+00:00. It records volume serial `1515164610` and file indices `281474979423216` (DEF_GUI_MO), `281474979423209` (QtGui) and `281474979423208` (QtCore). All three before/after/collective-final observations match; all three handles were closed successfully; errors are empty.

| Installed basename | Bytes | SHA-256 |
| --- | ---: | --- |
| DEF_GUI_MO_64.exe | 67611136 | fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc |
| QtGui4_64.dll | 10859008 | c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887 |
| QtCore4_64.dll | 3250688 | bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160 |

Every path is directly under `C:\Program Files\SFTC\DEFORM\v11.0.2`. Installed files were opened read-only and not changed. No original application, GUI, Frida, REA or build was invoked; no observer/probe ABI was imported and no child processes were created by the collector or tests.

## Tests and source binding

Root reran `python admin/test_color_installed_file_identity.py`: 11 tests, 10 passed, zero failed, zero errored, one OS symlink test skipped because this token lacks symlink privilege (WinError 1314). The final run took 0.031 seconds. `analysis/cp288-installed-file-identity-tests.json` retains that skip separately. A deterministic passing test injects FILE_ATTRIBUTE_REPARSE_POINT into a successful real `GetFileInformationByHandle` response, exercises the actual rejection branch and verifies cleanup; it does not replace the skipped OS symlink fixture with an asserted pass.

Passing cases cover matching hashes and IDs; all three held through collective validation; existing-writer sharing refusal; held-reader refusal of new write/delete handles; wrong content, size and final path; injected second-file read failure; changed final metadata; deterministic reparse rejection; and injected close-report failure with continued attempts for remaining handles. A close-report failure is conservatively HOLD with `allHandlesClosed=false`, even though that specific test closes the actual OS handle before injecting the error. Disposable scratch directories were under this converter's analysis directory and removed by TemporaryDirectory cleanup.

Collector SHA-256: `b0943bb7927df0eb140399b22e76df37255680ba95c416d7d721a5398b1725a7`.

Test source SHA-256: `d0d72ddad537909821fa76adf9d9f0a27c2198a5909b98455cb8b8a2143ecbc6`.

The single installed acquisition uses the same collector hash as the final passing test receipt. Test-source strengthening after acquisition did not change the collector. `analysis/cp288-installed-file-identity-evidence.json` binds final collector, tests, receipts and this report. Token measurements are unavailable; no efficiency improvement is claimed.
