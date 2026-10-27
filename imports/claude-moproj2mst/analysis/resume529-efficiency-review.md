# CP527–529 efficiency review

The CP527, CP528 and CP529 source-stable full-build steps measured 27.36,
52.98 and 179.39 seconds. Their bound 387-test launchers measured 22.656,
22.437 and 22.563 seconds. Focused subproject cases rose from 63 to 64;
all passed in frozen candidates. Historical Save stayed 93 SAME/five DIFF.

CP529 changed the shared model header and rebuilt 721 CMake/Ninja targets;
the long build is compilation dependency fanout, not test-launcher slowdown.
Exclusive source ownership let the port-model worker edit the shared types
while root prepared focused tests. The independent source review distinguished
rendered expansion rows from stable graph port identities. Token usage is
unavailable. Keep the grouped source-bound preflight/full-suite gate. Avoid
additional shared-header edits unless the next behavior needs them; this is
an observed compilation cost, not a reason to weaken coverage.
