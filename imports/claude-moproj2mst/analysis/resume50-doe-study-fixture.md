# B121/B122 and retained Type 2 fixture for candidate public Save

## Finding

Use `evidence/gui_mo/oracle_b/127_B_DOE_OPEN_before` as the source for a disposable candidate-only `--write-save` input. It is the closest retained action-equivalent fixture: the historical B127 action opens an already-existing zero-variable Type 2 study and saves it, with no study-file changes. It avoids pretending B121/B122's GUI-created state came from Open–Save. B128 is also an Open–Save pair, but contains the additional `Transfer Time` variable; use it only if variable-study preservation is the target.

This public Save experiment exercises the default main-project path and inactive-study preservation. It cannot currently exercise active Type 2 / DOE Output serialization: `core/convert.cpp:223` calls `activateSubProject(p, 0, ctx)` unconditionally. Do not reorder or rewrite the fixture to select the Type 2 subproject and still describe it as the same public-path comparison.

## Why B121/B122 are not the fixture

The retained before projects for B121 and B122 are the same 27,053-byte no-study project (SHA-256 `e5b529b6440ebd55bfbb701da26beae94a67c883f35c4c177a9a80caf530e523`), with one Type 0 subproject and no DOE control files. Their after projects add Type 2 `DOE Study 1` and `<SfDoeStudy name="DoeStudy1"/>`. B121 adds seven Control files and variable edits (plus `temp.KWD` in the captured folder); B122 adds the same seven Control files, three geometry files, and geometry-variable metadata. These are historical study creation/edit-and-save transitions, not an Open–Save-only expectation. The details and hashes are in `analysis/wave2-doe-evidence.json`; the action distinction is also recorded in `analysis/wave2-doe-findings.md` and `analysis/resume48-diff-b121.md` / `resume48-diff-b122.md`.

The B121/B122 after-states do contain a Type 2 study, so they can be separately treated as already-existing input models. They are less suitable for the smallest candidate regression because they carry extra variable/geometry branches absent from the zero-variable constructor/save slice.

## Selected fixture evidence

`127_B_DOE_OPEN_before/COG_P1_B1.moproj` is 34,774 bytes, SHA-256 `60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e`. Its second subproject is Type 2 `DOE Study 1` with `Active="5"` and `SfDoeStudy` name `DoeStudy1`; the study contains only its `Control` directory at input. It has seven pre-existing files, with these SHA-256 values:

| Relative file | SHA-256 |
|---|---|
| `DoeStudies/DoeStudy1/Control/COG_P1_B1.DS` | `97e4aaa28aa545dd08360e98191fba3b2cfb6c39098e10aeb3b45d02c33e563d` |
| `DoeStudies/DoeStudy1/Control/COG_P1_B1.MST.TEMP` | `0dbf96e9d2cf225cf6f19e4d91e32f3dc737913c9a3d34abbfb6c8ecd6d1141b` |
| `DoeStudies/DoeStudy1/Control/DEF_DOE_PARSER.INI` | `93ee4373d79f249409a144af1617283338a84582fc13462593a58389f2e3e195` |
| `DoeStudies/DoeStudy1/Control/DEF_DOE.INI` | `0f40dc9e2e72cafc9c0a4d7137751a5d5340362c6e7e4779a1975727ddbf4393` |
| `DoeStudies/DoeStudy1/Control/DEF_DOE.PAR` | `000edf802c560ab52196be82feea8ae0732cf263dcd1fe8b3e6c9f6b1ab0f42a` |
| `DoeStudies/DoeStudy1/Control/DoeInputTable.XML` | `1777bb9948fb2d6f55411f9dcfdc4a7c9196230086c0f86720255d03e76571e0` |
| `DoeStudies/DoeStudy1/Control/Samples_LHS.par` | `000edf802c560ab52196be82feea8ae0732cf263dcd1fe8b3e6c9f6b1ab0f42a` |

The B127 historical after-project is 34,732 bytes (SHA-256 `342c1c86e523787c0154163fbc841e566bceccbc75ad348cc97156de959fcce9`). Its diff reports the `.moproj` view-state rewrite and added `view_state.json`; the seven DOE files have no added/changed/removed delta. That makes B127 suitable as a retained action reference for ordinary Open–Save preservation, not evidence that Save regenerated the DOE files.

## Public-path reachability and first guard

In the current public full-Save flow, `convert.cpp:223` selects subproject zero, so B127's second Type 2 subproject remains inactive. `model/subproject.cpp:310-356` only parses a subproject when activated; its Type 2 `SfDoeStudy` branch calls `doe::readLazyStudy`. Consequently the public run does not parse B127's study InputTable/sample files or activate its Type 2 `SfOprDoeOutput`. The first DOE Output guard described in `analysis/resume48-b120-poststate-contract.md` (the `.DS` `Image` path reaching unsupported QImage decoding) is not reached by this unchanged public path. It becomes relevant only in a separately identified Type 2 activation experiment. The existing B127 study `.DS` and public driver's subproject selection should be recorded when interpreting any run.

## Smallest candidate-only regression

Make a disposable copy of the full B127 `_before` folder, verify the project SHA and the seven file hashes above, and call the candidate's public `--write-save` against the copied `.moproj`. Preserve before/after raw manifests and report separately: exit/result, added/removed directories and files, `.moproj`, MST, and each DOE control file. The prior candidate-only constructor receipt `analysis/wave2-doe127-constructor-v1-capture.json` already demonstrates this branch on this fixture family: candidate Save exited 0, created empty `Solutions` and `Output` directories, and left the retained input unchanged; it is not a new original-vs-candidate acceptance result.

Expected scope of the focused regression: ensure the public default-main Save preserves the existing Type 2 XML and seven control-file byte hashes while running eager Type 2 manager construction. Empty `Solutions`/`Output` directory creation is separately observable and currently recorded in the constructor receipt; do not count it as active DOE output generation. No original GUI run is needed for this candidate-only check, but it establishes no fresh-original parity. A real active DOE output Save test still needs a separately controlled activation route and resolution of the current `.DS` Image/QImage guard before task, StepData, or writer paths can be exercised.

## Evidence limits

The historical retained outputs are sourced from GUI sessions; they establish their recorded before/after states, not fresh-runtime repeatability. The source call order establishes candidate reachability, not original runtime equivalence. B127's input `Active="5"` is not enough to make the candidate activate subproject 1 because the public driver explicitly selects index 0. No fixture was modified or run for this note.
