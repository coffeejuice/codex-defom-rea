# CP141 source review: solver6 Fill release

## Result

The released source matches the corrected CP141 3D callsite and bounded admission. The exact-int32 predicate is safe for its stated domain, the four text conversions follow original order, raw SC values/count are preserved by Fill, and solver6 Apply remains rejected before candidate writes. The header comments describe the private text field as bounded and unknown for other fills. No source-level blocker found. Static review only; no build, GUI, original-app runtime, or mutable REA action was performed.

## Evidence checks

The corrected `analysis/resume141-editor-next.md` identifies original DEF_GUI_MO_64 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and actual 3D solver6 branch `[140ec2412,140ec26de)` SHA-256 `8bd29ab5e6548e966fb1c163b2dbb656b51fca4de1cfabaf0f8f3978bdbe0637`. The released source comments cite `140ec2552/25be/2627/2690`, which are the two g6 and two decimal-integer QString construction calls in that branch; the assembly places the corresponding QLineEdit setters at `258e/25fa/2663/26cc`. The earlier 2D body is not used as the 3D source citation.

`exactSignedInt32` inspects binary64 bits without converting rejected inputs. It admits signed zero; for exponents 0 through 30 it requires all fractional bits to be zero; at exponent 31 it admits only negative exact 2^31. Exponents below zero, above 31, all nonzero subnormals, and nonintegral/out-of-range values refuse. This is a candidate admission restriction, not a claim that native CVTT refuses those inputs. The focused tests cover both int32 endpoints, signed zero, representative integral values, fractions, nonfinite values, nonzero subnormal, and out-of-range values.

`fillIteration3D` gates solver6 p0/p1 for finiteness and p2/p3 with the exact predicate before normalization or page construction. The established `normalizeCgAtOpen` call is not changed by this review; solver6's known path does not acquire a new count normalization. The branch writes `qt::number(p0,'g',6)`, `qt::number(p1,'g',6)`, then decimal integer strings for p2/p3. Test coverage checks representative literal outputs and compares all four raw parameter bit patterns and `u2e8` before/after, including counts 0, 4, and 77.

`IterationPage::explicitParameterText` is an array of optional strings with a comment that text is populated by the bounded solver6 Fill and unknown for other fills. The adjacent solver6 comment explicitly retains Apply and unobserved edit histories as guards. `applyIteration3D` excludes solver6 in its opening condition, before `sc.u2e8 = 0` or any solver/parameter writes. The focused test confirms Apply returns `UnsupportedSnapshot` while preserving model/count and public incompletion.

The visibility-retention test exercises the candidate owner's declared toggle API on the retained page, confirms the field toggles, and verifies the optional text array stays equal to its saved value through 6→non6→6. Original callback evidence supports show/hide-only semantics; this test proves candidate model-state retention, not native event/widget delivery.

## Limits

No runtime comparison was performed against original QString formatting, CVTT status/exception behavior, or native widget state. Solver6 Apply/Save, unfilled or manually changed solver selection, validator states, and behavior outside the finite/exact-int32 profile remain unsupported.

## Frozen inputs

- Corrected proposal: `analysis/resume141-editor-next.md` (contains actual branch SHA `8bd29ab5e6548e966fb1c163b2dbb656b51fca4de1cfabaf0f8f3978bdbe0637`)
- Fill source: `source/apps/moproj2mst/src/engine/simcontrol300.cpp` — `E362DE4A524FF7B3E00EE783FCC854B31144A7E1A88AC6DA6ADDF95E67D9641B`
- Header: `source/apps/moproj2mst/include/m2m/engine/simcontrol300.hpp` — `A79E50008F9CE2D7FE983EDDD689C5FD43956F50DD0A91976E3EC467BBC5EF49`
- Focused test: `source/apps/moproj2mst/tests/mdt/test_simcontrol300_explicit_fill.cpp` — `8534345E739B3102DC7BF37C0E4D9555FC6EAF3EA2F062BA687354F65AB09871`

Root owns build and integration validation.
