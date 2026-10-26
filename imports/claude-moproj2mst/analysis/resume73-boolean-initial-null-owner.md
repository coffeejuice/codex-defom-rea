# CP73: Session ownership of the native initial-null area field

The native `+3c8=null` leaf can be modeled independently of Qt receiver delivery **at its reached constructor source point**. It cannot become an unconditional current-field default of `ConversionSession`. The first safe public binding now is Session-owned diagnostic storage whose field remains unavailable; a known-null commit needs the actual main-window owner and an admitted constructor cursor at `1400184fa`. This preserves progress on ownership without claiming that area construction, registrations, callbacks, or first Open Apply completed.

Read-only lane: only this report was written; no source/build/tests/GUI/native execution/mutable REA. Original EXE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Exact native edge and its position

CP71 immutable PE captures establish actual SfMoMainWindow constructor `140010490`, base construction at `1400104d2`, primary vtable installation `1420fb108` at `1400104dd..104e4`, initializer `1400106d0` at `14001058e`, and later setup `1400111c0` at `140010598`. The initializer calls `1400184f0` at `14001080c`. That leaf spills/reloads the same `this`, stores qword0 at `[this+3c8]` at `1400184fa`, and returns at `140018505`. No allocation, Qt call, signal or dispatch occurs inside the leaf.

Consequently the leaf's own effect is exactly one raw field write. It needs neither timer readiness nor the area receiver's existence. A partially constructed canonical native main-window instance can receive this field initialization; do not require completed QWidget construction as a fabricated prerequisite, and do not set the diagnostic native-alive flag true merely because this store was reached.

Its caller context remains material. Earlier base/helpers precede the leaf; after the leaf, `140010816 ->1400132b0`, `140010820 ->140018bb0` and other helpers run before initializer return and setup. These preceding/following effects are not closed by this leaf. There is no retained proof that the current candidate's constructor has executed an equivalent prefix or that the field stays null until a later public Open boundary.

Later setup's lazy getter `140018460` reads `+3c8`; its null branch calls area constructor at `1400184a2` and commits the returned pointer only at `1400184cb`. Area/impl registration and placeholder/Qt effects occur during that constructor, before this outer commit. Setup is earlier than loaded-project Open, and CP71's final null-Project reset is not loaded-project attachment. Thus setting `+3c8=null` at public Open or Save would move an earlier store across intervening effects and could erase a real cached area. No such integration is justified.

Separate cleanup leaf `140018510` deletes a nonnull area first, then writes null at `140018575`; the initializer is not a replacement for this destruction/Close effect.

## What current Session actually owns

The inspected Session constructor at `conversion_session.cpp:145` owns its notification state, runtime, native-view projections, model manager, DOE holder and Project scope. Its MainView object is SfMoMainView, not the SfMoMainWindow identity recovered above. The Session has no WidgetAreaWindowOwner, native main-window allocation/cursor or area/impl registration binding. Existing main-window-lifetime holders show a suitable **lifetime pattern**, not evidence that any native constructor source point ran.

Public construction of a Session, allocation of a diagnostic token, existence of `notificationState_`, a MainView pointer, a Project pointer or a nonempty source qualification cannot mint the missing constructor reachability. Project release must not reset a main-window field: the selected owner lifetime outlives Project. Session adapter teardown may revoke diagnostic admission but must not claim native area destruction or normal Close.

The available CP68 owner record already separates canonical allocation identity, diagnostic admission, unknown native-alive state, `area3c8` and `stack480`. Initial `area3c8` is unknown. `observeInitialArea3c8(nullptr,q)` records null without claiming liveness/Qt completion, but its current admission is only canonical scope plus a nonempty qualification. It is a low-level observation API, not a sufficient public proof that a Session reached the native store.

## First safe public binding and exact guard

A minimal future Session integration has two distinct operations:

1. **Bind storage/lifetime.** Retain one fresh private canonical identity and one WidgetAreaWindowOwner projection for this Session's intended SfMoMainWindow role, with all fields/native-alive unknown. A public read-only diagnostic accessor can expose that holder, explicitly as an unavailable projection until populated. It must not alias the MainView, Project, Boolean editor, stack or generic notifier allocation as the native main window. The adapter token supplies Session identity/lifetime, not an observed native address or constructor-completion certificate.
2. **Commit reached initial store.** Admit one opaque Session-owned constructor occurrence only when the actual SfMoMainWindow construction lineage, canonical owner/lifetime and source cursor identify the call `14001080c ->1400184f0` and store `1400184fa`. Commit only `area3c8=known null`; retain stack/current/impl/registration/native-alive and timer facts unknown. Then advance the cursor to the leaf's return, stopping before unimplemented caller continuation. No Qt callback is inside this increment.

