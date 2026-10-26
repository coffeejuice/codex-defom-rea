# DoeTableRefresh owner and first reached effect

The registered receiver is **`SfMoDoeTable`**, a main-window dock widget with its own child QTimer and UI payload. For an identified live receiver on the recovered DirectConnection path, `slotUpdateUI()` first calls **`QTimer::stop()`**, then synchronously enters its UI refresh virtual. This is not a newly queued refresh request and cannot be replaced by an empty notification or a passed Study callback. The actual table/timer/UI owner remains absent from the candidate integration, so public delivery stays guarded after preserving the InputTable effects.

Only this report and uniquely named bounded original PE evidence/receipt were written. No source, CMake, build, executable test, GUI, original application execution or mutable REA state changed. `/root/doe_active` owns the complementary XML/public seam work; native owner/slot findings were sent to that lane.

## Construction and actual owner path

Constructor `0x1400eb6f0` installs primary vtable **`0x14218bd38`** and secondary vtable `0x14218bcf8`. MSVC RTTI resolves the primary complete-object locator `0x1430d7730`, type descriptor `0x1435a46b0`, to **`.?AVSfMoDoeTable@@`**.

The confirmed normal construction path is:

`main-window initialization 0x1400111c0 --call0x140011227--> dock initialization 0x140019950 --call0x14001a513--> getter0x1400196d0 --call0x14001970f--> SfMoDoeTable ctor0x1400eb6f0`.

The getter checks main-window **`+0x458`**. When null it allocates `0xb8` bytes, passes parent null to the table constructor, and publishes the returned pointer to `+0x458` only after constructor return. Existing non-null state returns the cached object. The docking caller then uses the returned widget with the dock at main-window **`+0x460`**, through `QDockWidget::setWidget` at `0x14001a52a` after addDockWidget. The bounded captures confirm these call sites and argument flow; they do not prove every startup/indirect call or actual runtime parent/destruction state completed.

This owner is separate from the Project/SUB/Study that InputTable just saved. Normal Project release must not be treated as destruction of this session/main-window widget. An actual lifecycle binding must track its QObject guard and parent ownership rather than equate an escaped diagnostic lease with native liveness.

## Constructor members and registration order

The selected complete constructor establishes:

| Member | Source-qualified construction/purpose |
| --- | --- |
| `+0x38` | allocate/zero a `0x98` UI payload; call setup function `0x1400fcca0(payload,this)` |
| `+0x40` | initialize cached Project pointer null |
| `+0x5c` | initialize integer 0 |
| `+0x60`, `+0x68`, `+0x70` | construct distinct auxiliary objects through `0x1400ff0b0`, `0x1400ff570`, `0x1400ff840`; their full types/effects are outside this audit |
| `+0x78`, `+0x80`, `+0x88` | initialize null |
| `+0x90` | initialize a string-list holder, populated by the constructor's static keyword-selection loop |
| `+0x98` | initialize byte false; refresh writes true while applying UI state and clears it on normal completion |
| `+0x9c`, `+0xa0`, `+0xa4` | initialize integers -1, -1, 0 respectively; initial refresh may change some cached state |
| **`+0xb0`** | allocate `0x20`, construct **QTimer(parent=this)**; local vtable `0x1420fbb78` RTTI also identifies QTimer |

At `0x1400eb8a4`, constructor connects that timer's **`2timeout()`** to this table's **`1slotUpdateUI()`**, ConnectionType **0 (AutoConnection)**. This timer connection is distinct from the global messenger's DirectConnection invocation. No current thread, native timer ID, activation or queued-timeout state was observed.

At **`0x1400ebaf8`**, constructor calls the UI refresh body `0x1400f47e0` before registration. It subsequently disables selected controls and installs UI-item connections. Only at **`0x1400ebc5c`** does it call global registration `0x1402cfbe0`, owner `0x143d76700`, key `2signalDoeTableRefresh()`, receiver=this, slot `1slotUpdateUI()`, flag0. Thus a constructor stopped before that site has not registered this receiver, and an unreturned constructor has not yet been published by the main-window getter.

