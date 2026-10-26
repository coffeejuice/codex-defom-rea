# CP179 R300 Diestress KEY Save implementation

Status: source-ready, not built or executed. Ownership of the paired source files is released to root for integration. This worker edited only the pair below and this report; no shared state, frozen candidate, build, test run, GUI, live REA or deployment was changed.

## Change and source audit

`source/apps/moproj2mst/src/engine/save_l6.cpp`, only `diestressSaveKey`: replaced the function-scope raw step alias with an immediately invoked local lambda taking `DB_STEP_DATA*` and explicitly returning `void`. It receives `m->step.get()` once. The nullable borrow covers only the synchronous KEY capture.

Static inspection confirms:

- Exactly one `opModel(p, op)` lookup remains, with its null return first.
- `activeSubProject`, `subProjectDbPath(..., true)` and `.KEY` suffix construction remain before acquiring/checking the step.
- The callback returns on null before invoking `captureKey`; no scratch/file operation is introduced for that branch.
- `captureKey` is unchanged: remove scratch, invoke the writer inline, read/store existing scratch bytes, remove scratch. Neither callback nor pointer is retained, returned or queued.
- The inner writer still calls `write_key_file` exactly once with `const std::uint32_t flags = 0`, ignoring its result.
- No `withOpStep`, extra model lookup, geometry admission/refusal, header/layout or C signature change was introduced.
- CP163 SimControl and cache0/A source is unchanged. No cache1 authorization or behavior claim follows.

## Focused fixture preflight

Added `diestress_direct_save_null_step_has_no_effects_then_restores_writer` to the existing `test_l6_slots.cpp` executable. It opens/reclasses the configured copied corpus, loads Diestress, verifies the actual cached model has a step, temporarily moves that step out solely for direct `diestressSaveKey`, and restores it on both normal and exceptional exits before project destruction. It checks empty writes/deletes/mkdirs, restored model/step identity, then calls the direct helper again and requires one nonempty expected project DB-base `.KEY` write with no deletes/mkdirs and unchanged model/step identity.

This is an internal synthetic null-step fixture, not evidence of a public Save null-step route. The existing positive public Save fixture remains unchanged.

Preflight inspected the existing Testkit `main`/registration and supported `CHECK`, `CHECK_EQ`, `REQUIRE` definitions; no C `assert`, new quoted include or new dependency was added. `FileEffects` has each referenced member; existing headers declare the helper and complete step type. CMake glob registration maps `tests/mdt/test_l6_slots.cpp` to `m2m.mdt.l6_slots` and links `m2m_core` plus Testkit. No test or compiler execution occurred. Root must run the focused acceptance group, complete source-stable suite and freeze prescribed in `resume179-r300-next-cut.md`.

## SHA-256 and remaining risk

| File | Before | After |
| --- | --- | --- |
| `source/apps/moproj2mst/src/engine/save_l6.cpp` | `900a1e46a36a3925443b2d5937a234686b6708fe0aa54a066a159366c7da266e` | `3d0d2bc3c5ffc9e86b9504d46f3c9cd8273dc74a711c1c3c4e629c060410f9e4` |
| `source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp` | `9e1f3f5d8a031e27dbd3fd2a058940fa1549ffae921aa55c9b37412f481c768f` | `a9d53b00ec11a9d17a883cded8f541df9d373c41f6924496a0136af514ee262f` |

Unchanged checked fingerprints: `include/moproj2mst.h` = `34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40`; `src/engine/simcontrol300.cpp` = `67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae` (both relative to `source/apps/moproj2mst`).

The remaining immediate risk is uncompiled/unexecuted source and fixture integration. This cut does not close public `GuiModel::step`, other raw aliases, lifecycle/viewer or the parked-model gate. Rollback is confined to the paired files at their verified before hashes. Source inspection is the evidence for the narrow call-order claim; no new original behavior evidence was acquired.
