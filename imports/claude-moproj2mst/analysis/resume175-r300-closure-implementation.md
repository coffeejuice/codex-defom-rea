# CP175 R300 Save-local raw-return closure implementation

## Scope and hashes

Changed only `source/apps/moproj2mst/src/engine/save_current.cpp` for production: remove its local `stepOf` raw-return helper and migrate its five acquisitions to the existing synchronous `withOpStep` seam. This receipt is the second authored file, `analysis/resume175-r300-closure-implementation.md`. No test source was edited.

SHA-256:

- Production before: `bc77d02830ac0c75ea5fa669f303b818178610291fdbda837f1a1abb08185129` (verified before editing).
- Production after: `128d620cdf510e00b0fc8c77da9e061f8be7f599c4e29278626836e512f7ff72`.
- Existing `src/engine/engine_internal.hpp` seam unchanged: `f4e6627b808eebf31dbcf8aae0280fe1221bc50abbaa6f77639156ea704bcb0d`.
- Existing `tests/s/test_save_io.cpp` unchanged: `83fec33ee033409b1c0faf90aa9e15d8d82200800a0c2b3c50e55367f745cafb`.

Paths for the seam and test above are relative to `source/apps/moproj2mst/`.

## Static acceptance evidence

`rg -n 'stepOf\(' source/apps/moproj2mst/src source/apps/moproj2mst/include` produced no matches (exit 1). Exactly five `withOpStep` acquisitions now appear in the production file at lines 279, 500, 545, 565 and 599.

A read-only inverse transformation removed each callback wrapper, restored its original nullable local acquisition and loop `continue`, and restored the removed helper in memory. The reconstructed CRLF bytes matched the exact pre-cut SHA-256 above. This establishes that callback scope/indentation, the two per-slot null `continue` to callback `return` conversions, and removal of the helper (including its source-address comments) account for the entire production edit; the original rebuild acquisition's address comment is also omitted with that local declaration.

- `saveSimCtrl`: bool callback encloses all original work. Null returns false before path calculation, flags, native open or scratch capture. Native open/truncate, overlay invalidation, block writer, ignored close, recovery-list check, PPV open/write and final result remain in their original order.
- `rebuildSystemContacts`: void callback begins before the null and `lubSystem` checks. Null does not allocate the local step or clear inter-object data. Lubricant import, default-pair resolution, object lookup/lubricant writes and pair append ordering remain unchanged.
- `saveObject`: bool callback begins before object lookup and path calculation. There is deliberately no null check. Empty path returns false; admitted writer callbacks retain the existing unchecked `*step`. Native versus scratch/open/flag ordering remains unchanged.
- `saveMeshSettings`: one callback per slot admitted by its existing record/mesh-setting tests. A null callback return ends that slot's work and the outer loop proceeds. No lookup was hoisted.
- `saveObjProperties`: one callback per existing slot record; the same per-slot null behavior and writer order are retained.

The callback parameter type is explicitly `def::model::DB_STEP_DATA*` at all five sites. The existing included `engine_internal.hpp` restricts callback return types to void or bool and invokes the callback directly with `std::invoke`; these sites supply exactly those return types. This is a compile-visible type audit, not a compilation result.

No parameter pointer or nested address is returned/stored for later work. Nested writer lambdas capture by reference only within the enclosing callback. `captureKey` directly invokes its writer before checking/recording scratch bytes; `writeTaskKey` calls `captureKey` synchronously in recording mode and directly invokes the stream writer in native mode before closing/returning. Neither retains the writer. The existing `beforeNativeKeyWrite` hook receives only the target path. The contact pair/object aliases and temporary local step remain within the enclosing synchronous callback lifetime.

## Retained boundary and validation limitations

Direct step uses remain unchanged at current lines 337 (`contactFlagsFromEngine`), 394 (`classDefaultPairs`), 530 (`saveInterObject`), 582/585 (`saveBcc`), 641/642 (`saveMaterials`), 729 (equipment call in `syncOperationFiles`) and 734 (whole-model call in `syncOperationFiles`). This closes only the five local helper returns. It does not close all Save aliases or `GuiModel::step`, install a Stopping barrier, publish cache1, add geometry refusal, or alter the public C ABI.

No build, test, freeze, GUI interaction, live mutable REA operation, shared-state change, executable allocation or deployment was performed by this owner. Existing test sources include native task-key byte parity and object-open/flag cases, but this owner did not execute them or establish nullable admitted-slot coverage. Integration must compile the changed core under the recorded source hash, run the focused group from `resume175-r300-next-cut.md`, resolve whether a separate narrow missing-step fixture is needed, then complete the root source-stable suite and freeze. No new original public Open/Save fidelity claim follows from this static refactor.

Production and test ownership is released to root after this receipt; no further edits are planned by this owner.

## Follow-up: admitted null-step slot regression fixture

Root reported the CP175 focused Save group passing after the production edit. Independent review then identified missing admitted null-step slot coverage. This addendum supersedes the earlier statement that no test source was edited or nullable fixture was supplied; execution remains root-owned.

Added `save_missing_step_skips_admitted_slot_writers_without_file_effects` at `source/apps/moproj2mst/tests/s/test_save_io.cpp:1408`. It reuses `TaskKeyRig` and an existing positive object slot, sets that record's mesh-setting flag, then temporarily moves the selected model's public step owner into local storage. It confirms the operation getter still selects the same model, directly calls the existing internal SimCtrl/MeshSettings/ObjProperties helpers in both native and recording modes, and restores the step. Test-local declarations expose the two already externally linked slot helpers without modifying production headers or the C ABI. This is explicitly an internal nullable-state fixture, not an invented public Open/Save occurrence.

The assertions require false from SimCtrl, no native write hooks, no deferred or directory/removal/rename effects, unchanged overlay, and unchanged sentinel bytes in all three target files. The existing slot record and enabled mesh-setting flag ensure the changed per-slot lookup sites are admitted before their null checks. The test does not invoke `syncOperationFiles`, whose other unchanged unchecked aliases would make that synthetic whole-operation route unsupported.

Test-source SHA-256 after this follow-up: `b3bb63a55b79d9b530f360338e8398adb36a141bf95587ddf02e3a236a112fd4`. Production remains `128d620cdf510e00b0fc8c77da9e061f8be7f599c4e29278626836e512f7ff72`. No compilation or test execution was performed by this owner. Root must compile/run the augmented Save test and relevant integration gates. Only the test file and this report were edited in the follow-up; ownership is released again.
