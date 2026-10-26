# CP144: the posted-notify caller is concrete; its Boolean E/H receipt is missing

Native sendPostedEvents has a source-qualified handoff for the receiver/event pair, but retained evidence does not bind it to Boolean E and its raw0x4a event H. Candidate public lifecycle also has no reached QObject/event owner pair that can stand in for those identities. CP143's polish-cache prefix can consume a separately admitted handler entry; it cannot supply the missing post/dequeue/handler admission itself.

Only this new report was written. Bounded immutable QtCore disassembly, retained qualifications and current candidate interface reads were used. No source/shared edits, build/tests, GUI/app control, original process or mutable REA/import occurred.

## Actual caller-side source handoff

QCoreApplicationPrivate::sendPostedEvents6711b040..6711b4fe has the selected normal delivery ordering already qualified by CP51: receiver/thread and record predicates, posted/count/tombstone updates under lock, unlock, notify, event deletion, then relock/continuation. Receiver-wide cleanup is a different route and sends no notification.

The freshly re-read notify suffix identifies the exact caller receipt:

- At6711b313 it clears the event's spontaneous bit. **RDI is the retained actual event H**, not a newly allocated stack event.
- At6711b318 it loads application singleton672f4220, tests it atb31f and skips notify on null atb322 ->b330. A dequeued event alone therefore does not prove handler delivery.
- On the non-null branch, **R8=H atb324**, **RDX=retained receiver R13 atb327**, and **notifyInternal67119570 is called atb32a**, normal continuation **b32f**.
- After that callback, event H's virtual deleting destructor is called atb33e. H's lifetime/admission must not survive this deletion merely because its address remains in a diagnostic record.

Thus a notifyInternal frame with actual caller return **QtCore RVA11b32f** is the relevant posted-event source. The **RVA14231a** frame from CP123 is the dispatcher's zero-timer child notification, a different source/owner. Those caller markers cannot be interchanged.

Even a valid posted-notify frame does not by itself prove QWidget's raw0x4a branch ran: actual application notify/filter routing can consume or redirect the event. The sufficient handler-side confirmation is source PC6506cc2d with the same E/H (RDI/RSI in the qualified QWidget::event body), raw type0x4a, actual active Boolean table142cd9b18 and the same thread/frame. Only then can CP143 read its own source-stage current-meta/cache inputs.

## Current candidate boundary cannot provide E/H

WidgetPostEventQueue currently exposes observe and receiver/type removal; its header explicitly says it does not post, pump, wake or finish a constructor. A represented record removed/deleted by that helper cannot be promoted to a delivered event. WidgetEventState represents synchronous stack events and is not a posted H identity.

ConversionSession binds WidgetRuntime to its main-view/manager/split-view startup chain. Its current constructor-tail observations explicitly qualify a startup splitter; they are not an observation of a newly constructed Boolean E. Constructor PostPending still requires the actual shared queue/dispatcher continuation. No new public Boolean raw0x4a caller admission exists there.

The reached BooleanObjectEditor value in mst/loader_l5.hpp contains shown/workpieceText/cutterText. booleanEditorFill fills those selections, and Session's admitted Boolean reselection uses the loaded operation/model. Those source values do not allocate a QObject, assign an active native vtable, create a widget-private+d0 cell or identify H. In particular shown=true, operation.engineModel or the operation address cannot be relabeled E/E-private/H. BooleanSynchronousUpdateData separately requires real receiver/current-editor owner admission; it supplies no implicit raw0x4a route.

Existing WidgetState.fieldD0Present_ is only a presence projection in a guarded child-polished path. It supplies neither the cached metaobject pointer nor a same-Boolean owner. CP143 already requires canonical reconciliation before any future shared-owner integration; an unrelated startup field cannot seed that cache.

## Exact smallest receipt and safe integration condition

For the private polish prefix alone, a newly retained observation of the **actual6506cc2d same-E/H handler entry**, active Boolean source table, E/private allocation lifetimes, native thread/occurrence and current-meta/cache comparison-stage inputs is sufficient. It need not claim constructor-post origin, complete Open, OPI or Apply. That is the smallest live receipt missing now.

To attach it specifically to **constructor posting in public first Open**, additionally retain:

1. Source-owned Boolean E construction/active table transition and project/Open context, without borrowing Forming eb26f0. If OPI association is claimed, retain the actual E+38 guard binding separately; polishing does not read or manufacture it.
2. H's constructor-post enqueue record on E's actual queue/thread and record/event lifetime, then the selected normal dequeue and notify frame with caller return11b32f, target E, H/raw0x4a and normal callback return.
3. Nested same-E/H handler confirmation6506cc2d, source-stage current metaobject/cache values, and any cache-store/stack0x4b callback continuation actually claimed. Keep H alive through its admitted handler scope only; preserve later b33e deletion ordering.

Use one run/process/thread/module-hash identity, matched frame ordinals, explicit allocation/record lifetimes, probe/input/source-byte bindings, and complete error/overflow status. Timer request, wake, queue snapshot, normal cleanup, constructor table address or elapsed time cannot fill a missing frame. A cache-hit source decision is not an observed native return or Apply/add witness.

No source-closed automatic **public** admission is available from the current owner model. The safe connection is conditional: a Session-owned bridge may attach CP143 only after it owns the actual E/private/H relation and the reached handler receipt above. Until then keep it private and preserve PostPending. Applying Boolean still requires the distinct actual current-widget UpdateData ->E Apply11e2870 ->resolved OPI O ->normal add return295a/2bcb within the claimed complete Open span.

## Identity

The re-read QtCore6711b040..6711b4fe code-byte SHA256 is **0e2b4a74fe05cc290433f6f0aeaab69a5ea93e5f35f3b1011bfd91c2effa7c19**, original QtCore module SHA256bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160. DUMPBIN14.44.35228.0 and the original PE path/hash qualification remain as in CP142. Existing retained enqueue/startup and Forming probes contain no selected sendPostedEvents-to-Boolean handler receipt; this work creates none. No completion beyond an actually admitted return barrier is claimed.
