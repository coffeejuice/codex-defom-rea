# CP189 root integration

The retained QtCore caller's unconditional `EB 05` jump at RVA `0x13141e..0x131420` now advances the symbolic instruction cursor to `0x131425` without changing registers, flags, stack or backlinks. The state stops before publication into task `+0x08`. Independent source review and focused constructor/Session tests passed.

The initial editor300 proposal to edit `saveMaterials` was rejected by independent review because that scoped repeated-access implementation already existed in the CP188 freeze. The corrected cut belongs to `syncOperationFiles`: the restart whole-model writer now confines its cached model/step borrow to an immediate callback after path derivation, without adding an operation-model lookup or changing null policy. The existing corpus-49 TaskKeyRig test checks model/step identity, per-call NSTART sentinels and native/denied-open ordering. Independent source review and focused Save I/O tests passed. Equipment remains a separate raw-borrow cut.

`analysis/wave3-checkpoint189-validation.json` and `build/wave3-checkpoint189-verified/` freeze source SHA-256 `d7ed7b4b062621658be559c25f4c4f66edfaab9829030bfcf0dbe71beedf06b8` over 3,554 files. Complete regression: 361 registered, 354 clean, six logged-gap suites, one diagnostic-difference suite, zero failed/skipped/incomplete, no fixture changes and no changed logged diagnostics against CP188. Historical folder comparison remains 93 SAME/five DIFF.

CP190 was selected before freeze: the task `+0x08` private-pointer publication at RVA `0x131425..0x131429` and the disjoint equipment borrow investigation. Public Open–Save–Close, failure/Close, cache1 publication and fresh original comparison remain open. No GUI control, original application or mutable REA database was used.
