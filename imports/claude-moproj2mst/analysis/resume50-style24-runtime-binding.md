# Bind the image opacity helper to the live palette brush

The next safe increment is an internal shared-runtime binding for the existing
style24 **image-selected, no-conversion, inactive-copy** helper. It can advance
only when the actual palette-selected brush has qualified texture state. The
retained original probe observed style1, so there is no production style24 cache
observation to populate that binding yet. Keep pixmap, conversion, active-copy
and public mode300 guards.

Owner/reviewer: `gradient_state`. Exclusive write: this report. No source edits,
builds, new runtime captures, GUI control or mutable REA. This is a static timing
and ownership audit, not an original parity claim. Tokens unavailable.

## Current source and qualified evidence

Reviewed current files, SHA-256:

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `a3c7cff4a57a51b84c8cf4dd17999f8790c20e9448aada9eae70bd8456f4175a` |
| `source/apps/moproj2mst/src/engine/view_manager.cpp` | `69e4b6f1e88644502abb047824db88bab7d511d7dc76c3f3943986f1d0ea66b7` |
| `source/apps/moproj2mst/include/m2m/engine/editor_base.hpp` | `bd3506fd3fef4776083e04ee5e922899884c02552d135038c40aa0985db5f407` |
| `source/apps/moproj2mst/src/engine/editor_base.cpp` | `7ee9165d16dd0351024bead83e219c9a4c9fb1590a1bdbc955033a39c190f2bd` |
| `source/apps/moproj2mst/include/m2m/engine/brush_texture_image.hpp` | `1d11f86a79eb36573e855f9f2ea8e4cac17e8a95b669a2d44f0696d48a4348f0` |
| `source/apps/moproj2mst/src/engine/brush_texture_image.cpp` | `0c6e36bfb02695db032fe07dd55cbc909898efe1acf99769285c78db0b8e7bb3` |

Read native behavior and source qualifications in
`analysis/resume48-brush-style24-contract.md`, its unique bounded native asm,
`analysis/wave3-editor-parentless-opacity-static.json`, and
`analysis/wave3-editor-probe004-palette-binding-contract.json`. The original
QtGui4_64.dll identity remains SHA-256
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.

The orchestrator's existing `analysis/resume49-combined-full3-tests.json` records
`m2m.mdt.brush_texture_image` passed without logged gaps/differences/truncation;
its referenced log ends `0 of 13 cases failed`. This audit did not rerun tests.
Those focused helper tests do not supply actual widget/cache observations or
exercise editor binding.

## Actual state available now

`WidgetRuntimeState::Values` interns brush data in its own alias map.
`WidgetPaletteDataState::brushAt(group, role)` returns the actual shared brush
owner. `WidgetBrushDataState` currently stores only alias, optional style and
optional brush alpha. Neither the startup observation path nor
`observePrivateConstructionInputs` currently binds texture selection, cached
image value, cached pixmap presence or image-data metadata. The helper's state
therefore has no path into the live widget brush.

At `EditorBaseStage::PaletteGroup`, the actual palette's group word is assigned.
`BrushOpacity` obtains the same palette/group role10 brush; for an entered
isOpaque call it consumes brush alpha, retains the brush reference in
`opacityBrush_`, and commits `BrushAlphaConsumed` once. `BrushStyleDispatch`
still guards style24. It must not refetch another palette slot or fabricate a
helper source from test defaults when this guard is replaced.

The probe004 qualification identifies actual palette data `0x3e3edb0`, role10
brush slot `0x3e3ee08`, and brush data `0x4618db0`, style1/alpha255, for that
one qualified frame. `admin/prepare_editor_probe004.py` reads only brush data
pointer, style, alpha and QColor16 at this hook. Neither that capture nor its
solid-brush predicate result observes fields at B+70/B+80/B+88/B+90. Reading
those union fields from a solid brush would not qualify a texture instance.
The captured raw QColor16 is unrelated to the embedded QImage/cache region.

## Smallest shared binding

Keep one optional `BrushTextureImageState` payload **on each actual
WidgetBrushDataState**, corresponding to that brush's embedded image value and
selection/cache fields. No separate editor-owned source state, duplicate palette
or copied source image should be constructed. An accessor can expose a const
shared payload to the helper while runtime observation code owns its refinement.

Add one runtime-interned map for actual `BrushTextureImageDataState` owners,
keyed by a dedicated positive image-data alias. Distinct brush data may own
distinct embedded image values pointing to the same image-data owner. Multiple
palette slots referring to the same brush must see the identical texture payload.
The runtime registry's host shared_ptr references do not count as native image
value references.

An internal observation envelope needs only:

- Actual brush alias, texture-selection byte knowledge, cached-pixmap pointer
  presence knowledge and embedded image painting-active word.
- Embedded image-data pointer knowledge, represented for example by optional
  image-data alias: absent means unknown, explicit0 means known null, positive
  means lookup in this runtime's dedicated image-data registry.
