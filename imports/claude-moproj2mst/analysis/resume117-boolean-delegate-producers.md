# CP117: native tree delegates are produced by Qt; size hints expose item/style callbacks

The two tree receivers used by the recovered RefreshTree connection have concrete source producers: the dock's UI setup constructs **SfTreeView**, whose Qt base constructs a **QStyledItemDelegate**. Its virtual+60 is sizeHint6553c1c0. This closes the default-delegate producer hypothesis. It does not produce a Boolean Apply occurrence: sizeHint first queries model/item data, and its fallback invokes style measurement. Those concrete callbacks, rather than an unspecified delegate, are the next source boundaries.

Only this report was written. Bounded DUMPBIN14.44.35228.0 and local PE metadata reads ran in memory. No source edit, build, tests, GUI/control, original execution or mutable REA/import occurred. The zero-timer dequeue/Open-boundary witness from CP116 remains separate.

## The receiver's construction and delegate owner

SfMoTreeViewDock constructor1401144e0 stores its UI-private pointer at dock+28 at**11460a**, then calls UI setup14011e220 with that pointer at**114622**. Setup allocates0x38 and calls **SfTreeView140d39aa0 at11f2a7**, then stores its return into **UI-private+50 at11f2e2**. It similarly constructs the other tree at**1236fe** and stores it into **UI-private+1c0 at123739**. These are the same two field paths used by CP112's RefreshTree disconnect/reconnect instructions; they are not unrelated arbitrary view allocations.

SfTreeView's constructor calls imported **QTreeView(QWidget*)654e1e20 atd39ac1** and then installs table142a0b158 atd39ad4. On the normal Qt allocation/constructor path:

1. QTreeView calls QAbstractItemView's protected constructor654a9f30 at654e1e67.
2. That constructor calls private initialization654a9e00 at654a9f65.
3. Private initialization allocates0x10 and constructs **QStyledItemDelegate6553b500 at654a9e35**, parent=the same view q_ptr; it passes the result to **setItemDelegate654a7000 at9e44**. The allocation-failure branch supplies null; no successful-null delegate is invented.
4. QStyledItemDelegate constructor installs primary table **657400b8 at6553b572**. Direct PE read gives **table+60=6553c1c0** (exported QStyledItemDelegate::sizeHint), and **table+98=6553bbf0** (the concrete style-option initializer).
5. setItemDelegate changes the view's default guarded pointer **private+288** through QMetaObject::changeGuard at654a7154, after its old/new delegate notification work. It then updates the viewport; it does not call Boolean Apply in its selected body.

QAbstractItemViewPrivate construction initializes private+288 to null and binds its guard, then places the imported Qt shared-null map object into **+290 at654ae002** and **+298 at654ae022**. These are the row and column override maps used by CP117's call-free selector654a6b50. Thus the source-produced default begins as QStyledItemDelegate; a row/column override requires an actual later writer/owner relation.

The inspected SfTreeView, dock constructor and UI-setup bodies have no direct call through the imported default/row/column delegate setter slots. A bounded encoded-call survey found other application setter sites, but none was related to these same tree owners here. This is not a complete proof against indirect setters, property/callback effects or later changes. Constructor instructions supply reached effects on their actual owner, not implicit defaults for a new candidate Session.

## The helper's early size-hint branch is not a constructor inevitability

QTreeViewPrivate constructor654e1ac0 zeros EDI at1af8 and writes **private+420=0 at654e1b37**. This is the flag tested by recursive helper654e68d0 at6a66 before its early size-hint call6aa4. SfTreeView's explicit `edx=1` call atd39ca7 is **QAbstractItemView::setSelectionBehavior**, not setUniformRowHeights. It cannot supply+420=1.

Consequently, on the constructor-produced state with no subsequent same-owner flag change, the early uniform-height size-hint call is skipped. This narrows CP117's conditional frontier; merely filling/constructing the tree does not prove that call ran. The native EXE also imports setUniformRowHeights and contains direct encoded calls at140dc23fa and141345532, but no same-owner execution of either is established for this tree. The actual flag at a delivered layout callback remains unobserved.

## Exact behavior of the concrete +60 method

QStyledItemDelegate::sizeHint6553c1c0 first requests **role13** from the supplied QModelIndex via imported QModelIndex::data at**6553c1fa**. The returned QVariant chooses three branches:

