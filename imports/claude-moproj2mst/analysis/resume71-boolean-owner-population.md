# CP71: main-window/area receiver population and first Apply

The native owner-population chain is now tied to the actual SfMoMainWindow constructor. It initializes area field `+3c8` through a tiny null-store leaf, then later builds the property stack/area and its area/impl registrations before installing the main-window ShowWidgetArea and property-index registrations. These are conditional source-order facts, not observed completion of the original constructor or a candidate Session receiver population. First fresh-Open Apply and unknown Qt delivery remain guarded.

Only this report and bounded unique static evidence were written. No source, build, tests, GUI, original execution or mutable REA. Original EXE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Actual native owner and initialization edge

New `resume71-boolean-window-construction-static.json` captures `140010490..1400105d3` (end exclusive). At `1400104d2`, it calls base constructor `140c98b00`; at `1400104dd..1400104e4`, it installs primary vtable `1420fb108`, independently tied by CP66/70 metadata to SfMoMainWindow. It then calls initialization `1400106d0` at `14001058e`, followed by setup `1400111c0` at `140010598`. Thus this field/receiver owner is the native main window, not SfMoMainView, the diagram timer receiver, OperationClass, GuiModel or an arbitrary Session allocation.

New initialization capture `1400106d0..1400108d8` proves call `14001080c ->1400184f0`. The new bounded leaf capture is exact through RET `140018505`:

```
1400184f0  spill this
1400184f5  load this
1400184fa  [this+3c8] = 0
140018505  ret
```

The leaf itself has no Qt call, allocation, signal or event dispatch. This is the smallest newly admissible **conditional field-state edge**: at that exact initializer point for the actual canonical main-window owner, area `+3c8` becomes known null. It does not initialize stack `+480`, native-alive status, area/impl, a current editor, receiver lists or a callback schedule.

Do not move this null store to generic Session construction. There are earlier base/virtual/Qt/helper calls before reaching the leaf and later helper calls before setup. Their effects are not closed by the leaf. In particular, this proves the value immediately at the source store, not that intervening callbacks cannot create an area before a later getter reads the field. The candidate typed owner should remain unknown until a recovered execution/capture bridge reaches this specific source boundary.

The same new receipt captures `140018510`: nonnull `+3c8` is passed to virtual deleting destructor `+18(flag1)` at `14001855d`, then only after return field `+3c8` is set null at `140018575`. This identifies a later pointer invalidation boundary but does not implement native area destruction/Close or receiver cleanup; no such behavior is enabled here.

## Population order inside main-window setup

Retained complete `wave3-boolean-open-model-1400111c0.asm` shows setup `1400111c0` calls `140018790` at `14001121d`, before the relevant later main-window registrations. CP65/66 qualify `18790`:

1. Construct QStackedWidget and store main-window `+480` at `14001882c`.
2. Add the earlier central widget at `140018888`.
3. Call lazy area getter `140018460` at `1400188af`. Its known-null path allocates0x30, calls area constructor at `1400184a2` and stores returned area into main-window `+3c8` only at `1400184cb`. Its cached path does not reconstruct/register another area.
4. In the selected area-construction path, `140d14d30` constructs impl, stores it at area `+28`, then calls `140d14e90` at `140d14ddc`.
5. Register area and impl receivers as below, then execute constructor's `d14f70(nullptr)` tail. That tail may create/adopt the generic placeholder and emits WidgetDockHideBtn; area construction is not receiver-neutral. This occurs **before** the outer lazy getter's final main-window `+3c8` commit.
6. The caller reuses getter at `1400188c8`, adds area to stack at `1400188df`, builds its dock/container context and requests initial property page0 via `140018a93 ->1400320f0 ->14003227f ->140018ae0(0)`.
7. Setup returns from18790 and later installs main-window ShowWidgetArea and property-index receivers.

Every step is conditional on the preceding native calls actually returning through their selected paths. Null/cached/allocation/exception/reentrant branches retain their native distinctions. No constructor count, page index permanence, empty receiver population or successful native Qt callback is inferred.

| Registration site | Native receiver identity | Key -> normalized bare method |
| --- | --- | --- |
| `140d14eb8`, inside area ctor | Actual SfWidgetArea `this` | `2signalShowWidgetInWidgetArea()` -> `slotShowWidget` |
| `140d14f08`, same constructor registration body | Actual `area+28` SfWidgetAreaImpl | `2signalUpdateDataInWidgetArea()` -> `slotUpdateData` |
| `140011362`, after18790 in main setup | Actual SfMoMainWindow `this` | `2signalShowWidgetArea()` -> `slotShowWidgetArea` |
| `140011512`, same main setup | Same actual main-window receiver | `2signalGetPropertyStackWidgetIndex()` -> `slotGetPropertyStackWidgetIndex` |

These are custom registry calls `1402cfbe0` on global `143d76700`, with flag0 append/dedup policy, not Qt AutoConnection declarations. CP64's actual invocation leg is explicit raw type1. The current registration receipt retains carrier candidates plus existing decoded bodies; those candidates alone are not a whole-population/exhaustive absence proof. Earlier base/dynamic/other receivers cannot become an assumed empty prefix.

The area and impl entries exist earlier in source order than these main-window entries; this does not prove what unrelated WidgetDockHideBtn receivers can run during placeholder construction. That key's complete population and transitive effects remain separate. Reentrant source getters are not assumed impossible just because the outer `+3c8` store is later.

## Constructor reset callback is not loaded-project Open

The setup tail finishes its selected registrations, sets edx=0 at `140012018`, and calls `14002a0e0(this,null)` at `14001201f`, before calling `140c99a40` and returning to the constructor. The retained source qualification for02a0e0 distinguishes the explicit null branch from its nonnull Project branch that calls02a9b0.

