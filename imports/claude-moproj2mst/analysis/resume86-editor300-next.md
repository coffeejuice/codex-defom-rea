# CP86 next setting cut: signed integer advanced frequencies

The smallest next setting increment is negative exact int32 FRQNCY values in
the existing 6x1 advanced-page profile. Current Fill rejects every negative
value except first-row -1. Original Fill has no sign rejection: first-row
nonzero/non--1 selects custom index2, while other rows select custom index1
for any nonzero integer. Those custom callbacks only enable the input; they
do not overwrite FRQNCY. Existing conversion/text logic already represents
the signed value, so only the negative-value admission restriction is missing.

Only this report was written. CP85 source stays frozen. No source/build/test
execution, GUI/original execution or mutable REA was performed. Bounded new
DUMPBIN/PE checks used the imported original and retained captures.

## Reconciled current implementation and audit

`fillAdvanced3D()` still requires actual `model.simCtl()==&sc`, 3D/KnownC,
existing FRQNCY rows6/cols1/ncomp6/non-null values, finite source values equal
to their CVTTSD2SI-converted integers, and no negative zero. It additionally
refuses negative values except row0=-1. The returned page already computes
the source combo and `qt::number(converted)` text for all signed ints.

`applyAdvanced3D()` does not consume remembered frequency combo/text fields;
the existing test even mutates live FRQNCY to 99.5 after Fill and expects Apply
to preserve it. This is source-qualified to advanced Apply; it does not say a
separate interactive frequency-edit callback has no model effect.

The older wave2 advanced contract explicitly bounded initial frequencies to
positive integers/zero and first-row -1. That was a supported-profile limit,
not a native negative-input failure. It is the remaining current setting
restriction selected here. Later CP82..85 recovered zero movement-path
profiles; those live-model guards and their current 88-case snapshot file
remain unchanged by this proposal. This read-only report does not claim new
test acceptance or close the older audit's effective-widget-enabled-state gap.

## Exact original fill and callback behavior

The actual SC owns FRQNCY: rows at SC+328, columns+32c, embedded DYNARY+330,
ncomp+370 and values pointer+378. Original `140ec5bc0` first checks/repairs
shape at `140ec6d18..6d95`; keep the existing preallocated exact-shape guard,
so no allocation/repair branch needs implementation.

The first row's current double is converted with CVTTSD2SI at `140ec6db1`:

| Converted int | Original control effect |
|---|---|
| 0 | Set combo index0 and disable input |
| -1 | Set combo index1 and disable input |
| Any other signed int | Set combo index2, enable input, format the converted signed int and set its text |

The first row's custom path is `140ec6e5d..6f0e`. It does not compare the value
with zero by signed greater-than. Rows1..5 use the same CVTTSD2SI/nonzero test
but only indices0/1; nonzero means index1, input enabled and signed integer
text populated. Their conversion sites are `140ec6f3a`, `140ec7063`,
`140ec718d`, `140ec72b7` and `140ec73e1`. Display values are not clamped to
positive numbers by this code.

Retained default-callback decompilation plus bounded original bytes establish
the model effects:

- `140ecfab0` (first combo): index0 writes +0.0, index1 writes -1.0, and index2
  only issues `setEnabled(true)` on setup+1370. The index2 branch is exactly
  `140ecfb5e..fb7a` and has no value store.
- `140ecfb90`, `140ecfc30`, `140ecfcd0`, `140ecfd70`, `140ecfe10` (rows1..5):
  index0 writes +0.0 at values[row*cols], while index1 only enables its input.
  The last row's index1 branch is `140ecfe7c..fe98`; the first four have the
  same retained source structure with their respective row/receiver offsets.

The getters obtain the live SC through `140130f90`. The present candidate
already binds that SC to the actual GuiModel and uses cols1, so each row maps
to values[row] without a new alias or owner. The literal at `1421d3c90` is
verified -1.0. Existing exact +0 and first-row -1 callbacks are bit-idempotent
for this profile. New negative integer custom cases make no frequency store;
their signed decimal display is already implemented. This establishes the
same no-frequency-mutation Fill subset without changing the const SC API.

