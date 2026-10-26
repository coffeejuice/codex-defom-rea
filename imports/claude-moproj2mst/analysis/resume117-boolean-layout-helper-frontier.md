# CP117: recursive layout has a closed invalid-index exit and a delegate frontier

A bounded recursive branch returns without any model/delegate invocation or Boolean effect. For the fresh Qt tree-model table, lazy `fetchMore` is also ruled out by its inherited constant-false `canFetchMore`. The populated root branch remains different: an enabled uniform-height path reaches actual cell-widget/delegate size-hint callbacks. No same-owner Boolean Apply/add edge has been established through those callbacks.

Only this report was written. DUMPBIN14.44.35228.0 inspected bounded original PE regions in memory; no source, GUI/control, original process, mutable REA/import, build or test was used. CP112/114/116 evidence and frozen tooling were unchanged.

## Complete bounded termination branch

CP116's outer layout calls helper **654e68d0** with private pointer and parent view-item index `p=-1`. Recursive calls at654e6fb7 can instead supply `p>=0`. The distinction matters: a negative root index deliberately bypasses the invalid-recursive-index return below.

For **p>=0**, helper654e6987 calls **654dd1e0(private, output, p, column=0)**. This getter:

- Reads the view-items container at private+410 and its count at container+8, comparing p at**654dd20e/212**.
- If p is out of range, writes an invalid QModelIndex `(row=-1,column=-1,internal=0,model=0)` at**654dd2a3..2b4** and returns.
- If p is in range, copies that entry's stored index. Because the caller supplies column0 at654e6979, it skips the getter's imported sibling-query call at654dd257; this selected getter path makes no call.

Back in654e68d0, if p>=0 and the returned index has row<0, column<0 or null model, tests**654e69d5..69e5** branch at**69e7 ->7155**, ending at RET**7160**. Only stack/local output memory has been written on this path. The first model virtual call at6a02 is not reached; neither are delegate queries, layout-vector changes, recursion, selection dispatch or Apply/add. This is a complete normal source branch, not a claim that a real first Open selected it.

The top-level `p=-1` call cannot use this guard to certify Open neutral. A future isolated projection could test the exact p>=count/invalid-index exit with explicit known view count and index fields, but no new helper or public admission was added here.

## A fresh native model cannot use this helper to fetch more objects

After the guard, the helper asks model+78 `hasChildren` at**654e6a02**. If false, its row count stays0. If true, it asks model+f8 `canFetchMore` at**6a1b**, conditionally calls model+f0 `fetchMore` at6a37, then asks model+68 `rowCount` at6a4f.

For the source-qualified fresh tree model constructed by1400c4890, table**14215e7b8** resolves these slots as follows:

| Slot | Static target |
| --- | --- |
|+78|138a60/IAT141e0b750 ->QStandardItemModel::hasChildren6552a180, the call-free query qualified in CP116|
|+f8|1386d0/IAT141e03d90 ->QAbstractItemModel::canFetchMore**670c85c0**: bytes`32 c0 c3` (false; return)|
|+f0|1386ca/IAT141e03d98 ->QAbstractItemModel::fetchMore**67001000**: bytes`c2 00 00` (return)|
|+68|138a54/IAT141e0b760 ->QStandardItemModel::rowCount6552a360|

Thus the fresh-table branch does **not** admit the fetchMore call at6a37, and that inherited target itself is empty. There is no recovered engine-object creation from a supposed lazy-fetch step here. Actual model/table lifetime must still be bound; these slots cannot describe an unknown replacement model by default.

## Populated root path: exact callback frontier

For p==-1, private+420 nonzero at**654e6a66/6a6d** causes model index(0,0,parent) at6a94, then calls **654e09a0 at6aa4**, storing its height result at private+41c. With private+420==0 this early size-hint call is skipped. Root/vector setup, row visibility/expansion checks and conditional recursion then proceed; they are not an invocation of the application's NavigateToFirst handler.

