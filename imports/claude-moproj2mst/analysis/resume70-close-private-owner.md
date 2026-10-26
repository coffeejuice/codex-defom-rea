# CP70: status-private lifetime through one-project Close

Accepted project Close does not establish status-widget destruction or a reset of its QWidgetPrivate item cache. The valid existing-status-bar branch preserves the widget and calls showMessage on that result. Cache contents can still change through actual item/layout effects or reentrant callbacks; the selected original one-project runtime path and current cache value have not been observed. Preserve the owner and Unknown state rather than forcing either a reset or an unchanged-value claim.

The newly reconciled destructor boundary is more precise: QWidget's public guard cleanup can precede private destruction. QObject finally reloads public+8, invokes that private object's deleting provider, then stores public+8=null after the provider returns. QStatusBarPrivate destroys its instance payloads before QWidgetPrivate's cached-item backlink write. Losing the public guard is therefore not a private-free event, and private teardown is not a cache-null store.

Only this report was written. No source/build/tests/GUI, original execution or mutable REA operations occurred. Method: retained evidence plus bounded offline PE/export/import/DUMPBIN analysis. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`; QtCore4_64.dll `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. No reconstructed executable/deployment was inspected or changed.

## Close's actual owner route and one correction

The normal0299a0 tail sends DeleteWidgetInWidgetArea at029c8d, executes false busy at029c9b, then reports `All projects have been closed.` through2d0bd0 at029ce4. Its fresh main lookup may be null; nonnull calls QMainWindow::statusBar followed by showMessage(message,0), then ShowMsgInArea delivery. The tail and031f10 wrapper contain no subsequent direct main/Session/private destruction. These direct bodies do not close arbitrary synchronous receivers or reentrant Qt effects.

QMainWindow::statusBar653ca610 loads main.public+8 -> mainPrivate+1e0 layout and invokes653ccd80. Its branches are:

- Existing valid QStatusBar:653ca637/63a ->653ca67b returns the same result directly. No allocation, setStatusBar, layout-item replacement or deleteLater is called by this getter branch.
- Lookup null: allocate public0x28 at653ca63f; construct QStatusBar(parent=main) at653ca654, including its separate0x210 private allocation. Configure it, call setStatusBar at653ca673, then653ca678 returns the newly constructed bar retained in `rbx`.

**Correction to CP69:** its sentence that the getter must re-read the layout item after constructor/setter is inaccurate. The getter returns `rbx` after the setter. The **setter**653c9c50 re-reads the layout through653ccd80 at653c9c68,9c79,9c8a and9ca2. These reads surround comparison/hide/deleteLater work; reentrant changes cannot be collapsed into one stable old-bar identity. This report supersedes only that CP69 sentence. Its private-binding/Unknown-state conclusion is unchanged.

When a different nonnull bar is actually selected by those setter reads, the setter hides it and requests QObject::deleteLater. The layout setter653cc1f0 then deletes the old layout item through virtual slot0(flag1), creates a replacement V2 item and stores layout+4f8. Old item destruction and old widget destruction are different operations. A stable null lookup may avoid old-widget deleteLater entirely. Constructor/setter callbacks and lookup stability have not been established for the selected lifecycle.

## The private owner is the returned widget's allocation

CP69 source-qualifies the constructor aliases: QStatusBar65406d70 allocates its private block; QStatusBarPrivate invokes QWidgetPrivate(int);6505e7e3 writes private+c0=null. The protected QObject constructor6712e655/661 later binds public+8=private and private+8=public before thread/counter/QWidget/QStatusBar completion. The default null event cannot be replayed when late Close acquires an existing widget.

The normal nonnull lookup must establish the actual item's widget, its dynamic QStatusBar cast and that same widget's current public+8 private allocation. Neither Session::MainViewState nor a status metadata holder is this allocation. Current `ConversionSession` still contains no CloseStatusMainOwner field/acquisition, and the isolated status prefix still stops before6711ddeb's QObject::metaObject/cast work. `WidgetState` has no actual private-allocation lifetime or+c0 cell. The editor's constructor effect record and QObject counter record do not supply a status-private allocation.

The three links remain distinct:

| Location | Meaning | Relevant effect |
| --- | --- | --- |
| layout+4f8 | Owning status item slot | Item replacement/deletion can happen while the old widget survives. |
| item+10 | Nonowning raw widget pointer | QWidgetPrivate teardown may set this field null on its one actual cached item. |
| widget.public+8 -> private+c0 | Nonowning QWidgetItemV2 size-cache cell | V2 constructor publishes only if empty; item destructor clears only if this exact item matches. |

No Qt parent/lifetime or native allocation is constructed by retaining these links in C++ metadata. Unknown links, known null and unavailable nonnull targets must remain separate.

