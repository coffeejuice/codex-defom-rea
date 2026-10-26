# CP91 proposal: actual cached-reader marker borrow

The next short cut is `qualifiedGeometryReadiness`'s actual step
`u26c4[0x0b] == 0` condition. A bool withStep callback can read this fixed
signed32 cache marker at its current position, without retaining a pointer or
changing public guard/branch behavior. It extends the checked-access inventory
for the cached-versus-loader decision. Existing public barrier enforcement and
branch admission already work; this is an access migration with no newly
claimed native behavior.

## Exact proposed source scope

Own `source/apps/moproj2mst/src/engine/viewer.cpp`'s single cache condition and
this report if root assigns implementation after CP90 freeze/review. Existing
viewer_box tests are sufficient and should remain unchanged. No header/helper,
CMake, layout, ABI, Boolean/format, modelDim or subsequent DB field changes.
Keep other raw readiness accesses explicitly legacy.

```cpp
if (GuiModelAccess::withStep(*model, [](DB_STEP_DATA* nativeStep) {
        return nativeStep->u26c4[0x0b] == 0;
    })) {
```

The actual owner has already been qualified through the View's owner observer,
nonnull model step, and N/E identity checks. Keep that provenance. No nullable
default, unsigned conversion, positive-only test, MAR/A-body validation, copied
owner or retained callback result/pointer should be introduced. The helper's
existing bool policy fits; the member is std::int32_t u26c4[0x118].

## Native ledger and preserved effect order

`analysis/resume70-geometry-id0b-reader.md` qualifies original DEF_GUI_MO_64
SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Its complete reader interval0x14082d7d0..0x14082dc8b has native-byte SHA-256
`a3bc02528ce5ce18e576c5f938d82fc8b081616504437a8ca980001ce74dd6e4`.
At0x14082d876 it reads signed32 S.u26c4[id]; nonzero atd880 returns false
normally. Outer which0 ignores this reader result and continues normally.
Zero enters the later miss body. Native cached return reads no MAR pointee;
negative cache markers count as nonzero. Retain all of those distinctions.

The adapter condition currently follows these existing effects/checks:

1. Canonical/preallocated profile and actual owner qualification.
2. Actual zero synchronization byte, dimension3 and signed version>=6000.
3. Actual coordinate allocation/key0 COW prefix.
4. Process-global3D table selection.
5. Actual MAR index in0..139 and signed id limit greater than11; failure returns
   the existing HighVersionReaderPending after the real prefix.
6. Actual cache marker test.

Place the callback only at step6. A zero marker still starts/checkpoints the
actual node-reader timer, then follows the existing null/non-null DB branches,
receipt/bounds checks and possible persistent frame. Nonzero still bypasses
that work and completes only the currently admitted cached reader path.
Do not hoist the borrow before COW/table selection, move the bounds checks,
replay prefix effects, install/retire a frame, or change missing DB behavior.
The callback ends before all miss timers/DB effects.

Public viewer entries already refuse parked models before geometry effects.
The new availability check is reached only on the same unparked invocation;
frame installation is later in the miss branch. Maintain all earliest gates
and newly pending early returns. Public raw fields/preheld pointers remain
legacy limitations and cannot authorize pending continuation.

## Existing meaningful regression coverage

Target m2m_mdt_viewer_box / CTest m2m.mdt.viewer_box currently has45 cases.
No new mirrored or redundant test is needed for this expression replacement.

- `geometry_high_version_cached_reader_returns_normally_with_null_mar_and_array_bodies`:
  versions6000/11000/INT32_MAX, markers1/-1, actual/null E owner and populated/
  null connectivity, totaling24 rows. It checks getter dispatch/COW, unchanged
  pointers/cache/version and normal scene/skip result despite null MAR/A bodies.
- `geometry_high_version_cached_reader_bounds_and_miss_keep_real_prefix_guarded`
  and `geometry_high_version_cached_reader_uses_valid_altered_actual_header_indices`
  retain actual global-index bounds, negative marker and real prefix behavior.
- `geometry_high_version_cached_miss_cached_transition_preserves_each_actual_result`
  changes1→0→-1 on the same actual model and requires success→pending→success
  with exact dispatch/body/owner/cache state.
- Existing cache-miss timer, directory receipt/extent and parked owner/retry
  fixtures guard the later branch and its pre-effect refusal behavior.

These tests distinguish marker==0 from positive-only, truth inversion, added
body dereference, moved table prefix and replay. Root owns runtime validation;
this report does not claim the tests ran during investigation.

## Static, ABI and review gates

For implementation, reverse only this replacement and require the complete
input viewer.cpp SHA-256. Inspect the condition-bounded callback for one
signed32 cache read/comparison, bool result, no write/pointer escape/fallback.
Check exact prior owner/byte/dimension/version/COW/table/bounds and later
timer/DB/frame order. Do not claim the whole function is raw-access-free.

Helper/model/public headers and ABI source stay byte-for-byte unchanged.
Existing supported void/bool and rejected pointer/reference compile gates,
DB_STEP_DATA native size/offsets, GuiModel step type/nonpolymorphism,
APIversion1 and six C signatures remain. Root retains source-stable build,
focused/full tests and export/ABI/archive checks. Source editing must wait for
root's CP90 freeze and explicit implementation release.

## Read-only starting bindings (SHA-256)

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | 1f1192efda5844a0a7cd53445528b6eeceb89ffc9646995780caa86046a3dd62 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| analysis/resume70-geometry-id0b-reader.md | 250a77926b272ada42a4a126533f23b7915064febd20766afdef7f830ab480a2 |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |

Worker changed only this report. No source/build/CTest/GUI/mutable REA action.

## Authorized implementation and static release

Plan freeze SHA-256:
`7731cfe257441d640dd32b721f838bf41617a0f77b9700a4c7121a8821fe40d0`.
After CP90 freeze, root authorized only viewer.cpp's single cache condition.
That predicate now uses the inline bool withStep callback exactly as proposed.
It reads actual signed32 u26c4[11], compares with0, and ends before miss timer/
DB effects. Original COW/table/index bounds and all later branch/frame logic
remain unchanged. No body pointer, nullable default, marker normalization or
new native field write was introduced.

Static preflight passed: exact original owner/byte/dimension/version/COW/table/
index-bounds/cache/timer/DB/receipt/frame order; one selected cache read in its
callback; no pointer escape or null fallback. Native declaration remains
std::int32_t u26c4[0x118]. Reversing only the condition replacement restores
the complete CP90 viewer.cpp SHA-256. Existing45-case test and helper/model/C
ABI sources are byte-for-byte unchanged. Runtime acceptance is root-owned.

Final SHA-256 receipt:

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | 12f0e6ae1fa493d466bee87f9d83f0daaa63debf2fd51b53dd73972f7f7c4459 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp (unchanged) | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp (unchanged) | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp (unchanged) | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| source/apps/moproj2mst/include/moproj2mst.h (unchanged) | 34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40 |
| source/apps/moproj2mst/src/dll/abi.cpp (unchanged) | 6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848 |

Source is frozen/released for root's m2m_mdt_viewer_box / CTest
m2m.mdt.viewer_box and grouped source-stable validation. Worker performed no
build, CTest, GUI or mutable REA action.
