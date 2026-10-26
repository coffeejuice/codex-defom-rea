# CP67: minimal typed ownership for widget-area replacement

Reuse the existing WidgetRuntime/WidgetState machinery for actual QWidget identity, runtime/thread binding, counters and qualified event prefixes. Add one Session-held area bundle with distinct typed main-window field, stack, scroll-area, impl, widget and deletion-timer records. None of those records should default a completed area's old current to null. Its constructor's placeholder path, replacement mode and current-guard writes must determine that state at the actual source boundary.

This is a read-only design audit. Only this report was written; no source, build, tests, GUI, original execution or mutable REA. Native qualifications and exact disassembly/import receipts remain `analysis/resume66-widget-area-session-owner.md` and its referenced evidence; this proposal introduces no newly observed runtime values. CP64/65 helpers remain frozen.

## Existing seams that can be reused

| Existing candidate seam | Useful guarantee | Boundary that must stay visible |
| --- | --- | --- |
| `WidgetState`, `view_manager.hpp:344` | One component identity, canonical shared-owner check, separate native-alive status, runtime/thread, parent/layout/child-suffix state, explicit unavailable properties and receiver profile. `matchesComponentOwner` in `view_manager.cpp:716` compares pointer and control block. | Construction is private and granted to concrete owner/cursor classes. New area/stack/widget components need an explicitly reviewed typed construction seam, not a synthetic void token or unrestricted setter. Diagnostic shared leases do not imply native liveness. |
| `WidgetRuntimeState`, header459 | Same application/thread owner, partial allWidgets registrations, qualified private construction inputs, counts and common event observations. `bindWidgetComponent` binds an existing canonical component. | Binding is not native QWidget construction or allWidgets registration. `populationComplete()` remains false. Startup observations do not supply post-Open area/widget values or empty filters. |
| `QObjectConstructionRecord`, header320; runtime commit overloads | Exactly one QObject count effect can use the same process runtime; the plain-record overload is available for non-QWidget allocations. | It explicitly does not model QObject construction, affinity, parenthood or destruction. Impl/timer need their own typed QObject lifetime/thread/parent records rather than fake WidgetState instances. |
| `WidgetConstructorTail` and shared WidgetThreadState | Reuse actual receiver/runtime/thread at the common QWidget tail; shared posted queue and notify delta remain on that thread. | Tail still stops at raw0x4a PostPending. No area cursor may mark it complete or create a second queue. The Forming300-specific base cursor is not a generic area constructor. |
| `WidgetEventDispatch`, `widget_events.cpp:203` | Retains actual frame/event/receiver lifetime, canonical runtime/thread validation and exactly-once notify-depth entry across pending stages. It qualifies application/receiver filters before dispatch. | Only selected synchronous raw0xaa,0x0f,0x44 QWidget profiles are closed. It is not a generic QWidget::show/setParent executor, QTimer receiver, QStackedLayout dispatcher or QObject deferred-delete implementation. |
| `GridLayoutState`, header647 | Actual parented allocation identity, native-alive check, thread, known/partial layout flags, empty item collection and constructor/install stages. | The item collection is intentionally constructor-empty; no concrete widget-item insertion/removal API exists. Area add/removeWidget must implement the native QLayout/QGridLayout path and its actual item ownership rather than push a new pointer into an unrelated collection. |
| `SessionNotificationState` | One shared suppression/flag90/map lifetime; existing registries use typed weak receivers and, for ViewManager, a separate native-alive check. | No area registration list exists. The ViewManager four-key map cannot stand for area/impl keys, and absent population is not empty. |
| CP65 `WidgetAreaLazyParent/Getter` | Actual qualified parent alias, unknown/null/cached `+3c8`, allocation/constructor-return/store order and canonical control-block binding. | It deliberately does not create area QWidget/impl/registration. A returned area is not a current editor and not an admitted stack page. |
| CP64 `BooleanSynchronousUpdateData` | Actual Project/Operation/class/model binding plus receiver/editor aliases, explicit admitted suffix and consume-before-effect. | It does not authenticate aliases, allocate a native widget or supply notifier prefix/current guards. Future typed owners must provide these aliases and check their separate native-alive status at admission. |

The existing concrete component pattern is useful: MainView/ViewManager/SplitView own a WidgetState whose identity is their actual component address and whose canonical owner is initialized once. Native-owner release invalidates behavior while diagnostic leases may survive. Area components should follow that pattern, using the existing shared runtime/thread. They must not replace MainView or borrow its native identity: original SfMoMainWindow and SfMoMainView have distinct metadata and layout fields.

## One bundle, separate native records

The following names are proposed interfaces, not existing source. They can be nested records/classes in one area subsystem; a new generic GUI framework is unnecessary.

