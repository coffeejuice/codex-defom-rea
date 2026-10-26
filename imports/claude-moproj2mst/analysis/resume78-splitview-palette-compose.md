# Compose app_pal with the actual placeholder palette producer

The correct application owner is one future WidgetRuntime-owned
QApplicationPaletteOwnerState for the module slot QtGui+9c9690. The current CP77
instance is standalone; no production runtime owns it yet. Values::defaultPalette
is its possible shared-data backing after a qualified join, not proof that the
native slot is present. Composition must first establish that owner/registry join,
then retain one getter output for the SAME placeholder call. Palette construction
precedes font and every QObject/QWidget counter effect.

This report is the only write. No source/build/tests, original execution, GUI or
mutable REA. Two small direct DUMPBIN windows were inspected without writing new
raw files; pinned native byte hashes are below. Token usage is unavailable.

## Current owner inventory

CP77 QApplicationPaletteOwnerState models the singleton wrapper separately from
shared WidgetPaletteDataState and the class-key palette cache. Its slot is Unknown,
KnownNull or KnownPresent. read() from Unknown has no effects. KnownPresent copies
its wrapper value. KnownNull requires a qualified constructor provider with native
argument2, records wrapper allocation/construction/cache, then copies its result.
It currently has no WidgetRuntime association, module-origin binding, receiver
registry, native lifetime signal or production caller. Its value data may have
been created by any isolated runtime fixture. A qualification string alone does
not prove same runtime/module ownership.

Candidate Values::defaultPalette, exposed as applicationPalette(), stores a
WidgetPaletteValue. Startup seeds alias1/word13893616, classCachePresent=true and
QMenuBar/QMenu classes. The class-cache bool/map is NOT app_pal. The data graph
can back the global wrapper after identity qualification; creating a second
palette graph would violate sharing. No existing source certifies app_pal's slot
state by merely having defaultPalette or matching source data content.

The actual consumer is CP75 SplitViewPlaceholderCall::value()->widgetState(),
already bound to its actual runtime/control block. It is parentnull in private
construction, with native thread still null. It stops at PrivatePalette and
cannot be substituted by MainView, SplitView, or a staged editor.

## Exact native sequence, including temporary lifetime

The module path is6505e8d4 -> QPalette6504abb0 -> QApplication::palette65016f70.
Source QWidgetPrivate destination is SAME placeholder private+170.

1. Default QPalette constructor6504abb9 passes a local temporary at rsp+20 into
   QApplication getter. Getter loads actual module app_pal pointer at65016f8a.
2. Unknown slot cannot be treated as null. Native known nonnull slot skips lazy
   allocation. Native null requests16-byte QPalette wrapper, calls global-color
   constructor6504bf80 with argument2, writes slot65016fb9, then copies output.
   Failure/unwind/null continuation is not a normal-success alternative.
3. Getter65016fc6 invokes QPalette COPY constructor6504a8b0. It shares the same
   data owner, performs native ref at6504a8c6 and ultimately copies the complete
   source group/resolve word. Its two masked writes reconstruct that word; do not
   infer initialized padding from intermediate writes. The getter returns this
   temporary value, distinct from app_pal's retained wrapper and private output.
4. Default constructor6504abc8..abcb stores that SAME shared data pointer into
   the private destination. It then calls native deref on the getter temporary
   at6504abce. If false, the branch6504abd8..ac08 destroys its60 (0x3c) brush elements
   (element size8) through6511cbc0 and frees its data. This branch is real source;
   it cannot be replaced by an unconditional no-effect temporary discard.
5. After that branch returns,6504ac10 writes ENTIRE private destination word8=0.
   Then6504ac17 refs the destination data; RET6504ac25.
6. Only after this default constructor returns does QWidgetPrivate call default
   QFont at6505e8de. Locale and app/version checks are later, then actual QObject
   construction/thread retain/counter, QPaintDevice/QWidget class install/init,
   and much later the174->175 QWidget-count point6506e7b5.

