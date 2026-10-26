# CP65: qualified status-item widget accessor prefix

The owned status helper now advances a qualified QWidgetItemV2 through its pure widget-field accessor. A known-null widget passes only QMetaObject::cast's null leaf and reaches the existing allocation guard. A nonnull actual widget remains guarded before virtual QObject::metaObject and QStatusBar inheritance inspection. Allocation, parenting, status messages/timers and public Close are not enabled.

Exclusive source edits: `source/apps/moproj2mst/include/m2m/engine/close_status_owner_prefix.hpp`, `src/engine/close_status_owner_prefix.cpp`, and `tests/mdt/test_close_status_owner_prefix.cpp`. This report and eight unique bounded ASM files are additional evidence writes. No shared Session/model/CMake/plan/public Close or message-cache source changed. No build, CTest/executable test, GUI, target execution or mutable REA operation ran. Root owns validation.

Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`; QtCore4_64.dll `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. DUMPBIN14.44.35228.0 from MSVC14.44.35207 supplied offline views.

## Recovered known-item call

Native layout getter653ccd80 reads status-item+4f8, calls its virtual+60 and casts the returned QWidget to QStatusBar. Fixed Qt tables bind both QWidgetItem656a5818+60 and QWidgetItemV2 656a58c8+60 to exported QWidgetItem::widget at651a4ce0. Its complete leaf is `load qword this+0x10; return`, five bytes, code SHA `01b97d2da374096b4359dfb781da44ea9eb61ba0541de9f2d557cfad9adef8d3`. The field is+0x10, not+8. The V2 constructor650494b0 stores its supplied widget there and installs vtable656a58c8 before further constructor/cache work. This supports only the qualified actual V2 accessor profile, not arbitrary item virtuals or a supplied returned QStatusBar.

QtCore QMetaObject::cast at6711ddd0 first tests its object argument. Null returns0 without calling the object's vtable. Nonnull calls QObject::metaObject at virtual slot0, compares the resulting metaobject with the requested QStatusBar metaobject, then follows each metaobject's first superdata pointer until a match or null. A match returns the original object pointer. This is not qt_metacast or a name-based comparison. Actual dynamic metaobject/profile/inheritance data is not represented by the current WidgetState owner, so that virtual remains guarded.

The default exported QLayoutItem::widget655289a0 also returns null, but that unrelated profile is **not** enabled by this increment. No inference is made that every layout item is a QWidgetItemV2.

## Lazy allocation, parent and timer order

The enclosing source remains qualified by CP62/63 captures:

1.If layout getter result is null, QMainWindow::statusBar allocates0x28 bytes, invokes QStatusBar(actual main), sets QSizePolicy raw0x0d and calls QMainWindow::setStatusBar. The null widget/cast branch does not itself allocate or install anything.
2.QStatusBar ctor allocates private storage0x210 and enters QWidget construction with the actual main as parent. It writes status-private+1f0/+1f8/+200 null before later size-grip/visibility/setup calls. Those intermediate defaults are not current late-Close facts.
3.QMainWindow::setStatusBar653c9c50 re-queries the current status bar. If an existing one differs, it invokes its visibility virtual+58(false) and QObject::deleteLater before forwarding to layout setter653cc1f0. The fixed import65697218 is deleteLater; this is not immediate old-widget deletion.
4.Layout setter653cc1f0 calls QLayout::addChildWidget for a nonnull replacement, deletes the previous status **item** through its first deleting virtual with flag1, then allocates0x58/constructs QWidgetItemV2 for the replacement. Only afterward it stores layout+4f8 and tail-dispatches layout invalidation through the adjusted layout interface's virtual+58. QLayout::addChildWidget65045540 has its own existing-parent/layout/reparent effects. None is implemented here.
5.Only the returned actual QStatusBar receives showMessage(message,0). Message assignment precedes the timeout branch. Null current status timer skips deletion; nonnull invokes its deleting virtual while raw status-private+1f8 still names it, then nulls that field after return. The positive-timeout creation path supplies the status bar as QTimer parent and installs the private timer vtable6570bd98. Its deleting wrapper65406ac0 calls QTimer::~QTimer before conditional free. QtCore QObject/timer cleanup remains separately guarded.
6.The complete chained65406480 status-update tail includes child visibility, messageChanged, accessibility and repaint before subsequent ShowMsgInArea delivery. The process-global MessageArea duplicate cache is distinct from the status-private message/timer. No cache reset or GUI message effect follows from this prefix.

The source-backed allocation path establishes why current status-item V2 is a useful profile, but does not establish that a supplied candidate owner actually completed those native constructors/installations. Allocation failure/reentrancy/native Qt lifetime behavior is not replaced with invented success or null fallback.

## Implemented owner-bound contract

The existing main/layout/item owner chain remains required, with fresh CP62 nonnull entry and same-live-Session/manager identity. Unqualified KnownItem still returns ItemVirtualWidgetUnavailable at653ccd98. A new explicit V2 accessor qualification identifies only the fixed table/target profile; labels do not authenticate native observations or construct a QWidgetItem.

Qualified dispatch retains `AwaitItemWidgetField`. Its actual item field distinguishes Unknown, KnownNull and KnownWidget. Unknown stops before the raw load. KnownNull consumes the null field and cast-null leaf, then stops at653ca63f allocation. This conditional fixture is not evidence that a freshly constructed original V2 normally contains null.

