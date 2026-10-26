# CP166 connector: first QString shared-null binding on the retained private object

## Decision and next cursor

The smallest coherent next normal-path increment is QtCore RVA **`[0x1311a2, 0x1311b5)`**: load the exported `QString::shared_null` data address, store it at the retained QObjectPrivate object's `+0x30`, and apply its one unconditional atomic reference increment. Stop before RVA `0x1311b5` (`xor edi,edi`), which begins the later zero-field and QVector/List initialization. This cut is source-ready as a **symbolic `uint32_t +1` on one root-owned QString shared-null identity**, provided the CP165 frame is accepted by its own build/test gate. It does not establish an absolute count, QString text value, completed QObject, TaskInfo, task, cache entry, or release behavior.

The predecessor is the same pending Report and same `0x88`-byte sparse private allocation, at `BeforeQStringSharedNull`, with final private address-point RVA `0x1a9d10`. Its earlier `private+0x18` QList binding and first QList delta remain untouched. The CP165 source currently presents this state and stops before this window; this static pass did not execute its build or tests.

## Shipped instruction, PE, and export evidence

The shipped `DEF_GUI_MO_64.exe` is SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` at preferred base `0x140000000`. Its TaskBase normal path passes null parent to the imported QObject constructor. The shipped `QtCore4_64.dll` is SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` at preferred base `0x67000000`. The retained QtCore disassembly places this window in the private initializer `0x67131160`; earlier `0x67131178` copied the incoming private pointer into `RBX`. The PE export map separately names preferred VA `0x672f0b30` (RVA `0x2f0b30`) as `?shared_null@QString@@0UData@1@A`.

| Preferred VA / RVA | PE bytes | Effect on this path |
|---|---|---|
| `0x671311a2` / `0x1311a2` | `48 8d 05 87 f9 1b 00` | `RAX = QtCore+0x2f0b30` |
| `0x671311a9` / `0x1311a9` | `48 89 43 30` | store that address at `[RBX+0x30]` |
| `0x671311ad` / `0x1311ad` | `f0 83 05 7b f9 1b 00 01` | `lock add dword ptr [QtCore+0x2f0b30],1` |
| `0x671311b5` / `0x1311b5` | `33 ff` | next instruction; outside this cut |

The end-exclusive 19-byte slice has raw PE offset `0x1305a2`, hex `488d0587f91b0048894330f083057bf91b0001`, SHA-256 `c928bc54b233d3ba3aa403355eb2fd77c11cb03c1c68ac6c21223fc22d2d5277`. I recomputed the offset from the PE section table and the bytes/hash from the local binary. These are preferred-image VAs; source state should use the module SHA and RVAs, never a loaded VA or host pointer. The displayed `0x67131160..0x67131240` disassembly is an exception-directory unwind region, not proof of every logical path or a live constructor occurrence. This proposed event belongs to the already selected normal, non-null private-allocation path.

## Owner-bound source seam

`TaskFactoryRuntime::initializeNativePrefix` currently stops at `BeforeQStringSharedNull`. `TaskAllocationStore` owns the pending Report/private frame and is explicitly associated with the one `WidgetRuntimeState` of its `ConversionSession`; the factory retains a weak root. The root already owns the distinct `QListData::shared_null` record and its symbolic delta. Add one stable QString shared-null record to that **same expected root**, qualified by QtCore SHA-256 and symbol RVA `0x2f0b30`. The private frame should retain a nonowning binding for its `+0x30` field and a once-only committed marker. Do not create a factory-local copy or count.

The existing `QStringPathOwner` knows the same symbol RVA but is explicitly an isolated path/resource adapter scope. It is not proof that its record is the fresh-process QObject constructor's owner, nor a license to reuse its caller-supplied reference observation as this constructor's starting count. If the two projections are later placed in one actual process scope, they must reconcile against one canonical symbol identity rather than silently maintaining conflicting absolute counts.

