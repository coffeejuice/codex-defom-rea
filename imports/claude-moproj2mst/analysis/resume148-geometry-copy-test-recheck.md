# CP148 repaired copy-test recheck

The test-only repair resolves the source-selection gap from my CP148 review. The sizing observer waits until the last actual variable lookup (ordinal 27, after lookup), confirms both allocations exist, writes distinct exactly representable values `{1.25, -2.5, 7.75}` into the actual step-owned A source, and leaves separate `{11, 12, 13}` sentinels in the actual destination. The completed refresh then checks destination values plus byte equality of destination/source/expected across all 24 bytes. A zero-fill or wrong-source copy no longer passes. The later cached refresh still asserts that a new destination sentinel `{21,22,23}` is retained without repeating the copy path.

Production Viewer is unchanged from the reviewed CP148 source. No blocker found in this test-only repair. This remains a candidate-side adversarial observation, not a fresh original runtime capture. No build or test run was performed.

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/tests/mdt/test_geometry_a_producer.cpp` | `0E17210A2C19F4EF2CA22B4787496B430FEA869FBFA381DCAAB80D9570442C52` |
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `154245702A04FDB3C16E01F9EA8A6EAA9AEBE14DE841D141FC231B295A23EADA` |
| `analysis/resume148-geometry-copy-source-values-addendum.md` | `B87DB4D41518C752C216B4E93A55AD38A439681E50368FC186313C56C708526C` |
