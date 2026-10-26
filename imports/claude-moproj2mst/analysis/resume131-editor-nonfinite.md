# CP131 implementation: masked nonfinite meshing node target

The private meshing target load now admits nonfinite node-count bits only when the actual current thread MXCSR masks invalid-operation exceptions. It reuses the existing CVTTSD2SI intrinsic and spin clamp, displaying0 while preserving raw temporary bits until a later old-form commit. Unmasked nonfinite targets remain unsupported after the previous temporary commit. Other target/old-control guards and the public Editor300 palette boundary are unchanged.

## Source contract and exact change

Controlling report `analysis/resume131-editor-next.md` SHA-256 is `06e582cd680f3ce3de4e689a079a86b201f42528afb9a39e70d3de4b1419c6e2`. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The report pins native requested-node CVTTSD2SI at140ecf27f, QSpinBox setter140ecf29e, later previous spin-to-double store140ecef7f, the existing nonwrapping0..1e9 range, and Intel's masked/unmasked instruction semantics. No selected original MXCSR value is inferred.

The production diff is +8/-1 against frozen CP130: add bit/cstdint includes, remove only target.nodeCount from the floating finite predicate, classify its exponent through uint64 bits, and guard a nonfinite node on live `_mm_getcsr() & _MM_MASK_INVALID`. This avoids an FP predicate on signaling-NaN node data. Existing conversion and clamp code is unchanged. Production reads MXCSR; it contains no `_mm_setcsr`, mask installation, default-FP seeding or flag-clearing path.

Positive/negative infinity and NaN input produce integer-indefinite INT_MIN under masked invalid, then spin0. The natural instruction status effect is not suppressed. The source node double is not rewritten during display. With IM clear, admission stops before target radio/text/spin or stored-index mutation, preserving PreviousCommitted or the InitialLoad preflight guard. Native unmasked exception delivery/full post-Qt FP status remains unrecovered; UnsupportedSnapshot does not claim an original successful exception return.

All old numeric guards, the three other target finite checks, index/stage checks, negative-index no-op, same-row reload, legacy const separation, missing-object continuation and disabled remesh ordering remain intact. No finite-overflow redesign or broader nonfinite absolute-value/QString profile was added.

## Focused cases

The same target `m2m_mdt_simcontrol300_meshing_selection` / CTest `m2m.mdt.simcontrol300_meshing_selection` now has12 Release-active cases (8 retained,4 added):

- Initial Fill with finite other fields and node bits for positive/negative infinity, distinct signed quiet NaNs and a signaling NaN. With IM masked, loaded spin is0, raw model/temporary bits remain, and later Save commits positive zero.
- Later target activation for those same bit patterns preserves raw node bits through target load and changes them only on an old-form commit; model stays raw until copyback.
- A programmatic move followed by mutable Save copies the newly loaded target's raw nonfinite node to the model on that first Save. Only the following callback/Save commits spin0. This guards against early source mutation and a second conversion of the fresh target.
- With IM unmasked and finite old controls, target signaling-NaN node is refused at PreviousCommitted without target/model/global changes. Initial Fill is likewise refused without installing a default mask.

Test-only MxcsrFixture saves the complete entry word, controls IM explicitly, masks other exceptions for a known test profile, clears pending status before the unmasked guard case, and restores the entry control/status word through RAII. The unmasked calls restore before framework assertions/formatting. Tests never intentionally execute an unmasked CVTT conversion. Production does not perform these test-only environment changes. Conditional private component results do not establish the actual selected application's FP state, native widgets or full Session Open->Save.

## Static preflight and frozen files

Compared the complete production diff against build/wave3-checkpoint130-verified/source.zip, checked include targets and MSVC intrinsic header chain (emmintrin includes xmmintrin; `_MM_MASK_INVALID` is0x0080), valid unique12 TEST_CASE identifiers, supported Testkit macros, one main, no C assert/REQUIRE_EQ or raw NUL source, balanced braces and whitespace. Verified production has no `_mm_setcsr` and only node-bit classification/mask admission changed. git diff --check produced no errors.

No compiler/build/CTest, GUI, original execution or mutable REA operation occurred. No test was run and no reconstructed executable/deployment was inspected or produced. Static preflight is not compilation or runtime acceptance. Root owns grouped focused/full validation and archive binding.

| Exclusive file | SHA-256 | CP130 text diff |
| --- | --- | --- |
|source/apps/moproj2mst/src/engine/simcontrol300.cpp|`5c61460e70552a8e58ec7c5e1a3ce0aed98a45308b5948f0d1efc60626ec4a3a`|+8/-1|
|source/apps/moproj2mst/tests/mdt/test_simcontrol300_meshing_selection.cpp|`5ae8085a2ab2bc9e9a96833aec4239132eb72f83eedaf4fe9689b60d53146291`|+112/-0|

Only this pair and this new receipt were written. No shared header/lifecycle, Qt primitive, CMake, IO parser or public loader/factory was edited. Source/test are frozen and released for independent review/root build; no further source edits until root releases the window.