Admission should require the existing one pending Report and unchanged private identity; exact `BeforeQStringSharedNull` cursor; `qobjectBaseCompleted == false` and no backlinks; private address point `0x1a9d10`; committed, live `private+0x18` QList binding pointer-identical to the qualified QList record of the **allocation store's associated root**; and a qualified QString record on that same root. Reject an absent, expired, foreign, unassociated, or malformed root before writing `private+0x30` or incrementing. The earlier QList delta must not replay. Prepare any host allocations before publishing the new binding; perform the new binding, root symbolic `+1` modulo `2^32`, marker, and phase transition as one once-only adapter checkpoint. A later refusal must leave all identities, both deltas, and this cursor unchanged.

After admission, stop at an explicit phase such as `BeforePrivateZeroFields` / RVA `0x1311b5`. Do not enter the four zero stores at private `+0x40..+0x58`, QVector shared-null at `+0x68`, second QList reference at `+0x70`, or subsequent initialization. Public QObject has not returned, so `task+8` and `private+8` backlinks, thread selection, QObject counter, TaskInfo, session task, and factory maps remain unavailable. Adapter scope release is not native reference release or unwind evidence.

## Focused acceptance and missing witness

1. On a qualified CP165 frame, capture pending/private identities, both root symbol identities, QList delta, and phase. Advance once. Require the same pending/private/root identities, new private `+0x30` binding to the root QString record, exactly one QString symbolic `+1`, unchanged QList delta, and cursor before `0x1311b5`.
2. Refuse same-key and different-key reentry; inspect the retained frame after **each** attempt. Check no second QString increment, no prototype/task/cache growth, false QObject completion, and unavailable backlinks. An absent/foreign/expired root or tampered CP165 prerequisite should refuse with no QString write or delta.
3. Two independently prepared frames in one associated root may establish a common QString symbol identity and additive symbolic deltas; do not present a second frame as reachable through the current one-prototype native factory. Keep a separate root's identity/delta separate at the adapter scope boundary.
4. Run focused `doe_task_factory` and root-owner tests, then the normal source-stable checkpoint build/regression and original comparison gates required for implementation acceptance. This note reports no such executions.

No same-call live pre/post read of the actual QtCore RVA `0x2f0b30` DWORD is retained. Such an observation is required if a numeric absolute refcount or fresh-process baseline becomes necessary; PE initialized data and a test-supplied count cannot substitute. Null-allocation, exceptional unwind, native release, and actual constructor occurrence remain unobserved here.

## SHA-256 receipts and scope

Paths are relative to this converter checkout. The source receipts are of the files inspected for this note, not a candidate freeze.

| File | SHA-256 |
|---|---|
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/wave3-closure-next-close-clear-selection-empty-hash-leaves-static.json` (PE export map) | `17fce2e9c1d96c6f9497659be2a4485b161a65ba0efb0bb05f5f467c22739cd6` |
| `analysis/resume165-connector-next.md` | `4893ab62e5ade35694549c3308d0b98494c815bc868bed5821730886245dc267` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `dddec94ca7766ce4637fd5bf0e39475871d866663385f5a0757dd5b788b801ee` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `0614b3d702f2fd394fd9cdab26c4a22297781483d46245ff3c67719d69fe9a96` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `88406917fe165126567c84b0f4af586c36940ff65d22ef89f1a4b150f4f6f562` |
| `source/apps/moproj2mst/src/engine/view_manager.cpp` | `d1ac4321eef560232771f175c1d2bbdc8e7d7fa18a51269c4c060d950d92c382` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `e75573b6264abd07afc082472f1d532e9a0467ff262a458e15bc9242f3cd9d7d` |
| `source/apps/moproj2mst/include/m2m/doe_qstring_path_owner.hpp` | `61d028f5f9fd80df03c2a6dde9eb4b78c4228fe670e20f9076eabb945e39efe1` |
| `source/apps/moproj2mst/src/doe/qstring_path_owner.cpp` | `ae63e9c8e5c1b72b26a09d5d682c8e3ec1c7721c95eabc415c3923c416023bc4` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `debe5478a9eb0c89a964f77f8e6c48b61489f1ba335f7694a6d7da6d629c706a` |

This pass read only local PE/source/evidence and changed only this analysis note. No source, tests, shared plan/state, build, deployment, GUI control, original-process execution, or live REA session changed.
