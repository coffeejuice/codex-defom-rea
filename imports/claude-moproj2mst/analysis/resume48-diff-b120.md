# Historical diff lane: `B:120_B_DOE`

## Checkpoint 47 raw comparator result

The checkpoint 47 `m2m.s.save_folder` log is `build/test-runs/1792864027058355400/m2m.s.save_folder.log`. It reports:

- `1 differ`: `cog_p1_b1.moproj`
- `7 missing`: `DoeStudies/DoeStudy1/Control/COG_P1_B1.DS`, `COG_P1_B1.MST.TEMP`, `DEF_DOE.INI`, `DEF_DOE.PAR`, `DEF_DOE_PARSER.INI`, `DoeInputTable.XML`, and `Samples_LHS.par`
- `0 extra`

The comparator is a file-tree comparison; its `.moproj` comparison permits view masking for DOE cases, but no comparator adjustment is recorded for this case. The log does not include raw byte ranges for the `.moproj` mismatch. The retained evidence does establish the project-file delta: the `_before` project is 27,053 bytes, SHA-256 `e5b529b6440ebd55bfbb701da26beae94a67c883f35c4c177a9a80caf530e523`, has one Type=0 subproject and no study. The expected post-interaction project is 34,774 bytes, SHA-256 `60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e`, with an additional Type=2 `DOE Study 1` subproject and `<SfDoeStudy name="DoeStudy1"/>`. The expected folder's seven side files are recorded with hashes in `analysis/wave2-doe-evidence.json`; no expected-sidefile hash is a candidate output hash.

The test source itself marks `_B_DOE` as “saved after GUI interaction”; the case manifest/findings identify that interaction as study creation followed by Save. The before and after trees therefore represent different action sequences. This raw diff is evidence of those actions, not a valid failure of isolated Open–Save equivalence. Do not change converter behavior to create a DOE study absent from its input.

## First reachable branch in current source

On the actual `_before` input, the DOE lifecycle is not reached: there is one Type=0 subproject, no `SfDoeStudy`, and no DOE study files. The current conversion driver activates subproject index 0. The project read can parse this ordinary subproject, open/save its active operation, write the main MST and rewrite the `.moproj`; it has no source basis for synthesizing a second study subproject or the seven files above.

If the post-GUI `_after` project is instead supplied as input, the second Type=2 subproject is not the selected current subproject: `core/convert.cpp` calls `activateSubProject(p, 0)`. The current source lazily reads nonselected subproject contents only when activated. Thus the normal conversion still does not exercise that Type=2 study's active Open/Save writer chain. If a test selects a Type=2 project as the active/current one, its first implementation boundary is the explicit refusal for the `SfDoeStudy` child in `model/subproject.cpp::parseSubProject`; DOE manager/input-table/sample activation must be completed before reaching the later output-document and `.MST.TEMP` guards. Separately, `M2M_FLAG_DOE_TEMP` is explicitly unsupported in `core/convert.cpp`.

This conclusion is supported by `analysis/wave2-doe-findings.md`, `analysis/wave2-doe-evidence.json`, `analysis/wave2-doe-active-task-checkpoint.json`, the current `core/convert.cpp`, `model/subproject.cpp`, `moproj/read.cpp`, and the lazy activation code. The existing static DOE writers and sample parsers do not change the current driver's reachability.

## Source-qualified next action

Retain `B:120_B_DOE` in the historical comparison archive with its seven-file/project delta and relabel it as a post-GUI study-creation pair outside the isolated Open–Save denominator. For a useful DOE lifecycle comparison, use the saved post-interaction project as a **new input**, then capture original and candidate from fresh, equivalent project state through Open–Save–Close. Record the selected subproject and capture output bytes/tree independently. This separates “study creation plus Save” from “Open–Save of an existing study.”

If no fresh reference capture is available, the next bounded source task is the current active Type=2 parse/activation contract beginning at `SfDoeStudy` ownership and no-variable input-table loading. Do not claim parity from B:120, and do not wire all seven writers merely to make this historical pair match. Expected side files combine distinct Save stages (session `.DS`, DOE sub-MST `.MST.TEMP`, control INI/parser, input table, sample outputs); their original order/partial-failure semantics need to be preserved when that active Type=2 path becomes reachable.

## Evidence limits

The checkpoint 47 folder sweep is diagnostic and uses a historical post-interaction oracle. It did not launch the original application. The original reference may be a separately accumulated GUI state; the stored pair does not prove which individual writer generated each file during the prior interaction, nor provide a fresh Open–Save acceptance result. No raw candidate output snapshot or `.moproj` byte diff for this case is preserved in the inspected checkpoint summary.

This audit was read-only. No source edits, builds, application control, or mutable REA operations were performed.
