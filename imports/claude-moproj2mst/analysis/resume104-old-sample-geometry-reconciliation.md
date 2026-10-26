# CP104 Geometry-entry old sample-test reconciliation (corrected)

Reviewed and edited only `tests/doe/test_variable_missing_samples.cpp` and `tests/doe/test_variable_empty_samples.cpp`.

The missing-samples helper omits `numSamples`; in this format, absence is a qualified zero-sample profile. The Geometry row was therefore stale in the missing-samples no-owner guard table. It has been moved to `doe_variable_missing_samples_geometry_refusal_keeps_reached_header_prefix`, which checks the exact Geometry-entry refusal and retained concrete owner, header ID/dimension/zero count, prior Description, index -1, selection 0, and fresh sample invalidation.

The empty-samples helper supplies `numSamples=""`, also the qualified zero-sample profile. Its formerly stale Geometry guard row was already removed and replaced with an equivalent focused reached-prefix case. Genuinely malformed combinations (including explicit `numSamples="0"`, invalid numeric values, absent required name/ID/dimension combinations) remain no-owner admission guards. Keyword-entry prefix checks are unchanged.

Correction note: the prior receipt `resume104-old-sample-geometry-reconciliation.md` incorrectly described the missing-samples helper as an early guard. That claim is superseded by this receipt. No production/shared/CMake/fixture edits, build, test execution, GUI or mutable REA action was performed. This is candidate test reconciliation only, not original parity evidence.

## Freeze hashes

- `source/apps/moproj2mst/tests/doe/test_variable_missing_samples.cpp`: `42425563EB9EDBAA2F427D6BE2D230DE0F463E5C277103610BDDFAE7520C8CBD`
- `source/apps/moproj2mst/tests/doe/test_variable_empty_samples.cpp`: `42ACA144CF3EA70F431E0583DBFC2AEB7B85E296B9A2B66F710BF37F6E5FD0D7`