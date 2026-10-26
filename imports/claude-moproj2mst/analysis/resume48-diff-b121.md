# Historical diff lane B:121_B_DOE_VAR

## Finding

The checkpoint47 `DIFF B:121_B_DOE_VAR` is not an isolated Open–Save mismatch. The comparator feeds the preserved `_before` project with no DOE study, while its expected folder is the later GUI state after creating a DOE study, adding a variable, and saving. The raw comparator reports one differing file (`COG_P1_B1.moproj`), eight expected files absent, and no extras. It does not retain a candidate-vs-expected byte dump or the comparator scratch folder, so exact differing byte offsets/content for the `.moproj` cannot be recovered from this receipt. The `differ=1` label must not be expanded into an unverified byte-level claim.

Evidence: `analysis/wave3-checkpoint47-validation.json` classifies this as a historical diff; the raw log is `build/test-runs/1792864027058355400/m2m.s.save_folder.log` lines 93–102; `analysis/wave2-doe-evidence.json` records the before/after artifacts and hashes. The before project is 27,053 bytes, SHA-256 `e5b529b6440ebd55bfbb701da26beae94a67c883f35c4c177a9a80caf530e523`, with one Type=0 sub-project and no study files. The after project is 34,757 bytes, SHA-256 `fb271b8ef6ab67d14fd215827bccb3ac9e462f4d3ccd7ca878d39efe3d421c45`, with the added Type=2 `DOE Study 1` sub-project (`Id=1`, `Number=6`) and `<SfDoeStudy name="DoeStudy1"/>`.

## Exact comparator inventory

Expected paths missing from the conversion of the no-study `_before` fixture:

| Relative path | Oracle bytes | SHA-256 |
|---|---:|---|
| `DoeStudies/DoeStudy1/Control/COG_P1_B1.DS` | 2,013 | `97e4aaa28aa545dd08360e98191fba3b2cfb6c39098e10aeb3b45d02c33e563d` |
| `DoeStudies/DoeStudy1/Control/COG_P1_B1.MST.TEMP` | 7,316 | `97a984706624fac35cfce04beddebc285064d952cf34bc00dffe6845ef656376` |
| `DoeStudies/DoeStudy1/Control/DEF_DOE.INI` | 3,080 | `0f40dc9e2e72cafc9c0a4d7137751a5d5340362c6e7e4779a1975727ddbf4393` |
| `DoeStudies/DoeStudy1/Control/DEF_DOE.PAR` | 1,396 | `1b6887361a11fb31ddd728ab10c4c35f6405a5399a849e4f6d2b4285d0c50791` |
| `DoeStudies/DoeStudy1/Control/DEF_DOE_PARSER.INI` | 38 | `93ee4373d79f249409a144af1617283338a84582fc13462593a58389f2e3e195` |
| `DoeStudies/DoeStudy1/Control/DoeInputTable.XML` | 707 | `18ec0c482730c82ade12e91771ca67e53e4604573032624b99f3f4829a86b069` |
| `DoeStudies/DoeStudy1/Control/Samples_LHS.par` | 1,396 | `1b6887361a11fb31ddd728ab10c4c35f6405a5399a849e4f6d2b4285d0c50791` |
| project-root `temp.KWD` | 13 | `15eefcf3997f7c89507af9d42db2e38a37e38f4f20d7f3b0467bd992455dc404` |

The project-file differ is the oracle's added Type=2 study transition and its related serialized fields, but the raw comparator output does not identify which byte ranges differ. The expected after-state `DoeInputTable.XML` has one `DoeVariable`: `name="Transfer Time"`, `varId="1"`, `dimension="3"`, `numSamples="3"`, keyword `TLOC`, `unitType="0"`, parameter `-1`, nominal value `0.1`, and an operation mapping to id `1`. `DEF_DOE.PAR` and `Samples_LHS.par` are 101-line mode-0 sample tables with one value column; first rows include `1\t0.55605\t`, `2\t0.858065\t`, `3\t0.420254\t`. `temp.KWD` is the retained 13-byte text `TLOC    \t0.1\n`. These hashes describe the oracle after artifacts, not expected bytes that should be copied into an implementation.

The recorded original case sequence is therefore: begin from the single no-study base project; add `DOE Study 1`; add the `Transfer Time` TLOC variable on operation 1 with three samples; save. Historical file contents show the state, while the saved project pair establishes before-vs-after structure. The generic corpus comparator does not retain a timestamped UI action trace, so exact action ordering beyond the saved state is not asserted here.

## First reachable variable-study guards in current source

Keep parser and Save boundaries distinct from the Report QObject prerequisite.

- For the exact historical comparator input (`121_B_DOE_VAR_before`), there is no Type=2 sub-project or `SfDoeStudy`; the variable-study parser is not reached. The current converter serializes the main Type=0 sub-project and cannot recreate UI-created study/variable state from absent input. This explains why the historical expected files cannot be treated as pure Open–Save required output.
- If a separately identified run opens the retained **after-state** project and normal DOE ownership/I/O wiring is available, `model/subproject.cpp::parseSubProject` routes `<SfDoeStudy>` to `doe::readLazyStudy`. It first requires `ownedDoeStudy`, the `sub.doe` alias to that same object, and `p.doeIo`; otherwise the earlier generic guard reports `DOE lazy parsing requires owned study and configured I/O`. After that prerequisite, `readLazyStudy` calls `readInputTable`. The first variable-specific guard is `doe/doe_study.cpp::readInputTable`, at the `<DoeVariables>` element: nonempty child elements throw `M2M_E_UNSUPPORTED` with `DoeVariables reader 0x14090ee20 has no oracle case yet` (lines 339–341). In the retained 121 XML, that `<DoeVariables>` contains the `DoeVariable` immediately. The later empty-`DoeVariables` existing-state finalization check is not reached first.
- On Save, before the variable writer branch can run, `moproj/write.cpp::subProjectElement` currently throws for any `sub.type == 2 || sub.type == 3`: `project file: DOE / optimisation study elements (0x1400d29d0, 0x140907a10)` (around lines 131–133). Thus the first current Save-time guard for a Type=2 study is the general project-element refusal; it prevents reaching `doe_study.cpp::saveInputTable`'s variable guard at `0x1400d2b60` or its nested `writeInputTable` variable part at `0x14090a550`. The source-level variable writer guards (`!s.vars.empty()`) remain later blockers even after the general Type=2 serializer is recovered.

These are DOE source/parser/serialization limitations. They are independent of the first Report QObject prerequisite; resolving QObject construction would not itself provide the missing historical Add Study/Add Variable actions, variable XML reader/writer, DOE session/MST writers, or correct file ordering.

## Classification

Keep this row as a historical UI-transition reference pair, outside isolated Open–Save fidelity scoring. Preserve the raw `1 differ / 8 missing / 0 extra` result and the oracle hashes above. For Open–Save acceptance, use a separately identified input that already contains the study and variable state; it will test the actual active variable-study parse/Save guards and must retain its own oracle provenance.
