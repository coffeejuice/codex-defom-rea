# CP146 BC allocation design review

No concrete blocker found in `resume146-geometry-bc-sizing.md`. The six-attempt/node-count cut follows native order and stops at a real later variable-sizing boundary. This is design acceptance for source assignment; it is not a review of an in-flight patch or a build/test pass.

Only this report was written. No source/build/test/GUI/original execution or REA/database access. I reviewed frozen CP145's high frame/main-coordinate prefix, actual integer allocator/node owner/variable dispatcher and bounded immutable native bytes. Original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Native allocation/null/count order

For the already admitted positive n1/key-1 path, native140854261..4348 has the exact sequence below. All four plan byte hashes independently matched.

| Request D index | Actual field | Allocator call | Publication |
| --- | --- | --- | --- |
|12h|node+f0|140854280|14085428d|
|13h|node+f8|1408542a1|1408542ae|
|14h|node+100|1408542c2|1408542cf|
|15h|node+108|1408542e3|1408542f0|
|16h|node+110|140854304|140854311|
|17h|node+118|140854325|140854332|

There is no pointer-result test, rollback or early return between the six calls/stores. The key comparisons occur before this sequence. Each returned pointer, including null, is stored before the next request.140854342 writes actual node-class n only after the sixth publication. Seventh field+120 is not allocated and stays the reset null value.

`bc::calloc_ints` matches the verified helper: count0 returns null without entering calloc; otherwise it requests sign-extended count*4 bytes, reports its native-style diagnostic on failure and returns the possibly-null pointer. Under fixed n1 and preflighted nonnegative D entries there is no multiplication/sign-wrap ambiguity. Use this helper rather than simplified `node_set_alloc`'s private allocator, whose missing diagnostic is already documented.

Count0/null must not be confused with positive-request allocation failure. The latter is not a valid positive-sized span, but it does not authorize skipping later native attempts or withholding the final n store solely because a pointer is null. The plan preserves that distinction and does not claim successful backing for every completed attempt.

## Actual owner and sticky boundary

Extend only immediately after CP145's actual MainCoordinatesPublished in the original initial borrow. Require exact installed frame/control block/current model/step/DB/receipt/source-A facts, recorded actual main object/destination pointer/extent3, actual positive presence/count1/first1/D0=storedWidth3, node-class n0, canonical selectors and unique empty correctly slot-bound registry. All seven BC pointers must initially be null. Preflight **all six** counts before the first new attempt so a negative last count retains the same coordinate-only boundary as a negative first count.

Do not call the existing fresh-null-coordinate predicate or reset/allocation helper again after publication; it intentionally requires coordinates null and would either reject the completed prefix or replay earlier effects if weakened. Use an exact stage-specific destination/registry qualification. Successful BC buffers belong to the actual node class and its existing seven-pointer destructor/free path; the frame only records observation/attempt history.

After each reached publication, preserve that actual field and private count/pointer/order record. If source/receiver/storage facts change, guard with the consumed partial prefix intact before any later unsafe call or unknown-slot overwrite. Any repeated qualification must distinguish already-published slots from still-unattempted null slots. No retry may re-enter an earlier allocation. On the normal unchanged path, attempt all six, store n1, record BcAttemptsCommitted and leave the existing barrier installed. There must be no call to the high completer or node Stop after this intentional incomplete return.

The exact guard remains before14085435b. The variable dispatcher snapshots count, visits actual registered receivers in order and invokes virtual+10. Existing `Cls_140329c70::set_all` is not a transaction. Entering it would expose dimension changes, values allocation/publication and partial exception effects not closed by this allocator plan. A plausible future zero-size sizing profile is not admission for this cut. Equal-width memcpy and first reader completion still occur later.

No low/null caller, preexisting destination, CP141 mismatch or CP144 zero-main path is widened. A later cache1 refresh must not create a delayed BC/sizing/copy occurrence after the first frame was retired or parked.

## Focused test discrimination

All-zero, all-one and mixed0/1/2/3/4/5 patterns exercise every slot, ordinary null returns and later nonnull attempts. Terminal assertions should check exact per-field requested sizes/zero contents, untouched seventh field, n1, still-empty variable slots, stable source/destination/cache/version, one optional COW/arrays checkpoint, node timer2 and unknown scene. Mixed zero-before-positive counts and final n1 detect an erroneous null-result early stop. A deterministic positive-request/null-failure case is not claimed by these ordinary allocations; native failure continuation is established statically unless a separately assigned failure seam is added. Real OOM/huge requests are unnecessary.

Final zero-filled buffers alone do not expose temporal allocation order. The released code/private attempt ledger must be compared against the native table above; tests cover field/count mapping, zero/null behavior, omitted work and sticky retries. No new production allocator hook is required merely to claim a runtime ordering observation.

Preserve CP145's coordinate-only tests using a genuine next guard such as a negative BC count set before snapshots. Negative first/last cases must show no BC attempt or n publication while retaining the real coordinates/reset/source prefix. Existing wrong owner/selector/registry/source/destination and reset-callback mutation cases retain their prior guards. Normal positive BC tests must not silently inherit coordinate-only n0 expectations. Repeated refresh/mutator/raw-repair probes must preserve actual pointers without new reset/query/allocation/Stop or sizing.

## Immutable identities

| Half-open native range | SHA-256 |
| --- | --- |
|140854261..140854348 BC attempts/n|`c3a2e9059d39733cfc5248d66b66e8cc6311e07319e163e4f751fc341b6e516b`|
|140854339..140854348 n publication|`b7039d009a9b87170461288a758087fa6d7d9b6752bc546951f65653db91e43e`|
|1409c8020..1409c8078 integer allocator|`44434fe45f3e2f3ae90f722f3742863563bb2c7373526c7e18f72745b1664917`|
|140b3e8e0..140b3e969 variable dispatcher|`60f32b7aac76ba04a39dd5d1d3360002b800c69a8608a73bb5027c58b6ed2017`|

Plan SHA-256 `391dca8208d2ad3fffc3c8eb532036389b02e2e5036e4da502f6537cffd674c0`; inspected CP145 Viewer `8d93b079167fcdcde1475b9f77c96c0cdc2ecd900129c702b15fc7725c75f859`; producer test `91b69c80a2bcef6c5c23b1f1e0bf13eae628855be2dce3f2f6c435bf0438ce16`. Geometry worker owns source. Root must review released hashes and run the focused producer/Viewer/no-copy targets plus source-stable full validation. No source was edited and no public editor/import or Close gate was released by this review.
