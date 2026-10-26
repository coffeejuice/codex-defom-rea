# CP140: zero-width populated geometry contributes its XY rectangle

The private geometry rectangle builder now admits width zero after the existing populated readiness and both getter rounds. It still requires nonnull coordinate/connectivity returns, packed XY node metadata2, element metadata2/3, positive counts, the native 3N no-wrap bound and finite double/float coordinates. Negative widths remain unsupported.

The production change in `src/engine/viewer.cpp` is confined to `geometryMeshRect`: reject `width < 0`, and evaluate the element-count overflow division only when `width > 0`. Width zero therefore forms the defined product0, consumes no connectivity entries, and computes the current rectangle from all node coordinates. This follows `resume140-geometry-next.md`'s original loop, vector zero-resize and resolved rectangle-finalizer evidence. No native private vector owner, renderer state or retained-coordinate lifetime is invented by this projection.

Getter count/count/width ordering, CP137 repeated null-companion rounds, exact-frame retirement and owner/DB/cache/generation barriers are unchanged. Null connectivity still returns the earlier readiness skip before builder width/layout checks. Later builder refusal retains completed COW/reader/timer/edge-prefix effects and leaves the item unknown; it does not union a partial mesh rectangle.

Three new Testkit cases add 26 bounded profiles (44 CHECK and10 REQUIRE callsites):

- Six ownerless/cached/actual-null and element metadata2/3 profiles perform width0 success with invalid connectivity contents, positive-width refusal on those same contents, restored positive-width success with changed XY height, then a fresh width0 success with changed height and an INT32_MIN connectivity sentinel. Getter trace grows by exactly two per refresh; null readers retain Stop-before-second-getter ordering and eight arrays checkpoints over four refreshes. Owners, arrays, counts, metadata, version/cache and current step are checked.
- Fourteen ownerless/actual-null refusal/restoration profiles keep N dimension3, unsupported E metadata, negative width, NaN, infinity, double-to-float overflow and excessive 3N count guarded even at width zero. Both getter rounds precede refusal; restoring the real fields admits a fresh width-zero rectangle. The large declared node count has a local destructor restoring the allocated count before fixture destruction, as in CP139.
- Six ownerless/cached/actual-null and metadata2/3 profiles keep null connectivity at one getter round and edge-only bounds. Restoring the same allocated pointer then permits two more getter rounds and the mesh height1 contribution. Null-reader checkpoint totals distinguish one plus two rounds from replay or an extra getter.

The two older malformed-width cases now use -1 instead of0, preserving their unsupported-width purpose. The existing null-connectivity+width0 readiness test remains unchanged. No expected refusal was simply removed.

Static preflight passed for balanced delimiters after comments/literals were stripped, project include existence, trailing whitespace, unique case names and Testkit main. All54 new CHECK/REQUIRE macros have one argument at their outer parenthesis level. There are56 Testkit cases total, one `main`/`testkit::run_all`, and no `assert` macro. These are structural checks, not compilation or runtime validation.

No build/test execution, shared header/CMake edit, GUI, original launch or mutable REA operation occurred. Root owns target `m2m_mdt_viewer_box`, focused CTest `m2m.mdt.viewer_box`, full validation and archive. This receipt claims no test pass or newly recovered public file-loader path.

Frozen files:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `45107ea75fca2394a3fb50444fd8807825d7ff75e1c8adaf4c486186774975df` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `b1740fd1ce2ea86b37e4b1dbea4974b468652c345578d81cfa13d8f583647d18` |
| `analysis/resume140-geometry-next.md` | `d1baaa57d95366fd2e4fc1cc2b4ddfb3caffa84613efa55b45bf98fa2d9741d1` |
