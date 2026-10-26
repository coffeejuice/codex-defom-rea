# CP71 DOE Type 2 post-action structure and TLOC boundary

## Retained state

The B120–B122 folders are post-action states, not raw outputs of the same Open–Save action applied to their `_before` inputs. The most useful retained file-level specification is B121's `DoeInputTable.XML` plus its `.moproj`, PAR files and `temp.KWD`. It gives a concrete minimal scalar-variable record to target after the no-variable study case.

### B120: Type 2 study with no variables

The after `.moproj` adds a Type=2 subproject `Id=1`, `Number=6`, `Name="DOE Study 1"`, plus `<SfDoeStudy name="DoeStudy1"/>`; its `DoeInputTable.XML` has the common mesh/positioning/ROI headers followed by empty `<DoeVariables/>` and `<Operations/>`. Its expected output tree contains seven Control artifacts. This is the smallest retained post-action DOE state, but it is not a no-op Save oracle: the before project has no study at all.

The current candidate has useful lower-level coverage for this empty shape: `doe::readInputTable` accepts empty `DoeVariables` and `Operations`, while `readLazyStudy` then reads `Samples_LHS.par`, `Samples_FF.par`, `Samples_USR.par`, and current `DEF_DOE.PAR` in a fixed order. Missing sample-table files trigger constructor-like default sample writes. However, the public converter activates subproject index 0; its default path does not activate this second Type=2 subproject. At `model/subproject.cpp:384-388`, Type 2 `SfDoeStudy` parsing also requires an owned study, pointer identity, and configured DOE I/O before invoking `readLazyStudy`.

### B121: smallest retained TLOC variable

The B121 post-action `DoeInputTable.XML` contains one `DoeVariable`:

- name `Transfer Time`, `varId=1`, `dimension=3`, `numSamples=3`;
- nested `Keyword`: name `TLOC`, `parmType=double`, `unitType=0`, `parameter=-1`, nominal value `0.1`;
- `NominalData` text `TLOC     \t0.1`;
- `KwOperations` maps operation id `1`;
- top-level `Operations` maps operation id `1` back to variable `Transfer Time`.

The retained project adds a Type=2 study as well. `temp.KWD` contains `TLOC     \t0.1\n`, tying the scalar nominal value to the root project keyword text. `DEF_DOE.INI` names `OBJ_NO 1` and `OPR_NO 5`; this is distinct from the input-table's operation map id 1 and must not be conflated. The PAR files carry mode-0 sample rows; the first sample rows in `DEF_DOE.PAR` and `Samples_LHS.par` are `1 0.55605`, `2 0.858065`, `3 0.420254` (tab-delimited with a trailing tab). The file additionally contains a row-count/header and 100 sample entries; hashes and exact lengths are recorded in `analysis/wave2-doe-evidence.json`.

Current `readInputTable` stops at the first nested `DoeVariable` child (`doe_study.cpp:384-386`); the direct Save path refuses any nonempty `Study::vars` in both `saveInputTable` and `saveInputTable`'s operation-index finalization seam. `doe::Var` currently carries geometry files/items/operation membership, but no recovered scalar keyword payload. Therefore a correct TLOC path needs a model representation for variable identity and scalar Keyword data, parsing of both reciprocal operation maps, and an explicit mapping to the `TLOC` keyword nominal value. Simply bypassing the guard or treating the retained XML as output bytes would not implement that behavior.

### B122: geometry extension, not a larger scalar case

B122's input table contains a different one-variable shape, `Workpiece-Geometry`, with nested `Geometry` fields (`GeoObject=1`, `GeoOperation=1`, `MorphMethod=100`, three named GEO files) and no scalar `Keyword` payload. Its after project also adds a `DOE Output` flow operation and three GEO files. It is a separate geometry owner/mapping/copy branch; do not infer that scalar TLOC parsing suffices for B122.

## Smallest source-only recovery sequence

1. Build a focused candidate-only model test from the immutable B120 after-state record: construct the Type=2 study owner, parse header-only input, and assert study name/default fields and the exact sample-load sequence. Keep B120's expected filesystem files as contextual hashes, not raw equality targets for the no-study before input.
2. Use the B121 after-state `DoeInputTable.XML` as the next single-variable unit input. Extend the model to represent the scalar TLOC keyword payload and both mappings, then test exact `varId`, dimension/sample count, keyword attributes, nominal value, operation id and variable name round-trip. Add an independent `temp.KWD` effect assertion for `TLOC` nominal value. Preserve current-source refusal behavior until all constituent branches are modeled.
3. Keep samples and writers separate: validate PAR parsing/count/mode and write ordering independently from the XML variable/mapping path. The existing `readLazyStudy` ordering is input XML → the three sample files → current `DEF_DOE.PAR`; failures and partial effects are observable and should not be collapsed into one success/failure condition.
4. Only after scalar TLOC works, use B122's geometry entry as a distinct follow-up for `GeoFile` ownership, operation/object mapping, three GEO payload copies and DOE Output serialization.

The smallest recoverable data target is thus **B120 empty Type 2 study activation followed by B121's one TLOC variable**. It is a source and candidate-test plan, not an original parity claim. No source/build/test/GUI action or mutable REA operation was performed; fresh original capture remains gated.

Evidence reviewed: B120/B121/B122 after folders under `evidence/gui_mo/oracle_b`, `analysis/wave2-doe-evidence.json`, `analysis/resume48-diff-b120.md`, `analysis/resume48-diff-b121.md`, `analysis/resume48-diff-b122.md`, `source/apps/moproj2mst/src/model/subproject.cpp`, `src/doe/activation.cpp`, `src/doe/doe_study.cpp`, `include/m2m/doe_study.hpp`, and `include/m2m/mst/doe_model.hpp`.
