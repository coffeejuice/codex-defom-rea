# Isolated returned-image QPaintDevice prefix

Source is frozen for root review/build. The new helper gives each returned
QImage temporary an explicit prefix owner and models the word-read/base-class
store order. Only known word0 reaches TailReady. Nonzero retains a warning guard
after class installation; TailReady still requires the independent global cleanup
tail before any full destructor/opacity completion claim.

Owner: editor300. Only the three new source/test files below and this report were
written. No editor/runtime/shared files, CMake/plan, build, original execution,
GUI or mutable REA changes. Token measurement is unavailable. Tests are authored
and statically checked, not compiled or executed by this lane.

## Evidence and implementation

Controlling evidence is analysis/resume57-paint-device-prefix.md, SHA-256
`e1fb60b6c58d1f97d2e274600c53f970e1962d95d483442ac131dc1a69fc9e35`,
with its pinned original QtGui/QtCore files, DUMPBIN identity, native-byte hashes,
import/export resolutions and unique assembly captures. QtGui original SHA-256
is `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
This increment models only65138190's prefix: compare actual word+8 at65138196,
store QPaintDevice class at651381a5, skip qWarning only for0, tail-transfer at
651381bf to65148840. No new original observation is asserted.

New API in m2m::engine:

- PaintDevicePrefixState(qualification) starts with unknown word/class and an
  unqualified profile. A PaintDevicePrefixObservation requires its exact live
  receiver/control block and matching nonempty qualification; optional uint16
  word refinement is atomic and monotonic. Conflicts, foreign/rewrapped/expired
  receivers and released native owners are rejected before changing state.
- fromReturnedImageConstruction(qualification) creates a separately owned value
  with source-proven word0 and QImage class identity, without an external
  observation. It requires a nonempty qualification and an actual caller that
  has modeled returned-image construction. Native6513814c sets that word during
  the constructor called at650b9a93. This API does not observe or copy a cached
  source image's word and is not connected to production image/editor code yet.
- releaseNativeOwner invalidates the actual lifetime independently of diagnostic
  host leases. Calls hold only weak references. Prefix state owns no QImageData,
  Qt reference or global counter. The creator remains responsible for lifetime
  end after the later cleanup, or abandonment.
- PaintDevicePrefixCall resumes PaintingWord -> BaseClassInstall ->
  WarningContinuation or TailReady. Effects are PaintingWordRead,
  BaseClassInstalled, CleanupTailReached in that order. The report retains the
  exact uint16 word. Unknown word stops before read/class effects; known nonzero
  retains WarningUnrecovered after the class effect, unchanged on retries.
  Every nonzero bit pattern takes that branch; no signed/negative shortcut.
- Each value permits only one committed prefix invocation. A host identity token
  claims the value when the word read commits. A second call cannot repeat it,
  including after abandonment; it gets PrefixAlreadyStarted without effects.
  Waiting for an unknown word has no native effects and does not claim the value.
  That token is host bookkeeping, not a native allocation/ref effect. Rewrapped
  call owners are rejected against the state's canonical shared owner.
- TailReady is an idempotent historical transfer receipt, not full destruction,
  cleanup return or public-save completion. It does not run warning, logging,
  environment checks, allocation/free, registration, mutex/list work or image
  ref/deref. It cannot restart another invocation after native-owner release.

The modeled class identity is an enum, not a host Qt vtable pointer. This helper
makes reached internal state/order inspectable without claiming native Qt object
storage. The admitted prefix is a valid-live-object, normal, sequential profile;
access faults, concurrent painting changes, allocation failure and exception
unwind are not reconstructed. No synthetic warning-success input is exposed.

The actual warning dependency remains guarded. The retained QtCore audit proves
that warning output may call a handler, format/allocate, inspect uncaught
exception state, check QT_FATAL_WARNINGS after output and call exit(1). Merely
entering warning cannot authorize the cleanup tail or opacity publication.

## Focused cases and Release checks

Eighteen new TEST_CASE cases, 122 CHECK and22 REQUIRE uses cover:

1. Constructor-proven returned0, ordered effects, class change and repeated resume.
2. Missing/unqualified owner; unknown word before class install; late0 without
   observation-triggered continuation; empty qualification rejection.
3. Words1,2,7fff,8000,ffff preserving warning/class state and rejecting false0
   refinement; invalid qualification/foreign/rewrapped/missing observations.
4. Rewrapped call identity rejection, conflict rejection after completed class
   store, expired/released native owners despite diagnostic host leases.
5. Distinct returned values and independent class/call effects, with no image-data
   reference operation; one value cannot be replayed by another call after
   warning abandonment or TailReady, including late qualification races between
   two suspended host frames.
6. Abandonment before the first word read leaves the native prefix unclaimed.
7. Composition with the frozen PaintDeviceCleanupCall: TailReady remains separate
   from unknown/null/value-unknown/nonzero/zero actual globals; a warning guard
   never enters the cleanup call even when its counter is qualified exactly0.

The test uses testkit/check.hpp's CHECK/REQUIRE macros, which evaluate conditions
and record failures without NDEBUG-dependent assert behavior. Setup observations
are inside REQUIRE and therefore remain evaluated in Release. There is exactly
one main calling testkit::run_all. These are source-level Release-active checks,
not a Release build/test result. Root owns CMake wiring, focused compilation and
execution, followed by its required broader checks.

Static checks passed for all three files: direct whitespace/conflict checks,
balanced delimiters after comments/string stripping,18 unique test names, one
main, and no assert. No test execution or broader fidelity is claimed here.

## Frozen files and next dependency

Paths under source/apps/moproj2mst/:

| File | SHA-256 |
|---|---|
| include/m2m/engine/paint_device_prefix.hpp | d9fe56443e2f27a52491b4a210e9e4635376275885ab3c203fc19df9b2d336be |
| src/engine/paint_device_prefix.cpp | 26260f172124da076bcca46a07fcbb4524a6bbe5739e8c2fa698e5b699087142 |
| tests/mdt/test_paint_device_prefix.cpp | c2fae9adab137661ac5cc5b8491e2defffd186bc0d01cd484caab2ffa3d5e6db |

Next: root review/build of this isolated helper. Future image binding must create
one prefix state from the actual returned-image construction frame, retain it
through data release into the prefix and subsequent cleanup, and reconcile the
image helper's existing bounded Returned/ImageValueDestroyed summary. It must
preserve private opacity, once-only data release and canonical runtime globals.
An inner active-null copy would need its own distinct prefix/cleanup frame before
the outer result can proceed; no active-copy path is enabled by this increment.
Pixmap/conversion, nonzero warning and global lazy/nonzero cleanup, later wrappers
and the public mode300 constructor guard all remain unchanged.