KnownWidget consumes a weak identity of an existing actual WidgetState object. It does not allocate a duplicate widget or accept a cast result. Before any further dispatch, the helper requires a live owner and the current Session's actual WidgetRuntimeState. Missing/expired/logically-dead/foreign-runtime identity returns WidgetIdentityUnavailable; it is never interpreted as a native null field. A valid nonnull target returns WidgetMetaObjectUnavailable at6711ddeb. This is an unsupported-profile guard, not a claim that original invalid native pointers fail safely.

Accessor and field consumption are separately reported. Partial-query retry does not replay a completed virtual dispatch; consumed item/widget identity is retained even if later observations replace the layout slot or widget field. The item field is nonowning, and the retained weak identity/metadata does not prolong native QWidget lifetime. Old unqualified item and allocation guards remain unchanged.

## Tests, freeze and integration limits

Target `m2m_mdt_close_status_owner_prefix`; CTest `m2m.mdt.close_status_owner_prefix`.

```text
ctest --test-dir <combined-build-directory> -C Release -R ^m2m\.mdt\.close_status_owner_prefix$ --output-on-failure
```

The test file now has18 Release-active cases, eight added to the prior ten. Added cases cover qualified accessor with unknown field; known-null field/cast skip; actual nonnull widget/metaobject guard; consumed target retained after replacement; partial-query retry on the original item after layout replacement; foreign-runtime target refusal; logical native target death without null fallback; and blank/missing field-owner receipts. The nonnull case verifies actual runtime QObject/widget counts, parent and flags remain unchanged. Fixtures are declared model bindings, not original GUI captures or accepted-Close reachability evidence.

Static Testkit main/include resolution, CHECK/REQUIRE macros, absence of C assert/NDEBUG-dependent checks, current helper-only call sites and whitespace checks passed. No compilation or executable validation was run here. Source is frozen:

| File | SHA-256 |
| --- | --- |
| close_status_owner_prefix.hpp | 2c847cfbc0bc6d34ff118b930ba51e13f3dca2d184e27253263b835134d3be83 |
| close_status_owner_prefix.cpp | e916e76674fc9faab34717fff72f2458dde9d8e7fceaea1e2030651a0d06ae51 |
| test_close_status_owner_prefix.cpp | 103988722bff8d26bc309ea5cad5b1a5cf8be0ad5373b8c8362168bd3bc60c33 |

Root still needs actual native main/layout/item/widget bindings, class/lifetime validity, nonnull metaobject/inheritance results, allocator/constructor/parent/installation effects and status/timer/notification work. The internal CP62 entry-progress single-lookup integration requirement from CP64 remains. No public Close release, QStatusBar construction, timer deletion, message-cache assignment/reset or global reset is implemented.

## New raw evidence

All files are under analysis/, prefixed `resume65-close-status-`. Commands use `/nologo /disasm /range:<start>,<end-minus-one>`. The two getter leaf bounds end at decoded ret and exclude padding; other applicable bounds are unwind records, not blanket transitive closure.

| Suffix | End-exclusive bounds | Artifact SHA-256 |
| --- | --- | --- |
| 651a4ce0.asm | 651a4ce0..651a4ce5 | 969347d9df989af47902ff745d50a3e2a5d3be861cab0ee54d5a8a034ca5c0af |
| 655289a0.asm | 655289a0..655289a3 | e4b72ba0a5bfd19bdd22124702058bf1a2f4abdb7e32622ec92f476454917457 |
| 6711ddd0.asm | QtCore6711ddd0..6711de18 | 27fa9aa13715b3c39069e0a0ef7f0d424cac97de7a5ec7abd5f777dfd63a45fa |
| 650494b0.asm | 650494b0..65049579 | dad11bba1422910b0a15dabda03221b2b3edc3d2f606534107a7db5f2bee793b |
| 65045540.asm | 65045540..65045851 | 2acc2916f2a2924e93dac1fdb6255d83d269b46a151276b5fa949c0927b336b9 |
| 653c9c50.asm | 653c9c50..653c9cc9 | 0e7ea6be37fad48d30fe24ad58e7186f053835b8cfe023ab0cda4744f0e79371 |
| 653cc1f0.asm | 653cc1f0..653cc272 | 3f45f8912a07f1867cfab89418df56aae3e8799a43949ea53d0b555814b22de4 |
| 653cb190.asm | exploratory non-callee region, excluded from contract | d4bff2d526342eca9484484e4fe05e622cb51ef5bd3b3850b7aaa5a46e3ea155 |

The excluded exploratory region includes switch-table data and is not treated as a status-setter procedure. Actual setStatusBar's decoded tail target is653cc1f0. QtCore cast code SHA is `bac4c606b855c42a8f09bd8d5abe92594c95f012fe0bbb1ec6a6b0fe7d4bf10e`; layout setter code SHA `57cf7209c989a732926ea2418b9bd4e3009d8a25414290dac3e5141894934280`. Each fixed V2/base virtual+60 qword is651a4ce0 with raw-qword SHA `e426930a42ac2a656ab5400664c8900ce760804d5fa1f6d1461de7f239375c19`. Prior CP62/63 raw captures provide the enclosing statusBar/ctor/showMessage/timer/update bodies. No mutable analysis database was opened.
