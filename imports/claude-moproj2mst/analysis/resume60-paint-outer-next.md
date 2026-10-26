# Next bounded transition: finish the returned null outer image

The next source-qualified transition consumes CP59's actual returned value,
computes null-image hasAlpha=false, retains its negation privately, and finishes
that SAME outer QImage's destructor before publishing the image-branch result.
Its inner temporary has already been destroyed; inner completion is not an outer
destruction receipt. This can be an isolated value consumer and does not require
or establish any GUI/public EditorBase constructor.

Owner: editor300. This report is the only write. Source and retained native
captures were read, and exact native byte ranges were hashed directly from the
original PE. No source/shared/CMake changes, GUI, original execution, mutable
REA, build or tests. Token usage is unavailable. CP59 build acceptance is root's
separate responsibility; this audit does not infer a pass.

## Evidence and exact caller order

Read resume59-paint-next.md, resume59-paint-implementation.md,
resume58-paint-prefix.md and the CP57 warning audit. CP59 implementation receipt
SHA-256 is `ace6a2d155e2e9f6cc1180c87abfa47b63d177f215cf8a482f419fbe27e17d3f`.
The original QtGui SHA-256 remains
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Retained captures inspected:

- wave3-editor-parentless-opacity-6511c090.asm;
- resume48-brush-style24-6511a610.asm;
- resume48-brush-style24-650aa340.asm;
- resume48-brush-style24-650b3300.asm;
- CP57 prefix/warning and CP54 global-cleanup captures/qualifications.

The selected native style24 image branch has this order:

| Address | Operation |
|---|---|
| 6511a65b | Copy actual embedded source into returned image value |
| 6511a663..a67e | In selected style24, no fallback default-image source to destroy; return outer value |
| 6511c18d | Call QImage::hasAlphaChannel on the actual returned outer image |
| 650aa340..a347 | Read outer+10, branch on null |
| 650aa382..a384 | Null returns false without format, flag, palette or pixel reads |
| 6511c192..c194 | Test result, retain !hasAlpha in DIL |
| 6511c19d..c1ac | Clear QImage cleanup bit in local unwind bookkeeping, call outer QImage destructor |
| 650b331b..b3322 | Install QImage class again on that outer object |
| 650b3325..b332c | Read outer null data; skip image-data deref/free |
| 650b335f | Tail-jump to outer QPaintDevice prefix65138190 |
| 65138196..bf | Read outer word0, install QPaintDevice class, tail-transfer to global cleanup |
| 6511c1b2..c1db | After outer destructor returns, skip absent pixmap temporaries and publish DIL as AL |

For the image branch, local cleanup mask is4, so the later bit2/bit1 pixmap
cleanup tests do not create or destroy pixmaps. There is no isQBitmap predicate
here. CP59's active copy already destroys its inner image before returning at
650b9b19; the outer image is the value used by these later caller operations.

The distinction is important: the native program computes a boolean before
outer destruction, but it does not return that boolean before destruction.
Pending outer cleanup must retain the private predicate and withhold completion.
A null image means no image-data reference operations, not no QImage/QPaintDevice
destructor or global cleanup.

The selected raw image-branch result is true after normal destruction. This is
not a QWidget opaque-bit update or a public-save completion claim. The caller's
already consumed brush-alpha cursor, style/getter selection, cache behavior and
later QWidget/application wrappers remain separate. An active-null raw state is
still not established as reachable through valid original UI state.

## Exact next helper and prerequisite handoff seam

Proposed isolated consumer:

`PaintDeviceNullImageFinishCall(std::unique_ptr<PaintDeviceActiveCopyValue>);`

It must consume takeReturnedImage(), not reconstruct from prefixState diagnostics,
source flags or a synthetic null QImage. The unique value is available only after
CP59 Returned; null input remains guarded. The consumer owns native lifetime
responsibility while pending and through completion/abandonment. Original source
and copy-frame host objects need not remain alive after proper value transfer.

