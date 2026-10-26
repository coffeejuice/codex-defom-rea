# CP141: bounded finite solver6 Iteration Fill

The next source-closed prerequisite is private3D solver6 Fill with finite parameters0/1 and exactly representable signed-int32 parameters2/3. It can publish four actual formatted edit texts and visible explicit controls without changing SC parameters/count. Keep solver6 Apply and public constructor gates unchanged; visibility alone did not establish these texts in CP140.

Only this report was written. No source/build/GUI/original execution/REA database change occurred. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Evidence is retained native assembly/decompile and immutable-PE bytes/import mapping, not observed native constructor or event delivery.

## Exact 3D Fill branch

The3D solver6 branch `[140ec2412,140ec26de)` has SHA-256 `8bd29ab5e6548e966fb1c163b2dbb656b51fca4de1cfabaf0f8f3978bdbe0637`. It confirms SC nonnull/solver6, checks UI+b00 at244e, then directly shows UI+b18,b28,b38,b48,b10,b20,b30,b40 at246a..252e. It then populates in this order:

| Parameter/source | Conversion and setter |
| --- | --- |
| SC+2f0, p0 | QString::number(double,'g',6) at2552, setText(UI+b18) at258e |
| SC+2f8, p1 | same g6 conversion at25be, setText(UI+b28) at25fa |
| SC+300, p2 | CVTTSD2SI EDX at2614, QString::number(int,10) at2627, setText(UI+b38) at2663 |
| SC+308, p3 | CVTTSD2SI EDX at267d, QString::number(int,10) at2690, setText(UI+b48) at26cc |

Each temporary QString is destroyed after its setter, then the branch joins the remaining normal Iteration Fill at26de. Exact imports are141e04798 `?number@QString@@SA?AV1@NDH@Z`,141e03dc0 `?number@QString@@SA?AV1@HH@Z`,141e066b8 `?setText@QLineEdit@@QEAAXAEBVQString@@@Z`. Full retained Fill assembly SHA is `7e6924b494cb7cb747d9e5325850cf4591cd3658a3593c19b0f32274537ce56d`; decompile SHA `e9916be99bda0baf03df22d5d9fd3f56d5463292df0f8389910e52d01e086e7b`.

Correction before final source release, authorized by root: proposal revision SHA `114fd3c4e7155ce088a93f1ddd03ebfa6924f70d14515dc45d2a40c3fc1dbe68` mislabeled the earlier identical formatting leg1e3a..20ff as3D. That earlier leg's SHA `95ab6cd142da5d1114f668caef3d6730314b3d3f02ffa418fcc2ccf5c2851dde` remains a valid2D capture. Native130ea0 compares dimension with2 and returns true iff2; its half-open body130ea0..130ed3 SHA is `5522b3400c3666684a37f59c348a0a83cdf5d01317a7237bb3556850d09b5d92`. Fill calls it at1cd4 and JE at1cde routes3D(false) to20ff and the later leg above; routing bytes1ccc..1ce4 SHA `e1898dd7a532e2f0ed532ba5ec3c4b7f19f1396723bbf9874a5be012197675d8`. Both legs implement the same four conversions. Implementation behavior/tests are unchanged; source comments now cite actual3D callsites.

The branch never reads/writes u2e8 parameter count and never writes the four source parameters. Their storage is a fixed four-double model field at2f0, independent of count. Do not normalize count4, round SC values, or invoke CG normalization for solver6. Counts0/4/77 may remain unchanged under this bounded typed model profile.

## Numeric and ownership admission

Retain existing3D/KnownC, method/thermal radio and finite-CVGERR guards. For solver6 only, require p0/p1 finite and p2/p3 exact signed-int32 values, including+0/-0. Reject nonfinite, fractional, out-of-range last values before executing their CVTT instructions. These are candidate admission bounds, not native validation: original Fill truncates fractions and may reach SIMD exception/indefinite cases that this increment does not recover.

Use an IEEE-binary64 bit predicate for exact-int32 admission rather than converting rejected values merely to decide the guard. Zero magnitude accepts either sign. Nonzero unbiased exponent below0 or above31 refuses; exponent31 accepts only negative exactly2^31; exponent0..30 accepts only when all mantissa bits below the integer boundary are zero. After that guard, retain the existing CVTT-based integer adapter and base10 formatting. This avoids adding invalid/inexact CVTT behavior for excluded profiles and needs no assumed current MXCSR masks for those admitted exact conversions. Do not claim whole Qt formatting/native FP-status parity; reuse the existing known numeric value boundary, including source-g6 rounding for p0/p1.

