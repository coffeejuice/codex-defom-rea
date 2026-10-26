# CP173 root integration

The retained Report constructor now commits one symbolic `uint32_t` atomic increment to the same Session root's `QVectorData::shared_null` record through QtCore `0x1311d2..0x1311da`, then stops before private `+0x70`. The weak private binding remains pointer-identical to that root; no absolute reference count is inferred. Focused tests cover once-only effects, wrap, actual Session pairing, reentry and 35 refusal cases. Independent review accepted the source/test contract.

In a disjoint editor300 lane, the first raw-access closure cut replaced internal `opStep` raw returns with synchronous callback uses in four engine files. It preserves CP163 cache0/A/refusal behavior and does not publish cache1 or complete the Stopping pending-call barrier. Independent review found no pointer escape or changed call order in those migrated callbacks. Remaining public `GuiModel::step` and other raw aliases stay queued.

`analysis/wave3-checkpoint173-validation.json` and `build/wave3-checkpoint173-verified/` freeze source SHA-256 `ab67e3e532defdadbb637596e243c8bee9e45dadba2ac25376333c1943f6dc26` over 3,554 files: 361 registered, 354 clean passes, six logged-gap suites, one diagnostic-difference suite, zero failed/skipped/incomplete/fixture changes, and no changed logged diagnostics. Historical 93 SAME/five DIFF remains qualified. Focused factory, Session and Stopping tests passed.

CP174 was selected before freeze: bind the existing root-owned QList shared-null to private `+0x70` before its second atomic increment, while planning the next disjoint editor300 access closure cut. Public lifecycle and fresh original comparison gates remain open. No GUI control, original application or live REA session was acquired.
