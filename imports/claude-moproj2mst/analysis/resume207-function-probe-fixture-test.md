# CP207 harmless function-hook fixture test

`admin/test_function_owner_probe_fixture.mjs` extracts the actual `injectedSource` function from `admin/original_function_owner_probe.mjs` and evaluates its generated JavaScript in a restricted Node VM with mock Frida APIs, pointers, module addresses and memory. It does not import Frida, start a process, access the installed original, or acquire GUI control. The original runner's `ORIGINAL_RUN_REVIEWED` guard remained `false`.

The fixture run passed all six cases:

1. Qualified `eaf60` and `1d510` callers produce one owner entry, producer entry, and normal-return record with the same invocation ID and ordered sequences. The pre-store reads retain raw `H` and field values. The first AV core precedes the detailed backtrace message, snapshots the counters, and the handler returns `false`.
2. Wrong caller return addresses are rejected; a producer entry with no owner and one under recursive owner frames are rejected as ambiguous.
3. A producer call on a different TID cannot bind to the owner frame.
4. An entered producer without `onLeave` produces no return record.
5. An unreadable `[H+38h]` produces an explicit failed-read state rather than a zero value.
6. A seventeenth owner-entry event increments overflow after the cap of 16, while sequence remains at 16.

`node --check admin/test_function_owner_probe_fixture.mjs` and `node admin/test_function_owner_probe_fixture.mjs` both exited 0. The fixture emitted `{"outcome":"PASS",..."originalLaunched":false,"guiControlAcquired":false}`. Test source SHA-256: `2dc20ace7c4bccdef78a80bf53b684d7761bddb2a4b9080722aa070f5674a946`. Runner source SHA-256: `fdb5e056df5db4d17eded8d72cade418d6e47c6e9adbc656e7fad781af07bab`.

This checks callback and caller-filter behavior against mock Frida semantics. It cannot establish real Interceptor patchability, callback delivery under process teardown, absence of fault-path perturbation, or a causal result for the original. Those remain subject to a separately reviewed owned-original run.
