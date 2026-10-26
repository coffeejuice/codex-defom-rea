# Canonical module handoff and isolated outer null-image finish

Source is frozen for root review/build. The actual CP59 returned value now
carries its producer's weak canonical module owner. A new consumer keeps the
null-image predicate private while finishing that same outer value's QImage
prefix, QPaintDevice prefix and separate global cleanup, then publishes the
image-branch opacity result. Public/runtime construction remains guarded.

Owner: editor300. Only the existing paint_device_active_copy.hpp/.cpp,
test_paint_device_active_copy.cpp and this report were edited. No CMake, runtime,
editor_base, other shared helper/plan, original execution, GUI or mutable REA
changes. No build/CTest/tests executed by this lane. Token usage is unavailable.

## Evidence and implemented handoff

Read analysis/resume62-editor-paint-owner.md, SHA-256
`0c977468b636a60ee253d83ac25502974b85cfb7e41c8e2219660c92ff847664`,
and its raw capture, together with resume60-paint-outer-next.md and CP59's
source/evidence qualifications. Original QtGui SHA-256 remains
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.

The native caller's I1 return storage is preserved through650b9a70's copy,
6511a610's getter,6511c18d's predicate and6511c1ac's destructor. Inner J is a
different value. Both destructors reach65148840 and65146e30, which read the same
QtGui module slot at RVA9cb858. No per-image global-owner field is accessed on
that path. The weak provenance below is host bookkeeping, not a Qt layout/ref
operation or a new original runtime observation.

PaintDeviceActiveCopyValue gains exactly one private weak module origin and a
read-only `cleanupGlobalsOwner()` accessor. The producer assigns the SAME bound
owner only after inner cleanup Returned and before ActiveCopyReturned/take can
expose the outer value. Host ownership transfer preserves existing prefix/data
identity and adds no native constructor, ref or effect. No setter, replacement
argument or strong module lease was added to the value or consumer.

The actual source/runtime-module relation remains an integration precondition:
CP59's isolated source owner is not bound to the existing canonical brush source
registry. The handoff preserves the already selected producer owner; it cannot
create the missing production association. No new source observation is invented,
and no public/editor path was enabled.

## Consumer API and exact sequence

The consumer is in the existing helper files, namespace m2m::engine:

`explicit PaintDeviceNullImageFinishCall(std::unique_ptr<PaintDeviceActiveCopyValue>);`

It exposes resume/report/effects, diagnostic valuePrefixState and weak
cleanupGlobalsOwner, plus prefixCall/cleanupCall diagnostics. It has no module
binding or replacement API. Existing consumers of the copy helper remain valid;
no C ABI or other existing interface changed.

Stages are ValueQualification, AlphaPredicate, QImageDestruction,
PaintDevicePrefix, Cleanup, Returned. Missing/dead value, unexpected/claimed class,
missing/dead/noncanonical module origin, unknown/non-null data and pending prefix/
cleanup remain guarded. Origin/lifetime validation is modeled bookkeeping, not
extra reached native image reads. The painting-word read remains inside the later
PaintDevicePrefixCall. Private-value construction still proves word0.

On the admitted null path, nine ordered consumer effects are:

1. PredicateImageDataRead:650aa340 reads actual I1+10.
2. AlphaPredicateReturnedFalse:650aa382..384, without format/flag/pixel reads.
3. OpacityNegated:6511c194 stores the negation privately, with report.opaque absent.
4. QImageClassInstalled:650b3322 repeats the reached QImage class store, even
   though its modeled enum already equals QImage.
5. QImageDataRead:650b3325 separately reads null; no deref/free/ref event.
6. PaintDevicePrefixTailReached after the retained SAME-I1 prefix call's
   word-read/base-class-store/tail effects in native order.
7. CleanupReturned after a NEW outer cleanup call's getter/getter/exact0 read
   on inherited canonical globals. Inner J's call receipt is not reused.
8. OuterImageDestroyed, marking I1 native-owner loss.
9. OpacityReturned, publishing the private true result only after destruction,
   matching6511c1db. Returned is then an idempotent historical receipt.

The unique value remains the consumer's lifetime responsibility. Host abandonment
invalidates its native owner without pretending native destruction succeeded.
Diagnostic prefix/global leases cannot keep released native owners callable.
Normal historical Returned remains stable after later module/source loss and
cannot authorize a new destruction. completeForPublicSave stays false.

The selected native image path has only cleanup bit4, so no extra pixmap
destruction or isQBitmap call is introduced. No outer image-data release is
fabricated for null data. Other texture/format/pixel, warning, lazy/nonzero global,
exception/concurrency or QWidget/application-wrapper behavior is not admitted.
The existing image helper's bounded Returned/ImageValueDestroyed summary and
CP56's projected outer prefix are deliberately unchanged pending actual-owner
editor integration, where duplicate effects must be avoided.

## Focused cases and source verification

Eleven new Release-active cases bring the existing focused file to32 cases,
with299 CHECK and58 REQUIRE conditions. They cover:

- Origin committed after inner return, exact raw/control-block identity and
  unchanged actual prefix/data owner through unique transfer.
- Source words1/8000/ffff; exact nine consumer effects and nested prefix/cleanup
  effects; two distinct temporaries, private result before execution and final
  null-image result only after outer lifetime ends.
- Null/premature tokens; source/copy host-frame destruction after valid transfer
  while actual module remains live; native module release/expiry despite leases.
- Equal SHA/RVA/alias/value in another module container does not substitute for
  inherited origin; separate outer counter call uses the original actual owner.
- Released value and already claimed outer prefix do not replay destruction;
  two returned values share canonical source/module but have independent consumers.
- Abandonment without fake native success and stable historical completion after
  source/module release, with no replay of prefix/global/image-data operations.

No artificial mid-destructor yield, fabricated returned-value constructor or new
success flag was added for testing. CP59 only returns after inner counter0;
under the current monotonic snapshot contract its same globals cannot then turn
unknown/null/nonzero or be replaced. Standalone cleanup tests already cover those
guards; this consumer's reachable module-loss guards remain tested without
substituting another owner. Valid current CP59 values can finish in one resume.

The file still uses testkit/check.hpp. CHECK/REQUIRE evaluate conditions and setup
in Release without assert/NDEBUG elision. Static checks passed for direct
whitespace/conflict markers, balanced delimiters after comment/string stripping,
32 unique cases, exactly one main, no assert, and macro argument scans. A manual
source pass also corrected a mixed-type multi-auto declaration before freeze.
These are source checks, not compiler or test results. Root owns focused/full
validation; no build/CTest command was run here.

## Frozen files

Paths under source/apps/moproj2mst/:

| File | SHA-256 |
|---|---|
| include/m2m/engine/paint_device_active_copy.hpp | 69c453decfbd4112cc0da488007399bd904e7bda26486bba74190a3841d4261c |
| src/engine/paint_device_active_copy.cpp | 03d4168348a871214ac050b192522a1c6799b0b1e60925770513d3b7c0466eb9 |
| tests/mdt/test_paint_device_active_copy.cpp | 28e742f619917fc8a9266cf0fd60d26a6c313fe52281f19363d0a38d7772387f |

Next dependency: root validates this existing focused target, then assigns actual
source/module runtime interning and any outer editor hook. This selected finish
is an isolated value operation; GUI constructor, native UI reachability, public
mode300 and full converter/Open-Save-Close fidelity remain unestablished.