654e09a0 first rejects a negative row, negative column, index model unequal to installed private+280, or null default delegate private+288 at**654e09d0..09fd**, returning0 through0d61..0d76 before its rich callback suffix.

For an admitted index/delegate state, it can query an existing cell editor's size at **654e0bfb**, after lookup654a8e60 and guarded/persistent-editor membership checks. That cell editor is a Qt item-view cell owner; it is not automatically the widget area's current SfObjectSetupBool.

The other exact dynamic boundary is **654e0ca4 ->654a6b50**, then delegate virtual+60 at**654e0cc1**. Selector654a6b50 chooses:

1. A matching row delegate from private+290 (row key read at6b5c), returning at6bac..6bb5.
2. Otherwise a matching column delegate from private+298 (column key at6bb6), using the same return.
3. Otherwise default delegate private+288 at**6c0c**.

These maps/pointer are actual Qt view-private state, separate from current editor/OPI guards. The selector's inspected body has no call instruction. Choosing its returned delegate does not establish which concrete size-hint override executes or what that override does. QtGui's exported itemDelegate(index)654a7310 and setItemDelegate/ForRow/ForColumn interfaces identify the relevant owner family; this lane has not qualified their actual producer/writer history for the selected first-Open T.

The populated helper eventually recurses at654e6fb7 only on its selected expansion path. No direct Boolean Apply/add call is present in the inspected outer helper. That selected-body fact does not prove transitive neutrality through model items, cell editors, delegate overrides, styles, geometry or reentrancy.

## Separate next source and runtime work

The next **source** action is narrowly scoped: trace the selected SfTreeView instance's writers/producers for **delegate default+288, row map+290 and column map+298**, and resolve the concrete virtual+60 target at654e0cc1 (plus the guarded cell-editor size target at0bfb if that branch is reached). A source-qualified default delegate cannot stand in for a populated row override. The closed canFetchMore/invalid-index branches need no renewed broad callback census.

The next **runtime** witness remains separate: establish actual zero-layout delivery and helper entry within the correctly selected Open span, then retain p, relevant branch PC and actual cell/delegate lifetime if following that callback. For a positive Boolean creation claim, correlate any nested real Apply1411e2870 with its own E+38 resolved OPI and add normal return295a/2bcb. A Qt tree-model/delegate pointer, a layout return or an equal numeric timer ID supplies none of those links. The existing owned-session/release gate still controls any original observation; no run is authorized or attempted by this report.

## Bounded evidence identity

Immutable module hashes remain those in CP116: EXE`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, QtGui`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`, QtCore`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Reproduce with verified DUMPBIN `/disasm:bytes /range:<start>,<end-1>`. Bounds are unwind regions except the two explicitly bounded leaf/padding views; terminal instructions were inspected.

| Selected VA range, end exclusive | Code-byte SHA256 |
| --- | --- |
|QtGui654e68d0..654e7161|c2655d02709066d12ead5ccf1d968730455cdec4cd4367312112c637f0adaeb3|
|QtGui654dd1e0..654dd2c1|3672d9605ea62da18421838c81b10a91eb6e0b9173ebfa38793a12622abcd9f3|
|QtGui654e09a0..654e0d77|7f51eafff0b6dd002ff893ef0160e56cb3d4171298be3ee2f19b32970d960044|
|QtGui654a6b50..654a6c19|2a08d59f293be8402801eebc4f2cb535e16c96bf62ec6854f791ee2953ddb53b|
|QtGui654a8e60..654a8f74|208f4e1cee3e584805ba62f4a3b0b0d78b0a8964c387a69085b9184efe35d191|
|QtCore670c85c0..670c85d0 (explicit16 bytes incl padding)|9aa0858555b2f0dcfb2e62c18e238df28fbaf81f25346fcc428385196cd18139|
|QtCore67001000..67001010 (explicit16 bytes incl padding)|28f844b5972c241e804231b0e41b93e13696e59b98dde74f19029386db5da0c5|

