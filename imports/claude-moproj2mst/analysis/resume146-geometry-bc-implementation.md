# CP146: six BC attempts and node count commit before the sizing barrier

The initial high main-coordinate prefix now performs the six native BC integer calloc/publication attempts in order, then publishes node-class count1. It remains incomplete before variable sizing, equal-width copy, node Stop, retirement or second getter. The actual allocator/dispatched boundary and original hashes are in `resume146-geometry-bc-sizing.md`.

The existing exact initial frame/source-A/actual object/destination ownership proof is retained. Before the new cut, all six actual n1 counts D[0x12..0x17] must be nonnegative. The source records each requested count/pointer/attempt and publishes only node BC fields+f0..+118. Count0/null is normal; a diagnosed positive-count null is also published and does not add a success gate or rollback. With unchanged receiver/profile, later attempts continue exactly as native. The seventh +120 pointer stays null. Node n1 is written only after six attempts and the final receiver/source/profile recheck.

Already-published pointer/count pairs are checked at each next boundary, while unconsumed slots must remain null. Callback-sensitive profile changes retain the reached prefix and stop before further work. Private frame counts record requests; a failed positive request/null pointer is not represented as a valid positive-sized span. Actual node storage owns successful allocations and frees them normally.

The main descriptor and canonical empty-variable checks were factored without changing their prior requirements. Only the current high initial assignment path gets the new cut. Low/null callers, CP141 no-copy mismatch, CP144 zero-main completion and public/equal-copy guards remain unchanged. No whole allocator or variable-sizing helper is called as a shortcut.

The existing producer target now contains12 Testkit cases/129 profiles, with161 CHECK and60 REQUIRE callsites. Two new cases add18 profiles:

- Sixteen version6000/11000, source extent3/6 and BC count-pattern profiles (all0, all1, mixed0/1/2/3/4/5 and reverse) check real zero-filled integer buffers in their exact fields, ordinary nulls for0, untouched seventh field, node n1, unchanged layout/reset state, empty variable slots and dim0=0, and no main sizing dispatch. Leading null slots followed by positive allocations exercise continuation without a null-success gate. One optional COW/arrays checkpoint occurs; no Stop or known scene is produced.
- The same profiles repair raw presence and repeat refresh, requiring stable source/primary/BC pointers, no further COW/reset/allocation/sizing and a retained barrier.
- Two negative-count profiles place -1 in the first or last BC count. Both retain the coordinate-only prefix before any BC attempt, with node n0. Repairing the count does not resume the frame. No huge allocation or real OOM is used.

CP145's coordinate-publication cohort now explicitly seeds a negative first BC count, preserving its genuine producer-only stopping purpose. Other guard/reset-failure cohorts are unchanged. Native failure continuation is established by the source loop's unconditional pointer publication/next attempt and original body; no deterministic positive-request allocation failure is claimed as runtime tested.

Static preflight passed for comment/literal-aware delimiter balance, project include existence, no trailing whitespace, unique cases, one Testkit main, no assert macro and CHECK/REQUIRE outer arity. The existing diagnostic integer allocator declaration/behavior was inspected. No compilation or runtime pass is claimed.

Only `viewer.cpp`, existing `test_geometry_a_producer.cpp` and this receipt were edited for implementation. No shared header/CMake, build/test execution, GUI/original launch, new REA tool call or mutable REA access occurred. Root owns changed-test binding preflight, focused `m2m_mdt_geometry_a_producer` / CTest `m2m.mdt.geometry_a_producer`, other geometry regressions and the full suite.

Frozen files:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `c5eaf75cd46fa68ad5b91b0638b42d8405df1d1fbd3367acc37968ea2d2f0294` |
| `source/apps/moproj2mst/tests/mdt/test_geometry_a_producer.cpp` | `eb0a452c2cdb048d4a85d99095f9521d3fc3ed345072ca025d636725ae4f6d82` |
| `analysis/resume146-geometry-bc-sizing.md` | `391dca8208d2ad3fffc3c8eb532036389b02e2e5036e4da502f6537cffd674c0` |
