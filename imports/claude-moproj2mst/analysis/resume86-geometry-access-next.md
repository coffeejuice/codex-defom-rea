# CP86 next bounded step consumer: property-table current-object refresh

Select only `finalizeStepDataPropertyTable`'s nullable native
`step_current_object(...,true)` call in viewer.cpp. It is a synchronous mutation
whose result is ignored, and it fits the existing void withStep policy. End the
borrow immediately after that call, before table row iteration. This read-only
plan changes no source while CP85 is frozen.

## Proposed exclusive write set

- `source/apps/moproj2mst/src/engine/viewer.cpp`: replace this single nullable
  raw-step call with a void-result withStep callback at exactly its current
  position. The private helper include already exists from CP85.
- `source/apps/moproj2mst/tests/mdt/test_viewer_property_table.cpp`: strengthen
  the current-object prefix expectation and add ordinary missing-step coverage.
- `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`: add actual parked
  finalizer refusal to the existing real directory-query barrier fixture.
- This report for any later authorized implementation/static receipt.

Root must freeze CP85 and reserve these files before source editing. No helper,
header, CMake, native field/layout, PropertyTableState or public API change is
needed. Keep viewerRebuild, modelDim, geometry readers and other raw step paths
explicitly legacy. Their wider lifetimes and ordering are unnecessary here.

## Retained original evidence and exact effect order

The retained private read-only static export
`analysis/wave2-doe-stepdata-statevar-finalizer-decompile.json` identifies
original DEF_GUI_MO_64 SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Its completed, error-free function140d7c260 calls:

1. `FUN_140c82cb0(*(param_1+0x50), snapshot)` to copy current View pointers.
2. `FUN_140c7b1a0(*(param_1+0x50),1)` to refresh current object.
3. Iterator construction and View eligibility/owner/Settings lookup, followed
   by eligible table-cell queries. The constructor-empty/null-item path performs
   no checkbox flag read, write or itemChanged emission.

The bounded retained `wave2-doe-stepdata-primary-iteration-c7b1a0.asm` covers
0x140c7b1a0..0x140c7b1e7: obtain actual engine via c7acd0; null returns0;
nonnull obtains it again and invokes0x140823ca0 with the supplied refresh byte.
The existing source projection uses `def::pre::step_current_object` marked
`@src1401caa00` in `source/port/def_pre/src/menu56_common.cpp:142`.
These are retained static qualifications, not a new runtime observation.

That native helper refreshes actual S+0x1b4c. If last-object-number is less than
the current ID, it writes0. Only the separate `else if(cur==0)` branch selects
first deformable, then first rigid if needed. It does not immediately re-pick
after resetting an out-of-range nonzero ID. Preserve the real call and its
partial effect rather than duplicating or normalizing this logic.

The current adapter order is earliest GeometrySync availability guard, exact
table owner check, View-pointer snapshot, nullable current-object refresh,
then row/column iteration. Maintain the owner rejection before snapshot/native
effects, and preserve that the refresh may commit before a later unrecovered
cell refusal. The original pointer snapshot and all later Settings/item checks
stay outside the scoped borrow. Proposed replacement only:

```cpp
GuiModelAccess::withStep(model, [&](DB_STEP_DATA* nativeStep) {
    if (nativeStep) (void)def::pre::step_current_object(nativeStep, true);
});
```

The existing helper checks the model barrier before acquiring its actual nullable
borrow. Callback returns void; no pointer/reference is returned or saved, no
integer-result policy is added, and no native default owner is allocated.
Ignore the same native return value as today. Keep the finalizer's existing
entry guard: it precedes both owner checking and snapshot allocation.

## Small focused test additions

`test_viewer_property_table.cpp` currently has five direct cases covering
unbound/kind7 exclusion, eligible row numbering/flags, the first unqualified
cell refusal after refresh, and foreign-owner rejection before mutation.

1. In `property_table_unrecovered_population_refuses_at_first_cell_after_refresh`,
   strengthen `u1b4c != 999` to exact `u1b4c == 0`. The model has an actual object,
   so this catches incorrect reset-and-repick. Retain the first query0,1,
   queryCount1, object/view counts and refusal expectations.
2. Add an ordinary same-owner/no-step/no-viewer finalizer control: create the
   actual model/table pair, retire only that disposable model's step, invoke,
   and assert no new step/Viewer, no table queries and same table owner. This
   exercises the actual nullable borrow without a fabricated object or owner.
   Optionally add actual current0/valid-current rows to the existing eligible
   case only if an independent expectation needs coverage; no new native stub.
3. In the actual parked model fixture in viewer_box, create a local
   PropertyTableState bound to that model and call finalizer through its existing
   refusal ledger. Require queryCount0 and empty query prefix, with the existing
   native-step bytes, View identity/counts, timers, global tables and barrier
   state unchanged. Add the real property-table header include; no API hook.

