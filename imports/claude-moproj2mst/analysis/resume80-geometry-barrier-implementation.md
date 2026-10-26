# CP80 audited support-entry geometry barrier repair

Known DB, object-deletion, display and retained dimension-callback routes now
check the actual model's persistent geometry barrier before their first effect.
They cannot modify the parked step and then rely on a later viewer refusal.
The existing lower-query frame remains parked; this change does not resume,
retire or complete it.

Root reserved the seven source/test files below after the independent audit in
resume80-geometry-barrier-audit.md. No public header/API, native layout, CMake,
GUI or mutable REA change was made. This worker performed static preflight only;
root owns compilation, focused/full tests and archive.

## Internal guard and ordered entry effects

`requireModelGeometrySyncAvailable(const GuiModel&)` is declared only in the
private source header engine_internal.hpp and defined in viewer.cpp through the
existing GeometrySyncAccess bridge. It reads the same model-owned private
barrier and uses the existing unsupported refusal. It does not expose a public
clear/resume API or create a second owner/state.

| Actual route | Earliest guard and protected effect |
|---|---|
| modelDbReader | Guard precedes even the raw GuiModel.db access/reader return. This is a fresh mutable reader capability, including an expired raw DB backlink. |
| readDbStep | Guard precedes model DB resolution and directory lookup. No step-number/reader dereference occurs while parked. |
| readDbEntry | Guard precedes DB/step tests, optional viewer clear and the resetViewer=false path's u1b58 write/native DB step reader. |
| deleteObject | Guard precedes count/predicate work and actual step_delete_object. A non-PDIE deletion cannot finish outside the later setPdie gate. |
| retained g_loaderSlots.setDimension | The real installed callback checks the barrier before touching m.step or calling its native dimension setter. |
| displayShow / displayRefreshScene | A private display preflight checks an incoming model and any existing outgoing main association before projectDisplay can allocate storage or displayShow changes Display.main/GuiModel.display. Incoming null does not bypass a parked outgoing main. |
| displayResetView | The same preflight runs before camera quaternion/window changes. This additional route otherwise reached fit's later scene refusal only after camera mutation. |

The display guard reads an already-owned Project display without creating one.
Normal calls with no pending incoming/outgoing model keep their existing flow.
Read-only identity/camera diagnostics remain available. Actual-model destructor
cleanup and the CP79 value-only pdie/objectCount behavior are unchanged.

The retained slot initializer already existed in class_slots.cpp. It now has an
internal source-header declaration so the focused test can install and call the
actual callback without a stub or a new public API. viewer_camera.cpp now directly
includes engine_internal.hpp for the new internal gate; the test directly
includes cogging_save.hpp for its actual deleteObject declaration.

## Focused regression additions

The viewer target remains39 Testkit cases; its existing real-owner/two-View
composite case was extended rather than adding another copied barrier fixture.
It still uses the actual COG directory allocation and the actual parked model.

- With an existing in-range DB index and resetViewer=false, readDbEntry and
  readDbStep must refuse before u1b58 or any actual step bytes change. Fresh
  modelDbReader access is separately refused.
- Deletion selects an existing object whose number differs from the current
  PDIE, so a later setPdie gate cannot accidentally satisfy the test.
- The real installed setDimension callback must refuse before changing the
  step dimension. The test restores its previous LoaderSlots pointers at teardown.
- Whole actual step bytes, model tables/state, object/view counts, picking
  prefix, Viewer, all five timers and index-table globals remain unchanged
  around the guarded entry matrix.
- Incoming display calls, including the actual retained showModel callback,
  refuse before a missing Project display is allocated or model backlink changes.
- Calls against an existing display retain its main model, both model backlinks
  and all CameraState values. A separate actual display was associated with the
  target before parking, proving that null incoming displayShow/refresh and
  displayResetView also refuse before altering its outgoing association/camera.
- Nonpending controls retain successful real KEY import and mutable access,
  real object deletion, actual installed dimension change, display association
  and null association. Empty nonpending DB reads retain their false/null results.

Every file write belongs to the existing disposable fixture directory beneath
M2M_TEST_SCRATCH. This worker did not execute the test or mutate fixture data.

## Scope and validation

Public raw fields and pointers obtained before parking remain ordinary mutable
C++ aliases. These guards do not claim to prevent arbitrary external writes or
all possible free functions. Such external changes do not retire the model's
private barrier, make captured provenance current, or authorize reader/public
continuation. The frame remains durable across version/cache/Viewer changes and
DB owner expiration, as CP79's existing regression checks.

Static preflight passed: DB/delete/callback/display entry guards were checked
before effects;39 unique Release-active Testkit cases and main/macros were
verified; all quoted includes resolved against the actual include/src target
roots; new direct deletion/slot declarations were verified; diff whitespace was
clean. No compiler/build/CTest was run by this worker. The target is
`m2m_mdt_viewer_box`, CTest `m2m.mdt.viewer_box`. Root should include its existing
DB/cogging/display/loader focused targets because the guarded routes span those
source owners.

## Frozen release

Paths below are relative to source/apps/moproj2mst. SHA-256:

| File | SHA-256 |
|---|---|
| src/engine/engine_internal.hpp | d2ed7167152588552c4023a10d0f01e32fbbc677a42058e2e9cf834971ee67d8 |
| src/engine/viewer.cpp | da7f38ace35aa2edd244b1213e6b8d27a280e21f42bcaa00bcc803eeb0e777e6 |
| src/engine/db_load.cpp | 62dd38add3e0af1c00ed07827ff41cbf010064ca63cee6a085e4f9558b4d93a9 |
| src/engine/cogging_save.cpp | 816a907d7b7a82776d6c8911a9f89158209f5475327271fe2debe06d9ac48076 |
| src/engine/viewer_camera.cpp | 9deecc6d53f2dcd2c3d305ce46845bda9714f5e724d820e762206717992666a0 |
| src/engine/class_slots.cpp | 0470f3e60d1805745c8bb7b018b073e3e0cddb0b83dc0a8282fa31db84ab167d |
| tests/mdt/test_viewer_box.cpp | e5e9e94da0009655d791f250ea17b389279176886d3990acb831af0928b867e0 |

All seven files and this report are released frozen for root review/build.
GuiModel's public/private header and gui_model.cpp remain at CP79's repaired
hashes; no native receipt helper or other source file changed. Token measurement
is unavailable.

## Focused-control correction after root validation

Root reported a successful grouped build and five passing focused targets, with
viewer_box failing the nonpending control's expected null main after
displayShow(nullptr). The production entry was correct: after the new guard,
displayShow checks incoming m against the second/third QPointer values before
resetting main. Both fresh secondary slots are null, so incoming null matches
and returns, retaining the displayed main model and its backlink.

Only the focused test was corrected. It now requires the actual fresh prestate
(empty named list, null second/third guards) and asserts the unchanged main/
backlink. It then sets both secondary guards to distinct live nonpending models
and repeats the null call: neither secondary matches, so the existing main-null
assertion is exercised on its correct branch. The old backlink remains unchanged
on that branch too, matching the source. No production guard or display body was
changed or relaxed.

The test hash above is this corrected release; all six production/internal hashes
remain unchanged. Static preflight passed for39 cases, macros, includes/main,
direct displayModels declaration and whitespace. Root must rerun the focused
target; this worker performed no build/test execution.
