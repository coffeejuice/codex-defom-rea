# CP142: Boolean E's posted event and diagram timer have different callback tables

The deeper static route closes an owner distinction: Boolean editor E inherits QWidget::event and an empty QObject::timerEvent. Its constructor's raw0x4a event enters widget polishing, whereas the diagram's1000ms request creates a separate QSingleShotTimer S whose timeout connects to diagram D. A timer event merely targeting E cannot stand in for S's timeout/modelUpdated callback or for E's distinct Apply slot. Actual delivery/nesting remains uncaptured.

Only this new report was written. Bounded immutable PE/DUMPBIN reads and selected retained timer qualifications were used. No source/build/tests, GUI/app control, original execution or mutable REA/import occurred. The separate CP142 null-removal plan remains a plan, with no implementation.

## Boolean receiver table after its constructor's base return

Boolean constructor1411e2480 calls base130740 at11e24a5, then installs primary table **142cd9b18 at11e24b7**. Direct table/import/export reads give:

| Slot on that actual table | Target |
| --- | --- |
|+0 metaObject|1411e4f00|
|+20 event|14013898e ->IAT141e0ba58 ->QWidget::event6506c950|
|+30 timerEvent|14013862e ->IAT141e04418 ->QObject::timerEvent67001000|
|+1c0 Apply|1411e2870|

QObject::timerEvent67001000 is RET0. QWidget::event's type1 falls through to QObject::event; its timer case calls the actual receiver's+30 at6712cb49. Thus on this table the final E timer slot does not call Apply. This bounds that selected body only: earlier event filters/notify callbacks remain separate possible effects.

Base130740's setup130830 binds the incoming operation/OPI guard **E+38 through cbc920 at130857**. The setup also emits custom `2signalChangeActiveStepData()` at130ad1, with type literal `SfStepData *` and the separately resolved engine model. That is neither widget-area UpdateData nor a timer delivery. During this base setup, installed table1421a8938 has+1c0=131110; the Boolean Apply table has not yet been installed. A virtual+1c0 call made under that known base table must not be relabeled Boolean11e2870. This does not exclude a separately explicit call to Boolean code from an untraced callback.

At any real dispatch the current vtable/lifetime must be retained. Reusing E's numerical allocation address does not prove whether base construction, completed Boolean construction or destruction is active.

## Raw0x4a reaches polishing, with a concrete cache predicate

For QWidget::event6506c950, the main type dispatcher uses byte table6506d924[type-2] and target table6506d850. Type0x4a chooses index30 ->6506cc2d and calls **QWidget::ensurePolished65064f50 atcc30**. The preliminary enabled-widget filter table routes0x4a to this same main dispatcher, not its early false-return case.

ensurePolished calls the receiver's metaObject virtual0 at65064f77 and compares it with **widget-private+d0 at64f79**. Equality branches64f80 ->6506509b and returns without the following cache write, Polish send or child traversal. On E's installed table, metaObject1411e4f00 obtains its private pointer through0d8a50, then returns private+28's dynamic metaobject when non-null or static142cde640 otherwise. That accessor's only further helper call004b80 is a RET leaf; no Apply is in the selected metaobject accessor.

If unequal, ensurePolished first writes the actual metaobject to private+d0 at64f86. It constructs event **0x4b** at64f97 and calls **QCoreApplication::sendEvent(E,event) at64fa6**, then traverses a copied child list and can send a parent event. Reaching the cache store is not equivalent to completing these callbacks.

Raw0x4b chooses main-table index31 ->6506cc3a. It obtains the actual style through QWidget::style atcc3d and calls that style's **virtual+68 atcc4b**, passing E. It then performs attribute/list/widget work. These are style/child/parent callback recipients; no direct E+1c0 invocation is supplied by the selected0x4a/0x4b entry blocks. Transitive effects through the actual styles and event filters are not exhausted here.

The smallest source-closed event cut is therefore conditional: a separately admitted same-E event entry plus known actual current-meta/cache equality can select the no-polish suffix. Unequality admits the cache store before a guarded raw0x4b send. Neither branch supplies the original queue dequeue, actual equality value or an observed normal callback return. Constructor PostPending remains necessary.

## Diagram timer's actual callback owner is S, then D

