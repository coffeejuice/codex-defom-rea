# CP125 DOE refresh increments versus historical DIFFs

## Acceptance boundary

The latest completed full validation available is CP124 (`wave3-checkpoint124-validation.json`): 312 registered tests, 305 ordinary passes, six passed-with-logged-gaps, one pass-with-logged-differences, 93 historical SAME / five DIFF, no fixture changes. The five DIFF rows are still A:RS_55, B:120, B:121, B:122 and B:98. `s.save_folder` remains diagnostic and normalizes only its declared view/banner profiles; its historical rows have no DOE-path adjustment.

CP124's new Constraint XML prefix and CP125's pending notifier reset do not make any of those five existing full-folder comparisons newly equivalent or newly conclusive. CP125 has a source implementation report but no subsequent full-validation receipt in the current evidence set. Its source-level behavior must not be stated as validated candidate acceptance yet.

## DOE rows

| Historical row | Before/after and current guard | Effect of CP124/CP125 |
|---|---|---|
| B:120_B_DOE | Retained before `.moproj` has only a Type0 subproject and no Type2 Study; retained after-state has a Type2 Study whose `DoeInputTable.XML` has empty `DoeVariables` and `Operations`. The retained source-cut report `resume124-doe-output-source-cut.md` says the B120 active DoeOutput Open probe reaches the first `DBLIST`/`ReportList` child and refuses before a normal Open/Save continuation. | CP124's actual-Study serializer only runs after a Type2 Study owner already exists, and CP125 resets the actual notifier flag only after a normal XML writer return. Neither creates the absent pre-action Study, supplies the missing DBLIST owner, nor gets the historical before-state through Open. The old B120 folder comparison remains a different-action-state comparison. A narrow candidate-only XML-prefix check on a separately loaded post-action Type2 state could qualify that prefix, but it is not an existing row becoming a valid full Save comparator; the current retained B120 Open probe is earlier-blocked. |
| B:121_B_DOE_VAR | Before-state lacks the study/variable present in the retained post-GUI output. Its after `DoeInputTable.XML` has a nonempty scalar `Transfer Time` variable and an operation binding. | CP124 explicitly stops nonempty variables before reconciliation; the zero-variable Constraint XML prefix is inapplicable. CP125's notifier reset follows a normal XML write, which the scalar guard prevents. No new files or project state on this row are established. |
| B:122_B_DOE_GEO | Before-state lacks the post-action geometry variable/operation; after-state has a nonempty `Workpiece-Geometry` variable and three GEO files. | CP124's nonempty-variable guard and missing geometry ownership precede its Constraint writer; CP125 lies later still. It neither imports geometry nor writes these GEO/DOE outputs, so the ten-missing-file DIFF is unchanged. |

The `Constraint` serializer contract is itself narrow: actual Session-owned Type2 Study, zero-variable profile, coherent owned zero-valued records, and an existing refresh boundary. It does not consume the source-unknown `+0x10` record field. CP125 adds only the unsuppressed `flag90=false` scalar store before signal-key construction refusal; suppression preserves the flag. It leaves key construction, registration lookup/snapshot, receiver dispatch, timer stop, UI continuation, and the subsequent Type2 PAR calls unimplemented. These increments may improve a focused prefix test, but do not turn the native Save into a completed output producer.

## Other historical DIFFs

- **A:RS_55_Multiple_Operations_Example1:** the two raw `SimCtrl.KEY` byte changes remain governed by the unbound original `MoInputMode` and conditional CG `toggled(true)` delivery. Neither DOE increment touches this path.
- **B:98_B_DS3F:** the path-derived raw MST line and CP124 candidate-only exact-path qualification are independent. DOE state changes neither affect nor normalize the B98 file.

## Could an existing raw comparator gain meaning without the original GUI?

No existing five-case full-folder comparator can be promoted on the strength of CP124/CP125. B120–B122 still compare a before tree that lacks state created by the GUI against after trees containing that state; the new serializer/notifier code is downstream of owned-state admission and does not synthesize those transitions. A55 and B98 are unrelated. The retained folder sweep's raw outcome remains useful as a description of its captured artifacts, not as equivalent-state parity evidence.

Without original GUI access, the only defensible next comparison is explicitly candidate-only and prefix-scoped: once CP125's source-stable build/test receipt exists, exercise a retained post-action B120 Type2 state at the actual Study/StudyIo boundary and compare just the bytes the candidate actually accepts/writes before the known refresh guard, while retaining the raw file, path, normal-return and guard receipts. Do not compare that partial output as a complete Save or relabel B120's historical row. Current B120 evidence identifies an earlier DBLIST Open blocker, so this comparison must first demonstrate an independently source-qualified route to the same Study owner; otherwise remain at the existing focused test fixture and do not claim B120 coverage.

No source, build, test, GUI, original executable or mutable REA state was changed or used for this audit.
