# CP102 `props_l2` skip accounting fix

Updated `source/apps/moproj2mst/tests/moproj/test_props_l2.cpp` so the five class-registration open failures increment the existing `skipped` case counter before returning. Their existing per-case `SKIP` lines are unchanged; the suite summary will now report those five unqualified inputs instead of `0 cases skipped`. The existing caught-`M2M_E_UNSUPPORTED` path continues using the same counter.

No fixture, assertion, candidate behavior, production code, or CMake registration changed. No build or test was run per task scope; root can validate the grouped suite. The separate `mp.empty()` no-Oracle-B branch remains unchanged because this scoped correction targets the five present fixture open failures.
