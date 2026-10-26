# Actual widget-runtime PaintDevice cleanup globals

Source is frozen for root validation. The runtime now owns one explicitly
observed cleanup-global state and exposes it only to qualified live receivers.
No current pointer, counter value, successful cleanup or native module lifetime
is inferred from a file initializer, an image ref count or host object creation.

Owner: `gradient_state`. Exclusive files: `view_manager.hpp/.cpp`, new
`test_widget_paint_cleanup_binding.cpp`, and this report. No editor/helper/CMake
edits, build, GUI control, original execution or mutable REA. Token measurement
unavailable. The API/semantics were coordinated with `editor300` before edits.

## Native boundary and API

The controlling native qualifications remain in
`analysis/resume53-editor-style24-next.md`,
`analysis/resume54-paint-device-cleanup.md`, and independent review
`analysis/resume54-style24-ownership-review.md`. The isolated helper models
cleanup body65148840's existing-counter==0 path only. Lazy initialization,
nonzero mutex/list work, destructor prefix and image/editor cleanup ordering are
separate dependencies. This patch enables no public constructor or Save path.

The coordinated API is:

```cpp
void WidgetRuntimeState::observePaintDeviceCleanupInputs(
    const std::shared_ptr<WidgetState>&,
    const WidgetPaintDeviceCleanupObservation&);
std::shared_ptr<PaintDeviceCleanupGlobals>
    WidgetRuntimeState::paintDeviceCleanupGlobals(const WidgetState&) const noexcept;
```

The alias-only envelope contains receiver weak pointer, that widget's
private-input qualification, QtGui module SHA-256, pointer RVA, counter
observations and optional current counter alias. It supplies no caller-owned
global/counter shared_ptr. Module SHA/RVA are validated by the frozen helper:
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`, RVA9cb858.

## Owner, admission and lifetime

`WidgetRuntimeState::Values` retains one `PaintDeviceCleanupGlobals` plus weak
qualified receiver records. The global container is created/staged only for an
explicit observation and published only if its whole envelope passes. Its
stable internal qualification is the actual runtime's nonempty qualification;
each outer receiver separately supplies its matching private-input receipt.
Two widgets with different private receipt strings therefore share the same
module owner after each explicitly qualifies. A second widget's mere runtime
membership does not grant the accessor.

Admission checks the actual live private-constructor widget, matching object and
control block, component owner, runtime and nonempty private-input qualification.
The adapter stages a copied receiver map and the full helper envelope before
calling the isolated helper's atomic observation. After that helper succeeds,
only noexcept shared_ptr assignment/map swap remains; failure refines no old
counter, publishes no new alias/global and grants no receiver access.

Disengaged pointer/value stays unknown. Explicit current alias0 is known null
and preserves the lazy-global guard. Positive aliases resolve only in the
runtime-owned helper map. Equal/unknown-to-known observations preserve actual
owner/control-block identity; conflicting values and pointer replacement retain
the frozen helper's rejection semantics. A released owner is not replaced by
creating a fresh default container.

The accessor requires a live same-runtime private-input widget and its explicit
successful receiver record, plus a qualified live global owner. It returns null
for absent/unqualified/dead/foreign receivers or released globals. Returning an
owner with unknown pointer/value permits the existing precise helper guard; it
does not imply initialized-counter0.

`WidgetRuntimeState` now has an out-of-line destructor that marks its cleanup
global binding's native owner released. Diagnostic strong global/counter leases
therefore cannot keep that actual runtime binding callable after its owner ends.
This marks reconstruction lifetime; it does not unload a physical Qt module or
perform Qt ref/free operations. Destroying one widget leaves the module owner
live for other qualified widgets while the actual runtime remains owned.

## Authored checks and frozen files

Fourteen new focused cases cover no defaults, actual receiver/provenance,
foreign runtime/control block, pinned module/RVA, known-null lazy guard, late
value and pointer binding without replay, same-runtime sharing across different
widget receipts, late invalid link preserving metadata/aliases/receiver access,
duplicate/zero aliases and owner replacement, distinct runtimes/released owner,
dead widget versus live peer, runtime end with escaped diagnostic leases, and
observation-only editor/ref-ledger preservation. The last case uses image and
cleanup aliases both31 while image temp refs0 and cleanup value-1: the unrelated
image ledger does not turn the global cleanup into the zero fast path.

No test executable was run by this worker. Direct whitespace checks and
`git diff --check` passed. Root owns the focused target
`m2m_mdt_widget_paint_cleanup_binding`, required regression and source freeze.

| File under `source/apps/moproj2mst/` | SHA-256 |
|---|---|
| `include/m2m/engine/view_manager.hpp` | `4e3506ecf7ea8eb253b9055aacff6604dc82958795f9b563e0bbbdc46ac8b283` |
| `src/engine/view_manager.cpp` | `294a9dd682cd1de5841f49c10e4272573465062c8ef3be9b2ef12da29c141710` |
| `tests/mdt/test_widget_paint_cleanup_binding.cpp` | `cf8a2857b52a361d457b6ec77c6cf86c19c738d97e764e7880c44be51ef5a0dd` |

The next integration consumes this actual owner through the accessor at the
retained editor cleanup cursor, after single image-data release/prefix and before
opacity exposure. This runtime patch itself advances no image/editor constructor.
