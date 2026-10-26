# CP89 actual node-owner synchronization-byte borrow

Select only `qualifiedGeometryReadiness`'s actual-owner `S.u1b58 == 0`
condition in viewer.cpp. An inline bool withStep callback reads the same byte
at exactly the existing position and returns only the comparison. This short
borrow ends before dimension/version tests, native allocation/COW, table
selection, DB work or pending-frame installation. No native pointer escapes.

## Exclusive source/test scope and exact replacement

Root has authorized viewer.cpp and a narrow viewer_box test change, with this
report as the plan/receipt. No Boolean/format source overlap, helper/header,
CMake, native model or public ABI change. Freeze this plan before editing.

Replace only:

```cpp
if (model->step->u1b58 == 0) {
```

with the existing bool-result access policy:

```cpp
if (GuiModelAccess::withStep(*model, [](DB_STEP_DATA* nativeStep) {
        return nativeStep->u1b58 == 0;
    })) {
```

Do not add a null fallback. The preceding exact actual N-owner qualification
already requires model, its nonnull step and nodes.u130 == step.get(). Keep
the earlier E-owner identity guard, canonical registry/preallocated coordinate
guard and both owner tests unchanged. Retain every later dimension/version,
getter, timer, table, DB extent/receipt and persistent barrier branch as-is.
Other raw accesses in this function remain explicit legacy inventory; the
static zero-raw-access gate applies only to this selected condition. modelDim
is a const-only helper and cannot use the current mutable access policy without
widening it; do not include it in this slice.

## Retained original evidence and effect-order limit

Original DEF_GUI_MO_64 SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Retained `resume63-geometry-owner-up-to-date-leaves.asm` covers gate
0x14082bf10..0x14082bf7d. At0x14082bf1e, MOVZX reads the byte owner+0x1b58;
TEST/JNE atbf25/bf27 returns true for every nonzero value throughbf53.
Zero proceeds to owner-is3D0x14035b400 and signed version0x140825420; the
signed6000 comparison atbf44/bf49 selects the old-version fast return.
`resume66-geometry-next.md` retains the complete version/dimension evidence
and the original coordinate-getter/COW timing.

The current adapter's condition is a profile classification before executing
the qualified getter. Original native coordinate allocation/key0 COW dispatch
precedes the actual synchronization gate read. This migration does not move
either native effect or claim that the adapter's earlier profile test is that
native invocation. It preserves the existing adapter read position and invokes
the same actual getter/COW path afterward. Existing high-version prefix and
public guard behavior stay intact.

The model comes from the View's existing owner observer, not an arbitrary raw
N/E cast. withStep's availability preflight occurs immediately before the
selected field read. Public viewer entries already refuse a parked model
before geometry work; the new borrow is not reached on those retries. Keep
those earliest gates and newly pending early returns unchanged. Do not retain
the borrowed step in a callback capture, native owner field or pending frame.

## Narrow test and acceptance gates

Existing viewer_box tests already cover actual owner flags1/0x80, zero's signed
version threshold, canonical/preallocated and foreign-owner refusals, COW body
identity/dispatch counts, high-version prefixes, and persistent parked retries.
Expand only `geometry_actual_node_nonzero_flag_bypasses_version_rejection`:
cross versions6000/INT32_MAX with bytes1,0x80,0xff rather than fixed0x80, and
check the exact input flag remains unchanged. All rows retain the existing
two getter rounds, actual dimensions/owners, unique body identity, coordinates/
connectivity and scene result. This strengthens every-nonzero-byte behavior
without a new mirrored helper test or public hook. Current target has45 cases.

Exact focused target: m2m_mdt_viewer_box / CTest m2m.mdt.viewer_box. Root owns
build/runtime acceptance. Worker performs only static preflight: exactly one
condition replacement; original owner guards before it and dimension/version/
COW/table/DB/frame logic after it; one byte read in the new bool callback; no
pointer escape. Reversing only that replacement must restore the whole input
viewer.cpp hash. No whole-function raw-free claim is permitted.

Keep helper/header/public C ABI hashes unchanged and preserve existing compile
gates for bool/void callbacks, rejected pointer/reference results, DB_STEP_DATA
size0x3408, GuiModel step type/nonpolymorphism, APIversion1 and six C signatures.
Root retains normal source-stable/export/ABI validation. Public raw fields and
preheld pointers remain legacy limitations and never retire the parked barrier.

## Starting SHA-256 bindings

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | b7baa6c3046c5fc288a7f8b57ea80ced61314529d0b9d46ac2af885ba988da74 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 89f8dfcd24337f27413f7fef7ee4740706281e12fe971c10af0b56dedc9ae05f |
| analysis/resume63-geometry-owner-up-to-date-leaves.asm | 353c8eeabc1cfe0c04c2ecbc87339875672dc9a81c69d6a19136d2c3aa314c65 |
| analysis/resume66-geometry-next.md | 566ee4e2130e6cf5d80be13da9530c4b399c40743831fcc0fa764a4aa5a48daa |

No source change, build, CTest, GUI or mutable REA was performed before this
plan freeze. Implementation requires the already assigned exclusive lane.

## Completed implementation and static release

Plan freeze SHA-256:
`4c7dd5baea60710f7407dcbcfca60f93ba87acece63763932f37dada8d8679ef`.
Under root's explicit source assignment, the selected condition now uses the
inline bool withStep callback exactly as proposed. No preceding owner/profile
check or subsequent geometry/DB/barrier statement changed. The callback reads
only actual nativeStep.u1b58, compares with0 and retains no pointer. It does
not add a nullable fallback or change any public guard/admission.

The existing nonzero flag high-version test now has six rows: bytes1/0x80/0xff
cross versions6000/INT32_MAX. It retains the existing actual owner/dimensions,
two getter dispatches, unchanged unique collection body, scene, coordinate/
connectivity and version assertions, checking the exact input flag afterward.
Target count remains45. No new test stub or native layout proxy was added.

Static preflight passed: selected callback count/byte read are exactly one;
the full ordered owner/profile/dimension/version/COW/table/cache sequence is
unchanged. Reversing just that condition replacement restores the complete
starting viewer.cpp SHA-256. Test quoted includes resolve in actual roots,
main retains run_all and no unsupported REQUIRE_EQ/C assert was introduced.
Other raw step accesses remain explicitly unconverted. Helper/model header/
public C ABI sources remain byte-for-byte unchanged.

Final SHA-256 receipt:

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | b715bb8fe08b7e6b9d91e2f027fa50a652848b1ce98a26f84a02b682593e0fd0 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp (unchanged) | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp (unchanged) | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| source/apps/moproj2mst/include/moproj2mst.h (unchanged) | 34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40 |
| source/apps/moproj2mst/src/dll/abi.cpp (unchanged) | 6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848 |

Source/test files are frozen for root build/runtime validation. Focused target:
m2m_mdt_viewer_box / CTest m2m.mdt.viewer_box. Worker ran static preflight only;
no build, CTest, GUI or mutable REA action.
