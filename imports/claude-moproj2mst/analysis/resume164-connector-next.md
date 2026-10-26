# CP164 connector: first actual shared QList binding

## Decision and boundary

The next smallest source increment after the CP163 prototype-one private-allocation frame is **one root-owned QList shared-null binding and its first reference increment**. The existing pending Report/private identities remain the receivers. On the qualified normal, non-null private-allocation path, resume QtCore private initializer `0x67131160` at RVA `0x131185`, bind private offset `+0x18` to the one QtCore `QListData::shared_null` symbol, account for the unconditional atomic `+1` at RVA `0x131190`, and stop **before RVA `0x131198`**. Do not enter the subsequent private address-point change, QString or QVector references, second QList increment, backlinks, TLS/thread selection, process counter, TaskInfo, registration, or public DBLIST.

This is source-ready as a **symbolic shared reference delta with an unknown absolute base**. The original static bytes prove the address and unconditional increment on this path. They do not provide a live before/after count or establish all other users of the QtCore global. If implementation requires a numeric reference count, the missing witness is a same-call read of the actual `QtCore+0x2f0a50` DWORD immediately before and after this first `LOCK ADD`; do not use a test-provided `10 -> 11` ledger observation as that witness. An actual TaskInfo owner is not source-ready: public QObject has not returned, and the later TLS/QThreadData and call-time font values remain unqualified.

## Native chain and exact cursor

The shipped `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (preferred base `0x140000000`). Its TaskBase `0x141403210` passes null parent at `0x141403222..0x141403229` to the imported public QObject constructor. The shipped `QtCore4_64.dll` SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` (preferred base `0x67000000`). Public QObject `0x671313d0` creates the `0x88` private allocation and invokes private initializer `0x67131160` with version `0x40804` on the normal non-null branch. CP163 retains this same task and private identity with task address point `QtCore+0x23cac8`, initial private address point `QtCore+0x1a9a30`, and cursor before `0x67131185`.

The next three instructions are contiguous: `0x67131185` loads `QtCore+0x2f0a50` into `RCX`; `0x6713118c` stores `RCX` at private `+0x18`; `0x67131190` performs `lock add dword ptr [QtCore+0x2f0a50],1`. The next instruction at `0x67131198` loads the *later* private address point `QtCore+0x1a9d10`. The PE export map independently identifies `0x672f0a50` as `?shared_null@QListData@@2UData@1@A`, which binds the referenced address to the actual module global rather than a factory-local list. These are preferred-image VAs; model and tests must use the qualified module SHA-256 plus RVAs `0x2f0a50`, `0x131185`, `0x13118c`, `0x131190`, and `0x131198`, never assume a loaded base or treat a host pointer as a native pointer.

The byte window `[0x67131185, 0x67131198)` is 19 bytes at PE raw offset `0x130585`, SHA-256 `110efb63fd2d3c3005f668334e43abf219e8382a25575708272be433a53b4772`, hex `488d0dc4f81b0048894b18f08305b8f81b0001`. End is exclusive. This instruction window is within the retained `0x67131160..0x67131240` exception-directory unwind region; neither the unwind bound nor this window alone proves all logical-function paths.

## Actual owner and admission

`ConversionSession` constructs one `WidgetRuntimeState` and passes that same shared owner to `TaskFactoryRuntime`; the factory retains only a weak link. `TaskAllocationStore` owns the pending prototype, and its CP163 frame owns the sparse private state. `WidgetRuntimeState` currently owns the QObject process-counter projection, but has **no QList shared-null record**. `ReportQObjectSharedRefLedger::observeSharedRef` is inert and requires caller-supplied numeric before/after values. The existing `QStringPathOwner` is explicitly limited to its own adapter scope and must not be repurposed as the process-wide QList global.

Add a canonical, stable QList shared-null identity to the actual root `WidgetRuntimeState`, qualified by the QtCore digest and RVA `0x2f0a50`. The pending private frame should hold a nonowning binding to that root identity for its observed `private+0x18` value; do not duplicate the symbol/count in the factory or turn a host object address into native bytes. Retain an optional observed absolute reference count plus a `uint32` symbolic delta. On the first increment, record delta `+1` with native wraparound semantics even if the absolute count is unknown. A later observed base can reconcile the projected count; it must not be inferred from PE initialized data, a test fixture, or unrelated startup observations. The root owner must be the same one used by other constructor frames in this fresh-process adapter scope.

