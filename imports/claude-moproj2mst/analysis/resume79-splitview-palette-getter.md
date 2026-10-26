# CP79 smallest getter continuation for the same placeholder

The next isolated cut can retain ONE successful QApplication palette getter
output for the actual CP75 placeholder, then stop BEFORE QPalette6504abcb stores
the private data pointer. It needs a runtime-mediated read of CP78's registered
app_pal owner, not an externally supplied helper or a startup default fallback.
No destination palette, font, thread, registry, counter or public SplitView gate
should advance in this increment.

This report is the only write. No source/build/tests, GUI, original execution,
mutable REA or new raw capture. CP78 source remains frozen for review/archive;
no source editing starts before root releases the exclusive set below. Token
usage is unavailable.

## Existing source-qualified boundary

CP75 leaves SAME placeholder value/WidgetState at PrivatePalette, native thread
null and no palette/font/locale. CP78 registers one app_pal helper in the SAME
WidgetRuntime Values from exact module/RVA9c9690 and registered shared palette
identity. Its public diagnostic accessor is const, so a consumer cannot directly
invoke the nonconst read(). That is useful: actual execution should go through
the runtime that owns the canonical slot rather than any escaped diagnostic lease.

Native6505e8d4 calls default QPalette6504abb0. That constructor first invokes
QApplication getter65016f70 into its own local temporary. Getter copy6504a8b0
preserves app_pal's data/full word. Only after getter return does6504abcb write
the private destination's data pointer. Temporary deref/destruction, private word0
and destination ref follow. CP78's composition report pins these exact ranges.
This proposed cut ends before that pointer write, so no partial WidgetState
palette representation or Qt temporary destruction claim is necessary yet.

## Runtime-mediated no-provider read

Proposed source signature:

`QApplicationPaletteRead WidgetRuntimeState::readApplicationPaletteFor(const shared_ptr<WidgetState>& receiver, const shared_ptr<const QApplicationPaletteOwnerState>& expectedOwner)`

The method must validate, BEFORE helper.read():

- Runtime has its canonical live shared owner, existing Values and registered
  applicationPaletteOwner. No default creation or module observation occurs.
- Receiver is nonnull/live, matches its actual component control block and has
  this exact runtime/control-block binding. A rewrapped equal raw receiver,
  foreign runtime or invalidated native owner is rejected without a read.
- expectedOwner is nonnull and matches the registered owner raw address AND
  shared control block. A copied standalone helper, equal qualification string,
  another runtime's helper or rewrapped owner cannot substitute it.
- For KnownPresent, the retained wrapper/data must still be the exact data owner
  registered under its alias in THIS Values registry. No class/default palette
  value is substituted for the slot. The CP78 module profile remains canonical.

Invalid ownership is M2M_E_UNSUPPORTED, consistent with other bound runtime
seams. The actual placeholder already revalidates its value/runtime at entry;
direct runtime tests should exercise these checks independently.

Only after this preflight call the existing canonical helper's read() with NO
constructor provider. Therefore:

| Source slot | Result / effects |
|---|---|
|Unknown|SlotStateUnknown, no output/effects/provider invocation|
|KnownNull|InitializerUnavailable, no allocation/constructor/cache/output|
|KnownPresent|One completed output sharing SAME data and preserving wrapper word; OutputCopied only|

No provider parameter should be exposed in this increment. QPalette(GlobalColor2)
constructor remains a separate dependency, and copying runtime.applicationPalette()
is not its implementation. The read method creates no private-input observation,
no new owner and no count/lifetime transition outside the returned getter value.

Header view_manager.hpp needs only `struct QApplicationPaletteRead;` forward
plus the method declaration. Its return-by-value incomplete declaration is valid;
view_manager.cpp already includes application_palette_owner.hpp. No circular
header inclusion or CP77 helper change is required.

## One retained getter result in the placeholder

Suggested added stages: ApplicationPaletteGetter and PrivatePaletteCopy. Keep
NotStarted/PrivatePalette and the existing value/private-prefix ownership.

1. At PrivatePalette, resolve runtime.applicationPaletteOwner(). If absent, keep
   the current no-owner dependency. Existing explicit per-widget inputs alone
   may still report their legacy PaletteCopyUnrecovered guard; they must not
   become a getter output or authorize native progress. This preserves existing
   isolated prefix tests without turning their observations into execution.
2. When a registered owner is available, bind a weak const owner identity once
   and enter ApplicationPaletteGetter. It must be the same runtime's owner; no
   standalone parameter or generic ready boolean. If existing private inputs
   have a conflicting source qualification, preserve that provenance guard.
   A completed private defaultPalette is NOT required: this is its producer.
3. On this stage, resolve the SAME weak owner and compare against current runtime
   owner, then call the runtime-mediated read. Pending slot issue is retained in
   a diagnostic read report and mapped to ApplicationPaletteGetterPending. Late
   Unknown->KnownPresent observation can resume the same owner/cursor; observation
   alone never calls read or advances the helper.
4. On successful output, store the actual QApplicationPaletteRead result ONCE in
   a stable call member (for example optional<QApplicationPaletteRead>). Preserve
   its one OutputCopied receipt; add one ApplicationPaletteGetterReturned caller
   receipt, not a second native copy. Enter PrivatePaletteCopy and report
   PaletteCopyUnrecovered, ending BEFORE6504abcb's private pointer store.
5. Every later resume revalidates the same value/runtime/owner, but must NOT call
   read again in PrivatePaletteCopy. Its returned optional value stays present
   and shares source data; QWidget private palette/font/locale stay absent.

The result member's address is a stable host model of this call's temporary,
not an invented original pointer/observation alias. Call/value are nonmovable;
no second temporary is constructed on retry. The existing helper returns a
value/copy report, not an outer private-copy completion receipt.

