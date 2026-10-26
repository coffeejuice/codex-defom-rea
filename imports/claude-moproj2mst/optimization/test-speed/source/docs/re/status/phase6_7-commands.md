# Status: Phase 6-7 command specs (thread branch `claude/project-thread-6cvah9`)

Analysis only; porting threads implement from these specs. Index and per-command status:
[docs/re/commands/phase6_7-index.md](../commands/phase6_7-index.md).

| Item | State |
|---|---|
| Specs for OBJPOS, CPYOBJ, SVSCAL, DOESCL, WRTOBJ, GENAXS, GENBCC, GENSUB, TCYCLE, INICTC, GENCTC | written, pushed |
| Test scripts (210 KEY command files + 64 extra + 10 baselines) | all run on the oracle and checked; reports in docs/re/commands/oracle/ |
| Bit-exact follow-ups (OBJPOS kernels, drop solver, 3D GENCTC search and decision) | written, pushed |
| FP-formula audit of first-pass specs | done, 7 corrections (fp-audit.md) |

Working files (not in git): `C:\Users\alext\Projects\deform-rea-work\out\p67\` (decompiles,
listings per worker), Ghidra copies `ghidra_p67`, `ghidra_p67a`..`e`.

Suggested split for porting: (1) OBJPOS geometric modes + coupled/STROKE; (2) OBJPOS interference;
(3) OBJPOS drop; (4) CPYOBJ/SVSCAL/DOESCL/WRTOBJ; (5) GENAXS/GENBCC/GENSUB/TCYCLE; (6) INICTC +
GENCTC 2D; (7) GENCTC 3D (largest single item, depends on the two 3D follow-up specs).
