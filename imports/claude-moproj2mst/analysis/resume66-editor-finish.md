# Next editor cut: actual null-image finish before opacity decision

The next bounded integration is one-time transfer of CP65's held actual I1 into
PaintDeviceNullImageFinishCall, then entry to the existing OpacityDecision only
after that consumer returns. The consumer's same-value outer destruction and
independent canonical global cleanup must precede opacity publication. No public
mode300/default-palette admission or shared-helper change is required or justified.

Owner: editor300. Only this report was written. CP65 source remains frozen and
its hashes were independently rechecked. Existing bounded native captures/source
were read and selected native PE bytes hashed; no source/build/CTest, original
execution, GUI or mutable REA actions. Token usage is unavailable. Root's CP65
validation is separate; this audit does not assume its result.

## Independent native order

Original QtGui SHA-256 remains
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Read retained wave3-editor-parentless-opacity-6511c090.asm,
resume48-brush-style24-650aa340.asm/650b3300.asm,
wave3-closure-next-6506e4d0.asm and
wave3-editor-parentless-opacity-65097940.asm, plus CP62/63/64 owner qualifications.
The selected native chain is:

| Address | Required order |
|---|---|
| 650b9b19 ->6511a67e | Active null-copy returns the actual caller-owned I1, after inner J destruction |
| 6511c18d | hasAlphaChannel(I1); null data reads only I1+10 and returns false |
| 6511c194 | Store !hasAlpha privately in DIL |
| 6511c1ac | Destroy SAME I1 through QImage class/null-data prefix, QPaintDevice prefix and global cleanup |
| 6511c1db | Only after destructor returns, publish DIL as AL |
| 6506e71c..e729 | QWidget's isOpaque call returns; normalize entered nonzero-style window result |
| 6506e72b..e73c | Read current private+1d8 bit17 and compare with desired result |
| 6506e73e | Equal bit skips store AND native-opacity helper |
| 6506e74d | Changed bit stores only bit17 in private+1d8 |
| 6506e756 | Call native-opacity helper65097940 AFTER that committed store |
| 6506e75b | Continue later QWidget tail |

The image branch's cleanup mask is4, so its later pixmap cleanup tests skip both
pixmap temporaries. No isQBitmap call belongs to this path. The actual returned
image's word0 follows its own constructor, independently of cached source word1/
8000/ffff and brush alpha. Inner J and outer I1 share module origin, not lifetime,
prefix state or a cleanup receipt. Their getter/getter/exact0 reads occur twice
on separate call frames. Null data causes no image-data ref/deref/free.

Native-opacity helper65097940 checks actual window status, then raw120; known
false returns at650979cf without platform handle/style calls. True continues
into still-guarded native-window state. Existing editor code conservatively
holds unknown/true raw120 after storing bit17. A retry must retain OpacityEffect;
it must not recompare the newly equal opaque bit and skip a previously reached
native helper. That downstream behavior already exists and should be reused.

## Current ownership and exact integration seam

CP65 has already selected the actual current palette brush and consumed alpha,
then reached the original image getter/cache active-copy dependency. CP64 supplies
one canonical source view with per-current-receiver module grant. One retained
producer finishes inner cleanup and moves actual I1 into
textureImageActiveValue_. TextureImageActiveFinish currently stops there.
No predicate or outer destruction has happened, and no old ordinary-image data
boundary/projection may be used to finish that value.

Add only a retained unique_ptr<PaintDeviceNullImageFinishCall> member in
EditorBaseConstruction, with a const diagnostic accessor such as
textureImageActiveFinishCall(). Reuse TextureImageActiveFinish as the retained
callee stage. Add a precise pending issue (TextureImageActiveFinishPending) and
an outer-finish returned receipt, rather than treating a pending callee as success.
Existing active producer/value diagnostics must retain clear ownership semantics:
after the unique move, the old value holder is empty and the finish diagnostic
provides the SAME prefix owner. Do not copy/reconstruct a value just to keep the
old raw-pointer accessor nonnull.

The stage must perform:

1. If no finish callee exists, validate held actual I1 lifetime and exact inherited
   weak module origin against runtime->paintDeviceCleanupGlobals(currentWidget).
   This is the CURRENT receiver grant, not a source-widget, startup or editor-local
   module. Only then move the unique value into the existing consumer once.
2. On every pending finish resume, validate the callee's inherited origin against
   that same current receiver. Do not require cached source-view lifetime after
   the producer has returned: I1 is independently owned. Native widget/runtime
   identity checks at the top of EditorBase still apply.
3. Resume the SAME finish object. If not Returned or opaque.has_value is false,
   retain TextureImageActiveFinishPending with no opacity decision/bit/tail effects.
   Nested value/class/module/prefix/cleanup guards remain visible in diagnostics.
4. Once Returned with an actual opaque value, record the active finish return,
   assign opacityDesired_ from that returned value, emit TextureImageOpacityResolved
   once, and set stage OpacityDecision. Do not derive it from consumed brush alpha
   or insert an opaque-result observation/input. The current null-only consumer
   returns true, but the caller should still consume its actual optional result.
5. Reuse existing OpacityDecision/OpacityEffect/SharedTail. Those commit one
   PaletteOpacityResolved, changed bit17 if needed, and the reached native-helper
   cursor before later constructor dependencies. After leaving ActiveFinish,
   retries must never resume the producer, move the token, call finish again or
   rerun original getter/predicate/destructor/global reads.

