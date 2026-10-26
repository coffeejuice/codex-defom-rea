# CP67: QWidgetPrivate item-cache owner and synchronous cleanup seam

The additional source-closed prefix is QWidgetPrivate destruction's direct nulling of its cached item's widget field. The field at private+0xc0 is a per-widget nonowning QWidgetItemV2 size-cache link. It is neither the layout's owning item slot nor a process-global cache. Implementing this prefix needs an actual private-allocation/cache owner; the current public WidgetState liveness flag alone is insufficient.

This report is the sole write. No source/build/tests/GUI, original execution or mutable REA operation occurred. CP65 source remained frozen. Method: retained captures/current source and bounded offline PE/DUMPBIN. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`; QtCore4_64.dll `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. DUMPBIN14.44.35228.0 from MSVC14.44.35207 supplied the new bounded views, retained inline here.

## Exact three-field graph

| Native location | Meaning and source behavior | Ownership consequence |
| --- | --- | --- |
| Main-window layout+4f8 | Its status QLayoutItem pointer; replacement explicitly deletes the old item then stores the new one | Layout owns item storage. |
| QWidgetItemV2+10 | Raw QWidget pointer returned by651a4ce0 | Item does not own/delete the QWidget. |
| QWidget.public+8 -> QWidgetPrivate+c0 | Cached QWidgetItemV2 identity for that widget | Private does not own/delete this item; it invalidates/back-unlinks it. |

Root must retain these as separate fields of the same actual candidate owners, not invent a second item/widget to break the nonowning links. Empty weak metadata is not a known-null native pointer.

QWidgetPrivate(int)6505e780 first invokes its QObjectPrivate constructor, then initializes instance fields. At6505e7e3 it writes+c0=null. QStatusBarPrivate ctor65406880 explicitly calls this normal base constructor at6540689b with argument0x40804 before installing its own private vtable6570bd18 and constructing its list/QString members. This qualifies the null value **at that constructor event**, not at late Close after later widget/layout activity. The separate QWidgetPrivate copy constructor650116a0 copies source+c0 to destination+c0 at65011736/6501173d; do not apply the fresh-int-constructor null rule to that path.

QWidgetItemV2 ctor650494b0 stores its actual widget at item+10, installs table656a58c8 and initializes caches. At65049543..65049555 it reads widget.private+c0 and publishes this item there only if the cell was null. A different existing cached item is retained. This publication is not a QObject parent/ownership transfer.

The geometry provider also uses this same link: QWidget::updateGeometry65062590 forwards its private pointer to65062460. That helper reads private+c0 at65062486; nonnull calls QSize::setWidth(item+18,-123) through import65696e90 and clears word item+4a at650624a4 before later geometry/layout work. This independently establishes the cache's size-hint role. The rest of that geometry provider is not closed by this prefix.

## Two different destructor operations

CP66 already qualifies item destruction: if item.widget is nonnull, inspect that widget's private+c0; clear it **only if it equals this item**. Then install base QLayoutItem class and, for the deleting scalar profile, free only item storage. A different cached item is preserved. No QWidget destruction follows from dropping the item.

The newly bounded QWidgetPrivate destructor650676d0 does the reverse:

```text
650676f2  install QWidgetPrivate vtable6569c070
650676f5  load this.private+c0 into rax
650676fc  test rax
650676ff  null skips to65067709
65067701  nonnull: store qword [cachedItem+10]=0
65067709  next private extra/member cleanup boundary
```

There is no virtual call, cached-item deletion, old widget-field read or backlink comparison in this prefix. When the cache target is known, it writes that exact item's widget field null regardless of its previous value. It does **not** null private+c0 itself before subsequent private-member cleanup. A model must not add either an old-backlink equality precondition or a convenience cache reset.

This supplies a real source operation for CP65's KnownNull item-widget observation: a surviving cached item can remain after its widget/private teardown with item+10 explicitly null. It does not mean all items referencing the widget are nulled. For example, when constructor publication preserved an earlier cached item, only the actual+c0 target receives this private-destructor store. A dead/expired weak widget reference on another item must still remain unavailable, not become native null automatically. Actual layout-removal/destruction order determines which items survive; that population is not assumed here.

The native cache cell and target must be known independently. A known-null cell skips the target store. A known nonnull identity with an unresolved/dead item allocation cannot be treated as null or replaced by an invented item; guard before the direct write.

## Actual QStatusBar private provider sequence

QStatusBarPrivate ctor installs table6570bd18. Its first virtual points to65406b60. That deleting provider:

1.Destroys the **instance** QString at private+1e8 at65406b84.
2.Decrements the list backing at private+1e0 and conditionally qFree's that backing at65406ba3.
3.Calls QWidgetPrivate::~QWidgetPrivate650676d0 at65406bad.
4.Only after base-private cleanup returns does flag&1 permit freeing the private allocation.

Imports65696760/65696978/65696960 bind QString::~QString, QBasicAtomicInt::deref and qFree. The cache-backlink prefix therefore cannot be used to bypass the preceding status-private string/list effects on a real QStatusBar destruction path. Once the actual base-private entry has been reached, its cache nulling is synchronous and precedes65067709's further extra/member cleanup and later QObjectPrivate teardown.

