# Isolated active-source/null-data QImage copy

Source is frozen for root review/build. The new internal helper returns a null
outer image only after constructing, assigning and destroying its distinct null
inner temporary through the zero-word prefix and selected actual-global cleanup.
Unknown, inactive and nonnull source branches stay guarded. No public/editor
path, existing image-helper summary or runtime binding was changed.

Owner: editor300. Only the three new source/test files below and this report
were written. No existing shared interfaces, CMake/plan, build, original execution,
GUI or mutable REA changes. Token usage is unavailable. Tests are authored and
statically checked, not compiled or executed in this lane.

## Evidence and exact scope

Controlling report: analysis/resume59-paint-next.md, SHA-256
`c8f3d8a751ecc3ef994ae400d3f834fbf792f5f208d89f89659d11c973821bed`.
It retains original QtGui/QtCore identities, native-byte hashes and the exact
branch/call order. Also read the CP57 warning audit and CP58 prefix contract.
The original QtGui SHA-256 remains
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
No new original runtime observation or valid-UI active-null reachability is
asserted; synthetic raw-state coverage does not establish real-project parity.

The selected copy is650b9a70: constructor65138140 initializes outer word0 before
650b9aa3 reads the source word. Nonzero source initializes outer data=null at
650b9aaa and constructs default QRect. QImage::copy tests source data at
650b5df7 before any geometry/pixel read. Its null branch constructs inner word0,
QImage class and null data, then returns through650b66cd. Assignment650b8f20
reads inner word0, inner null data, outer null data, rereads inner data and stores
outer null again at650b8ff7. It has no pointer-equality shortcut.

The copy's inline inner destructor installs QImage class at650b9ada, reads null
inner data and calls QPaintDevice prefix at650b9b13. Only after its actual cleanup
returns does650b9b19 return the outer image. The two temporary painting words are
constructor-proven0 even when the cached source word is1,8000 orffff. No warning,
image-data retain/release/free, geometry/pixel read or pixel allocation is needed
for this branch. Default QRect stores remain reached; endpoints are0,0,-1,-1.

## New isolated API and ownership

- PaintDeviceActiveCopySourceState represents one actual native source QImage.
  Its receiver-scoped observation carries matching nonempty qualification,
  optional uint16 painting word and optional typed shared QImageData owner.
  Unknown/null/nonnull data are distinct. This is an isolated owner, not a copied
  view of the existing runtime brush metadata or a copy-success flag.
- Observation validates the full envelope before mutation: canonical exact
  receiver/control block, qualification, nonzero data alias and monotonic word/
  data-owner identity. Conflicting/foreign/rewrapped/dead observations are rejected
  atomically. Source native lifetime is explicitly separate from host leases;
  calls borrow it weakly and reject source release/expiry or a rewrapped call.
- PaintDeviceActiveCopyCall(source, frameQualification) retains the native cursor.
  It creates outer PaintDevicePrefixState at the reached constructor entry even
  while source word is unknown. Known0 retains InactiveSourceUnrecovered after
  the word read; this isolated helper does not reimplement the ordinary branch.
  Active nonnull data stops after outer null/default rectangle/source-data-read
  effects with NonNullActiveCopyUnrecovered, before geometry or pixels.
- The admitted null branch creates a distinct inner prefix owner, null assignment
  effects and inner QImage destructor class/data effects. One retained
  PaintDevicePrefixCall completes inner's word0 prefix. One separate retained
  PaintDeviceCleanupCall then controls normal copy return. Effects retain both
  outer null stores and all assignment reads, including the inner-data reread.
- bindCleanupGlobals accepts only a live, qualified, canonical module owner.
  Absent-to-owner binding is monotonic; exact raw/control-block owner rebinding
  is idempotent, while null/unqualified/rewrapped/released/replacement binding is
  rejected without cursor/effects changes. The owner is weakly held, and an
  expired bound owner cannot be replaced. Future integration must supply only
  WidgetRuntimeState::paintDeviceCleanupGlobals(actualWidget), with live private
  source qualification on every continuation; no local substitute is created.
- Missing globals, unknown/null/lazy pointer, unknown counter and every nonzero
  counter hold at inner cleanup. No copy return or alpha/opacity is exposed.
  Late legitimate counter refinement resumes the same cleanup call without
  replaying constructor, rectangle, assignment, prefix or getter effects.
- InnerCleanupReturned and InnerImageDestroyed precede ActiveCopyReturned. The
  inner native lifetime ends; outer remains QImage, word0 and null, with its own
  later prefix/cleanup still required. The report has no alpha/opaque result and
  completeForPublicSave is always false. Returned is a historical copy receipt.

