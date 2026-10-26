# CP80 isolated native palette reference primitive

Source is frozen for root integration and independent review. New
PaletteNativeRefState and PaletteNativeValueRef implement raw32 Qt LOCK ADD
semantics with once-only native value tokens. Unknown deref stays held; a committed
zero deref releases that token once and retains a shared destruction-pending
guard. Native ref returning false on wrap is diagnostic only.

Only three NEW helper/test files and this receipt were written. Existing
view_manager, splitview_placeholder, application_palette_owner, editor/session/
public source and CMake were not edited. Source/test globs already register the
new files. No build/compiler/tests/CTest, original execution, GUI or mutable REA.
Token usage is unavailable. Root explicitly assigned the isolated source cut;
distortion_rea owns the independent read-only review.

## API and semantics

Header m2m/engine/palette_native_ref.hpp, namespace m2m::engine:

- PaletteNativeRefState(qualification) is noncopyable and shared-owned. Initial
  currentBits is UNKNOWN; mutationSequence/refMutations/derefMutations and
  nativeValueBalance start0. No native counter value is inferred.
- observeCurrent(bits,currentSequence,qualification) accepts only exact nonempty
  qualification/current sequence and unknown-or-equal numeric bits. It changes
  no operation/sequence/value balance. Known count cannot be reset; stale
  snapshots cannot replay getter refs. Equal zero observation does not clear
  an existing destruction dependency.
- PaletteNativeValueRef(sharedState,GetterTemporary|PrivateDestination) is one
  noncopyable source value token. The token can be shared by host diagnostics;
  those host copies do not acquire a second native reference. Construction and
  inspection perform no native operation.
- retain() mutates only on AwaitingRetain. It increments raw known bits modulo
  2^32, or retains unknown numeric state while recording its one native ref/
  mutation sequence. Stage becomes Held even if ref's return is false/unknown.
- release() requires Held and a KNOWN current word; unknown returns
  RefCountUnavailable without consuming token or sequence. Known bits decrement
  modulo2^32; a release receipt and Released stage commit immediately, exactly
  once. False deref sets shared DataDestructionPending and performs no deletion.
- Separate retain/release receipts preserve sequence, optional before/after bits
  and optional native nonzero result. Retain result is diagnostic, never an
  invented branch. Release retry cannot repeat arithmetic. A released value
  cannot be retained again as if it were a new source object.

nativeValueBalance counts explicit modeled native retain/release operations,
not physical atomic bits or C++ use_count. Host token teardown never decrements
it or the raw counter. Original unwind/destruction remains unclosed; there is
no implicit native ref rollback. CurrentBits may include other unmodeled native
owners, so balance is not used to infer/admit numeric values or positivity.

Shared destructionPending blocks later retain/release activity until its missing
native destruction is recovered; no method clears the guard. No QBrush destroy,
free, private pointer/word write or completed QPalette operation exists here.
releaseNativeOwner invalidates callability while preserving historical count/
receipts; it is not a native data-free implementation.

## Owner lifetime and scope

Token constructor validates exact raw/control-block equality against the state's
weak_from_this owner before storing a weak reference. A rejected rewrapped block
is never dereferenced after canonical object release. Null/unqualified/expired or
invalidated state guards before operations, retaining prior receipts and token
stage. The token does not keep its state alive merely to avoid a lifetime guard.

This is an ISOLATED per-data reference primitive. It has no WidgetPaletteDataState
or runtime registry binding and therefore does not claim that an arbitrary state
is the current native atomic for an actual application palette. That exact
same-data/module/runtime association is the required next seam. Value kind names
identify source-edge intent; they do not certify that a private pointer/word was
stored or that earlier SplitView construction returned. No public gate/count
history can be advanced from this helper alone.

## Native evidence

Exact exports in pinned QtCore resolve:

- ref67001aa0: LOCK ADD dword,+1; SETNE AL; RET.
- deref67001ac0: LOCK ADD dword,-1; SETNE AL; RET.

No negative-count sentinel, saturation or positive-only guard occurs in those
leaves. Native QPalette6504abce consumes deref's result; its zero path destroys
60 QBrushes/frees data and stays unsupported here. Native6504a8c6/6504ac17 ignore
ref's result, including false after wrappingffffffff->0. The explicit raw-state
wrap/zero tests do not claim that valid original UI produced unusual counts.

Original QtCore SHA-256:
bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160.
Exact 8-byte leaf hashes:
ref7b57a50acb800ca3e7d326b624b14349c569ea06f18e594273c33a92fe1256de;
deref06457cdd529a46d5cd423e2ccd7a761b5abfd4d97a001e30895a55157b8065b6.
CP80 design and CP78 report retain the surrounding private-store/deref/word0/ref
bounds and qualifications. No new original runtime value was acquired.

## Focused tests and static preflight

New test_palette_native_ref.cpp has15 TEST_CASEs,196 CHECK expressions and21
REQUIRE expressions, one testkit::run_all main, and Release-active macros.
Cases cover unknown initial state; exact/equal/stale observation; one getter ref
with unknown count; late current-count admission already including that ref;
nonzero temp-to-destination conversion; zero decrement once and sticky global
destruction guard; raw underflow/ignored ref wrap; signed boundary/negative bits;
getter ref wrap; two independent frames sharing one numeric state; late snapshot
including both refs; host copies/destruction with no native mutations; missing/
unqualified/rewrapped state including canonical-owner death; unretained release/
native-owner loss/history; and independent equal-qualified state identities.

Unknown deref tests assert Held with no release receipt. Zero tests assert
Released, count0 and exactly one deref, while destination remains unretained.
Wrap tests assert a destination Held with false retain return and no destruction
guard. Lifetime tests retain actual weak state origin and ensure host operations
never emit a native cleanup effect.

Static checks passed: all quoted includes resolve, balanced delimiters after
comments/literals removed, unique case names, supported macro arity, one main,
no assert/REQUIRE_EQ, no trailing whitespace/conflict markers. CMakeLists lines
90/124 already use CONFIGURE_DEPENDS source/test globs; no registration edit was
needed. Suggested focused target m2m.mdt.palette_native_ref,15 cases. No executable
was built/run; root owns focused/grouped/full validation and archive.

Independent source review was requested from distortion_rea with these frozen
hashes. Its native/design ordering review had already confirmed exact arithmetic,
late sequence qualification and the zero/ref-wrap distinctions. The independent
source review result is separate from this implementation/static receipt.

## Frozen source

Paths relative to source/apps/moproj2mst/:

| File | SHA-256 |
|---|---|
| include/m2m/engine/palette_native_ref.hpp | e4b3ea0aa19fcd9616593eb2d6eece522ff7ebbc9ea8aa020a107e7a602468cd |
| src/engine/palette_native_ref.cpp | 188ef36821488a00a31956b44642a59476ae6a5b68eae1f97e8e1e5098be64c5 |
| tests/mdt/test_palette_native_ref.cpp | 1d85174f6a946c872cd694aa5add6bc2b6ff5f907dfdc5038d298fa4501d23df |

Next dependency: attach exactly one state/token source to the actual runtime-
interned palette data and retained getter result, then represent the canonical
partial private slot. The private pointer store and unknown-ref guard, nonzero
word0/ref completion, zero destruction, font and earlier public SplitView gates
remain unimplemented here. No QWidget counter175/760 or Open-Save-Close completion
is claimed.
