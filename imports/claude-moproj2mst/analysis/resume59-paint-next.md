# Next paint/image increment: active source with null image data

Select the active-source/null-QImageData copy branch as the next internal
increment, after root accepts the frozen prefix helper. It can return normally
using the known-zero prefix and existing-counter-zero cleanup without recovering
warning execution, geometry or pixels. Retain the nonnull active-copy, warning,
pixmap/conversion and public mode300 guards. Its reachability through valid
original UI state remains unestablished; this is a raw-state internal branch,
not another accepted real-project behavior.

Owner: editor300. This report is the only write. Existing source and retained
assembly were read; bounded DUMPBIN ranges were rechecked directly against the
original files without writing new captures. No source/CMake/shared changes,
GUI, original execution, mutable REA, build or tests. Token usage is unavailable.
Root's CP58 validation was pending during this audit; no pass is assumed here.

## Choice and native qualification

Read resume57-paint-device-prefix.md and resume58-paint-prefix.md, with
resume53-editor-style24-next.md and resume48-brush-style24-contract.md. The
controlling binaries remain original QtGui SHA-256
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`
and QtCore SHA-256
`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
DUMPBIN SHA-256 remains
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.

The nonzero destructor word remains the larger dependency. Read-word -> install
base class -> qWarning -> normal return -> cleanup is the required order. The
warning formatter branches on std::uncaught_exception, dispatches a callback or
stdio, checks QT_FATAL_WARNINGS after output and may exit(1). There is no closed
warning implementation or actual warning-environment/handler observation seam
in the current runtime. A known zero cleanup counter says nothing about whether
the warning returns. Do not add warningSucceeded, ignore this warning, or treat
a callback return as sufficient. The CP58 prefix correctly holds nonzero after
class installation; its public/normal branch stays restricted to word0.

The active-copy branch needs no nonzero destructor warning: it reads the cached
SOURCE word as nonzero, while both newly constructed temporary words are0.
Those are three distinct QImage values, with independent class/lifetime state.
The original active cached image is borrowed and not destroyed by this copy.

| Native point | Selected operation and required effect |
|---|---|
| 650b9a93..aa0 | Construct outer returned QPaintDevice word0, install QImage class |
| 650b9aa3..aa8 | Read source painting word and select nonzero branch |
| 650b9aaa | Initialize outer image-data pointer to null |
| 650b9ab7 | Default QRect constructor, endpoints0,0,-1,-1 |
| 650b9ac8 ->650b5df7 | QImage::copy rereads actual source image-data pointer; require null |
| 650b5e00..e10 | Construct distinct inner QImage word0, install its QImage class and null data |
| 650b5e17 ->650b66cd | Selected null-copy epilogue, no rectangle/pixel reads |
| 650b9ad4 ->650b8f3f | Assign inner to outer; read inner source word0 |
| 650b8fb8..ff7 | Read inner null data, read outer null data, reread inner null, STORE outer null again |
| 650b9ada..ae7 | Install inner QImage class for destruction, read/check inner null data |
| 650b9b13 | Inner QPaintDevice prefix and global cleanup must return |
| 650b9b19..b49 | Only then return the outer copy value |

The assignment has no null/null equality shortcut. Both stores to outer+10 are
real, at650b9aaa and650b8ff7. Nullness eliminates data ref/deref/free, not these
stores or the inner destructor. Source data is checked by QImage::copy before
any rectangle or pixel read; default QRect construction still occurs earlier.
Its four native store addresses are6700b232(+c=-1),b239(+8=-1),b240(+4=0),
b243(+0=0), then RETb248. Geometry must not be requested for this null branch.
The outer hasAlpha=false predicate belongs after the copy returns, not while its
inner destructor/global cleanup is suspended.

## Exact implementation contract

