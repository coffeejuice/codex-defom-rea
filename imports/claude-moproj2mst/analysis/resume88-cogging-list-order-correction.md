# CP88 native list refresh/union ordering correction

Independent review compared the implementation to the retained
`source/docs/re/moproj2mst/mdt/V/spec_V.md` §3.1 contract: `0x1411f5540`
first calls `0x1411f5a90` to refresh the entire item list, then makes one
`0x1411f58e0` aggregate Box3 call. The first CP88 implementation interleaved
refresh and per-item union, which could leave earlier item boxes in
`view.box` if a later item was unsupported.

Corrected `recompute3D` to use distinct passes: refresh all items; if geometry
sync is pending, return before any item-box union; validate that every refreshed
item rectangle is known; then union all item rectangles. The pass remains
between the geometry-rep and FE-mesh-rep branches. Unknown positive mesh state
still throws the existing unsupported diagnostic.

Added `cross_section_geometry_refresh_validates_all_items_before_any_box_union`.
Its first item has a known nonempty edge rectangle; the later item's populated
synthetic geometry mesh has an unqualified owner. The test enables `ObjectOn`
and `CrossSectionGeoOn`, expects unsupported, and verifies the per-view box
remains empty after failure, proving that the earlier item was not aggregated
before the complete list passed validation. It also checks no geometry getter
dispatch and unchanged source arrays/owner field.

Only `viewer.cpp` and `test_viewer_box.cpp` changed in this correction. Static
preflight reports 45 unique Testkit cases, `main` present, no C `assert`, and no
trailing whitespace. SHA-256:

- `viewer.cpp`: `b7baa6c3046c5fc288a7f8b57ea80ced61314529d0b9d46ac2af885ba988da74`
- `test_viewer_box.cpp`: `89f8dfcd24337f27413f7fef7ee4740706281e12fe971c10af0b56dedc9ae05f`

No build or test was run; root owns validation. No source beyond the two
assigned files, CMake, GUI, or mutable analysis DB was changed or used.
