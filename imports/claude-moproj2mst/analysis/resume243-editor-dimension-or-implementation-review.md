# CP243 editor dimensionOr implementation review

**PASS — the scalar extraction exactly preserves the selected conditional and its adapter evaluation point. No unresolved defect found.** This is an independent static source/test review; root owns integration, the full suite and freeze.

Reviewed the CP242 proposal and PASS review, CP243 implementation note, the four affected file bodies/diff, existing model dimension tests and lifecycle Environment/view-factor cases. Other accumulated model API changes in the shared files were distinguished from this leaf.

The new declaration/definition is `int dimensionOr(int whenNoStep) const noexcept`, adjacent to the existing getter. Its complete body is `return step ? step->dimension : whenNoStep;`. It does not invoke a geometry guard, setter, initializer or another accessor, expose an alias, clamp values, or interpret zero as absent. Every live dimension integer is returned unchanged; only missing storage selects the exact supplied fallback. The existing dimension() behavior is unchanged.

The caller changes only the first EnvironmentViewFactorState aggregate argument to model.dimensionOr(3). It remains after successful/non-unsupported Stopping, Meshing and Iteration page processing and during argument construction before Environment Apply enters. Curve/definition arguments, page record handling, Advanced/Extra3D order and all reached prefix effects remain unchanged. No new admission/exception point is inserted after page effects, and no Stopping/cache1 behavior changes.

New direct tests compare live 2/3 with two distinct fallback values, distinguish live zero from fallback 3, and verify absent storage returns 3 and -17 while ordinary dimension() still returns zero. These are meaningful discriminators against a fixed-zero getter, hardcoded fallback or truthiness substitution. The zero is assigned directly rather than invoking unrelated setter behavior. The fixture has no object/viewer state requiring a restored live-zero field before unrelated work. Existing Testkit entry point and supported assertions remain; no C assert, new include or target is introduced.

The unchanged lifecycle checked view-factor case verifies definition/curve copy and resulting native fields; the unchecked case verifies the existing preserve-live-value behavior. Those ordinary regressions do not prove absent/zero states are reachable through full Apply. No new parked dimensionOr case was added: source inspection establishes absence of a guard here, while existing parked dimension()/setter cases are not dynamic coverage of this new method. The implementation report does not claim otherwise.

The owner reports a source-stable focused build (including CLI) and 2/2 model-dimension/lifecycle CTest targets passing. This reviewer ran no build or tests and does not independently certify those receipts. Full suite, stopping-cached/viewer integration and freeze remain root gates.

The fallback 3 is adapter EnvironmentViewFactorState assembly behavior, not an original getter fallback. The existing adapter evaluates the scalar before entering Environment Apply; retained comments describe a later live dimension query in the original native view-factor branch. This leaf preserves current adapter timing and does not establish original live-query timing, a fresh original observation, complete lifecycle support, or ownership closure. The implementation report states those limits accurately. No ABI change belongs to this leaf; simcontrol300.cpp, lifecycle tests and stopping_cached test have no working diff.

Reviewed full-file SHA-256 values match the implementation note:

- engine.hpp: `B2787DCA14FC84A8AFA247AF453B803135F1A465FEAF62C3132CF050C848A7EE`
- gui_model.cpp: `69D5262ACD3BCB16F19930B404783378CEB9B8ACA2A769F4FB90CDF15B3B2FAE`
- simcontrol300_lifecycle.cpp: `81F03757856B0E0D1808F4F29F9D5638AB728452A4D088BEE21CABC63E6ACDB5`
- test_gui_model_dimension.cpp: `BD5827BA97E03E6B1C40EB0BFEAB9A26C1CE746DBFE0B8A404F5DC152C699A0A`

Only this report was written. No source/test/build/shared recovery state, original process, GUI or REA state was changed by this reviewer.