Do not early-check the old unique value holder on a retry after it was moved;
that would falsely report ValueUnavailable. The consumer owns pending lifetime
and its completed historical receipt. Likewise, its native owner is deliberately
dead after completion, so caller logic must not reinterpret a completed receipt
as a missing value and rebuild it. Keep return/guard checks in their proper phase.

The existing ordinary TextureImagePaintDevicePrefix/TextureImageCleanup path and
its predicate/data summary remain untouched. Active completion must bypass
TextureImageDataBoundaryReached, projected TextureImagePaintingWordRead/
TextureImagePaintDeviceClassInstalled and the ordinary textureImageCleanup_ frame.
Only the consumer's actual I1 nested prefix/cleanup count for active completion.
TextureImageOpacityResolved can represent either completed image branch, followed
by the one common PaletteOpacityResolved stage.

## Reachable focused tests and migration of CP65 cases

Suggested exclusive files remain editor_base.hpp/.cpp and test_editor_base.cpp;
root owns build/integration. No view_manager, paint/helper, CMake or public/session
file is needed. Extend the existing m2m.mdt.editor_base target with:

- Alpha0/255 and source word1/8000/ffff with explicit null/cache-free metadata and
  actual counter0: one source/producer/consumer, same outer owner, separate inner/
  outer prefix+cleanup frames, exact finish-before-opacity-before-bit-store order.
- Confirm actual QImage consumer effects (predicate read/private negation, QImage
  class/data read, outer prefix, new getter/getter/comparison, destruction, result)
  precede all widget opacity effects. Ordinary summary/projection/cleanup effects
  remain absent, old image helper does not retain/destroy anything and source
  fields/image-data refs stay unchanged.
- Existing late unknown-pointer/unknown-counter then0 case advances through the
  same producer into one consumer, without alpha, getter, rectangle or inner
  cleanup replay. Observations alone do not advance either call.
- Set raw120=true only AFTER the fixed constructor's extended-word zero stage,
  using the existing DirectionUnavailable/late-direction fixture pattern. Verify
  outer destruction already completed before NativeOpacityUnrecovered, bit17 is
  committed, repeated resumes preserve all producer/consumer effects, and late
  raw120=false resumes only the native helper/tail. Do not set it before the
  constructor and accidentally test a value that the constructor overwrites.
- Keep actual current-group/brush24 versus decoy brush23 test; one receiver's
  module grant cannot authorize a second widget before its own explicit grant.
  Two values share source/module but own independent finish/prefix/cleanup calls.
- Module/source loss while the PRODUCER is genuinely pending prevents handoff;
  abandonment invalidates held values without fake native destruction. Successful
  independent returned-value lifetime/consumer module-loss behavior is already
  tested in the standalone helper; do not add artificial yields/backdoors.
- Active nonnull, unknown painting word, pixmap selection/cache conversion and
  private/public constructor dependencies retain existing guards. Complete
  QWidget tail only when its separate qualified inputs exist; wrappers/public
  save stay guarded even if that tail eventually returns.

CP65 cases that stopped at FinishUnrecovered should now expect the next genuine
NativeOpacity/constructor-tail dependency. They cannot interpose source/view
release between producer return and consumer invocation within a valid synchronous
resume. Preserve that lifetime coverage in standalone owner tests, or exercise
source loss before producer return and the downstream historical receipt after
finish. Do not introduce a synthetic suspend point merely to retain old assertions.

A valid current CP59/65 returned value implies inner existing-counter0; the
monotonic canonical snapshot cannot become unknown/null/nonzero before outer
cleanup via replacement observations. Source/module substitution is not a valid
way to manufacture outer pending cases. Standalone cleanup guards cover those
states. The actual constructor initializes opaque=false, so this null branch's
true result takes the changed-bit path under the current fresh bounded profile;
unchanged-bit behavior is already covered by other branches. Do not invent a
setter/backdoor to fabricate an unsupported constructor transition for coverage.

These tests are proposed, not authored/run. Use Release-active CHECK/REQUIRE with
setup evaluated; no assertion elision, final-image success shortcut or GUI receipt.

## Bounded byte and frozen-source receipts

Independently computed original QtGui PE byte hashes, ranges end-exclusive:

| Range | Native SHA-256 |
|---|---|
| 6511c18d..6511c1e3 | 068f095fd7a3d01cc341e2805aed5cd1b6e6d9347dfb2be6e6588737103429d9 |
| 6506e71c..6506e75b | 909ca72f1dd4adcc4e032e5847078c2d442ea1cfeb099539f472924cd147cd41 |
| 65097940..650979da | ff1c67468e44846bcd917cb85d59bfd1a7242b0fdeecf10a973ff606130d3fef |

No new disassembly artifact was needed; retained captures above contain these
instructions. Current CP65 header/source/test hashes are unchanged:
`ecbf786c000a6ad904ff2dfd5db90c34c2acea910801b24f85627aba5e5855aa`,
`ce2adbdc400c7759d3e2887ca4c52e373068bb4238e2f3d05eaf97c16dea25d9`,
`0a2b64d661d8e7e5f624f2efac6ba3403e15911fd75a92198fcdb196ff779363`.

The cut is source-qualified as an internal active-null image operation. It
supplies no unobserved post-Open private palette/font/source aliases. Public
Session PrivatePaletteUnavailable/not_ported6504abb0, later application-wrapper/
setup dependencies, original UI reachability and full fresh Open-Save-Close
fidelity remain guarded and unestablished.
