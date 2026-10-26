# CP119 DOE Save receiver-generation evidence

## Result

Retained static evidence does not establish a same-generation `QPointer` receiver, `slotUpdateUI()` completion, or timer-stop return for an active Type2 Save occurrence. The existing public Save guard remains necessary. This pass rechecked the table owner's PE receipt and dispatch chain and checked the newer CP118 static producer inventory for an additional Save-to-table edge; none supplies dynamic occurrence ownership.

## Static facts retained

The original PE SHA-256 is `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

- Getter `0x1400196d0` publishes a constructed table at main-window `+0x458` only after ctor `0x1400eb6f0` returns. The ctor passes the same `this` pointer to the global notifier registration at `0x1400ebc5c` with key `2signalDoeTableRefresh()` and slot `1slotUpdateUI()`.
- The ctor also makes `+0xb0` a child `QTimer` and connects `2timeout()` to that table's `1slotUpdateUI()` using AutoConnection. This is an independent timer route; it is not evidence of the Save signal's delivery.
- The `SfMoDoeTable` metadata/static-metacall maps local method 0 `slotUpdateUI()` to `0x1400f4e10`. If entered, that body calls `QTimer::stop()` on receiver `+0xb0`, then dispatches receiver virtual `+0x1b8` to the identified primary implementation `0x1400f47e0`.
- The destructor body `0x1400ebc80` has no explicit DoeTableRefresh unregister call in its selected complete body. The global dispatcher uses weak-guard resolution on a snapshot, skips a null guard, and DirectConnection invokes a non-null resolved object. The dispatcher does not compare the resolved receiver to main-window `+0x458`; its `invokeMethod` Boolean is not tested.

These facts establish a potential constructor-generation relation only. Registration occurs before cache publication. Destruction/Qt weak-guard invalidation, registration-list contents, selected pointer, QTimer state, and method completion depend on runtime state and are not recoverable from these static bodies alone.

## Check of newer retained evidence

CP118's bounded fixed-string producer analysis identifies one constructor registration and several emitting bodies for the similarly named Geometry-editor key `2signalShowCentral2DGeoEditor()`. The newly classified direct callers are geometry import/save/edit and cross-section actions, not normal project Save or Close. That evidence does not update the DoeTableRefresh registration snapshot or bind its receiver to the current table cache. The retained Qt metaobject confirms callability/signature, not an invocation in the selected lifecycle.

The original Save emitter `0x1400d369d` sends only the no-argument DoeTableRefresh key through the global notifier. It supplies no table pointer, receiver-generation token, timer pointer, or Save-manager identity. A dispatcher return only establishes that its own body returned. A true `invokeMethod` result, if observed, would still not prove that the receiver was the current `+0x458` generation or that the slot and nested refresh returned normally.

## Smallest missing witness

The minimal decisive addition is one correlated runtime receipt for the same active Type2 Save occurrence containing:

1. `mainwindow+0x458` at dispatch time and the exact registration snapshot entry selected for `2signalDoeTableRefresh()`, including its weak-guard result;
2. equality between that resolved receiver and the published table generation, plus the exact `slotUpdateUI()` method actually entered;
3. that receiver's `+0xb0` timer identity and a normal return from `QTimer::stop()`;
4. same-receiver refresh entry and normal slot return, with refresh-time current Project/SUB recorded separately from Save's earlier cached manager.

Without this single-occurrence binding, neither another static caller scan nor a synthetic callback-success flag can close the boundary. No isolated production helper is justified by this evidence.

## Scope and receipts

Read-only static evidence review. No source, build, GUI, original process or mutable REA operation was used. CP117 receiver report SHA-256 `F9041816850809BF0E592CEA66CD28013DE4456F6CBD6133ADA8F6D6771C8E05`; CP117 dispatch-ownership report SHA-256 `1C75AFC3210E534FF659070CFC5CA664A8FB5A0733722A2787ED9C8ED18E1F3F`; CP65 PE receipt SHA-256 `BB41B752BF8F46F8B6449F596F118B639B4B501913D984B3362B59939F03520B`; CP118 producer report SHA-256 `C604FAB626CDC5D9178DA82E0BF39E8A8213781B8067C24FB207F304918990BD`.
