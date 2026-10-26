# CP138: browser clear walks a frozen snapshot backwards

The next source-closed continuation is the browser-clear traversal requested by CP136. Native1413dd960 copies the browser's top-level property list and removes its snapshot entries **last to first**. This is independent of GUI observation as a conditional algorithm; actual first-Open delivery, browser identity and current snapshot remain unbound. No Boolean E/OPI/Apply occurrence follows from the traversal.

Only this report was written. Bounded immutable PE/DUMPBIN reads and current helper/Session boundary reads were used; no source/build/tests, GUI, original execution, mutable REA or import occurred.

## Distinct owner cells and producer

In SfPropertyEditor initialization13726b0, primary receiver P+18 supplies editor implementation D. Its mode1 branch constructs browser1413da8d0 at137279c and stores its return into D+8 at13727d5. Mode2 similarly calls13d7510 at1372802 and stores D+8 at137283b; the other branch calls13d3080 at1372865 and stores at137289e. Allocation-failure/null remains a real branch; no successful browser is assumed.

All three browser constructors call common base13dd670. It allocates browser-private C, constructs13dc610 and commits C to browser B+28 at13dd715. C+8 is initialized through18f9e0 at13dc62b; that helper writes imported QListData::shared_null and increments its atomic reference. This establishes a constructor-stage list producer, not the actual list at first Open after other initialization/callbacks. Editor implementation D, browser B, browser-private C and Boolean editor E are separate roles.

## Exact snapshot/order contract

Clear13dd960 calls13dd830 at13dd97c. The latter reads B+28, copies its list at C+8 through259ad0 and returns a local list value. At13dd98c,13885f0 copies it again into the iterator's list. The iterator's current pointer initially receives begin, but clear explicitly repositions it through0b5030 at13dd997: that method calls136ff0, whose PE import141e041f0 is **QListData::end**, and writes iterator+8.

Loop predicate0b5060 at13dd9a1 obtains the snapshot's **begin** through307560/IAT141e041e0 and compares it with iterator+8 through126470. If different,13df300 at13dd9b2 first decrements the iterator pointer by8 via066ef0, then dereferences it through64f400. Clear calls13ddbc0(B,property) at13dd9c9 and repeats. The source order is therefore snapshot[n-1], snapshot[n-2], ... snapshot[0], including the exact retained pointer values. Do not sort, deduplicate or replace it with iteration over the changing browser list.

When the snapshot is empty, the predicate is false and branches13dd9ab ->13dd9d0, skipping every13ddbc0 removal. It still executes iterator cleanupc51520 at13dd9d5 and local-list cleanup2081a0 at13dd9e0 before RET13dd9e9. Consequently, **empty snapshot proves no per-property removal, not an already observed normal clear return**. Copies/atomic-reference and temporary cleanup branches remain distinct from removal callbacks. CP136's completion barrier must not be bypassed by reporting that the list has zero entries.

For a non-null property actually present in the current top-level list,13ddbc0 removes its list entry through1f4660 at13ddc38 before calling deeper browser-private removal13dc8a0 at13ddc4e and13dcff0 at13ddc64. It also has null/not-found branches. Earlier removal callbacks can alter the current list; later snapshot entries still must be passed to the native removal continuation even if now absent. The snapshot is not evidence that every removal succeeds or that each property remains live.

## Bounded successor and missing witness

A safe isolated successor would be an owner-bound **snapshot/cursor prefix**: admit actual B/C lifetime and an explicit snapshot; emit one removal request for the last remaining snapshot entry; wait for that invocation's normal return before advancing. It must retain all input entries, allow the actual removal to report its null/not-found branch, and model no browser callback by default. Empty/exhausted snapshot reaches CleanupPending; an explicit admitted cleanup/normal-return boundary is still needed before producing the matching clear completion consumed by CP136. Foreign/expired owners, snapshot replacement, mismatched return and interrupted removal remain guarded. This contract can be tested synthetically without claiming native readiness. No successor files were created.

Current ConversionSession header/cpp has no PropertyRefresh owner/admission binding. Thus even constructor-produced emptiness cannot attach this isolated model to a reached public Open path. To close the actual delivery gap, retain notifier local3 emission with suppression permitting it, actual live P registration/thread route ->slot1376590 ->interface-adjusted1375950, D+8=B and the same B/C snapshot/removal/clear-return frame. The original tree-timer occurrence is not a substitute for this property receiver. To claim Boolean creation additionally retain nested current-widget UpdateData ->Boolean E ->resolved OPI O ->normal add return295a/2bcb inside complete Open. No such original frame was produced by this work.

## Reproduction identities

Use CP135's original module hashes and DUMPBIN14.44.35228.0 identity. New selected code-byte bounds (end exclusive):

| Range | SHA256 |
| --- | --- |
|1413dd830..1413dd872|bec5a6434b91d77304cc690f1278d766c7aa9bedf297227f8ed1c2c1079ced42|
|1413885f0..141388635|261ef0f3f39b0c5f573c0d512e9dae2ed07d1ce327d62fab752fbadb0d64c42b|
|1400b5030..1400b5059|20dfe8885958dbb4cbada4e1eab05f4ff2b813d64966c3a586a510d6eeb69c74|
|1400b5060..1400b508e|849df2274aa6f746e00002aea60080b79aac99923fee0287ff6179ae9f017124|
|1413df300..1413df324|9af4e2e65ed763f6d1523bbc924b0b5bc3ee112d87809e829745693d191255b2|
|1413ddbc0..1413ddc95|0a400be5ebde58fc8aedb1cfef1a556de883a7886d5e06a19d984fbdddb33a92|
|1413dd670..1413dd734|53c2a1e1d6f66ed4ccfbe31eb7dfeeea09d4b04c9203c1724f419453009637c0|
|1413dc610..1413dc692|082a6ec6a1be6b3aa76f14dce4aa14429bbff30782943c53b972e34b848f490d|
|14018f9e0..14018fa10|a17fd6ed20323281552f62200d2152f06cc39e181381a9a84235da56541d621b|

The066ef0 leaf was inspected with a bounded48-byte view; its decrement/return ends at066f0e and the subsequent padding/next procedure is not part of that claim. CP136 source remains frozen; no public activation is proposed.
