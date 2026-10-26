# CP114: model replacement does not imply a current-index Apply

The unequal-model route reaches a new selection owner and a reset, but it does **not** immediately call the application's row-zero/Boolean Apply handler. `setSelectionModel` registers current/selection notifications; native `QItemSelectionModel::reset` then blocks that sender's signals while clearing. A separate root-index operation can request a zero-delay layout timer. These findings narrow the synchronous versus scheduled edges without assuming which branch a real first Open selects.

Only this report and bounded static evidence were written. No shared/source/helper edit, build, test, original execution, GUI, REA import or database access occurred. DUMPBIN14.44.35228.0 read the same immutable EXE and QtGui binaries qualified by CP112. The CP112 witness checker remains frozen.

## Exact replacement sequence and guards

CP112's QTreeView call at654e3339 reaches **QAbstractItemView::setModel654a48a0**. Its selected body establishes:

| Condition / source site | Native effect |
| --- | --- |
|Incoming RDX equals current private+280 at654a48cf/48d2|Return at4caf..4cc3; no replacement suffix. This is a second read/check after QTreeView's earlier header-model call; do not substitute the earlier comparison as the later observed value.|
|Old model nonnull and not Qt's static empty model,48d8..48ed|Disconnect its listed model notifications before replacement. The disconnection calls are earlier than any new selection model.|
|Incoming model null at4a57/4a5a|Resolve staticEmptyModel at4a5c, then store the normalized model to private+280 at**4a65**. A null argument is not retained as the installed model here.|
|Normalized model differs from staticEmptyModel at4a72/4a75|Register its model-change notifications. Empty-model normalization skips those connections but still joins selection allocation at4c3f.|
|Allocation succeeds at4c4e/4c51|Construct **QItemSelectionModel654f1bd0** at**4c60**, with model from private+280 and parent equal to the view. Constructor installs its native table657330d8 at654f1c2d and binds its guarded model in654f0c00. This is a Qt selection owner, not OPI or GuiModel.|
|After allocation branch joins|Connect model destroyed to new selection deleteLater at4c8c, call view virtual+1a8 at**4c9b**, then view virtual+1e0 at**4ca8**.|

A failed allocation sets the local selection pointer to null at4c6a and still enters the following native calls. This report supplies no successful-null or rollback interpretation. QObject construction includes the parent-dependent call at654f1c1f; no complete transitive neutrality of constructor/event effects is claimed.

On the retained **SfTreeView** primary table142a0b158, +1a8 resolves to QTreeView::setSelectionModel654dd3d0 and +1e0 to QTreeView::reset654e5790. +220/+228 resolve to QTreeView's own selectionChanged/currentChanged implementations, not application Boolean methods. These mappings remain conditional on the actual receiver retaining that source-qualified table/lifetime.

## Selection installation registers callbacks; it does not call them

QTreeView::setSelectionModel first updates its header's selection model at654dd41d, then calls **QAbstractItemView::setSelectionModel654a4d00 at654dd429**.

That function reads the supplied selection's model through654f1530 and compares it to the view's installed private+280 at**654a4d21**. Mismatch takes the warning/tail-return at4d2a..4d40, before replacing the view selection guard. Matching proceeds as follows:

- Disconnect old selectionChanged/currentChanged relations if private+2a0 resolves nonnull,4d47..4d82.
- If old and supplied selection pointers differ, call **QMetaObject::changeGuard** on private+2a0 at4d9b.
- If the resulting owner is nonnull, connect `selectionChanged(QItemSelection,QItemSelection)` to the view's matching slot at**4dc6**, and `currentChanged(QModelIndex,QModelIndex)` to its matching slot at**4dec**. Both pass raw connection type3; neither is an activation or direct invocation instruction.
- Return at4e01. There is no immediate currentChanged virtual call in this selected function.

This closes the mistaken shortcut “selection owner installed, therefore selection callback/Apply ran.” It does not assert that every other receiver relation or reentrant callback in the process is absent.

## Reset's current-index activation is suppressed

