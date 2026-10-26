# CP173 ABI Unicode truncation regression

Added `unicode_diagnostic_truncation_preserves_scalar_boundary` to
`source/apps/moproj2mst/tests/g0/test_abi.cpp`. It constructs a missing
filename containing U+1F600 from explicit UTF-16 code units, converts that
same path to UTF-8 with the Windows UTF-8 API, and calls both public ABI entry
points. The test checks `M2M_E_OPEN`, locates the scalar in each complete
diagnostic, verifies that constrained capacities which would otherwise retain
only the high surrogate or UTF-8 lead byte instead terminate before the
scalar, and strictly decodes the short UTF-8 result. It also checks equality
of the complete diagnostic texts and truncated prefixes after UTF-8 decoding.

The missing leaf is placed below `M2M_TEST_SCRATCH` using the existing
`m2m_test::scratchRoot()` helper. The test does not create that leaf. It uses
numeric UTF-16 surrogate values and `WideCharToMultiByte` / `MultiByteToWideChar`
with `CP_UTF8`, so it does not rely on source-file encoding, an active locale,
or the system ANSI code page. Its source-determined assumptions are the public
header's NUL/truncation contract and the adapter's UTF-8 message conversion.
The absent-path precondition is a `REQUIRE`, so later assertions only run after
the scratch leaf has been confirmed absent.

Source SHA-256 after this edit:

`DAF5D17361747D14626AA94889A834C1164ADA6FB382B514DC857CC25FD4254C`

No build or test was run for this isolated source change. Integration owner
should run the focused `abi` target after CP172 source repair and record its
result. I own only this test source for the implementation step; integration,
build/test execution, and any source archive/freeze remain with the root owner.