One small prerequisite is currently missing: PaintDeviceActiveCopyValue carries
prefix/data identity but no module-global provenance. Its producer did bind the
actual canonical globals for inner cleanup, yet that weak identity is not handed
off with the outer value. Do not resolve this by taking an arbitrary independent
outer global owner, inferring a counter0, or constructing editor-local globals.

Proposed narrow shared-owner extension, only when root assigns those files:

- Add one private weak PaintDeviceCleanupGlobals owner to the returned value.
- CP59 commits that SAME owner when its retained inner cleanup normally returns,
  before ActiveCopyReturned/takeReturnedImage. This is host provenance bookkeeping,
  not a new native Qt store/ref operation.
- Expose `std::weak_ptr<PaintDeviceCleanupGlobals> cleanupGlobalsOwner() const noexcept;`
  on the value, or grant the isolated consumer equivalent narrowly scoped access.
  Keep the constructor private to the producer. No setter or independent replacement
  parameter is needed on the consumer.
- The consumer verifies this origin is qualified, canonical and natively alive,
  and weakly retains it. Owner expiry/release blocks pending continuation. Host
  leases must not substitute for the runtime's native owner. The exact runtime
  widget/source binding is still a separate future integration dependency.

This is a smaller identity seam than a new runtime observation scheme. It allows
an isolated returned-value consumer to use the same actual module as the inner
call without assuming a GUI constructor. Root must explicitly assign the existing
value/helper files for this extension; this audit changes none of them.

## Retained consumer state and effects

Suggested stages: ValueQualification, AlphaPredicate, QImageDestruction,
PaintDevicePrefix, Cleanup, Returned. A report's optional opaque result stays
absent until Returned; completeForPublicSave remains false.

1. Validate actual unique value, live native prefix state, expected QImage class,
   qualified module origin, known null image data and constructor-proven word0.
   Unknown/non-null data or invalid origin/value remain guarded. Do not consume
   another frame's receipt or manufacture a value from a diagnostic shared_ptr.
   These are modeled origin/lifetime checks, not extra native field reads. The
   actual painting-word read remains later inside the QPaintDevice prefix.
2. Read outer data for hasAlpha; for null retain false. Negate into a PRIVATE
   predicate, matching6511c194. Record the reached predicate read/result/negation
   once; no format, alpha-bit, rectangle, pixel or image-data ref operation.
3. Record the repeated QImage class store BEFORE its separate data read/check.
   The class is already QImage from construction, but the reached destructor
   store must not be skipped merely because the enum is equal. Null data skips
   deref/free; do not emit a fabricated ImageDataReleased event.
4. Retain one PaintDevicePrefixCall on the same outer prefix state. Its known0
   normal branch emits read/base-class/tail once. Warning/claim/owner guards remain
   pending and cannot authorize cleanup. Do not create another prefix state.
5. Retain one NEW PaintDeviceCleanupCall on the weak inherited module owner.
   The inner call's historical Returned is not reused. Required effects are a
   new first getter, new second getter and new exact0 comparison. A failure/loss
   or pending helper keeps the private predicate and committed earlier effects.
6. Only outer cleanup Returned ends this native value lifetime and records outer
   destruction. Then publish the retained opaque=true result and Returned. Keep
   a historical receipt on retries without duplicate data/predicate/class/prefix/
   getter effects. Host abandonment marks native-owner loss, not a successful
   QImage destructor or a second image-data release.

The module must remain valid before pending native continuation, including class
store execution; a dead runtime cannot be revived by escaped host leases. Normal
completion may retain diagnostic state marked native-dead, as CP59 does for its
inner value. The unique token prevents two consumers owning one image; CP58's
invocation claim additionally rejects replay through diagnostic prefix access.

This selected null path neither invokes the unrecovered warning branch nor
requires new actual source, format, pixel or global-allocation observations.
It does not repair the existing ordinary image helper's bounded
Returned/ImageValueDestroyed summary. That integration must separately replace
CP56's projected outer prefix with the actual value owner and avoid double effects.

## Reachable tests and limits of the current producer