This report qualifies the custom callback's requested enable operation, not
effective enabled state through all QWidget parents or a user-edit acceptance
path. Neither is needed to expand the current isolated snapshot profile.

## Minimal exclusive source/test contract

After root freezes/releases CP85, two files suffice:

1. `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: remove only the
   `value<0 && !(row0 && value==-1)` refusal. Keep finite, exact-converted-int,
   negative-zero, shape and other page/live-model guards. Clarify the Fill
   comment that signed exact integers preserve stored frequency bytes.
2. `source/apps/moproj2mst/tests/mdt/test_simcontrol300_snapshot.cpp`: add focused
   signed-frequency cases and reconcile its old negative rejection assertion.

No header signature/layout change is necessary: its current contract already
says existing6x1 integer FRQNCY. No CMake, runtime, public session, lifecycle,
palette or movement-path source needs an edit.

Recommended focused cases:

- Fill all six rows with distinct exact negative int32 values including first
  row -2, a later row -1, INT32_MIN and -2147483647. Assert first custom index2,
  other custom index1 and exact signed decimal text. Preserve every raw double
  bit through Fill and advanced Apply. Change a live frequency after Fill;
  Apply must still leave that current model value untouched.
- At each row, exercise valid signed limits and retained refusals: negative
  zero, fractions such as -0.5/-1.5/-2.5, values outside int32 range, infinity
  and NaN. No refused Fill may mutate FRQNCY. First-row -1 retains its existing
  sentinel/no-populated-text case; a later-row -1 is ordinary custom text.

The current `advanced_frequency_snapshot_retains_only_original_populated_text`
puts -2 in its first-row rejection list. Move that value into the accepted
coverage; retain -0, 1.5, 2147483648 and infinity refusals. Do not relax the
exact-integer condition: fractional first-row values truncating to -1 would
cause the original sentinel callback to overwrite the raw double, which this
const Fill subset does not model. Negative zero similarly reaches a +0 store.
NaN and out-of-range CVTTSD2SI's INT_MIN result are not legitimate integer
inputs. Actual exact -2147483648 is legitimate and distinct after the finite/
equality checks.

Root target remains `m2m_mdt_simcontrol300_snapshot` / CTest
`m2m.mdt.simcontrol300_snapshot`. This report did not build or run it.

## Evidence receipt and next boundary

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Verified DUMPBIN 14.44.35228.0 SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Preferred-VA half-open raw ranges and retained files:

| Range/file | SHA-256 |
|---|---|
| `140ec6d18..140ec74e3` (frequency fill body) | `49c1ef820307996f1e2f457c98be1945e5b6d9ca3bef59003a2e91c490980b56` |
| `140ecfab0..140ecfea3` (six callback bodies plus alignment) | `56dce6c9c57217e103b478b2243f6c20a5cf4b9feeb2ff01a3662dc1f39da693` |
| `wave2-doe-sim300-default-callbacks-decompile.json` | `52bc48c86351fd21cc6945ba57186f25f8f147947411a67b0a00452155687ee8` |
| `wave2-save-diffs-300-advanced-fill-complete.asm` | `ace713b8a8bec7217e7178ecadf86c1d10ca468d8797dd0e3b4cedd0f30f3c16` |
| `wave2-save-diffs-300-apply-140ec7500.c` | `f36256fe2a8bae92c2e23ebf8d656eb0ef45be2bc3486222eb91ff8bf9f6ded9` |
| `wave2-save-diffs-300-advanced-contract.json` | `532487e05a0e51fb9730489543b406b981dfe39d9f2904ad36ddb0b2c37eb206` |

All retained files in the table are under analysis/. No new raw artifact was
written. Fractional/negative-zero normalization, frequency shape allocation,
interactive frequency editing and public constructor reachability remain
separate guarded dependencies after this proposed integer increment.
