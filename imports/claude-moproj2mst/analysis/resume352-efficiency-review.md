# CP350–352 efficiency review

The first monitored native attempt returned in about 0.22 seconds and failed before entering `CreateProcessW`; its exact report exposed one missing security preparation step. A 594-test injected suite then took 0.336 and 0.337 seconds in the two module orders after the repair. The earlier 594-test run with stale fakes failed, and its raw log is retained. These timings describe narrow checks, not converter recovery speed; token use and end-to-end original comparisons remain unmeasured.

The native receipt had more diagnostic value than another fake-only layer: it located the earliest actual owner precondition with no worker started and closed cleanup. CP353 found no further deterministic pre-resume mismatch; one added test composes real CP326/CP328 owners up to an injected `CreateProcessW` failure. Freeze the repaired source, then use one new monitored occurrence to test the next OS boundary. Do not retry a cleanup-uncertain result automatically or infer public converter parity from an inert fixture.

Review again after three further frozen checkpoints or one hour at a safe boundary. The public C++ candidate remains CP301.
