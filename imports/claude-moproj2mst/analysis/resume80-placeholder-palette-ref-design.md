# Private QPalette copy: shared native reference state and exact guard

The smallest faithful private-copy continuation needs ONE native-reference state
on the SAME runtime-interned WidgetPaletteDataState, one native-value lease for
the retained getter temporary, and a partial destination slot on the SAME widget.
Store the private data pointer first. If its current native ref word is unknown,
guard before deref. Otherwise decrement once; zero must stop before data/brush
destruction, while nonzero can write private word0 and retain the destination.
Do not infer a ref count from shared_ptr leases, app_pal presence or QWidget counts.

This report is the only write. No source/build/tests, original execution, GUI or
mutable REA. A bounded QtCore export/disassembly check was inspected without new
raw files. CP79 source remains frozen during root validation/repair. Token usage
is unavailable.

## Exact native arithmetic and order

CP78 pinned full default constructor6504abb0..6504ac26. Runtime getter result is
already retained by CP79; the next operations are:

| Address | Native effect |
|---|---|
|6504abcb|Store getter's SAME palette data pointer to destination private+170; no destination Qt ref yet|
|6504abce|QBasicAtomicInt::deref on SAME temporary data's first dword|
|6504abd4..abd6|Test AL; nonzero skips destruction|
|6504abd8..ac08|On zero, destroy60 (0x3c) QBrush elements starting data+8, size8, through6511cbc0; free data through655e10ae|
|6504ac10|After that branch returns, write ENTIRE destination word+8=0|
|6504ac17|QBasicAtomicInt::ref on destination data; returned bool is ignored|
|6504ac25|QPalette default ctor returns; QWidgetPrivate's QFont call6505e8de follows|

Pinned QtCore exports resolve ref to67001aa0 and deref to67001ac0. Their complete
8-byte leaves are LOCK ADD dword,+1/-1; SETNE AL; RET. There is NO shared-null
sentinel check, positivity test, saturation or error branch. Use raw uint32 modular
arithmetic. Deref returns false exactly when the UPDATED bits are0. A current
word1 therefore selects destruction; word0 decrements toffffffff and skips it.
Destination ref may wrapffffffff to0; the caller ignores its false return, so a
new ref-zero refusal would redesign the source. Raw unusual values are arithmetic
branch tests, not observations of a valid original UI state.

## Minimal shared ledger, independent of host ownership

Proposed name: PaletteNativeRefState, attached once to the actual runtime-interned
WidgetPaletteDataState, keyed by its pointer/control block, not another alias-only
or editor-local counter. The native atomic is in palette DATA, not app_pal wrapper,
QObject, QWidget counts, QImage refs or QPaintDevice cleanup globals.

Minimum state:

- optional<uint32_t> currentBits: unknown stays unknown until qualified at the
  actual current source point; no initial1 or count from use_count().
- monotonic modeled mutation sequence identifying native ref operations already
  represented on this data; it is provenance, not the native atomic word.
- native data lifetime/destruction-pending status, separate from diagnostic C++
  metadata lifetime.
- explicit source-value lease/token for each modeled getter temporary/destination,
  identifying its SAME data and whether its native ref was acquired/released.
  Host copies of a diagnostic report share that token; they are not new Qt values.

A qualified current-count observation must target this exact data/runtime and the
CURRENT mutation sequence. If unknown, it binds the current word at that point;
if known, only equal bits are accepted. Stale sequence, foreign data/module/owner,
conflicting known count and released native source reject without mutation. Such
an input is still unavailable in original captures here; public progress remains
guarded. Do not relabel startup widget174 or a palette value alias as this count.

Sequence qualification matters when admission is late. The actual getter's copy
constructor6504a8c6 already refs data. Any new integrated getter must acquire its
native temporary token/ref exactly once at that source copy point. If currentBits
is known, add1; if unknown, leave numeric state unknown but record that ref event
and sequence. A later observed current word ALREADY includes those getter refs:
never replay them after anchoring. A new process/test builds the revised flow;
there is no license to attach a fictional ref to an already-running old frame.

The runtime-mediated getter can validate/prepare the token before its successful
KnownPresent read and return it alongside the existing output receipt. Standalone
CP77 reads may retain their isolated mode, but a private-copy consumer must require
an actual runtime-bound token. Do not implement this by changing shared_ptr copy/
destructor behavior or giving every WidgetPaletteValue copy an automatic native
ref. App-pal observation records an existing wrapper, not a new native ref.

