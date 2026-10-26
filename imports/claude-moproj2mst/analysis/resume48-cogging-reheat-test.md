# Resume 48: focused Cogging reheat boundary test

Added `TEST_CASE(cogging_reheat_count_boundary)` to `source/apps/moproj2mst/tests/mdt/test_cogging_save.cpp` in the imported converter tree. The test uses the existing `datafiles::writeMdtBytes` seam with default-valued `CogModel` data and default `Project::CogGlobals`; it resizes each fixture's `reheats` vector to match its declared count so the multi-reheat assertion reaches the intended guard.

For one reheat it asserts the returned output orders `SIMLST`, `NoReheats`, `Reheat 1`, and `Process Data`, and contains no `Reheat 2`. For two reheats it asserts the candidate throws `M2M_E_UNSUPPORTED` and identifies `NoReheats 2`. These assertions cover the current deterministic one-reheat writer and current explicit refusal only. They make no claim about bytes, partial output, crash, or data read by the original's out-of-bounds loop.

No build or test was run; the root owns validation. The edited file currently appears untracked from the parent repository's view (`?? imports/claude-moproj2mst/source/apps/moproj2mst/tests/mdt/test_cogging_save.cpp`), so integration must preserve the imported subtree's existing ownership/commit convention.

SHA-256 of the edited test file: `2a8ab9979f7189bde22749916dc9c0d4692ec11ec0e940afd5ea03a410ebfa31`.
