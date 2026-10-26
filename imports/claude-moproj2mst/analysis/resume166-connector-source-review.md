# CP166 connector independent source review

## Decision

**Accept the five-file CP166 source and focused-test change for integration gating.** Static inspection found no blocking discrepancy with `resume166-connector-next.md` or its plan review. This is a source review, not a build, CTest, original-process comparison, or candidate freeze. CP165 acceptance and the parent-owned source-stable gates remain prerequisites to claiming implementation acceptance.

The five inspected files match the SHA-256 receipts in `resume166-connector-implementation.md` exactly. I made no source, test, build, GUI, or live REA changes.

## Contract check

`ConversionSession` constructs one `TaskAllocationStore` and one `WidgetRuntimeState`, passes both to `TaskFactoryRuntime`, and associates that root with the store (`conversion_session.cpp:163-172`). The store's `ownsWidgetRuntime` checks both the shared ownership control block and pointee address (`task_runtime.cpp:22-38`). The CP166 guard uses that exact association before the new write (`task_factory.cpp:166-194`). It also requires the live CP165 private `+0x18` binding to be pointer-identical to the associated root's qualified QList record, the final private address point `0x1a9d10`, the retained Report/private frame, and unavailable completion and backlinks.

The root owns a stable QString shared-null record qualified by the QtCore digest and RVA `0x2f0b30` (`view_manager.hpp:501-507,554-555,592`). It is a distinct object from the root's QList record at RVA `0x2f0a50`. The private frame keeps a nonowning `weak_ptr` binding for `+0x30` and a committed marker (`doe_task_runtime.hpp:25-46`). On admission, the factory binds that exact root record, applies one `uint32_t` increment, marks it committed, and advances to `BeforePrivateZeroFields` (`task_factory.cpp:196-199`). The previous QList delta is not touched. Because the next call encounters the later phase, it cannot replay this increment. The unconditional refusal then stops before QtCore RVA `0x1311b5`; no zero fields, later containers, QObject completion, TaskInfo, task, or cache entry are created.

All CP166 rejection checks precede the field write and increment. The root record is allocated with the root, and no host allocation or throwing conversion follows the guard. An invalid CP165 frame, foreign or expired root, or existing QString binding therefore leaves this checkpoint's cursor, binding, and both deltas unchanged. The symbolic `uint32_t` increment wraps modulo `2^32`; the new record stores no absolute count or QString contents.

The focused tests inspect the retained pending/private identities, both root bindings and deltas, cursor, cache/task absence, and reentry after each same-key and different-key refusal (`test_doe_task_factory.cpp:299-356`). Synthetic rewinds exercise damaged CP165 prerequisites and foreign, unassociated, and expired roots without treating those rewinds as a reachable second constructor path (`:419-489`). Two independent factory frames sharing one root demonstrate identity reuse and additive symbolic deltas (`:561-581`). The root-owner test checks stability, distinct roots, and independence from QList (`test_view_manager.cpp:1403-1419`).

## Integration action

Run the focused factory and root-owner targets, then the required source-stable checkpoint build, full regression, freeze, and original comparison gates. Do not describe the static source review as a native occurrence witness or report an absolute QString reference count. No same-call live pre/post QtCore DWORD observation is retained.

## SHA-256 receipts

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `0570464fc5a7b84f1d468fe5de07552e6de587663c8a1e2f6bb7a35b1d7e2c88` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `3aac8e587e26263498297996081f4d10aaa4f5ebc04e68f02be4eb944e421171` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `a33b4b05e059b7313efb196fb34967c20196399b87226c4ff431c1c2106c1d88` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `af42a0ab96e08e0fbb9b7261c3c75e3eeb3b6b66d2f2a54277825631591119a8` |
| `source/apps/moproj2mst/tests/mdt/test_view_manager.cpp` | `12999d18b38706a0e997980a8c62ebe4e373b4d69b2ac329918f9dc0bb9c774a` |

## Focused-test repair review

**Accept the test-only repair for rerun.** The revised `test_doe_task_factory.cpp` is SHA-256 `9a0c963de03209f070aafc4e3c5b5bcc66b231be889c6f574a6f3d11f9c10bd7`, matching the updated implementation report. The four production source/header hashes in that report are unchanged from the receipts above.

The former foreign/null reassociation expectation conflicted with `TaskAllocationStore::associateWidgetRuntime`: once a nonnull root is associated, later calls return without replacing it (`task_runtime.cpp:22-28`). The repaired test applies those attempted reassociations before the factory call, verifies that the original root remains the store owner, and expects CP166 to advance on that original root with one QList and one QString delta (`test_doe_task_factory.cpp:458-482`). This is the correct positive path for the one-shot association rule.

The expired-root negative remains separate (`test_doe_task_factory.cpp:485-510`). The store and factory hold weak root references, while the test retains only the two symbol records after `widgets.reset()`. Its synthetic rewind returns the frame to the CP165 cut; the next call must refuse before a second QString write or increment, leaving the retained frame, cursor, and both baseline deltas intact. The retained symbol records do not keep the `WidgetRuntimeState` alive, so this still exercises an expired root. The rewind is a guard probe, not evidence of another reachable native constructor path.

The updated implementation report accurately records the initial focused-test failure and says the repaired test has not yet been rebuilt or rerun. Its new hash supersedes the earlier test hash in the table above. A focused CTest rerun and the remaining integration gates are still required; this static review does not claim they passed.
