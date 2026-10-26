# CP141: actual main-node width mismatch completes the no-copy geometry reader

The private geometry reader now admits the native existing-allocation/LLCRZ width-mismatch return for nonzero main-node presence. It still runs the actual node helper and node Stop in the same invocation. The precise native branch, selector table and original byte hashes are in `resume141-geometry-next.md`.

Only `src/engine/viewer.cpp`, new `tests/mdt/test_geometry_node_width_mismatch.cpp` and this receipt were edited for implementation. Existing `test_viewer_box.cpp`, shared headers, public APIs and CMake were not changed. Root owns test-target registration and builds.

The new private predicate requires actual dimension3/null DB/u1b58=0/high version/cache0, exact region11, positive main presence, existing main coordinates and the exact22 relevant selector pairs. Only after confirming the width selector151 does it compare actual `step.u15f8[0]` with actual `step.u1dc0[151]`; equality refuses. It does not read or form any A/MAR pointer. The unchanged caller still performs the MAR-selector/stride input load before object skips, and enforces state/flag/current-step owner checks before this predicate.

Each eligible nonzero object is requalified on each getter round after the real COW/read/arrays prefix. An admitted object calls the existing `obj_read_node_arrays`: native table selection, no allocation because coordinates exist, the width-mismatch jump, excluded region tests, version read and return. After all eligible objects return, the existing single node Stop completes that reader. No bare success substitution, pointer copy, new frame, query, cache mutation, saved-step replay or generic continuation permission was added.

Zero-presence and native null/type7/byte3610 skips retain their previous paths. Low-query/non-null DB, missing main allocation, matching width, selector aliases, negative presence and foreign owners remain guarded. CP139/140 packed-XY, element metadata, width-zero/non-null readiness, finite coordinates and 3N bounds are unchanged. This remains private geometry support and releases no public editor/import guard.

The standalone Testkit file adds three cases and33 bounded profiles, with40 CHECK and22 REQUIRE callsites:

- Sixteen valid profiles combine four versions (6000,10999,11000,INT32_MAX), optional widths0/4 and element metadata2/3. A real secondary main object is created by `llobj_ctor::get_or_create`, with count1 and main coordinates allocated by `bc::alloc_node_arrays` under its actual step. It gets no separate Viewer view. Null A/MAR remain unused; both getter rounds, first-Stop/second-getter timing, two arrays checkpoints, scene bounds, owner/count/content/version/cache and current step are checked.
- Sixteen refusal/restoration profiles combine optional widths0/4 with equal widths, missing main coordinates, two selector aliases, positive/negative out-of-range width selectors, negative presence and an actual foreign step owner. They require one COW/read/arrays prefix with no Stop and unknown scene, then restore actual fields and require a fresh successful two-round contribution.
- One declared callback mutation changes stored width to equal D[0] at the second coordinate dispatch. It requires first Stop to remain, second COW/read/arrays effects to remain, second Stop to be withheld, and a fresh two-round success after restoration. No original process is instrumented.

Fixture destructors repair declared owner/pointer mutations and reset optional nonowning owner fields before actual model destruction. Process-global table/timer state and dispatcher hooks are restored by local destructors. Main and optional allocations use their normal owning destructors.

Static preflight passed: balanced delimiters after comments/literals were stripped, project include existence, trailing whitespace, unique case names, one Testkit main, no assert macro and one outer argument for every CHECK/REQUIRE. The actual allocator, Viewer, typed object constructor and const raw-access declarations were checked. This is structural/declaration inspection, not compilation or runtime verification.

No build/test execution, GUI/original launch or mutable REA occurred. Suggested root-owned target is `m2m_mdt_geometry_node_width_mismatch`, focused CTest `m2m.mdt.geometry_node_width_mismatch`; also retain `m2m_mdt_viewer_box` / `m2m.mdt.viewer_box` regression validation. No test pass is claimed here.

Frozen files:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `c0d43362c4959a56062f4f7c6150e1ee29c9502d8663e7b57733acf52f3bf1ab` |
| `source/apps/moproj2mst/tests/mdt/test_geometry_node_width_mismatch.cpp` | `faa3396d642e5d73b1a183dbc0371cca3e1f2c48cbe3cfe4e7f2244fb2712c7d` |
| Unchanged `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `b1740fd1ce2ea86b37e4b1dbea4974b468652c345578d81cfa13d8f583647d18` |
| `analysis/resume141-geometry-next.md` | `387d5172af7165a04d569f8888060ca29c9df7d0148ed89c885f91cfd5b74d9f` |
