# CP85 narrow viewerSync count borrow

Migrate only the native last-object-number read in `viewerSync`. Use the existing
void-result `GuiModelAccess::withStep` callback to copy one integer into its
stack local `n`, then end the borrow before resizing or binding views. This is
smaller than retaining a step pointer through the viewer/geometry pipeline.
The plan began read-only while CP84 was frozen; root subsequently authorized
this successor after the plan freeze.

## Exclusive files and helper constraints

- `source/apps/moproj2mst/src/engine/viewer.cpp`: add the private access-helper
  include and replace only viewerSync's count-read expression with the scoped
  callback. Keep its existing GeometrySyncAccess entry preflight.
- `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`: add count-boundary and
  null-object controls, plus a parked null-object refusal assertion.
- This report for plan freeze and eventual static-release receipt.

Do not edit viewerRebuild, modelDim, geometry readers, other raw consumers,
gui_model_access.hpp, engine.hpp, native model helpers, CMake or ABI files.
Current helper supports only synchronous void/bool callbacks. An integer
return or pointer/reference result would require a policy change; neither is
needed. Capture `n` by reference, return void, and retain no native pointer.

## Original-qualified operation order

Packet V `source/docs/re/moproj2mst/mdt/V/spec_V.md` sections2.1-2.3 qualifies
viewerSync0x140c80280: absent object returns; obtain last-object-number through
0x140c7c220/0x1408301c0 (DEF_PRE0x1401d6df0); resize0x140c82930; resolve object
view0x140c82d50 using obj+8; absent view returns; bind0x1411f17f0; obtain settings;
conditionally apply default flags0x1411ee800; refresh0x140c8e640.

The existing adapter adds the model parked-barrier preflight first, lazily
creates/resolves Viewer before the count read, and returns immediately when
binding parks geometry (marking modelRectKnown false). Preserve those positions
and all current guards/partial effects. The callback belongs exactly after
`Viewer& v = viewerOf(m)`, where the raw count expression is today. Do not move
viewer creation, enter the seam before the null-object return, remove the
earliest preflight, wrap the whole binding pipeline, or add an availability
check after a newly parked geometry call.

Proposed local replacement:

```cpp
int n = 0;
GuiModelAccess::withStep(m, [&](DB_STEP_DATA* nativeStep) {
    n = nativeStep ? def::model::last_object_number(*nativeStep) : 0;
});
```

`source/port/defmodel/src/gui_twins/step_utils.cpp:48`, marked
`@src1401d6df0`, returns0 when S.u1378<1 or actual objects.tail is null;
otherwise it reads tail object+8. This is the tail's object number, not the
count of non-kind7 objects. Continue calling that actual helper once for a
nonnull step, and preserve nullable-step0 without allocating a default.
The additional access preflight is after viewerOf's own availability check;
viewerOf either creates the adapter Viewer or refuses pendingRemoval. It does
not execute the native geometry query or retire the model barrier. No native
work occurs between its return and this synchronous borrow.

## Focused tests and limits

Use target `m2m_mdt_viewer_box` / CTest `m2m.mdt.viewer_box` (39 cases at the
starting freeze). Add one boundary case:

1. Nonpending fresh model + null object: no lazy Viewer allocation or step
   replacement. Repeat with its actual step absent; do not create a default.
2. Actual model object1 and object4, created through existing objectWrap;
   native get_or_create creates kind7 placeholders2/3. There are two counted
   objects but tail number4. Clear existing Views, then synchronize object1
   with flags=false; require four Views and unchanged actual step identity,
   object count and placeholder kinds. This independently distinguishes the
   original tail-number rule from a substitute counted-object rule.
3. In the actual parked directory-query fixture, call viewerSync with nullptr
   as well as its existing actual-object call; both must refuse before the
   null-object shortcut and preserve the existing state/timer/table ledger.

The existing independent vdie42 cached-box expectations and populated/skip/
populated geometry cases cover downstream behavior. Existing parked retry and
bool/void seam checks remain. This group does not claim direct runtime coverage
of the unsupported nonnull-object/absent-step binding path; it preserves that
nullable count expression statically without manufacturing a foreign object.
No duplicate wrapper test, native layout proxy, or public test API is needed.

