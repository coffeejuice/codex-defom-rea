# CP87 proposal: two short viewerRebuild step borrows

Select only viewerRebuild's nullable-step test and last-object-number read.
Use two short existing withStep callbacks at their current positions. This
removes that function's raw step accesses without retaining a native pointer
through view clearing, binding or a newly parked geometry query. This is a
read-only proposal while CP86 source remains frozen.

## Proposed exclusive files

- `source/apps/moproj2mst/src/engine/viewer.cpp`: only the two viewerRebuild
  expressions, preserving the entire surrounding body.
- `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`: direct ordinary/null
  step controls and expand the actual parked fixture's rebuild argument matrix.
- This report for the plan and any later authorized static receipt.

No helper/include/header/native source/CMake/API change. viewer.cpp already
includes the private access helper. Root must archive CP86 and assign these
files before source edits. Other raw step consumers remain legacy; this is
not whole-file encapsulation or a reader-continuation admission.

## Source-qualified original order

MDT-V `source/docs/re/moproj2mst/mdt/V/ledger.tsv` maps native0x140c7ff80 to
viewer.cpp. `spec_V.md` section7 identifies rebind-all as an existing material
KEY/restart-load caller of0x1411f17f0, binding the i-th View to the i-th native
object0x1408302a0, applying flags only when reset is true, then refreshing.
Sections2.2/2.3 qualify view-list sizing/Settings lifetime; View removal leaves
the Settings list. This proposal preserves that existing projection rather
than claiming newly recovered native behavior.

The exact current adapter order is:

1. GeometrySyncAccess::requireAvailable(m).
2. Missing actual step returns before lazy Viewer acquisition or clearing.
3. viewerOf(m) resolves/creates the adapter Viewer, refusing pendingRemoval.
4. If a, viewerClear(m) removes/unbinds Views in existing order. Its errors and
   already committed effects propagate; there is no rollback.
5. Read actual native last_object_number, then resizeViews.
6. For each existing View in order, increment ordinal; resolve model.objectAt;
   bind; if geometry parked, mark modelRectKnown false and return immediately.
7. Resolve Settings; only if a require Settings/object and reset default flags.
8. refresh; b remains the currently unprojected contact-display argument.

The native helper `source/port/defmodel/src/gui_twins/step_utils.cpp:48`
(`@src1401d6df0`, GUI twin0x1408301c0) returns0 when u1378<1 or tail is null;
otherwise it reads the actual tail object's ID at+8. Keep that helper exactly,
not a count-of-nonempty-objects substitute.

## Exact narrow migration

Replace only the entry step predicate, keeping it after the existing guard:

```cpp
if (!GuiModelAccess::withStep(m, [](DB_STEP_DATA* nativeStep) {
        return nativeStep != nullptr;
    })) return;
```

After viewerOf and optional viewerClear, replace only the native count read:

```cpp
int n = 0;
GuiModelAccess::withStep(m, [&](DB_STEP_DATA* nativeStep) {
    n = def::model::last_object_number(*nativeStep);
});
```

First callback returns bool; second returns void and copies an integer into
local n. Both fit the current helper policy; neither saves/returns a native
pointer or needs an integer-result policy. Reacquire the actual pointer after
optional clear instead of caching it across unbind/destruction. Preserve the
second expression's unconditional dereference: do not invent a nullable0
fallback, allocate a default step, or silently recover external owner changes.

Keep the earliest geometry preflight and all later pending returns. Additional
withStep preflights are before the original step reads. viewerOf constructs
adapter state; viewerClear calls existing unbind/removal and does not run the
geometry reader or retire its barrier. If it throws, the second callback is
not reached, as today. End the second borrow before resizeViews/bind. Do not
wrap the entire function or introduce a post-bind access check after parking.
This does not add arbitrary reentrancy, concurrent external mutation or stale
raw-alias safety guarantees.

## Small focused test increment

Use existing target `m2m_mdt_viewer_box` / CTest `m2m.mdt.viewer_box`, currently
40 cases. Existing corpus box expectations, CP85 sparse-tail check, DB-read
fixtures and populated/skip/populated geometry cases remain applicable.

Add a direct ordinary rebuild case using real GuiModel/object construction:

- For an actual step and small valid bound objects, invoke both a values and
  both b values; verify same actual step, expected view count/object identities
  and available display Settings. Existing native bind/refresh remains the
  behavior under test; do not stub or reproduce it.
- Fresh model with its disposable step absent: every a/b combination returns
  with no step/Viewer creation. This catches moving Viewer creation before the
  existing null-step shortcut.
- Existing Viewer plus temporarily retained actual step owner: move the
  disposable model's step out, call a=true, and require existing Viewer
  identity/count and display snapshots unchanged. Restore that same actual
  step before teardown. This checks that missing-step return also precedes
  clearing; it is a test of the legacy nullable field, not a supported way to
  retire a pending barrier or invent a different native owner.