- **Valid and userType21 (QSize):** tests6553c20c/21b select a direct size copy atc225..c22e, followed by cleanup and return throughc274..c282/c374. After that model query returns, this suffix invokes neither style-option initialization nor style measurement nor Apply.
- **Valid, another type:** conversion to type21 uses the QVariant handler virtual+30 at**c259**. The handler's concrete type is an additional callback boundary; do not silently classify all conversions as pure QSize copies.
- **Invalid:** atc287 it copies the view option and invokes the actual delegate's virtual+98 at**c2a3**. For the just-created QStyledItemDelegate that target is **6553bbf0**, which queries further model roles while populating font/alignment/color/check/text/icon fields. It then chooses the option widget's style or application style, and calls **style virtual+e0 atc303**, with content type0x18, to measure the item.

These are item/view option and style objects. None is automatically the widget area's current Boolean editor or the OPI carried by Apply. No direct native Boolean Apply/add call appears in this selected Qt method; complete transitive neutrality through item data, QVariant handlers and actual styles is not established.

## The newly concrete model/item frontier

For the fresh tree model table14215e7b8, slot+80 resolves through138ad2/IAT141e0b680 to **QStandardItemModel::data65529fd0**. The method checks/maps the index to an actual QStandardItem. With an item, it preserves the requested role and calls **that item's virtual+8 at6552a069**. With no item it creates an invalid QVariant at6552a07d instead.

Therefore the **first unresolved source callback on the default size-hint path is the actual tree item's data override at6552a069**, reached by the role13 request. Model type alone does not identify that item override. The next bounded source action is to trace Boolean tree-fill item producers and their installed item vtables/+8 role handling. For the invalid-role fallback, the default delegate's6553bbf0 and selected style+e0 remain distinct later boundaries. For a real row/column override, retain the writer and returned selector identity before substituting the default6553c1c0 target.

This lane supplies no same-owner Apply/add during first Open. It also does not prove all such effects impossible: an actual item/style/override callback could reach application code, but that edge has not been recovered here. Keep the public initial-Open/PostPending guards.

## Runtime witness stays separate

Static default production is not a zero-event delivery observation. To place this route within Open, retain CP116's actual dispatcher record/0x9a notification and T lifetime, the helper's reached branch, selected delegate/cell/item lifetime and callback invocation. A Boolean effect additionally needs the nested actual current-widget Apply, E+38 resolved OPI O and normal add return295a/2bcb. A source class name, default-delegate allocation or QSize return does not supply those links. No original run is authorized or attempted by this report.

## Bounded identity and reproduction

Use the original hashes and verified DUMPBIN path in CP116 with `/disasm:bytes /range:<start>,<end-1>`. The following code-byte hashes bind the selected unwind regions; they are not complete transitive callgraph bounds.

| VA range, end exclusive | Code-byte SHA256 |
| --- | --- |
|EXE1401144e0..140115d3e|e3227f386c1a79fdd709a422c9dbc7c411a33b5c9d1229776d5fed30bbc67c83|
|EXE14011e220..140124c1b|147bf3953743f256800caacd090da715b85f9511cafbfe9a280a37c4410b0fa0|
|EXE140d39aa0..140d39cf4|4871736e72a2dc4abfad50a64e626fd53311095ad69b6cfe3dd89a875752a8f4|
|QtGui654a9e00..654a9f2e|3c1e7f22bcac95b13fc468bc5f43aeae2afd73015aeea5788b5bd694e44f099b|
|QtGui654adfb0..654ae298|4e3347aa993d886c3190e16c5f13d11cae20929ac3aaf434880be865b5837966|
|QtGui654e1ac0..654e1c3c|6580e5a2b2a7df5ff3bd5fbceb50799ffb6e8c983136dbea4217c89d1df4b450|
|QtGui6553b500..6553b588|c0a09e4ace9db6b340185e28ea7b129e08d2fb040045f899a63da5badf0a79b3|
|QtGui654a7000..654a7185|6b693bedfc0a9a6434554f559ec014672a671e7ffbcd969946bb0c857c0bf1c6|
|QtGui6553c1c0..6553c38d|6d2c5585adf345d262612ccaae95cdab78a17763e987e20b2f25eb7bf779baa1|
|QtGui6553bbf0..6553c1b7|5e847e4f5e1eaeaf8989a8e12160633a530efb0257ea7ba080ca139f730f0068|
|QtGui65529fd0..6552a0df|e0730198968b13a2a8eaa13e7d4d40306d9c617a22080b36449961433a1cb406|

