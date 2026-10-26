# CP86 independent source review — signed FRQNCY Fill

## Result

The frozen change removes only the extra refusal for negative values other than the existing first-row `-1` sentinel. The Fill profile still requires the established six-row, one-column, six-component FRQNCY value array. Each value must be finite, exactly equal to the result of the source-matched CVTTSD2SI conversion, and not negative zero. That admits every exact signed int32, including `INT32_MIN`, while refusing fractions, nonfinite values and out-of-range inputs.

The row-selection and text projection preserve the native cases: row zero `-1` remains combo index 1 with no populated text; other nonzero row-zero values, including negative values, use custom index 2 and signed text; later nonzero values, including `-1`, use custom index 1 and signed text. In the original callback evidence, those custom branches only enable their inputs and do not store FRQNCY. Candidate Fill projects the selection/text into `AdvancedPage` and does not modify the values.

## Guards and conversion

In `source/apps/moproj2mst/src/engine/simcontrol300.cpp:1159-1205`, the Fill entry retains model/SimControl identity, 3D, KnownC locale, movement-path profile, page-state guards, and the existing FRQNCY shape checks (`rows==6`, `cols==1`, `ncomp==6`, nonnull values). For each of the six values, it uses the existing `_mm_cvttsd_si32`-backed `integer()` helper. `!isfinite(value)` rejects NaN/infinity; comparing the original double to the converted int rejects fractions and out-of-range conversion results; `value==0.0 && signbit(value)` specifically rejects negative zero while allowing positive zero. Exact `-2147483648.0` remains legal because its conversion and equality match; values below it convert to the x86 indefinite `INT_MIN` but fail equality. Exact `2147483647.0` is legal; `2147483648.0` fails equality.

The production diff described in the freeze removes the negative-value-only condition. It does not change shape/allocation, combo mapping, callback handling, API, or any source used by Apply. The candidate `SimControl` initializer sets FRQNCY to a six-component DYNARY and allocates its value array; no new pointer/alias source was introduced for this narrow signed-value extension.

## Row-zero and later-row behavior

After validation, the unchanged projection maps `0` to index 0; `-1` in row zero to index 1; every other nonzero row-zero integer to index 2; and every nonzero integer in rows 1–5 to index 1. Text is set for nonzero values except the row-zero `-1` sentinel. Therefore row-zero `-2` displays `-2` on the custom input, while row-one `-1` is an ordinary custom value displaying `-1`.

The retained original callback decompilation/bytes in `analysis/wave2-doe-sim300-default-callbacks-decompile.json` and `analysis/wave2-save-diffs-300-advanced-fill-complete.asm` qualify why this projection is safe for the selected profile. `140ecfab0` index 2 only calls `setEnabled(true)`; it does not write the first FRQNCY slot. The later-row callbacks' index 1 cases likewise only enable their corresponding inputs. By contrast, row-zero index 1 writes the `-1` sentinel, and row-zero index 0 writes zero; rows 1–5 index 0 also write zero. Avoiding those write-producing indexes is necessary to preserve raw negative inputs during Fill.

## Apply and test boundary

`applyAdvanced3D()` does not read or store `frequencyCombo` or `populatedFrequencyText`; its existing `advancedApplyProfile()` rechecks model/path and other page guards, then Apply writes unrelated advanced values. It does **not** revalidate FRQNCY at Apply. That is consistent with the current source contract because Apply does not consume the remembered frequency display. The tests confirm this behavior: Fill and Apply preserve input bits, and a later live model change to `-909.25` remains in FRQNCY after Apply. They do not claim an interactive frequency-edit callback has no effect, nor do they claim Apply rejects a changed live FRQNCY value.

`tests/mdt/test_simcontrol300_snapshot.cpp` retains the first-row `-1`/no-text case, admits row-zero `-2` with index 2 and `-2` text, and covers six distinct signed values including a later-row `-1`, `INT32_MIN`, and `-2147483647`. The per-row limits test admits both signed bounds in each of six positions and refuses `-0`, negative fractions, values outside int32 range, infinities and NaN while checking source bits remain intact. Raw doubles are checked across Fill/Apply; the changed live model value check establishes Apply preservation, not a frequency-specific recheck.

No discrepancy found in the requested behavior. Earlier profile shape guards and public editor300 constructor reachability remain separate limitations.

## Frozen hashes and validation

| File | SHA-256 |
|---|---|
| `src/engine/simcontrol300.cpp` | `0568dc61f1023d28da4930338dbb99119fc4c2aef78f0399d3cb1be042534b68` |
| `tests/mdt/test_simcontrol300_snapshot.cpp` | `0f52dff5154f5fb70c4566be753f03d0183a9fc0fba82c0ea66e4032530ac250` |

The CP86 implementation report records static preflight only. I made no source edits and did not build or run tests.
