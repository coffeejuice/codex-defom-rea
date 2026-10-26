# CP90 signed actual-owner version predicate borrow

Convert only `qualifiedGeometryReadiness`'s actual-owner
`step_version >= 6000` predicate to a short bool withStep callback. Root has
assigned the exclusive viewer.cpp/viewer_box lane. The current helper already
supports this comparison; no policy, pointer lifetime or public guard change
is required. Existing focused tests fully exercise this signed boundary and
can remain unchanged.

## Exact source cut and original qualification

After canonical/preallocated profile checks and exact actual N/E owner
qualification, the CP89 byte borrow classifies owner.u1b58. Only its zero branch
reads dimension; dimension!=3 remains Unqualified. Replace the immediately
following version condition only:

```cpp
if (GuiModelAccess::withStep(*model, [](DB_STEP_DATA* nativeStep) {
        return nativeStep->step_version >= 6000;
    })) {
```

Do not cast to unsigned, truncate, default a missing owner, or save the native
pointer/version for later reuse. The preceding owner checks establish the
actual nonnull step. This callback returns only bool and ends before the
existing allocation/COW, global table selection, cached reader, timer, DB
receipt/query and barrier work. All earlier/later checks and partial-effect
orders remain untouched. Keep public parked entry gates and newly pending
early returns. modelDim's const API and the function's other raw fields stay
explicitly legacy.

Retained `analysis/resume66-geometry-next.md` qualifies original
DEF_GUI_MO_64 SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The leaf0x140825420..0x140825431 reads the actual owner+0x1b64 version into eax
at0x14082542a. Complete bytes:
`48894c2408488b4424088b80641b0000c3`.
Native-byte SHA-256 is
`0b55588ba6865add0412d11de3790062a236c71843a95c3a99ad3808cecdd331`.
Its caller gate at0x14082bf44 compares eax with0x1770; signed JL atbf49 selects
the version<6000 fast return. Nonzero owner+1b58 skips dimension/version. The
retained CP63 gate disassembly records those exact instructions.

As documented for CP89, this is the adapter's existing profile test, not a new
native synchronization invocation: original coordinate allocation/COW precedes
its actual native gate, while the adapter classifies admitted profiles before
the qualified getter. This migration moves neither operation or read position
and does not claim new branch closure or additional native allocations.

## Exclusive files and focused tests

Write only `source/apps/moproj2mst/src/engine/viewer.cpp`'s selected condition
and this report. The assigned viewer_box test remains unchanged because
`geometry_actual_node_zero_flag_signed_version_gate_preserves_getter_cow`
already covers INT32_MIN,-1,0,5999,6000,6001,INT32_MAX crossed with actual/null
E owner and populated/null connectivity:28 rows. It checks exact native
getter dispatch/COW behavior, body/variable/value identity, original owner and
array pointers, the high-version table prefix and unchanged version/flags.
CP89's six nonzero-byte/high-version rows independently require version bypass.
Existing unknown-owner/registry/missing-coordinate refusals and parked retries
remain active. Adding a copy of those checks would not strengthen this cut.

Target `m2m_mdt_viewer_box` / CTest `m2m.mdt.viewer_box` currently has45 cases.
Root owns builds/runtime validation. Worker static preflight must prove exactly
one condition edit, signed member comparison>=6000, no pointer escape or null
fallback, same earlier byte/dimension gates and later native prefix order.
Reversing the replacement must restore the entire input viewer.cpp hash.
Do not claim the whole readiness function is free of raw accesses.

Helper, engine header, C header and ABI implementation remain unchanged.
Existing void/bool and rejected pointer/reference compile gates, native layout
size/offsets, GuiModel step type/nonpolymorphism, APIversion1 and six C
signatures remain in force. Root retains source-stable tests/export/ABI/archive
checks. Public raw fields/preheld pointers remain legacy limitations and never
retire the persistent model barrier or authorize pending continuation.

## Starting SHA-256 bindings

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | b715bb8fe08b7e6b9d91e2f027fa50a652848b1ce98a26f84a02b682593e0fd0 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| analysis/resume66-geometry-next.md | 566ee4e2130e6cf5d80be13da9530c4b399c40743831fcc0fa764a4aa5a48daa |
| analysis/resume63-geometry-owner-up-to-date-leaves.asm | 353c8eeabc1cfe0c04c2ecbc87339875672dc9a81c69d6a19136d2c3aa314c65 |

No source/build/CTest/GUI/mutable REA action occurred before this plan freeze.

## Implementation and static release

Plan freeze SHA-256:
`757051dd9dbd9fe07251898a37dbe71bae6b34da7b630a5e019370901835e07a`.
The authorized single predicate now executes through inline bool withStep.
Its exact signed native member comparison remains>=6000; no unsigned cast,
nullable fallback, saved pointer or later result cache was introduced. Earlier
owner/byte/dimension guards and later native prefix work are unchanged. The
callback finishes before allocation/COW or table/reader/timer/DB effects.

Static preflight passed: native member declaration is std::int32_t; selected
condition appears once; exact preceding/following ordered gates remain;
reversing only that predicate replacement reconstructs the whole input source
SHA-256. The45-case viewer test file is byte-for-byte unchanged and retains its
existing28 signed boundary/COW rows and six nonzero/version bypass rows. No
helper/header/public C ABI source changed. These are static results; root owns
all runtime/build acceptance.

Final SHA-256 receipt:

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | 1f1192efda5844a0a7cd53445528b6eeceb89ffc9646995780caa86046a3dd62 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp (unchanged) | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp (unchanged) | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp (unchanged) | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| source/apps/moproj2mst/include/moproj2mst.h (unchanged) | 34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40 |
| source/apps/moproj2mst/src/dll/abi.cpp (unchanged) | 6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848 |

Source is frozen/released for root's m2m_mdt_viewer_box / CTest
m2m.mdt.viewer_box and grouped source-stable validation. Worker performed no
build, CTest, GUI or mutable REA action.
