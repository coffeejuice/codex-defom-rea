# CP165 connector implementation

## Change

Implemented the bounded QtCore address-point slice `[RVA 0x131198, 0x1311a2)`. At the existing `BeforePrivateFinalAddressPoint` seam, the factory now requires the live allocation store's associated widget root, the committed CP164 reference marker, a live private weak binding identical to that root's qualified QList shared-null record (QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, RVA `0x2f0a50`), the initial private address point `0x1a9a30`, and incomplete QObject state. Admission changes only that same private object's address-point RVA to `0x1a9d10` and advances to `BeforeQStringSharedNull`. The existing CP164 `+1` is not replayed. The next refusal boundary is RVA `0x1311a2`, before QString shared-null.

Repeated same-key and different-key calls retain the pending Report, private object, root binding, symbolic delta, and final address point, with the final RVA asserted after each reentry. The allocation store's root check now requires both matching shared ownership and exact stored pointee equality. A focused aliasing-`shared_ptr` test proves that a foreign pointee sharing the expected control block is refused before CP164 binds or increments. The actual `ConversionSession` positive pairing remains covered. A controlled test reconstructs the CP164 pre-write phase and injects either a foreign weak binding or an uncommitted marker; both must refuse with the initial address point and phase unchanged, preserving the one existing root delta.

## Preflight and verification

The edited test file already uses Testkit `TEST_CASE`, `CHECK`, and `REQUIRE`; its includes are in the same source/test include conventions. No C `assert` was added. `git diff --check` passed. No build or tests were run, as directed by the parent task. No GUI control or live REA session was used. Changes are limited to the three assigned paths, `task_runtime.cpp` (needed for exact pointee equality), and this report.

## SHA-256 receipts

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `dddec94ca7766ce4637fd5bf0e39475871d866663385f5a0757dd5b788b801ee` |
| `source/apps/moproj2mst/src/doe/task_runtime.cpp` | `5d9b94001f94f014b03b32d88521aff8ce1c4c751b735123145e91b15d4befef` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `0614b3d702f2fd394fd9cdab26c4a22297781483d46245ff3c67719d69fe9a96` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `b662d12e393e794d7823525447f9d5d5accc8380fed1f0b6a55b77091577fd8a` |

## Handoff

Source ownership for the four source/test paths is released to the integration owner. The pending integration gates are the focused factory/root-owner tests, then the source-stable checkpoint build/regression and freeze. This implementation report does not claim those gates passed.
