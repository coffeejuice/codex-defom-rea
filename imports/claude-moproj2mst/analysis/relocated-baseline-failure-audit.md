# Relocated baseline failure audit

The 94-test run recorded six failing executables and no imported fixture changes.
Every failing case stopped on `weakly_canonical: Access is denied`; these logs do
not establish a converter byte, arithmetic or state mismatch. The preserved
receipt is `relocated-baseline-tests.json`, run `1792751668406402100`.

| Executable | Stopping path and classification |
| --- | --- |
| `mdt.db_load` | Planned `Equipments/Equipment00005.KEY` in the scratch project; inherited `fs::relative` canonicalizes an unwritten effect path. Six of seven cases reported success. |
| `mdt.multiblow_save` | Planned `SimCtrl.KEY` under three imported `_before` folders; all three cases stop in inherited effect-path relativization. Read-only probes confirm those files are absent and their parents readable. |
| `mdt.save_l6` | Planned scratch `98_B_DS3F/Gear_blank.KEY`; inherited effect-path relativization. Its second test case reports successful 107/119 comparisons and a missing 87 oracle skip. |
| `moproj.moproj_ops` | New helper canonicalizes the not-yet-created `dump-moproj` directory. This is a relocation-helper defect; it occurs before comparison results are printed. |
| `s.postload_views` | Planned `Equipment00007.KEY` under `96_B_FNOEQ_before`; inherited effect-path relativization. Earlier 93/94 cases reported PASS. |
| `s.save_current` | Planned `95_B_FE3ACT_before/Operations/Task00003/SimCtrl.KEY`; inherited effect-path relativization. Earlier historical comparisons reported PASS; the corpus test reports 56 saved operations. |

Absent planned files are not evidence of an import loss. The five inherited
comparators should use lexical relative paths for their recorded in-memory
effects. Actual output containment must still resolve existing ancestors and
reject symlink/reparse escapes. The new dump helper needs to handle a missing
leaf without trying to canonicalize that leaf before creation.

## Diagnostic gaps

Visible gaps are an active-cogging refusal in `mdt_write`, its optional imported
PACK skip, the optional external deflate-vector skip, missing Oracle-B case 87,
two unavailable corpus projects (29/30), and three declared Oracle-A exclusions
(53/55/56). The p07 summary reports 45 pass and five skipped; its summary line is
not a sixth distinct skipped case. Existing fresh-session qualifications and the
ABI OK-or-UNSUPPORTED assertion remain applicable.

Six passing JUnit outputs are truncated at 1024 bytes: `runner_self_test`,
`moproj_write`, `props_l2`, `p12.objects`, `release.unchanged`, and `save_folder`.
Consequently additional gaps cannot be counted from these preserved logs.

The folder parser's test name and `DIFF ` prefix match the test source. Its input
is incomplete: only twenty initial SAME rows survive, followed by the truncation
notice. The recorded zero diagnostic differences/refusals is therefore unknown,
not a zero-result finding. The overall CTest log and console do not contain the
missing passing bodies. A complete-output rerun is required; the old result is
preserved without silently filling in missing rows.

The next infrastructure revision should capture verbose passing output, raise
CTest passed/failed output limits, detect truncation or absent folder summaries,
and report actual sweep denominators, DIFFERENT/SAME, refusals, exclusions and
normalized profile/banner counts. Diagnostic SAME remains comparator-qualified.

This audit performed read-only inspection and Python file-status probes. No test,
build, production edit or original application execution was performed. Exact
log excerpts and source locations are retained in the companion JSON.
