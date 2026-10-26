# Resume 53: Candidate public two-reheat guard regression

## Change

Added `source/apps/moproj2mst/tests/s/test_cogging_two_reheat_guard.cpp`, a new public conversion test discovered by the existing `tests/s/test_*.cpp` glob. It copies the retained `42_vdie` project under the configured test scratch root and keeps its active index and project files. The test changes only the disposable `Task00004/Cogging.MDT`: it duplicates the existing complete REHEAT record, updates `NoReheats` to 2, and duplicates the Simulation Data rows with the second reheat labels.

Before conversion, it feeds the modified bytes through `readMdtFile` and requires both `parsed.cog.nReheats == 2` and `parsed.cog.reheats.size() == 2`. This proves the fixture is a parsed two-element model rather than only a changed input token.

It then calls the public `m2m::convert` entry point with `M2M_FLAG_WRITE_MDT` and a separate candidate MST destination. The assertions pin the current candidate contract: `M2M_E_UNSUPPORTED`, an error message containing `NoReheats 2`, byte-identical full copied-project inventory after conversion, unchanged two-reheat Cogging input, and no output MST. This covers deferred staged MDT/MST application on the candidate route.

## Evidence limits and validation

The retained original loop analysis establishes a one-past read for two reheats but not a stable fault, status, output file, or stage continuation. This test makes no original behavior claim and does not compare its refusal to an original oracle. Retained corpus/oracle cases have one parsed reheat, so there is no native multi-reheat output fixture to compare.

No production source or CMake file was changed. The test was not built or run in this bounded item; root validation remains pending. The only files changed for this item are this new test and this report.
