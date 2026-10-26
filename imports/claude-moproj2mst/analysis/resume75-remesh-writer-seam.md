# CP75 private remesh writer seam

## Change

Added the source-private `simcontrol300_remesh_internal.hpp` seam. `saveRemeshAtProjectSave` still uses its native `CoggingFile` by default; that per-call native factory forwards each format call to the existing legacy `vfprintf` formatter and closes the same `CoggingFile`. The public `include/m2m` interface and converter/Save interfaces are unchanged. The existing path-profile, locale, failed-open, invalid-parameter guard, and disabled-removal branches remain before/around the writer as before.

Added `remesh_save_ignores_post_open_write_and_close_errors_and_continues`. Its fake file reports successful Open, returns failure on formatted write 6, returns success for later writes, and reports a failed Close (which also represents the only flush boundary in this writer; the original has no explicit flush call). The test checks that all 31 ordered format attempts and Close still occur and that the report remains `Completed`. The fake has no backing file, and the test makes no physical residue assertion.

## Static preflight and limits

`git diff --check`/targeted trailing-whitespace scan found no whitespace issues in the owned edits. No build or test was run. Root should build the existing `m2m.mdt.simcontrol300_remesh` target and confirm the pre-existing literal-derived byte tests still pass before integration.

The static native-source contract is `analysis/wave3-remesh-save-lifecycle-contract.json`: enabled writer opens with `wt`, performs the 31 ordered `fprintf` calls, closes, and checks no write/close return values. `analysis/wave3-remesh-invalid-parameter-startup-contract.json` qualifies the separate failed-open/null-FILE path; the new test never enters that branch. Neither test nor report claims fresh original fault execution or a concrete partial-file residue.

## Frozen SHA-256

- `src/engine/simcontrol300_remesh.cpp`: `5ED9068356712ACD03D8643D6006F921E32E69E4D07C7719B8BA726911E0A1BF`
- `src/engine/simcontrol300_remesh_internal.hpp`: `4B4D3DF42673AD02E587A7BC52F03B41C83486369105CB820208BFCF25981936`
- `tests/mdt/test_simcontrol300_remesh.cpp`: `24C82860089C0E2DDC49A12652CA362560EB6CA4814BFE6D90A0E69D988BE769`