The exact guard for operation2 is: **this Session's active main-window construction scope + this canonical main-window allocation/lifetime + an unconsumed source occurrence positioned at the initializer leaf, with its preceding constructor continuation actually admitted + no conflicting later field state**. Reject a foreign Session, released adapter, revived/reused allocation generation, arbitrary string, later Open/Save cursor, duplicate occurrence or already nonnull field. Do not reconstruct the guard from a caller-supplied boolean such as `constructorReached=true`.

Current public Session code supplies none of that reached-constructor receipt/cursor, so the known-null operation must remain unavailable/refuse with a precise dependency such as: `Main-window +3c8 initialization requires the actual SfMoMainWindow constructor owner and reached1400184fa occurrence; Session allocation is not that execution.` This is the recommended guard wording/semantics, not an implemented exception here.

An immutable historical record saying “native initializer writes null” can also be stored now, but must never be returned as the current mutable `area3c8` snapshot. A current-field getter needs state at its reached cursor, not an earlier constructor fact replayed later.

## Avoid two competing area fields

CP68 `WidgetAreaObservedReference::observeInitial` accepts a repeated matching observation but refuses replacement of a known reference. Therefore a CP68 known-null area cannot later become nonnull through `observeInitialArea3c8`. CP65 `WidgetAreaLazyParent` separately owns an optional raw `field_` and mutable commit counter. Merely attaching both helpers to the same Session/token does not make their field storage canonical: CP65 could commit an area while CP68 continues reporting known null.

Before any real lazy-getter integration, choose one canonical live `+3c8` cell or add an explicit source-qualified transition at `1400184cb` that updates the shared projection after the same admitted area constructor return. Preserve allocation/constructor failure no-commit ordering and any reentrant source profile. Do not “fix” this by weakening all immutable initial observations, by overwriting the typed field at Open, or by importing the raw CP65 pointer as a live registered impl. The source still guards unresolved area construction/Qt effects before such a return can be admitted.

## Focused candidate-only verification for an eventual bounded increment

A Session storage binding should verify distinct Session owner identities, unknown initial field/native-alive/stack, persistence across Project release, and revoked escaped admission after Session adapter teardown. A reached-null increment should reject foreign/stale/colliding-address owners, arbitrary or duplicate occurrences, and later nonnull state; a valid source occurrence should change only known-null `area3c8`. Registration lists, current editor, pending timer events, receiver liveness and first-Open Apply must remain untouched. If later lazy commit is added, verify typed/raw projections cannot diverge and an interrupted constructor leaves the field unchanged. These tests are proposed; this lane did not implement or execute them.

## Evidence and inspection receipt

Native evidence: `resume71-boolean-window-construction-static.json`, `resume71-boolean-window-initialization-1400106d0.asm`, `resume71-boolean-area-field-init-1400184f0.asm`, the separate18510 body, CP65 lazy-getter receipt and CP71 population report. CP70/72 retain unresolved replacement/delivery distinctions. These are source-order and body-local facts, not an original-process observation.

Inspected candidate paths are under `source/apps/moproj2mst/`:

| File | SHA-256 |
| --- | --- |
| `include/m2m/engine/conversion_session.hpp` | `6b05d5d085ff23b42e7a3f1c784ea5e92576945504e471d1dda29d44abb17a99` |
| `src/engine/conversion_session.cpp` | `99731a89662bac4b9162c22f0bbb649da3e65c64a5431c96048060774b4ab8a3` |
| `include/m2m/engine/widget_area_owner_state.hpp` | `6e4e4ca3c8b2fe373f8044f7db63b63dd1974006cf0443e5e2e60aa4651788a3` |
| `src/engine/widget_area_owner_state.cpp` | `b77f84438ccc562174ee831bc15435ded548ce34245ae3964043663d5cf98b19` |
| `include/m2m/engine/widget_area_lazy.hpp` | `d5795e6a8aa6046ae4ebfca78d03bfef3a226b5eca5c7e4787cfcf5475199e5b` |

No source-freeze/build or behavior-completion claim is made. The immediate safe increment is an unknown Session-owned holder; actual known-null execution waits for the main-window constructor occurrence bridge. Fresh-Open Apply and public constructor PostPending remain guarded independently.
