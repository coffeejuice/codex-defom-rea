# CP80 geometry barrier source review

## Result

The expanded barrier now stops the identified entrypoints before their first model/display effect. In particular, the loader callbacks have the required guard placement: `g_loaderSlots.setDimension` calls `requireModelGeometrySyncAvailable(m)` before `set_dimension`, and `g_loaderSlots.showModel` is bound directly to `displayShow`, which checks both the incoming model and any currently displayed model before creating/updating the display.

`displayResetView` also checks the outgoing displayed model before calling `projectDisplay`, changing the quaternion, or fitting the camera. This covers the previously easy-to-miss null-incoming case: reset-view acts on the current model's display even though it accepts no model argument. The pending regression calls `displayResetView` against a project whose main model is parked, then checks display identity, model backlink, and camera state are unchanged (`test_viewer_box.cpp:1927-1930`).

## Earliest-effect checks

| Entry | Guard and first effect | Assessment |
|---|---|---|
| `readDbStep` | Gate at function entry, before DB/step queries (`db_load.cpp:380`) | Correct. |
| `readDbEntry` | Gate at entry, before `viewerClear`, `u1b58`, DB reader or DB read (`db_load.cpp:393`) | Correct, including `resetViewer == false`. |
| `modelDbReader` | Gate before returning the mutable reader (`db_load.cpp:249`) | Prevents fresh reader acquisition while parked; pre-held reader aliases remain outside this guarantee. |
| `deleteObject` | Gate before validating/deleting the object (`cogging_save.cpp:255`) | Correct; prevents deletion before a later PDIE check. |
| `setDimension` callback | Gate before the native dimension call (`class_slots.cpp:182-184`) | Correct. The local scalar construction is effect-free. |
| `displayShow` | Incoming and current-main checks precede `projectDisplay` (`viewer_camera.cpp:403-414`) | Correct for both incoming and outgoing parked models. |
| `displayRefreshScene` | Checks before `displayShow`, then calls the guarded helper again (`viewer_camera.cpp:427-431`) | Correct; no display association or fit occurs first. Redundant checking is harmless. |
| `displayResetView` | Outgoing-model check before `projectDisplay` and any camera write (`viewer_camera.cpp:434-436`) | Correct. |

The null `displayShow` / `displayRefreshScene` case still checks the outgoing display main model before clearing it or fitting. When there is no current main model, the helper is a no-op and the original lazy display creation path remains available.

## Behavior and test coverage

The existing nonpending fixture verifies ordinary `deleteObject`, `setDimension`, `readDbEntry`/`readDbStep` on an empty model, and direct `displayShow` setup/clear continue to work (`test_viewer_box.cpp:1768-1785`). The pending fixture verifies `readDbEntry` and `readDbStep` refuse with step/table/timer state unchanged, deletion and `setDimension` refuse, and all display calls—including the `showModel` loader callback, outgoing-null refresh/show, and `displayResetView`—leave associations and camera unchanged (`1878-1930`).

One narrow coverage improvement remains: the nonpending test invokes `displayShow` directly but does not invoke `g_loaderSlots.showModel` itself, and it does not call `displayRefreshScene` or `displayResetView` on an unparked model. These callbacks are direct function bindings, so source inspection makes their ordinary behavior strongly predictable; a tiny nonpending callback assertion would still protect loader wiring and the normal camera path. This is a test-coverage suggestion, not an observed functional defect.

The check concerns model-owned geometry synchronization only. It does not close public raw-field mutation or invalidate pointers acquired before parking, as documented by the barrier's internal comment and prior audit. No source/test files were edited and no builds or tests were run for this review.
