# CP74: guarded B122 geometry record binding into Study/Var

## Smallest safe state prefix

The CP73 parser's returned `B122GeometryVariableRecord` can join an isolated `Study` through the same basic owner/list shape already used by the B121 TLOC prefix: a `Study`-owned `unique_ptr<Var>` plus a nonowning pointer in `Model::vars`. It must use a **separate geometry payload and owner collection**. Do not place this record in `Var::scalar`, reuse `scalarPrefixVariableOwners`, or route it through `readInputTable`.

A minimal candidate binding would construct one `Var` with `name=Workpiece-Geometry`, `number=variableId (1)`, and `index=-1` pending the existing Model reindex operation. Store `dimension`, `sampleCount`, `GeoObject`, `GeoOperation`, `MorphMethod`, defined flags, and the three explicit file-name references in a new typed optional geometry payload on `Var`. Store the separate top-level `Operation id=1 -> DoeOprVariable name` relationship in the existing `Var::ops` list. Keep `geometry.GeoOperation` in its own geometry payload field even though it is 1 in this fixture; do not collapse it into the top-level `ops` association.

The binding owns the new `Var` in a dedicated Study-side vector, for example `geometryPrefixVariableOwners`, and only then appends its pointer to `Study::vars`. Requiring a fresh empty Study/owner/list before binding prevents duplicate or mixed-prefix ownership. The helper should return a bounded binding receipt or the owner pointer; it should not select a Project/SUB, search the filesystem, or create directories.

## Guarded effects and what remains unsupported

This is a model-state prefix only. It should **not** create `GeoFile` entries in `Var::files`, call `geoPath`, rename/copy files, or write any GEO bytes. The XML's three paths are references; the retained case's three 41,410-byte files were created by the original geometry edit action before Save. The mapping between `GeoFile.path/before/after`, edit-time geometry source objects and the three Save outputs still needs independent ownership/order evidence. The `UpperDefined=false` and `LowerDefined=false` attributes do not erase those named path references.

It should leave these existing guards untouched:

- `model/subproject.cpp::parseSubProject` ownership/I/O check before public lazy Study parsing;
- `doe::readInputTable` refusal on nonempty `DoeVariables`, before B122 reaches the later `Operations` guard;
- public `writeInputTable` / `saveInputTable` refusal on nonempty `Study::vars`;
- `Study::geometryVars(opId,objId)` refusal on nonempty variables.

Generic `Model::count`, `varAt`, `allVars`, and `varsForOperation` can be tested against the bound one-record model state; the top-level `Var::ops={1}` is the relationship those existing queries use. Do not call `geometryVars` successfully or filter by `GeoObject` until the native object/operation query body is recovered. A passing value parser or generic operation query does not prove geometry dispatch, GEO ownership, or public B122 acceptance.

## Focused test boundary

The next bounded integration test should start from the CP73 pure parsed record, bind it to a fresh `Study`, and assert exactly one owned `Var`, stable alias identity (`Study::vars[0] == geometryPrefixVariableOwners[0].get()`), all B122 metadata/path fields, `count/varAt/varsForOperation(1)`, and no mapping for another top-level operation. It should also assert that no `Var::files`, FileEffects, directory attempts, GEO reads/writes, InputTable XML writes, or public activation calls occur. A fresh empty Study may refuse a second binding without mutation. These assertions cover only candidate model ownership and projection.

Do not combine this with the TLOC writer prefix: B121's scalar value and nested Keyword/KwOperations model is a separate payload. The current TLOC helpers in `doe_study.cpp/.hpp` should remain unchanged by this geometry binding proposal.

## Exact next source ownership

If root authorizes the binding increment, its shared model edits belong in:

- `include/m2m/mst/doe_model.hpp`: a dedicated typed `GeometryVariable` payload and `Var::geometry` optional, separate from `Var::scalar` and the existing `GeoFile` file records.
- `include/m2m/doe_study.hpp`: a separate Study owner vector for geometry-prefix Vars.
- New `src/doe/geometry_variable_binding.hpp/.cpp`: isolated checked bind from CP73 value record to one Study-owned Var/list entry; no edits to public reader, writer, or conversion dispatch.
- New `tests/doe/test_b122_geometry_variable_binding.cpp`: direct model ownership/query test, without GUI or file effects.

Because the first two headers are also shared with the TLOC model implementation, root should explicitly assign those changes before implementation. The CP73 parser stays an independent source file and remains usable without this binding. `src/doe/doe_study.cpp` should not be the next B122 owner: it currently owns the scalar TLOC prefix and public empty-variable reader/writer seams, and a separate binder avoids mixing the two record families or accidentally routing geometry through those guards.

## Evidence limits

Current ownership pattern inspected: `Study::scalarPrefixVariableOwners`, `readB121TlocModelPrefix`, and its isolated writer in `src/doe/doe_study.cpp`; current `Var` and generic `Model` declarations in `include/m2m/mst/doe_model.hpp`; `Study::geometryVars` and public read/write guards; CP73 B122 value record and `analysis/resume72-doe-b122-geometry-contract.md`. This is a read-only integration design. No source, build, test, GUI, original execution, or mutable REA state changed.