Recommended bounded implementation location is the existing internal
BrushTextureImageOpacityCall, because it already holds the actual runtime-bound
BrushTextureImageState and image-getter/cache order. Avoid introducing a second
observed source image or an activeNullCopySucceeded input. Root should assign
that helper's owned files and the editor binding separately before any edits.
An isolated active-null call can use the same contract if root prefers a new
file, but it must consume that actual source owner rather than copied flags.

Minimum new binding API:

`bool bindCleanupGlobals(const std::shared_ptr<PaintDeviceCleanupGlobals>&) noexcept;`

Bind only the actual runtime owner returned by
WidgetRuntimeState::paintDeviceCleanupGlobals(actualWidget), after the normal
live widget/private-source qualification. Null, unqualified/released owner or
replacement raw/control-block identity is rejected without effects. Unknown to
qualified owner is monotonic; equal owner is idempotent. Store it weakly. A
module profile may be qualified while pointer/value remain unknown: the retained
cleanup call must handle their later refinement. No editor-local globals or
counter0/default allocation is permitted. Observation/binding does not resume.
The runtime/private-source qualification must still hold on each editor resume;
escaped host payload/global leases cannot make a dead runtime callable.

Value-owner API needed for the later outer hook:

`std::shared_ptr<PaintDevicePrefixState> returnedImagePrefixState() const noexcept;`

The helper creates this outer value exactly once at the native copy-constructor
entry, before the reached source-word read; use CP58
fromReturnedImageConstruction with the actual frame's stable qualification.
A getter is diagnostic/continuation binding, not proof of full destruction.
Do not create the outer value from the cached source in the editor destructor
stage. Keep it alive across the copy, metadata and outer cleanup; its creator
must releaseNativeOwner only at actual completion or host abandonment. This
small value-owner seam is a prerequisite for binding the isolated CP58 prefix
into the existing ordinary editor path; it must not duplicate CP56's projected
read/class effects when that hook is introduced.

Retained active-null sequence:

1. Keep existing image selection/nullness/cache checks. Null data with unknown
   cached-pixmap presence waits there; present pixmap retains conversion guard.
   An unknown source painting word waits before selecting active copy. Ordinary
   word0 copy remains its existing branch.
2. After selecting nonzero source word, commit outer-null initialization and
   default-rectangle construction once, then the source data read in copy. Null
   admits the branch; nonnull retains ActiveImageCopyUnrecovered before any
   geometry/pixel operation. Do not move that guard ahead of reached constructor,
   outer-null, rectangle and source-data-read effects just to preflight safety.
3. Create the distinct inner prefix owner from its actual default QImage
   constructor; its word0 is source-proven, not copied from the active source.
   Commit inner-null construction, the assignment word read, inner-data read,
   outer-data read and inner-data reread, then the second
   outer-null store. Both nulls mean no image-data retain/release/free.
4. Commit the inner QImage destructor's class store and null-data read. Resume
   one retained PaintDevicePrefixCall on INNER only. TailReady then permits one
   retained PaintDeviceCleanupCall on the canonical runtime global owner.
   Missing owner, unknown/null/lazy pointer, unknown value and every nonzero
   counter remain pending with precise helper issues. Do not expose copy return,
   predicate, outer destruction or editor opacity while inner cleanup waits.
5. Only inner cleanup Returned ends that inner native lifetime and emits
   InnerImageDestroyed/ActiveCopyReturned (or equivalent exact events). Enter
   existing ImageFormat with an actual null outer value. Its hasAlpha=false
   follows without format/alpha/pixel reads. Outer data release is a null no-op,
   but outer QImage class/prefix/cleanup remain separate later caller work.
6. Retry retains construction/assignment/inner destructor and helper cursors;
   no replayed stores, rectangle, class effects or cleanup call. The outer
   predicate remains private until its own later prefix AND cleanup return.
   Inner and outer use two prefix owners and two cleanup call frames, even when
   the same typed runtime counter is borrowed by both. No frame borrows another
   frame's historical TailReady/Returned receipt.