Current numeric state reflects a bounded qualified segment. Unrecovered external
palette mutation/lifetime history cannot be repaired by count replacement. The
short known-present/private nonzero branch has no arbitrary callback between
its count operations; the zero destruction path does, and remains guarded.

## Partial private slot is required before the ref guard

WidgetState currently exposes optional<WidgetPaletteValue>, whose word is always
a known uint32. It cannot truthfully represent6504abcb's pointer store while the
private word remains unassigned. Setting{data,0} early would move6504ac10 across
temporary destruction and erase a meaningful pending effect.

Add a narrowly scoped constructor slot on the SAME WidgetState (conceptual name
WidgetPaletteConstructionSlot), with actual data identity and optional word,
plus destination-native-ref state. At the pointer-store cursor, its data becomes
the retained getter data; word remains unknown and no destination native lease is
acquired. This is one source slot under construction, not a second palette graph.
Keep palette() absent until fully constructed; provide a const partial-slot
diagnostic rather than inventing a completed palette.

When complete, promote/move that slot into the widget's completed palette view,
with SAME data/word0 and its actual destination lease; no duplicate retain, data
clone or observer overwrite. The completed view and construction receipt must
not become independent mutable authorities. A host shared metadata lease needed
to keep this partial pointer inspectable is not another native Qt ref.

## Retained private-copy cursor and guards

Suggested stages/effects, in native order:

1. Validate actual placeholder/runtime, its bound registered app_pal origin,
   completed getter output/token and SAME registered palette data/ref state.
   Missing source/native-token qualification guards before the pointer store.
2. PrivateDataStored: commit6504abcb once on the canonical partial widget slot.
   Missing currentBits now reports PaletteRefUnavailable WITHOUT replaying the
   getter or clearing the pointer. No temp deref has executed yet.
3. TemporaryDeref: require the temporary's native token is still held; update
   currentBits=(bits-1)mod2^32 and sequence once, record returned nonzero bool,
   consume ONLY this temporary lease and advance the cursor immediately.
4. If result0, set DataDestructionPending/PaletteDataDestructionUnrecovered.
   Count0 and the stored pointer remain committed; private word is unknown,
   destination ref absent. Repeated resume must not decrement again, free data,
   normalize word0 or claim constructor return. C++ data metadata can remain
   inspectable while native destruction is pending. Do not mark free completed
   merely because the atomic result was0.
5. For nonzero, skip destructor/free. Write private word0 once, then destination
   ref once, recording native return only as diagnostic (never a new guard).
   Acquire this private value's own native token. Normal completion exposes
   WidgetPaletteValue{sameData,0}, records PaletteConstructed and enters
   PrivateFont, whose actual application-font producer remains unqualified.

The zero branch requires all60 actual QBrush owners and their destruction/native
free effects. The current sparse palette slot model does not supply that complete
payload or its nested brush-data lifetimes. Its destruction MUST remain the next
unsupported guard; no erase/free/rollback shortcut is proposed.

Native-data destruction-pending state must be visible on the shared data owner
so another retained frame cannot treat the same zero-count object as an ordinary
live source while its prior destructor branch is unclosed. That is a model
continuation dependency, not an invented native error return. Any future actual
destruction implementation controls the later native-lifetime transition.

Runtime/module loss always preserves historical getter/pointer/atomic receipts
and guards later continuation. Abandoning the host call must not synthesize
native temporary/private QPalette destruction or ref rollback: native unwind is
outside this cut. Diagnostic leases can be dropped without emitting Qt effects.
Unclosed source references/lifetimes remain explicit, never silently subtracted
from another frame's numeric state. Metadata lifetime and native callability
remain separate.

## Smallest implementation split / ownership coordination

A coherent first source lane could add NEW palette_native_ref.hpp/.cpp and
TEST test_palette_native_ref.cpp for raw32 state/sequence/value-token primitives,
with actual data-origin binding required at integration. It should not enable
private-copy execution by itself. Root reserves/wires new files and tests.

