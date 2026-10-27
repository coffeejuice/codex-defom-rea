# CP292 staged ordinary-finite render transform

Integration note: this report records the historical staging state and its independently reviewed derivations. The reviewed header, implementation and test were subsequently copied unchanged into source/apps/moproj2mst/src/engine and tests/mdt. The current combined build and source-bound regression are recorded in analysis/cp292-293-full-validation.json. The staging scratch directory was retired after its compact evidence was retained under these analysis paths: cp292-transform-staging-compile.cmd, cp292-transform-staging-compile.log, cp292-transform-staging-test.log, cp292-transform-derive-analytic-expectations.py, cp292-transform-analytic-expectations.json and cp292-transform-mutation-checks.json. Public rendering remains HOLD.

Ownership released: only `curve_render_transform.hpp`, `curve_render_transform.cpp`, `test_curve_render_transform.cpp`, this report, and isolated `scratch/` outputs were written. No `source/`, CMake, shared plan, live build directory, original executable, GUI, Frida, or mutable REA session was touched. Nothing was committed. Public rendering remains HOLD.

## Contract and integration

The source is the retained immutable-target qualification and bounded instruction analysis in `analysis/resume292-render-transform-static.md` (original DEF_GUI_MO_64 retained v11.0.2; SHA-256 fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc). This implementation is a reconstruction of that static contract, not newly demonstrated runtime equivalence.

Copy the header and implementation verbatim to `source/apps/moproj2mst/src/engine/`. Copy the test verbatim to `source/apps/moproj2mst/tests/mdt/`. The test selects the actual `../../src/engine/curve_render_transform.hpp` include there; only the isolated staging compile uses its local fallback. Link the implementation into the appropriate converter/test target and retain strict scalar floating behavior (no fast-math, reassociation, or contraction). The implementation explicitly disables MSVC contraction and enables precise evaluation; isolated validation used `/fp:strict`. This API does not wire a public renderer, derive width from tolerance, or resolve render/color ownership.

`transformCurveRender2D(int32_t,const double*,double,double*,bool)` uses explicit supplied width/relative state. Results carry outcome, ordered admission reason, optional native status, retained count, closed flag, output-write flag, and optional stopped correction segment plus partial retained-point diagnostic XY. `nativeStatus` is set only for recovered admission (-1) or deterministic completion (0). The module repeats admission at the subordinate core; its zero-filled scratch would be expanded even on subordinate rejection. Such a rejection is unreachable for successfully admitted finite wrapper arguments under the qualified count domain.

The implementation preserves strict `1e-12 >= abs(width)` admission (NaN passes it), bbox/distance threshold arithmetic, equality coalescing, forced final retained point, mapping expansion, zero-initialized normal/scale/transformed scratch, cyclic initial normals, old-normal smoothing, biased ternaries, relative scaling after admission, width/scale/component operation order, correction ordering, exact-slope intersection behavior, strict-between and both-component displacement predicates, and final seam selection. The native 24-byte normal scratch stride is retained.

A failed ignored mode-1 intersection stops BEFORE a native indeterminate candidate read. It returns `indeterminateCandidate`, unset native status, diagnostic already-computed retained output and the correction segment. Caller output is left untouched by this analysis seam on incomplete paths; this is explicitly an implementation qualification, not an invented recovered native rollback or early return. The negative-width two-point case exercises this stop.

Nonfinite inputs/width or nonfinite arithmetic intermediates and zero divisors are classified `outsideQualifiedArithmetic`, without guessing native exceptional-floating outcomes. Signed 32-bit doubling overflow or impossible container size is `outsideQualifiedSize`; allocation failure is `outsideQualifiedAllocation`. Allocations use C++ value-initialized scratch with recovered order where observable within the finite core. Native fault cleanup, exact allocator calls, exceptional-floating CRT behavior, arbitrary bad pointers, uninitialized bytes, and exact allocation-failure behavior are not qualified. No fault injection was performed.

## Isolated checks

MSVC 14.44.35207, x64, C++20. `scratch/compile.cmd` uses the installed developer environment, Testkit's actual include directory, `/EHsc /W4 /O2 /DNDEBUG /D_CRT_SECURE_NO_WARNINGS /fp:strict`. Compile exit 0, test exit 0, **15 cases passed**; complete compact logs are `scratch/compile.log` and `scratch/test.log`. No CMake build ran. No DMO or reconstructed DEF_PRE binary was built or deployed.

Cases cover ordered rejection and untouched output; NaN admission followed by domain classification; positive absolute/relative two-point displacement; vertical geometry; grouping equality and forced-last representative; open collinear interior; open corner; closed square/seam; ignored parallel-intersection indeterminacy; count and floating-domain qualification; a single retained group; and asymmetric smoothing using all old normals. Checks use Testkit's actual supported macros and entry point; there are no C asserts that disappear in Release. The asymmetric test's first hand expectation initially chose the wrong limiting edge; analysis of the recovered `s2>s1 ? s1 : s2` formula corrected that expectation, with no implementation formula change. Geometric irrational expectations use a stated 1e-14 bound; simple exactly representable expected results use bit checks.

SHA-256 at release:

