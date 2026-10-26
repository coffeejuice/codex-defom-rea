# CP81 actual palette ref owner and partial private pointer store

Source is frozen/released for root integration. One native-reference state is now
attached to each actually used runtime-interned palette data owner. A successful
KnownPresent getter retains one source token and carries it in its SAME returned
result. The actual placeholder stores that data pointer once in its canonical
partial private slot, leaves its word UNKNOWN, then stops before temporary deref.
No native numeric count, private word0/destination ref, font, QWidget counter or
public gate was added.

Root approved this seven-file set after CP80 archive. Only the seven approved
source/test files and CP81 plan/implementation reports were written. CP80 primitive,
application_palette_owner.cpp, standalone helper tests, CMake, session/editor/
public callers and other lanes were not edited. No build/compiler/tests/CTest,
original execution, GUI or mutable REA. Token usage is unavailable.

## Same runtime/data reference owner

WidgetPaletteDataState owns one private shared PaletteNativeRefState, exposed by
const nativeReferenceState(). The validated runtime KnownPresent getter lazily
creates it with stable runtime/data-alias qualification. It uses the SAME mutable
registered data object already matched to app_pal's wrapper raw/control block;
it does not create another palette data graph or accept an external ref state.
Existing state must retain its exact canonical control block, qualification,
native availability and no destruction dependency. Invalid state is not replaced.

The runtime prepares one GetterTemporary token and performs its retain at the
source6504a8c6 boundary before the known-present output copy returns. This records
one mutation/ref/value balance with UNKNOWN numeric bits. It never calls
observeCurrent or seeds1/another number. A second actual getter uses SAME state
but a separate token. Unknown and KnownNull reads keep their previous no-provider
pending behavior and create neither reference state nor token.

QApplicationPaletteRead holds this mutable token privately, with runtime/actual
placeholder friendship and const nativeReference() diagnostic. Host report copies
share the actual token/control block rather than another native ref. Standalone
CP77 reads remain tokenless and cannot satisfy the actual pointer-store origin
checks. No standalone owner-helper algorithm was changed.

Runtime destructor invalidates all attached ref-state callability, without deref,
count reset or native data free. Strong data/state/token diagnostics do not keep
the runtime alive. Numeric state and retain receipts remain historical, including
unclosed native references after host abandonment; C++ token destruction is not
Qt unwind. Allocation/unwind failure behavior remains outside this selected
normal allocation/getter profile, as in CP75/77/79; no rollback redesign is added.

## Canonical private source slot

WidgetPaletteConstructionSlot contains SAME shared palette data and optional
uint32 groupResolveWord. WidgetState owns an optional of that slot and exposes
it only by const diagnostic. Its presence records6504abcb's data-pointer store;
its word remains nullopt. palette() remains absent. This is one constructor slot
under construction with shared backing data, not a completed QPalette or second
mutable data graph.

After CP79 getter completion, the actual placeholder validates:

- Same live widget/value/runtime and bound canonical app_pal owner as before.
- Returned data equals app_pal wrapper's data/control block.
- GetterTemporary token is Held with one retain receipt, and its state/control
  block equals that exact data's nativeReferenceState; native origin is live and
  not destruction-pending.
- No existing completed/partial private palette is replaced.

It then stores SAME data with unknown word once, adds PrivatePaletteDataStored
and advances from PrivatePaletteCopy to PaletteTemporaryDeref. Unknown numeric
bits report PaletteRefUnavailable. Even a deliberately supplied isolated primitive
snapshot reports PaletteTemporaryDerefUnrecovered: this source cut performs no
dec, private word0, destination ref, destruction, free or later font mutation.
The state observer itself remains the isolated CP80 primitive, not a new runtime
current-count producer/admission API.

Repeated resumes revalidate source ownership and skip allocation/getter/ref/store.
Runtime/native-ref-origin loss keeps the committed pointer/getter/retain receipts
while guarding later progress. Host WidgetState invalidation drops only its
partial-slot metadata, like its existing completed palette metadata; no source
pointer rollback, native Qt destructor/ref decrement or counter effect is claimed.
Earlier SplitView/QFrame/QSplitter/derived entry and public editor gates remain
closed; no new production caller was added.

