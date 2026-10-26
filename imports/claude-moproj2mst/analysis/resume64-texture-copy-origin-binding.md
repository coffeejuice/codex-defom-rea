# Runtime-owned texture copy-source interner

Source is frozen for root build/integration. The runtime now joins its actual
style24 brush payload to that receiver's canonical module owner and interns one
source view per payload. Bind/refine operations do not run native construction,
copy/destruction or opacity logic. Public editor mode300 remains guarded.

Owner: editor300. Only view_manager.hpp/.cpp, the new focused test below and this
report were written. No paint_device_active_copy/brush helper, editor/public
mode300, CMake/plan, original execution, GUI or mutable REA changes. No build,
CTest or tests executed in this lane. Token usage is unavailable. Source remains
frozen until root explicitly releases it.

## Evidence and exact API

Read analysis/resume63-editor-origin-binding.md, SHA-256
`0d1998677db7efcb7660a3d8caf4ed45dca82e98f156da743e9f7a16f087dc9a`,
its CP63 helper implementation receipt and CP62 original owner audit. Those
qualify embedded B+78 source identity, distinct returned I1/inner J, canonical
QtGui module slot RVA9cb858 and the public private-palette/wrapper guards.
Original QtGui SHA-256 remains
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
No new original native/runtime value or source reachability is asserted.

New API on WidgetRuntimeState:

`shared_ptr<PaintDeviceActiveCopySourceState> bindTextureImageCopySource(const shared_ptr<WidgetState>&, WidgetBrushAlias);`

The header forward-declares the source state. The implementation includes the
existing active-copy helper and stores views inside WidgetRuntimeState::Values.
It consumes the unchanged CP63 helper factory/accessor signatures; root confirmed
those signatures remained stable during this exclusive lane.

## Receiver, registry and module join

The binder requires a live canonical component widget, exact same runtime raw
pointer/control block and existing privateConstructionInputs for this receiver.
Then paintDeviceCleanupGlobals(widget) must return this receiver's explicitly
qualified canonical live module owner. An owner granted to another widget is
not sufficient, and a rewrapped widget/module cannot acquire access.

The positive brush alias must resolve in THIS runtime's brush registry, with
known style24 and an existing actual textureImage_ payload. Missing owner/style/
payload stays unsupported; the binder does not default-create a native source
or any image metadata. It does not decide pixmap/image selection or cache path.
A later native caller must still respect those existing getter/selection guards.

The cache is keyed by the actual BrushTextureImageState pointer, not image-data
alias, current field values or widget receipt. Each entry strongly records the
actual payload control block and one source view. Reuse requires:

- Same payload raw pointer/control block;
- Same canonical live source view and its actual payload identity;
- Same exact canonical module owner/control block, with native lifetime valid.

A released or mismatched cached view cannot be replaced or revived. Different
actual brush payloads remain different views even when they share QImageData.
Qualified widgets/palette slots referencing the same actual brush can reuse the
same view after each current receiver obtains its module grant. A view is not
tied to the first widget's qualification or lifetime.

Validation and host allocations precede cache publication. Failure changes no
receiver grants, observed fields or existing binding. Binding emits no native
Qt constructors, copy/ref effects, predicates or opacity completion. Actual
metadata refinement continues through observeTextureImageInputs on the SAME
payload; the view delegates its reads and sees late qualification without
observer fan-out. A rejected observation envelope remains atomic for those reads.

## Lifetime and producer provenance

The runtime registry owns the source view. Its strong payload handle is a
metadata lease, while the existing weak canonical module origin controls native
availability. WidgetRuntimeState's existing destructor releases that module's
native owner before member cleanup, invalidating escaped views even if host
payload/global diagnostics remain alive. No extra physical module/ref release
was added. Explicit view release remains terminal for its cached identity.

The existing bound producer automatically fixes globals to that view's origin;
foreign equal-profile containers remain rejected. CP62's actual unique returned
value and null outer consumer inherit that SAME origin after inner cleanup.
This binding closes the registry identity seam for explicit qualified runtime
profiles; it neither creates missing source observations nor changes any native
pointer/counter defaults. Qualified module identity with unknown pointer/value
still retains the cleanup guard until actual legitimate refinement.

## Focused tests and verification

New test_widget_texture_copy_origin_binding.cpp contains12 Release-active cases,
77 CHECK and28 REQUIRE conditions. The fixture binds actual StagedEditor300
widget/runtime and explicit private observations without running native base.
Cases cover:

- Missing private/module grant, null/foreign/rewrapped receiver, zero/unknown/
  non24 brush aliases and absent payload, preserving existing valid cache state.
- Exact payload/control-block interning, unknown fields and no default counter,
  with observation/binding leaving nativeBaseConstruction absent.
- Same brush/module shared by two qualified widgets, current receiver grant
  required, and first widget release not destroying the shared actual source.
- Distinct brush payloads sharing image data, maintaining separate view identities.
- Late actual field refinements, no automatic continuation, active-null copy plus
  outer finish using the exact origin, without invoking public editor construction.
- Rejected late metadata envelope leaving bound fields/cache unchanged, and no
  duplicate standalone observe path into the bound view.
- Module grant with unknown pointer retains cleanup; explicit late counter0
  resumes the same helper and preserves value/module provenance.
- Released view/module cannot be rebound/recreated, runtime death invalidates
  escaped metadata/global leases, and identical aliases in distinct runtimes
  remain distinct origins with foreign producer binding rejected.

Setup remains inside REQUIRE and checks use testkit/check.hpp; no assert/NDEBUG
elision occurs in Release. Static checks passed for whitespace/conflict markers,
balanced delimiters after comment/string stripping,12 unique case names, one
main, no assert and CHECK/REQUIRE argument scans. These are source checks, not
compiler/test results. Root owns new target wiring and focused/full execution.
Suggested consistent CTest name: m2m.mdt.widget_texture_copy_origin_binding.

## Frozen source

Paths under source/apps/moproj2mst/:

| File | SHA-256 |
|---|---|
| include/m2m/engine/view_manager.hpp | c0c146eddb8cba22f30b573a9109c97494046305dd81fdb03c2e6164433d9c53 |
| src/engine/view_manager.cpp | dc6ad8356e061bf7e9aebaa1d32ef2caa8a0d4d41da89720c023a4b825899a4c |
| tests/mdt/test_widget_texture_copy_origin_binding.cpp | f1f46c5ae3b0993f20e49792ac38217f328b6895fcef3b13b37d86b0ed467d0c |

Next: root build/test integration, then separately assign the selected-brush
editor hook. It must obtain this actual runtime view rather than constructing
another source or copying flags; preserve consumed alpha, inner/outer lifetimes,
private opacity and existing prefix/data-release cursors without duplicate effects.
No such editor hook was made. Public Session still lacks post-Open private-palette
aliases and retains not_ported6504abb0; later wrapper/setup, active-nonnull,
warning, pixmap/cache conversion and lazy/nonzero cleanup guards remain. Isolated
success is not proof that public mode300 reaches this state or that fresh
Open-Save-Close fidelity is complete.
