# CP133 isolated-worker EOF/exit race repair

The early-exit failure is caused by a race between protocol EOF classification and natural process termination. The v2 collector correctly marks `v2 EOF without final outcome` as invalid, but supervisor cleanup could observe the process as still live immediately afterward and terminate the job with `0xe1050001` before exit 66 became observable.

## Change

`src/engine/terminal_harness_supervisor.cpp` now gives only a v2 EOF-without-final-outcome protocol failure up to 1,000 ms to exit naturally before requesting termination. The wait is capped at the original child deadline, polls in at most 10 ms intervals, and rechecks stdout/stderr capture limits during the grace. Capture-size exceptions are caught locally, appended as a secondary diagnostic, and lead directly to the existing bounded owned-child termination/cleanup path. The original sticky protocol stop reason and invalid transcript remain unchanged; this does not convert missing final protocol output into success or allow a capture exception to escape `supervise()`.

`tests/g0/test_cli_isolated_worker.cpp` strengthens the existing stale-job-nonce case to assert the precise EOF protocol diagnostic, transcript invalidity, and absence of supervisor termination while still requiring child exit 66 and harness-failure classification. This is the existing deterministic child path that previously exposed the race.

## Static validation and binding

- Test source static scan: 4 quoted includes resolved, 7 unique Testkit case identifiers, `wmain` and `testkit::run_all()` present.
- A read-only trailing-whitespace scan passed for the two owned source files.
- The repository's generic `check_test_source_preflight.py` was also invoked on this test but reports `missing_int_main`: this file intentionally uses the Windows `wmain` entrypoint, so that check is inapplicable. No source was changed to satisfy the generic check.
- No compiler, build, CTest, full launcher, GUI, or application was run. The prior CP132 raw failed and retry logs remain untouched.

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/terminal_harness_supervisor.cpp` | `1BA9ED66823F62C6FE509F490070BEC150D2C47DE2E7EDCE2110218282884424` |
| `source/apps/moproj2mst/tests/g0/test_cli_isolated_worker.cpp` | `506ED3841129E3FB6BC558850B2BE3600FF17C4B22C85D6B8AF5ABF72D285583` |
| `build/logs/resume132-final-full.stdout.json` (preserved failure) | `B30C30CB775DDF05486DAE45DDAF79F840DEFD435718CA6A0194B55B32E244AC` |
| `build/logs/resume132-final-full-rerun.stdout.json` (preserved retry) | `0C2182D6576037AA84489C0FA4B636756B72D28E7BE8CC7FCF2AFCE8957FD469` |

CTest test name remains `m2m.g0.cli_isolated_worker`; its existing target is `m2m_g0_cli_isolated_worker`. Runtime behavior after this change remains unverified pending root's build and test.



## CP133 exception-safety follow-up

The grace-loop capture inspection is now wrapped in a local std::exception catch. On inspection failure it records a secondary note, breaks the grace loop, and continues into the existing owned-child termination and bounded stop confirmation. The sticky EOF protocol failure is retained. Updated supervisor SHA-256: 1BA9ED66823F62C6FE509F490070BEC150D2C47DE2E7EDCE2110218282884424. Static include/case/whitespace checks were rerun; build and runtime remain root-owned.

