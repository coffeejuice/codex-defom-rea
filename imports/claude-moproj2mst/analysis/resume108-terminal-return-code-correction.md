# CP108 RemeshV2 returned-code correction

The shared classifier incorrectly applied v1's process-exit/payload equality rule to RemeshV2. Root's focused actual worker receipt exposed the mismatch: Unknown/Custom produced a valid Returned adapter code7 with successful worker OS exit0, no protocol error and no supervisor failure, yet were classified HarnessFailure.

The frozen correction separates these meanings. RemeshV2 Returned requires OS exit0 and adapter returnedCode0..7. A nonzero OS exit remains failure even if it equals the adapter payload; terminal status without the terminal boundary remains failure. SafetyV1 retains its original status==returnedCode rule. Sticky timeout/protocol/backend/inspection failure and terminal-boundary logic are unchanged. No worker, supervisor, C ABI or core conversion source was edited.

The existing v2 returned-snapshot case now checks adapter7/OS0 success with either present/absent snapshot, OS7/terminal-status rejection, all adapter codes0..7 with OS0, invalid payload8 rejection, and an explicit v1 code7/OS7 control plus v1 code7/OS0 rejection. The v2 target remains8 cases. Read-only source preflight passed for the changed test; includes, Testkit main/macros and whitespace were checked. No build, executable/test run, GUI, original or REA action occurred. Root owns the rerun/full freeze; no dynamic success is claimed here.

Frozen SHA-256:

- `source/apps/moproj2mst/src/engine/terminal_harness_protocol.cpp`: `1d35912181a38ed2f08be1fb4f9736bc84551d42799a791fd9fd3ef20c481db2`
- `source/apps/moproj2mst/tests/mdt/test_terminal_harness_remesh_v2.cpp`: `dec55e6daa67c43461b53a776fb881977c1722c20713cfe97d2a22d29851a5c6`

Both files are released and held for root integration. This report supersedes earlier design wording that conflated v2 worker transport status with the separately published adapter result; it does not alter the legacy product CLI/C ABI exit policy.
