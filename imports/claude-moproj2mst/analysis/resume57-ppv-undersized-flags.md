# Checkpoint 57: PPV undersized-flags regression

Added `simctrl_ppv_binding_rejects_undersized_flags_before_mutating_owners` to `source/apps/moproj2mst/tests/mdt/test_simctrl_ppv_binding.cpp`. It seeds a nonempty local owner, two shared/root PPV points, and a non-null but empty flag vector. Its valid PPV row matches shared point index 1. The candidate adapter is expected to refuse this inconsistent owner shape before calling the frozen reader or clearing/committing any owner. Release-active Testkit `REQUIRE`/`CHECK_EQ` assertions verify the refusal and preservation of every field in the local and root entries plus the flag-vector size.

The test targets the existing guard at `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp:50`. The reader's source contract in `include/m2m/engine/simctrl_ppv_reader.hpp` requires any supplied flag vector to cover insertion indices; without the adapter guard, the matching index 1 would be outside this empty vector. This test covers a deliberately malformed candidate model state only. It does not assert how the original handles malformed native lists and does not claim original parity.

The test was not built or run, as requested. Frozen edited files:

- `source/apps/moproj2mst/tests/mdt/test_simctrl_ppv_binding.cpp` — SHA-256 `53d3d6ca57d9adad76763470772d36c3cf020dd7b9bc64fcbd376f97bb7213d1`
- `analysis/resume57-ppv-undersized-flags.md` — this report

No production source, CMake file, fixture, GUI state, or REA data was changed.