- Dedicated image-data observations with alias, optional native format and
  optional native ID+58 mask4.

The adapter resolves aliases itself into the helper's shared objects. Do not
accept a caller-provided helper shared_ptr as proof that it belongs to this
runtime. Reuse existing live-widget/component/runtime and qualification checks;
retain the same observation qualification across retries. Reject unknown aliases,
foreign owners, conflicting metadata/control blocks, and attempted source-owner
replacement before changing existing brush or image owners. Stage new aliases
and validate the whole supplied envelope before committing any shared mutation;
calling individual `observe` methods sequentially would otherwise allow an early
metadata refinement to survive a later conflicting image binding.

Both startup and private-construction input paths currently populate the same
runtime brush registry. They must either route these new observations through
one shared validator/interning path or explicitly guard the path not yet handled;
do not create separate image owners for the same native data across the two paths.

## Exact helper entry and continuation timing

1. Leave palette-group selection and the existing alpha consumption where they
   are. A missing brush alpha blocks before any style24 cache/image read. Alpha0
   and alpha255 both enter the same texture path after that consumed read.
2. In style24 dispatch on the **retained same brush**, create exactly one
   `BrushTextureImageOpacityCall` bound to its actual shared texture payload.
   Keep that call in the editor's constructor frame across retries, or give it
   an explicit dedicated cursor. Missing payload stays pending after the consumed
   alpha; do not recreate the helper after it has copied an image.
3. The helper first reads B+90. Unknown blocks; nonzero keeps the pixmap guard.
   Zero proceeds to QImage nullness from B+88. With a nonnull cached image,
   B+70 is not read. With a null image, known-null B+70 admits the no-conversion
   path; unknown B+70 blocks; nonnull B+70 keeps the conversion guard.
4. Source QImage painting-active word at B+80 is read next, even when its ID is
   null. Unknown blocks; nonzero keeps the active-copy guard; zero creates the
   returned QImage value. A nonnull ID gains one modeled temporary value ref.
5. Null ID returns opaque=true without format or flag reads. Nonnull ID reads
   format first. Formats5/6/8/10/12/15 yield opaque=false without reading ID+58.
   All other formats read ID+58 mask4 **before** comparisons1/2/3; unknown mask
   blocks even for a format where its value cannot affect the result. The copied
   image and its temporary ref remain alive while that metadata read is pending.
6. The helper destroys its returned QImage and releases its ref before reporting
   completed opacity. Only then emit the editor's `PaletteOpacityResolved`,
   compare/change actual widget opacity bit17, and enter the existing
   `OpacityEffect`/native raw120 cursor. Neither a metadata retry nor a later
   native-window guard may replay image copying or release.

This subset performs no image/pixmap cache writes. The current monotonic helper
state cannot stand in for future fromImage/toImage cache mutation: cache-miss
branches must remain guarded until those source mutations and lifetime transitions
are separately represented.

## Minimum missing original observation

A future qualifying receipt must bind the actual fresh-project editor/widget,
palette data/group/role10 slot, QBrush value and B pointer at the reached
constructor frame. It must establish style24 and consumed B+0d brush alpha,
then capture only the reached image-route fields:

| Field | Minimum evidence for this subset |
|---|---|
| B+90 | byte0, establishing image-selected route |
| B+88 | cached image-data pointer: explicit null or actual ID identity |
| B+70 | pointer null only if B+88 is null; otherwise unconsumed |
| B+80 | embedded QImage painting-active word0 |
| ID+30 | native format when ID nonnull |
| ID+58 mask4 | read only when native format is outside5/6/8/10/12/15 |

Observe the textureImage call's returned QImage identity/data owner and release
boundary to validate that the copy retains/releases the same ID before the
outer opacity decision. A paired before/after source snapshot establishes that
the admitted no-conversion route preserved its cache. Qualified current native
absolute ref counts can strengthen validation but must not be inferred from
the static file or host registry.

No retained receipt currently meets that style24 frame requirement. This report
does not authorize or perform a new GUI/capture session. A new observation is
needed for actual-instance acceptance; source-qualified isolated binding/tests
can proceed without inventing production values.

## Required focused integration checks

Cover actual runtime slot/brush/image identity sharing and foreign/conflicting
observation rejection; both-null and nonnull-ID ordinary-copy routes; no required
cached-pixmap observation when image is already nonnull; alpha-before-cache,
nullness-before-cache, copy-before-format and format-before-flag ordering; held
temporary refs on metadata guards and single release on completion; retries
through the later native-opacity guard without helper replay. Test image
selection/pixmap selection, conversion and active-copy guards separately. Ensure
the public constructor/mode300 still refuses admission.

Decision: source supports the narrow shared binding above. The current live
runtime lacks the fields, and current retained original observations cannot seed
them. Treat helper test success, runtime binding and actual-instance parity as
three distinct evidence states.
