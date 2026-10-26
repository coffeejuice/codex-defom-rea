# CP167 connector: first zeroed private field after QString shared-null

## Decision and predecessor

The smallest next owner-bound, source-ready normal-path instruction cut is **QtCore RVA `[0x1311b5, 0x1311bb)`**, end exclusive. `xor edi,edi` establishes a zero register, then `mov [rbx+0x40],rdi` writes one known 64-bit zero to the **same** retained QObjectPrivate allocation. Stop before RVA `0x1311bb`, the independent write to private `+0x48`. This is a six-byte, one-field checkpoint; grouping the four adjacent zero stores through `0x1311c7` is possible later, but is not the smallest state change. No QVector or second QList event is included.

This cut is conditional on an accepted CP166 frame at `BeforePrivateZeroFields` / RVA `0x1311b5`: private `+0x30` must already be bound to the expected root's qualified QString shared-null identity with exactly one committed symbolic increment. The inspected `task_factory.cpp` and `doe_task_runtime.hpp` still stop at `BeforeQStringSharedNull` / `0x1311a2`; a root QString identity exists in the inspected `view_manager.hpp`, but this note does not claim CP166 source or its build/test gate is complete. The CP166 plan and review supply the predecessor contract.

## Original bytes and qualification

The local shipped `DEF_GUI_MO_64.exe` has SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, preferred base `0x140000000`; the selected TaskBase path calls public QObject with null parent. The local `QtCore4_64.dll` has SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred base `0x67000000`. The retained disassembly identifies `RBX` with the incoming private pointer at RVA `0x131178`; public QObject's selected non-null path supplies the `0x88`-byte private allocation. These are static, preferred-image facts, not a live constructor occurrence.

| Preferred VA / RVA | PE bytes | Selected effect |
|---|---|---|
| `0x671311b5` / `0x1311b5` | `33 ff` | `RDI = 0` (32-bit XOR also clears the upper half). |
| `0x671311b7` / `0x1311b7` | `48 89 7b 40` | Write eight zero bytes to `[RBX+0x40]`. |
| `0x671311bb` / `0x1311bb` | `48 89 7b 48` | Next private-field write; outside this cut. |

The PE `.text` section maps RVA `0x1000` to raw offset `0x400`; hence this slice starts at raw offset `0x1305b5`. Its six bytes are `33ff48897b40`, SHA-256 `83e3b8600396187be92c30488605db2a899dfb855a60416dfb8426024eea4494`. For comparison only, the full zero-field quartet `[0x1311b5, 0x1311c7)` is 18 bytes `33ff48897b4048897b4848897b5048897b58`, SHA-256 `78771835f6eabd8df4fe13c7fd49ac6843a294532b76624a1690949bf55dc69a`. The next container instruction is the QVector shared-null load at `0x1311c7`; its binding and increment, plus the subsequent second QList binding and increment, require later cuts. The displayed private initializer is an exception-directory unwind region, which does not establish every logical path, null-allocation outcome, or native unwind.

## Owner-bound model and admission

Extend `PendingTaskPrototype::SparseQObjectPrivate` with an **optional 64-bit observation for native offset `+0x40`**. Its absence means the field's allocated bytes remain unknown; a default-constructed host value must not be misreported as an observed native zero. The pending Report owns the same sparse private object throughout. The field observation belongs there, not in a factory-local surrogate, `SessionTask`, or the root shared-null records.

At a new exact phase `BeforePrivateZeroFields`, require the one pending Report and unchanged private identity, `nativeAllocationBytes == 0x88`, private address point RVA `0x1a9d10`, false QObject/base completion and unavailable backlinks, the committed `private+0x18` QList and `private+0x30` QString bindings, and their pointer identity with qualified records on the allocation store's associated **same** live `WidgetRuntimeState`. Require the QString and QList symbolic deltas to be already committed, not re-applied. Require `+0x40` unobserved before this write. An absent, expired, aliased-foreign, unassociated, or mismatched root, or any malformed predecessor frame, must refuse without changing `+0x40`, phase, bindings, or either delta.

On admission, set only private `+0x40` to known `uint64_t(0)` and advance once to an explicit `BeforePrivateZero48` phase / RVA `0x1311bb`. Preserve pending/private/root identities and all other unknown fields. Register `RDI=0` is the instruction-level basis for the later stores; no host register object is needed to claim the first field. Do not zero `+0x48`, `+0x50`, or `+0x58` early, and do not assign QVector or second QList identities/counts. Public QObject has not returned; task/private backlinks, thread selection, QObject counter, TaskInfo, session task, and factory maps remain unavailable.

## Focused acceptance and missing witness

1. Prepare a qualified, accepted CP166 frame. Capture pending/private pointers, the expected root and both shared-null object identities, both symbolic deltas, all four zero-field observations, and phase. Advance once. Require the same identities and deltas, only `+0x40 == 0` newly known, `+0x48/+0x50/+0x58` still unknown, and phase before `0x1311bb`.
2. Attempt same-key and different-key reentry. Inspect the retained frame **after each** refusal: `+0x40` stays zero, no second write/event is counted, both root deltas are unchanged, no prototype/task/cache growth occurs, and QObject completion/backlinks remain false/unavailable.
3. Before the write, inject missing/expired/foreign root or binding, uncommitted CP166 marker, wrong address point, or a prematurely observed `+0x40`. Each must refuse without writing or advancing. Include an aliasing `shared_ptr` case to enforce the store's exact-pointee root check, and a positive actual `ConversionSession` pairing.
4. Run focused `doe_task_factory` and root-owner tests, then the source-stable checkpoint build/regression and original comparison gates used for implementation acceptance. This static pass ran none of them.

No new live witness is needed to assert this one write on the selected normal path: the instruction bytes and retained private pointer are sufficient. **The predecessor is the missing acceptance witness in this checkout snapshot**: CP166 source must produce its guarded `BeforePrivateZeroFields` frame and pass its required gates before this cut can be integrated. A numeric absolute QString/QList count, actual constructor occurrence, exceptional path, or native release would need separate runtime evidence; this zero write supplies none.

## SHA-256 receipts and scope

Paths below are relative to this converter checkout. The file receipts describe the inspected snapshot, not a candidate freeze.

| File | SHA-256 |
|---|---|
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/resume166-connector-next.md` | `566781bf6a76db55ba945e19ccdc3ecd4008e4ba83d3a8fc347f1c509ea0dca9` |
| `analysis/resume166-connector-plan-review.md` | `da0b182b61f2ff0c15b79a77ba58b5b301dd750633535b1262f546473649ec7c` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `3aac8e587e26263498297996081f4d10aaa4f5ebc04e68f02be4eb944e421171` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `dddec94ca7766ce4637fd5bf0e39475871d866663385f5a0757dd5b788b801ee` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `a33b4b05e059b7313efb196fb34967c20196399b87226c4ff431c1c2106c1d88` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `b662d12e393e794d7823525447f9d5d5accc8380fed1f0b6a55b77091577fd8a` |

This pass read only local PE/source/evidence and changed only this note. No source, tests, shared plan/state, build, deployment, GUI control, original-process execution, or live REA session changed.