Public QWidget/QObject lifetime and private-allocation lifetime are distinct. The actual QObject destructor eventually dispatches through the dynamic private provider, as qualified by the retained QObject cleanup contract. Public guards/destruction signals and earlier QWidget/status payloads are not implied by the base-private cache prefix. A public `WidgetState::nativeOwnerAlive()==false` cannot, by itself, mean private memory is already unavailable; nor does a diagnostic C++ lease prove that native private storage survives. Root needs a private lifetime/phase identity through this call.

For normal one-project Close, none of this is an unconditional next operation after a successful status getter. A valid existing QStatusBar is preserved and receives showMessage; replacement/deleteLater and actual event delivery have their own conditions. The old **layout item** may be synchronously deleted during replacement before the old widget is later destroyed, in which case item destruction already cleared its matching widget cache. Earlier DeleteWidgetInWidgetArea may destroy other widgets; its actual recipients remain required evidence. Do not insert widget-private destruction simply because Project Close was accepted.

The instance QString/list cleanup above is separate from process-global MessageArea QString1446b3bb0. It supplies no normal-Close/process-global reset. CP64's duplicate-message cache reset guards remain correct.

## Missing owner interface and smallest implementable contract

Current `WidgetState` in `include/m2m/engine/view_manager.hpp` has the actual candidate widget identity/runtime and public native liveness, but no QWidgetPrivate allocation-phase identity or+c0 item-cache field. `CloseStatusItemOwner` in `close_status_owner_prefix.hpp` has the actual candidate item and Unknown/KnownNull/KnownWidget raw widget-field projection; its `observeNullWidgetField` can represent the value store once its real producer is bound. `GridLayoutItemState` is currently only an empty layout-item base; it is not a populated common QWidgetItemV2 identity implementation.

The next root-owned seam should place an Unknown/KnownNull/KnownItem nonowning cache cell on the **actual widget-private owner**, associated with its same public WidgetState. KnownItem must resolve to the same concrete item identity used by its layout and widget field. A status-only duplicated item token is insufficient when the cell names another layout's item. Opaque other identity may suffice for the item-destructor comparison/preserve branch, but private destruction needs the actual target's writable+10 field or must guard.

An isolated base-private cleanup helper can then:

1.Require a qualified entry at650676d0 for that live private allocation, independently of public QObject liveness. Do not certify preceding status/QString/list/Widget/QObject effects with a detached success Boolean.
2.Commit the private class transition, then retain the cache read's identity. Unknown cell or unresolved nonnull target stops without substituting null or rolling back the completed class phase.
3.KnownNull skips. KnownItem writes that actual item's widget field KnownNull, without reading its old widget value, deleting item/widget, or clearing the private cache cell.
4.Stop before65067709's remaining private payload/free boundary. Retrying must not repeat a completed target store or retarget a later cache replacement.

Constructor-default null and V2 publish-if-empty can be separate explicitly qualified owner events; they must not make late state implicitly null. Geometry invalidation needs its own actual item cache fields/effects. Actual integration belongs in root's shared WidgetState/private-owner interfaces; this evidence lane makes no source edit.

Focused future tests: null/unknown/cache-target-unavailable; actual cached-item write with arbitrary prior widget-field state; cache pointer retained while the target widget field becomes null; a different item unaffected; item-destructor matching versus different-cache behavior; same item identity shared with layout; private still live after the public boundary; and retry retaining consumed identity. Test the base-private seam explicitly rather than counting it as completed original Close or destructor parity. No tests were created or run here.

## Evidence receipts

New disassembly was retained inline; no additional raw files were written. Reproduce using DUMPBIN `/nologo /disasm /range:<entry>,<end-minus-one>` against retained QtGui4_64.dll. The existing `wave3-closure-next-6505e780.asm` supplies the base constructor capture; CP65/66 supply item ctor/dtor and enclosing status/Close contracts.

| End-exclusive native region | Code SHA-256 | Scope |
| --- | --- | --- |
| 6505e780..6505e9d3 | 89f63a54971317ee9299fa28ef522a28f7392c472e1ca5bfec7fc1635e0e617f | normal QWidgetPrivate ctor |
| 65406880..654068de | b9dacb181b9906664ba8d9185b39d81c3a72621881460ce4ceb32e21bea0d4ec | QStatusBar private ctor provider |
| 650116a0..65011abf | 945492af954916fe07e453127471a4b77cb1401492d1db2cf24031675479ecaa | copy-path exclusion; raw+c0 copy |
| 650494b0..65049579 | b95d4668336c234ed541c1a2a12b6c3830c9ca732d634d3d27cda58bb8004608 | V2 publish-if-empty |
| 65062460..65062581 | 19c23015e40a84a175b02cc00efc734aa4e18ad99ffc8e183a2b1b57c23d1bc2 | geometry cache consumer |
| 650676d0..65067836 | 4afbd49bcc764c00b68cb5b80adc7d0930be75fc2a013709b84c8441015197cb | base-private destructor/cache write |
| 65406b60..65406bd3 | 94c98f2fbfefe601f7043e57ba43adb44853bfff9246adc09ca9ef5e26b6cd73 | dynamic status-private deleting provider |

Unwind bounds qualify selected direct regions, not all transitive payload behavior. Native main/layout/widget/private/item bindings, destructor reachability, remaining Qt/provider effects, deferred delivery and one-project lifecycle acceptance remain unresolved.