Focused tests should construct values through real CP59 calls and ownership
transfer, not a new public fabricated-value constructor or test-only success flag:

1. Active/null source with explicit actual counter0: inner return, take outer,
   consume it; exact predicate -> QImage class/data -> outer prefix -> independent
   cleanup -> destruction -> opaque publication order, source unchanged.
2. Inspect outer QImage/word0/native lifetime before consumption; inner and outer
   owners/calls differ. Each emits its own getter/getter/comparison sequence.
3. Null unique token or premature/second take does not cause predicate effects.
   A manually released outer native owner is rejected despite diagnostic leases.
4. Destroy the source/copy host frame after transfer while the actual module
   remains live: the consumer still owns and finishes the outer value. Conversely,
   module release/expiry before completion prevents native effects/public result.
5. Attempt to use a prefix already claimed through diagnostics: do not replay
   destruction or reuse another call's TailReady receipt. Wrong/expired class or
   lifetime domain cannot be promoted to a completed null-image operation.
6. Two actual copy results from one source/module: independent outer consumers,
   separate prefix/cleanup cursors and value lifetime; one completion/abandonment
   does not finish or release the other.
7. Abandon before predicate, during any genuinely reachable pending guard, and
   after normal completion; no image-data ref/deref and no fake destructor return.
8. Released module despite retained host global/value leases stays blocked;
   no outer owner replacement or counter inferred from inner/image references.
9. Release-active CHECK/REQUIRE must evaluate all setup; public/runtime/editor
   guard and no-format/no-pixel behavior remain separate regression boundaries.

CP59 can hand off a value only AFTER its inner cleanup's existing-counter0 return.
In the current monotonic snapshot model, that SAME global pointer/value cannot
then become unknown, null, nonzero or a different owner. Consequently tests that
create a CP59 result and bind unrelated unknown/nonzero outer globals would test
an invalid owner substitution, not a reachable native continuation. Do not add a
backdoor just to cover those branches. Existing standalone cleanup tests qualify
its unknown/null/nonzero guards. Future ordinary-null producers could legitimately
reach outer cleanup without a prior inner comparison; they need separate source
qualification before widening consumer cases. No dynamic counter transition or
concurrency model is recovered by this report. For a valid CP59 value and live
module, the currently admitted consumer can complete in one resume; do not add
artificial yield/mutation seams merely to manufacture a mid-destructor test guard.

These are proposed tests, not authored/executed checks. Module loss and token
misuse are reachable internal guard tests; arbitrary warning success, alpha data
and mutable snapshot transitions are not admitted.

## Native-byte receipts and current source identity

Ranges are end-exclusive, independently hashed from the pinned original PE:

| Interval | SHA-256 |
|---|---|
| 650aa340..650aa385, exact leaf through RET | 269882aeeceb30c5c919cafa6a370c35653e6028feb7f00b29ebe16580676ef0 |
| 650b3300..650b3364, destructor through tail jump | 5784a5e783c8f86a7dd0967e92f75e3a590ba85c95d1f147a83f9d3a14d55e64 |
| 6511c170..6511c1e3, image/caller cleanup slice | 3ebfc309bad6ea17f6c0f1e3c33dbfa9906c20a9ca0f0a30b5f9fc5541186265 |
| 6511a610..6511a68d, getter unwind region | f5704cb3df184a1e9874b7e005bf62cf4021e9b0d86c55447bc4a8a77c5798cd |

The older650aa340 capture contains neighboring bytes after the relevant RET;
this report claims only650aa340..a385. Unwind bounds are not all exception/error
coverage. CP59 header/source hashes remain
`311847e0420c3738339954619a85104a47b2f9a7d305c3f36bf85d2aa3efbcd8` and
`0c216d2ced9c727a095f60dc52152f014a5794af151abac1e4d7a136c3f40290`.

Next dependency: root review/assign the weak module-origin handoff and isolated
null outer consumer, then focused validation. Actual canonical brush/source
runtime binding, outer editor hook, warning, nonnull active copy, pixmap/cache
conversion and public constructor/application wrappers remain unclosed.
