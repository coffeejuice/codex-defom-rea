# CP144 post-source review — FRCROT pair-prefix helper

## Result

The new private helper matches the CP144 plan and the review amendment on the central ordering and retained-state requirements. `applyFrcrotPairPrefix` starts from a value copy of the ordered existing pair identities, ensures the exact ordered forward `(master, slave)` tuple before checking mode-line success, and returns immediately on mode-line failure. That early result retains the forward pair found/appended outcome, reports `ModeLineFailure`, and has no table-reset/mode writes, reader event, reverse-pair event, or mirror-copy event.

On mode-line success, it records reset first, then the mode-specific write projection; it records the table-reader call and proceeds through self/non-self common-tail behavior regardless of the prepared reader result. A non-self reverse tuple is looked up/appended in ordered `(slave, master)` form, then direction-set and mirror-copy events are recorded. The final table-reader status is assigned only after that tail. The late-failure mode-0 test verifies the dimension/constant projection remains present with `TableReadFailure`, and the reverse append/mirror observations remain reached. This satisfies the required distinction between early mode failure and later table-reader failure.

The helper is pure prepared-value code. It does not parse FRCROT text, mutate `DB_STEP_DATA`, read/write files, synthesize row or mirrored payload bytes, or connect into public conversion. `doe_study.cpp::keywordValues` still refuses any non-default handler3 before it writes `temp.KWD`, and there are no production callsites for the new helper outside its own source. The public keyword/FRCROT route therefore remains guarded.

## Small test-strengthening note

No source blocker found. The early-failure cases check `resetReached == false`; the new result type also has four optional write fields, which default empty. It would be slightly stronger to assert each optional is empty in the early-failure test, directly freezing the review requirement that no table field mutation is reported. The source currently satisfies it by construction. The late failure currently exercises mode 0; modes 1 and 2 are covered on successful-reader paths, but a failure case for those modes is optional because they share the same ordered path after their mode-specific writes.

The tests intentionally assert only logical observations. They do not establish native pair allocation/constructor defaults, underlying list iterator/failure behavior, mode value parsing/casts, table-reader row partial effects, mirror-copy payload validity, or handler3 selection and copied-value lifetime.

## Files reviewed

- `source/apps/moproj2mst/src/doe/frcrot_pair_prefix.hpp`
- `source/apps/moproj2mst/src/doe/frcrot_pair_prefix.cpp`
- `source/apps/moproj2mst/tests/doe/test_frcrot_pair_prefix.cpp`
- `analysis/resume144-frcrot-pair-prefix-plan.md`
- `analysis/resume144-frcrot-pair-prefix-review.md`
- `analysis/resume143-frcrot-parser-order.md`
- `analysis/resume142-frcrot-input-provenance.md`

Source/evidence basis remains original GUI PE SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. Static source review only; no build, test execution, GUI, original execution, or mutable REA action occurred.
