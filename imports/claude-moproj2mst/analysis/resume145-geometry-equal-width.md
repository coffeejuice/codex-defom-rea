# CP145: publish a real fresh main-coordinate allocation before admitting copy

The smallest safe successor is the **fresh main-coordinate producer prefix**, with an intentional sticky stop before BC allocation/variable sizing and equal-width memcpy. This produces a real destination pointer/extent under the actual object, rather than certifying preexisting coordinates from metadata. Equal-width assignment belongs inside the first reader, before high-frame retirement and the cached second getter; it must not be replayed later merely because main presence changes on a cache1 refresh.

Only this report was written. No source/build/test execution, GUI/original launch or mutable REA occurred. Evidence is retained source plus bounded offline immutable PE/DUMPBIN reads, not new REA calls. Root has not assigned source edits in this lane yet.

## Exact original destination producer

Original EXE remains SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Half-open byte receipts:

| Range | Meaning | SHA-256 |
| --- | --- | --- |
| `[140837080,140837114)` | Main free/allocate wrapper | `cd384c9ca013c3779c04b3cf7526cf6a5a427317bfc2f164183384c3899c2c47` |
| `[140837300,14083731f)` | Main node free wrapper | `d9eeb5175dc379f2d07018fe4384f9831a5cac71304a49bddeca0e3eaf3179fa` |
| `[140853350,1408534fd)` | Node coordinates/seven BC arrays/reset body | `3e352313eab136fced167692f9b68a1ad5af3eb6cb6d6c12aa8737f10bb09999` |
| `[140854200,140854365)` | Node allocation/sizing body | `eded3d5ab5ecc12797003b14aecf8ef582451f28dc9b6fd934c000276c42520d` |
| `[140854238,140854261)` | Coordinate calloc/publication before BC work | `7765d36386bc20d07540a86a9370e49f085ca4c40d2867a05c396d69ee6c900c` |
| `[1408422c7,140842354)` | Subsequent equal-width copy | `96746cc457c5b8225807caa92a6614ab8e83d48820b580a99e44d087afb149a3` |

Node helper `140842239` tests actual main coordinates at object+8a8; null calls `140837080` at `84225b`. That wrapper first calls `140837300` (`837093`), which selects node subobject object+8a0 and calls `140853350`. It frees/nulls coordinate+8 and all seven BC pointers+f0..+120, sets node-class n/+12c and +13c to0, then clears registered state variables through `140b3e840`.

Wrapper then reads owner and GUI names predicate, but both resulting branches invoke exactly the same node allocation routine `140854200` with actual object+880 node count, actual D table and key=-1. In its positive-count/key-1/null-coordinate branch, `85424d` computes low32 n*D[0], calls verified `1409c83b0` at `854250`, and publishes node+8 at `85425d`. The next instruction boundary `854261` begins the separate key-1 BC allocation work; node-class count is not written until `854342`, followed by variable sizing at `85435b`.

Thus a prefix stopping just after coordinate publication leaves a real new allocation but node-class n=0, BC arrays empty and variable sizing not yet applied. It is not a completed `alloc_node_arrays` call and cannot authorize the existing whole node helper or reader return.

## Source-ready fixed admission

Use only the current initial high frame after its actual fresh A/cache1 producer and caller arrays checkpoint. Preserve exact installed frame/current borrowed step/actual DB/receipt identities and its successful internal source-A pointer/count. Never infer source extent from +1b6c/+1b74. Require actual A still equals the produced pointer, guarded count still equals maximum+2 and count>=3.

Retain current pre-skip MAR selector/stride load and caller null/type7/byte3610 skips. On an eligible positive-presence main object, require actual same-step owner, state=false/flag1, actual dimension3/high/cache1, selected id11, the exact relevant selector profile from CP141, and stored coordinate width `u1dc0[151] == D[0] == 3`. Start with actual object node count1 and first-node1, so the prospective selected copy is exactly three doubles at source offset0, entirely inside the produced span. Do not generalize wrapping products or arbitrary row spans here.

