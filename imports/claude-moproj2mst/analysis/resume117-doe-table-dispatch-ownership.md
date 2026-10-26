# CP117 DOE table dispatch ownership through Qt delivery

## Finding

The static chain ties a table constructor generation to a global weak registration and ties that table's timer to its own refresh slot. It still does not establish which registration generation the dispatcher selects during a particular Save, whether that selected object is the currently published main-window `+0x458` table, or whether the slot returns normally. There is therefore no source-closed same-owner callback-return witness for active DOE Save. Keep the public Save guard.

## Owner and registration sequence

The original GUI PE is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

1. Getter `0x1400196d0` checks main-window `+0x458`. On null, it allocates `0xb8`, invokes `SfMoDoeTable` constructor `0x1400eb6f0` with null parent, and publishes the returned pointer into `+0x458` only after the constructor returns (`resume65-doe-refresh-getter.asm`). A non-null cache is returned directly. The static order distinguishes a completed construction/publication from a merely allocated or interrupted object.
2. The constructor stores a `QTimer` parented to that table at `+0xb0`, connects its `2timeout()` signal to its own `1slotUpdateUI()` with AutoConnection at `0x1400eb8a4`, performs initial refresh, then registers `this` under `2signalDoeTableRefresh()` / `1slotUpdateUI()` at `0x1400ebc5c`. Registration therefore precedes getter publication. The timeout connection is a separate Qt event/timer route; it does not show that a Save-triggered message was queued or delivered.
3. The selected table deleting-destructor body `0x1400ebc80` has no explicit call to unregister this key. It destroys the table's list member and calls a base destructor. This absence does not establish Qt parent/child teardown timing or weak-guard invalidation: those are runtime QObject lifecycle effects not executed by the static review.
4. On Save's no-argument refresh message, dispatcher `0x1402cf3c0` gets the ordered registration list and copies it to a local snapshot. It tests each stored weak QObject guard; a null result branches to local snapshot cleanup/skip. A live object is passed to `QMetaObject::invokeMethod` as a direct call. The dispatcher never loads or compares main-window `+0x458` at receiver selection, and the invoke Boolean is stored but not tested. Thus list order and weak-guard checks are statically visible, while this occurrence's registration contents, guard resolution, chosen pointer and method result are not.
5. If the selected pointer is the expected table and its ordinary meta-call binding resolves, `slotUpdateUI()` at `0x1400f4e10` first loads that same object's `+0xb0` and calls `QTimer::stop()` at `0x1400f4e25`; it then invokes that receiver's virtual `+0x1b8` at `0x1400f4e38`. The identified primary vtable maps the virtual to `0x1400f47e0`. Only slot entry establishes the timer-stop call; only completion of refresh and slot establishes the corresponding normal return. Neither follows from dispatcher return or `invokeMethod`'s Boolean alone.

## Generation/lifetime boundary

The constructor/getter relation is a static *potential* generation binding: the pointer registered as `this` is the pointer returned and cached if that construction completes. It does not bind a later snapshot entry to that cache. Since registration occurs before publication, static evidence also cannot rule out an interval where the registration exists before `+0x458` is populated. Since the destructor has no visible explicit unregister, a historic registration can remain in the global list; the dispatcher's weak-guard check can skip a dead receiver, but no static value proves whether the guard is live for a given Save.

Likewise, `QTimer::stop()` is only an imported call on the receiver's `+0xb0` pointer. The static bodies do not reveal the timer's active state, thread/event dispatcher, actual timeout delivery, or the consequences of concurrent/reentrant teardown. The separate AutoConnection connects a timer timeout to the same slot, but no timer event receipt links that path to Save's synchronous DirectConnection delivery.

## Exact missing occurrence witness

A single correlated Type2 Save occurrence would need to record: (a) main-window `+0x458`; (b) the complete ordered key registration snapshot and each weak-guard resolution at dispatcher entry; (c) the exact object and method passed to `invokeMethod`; (d) equality of that object with the currently published table generation; (e) that object's `+0xb0` timer identity and normal return of `QTimer::stop()`; and (f) same-object `+0x1b8` refresh and normal slot/dispatcher return, retaining the refresh-time Project/SUB separately from Save's cached manager. No retained static report or capture supplies these dynamic, same-occurrence values.

In particular, the dispatcher returns after iterating its snapshot even if it invokes no slot, and it ignores `invokeMethod`'s Boolean. A successful invoke result would still not identify the object with `+0x458` or establish normal slot completion without a correlated return witness. The candidate's diagnostic table-owner tokens cannot substitute for Qt's registration list, `QPointer`, widget/timer lifetime or meta-call.

## Disposition and evidence scope

No additional helper or isolated source seam closes this owner gap. Do not turn static constructor registration into a callback-success token or enable the active Type2 Save continuation on that basis. The decisive next evidence is the one-occurrence runtime witness above; otherwise preserve the guard.

Read-only review only. No source, build, GUI, original execution or mutable REA operation was performed. Supporting retained evidence (hashes): `resume117-doe-save-invoke-receiver.md` `F9041816850809BF0E592CEA66CD28013DE4456F6CBD6133ADA8F6D6771C8E05`; `resume65-doe-refresh-owner.md` `34D23E77AEE1693B151479F089FB2BAF7C1A1484004CB5F773036AD74BD53855`; getter ASM `8B656E3ED3E8FEB028DB5E69A3550091D7F000B9B7C965C8287117DBA7839AB9`; constructor ASM `77ED9A91909A37FFB1C81EDB6708D4593B04EE7F2535A1B585F57445186DDA89`; destructor ASM `A6E6A573AD69CB57FDD5B0A4BB8F139075F354FCDD394C597544AF000A56170A`; dispatcher ASM `72E47AEEF793CF4E9347BCEAE891A6EE540CECDD0254EE19C7D9FF8360395957`.
