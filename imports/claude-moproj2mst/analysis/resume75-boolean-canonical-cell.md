# CP75 follow-on: one canonical main-window area cell

Use one live `SfMoMainWindow+3c8` cell for both the lazy getter and typed owner view. Retain the historical constructor-reached fact separately. Original probe004's constructor return proves earlier traversal of the null initializer for its selected owner, but supplies no current field value to seed either live cell. This report defines the reconciliation and exact next witnesses; it changes no source or guarded public behavior.

No source/build/tests/GUI/native execution/mutable REA. Evidence is the same original executable and retained capture qualified by `resume75-boolean-constructor-owner.md` and `resume75-boolean-owner-retained-receipt.json`.

## Why the current declarations cannot both be authoritative

CP65 `WidgetAreaLazyParent` owns `optional<shared_ptr<const void>> field_`: disengaged means unavailable, engaged empty means null, engaged nonempty means cached pointer. Its staged getter writes that field in ParentCommitPending and increments commits. The staged invocation holds a selected parent, allocation/result and active-frame identity; it rejects nested/concurrent construction as unrepresented.

CP68 `WidgetAreaWindowOwner` independently owns `WidgetAreaObservedReference<WidgetAreaScrollOwner> area_`: `known`, `nonNull` and a weak target. Its initial observation rejects a conflicting later value. This representation usefully preserves a known nonnull pointer when its target lease expires, but it is not a mutable lazy-getter field.

Sharing only a parent token would leave two values: CP65 could commit a nonnull result while CP68 still reports its earlier null. Repeatedly copying snapshots would also lose ordering and failure/reentrant distinctions. Conversely weakening every `observeInitial` method would incorrectly turn observations of stack/current/impl fields into execution effects. These helpers are isolated source projections, not already integrated competing native executions.

## Canonical cell contract

The following is a design schema, not a new C++ declaration or an executable native claim:

| Cell member | Meaning and constraint |
| --- | --- |
| canonical parent identity + lifetime generation | One Session-held SfMoMainWindow role, distinct from MainView, Project, diagram receiver and area. A native trace mapping additionally names process/module/thread/capture identity; captured raw addresses never identify another process or generation. |
| value knowledge | `Unavailable`, `Null`, or `NonNull`. Never collapse unavailable, expired, released or unresolved typed mapping to Null. |
| nonnull pointer identity | Retained raw/adapter allocation identity for the actual `+3c8` value. Its presence does not prove Qt liveness, parenting, impl population or receiver registration. |
| typed target binding | Optional canonical `WidgetAreaScrollOwner` for that same pointer/allocation generation. A known raw pointer may remain unbound or its diagnostic lease may expire; do not fabricate a live typed target. |
| revision and source occurrence | The last admitted field observation/write and its execution/capture cursor. This is not a count inferred from constructor return or elapsed time. |
| diagnostic admission scope | Revoked on Session adapter teardown without inventing native destruction or a null store. Project release does not revoke this main-window lifetime. |

An allocation-retention store/frame may hold strong diagnostic leases independently. Shared-pointer retention is not native-alive evidence or proof the Qt parent owns/frees the object. CP65's report/frame can retain its allocation and CP68 can retain weak typed relations while both read the same canonical cell. If a nonnull field lacks a usable return-identity/lifetime binding, a typed getter stops with that exact dependency; it cannot take the null branch.

Keep `HistoricalInitializationReached(owner,capture,constructorOccurrence,inferenceQualification)` outside the cell. CP75's original ctor-return inference can populate that historical fact. It cannot advance the current cell's revision or replace its live value.

## Source-point transitions and consumers

