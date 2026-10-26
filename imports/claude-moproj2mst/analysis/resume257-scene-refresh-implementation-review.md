# CP257 scene-refresh implementation independent review

## Verdict: PASS

The corrected implementation preserves the approved source order: shown main, shown secondary, shown third, all before `sceneBox` and camera work. Each branch checks its own live guard before its shown flag, does not deduplicate aliases, and leaves pending `viewerRefresh` behavior unchanged. The diff does not expand `sceneBox`, touch camera radius/reset logic, or refresh named models.

The corrected actual-caller test now covers the required cache prefixes: main-only refresh with hidden secondary/third stale; secondary-only refresh with third stale; and both shown slots updating in one `displayRefreshScene` call. It then calls `displayDropSecond` while the canonical secondary guard is live, verifies the guard and owner are gone, and confirms the shown third still refreshes with the naturally empty secondary slot. This removes the prior artificial null-guard/live-owner state and avoids dereferencing the destroyed secondary. Curve extents remain independently distinguishable, and assertions inspect each model's cached box rather than claiming a multi-model camera-box union.

The focused receipt reports binding preflight, stable focused build and CTest 1/1 after the correction; `git diff --check` passed. Recorded source hashes bind `viewer_camera.cpp` and `test_viewer_box.cpp`. No full-suite run or freeze is claimed, leaving those to integration.

The implementation remains bounded by the static original refresh-order qualification. It does not claim natural original secondary/third reachability, Qt callback/reentrancy parity, pending native continuation, or final camera union equivalence. Concurrent CP251/CP254 test hunks in the shared test file remain outside the CP257 review scope.

No source, tests, build, GUI, or mutable REA state was changed or run for this rereview.
