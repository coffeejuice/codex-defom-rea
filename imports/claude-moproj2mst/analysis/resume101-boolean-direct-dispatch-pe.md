# CP101: direct dispatch population and Boolean OPI order

Retained PE closes a useful conditional ordering: modelUpdated's DiagItemClicked dispatch finishes before its later UpdateMainView dispatch; an actual synchronous Boolean Apply/addObject nested in the first therefore finishes before the second begins. It still does not place either invocation against first Open completion. Registration population, emission snapshot and live invocation are different source boundaries.

Only this report was written. No REA database access, original execution, GUI/control, production edit, build or tests. New bounded DUMPBIN reads used immutable original EXE `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; banner14.44.35228.0. The complete list-copy constructor region2ef070..2ef0c1 was decoded in memory using PE unwind bounds; no new raw artifact/database was created.

## Exact registration and direct invocation

| Selected registration | Actual receiver supplied | Key/method literals |
| --- | --- | --- |
|0111c0 setup at14001170e|SfMoMainWindow `this`, R8 from saved owner|1420f8d58 `2signalDiagItemClicked()` ->1420f8d40 `1slotDiagItemClicked()`|
|SfMoMainView ctor e9400 at1400e9e42|That actual MainView `this`|142187130 `2signalUpdateMainView()` ->142187118 `1slotUpdateMainView()`|
|Area registration d14e90 at140d14f08|Actual area+28 impl|UpdateDataInWidgetArea ->slotUpdateData|

The latter is CP64/71's receiver registration, not either main-window/view receiver. All use custom registry1402cfbe0 on143d76700. Flag0 is append/dedup policy, not AutoConnection. The registry strips the slot to its bare method; key text `()` does not declare argument arity. Direct PE literal reads bind modelUpdated's first generic argument type1421d8318 to `ISfDiagItem*`.

The actual dispatch1402cf3c0 gates on notifier+141 at2cf3f1, clears+90, resolves the keyed list and copies it into the local frame at2cf461, then captures its count at2cf47a. New decode of copy constructor2ef070 shows container-data copy/ref retain and conditional detach helper15d230; it is not a clone of live QObjects. Receiver guard resolution occurs at2cf4c7/4dd and again2cf638 before invoke2cf6c6 with R8D=1. Metadata lookup/invocation success and receiver state are therefore later than registration/list copy. Expired entries are removed from the local list; this does not prove stored-registry cleanup.

“Dispatch generation” should mean an observation identity for this emission's selected keyed list/entry and receiver lifetime. No native integer generation counter was found/proven here. Do not stamp an arbitrary native generation field or treat constructor registration as the current emission population. Membership/copy facts do not close every transitive registration/removal mutation or receiver effect during earlier callbacks.

## Source-closed intra-callback order

In183160, fit returns before current diagram attachment/item lookup. On its nonnull selected path:

- 1833fe invokes DiagItemClicked using key1421d8328 and the current item payload. The retained main-window metadata route is local53 ->0db1a0/case0db9f4 ->0387b0; the qualified selection path then reaches5f36d0.
- Only after1833fe returns does1835fb invoke UpdateMainView, key1421d8348. That key has its distinct MainView receiver above. Do not conflate it with DiagItemClicked's main-window slot or use the second key as proof that the first invoked Apply.
- In qualified5f36d0, UpdateData at5f4524 precedes Leave/new factory5f4756/Fill/Show. Its actual impl current/cast dae340 calls virtual+1c0 atdae3ab; Boolean table142cd9b18 targets1411e2870. Explicit type1 makes this admitted leg synchronous, with no separate UpdateData queue/wake prerequisite.

Each emission reads its own suppression gate/population. One admitted key does not certify later keys after synchronous recipients run. A suppressed first key can return and still allow the second key's own independent gate/dispatch. No complete transitive absence of reentrant Apply is claimed.

## CP98 OPI selector retained

Apply1411e2870 calls130af0, which resolves **editor+38 guarded OPI** throughc1aef0; the result is saved at[rsp+38].051230 on that OPI gets properties, then025010 obtains BooleanData. Workpiece path reloads that same OPI at2950 before057ba0 call2955/return295a; Cutter reloads it at2bc1 before call2bc6/return2bcb. The native RCX is OPI, not GuiModel.060530 first-free and0585e0 rename also belong to that operation path.

D's application-private diagram attachment (D+28 ->field18) is a third, separate object relation. Operation-model05f770 resolution must be observed separately if asserting an engine-model address. `engine::addObject(Project&,Operation&,...)` in the current source corroborates the OPI role; it supplies no runtime address or timing.

## Smallest remaining witness and next action

For a positive first-Open callback effect, correlate one actual D183160 invocation with the explicitly selected enclosing Open span, its actual DiagItemClicked/list-entry invocation on W, nested UpdateData/current impl/editor E, Apply and resolved OPI. If object creation is the target, add normal057ba0 return filtered by295a/2bcb and result/object number. A successful nested add can establish an occurrence before an observed enclosing completion without reconstructing all timer deadlines; exact Open-registration attribution additionally needs the actual timer/connection generation. “First” or absence needs gap-free coverage.

Use CP82 boundaries: GUI0271b0 ->actual return028514; CLI loader return1c94 is **before** postOpen1ca9/return1cae and branch completion1e4e. Capture the relevant2cf461 snapshot identity/count and actual2cf6c6 target at each selected key, rather than inferring delivery from1170e/e9e42/d14f08 registration instructions. Continue to keep actual guard/lifetime/current state unknown where not observed.

Concrete next action is a release-safe passive owner-correlated trace with these selectors and CP98's corrected OPI field labels, once the existing owned-session/release gate permits original execution. No such run was attempted. Until then, static call-return order and candidate-declared schedules cannot select the original first-Open callback phase. Public PostPending/first-Open dispatch remains guarded.

Evidence: retained111c0/e9400/183160/2cf3c0/2cfbe0 bodies; CP64 direct invocation, CP70 argument-shape qualification, CP71 area population, CP82 Open boundaries, CP98 owner correction; current loader.cpp addObject signature. All conclusions are selected-body/source-order facts, not complete callgraph or fresh runtime parity.
