# Canonical texture source-view helper bridge

Source is frozen for root build/test/archive. The active-copy helper now supports
a delegated source view of the actual BrushTextureImageState and inherits its
weak canonical module origin. No word/data snapshot is copied into a second
source observer. Runtime registry ownership/join and public editor admission
remain guarded dependencies.

Owner: editor300. Only brush_texture_image.hpp, paint_device_active_copy.hpp/.cpp,
test_paint_device_active_copy.cpp and the two CP63 reports were written. No
view_manager/editor_base/CMake or other shared source, build/CTest, original
execution, GUI or mutable REA changes. Token usage is unavailable. The source/test
lane is frozen until root explicitly releases it.

## Evidence and architecture

Read analysis/resume63-editor-origin-binding.md, SHA-256
`0d1998677db7efcb7660a3d8caf4ed45dca82e98f156da743e9f7a16f087dc9a`.
It traces the existing runtime brush/payload registry, CP62 actual I1/J identities,
QtGui module slot RVA9cb858 and public Session mode300 guard. Original QtGui
SHA-256 remains
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
No new native/runtime observation or public call reachability is claimed.

## Implemented bridge

BrushTextureImageState exposes two const getters over its existing fields:
imageDataOwner() and imagePaintingActiveWord(). Neither observes, allocates a
native value, increments image references or changes the current source.

The exact factory is:

`static shared_ptr<PaintDeviceActiveCopySourceState> fromTextureImageSource(shared_ptr<const BrushTextureImageState>, const shared_ptr<PaintDeviceCleanupGlobals>&, string qualification);`

It requires actual payload, nonempty qualification and a live qualified canonical
module owner (same raw/control-block identity against weak_from_this). It creates
a VIEW, with one strong diagnostic payload lease and weak module origin. The
strong lease keeps returned metadata references stable; native callability still
ends when the view/module's native owner is released or module expires.

Accessors textureImageSource(), cleanupGlobalsOwner() and textureImageBound()
identify that binding. paintingWord()/imageData() delegate directly to the actual
payload getters in bound mode. The bound view's standalone observe method always
rejects, including equal-looking fields; all actual metadata refinement stays
on the canonical payload's existing observer. There is no snapshot fan-out or
second source-data cache.

PaintDeviceActiveCopyCall seeds its weak globals from a bound view at construction
and fixes that binding. Explicit bindCleanupGlobals rejects any foreign origin
or dead view, even when another container has equal module SHA/RVA/alias/value.
Standalone isolated mode retains its prior explicit monotonic binding contract.
CP62's existing producer handoff/unique returned value/null-image consumer then
preserve this exact same origin without new native store/ref effects.

The source view remains an internal helper constructor. It cannot prove that
a supplied payload belongs to a particular runtime registry. Future callers
must use the runtime's canonical interner/binding API, not call this factory on
copied field values and claim an actual native source association.

## Focused cases and verification

Eight new Release-active cases bring test_paint_device_active_copy.cpp to40 cases,
374 CHECK and76 REQUIRE conditions. New cases cover:

- Missing payload/qualification/module, unqualified/rewrapped/released module
  rejection; exact actual payload control block and unknown fields preserved.
- Late painting/data refinement through the SAME payload after precise pending
  guards, no observation-triggered continuation, direct field-reference identity
  and exact producer/outer-consumer module provenance through normal return.
- Bound standalone observe rejection and atomic rejection of invalid canonical
  payload metadata before any fields change.
- Nonnull actual data-owner identity and active-nonnull guard, without temporary
  image refs or pixel work.
- Producer origin fixed before first native effect; same origin idempotent and
  same-profile foreign owner rejected.
- Diagnostic payload lease remaining readable while weak module expiry blocks
  native continuation; explicit view release cannot resume/rebind/reobserve.
- Standalone mode preserving its prior missing-globals guard and explicit binding.

The existing32 producer/consumer/ownership cases remain in the same file. Setup
uses REQUIRE and checks use Testkit macros that remain active under NDEBUG.
Static checks passed: whitespace/conflict markers, balanced delimiters after
comment/string stripping,40 unique cases, one main, no assert and CHECK/REQUIRE
argument scans. No compiler or runtime test result is claimed by this lane.

Existing target verified by reading build/candidate/CTestTestfile.cmake:
`m2m.mdt.paint_device_active_copy`, executable/build target
`m2m_mdt_paint_device_active_copy`. Root owns focused and combined validation.
No CMake/build command was executed here and no new target wiring is needed for
these existing files.

## Frozen source

Paths under source/apps/moproj2mst/:

| File | SHA-256 |
|---|---|
| include/m2m/engine/brush_texture_image.hpp | 103208d46b8aba88a1510b34469f54641f47e2bf610848a9248726102e80aa52 |
| include/m2m/engine/paint_device_active_copy.hpp | 9cbde173bf1bd542bde9ecf2738489643ec857af7d021f2ca7e4de07be937a4e |
| src/engine/paint_device_active_copy.cpp | 06afe87c8c1f1a79fcf4568e5a4ede0ade706a03129b948ce41f40fc7ef5223c |
| tests/mdt/test_paint_device_active_copy.cpp | b1228bb849f7acc0a2235da3d17a58cbddd76e020bf9ac6b98b78f6df1d39508 |

## Exact unresolved registry/public dependency

view_manager.hpp/.cpp still need an explicit bindTextureImageCopySource receiver/
brush API and cache that interns/owns one view per actual brush payload. It must
validate same live canonical private widget/runtime, actual style24 registry brush,
existing texture payload and this receiver's explicit canonical module grant.
It must reuse the same view for shared brushes across qualified widgets, retain
distinct views for distinct brush values and reject mismatched/dead/replaced
payload/module identity before publishing registry state. The helper factory
alone supplies none of these registry grants. Suggested separate focused file:
tests/mdt/test_widget_texture_copy_origin_binding.cpp, owned with that runtime seam.

No actual selected-brush editor hook was made. Public Session mode300 still
retains PrivatePaletteUnavailable and not_ported6504abb0 because post-Open private
aliases are not bridged from startup; later ApplicationWrappersUnrecovered and
constructor/setup guards remain. Current active-null UI reachability, warning,
nonnull active copy, pixmap/cache conversion, lazy/nonzero global cleanup and
full Open-Save-Close fidelity remain unestablished. Root can validate this bridge
while that registry/public work stays explicitly guarded.
