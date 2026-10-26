# CP167 root integration

The retained prototype-one QObject private object now records a known 64-bit zero at `+0x40` through the bounded QtCore `0x1311b5..0x1311bb` instruction cut, then refuses before `+0x48`. The field is absent before the cut, avoiding an invented initial value. The actual Session root, prior QList/QString identities and symbolic deltas, and private object identity remain unchanged. Later constructor fields, TaskInfo, native release and public DOE completion remain guarded.

Independent source review found that early tests expected the previous CP166 stop on an admitted first call, even though `initializeNativePrefix` falls through to CP167 in that call. The owner corrected all admitted first-call boundaries, reconstructed earlier phases explicitly for legacy fault probes, and added an eight-case pre-write refusal matrix. A final static review found and repaired one assertion that expected a deliberately removed QString binding to remain present. The repaired focused build and complete regression passed.

`analysis/wave3-checkpoint167-validation.json` and `build/wave3-checkpoint167-verified/` freeze source SHA-256 `bb394a50a055f366a50c99f9f319f5de18fd962e5ffa6633ce9fc7e886b14895` over 3,554 files: 361 registered, 354 clean passes, six logged-gap suites, one diagnostic-difference suite, zero failed/skipped/incomplete/fixture changes, and no changed logged diagnostics. Historical 93 SAME/five DIFF remains qualified. No GUI control, live REA or original application session was acquired.

CP168 was selected before freeze. Independent evidence and plan review accepted the next same-private-object `+0x48` known-zero instruction cut; an exclusive source owner began after the archive. Public lifecycle and fresh original comparison gates remain open.
