# CP141: nonzero main-node state can complete a qualified no-copy reader

The next source-closed cut is a bounded **high-version/cache0/null-companion LLCRZ width-mismatch** branch. An actual main object with nonzero node-presence state and an existing coordinate allocation can traverse the native node-assignment helper without copying any A data when the selected coordinate widths differ. Exact relevant region selectors must exclude every other assignment block. This would extend the existing integrated geometry getter/scene path, without inventing A/MAR ownership or adding a standalone state helper.

This read-only lane wrote only this report. No source/build/test execution, GUI/original launch, mutable REA, or shared analysis database operation occurred. Curve projection already handles the recovered point/type1 and segment/type2 branches (`viewer2d.cpp:324`); this scan found no smaller missing curve type behavior. Negative widths, unsupported node storage, malformed positive-width connectivity and nonfinite builder data remain guarded.

## Bounded original evidence

The immutable original is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Offline DUMPBIN and retained port qualifications were used; output was restricted to entry/width skip, region-compare summaries and special tail edges.

| Original half-open byte range | Meaning | SHA-256 |
| --- | --- | --- |
| `[1408421d0,140843af4)` | Complete native node-array helper | `223273ef5b5572cc166c6c3a9b06c33e8704d8f560b4378d393f87aebc425253` |
| `[140842239,140842354)` | Existing-allocation and selected-width branch | `dddee711ef70f9c3501d2de7ab9ead32bfac7748205620e0ddca541645a3a90e` |
| `[14084316c,1408431ac)` | 3D literal-region19 skip | `4dc42138fcc0ce60fe6996a612f3af97e114ebd64777c5a87c557bc643b1a082` |
| `[140843846,140843af4)` | Version/two remaining regions/return | `229326d1ce968e7e3c261bdc9c4456309b23787c969ceaeee149a87811ae4ead` |

Native `1408421d0` corresponds to ported `def::dbio::dbread::obj_read_node_arrays` in `source/port/defdbio/src/dbread/w2_objarrays.cpp:128`. It selects index tables from the object's actual owner first. Zero `object+87c` returns at `140842234`. Nonzero reaches `842239`, tests coordinate pointer `object+8a8`; nonnull skips allocation call `140837080` at `84225b` and reaches `842260`.

For exact selected region11 (not -1), the LLCRZ selector global `143dbdbdc` must equal11. The start-offset -1 adjustment is skipped. `8422c7` reads width-selector global `143dbdcc8`; with value151 it compares actual `D[0]` to actual `a_tab[151]` at `8422e1`. Unequal jumps directly to `842354`, before coordinate source-address formation at `8422e5..842312`, count multiplication or memcpy. Existing allocated coordinates are not touched.

All remaining region tests exclude11 under this exact relevant profile. In `g_a_index[slot] = value` notation:

```text
11=11, 70=151,
12=12, 13=13, 14=15, 15=16, 16=17, 17=18, 18=20,
19=21, 20=22, 21=23, 22=24, 23=25, 24=26, 25=27,
27=29, 32=34, 35=38, 42=46, 55=58, 57=61
```

These are the relevant values from `source/port/defmodel/src/step/index_tables.cpp:261`'s 3D profile; a proposed admission must check the actual live globals rather than assume the initializer ran or that the global dimension marker guarantees intact tables. A bounded native scan matched the corresponding22 region-compare sites from `842290` through `8439c6`. The native 2D-only block tests owner dimension at `842995` and skips to `842a9d` for dimension3. The 3D block at `84317b` additionally compares literal region0x13 at `8431a3`; selected11 skips it at `8431a6`. The remaining version getter `140825420` at `843855` only decides whether to test the final selectors58/61; both exclude11. The epilogue is `843aef..843af3`.

The complete-body disassembly has no reads of saved MAR and MAR-table arguments at post-prologue stack offsets88/98, agreeing with the port's unused parameters. Thus the caller's wrapping32 MAR record address remains dead in this branch as in the previous zero-node branch. Keep the existing actual MAR selector/stride input load before every caller skip, and do not form an out-of-bounds C++ pointer merely to imitate unused native arithmetic.

## Smallest integrated source proposal

Keep `completeGeometryNodeSkip`'s current zero-node case and existing null/type7/byte3610 skips. After its pre-skip MAR input load and state=false/flag1/actual-owner checks, allow a nonzero object only for this additional profile:

- Actual current step dimension3, null DB, u1b58=0, version>=6000 and cache slot0b=0. Keep low-query and nonnull-DB branches at their existing zero-node guard in this first cut.
- Positive main node-presence field, with an existing actual main coordinate allocation at +8a8. Keep the native allocation branch unresolved. Use an owned valid object/node allocation in tests, not a foreign pointer or made-up lease.
- Exact selected id11 and the relevant live selector profile above; actual `D[0] != step.u1dc0[151]`. Field151 is within the existing fixed280-entry A address table. Do not require/read A data or form any A source pointer.

Then call the existing real `obj_read_node_arrays` with actual step arrays and id. It performs its native table selection, width mismatch, excluded-region chain, version read and return. Do not replace the helper with a bare success flag. Continue the same object loop and issue the existing one node Stop only when every eligible object completed. Preserve ordinal/MAR progress and refusal after earlier object/reader effects. No new shared interface or public ABI is necessary; a small private predicate in Viewer suffices.

This only releases a no-copy branch. Equal widths, missing main coordinates, a selector alias to11, state/flag/owner mismatch, nonnull DB and populated assignment remain guarded. Those branches need actual destination allocation and/or a bounded live A span with generation and object row/width bounds. A raw nonnull step.u1b78 and its nominal length alone must not be treated as that owner witness. Do not broaden low-frame retirement or resume rules.

## Test contract and owner isolation

- Add an actual secondary main object through existing `llobj_ctor::get_or_create`, with no additional Viewer/view. Allocate its main node storage through the existing real allocator, with consistent count/width and actual step ownership, then set the native presence field. This lets the optional geometry view exercise the reader without accidentally adding an unrecovered main-mesh display path.
- Set the selected stored width unequal to actual D[0], use null A/MAR pointees, and require exactly two completed same-step null-companion getter rounds, two arrays checkpoints, node Stop and the expected populated/edge bounds. Main and optional coordinates, presence/counts, version/cache, owner and current step must remain unchanged. Exercise positive and width-zero optional geometry.
- In the same actual fixture, equalize widths, remove the main coordinate pointer, or alias a relevant selector to11. Each must refuse before calling an unsafe helper/copy/allocation, retain earlier COW/read/arrays effects and leave the scene unknown. Restore the source fields and require a fresh successful two-round invocation. Restore owned pointers before destruction on every exit.
- Inject an equal-width change at the second coordinate-dispatch callback: first reader Stop remains; second COW/read/arrays checkpoint occurs, but second node Stop and complete scene contribution remain guarded. This proves no first-result replay or A/MAR fallback.
- Keep foreign owner, nonzero state/flag refusal, malformed MAR selector, nonnull DB, low-query exact-frame and all CP139/140 layout/data/readiness tests unchanged. A selector guard is a prerequisite for the new branch, not permission to repair process globals or skip their native order.

Exclusive implementation proposal remains Viewer/test pair and a new receipt. Target `m2m_mdt_viewer_box`, CTest `m2m.mdt.viewer_box`. Source remained at CP140 hashes `45107ea75fca2394a3fb50444fd8807825d7ff75e1c8adaf4c486186774975df` / `b1740fd1ce2ea86b37e4b1dbea4974b468652c345578d81cfa13d8f583647d18`. No implementation or validation pass is claimed.
