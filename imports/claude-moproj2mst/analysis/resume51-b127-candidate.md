# Candidate-only public Open–Save on retained B127 study input

## Run

I copied the retained input `evidence/gui_mo/oracle_b/127_B_DOE_OPEN_before` to the unique disposable directory `build/b127-candidate-1792867297492/project` and ran the existing public candidate CLI:

```text
build/candidate/bin/moproj2mst.exe <disposable-copy>/COG_P1_B1.moproj --write-save
```

The command exited 0 with no stdout/stderr. The executable SHA-256 was `622073cb4f11cedf439820a7de39f9f3e7e1abdf3ae5eae621e6becf90040337`, matching the artifact in `analysis/resume50-close-dialog-focused-build.json`. This task did not rebuild or otherwise modify the shared build. The input project was 34,774 bytes with SHA-256 `60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e`, matching the fixture qualification in `analysis/resume50-doe-study-fixture.md`.

Raw complete pre/post file manifests and comparison artifacts are retained under `build/b127-candidate-1792867297492/`: `before-manifest.json`, `after-manifest.json`, `comparison.json`, `directory-and-selection.json`, `moproj.diff`, `candidate.log`, and `result.json`. Each manifest records every relative file path, byte length, and SHA-256.

## Observed output

There were 71 files before and after, with no added or removed files. Exactly one file's contents changed:

| File | Before | After |
|---|---|---|
| `COG_P1_B1.moproj` | 34,774 bytes; SHA-256 `60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e` | 34,702 bytes; SHA-256 `37ff4cd0ce1860cc1eaa4d2903af5aa2c1b31124722197c6d7ffa9657cec64a6` |

The retained raw diff shows the changes only in the Type 0 subproject's `ViewTransform` and `SfDispView` values (center, quaternion, bounds and origin). The Type 2 subproject XML remained byte-for-byte embedded in the same serialized structure apart from overall view rewrite; its `Active="5"` and `<SfDoeStudy name="DoeStudy1"/>` are retained. All seven study Control files retained their exact input hashes, including `.DS`, `.MST.TEMP`, `DEF_DOE.INI`, `DEF_DOE.PAR`, `DEF_DOE_PARSER.INI`, `DoeInputTable.XML`, and `Samples_LHS.par`.

The input tree had no `DoeStudies/DoeStudy1/Output` or `DoeStudies/DoeStudy1/Solutions` directory. Candidate Save created both empty directories. No directories were removed. These match the earlier candidate-only constructor receipt `analysis/wave2-doe127-constructor-v1-capture.json`; they are constructor effects, not evidence of DOE Output serialization.

## Selected subproject and DOE reachability

The public driver selected subproject index 0: `core/convert.cpp:223` explicitly calls `activateSubProject(p, 0, ctx)`. The copied project's retained Type 2 XML row has `Id="1"`, `Type="2"`, `Active="5"`; that `Active` attribute selects a diagram item within that subproject and does not override the driver's subproject index. `model/project.cpp:57` also initializes the model's selected subproject to 0, and `model/subproject.cpp:310-356` lazily parses only the subproject the driver activates.

Thus this public run exercised ordinary main-project Open–Save plus Type 2 manager construction and inactive-study preservation. It did not parse the Type 2 `SfDoeStudy`, load its active `SfOprDoeOutput`, import its `.DS` session, or call the DOE Output writer. The retained `.DS` does contain one `Image` tag. The existing Image/QImage refusal documented in `analysis/resume48-b120-poststate-contract.md` would be relevant only after a separately controlled Type 2 activation; this run never reached that guard.

## Qualification

This is candidate-only evidence on a copied historical B127 pre-action folder. It is not a paired fresh-original Open–Save capture and makes no original-parity claim. The output establishes preservation of the inactive DOE side files for this public path and confirms the two empty-directory constructor effects. It does not establish that any DOE output or optimization branch is supported. No original app, GUI, source edit, build, or mutable REA database was used.
