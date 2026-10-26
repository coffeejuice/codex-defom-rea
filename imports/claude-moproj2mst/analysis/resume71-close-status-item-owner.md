# CP71: the status item allocation and private-cache binding edge

The actual status layout setter supplies a concrete item producer:653cc249 constructs QWidgetItemV2 on a newly allocated0x58 block, then653cc256 stores the returned item in layout+4f8. Its constructor stores item.widget and conditionally publishes that **same item allocation** into widget.private+c0. The missing edge is a canonical private-cache target resolving to this common item allocation and its writable+10 field. Current CloseStatusItemOwner metadata has no native allocation lifetime or private-cache link; no retained late-Close runtime alias was found in the bounded evidence searched.

Cache publication precedes layout adoption, so assigning an owning layout at metadata creation would also erase a real constructor-frame interval. A different preexisting cached item must remain distinct from the newly adopted status item. This matters directly to the isolated private destructor: it nulls the+10 field of the actual+c0 target, not necessarily the layout's current status item.

This report is the sole write. No source/build/test execution/GUI/original execution/mutable REA operation occurred. Method: current source, retained CP63/65/67/69/70 captures and bounded offline PE/export/DUMPBIN analysis. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`. No reconstructed executable/deployment was inspected or changed.

## Constructor and use xrefs

An offline QtGui.text scan found eight E8-relative byte candidates targeting exported QWidgetItemV2::QWidgetItemV2650494b0. Bounded disassembly of their containing unwind regions validated all eight as direct calls. This is a direct-call search in this module, not complete indirect/import/inlined coverage or a runtime reachability claim.

| Call site ->650494b0 | Containing direct region, end exclusive | Qualification |
| --- | --- | --- |
| 6504396f | 65043930..6504397e | Exported QLayoutPrivate::createWidgetItem. |
| 65046346 | 650462f0..65046366 | Exported QLayout::addWidget. |
| 653a1092 | 653a1010..653a112a | Exported QDockWidgetLayout::setWidgetForRole; another allocation context. |
| 653cbb7b | 653cbb20..653cbba3 | Another direct wrapper deletes/replaces its item field+f8; not the status+4f8 producer. |
| **653cc249** | **653cc1f0..653cc272** | **Actual status layout setter; candidate status-item producer.** |
| 6543e0f2 | 6543dc80..6543e292 | Direct call validated; enclosing transitive semantics not needed/claimed here. |
| 65440897 | 65440850..654408c4 | Direct call validated; not selected as status producer. |
| 65440978 | 65440930..6544099f | Direct call validated; not selected as status producer. |

The first two functions consult exported QLayoutPrivate::widgetItemFactoryMethod at659c98e0. A nonnull hook can return an item; null hook/result falls back to direct V2 allocation. QLayout::addWidget first performs addChildWidget, then passes the resulting item to layout virtual+58. Thus generic QLayout item creation cannot always be treated as fixed V2 without closing the actual hook/result branch.

The status setter653cc1f0 **does not consult that factory cell**. Its selected nonnull new-widget branch directly allocates0x58 and calls650494b0. This closes the constructor target for this producer, subject to actual branch/return evidence; it does not establish that the late Close path created or still owns such an item. It also does not make the status wrapper the only possible cached item for the widget.

Independent exported useSizeCache65049160 establishes the cache identity rule without ownership inference:

```text
65049160 load widget = [item+10]
65049164 load private = [widget+8]
65049168 compare [private+c0], item
6504916f return equality
```

There is no null/lifetime check in that native leaf. Its equality test does not transfer ownership or validate a dangling alias. CP67's geometry use reads the same+c0 and invalidates the targeted item's size cache; the private destructor writes that target's widget+10=null. CP66's item destructor clears widget.private+c0 only when the cache equals the item being destroyed. These independent consumers require one common item identity, not separately allocated status/cache tokens.

## Exact producer and ownership interval

For the actual main's selected layout receiver, status setter653cc1f0 performs:

1. Retain incoming widget. If nonnull, call addChildWidget65045540 at653cc20f. Its actual parent/child/callback effects precede item construction and remain dependencies.
2. Load the existing owning slot layout+4f8 at653cc214. Nonnull invokes its actual deleting virtual slot0(flag1) at653cc228. The slot still contains the old pointer during that call. Do not clear it early or infer which widget is deleted: item deletion does not itself delete its QWidget.
3. For a nonnull incoming widget, allocate0x58 at653cc234. The constructor receives that allocation as `this` and the same incoming widget as `rdx` at653cc249. Before adoption, the constructor/setter call frame retains this allocation identity. Allocation failure/exception behavior is not replaced with a fabricated live item.
4. Constructor650494da stores item+10=that widget;650494e5 installs V2 table656a58c8; cache members are initialized before the private-cache branch. At65049543 load that item's widget,65049547 load its actual public+8 private, then6504954b read private+c0. If null,65049555 publishes this exact item into that cell. Nonnull preserves the existing cache identity. The cache is nonowning in both branches.
5. After constructor return,653cc256 stores the returned item into this actual layout+4f8. This is the owning-slot adoption event. The cache may already point at the new item from step4, or may still name another item. A null incoming widget follows the explicit null store branch instead; it is not a known-null cache observation.
6. Layout invalidation virtual+58 follows via the adjusted layout receiver at653cc26e. Its callbacks and any later cache/slot mutations require actual state/effect tracking.

The existing-bar getter653ccd80 consumes only layout+4f8, its item's virtual+60 and QStatusBar cast. It does not read widget.private+c0 or establish equality between that cache and the current status item. Even a successfully cast live status bar leaves that equality unavailable without the actual cache read/publication history.

CP70 remains the corrected getter/setter contract: the lazy getter returns its constructed bar after setStatusBar; repeated layout getter reads occur inside the setter. The actual lifecycle's constructor/reentry/old-item behavior remains unobserved.

## Exact absent source edge

Current files were searched for QWidgetItemV2, CloseStatusItemOwner, statusItemOwner, widgetFieldOwner and cache-owner terms across the converter include/src tree. The relevant existing representation is:

- `include/m2m/engine/close_status_owner_prefix.hpp`: CloseStatusItemOwner contains a weak layout projection, accessor qualification, Unknown/KnownNull/KnownWidget raw-widget knowledge and weak WidgetState. It has no actual item allocation alias/lifetime, constructor frame or private-cache cell. Its header explicitly calls it candidate identity/ownership metadata.
- `src/engine/close_status_owner_prefix.cpp:67`: observeStatusItem creates a new metadata object and immediately assigns the projection slot. This is an observation API, not650494b0 execution or653cc256 adoption. Calling it cannot authenticate those events or construct a private target.
- `include/m2m/engine/view_manager.hpp:637`: GridLayoutItemState is only an empty polymorphic base for the currently recovered empty layout population. It supplies no concrete shared V2 identity or writable widget field.
- WidgetState contains actual candidate widget/runtime/public-liveness state, but no separate QWidgetPrivate allocation/+c0 owner. QObjectConstructionRecord is a counter-effect record. The editor construction cursor is tied to its own editor occurrence and cannot supply a status-private allocation.
- ConversionSession still has no production status-main/private/item acquisition field or call. The isolated status helper's nonnull widget remains guarded before QObject::metaObject/cast.

Search of the retained close/status reports and bounded raw captures found static instructions and explicit test fixtures, not an original late-Close tuple `(actual main, layout, item, widget, private, private+c0 target)`. This states the searched evidence boundary; it is not proof that no such value exists in any external/archived REA store. No stale/mutable REA session was queried.

The exact edge needed before the private-destructor prefix is:

```text
actual widget public+8 -> actual still-allocated private owner
actual private.c0 read -> {Unknown | observed null | nonnull canonical item allocation}
nonnull canonical item allocation -> same writable raw widget+10 field
```

The layout ownership edge is separate. On the qualified status producer, the same canonical item is retained by the construction frame until653cc256, then owned by the actual layout slot. The private cache must borrow it. If+c0 names a different item, that item's actual allocation/field owner must resolve independently or the destructor must guard. A merely opaque identity can support the item-destructor equality/preserve comparison, but cannot authorize the private destructor's write through that pointer.

## Smallest binding increment and focused validation

Root's prerequisite is one canonical concrete item allocation/field owner reusable by the layout slot and the private cache, plus one actual private-allocation/phase record on the corresponding widget. Preserve Unknown as the initial state. Do not create native lifetimes from metadata, replace expired nonnull links with null, or infer cache==status item because both refer to the same widget.

The first source-qualified binding events are item.widget store650494da, conditional publish65049555 and owning adoption653cc256, in that order, on the same actual widget/private/item/layout identities. A late observation may instead bind an actual current cache read, but must not replay the constructor to force equality. Root's Session/status metadata can retain these common owners after binding; it is not their producer.

Only after this graph and actual base-private entry are bound can the isolated650676d0 prefix commit the base-private class phase, consume+c0 once, skip known null or null the exact cached item's+10, then stop before65067709. It must retain the cache pointer, ignore the target's old widget value, preserve other items, and reject unresolved nonnull allocation targets. Preceding QStatusBarPrivate instance QString/list cleanup and public QWidget/QObject teardown remain required on any production path. The valid existing status-bar Close branch does not reach this destructor merely because the Project was accepted for closure.

Focused future tests should cover constructor-frame retention before adoption; conditional null-cache publication versus preserving another item; same canonical item shared by layout/cache; status item distinct from cached item; unknown/opaque/dead nonnull target refusing a write; explicit private-null skip; exact target+10 write with unrelated prior value; item destructor equality versus private destructor unconditional target store; no cache-cell nulling on private-prefix completion; consumed identity retained on retries; and no public Close/parent/counter/timer/event/process-cache effects. Tests that only construct diagnostic observations establish representation rules, not native producer/teardown execution.

## Evidence receipts

New disassembly is retained inline above; no raw file was added. DUMPBIN14.44.35228.0 from MSVC14.44.35207 supplied bounded views. PE exports independently name the V2 constructor/useSizeCache, factory hook, QLayoutPrivate::createWidgetItem and QLayout::addWidget. Direct-call candidates were checked against disassembly starting at their enclosing .pdata region entries; this does not close each caller's transitive effects.

| End-exclusive native region | Code SHA-256 | Scope |
| --- | --- | --- |
| 65043930..6504397e | `69b4643ab75a5ae37e3b8edbe5a9f5d6e59edc4f4312f402aa7b417dff626128` | Generic createWidgetItem hook/fallback. |
| 650462f0..65046366 | `2861dec0e3e7845b606f84cce93a8d0d30775ea22410545cc60e0306497b603d` | addWidget hook/fallback and virtual adoption. |
| 65049160..65049173 | `2ac617ce277140467a0779badfad600622059d55ac35dd4a5dd9b7edeb26f8dd` | Complete leaf useSizeCache, excluding alignment padding. |
| 653cc1f0..653cc272 | `57cf7209c989a732926ea2418b9bd4e3009d8a25414290dac3e5141894934280` | Status setter direct body. |

CP65/67 retain650494b0..65049579 constructor hash `b95d4668336c234ed541c1a2a12b6c3830c9ca732d634d3d27cda58bb8004608` and item/private/geometry destructor-use receipts. CP69/70 retain private acquisition and provider ordering. Native late-owner bindings, actual cache equality/current value, intervening callbacks, status constructor completion and destructor reachability are still missing; public Close and process-reset guards remain necessary.