A getter diagnostic can expose the retained result by const pointer/reference,
plus its bound-owner weak diagnostic if useful. Do not provide takeOutput or a
mutable getter/provider API. Distinguish pending last-read issue from completed
retained output so a retry cannot overwrite successful data with a pending read.
Use explicit stage/success checks rather than optional<bool> truthiness.

## Lease and abandonment requirements

The runtime owns app_pal strongly; the placeholder stores it weakly. The completed
getter result holds its own shared data lease, separate from app_pal wrapper
lifetime and from the placeholder value's native owner. Never keep the runtime
alive just to avoid a guard. An escaped diagnostic app_pal owner may outlive the
runtime as historical data, but that does not authorize another native read.

If runtime expires before/during a pending or completed getter stage, preserve
all historical prefix/output receipts and report RuntimeUnavailable; do not
reconstruct an owner, reread, clear/release the modeled temporary as if native
QPalette destruction returned, or assign the private palette. An equal raw
foreign control block must not be retained for later dereference; registry/weak
owner identity prevents that substitution.

On host abandonment, ordinary C++ result leases can be dropped and the actual
placeholder WidgetState is invalidated by its existing value owner. This is not
native temporary deref/destructor, QWidget destructor, count decrement or queue
cleanup. The unresolved native release belongs to the later private-copy cut.
Two placeholders sharing one app_pal need independent getter outputs/cursors;
finishing or abandoning one must not reset/replay the other's read.

## Exact exclusive write set for a separately approved implementation

All existing files under source/apps/moproj2mst/:

1. include/m2m/engine/view_manager.hpp: forward result declaration and runtime read
   signature only; no new friend/public factory/default fallback.
2. src/engine/view_manager.cpp: validated no-provider read against canonical Values
   owner and same-runtime data registry; no observer behavior changes.
3. include/m2m/engine/splitview_placeholder.hpp: new stages/issues/caller receipt,
   weak bound owner and retained read-result member/const diagnostics. Include
   application_palette_owner.hpp (and optional explicitly) for complete result.
4. src/engine/splitview_placeholder.cpp: one-owner/one-read retained continuation
   ending before private data store; earlier prefix/legacy guard unchanged.
5. tests/mdt/test_splitview_placeholder.cpp: extend actual owner/lease/no-private-
   copy/count-preservation cases while retaining original prefix behavior.
6. tests/mdt/test_widget_application_palette_binding.cpp: direct read ownership
   rejection and no-provider slot semantics under the registered runtime seam.

No application_palette_owner.hpp/.cpp changes are required for this cut, no new
CMake target, no ConversionSession/editor/public caller, no count/registry/private
copy implementation and no application palette default/font producer mutation.
Root must reserve all six files exclusively after CP78 archive; the runtime and
placeholder halves should freeze together for grouped focused build/tests.

## Meaningful focused checks

Runtime tests: missing registered owner, foreign/rewrapped expected owner, foreign/
rewrapped/dead receiver and wrong runtime reject before read. Unknown returns no
output/effects; KnownNull remains null without provider/caching. KnownPresent
returns exact data/control block/full word and only OutputCopied, without changing
canonical wrapper. No receiver private record is required or created.

Placeholder tests: prefix alone/global default alone never reads; module observation
alone does not resume; Unknown then late present uses SAME bound owner and one
completed read. KnownNull remains pending at initializer dependency. KnownPresent
with a deliberately nonzero word retains that exact temporary word while widget
palette stays absent; repeat resume has identical output/receipts. Two actual
placeholder values share owner but have different result storage and independent
abandonment. Runtime loss after output retains diagnostic data and receipt while
guarding continuation. Incorrect private-call qualification is not bypassed.
Known historical current/max174 and QObject baseline remain unchanged, as do
thread-null/registry/private class and earlier SplitView/public gates.

Fixtures may use actual existing startup owners or register palette data through
an observation that does not supply defaultPalette; they must not need a fake
editor or pre-completed placeholder palette to break a circular dependency.
Release-active Testkit checks/main/includes and static preflight remain mandatory;
root owns compiler/focused/grouped/full validation. No tests are authored/run by
this design lane.

## Source/evidence snapshot

The CP78 composition report retains exact native sequences and bounds. Original
QtGui SHA-256 remains
c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887.
Relevant native intervals:65016f70 getter;6504a8b0..6504a8fd copy;
6504abb0..6504ac26 default constructor. No new native observation was acquired.

| Read snapshot | SHA-256 |
|---|---|
| source/apps/moproj2mst/include/m2m/engine/view_manager.hpp | e69f43da56b543c5fa5ca91f6660d43361d3724a4eb0b58b3e5b0541594e3615 |
| source/apps/moproj2mst/src/engine/view_manager.cpp | 0ab08f3cb4ac4a8dc24a9469153cdf518c5f8e8654072dc4080641b464e2ffb3 |
| source/apps/moproj2mst/include/m2m/engine/splitview_placeholder.hpp | 71dae7b930cb7b9e4f79379dfbe79b89a73b292be4f794954f09e10831ffb0f9 |
| source/apps/moproj2mst/src/engine/splitview_placeholder.cpp | 80427dacd058628cfd6baf0b699ee4cd43b861f1c041abf99e1f20e8a1ae0e04 |
| source/apps/moproj2mst/include/m2m/engine/application_palette_owner.hpp | ac6badf7155d8c1c7b1cfd816719e3685d5b3e65dbb2731f51f41a346b858e58 |
| source/apps/moproj2mst/src/engine/application_palette_owner.cpp | 58d6355b88150698b0af87128e8994f3595fa2c4d0a8db995aec1875d7851348 |

No source changed. Public/private-copy/counter-history completion remains unclaimed.
