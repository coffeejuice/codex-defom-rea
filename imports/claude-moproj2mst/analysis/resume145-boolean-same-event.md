# CP145: constructor source links E to the post request, not to its later handler

The selected Boolean constructor provides a stronger source identity/order link: its QWidget base uses the same primary E, and the raw0x4a post request occurs **before** E+38's operation guard binding and the Boolean vtable installation. This does not determine when that event is delivered. A newly checked QWidget::setVisible(true) path can call ensurePolished directly, so the eventual posted event cannot assume an untouched cache or that earlier polishing came from a queue.

Only this report was written. New function/argument checks used immutable original PE bytes and DUMPBIN14.44.35228.0, with selected retained qualifications. No source/build/tests, GUI/app control, original process, import or analysis-database opening occurred.

## REA access qualification

REA binary_session responded normally and reported open=false/cleanup_pending=false. No target was opened. Root then confirmed there is no verified disposable original-EXE analyzed-database path/provider selector and prohibited creating/importing a new project or touching Claude/user-owned sessions. Retained GPRs remain untouched. This is a missing verified analysis context, not a diagnosed REA connection failure.

For future native decompilation the unresolved access prerequisite is a root-verified disposable original-EXE analysis copy, program/provider selector and binary/analysis identity manifest, with exclusive ownership and a documented read-only query/close path. Opening retained evidence GPRs in place or importing a new project was not substituted. That context could improve a static callgraph view; it still could not supply an uncaptured historical E/H dispatch occurrence. This task continued through bounded PE methods and was not blocked from the source checks below.

## Exact source object/event construction ordering

The qualified branch of Boolean factory1411e0510 allocates **0x50 bytes at11e065b**, passes that result as constructor this at11e0672 and calls **1411e2480 at11e0677**, normal continuation11e067c. The factory's separate0x58 branch calls11e3970; its different editor must not be used as this E.

The selected normal constructor sequence is:

1.11e2480 passes E unchanged to base130740 at11e24a5. That base passes its primary this to377950 at130763.
2.377950 constructs zero window flags in a local value, then passes **RCX=the same E** to imported QWidget constructor at37798a. The preceding546c70 helper is only that flag-value initializer, not a QWidget owner or allocation.
3.The import resolves to QWidget65070d70. It constructs/uses E's private widget object, installs QWidget tables and calls private initialization **6506e4d0 at65070dfa**, RCX=[E+8]. That initializer reads its q_ptr at private+8 into RSI at6506e4f9, preserving the primary receiver E for its tail.
4.On the reached constructor-post tail, heap event allocation is attempted at6506e81f. Non-null allocation is initialized with type **0x4a at6506e83c**, event table6569e350 at6506e849, then passed as **RDX=H and RCX=E at6506e851/854** to postEvent at6506e857. The null allocation branch also exists; a normal E constructor return alone does not prove a non-null H or successful posting.
5.Only after that post call returns can the tail perform private+c8 store6506e85d and stack Create destruction6506e869, then unwind the QWidget/base calls. Source ordering does not assert those callback/post continuations completed in a retained Boolean run.
6.Back in130740, setup130830 binds E+38 through cbc920 at130857 and performs its ChangeActiveStepData notification. **Boolean table142cd9b18 is installed later at11e24b7**, after130740 returns. Constructor UI setup11e2dd0 and fill11e25b0 follow later in the derived constructor.

This establishes one source-conditioned primary-address lineage and ordering, without fabricating heap H, a QObject/OPI value or a current editor. No source rule here requires H's notification to wait until E's final table is installed: any claimed delivery must retain the actual active table/lifetime at its occurrence. Base-time notification and later completed-Boolean dispatch are different source states.

## A separate same-receiver polish caller can change the cache

QWidget::setVisible(bool) is exported at65070410. Its true argument branch begins at65070437 and has widget/visibility/parent predicates before **direct ensurePolished65064f50 at65070547**, with RCX=this from RDI. This source caller does not consume a posted raw0x4a H. The retained branch alone does not prove that it ran on Boolean E, nor that its callbacks returned.

Therefore even after normal derived construction, knowing that H exists or that current metadata is Boolean does not establish the value of E-private+d0 at later65064f79. Actual source-stage cache/current-meta observations remain necessary. Direct visibility polishing, the raw0x4a handler and nested raw0x4b style callback must have distinct source-origin/frame labels. CP143's raw0x4a admission must not accept a direct ensurePolished call as if it were that handler.

## Missing occurrence and smallest source-ready cut

The unresolved live receipt remains exactly: same E allocation/lifetime and H allocation/event lifetime ->actual post/dequeue record ->notifyInternal67119570 with caller return **QtCore RVA11b32f**, RDX=E/R8=H/type0x4a ->same-thread selected QWidget branch6506cc2d, active source table/private owner and current-meta/cache inputs. Normal notify return precedes H's deleting destructor6711b33e. The zero-timer child caller RVA14231a is not this posted-notify source. Existing Forming and startup captures cannot provide this Boolean frame.

For the private cache prefix alone, the smaller handler-entry receipt from CP144 remains sufficient without claiming constructor-post origin. For public first-Open or object-creation timing, keep the complete Open interval and actual current-widget UpdateData ->Boolean E Apply11e2870 ->resolved OPI O ->normal add return295a/2bcb. Static constructor ordering does not bridge those occurrences; synchronous zero-slot postLoad creation remains separately attributed.

The smallest new source-ready cut is an **argument/constructor-stage identity projection** for the selected0x50 branch: primary E is preserved through the base/widget calls, and a post request is ordered before guard/table publication. It may retain explicit owner/allocator/post-return observations and report pending dependencies; it may not create WidgetState, H, successful post/dequeue, E+38 contents, a cached metaobject value or normal callback completion by default. CP143's already planned isolated cache comparison/store remains conditional on the handler receipt. No automatic public admission or guard change is justified.

## Reproduction identities

Original EXE SHA256fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc; QtGui SHA256c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887. Selected end-exclusive code bounds:

| Range | SHA256 |
| --- | --- |
|EXE1411e0510..1411e0736|3aa90a30675dee654f5ad165796a5a63fe229f8e7f018c4a03767279d5375747|
|EXE140377950..1403779f4|77984c579a935d969ea7eaa77aa23fcf9faedc9abf36903c23a0a36a23e86073|
|QtGui65070d70..65070e11|8ce1b92803ee6ae18541b370257fb788a17787a744bc7f87ce20c16f710ed4f4|
|QtGui6506e4d0..6506e888|bc29e925b8435cf2de9e8849037717d184af66a22331f06bbe4b3010821d871c|
|QtGui65070410..65070952|f04db89c63e2bcd517f03136db1b48bc237747d175a6b54da6faa5ba0e39620a|

The546c70 flag initializer's logical leaf ends at546c8a; adjacent bytes in its bounded view are not constructor code. There is no new native occurrence, object-creation timestamp or process-value claim in this receipt. No static/GUI session was acquired or claimed released beyond the recorded no-open REA status.
