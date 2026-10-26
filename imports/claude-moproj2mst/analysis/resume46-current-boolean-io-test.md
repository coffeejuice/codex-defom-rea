# Resume 46 current-operation Boolean I/O regression

## Settled result

The test `save_current_boolean_bwz_open_failure_continues_ini_and_mst` in `source/apps/moproj2mst/tests/s/test_save_io.cpp` uses corpus fixture `46_MO2_Boolean_Oprations`, explicitly seeds the disposable `Task00004/Boolean.BWZ` from the fixture, then denies its write open. The fixture's `.moproj` has `<Sub_Project Active="3">`; diagram item 3 is `SfOprBoolean2D`, operation id 4. `parseSubProject()` maps that index to `flow.activeItem`; `saveCurrent()` resolves the active flow item and calls `saveCurrentOperation()`. The open path marks the class's `current` pointer before Save. The initial fixture 49 assumption was incorrect and is superseded.

Root's integration result in `analysis/resume46-saveio-b-build.json` records a successful focused build and `m2m.s.save_io` passing 66/66. The test passes against the existing production path and confirms that this current-operation denied-BWZ scenario retains the prior BWZ while continuing through the expected Boolean INI output and completed MST. Therefore the earlier suggested current-operation BWZ open-failure gap is closed for this case; no production change is indicated by this test.

The original failure contract remains source-qualified by `analysis/wave3-boolean-save-io-contract.json`; no original fault was injected. The test establishes reconstructed behavior for this case, not independent original acceptance or successful-open partial-write behavior. The adjacent property-row test remains separate coverage.
