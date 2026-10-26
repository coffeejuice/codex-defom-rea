# CP88 — inherited Boolean property Save effect reconciliation

## Selected public-path effect

The Boolean properties Save slot writes Boolean.BWZ and DEF_MACHDIST_CTRL2.INI during File > Save step 4, before the MST writer opens its output. This is a source-qualified inherited property effect independent of Report's document owner. It was a live ordering gap in the CP71 audit and is now integrated in the full-Save candidate.

## Original contract and current source

The retained original Save dispatcher calls project Save, then operation-properties vt+0xb8 dispatch in OprManager order, then MST generation. The Boolean properties slot 0x1411e09a0 calls 0x1411e1820, whose side effects are the Boolean.BWZ and DEF_MACHDIST_CTRL2.INI files. The independent Boolean MST writer call is retained later in step 5; the two original call sites are not collapsed into one.

Current source/apps/moproj2mst/src/core/convert.cpp:365-390 walks operations in step 4 when withSave is true. For a BooleanProps with data, it invokes the existing booleanSideFiles slot at lines 381-384 before continuing other property writers. The nativeSave path uses direct file effects, then proceeds to MST generation. The Boolean MST writer's later invocation remains in src/apps/moproj2mst/src/ops/boolean.cpp.

This closes the specific ordinary bound-Boolean step-4 dispatch branch. It does not establish malformed/detached property-owner behavior. CP72's independent review also identifies a separate convertToBytes recording-mode question: because the supported original has two save call sites, the candidate may record duplicate same-path SideBytes entries; the intended return-vector coalescing/attempt semantics have not been qualified. Keep that API issue separate from the integrated filesystem Save effect.

## Concrete regression and validation

source/apps/moproj2mst/tests/s/test_save_io.cpp contains two public candidate regressions:

- save_failed_mst_open_keeps_step4_boolean_files_and_prior_save_effects requires the fixture-qualified Boolean.BWZ and DEF_MACHDIST_CTRL2.INI content even when the preexisting MST target cannot be opened for writing. It also checks the retained MST bytes and earlier Project Save effects.
- save_missing_mst_parent_keeps_step4_boolean_files requires both Boolean files while the missing MST parent remains absent.

The latest retained integrated test receipt, analysis/resume84-integrated-grouped-tests.json, records 247 registered tests, CTest exit code 0, and identical source-tree hashes before/after: a516dd460fec17492a8a86b842b02f04c89835494a790a8b6a2c61b21f19f3a4. Its log, build/test-runs/1792894387937632400/ctest.log, records m2m.s.save_io passed, with both named tests marked [ ok ]. This is candidate validation, not fresh original-runtime parity.

## Queue reconciliation

Resume46's broad Boolean Properties candidate is not a single settled behavior: the earlier dispatch validation closed only a broader order slice, and CP71 later identified this Boolean side-file step-4 omission. CP72 integrated the concrete branch and added the distinguishing tests; the CP84 source-stable suite passes them. Mark this specific step-4 Boolean side-file effect as integrated and candidate-tested.

Do not close RINHERIT/C13 as a whole. Other inherited candidates remain independently open or qualified, including Convert2Dto3D/CopyMirror load-time property application, XML grammar/UI-derived state, PPV supported-profile limits, and Group.Expanded's uninitialized native value. The separate recording-mode duplicate SideBytes question also remains undecided.

## Scope

Read-only source, test and retained receipt audit. No source/test/build/GUI/REA state was changed or run during this review.

