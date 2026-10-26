# CP141: bounded private solver6 Apply successor

The native solver6 Apply order is source-closed at the existing value-projection seam: clear parameter count, select6/set count4, convert and store all four edit texts sequentially, then store solver/method/thermal selections and process the existing validation tail. The next useful private increment can admit an explicit bounded text profile without inventing text from visibility, imposing integer-only last fields or changing the public constructor guard. No source edit was made in this lane.

## Dimension and immutable evidence

Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. `140130ea0` returns dimension==2, so an actual3D receiver takes false. Apply tests that result at31b4/31b6 and enters the later3D leg at35ec. Do not label the earlier32xx explicit block as3D merely because the decompiler displays the false branch first.

The same independent PE check found that Fill's dimension call at1cd4 takes false to20ff; its later explicit leg is3D. Editor300 corrected its CP141 Fill report/source address comments before freezing. Corrected proposal `resume141-editor-next.md` SHA `b01386c02f7fffe10b9890c8308b455b858a715a1fdca18ba5246a32ac211fcf` identifies3D Fill `[140ec2412,140ec26de)`. This changes evidence labels, not the common g6/g6/int/int Fill algorithm.

Retained Apply decompile `wave2-save-diffs-300-apply-140ec3130.c` SHA `8f49f547a6d720a0047d325140949e530aeb4cdd0099eeabeebbc154cd89526d` and ASM `wave2-save-diffs-300-save-140ec3130.asm` SHA `f464b82617ebd1a47ac70891efcaddb90eead1215a8e13c3d2c38c2f940f54cc` were read alongside section-mapped PE bytes. Independently verified end-exclusive native ranges:

| Range | SHA-256 |
| --- | --- |
|140130ea0..140130ed3 dimension predicate|`5522b3400c3666684a37f59c348a0a83cdf5d01317a7237bb3556850d09b5d92`|
|140ec3130..140ec3f8b full Iteration Apply|`aecf667709da2abfc1e411cfe511ef3851ac38e217f080d542edba09abd534fc`|
|140ec38b6..140ec3a7d3D explicit parameter leg|`90149b9fb91edf33fb8b9c72ce385ffcd9367bba7a3be66f39c5154aab052bad`|
|140ec3c12..140ec3f8b common validation/tail|`e13eb8f48eab9c4db35e0f52884b7db07cf30d364625f9ca4f8059dd15f43a70`|

## Exact normal and early-return order

Native entry constructs the QString, QDoubleValidator and QIntValidator before the dimension/radio branch. Their actual QObject/QString allocation/EH lifetime is not supplied by the private value helper; validator results remain explicit existing projected input.

For the3D explicit selection:

| Order | Native occurrence / model effect |
| --- | --- |
|1|35f4 stores SC+2e8/u2e8=0 before radio selection.|
|2|38b6 selects local solver6;38c9 stores u2e8=4. This local selection does not yet write SC.solmtd.|
|3|UI+b18 text at38f1; QString::toDouble(nullptr)3919; p0/SC+2f0 store3927; QString destruction3937.|
|4|UI+b28 text395b; toDouble3983; p1/+2f8 store3991; destruction39a1.|
|5|UI+b38 text39c5; toDouble39ed; p2/+300 store39fb; destruction3a0b.|
|6|UI+b48 text3a2f; toDouble3a57; p3/+308 store3a65; destruction3a75.|
|7|Read method/thermal radios, then store solmtd/+2e4 at3be8, itrmth/+2e0 at3bfa, solmtt/+318 at3c0c.|
|8|Validate convergence0 at3c7f. Invalid0 returns through cleanup with count/parameters/three selections retained; neither convergence value nor later bandwidth/iteration fields is written.|
|9|For non-Invalid0, toDouble3cbf stores cvgerr0/+2d0 at3ccd. Validate convergence1 at3d42; Invalid1 retains that additional store and returns before cvgerr1/later fields.|
|10|For non-Invalid1, toDouble3d82 stores cvgerr1/+2d8 at3d90. Then defbwd/+314 at3dbf and tmpbwd/+320 at3dec.|
|11|Integer iteration validator Invalid0 writes200 to itrmxd/+310 at3e78, otherwise toInt/store3ea2. Thermal iteration follows independently, Invalid writes200 at3f2f, otherwise toInt/store3f59. Common cleanup/return follows.|

There is no parameter validator or conversion-success branch for any of the four explicit edits: each passes null for the toDouble success pointer. A malformed numeric text is not a native validation stop. The last two Apply conversions are double conversions, despite their integer formatting in Fill. Intermediate convergence/integer validation states follow the nonzero branch rather than Invalid handling. The native function returns void; the existing ValidationStopped enum denotes this page's partial return, not a global Save abort. Existing lifecycle composition continues later pages for ValidationStopped and stops only its candidate UnsupportedSnapshot boundary; do not change that behavior in this increment.