| Proposed typed record | Minimum required state and native identity |
| --- | --- |
| **MainWindowAreaFields** | Actual SfMoMainWindow identity/lifetime, qualified optional `+3c8` area field and `+480` stack field. Connect CP65's lazy getter to this exact owner. Unknown initial fields remain unknown until constructor initialization or a source-point observation closes them; do not assume null from C++ record construction. |
| **PropertyStackState** | Actual QStackedWidget WidgetState and page-layout owner, ordered known/unknown page population, actual page identities, optional current index and selected native effect cursor. Initial central addWidget precedes area's minimum-size call and area addWidget. Page membership, parent transfer, callback delivery and selection are distinct. A stored area or editor `shown` flag must not select its page. |
| **WidgetAreaState** | Actual QScrollArea WidgetState, actual impl allocation and back-reference mapping, and separately qualified scroll-area content/viewport state. Its content cell must remain distinct from impl current: native changes impl current before the outer takeWidget/setWidget suffix, and Qt callbacks can observe intermediate relationships. Do not assume final viewport parent without its Qt continuation. |
| **WidgetAreaImplState** | Plain QObject identity/native-alive/thread record; optional flags `+10/+11`; three distinct guarded references current `+18`, saved `+20`, pending deletion `+38`; back-reference area `+28`; actual layout `+30`; timer `+40`. Guard known-null differs from unknown. References resolve through canonical widget ownership plus native-alive status. |
| **AreaWidgetAllocation** | Distinct actual QWidget allocation/WidgetState for each placeholder or Boolean editor, with type/projection kind, native lifetime and source-specific construction/adoption cursor. A placeholder is a real generic widget/layout/UI allocation with receiver effects; a Boolean editor has its own Fill/selection snapshot and operation binding. Opaque unknown kind remains guarded. The native old widget and newly created editor cannot share the class-private value's identity simply because the current adapter uses one embedded snapshot. |
| **AreaDeleteTimerState** | Actual impl-owned QTimer QObject identity/registration/affinity and explicit timer state, separate from the1000ms diagram timer. Start15, timeout slot and deleteLater are admitted native effects; they do not arise from elapsed time or a Boolean due ID. |

Keep diagnostic allocation retention separate from Qt parenting and guarded references. The raw `+38` is one native guard, not an invented vector of pending widgets; it can be overwritten on later replacement. Live detached allocations/posted deletion work require their actual ownership/lifetime projection independently of that guard. The runtime's weak allWidgets list is not a heap owner. Never immediately destroy an old `unique_ptr` just because current changed.

Impl constructor passes null to its QObject base, stores the area back-reference and creates a timer parented to the impl; its layout is parented to the area. Do not infer impl is a QObject child of the area from its C++ placement. C++ deletion/Close ownership must follow separately recovered destructors. A Session-held bundle is adapter retention, not a claim to have executed native destruction.

## Source effects and exact guard transitions

The same typed state must carry each of these source effects in order. A callback-return boolean without the actual represented changes cannot advance the public path.

1. **Constructor chain.** Reuse actual WidgetRuntime inputs and constructor stages for QStackedWidget/QScrollArea/placeholder widgets; plain QObject stages for impl/timer. Commit area `+28`, registry entries and parent `+3c8` only at their source stores/returns. The impl's early null guards do not certify constructor-end current: area constructor calls `d14f70(nullptr)` and `+11=1` permits placeholder factory `dad540` and its two WidgetDockHideBtn emissions.
2. **Property stack.** Original `18790` adds the central widget at18888, sets area's minimum400x300 at188ba, then adds area at188df. Recover the selected QStackedWidget/QStackedLayout insert/currentChanged path under actual page/layout owners before claiming membership or current index. `slotGetPropertyStackWidgetIndex` directly reads `+480` currentIndex; keep that returned-value query on the real shared notifier. No editor-local integer replaces it.
3. **Show mode.** `d14fc0 ->dae230` precedes impl replacement. Its true mode saves current to `+20` and hides it when live, then `+10` becomes the inverse mode. Preserve the actual payload/mode and saved guard, not a generic replacement-default bool. The registered no-argument ShowWidgetArea method still needs its inherited/default-argument resolution; the derived QWidget* overload alone is insufficient.
4. **Old current.** Resolve actual current `+18` and compare incoming identity. For a distinct old current, remove from layout first. With `+10!=0`, copy it to `+38`, detach/remove/start15/hide, deliver source notifications/deleteLater, and clear `+18` at `dadfec`. Its native allocation can remain live. With `+10==0`, hide old at `dae007` and leave current pointing to it until the later replacement store. Unknown old kind/flags stop before these effects; they never choose the null branch.
5. **New incoming.** Execute layout/margins/item changes, addWidget at `dae18c`, setParent at `dae1a6`, show at `dae1b4`, and Enter at `dae1f9` before assigning current `+18` at `dae213`. Boolean Enter itself is empty, but previous Qt calls may reenter. In the deletion-mode branch Enter may see null; in the hide-only branch it may see the old current. Do not force a single pre-Enter current profile or invent rollback/revalidation absent from native code.
6. **Same current and scroll suffix.** Incoming==current can skip the incoming layout/show/Enter/store portion at the inequality guard `dae0c5`. The outer wrapper still calls QScrollArea::takeWidget at `d14f96` and setWidget at `d14fa6`. Keep those native fields/callbacks independent of impl guard equality.
7. **UpdateData admission.** At an actual eligible emission, resolve the same registered live impl and actual current guard/cast; then CP64 invokes the real Boolean Apply synchronously. Revoke a prior CP64 scope at the original guard-clear/store point: `dadfec` for the deletion branch, or `dae213` for hide-only replacement. Saved/pending old references do not preserve permission to Apply it as current.