## Exact conditional destructor order

This order applies only when the actual status widget is destroyed; it is not an unconditional Close suffix. DeferredDelete posting is insufficient: CP66 qualifies queue/thread/loop-level eligibility, actual event delivery and receiver deleting dispatch separately.

1. QStatusBar deleting scalar provider65407680's scalar branch invokes QStatusBar::~QStatusBar654075a0. The public destructor installs status classes, drains its private+1e0 descriptor list, and tail-calls QWidget::~QWidget6506e890 at65407678. The descriptor cleanup is not a substitute for child QWidget destruction and is not a process-global cache reset.
2. QWidget destructor6506e890 installs QWidget/paint-device classes and retains its private pointer. It performs substantial widget/layout/platform/event work; those providers remain dependencies. In its later selected path,6506ef7d tests private+20 bit9 and6506ef88 conditionally calls imported QObjectPrivate::clearGuards(publicWidget). Private+78 callback handling follows at6506ef8e..efa3. If the actual child list is nonempty,6506efb8 calls QObjectPrivate::deleteChildren on this private. Remaining QWidget teardown precedes QPaintDevice destruction6506f054 and the tail-call to imported QObject::~QObject at6506f06f.
3. QObject::~QObject6712f870 installs QObject's public class, retains the entry private pointer in `r12`, changes private flags and performs conditional guard/reference/signal/connection cleanup. Its non-widget guard-clear condition is not an extra unconditional widget expiry event. Other QObject child cleanup/count/parent-detach work precedes the private deleting dispatch. Direct call ordering is established; all callback populations/transitive behavior are not.
4. At6712fd9a QObject **reloads current public+8**, rather than blindly using its entry `r12`. If nonnull,6712fda9 invokes that actual private vtable slot0 with flag1. Any model must validate this consumed actual private identity at this source point, not use a stale helper token after callbacks.
5. For the qualified QStatusBarPrivate table6570bd18, slot0 is65406b60. It destroys the instance QString private+1e8 at65406b84, decrements the list backing at private+1e0 at65406b92, conditionally qFree's that backing at65406ba3, then calls QWidgetPrivate::~QWidgetPrivate650676d0 at65406bad.
6. QWidgetPrivate installs base-private class at650676f2, reads its current+c0 at650676f5, and if nonnull writes **that item's+10=null** at65067701. It does not read/compare the target's old widget pointer, delete the target item, or clear its own+c0 cell. A null cache skips the store. An unresolved nonnull target cannot be treated as null. Further private extra/member cleanup starts65067709 and precedes its tail-call6506782f to imported QObjectPrivate::~QObjectPrivate.
7. QObjectPrivate67131240 conditionally calls its thread dispatcher virtual+88; nonzero private+24 posted count causes removePostedEvents(public=private+8,type0) at6713129d; only afterward does it release threadData and remaining private members. Removing queued events is not DeferredDelete delivery and is not destruction of the application dispatcher.
8. After all base-private cleanup returns, QStatusBarPrivate65406bb2 tests flag&1 and65406bbb frees the private allocation for flag1. Only after this provider returns does QObject6712fdab store public+8=null. The later public scalar deleting provider may separately free the public allocation.

There is an interval in this order where public guards are invalid but private allocation/cache storage still exists. There is also a final interval after the private provider freed storage and before QObject clears its public d-pointer. A diagnostic lease does not make either allocation live. Never turn native allocation expiry into a new readable KnownNull private.cache value; the cell has ceased to exist. The public d-pointer-null store is a different field operation.

## What persists, and what can reset

| State | Established conditional behavior | What is not established |
| --- | --- | --- |
| Existing status widget/private allocation | Getter's valid-bar branch returns it without replacement/destruction. Project storage release alone is not a producer of its death. | That this is the actual late one-project branch, or that synchronous callbacks cannot alter owners. |
| QWidgetPrivate+c0 | Normal constructor starts null; V2 may publish; matching item destruction can clear it; private destruction nulls the cached item's widget field and later frees the cell's allocation. | A Close reset, late default null, or an unchanged cache value across all Close callbacks. Geometry may invalidate item caches without clearing this pointer. |
| Status-private QString+1e8 | showMessage assigns the closing message before handling its timer. It is destroyed only on the actual private destructor path. | Equivalence with the MessageArea process cache, or a normal-Close destructor call. |
| Status-private timer+1f8 | For timeout0, nonnull timer deleting virtual+18 runs, then the field is stored null only after return. Known null skips that deletion. | A blanket event/diagram-timer purge or the destruction of the containing status widget/private. |
| MessageArea process QString1446b3bb0 / init word1446b3bb8 | Shared across receiver/Project metadata lifetimes; duplicate skip and post-append assignment are separate source events. | Normal-Close/receiver teardown reset. Its registered exit callback destroys the QString; exact exit delivery and init-word reset remain unqualified. |