In the existing real parked directory-query fixture, replace the single
viewerRebuild(false,false) refusal with all a/b combinations. Perform those
calls within the existing native-step/Viewer/timer/global pre-effect ledger
interval, so a=true cannot clear any Views. Keep all state comparisons and
pending retry/owner-bypass checks. Do not use this test to weaken a valid
partial-effect assertion or admit unsupported geometry layouts.

## Static and ABI gates

Balanced viewerRebuild extraction must show two withStep calls, one native
last_object_number, no raw `.step`/`->step`, a bool null predicate and void
count callback. Check all stages above remain ordered, including optional
clear before the second callback and newly pending return before flags/refresh.
No native pointer may leave either callback. Reversing exactly the two edits
must restore the whole starting viewer.cpp hash.

Helper, engine header, public C header and ABI implementation stay unchanged.
Retain existing positive void/bool and negative pointer/reference compile
gates, DB_STEP_DATA size0x3408, GuiModel step type/nonpolymorphism, APIversion1
and six C signatures. Root performs source-stable build/runtime tests and
normal export/ABI checks. No member order, access label, native layout or
public signature change is needed. Public raw fields and preheld pointers
remain explicit legacy limitations and never authorize pending continuation.

## Starting SHA-256 bindings

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | 178c1f105241932472068ba35f9a907988fe71166846eec9dbe6937ac04d83b8 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 0faff0519c3cb3c9c8b5a448eb7b10f7cb52d14159afd020e835e4d21305a6c6 |
| source/port/defmodel/src/gui_twins/step_utils.cpp | 75d9bcb4e52596348dc3f4cead6b78b74dd688e40223c0790aeaa871616f74cd |
| source/docs/re/moproj2mst/mdt/V/spec_V.md | 3c33f779871f12821751c98dea62475a5e2a8ce444646ac241d26d3bd6e9c1e2 |

Worker changed only this report. No source/build/CTest/GUI/mutable REA action.

## Authorized implementation and static release

Read-only plan freeze SHA-256:
`eca47851c5ed001039e31a89cab1f91c40b0a65f61e2a4672ec062a3ef9bb170`.
After CP86 freeze, root authorized viewer.cpp and viewer_box test source. The
entry nullable-step predicate now uses the bool withStep callback; the native
count read uses a separate void callback after optional clear. The second
callback retains the unconditional nativeStep dereference and copies only n.
Every surrounding production statement, including newly pending return and
existing partial-effect errors, is unchanged. Neither borrow crosses clearing
or binding and no helper/header/API policy was widened.

The new direct boundary case uses a real2D model with actual objects1/2 and
normal settings produced by viewerSync. All four reset/contact combinations
preserve actual model/Viewer/object identities, object/view counts and the
already-default Settings snapshots. An RAII test owner temporarily retains
the actual step while its legacy public model field is absent: all four
combinations return without clearing Views or altering Settings. It restores
that same step before teardown, including exception unwinding. A fresh model
with absent step likewise creates neither native owner nor Viewer. This tests
the old null shortcut; it does not create a pending-frame retirement mechanism.

The real parked directory-query fixture now refuses all four rebuild argument
combinations. Those calls were moved from the earlier single-call check into
the existing native-step/adapter-table/Viewer/timer/global comparison interval,
retaining all comparisons afterward. No a=true clearing effect can pass that
ledger. Case count is41; existing pointer/reference-result and ABI compile
checks remain in the same test target.

Static preflight passed: balanced function extraction finds two withStep,
exactly one last_object_number, zero raw step member accesses, and exact
preflight/predicate/viewer/clear/count/resize/objects/bind/pending/flags/refresh
order. The second native call has no added nullable fallback. Reversing only
the two expression edits restores the whole starting viewer.cpp SHA-256.
Test quoted includes resolve against actual include roots, main retains
run_all, and no unsupported REQUIRE_EQ/C assert was introduced. These are
static checks, not runtime acceptance.

Final SHA-256 receipt:

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | ca51fc9a3b87db9b518b067deeb94d06b84b189a17b3f507ab06e657a878c86f |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 5553cb023ca8898e2907c6757d9a4d7d3ec1b2317bdbb403eebcb6afeac5f890 |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp (unchanged) | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp (unchanged) | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| source/apps/moproj2mst/include/moproj2mst.h (unchanged) | 34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40 |
| source/apps/moproj2mst/src/dll/abi.cpp (unchanged) | 6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848 |

Source/test files are frozen for root build/runtime validation. Exact focused
target is m2m_mdt_viewer_box / CTest m2m.mdt.viewer_box. Worker performed no
build, CTest, GUI or mutable REA action.
