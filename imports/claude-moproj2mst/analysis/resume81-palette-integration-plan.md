# CP81 bounded palette data/reference and partial-slot integration

The smallest coherent integrated cut is one PaletteNativeRefState per actual
runtime-interned WidgetPaletteDataState, one native getter token in each retained
QApplicationPaletteRead, and the first private pointer store on SAME WidgetState.
After that store, stop before temporary deref. No count value is seeded, private
word0/ref/font/counters/public entry are added, or zero-destruction branch opened.

This plan was prepared read-only during CP80 acceptance. Root subsequently
confirmed CP80 archive and approved the exact seven-file source set below. The
implementation receipt is separate; this plan itself makes no build/test/runtime
claim. No GUI/original execution/mutable REA or new native capture is involved.
Token usage is unavailable.

## Canonical shared owner and getter token

WidgetPaletteDataState gains one private shared PaletteNativeRefState and a const
nativeReferenceState diagnostic. Create it lazily in the validated KnownPresent
runtime getter, using qualification derived from this exact runtime/data alias.
Data pointer AND control block already resolve through THIS Values registry;
never build a new data owner or attach a state from an external helper/profile.
Unknown and KnownNull slot reads remain their CP79 no-provider dependencies and
create neither ref state nor native token.

All owner/data/ref-origin preflight precedes the getter's native ref. Prepare one
GetterTemporary token, retain it once at the source6504a8c6 boundary, and carry
that SAME token with the returned value. Physical count begins UNKNOWN; its raw
word stays unknown while refMutations/mutationSequence/nativeValueBalance each
advance once. No native count observation API is required in this smallest cut.
Host report copies share one token without another ref.

QApplicationPaletteRead should store the mutable token privately, with friendship
for WidgetRuntimeState/actual placeholder constructor and a public const token
diagnostic. This avoids exposing retain/release as an accidental report operation.
Standalone CP77 reads can keep null tokens; they cannot satisfy the actual
runtime-bound private pointer-store admission. No owner-helper algorithm changes
are needed. The runtime-mediated getter alone supplies the new provenance.

Runtime destruction invalidates every attached ref state's native callability,
without deref/count reset/free. Escaped const data/state/token diagnostics retain
historical receipts but do not extend runtime/module lifetime. Do not infer Qt
refs from their C++ shared counts or treat data descriptor survival as native
module availability.

## Partial private source slot

Add WidgetPaletteConstructionSlot with actual shared data and optional<uint32_t>
groupResolveWord. It lives on SAME WidgetState as a private optional, exposed by
const diagnostic. Presence means source6504abcb pointer was stored. Word remains
UNKNOWN in this cut, and no native destination reference exists.

palette() remains absent. The partial slot is one source field under construction,
not an independent palette graph; both data references point to the same interned
owner. Later completion can promote that same slot once, but is not implemented
now. Host native-owner invalidation can drop metadata without claiming a native
pointer rollback or Qt destructor.

At CP79 PrivatePaletteCopy, validate SAME runtime/app_pal/getter data owner, exact
state/token origin, GetterTemporary kind, Held stage and its completed retain
receipt. A standalone/copy/missing/released token cannot authorize the store.
Only then store the actual data pointer once, record PrivatePaletteDataStored and
advance to PaletteTemporaryDeref. Do not create a token, reread getter or invent
private word0 at this point.

Unknown native count reports PaletteRefUnavailable after that committed store.
Even an explicitly known primitive state must remain TemporaryDerefUnrecovered:
this checkpoint implements no decrement branch. Runtime/state loss retains source
pointer/getter/retain receipts while guarding continuation; repeated resume cannot
replay allocation/getter/ref/store. Neither175 nor760 is admitted.

## Approved exclusive source set

Existing files under source/apps/moproj2mst:

1. include/m2m/engine/view_manager.hpp: state forward declaration, one data-owned
   reference state/const diagnostic, partial-slot type and WidgetState storage/
   const diagnostic. No arbitrary state factory or numeric admission.
2. src/engine/view_manager.cpp: validated getter state/token retention and runtime
   callability invalidation; host partial-slot metadata cleanup only.
3. include/m2m/engine/application_palette_owner.hpp: private shared token plus
   const accessor/friends; existing standalone algorithm untouched.
4. include/m2m/engine/splitview_placeholder.hpp: new pointer-store cursor/effect/
   issues and any const diagnostics; no public constructor entry.
5. src/engine/splitview_placeholder.cpp: same-value token/state validation, single
   canonical partial pointer store and pre-deref guard.
6. tests/mdt/test_splitview_placeholder.cpp: update getter-success expectations
   and add actual data/state/slot/retry/loss/host-copy tests.
7. tests/mdt/test_widget_application_palette_binding.cpp: validate one state per
   actual data and a distinct once-retained token per getter result.

Do not edit palette_native_ref primitive, application_palette_owner.cpp, existing
standalone helper tests, CMake, session/editor/public paths or earlier SplitView
gates. Root owns builds/grouped/full validation/archive. The seven-file set was
approved after the read-only preparation; no competing source lane may use it.

## Focused acceptance checks

- Unknown/null app slot produces no native ref state or token/provider/cache.
- KnownPresent actual data gets one shared state; each actual getter has a unique
  Held token/receipt, currentBits unknown and exact sequence/ref/balance increment.
  Two frames share state while numeric aliases in another runtime stay distinct.
- Host copies of read diagnostics share exact token and do not add ref/sequence.
  Observer/binding alone does not execute getter/ref/store.
- Private slot has SAME data, unknown word, palette() absent, token still Held;
  one pointer-store receipt survives retries and late guards with no reread/ref.
- Missing/wrong origin/token rejects before pointer store. Expired runtime/data
  native grant preserves history while preventing continuation.
- Runtime loss with escaped strong metadata marks ref state unavailable without
  deref or decrement. Host call abandonment never fabricates native temp unwind.
- Widget global current/max174 and separate QObject98 stay unchanged; no font,
  thread, widget classes/registry, addWidget/next placeholder or public gate effect.
- Explicit known primitive count, if used in an isolated branch test, STILL stops
  before deref/word0/ref and is never inferred from runtime defaults/use_count.

CP80 isolated15-case primitive and independent review established exact raw32
atomics, token once and destruction guard. This integration uses that unchanged
primitive; it does not complete its current-count evidence or any private copy.
Next source step remains qualified count admission and retained temporary deref,
with zero data destruction guarded and nonzero word0/destination ref kept ordered.
