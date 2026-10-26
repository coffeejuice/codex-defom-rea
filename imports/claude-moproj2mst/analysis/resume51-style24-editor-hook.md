# Next style24 image hook after the gradient cursor integration

Add one image-helper cursor between style dispatch and the existing
`OpacityDecision`. Its input must be the retained actual brush's runtime-bound
image payload, and its completion must follow destruction of the temporary
QImage. The current helper already supplies that destruction boundary. No
gradient scan stage, shared-null symbol, or second image-value copy is needed.

Owner: `gradient_state`. Exclusive write: this file. No source edits, builds,
original execution, GUI control or mutable REA. `editor300` confirmed its
checkpoint50 cursor is frozen and is independently investigating style16 in
`analysis/resume51-editor-style-next.md`; this report owns the detailed style24
hook. Token measurement unavailable.

## Evidence and current source

Read the original behavior/qualification in
`analysis/resume48-brush-style24-contract.md`, its uniquely named bounded native
asm, and `analysis/resume50-style24-runtime-binding.md`. The latter remains the
full native owner/field/capture contract. The original DLL SHA-256 is
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
No new original observations were collected. In particular, probe004's solid
brush does not qualify a style24 image or cache.

Reviewed source SHA-256:

| File under `source/apps/moproj2mst/` | SHA-256 |
|---|---|
| `include/m2m/engine/editor_base.hpp` | `a455853a2c808deb8247effe98f0e63872c2da700c0961e5511332e4eec92277` |
| `src/engine/editor_base.cpp` | `43a7dff9f800a8cbfb248045cea2412f35332d4360943c787a6651abfbec281e` |
| `include/m2m/engine/view_manager.hpp` | `cf76337e9f390a321a02031e4c625368dde95315c7acfe62a01c7248a4d8ce3c` |
| `src/engine/view_manager.cpp` | `d520ad1fb30bed0ed10f6e579d4d021466b488ff282b5b333863496458004976` |
| `include/m2m/engine/brush_texture_image.hpp` | `1d11f86a79eb36573e855f9f2ea8e4cac17e8a95b669a2d44f0696d48a4348f0` |
| `src/engine/brush_texture_image.cpp` | `0c6e36bfb02695db032fe07dd55cbc909898efe1acf99769285c78db0b8e7bb3` |

## Exact editor hook

Proposed internal additions, names illustrative:

- `EditorBaseStage::TextureImage`, entered only for style24 after
  `BrushAlphaConsumed` in the existing `BrushStyleDispatch`.
- One `unique_ptr<BrushTextureImageOpacityCall>` held by the same
  `EditorBaseConstruction` frame, plus a const diagnostic accessor.
- Issues distinguishing missing actual payload from a pending helper, such as
  `TextureImageSourceUnavailable` and `TextureImagePending`. The helper report
  provides the precise missing read or unsupported branch; do not replace its
  diagnostic with a fabricated desired opacity.

At `TextureImage`, create the helper only when `opacityBrush_.lock()` resolves
the same retained actual brush and that brush exposes its shared texture payload.
If the payload is absent, leave the editor pending after the alpha read and do
not create a placeholder editor-owned source. A later observation can bind the
payload, after which the same editor frame constructs its helper once.

Once created, keep the helper and its source payload identity through retries.
Metadata refinement occurs **inside that same state**. There is no counterpart
to the gradient's late `bindSharedNull`: the image route has no global symbol
dependency. Replacing the payload after helper creation must be rejected by the
runtime observation adapter, not hidden by reconstructing the call.

Resume the helper. While its stage is not `BrushTextureImageStage::Returned`,
return the editor's pending-helper issue without touching `opacityDesired_`,
widget opacity, constructor tail, or already-consumed prefix effects. On
`Returned`, require `report.opaque.has_value()` and assign that value to the
existing `opacityDesired_`; transition directly to `OpacityDecision`.

The helper's normal completion releases/destroys its returned QImage before it
publishes the result. The editor must **not** copy/destroy another QImage or
release its data again. An optional single `TextureImageOpacityResolved` editor
effect can mark this transition; helper effects are the native image read/ref
ledger. The shared `OpacityDecision` then emits `PaletteOpacityResolved`, compares
and updates bit17 if needed, and continues through the existing `OpacityEffect`
raw120 guard and `SharedTail`. Later native-helper suspension must not return to
the image cursor or replay copying/release.

Keep style16 on its radial guard. Preserve source-only empty-gradient handling
for15/17. Public factory/setup/Save guards remain independent of image success.

## Runtime ownership and atomic observation precondition

Reuse the pattern of `observeGradientInputs` for a dedicated, narrowly scoped
private-constructor image observation entry point. It must require the same live
widget, matching control block/component owner/runtime, nonempty qualification
equal to that widget's existing private-input qualification, and existing
style24 brush aliases. This avoids expanding generic startup observation
structures just to support this one continuation. Startup-created brush aliases
can still be refined later through the dedicated qualified entry point.

The runtime owns a dedicated positive image-data alias registry. Each actual
brush owns one optional `BrushTextureImageState` payload. Its embedded image
value references the interned data owner or an explicitly known null pointer.
Multiple palettes sharing a brush share its payload; distinct brush payloads may
reference the same image-data owner. Registry/diagnostic shared_ptr leases are
not native QImage value references.

