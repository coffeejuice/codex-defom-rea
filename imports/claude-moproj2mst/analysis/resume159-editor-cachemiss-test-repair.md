# CP159 test fixture repair receipt

Repaired only `source/apps/moproj2mst/tests/mdt/test_simcontrol300_stopping_cached.cpp` in response to independent source review. Restored the CP158 matrix so only profile 1 uses cache zero; profile 0 remains a cache hit. Restored the corresponding support expectation and confines the two-entry timer assertion to the valid profile-1 miss.

The repeated-miss case now explicitly sets cache id 0 and creates a fresh `StoppingPage` for each invocation, so each iteration delivers a new callback instead of relying on a completed spin control to emit one again.

Preflight rechecked: Testkit `TEST_CASE` and `testkit::run_all()` entry point remain present; no `REQUIRE_EQ` or C `assert` is used. No build, test, GUI, production source, shared interface, or mutable REA operation was performed.

Repaired test SHA-256: `568A4E64BCF3C910B69F58F550A3D804074967AD69B9691EA69B33184C34968E`

Test-file ownership released to root.
