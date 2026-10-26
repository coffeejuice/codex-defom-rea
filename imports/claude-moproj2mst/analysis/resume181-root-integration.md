# CP181 root integration

The retained Report constructor now records a known 64-bit zero at private `+0x28` through QtCore `0x131220..0x131224`, then stops before the flags read. The internal sparse stop budget preserves CP180 ordinals 1–5; the default is ordinal 6. Earlier five zero stores, owner/version/root qualifications and symbolic reference deltas remain unchanged. Independent source review accepted the new store and focused refusal/Session tests. The first focused run exposed two stale reentry diagnostic assertions; after repair, the source-stable focused and full suites passed.

In a disjoint Save lane, Express Save's SimControl pointer is confined to the existing object loop. The heat-mode check still precedes the one model lookup, and `GuiModel::simCtl()` still enforces its geometry refusal before objectCount. A focused positive window-update and synthetic null-step fixture passed. No C ABI change or Stopping cache1 publication was introduced.

`analysis/wave3-checkpoint181-validation.json` and `build/wave3-checkpoint181-verified/` freeze source SHA-256 `ddfbb8a08f0a9299ed05aaae442c6b9bbd1f2872a40c9cd3adcaae2b14a4eaf8` over 3,554 files: 361 registered, 354 clean passes, six logged-gap suites, one diagnostic-difference suite, zero failed/skipped/incomplete/fixture changes, and no changed logged diagnostics. Historical 93 SAME/five DIFF remains qualified. Focused factory, Session, L3, Stopping, Save and ABI tests passed.

CP182 was selected before freeze: a symbolic read/AND/OR/write of private `+0x20` at `0x131224..0x131232` with four intermediate cursors, plus a disjoint FormingExpress default-pairs step-borrow closure. Public lifecycle and fresh original comparison gates remain open. No GUI control, original application or live REA session was acquired.