This branch does not require source mutation, image pixel allocation, geometry
observations, global allocation/registration, conversion or warning callbacks.
Host abandonment before or after inner cleanup must not synthesize a native tail
return, repeat destruction or decrement a null image-data ref. The original
source's nonzero word, null data, cache and class identity remain unchanged.
The model's bookkeeping allocations are not native pixel-allocation effects.

## Focused tests to author

1. Qualified image-selected source, null data, no cached pixmap, words1/8000/ffff,
   actual existing counter0: exact outer/rectangle/inner/assignment/destruction
   order; two null stores; inner cleanup before copy return; source unchanged.
2. Unknown source word, unknown data and unknown cache presence keep their
   original reached guards; cached pixmap=true keeps conversion guard. Nonnull
   active image stops after the reached copy-entry prefix with no pixel work.
3. Missing global owner after inner assignment/prefix: no ImageValueCopied/copy
   return, hasAlpha, opaque result, outer destruction or editor bit17 effects.
   Late canonical binding resumes only the inner cleanup frame.
4. Qualified globals with unknown/null pointer, unknown counter and positive or
   negative nonzero value retain exact guards. Later unknown-value-to0 resumes
   without repeating getters, rectangle, null stores or destructor class install.
5. Explicit same owner rebinding is idempotent; foreign/rewrapped/released or
   replacement owner rejected atomically. Runtime death despite host leases
   blocks pending continuation. Observations/binding alone perform no effects.
6. Inner and outer prefix owners are distinct, each word0/class/lifetime local;
   active cached source word never becomes a destructor input. Inner destruction
   cannot release the outer owner or authorize its later prefix via one receipt.
7. After inner return, outer null predicate still waits for its own cleanup before
   editor opacity. Same counter produces two separate getter/getter/comparison
   sequences on two frames; source/image-data refs stay unchanged throughout.
8. Two independent copies of one active-null source share source/globals but
   have distinct outer/inner values and independent pending continuations.
9. Abandonment before inner prefix, during global pending and after inner return:
   no null-data ref/deref and no duplicate native destruction on retry or teardown.
10. Regression: ordinary word0 image path, alpha-capable/nonalpha format paths,
    pixmap/conversion and public application-wrapper guards remain qualified as
    before. Use Release-active CHECK/REQUIRE with setup evaluated outside assert.

These tests are proposed, not authored or executed. No real-project parity or
original UI reachability follows from synthetic active-null tests.

## Rechecked static intervals

Native byte SHA-256 below covers original file ranges, end-exclusive. DUMPBIN
commands used `/disasm:bytes /range:0xSTART,0x(END-1)`. Fresh bounded output was
inspected without persisting new asm; retained full copy/assignment/destructor
qualifications remain in the earlier reports and their artifacts.

| Interval | Native-byte SHA-256 |
|---|---|
| QtGui650b5dc0..650b5e1c | 8d651f8bcca0539698d8874baebbbaaaeb29103759cc41d3b46aac8240a45c5b |
| QtGui650b66cd..650b66e8 | 99d6d9ea16585b096da81c9f8eeae8aadc7dbb5ad229b481c291b78fe1f90bf8 |
| QtGui650b8f20..650b900e | 2cd423de4596c68fca22b5aa2934f142ec48e428a81390c465a9a12d8a73359c |
| QtCore6700b230..6700b249 | 642946cc8014e5ebaed10cdba9cd4b827e4804dceba539ef4e906fc40987449d |

Retained QtGui copy-constructor650b9a70..650b9b4a byte hash is
`5ded55d6db93d288f41fbb9c4e819b62fcb25f335e43662f5e98d0c28d30be07`.
Warning evidence remains the bounded CP57 captures and its pinned identities;
no warning, exception/fatal behavior or currently installed handler was executed.

Next dependency: root accepts CP58, assigns the actual temporary-owner seam and
active-null inner copy/cleanup helper scope, then reviews their integration into
the existing caller without premature predicate publication. No source edit or
new branch is enabled by this report; public mode300 remains guarded.
