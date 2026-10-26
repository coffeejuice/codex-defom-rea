# CP86 signed integer frequency source freeze

Advanced Fill now admits negative exact signed-int32 FRQNCY values in its
existing actual 6x1 table profile. It retains finite/equality/negative-zero
guards, the first-row -1 special selection and every other page/model guard.
No frequency store, callback selection behavior or remembered text conversion
was added or changed.

Source qualification is the approved plan
`analysis/resume86-editor300-next.md`, SHA-256
`4b52c7fcd25994f37aa2cac688b3d33a3dd5f3c39afc4ba22d47b2981a142192`.
Its original fill and six callback receipts establish signed custom selection
without a native sign rejection or frequency store in the custom branches.

## Exact production change

In `fillAdvanced3D()`, removed only the exclusion of negative values other than
first-row -1. Kept the original finite test, equality with the hardware
CVTTSD2SI-converted int, and explicit negative-zero refusal. The source comment
now describes exact signed integers. The existing const SC interface remains.

Existing projection logic is unchanged: first-row -1 uses index1 without
populated custom text; any other nonzero first-row int uses index2 with signed
text; later rows use index1 with signed text for any nonzero int. Thus the
native custom enable branch remains the same. This does not claim effective
enabled state through all QWidget parents or add an interactive text callback.

No shape allocation, frequency write, API/header/layout, CMake, public session,
runtime, palette/counter, movement or lifecycle code changed. Advanced Apply
still ignores remembered frequency display fields and preserves current model
frequency values, independently of the narrower Fill profile.

## Focused tests and static preflight

The old frequency snapshot case now explicitly admits -2, requiring index2 and
text "-2". It retains first-row -1's existing special assertion and the old
negative-zero/fraction/overflow/infinity refusal list.

Two new Release-active cases exercise:

- distinct signed values across all six rows, including first-row -2,
  later-row -1, INT32_MIN and -2147483647; exact indices/signed decimal text and
  raw frequency bits remain unchanged through Fill and changed-time Apply.
  A later live frequency change to -909.25 is preserved by Apply rather than
  being replaced by remembered text; this asserts Advanced Apply's existing
  behavior, not an interactive text-edit callback.
- both valid signed int32 limits in each row, plus per-row rejection of -0,
  -0.5/-1.5/-2.5, values immediately outside the signed range, both infinities
  and quiet NaN, with raw input bits preserved on every refusal.

Compared only these two files against CP85's archived source. Quoted includes
resolve, added macros are supported Testkit macros, delimiters are balanced,
one Testkit main is present and no C assert/trailing whitespace is present.
No new include or registration is required. The production diff is one removed
condition and its source comment; movement/public guards are untouched.

Root target `m2m_mdt_simcontrol300_snapshot`, CTest
`m2m.mdt.simcontrol300_snapshot`, now contains 90 cases (88+2). No build,
executable test, CTest, GUI/original execution or mutable REA was run by this
worker. Root owns grouped build/validation/archive.

## Frozen source hashes and remaining boundary

Paths are relative to `source/apps/moproj2mst/`:

| File | SHA-256 |
|---|---|
| `src/engine/simcontrol300.cpp` | `0568dc61f1023d28da4930338dbb99119fc4c2aef78f0399d3cb1be042534b68` |
| `tests/mdt/test_simcontrol300_snapshot.cpp` | `0f52dff5154f5fb70c4566be753f03d0183a9fc0fba82c0ea66e4032530ac250` |

Both files are released/frozen. Fractional and negative-zero values that could
make a combo callback rewrite raw frequency data remain guarded by Fill.
Nonfinite/out-of-range conversions, shape repair/allocation, interactive edits
and earlier public editor300 construction remain separate dependencies.
