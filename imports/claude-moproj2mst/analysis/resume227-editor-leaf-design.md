# CP227: editor title copy owner-operation leaf

## Recommendation

Migrate the single native title write in `StagedEditor300::titleChanged` into a named `GuiModel` operation, e.g. `copyEditorTitleIfControlMatches(const SimControl* expected, std::string_view ascii)`. This is a small, genuine editor sim-control alias consumer: today the editor resolves a raw `SimControl*`, validates it against the cached source-control identity, and writes `control->title` with `memcpy`. The model operation can perform the same fresh lookup, identity comparison, and bounded copy without returning a control alias to the editor. It does not touch the Stopping fill/apply path or any cache publication/continuation.

## Current source and preserved order

In `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`, `StagedEditor300::titleChanged` currently performs, in order:

1. editor binding/state/page/source-model/source-remesh and delivered-widget-text checks;
2. title length (80-byte) and printable-ASCII admission;
3. conversion through `qt::toAscii(deliveredText)`;
4. `binding.model.simCtl()` lookup and equality against `pages_->sourceControl`; `GuiModel::simCtl()` first calls `requireGeometrySyncAvailable()`, then returns the current control or null;
5. `memcpy` of `bytes.size() + 1` bytes into the native title field.

The named operation should be called only after steps 1–3. At that same point it must first call `requireGeometrySyncAvailable()`, then freshly resolve the current control, compare it with the expected identity token, and copy only on equality. The geometry barrier must therefore refuse after ASCII conversion but before null/identity checks and before the title write, matching the current `simCtl()` call position. Return a boolean/status so the caller maps a null or mismatched control to the existing `ApplyResult::UnsupportedSnapshot`; success maps to `Completed`. Do not dereference the expected pointer, use a cached control for the write, move ASCII conversion/admission after control lookup, change the 80-character boundary, change the null/replaced-control outcome, or alter the copy length. `simCtl()` already supplies the geometry barrier; the new named command must retain that barrier-first behavior rather than bypassing it or adding an earlier guard in `titleChanged`.

## Exact files and focused coverage

Production:

- `source/apps/moproj2mst/include/m2m/engine.hpp`: declare the named command next to the other `GuiModel` model-owned operations. Forward-declare `SimControl` as needed. Keep the argument as an identity token only; do not expose a getter or return a native pointer.
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: implement by calling `requireGeometrySyncAvailable()` first, then resolving the current control, returning mismatch for null or nonmatching identity, and copying the supplied already-admitted ASCII bytes into `title` with the same NUL-inclusive length.
- `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`: replace only the final fresh `simCtl()` lookup/equality/copy block; preserve all prior checks and result mapping.

Tests:

- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_title_changed.cpp`: extend existing success and rejection cases. Retain existing successful title bytes and invalid/replaced binding checks; add an explicit current-control replacement/mismatch assertion if the fixture can safely replace it, and verify the title remains unchanged on refusal. Existing test target covers this file; root should confirm configured target metadata. Test through Testkit macros.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_stopping_cached.cpp`: no source/test change is required for this leaf. Run it as a guard regression and confirm cache index 11 remains zero for the existing covered refusal/retry paths. Its R300 behavior is untouched by this editor-only API.

## Compatibility with CP225/CP226 and invariants

There is no semantic API conflict with CP225's `GuiModel::lastObjectNumber()` or CP226's `GuiModel::setDimension(int)`: all three are distinct named operations and all avoid returning a borrowed pointer. They do create integration overlap in the shared `engine.hpp` and `gui_model.cpp`; sequence the API hunks or integrate them in one owner-controlled pass, then run the focused targets together. CP226's call sites remain in `class_slots.cpp`/`l6_slots.cpp`; CP225 remains in `viewer.cpp`. Do not modify those files for CP227.

No C ABI header, exported signature, layout, API version, cache1 store, Stopping predicate, coordinate reader, widget construction/order, or `simcontrol300.cpp` path needs to change. Existing widget event order, geometry-barrier refusal point, and pre-write validation remain intact. This is an incremental owner migration, not proof of source closure or authorization to publish cache1.

## Evidence and limits

The source evidence is the live implementation in `src/engine/simcontrol300_owner.cpp`, `GuiModel::simCtl()` in `src/engine/gui_model.cpp` (barrier call precedes the nullable control return), and the existing focused test `tests/mdt/test_simcontrol300_title_changed.cpp`. The alias ledger is `analysis/resume222-editor-alias-ledger.md`; the architectural closure constraints and cached-Stopping regression are in `analysis/resume222-editor-closure-architecture.md`. CP225's selected leaf contract is `analysis/resume225-next-viewer-leaf.md`; CP226's shared model API/integration contract is `analysis/resume226-dimension-owner-command-contract.md`. This is a read-only design proposal, not an implementation or test result. No source, build, C ABI, shared state, original process, GUI, or REA state was changed.