The new callback cursors should reuse existing canonical receiver/runtime/event checks, retain mutations before a pending/throw boundary and resume the same call frame. A generic `setParent` field assignment, `show=true`, timer counter or signal log cannot substitute for the selected Qt effect. Native failure/exception behavior needs its original branch; do not add transaction rollback or a safer failure policy.

## Shared notification ownership

Extend the same SessionNotificationState with an explicitly qualified optional area registration population. Each entry needs exact native key/bare method, actual typed receiver (area for Show wrapper; impl for UpdateData/leave/delete/update UI), insertion order, weak allocation owner and native-alive check. Preserve native append/dedup policy and local receiver-list snapshot dispatch; do not turn these custom notifier registrations into a second Qt event queue.

Only the typed area owner may expose an impl alias and current editor alias for CP64 after the appropriate guard commit. That conversion is an internal bridge over existing control blocks, not fresh identity tokens. The shared suppression, flag90 clear and key/list prefix remain on SessionNotificationState. Other registered receivers and callback order must stay qualified before a public whole-emission claim.

## Smallest safe source increment

First add the **typed storage/lifetime skeleton** under shared source ownership: one Session-held pending area bundle, concrete canonical WidgetState allocation seam for its QWidget components, plain impl QObject record, and explicit unknown current/saved/pending/flags. Reuse CP65's lazy allocation cursor and leave impl/registration unresolved. Storage construction itself must not increment native counters, register allWidgets, select a page, synthesize a placeholder or attach an editor.

Then close one selected native ctor/effect cursor at a time: actual QObject/widget base, impl's source null stores/back-reference, timer/layout allocation and registration, constructor placeholder path, then one explicitly observed old-placeholder replacement. Concrete widget layout items and scroll/stack effect APIs need root-reviewed ownership changes in existing runtime classes; a Boolean-only suffix wrapper cannot supply them. Stop at the earliest unclosed callback rather than returning a completed adoption token.

After that work, a focused candidate test should build a typed placeholder owner, not assume null. Cover both `+10` branches, same-current input, callback throw before current commit, live pending old allocation, scroll-content/current mismatch during the suffix, native-alive invalidation with retained diagnostic leases, colliding owner addresses/control blocks and cross-session rejection. Only a fully qualified receiver/emission bridge may create a CP64 scope. Neither these tests nor typed storage will establish first fresh-Open emission/timer delivery.

## Candidate source identities inspected

All paths below are relative to `source/apps/moproj2mst`; these hashes qualify candidate capabilities, not original behavior.

| File | SHA-256 |
| --- | --- |
| `include/m2m/engine/view_manager.hpp` | `c0c146eddb8cba22f30b573a9109c97494046305dd81fdb03c2e6164433d9c53` |
| `src/engine/view_manager.cpp` | `dc6ad8356e061bf7e9aebaa1d32ef2caa8a0d4d41da89720c023a4b825899a4c` |
| `include/m2m/engine/widget_events.hpp` | `e75a9f7631dab2202a894310e94adfbda0906232074a6b57c8ab1531cd55b4ec` |
| `src/engine/widget_events.cpp` | `1ddfbe54765865f795262f93279a9ae2db36d6b640eedcce54b7c0c6f2ca14aa` |
| `include/m2m/doe_session_notifications.hpp` | `be68879bf7756a67b229662d5d2397f16d89668c3accf277c7a97b442c5ad625` |
| `include/m2m/engine/boolean_lifecycle.hpp` | `976cd0c9b2388d3e14de53c906ebfa027f09dbe2c78699247198d289fabce69a` |
| `include/m2m/engine/widget_area_lazy.hpp` | `d5795e6a8aa6046ae4ebfca78d03bfef3a226b5eca5c7e4787cfcf5475199e5b` |

The existing public raw0x4a PostPending continuation, first-Open timing, ShowWidget overload resolution, Qt callback populations and actual dispatcher values remain guarded. This report proposes ownership boundaries; it enables none of those paths.
