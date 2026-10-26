# Style24 editor cleanup sequencing

Source is frozen for root review/build. The editor now withholds texture opacity
after the image-data boundary until the actual runtime's qualified
QPaintDevice cleanup tail returns. Unknown, null/lazy, nonzero and unavailable
global owners cannot enter `OpacityDecision` or the QWidget tail.

Owner: `editor300`. Only `editor_base.hpp`, `editor_base.cpp`,
`tests/mdt/test_editor_base.cpp` and this report were edited. No helper/runtime,
CMake/plan, build, original execution, GUI or mutable REA changes. Runtime owner
binding was supplied independently by `gradient_state`. Token measurement is
unavailable; tests are authored, not executed by this lane.

## Evidence and sequence

Read `resume53-editor-style24-next.md`,
`resume54-paint-device-cleanup.md`, and the independent
`resume54-style24-ownership-review.md` with their binary, bounds and ownership
qualifications. Original QtGui SHA-256 remains
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.

The retained `TextureImage` call still uses the actual selected shared brush
payload. On its bounded `Returned` result, editor saves the predicate privately
and commits `TextureImageDataBoundaryReached` once. It never resumes that helper
again while subsequent cleanup is pending, so metadata reads and data release
cannot replay.

`TextureImagePaintDevicePrefix` projects the returned temporary's source-proven
word+8=0 from its QPaintDevice constructor (`6513814c`). This is distinct from
the cached source image's painting word. The destructor reads that word at
`65138196` before installing QPaintDevice class identity at `651381a5`; the two
ordered editor effects reflect that sequence. The admitted metadata path does
not paint the returned temporary, so the nonzero warning branch is not admitted.

`TextureImageCleanup` obtains the canonical module owner only through
`WidgetRuntimeState::paintDeviceCleanupGlobals(actualWidget)`. No editor-local
global, counter or context substitute is created. Until that receiver has a
qualified runtime binding, the editor returns
`TextureImageCleanupGlobalsUnavailable` after the committed data/prefix boundary.
Once available, one retained `PaintDeviceCleanupCall` weakly binds it. A different
control-block owner is rejected; owner loss remains a pending helper guard.
Counter pointer/value refinement resumes the same helper frame. Only its exact
existing-counter-zero `Returned` emits `TextureImageCleanupReturned`, publishes
`TextureImageOpacityResolved`, and enters the existing `OpacityDecision`.

The resulting selected-path order is:

`image predicate/data release -> returned word0 read -> QPaintDevice class
projection -> runtime global getter/getter/exact0 comparison -> texture opacity
resolution -> optional widget bit17 write -> native-opacity helper -> QWidget tail`.

Repeated metadata/global observations perform no editor continuation. Repeated
pending or later raw120 resumes perform no second image release or cleanup call.
Two editor temporaries own two cleanup frames even when they share one image
data owner and one module counter. Typed image/counter aliases are independent.

## Exact remaining qualification

This task intentionally does **not** alter
`BrushTextureImageOpacityCall::Returned`, its `opaque` diagnostic or legacy
`ImageValueDestroyed` effect. Those remain the isolated helper's bounded
predicate/data-lifetime result, not a full native QImage destructor receipt.
Standalone helper callers still need the later cleanup boundary; only the
retained editor sequence now supplies and gates it. The private editor predicate
is not exposed as completed widget opacity during that interval.

The QImage destructor's earlier class-install/data-release prefix remains
represented by that existing helper summary; this increment adds the subsequent
QPaintDevice word/class projection and selected global tail. It does not create
native Qt vtable storage or establish all destructor/exception behavior. Source
cache ownership remains the admitted ordinary-copy profile from the review;
modeled temporary reference counts are not absolute Qt atomic counts or proof
that source image data was freed.

Null/lazy global creation and callback registration, every nonzero counter's
mutex/list continuation, warning/painting behavior, active-copy inner temporary,
pixmap/platform conversion, changed source ownership and exceptions remain
unclosed. Runtime/module profile observations must be actual and current; no
counter0 is inferred from image refs, DLL initializers or startup captures.
QWidget/font/post-event and application-wrapper/setup/public mode300 gates remain
independent. No new original runtime capture or full Open-Save-Close parity is
claimed by the selected tail or unit cases.

## Focused verification

Seven new cases bring the editor file to63 cases. They cover data release before
missing globals; late pointer/value qualification with one helper frame; null
images requiring the same cleanup; unknown/null/lazy and positive/negative
counter guards; true/false predicates and brush alpha0/255; two temporaries with
separate cleanup frames sharing typed owners; rejected owner rebinding and owner
loss; observation-only behavior; and host abandonment after data release.
Existing before-release abandonment and raw120 retry cases were retained and
extended with explicit qualified cleanup profiles/returned-tail assertions.
Prior successful image cases now supply counter0 explicitly in their isolated
fixture, rather than adding a default to production or fixture construction.

Static checks passed: exactly one `int main`, 63 unique `TEST_CASE` names,
balanced parentheses/brackets/braces after comment/string stripping, explicit
cleanup-helper include, direct whitespace/conflict checks and `git diff --check`.
These are source checks, not a compiler/test pass. Root owns focused/full
validation and any external fixture expectation updates.

## Frozen source

Paths under `source/apps/moproj2mst/`:

| File | SHA-256 |
|---|---|
| `include/m2m/engine/editor_base.hpp` | `034aaf93e0fe500640bfe05bff3aba5adaef33adc537eb5d1a0c8e57b46ea555` |
| `src/engine/editor_base.cpp` | `df83083ec1aaaf1a01da9484912e4fccc3cdaf22d55b1b2738870557eeae03b1` |
| `tests/mdt/test_editor_base.cpp` | `fc5bab4d10850dc5efb790c846b81cd407311491212b91e67ae6243192103465` |

Next: combined runtime/editor review and root validation, then choose the next
unclosed native cleanup/copy dependency from the retained evidence. No broader
constructor completion follows from this source freeze.