Focused targets are `m2m_mdt_viewer_property_table` / CTest
`m2m.mdt.viewer_property_table` and `m2m_mdt_viewer_box` / CTest
`m2m.mdt.viewer_box`. Current counts are5 and40. CMake's existing test filename
registration generates these targets; no registration edit is required.
Existing DOE StepData integration tests remain useful grouped validation, but
this slice does not admit populated checkbox behavior or alter readStepData's
earlier child-import effects. A direct finalizer guard does not newly promise
an all-or-nothing guard for the complete outer DOE parser.

## Static, layout and ABI gates

Balanced finalizer-body inspection must show exactly one withStep/native
current-object call, zero raw `.step`/`->step`, void callback and no borrow escape.
Check order: earliest geometry guard, owner rejection, pointer snapshot, scoped
native refresh, row iteration and cell refusal. Reversing only that replacement
must restore the complete initial viewer.cpp hash. No call or output replay.

Keep CP81 compile gates for supported void/bool and rejected pointer/reference
callback results, DB_STEP_DATA size0x3408, GuiModel step type/nonpolymorphism,
APIversion1 and six C function signatures. Native object/header member order,
access labels, helper policy and public signatures stay unchanged. Root performs
source-stable builds/tests and its normal export/ABI comparison before archive.
Public raw fields/preheld pointers remain legacy limitations and never retire
the parked model barrier or authorize its continuation.

## Evidence/source binding (SHA-256)

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | 6ec5949dad792731cee02051938d2fadfe3271acf71c100a1935525d24a18446 |
| source/apps/moproj2mst/tests/mdt/test_viewer_property_table.cpp | fd71cd29ca83c02b03cedbff2721fa6188d0b6efbcc680cf116fcced70aa35f1 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | c98d494461d0f144e508033dba649ae18b6d9b75b549ef35c601ee83d7725a91 |
| source/port/def_pre/src/menu56_common.cpp | d3fdb14976f4b522c4b320e0ce7a8c01329c9d1ad96df836a85bc5800292e223 |
| analysis/wave2-doe-stepdata-checkbox-empty-contract.json | dbf8198ef21d20104edcc9b5f04541ae92e1e03177bd4148572884c070963ad8 |
| analysis/wave2-doe-stepdata-statevar-finalizer-decompile.json | 3f8e923f48538cf0454b6e119572b2528bd61e1d7edb26be50102432f09a6e96 |
| analysis/wave2-doe-stepdata-primary-iteration-c7b1a0.asm | 76d3b09525513a27dfcad0228dd9ae08d218bd908b06af3bc409624f15f3f16a |

Worker wrote only this report; no source/build/CTest/GUI/mutable REA operation.

## Authorized implementation and static release

Read-only plan freeze SHA-256:
`540c8a11821e95175c741ffd95ec446c7f772dac46ee2832403154e781df43c7`.
After CP85 archive, root authorized the three-file source/test successor. The
single finalizer native refresh now executes in the existing void withStep
callback, after the complete View-pointer snapshot and before row0/iteration.
No additional production statement changed. The callback preserves null-step
skip, refresh=true, ignored result and any current-object mutation before a
later table-cell refusal. All existing guards and row/Settings checks remain.

Focused tests now require exact999-to0 on the unrecovered first-cell refusal;
retain query0,1/count1 and object/view counts; and add a same-owner absent-step/
absent-Viewer control requiring no owner allocation and no table queries. The
actual parked geometry fixture owns a local PropertyTableState and requires
finalizer refusal with queryCount0/empty prefix. That invocation is within the
fixture's native-step/table/Viewer/timer/global pre-effect comparison interval.
No test hook or fabricated native owner was added. Counts are6 property-table
and40 viewer-box cases.

Static preflight passed: balanced finalizer extraction finds one withStep and
one native current-object call, no raw step member access, and exact original
guard/owner/snapshot/refresh/row/query order. Reversing only the callback edit
restores the whole CP85 viewer.cpp source hash. Test quoted includes resolve
through their real include roots, Testkit checks remain Release-active, main
retains run_all and no unsupported REQUIRE_EQ/C assert was added. Helper, model
header and C ABI source hashes remain unchanged.

Final SHA-256 receipt:

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | 178c1f105241932472068ba35f9a907988fe71166846eec9dbe6937ac04d83b8 |
| source/apps/moproj2mst/tests/mdt/test_viewer_property_table.cpp | bbef918504bf3ba328865a63ccd847214757fe3f132184ffc49f07931ef9be01 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 0faff0519c3cb3c9c8b5a448eb7b10f7cb52d14159afd020e835e4d21305a6c6 |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp (unchanged) | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp (unchanged) | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| source/apps/moproj2mst/include/moproj2mst.h (unchanged) | 34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40 |
| source/apps/moproj2mst/src/dll/abi.cpp (unchanged) | 6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848 |

Source/test files are frozen for root builds/runtime validation. Focused targets:
m2m_mdt_viewer_property_table / m2m.mdt.viewer_property_table and
m2m_mdt_viewer_box / m2m.mdt.viewer_box. Worker performed static preflight only,
with no build, CTest, GUI or mutable REA action.