Temporary ownership uses PaintDeviceActiveCopyValue. The call initially owns
outer and inner values uniquely; diagnostic prefix-state leases do not extend
their native lifetime. takeReturnedImage transfers the actual outer value's
lifetime responsibility only after Returned. A mere diagnostic lease cannot
perform that transfer. The unique value owner marks native-owner loss when
abandoned; it does not invent native destructor/cleanup effects. Taking the
result twice or before return fails. An escaped result may outlive the call,
but its own owner must still complete later outer destruction or abandon it.

Inner/outer values use the frozen CP58 state. Each has distinct class state and
prefix invocation; the active source is never used as their destructor receiver.
The helper neither destroys nor changes its source. It also stops if a pending
temporary's native owner was explicitly released, despite a diagnostic lease.
Independent calls can share source and actual globals, but never value owners,
prefix calls, cleanup cursors or historical completion receipts.

## Deliberate integration boundary

The existing BrushTextureImageState has no public field accessors and shared
interfaces were excluded from this task. Therefore this implementation does not
bind a new owner beside its existing runtime cache or duplicate observations
into it. Future integration must intern this source as that actual native QImage
value (or deliberately adapt the isolated call to the existing canonical owner),
not copy its flags into an unrelated value and claim identity. This remaining
seam was announced to root before source implementation.

No alpha predicate, outer QImage destructor, public EditorBase branch, cache
conversion or active-nonnull copy is enabled. The caller must retain the returned
outer owner through its null predicate and separate outer class/data/prefix/
cleanup sequence before publishing editor opacity. Existing CP56 projected outer
prefix effects and the old image-helper Returned/ImageValueDestroyed summary
must be reconciled during that later hook, without replaying data release.

The helper assumes valid-live-object normal sequential execution. Allocation
failure, exception unwind, concurrent source/global mutation, warning callbacks,
Qt pixel lifetime and arbitrary actual source transitions remain unclosed.
Bookkeeping allocations and host shared_ptr operations are not assertions of
native pixel allocations or Qt image-data reference effects. Runtime/module
profile ownership cannot be supplied by startup values or image ref counts.

## Focused tests and checks

Twenty-one new Testkit cases use185 CHECK and37 REQUIRE conditions. They cover:

- Source words1/8000/ffff with known null data and actual counter0; exact17-effect
  order, default rectangle, two null stores, inner/outer identity/class/lifetime,
  source preservation and idempotent historical return.
- Missing qualification; unknown word after outer construction; unknown data
  after outer-null/rectangle; inactive and active-nonnull guards without pixel work.
- Missing/late globals; unknown/null/lazy pointer; unknown counter then0; positive
  and negative nonzero counters without copy return or image-data ref mutation.
- Whole-envelope rejection, data-owner/control-block identity, foreign/rewrapped
  source/module owners, source/module expiry/release and rejected replacement.
- Released temporary guards, independent simultaneous calls sharing actual source
  and globals, and separate outer prefix/cleanup after inner return.
- Abandonment before word read, during global pending and after inner destruction;
  diagnostic leases versus explicit returned-value ownership transfer; attempted
  replacement of the inner word0 with the source's active word.

The test uses testkit/check.hpp, whose CHECK/REQUIRE evaluate conditions in
Release without NDEBUG-dependent assert. Setup observations remain evaluated
inside REQUIRE. Static checks passed for all three files: whitespace/conflict
checks, balanced delimiters after comment/string stripping,21 unique cases,
exactly one main, no assert and no unparenthesized comma in CHECK/REQUIRE macro
arguments. These are source checks; no build/test execution is claimed. Root
owns CMake wiring, focused Release compilation/execution and required integration
checks. Existing helper/editor/runtime tests were not edited or re-run here.

## Frozen source and next dependency

Paths under source/apps/moproj2mst/:

| File | SHA-256 |
|---|---|
| include/m2m/engine/paint_device_active_copy.hpp | 311847e0420c3738339954619a85104a47b2f9a7d305c3f36bf85d2aa3efbcd8 |
| src/engine/paint_device_active_copy.cpp | 0c216d2ced9c727a095f60dc52152f014a5794af151abac1e4d7a136c3f40290 |
| tests/mdt/test_paint_device_active_copy.cpp | 8f5aaea0902b504b41402ccfef95d6b8cc702a5bdfa6dd2e04cba298ce993a91 |

Next: root review/build this isolated copy contract, then assign canonical
source/value binding and outer lifecycle integration. Warning execution,
nonnull active copy, pixmap/conversion, global lazy/nonzero cleanup and public
constructor/application-wrapper guards remain unchanged.
