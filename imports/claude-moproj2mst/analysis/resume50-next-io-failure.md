# Resume 50 public MST first-write fault regression

Added `save_mst_stream_failure_after_open_keeps_generation_side_effects` to `source/apps/moproj2mst/tests/s/test_save_io.cpp`.

The test uses the existing full-Save `Rig`, confirms Boolean destinations are initially absent, seeds a separate MST output path, and byte-locks offset zero through a second read/write-shared handle before calling public conversion. The locked path is the MST output, not the project input, so Open can complete. The existing host probe establishes that QFile-compatible open/truncate succeeds under the lock and the overlapping first write fails with `ERROR_LOCK_VIOLATION`; the test therefore requires an empty truncated MST target.

Assertions distinguish three effects:

- Candidate adapter result is `M2M_E_WRITE` and its message begins `cannot write MST `, matching `convert.cpp`'s explicit post-close `output.writeFailed()` mapping.
- Boolean BWZ and control INI markers are written after MST open, showing generation callbacks/side effects continue despite the stream failure.
- The MST target remains empty because its first locked write fails after truncation.

The original behavior qualification is separate. `analysis/wave3-boolean-save-io-contract.json` states that after MST opens, stream failure leaves the original internal MST result true, generation continues, and top Save returns its prior successful sub-project-check status. The GUI wrapper's result is not the m2m adapter status; this test does not assert that `M2M_E_WRITE` is original GUI behavior. The companion `save_failed_mst_open_keeps_prior_save_effects_but_skips_boolean_writers` continues to cover the earlier open-failure gate.

No build/test was run by this worker; root owns validation. No production or CMake files changed.
