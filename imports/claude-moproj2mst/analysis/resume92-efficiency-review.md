# Efficiency review after checkpoints 89–91

The successful full test launchers took 14.141 seconds for 253 registered tests
at CP89, 14.234 seconds for 254 at CP90, and 15.094 seconds for 255 at CP91.
CP91 also consumed a 14.000-second first run that found one stale test
expectation. The identical production candidate passed after that fixture was
corrected. CP92's next successful run took 15.078 seconds for 256 tests; it
is an observation at this review boundary, not an additional checkpoint in
the three-checkpoint interval. All these launchers used threaded fixture
hashing, source binding and no fixture mutation.

The CP89 grouped configure/focused/full build phases took 2.156/5.000/18.687
seconds; CP90 took 2.250/6.547/30.219; CP91 took
2.156/6.547/30.844. The later CP91 test-only repair reused production
objects and took 2.109/2.547/0.797 seconds. The larger full-build phases
coincide with new DOE record types and tests; this is an observation across
different edits, not a controlled slowdown estimate.

Grouped source ownership avoided concurrent builds. Focused CTest runs stayed
under one second for the affected targets except `m2m.s.save_io`, which took
5.23 seconds in the CP89 focused set. Independent reviews caught the CP88
Cogging ordering issue and the B98 harness's input-mode/hash binding issues
before later acceptance claims; those reviews added work but prevented false
completion. The verified B98 comparison is candidate versus retained output,
not a new original run.

The previously accepted fast test configuration remains appropriate. No
measured 10× end-to-end reconstruction gain is claimed. Continue grouping
disjoint source edits for one build, preserve failed-run receipts, and run the
full suite once per source-stable candidate after focused tests and reviews.
