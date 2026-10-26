# CP190 root integration

The retained QtCore caller now publishes the qualified private allocation identity into task `+0x08` at RVA `0x131425..0x131429`. This is a deliberately partial state: private `+0x08` remains zero and the reverse backlink at `0x131431` has not run. Guards keep the allocation, owner, return slot and prior caller state bound; independent source review and focused factory/Session tests passed. Legacy tests with explicit CP189 stop budgets remain available.

In a disjoint editor300 lane, the equipment loop in `syncOperationFiles` confines its per-slot `eo` and late step borrow to an immediate callback. The original `object(s+1)` geometry/null-step check still precedes `eo`, nonpositive IDs still skip, and missing-manager fallback retains its cwd-derived path behavior. Corpus-49 TaskKeyRig tests cover positive output, skipped IDs and recording-only fallback without filesystem side effects. Independent source review and focused Save I/O tests passed.

`analysis/wave3-checkpoint190-validation.json` and `build/wave3-checkpoint190-verified/` freeze source SHA-256 `ce42b0849ab0a7121bc9f5001b1fa2fb7adf80d44202f7fe1e5f1b7f8dd83b85` over 3,554 files. Complete regression: 361 registered, 354 clean, six logged-gap suites, one diagnostic-difference suite, zero failed/skipped/incomplete, no fixture changes and no changed logged diagnostics against CP189. Historical folder comparison remains 93 SAME/five DIFF.

CP191 was selected before freeze: caller `MOV R12,RAX` before the private reverse backlink, plus a bounded audit of remaining editor300 raw step aliases. Public Open–Save–Close, failure/Close, cache1 publication and fresh original comparison remain open. No GUI control, original application or mutable REA database was used.
