# CP257: refresh shown secondary and third models before fitting

The next dependency-ready scene leaf is the missing secondary/third refresh prefix in `viewer_camera.cpp::fit`. Original `0x140cfe9c0`, in its scene-camera branch (`Display+0x708 == 0`), refreshes live shown main, secondary, and third models in that order before querying radius or scene boxes. Current converter refreshes only main. This is a real effect omission, independently of the still-incomplete multi-model camera-box union.

Release a bounded implementation in `source/apps/moproj2mst/src/engine/viewer_camera.cpp` and actual-caller tests in `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` after root confirms source ownership. No header/model/scene-creator edit is needed. Only this report was written; no source/test edit, test/build run, original execution, GUI control, mutable REA session, or shared-state change occurred.

## Exact implementation contract

At current `viewer_camera.cpp:268` (`fit`), retain the main refresh and add the two omitted slot checks immediately after it, before `sceneBox(d)`:

```cpp
if (d.main.get() && d.mainShown) viewerRefresh(*d.main.get());
if (d.models.second.get() && d.models.secondShown)
    viewerRefresh(*d.models.second.get());
if (d.models.third.get() && d.models.thirdShown)
    viewerRefresh(*d.models.third.get());
```

The existing `Display` owner already stores these guards and flags. Use that state directly inside its implementation. Do not expose a retained borrow or add another source of ownership. Keep main-before-secondary-before-third order, guard-before-flag short circuits, independent slot visits, and all refresh calls before box/radius/window/reset/origin work. Do not deduplicate aliases: the native function visits slots, even if they identify the same model. Do not refresh named models in this prefix; original fit has exactly these three refresh branches. Do not set shown/keep flags, replace models, create absent slots, catch refresh errors, or roll back a completed earlier refresh.

Keep the existing `viewerRefresh` semantics. In particular `viewer.cpp::refresh` returns immediately for a pending geometry barrier, and can also suspend during a geometry-item refresh. Adding a preflight `requireModelGeometrySyncAvailable` for every slot, or inventing a new stop/continuation condition after a return, would change the existing adapter contract. This leaf merely routes the two missing native calls through the current refresh implementation; it does not recover suspended native continuation or authorize blocked geometry reads.

`displayRefreshScene(Project&, GuiModel*)` around lines 426–431 is the primary test caller: existing display admission, `displayShow`, then `fit`. `displayResetView` around lines 434–473 also calls `fit`, after its existing dimension/rotation decision. Keep both callers and their admission/order unchanged. In particular, do not bundle a reset-view active-model selection repair.

## Original evidence and effect order

The retained full decompile is `evidence/gui_mo/s1/viewer_rect/a1.c`, `FUN_140cfe9c0`, around lines 486–570. Its scene branch performs:

| Native sequence | Meaning |
| --- | --- |
| `cf94f0`; `cf9730`; repeated `cf94f0`; `c8e640` | Live shown main refresh |
| `cf9960`; `cf9cb0`; repeated `cf9960`; `c8e640` | Live shown secondary refresh |
| `cf9d20`; `cf9f90`; repeated `cf9d20`; `c8e640` | Live shown third refresh |
| `2c8170(APP)` | Main-window scene availability check, after refresh effects |
| `cfe0f0`, `cfd750(...,0)`, camera setters | Radius/box/camera work after the prefix |

Independent immutable PE disassembly confirms the relevant sequence at `0x140cfec06..0x140cfecb7`: main call `0x140cfec3c`, secondary call `0x140cfec77`, third call `0x140cfecb2`. The following main-window query starts at `0x140cfecb7`. Thus these effects are not conditional on the later global scene-presence result. The converter already assumes a present scene and retains its current profile; no new global-availability behavior is proposed.

