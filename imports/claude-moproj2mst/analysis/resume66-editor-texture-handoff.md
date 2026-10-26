# Editor texture-image handoff to the null-image finisher (checkpoint 66)

## Handoff boundary

The selected style24 path already carries the exact current brush payload into a
bounded active-copy producer, but the editor deliberately stops before finishing
the returned outer image. The existing `PaintDeviceNullImageFinishCall` is the
correct next consumer for that one outer value. Its result is the precise point
where the active-copy branch can return image opacity to the QWidget constructor
cursor.

Current editor state sequence in `src/engine/editor_base.cpp`:

1. At `TextureImage`, the editor resumes `BrushTextureImageOpacityCall` against
the selected `BrushTextureImageState`. If it reaches
   `ImageCopy/ActiveImageCopyUnrecovered`, the ordinary helper has not copied or
   retained image data and has not returned a predicate. The editor preserves
   that call and source in its existing call frame.
2. `TextureImageActiveSource` revalidates style24, the exact selected payload
   identity and the current widget's canonical cleanup globals, then obtains a
   bound source view. `PaintDeviceActiveCopySourceState::fromTextureImageSource`
   retains the actual image payload as host metadata, weakly records the actual
   module globals, and introduces no native image-data reference.
3. `TextureImageActiveCopy` resumes the one producer. Only its qualified
   active-source/null-data route reaches Returned. The producer constructs
   outer I1 and inner J; J is copied/destroyed through its own QImage prefix and
   cleanup. At Returned, I1 remains live and null, with constructor-proven
   painting word0 and the producer's weak module provenance.
4. The editor calls `takeReturnedImage()` once and stores the unique
   `PaintDeviceActiveCopyValue` in `textureImageActiveValue_`. This changes
   native-value lifetime responsibility from producer to the editor call frame;
   diagnostic prefix pointers are not substitutes for it.
5. `TextureImageActiveFinish` currently validates that same live I1 and that its
   inherited module origin still matches the exact current runtime owner, then
   returns `TextureImageActiveFinishUnrecovered`. This is the intentional
   unimplemented one-time consumer handoff. No outer predicate, outer
   destruction, opacity publication, ordinary image-destruction summary, or
   QWidget opacity decision has run yet.

The next wiring should transfer `textureImageActiveValue_` exactly once into a
retained `PaintDeviceNullImageFinishCall` only after the current value/module
checks pass. Keep the finisher in the editor call frame while pending. On its
Returned report, read the now-published `opaque` result (true for this null-data
branch), retain it as the image predicate, and advance directly to the existing
`OpacityDecision` cursor. It must not continue through
`TextureImagePaintDevicePrefix` or `TextureImageCleanup`: the finish consumer
has already executed I1's QImage destructor, QPaintDevice prefix and outer
cleanup. Reusing either the ordinary data-only helper summary or the inner J
cleanup would duplicate effects or cross the wrong returned value.

## Ownership, ordering and no-replay rules

- The canonical brush image at payload `+0x78`, its embedded image data, the
  active-source view, inner J, returned outer I1 and module-global cleanup owner
  are separate identities. The producer borrows the source view; it owns J and
  I1 while active. `takeReturnedImage()` transfers only I1 through a
  `unique_ptr`.
- `PaintDeviceActiveCopyValue` carries I1's actual QImage prefix state and
  optional null data pointer. Its module origin is weak provenance inherited
  from the producer's exact inner cleanup owner. Diagnostic shared leases must
  not keep its native value or QtGui module callable.
- `PaintDeviceNullImageFinishCall` takes sole value ownership. Its constructor
  captures I1's same weak module origin and has no replacement-owner API. It
  validates a live QImage value, qualified live origin and known-null image data
  before effects. Unknown/non-null payloads and prefix/cleanup uncertainty stay
  guarded.
- The finisher reads I1 data for `QImage::hasAlphaChannel`; null returns false
  without format, alpha-bit, pixel or image-data reference operations. It
  retains `!hasAlpha` privately. Then it repeats QImage class installation and
  the null data read, runs I1's separate PaintDevice prefix, and calls a new
  outer cleanup against the same module globals. Only after cleanup returns
  does it mark I1 destroyed and publish opacity true. Pending cleanup leaves
  the predicate private and `report.opaque` disengaged.
- The producer's J cleanup is already complete and stays a distinct historical
  receipt. It cannot stand in for I1 cleanup, even though both use the same
  actual module counter owner. I1 cleanup has its own `PaintDeviceCleanupCall`.