The class deleting destructor `0x1400ffb90` calls body `0x1400ebc80`. That selected body reinstalls its vtables, frees the UI payload, destroys the list holder and calls base destructor `0x140377ac0`. It contains no explicit unregister call for DoeTableRefresh. Complete Qt parent/child destruction and weak-guard invalidation were not executed or closed here; use actual QObject liveness rather than pruning the persistent notifier history on Project destruction.

## Method binding and first synchronous effect

PE-resident static metaobject **`0x14216a080`** points to string table `0x142169dc0` and metadata `0x142169bc0`, revision6, 22 methods, method-table offset14 words. Local method **0** has row `[14,13,13,13,10]`, decoding to exact **`slotUpdateUI()`** with no arguments.

Primary vtable `+0x10 ->0x1400d9ba0` is qt_metacall. It delegates to base, then dispatches local IDs below22 through **`0x1400d9880`**. Jump-table entry0 at `0x1400d9a88` selects **`0x1400d98dc`**, whose call at `0x1400d98e1` targets **`0x1400f4e10`**. Code returns at `0x1400d9a87`; subsequent table bytes are DATA and are excluded from the code capture. The metaObject accessor also admits a dynamic QObject metadata pointer when present; no runtime dynamic override was observed.

The complete 51-byte slot `0x1400f4e10..0x1400f4e43` does exactly:

1. Load table **`+0xb0`**, call import **`0x141e045d8 = QTimer::stop()`** at **`0x1400f4e25`**. There is no timer-null guard in this body.
2. Invoke the actual table's virtual **`+0x1b8`** at **`0x1400f4e38`**. The identified primary vtable resolves this to **`0x1400f47e0`**.
3. Return after that virtual returns.

The slot does not call QTimer.start, singleShot or postEvent before these effects. Its separately connected timeout route may invoke the same slot later under Qt's own scheduling; that does not make this global message delivery queued. Complete Qt timer cancellation/posted-event behavior requires the actual timer/thread/dispatcher state and is not inferred from this import call alone.

The retained complete global dispatcher `0x1402cf3c0` checks suppression first, otherwise resets flag90, snapshots the key's ordered registrations, skips dead guards locally and calls QMetaObject::invokeMethod with **ConnectionType1 (DirectConnection)**, ignoring its Boolean result. Suppression or a dead/non-invoked registration is distinct from a successfully resolved live table slot. Actual runtime population and invocation equivalence remain prerequisites for public use.

## UI refresh state after timer stop

The complete selected `0x1400f47e0..0x1400f4e06` begins:

1. **`0x1400f4800`: table `+0x98 = 1`.** This store precedes any current-Project resolution.
2. Inspect selector integer `+0xa0`; call **`QWidget::setEnabled(uiPayload+0x30, selector>-1)`** at `0x1400f482a` or `0x1400f4844`. The native body assumes setupUi supplied the live widget.
3. Resolve global main-window context `0x143d766b8`, dynamic-cast from SfMainWindowBase to SfMoMainWindow, and obtain its **current Project** with `0x14001d510`. Store it in table `+0x40` at `0x1400f48a3`.
4. For a non-null Project, cache Project unit (`0x1400a2180`, native Project `+0x20`) in table `+0x58`, active SUB through `0x1400a7390` in `+0x48`, and that SUB's manager through `0x1400b5a90` in `+0x50`. A null current Project clears `+0x48/+0x50`. Failed main-window cast likewise clears those two aliases; it does not establish a fresh independent Project.
5. Continue actual list/table/control updates and cached selection work. On normal tail completion, **`0x1400f4ded` clears `+0x98`**. The descendants of those UI calls, signal/reentrant effects and all selection/content paths remain outside this audit's completed behavioral boundary.

