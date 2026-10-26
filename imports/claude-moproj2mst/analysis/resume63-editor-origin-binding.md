# Bind active-copy source identity to the canonical texture payload

The next safe binding is one runtime-owned source VIEW of the existing shared
BrushTextureImageState, with that runtime's exact weak PaintDeviceCleanupGlobals
origin. Read painting/data fields directly from the canonical payload. Do not
copy them into a second observed source record. Binding establishes ownership
only; it must not resume construction, copy, destruction or publish opacity.

Owner: editor300. This architecture pass writes only this report; no new raw
capture was required. Existing source and bounded CP48/CP62 native captures were
read. No source, CMake/build, GUI, original execution, mutable REA or tests during
the audit. Token usage is unavailable. A subsequent helper-only implementation
was separately assigned by root while this report was being finalized.

## Existing actual owners and native qualification

CP62's native owner audit pins original QtGui SHA-256
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`
and proves caller I1 return storage, distinct inner J, and fixed module slot
RVA9cb858 used by both destructors. Its capture
resume62-editor-paint-owner-qtgui.asm SHA-256 is
`10689dcb13af2864b43a021fb19a186605f51fa3b222e1479fa00e488f8cdd41`.
CP48's6511a1a0/6511a610/650b9a70 captures establish B+78 as the actual embedded
source image, returned to the getter by address and copied into I1. No native
flattened source-metadata copy or per-image global-owner field is involved.

Current host ownership is already:

`WidgetRuntimeState::Values::brushes[alias] -> WidgetBrushDataState -> textureImage_`

The textureImage_ object is the canonical embedded B+78 payload. Multiple
palette slots/widgets sharing one actual brush share that SAME payload, while
different brushes with identical image-data aliases retain distinct image values.
WidgetRuntimeState::observeTextureImageInputs preflights image aliases and source
links, stages receiver access, then refines existing payloads in place. Unknown
word/data stay unknown. There is no need for new observations of identical fields.

The canonical module is Values::paintDeviceCleanup. The existing
paintDeviceCleanupGlobals(actualWidget) accessor requires this live runtime's
qualified private-input receiver and its explicit module-profile grant. Merely
having a container because another widget qualified it does not grant access to
an unqualified receiver. Runtime destruction releases its native owner, invalidating
escaped diagnostic host leases. A source view should use that existing lifetime
signal instead of making an independent module container or guessing current0.

## Minimum helper bridge

Add two const getters on BrushTextureImageState:

- imageDataOwner(): const optional<shared_ptr<BrushTextureImageDataState>>&;
- imagePaintingActiveWord(): optional<uint16_t>.

These expose the existing fields without new storage or effects. The active-copy
source may retain a strong diagnostic lease to this payload so returned field
references remain valid, while weak module provenance controls native callability.
Host metadata lifetime does not keep a dead runtime/module callable.

Extend PaintDeviceActiveCopySourceState with a bound mode, preserving its existing
standalone isolated observation mode. Proposed factory:

`fromTextureImageSource(shared_ptr<const BrushTextureImageState>, const shared_ptr<PaintDeviceCleanupGlobals>&, string qualification)`

Require nonnull payload, nonempty qualification, live qualified canonical module
(raw/control-block identity against weak_from_this). Bound mode reads the two
payload getters directly and exposes textureImageSource(), cleanupGlobalsOwner()
and textureImageBound() for registry/continuation checks. It must reject its
standalone observe method entirely, including equal-looking supplied fields.
Only canonical runtime texture observation may refine the actual bound metadata.
No bound word/data snapshot or observer fan-out is needed.

Native source availability becomes own alive flag AND bound metadata presence AND
that exact module's live qualified native owner. Releasing a view or its module
cannot be undone by new observations or a replacement origin. Strong diagnostic
payload/global leases cannot bypass native-owner release. Standalone mode retains
its existing behavior and explicit isolated source/global association precondition.

For a bound source, PaintDeviceActiveCopyCall initializes its weak globals from
the source origin and marks that binding fixed. Any explicit bindCleanupGlobals
must match the exact origin; an equal SHA/RVA/alias object is insufficient.
Standalone mode keeps its current monotonic explicit binding. CP62's returned
value then inherits this same origin after inner cleanup; the null-image consumer
uses it without replacement. No extra native Qt store/ref is emitted by binding.

The helper factory cannot itself prove that a supplied payload belongs to a
particular WidgetRuntimeState. That registry join remains a mandatory next seam;
the factory is an internal owner constructor, not production proof or GUI admission.

## Runtime registry seam, separate shared-file assignment

Proposed runtime API:

`shared_ptr<PaintDeviceActiveCopySourceState> bindTextureImageCopySource(const shared_ptr<WidgetState>&, WidgetBrushAlias);`

It allocates/records host ownership, so it should be an explicit bind method rather
than a supposedly read-only getter. Invalid identity/profile throws M2M_E_UNSUPPORTED,
consistent with existing qualified runtime binding methods. It performs no native
construction/copy/resume. Preconditions and commit:

1. Validate live canonical widget receiver, component owner and same runtime,
   with existing privateConstructionInputs. Require this receiver's actual
   paintDeviceCleanupGlobals grant, not merely a module pointer in Values.
2. Resolve positive brush alias in THIS runtime registry; require actual style24
   and existing canonical textureImage_ payload. Do not default-create missing
   image metadata or infer selection/data/painting fields. Binding B+78 identity
   itself does not choose pixmap versus image or execute a copy.
3. Intern one view per actual brush/payload. Store registry brush/payload/module
   identities with exact raw/control blocks, use stable runtime/brush qualification,
   and reuse the same view across qualified widgets/slots sharing that brush.
   Different actual brushes must not merge just because data aliases/fields match.
4. Validate existing cached identities before return; reject replacement, dead
   view, wrong module/runtime or different payload. Stage allocations before
   publishing a new registry entry. Failure changes no grants or existing state.
5. Metadata observations continue solely through existing runtime APIs. Their
   whole-envelope preflight/atomic commit automatically appears through the view;
   no binding refresh, duplicate source observe or continuation is triggered.

A shared payload observed through one widget may be reused by another actual
widget whose private palette references the same runtime brush. The new source
identity is not per-widget and must not be tied to the first widget's lifetime.
Each current caller still needs its own live private/module receiver grant.
Do not invent another symbol qualification for already shared brush metadata.

Lifetime: the runtime registry owns the view; a view's diagnostic payload lease
cannot extend actual module lifetime. Existing runtime destructor releases the
canonical globals and therefore invalidates all bound views. If later work models
actual brush/native source destruction before runtime end, invalidate its view
before dropping the registry owner; no such dynamic transition is admitted now.
A returned I1 may outlive its borrowed source/copy frame under unique ownership,
but it still requires its inherited live module for later destruction. Its lifetime
is independent of the original cached B+78 and the originating widget.

## Smallest file/ownership split

Helper-only bridge can be assigned as one bounded increment:

- include/m2m/engine/brush_texture_image.hpp: const canonical-field getters;
- include/m2m/engine/paint_device_active_copy.hpp and src/engine/paint_device_active_copy.cpp:
  delegated bound view, weak origin/liveness and exact producer provenance;
- tests/mdt/test_paint_device_active_copy.cpp: bound-view focused regressions.

No brush_texture_image.cpp algorithm change or old observer rewrite is required.

The next shared runtime increment must separately own view_manager.hpp/.cpp plus
one new tests/mdt/test_widget_texture_copy_origin_binding.cpp. Root owns CMake
wiring. Keep editor_base.hpp/.cpp and public conversion/session code out of both
increments; an actual selected-brush caller hook comes only after source registry
proof and must preserve consumed alpha and pending opacity/destructor cursors.

## Focused acceptance contract

Helper bridge tests: actual payload pointer/control block retained; observations
refine the SAME payload and are visible after a pending word/data guard without
new state copies; bound observe rejected; standalone mode unchanged; canonical
module qualification/lifetime required; same-profile foreign origin rejected;
producer automatically uses exact origin and CP62 returned value/consumer preserve
it; module/view death invalidates calls despite host metadata leases. No copy or
cleanup effects occur merely on binding/refinement.

Runtime tests: missing private inputs, foreign/rewrapped/dead receiver, missing
per-receiver module grant, absent/non24 brush and missing payload rejected without
cache publication; two widgets sharing one brush/module reuse the exact view;
different brushes remain distinct even with shared image data; unknown fields
remain unknown; invalid late observation envelope leaves bound reads unchanged;
module death and diagnostics cannot revive a view; observation/bind does not
resume native base/copy. Exercise active-null producer and outer finish only with
explicit counter0 and existing image/getter selection guards respected. Use
Release-active CHECK/REQUIRE. These are proposed tests, not executed results.

## Public mode300 source and unchanged guard

Current loader_l3.cpp formingSimControl300Open calls the actual owning Session's
beginEditor300Factory, then retains not_ported6504abb0. conversion_session.cpp
constructs StagedEditor300 with actual Project/Operation/GuiModel/Remesh addresses,
binds the Session widgetRuntime_, and calls continueNativeBase with a receipt
explicitly stating post-Open private-value aliases are NOT bridged from startup.
It requires the expected PrivatePaletteUnavailable report. Binding runtime
ownership does not certify QWidget construction.

The original application path recorded in loader_l3.cpp is140eb26f0 ->140eb5630
->140eb80c0(page0) ->140ec19f0; the later CG normalization belongs after that UI
setup/fill path. Isolated editor setup or PaintDevice tests cannot bypass this
private palette/QObject/QWidget/wrapper boundary. EditorBase later chooses the
current palette group, Window role10 and actual shared brush, consumes brush
alpha before style dispatch, and currently holds active image copy guarded.

Therefore public mode300 cannot be enabled by the source view or weak provenance.
Actual post-Open private/default palette/font/locale and selected-brush aliases,
remaining constructor/event/font/wrapper/setup effects and fresh runtime evidence
remain dependencies. The source view is a necessary owner join, not evidence
that a real public call currently reaches a texture image. Preserve the public
6504abb0 guard and later ApplicationWrappersUnrecovered behavior absent proof.

Next implementation now assigned by root: helper-only bridge above. Unresolved
registry dependency is explicit; no new original capture or module values were
established, and no runtime/public binding was implemented by this audit.
