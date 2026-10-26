# CP87 correction: activation refusal fixture

Root's full-suite run failed `m2m.mdt.doe_activation` at the first refusal assertions in `doe_lazy_unsupported_branch_preserves_prior_header_and_sample_effects`. The fixture used `<DoeVariables><Var/></DoeVariables>`. Under the qualified original contract, `Var` is an unknown tag and is skipped, so lazy read correctly continues into the three sample writes and current-PAR read. Production dispatch is correct; the fixture was stale.

Original `0x14090eef6` compares the direct child tag with PE literal `0x1427bb508`, verified as exactly `DoeVariable`. Unknown children never enter the variable payload body. This source evidence and its original-module identity are frozen in `resume87-doe-next.md` and `resume87-doe-variable-dispatch-implementation.md`.

Only `source/apps/moproj2mst/tests/mdt/test_doe_activation.cpp` changed. Its refusal fixture now uses `<DoeVariable/>`, retaining the intended test: the Study name and earlier roiType8 are committed, the recognized variable payload guard occurs, and no sample-file operation follows. The catch now checks the precise DoeVariables payload message; the I/O assertion checks the exact sole `read:DoeInputTable.XML` prefix and an empty text-write list. Explicit standard string/vector headers were added. The second half, which retains mode9 after three sample writes before the unsafe-index guard, is unchanged.

The unknown-child continuation is already covered by the new CP87 dispatch target, including actual-owner lazy read. No production rollback or extra refusal was added to satisfy the old fixture.

| File | Frozen SHA-256 |
| --- | --- |
| Corrected `source/apps/moproj2mst/tests/mdt/test_doe_activation.cpp` | `4483eeef174deaec3a2f4c3ff6d050606eac42cc912be00949162a9fd73ffc49` |
| Unchanged `source/apps/moproj2mst/src/doe/doe_study.cpp` | `77a08465c67e581207bc6efb555f0771c7f161703d2de6bcf02470b2a01293f2` |

Quoted includes resolve; active Testkit main/macros and trailing-whitespace checks pass. No worker build, test execution, GUI or mutable REA action occurred. Root owns the `m2m_mdt_doe_activation` rebuild and focused/full validation. The corrected test is frozen and held through that validation/archive.
