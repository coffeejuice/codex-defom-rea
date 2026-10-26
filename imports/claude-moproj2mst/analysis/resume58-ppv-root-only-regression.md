# Checkpoint 58 root-only PPV Save regression

## Correction from the retained native gate

The immutable original capture in `analysis/resume58-ppv-save-root-gate.md`
resolves the predicate: the native PPV writer tests `QListData::isEmpty()` on
the **operation-local** point list. If that list is empty, the helper returns
false before QFile construction/open, irrespective of nonempty shared/root
points or flags. The caller has already written `SimCtrl.KEY` and ignores the
PPV helper's boolean. Therefore root-only state is not a missing-root-row
serialization case and must not be changed into an unsupported refusal merely
because the root list is nonempty.

## Test added

Added `tests/mdt/test_ppv_root_only_save_guard.cpp`, discovered by the existing
test glob. It opens the retained disposable `93_B_MB3ACT_before/Gear_blank`
project, selects the real active operation, leaves its actual local
`recoveryB` empty, and supplies one shared point through the owning Project's
`recoveryA`. It calls the existing `saveCurrentOperation` seam and asserts the
qualified current status: the call completes without an Error, a
`SimCtrl.KEY` effect is present, and no `SimCtrl.PPV` write effect is generated.
The effects are in-memory `FileEffects`; the test does not create or alter
corpus files.

This is a characterization of the native local-empty gate and current
candidate observable behavior, not a refusal test and not proof of the full
writer or root-list path. The original writer's skipped QFile means an existing
PPV file is untouched by that helper; this candidate effect assertion does not
inspect unrelated later Save phases or a physical preexisting file.

## Validation and scope

The test uses Testkit `REQUIRE`/`CHECK` macros, which remain active in Release.
No build or test was run in this task. No production source, CMake, GUI, mutable
REA state, or corpus file was changed. No claim is made about malformed
native owner shapes, nonempty local-list writer payloads, physical I/O failure,
or fresh original-session acceptance.
