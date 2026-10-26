# CP141 released geometry source review

No blocking defect found in the released Viewer width-mismatch implementation or its focused test. It implements the accepted bounded no-copy branch, rechecks actual state on each object/getter round and preserves public/other unsupported paths. This is static acceptance for root's focused/full validation, not a build or runtime pass.

Only this report was written. No source/build/test/GUI/original execution or mutable REA access. Native qualification remains the independently verified ranges in `resume141-geometry-next-review.md`, SHA-256 `672d4a9cfc881606da95d9ec3d991fc40cd449ffdb9e9c505240840cee2a62c9`; original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Runtime guard and effect review

Compared `src/engine/viewer.cpp` with frozen CP140 `source.zip`. The only behavioral changes are new private `qualifiedGeometryNodeWidthMismatch` and its use for eligible nonzero main presence. Existing zero-presence behavior, reader call, timer Stop, public headers/interfaces, scene/layout/finite-data guards and deferred low-frame logic are unchanged.

The predicate requires actual null companion, dimension3, u1b58=0, high version/cache0, selected id11, positive presence and an existing coordinate pointer. Its22 selector pairs exactly match the plan:21 non-2D region gates plus width-selector70=151. The width comparison reads fixed `u1dc0[151]` only after those checks, so invalid live width selectors cannot index the table. Native dimension2-only and literal-region19 blocks are separately excluded. Equal widths, aliasing relevant selectors and negative presence refuse before the actual node helper.

`completeGeometryNodeSkip` retains actual MAR-selector bounds/stride loads before null/kind7/byte3610 skips. For an eligible object, state/flag and `object->owner == &step` precede the predicate; a foreign owner cannot reach its owner-dependent helper. The predicate is evaluated immediately before each actual `obj_read_node_arrays` call, on every eligible object and on both getter rounds. No saved success bit or borrowed report substitutes for current fields. The helper receives actual step tables and source arguments without new A/MAR pointer arithmetic, performs the qualified native mismatch/excluded-region path and returns. Node Stop still occurs only after the complete loop and its existing timer-count check.

The added branch explicitly checks null DB/high version, so calls through older common-helper sites retain their previous zero-presence restriction. No new frame, current-main, application-lifetime, Project teardown, accepted Close or public editor/import admission is introduced.

## Test owner and coverage review

`test_geometry_node_width_mismatch.cpp` constructs an actual `GuiModel`/step, a real main object from `llobj_ctor::get_or_create`, count1 and three-double coordinates through the real `bc::alloc_node_arrays`. The nonzero object is secondary and has no additional Viewer view; its existing coordinate allocation is owned by the real main object's node collection. Optional geometry allocations are owned by their normal mesh/node/element destructors. Null A/MAR are actual fixture facts, not forged spans.

The missing-coordinate and foreign-owner mutations occur only after Fixture construction. Its destructor restores the owned coordinate pointer and actual owner before model destruction, including Testkit abort paths. The test-only foreign model is destroyed earlier, but restoration does not dereference its address. Scoped hook and global table/timer destructors also restore state. No manually freed old pointer or second native free was introduced. The real node collection destructor uses its actual allocation ownership; the no-copy branch itself performs no free/reallocation.

All three cases are meaningful:16 positive profiles vary version boundaries, optional width0/4 and metadata2/3;16 refusal/restoration profiles exercise equal width, null allocation, two region aliases, invalid width-selector280/-1, negative presence and foreign actual owner; one dispatch callback changes equality only at the second getter. Assertions verify two real dispatches/arrays checkpoints on success, first-Stop then second-getter ordering, no Stop on refused phase, unknown scene and fresh successful retry, coordinates/counts/owner/current-step/version/cache and expected scene bounds. The second-round mutation test observes the first Stop before changing the live field, then requires the second reader to stop at its guard after its COW/read/arrays effects.

Coverage limits are nonblocking: the tests sample two region aliases rather than all20 later selectors, and do not separately corrupt selected selector11. I independently checked the entire exact selector array against the accepted native-derived list, so no missing guard was found. An alias sweep would strengthen regression coverage but is not a prerequisite inferred from this review.

Static preflight independently resolved all quoted and project/Testkit angle includes, confirmed the supported Testkit main/macros and three unique cases, and found no C assert/REQUIRE_EQ. I did not compile or execute them. Root targets are `m2m_mdt_geometry_node_width_mismatch` and regression `m2m_mdt_viewer_box`, with corresponding CTest names `m2m.mdt.geometry_node_width_mismatch` and `m2m.mdt.viewer_box`.

## Verified release identities

| File | SHA-256 |
| --- | --- |
|source/apps/moproj2mst/src/engine/viewer.cpp|`c0d43362c4959a56062f4f7c6150e1ee29c9502d8663e7b57733acf52f3bf1ab`|
|source/apps/moproj2mst/tests/mdt/test_geometry_node_width_mismatch.cpp|`faa3396d642e5d73b1a183dbc0371cca3e1f2c48cbe3cfe4e7f2244fb2712c7d`|
|Unchanged source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp|`b1740fd1ce2ea86b37e4b1dbea4974b468652c345578d81cfa13d8f583647d18`|
|analysis/resume141-geometry-node-width-mismatch.md implementation receipt|`8e79f9492df37fcfb5993e2c3024137979d368a942c190866f349b9f6542aca4`|

All match the released implementation receipt. Public behavior remains limited to the explicitly qualified geometry no-copy return; populated source assignments and unresolved lifecycle paths retain their guards.