This is a useful startup boundary: the constructor's final postOpen/reset helper is invoked **after** those registrations, with null Project argument. It must not be counted as the successful loaded-input Project attachment, nor used to invent a Boolean modelUpdated delivery or a timer count. Its global null-project notifications and UI descendants can have effects; this lane does not claim their complete neutrality. Fresh loaded-input Open still follows its separately qualified0271b0/normal-read/nonnull-postOpen route.

## Selected first-Open path after population

CP70's positive payload result remains closed: `5f36d0` sends one typed QWidget* in ShowWidgetInWidgetArea and ShowWidgetArea even though application keys spell `()`. The former selects the area's one-widget clone, supplies false, and `dae230(false)` stores `impl+10=1` atdae2b2 without a preceding Qt call in that selected branch. The latter resolves the derived main-window QWidget* overload and requests stack index1.

With a distinct live placeholder/current, the first unclosed replacement call is layout removal `dadc91`, **before** pending copy `dadcc3`. Selected delete-mode effects later clear current atdadfec; incoming Qt/layout/parent/show and empty Boolean Enter precede new current store atdae213. Saved/pending old widgets cannot authorize UpdateData as current. A same-current/null/expired/reentrant case needs its actual source profile; no case is selected from record allocation alone.

The outer selection function emits UpdateData at `5f4524` **before** Leave/new factory/show when its actual property-index query is nonzero. Once an actual live impl/current Boolean widget and notifier prefix admit it, this leg Apply is synchronous. Actual index0 takes propsToEngine rather than this UpdateData leg. Creating/filling the first editor and requesting page1 can make a **later** admitted selection eligible, but source population/order does not identify a second selection or delivered timer before fresh Open return.

Unknown Qt dock/layout/show/stack/filter/central-close callbacks could reenter. The empty Boolean Enter leaf excludes only that leaf as an additional Apply, not those descendants. The1000ms diagram registration, area deletion timer15, raw constructor0x4a and synchronous UpdateData are separate paths. Preserve the existing delivery guards.

## Candidate Session state and smallest next effects

Current inspected Session still holds shared notifier, MainView/ViewManager, runtime/thread and other typed subsystems, but no actual SfMoMainWindow/WidgetArea/impl binding or area registration list. CP68 owner records are available, initially unknown, and intentionally refuse adoption effects. A Session-held diagnostic holder would preserve state lifetime; it would not by itself certify any native constructor or registration.

Two minimal conditional effects are now concrete:

- **Initialization edge:** actual main-window field initialization at184fa can set only typed `+3c8` to known null when an admitted cursor reaches that leaf. Retain all surrounding constructor guards and actual owner identity. It must not replace later observed nonnull state or initialize other fields.
- **Reached Show receiver prefix:** after actual area/impl, shared registration, emitted QWidget* and clone/method admission, execute `dae230(false)`'s mode store `+10=1`; keep current/saved/pending/stack untouched and stop before the selected unclosed replacement effect. This is a later mutable native mode effect; CP68's immutable `observeInitialFlags` is not an executor and cannot be reused to fake an existing mode transition. A source-bound effect method/frame with proper owner/occurrence admission is needed.

Neither edge admits a public first-Open Apply. The next population bridge must connect the actual main-window allocation at10490 to CP65/68 state, preserve the area/impl registration points and same SessionNotificationState map, distinguish known suffix from unavailable population, and retain current/widget lifetimes through actual callbacks. Do not add fake receiver lists while the actual constructors remain earlier guards.

The smallest discriminating original timing evidence remains first Apply1411e2870 with actual impl/current Boolean editor, stack query result, argument/registration identity and bounded caller stack, correlated with Open entry/return, first factory/adoption and modelUpdated nesting. The available native callgraph/state edges reduce the probe scope but do not supply those runtime facts. No GUI acquisition is performed/authorized by this lane.

## Evidence and source snapshot

- New `resume71-boolean-window-construction-static.json` / asm: actual ctor10490 and its ordered initialization/setup calls.
- New `resume71-boolean-window-initialization-static.json` / asm:106d0 and call to area field initializer.
- New `resume71-boolean-area-field-init-static.json`: leaf184f0 through RET18505 and separate18510 delete-then-clear helper. Trailing leaf padding is not another procedure.
- New `resume71-boolean-owner-candidates.json` / `resume71-boolean-area-null-candidates.json`: encoded CALL/LEA/zero-store carrier discovery; positive disassembly above qualifies the chosen hits, not a complete xref/callgraph.
- New `resume71-boolean-owner-population-receipt.json`: four selected retained registration candidates and current source hashes. Actual decoded registration bodies and payload/Qt contracts remain CP63/64/66/70 evidence, plus independent `resume70-boolean-qt-connection-review.md`.

Inspected source hashes under `source/apps/moproj2mst`: Session header `6b05d5d085ff23b42e7a3f1c784ea5e92576945504e471d1dda29d44abb17a99`, Session source `99731a89662bac4b9162c22f0bbb649da3e65c64a5431c96048060774b4ab8a3`, notifier header `be68879bf7756a67b229662d5d2397f16d89668c3accf277c7a97b442c5ad625`, CP68 header `6e4e4ca3c8b2fe373f8044f7db63b63dd1974006cf0443e5e2e60aa4651788a3`, source `b77f84438ccc562174ee831bc15435ded548ce34245ae3964043663d5cf98b19`. They qualify this inspection, not a tested build or an original behavioral oracle.

Public raw0x4a PostPending, first fresh-Open timing and unavailable Qt delivery remain guarded. No source integration or behavior-completion claim follows from this read-only lane.