## Static, layout and ABI gates

Function-bounded extraction must show no raw `.step`/`->step` in viewerSync,
one withStep, one native last_object_number call and no retained native pointer.
Check exact order: entry preflight, null-object return, viewerOf, callback,
resizeViews, viewOf, absent-view return, bind, pending return, settings/default
flags, refresh. Reversing the count edit/include must restore the initial whole
viewer.cpp hash. Other legacy raw fields remain explicitly outside this seam.

No public/member/layout/access-label/signature change. Keep CP81 compile gates
for void/bool callbacks, rejected pointer/reference results, DB_STEP_DATA size
0x3408, GuiModel step type/nonpolymorphism, APIversion1 and six C signatures.
Helper, engine header, public C header and ABI implementation hashes remain
unchanged; root performs focused/full build/tests and usual export/ABI gates.
Public raw fields/preheld pointers remain legacy limitations and cannot retire
the persistent model barrier or authorize pending continuation.

## Read-only plan bindings (SHA-256)

| File | Hash |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | da7f38ace35aa2edd244b1213e6b8d27a280e21f42bcaa00bcc803eeb0e777e6 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | f6f12fb475836c482fdde3e74ab7d92dcd357ad6479419091352289705022d0d |
| source/docs/re/moproj2mst/mdt/V/spec_V.md | 3c33f779871f12821751c98dea62475a5e2a8ce444646ac241d26d3bd6e9c1e2 |
| source/port/defmodel/src/gui_twins/step_utils.cpp | 75d9bcb4e52596348dc3f4cead6b78b74dd688e40223c0790aeaa871616f74cd |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |

Worker scope is static preflight only: no build, CTest, GUI or mutable REA.

## Completed implementation and static release

Read-only plan freeze SHA-256:
`218060f2a56f1e0ce23e2394be7b7a18233d219ba8d866ac2313e0c2b70d9ad2`.
Root authorized the two-file source/test successor. viewerSync now copies the
actual nullable step's native last_object_number into local n through the
existing void callback at the original read position. The borrow ends before
resizeViews. The original earliest barrier, null-object shortcut, lazy Viewer
creation, view lookup, bind, newly pending return, flags and refresh remain in
order. viewerRebuild and all other production bodies are unchanged.

The new boundary case checks fresh/null-object no-allocation behavior, then an
actual object1/object4 list. GuiModel::objectCount is the stored u1378 count4;
the separately called original step_count_objects is2 because placeholders2/3
are kind7. After clearing Views, synchronization of object1 produces four Views
while preserving both counts, placeholder kinds and actual step identity. The
same case checks a no-step/null-object call creates neither owner nor Viewer.
The parked fixture additionally refuses viewerSync(model,nullptr,true) using
its existing state-preserving refusal ledger. Test cases total40.

Static preflight passed: balanced function extraction found zero raw step member
accesses in viewerSync, one withStep and one native last_object_number; the void
callback has no return statement or escaping native pointer. Exact ordered
tokens match all production stages above. Reversing only the scoped read and
one helper include restores the complete starting viewer.cpp SHA-256, so no
other production code changed. New quoted includes resolve through the actual
test/source include roots, Testkit checks remain Release-active, main retains
run_all, and no unsupported REQUIRE_EQ/C assert was added.

Final SHA-256 receipt:

| File | Hash |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | 6ec5949dad792731cee02051938d2fadfe3271acf71c100a1935525d24a18446 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | c98d494461d0f144e508033dba649ae18b6d9b75b549ef35c601ee83d7725a91 |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp (unchanged) | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp (unchanged) | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| source/apps/moproj2mst/include/moproj2mst.h (unchanged) | 34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40 |
| source/apps/moproj2mst/src/dll/abi.cpp (unchanged) | 6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848 |

Source/test files are frozen for root's focused target m2m_mdt_viewer_box / CTest
m2m.mdt.viewer_box, grouped build, runtime tests and archive. No worker build,
CTest, GUI or mutable REA action was performed.