## Smallest recommended implementation and admission

Extend only existing applyIteration3D plus one private lexical predicate, using the four optional `IterationPage.explicitParameterText` fields already supplied by CP141 Fill. No new model field, owner, callback or public API is needed. For solver6 require all four texts known before source-valued writes. Non6 branches continue to ignore those texts. A non6→6 selection with unknown setup/edit history stays guarded; do not seed zero/default/current-SC text from the visibility event.

For a particularly small first numerical profile, admit ASCII decimal/scientific text with optional sign, one optional decimal point, at least one mantissa digit, at most15 total mantissa digits, and optional e/E exponent with optional sign and one/two digits of magnitude<=32. Reject other characters, whitespace, empty/null/missing input, NaN/Inf, and out-of-profile length/exponents before writes. This is a **candidate admission**, not original validation or a requirement that all CP141 Fill values be saveable. Nonzero admitted magnitudes lie between approximately1e-47 and1e47, avoiding overflow/underflow/nonfinite domains. It includes ordinary g6 outputs, signed32 decimal endpoints, signed zero and fractional last-two texts; larger g6 exponents remain explicitly guarded until widened deliberately.

The gate can inspect characters/integer counters without converting a double merely to decide admission. After it passes, preserve the actual value sequence: u2e8=0, explicit u2e8=4, then `sc.solmtd_p[i] = qt::toDouble(*form.explicitParameterText[i])` once for i0..3, followed by the unchanged selection/validation tail. Do not preconvert all four into a temporary array or move convergence checks before parameter writes. Do not require explicitControlsVisible=true: native Apply reads the selected radio/texts, not visibility. Do not normalize last-two texts through toInt or reuse pre-Fill raw values.

Existing candidate `qt::toDouble` at src/qt4/number.cpp:478 is the established C-locale value adapter (file SHA `bf90da77dc90b3adce1f688b897207686b430ab56bf6a149725a8e3f28d51ee6`). Normal numeric spelling is converted once at each store. Native Qt widget reads, QString allocation/refcounts, locale-global changes, signals, numeric FP/TLS status and exception/cleanup timing remain outside this admitted synchronous value projection. If a broader numerical profile is preferred, qualify it explicitly rather than making success/finiteness checks into invented native validation returns after earlier writes.

## Reservation and focused tests for the next checkpoint

After root's CP141 archive/release, reserve existing src/engine/simcontrol300.cpp, NEW tests/mdt/test_simcontrol300_explicit_apply.cpp, and the final Apply-guard case in tests/mdt/test_simcontrol300_explicit_fill.cpp. Root may update the header's old blanket solver6-Apply exclusion comment; no structural/shared interface change is required. Existing globs supply the new target `m2m_mdt_simcontrol300_explicit_apply` / CTest `m2m.mdt.simcontrol300_explicit_apply`.

Required focused cases:

- Actual bounded solver6 Fill followed by Apply writes count4 and formatted-round-trip p0/p1 rather than original raw values; p2/p3 integer texts convert through toDouble. Test counts0/77 and preserve filled text/visibility.
- Explicitly supplied known texts `-3.25` and `2.5e-1` in the last two positions prove Apply is not integer-only. Include signed-zero bits and a hidden/unknown visibility projection with cached solver6 to show visibility is no prerequisite.
- Missing each optional or excluded spelling in a later position refuses before any count/parameter/selection write under the explicit candidate admission. Keep those guards separate from native ValidationStopped.
- Invalid convergence0 leaves all four new parameters, count4, solver/method/thermal stores but old convergence/bandwidth/iteration fields. Invalid convergence1 additionally retains cvgerr0. Intermediate continues, with integer Invalid mapping to200 and Intermediate converting normally.
- Preserve every existing non6/CG case. Non6→6 without populated texts remains guarded; no setup-history defaults. Partial/public owner completeness remains false.
- CP141's `explicit_new_fill_does_not_enable_solver_six_apply_or_public_owner` must be migrated coherently: qualified text Apply will intentionally become supported. Preserve a separate missing/out-of-profile text refusal and the independent public-owner assertions, then add the actual new success regression. Do not simply weaken its parameter/count assertions.

No implementation was authorized in this read-only lane. The current applyIteration3D solver6 guard remains before effects. The read-time normalized function-body SHA-256 (UTF-8/LF, from its signature through the following pre-StepSize comment boundary) is `75640bdfd4fa1b760e6be7c5b2ae83d1fac5e19c65c0d985aa61c1db8834dc06`; the CP141 Fill owner is separately freezing its file. Public palette6504abb0, full constructor, native validators/Qt allocation/EH and completeForPublicSave=false remain independent guards.

Only this report was written. No source/test edit, build/test run, GUI/original execution or mutable REA action occurred.
