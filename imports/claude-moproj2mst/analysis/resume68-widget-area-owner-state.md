# CP68: typed widget-area state/lifetime skeleton

The new isolated state model distinguishes main-window `+3c8` area, `+480` stack, scroll-area content, impl back-reference/current/saved/pending guards and explicit placeholder/Boolean widget identities. Native liveness and every field start unavailable. Source-point observation methods can record the two replacement modes' different guard writes without executing or claiming completion of any Qt callback.

This is typed diagnostic storage, not an adoption executor, Session bridge, captured native construction or first-Open Apply. `WidgetAreaImplOwner::requireAdoptionEffects()` always refuses. No public PostPending/Boolean path, existing helper, Session, WidgetRuntime or shared CMake file was edited.

## Frozen exclusive files

| New file | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/engine/widget_area_owner_state.hpp` | `6e4e4ca3c8b2fe373f8044f7db63b63dd1974006cf0443e5e2e60aa4651788a3` |
| `source/apps/moproj2mst/src/engine/widget_area_owner_state.cpp` | `b77f84438ccc562174ee831bc15435ded548ce34245ae3964043663d5cf98b19` |
| `source/apps/moproj2mst/tests/mdt/test_widget_area_owner_state.cpp` | `d2b4ca91451a3e874bafcec0feacc0cc3eeb06edc02043f4ae3938114e5c2a1e` |

Only these announced new files and this receipt were written. No compiler, build, CTest/test executable, GUI, original runtime or mutable REA ran. Root owns coordinated validation.

## Native qualifications and bounded implementation

The design/source basis is `analysis/resume67-widget-area-typed-owner.md`, with native address/import/metadata qualifications in `resume66-widget-area-session-owner.md`. Original EXE SHA-256 remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; no new native runtime values were acquired.

The typed records are:

- `WidgetAreaWindowOwner`: independent initially unknown area `+3c8` and stack `+480` links.
- `WidgetAreaStackOwner`: initially unavailable current index. Its initial observation is not a page-selection operation and cannot later become one by replacing the observed value.
- `WidgetAreaScrollOwner`: separate impl `+28` and scroll content links. Updating impl current does not change scroll content or infer a viewport parent.
- `WidgetAreaImplOwner`: initially unavailable area back-reference `+28`, current `+18`, saved `+20`, pending `+38`, delete-mode `+10` and placeholder-enabled `+11`.
- `WidgetAreaChildOwner`: explicit separately lived widget allocation; kind starts Unknown and can be qualified as Placeholder, BooleanEditor or Other. It cannot silently switch type.

All use `WidgetAreaLifetimeOwner`: one explicitly supplied actual allocation alias, canonical record/control block, unknown native-alive value and separate revocable diagnostic admission. Creation does not construct a native object, increment counters, set QObject parenting, register allWidgets or bind a runtime. Observing native-alive false is terminal for that record; releasing diagnostic admission does not claim native destruction occurred.

Reference snapshots distinguish unknown, observed null, live observed nonnull, and previously observed nonnull whose typed owner is unavailable. Weak expiry or native-liveness loss never fabricates a native null field store. `usableNativeTarget()` is only an identity/lifetime check, not callback/constructor/Apply admission. Full native QObject guard invalidation/destruction behavior remains future effect work.

Initial field observations are immutable except idempotent repetition of the same canonical link. Targets must be their canonical control blocks with available observation scope; a separate shared control block at the same C++ address is rejected. Scalar observations reject conflicting values before partially merging other flags. Nonempty qualification text is caller provenance, not authentication of an original capture or proof that the surrounding source path ran.

## Branch-specific observed writes

Only three source-write observations mutate an already observed impl guard. They require an explicitly live impl, an actual live old widget of known kind when nonnull, a known mode, matching canonical owner identities and the corresponding pointer preconditions. They do not execute intervening callbacks or claim a balanced native trace.

| Method/source site | Represented field change | Preserved distinction |
| --- | --- | --- |
| `observePendingCopyAtDadcc3` | With delete mode `+10=true` and matching current old owner, assign that owner to the single pending guard `+38`. | Overwrites the one observed guard, not an invented deferred-deletion vector. Old allocation remains live; current/saved/scroll content do not change. |
| `observeCurrentClearAtDadfec` | With the same current/pending old owner and delete mode true, record current `+18=null`. | This source clear precedes incoming Enter in the deletion branch. It does not delete the old allocation or consume its timer/deferred event. Hide-only mode cannot use this clear. |
| `observeCurrentStoreAtDae213` | Record incoming as current at the native post-Enter-site store. Delete mode with a nonnull old-at-entry requires already observed current-null/pending-old. Hide-only mode requires old still current. Explicit null-old-at-entry requires observed current-null, never an unavailable guard or a live placeholder. | No layout/parent/show/Enter/notifier/scroll operation is run or marked complete. Same incoming/old identity rejects this source store because the selected native same-current path skips it. |

These methods mirror qualified observations of writes in the selected bounded non-reentrant profiles. They are not a call-frame dispatcher and do not prove which preceding callback caused a field value. A capture/integration layer may begin from a source-qualified intermediate observation; this skeleton does not authenticate complete prefix/occurrence ordering. Reentrant changes or other source writes remain guarded until their real effect cursor is implemented. The observations must not be called by public conversion code merely to manufacture desired state.

An area constructor can attempt to install a generic placeholder through its `d14f70(nullptr)` tail; no node or initial current state is automatically created here. A known nonnull Placeholder cannot be passed as a null-old-current profile. Old current losing that role does not imply its allocation is destroyed, and changing impl current never implicitly selects the property stack or changes QScrollArea content.

## Focused tests and preflight

The new automatically discovered target `m2m_mdt_widget_area_owner_state` contains12 Release-active Testkit cases covering:

1. All references, native lifetimes, kinds, flags and index start unknown; adoption-effects gate refuses.
2. Window area/stack, area/impl back-references and scroll content are separate; field linkage does not select a page, and initial observations cannot be replaced.
3. Delete mode copies pending, clears current, then records new current; old stays live and scroll content remains the old widget.
4. Hide-only mode cannot copy/clear via deletion sites and keeps old current until the later store.
5. Delete-mode post-Enter-site store refuses missing pending-copy/current-clear state.
6. Null-old profile cannot replace a placeholder; unavailable current is not known null. Explicitly observed null is handled without inventing pending/placeholder flags.
7. Unknown kind/liveness/flags do not choose a replacement branch; conflicting flags do not partially update state.
8. Same-address/different-control-block alias, unrelated old owner and same-current store are refused.
9. Unknown mode and impl liveness are tested independently with the other preconditions already qualified, avoiding a masked guard test.
10. Dead native owner cannot be revived; lifetime loss/revocation does not auto-clear a recorded field.
11. Allocation admission or typed-node expiry remains distinct from known null.
12. Pending `+38` is one overwritten guard; saved `+20` and its old allocation remain independent.

Fixtures label their inputs as synthetic source-point witnesses, not native captures. The assertions test typed relationships and state boundaries; none claims Qt callbacks ran.

Source preflight passed: Testkit entry point exists; no C `assert` or unsupported REQUIRE_EQ is present; quoted includes resolve under the source/configured include roots; all three files have no trailing whitespace. Existing core/test globs discover these files, so no shared CMake edit is required. No compiler/test success is claimed; root must run the new focused target and coordinated integration.

## Remaining ownership/effect integration

The skeleton does not allocate actual WidgetState/QObject/runtime components, construct a property-stack page population, model layout items, own a native timer, install registry receivers, execute placeholder UI callbacks, release native widgets or supply an admitted UpdateData emission. Window/area/impl cross-links are individually qualified observations, not an authenticated Session hierarchy. Actual same-Session/runtime/thread and full native-alive relationships remain the future bridge's responsibility.

Root can later bind these typed records to the actual Session-owned main-window/area allocations and existing canonical WidgetRuntime components at source-qualified stages. It must preserve the actual native counter/constructor points, Shared SessionNotificationState, callback effects and branch-specific current revocation. Only after those effects and actual emission/current-widget admission are closed may the CP64 suffix use the resulting aliases.

The public raw0x4a guard remains `Constructor raw0x4a posting requires the actual shared queue/dispatcher continuation`, verified unchanged during preflight. No first-Open Apply, ShowWidget overload, dispatcher value or PostPending readiness is fabricated by this state model.
