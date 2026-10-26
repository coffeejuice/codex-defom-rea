# Save I/O test timing

The `save_io` JUnit result was historically about 5.57 seconds. That is a baseline observation, not a measurement of this instrumentation or a claim that the test is slow in a particular phase. The root worker owns builds and test runs.

## Optional timing output

Set `M2M_SAVE_IO_TIMING=1` in the test process environment to emit timing records to stderr. Each line names the scenario, phase, and elapsed milliseconds:

- `total`: full scenario function wall time, including assertions and cleanup.
- `fixture-copy`: time spent in the recursive fixture copy in `Rig` construction.
- `convert`: time spent inside a `Rig::convert` call.

The test's normal stdout, case registration names, assertions, exit code, fixture inputs, and native Windows handle/share/lock setup are unchanged when the variable is unset. When enabled, only diagnostic timing lines are added. The scenario wrapper and timing scopes do not alter file operations or catch test failures.

## Suggested comparison

Run the existing `save_io` test once with the variable unset and once with it set to `1`, using the same build, fixtures, and machine state. Preserve the JUnit total and the emitted phase lines. Compare repeated runs before attributing a change to a code path; the scenario total includes filesystem setup and cleanup beyond the two instrumented `Rig` phases.
