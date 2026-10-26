# CP203 RGEOM converter geometry-dependency audit

## Result

No converter-only source change or focused test is source-ready. The exact missing witness is one existing, independently captured original geometry-list mesh occurrence that reaches the rectangle builder after the original getter path returns both coordinates and connectivity, with its owner/dimension and array lifetime qualified. The current port guard already preserves the known false/skip paths and refuses profiles whose getter effects or packed 2D builder contract remain unknown. A unit fixture that directly constructs `OBJ_MESH2D` does not supply this witness.

This is a bounded review of retained evidence and current converter code. It does not claim the original binary lacks such a producer or that no fixture exists elsewhere.

## Converter paths reviewed

`source/apps/moproj2mst/src/engine/viewer.cpp` keeps the optional geometry-list mesh separate from the object's main FE mesh. `refreshGeometryItem` first handles owner/view/item binding and geometry lookup, resets the cached item rectangle, and unions sampled edge curves. When an optional `geometry->mesh` exists and both node and element counts are positive, it calls `qualifiedGeometryReadiness` before the rectangle builder. A known `Skip` returns with the edge prefix; unqualified getter/read states mark the rectangle unknown and preserve the unsupported public scene-box guard. Only qualified populated/repeated/high-cached states proceed to `geometryMeshRect`.

The rectangle helper then uses the qualified getter route, checks positive counts, local 2D node layout and admitted element metadata, signed count bounds, nonnull coordinate/connectivity pointers, one-based connectivity range, and finite float-rounded XY coordinates. This is a guarded candidate contract. It does not prove the public Open/Save workflow creates or populates this optional geometry mesh.

`tests/mdt/test_viewer_box.cpp` includes direct helper/viewer tests such as `empty_and_populated_geometry_mesh_rectangles`, `cross_section_geometry_refresh_keeps_unqualified_mesh_refused`, and malformed-array/layout cases. Those tests establish candidate behavior for constructed states. They are not evidence that an original Open/Save run publishes the same nested owner, dimension, coordinate allocation, connectivity allocation, or getter outcomes.

## Original evidence and dependency boundary

The original target qualified in the retained records is DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

`analysis/resume53-geometry3d-ownership.md` and the later `analysis/resume200-geometry-optional-writer.md` distinguish two ownership levels: the geometry cross-section owns the optional `OBJ_MESH2D` at `+0x28`; this does not establish a model owner for its nested node set at mesh `+0x008+0x130`. Retained constructor/reset/copy paths establish null owner initialization and local dimension setup, and copy mechanics, but not a real GUI producer/read occurrence or exact original xrefs for a nonnull owner publication. The existing original consumer chain is geometry item refresh `0x14126d0c0` → readiness predicate `0x1408ee940` → rectangle builder `0x14124bec0`; readiness requires mesh, positive node/element counts, nonnull coordinate getter, and nonnull connectivity getter. The coordinate wrapper's owner-derived dimension dependency remains relevant when storage is absent.

The later retained qualification allows already-populated arrays on a null-owner path for its bounded profile, but does not make a positive synthetic state into a real project occurrence. The main object's node set and its recovered getter bodies are not substitutes for the optional geometry-list mesh's nested node set.

## Exact witness needed before implementation

One immutable, source-bound original occurrence must establish either:

1. The optional mesh's nested node-set `+0x130` owner store, matching `u138` dimension setup/value, incoming owner identity/lifetime, exact xrefs into the cross-section mesh at `+0x28`, and coordinate/connectivity getter returns; or
2. If the owner remains null, the exact creator/copy/read path and source identity showing positive counts plus preexisting coordinate and connectivity arrays published before those getters, with the same-profile successful builder entry.

The occurrence also needs an existing project fixture and independent literal expected effects. Without that, neither another isolated helper test nor a public Open/Save assertion can be grounded. If the witness establishes only the known rejection/skip branch, retain the current guard; do not widen it.

No source or test was edited. No build, test, GUI, original process, mutable REA database, fixture, or deployment was used or changed.