The table therefore refreshes the **actual current global Project/Study at invocation**, not a copy of the Study pointer passed to the XML writer. It cannot be represented by changing Operation.hasDoe, setting a diagnostic "refreshed" Boolean, or assigning the caller's Study without its real context. Timer stop and the `+0x98` store must survive a later refusal; do not invent unconditional cleanup of the byte on an uncompleted native path.

## InputTable false-result continuation and integration boundary

The original `0x1400d3200` saves the writer Boolean, builds ten empty QGenericArguments, emits **`2signalDoeTableRefresh()`** at `0x1400d369d` without passing that Boolean, then returns the saved result. A false open/post-close result therefore reaches the same no-argument slot and first timer/UI effects as true. If delivery returns, the public Type2 `0x140099040` arm ignores false and proceeds to current/LHS/FF/USR files. If delivery refuses/throws, XML cleanup/error/partial-file effects already happened and later PAR stages remain unreached.

The next actual public integration guard is before the **real table QTimer stop**, unless complete actual suppression/dead-receiver behavior closes an earlier return. Required owner facts are the session/main-window table cache, dock/Qt parent lifecycle, complete ordered same-key notifier population, actual QTimer/thread/state, completed UI payload and the table's ordinary/dynamic metadata binding. The existing Boolean diagram event receiver is a different QObject and cannot supply these timer facts.

Do not add another default empty-key helper. Bind this owner to the XML seam only at the actual reached message stage, preserve already committed XML/error effects, then advance in order only as each real effect is qualified. If timer stop is actually modeled, the next bounded state transition is `+0x98=1` followed by the live widget enable call; unqualified UI ownership must refuse there with earlier effects retained. Keep B121 and Type3 success guarded.

An integrated test contract should cover true/false XML results both reaching the same no-argument dispatch; suppression preserving flag/timer/UI state; actual ordered weak receiver/death handling; timer stop before `+0x98` and first widget enable; Project release/context change before current-Project lookup; constructor refusal before registration/publication; receiver lifetime across Project Close; and refusal after timer stop retaining XML/error/timer prefix effects while skipping PAR output. Declared diagnostic timer/UI drivers alone do not establish native completion or fresh-original parity.

The CP64 architecture lane also established that the current public ABI activates SUB0 type0; an explicitly selected Type2 SUB1 shared-writer fixture is an integration test of that real shared writer, not proof that public Open/Save selects Type2 automatically.

## Evidence receipt and limits

Original PE SHA-256 remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. `analysis/resume65-doe-refresh-pe-receipt.json` hashes **`bb41b752bf8f46f8b6449f596f118b639b4b501913d984b3362b59939f03520b`** and records 10 new captures totaling **66,805 bytes**, decoded metadata/RTTI/imports and qualified direct-call candidates.

| Complete selected body | Body SHA-256 |
| --- | --- |
| Table ctor `[0x1400eb6f0,0x1400ebc73)` | `192e4563aaf27f7a6ca8c2c0da54ef5fb8cfdbabe43e3f9888291cda7397b493` |
| Getter `[0x1400196d0,0x140019750)` | `50372bd1d38fac95e06367f948f3b53f77a9bc631f28f87d6d7b862d5e693646` |
| Slot `[0x1400f4e10,0x1400f4e43)` | `32e64ca5d8545782c6df760ae88f856e2182507e9b14130e9d56fd121f869176` |
| Refresh `[0x1400f47e0,0x1400f4e06)` | `e38e7942cf586759712b6bc860d9eb12debe2d10fdfe2179fe4fa0190eef566a` |
| Destructor `[0x1400ebc80,0x1400ebcea)` | `8c55b613112f39de8df599c948f589465d8a7d7d1b0717424e1976679c017466` |

The receipt distinguishes full selected unwind bodies from caller windows and static-metacall code excluding its data table. No runtime table population, timer activity/thread IDs, complete UI/widget effects, complete constructor/destructor dependencies, original false-result execution, public active DOE success or objective completion is established. Token measurements are unavailable.
