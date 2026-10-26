# Historical DOE geometry diff audit: B:122_B_DOE_GEO (checkpoint 47)

## Comparator result and exact filesystem gap

The checkpoint 47 folder comparator log is `build/test-runs/1792864027058355400/m2m.s.save_folder.log` (same frozen source/candidate context as `analysis/wave3-checkpoint47-validation.json`). It reports `DIFF B:122_B_DOE_GEO: 1 differ, 10 missing, 0 extra`, with no comparator adjustments. The one changed file is `cog_p1_b1.moproj`. The ten missing paths under `doestudies/doestudy1/control/` are:

- `cog_p1_b1.ds`
- `cog_p1_b1.mst.temp`
- `def_doe.ini`
- `def_doe.par`
- `def_doe_parser.ini`
- `doeinputtable.xml`
- `samples_lhs.par`
- `workpiece-geometry_workpiece_1_lower.geo`
- `workpiece-geometry_workpiece_1_nominal.geo`
- `workpiece-geometry_workpiece_1_upper.geo`

The retained oracle output `evidence/gui_mo/oracle_b/122_B_DOE_GEO_diff/summary.txt` reports the original added these ten control files plus screenshots/view-state sidecars (`g01.png`…`g06_panel.png`, `view_state.json`). Those sidecars are not among the ten project-output omissions counted by this comparator. `COG_P1_B1.moproj.diff` shows the original action changed `Operation_List` 5→6, appended a `DOE Output` operation (id 6), changed the active project/subproject state, and added the DOE Study 1 subproject. This is a real edited-project result, not a no-op baseline Save.

## Original case actions and input qualification

The corresponding before-folder `evidence/gui_mo/oracle_b/122_B_DOE_GEO_before/COG_P1_B1.moproj` has five operations and no DOE subproject or `<SfDoeStudy>` element. It has no `DoeStudies` files. The after-folder adds `<SfDoeStudy name="DoeStudy1"/>` and a new DOE Output operation. Its generated `DoeInputTable.XML` contains one `DoeVariable` named `Workpiece-Geometry`, `dimension="3"`, `numSamples="3"`, with a nested `Geometry` child carrying `UpperGeoFile`, `LowerGeoFile`, and `NominalGeoFile`; its `<Operations>` child maps the variable to operation 1. The lower/nominal/upper GEO files are distinct required project effects.

The frozen original-source notes `build/reference-harness/billet-training-004/frozen-source/docs/re/moproj2mst/mdt/L6/doe.md` classify 122 as “Workpiece geometry.” They record that Add DOE Study writes the `.moproj` and Control files, and the geometry “+” action writes the three `.GEO` copies before Save. Thus the historical case includes explicit GUI state-creation/edit actions before Save. A simple Open→Save over the before-folder cannot be expected to synthesize the new DOE Study, geometry variable, DOE Output operation, or three GEO variants unless those actions are represented in the candidate's input/action model.

## Current source comparison and first guard

The current converter has partial DOE serialization for the no-variable study case (case 120). It does not yet implement the 122 variable/geometry path:

1. On a post-action project containing a DOE Study subproject, `model/subproject.cpp::parseSubProject` first requires `ownedDoeStudy`, pointer identity `sub.doe == sub.ownedDoeStudy.get()`, and configured `p.doeIo`; otherwise it raises `M2M_E_UNSUPPORTED` with “DOE lazy parsing requires owned study and configured I/O.” This is the first enclosing lifecycle guard, but it is not specific to geometry.
2. When those dependencies are present, `doe::readLazyStudy` reads `DoeInputTable.XML`. Its `DoeVariables` branch refuses as soon as the element has children: “DoeVariables reader 0x14090ee20 has no oracle case yet.” Case 122 has a nonempty DoeVariables element containing the Workpiece-Geometry variable and nested Geometry payload. Its `Operations` branch likewise refuses nonempty children (`0x140912a20`). These are the first direct content guards for the recorded geometry-variable case.
3. On a state that reaches Save with a variable, `saveInputTable` refuses nonempty `s.vars` at `0x1400d2b60`; `Study::geometryVars` separately refuses mapping variables to geometry (`0x14091ba60`). `writeDefDoeIni` records `REGIONS` paths only for `s.regions`, but no path here provides the missing geometry-variable import/copy semantics. The current source notes explicitly classify variables and constraints as refused pending 121/122.

These guards explain why the implementation cannot round-trip the post-action state or generate the geometry side files. They do **not**, alone, explain the checkpoint 47 10-missing diff from a before-folder input: the before-folder has no DOE Study or variable to parse. The comparator's changed `.moproj` plus missing DOE effects primarily reflects that the original run performed DOE creation and geometry editing that the candidate was not given as explicit lifecycle actions. The state mutation difference and the missing writer/geometry support are related but distinct gaps.

## Disposition

Treat this historical mismatch as two linked items:

- **Action-model coverage:** encode the original Add DOE Study, add Workpiece-Geometry (three samples), and DOE Output operation actions—or compare from a qualified post-action checkpoint—before judging Open/Save parity for those effects.
- **DOE geometry implementation gap:** recover the `DoeVariables` / `Operations` geometry readers, variable-to-geometry mapping, GEO copy/write ordering, and remaining DOE output writers (`.DS`, `.MST.TEMP`, `DEF_DOE.INI`, `DEF_DOE.PAR`, parser INI, samples PAR, input XML) from original evidence. Do not claim the 122 mismatch is fixed by implementing only the early reader guard or by generating three GEO files.

The comparator itself is diagnostic and historical; checkpoint 47 validation says the folder sweep is not fresh original-session acceptance evidence and normalizes declared profiles. No source/build/app/mutable REA actions were performed for this audit.