## Focused test updates

m2m.mdt.splitview_placeholder now has25 cases (277 CHECK expressions,41 REQUIRE):
its21 prior cases remain, with successful getter cases migrated to the more
precise PaletteTemporaryDeref/PaletteRefUnavailable boundary. The four added
cases verify partial source pointer/unknown word/held getter token; copied read
diagnostics and retry without another ref; known isolated snapshot still stopping
before decrement/word0; native-ref origin loss retaining pointer/history; and
host abandonment without native reference rollback. (The first case includes
copy/retry checks, so these are four case functions.) Known widget current/max174
and separate QObject98 checks remain unchanged in the existing cases.

m2m.mdt.widget_application_palette_binding now has19 cases (122 CHECK expressions,
19 REQUIRE): four new cases verify one state per actual data and a unique token
per getter, host copy without ref; equal local aliases in separate runtimes
remaining distinct; runtime loss invalidating callability without deref; and
withdrawn native data-ref origin refusing another getter without replacement.
Existing unknown/null tests also require no state/token creation.

The test source uses explicit isolated profiles and actual existing owner
fixtures. Unknown physical counts are never inferred from host ref counts or
observed QWidget totals. A direct primitive snapshot is labelled isolated and
STILL cannot advance the integrated deref. No original current-ref observation,
valid-UI count or full private-copy result is asserted.

Static preflight passed: quoted includes, forward/complete types and header-cycle
review, balanced delimiters after comment/literal stripping, supported Testkit
macro arity, unique names, single main per target, no C assert/REQUIRE_EQ,
trailing whitespace or conflict markers. QApplicationPaletteRead has no aggregate
initializers elsewhere requiring adaptation to its private token field. No build
or test execution is claimed. Root owns grouped focused25/19 targets, the unchanged
15-case palette_native_ref target as needed, then complete validation/archive.

## Approved file boundary and frozen hashes

Paths relative to source/apps/moproj2mst/:

| File | SHA-256 |
|---|---|
| include/m2m/engine/view_manager.hpp | 0715b73412eaac42084d3830de6a667d4804777bc76c040137053ca9a4078eff |
| src/engine/view_manager.cpp | 4f11d60d40ccef3730f4cd7575af53ebf60af1af8581b76fb7d732e64c69e805 |
| include/m2m/engine/application_palette_owner.hpp | a28bfa6b3fa141d7e063350c2b5f9c8f71ca74cc68f04d2a62540391c2dfd78b |
| include/m2m/engine/splitview_placeholder.hpp | bed773b7c03166e16d9295857459b71ad7a6b86c71a6f3e9767360264c1741b8 |
| src/engine/splitview_placeholder.cpp | 560c4fbd58c6d5fe15447d18929b0e276121cf0e19bf56c3bb2dc9f627038128 |
| tests/mdt/test_splitview_placeholder.cpp | d7b4e2f05bb9af07cd3cd9b61a7058039f7beb6464a219a74b38098241b6665d |
| tests/mdt/test_widget_application_palette_binding.cpp | 0e560b1961d8a537ab7d7d152c771199082fbaa554cae30433fa8ad9455dd33e |

Plan: analysis/resume81-palette-integration-plan.md SHA
08751465b32c901a3f1b717d277603ad6d8c2da9c05bd9db143ace6bc996d24d.
Native source identities remain QtGui
c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887 and
QtCore bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160.
CP78/80 reports retain exact byte intervals and atomic/source ordering. No new
native runtime value was observed.

Next dependency: actual current ref-count admission at the right mutation
sequence, then once-only temporary deref with zero destruction still guarded and
nonzero word0/destination ref kept ordered. Font/locale/thread/widget-counter
history and public Open-Save-Close completion are not advanced by this increment.
