# Efficiency review after checkpoints 86–88

The successful complete test launchers registered 249, 250 and 252 tests and
took 13.469, 13.859 and 13.578 seconds respectively. All three used the
threaded fixture hash configuration and recorded stable source binding and no
fixture changes. CP87 additionally consumed a 13.875-second failed full run
before two test-only corrections. CP88 had a focused `viewer_box` fixture
failure before a 13.578-second passing full run. Independent source review
then found an item refresh/union ordering defect; the corrected candidate
took another 13.750-second full run. Failed and superseded attempts count
toward elapsed work and remain retained as diagnostics.

The grouped CP86 build phases took 2.047 seconds to configure, 3.750 seconds
for focused preflight and 17.766 seconds for the full build. CP87 and the
first CP88 test-only repair builds reused compiled production objects: their
configure, focused and full phases took 2.157/3.312/0.813 and
2.203/4.110/0.844 seconds. The later CP88 production-order repair took
2.187/4.031/19.265 seconds. These runs had different source changes and
are not a controlled build-speed comparison.

The bounded folder IPC open retry did not create a visible full-suite time
increase in these three successful launchers. One CP85 transient motivated
it; three subsequent clean launches cannot establish a failure-rate ratio.
Exclusive source ownership and grouped preflight continue to avoid overlapping
builds, but source-review and original-evidence gates remain the limiting
steps. No measured 10× end-to-end speedup is claimed. Continue exact fixture,
source, artifact, ABI and historical checks; avoid repeating unchanged full
runs unless a concrete failure requires diagnosis.