The external observation envelope uses aliases and primitive knowledge, not
caller-created helper shared_ptr objects. Minimum fields are receiver,
qualification, image-data entries `{alias, format?, alphaFlag?}`, and brush image
entries `{brushAlias, pixmapSelected?, cachedPixmapPresent?, paintingWord?,
imageDataAlias?}`. Disengaged image alias means unknown; explicit0 means known
null; positive resolves only in this runtime's registry.

**Whole-envelope preflight is required before refining any existing owners.**
The helper currently exposes only mutating `observe` and private metadata fields.
A small nonmutating `canObserve(...) const noexcept` for both data/source state,
or equivalent detached validation records, is therefore a shared-helper adapter
dependency. Validate each prospective observation and every brush/data link,
including duplicates, wrong style, missing alias and attempted owner replacement,
then commit. Sequentially mutating ID metadata before discovering a later bad
brush link would violate the already established rejection-without-partial-merge
contract. New registry objects can be staged off to the side until preflight
passes. The editor lane must not solve this by bypassing shared validation.

Observations themselves must never call the editor resume method. A test that
checks observation-only behavior must inspect the existing base report directly.

Unlike the gradient shared-null profile, image data is actual brush-owned state,
not a symbol synthesized from a receiver-specific header qualification. A second
live widget sharing that actual brush can read already-bound source/cache data
without inventing a second owner or requiring a fake image-symbol qualification.
Any diagnostic runtime alias lookup may still require a qualified live receiver;
that access rule is separate from the shared brush's actual data ownership.

## Ordered reads and held references

| Reached condition | Next native/helper action and boundary |
|---|---|
| style24, brush alpha unavailable | Stay before texture dispatch; no helper/cache reads |
| entered style24 | B+90 selection read; unknown pending, nonzero pixmap guard |
| B+90 zero | B+88 cached image-data nullness read |
| ID nonnull | Skip B+70 lookup entirely |
| ID null | Read B+70; unknown pending, nonnull conversion guard, null continue |
| selected embedded image | Read B+80 painting word; unknown pending, nonzero active-copy guard |
| painting word0 | Copy returned image; nonnull ID gains exactly one modeled temporary ref |
| copied ID null | Complete opaque=true with no format/flag reads, destroy null image value |
| copied ID nonnull | Read ID+30 native format |
| format5/6/8/10/12/15 | Complete opaque=false without ID+58 read |
| every other format | Read ID+58 mask4 before checking formats1/2/3; unknown flag holds copied image/ref pending |
| complete metadata predicate | Release temporary ID ref and destroy QImage, then expose opacity |

Brush alpha's comparison is consumed but does not determine texture opacity.
The subset reads no image pixels, color-table entries or RGB values. Known null
and unknown pointer are different states. Cache misses and active-copy routes
remain guards; monotonic observations cannot pretend those source mutations
have already happened.

## Focused tests before editor admission

1. Runtime binding: require actual live receiver/qualification; reject foreign
   runtime/control blocks, duplicate/wrong aliases, style1/16 links and source
   pointer replacement. A late bad link must leave all earlier data refinements
   and newly staged aliases uncommitted.
2. Sharing: same brush across palette values exposes the same payload; distinct
   brushes can share ID. Two editor calls pending on missing metadata must hold
   two separate temporary value refs on that ID; completing/destroying one must
   leave the other's ref intact.
3. Missing alpha: with a fully bound image payload, the editor still stops before
   helper creation until brush alpha is qualified. Both alpha0/255 then use the
   same image metadata result, and `BrushAlphaConsumed` appears once.
4. Missing payload then late payload: initial `TextureImageSourceUnavailable`;
   observing state does not change the base report/effects; explicit resume
   creates one frame. Later format/flag refinement resumes that same frame and
   keeps source payload/data identities.
5. Null route: image known null, cached pixmap unknown -> cache-presence guard
   after one nullness read; cached pixmap known null plus painting word0 returns
   opaque=true with no ID retain/format read. Painting unknown/nonzero stays
   pending/active-copy guard even with null ID.
6. Nonnull route: omit cached-pixmap observation to prove it is skipped. Direct
   alpha formats return false without an alpha-flag observation. Other formats
   remain pending until the flag read is qualified, even when their eventual
   result is independent of that flag.
7. Cleanup/order: helper release and value destruction precede
   `PaletteOpacityResolved` and any bit17 write. Changed opacity with unknown or
   true raw120 reaches `NativeOpacityUnrecovered` after cleanup; retries do not
   copy or retain again, release twice, or replay group/QObject/alpha effects.
   Equal-opacity paths skip the native helper as before.
8. Guards: pixmap-selected state never reaches QImage copy; known null image plus
   cached pixmap stays behind conversion; painting word!=0 stays behind deep
   copy; public setup/factory/mode300 remains refused. Update fixtures' expected
   initial stages based on whether their shared brush already has a payload.

These are executable prerequisites for the next implementation, not newly run
tests. A future original style24 receipt is still required for actual-instance
acceptance; it does not prevent source-qualified internal binding/test work now.
