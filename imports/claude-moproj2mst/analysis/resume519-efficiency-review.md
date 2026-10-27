# CP516–519 efficiency review

The three source-stable verified checkpoints CP516r, CP517 and CP519 had
grouped full-build steps of 35.562, 51.156 and 51.328 seconds. Their bound
387-test launchers measured 24.453, 25.500 and 22.766 seconds. Focused
subproject cases rose from 52 to 53 to 54, all passing. Historical Save
remained 93 SAME/five DIFF.

CP516's first focused test correctly rejected a decimal boundary fixture
that rounded to the upper integer. The corrected candidate was rebuilt and
source-bound before freezing; the initial failed run is retained in
`analysis/cp516-build.json`. This is test-design rework, not a native behavior
discrepancy. The build/launcher receipts do not establish a tenfold speedup.
Independent PE audits and read-only reviews ran alongside source-stable
builds without competing source writers or mutable original sessions.
Token usage is unavailable from local receipts. Continue the grouped
preflight, full launcher and freeze gates. Review again after three more
checkpoints or one hour at a safe boundary.
