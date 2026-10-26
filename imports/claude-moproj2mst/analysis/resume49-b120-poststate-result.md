# B120 post-state candidate Open–Save result

## Run identity and input integrity

I ran the existing checkpoint 47 candidate CLI read-only from the shared build tree; no configure/build step or shared-build file was changed. The executable was `build/candidate/bin/moproj2mst.exe`, SHA-256 `890deaa69b91be2eeec13619abf0599225e5ce1f892d0b4c401b0cf7b84fe361`. Its associated build receipt is `analysis/resume47-combined-full-build.json` (source tree SHA-256 `d37a1b509af1180191be7a7e633e38f277adf3e3bc893ab94a0b536fc83fff69`).

I copied the retained post-GUI B120 directory to the unique scratch path `build/b120-poststate-candidate/project/`. The source `.moproj` SHA-256 before the run was `60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e`, matching `analysis/wave2-doe-evidence.json`. The copy contained 76 files; exact relative paths, byte lengths, and SHA-256 values are saved in `build/b120-poststate-candidate/before-manifest.json`.

The candidate command was:

```text
moproj2mst.exe <copied-project>/COG_P1_B1.moproj --write-save
```

It exited with code 0 and an empty stderr/stdout log (`candidate.log`, SHA-256 `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855`). The complete after-run file inventory is in `after-manifest.json`; `comparison.json` records the summary.

## Raw output comparison

The copy still contained 76 files after Save. There were no added or removed files. Exactly one file changed:

| Path | Before | Candidate after |
|---|---|---|
| `COG_P1_B1.moproj` | 34,774 bytes, SHA-256 `60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e` | 34,702 bytes, SHA-256 `37ff4cd0ce1860cc1eaa4d2903af5aa2c1b31124722197c6d7ffa9657cec64a6` |

The line diff places all observed `.moproj` value changes inside `ViewTransform` / `SfDispView`: view center, quaternion, ROI bounds and origin are reset/updated. Removing those two view elements with the same rule used by `tests/s/test_save_folder.cpp::maskView(..., true)` makes the input and candidate `.moproj` byte strings equal. The main MST remained byte-identical at SHA-256 `07b0a85d88c31326ecd1f069d765fdc14e8f5efdc046195ed301eb014e20eb1c`.

All seven retained DOE control side files remained byte-identical and present:

- `DoeStudies/DoeStudy1/Control/COG_P1_B1.DS`
- `DoeStudies/DoeStudy1/Control/COG_P1_B1.MST.TEMP`
- `DoeStudies/DoeStudy1/Control/DEF_DOE.INI`
- `DoeStudies/DoeStudy1/Control/DEF_DOE.PAR`
- `DoeStudies/DoeStudy1/Control/DEF_DOE_PARSER.INI`
- `DoeStudies/DoeStudy1/Control/DoeInputTable.XML`
- `DoeStudies/DoeStudy1/Control/Samples_LHS.par`

Their before/after hashes are in the manifests. In particular, unchanged `.DS` and `.MST.TEMP` hashes are preservation results, not proof the candidate parsed or regenerated those outputs.

## Reachability and qualification

This candidate run used the post-GUI project as a static input model. The public driver selected subproject 0 (Type 0); it did not activate the Type 2 DOE subproject. Therefore the run establishes candidate-side ordinary Open–Save preservation for the inactive study folder, plus a `.moproj` view rewrite. It does not validate fresh-original parity, Type 2 lazy-study parsing, DoeOutput loading, task/StepData, sample generation, DOE side-file writers, or `.MST.TEMP` generation.

No original app was run, so this is not a paired Open–Save result. The retained post-GUI folder is not a fresh original Save output for this same starting state. The historical B120 result remains an action-sequence mismatch (study creation plus Save against a no-study `_before` project); this run does not reclassify that pair as parity evidence.

## Raw artifacts

The disposable copy and raw inventories are retained at:

- `build/b120-poststate-candidate/project/`
- `build/b120-poststate-candidate/before-manifest.json`
- `build/b120-poststate-candidate/after-manifest.json`
- `build/b120-poststate-candidate/comparison.json`
- `build/b120-poststate-candidate/candidate.log`

No source files, original project evidence, application state, or mutable REA database were changed.
