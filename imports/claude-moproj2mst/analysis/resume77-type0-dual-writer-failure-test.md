# CP77 paired Type-0 project and MST writer failure test

Added `save_mst_failure_precedes_project_postclose_failure_after_remesh` to `tests/s/test_save_io.cpp` only.

The paired disposable runs use the existing private project-output factory. Its callback seeds an MST sentinel and acquires a read handle with `FILE_SHARE_READ` from the project writer's `aboutToClose` event, after project and input Open/serialization have succeeded. The native MST writer then encounters a sharing-denied open. The control run reports the real project post-close status; the second run reports scripted QFile error 7 at the post-close query. Both keep the MST sentinel unchanged and verify the earlier Boolean BWZ/INI effects and enabled remesh output.

The test checks `SaveCallState` separately: the control project writer succeeds; the injected project writer returns normally but reports false; both MST attempts return false. It expects the adapter to select `cannot open MST` and `M2M_E_WRITE` in both runs, with matching `details.returnedCode`. This is a candidate adapter precedence test. Original static Save combines its project/MST booleans but returns the earlier check/status local, so neither the message nor `M2M_E_WRITE` is claimed as the native GUI return. Project error 7 is scripted; the MST denial is a physical lock on disposable test output. The lock and seeded file live only under the copied test Rig and are released before residue inspection.

Static preflight only; no build/CTest was run. The existing `native_full_save_remesh_tail_runs_after_project_or_mst_open_failure` provides prior candidate evidence for a `FILE_SHARE_READ`-locked MST open failure and retained bytes; the new test changes the timing to arm the output lock after project Open, and pairs that failure with the post-close project status injection. Root should build the focused Save I/O target before counting this case as validated.

Test file SHA-256: `1DB88821D83D69D5407F3ED72AA5CBFD3947D4C899BD7D69C56CF39EF36266E4`.

## CP77 focused-run correction

Root's first focused `m2m.s.save_io` run failed only because the test compared the control and injected runs' complete `Result.message` strings. Each disposable Rig has a unique `native-output.MST` path, and the adapter includes that path after the stable `cannot open MST ` prefix. The test now checks that prefix and independently compares each full message with its own `observation.mst` path. This preserves exact per-run path validation without comparing unrelated random scratch roots.

The focused-run message text supplied for this correction did not include literal full paths; none are fabricated in this receipt. No build or rerun was performed for this correction. Updated test SHA-256: `83FEC33EE033409B1C0FAF90AA9E15D8D82200800A0C2B3C50E55367F745CAFB`.