Re-read registration170430 confirms1000ms `singleShot(...,this,1modelUpdated())` at17085f, literal1421d7e18. The non-null branch's receiver is the actual diagram D, not Boolean E or its OPI. Qt constructor67134870 installs **QSingleShotTimer table6723dad8 at671348b7**, connects `2timeout()` to the supplied D/member at348d3 with raw type3 (normalized0 by QObject::connect), and starts its native timer at348dd.

That installed table's **+30=67134ca0**. The handler kills its stored positive ID at34cb0, writes ID=-1 at34cc5, activates timeout via6712ec70 at34ccc and tail-deletes S. Actual activation still needs sender/receiver/thread/connection gates qualified by CP80; registration or handler source order does not establish an actual modelUpdated call.

The diagram MOC route needs a precise refinement to CP80's shorthand: local23 ->14018c194 **loads D's table and calls virtual+278 at18c1a1**. It is not a direct unconditional call to183160. Source constructor16f870 installs SfDiagEditor table1421d8e98 at16f8ae; constructor14ef60 calls that base and installs SfDiagCellEditor table1421cded8 at14ef8c. Both inspected tables have+278=140183160. Their qt_metacast literals independently identify the two classes. Thus183160 is the source-qualified target on those tables; another/current/unbound table must not be silently substituted. The callback then uses current model/item after fit as previously qualified, not a registration-time Boolean item snapshot.

## Exact missing same-owner occurrence

For E's post path, the missing frame is actual Boolean E/OPI construction and guard binding, receiver/heap-event lifetime and raw0x4a enqueue/dequeue on its actual queue/thread, active E table at dispatch, metaobject/cache inputs and any nested raw0x4b/actual style callback, all inside complete first Open. Forming Editor004 and an unrelated startup queue cannot supply E's lifetime, OPI or dispatch state.

For the1000ms path, retain D's registration ->actual S/native timer ID and dispatcher-record lifetime ->S timerEvent34ca0 ->actual timeout dispatch to the same live D ->D table+278/183160 and normal fit return ->current item/editor selection. A zero-layout timer on tree T is another receiver/record and cannot replace S. No static callback-table target or elapsed1000ms value selects that schedule.

A positive Boolean creation claim additionally needs nested actual current-widget UpdateData ->E Apply11e2870 ->E+38's resolved OPI O ->normal add return11e295a/11e2bcb with O before the same complete Open finishes. Keep zero-slot synchronous postLoad creation11df0e9/return0ee separately attributed. Current private property-refresh/clear/null-removal models admit none of these native occurrences. The safe next action is a narrowly bound observation at the identified owner-specific route, or an isolated conditional polish-cache prefix; no public timer/Apply activation is justified.

## Reproduction identities

Use original module hashes and verified DUMPBIN14.44.35228.0 path from CP135/CP120. Selected code bounds (end exclusive):

| Range | SHA256 |
| --- | --- |
|EXE1411e2480..1411e2544|c936cee30197093aef78c4c440a0864e37ebd983a41e02ae2226e87d75753b90|
|EXE1411e4f00..1411e4f4d|24f23901e7bde7e66339b1d4302fa20863745b613430edefadde35656fb89eb5|
|EXE140130830..140130ae0|608b09364e69d463491cc8d205ea0022bf5c9b9cde168949fb01a24658e3188c|
|QtGui6506c950..6506d9e9, includes dispatch data|b73ef66751097ecb85253e5e516888a27fb0c220980593848ecceb4a7d29bba2|
|QtGui65064f50..650650b1|9535edea24baaef6d1c00e784b081984baaa3aaf894a0c4c9ddc7242665fe090|
|QtCore67134870..671348fd|5c18bff0069361403edf333affc137fb21dae5a0fde9d0939e7c2792fc7b954c|
|QtCore67134ca0..67134cde|0475c3932940d2e3a86ad251a410482ef8e37e4491276d02352258f9f1fab27b|
|EXE14018bb30..14018d168|4989a481069b930095accfcc54f7b2534e84c0666e0af124733db0f7e5664575|

The two imported EXE thunks are six-byte jumps; bounded views containing adjacent thunks are not treated as additional method code. QObject::timerEvent's complete selected body is the three-byte RET0 at67001000..67001003.
