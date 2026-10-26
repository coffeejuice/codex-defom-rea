# Selected editor texture source: bounded active-copy producer prefix

Source is frozen for root build/test/archive. The editor's selected style24 image
path now binds the actual CP64 source view at its reached active-copy dependency,
retains one copy producer, and holds the actual returned outer value at an
explicit outer-finish guard. It does not run the outer predicate/destructor or
publish opacity. Public/private-palette mode300 refusal remains unchanged.

Owner: editor300. Only editor_base.hpp/.cpp, test_editor_base.cpp and this report
were edited. No view_manager, texture/paint helper, public/session loader, CMake,
plan, original execution, GUI or mutable REA changes. No build/CTest/tests run
in this lane. Token usage is unavailable. Root confirmed the bounded producer
endpoint and outer-finish guard before implementation; source is frozen pending
root release.

## Native caller and actual selected source

Read CP63 architecture/implementation and CP64 runtime interner receipts,
with CP62's native returned-value/global-origin audit. Original QtGui SHA-256
remains `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Existing bounded captures qualify:

- QWidget6506e6f2..721 chooses its CURRENT palette group, Window role10 and actual
  shared brush, then enters isOpaque6511c090 for a nonzero-style window brush.
- Entered isOpaque consumes brush alpha before style dispatch, including style24.
  Its image-selected branch6511c170 calls textureImage6511a610; cached-source
  helper6511a1a0 checks null/cache/conversion dependencies before650b9a70 copy.
- Active source word selects the copy branch with a distinct outer I1 and inner J.
  CP59's null-data producer closes the constructor/rectangle/null-assignment/inner
  cleanup prefix and returns I1. The outer hasAlpha/destructor at6511c18d/c1ac
  occur later, before any return to QWidget's opaque-bit decision.

Current editor code already captures opacityBrush_ after actual current-group
selection and consumes alpha once. The original BrushTextureImageOpacityCall
holds the actual selected brush payload and enforces selection/getter/cache
order. Its ImageCopy/ActiveImageCopyUnrecovered guard occurs before it retains
or copies image data and has no native copy/ref completion effect. That precise
reached edge is the new handoff; no unobserved post-Open source fields are supplied.

## New retained stages and guards

TextureImage keeps its ordinary path unchanged. Only a retained getter report
with stage ImageCopy and ActiveImageCopyUnrecovered enters TextureImageActiveSource.
Unknown painting word, pixmap-selected path, unknown cache presence and conversion
continue to hold their existing earlier dependency; they cannot bypass the getter.
The old active guard's field inspection is qualification, not a second claimed
native source-word-read effect. Actual covered copy-constructor/source-word effects
belong to the new retained producer.

TextureImageActiveSource checks the captured brush is still actual style24 and
its payload matches the originally bound payload/control block. It calls
WidgetRuntimeState::bindTextureImageCopySource(actualWidget, capturedBrushAlias),
then validates returned view/payload and this receiver's exact current canonical
globals. Unsupported qualification remains TextureImageActiveSourceUnavailable;
other errors propagate. No editor-local source/module or copied field snapshot
is created. TextureImageActiveSourceBound is host provenance bookkeeping, not a
new native Qt field/ref operation.

TextureImageActiveCopy holds one PaintDeviceActiveCopyCall. Before every pending
continuation it checks actual source identity/lifetime and same current receiver
module origin. Unknown/null/nonzero global or active-nonnull pixel dependencies
remain nested helper guards, reported as TextureImageActiveCopyPending. Late
legitimate qualification resumes the same producer and helper cursor. Alpha and
the original getter are not re-executed. An active nonnull image retains the
source-qualified constructor/rectangle/data-read prefix but no pixel or image
reference operations.

On producer Returned, the editor takes its unique actual I1 exactly once and
records TextureImageActiveCopyReturned. Stage TextureImageActiveFinish holds it
live as QImage, constructor word0, with inherited exact weak globals. Missing/
released value reports TextureImageActiveValueUnavailable; current module-origin
loss reports TextureImageActiveSourceUnavailable. A returned value can survive
source-view release, because source B+78 and actual I1 have independent lifetimes.

The admitted endpoint is TextureImageActiveFinishUnrecovered. No null-image
predicate, QImage destructor, outer prefix/cleanup, opaque assignment or QWidget
tail is run here. In particular it does not enter the ordinary path's bounded
ImageValueDestroyed/data-boundary projection or reuse inner J's cleanup receipt.
The original image helper stays at its active dependency with no retained data.
Host factory abandonment ends the held I1 lifetime without claiming native outer
destruction success. No data release, getter, copy or provenance effect replays.

## Focused regressions

Seven new cases bring test_editor_base.cpp to70 cases,716 CHECK and57 REQUIRE
conditions. The earlier no-module active-painting case was renamed/updated to
expect the precise active-source qualification guard; its no-copy/ref assertions
remain. New cases cover:

1. Alpha0/255 consumed once, absent module prevents producer creation, late exact
   module enables one producer and retains actual outer token at finish guard;
   ordinary cleanup/data-prefix/opacity/tail effects remain absent.
2. Qualified module with unknown pointer, then unknown counter, then0: same
   producer/inner cleanup, exact getter/comparison counts and no earlier replay.
3. Active nonnull data: constructor/rectangle prefix then pixel guard, zero
   image-data temp refs and no inner cleanup or returned value.
4. Actual disabled/current palette group1 selects brush24 rather than a decoy
   brush23, preserving captured source wordffff and source/module ownership.
5. Unknown source word and conversion cache stay before active binding; late
   word qualification consumes alpha once and enters the qualified producer.
6. Source-view release after copy leaves I1 valid; module loss blocks later
   continuation, and factory abandonment invalidates the saved value owner.
7. Two widgets sharing one source/runtime require each current module grant,
   reuse the exact source view but own separate I1 values/cursors; abandonment
   of one does not finish/release the other.

These isolated explicit profiles establish no public texture/UI reachability.
Setup/checks use Release-active testkit CHECK/REQUIRE without assert elision.
Static checks passed: whitespace/conflict markers, balanced delimiters after
comment/string stripping,70 unique case names, one main, no assert and macro
argument scans. Explicit m2m/common.hpp supplies Error/unsupported definitions.
No compiler/test pass is claimed. Root owns m2m.mdt.editor_base focused and
combined validation, and any external fixture changes found by those checks.

## Frozen source and next dependency

Paths under source/apps/moproj2mst/:

| File | SHA-256 |
|---|---|
| include/m2m/engine/editor_base.hpp | ecbf786c000a6ad904ff2dfd5db90c34c2acea910801b24f85627aba5e5855aa |
| src/engine/editor_base.cpp | ce2adbdc400c7759d3e2887ca4c52e373068bb4238e2f3d05eaf97c16dea25d9 |
| tests/mdt/test_editor_base.cpp | 0a2b64d661d8e7e5f624f2efac6ba3403e15911fd75a92198fcdb196ff779363 |

Next bounded code edge is moving the retained actual I1 once into the already
qualified PaintDeviceNullImageFinishCall, preserving the same current module
origin and withholding opacity until its outer destruction returns. That hook
is not implemented in this increment. It must bypass, rather than duplicate,
the ordinary image data/prefix summary and use separate inner/outer frames.

Public ConversionSession still has no post-Open private/default value aliases;
beginEditor300Factory requires PrivatePaletteUnavailable and loader retains
not_ported6504abb0. Later constructor/wrapper/setup gates, active-nonnull pixel,
pixmap/conversion, warning and lazy/nonzero cleanup behavior remain guarded.
No mode300/public-save or fresh Open-Save-Close completion follows from this prefix.
