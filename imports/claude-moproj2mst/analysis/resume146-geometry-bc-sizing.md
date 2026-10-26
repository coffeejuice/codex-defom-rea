# CP146: complete six BC allocation attempts and node count, then guard variable sizing

The smallest source-ready successor is the six ordered BC integer allocations/publications after CP145's real coordinate allocation, followed by node-class count1. Keep an intentional sticky stop immediately before the variable-sizing dispatcher at `14085435b`. This advances actual owned buffers and native header state while leaving variable allocation, equal-width copy, Stop, retirement and second getter guarded.

Only this report was written. No source/build/test execution, GUI/original launch, new REA tool call or mutable REA access occurred. Evidence uses retained source and bounded immutable PE/DUMPBIN; the graph lane's analysis database was not accessed.

## Exact normal and failure order

Original EXE was rehashed as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Half-open byte receipts:

| Range | Meaning | SHA-256 |
| --- | --- | --- |
| `[140854261,140854348)` | Six BC allocations/publications, then node n | `c3a2e9059d39733cfc5248d66b66e8cc6311e07319e163e4f751fc341b6e516b` |
| `[140854339,140854348)` | Actual node-class count publication | `b7039d009a9b87170461288a758087fa6d7d9b6752bc546951f65653db91e43e` |
| `[1409c8020,1409c8078)` | Integer calloc/error helper | `44434fe45f3e2f3ae90f722f3742863563bb2c7373526c7e18f72745b1664917` |
| `[140b3e8e0,140b3e969)` | Ordered variable-sizing dispatcher | `60f32b7aac76ba04a39dd5d1d3360002b800c69a8608a73bb5027c58b6ed2017` |

The node allocator's positive-count/key-1 path proceeds in this exact order after coordinate publication at `14085425d`:

| D entry / node field | calloc call | pointer store |
| --- | --- | --- |
| D[0x12] / +0xf0 | `140854280` | `14085428d` |
| D[0x13] / +0xf8 | `1408542a1` | `1408542ae` |
| D[0x14] / +0x100 | `1408542c2` | `1408542cf` |
| D[0x15] / +0x108 | `1408542e3` | `1408542f0` |
| D[0x16] / +0x110 | `140854304` | `140854311` |
| D[0x17] / +0x118 | `140854325` | `140854332` |

Each request is low32 `n * D[k]`. Under the existing n1 cut this is simply the actual nonnegative D[k], with no multiplication overflow. The seventh BC pointer +0x120 is not allocated here. It remains the null value established by CP145's reset.

Every return, including null, is published before the next allocation. There is no conditional success test, rollback or early return between these six calls. After all six attempts, `140854342` stores n at node+0x12c, then `14085435b` calls the variable dispatcher with actual node set, n, D and key=-1. A recovered cut must not stop after a null BC pointer while claiming the native BC sequence completed.

Integer helper returns null without allocating when count0. Otherwise it sign-extends count, requests calloc(count,4), emits the allocation diagnostic on failure, then returns the pointer (possibly null). Verified literals are `14387b2b8` = `INT`, `14387b2c0` = `ERROR    ` and `14387b2d0` = `%s Error when allocating %s memory of size %d\n`. Existing `bc::calloc_ints` in `bc/boundary_topology.cpp:65` matches this behavior. Use it instead of the private simplified allocator inside `node_set_alloc`, whose source comment says its diagnostic is not reproduced.

## Same-invocation implementation boundary

Extend only the currently active high prefix immediately after MainCoordinatesPublished. Require its exact installed frame/current borrowed step/DB/receipt/source-A facts, the exact recorded destination object/pointer/extent3 still matching main+8a8, positive presence, logical n1/first1/D0=storedWidth3, node-class n0, canonical relevant selectors, unique empty bound variable registry and all seven BC fields still null. Preflight all six counts as nonnegative; negative counts retain the existing coordinate-publication stop before this new admission. No fresh reset or coordinate allocation is repeated.

