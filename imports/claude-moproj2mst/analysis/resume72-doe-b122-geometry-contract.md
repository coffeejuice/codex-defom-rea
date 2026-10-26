# CP72: B122 geometry variable and GEO payload contract

## Retained post-action record

The retained B122 case is an **after-action** Save result, not a no-op Open→Save baseline. The corresponding `122_B_DOE_GEO_before/COG_P1_B1.moproj` has five operations, no DOE subproject or study, and no DOE files. The after-state adds a DOE Output operation (id 6), a Type2 `SfDoeStudy` named `DoeStudy1`, and the `Workpiece-Geometry` entry below. The original case notes say the user first added the study and used the geometry “+” action, which copied the three GEO variants before Save. A converter given only the before-state cannot be expected to synthesize those actions or files.

The after-state input file is `evidence/gui_mo/oracle_b/122_B_DOE_GEO/DoeStudies/DoeStudy1/Control/DoeInputTable.XML` (817 bytes; SHA-256 `79d875540a7c5e84442e183be527f02fa3eb345e5c25900268b8478030c3bf9b`). Its single variable records:

| Field | Value |
| --- | --- |
| `DoeVariable.name` | `Workpiece-Geometry` |
| `varId` | `1` |
| `dimension` / `numSamples` | `3` / `3` |
| `Geometry.GeoObject` | `1` |
| `Geometry.GeoOperation` | `1` |
| `Geometry.MorphMethod` | `100` |
| `UpperDefined` / `LowerDefined` | `false` / `false` |
| `NominalGeoFile` | `Workpiece-Geometry_Workpiece_1_Nominal.GEO` |
| `UpperGeoFile` / `LowerGeoFile` | corresponding `_Upper.GEO` / `_Lower.GEO` names |
| correspondence children | empty `UpperCorrespondence` and `LowerCorrespondence` |

The separate top-level `<Operations><Operation id="1"><DoeOprVariable name="Workpiece-Geometry"/></Operation></Operations>` maps the variable to operation 1. Preserve it separately from `Geometry.GeoOperation=1`; the fact that both values are 1 here does not prove they are the same relationship.

All three retained GEO payloads are 41,410 bytes with the same SHA-256 `96f8cfaed97d25fedebd9fbe9e7c25127164700abe051aa78a6e84eedc67a526`. Each begins with node count `242`, followed by numbered coordinate triples. This proves the three recorded paths exist and their retained bytes match; it does not establish that every B122 geometry should produce identical variants or explain the upstream geometry-edit/copy procedure. The false `UpperDefined`/`LowerDefined` flags coexist with all three files, so those flags must not be interpreted as permission to omit the observed files. No `.COR` correspondence payload is present in this folder listing.

The comparator report `analysis/resume48-diff-b122.md` records that the historical original added the `DoeInputTable.XML`, three `.GEO` files, sample/current PAR, DEF_DOE.INI, parser INI, `.DS`, `.MST.TEMP`, and DOE Output side effects. It also identifies the GUI study/geometry actions preceding Save. It is therefore wrong to treat the ten-file diff against the before-folder as a single converter Open/Save serializer defect.

## Current candidate reachability and guard

For a post-state Type2 `SfDoeStudy`, `model/subproject.cpp::parseSubProject` first requires `ownedDoeStudy`, pointer identity `sub.doe == sub.ownedDoeStudy.get()`, and configured `p.doeIo`; otherwise it raises `M2M_E_UNSUPPORTED` with “DOE lazy parsing requires owned study and configured I/O.” Once the owner exists, `doe::readLazyStudy` reads `DoeInputTable.XML`. `doe::readInputTable` iterates children in document order and refuses at the first child element of `DoeVariables` with `DoeVariables reader 0x14090ee20 has no oracle case yet`. Thus this B122 record does not reach its following `Operations` content in the current candidate. If that first guard were passed, nonempty `Operations` is independently guarded by `0x140912a20`.

On a future Save, `writeInputTable` currently refuses any nonempty `Study::vars` before document serialization; `Study::geometryVars` refuses geometry mapping whenever variables are present (`0x14091ba60`). Existing model types in `include/m2m/mst/doe_model.hpp` hold `Var.name/index/number/geoType`, a vector of `GeoFile {path,before,after,useBefore,useAfter}`, geometry `items`, top-level operation ids `ops`, and `link`. They do not currently retain the B122 `varId`, `dimension`, `numSamples`, `GeoObject`, `GeoOperation`, `MorphMethod`, defined flags, or correspondence elements. In particular, `Study::geometryVars(opId,objId)` cannot implement the required object filter from the current Var fields.

The geometry source also has side-effect semantics beyond parsing: retained B122 has three named payloads, while current `renameVarFiles` and `GeoFile` represent file path/before/after slots and rename effects. Those fields are plausible storage for the nominal/upper/lower paths, but B122 alone does not close exact edit-time source ownership, generation, copying, or Save ordering. `DEF_DOE.INI` includes run-path entries and the rest of the DOE output set has separate owners; a geometry-variable reader is not a complete B122 Save implementation.

## Smallest isolated model/test seam

Keep public Type2 activation and both reader guards unchanged. The smallest useful next target is a pure one-record geometry-variable parser/model projection, testable directly from the retained `<DoeVariable>` element without calling `parseSubProject`, `readLazyStudy`, Save, or file-copy code. It should retain:

- variable identity/name (`varId=1`, `Workpiece-Geometry`), `dimension=3`, and `numSamples=3` as distinct fields;
- geometry owner selectors `GeoObject=1`, `GeoOperation=1`, and `MorphMethod=100`;
- nominal/upper/lower path strings and independent `UpperDefined=false` / `LowerDefined=false` values;
- empty correspondence children as empty records, not synthesized `.COR` files;
- the top-level operation-1 name mapping as a distinct relationship from the Geometry child.

A focused test under `tests/doe` can parse only this fixture record and assert those retained values, path spelling, and separate operation mapping. Do not infer sample row count from `numSamples`, copy any retained evidence into a generated output folder, or claim the `.GEO` bytes were regenerated. Keep source data and oracle fixture read-only. A later separately qualified step can bind the record to `Var`/`GeoFile`, implement `geometryVars(opId,objId)` for object 1, and then investigate file-owner/copy order; this minimal parser seam alone must not clear the public B122 guard.

## Evidence and limits

Primary retained static comparison: `analysis/resume48-diff-b122.md`; original sample context: `analysis/wave2-doe-findings.md`; exact historical XML and GEO files are listed above. Current source inspected: `src/model/subproject.cpp`, `src/doe/activation.cpp`, `src/doe/doe_study.cpp`, `include/m2m/doe_study.hpp`, and `include/m2m/mst/doe_model.hpp`. This is a read-only source/fixture audit. No source, build, tests, GUI, original execution, or mutable REA state changed. The report does not establish fresh original runtime, complete geometry editor actions, full file generation semantics, or complete active DOE Save.
