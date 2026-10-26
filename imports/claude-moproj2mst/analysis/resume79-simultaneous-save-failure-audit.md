# CP79: remaining combined Save failure case

## CP77 result and original status qualification

CP77's paired candidate test distinguishes the post-close project result in `SaveCallState`, but when the MST open also fails both runs map to the same adapter error: `M2M_E_WRITE` with `cannot open MST <that run's scratch path>`. The project error 7 is a declared post-close observation; the MST denial is a real sharing lock on disposable output. The paths differ between Rigs, so only each message's prefix and own expected path should be compared.

This adapter mapping is separate from the original top Save result. `analysis/save-semantics.md` and original `FUN_1400980d0` evidence show the project/MST booleans are combined, but the function returns its earlier check/status local. The output-order contract is source-qualified; no claim is made that `M2M_E_WRITE` is the GUI's returned status.

## One remaining three-failure candidate combination

A source-ordered but not yet integrated combination is:

1. Project writer's close/error query reports false (existing private service can declare error 7).
2. MST target is seeded and locked against writes from the project's `aboutToClose` callback, so its later open fails.
3. Remesh is enabled from `LOC_DEN_CTRL.DAT`; callback creates a prior `LOCAL_REMESH.DAT` sentinel and locks it against writes, so remesh output open fails after the MST attempt.

The current adapter should preserve the earlier Boolean step4 outputs, leave both sentinels unchanged, record project false and MST attempted/returned/false, reach the remesh tail, and report its guarded `M2M_E_UNSUPPORTED` before the MST/project `M2M_E_WRITE` mappings. The existing CP77 callback can arm both disposable locks after input/project Open; no production seam is needed. This combines CP74's project-plus-remesh failure with CP77's project-plus-MST failure and checks precedence when all three are present.

This is a candidate integration check, not an original returned-status test. Original Save's control flow reaches the enabled remesh writer even after project/MST false, but the original writer does not check `fopen` before `fprintf(NULL)`. The qualified default invalid-parameter path reaches `TerminateProcess(0xC0000417)`; with that profile, earlier Boolean/MST effects survive and there is no normal Save status or Close. Therefore an adapter result of `M2M_E_UNSUPPORTED` is an explicit safe guard, not native status parity. The combined test would validate candidate partial effects and guard precedence only.

## Priority

The combined test is straightforward and source-order deterministic, but adds limited new original-contract evidence: CP74 and CP77 already cover both pairwise paths, while static source fixes the order. Add it only if root wants explicit three-way adapter precedence coverage. No other source-closed simultaneous failure was found that changes original normal-return status: the native return remains the earlier check/status local; disabled remesh `QFile::remove` denial is ignored and can only alter residue. Post-open remesh writer errors remain ignored by the source and have no separate status branch.

This was a read-only audit. No source, tests, build, GUI, or mutable REA state changed.
