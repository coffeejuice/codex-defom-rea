# CP132 root integration

The isolated feature ContextEnvelope now embeds its child map at `C+0x1458`
and records the 55 source-qualified pre-string stores. The standalone map
path remains separate. The private Type3 classifier covers all 93 original
table entries. SPRING2's exact two-row `0B2\r\n` retry is narrowly gated,
and the geometry reader advances through the qualified pre-skip MAR load and
all-zero node-copy early return to a node-timer Stop. These cuts preserve
Project, public Save and Close admission guards.

The grouped full build is `analysis/resume132-final-build.json`, source SHA-256
`06668fc4fbe3d5064910274fd2de89badb389c509ea8a7c4714634bb524c30e8`
over 3,473 source files, stable before/after. Five focused CTest targets
passed after the geometry fixture repair, including the new feature-context
target and the existing feature CRT runtime target.

The first full test run had one `m2m.g0.cli_isolated_worker` case failure:
the stale-job-nonce test observed supervisor stop status `0xe1050001` instead
of expected 66. Its raw output is retained in
`build/logs/resume132-final-full.stdout.json` and its case log in the first
run's test-runs directory. The same binary passed the test on a focused
recheck; without changing source or binaries, the full suite then passed.
The successful source-bound rerun is in `analysis/test-relocation-results.json`
before freeze: 317 registered, 310 clean, six logged-gap passes, one
logged-difference pass, zero failed/skipped, stable source/binaries and zero
fixture changes. The initial failure's cause remains unproven and is retained
as a validation qualification, not erased.

Worker source contracts are `analysis/resume132-feature-context-prefix.md`,
`analysis/resume132-spring-two-row.md`,
`analysis/resume132-type3-classifier.md`, and
`analysis/resume132-geometry-node-skip.md`. Independent reviews and the
Spring gate and geometry pre-skip repairs are retained alongside them. No GUI
control or original application session was used.
