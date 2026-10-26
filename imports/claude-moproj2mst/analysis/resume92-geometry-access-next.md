# CP92 proposal: companion-DB presence predicate

Select only `qualifiedGeometryReadiness`'s `model->step->db != nullptr`
condition. A short bool withStep callback can test the actual S+0x3390 pointer
in place, without returning that pointer or changing the companion's lifetime.
This is the next checked input boundary after the cache marker; all native
branch admission and public parked guards remain unchanged. No source edit
is authorized by this read-only proposal.

## Smallest source cut

Proposed exclusive source is viewer.cpp's single condition plus this report;
existing viewer_box tests remain sufficient and unchanged. No helper/header,
native model, CMake, ABI, outer owner API or subsequent reader/query migration.
Wait for root's CP91 freeze and explicit implementation assignment.

```cpp
if (GuiModelAccess::withStep(*model, [](DB_STEP_DATA* nativeStep) {
        return nativeStep->db != nullptr;
    })) {
```

The helper supports this bool comparison. It rejects a raw pointer result, so
do not replace it with a pointer-returning step accessor, capture S.db outside
the callback, or retain the borrowed step. Earlier actual N/E owner checks
already qualify the nonnull nativeStep. Do not add a null-step fallback or
dereference the companion pointer in this callback. Native member type is
Cls_1401dbfe0* at DB_STEP_DATA+0x3390.

## Original qualification and preserved order

`resume70-geometry-id0b-reader.md` qualifies the companion-pointer gate
0x140828083..0x1408280aa, native-byte SHA-256
`4d8807237793a6aac3af8b9721183a84da7f311dbc2c46899aea8297522132cc`.
`resume71-geometry-cache-miss-loader.md` qualifies complete loader
0x140828060..0x1408283d5, native-byte SHA-256
`cb9d5ee4388c41ed115d75b236989b9f4cebf0bbe36ed3264d206508e70be796`.
Both bind original DEF_GUI_MO_64 SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The loader reads S+3390: null skips its DB body, nonnull enters it. Native
reader miss diagnostics precede the loader call. A null companion does not
prove later timer/model-object work harmless; keep current later guards.

The current adapter executes real canonical COW, actual3D table selection,
MAR/id bounds, zero cache decision, then node-reader timer Start and
ReadSingleAArrayFromDB checkpoint. Only then does it test S.db. Place the
callback exactly there. Do not move it before the committed timer prefix.

On nonnull, retain currentProject resolution and a strong lease of actual
Project.dataDb; require same raw GuiModel.db identity; resolve its actual
unique reader; compare it to S.db; qualify the live allocation receipt and
actual physical index bounds; only then run the existing bounded query/frame
branches. The callback must not change that sequence or dereference a forged
companion before the existing owner qualification. This migration supplies
only presence, not a DB lifetime lease.

On null, retain the actual arrays-timer prefix and current later pending guard
before unported object assignment. Do not synthesize a DB owner, allocation,
query, cache write or successful reader return. Keep all existing public
barrier checks/newly pending returns and raw-field limitations. No borrowed
pointer crosses any timer, DB callback or frame installation.

## Existing meaningful tests

Target m2m_mdt_viewer_box / CTest m2m.mdt.viewer_box currently has45 cases.
Reuse these without redundant test/API edits:

- `geometry_cache_miss_records_actual_node_timer_prefix_before_db_loader_guard`
  and `geometry_null_companion_records_arrays_timer_then_guards_object_assignment`
  check real timer ordering, null companion, preserved pointers/cache and later
  refusal. Nonmiss/unqualified timer tests cover bypass paths.
- `geometry_nonnull_directory_query_keeps_high_versions_pending_with_actual_owner`
  exercises the actual Project/DbHandle/reader/receipt chain and query boundary.
- `geometry_nonnull_directory_query_rejects_missing_foreign_expired_and_unobserved_owners`
  has eight actual owner/reader/provenance failure profiles, including a forged
  nonnull S.db. Those refusals must still occur after the real prefix and before
  unqualified query/data access.
- Bounds, reload/multipart, failed-reload and parked retry cases retain exact
  receipt/generation/state behavior. Cached-marker cases skip all miss work.

These expectations distinguish null/non-null branch inversion, pointer
dereference in the predicate, moved timer effects and bypassed owner receipt.
Root owns runtime validation; no test execution is claimed here.

## Static, layout and ABI gates

Reverse only the condition replacement and require the complete input
viewer.cpp hash. Verify one actual pointer presence comparison in a bool
callback, no companion dereference/pointer escape/null-step fallback. Check
prior COW/table/bounds/cache and both node timer calls remain before it, and
currentProject/strong owner/reader/receipt/query/frame effects remain after it.
Do not claim this closes all raw accesses in qualifiedGeometryReadiness.

Keep helper/model/public C headers and ABI sources unchanged, retaining current
void/bool and pointer/reference rejection, native layout offsets/size,
GuiModel step type/nonpolymorphism, APIversion1 and six C signature compile
gates. Root retains source-stable build/tests/export/ABI/archive checks. Public
raw fields/preheld pointers never retire or authorize the pending barrier.

## Read-only starting bindings (SHA-256)

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | 12f0e6ae1fa493d466bee87f9d83f0daaa63debf2fd51b53dd73972f7f7c4459 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| analysis/resume70-geometry-id0b-reader.md | 250a77926b272ada42a4a126533f23b7915064febd20766afdef7f830ab480a2 |
| analysis/resume71-geometry-cache-miss-loader.md | 9ff0bc3243382220d4964e5fb4ff0a43a9ac64c91ef97b81b449dc7226f54c7b |
| analysis/resume74-geometry-companion-owner.md | ba5be0e61b848115afedb1e8e3e1ed69c6d8b6f65cf89b4a205c71a8e122f267 |

Worker wrote only this report. No source/build/CTest/GUI/mutable REA action.

## Authorized implementation and static release

Plan freeze SHA-256:
`fb233ff4a5c9c4950d0f3ff2b6bbadfd28b4fe52f3acdeddcb6754a0e8f41acc`.
After CP91 freeze, root authorized the single viewer.cpp presence condition.
The bool withStep callback now compares actual nativeStep.db with nullptr
after the two original node timer calls and before currentProject/DB lease
resolution. It returns no DB/step pointer, performs no companion dereference,
and adds no native owner/default/write. Null/non-null branch bodies, later
reader identity/receipt/query/frame checks and all public guards are unchanged.

Static preflight passed: one selected pointer comparison and exact preceding
byte/version/COW/table/bounds/cache/timer plus subsequent Project/strong owner/
reader/receipt/frame order. Native member type remains Cls_1401dbfe0*. Reversing
only this condition replacement restores the complete CP91 viewer.cpp hash.
Existing45-case viewer test, helper/model headers and C ABI sources remain
byte-for-byte unchanged. Root owns runtime/build acceptance.

Final SHA-256 receipt:

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | b94efd57a7beb810b7d73498eb43f0f5c52aba320150ca9dd07aa5d11f4cbf38 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp (unchanged) | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp (unchanged) | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp (unchanged) | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| source/apps/moproj2mst/include/moproj2mst.h (unchanged) | 34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40 |
| source/apps/moproj2mst/src/dll/abi.cpp (unchanged) | 6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848 |

Source is frozen/released for root's m2m_mdt_viewer_box / CTest
m2m.mdt.viewer_box and grouped source-stable validation. Worker performed no
build, CTest, GUI or mutable REA action.