CP77 read().output should preserve app_pal wrapper's nonzero group/resolve word;
that is the QApplication getter result. The placeholder's default QPalette must
end with word0, sharing the same DATA. Copying13893616 or a synthetic test word
like12345678 directly into the private field would skip6504ac10. Conversely,
zeroing app_pal wrapper/output at the getter would corrupt its separate value.

Current CP77 helpers use shared C++ value ownership but no explicit Qt atomic-ref
ledger or retained temporary-destruction cursor. WidgetPaletteDataState also has
no such ledger. Therefore a composition may recover owner/getter binding first;
it must not announce every native QPalette ref/release effect as modeled by one
shared_ptr assignment. A complete private-copy cut needs explicit lifetime/order
closure for that normal shared-data path (including proof/guard of the consumed
deref branch). The real retained app_pal wrapper is important evidence for
sharing, but numeric Qt refcount values were not captured for this call here.

## Smallest runtime owner join

Proposed independent shared-runtime seam, not an implemented API:

- One applicationPaletteOwner field in WidgetRuntimeState::Values, created only
  from an explicit qualified module/slot observation. Validate pinned QtGui SHA
  and slot RVA9c9690. Create the helper internally rather than accepting an
  arbitrary externally copied QApplicationPaletteOwnerState.
- A known-present slot names a positive palette-data alias resolved through THIS
  runtime's existing palettes registry and its actual wrapper group/resolve word.
  Preserve exact data raw/control-block identity. Identical aliases in another
  runtime, equal words or matching palette contents are insufficient.
- Unknown remains unknown and cannot call a provider. KnownNull may progress only
  through the source-qualified QPalette(GlobalColor2) producer. The existing
  runtime default value is NOT an implementation of that constructor; production
  provider absence retains InitializerUnavailable.
- KnownPresent reuses the same canonical wrapper/owner; do not allocate one owner
  per placeholder, copy an external helper, infer presence from classCachePresent,
  or recreate the module slot after runtime loss. Preserve existing helper's
  conflict rules and treat any future native setter/replacement as a separate
  source transition, not relaxed observation merging.
- Resolve the registered owner for a live canonical same-runtime placeholder;
  retained production calls check that exact owner and runtime on every resume.
  If diagnostic strong helper leases survive runtime death, they must not confer
  native callability. Current standalone helper read is not itself that grant.

An illustrative runtime observation would carry module hash/RVA, qualification,
slot knowledge and optional present WidgetPaletteValueObservation. The exact API
belongs to a separately assigned runtime owner lane. Reading/binding the owner
must not resume the placeholder or change its field/counters.

Do not require a completed receiver defaultPalette before resolving app_pal:
that would be circular because this is the producer of that field. Receiver
qualification can use its actual live prefix/call identity and source stage;
module-global provenance belongs to the runtime owner. If a per-widget private
input record is retained for later helper access, distinguish an execution-
produced receipt from an unrelated external observation. Existing explicit
observations must agree with the same current data, never replace it.

## Bounded composition cursor

After that runtime seam freezes, an isolated placeholder continuation can:

1. At PrivatePalette, resolve exact runtime app_pal owner for SAME value/widget.
   Missing owner/profile remains a precise guard; do not use defaultPalette as
   fallback. Retain the owner identity once, rejecting changed origin.
2. Execute/retain ONE owner read result for this call. Pending Unknown/null-provider
   dependencies keep the source prefix and have no output copy. A successful
   getter output is retained across all subsequent private-copy guards. Repeated
   resume must not call read again, allocate/cache again, or emit OutputCopied
   again. Another placeholder has its own returned temporary/cursor while sharing
   the SAME module wrapper/data.
3. Before claiming full QPalette completion, recover the destination pointer
   store -> temporary deref/release branch -> word0 store -> destination ref in
   that order. If ref-lifetime qualification is incomplete, the smallest safe
   integrated cut can retain the completed getter output and guard BEFORE the
   private pointer store. This avoids pretending a partial field is fully built.
4. On a completed private copy, assign this widget's palette with SAME data and
   word0 once, record PaletteConstructed, then advance to PrivateFont and guard
   its independently unqualified current application font. Do not infer font,
   locale, thread, app presence, registry, opaque masks or counters from palette.