Integration then needs an exclusive coordinated set, because data/read/copy are
shared: view_manager.hpp/.cpp (one ref state per palette data + qualified current
observation + getter token/ref); application_palette_owner.hpp only if its returned
read adds a shared token field; splitview_placeholder.hpp/.cpp and focused tests
for actual partial slot/cursor. CP77 helper algorithm need not gain editor-local
state. A future source owner must announce exact files after root archive; no
shared/source edit is authorized by this report.

If that set is too large for one checkpoint, the smallest integrated increment
is qualified native-ref owner/token plus actual private pointer store and
PaletteRefUnavailable guard, with no decrement or private word yet. The next
bounded increment can implement only the known-count nonzero branch while
retaining the zero destructor guard. Neither split may seed numeric ref values,
open earlier SplitView gates, fake a font or reach175/760.

## Focused test contract

- Unknown numeric refs: runtime getter records ONE native ref/token with unknown
  count; private pointer stored once with unknown word, then RefUnavailable.
  Observation alone does not advance. Current-sequence count3 resumes deref3->2,
  word0, ref2->3, then PrivateFont guard; no getter/ref replay on retry.
- Known fast path: starting total2 BEFORE getter produces3, private pointer store,
  deref3->2, word0, ref2->3. Source slot/wrapper words stay nonzero; completed
  private word is0. QWidget current/max174 and QObject baseline remain unchanged.
- Two getter frames on SAME data: total2->3->4; each converts its temporary ref to
  its own private ref once, with shared numeric state and independent cursors.
  Numerically equal aliases in another runtime cannot supply the ref state/token.
- Raw zero-destruction branch: current1 immediately before temporary deref becomes0
  once, then destruction guard. Pointer remains committed, word unknown/no private
  ref/font/free; retry has no second decrement. Label this explicit raw branch,
  not evidence that original valid UI produced that combination.
- Raw wrap: current0 before deref becomesffffffff (nonzero), private word0, then
  ref becomes0; ignored ref return does not create a new refusal. Signed overflow
  is avoided by uint32 arithmetic; no sentinel/positive-count special casing.
- Stale mutation sequence and conflicting/foreign source observations reject;
  known current count is never reset to reconcile an omitted native history.
  Host copies of getter diagnostics create no native ref or separate temp token.
- Runtime loss after pointer/decrement stages retains exact committed receipt;
  expired origin cannot resume from a strong metadata lease. Host abandonment
  before/after temp release emits no guessed native unwind/destination release.

Tests must be Release-active Testkit with resolved includes/main/macros, static
preflight and root-owned focused/grouped validation. None authored/executed here.

## Evidence identity and byte bounds

Original QtGui SHA-256:
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Original QtCore SHA-256:
`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
DUMPBIN14.44.35228.0 windows were inspected offline. Addresses use preferred
65000000/67000000 images; only exact leaves/ranges below are claimed.

| Native interval, end exclusive | SHA-256 |
|---|---|
|67001aa0..67001aa8 (ref)|7b57a50acb800ca3e7d326b624b14349c569ea06f18e594273c33a92fe1256de|
|67001ac0..67001ac8 (deref)|06457cdd529a46d5cd423e2ccd7a761b5abfd4d97a001e30895a55157b8065b6|
|6504abb0..6504ac26 (default ctor, CP78)|704806e2a08c3327f0cb7f8d0ca7c496af0c69dbd897d456e48713cf1fa6a399|

Read snapshot (no edits):

| Artifact | SHA-256 |
|---|---|
| source/apps/moproj2mst/include/m2m/engine/view_manager.hpp | b7ec2ca09e342a2fd5760b7f97c236fc23b159d54a1a9094dadb614d4793aba5 |
| source/apps/moproj2mst/src/engine/view_manager.cpp | 958324aa0d33696057bec472b72bb149be20c28dd155f1eead54d8539e1f8718 |
| source/apps/moproj2mst/include/m2m/engine/splitview_placeholder.hpp | d9d6a335d5d8ac7be4ddff5ec86504ed72fc7c6cac72b9e9b66accf9ce90354b |
| source/apps/moproj2mst/src/engine/splitview_placeholder.cpp | 9db88e6ca7144796e88b91192af3ec5221d3d16ac7a35314cf81401371e1668d |
| analysis/resume78-splitview-palette-compose.md | 0b772aa972d720780ad9f78529ab94bbfa9230adf9a2b71fdb2f62e0d58d4893 |

No source or runtime validation result, private constructor completion, public
SplitView entry or Open-Save-Close completion is claimed.