- On reentry, resume the retained producer or finisher at its existing stage.
  Do not call the brush-image getter again, re-read/consume the active source
  word, reconstruct I1 from a diagnostic prefix, retake the producer result, or
  replay J cleanup. After finish returns, keep the editor cursor past the finish
  stage so neither I1 destruction nor opacity transfer runs twice.

The `TextureImageOpacityResolved`/`OpacityDecision` boundary is the image result
handed back to QWidget's opacity logic. For this active-copy branch, it follows
I1's completed destruction. `TextureImageDataBoundaryReached` is specific to
the ordinary metadata helper and must remain absent.

## Evidence and qualifications

- `analysis/resume65-editor-selected-texture.md` SHA-256
  `548C767F676DC2B88A10F3AF8C03D21377E7A8C1213F784558C82C01A748118C`
  freezes the editor's actual selected-payload binding and active producer
  endpoint. It records that the present endpoint is the unrecovered outer
  finish guard and preserves the exact source/module checks.
- `analysis/resume62-editor-paint-implementation.md` SHA-256
  `C3283D91417B204431DCC85691A95F266A65EA60E448633EED6F1365D3A95019`
  specifies the one-value consumer seam and effect ordering. `resume62-editor-paint-owner.md`
  SHA-256 `0C977468B636A60EE253D83AC25502974B85CFB7E41C8E2219660C92FF847664`
  documents the weak module provenance handoff. `analysis/resume60-paint-outer-next.md`
  SHA-256 `1608A538270EEC20DB40F14F2E9790AE7131F8DE9B52AE490CD5DA470F847168`
  traces the native outer return, `hasAlphaChannel` and destructor order.
- Original QtGui identity for the retained native captures is SHA-256
  `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
  Existing native qualification places source selection in QWidget's
  `6506e6f2..6506e721`, style24 brush opacity dispatch in `6511c090/6511c170`,
  the active QImage copy at `650b9a70`, outer predicate at `6511c18d`, and outer
  destruction at `6511c1ac`. The null-data `hasAlphaChannel` leaf is
  `650aa340..650aa384`; it returns false without format/flags/pixels. `resume48-brush-style24-contract.md`
  SHA-256 `16FED4DFD99F7FA8E304B6B8C69744FCF13A4C356ECC4A29117ED803F1A4D3E5`
  preserves the native-byte evidence and bounds.
- Candidate `brush_texture_image.cpp` SHA-256
  `8900978E4EA8B8D3A344FA71CE967F07CC58830AA824E20F7A83DB7E8B7CF11B`;
  it returns at the data predicate boundary and does not model the active
  consumer's QPaintDevice tail. `paint_device_active_copy.hpp` SHA-256
  `9CBDE173BF1BD542BDE9ECF2738489643EC857AF7D021F2CA7E4DE07BE937A4E` and
  `.cpp` SHA-256 `06AFE87C8C1F1A79FCF4568E5A4EDE0ADE706A03129B948CE41F40FC7EF5223C`
  contain both the producer and null-image consumer. `editor_base.cpp` SHA-256
  `CE2ADBDC400C7759D3E2887CA4C52E373068BB4238E2F3D05EAF97C16DEA25D9` currently
  retains I1 but stops at `TextureImageActiveFinishUnrecovered`.
- Direct helper tests in `test_paint_device_active_copy.cpp` cover producer-to-
  finisher unique ownership, distinct inner/outer cleanup calls, predicate
  withholding during pending outer cleanup, opacity after outer destruction,
  weak origin loss, and idempotent returned receipts. `test_editor_base.cpp`
  verifies the selected editor path retains the outer value, source/module
  identity and finish guard without publishing opacity or entering the ordinary
  finish prefix. These tests are static source review evidence here, not a new
  build/test result.

## Acceptance boundary for a later integration

A focused editor integration should demonstrate that one selected actual style24
payload and current module produce one I1; the helper remains at its active
copy edge; `takeReturnedImage` happens once; and the finisher receives that exact
unique value and inherited weak module. With outer cleanup pending, the same
finisher/value/cursor must remain, no `opaque` result may escape, and neither
`TextureImageOpacityResolved`, widget opacity assignment nor ordinary
image-prefix/cleanup effects may appear. After qualifying the same cleanup
owner's exact normal fast path, the finisher should return opacity true only
after `OuterImageDestroyed`; the editor advances to QWidget `OpacityDecision`
without a second QPaintDevice cleanup or inner J replay. Source/payload/module
loss, nonnull data, wrong owner, or repeated completion must preserve the
appropriate guard and no-replay properties.

No source, build/test, GUI, or mutable REA operation was performed. This is a
read-only trace; no claim of public editor300 construction completion, valid
original UI reachability for active-null state, or Open-to-Save parity follows.
Token measurement unavailable.