WidgetState currently has optional<WidgetPaletteValue> with a fully known word,
not a separate partially stored data pointer. Publishing{data,0} before the
native temporary-release boundary would misorder a retained pending call. Either
close the entire normal private-copy body without an intervening unsupported
edge, or explicitly represent that partial source field before crossing it; do
not add a second live palette graph or use a success boolean. Holding getter
output before the pointer store is a small honest interim cut.

The first useful code split is runtime owner binding and focused registry/lifetime
tests, separately from the placeholder .hpp/.cpp/test consumption. A third native
palette-value lifetime primitive may be needed to close full private copying;
that belongs to the owner helper lane, not an editor-local refcount. Existing
EditorBase/public palette guards and all earlier SplitView post/QFrame/QSplitter
and derived entry gates remain unchanged.

## Focused acceptance contract

Runtime join: unknown does not create slot/read/provider effects; present same-
runtime alias gives same canonical wrapper/data; wrong module/RVA, foreign data
control block, conflicting state/word, dead/rewrapped receiver and expired runtime
reject without publication. Two placeholders share module owner but have distinct
call temporaries. Null provider requires actual argument2 and source constructor,
not copied startup defaults. No class-cache substitution.

Composition: observer/bind only does not resume; unknown owner stays pre-copy;
late known-present input returns one retained getter temporary; retry does not
repeat OutputCopied/cache; nonzero app wrapper word remains nonzero while completed
private output is0; source data identity is preserved. Temporary release precedes
private word0/ref and font stage. Pending ref/lifetime dependency must not expose
a completed private palette. Font/locale/thread/registry stay pending, current/
maximum remain174 (or unknown), and no second placeholder/addWidget/public entry
is implied. No tests authored/executed here.

## Evidence identities and bounds

Pinned original QtGui SHA-256:
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Direct bounded DUMPBIN14.44.35228.0 windows used preferred65000000 image, with
no DLL loading. Native ranges end-exclusive:

| Native bytes | SHA-256 |
|---|---|
|6504abb0..6504ac26 (full selected default ctor)|704806e2a08c3327f0cb7f8d0ca7c496af0c69dbd897d456e48713cf1fa6a399|
|6504a8b0..6504a8fd (copy ctor through RET)|a0c82e2e8037dc55d2034c9b82ad3ae911f3c4af2e740438ecfbe4e9970cc03b|

The copy window extended past RET into padding/next procedure; only the exact
range above is claimed. Existing import map binds65696978 to QBasicAtomicInt::deref
and656968c0 to ref. Retained source hashes:

| Artifact | SHA-256 |
|---|---|
| include/m2m/engine/application_palette_owner.hpp (app root) | ac6badf7155d8c1c7b1cfd816719e3685d5b3e65dbb2731f51f41a346b858e58 |
| src/engine/application_palette_owner.cpp (app root) | 58d6355b88150698b0af87128e8994f3595fa2c4d0a8db995aec1875d7851348 |
| src/engine/splitview_placeholder.cpp (app root) | 80427dacd058628cfd6baf0b699ee4cd43b861f1c041abf99e1f20e8a1ae0e04 |
| src/engine/view_manager.cpp (app root) | dc6ad8356e061bf7e9aebaa1d32ef2caa8a0d4d41da89720c023a4b825899a4c |
| analysis/resume76-qapplication-palette-owner-cache.md | 1a636d1abec8fca80441ad6573ebaa403b0e05eaa3e660bfb96c1ed682c14ac9 |
| analysis/resume77-app-pal-owner-implementation.md | fd2601f85fb6c686199d9d0475c7dd2622b31c0ab0afef2c9567fb666feb80f5 |
| analysis/wave3-closure-next-65016f70.asm | 632a5d4727fec292c646769f91acee102a871b4dddbb1f202c94da6670117c39 |
| analysis/wave3-closure-next-6504ac0d.asm | 93fa1d9b361e44b142170e0b2a03a573cdb174b316185103a184d3409f5c1163 |

App root is source/apps/moproj2mst. Native slot state has not been observed here;
no continuous startup/Open, palette constructor completion or public Save is claimed.
