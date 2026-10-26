# CP248: finite meshing-object writeback under GuiModel ownership

The next dependency-ready alias migration is `copyMeshingObject` in `src/engine/simcontrol300.cpp`: its current-object lookup and four scalar writes can become one finite `GuiModel` command taking the existing pointer-free `MeshObjectSnapshot`. Both the legacy const Apply and mutable Selection Apply already reach this leaf. This removes the native object pointer from those editor writeback callers without widening meshing behavior, issuing an ObjectKey or requiring another GUI witness.

CP245's reaction-rate coordinator and CP246's HeatTreat Mode commands are implemented in current source and their receipts; do not reassign those completed cuts. Current `copyMeshingObject` still performs the raw `model.object(value.number)` borrow and all four writes. Only this report is written: no source/test, build, shared workflow state, original process, GUI control or mutable REA state changed; no tests ran.

## Exact existing seam and callers

All application paths below are relative to `source/apps/moproj2mst/`.

`src/engine/simcontrol300.cpp`, anonymous namespace around lines 1287–1296:

```cpp
void copyMeshingObject(GuiModel& model, const MeshObjectSnapshot& value)
{
    if (auto* object = model.object(value.number)) {
        objectField<double>(object, 0x2dc8) = value.interference;
        objectField<double>(object, 0x2dd0) = value.sizeRatio;
        objectField<double>(object, 0x2de0) = value.tolerance;
        objectField<double>(object, 0x2dd8) = value.nodeCount;
    }
}
```

Its two current consumers are:

- `applyMeshing3D`, around lines 1314–1333: retain existing model/dimension/method/selection preflight; for each remembered row copy it locally; for row zero only, derive the four current control values using the existing conversion/absolute-value/sign/spin rules; invoke `copyMeshingObject`; after the whole loop apply singleton remesh controls.
- `applyMeshingSelection3D`, around lines 1338–1352: retain preflight; run `transitionMeshingObject3D` with the actual combo index; refuse if incomplete; copy every committed remembered row through `copyMeshingObject`; only after the entire loop apply singleton controls. It intentionally does not reconvert the freshly loaded selected row.

Recheck those named functions and `rg` call sites when reserving ownership because line numbers can move. Neither activation itself nor Fill calls this native writeback helper. Do not move the new command into selection activation or use it to eagerly synchronize temporaries.

## Proposed finite owner command

Declare a command such as:

```cpp
void GuiModel::copyEditorMeshingObjectValues(
    const simcontrol300::MeshObjectSnapshot& value);
```

Add only a forward declaration of `simcontrol300::MeshObjectSnapshot` to the existing `simcontrol300` declaration block in `include/m2m/engine.hpp`. The complete record already lives in `include/m2m/engine/simcontrol300.hpp`: one object number and four doubles, with no pointer/reference/owner fields. `gui_model.cpp` already includes that editor header for CP240's StepCount owner command. This uses the established incomplete-value-type boundary; do not include the editor header back into the general engine header or create an include cycle.

The definition in `src/engine/gui_model.cpp` must perform the same `object(value.number)` lookup and the four assignments in the displayed order. Use the existing engine `at<T>` helper or equivalent local byte-field access already used in that file. It returns void and retains no input/native pointer. No callback, native reference, generic resolver, new allocation, public offset/index setter, bool success policy or `noexcept` promise is needed.

Taking the existing copied-value record by const reference preserves scalar reads **after** the object lookup, just as the old helper. Do not expand it into five scalar arguments that are evaluated before the lookup. The record is not native model storage: it is the already-owned page/local snapshot supplied synchronously, and the owner method must not retain its address.

Keep `object()` as the one lookup/admission operation inside the command. It currently calls `requireGeometrySyncAvailable()` before nullable step/object lookup. Do not add a second geometry guard, preflight a StepIdentity, or require object existence earlier. Missing step/object remains a guarded no-op for this command. A geometry error propagates at the same per-row point; prior row writes remain committed, and the later singleton state update is not performed.

The write order is interference (`+0x2dc8`), size ratio (`+0x2dd0`), tolerance (`+0x2de0`), then node count (`+0x2dd8`). Do not reorder by increasing offset, round/clamp/validate again, substitute a whole mesh/settings copy, change `nodeCount` type, or canonicalize floating-point values. Only the selected control path converts values before this helper; unvisited remembered payloads must preserve their existing double representation, including qualified nonfinite/signed-zero cases. Source ordering remains part of review; final-state tests cannot prove every intermediate store.

Replace the two calls with `model.copyEditorMeshingObjectValues(value)` at the same locations and remove the now-unused local helper. Alternatively a temporary one-line delegating helper can compile, but leaving it as a permanent forwarding layer provides no extra contract. Keep `objectField` and native object includes elsewhere in `simcontrol300.cpp`: many other reads/writes still need them.

## Why object-number lookup does not require ObjectKey

This command intentionally names the currently present object by the remembered number, matching the existing native-qualified Apply loop. It does not claim that the row identifies the same storage incarnation as at Fill. If an object is missing, its copy is skipped and later rows continue. If a new current object now has that number, the existing by-number behavior selects it; do not introduce a stale-key refusal or same-address/identity admission as an unsolicited behavior change.

