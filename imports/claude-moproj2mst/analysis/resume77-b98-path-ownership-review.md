# CP77 B98 Diestress path ownership review

## Result

I found no source-closed Diestress path normalization or project-owner defect in the B98 path. The current writer uses the same source-qualified path owner and transformation: `diestress.cpp:115-120` obtains the Project's active subproject and calls `subProjectDbPath`; `paths.cpp:12-17` derives the full project path with `.DB`; `diestress.cpp:170-181` changes `.DB` to `.KEY` case-insensitively and emits the resulting path through the QString `KFREAD` helper. On B98's ordinary project subproject, this gives the directory of `Gear_blank.moproj` plus `Gear_blank.KEY`, which matches the retained original's case-specific project-local key path. There is no evidence for replacing this with a stable root, relative path, output-MST directory, or global normalization. No production converter change is justified.

The original historical MST is 2,756 bytes (SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`) and contains the old absolute case root on its Diestress KFREAD path. The previous candidate reproduction has a single 30-byte longer path line, with all remaining bytes equal under exact replacement of that path (`analysis/resume48-b98-raw-repro.md`). This is expected when the candidate project lives in a different directory; it is a valid raw `DIFF`, not evidence that the writer should emit the historical directory.

The CP53 test (`tests/s/test_b98_kfread.cpp`) copies the fixture and checks the candidate path equals the copied project's `Gear_blank.KEY`, then replaces only that exact value in an in-memory comparison string and checks the entire candidate MST against the historical bytes. The test never overwrites either raw file. An integrated receipt at `build/test-runs/1792887553418918400/receipt.json` records `m2m.s.b98_kfread` passing and the separate log shows the case passed. That supports the bounded candidate rule; it does not demonstrate fresh-original execution or raw byte parity under a matched root.

## Actionable harness constraint

There is a concrete path-policy mismatch in the current focused test setup. The CP53 report says the test verifies full paths below 100 characters, but `tests/s/test_b98_kfread.cpp` contains no such length check. The configured `M2M_TEST_SCRATCH` recorded in generated `build/candidate/CTestTestfile.cmake` is already 93 characters (`C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst/build/candidate/t/<hash>`); the test then appends `b98r-<pid>-<tick>/Gear_blank.moproj`, producing a generated project path over 100 characters. Current CMake setup (`source/apps/moproj2mst/CMakeLists.txt:31-36`) also requires test scratch to remain beneath the takeover's build directory, so setting just the test environment to `C:\PROBLEM` would fail its configuration/runtime containment assumptions.

Smallest safe qualification for a future test rerun: configure a dedicated disposable binary tree under `C:\PROBLEM` (for example `C:\PROBLEM\b98build`) so its generated per-test scratch and B98 copied project remain under 100 characters; explicitly assert the absolute project, KEY and MST paths' lengths before copying/running. This is a harness/test-path correction, not a converter fix. No build or test was run for this review.

## Missing evidence for a stronger claim

To call B98 raw parity at a fixed root, the missing evidence is a fresh original Save of the same B98 input using an equivalent short project root, with original executable/action/session identity and raw MST hash captured. The original comparison gate remains closed until supported GUI-control release and a verifiable release receipt are available; this review does not recommend acquiring control now. The historical fixture plus candidate-only test is sufficient to retain the path as project-root-derived, but not to claim fresh-original parity, non-ASCII locale behavior, or generalize this path rule to other KFREAD writers.

Read-only review only: no source, fixture, comparator, build, test, GUI, original app or mutable REA data was changed or run.
