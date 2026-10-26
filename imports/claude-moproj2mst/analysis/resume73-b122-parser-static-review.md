# CP73 — Static review of the isolated B122 geometry parser

## Assessment

The new parser is a deliberately narrow value projection of the retained post-action B122 XML. Its field mapping and three GEO filename references match the fixture, and it is isolated from `Study`, the public input-table reader/writer, geometry queries, and GEO I/O. No public B122 behavior is enabled by this change.

The main limit is scope: it is a one-fixture recognizer, not a general DOE geometry parser or a full-input XML validator. Several strictness branches are implemented but not individually covered by the focused malformed cases. Keep the existing public guards.

## Fixture and field mapping

The checked-in retained input is `evidence/gui_mo/oracle_b/122_B_DOE_GEO/DoeStudies/DoeStudy1/Control/DoeInputTable.XML` (817 bytes, SHA-256 `79d875540a7c5e84442e183be527f02fa3eb345e5c25900268b8478030c3bf9b`). It has exactly one `<DoeVariable>` named `Workpiece-Geometry` (`varId=1`, `dimension=3`, `numSamples=3`), one `<Geometry>` with `GeoObject=1`, `GeoOperation=1`, `MorphMethod=100`, false upper/lower flags, three named GEO references, and ordered empty Upper/Lower correspondence elements. It separately has top-level operation id 1 bound by name to the variable.

`src/doe/geometry_variable_prefix.cpp:46-106` reads those values from the respective nodes and stores `geoOperation` and `operationBindingId` in separate fields. This preserves the distinction in structure; both values happen to equal 1 in the only retained record, so the fixture alone cannot demonstrate a differing pair. The three filenames match the retained files in the same `Control` directory, each 41,410 bytes and SHA-256 `96f8cfaed97d25fedebd9fbe9e7c25127164700abe051aa78a6e84eedc67a526` per `analysis/resume72-doe-b122-geometry-contract.md`.

The parser returns these filenames as XML references only. It does not resolve paths, open or copy the GEO files, parse their geometry contents, or assert that identical retained payloads are a general B122 rule. The positive test checks that the referenced files exist at the known fixture location and have the expected size; it does not hash/read the payloads. That matches the helper's stated boundary, though the test name's phrase “reads ... files” should be read as checking references/existence/size, not parsing bytes.

## Strictness and malformed rejection

The helper checks the two supplied nodes' element tags, one variable and one Geometry child, required attributes, fixed retained values/names, absent `LinkTo`, false flags, ordered empty correspondence elements, one top-level Operation, one `DoeOprVariable`, operation id 1, and matching binding name. It throws `M2M_E_UNSUPPORTED` for unsupported shapes. Numeric parse failures are checked via `qt::toInt(..., &ok)`; required strings must exist and be nonempty.

The focused test covers duplicate variables, a missing nominal GEO path, mismatched operation-binding name, and an unexpected Geometry child. Other explicit rejection branches (wrong/missing numeric fields, wrong flag values, missing upper/lower filenames, correspondence order/content, top-level operation count/id, `LinkTo`, wrong input node tags) are not each exercised. These are test coverage opportunities, not evidence that the retained fixture is parsed incorrectly.

Some structural details are intentionally bounded rather than canonicalized: the helper receives `DoeVariables` and `Operations` directly, so it does not validate the document doctype/root/header or guarantee that the two nodes came from the same XML document. `childElements()` counts only element children; unknown attributes and non-element siblings outside the correspondence nodes are ignored. `qt::toInt` compares parsed values rather than lexical spellings. These choices are acceptable for the current direct-fixture value projection, but should not be described as validating all native input variants.

## Separation from public Study and GEO behavior

Repository search shows `parseB122GeometryVariablePrefix` is referenced only by its declaration/definition and `tests/doe/test_b122_geometry_variable_prefix.cpp`. It is not called by `readInputTable`, `readLazyStudy`, `parseSubProject`, `Study`, `Var`, Save, or conversion dispatch.

The ordinary path remains guarded: `readInputTable` in `src/doe/doe_study.cpp` rejects nonempty `DoeVariables` at the variable reader boundary; `writeInputTable` rejects nonempty `Study::vars`; and `Study::geometryVars` rejects mapping when variables are present. The earlier owner/I/O checks in `parseSubProject` and the lazy study path remain separate. The helper does not append to `Study::vars`, mutate the shared B121 `Var` model, implement `geometryVars(opId,objId)`, generate `.GEO` payloads, or replay the GUI geometry “+” action documented in the retained B122 contract.

## Review disposition

The implementation is appropriately isolated and fixture-accurate for its stated purpose. Treat the parser as a strict recognizer for this one retained B122 row, not as general XML acceptance or evidence of round-trip support. No source edits, builds/tests, GUI actions, or mutable REA operations were performed during this review.