- curve_render_transform.hpp: b18b4a555ea012ff38f10e92de82ef08916e0ee6cd49abdbc7908b9be33e3f02
- curve_render_transform.cpp: 22a5d439d940266387a04d2ece226b1f6389c0b50a82a875aa00358a590fbd4e
- test_curve_render_transform.cpp: 87c3d10706ae5cf9b5b9da8255367a73a00af5d037c32b6bfdf45ca7a0447336

The root integration owner must run the changed targets together and the required source-stable regression/freeze. Static contract tests do not claim original runtime parity, public render readiness, or complete Open-to-Save acceptance.

## Independent-review coverage follow-up

The original 12-case source passed independent review with two coverage requests. Three cases now check successful sequential correction and both unequal seam branches. No header/implementation formula changed and no discrepancy was found. Current isolated optimized strict-FP build/test exits are 0/0 with 15/15 passing; `scratch/compile.log` and `scratch/test.log` are the release logs. Public HOLD and runtime qualification are unchanged.

A bounded scratch Python formula search selected fixtures; its output is exploratory and is not the expected-value oracle. Final expectations were derived separately with 80-digit Decimal square roots and vector determinant intersections in `scratch/derive_analytic_expectations.py`; full values are in `scratch/analytic-expectations.json`. Unlike the native slope implementation, this derivation intersects rays A+t*u and B+s*v as `A + cross(B-A,v)/cross(u,v) * u`. It does not execute the staged C++ module or an original program.

For the sequential fixture, points are `(0,0),(4,-4),(6,-4),(6,0),(0,6),(6.5,0)` and width is 6. Endpoint normal is `(1,1)/sqrt(2)`, placing initial displaced point zero at `(3*sqrt(2),3*sqrt(2))`. Initial normal one has angle 67.5 degrees; its two neighbor sums have angles 56.25 and 101.25 degrees. Recovered smoothing weights are 11.25 and 90, so its displacement ray direction is the sum of those three independently specified angle vectors with those weights. Neither directional limiter clips it. Intersecting that ray from `(4,-4)` with `y=x` yields `t=3.2016154654039349845...`; the nonconvex-quad correction replaces both displaced ends with that intersection. Original segment j=3 is `x+y=6`, so its next successful segment-mode intersection shortens point zero to exactly `(3,3)` and both displacement-component predicates pass. Segment j=4 from `(0,6)` to `(6.5,0)` meets `y=x` at `(78/25,78/25)=(3.12,3.12)`. It lies beyond the shortened interval and must be rejected, although it lies inside the pre-j=3 interval. Exact-bit checks require final `(3,3)`; no tolerance is used for this correction expectation.

For the wide closed triangle, A=(0,0), B=(4,0), C=(0,2), followed by A; width is 1. Write U(v)=v/length(v), h=1/sqrt(2), r=sqrt(5). Independently derived old normals are n0=(h,h), n1=U((-1/r,1-2/r)), n2=U((1-1/r,-2/r)). Displacement ray directions before normalization/scales are:

- k0 = n0 + 2.5*U(n0+n2) + .625*U(n0+n1).
- k1 = n1 + .625*U(n1+n0) + .5*U(n1+n2).
- k2 = (2/r,-1/r-.2), because the recovered x<0 directional limiter selects c at C; the other directional limiter is nonnegative.

k0 and k1 are not clipped. Normalization/scales do not alter these ray lines. Initial displaced points are `(2.9626944462854012512,1)`, `(-.79422271604592608653,1.4196578985619118406)`, and `(4.0507323981137496717,-.93113749914118335186)`; the repeated final point begins with the first displacement. Vector-determinant intersections of rays AB, BC, CA are respectively:

- Q01=(1.8692900517495214212,.63094257124396338170).
- Q12=(1.9077155228133837127,.61956407951908046075).
- Q20=(1.8847700834179234250,.63616755544299569284).

The successive nonconvex corrections replace both endpoints with each intersection while it is strictly inside both current ray intervals, yielding pre-seam Q01,Q12,Q20,Q20. None of those inward intervals crosses an original triangle boundary away from its originating vertex, so the original-segment pass does not change these points. Seam distances are strictly `1.9728998519381748589 < 1.9892379007915375593`, selecting first-to-last; expected result is Q01,Q12,Q20,Q01.

For the tall triangle `(0,0),(2,0),(0,4),(0,0)`, transposing wide coordinates and reversing vertex order preserves CCW winding. Let T swap XY. Pre-seam endpoints/rows are T(Q20),T(Q20),T(Q12),T(Q01): at the second correction T(Q12) lies beyond the shortened first ray interval, and at the third T(Q01) lies beyond the shortened first ray interval, so those two strict-between replacements are excluded. The first seam distance is now the larger value and last-to-first is selected. Expected result is T(Q01),T(Q20),T(Q12),T(Q01). Both seam tests assert the independent strict distance gap exceeds .016, compare coordinates to the 80-digit derived values within 1e-14 (allowing binary rounding and determinant-versus-slope evaluation order), and require copied seam coordinates to match bitwise.

Before the root's stop request, three isolated scratch mutation checks completed. Freezing per-original-segment correction intervals fails only the new sequential test; forcing first-to-last fails only the new tall-triangle test; forcing last-to-first fails only the new wide-triangle test. Each mutant compiled and returned one failed case as intended, with detailed receipts in `scratch/mutation-checks.json`. They were never applied to the staged module. No further optional checks ran after the stop request. These mutation results demonstrate that the new cases distinguish the requested state-update/branch decisions, not original-runtime parity.
