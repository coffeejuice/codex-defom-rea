# Save-current timing instrumentation

`optimization/test-speed/source/apps/moproj2mst/tests/s/test_save_current.cpp` now accepts `M2M_S_TIMING=1` to emit one timing receipt per completed `runCase` call to stderr. The default test path does not emit timing lines or change its stdout, case list, native converter calls, comparisons, or assertions.

Each receipt includes case setup (project initialization through view preparation), active-operation loading, `engine::saveProject`, file/oracle comparison, and total elapsed wall time. Example format:

```
TIMING save_current case=R11 setup=...ms open_operation=...ms save_project=...ms compare=...ms total=...ms
```

The timer uses `std::chrono::steady_clock`. The environment switch is presence-based: set it to any value to enable. Timing is observational only; it does not skip work or change test outcomes.

No build or test was run for this change, as directed. Historical JUnit runtime: approximately 4.99 seconds. No new timing baseline has been measured yet.
