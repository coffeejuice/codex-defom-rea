# Returned QImage identity and canonical outer-cleanup owner

Static original evidence resolves the outer value as the caller's hidden return
storage and resolves both inner/outer cleanup calls to the same QtGui module
symbol. The smallest next implementation is to carry the producer's exact weak
module owner with that unique returned value, then consume it for the outer null
predicate/destructor. This provenance carrier is reconstruction bookkeeping,
not a newly discovered Qt field or a native weak-reference operation.

Owner: editor300. Only this report and the unique raw capture below were written.
No source/shared/CMake edits, GUI, original execution, mutable REA, builds or tests.
No mutable REA lane is needed for this bounded address/control-flow proof.
Token usage is unavailable. Source/runtime binding and public constructor gates
remain unresolved; this report is not a completion or runtime-observation claim.

## Exact value identity

Read resume60-paint-outer-next.md, resume59-paint-next.md and
resume59-paint-implementation.md with their pinned original byte/source hashes.
Original QtGui SHA-256 remains
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Retained wrapper/getter/copy/predicate/destructor captures establish this chain:

| Native point | Identity consequence |
|---|---|
| 6511c170 | Caller computes I1=its own current rsp+28 as hidden output storage |
| 6511c178 | Pass I1 as RDX to QBrush::textureImage6511a610 |
| 6511a628 | Getter saves that SAME hidden output address in RDI |
| 6511a63a ->6511a1f5/fb | Selected style24 cache getter returns embedded source address B+78 |
| 6511a655..65b | RDX=source, RCX=I1; call QImage copy650b9a70 |
| 650b9a90 | Copy saves actual output I1 in RBX; constructor writes it in place |
| 650b9ac0/9b0e | Inner J is copy frame's OWN rsp+38, distinct from I1 |
| 650b9b19 | Copy returns RAX=I1 after inner J destruction returns |
| 6511a67e | Getter returns RAX=its saved I1 |
| 6511c18a..18d | Alpha predicate receives RCX=I1 |
| 6511c1a7..1ac | Outer destructor receives caller's SAME rsp+28=I1 |

The offsets refer to different stack frames; matching small offset numbers are
not native identity. These are statically proved relative storage identities,
not observed absolute process stack addresses. There is no additional QImage
copy/move from a getter-local outer image into I1 in the selected path.
Consequently takeReturnedImage() must transfer host lifetime responsibility while
preserving the actual prefix/data value owner. It must not construct another
QImage value, emit another retain or recycle inner J's completion receipt.

The selected style24 getter skips its fallback default-source cleanup at
6511a663..67e. After I1 returns, native650aa340 reads I1+10; null returns false
at650aa382..384 without format/alpha/pixel reads. Caller6511c194 retains its
negation privately. Destructor650b3300 reinstalls QImage class and checks null
data, then tail-jumps65138190. Only after that returns does6511c1db publish AL.
Thus the returned value is both the predicate receiver and destructor receiver;
original cached B+78 remains a separate borrowed source.

## Exact canonical global identity

The unique new capture rechecks these direct calls from the pinned QtGui file:

- Inner J destructor call650b9b13 -> QPaintDevice prefix65138190.
- Outer I1 QImage tail650b335f -> the same prefix65138190.
- Prefix651381bf -> cleanup65148840.
- Cleanup65148858 and65148866 -> the SAME getter65146e30.
- Getter65146e3d reads QtGui preferred VA659cb858, i.e. module RVA9cb858;
  its existing-pointer return also reloads that slot at65146edc.

The owner key is the actual QtGui MODULE INSTANCE plus RVA9cb858, not an image
value, widget, source data alias, painting word or counter value. The current
pointed-to QAtomicInt is separate state. The selected counter0 branch borrows it
through two getter calls and compares exactly0; it adds no native counter ref.
Cleanup saves the device address in RSI, but its exact0 fast path never reads
that device's fields. A null image or zero painting word cannot establish a
counter pointer/value.

No per-image global-owner field is accessed or transferred by this selected
copy/cleanup sequence. In particular the relevant image accesses are class+0,
painting word+8 and data+10, while the cleanup getter reads the fixed module slot.
Do not add a weak pointer to a claimed native QImage layout. The host carrier is
needed because isolated reconstruction objects cannot otherwise remember which
module-instance domain their native operations belong to.

File SHA and slot RVA qualify code/profile identity, but do not prove two host
PaintDeviceCleanupGlobals objects are the same actual module instance. Require
same raw object AND control block. The original has one slot in one loaded image;
independent runtime fixtures model separate domains, not extra native globals
inside that one original image. ASLR changes loaded base, not the slot RVA.

## Current host owner and exact remaining gap

The current WidgetRuntimeState implementation has one values_->paintDeviceCleanup
container. observePaintDeviceCleanupInputs validates same live private-input
widget/runtime, then validates the original module SHA/RVA and monotonic typed
counter envelope before granting receiver access. paintDeviceCleanupGlobals(widget)
returns that SAME container only for a qualified receiver. Runtime destruction
calls releaseNativeOwner on it, invalidating escaped host leases. This is the
existing canonical origin seam; no new per-editor/global container is justified.

CP59 PaintDeviceActiveCopyCall already binds globals weakly with canonical
raw/control-block checks, qualification and native lifetime checks. Binding is
monotonic and rejects replacements. Its retained inner cleanup uses that owner.
However PaintDeviceActiveCopyValue currently holds only prefix/data state;
takeReturnedImage transfers no module provenance. A later caller could otherwise
supply a fresh qualified global object with identical SHA/RVA/alias numbers.
That would not prove native module identity.