QTreeView::reset654e5790 performs its local cleanup and tail-calls **QAbstractItemView::reset654b0790** at654e57de. The latter constructs an invalid QModelIndex at654b0917..092c and calls view virtual+1e8 at**654b093d**, then, if selection guard private+2a0 is nonnull, calls selection virtual+70 at**654b0953**.

For the newly constructed native QItemSelectionModel table657330d8, +70 is **reset654f0620**, and +68 is **clear654f2590**. Reset:

1. Calls ordinary imported `QObject::blockSignals(true)` at**654f062f**, retaining its returned prior flag.
2. Calls virtual clear+68 at**654f063e**.
3. On its normal return, restores the prior flag with the blockSignals tail-call at**654f0651**.

Clear replaces its private+A0 persistent current-index holder with an invalid index at654f2608. It only attempts current/row/column activation when the copied old index has nonnegative row and column and nonnull model, tested at**654f260f/2619/2623**. Those activations at265e/2694/26ca occur inside reset's blocked-signals interval. The retained Qt activation contract's sender block gate (6712ed15, mask04) prevents receiver delivery. Thus even a formerly valid current index does not make this native reset a delivered Boolean Apply. Normal-return restoration is source-qualified; no exceptional cleanup/rollback has been invented.

## A distinct zero-delay request precedes that blocked reset

SfTreeView's +1e8 maps to QTreeView::setRootIndex654dd390. It updates the header first at654dd3ae, then tail-calls **QAbstractItemView::setRootIndex654a4ef0**. The invalid index supplied by reset passes the source validity checks without a foreign-model warning. The setter updates persistent root private+308 at4f31. If layout-pending byte **private+3a2 is zero**, it writes1 at4f4d and calls **QBasicTimer::start(0, q_ptr)** at**4f54**, using timer storage private+3a4. If the byte is already nonzero, this request is skipped.

That is a separate layout timer, not the diagram receiver's1000ms singleShot. `start(0)` is a request; no event-loop pump or inline timer callback is present in this selected root-index setter. It must not be converted into a due observation or an Apply count. The header's virtual root-index update precedes it and remains a distinct callback-sensitive owner edge.

## What this resolves for first Open

The fixed native **selection-installation plus selection-reset suffix supplies no unconditional same-owner Apply/add call**. The two explicit selection connections target inherited Qt view slots, and reset's attempted current-change emissions are blocked. This is a source-closed reason to reject that proposed automatic trigger, independent of the diagram timer's HWND/serial state.

It is not a proof that the whole unequal-model operation, or first Open, has no Apply. Earlier model disconnects, parent-dependent QObject construction, header model/selection/root virtual calls, and existing cell-editor cleanup during AbstractItemView::reset precede the blocked suffix. In particular reset iterates its private+2c0 editor structure and can call an actual child editor virtual+58 at**654b0881** before invalidating root/resetting selection. Those cell-editor identities must not be equated with SfObjectSetupBool. After the suffix, the caller's remaining layout/model operations and any actually delivered timers remain separate routes.

The next bounded static target is therefore **the zero-layout-timer handler/ID comparison reached from private+3a4**, together with the SfTreeView/header root-index virtual relation at654dd3ae. For a synchronous positive Apply through an earlier callback, retain its exact parent/header/editor route into the existing application UpdateData/current-Boolean chain; none is established by the selected internal selection connections. Do not add a second initial-Open Apply or enable PostPending from this analysis.

Evidence: `analysis/resume114-boolean-selection-routing-pe.json` SHA `b100422175324dc495dc2e3c2f7b20913193edaf7aa26b587bd7ef71cef92f80` binds EXE/QtGui hashes, import/export names, table entries and file-backed bytes. The unique `resume114-boolean-*-static.json` receipts bind all retained disassemblies and commands. CP112 supplies the preceding RefreshTree/local0 route and source table qualification; CP72 supplies the Qt sender blocked-signal activation gate. Selected unwind regions and these callback summaries do not establish complete transitive callgraph absence.

