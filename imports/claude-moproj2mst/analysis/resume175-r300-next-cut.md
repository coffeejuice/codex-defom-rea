# CP175 R300 next raw-access cut: Save-local `stepOf`

## Decision

After the frozen `opStep` and `propsStep` callback cuts, the smallest next compileable raw-return closure is entirely in `source/apps/moproj2mst/src/engine/save_current.cpp`. Its local `stepOf(Project&, Operation&)` still returns `opModel(...)->step.get()` to five call expressions in that file. Remove that helper and use CP173's existing `withOpStep` callback at each original acquisition point. This cut changes no public C export or signature, does not install the Stopping barrier, and leaves CP163's cache0/A publication/after-A `CoordinateSync` refusal intact. It is an access migration, not permission to publish cache1 or to claim Save refusal on a parked model.

The source baseline inspected here is `save_current.cpp` SHA-256 `bc77d02830ac0c75ea5fa669f303b818178610291fdbda837f1a1abb08185129`; CP173's `engine_internal.hpp` callback seam is SHA-256 `f4e6627b808eebf31dbcf8aae0280fe1221bc50abbaa6f77639156ea704bcb0d`. CP174's passed source-stable suite is reported by root; this read-only plan did not rerun it or inspect a new original runtime occurrence. The retained original remains DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Static native zero-branch order does not supply an original public Open/Save witness for this refactor.

## Exclusive owner and source rules

Assign one source owner **only** `source/apps/moproj2mst/src/engine/save_current.cpp`. No shared header or C ABI file is needed; `engine_internal.hpp` is already included. A separate test owner may use `source/apps/moproj2mst/tests/s/test_save_io.cpp` if a missing-path assertion is needed; its inspected pre-cut SHA-256 is `83fec33ee033409b1c0faf90aa9e15d8d82200800a0c2b3c50e55367f745cafb`. Keep the test owner disjoint from the production owner. Connector175 owns `src/doe/task_factory.cpp`, `include/m2m/doe_task_runtime.hpp`, and `tests/mdt/test_doe_task_factory.cpp`, with `view_manager.hpp` only if required; none overlaps this cut. Other lanes should avoid these files through source-stable integration.

Migrate exactly the five `stepOf` calls, preserving their separate control flow:

| Caller | Required order and null behavior |
| --- | --- |
| `saveSimCtrl` | Acquire step first; return false on null before path, native open, scratch capture, flags and PPV. Keep native versus recorded writer, PPV, diagnostic and `FileEffects` order inside the callback. |
| `rebuildSystemContacts` | Acquire before `lubSystem` check and local step creation. Null returns without clearing inter-object state. Keep lubricant file, default-pair lookup and pair writes in the same order. |
| `saveObject` | Acquire before item lookup/path calculation. Do **not** add a null guard: the existing path reaches an unchecked `*step` only when a nonempty object path causes a writer call. Keep its existing empty-path false result and native/recorded writer behavior. |
| `saveMeshSettings` | Preserve one lookup for each slot admitted by `meshSetting`; null skips only that slot. Do not hoist the lookup outside the loop or move it ahead of slot tests. |
| `saveObjProperties` | Preserve one lookup for each existing slot record; null skips only that slot. Do not hoist or combine with another Save lookup. |

The callback must run synchronously, return only void/bool, and never store a step pointer or nested address. The relevant `captureKey` and `writeTaskKey` implementations invoke their writer callbacks before returning; confirm this remains true after editing. Keep all effectful Save work in the same enclosing callback where the old local pointer was live. A short callback that returns followed by use of a captured pointer would defeat the cut. Do not route these sites through `GuiModelAccess::withStep`, which adds a geometry refusal that the current `stepOf` lacks. Do not pre-gate `saveCurrentOperation`: some class routes reach directory or property effects before a model lookup under the current recovered order, and that broader barrier decision needs a separate review.

## Acceptance

Static gate: `rg -n 'stepOf\(' source/apps/moproj2mst/src source/apps/moproj2mst/include` finds no definition or call; five newly scoped `withOpStep` acquisitions remain at the former sites, with no callback returning or storing a step alias. Search the rest of `save_current.cpp` for `m->step`/`m.step` and list the remaining direct uses rather than claiming this file, Save, or `GuiModel::step` is closed. Source review must compare ordering of model lookup, slot admission, path computation, native opening, overlay invalidation, `FileEffects` recording, PPV writer and callbacks against the pre-cut hash. The nullable cases must retain false, return, continue, or existing unchecked dereference exactly as above; no new safe fallback may be inferred as original behavior.

Compile the changed core under a source hash, then run focused CTest `m2m.s.save_io`, `m2m.s.save_current`, `m2m.s.save_dispatch`, `m2m.mdt.simcontrol300_stopping_cached`, and `m2m.g0.abi`. In `m2m.s.save_io`, require the existing named native SimCtrl failed-open/PPV ordering, object writer flag/open behavior, task-key byte parity, and inter-object denial/contact-update cases to execute with the same effects and diagnostics. In `m2m.s.save_current`, preserve the asserted oracle-B step-2 cases; printed or skipped cases are not counted as parity proof. If these tests do not reach a nullable admitted slot, add one narrow fixture in `test_save_io.cpp` that observes a missing step's current return/continue and unchanged output effects without manufacturing an unsupported public route. Then run root's complete source-stable suite and freeze before releasing another source owner.

Rollback is the one-file production patch plus any paired test edit as a unit: restore `save_current.cpp` to the recorded pre-cut hash, restore the `stepOf` helper and five calls, rerun the focused group, and do not alter model A, cache11, fixtures, or any installed program. No original capture or C ABI rollback is required.

## Remaining boundary

This removes only the Save-local helper return. The same file still has direct `GuiModel::step` aliases in contact flags, inter-object, BCC, equipment and whole-model paths; `opModel`, `opSimCtl`, `GuiModel::simCtl/object/objectAt` and other files' direct step uses also remain. A later exclusive-file migration must close those paths, then make `GuiModel::step` private without moving its declaration, and establish a whole-operation gate before Save/file effects and cached success. The Stopping frame must install with a reviewed no-throw publication path before zero-length cache1; original same-occurrence reader-tail and public Open/Save fidelity remain unsupported.
