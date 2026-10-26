# CP112: postLoad tree refresh reaches a model-identity decision

The newly narrowed edge is **RefreshTree -> slotRefresh -> QTreeView::setModel**, rather than an unconditional row-zero selection or Boolean Apply. Two different metaobjects use local method index5 for different things. SfNotifier's local5 is `signalRefreshTree(SfItemModel*)`; SfTreeView's local5 is `slotNavigateToFirst()`. The actual RefreshTree connection selects SfTreeView **local0**, not local5. Substituting those indices would manufacture a synchronous Apply route.

This lane wrote only this report and unique bounded static evidence. No source/helper changes, tests, build, original process, GUI, REA database/import or mutable analysis occurred. Frozen CP112 checker files were not edited. DUMPBIN14.44.35228.0 read immutable EXE SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtGui SHA `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.

## The concrete Open-side chain

On the admitted Boolean base-load path, the existing postLoad creation returns before the views call at1405ef4da. Its callee1400547b0 first calls054820 at0547c2, then0548c0 at0547d1, and only later054df0 at0547db. These are separate ordered notifications; they must not be collapsed into a timer callback.

- **Scene payload:**054820 resolves the operation's engine model through05f770, then calls notifier2ce790 at05484d. That payload is not the Qt tree model below.
- **Tree payload:**0548c0 checks the class, reads application process type at0548ed, and only type5 calls054950 at054925. In054950 the operation's guarded **+160 Qt item/tree model** is created/bound on its null branch, filled via class+a8 and+278, then resolved again and passed to2ceae0 at**054bea**. This guard/model is not Apply's OPI or the engine GuiModel.
- **Qt emission:**2ceae0 reads notifier+141 at2ceaf3; nonzero returns without activation. Otherwise2ceb08 calls3d0a70. At3d0a96 the latter sets signal-local ID5, uses metaobject143d94cb0, and calls ordinary imported `QMetaObject::activate` at**3d0aa8**. Static initializer141ce2ea0 binds that metadata to class **SfNotifier**, strings14234dcc0 and table14234dbd0. Local5's exact string is `signalRefreshTree(SfItemModel*)`.

This is a Qt connection dispatch, unlike custom notifier2cf3c0's explicit raw type1 invocation recovered earlier. Knowing the latter is synchronous does not set the former's connection mode or live population.

## Receiver and method resolution

Dock method140116ed0 disconnects this signal from actual receivers `[dock+28]+50` and `[dock+28]+1c0` at117491/1174bf. Its selected model branch reconnects the first at**117521**, or the other branch reconnects the second at**1175e3**. Both pass stack connection type0. The PE import is ordinary `QObject::connect`; raw0 is AutoConnection here. The receiver slot string is `1slotRefresh(SfItemModel*)`. These source instructions do not certify a receiver's current lifetime, branch or thread at a later emission.

SfTreeView metadata142a6da40/strings142a6dbb0 identifies local0 as `slotRefresh(SfItemModel*)`, local5 as `slotNavigateToFirst()`. Its qt_metacall140dd7d40 delegates to local dispatcher140dd7a80. Jump-table index0 reachesdd7add and calls **d3aa60 atdd7aee**; index5 reachesdd7b52 and calls **d3b300 atdd7b57**. This closes the method distinction from PE bytes, rather than relying on the old L5 note's inferred two editor displays.

In d3aa60 the null-model branch emits custom key `2signalDeleteWidgetInWidgetArea()` atd3ac86. Both paths then reach virtual+1a0 at**d3aca6**, followed by clearing tree+28 atd3acb4. On the nonnull branch, additional model/root/column work follows. The selected d3aa60 body has no explicit d3b300 or Boolean Apply call; this is a selected-body fact, not transitive absence through Qt effects.

RTTI type descriptor143972520 / primary COL1430f2730 identifies SfTreeView table142a0b158. Its +1a0 entry is138d00 ->IAT141e06fa0 ->**QTreeView::setModel(QAbstractItemModel*)**, exported at**654e3220** in the retained QtGui. Its +1a8 entry similarly resolves to QTreeView::setSelectionModel. Actual receiver dynamic type/override remains a binding prerequisite; this table must not silently stand in for an uncaptured different receiver.

## A source-closed decision inside that call

At654e322f, QTreeView::setModel reads `private=[this+8]`; at654e3239 it reads the currently installed Qt model from **private+280**. The incoming model is RDX. Comparison654e3240 / branch654e3243 sends equality directly to epilogue654e340b..654e341a. That equality path makes **no call and no model store** inside setModel. It therefore cannot itself deliver Apply. This is a complete, bounded local decision.

On inequality, the separately retained continuation654e3249..654e340b performs old-model/selection disconnections, header-model work at654e332d, and a further call at**654e3339 ->654a48a0**, with more connection work afterward. This is where the next static callback analysis belongs. It is no longer an unspecified question about every Open callback.

Equality is **not** a no-op guarantee for the entire slotRefresh: after setModel returns, d3aa60 still clears tree+28 and performs its later model/root/column operations. Neither equality nor a RefreshTree emission proves an actual first-Open Apply absent or present. The actual incoming SfItemModel, installed Qt model, receiver/table, suppression and connection state have not been observed.

## Next bounded action

Trace **654a48a0 and the selected selection/current-index effects reached from654e3339**, including the resulting SfTreeView slot relation, before revisiting timer scheduling. In parallel, qualify the actual first-Open RefreshTree receiver/model identities if new retained evidence becomes available. Any source implementation of the closed equality prefix must consume explicit known Qt receiver/private/current-model identities and stop on unknowns; the engine GuiModel or a new Session is not that owner.

No helper was added because this lane closes the comparison leaf but does not supply a production Qt-tree owner or decide which original branch first Open selects. Keep public initial-Open/PostPending guarded. This report narrows the outstanding source edge and its exact private-state read; it does not repeat CP105's selector design or reinterpret a candidate-declared schedule as native timing.

Evidence: `resume112-boolean-postload-routing-pe.json` SHA `d628182c60845cb81609ad1dd0711b59bf4d6e53adfb1970cfc0153a4c3b3cc2` retains metadata, RTTI/table, import/export and file-backed byte/hash bindings. Unique `resume112-boolean-postload-*` / `resume112-boolean-tree-*` static receipts bind their commands and disassemblies, including split Qt unwind regions3220..3249,3249..340b and340b..341b. Earlier054950 and class-load/Boolean qualifications remain referenced rather than duplicated. Unwind bounds delimit selected regions, not entire transitive behavior.