Require fresh destination coordinates null, node-class n=0, all seven BC pointers null, all28 variable slots null, canonical actual node registry count/types/ids and correct per-variable slot binding. Require its collection and empty values bodies uniquely owned; the existing CowList/CowVector `use_count()` APIs allow this check. This bounds the real clear/reset to the known owned empty profile and excludes unknown COW allocation or foreign slot publication. Other D widths are not consumed before `854261` and need not be constrained: the earlier informal all-other-widths-zero suggestion was stronger than necessary.

Within this same initial callback, preserve native table selection from the actual owner (the outer already selected3; require the relevant actual tables). Execute existing `bc::free_node_arrays` for the exact owned main object, record a private MainNodeResetCommitted stage, and recheck actual frame/source/owner/count/D identities after its callback-capable clear operations. Then call existing diagnostic `bc::calloc_doubles(3)`, publish actual node+8, and record the pointer plus successful extent3 as observational frame state owned by the actual main node class. On null allocation retain the diagnostic/null publication and park. Mark MainCoordinatesPublished and **return incomplete immediately**.

Use the diagnostic allocator directly for this split prefix. Calling whole `bc::alloc_node_arrays` would continue into BC/variable sizing; calling whole `obj_read_node_arrays` would continue through the guarded copy. A partial private wrapper must not pretend either whole call completed. The native wrapper's intervening owner/names loads are pure under the already-qualified current owner/high profile and choose the same allocator branch; no extra generic names fold or replay is needed.

Keep the exact installed barrier after publication. Do not issue node Stop, retire the high frame, return optional coordinates, request the builder second getter, set a generic destination-ready flag or resume on a later invocation. Preexisting destination pointers, unknown registry/storage, short source spans, noncanonical selectors, other counts/widths/offsets and all other nonzero main profiles retain their current guard before this new reset/allocation. Existing zero-main/high completion and CP141 null-DB mismatch behavior remain unchanged.

## Equal-width copy remains the next boundary

Native selected copy uses `(first-1)*D[0]` source offset and `mainCount*D[0]` length, then continues through the excluded region chain/current-version tail. Actual main allocation provenance is now recoverable, but BC/variable sizing and its partial failures still occur before that copy. Do not use the new pointer/count to bypass them or call the whole helper on a parked retry. A later same-invocation cut can complete those exact stages, validate both actual allocation receipts, then perform the copy; it needs separate source assignment/review.

## Integrated tests and write set

Proposed write set is Viewer, existing `tests/mdt/test_geometry_a_producer.cpp`, and a new receipt; no shared header/CMake/helper is needed. Pass private frame context only at the existing high initial assignment callsite; low/null callers get no new allocation admission.

- Configure a real secondary main object without an extra Viewer, before the first fresh producer invocation: actual owner, positive presence, count1/first1, canonical fresh node storage and D/stored width3. With actual source extent3/6 require one coordinate COW, source A/cache1, one arrays checkpoint, real new zero-filled destination extent3, node-class n0, unchanged seven null BC arrays/empty variable slots, node timer count2 and unknown scene. Both allocations use their real owning destructors.
- Repeat refresh/mutator probes after publication: no new query/COW/reset/allocation/Stop; both pointers and prefix mutations persist. Raw presence/count repair must not bypass the barrier.
- Source extent2, preexisting owned destination with sentinel data, wrong count/first/width, selector alias/width-selector failure, nonempty/shared/foreign-bound registry state or foreign object owner must retain the old guard and leave main storage untouched, after the actual earlier producer/arrays effects.
- A declared main-clear dispatcher callback can change a guarded owner/count/source identity. Require reset effects to remain, then park before coordinate allocation/copy. Restore mutated owning pointers before destruction. Do not seed production arrays or claim synthetic test values as an original runtime witness.
- Retain CP144 normal zero-main/cache1 completion and all low/query/equal-copy barriers. Changing a main object only after a completed cache1 getter must not invent a delayed first-reader allocation/copy.

This fresh publication prefix is source-ready. Full equal-width copying is still guarded, and no public editor/import branch is released by it.
