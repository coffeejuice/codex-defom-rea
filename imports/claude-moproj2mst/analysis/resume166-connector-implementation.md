# CP166 connector implementation: QString shared-null on the retained private object

## Outcome

The native factory's selected normal path now advances the same pending Report and same sparse `0x88`-byte QObjectPrivate frame through QtCore RVA `[0x1311a2, 0x1311b5)`. It binds private `+0x30` to the `QString::shared_null` identity qualified by QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and symbol RVA `0x2f0b30`, then records one symbolic `uint32_t` increment on the associated root. The cursor becomes `BeforePrivateZeroFields` and refuses at RVA `0x1311b5`.

The root's QString identity is distinct from its existing QList identity. The private binding is nonowning. Admission checks the allocation store's exact associated `WidgetRuntimeState`, the committed CP165 `private+0x18` QList binding on that root, the retained pending/private identity, the final private address point RVA `0x1a9d10`, uncommitted QString field, and unavailable QObject completion/backlinks. All checks precede the new field write and increment. The existing QList delta and private address point stay intact. This models no absolute QString count, string content, zero fields, later containers, thread/counter state, TaskInfo, task, cache entry, native release, or unwind.

The source basis is `analysis/resume166-connector-next.md`, its independent review, and the qualified static disassembly `analysis/resume163-connector-qobject-67131160.asm`. That disassembly is an unwind region, not a live constructor occurrence or proof of other paths.

## Focused coverage prepared

- The actual `ConversionSession` test checks its factory/store/root association and the new root-owned private binding and delta.
- The one-frame test checks retained pending/private/root identities, unchanged QList delta, same-key and different-key reentry after each refusal, no task/cache growth, and no QObject completion/backlinks.
- A synthetic rewind to the CP165 cut exercises refusal for an uncommitted or foreign QList binding, wrong final address point, premature backlink/completion, and an expired root. These rewinds are guard probes, not claims of a second native constructor path. A separate test checks that attempted foreign or null reassociation is ignored by the store's one-shot association and that CP166 advances on the original root.
- Two independently prepared factory frames under one root share both symbol identities and accumulate symbolic deltas. A root-owner test checks QString identity stability within a root, separation across roots, and separation from QList delta. This does not assert a second prototype reachable through one native factory.

The parent ran the initial focused CTest; `build/logs/resume166-grouped-focused-ctest-01-03.log` showed two failures in the root reassociation test. Inspection of `TaskAllocationStore::associateWidgetRuntime` established that its association is one-shot, so the test's attempted foreign/null reassociations could not invalidate the expected root. I corrected that test only; this revised source has not yet been rebuilt or rerun by this worker. No source-stable freeze, original-process comparison, GUI control, or live REA was run in this worker lane. A direct scan of the edited test file found no trailing whitespace. The C++ tests use Testkit `TEST_CASE`, `CHECK`, and `REQUIRE`; quoted includes were already present and remain unchanged. No C `assert` or external missing-file probe was added.

## SHA-256 receipts and release

| Source path | SHA-256 after this edit |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `0570464fc5a7b84f1d468fe5de07552e6de587663c8a1e2f6bb7a35b1d7e2c88` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `3aac8e587e26263498297996081f4d10aaa4f5ebc04e68f02be4eb944e421171` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `a33b4b05e059b7313efb196fb34967c20196399b87226c4ff431c1c2106c1d88` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `9a0c963de03209f070aafc4e3c5b5bcc66b231be889c6f574a6f3d11f9c10bd7` |
| `source/apps/moproj2mst/tests/mdt/test_view_manager.cpp` | `12999d18b38706a0e997980a8c62ebe4e373b4d69b2ac329918f9dc0bb9c774a` |

No change was needed in `src/engine/view_manager.cpp`. I release exclusive ownership of the listed CP166 source and test paths to the parent for integration review and testing.