CP225 requires live incarnation checks when introducing retained ObjectKey semantics. This leaf introduces no such key or dereference capability: the current native pointer exists only during the single named command. It neither solves nor depends on global create/remove/recreate/reload instrumentation. Do not advertise a copied number as an opaque incarnation or use StepIdentity currentness as permission to access it later.

## Exact bounded ownership

One owner needs:

1. `include/m2m/engine.hpp`: forward declaration and named command declaration.
2. `src/engine/gui_model.cpp`: current-object lookup and four-field implementation.
3. `src/engine/simcontrol300.cpp`: migrate the two calls and remove the local raw-alias helper.

Root must serialize the two common model files with any active API owner. No `simcontrol300_owner.hpp/.cpp`, reaction-rate coordinator, HeatTreat property file, loader, layout, C ABI or meshing-page representation change is required. `MeshObjectSnapshot` itself stays unchanged. No new source file or build registration should be necessary.

## Focused tests

The existing tests already exercise actual callers of both writeback paths; use them rather than testing only a new direct setter:

- `tests/mdt/test_simcontrol300_disabled_remesh.cpp::disabled_remesh_ignores_method_projection_but_commits_objects_before_shared_enabled_byte` checks const Apply's selected-first rounded values, unvisited raw-bit values, unrelated object fields and unchanged non-enabled singleton state. It uses the actual Session-owned RemeshState explicitly but is not a public Session callback witness.
- `disabled_remesh_missing_remembered_object_does_not_stop_later_object_or_enabled_store` physically removes a remembered model object and proves its missing-number skip does not prevent later object updates and the final enabled-byte store. The deleted pointer is not used afterwards.
- The same executable's enabled-invalid-method, wrong-dimension and absent-step cases keep existing caller preflight ahead of effects.
- `tests/mdt/test_simcontrol300_meshing_selection.cpp::meshing_selection_spin_conversion_and_missing_live_object_keep_original_order` covers mutable Selection Apply, missing remembered object, later updates and singleton state. Retain its same-row/programmatic-selection tests and masked nonfinite payload cases; the new owner command must not perform another conversion.

Expected targets/CTest names are `m2m_mdt_simcontrol300_disabled_remesh` / `m2m.mdt.simcontrol300_disabled_remesh` and `m2m_mdt_simcontrol300_meshing_selection` / `m2m.mdt.simcontrol300_meshing_selection`; confirm configured names. No changes to these two files are necessary unless inspection identifies a missing assertion relevant to the moved writes.

For the newly exposed owner command, add one direct refusal assertion in the established parked geometry block of `tests/mdt/test_viewer_box.cpp::geometry_low_query_model_barrier_stops_next_view_retries_and_owner_bypass`. Supply a local `MeshObjectSnapshot` with the real object number and conspicuously different values, invoke the command through its existing `refused` helper, and retain later byte/state comparisons. This confirms the new entry does not bypass `object()`'s guard. Do not construct a new GUI/editor session or call parked `pageState() noexcept`.

Root's minimum focused group is those two meshing targets plus `m2m.mdt.viewer_box`, followed by the normal source-stable full launcher/freeze. Keep Testkit's existing entry points, resolvable includes, `CHECK`/`CHECK_EQ`/`REQUIRE`, and no C `assert`/`REQUIRE_EQ`. No external file probe, new fixture registration or production test hook is required. No validation result is claimed by this report.

## Original evidence and exact remaining limitation

Reviewed CP245/246 implementation receipts/current source, CP222's editor/ownership architecture, CP225's object-mutation contract, CP128's disabled-remesh report/review, CP129's meshing-selection contract, CP165's meshing evidence boundary, current caller/source/value record and the focused tests above.

`analysis/resume128-editor300-next.md` and `resume128-editor-disabled-review.md` qualify original controller `0x140ec1350`: current-form commit at `0x140ec13a0`, current-object lookup/four-field helper call at `0x140ec14df`, missing-object continuation, then singleton disabled/enabled work. The report pins original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, retained `wave2-save-diffs-300-save-140ec1350.asm` / `wave2-save-diffs-300-apply-140ec1350.c`, and bounded byte hashes. CP129 qualifies the separate retained temporary owner, explicit activation/current-index sequence and four-scalar projection. These are retained static qualifications, not fresh original execution here. The exact four-store source sequence moved by this proposal is current reconstructed-source evidence; this audit does not claim to independently rehash/rederive every original helper store.

CP165 correctly blocks **broadening** the 0x158-byte temporary record, adding controls/fields or asserting native allocation/lifecycle parity without a same-owner witness. This proposal does none of those things: it moves only the already-supported four-field operation behind the model owner. Thus its missing broader meshing witness does not block this ownership refactoring. Keep the existing finite projection and public-editor constructor/palette guard intact.

The focused tests qualify reconstructed page/model behavior and floating-point profiles, not physical QWidget delivery, actual original activation, fresh Open–Save–Close fidelity or original row allocation/destruction. Other editor/native aliases remain, including Fill/object scans and caller dimension/raw-step checks. No cache1/Stopping continuation, global ownership closure, accepted Close, GUI-release lifecycle or original-witness gate is advanced by this leaf.
