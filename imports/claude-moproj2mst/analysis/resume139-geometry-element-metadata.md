# CP139: populated packed-XY geometry with element metadata3

The existing private Viewer mesh rectangle now admits actual element metadata `E.u0f8 == 3` alongside2 while retaining node packed-XY qualification `N.u138 == 2`. This is the source-qualified flat-array builder/rectangle cut in `resume139-geometry-next.md`; it is not a new 3D-coordinate layout or a public loader completion claim. Positive element width remains generic and checked, independently of the metadata value.

The only production change is in `src/engine/viewer.cpp`'s private `geometryMeshRect`: the element metadata guard admits2/3, and the supported node count is bounded by `INT32_MAX/3`. The latter preserves the native initializer's 32-bit 3N float-vector allocation (`14124c270`) and finalizer triple stride (`14124c41c`) without signed overflow. The existing positive width/connectivity product, index, pointer, finite-double/float and owner guards remain.

Both checks occur after count/count/width snapshots and the existing second coordinate/connectivity getter. Null-companion populated profiles still perform exactly two real COW/read/timer rounds; no third legacy getter is added. Later builder refusal preserves prior reader/timer and edge-prefix effects, and keeps the item unknown. CP134 exact installed-frame retirement and all DB/nonzero-assignment barriers are unchanged.

`tests/mdt/test_viewer_box.cpp` adds three Testkit cases (41 CHECK and8 REQUIRE callsites):

- 12 valid profiles: metadata2/3, widths3/4 and ownerless/cached/actual-null owners. They assert the same mesh/edge bounds, two getter dispatches, COW detach/reuse, current owner identity, unchanged array contents/metadata and the null-reader ordering/timers.
- 16 refusal/restoration profiles: ownerless/actual-null, N dimension3, unsupported E metadata0/-1, width0, invalid index, NaN, infinity and double-to-float overflow. Both getter rounds precede refusal; restoring the source fields admits a fresh two-round contribution.
- Four triple-count refusal/restoration profiles: metadata2/3 and ownerless/actual-null. A small allocated mesh declares `INT32_MAX/3+1` nodes (still below the old /2 limit), completes both getters and refuses before coordinate traversal. A local destructor restores the real count even if the case throws. The canonical key0 registry contains no id0, so the COW dispatch does not allocate variable values from that large count; existing coordinates are nonnull and main-object zero-node guards avoid node copies. Restoring count4 then rebuilds the expected contribution.

Static preflight only: both edited translation units have balanced delimiters after comment/literal stripping and no trailing whitespace; all project include paths resolve through the source include roots; test names are unique (53 cases total), with one `main` calling Testkit and no `assert` macro. This checks file structure and include existence, not C++ compilation or runtime behavior. A preliminary include scanner wrongly restricted quoted headers to the translation unit directory; the corrected scanner includes configured project include roots and reports no missing paths.

No build/test execution, shared header/CMake edit, GUI, original launch or mutable REA operation occurred. Root owns target `m2m_mdt_viewer_box`, focused CTest `m2m.mdt.viewer_box`, full validation and archive. No validation pass is claimed by this receipt.

Frozen source hashes:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `b35f4e69d675e6029a30ccad2eba010b7418cf6c0bd01c27e6141c3eb7af247d` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `b4d35808ab915c836d76a4caa30df47601bffa8ab7e24bc3b96378ed78dafd02` |
| `analysis/resume139-geometry-next.md` | `35d448aae683a01ee3d688a831ab90a18e09742515cda42ce53bc5b0e8064c64` |
