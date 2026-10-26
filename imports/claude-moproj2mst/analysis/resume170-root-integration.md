# CP170 root integration

The retained prototype-one QObject private object now records a known 64-bit zero at `+0x58` through the bounded QtCore `0x1311c3..0x1311c7` instruction cut. It refuses before QVector shared-null setup. The allocation-time private identity, Session root, QList/QString bindings, earlier zero fields, symbolic reference deltas and refusal ordering remain intact.

The independent source review accepted the final tests after a duplicate-local compile repair. A foreign factory root cannot be injected at this retained phase through the public API: a new factory allocates a new Report and is refused at the first QList owner guard. Existing earlier owner cases, the same-frame expired-root case and the `+0x58` foreign-binding cases cover reachable failures. Focused factory and Session tests passed.

`analysis/wave3-checkpoint170-validation.json` and `build/wave3-checkpoint170-verified/` freeze source SHA-256 `bb76dd6d7638f1235ce48d753c4578da1c0ecb73cacde376d4792ccbc4ae46e7` over 3,554 files: 361 registered, 354 clean passes, six logged-gap suites, one diagnostic-difference suite, zero failed/skipped/incomplete/fixture changes, and no changed logged diagnostics. Historical 93 SAME/five DIFF remains qualified; this is not a fresh original comparison. No GUI control, live REA or original application session was acquired.

CP171 was selected before freeze: a branch-local shared constructor admission validator with phase-local guards, diagnostics, writes and cursor preserved. The next native instruction after that refactor is QVector shared-null binding at `0x1311c7..0x1311d2`. Public lifecycle and fresh original comparison gates remain open.
