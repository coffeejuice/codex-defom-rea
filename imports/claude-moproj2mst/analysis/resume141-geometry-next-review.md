# CP141 independent geometry proposal review

The proposed nonzero-node LLCRZ width-mismatch branch is source-closed under its complete stated live-owner/profile guards. I found no blocking design defect in `analysis/resume141-geometry-next.md`. This accepts the bounded design, not an unreviewed implementation or a test pass. Root must still review the released patch and run the focused target before integration/freeze.

Only this report was written. No source/build/test/GUI/original execution or mutable REA work. I independently read the ported full node helper, current Viewer caller, table initializers/selectors, real node allocator and existing geometry fixtures. Bounded DUMPBIN and section-mapped bytes from retained original DEF_GUI_MO_64.exe v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, corroborated the proposed native ranges. No reconstructed executable/deployment was inspected or changed.

## Guard completeness

At140842239 an existing object+8a8 pointer skips the actual allocation branch. With exact selected id11 and global LLCRZ selector11, the selected path does not compute `a_tab[LLCRZ]-1`. At1408422e1 the actual D[0] versus actual `a_tab[151]` inequality branches140842354 before A address formation, multiplication or copy. Passing a null A argument is therefore safe on this exact path; merely requesting a zero-length copy would not provide the same assurance.

I extracted all22 `take` region slots from `obj_read_node_arrays` in `source/port/defdbio/src/dbread/w2_objarrays.cpp`. In native/source traversal order they are:

```text
11,12,13,14,15,16,17,18,51,19,20,21,22,24,23,25,27,32,35,42,55,57
```

Slot51 is the dimension2-only block, excluded by the actual same-owner dimension3 check. The proposal checks every other slot against its exact canonical value, plus width-selector slot70=151. All later selected-region tests therefore exclude11, including resize/width-changing LLDMG/LLGSZ and version-gated LLTRC/LLNRV. The separate dimension3 literal region19 block is excluded by id11 regardless of any table value. The actual version getter after the skipped blocks only reads step+1b64; it does not create an unguarded assignment path. The proposed profile thus covers all relevant branches, not only the first coordinate block.

Guard actual live selectors after the original caller's table-selection effects. `select_index_tables_3d` skips reinitialization when `g_index_tables_dim==3`, so the marker alone does not establish intact tables. Within the admitted helper call the actual object's owner is the same dimension3 step, and no callback intervenes between the predicate and table/helper use. Its own select_tables then leaves the already-selected tables intact. Do not use this reasoning to admit a foreign owner or a cached predicate from an earlier getter round.

The common helper is also used by existing low-record/non-null-DB paths. Any new admission must explicitly require actual null DB, dimension3, u1b58=0, step_version>=6000, cache0b=0 and id11, preserving those other paths' current zero-node guard. Positive presence is a conservative subset of native nonzero presence. Keep optional geometry's existing negative-width/finite-data validation unchanged; this no-copy path does not justify treating unsupported populated geometry as ready.

## Owner and effect order

The proposed same-step main object must come from the actual step object registry; `object->owner == &step` is necessary before its helper reads the owner or version. Preserve native null/kind7/byte3610 caller skips before that check. Preserve state/flag qualification and the actual MAR selector bounds/stride load before every caller skip and before the new admission. Null A/MAR pointees remain acceptable only because the branch never forms a source/record pointer; do not construct a null-plus-offset C++ pointer just to pass unused native arguments.

Continue calling the real `obj_read_node_arrays`, with the actual step's D/address tables and selected id. A predicate-only success flag would omit table selection and the version read. Continue every eligible main object in order and emit node Stop only after they all return. A later refused object or second getter round must retain prior COW/reader/arrays/first-round Stop effects, while keeping complete scene readiness unavailable.

Tests can safely allocate an actual secondary main object with `llobj_ctor::get_or_create`, using the real node allocator and small consistent positive count/dimensions. `alloc_node_arrays` frees/reallocates the actual collection; it must run only against that owned object's existing storage. Do not fabricate a pointer or count solely to satisfy the branch. Keep this object outside the Viewer list so later main-mesh display is not accidentally admitted. A null-coordinate negative probe must restore the saved owned pointer with RAII on every exit before model/object destruction; restore presence/count/dimensions consistently as well. No extra native free is needed just to test width mismatch.

## Focused test acceptance conditions

The proposed positive and partial-effect tests are meaningful: actual allocated secondary storage with nonzero presence, null A/MAR, width mismatch, two real getter rounds and two arrays checkpoints; unchanged main/optional storage, counts, owner, step, version/cache; both populated and optional width-zero geometry. Verify expected node Stop per completed round and actual scene/bounds, not just absence of an exception.

Before integration, require negative cases for equal widths and absent coordinate allocation, plus a live relevant-region alias to11 while the dimension marker remains3. Prefer enumerating all20 later non-2D selector slots, since omitting any one can expose a different copy/resize block; at minimum the released predicate must be independently compared against the full list above. Also test width selector70 corruption (including an invalid index) refuses before reading `u1dc0` through it, and selected selector11 corruption refuses. These checks must not repair globals to make the test pass.

At second coordinate dispatch, change the selected stored width to equality and verify the first reader Stop survives, the second COW/read/arrays effects occur, but the second Stop and complete scene do not. Restore fields, invoke afresh and require two successful rounds. RAII should restore process tables/timers and owned pointers even if a Testkit REQUIRE aborts the current case. Existing foreign-owner, state/flag, MAR-index, nonnull-DB and low-frame retirement tests remain required.

No source/test patch was assigned to this reviewer, so assertion macros/includes and actual test count cannot be certified here. Root validation target remains `m2m_mdt_viewer_box` / `m2m.mdt.viewer_box`; review the implementation receipt and hashes separately.

## Independent byte verification

All four hashes in the proposal independently matched the retained original:

| Half-open range | SHA-256 |
| --- | --- |
|1408421d0..140843af4 full helper|`223273ef5b5572cc166c6c3a9b06c33e8704d8f560b4378d393f87aebc425253`|
|140842239..140842354 allocation/width branch|`dddee711ef70f9c3501d2de7ab9ead32bfac7748205620e0ddca541645a3a90e`|
|14084316c..1408431ac literal-region19 gate|`4dc42138fcc0ce60fe6996a612f3af97e114ebd64777c5a87c557bc643b1a082`|
|140843846..140843af4 version/tail|`229326d1ce968e7e3c261bdc9c4456309b23787c969ceaeee149a87811ae4ead`|

The plan's selector values also match `source/port/defmodel/src/step/index_tables.cpp::init_a_index_3d`. This branch releases no populated assignment, destination allocation, live A-span ownership, full application/Project lifecycle or public Close behavior.
