# CP75: next isolated DOE writer prefix after B121 TLOC

## Recommendation

The next independently testable writer increment is a **separate B122 geometry-record fixture serializer**, consuming the already bounded `B122GeometryVariableRecord` or its existing `Study`-owned `Var::geometry` binding and producing only `DoeInputTable.XML` in `FileEffects`. Keep it out of `writeInputTable`, `saveInputTable`, and Type 2 conversion dispatch. Do not fold Geometry fields into `writeB121TlocModelPrefix`: the B121 scalar `Keyword/NominalData/KwOperations` row and B122 geometry row are distinct record families.

This is the smallest source-closed writer projection available from the current state: the parser/binder already provide the exact B122 metadata, all three filename references, and the separate top-level `Var::ops` association. A dedicated helper can reject every non-B122 model/header shape before appending an effect, emit one fixed B122 document, and compare its planned bytes directly to the retained 817-byte fixture. It should add exactly one requested XML write and no GEO, PAR, temp.KWD, refresh, directory, or project-DOM effects. Its success means planning only, not QFile success or native Save completion.

## Evidence and exact bounded output

Retained B122 input: `evidence/gui_mo/oracle_b/122_B_DOE_GEO/DoeStudies/DoeStudy1/Control/DoeInputTable.XML`, 817 bytes, SHA-256 `79D875540A7C5E84442E183BE527F02FA3EB345E5C25900268B8478030C3BF9B`. The literal fixture provides the exact observed element/attribute order for this one model row:

- standard empty header fields and `DoeVariables`;
- `DoeVariable` attributes in the observed `numSamples`, `varId`, `dimension`, `name` order;
- one `Geometry` child with `GeoObject`, `UpperDefined`, `LowerDefined`, `GeoOperation`, `MorphMethod`, then `UpperGeoFile`, `LowerGeoFile`, `NominalGeoFile` as represented in the retained bytes;
- empty `UpperCorrespondence` then `LowerCorrespondence` children;
- later top-level `Operations/Operation id=1/DoeOprVariable name=Workpiece-Geometry`.

The 100%-bounded values and current parser/binder shape are recorded in `analysis/resume72-doe-b122-geometry-contract.md`, `analysis/resume73-b122-parser-static-review.md`, and `analysis/resume74-doe-b122-model-binding.md`. Existing B121 evidence (`analysis/resume73-doe-tloc-writer-evidence.md`) establishes the common outer document sequence and warns that QDom output attribute order must not be inferred from native setter-call order. So this serializer must call its ordering a retained-fixture byte projection, even though original d0.c includes the geometry serializer branch; this one record cannot establish general attribute-map behavior, optional metadata branches, multiple variables, geometry correspondence contents, or generic geometry naming.

The three named GEO paths remain references only. Their three payload files are real retained effects from the preceding GUI edit action (the retained report says each is 41,410 bytes and same hash); a serializer must not create/copy them or interpret `UpperDefined=false` / `LowerDefined=false` as removing their path references.

## Suggested focused test

Add a dedicated `tests/doe/test_b122_geometry_variable_writer_prefix.cpp` (or equivalent isolated file) that:

1. Reads the actual retained B122 fixture in binary mode through `M2M_ORACLE_B`, parses the two relevant sections with `parseB122GeometryVariablePrefix`, and binds into a fresh `Study` via `bindB122GeometryVariablePrefix`.
2. Calls only the new isolated helper; asserts one in-memory write to a synthetic path, no non-XML effects, and direct byte equality against the independently read 817-byte artifact (with its known length/hash receipt). Do not construct the expected artifact from a duplicated XML literal or normalize CRLF.
3. Mutates one admitted header/payload field at a time and verifies rejection occurs before effects. Keep parser/binder mechanics tests separate from writer bytes.
4. Keeps public `readInputTable`/`writeInputTable` guard tests intact and does not call `geometryVars`, project activation, or public Save.

Candidate-only caveat: one raw fixture pass would establish exact bytes for this admitted B122 record, not native behavior on a fresh Save. Public B122 still needs the original owner/query, geometry file ownership/copy, ordered effects, and DoeOutput/process-global contracts. The existing public empty-variable gates remain in `doe_study.cpp`; do not remove them as part of this slice.

## Static review scope

I inspected the retained B121/B122 fixture receipts, current isolated B121 writer, B122 parser and Study binder, current public writer refusal, and the B122 source/test contracts. No source/build/test/GUI/mutable REA actions were performed. This note is the only written file.
