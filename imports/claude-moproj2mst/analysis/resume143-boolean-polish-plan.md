# CP143 plan: admit an existing raw0x4a handler, then project its polish cache

The private cut begins at a separately admitted same-receiver QWidget::event raw0x4a branch. It does not construct Boolean editor E, bind E's OPI, post/dequeue the incoming event or establish that this happened during Open. The result must never create a diagram due observation or Apply/add-return witness. Only this plan is written; no source/build/tests, GUI/app control, original process or mutable REA/import occurred.

## Exact source scope

On Boolean primary table142cd9b18, event+20 forwards to QWidget::event6506c950. Raw0x4a selects6506cc2d and calls ensurePolished65064f50 atcc30. The latter calls actual receiver metaObject+0 at64f77, compares its return with widget-private+d0 at64f79, and:

- Equal:64f80 ->6506509b, skipping the cache write, Polish construction/send and child traversal in this invocation.
- Unequal:store current metaobject to widget-private+d0 at64f86, then construct a **new stack event0x4b** at64f97, send it to E at64fa6, and continue to child/parent work only after that send returns.

CP142's original bounds remain controlling: QtGui65064f50..650650b1 SHA2569535edea24baaef6d1c00e784b081984baaa3aaf894a0c4c9ddc7242665fe090; QWidget event6506c950..6506d9e9 SHA256b73ef66751097ecb85253e5e516888a27fb0c220980593848ecceb4a7d29bba2. No equality value or occurrence can be obtained from those bytes alone.

## Receiver, event and field ownership

Use a new isolated admission state, not construction of WidgetState or defaults in WidgetRuntime. It holds one explicitly qualified primary receiver E allocation/lifetime and widget-private address/lifetime, plus one canonical optional cached-metaobject cell representing that private object's+d0. Unknown cache is not null. Rebinding, alias/control-block mismatch, expiry and reused numerical addresses must be rejected. Aliases within this isolated model must share the canonical cell rather than synchronize independent copies. A future Session binding must attach the real owner/cell; this task supplies none.

Existing WidgetState has an optional boolean fieldD0Present_ used by the still-guarded child-polished route. Presence alone cannot supply the cached pointer or equality with the current metaobject. Do not bind the new isolated state to that existing field or seed a second production value. Any later shared-owner integration must reconcile both views through one canonical field, deriving known presence from its known pointer value; that shared change is outside this plan.

The source profile must identify the **actual table at dispatch**, EXE RVA2cd9b18, not merely a historical Boolean constructor address. Base construction has another table; Forming Editor004 cannot supply this qualification. Capture/artifact/module authentication stays outside the helper. A diagnostic synthetic test can declare the source profile explicitly, while labeling itself synthetic.

The incoming event H is separate from the outgoing stack event K. Admit H's actual event identity/lifetime, raw type0x4a, receiver E and native thread/handler occurrence. The entry qualification must cover the selected handler branch6506cc2d after actual notify/filter routing. An event object or a queue record by itself is insufficient. H's posted bit may already be cleared before handler entry; do not require posted=true, fabricate its constructor flags or reuse a stack WidgetEventState::create as the posted event. Admitting this handler prefix does not claim H came from the constructor's original post unless a separately retained enqueue/dequeue chain proves that origin.

No OPI input is needed for this field comparison and none may be invented. E+38's guard, the diagram receiver, QSingleShotTimer and tree layout timer remain distinct external roles. The helper exposes no operation/editor-selection API and no BooleanDueObservation.

## Minimal staged implementation contract

One prefix frame binds the same receiver/field owner and H/handler occurrence. It consumes an explicitly qualified normal metaObject-return value for the call at64f77. Unknown return stays pending. A zero current-metaobject return is invalid for this source-qualified Boolean getter profile; do not silently convert it to static metadata. A known zero cached+d0 value is allowed and remains distinct from unknown.

The cached field must be qualified for the source comparison stage64f79, not borrowed from a constructor or another widget. Once the current-metaobject and cache-read values are admitted for this occurrence, they cannot be silently replaced on resume. This preserves read order without assuming values stayed equal across unrelated callbacks.

On equality report a terminal **SourceCacheHitSuffix** decision with no store or callback request. Do not call it observed event completion or report a native handler return. On inequality commit the source cache store once and stop at **PolishEventConstructionPending**, before64f97. A request may identify raw type0x4b, the same E, source constructor site and a per-frame token; it must not contain a fabricated stack-event address or claim that construction/send succeeded. Repeated resume must not repeat the store. No rollback is introduced when later callbacks remain unknown or are interrupted.

This first implementation should stop at that construction request. If root separately authorizes a later callback bridge, its contract must require an actual matching stack K constructor-return observation before producing SendEventPending(E,K). K must have a distinct event identity/lifetime from H, raw type0x4b and the same invocation token/receiver; it never enters a posted queue. An admitted send normal return can only reach a separate PostSendContinuationPending stage before child/parent work. Style+68 at6506cc4b and all downstream callbacks remain unrepresented. No elapsed time, event-request issuance, meta-cache store or constructor request satisfies any of those return barriers.

## Focused tests for the first authorized source cut

Use Release-active Testkit cases with declared receiver/event/field owners, not an original capture. Check:

1. Missing handler admission, wrong raw type, wrong source table/thread/receiver/event lifetime and missing qualification never select a cache branch.
2. Unknown current-meta return and unknown cache remain guarded; known cache0 is not treated as unknown; current-meta0 is rejected.
3. Equal pointers select only SourceCacheHitSuffix: no field write, event construction/send or normal handler-return claim.
4. Unequal pointers write the canonical cell exactly once before the construction request. Repeated resume preserves the count/token and cannot execute/send a stack event.
5. Colliding numeric IDs, another owner's event, rebind, released owner scope and replaced stage observations are rejected; aliases sharing the canonical owner observe the same committed field.
6. Any later unmodeled callback/external field observation is not rolled back or interpreted as an Apply. This first cut does not model concurrent/reentrant callback execution.
7. Projection reports/requests are not convertible to CP139 removal return, CP136 clear return, BooleanBoundTraceDue or another native-delivery/completion observation. Existing constructor PostPending remains guarded.

## Proposed exclusive files after root review/release

New names, verified absent:

- source/apps/moproj2mst/include/m2m/engine/boolean_polish_cache_prefix.hpp
- source/apps/moproj2mst/src/engine/boolean_polish_cache_prefix.cpp
- source/apps/moproj2mst/tests/mdt/test_boolean_polish_cache_prefix.cpp
- A new implementation receipt assigned at source release.

No shared WidgetEventState/WidgetRuntime, Session, CMake or public Open/Save edits are required for the isolated cut. Current WidgetEventState explicitly represents synchronous stack events and must not be repurposed as H's posted-heap identity. Existing recursive source/test globs can be verified by root during grouped configure. This plan supplies a source-conditional private cache transition, not the missing same-E/OPI first-Open occurrence or Boolean Apply/add delivery.
