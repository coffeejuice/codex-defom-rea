# CP395–398 efficiency review

CP395 and CP397 each used one fresh, source-frozen monitored inert occurrence; the command runner reported about 0.97 and 1.38 seconds respectively for the monitored command. CP396's final dual-root injected suite ran 629 tests in 0.430 and 0.445 seconds. CP398's final dual-root suite ran 632 tests in 0.439 and 0.425 seconds. These are process-reported test times and exclude design, preflight, review, Git and documentation. No C++ source changed, so no public converter build was needed.

The CP395 exact flag value exposed a concrete harness mismatch, and CP397 confirmed its repair reached CP333. CP398 now retains only high-value CP334 markers in the live child to reduce fsync overhead and leave trace capacity for CP333. Keep one fresh occurrence per frozen change and inspect full cleanup before any successor. No token count or end-to-end speedup is established. Review again after three checkpoints or one hour at a safe boundary.