For k=0..5, read the actual qualified count, call `bc::calloc_ints`, publish exactly that node field, and record requested count/pointer/attempt order in private observational frame state. The node class owns successful allocations. A null result with positive request is a failed allocation, not a valid positive-sized span; preserve the diagnostic/null slot and continue later normal attempts as native does. Count0/null is the ordinary empty array. Do not claim all pointers are valid merely because all attempts completed.

If a callback-sensitive source/receiver/count fact changes, a candidate may guard after the actual reached publication with that partial prefix retained. It must not overwrite an unknown slot, retry earlier allocations or infer a new span from metadata. For a normal unchanged profile, complete all six attempts, publish node-class n1, record a BcAttemptsCommitted stage and stop before `14085435b`. Retain the exact barrier even though source A, primary coordinates and some BC buffers now exist. No generic ready/resume permission or high completer invocation is allowed.

No extra A/MAR read or allocation is needed. Low/null callers, CP141 mismatch, CP144 zero-main completion and preexisting-destination/equal-width guards remain unchanged. Proposed write set is Viewer, existing `test_geometry_a_producer.cpp` and a new receipt; no shared header/CMake/helper is needed.

## Variable sizing is a separate partial-effect boundary

Native `140b3e8e0` snapshots registry count at `b3e905`, looks up each index through `14086e340` at `b3e937`, and calls that actual receiver's virtual +0x10 at `b3e95f`, in registry order. The corresponding port is `Cls_140329c70::set_all` (`members/statevar_collection.cpp:57`). It does not transact or roll back prior variables.

Port-qualified subtype semantics: ordinary StateVarNode inherits Base slot2, key-1 updates dim0 then dim1 and calls resize(false); four StateVarNodePost receivers accept only their own id, so key-1 leaves them untouched. The ordinary resize computes the requested dimension product, may allocate a temporary copy and resize storage, and only then publishes the bound data slot. Therefore an exception can leave earlier variables changed and the current dimensions updated before its data pointer is published. The port explicitly throws `bad_alloc` on temporary allocation failure; exact original leaf allocator/unwind cleanup is not independently closed in this lane. Do not label those failure paths verified merely from the dispatch loop.

A future no-allocation sizing profile is plausible when all ordinary registered D[id] are0, values remain empty and dim2 is1: requested size stays0, so only ordinary dimensions change and post variables skip. This is distinct from BC D[0x12..0x17], whose ids are not those ordinary registered node variables. It may allow positive BC buffers while avoiding variable heap allocation, but still needs an exact next source/leaf qualification. This report does not authorize entering the dispatcher or equal-width memcpy.

## Tests and old cohort maintenance

- Use the real CP145 secondary object/source producer/destination allocation. Exercise six-count patterns all0, all1 and distinct/mixed0/1/2/3/4/5. Check actual zero-filled integer buffers, nulls for0, exact pointer-to-field ordering, untouched seventh field, node-class n1 and still-empty variable slots, one optional COW/arrays checkpoint, node timer2 and unknown scene. Both real owners free all buffers normally.
- Repeated refresh/mutator probes must not allocate/reset/query/Stop or advance variable sizing; raw presence repair cannot bypass the barrier. Keep source/destination/BC pointer identities and prior cache/version effects.
- Negative first or last BC count should retain the coordinate-only boundary before any BC attempt; do not test it by requesting a huge real allocation. Preserve wrong owner/selector/source/destination/registry guards before this sequence. Native no-rollback/failure continuation is established statically; deterministic failure injection would need a separately authorized allocator seam, not real OOM.
- Preserve CP145 coordinate-only cohorts with a genuine next guard (e.g. a negative BC count initialized before their snapshots), or explicitly update normal expectations to the new BC/node-count behavior. Do not simply remove their unsupported assertions. Other guard/reset-failure cohorts remain unchanged.

The six-attempt/node-count cut is source-ready. Variable sizing and equal-width copy remain the precise next blockers.