Unknown actual QWidget/QString allocation/refcount/emissions or formatting failure timing remain outside the private value projection. The normal-return final page can publish the complete tuple; this does not certify native partial widget effects on excluded input or exceptions.

## Shared field and minimal implementation

Root appends to IterationPage:

```cpp
// Text actually populated by the explicit Fill branch; unknown for non6.
std::array<std::optional<qt::String>, 4> explicitParameterText{};
```

Worker modifies only fillIteration3D and a private exact-int32 predicate in simcontrol300.cpp. Admit solver6 under the profile above, keep all previous solver behavior, set explicitControlsVisible=true for that case, and populate the four optional texts in source order. Non6 Fill retains visibility=false and disengaged explicitParameterText rather than invented empty/default strings. Preserve remaining method/thermal/convergence/bandwidth/iteration fields exactly as before. Existing normalizeCgAtOpen is a no-op for6 and must not be expanded.

No owner callback rewrite is needed: a subsequent declared explicit toggle only changes visibility and must retain these newly known texts. Keep applyIteration3D's solver6 UnsupportedSnapshot guard before candidate model stores. A non6→6 post-Open selection without this Fill still has unknown explicit edit texts; do not seed them from SC/defaults or claim Save support.

Root owns field/header qualification comments. Worker exact files: existing `src/engine/simcontrol300.cpp`, NEW `tests/mdt/test_simcontrol300_explicit_fill.cpp`, implementation receipt. No owner.cpp, lifecycle, CMake, parser/writer or public Session change. Expected target `m2m_mdt_simcontrol300_explicit_fill`, CTest `m2m.mdt.simcontrol300_explicit_fill`.

## Focused tests and prior guard reconciliation

- Actual GuiModel SC solver6 with p0=1.234567891,p1=12345678.9,p2=INT32_MIN,p3=INT32_MAX yields literal texts `1.23457`, `1.23457e+07`, `-2147483648`, `2147483647`, visibility=true and unchanged raw source/count. Existing Qt number precision/exponent semantics are reused, not reimplemented.
- Boundary integer pairs,±0 and count0/4/77 preserve raw bits/count while formatting last-two zeros as `0`. Verify first-two general-format values and other filled controls retain existing behavior.
- Reject fractional/out-of-range/NaN/Inf last values, nonfinite first values, wrong dimension/locale and old invalid radio/CVGERR domains before model mutation. Preserve all previously accepted non6 results and unknown optional explicit texts.
- Construct actual bound Project/Operation/GuiModel/StagedEditor300 private pages with the qualified solver6 SC and declared existing initial feature observations. They may now reach AwaitingNativeBoundaries with all four known texts; this remains private page completion, not public palette/constructor completion.
- On that retained page,6→non6→6 explicit toggle events change visibility only and retain populated texts; existing solver6 Apply remains unsupported and does not mutate count/model.

Two old refusal sites were checked. Snapshot test `iteration_guards_are_explicit_and_other_solver_parameters_stay_unwritten` uses initialIteration p2=-3.9,p3=5.9 before solver6 at line265, so remains refused under the new fractional guard. CP140 `explicit_visibility_does_not_enable_unknown_solver_six_fill_or_apply` sets each p[i]=i+0.25, also still excluded. No amendment to those old tests is required. Read-time hashes: snapshot `0f52dff5154f5fb70c4566be753f03d0183a9fc0fba82c0ea66e4032530ac250`; explicit-toggle test `85ff18816f579adc1a78110c76781f5fb7bae9a78031320dec511ce166d94564`.

## Next boundary

Future solver6 Apply needs the actual four known texts and sequential toDouble value/conversion admission. Native Save clears count, selects6/count4, writes p0..p3 in order through toDouble for **all four**, then continues method/thermal and convergence validation. Last-two integer Fill formatting does not make Save integer-only. This proposal does not implement that successor or release any native/default text/locale/validator/partial-write guard. Selected public palette history/6504abb0 remains the earlier independent dependency.

No new raw evidence outside this report or reconstructed executable/deployment was produced. Source requires root's declaration, exclusive assignment and frozen-tree release.
