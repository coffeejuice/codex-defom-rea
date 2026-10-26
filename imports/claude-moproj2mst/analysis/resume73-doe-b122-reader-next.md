# CP73: isolated B122 geometry reader admission

## Boundary from the B121 TLOC prefix

The current B121 prefix in `src/doe/doe_study.cpp` and `include/m2m/doe_study.hpp` is explicitly scalar-only: it accepts one `Transfer Time` / `TLOC` Keyword record and builds a `Var::scalar` payload. Its writer requires that scalar payload and retains the ordinary public `readInputTable` and `writeInputTable` refusals. B122 is a different variable family and must not be added as another conditional inside the TLOC parser/writer or represented by fabricated scalar fields.

The next implementation should use new isolated files, leaving TLOC's files alone:

- `source/apps/moproj2mst/src/doe/geometry_variable_prefix.hpp`
- `source/apps/moproj2mst/src/doe/geometry_variable_prefix.cpp`
- `source/apps/moproj2mst/tests/doe/test_b122_geometry_variable_prefix.cpp`

The private header/source should define a bounded `B122GeometryVariableRecord` and a pure parser that takes the `DoeVariables` and `Operations` child nodes and returns that record. It should not append a `Var` to `Study`, modify `readInputTable`, invoke `readLazyStudy`, perform file I/O, copy any GEO payload, or change public conversion/Save dispatch. This dedicated source/test seam avoids conflict with the current TLOC owner and avoids editing the shared `m2m/mst/doe_model.hpp` until the root assigns the common `Var` integration.

## One-record contract to retain

The parser target is the retained post-action B122 `DoeInputTable.XML` (817 bytes, SHA-256 `79d875540a7c5e84442e183be527f02fa3eb345e5c25900268b8478030c3bf9b`). It contains exactly one `DoeVariable`:

- identity/name `varId=1`, `Workpiece-Geometry`;
- `dimension=3` and `numSamples=3` as independent metadata;
- one `Geometry` record: `GeoObject=1`, `GeoOperation=1`, `MorphMethod=100`;
- `UpperDefined=false`, `LowerDefined=false`;
- three independent file-name values: nominal, upper, lower;
- empty upper/lower correspondence child elements;
- a separate top-level operation map `Operation id=1 -> DoeOprVariable name=Workpiece-Geometry`.

Do not merge `Geometry.GeoOperation` with the top-level operation binding just because both are 1. Do not omit named GEOs because the `*Defined` flags are false: the retained post-state contains all three named files. All three files are 41,410 bytes with SHA-256 `96f8cfaed97d25fedebd9fbe9e7c25127164700abe051aa78a6e84eedc67a526`; they begin with count 242 and numbered coordinate triples. These are retained payload receipts, not output to duplicate or a general rule that upper/nominal/lower variants are byte-identical.

## How the bounded record relates to current model and payload ownership

Current `doe::Var` in `include/m2m/mst/doe_model.hpp` has `name`, reindexed `index`, `number`, `geoType`, `GeoFile {path,before,after,useBefore,useAfter}`, geometry `items`, operation-id vector `ops`, and link. That shape does not represent the B122 dimensions/sample-count distinction, object selector, geometry-operation selector, morph method, definition flags, or correspondence payload. `Study::geometryVars(opId,objId)` in `doe_study.cpp` still refuses a nonempty variable list, so an isolated parser cannot be mistaken for a working geometry query.

`GeoFile` is the nearest existing file-record concept: `renameVarFiles` maps nominal path to `GeoFile.path`, upper to `.before`, and lower to `.after`. This provides a source-qualified candidate mapping for a later `Var` integration, but the new parser record should initially keep explicit named `nominalGeoFile`, `upperGeoFile`, and `lowerGeoFile` fields. That prevents field aliases from erasing the native XML labels before they are separately proven. It represents **path references only**; it does not own, generate, rename, or copy the file bytes.

The exact original sidefile producer remains the GUI geometry “+” action reported in `analysis/resume48-diff-b122.md`: three geometry variants already exist by the historical Save state. Their path/value parser is independent from reconstructing that user action. Do not claim that parsing the XML is sufficient to regenerate GEO contents or the other B122 effects (`.DS`, `.MST.TEMP`, `DEF_DOE.INI`, PAR/sample outputs, DOE Output operation, or Project DOM edits).

## Current guards and safe test shape

For post-state Project loading, `model/subproject.cpp::parseSubProject` first checks the actual owned Study aliases and `p.doeIo`; `doe::readLazyStudy` then calls public `readInputTable`. In `readInputTable`, the first B122 content guard is the nonempty `DoeVariables` child list, before its following `Operations` element; that branch throws `DoeVariables reader 0x14090ee20 has no oracle case yet`. If passed, nonempty top-level `Operations` is separately guarded by `0x140912a20`. On Save, `writeInputTable` rejects nonempty `Study::vars`, and `geometryVars` still refuses mapping. Keep each guard until its complete owner/query/effect contract is integrated.

The focused unit should feed only the retained XML bytes to the XML parser, call the new pure helper on the two relevant nodes, and assert all fields, empty correspondence lists, separate top-level and geometry operation ids, and all three exact file-name strings. Negative parser cases may validate bounded shape rejection (duplicate geometry child, missing file attribute, extra variable, malformed numeric field); label those as parser-mechanics tests, not original validation behavior. It should not open/read the three GEO files, mutate the fixture, use `Study`, or claim public B122 acceptance.

After this bounded record is stable, root should assign an explicit follow-on owner for shared `Var`/`Study::geometryVars` integration in `include/m2m/mst/doe_model.hpp` and `src/doe/doe_study.cpp`; those are shared with TLOC's model and writer changes and should not be edited in this isolated admission. The public reader/writer dispatch files remain a still later integration boundary.

## Evidence scope

The retained input and sidefile facts are recorded in `analysis/resume48-diff-b122.md` and `analysis/resume72-doe-b122-geometry-contract.md`. Current TLOC prefix is documented in `analysis/resume72-doe-tloc-model-contract.md`. Source inspected: `src/doe/doe_study.cpp`, `src/doe/activation.cpp`, `src/model/subproject.cpp`, `include/m2m/doe_study.hpp`, and `include/m2m/mst/doe_model.hpp`. This is a read-only design note; no source, build, tests, GUI, original execution, or mutable REA state changed.
