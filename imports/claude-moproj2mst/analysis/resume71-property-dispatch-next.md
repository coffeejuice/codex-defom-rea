# Checkpoint 71: Save property-dispatch gap

## Selected source-closed branch

The smallest unimplemented reachable property-save branch is `SfOprBooleanProperties::vt+0xb8` (native `0x1411e09a0` → `0x1411e1820`), which writes `Boolean.BWZ` and `DEF_MACHDIST_CTRL2.INI` during File > Save step 4. The candidate currently writes those files only from the Boolean operation MST writer, later in step 5. The helper and file-format implementation already exist; the missing work is the earlier step-4 dispatch in `source/apps/moproj2mst/src/core/convert.cpp`.

This is a source-supported ordering gap with a concrete test target. It does not require implementing the Boolean writer again or changing public guards. The retained source marks the native slot out of scope in `source/docs/re/moproj2mst/status.md`, but the decompilation in `s1-04/spec.md` and existing candidate `booleanSideFiles` implementation are sufficient to bound the path.

## Native and candidate path

Native Save step 4, `0x1400982e0`, walks OprManager rows in order and calls properties `vt+0xb8` on each operation that has properties (`source/docs/re/moproj2mst/mdt/spec.md` §1). `SfOprBooleanProperties` owns a `vt+0xb8` slot (`0x1411e09a0`); `source/docs/re/moproj2mst/s1-04/spec.md` §1.2 resolves its body `0x1411e1820` to `Boolean.BWZ` and `DEF_MACHDIST_CTRL2.INI` writes. The Boolean writer independently calls the same helper while generating its MST block in step 5, after `KFWRIT/Step.KEY` and before `EXECMD` (s1-04 §1 and `src/ops/boolean.cpp`). These are two native invocations in a full Save; the later MST-time write does not erase the earlier ordering requirement.

The candidate's full-Save step-4 loop in `src/core/convert.cpp` tries `saveCoggingMdt`, then `l6PropsSave`, then `expressPropsSave`. Boolean properties return false from those dispatchers, so no Boolean step-4 effects occur. `booleanSideFiles` exists in `src/engine/boolean_editor.cpp` and is wired to `g_l5Slots.booleanSideFiles`, but `src/ops/boolean.cpp` invokes that slot only during the MST writer when `w.sideEffects` is present. Thus the current candidate can generate the final files on a successful MST path while missing the earlier Save-step effects.

## Reachable fixture and discriminating existing test

`tests/s/test_save_io.cpp::Rig` defaults to a disposable copy of retained `133_B_DBSTEP_BOOL3_before/COG_P1_B1.moproj`. That project has a `SfOprBoolean3D` operation (ID 2) and a Cogging operation (ID 3). `Rig` sets its target task directory to `Operations/Task00002` and observes `Boolean.BWZ` and `DEF_MACHDIST_CTRL2.INI` there.

Two existing tests currently encode the candidate's later-only behavior:

- `save_failed_mst_open_keeps_prior_save_effects_but_skips_boolean_writers` locks the MST output so its step-5 open fails, then asserts that both Boolean side files do not exist.
- `save_missing_mst_parent_does_not_create_it_or_run_boolean_writers` uses a missing output parent and also asserts that both Boolean files do not exist.

Under the recovered native order, both assertions conflict with Boolean-properties step 4: the file writes are attempted before the step-5 MST output is opened. An implementation successor can use these tests as the focused discriminant and update the expected earlier Boolean effects while retaining the MST failure result and existing side-effect ordering. This is a candidate test contract derived from static source; it is not a fresh original GUI run or proof that every filesystem failure behaves identically.

## Bounded implementation/test contract for CP72

- Exclusive production target: `source/apps/moproj2mst/src/core/convert.cpp`, at the existing step-4 per-operation dispatch.
- Existing implementation to reuse: `source/apps/moproj2mst/src/engine/boolean_editor.cpp::booleanSideFiles`, exposed through `g_l5Slots.booleanSideFiles` in `source/apps/moproj2mst/include/m2m/mst/loader_l5.hpp` and installed in `source/apps/moproj2mst/src/engine/l5_slots.cpp`.
- Trigger only for an operation whose `BooleanProps` and `data` satisfy the same owner/data assumptions as the current Boolean writer path. Preserve OprManager row order and continue-on-open-failure behavior for each side file.
- Keep the independent step-5 Boolean writer invocation. Native evidence has both the property-level Save dispatch and the Boolean MST writer call; a single final byte comparison on a successful Save cannot prove the two calls are equivalent.
- Focused candidate check: on the disposable retained 133 fixture, lock the MST output against opening, run full Save, require the existing write-error result, and require that the two Boolean files were attempted/written before the failure where task directories and file opens permit it. Also check that a later step-5 writer failure path retains the already-written step-4 files. Do not claim original parity without a matching original run.
- The candidate's existing `ctx.nativeMstSideFiles` and `booleanSideFiles` direct-write mode is currently reached only from step 5. The implementation must qualify file-open semantics and avoid accidentally staging/replaying the same writes in the native Save path. Do not change public guards or claim that matching final bytes alone closes ordering.

## Evidence and limits

- Native property loop and Save order: `source/docs/re/moproj2mst/mdt/spec.md` §1, SHA-256 `607731629D7382770222FC223E5174FB14D82B9F7D7010C0126566CCEDBF7A4C`.
- Native Boolean writer's `vt+0x280` call to `0x1411e1820` and Boolean side-file body: `source/docs/re/moproj2mst/s1-04/spec.md` §1 / §1.2, SHA-256 `2E1F9F7B75F49B4E3BCEC811755C4C0666E217CCBA8864CF2128E58268B93790`.
- Candidate step-4 dispatch: `source/apps/moproj2mst/src/core/convert.cpp`, SHA-256 `5A167FCDFD8326E80377D019BD7A53B87B7F378A031E5863F804C2D0754DEC33`.
- Candidate Boolean step-5 call: `source/apps/moproj2mst/src/ops/boolean.cpp`, SHA-256 `4D881B968CA13EB9E78FB57114B4E062358F870E2A075F7C9CE7740CECB2AB8E`.
- Candidate file output implementation: `source/apps/moproj2mst/src/engine/boolean_editor.cpp`, SHA-256 `5AAB83CB71D5EEDC69AD1476C4E5FF2ADC566687655694E59E3C3D949FF9A44F`.
- Existing fixture and currently conflicting failure-order assertions: `source/apps/moproj2mst/tests/s/test_save_io.cpp`, SHA-256 `229102E09DFBCDBB9A4F24EFD19E45EADDC13637779EBF86B991B034D1C2A3E4`.
- `source/docs/re/moproj2mst/status.md` lists `0x1411e09a0` as out of scope. The candidate's internal helper port does not constitute a fresh original comparison.

Read-only inventory only. No source, build, GUI, fixture, or mutable REA database was changed or run.
