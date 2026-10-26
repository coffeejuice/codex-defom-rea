# CP87 logged-gap and no-GUI comparison audit

## Current validation and queue

Latest integrated receipt inspected: `analysis/resume85-integrated-grouped-tests.json`. It records 248 registered tests: 241 clean passes, six passes with logged gaps, one pass with logged differences, zero failures/skips/incomplete outputs. The latter `m2m.s.save_folder` result contains five historical raw DIFF rows, not one: A55; B120, B121, B122; and B98. A passing sweep does not convert those rows to matches.

Independent queue checks (`python tools/recovery_status.py` and `python tools/gui_validation_status.py`) still show no available task. `control_release` is blocked because the supported Windows tool surface has no release operation plus a verifiable receipt/status; all remaining original GUI tasks depend on it. Thus **no fresh-original runtime comparison is possible now**. This is the exact blocker; the six suite gaps do not supply a release capability.

## Six logged-gap suites

| Suite | Retained gap | Acceptance effect / non-GUI closure |
|---|---|---|
| `m2m.mdt.mdt_write` | Active Cogging writer is refused; optional PACK header check skipped because `M2M_INSTALL_FIXTURE` is unset. | The refusal is an explicit scope boundary; the optional fixture check can only be enabled with a valid read-only install fixture. Neither is an original-run comparison. |
| `m2m.mdt.save_l6` | Oracle case `87_B_HTFF` is absent. | Missing reference data prevents that case comparison; locating/providing a qualified retained oracle could close it without GUI, but no such data is included in this receipt. |
| `m2m.moproj.deflate` | Optional vectors not supplied (`M2M_DEFLATE_VECTORS` unset). | This is optional vector coverage, not a runtime acceptance gap. Supply the qualified vectors if available; no original GUI is implied. |
| `m2m.moproj.props_l2` | Five cases do not open because their operation classes are unregistered (`B11_CM`, `T_CM_DB.x`, `T_CM_ONLY.x`, `T_GRP.x`, `T_GRP2.x`). | Candidate registration/reader scope remains incomplete for those classes. Add them only with source-backed behavior and focused tests; retained historical open failures are not fresh app comparisons. |
| `m2m.p07.corpus` | Two projects are absent from corpus copy; A-excluded Cogging 53 and A55/A56 are excluded; summary says 45 pass, five skipped. | Coverage exclusions/missing inputs remain explicit. A55/A56 are separately represented in the Save-folder diagnostic, but P07 status is not a fresh Save comparison. |
| `m2m.s.save_current` | Five `.moproj` files cannot be compared because the inputs predate GUI edits (`R46`, `83_R47...`, `59_B_HT3`, `60_B_HT2`, `62_B_BM2`). | No equivalent pristine before-state is retained for these edited outputs. Do not synthesize it; find a qualified paired input or leave these cases excluded. |

The single diagnostic-difference suite's additional scope notes are seven corpus re-saves without completed fresh references and three cases with a preceding GUI interaction or answer mismatch (B114, B119, B130n). Those are not silently passing comparisons.

## Highest-value GUI-free independent comparison

For a newly generated candidate artifact compared to retained original bytes, B98 is the shortest actionable check. Run the already-frozen candidate CLI (bind its executable SHA-256 first) on an isolated copy of `evidence/gui_mo/oracle_b/98_B_DS3F`, with the copied `.moproj` under `C:\PROBLEM` and all generated absolute project/KEY/MST paths below 100 characters. Preserve the fixture manifest, exact command/cwd/status, raw candidate MST and SHA-256, and historical original MST SHA-256. Compare raw bytes first; then create a separate comparison copy replacing only the candidate `Gear_blank.KEY` path with the exact retained historical path and require every other byte to match. Do not change either raw file or the comparator. CP77 found no production path-owner defect; it found that the registered test's default build scratch path exceeds the project's short-path constraint, so this check should use a short-root run rather than rely on that existing test environment.

This is an independent **candidate-versus-retained-original-artifact** check for a source-qualified path rule. It is not a fresh-original execution, not raw-byte parity, and does not close the release gate. A fresh original B98 rerun at the same short path still requires the blocked app-control lifecycle. A55 and DOE fresh comparisons likewise require original application interaction/lifecycle and remain queued behind `control_release`; A55 also needs mode provenance, while B120–B122 are historical before/after action-state mismatches.

No source, build, test, fixture, comparator, GUI, original app, or mutable REA state was changed or run for this audit.