Original target: retained **DEF_GUI_MO_64.exe v11.0.2**, `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. DUMPBIN 14.44.35228.0 decoded only immutable file bytes; no original process or analysis database was opened. PE-section-mapped byte SHA-256 for half-open refresh-prefix range `[0x140cfec06,0x140cfecb7)` is `691dbba5f48bb6066126a1901a1834c2a42719e54a94040f5b7fde4e03f401e3`. Full retained `a1.c` SHA-256 is `96f49d3eed9eca5866326ac00ad730dd3e91f2bb92c051aa479d53536a984e35`.

This establishes native call order and gates statically. It does not establish a naturally reached fresh-project secondary/third occurrence, literal final multi-model camera values, Qt callback/reentrancy behavior, or equality of native and reconstructed geometry refresh internals.

## Meaningful actual-caller tests

Reuse `test_viewer_box.cpp`'s existing `geometryLine` helper around line 292 and cached-geometry test `geometry_item_rect_refresh_and_missing_source_keep_original_state_order` around line 584. That fixture builds a bound curve-only 3D view, caches its rectangle through `viewerSync`, changes the curve coordinates, and observes a changed rectangle only after a refresh. It needs no optional cross-section mesh, corpus rewrite, geometry owner fabrication, or external file.

Add a focused test using the actual `displayRefreshScene` caller:

1. Show a stable ordinary 3D main in a `Project`. Obtain the canonical owned secondary through `displaySecond`. Create a locally owned third model and connect it through `displayModels(display).third`; keep its owner alive through the test. Set up distinct curve-only cached boxes on main/secondary/third, then mutate their native curve coordinates without rebinding or refreshing. Read `viewerSceneBox` first to establish that each cache still contains its old extent.
2. With secondary and third shown flags false, call `displayRefreshScene(project, &main)`. Verify main changes to its new extent while both hidden-slot caches retain their old extents. This proves the flags gate effects, rather than merely checking returned values from a helper.
3. Set secondary shown true, leave third hidden, call the same production entry point, and verify secondary updates while third stays stale. Mutate secondary again, enable third, call again, and verify both update. Use independently distinguishable extents. `viewerSceneBox` reads the cache; do not accidentally invoke a refreshing helper between mutation and the production call.
4. A live shown third with an expired secondary guard still refreshes; an expired shown third guard is skipped safely. Use an independently retained `GuiModelGuard`/normal owner destruction where useful; never dereference a destroyed borrowed pointer. Keep the canonical secondary owned by the display, and use `displayDropSecond` for its explicit teardown when required.
5. A flagged named model is outside this refresh prefix: if covered, seed and mutate an ordinary live-step named model and verify this prefix does not refresh its cache. Avoid the excluded named-map-miss/no-step state. This is optional if it would expand the fixture disproportionately.

The direct successful caller cases are sufficient for this bounded repair. Preserve source order in review; do not add a production event observer solely to count slot calls. A failure-prefix test is useful only if the existing suite supplies a deterministic non-barrier refresh error: assert already-completed main/secondary cache changes remain and later slots/camera work have not run. Do not use a parked barrier as a claimed throwing `viewerRefresh` fixture, because current refresh returns for that state. Do not invent a geometry failure trigger or claim native exception parity from an adapter error.

The test must inspect per-model cached boxes, not claim secondary/third contributions appear in the final camera. Current `sceneBox` around lines 253–264 still unions only main, whereas the retained native `cfd750` also has other slot/named branches. Repairing that union and native radius `cfe0f0` is a separate leaf requiring its own order/arithmetic/owner review. This prefix is useful despite that remaining limitation: later legitimate readers and save paths observe the refreshed model caches, and omitted refresh effects are repaired at their actual owner caller.

## Ownership, preflight, and integration

Production ownership: only `source/apps/moproj2mst/src/engine/viewer_camera.cpp`. Test ownership: only `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`. Existing includes already provide `engine_scene.hpp`, `engine.hpp`, native curve/step definitions and model APIs; avoid new includes unless required. Keep the existing Testkit `main`, `CHECK`/`CHECK_EQ`/`REQUIRE`; no C `assert` or unsupported `REQUIRE_EQ`.

Configured target/CTest confirmed read-only in candidate metadata: `m2m_mdt_viewer_box` / `m2m.mdt.viewer_box`. Root should run `admin/check_new_cpp_bindings.py` for the changed test, build/run this focused target, group with `m2m.mdt.display_selection` and relevant scene regressions, then complete the source-stable launcher/freeze with owners held. No build or test has been run by this audit.

Inspected source SHA-256: `viewer_camera.cpp` `9deecc6d53f2dcd2c3d305ce46845bda9714f5e724d820e762206717992666a0`; `test_viewer_box.cpp` `0c45caec6aea584a957d6bcd567c9964cbd346153cd5e67fadd0a994aedd9527`. Recheck these and ownership before implementation because other checkpoints may be active.

## Reconciliation and rejected shortcuts

CP250 migrated named creation's live-step dimension read; CP252 corrected secondary absent-step dimension to 3; CP254 now publishes secondary ownership before backlink/control access. Current `scene.cpp` contains all three changes. This proposed fit prefix does not duplicate or alter them. CP225's unresolved live ObjectKey/storage-generation routes remain unresolved; curve pointers in this test are finite fixture-owned construction/mutation state, not an issued currentness token.

The inspected CopyMirror load (`l5_slots.cpp:696–714`, native `0x140e24b80` in retained `L5/d0.c`) explicitly clears keep, drops a 2D second, then replaces only when its local second is absent. Native setter `cf9930` only changes the guard, while the converter `displaySetSecond` also owns/deletes replaced storage. That broader replacement/lifetime difference is not demonstrated on this canonical caller, which drops first. Do not manufacture a replacement discrepancy by mutating inconsistent public owner/guard fields. Moreover CopyMirror is absent from the original GUI registry and existing tests insert it explicitly, so it is a weaker reachable successor than the scene fit omission.

The reset-view `shownDim` shortcut also differs from native active-model selection, but it is not a safe one-expression migration. Immutable disassembly of `cfaec0` calls `cfac50` before the default-3 model getter; `cfac50` includes main/third/named/secondary priority and a property-table-driven secondary override via `d0c920`. Existing `doe/statevar.cpp:198–203` already refuses that unresolved populated-table selection profile. Neither `displaySelectedModel` nor main alone represents that owner contract. Byte SHA-256: `[0x140cfaec0,0x140cfaeff)` `d7f6eadcecb640cca98a3f1641cfedad2885296f0460b83927dffd92e49e454a`; `[0x140cfac50,0x140cfae3c)` `08ef4d33acd954fcd806f5cb3d876265b0c28dff342f6592adca637380dba4c3`. Keep that larger selection/property-table dependency separate, along with the named no-step exceptional path and RGEOM's missing optional nested-owner occurrence.