Admission requires the existing CP163 pending Report and private identities at `BeforeFirstSharedQList`, a live matching `WidgetRuntimeState`, and the qualified QtCore symbol. Reject an absent/mismatched runtime without consuming this event. Keep a once-only cursor or event identity on the pending frame so repeated `createTaskBase` refusals do not rebind or increment again. Record the private `+0x18` binding before committing the shared delta, matching the two native writes; if host allocation or publication can fail, prepare the needed adapter state before advancing the cursor and make no native unwind claim. After committing, the phase is `BeforePrivateFinalAddressPoint` / RVA `0x131198`. The private address point is still `0x1a9a30`; public `task+8` and private `private+8` remain unavailable. Both factory maps remain empty, and `qobjectBaseCompleted` remains false.

The exact current source seam is `TaskFactoryRuntime::initializeNativePrefix` immediately after the existing frame's `BeforeFirstSharedQList` guard. The new shared state belongs in the root runtime, with only a nonowning link in `PendingTaskPrototype::SparseQObjectPrivate` or its `QObjectPrefix`. Do not synthesize `SessionTask`/TaskInfo, preseed a map, or advance the inert ledger as a substitute for root-owned state. Constructor failure/null-allocation and destructor/ref-release behavior remain separate unsupported paths.

## Focused acceptance contract

1. In a native factory bound to an actual `WidgetRuntimeState`, the first refusal after CP163 preserves the same task/private identities, records private `+0x18` as the root's qualified QList identity, records exactly one symbolic `+1`, and stops before RVA `0x131198`.
2. Reentry with the same or a different requested key preserves that exact binding, delta, cursor, and one pending prototype; no second reference is counted and no requested task is returned.
3. A missing or foreign root runtime refuses without claiming a QList binding/increment. An inert ledger observation or an isolated QString owner cannot satisfy this admission.
4. The private address point remains `0x1a9a30`; backlinks, second QList/QString/QVector deltas, thread and QObject counters, TaskInfo, completed task, and both factory maps remain absent/unchanged. A separate two-frame unit check on one root owner should show a common symbol identity and additive deltas, without suggesting prototype two is reachable through the current factory.
5. Run the focused `doe_task_factory` and root-owner tests and the normal checkpoint build/regression gate after implementation. This converter pass ran no tests or build and establishes no runtime count or original positive-path comparison.

## Evidence and source receipts

All paths below are relative to this converter checkout. The original binary hashes above were rechecked against local retained files. SHA-256 receipts for the exact inspected snapshots:

| File | SHA-256 |
|---|---|
| `analysis/resume163-connector-prototype-bytes.json` | `b38ce1278e3ef032cb2f1a8e125c22c73357bc176dbc3e46f3a411e320f381c3` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/wave3-closure-next-close-splitter-locale-equality-body-static.json` (PE export map) | `a0683413a66fc6a14925aab8e5e9d8647610c8024ef2071de2f0fbb027533012` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `87cd85e2fd08ac2550f89db01ba9092bf6302bf4166597d6781debe7c1a06520` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `d708e5a3e204ea574fa775184669bb357c250b2395dfa0127bd15253717ab936` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `0715b73412eaac42084d3830de6a667d4804777bc76c040137053ca9a4078eff` |
| `source/apps/moproj2mst/src/engine/view_manager.cpp` | `4f11d60d40ccef3730f4cd7575af53ebf60af1af8581b76fb7d732e64c69e805` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `679f4c2195a4e664709ec946a67253c177e3b3957ef36bf253dce53178eef9a5` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `e6a5eb7d43d4e31b2e58caa497457f7597f5ab65ae27afbc134661df39059c14` |

This was read-only PE/source analysis plus this note. No source, tests, shared plan/state, GUI, application, live REA session, executable, or deployment changed.