There is also a production boundary that handoff alone cannot close:
PaintDeviceActiveCopySourceState is still an isolated source owner, with no
binding to the existing runtime brush source registry/module context. Its
qualification string is not proof of actual same-runtime origin. The isolated
fixture's source/global association remains a caller precondition. Runtime
integration must intern/bind the actual source value and canonical globals within
the same qualified live context; it may not duplicate brush flags into an
unrelated source owner. Preserve a source/runtime-origin guard until this is done.
The native trace proves which module symbol is used, not its current pointer,
value or an unobserved actual widget's source association.

## Exact next implementation contract

First assign the two CP59 helper files for this narrow handoff extension:

1. Add one private weak_ptr<PaintDeviceCleanupGlobals> origin to
   PaintDeviceActiveCopyValue. Keep its constructor private to the producer.
2. In the producer's InnerCleanup stage, after its retained inner cleanup returns
   using the existing bound owner, set outer value's weak origin to that EXACT
   owner before ActiveCopyReturned/takeReturnedImage can expose the value. Never
   derive the origin from counter aliases or instantiate a new global object.
3. Add `std::weak_ptr<PaintDeviceCleanupGlobals> cleanupGlobalsOwner() const noexcept;`
   as diagnostic/continuation access, or equivalent narrow consumer friendship.
   Do not add a setter, replacement argument or strong module lease. No native
   store/ref effect should accompany this host provenance assignment.
4. Returned value identity and prefix/data owner remain unchanged on transfer;
   diagnostic prefix shared_ptr leases still do not own native lifetime. Failed
   or premature take returns no token, and one token cannot be consumed twice.

Then an isolated consumer can take
`PaintDeviceNullImageFinishCall(std::unique_ptr<PaintDeviceActiveCopyValue>)`:

- Guard missing value, dead/claimed/wrong-class prefix state, missing/dead/foreign
  origin and unsupported non-null/unknown data. Origin checks establish modeled
  provenance; they are not extra native image reads or GUI-constructor effects.
- Inherit the exact weak origin, without independent module binding/replacement.
  Runtime death must block pending native continuation despite host leases.
- Read actual returned null data for hasAlpha; keep false and its negation private.
  Repeat outer QImage class store, then its separate null-data read/check.
- Resume one NEW outer PaintDevicePrefixCall on the SAME I1 owner. Only its
  TailReady permits one NEW PaintDeviceCleanupCall on inherited globals. Inner
  J's historical call cannot be reused. Keep warning, owner and cleanup guards.
- Only outer cleanup Returned ends I1's native lifetime and publishes opaque=true.
  Record once-only effects and preserve historical Returned on retries. Host
  abandonment marks native-owner loss, not native destructor success/ref release.

Public/editor integration is a later assignment: require the actual runtime
source/global relation above, preserve consumed brush-alpha state and pending
opacity, and replace CP56's projected outer prefix only once. Do not execute the
old image-helper destruction summary PLUS a duplicate reconstructed data/prefix
sequence. No QImage format/pixel, pixmap/cache conversion, nonnull active-copy,
warning, lazy/nonzero global cleanup or QWidget/application-wrapper gate closes
through this handoff extension.

## Focused reachable verification contract

Future tests must use CP59-produced unique values rather than a new fabricated
public value constructor or a completion flag:

- Exact same origin raw/control block before and after take, including frame/
  source host destruction while actual module and returned value remain live.
- Different module containers with equal file SHA/RVA/alias/value are not valid
  substitute owners; there is no consumer replacement seam to exploit.
- Weak provenance does not extend actual module life. Module expiry/release before
  consumer work blocks completion even if diagnostic host leases survive.
- Inner J and outer I1 are distinct, with separate prefix/cleanup frames. New
  outer getter/getter/comparison follows predicate/class/data effects, and no
  opaque result appears before outer destruction.
- Null/premature/second token take, released/claimed outer state, idempotent
  completion and abandonment do not repeat construction, refs or destruction.
- Two copies share one actual module/source but retain independent returned
  value owners and consumers. Completing one never finishes the other.

CP59 only returns after inner existing-counter0. Under its monotonic snapshots,
the same module cannot subsequently become unknown/null/nonzero by an observation
or replacement. Do not fabricate those outer test cases by substituting a new
owner; standalone cleanup tests already cover its guards. No new mutable global
transition/concurrency model follows from the static trace. A valid CP59 value
and live module can finish in one consumer resume. Release-active CHECK/REQUIRE
must evaluate setup, but this report authors/runs no tests.

## Evidence receipt

New raw artifact: analysis/resume62-editor-paint-owner-qtgui.asm, SHA-256
`10689dcb13af2864b43a021fb19a186605f51fa3b222e1479fa00e488f8cdd41`.
It is the concatenated unmodified DUMPBIN stdout for the following end-exclusive
original ranges, using `/disasm:bytes /range:0xSTART,0x(END-1)`:

| Range | Native-byte SHA-256 |
|---|---|
| 65146e30..65146ee8 | e1081d23f188772b27de7ca0711c1bcf8d7ceefbf235d826a4ef6bc634f1303e |
| 65148840..6514898e | 7d5683790552c0a5baf547f49e0618d24b551b5f03920fb8cc00d770d0b03f16 |

Tool: VS BuildTools14.44.35207/bin/Hostx64/x64/dumpbin.exe, file SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`,
banner14.44.35228.0. Bounds cover the retained normal unwind regions, not all
exception/concurrency behavior. Null/lazy registration and nonzero mutex/list
continuations visible in these dumps remain unimplemented guards.

Current CP59 header/source identity remains
`311847e0420c3738339954619a85104a47b2f9a7d305c3f36bf85d2aa3efbcd8` and
`0c216d2ced9c727a095f60dc52152f014a5794af151abac1e4d7a136c3f40290`.
Native value identity is resolved; actual runtime source/module binding and
current values remain qualified inputs, not facts supplied by this audit.