1. **Session diagnostic allocation:** create owner/cell with value Unavailable and native-alive unknown. Merely binding storage executes no native constructor.
2. **Initializer write184fa:** with the actual admitted constructor owner/occurrence at this leaf, set Null. It writes only the area field. This source edge is callback-free, but its preceding constructor continuation remains required. A historical store inferred before later setup cannot be applied at today's Open cursor.
3. **Getter read18477:** admit an observation of the actual parent field at that reached read. Null selects allocation; NonNull selects cached return; Unavailable guards. A direct read witness can establish current value without replaying the older initializer or claiming its exact first occurrence. Typed mapping/liveness remains separately unknown where not observed.
4. **Getter commit184cb:** after the same invocation's admitted normal area constructor return, assign that same allocation identity to the cell; the explicit native null-allocation path commits Null without a constructor. Interrupted/unreturned constructor makes no commit. Do not execute this step solely because its stage says ParentCommitPending; the enclosing source/capture occurrence must admit the store. The isolated CP65 helper's current semantics remain unchanged until a reviewed integration.
5. **Cleanup store18575:** only after the distinct source-qualified deletion helper's selected destructor returns. Do not reuse initializer or adapter teardown to produce this store.

CP65 becomes an invocation cursor over the canonical cell rather than a second owner of its value. CP68 `area3c8()` becomes a typed snapshot of that same cell, with initial observation remaining an observation API. Add source-specific cell transitions instead of general mutable setters on every CP68 reference. Stack `+480`, scroll content, impl current/saved/pending guards and replacement mode remain distinct cells.

Before a resumed getter commit, validate canonical parent, allocation/constructor result identity, active occurrence and cell revision against its selected branch. Any intervening admitted field change, expired mapping or nested entry currently stops as unrepresented. This guard preserves the bounded helper; it does not assert native reentrancy is impossible or turn a valid native reentrant branch into recovered error behavior.

## Exact next witness: choose the intended edge

**To establish the earlier initializer occurrence:** observe the selected original main-window constructor entry and retain its actual owner/frame/thread. At `1400184fa`, RAX is that owner; the leaf has not changed RSP, so `[rsp]` is its return address. Require the expected caller continuation `140010811` (rebased to the captured module). Record a unique entry ordinal, owner generation, caller and field address; at `140018505`, observe the resulting qword0 before return. No Qt call occurs between this selected store and RET. If the goal is globally first, monitor the full selected constructor interval without gaps and retain earlier/reentrant occurrences rather than choosing the first observed hit. This is the missing direct source-point witness; probe004 recorded only outer return.

**To drive the next real lazy-getter branch instead:** observe actual `140018460` entry and the read/compare at `140018477` for the same main-window owner, with ordered occurrence/thread and actual qword `owner+3c8`. This is the smaller useful witness for current state. A nonnull value needs its actual area identity/generation before typed use; null permits only the allocation request prefix. Continuing beyond constructor-pending needs actual allocation result, ctor `140d14d30` return identity and all guarded QWidget/impl/registration/placeholder effects, followed by the reached184cb store. A field read does not certify those descendants.

Both are future capture specifications, not executed actions or authorization to acquire app control. Alternatively a reconstructed constructor continuation may reach these exact source points with recovered preceding effects; the present Session has no such cursor. A qualification string, a user-declared ready flag, ctor-return-only historical receipt, or a pointer copied from PID27216 is not a replacement.

## Bounded acceptance for a later source increment

The canonical-cell integration should test one shared value through both projections; Null -> admitted nonnull commit; null allocator commit; interrupted constructor no commit; unavailable and nonnull-unbound remaining distinct; expiration/revocation preserving nonnull knowledge; foreign/colliding-address generations and source replay rejection; and diagnostic Project-release versus Session-teardown lifetime. A later field revision during a paused getter must retain the guard. No test should claim callbacks, receiver population, first Open Apply or PostPending merely from canonical storage.

Inspected sources: `src/engine/widget_area_lazy.cpp`, `include/m2m/engine/widget_area_lazy.hpp`, `include/m2m/engine/widget_area_owner_state.hpp`, `src/engine/widget_area_owner_state.cpp`, under `source/apps/moproj2mst/`. CP75's parent report binds the retained original capture/code. Source remains frozen; no integration or test result is claimed.