Thus the faithful current decision is to retain these distinct owner/state projections across project-only cleanup and continue guarding unobserved effects. This is not a claim that a runtime cache value was measured to persist. Fresh-process comparison is an acceptance requirement, not authority to zero every remaining application/process field after Close.

## Smallest next executable action

No additional public Close phase is released by this read-only trace. Its first unresolved nonnull status boundary is still the actual QObject::metaObject/cast or lazy QStatusBar constructor path. A detached private-destructor helper cannot bypass that dependency.

The smallest implementable prerequisite is a canonical widget-private allocation/phase record on the actual widget owner, with Unknown/nonowning+c0 storage that refers to a common actual item identity. Creation must perform no constructor, parent, counter, registry or destructor effects. Its first binding action should record the exact public/private alias relationship from a qualified constructor/read occurrence, without converting Unknown to fresh null or completed QStatusBar. Source-point observations must remain explicitly different from executing native effects.

With that prerequisite, the smallest **native-effect** test target is the isolated650676d0 base-private prefix: require its actual allocation/provider entry; commit the base-class transition; consume the actual+c0 once; null skips or a resolved cache target receives+10=null; stop before65067709. Unknown target keeps the completed class phase and guards. This contract must not certify the preceding QStatusBarPrivate QString/list work, public guard cleanup, QObject callbacks, private free, or public Close completion. Tests must enter at the explicit qualified seam; production integration must reach it through the ordered owner path above.

Focused future tests: unknown private versus live allocation and freed allocation; correct public/private aliases; constructor-null event versus late/copy binding; canonical shared item identities; null cache skip; unresolved nonnull guard after class transition; exact target write regardless of old item.widget; unrelated item preserved; private.cache pointer retained through the prefix; public guard invalidation not expiring private; private free not fabricating cache null; provider return preceding public.d_ptr null; retries retaining consumed target; and Project cleanup preserving the application/process holders. No test was written or run here.

## Evidence receipt and unresolved reachability

New bounded views are retained inline in this report; no raw evidence file was added. DUMPBIN14.44.35228.0 was run on the retained originals. Export resolution names QStatusBar/QWidget/QObject and their private destructors. Imports65696740 and65696798 bind QObject and QObjectPrivate destructors;656974f0 binds clearGuards;656974e0 binds deleteChildren. QtCore exports bind6712de20=clearGuards,6712d300=deleteChildren,6712d3f0=setParent_helper and6712ec70=QMetaObject::activate.

```text
6506ef88 call [656974f0]         conditional public QWidget guard clearing
6506efb8 call [656974e0]         conditional child deletion
6506f06f jmp  [65696740]         QObject destructor after QWidget/PaintDevice
6712fd9a mov rcx,[public+8]      fresh private load at final provider boundary
6712fda9 call [private.vtable]   flag1 actual deleting provider
6712fdab mov [public+8],0        only after provider returns
65067701 mov [cachedItem+10],0   base-private cache backlink write, not cache=null
6506782f jmp [65696798]          QObjectPrivate after QWidgetPrivate members
65406bbb free(private)          after base-private returns, when flag&1
```

| Region, end exclusive | Code SHA-256 | Scope |
| --- | --- | --- |
| QtGui654075a0..6540767d | `6b21a8d8852a6840c9ed65c70618ba0139b2302fff3063caba62da5d44a117d4` | Public status destructor direct body. |
| QtGui6506e890..6506f076 | `db1b085ddf563782cfd62c7987b0a1b97b104f49a9beee954ea559d24645af9c` | Retained `wave3-closure-next-6506e890.asm`; direct widget destructor and tail import. |
| QtCore6712f870..6712fdbf | `e629e2450ab541e6cfc9370ae975aa17e3911537c42d54fd923f35d3bcc29693` | Public QObject direct destructor and final provider/null store. |
| QtGui650676d0..65067836 | `4afbd49bcc764c00b68cb5b80adc7d0930be75fc2a013709b84c8441015197cb` | Base-private direct body. |
| QtCore67131240..671313cf | `11c366a18fd9e4f96751d93e73185a64bf90047881575eb6acc167409cc3db85` | QObjectPrivate direct body; selected ordering, not transitive dispatcher closure. |

CP63/65/66/67/69 retain the enclosing getter/setter, constructor, item/cache and DeferredDelete source qualifications and code hashes. Missing evidence remains exact native main/layout/status/widget/private/item bindings at the late Close occurrence; dynamic cast/provider results; current cache/timer/list/child/connection population; synchronous/reentrant mutations; modal/default schedule; and actual one-project destructor reachability. No report or supplied label substitutes for those observations, and no blanket process reset is qualified.
