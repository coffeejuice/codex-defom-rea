# CP165 connector: same private object's QObjectPrivate address point

## Decision and current gate

The smallest next normal-path constructor increment is the ten-byte QtCore window **RVA `0x131198..0x1311a2`**, end exclusive. It loads the exported `QObjectPrivate` address point at QtCore RVA `0x1a9d10` and writes it to offset zero of the **same** sparse private object retained since CP163. Stop before RVA `0x1311a2`, where the initializer begins loading QString shared-null for private `+0x30`. This cut creates no new owner, shared reference, task, backlink, thread selection, or QObject completion. It is source-ready **after** the CP164 root-admission defect is repaired.

The current CP164 implementation is not yet an accepted predecessor. `analysis/resume164-connector-source-review.md` identifies a concrete foreign-root case: `TaskFactoryRuntime(storeFromScopeA, widgetsFromScopeB)` passes the fixed QtCore SHA/RVA check and charges B's QList record. Before taking this cut, bind the expected `WidgetRuntimeState` identity to the allocation/session scope and reject that pair without advancing the CP164 `private+0x18` event. The production `ConversionSession` pairs its one store and root at construction, but the general factory API does not enforce the pair. Checking only the new frame's weak binding against the factory's root would preserve a previously wrong foreign binding; the scope guard must be established at the earlier admission.

## Original instruction and identity evidence

The shipped `DEF_GUI_MO_64.exe` is SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, preferred base `0x140000000`. Its TaskBase normal path passes null parent to the imported QObject constructor before TaskInfo. The shipped `QtCore4_64.dll` is SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred base `0x67000000`. Public QObject allocates `0x88` bytes for the private object and invokes `0x67131160` only on its displayed non-null path. CP163 preserves this private identity and its first address point, RVA `0x1a9a30`. CP164 binds its `+0x18` field to the root-owned QList shared-null record and accounts for one atomic increment, stopping before this window.

At preferred VA `0x67131198`, bytes `48 8d 05 71 8b 07 00` load `0x671a9d10` into `RAX`. At `0x6713119f`, bytes `48 89 03` store `RAX` at `[RBX]`; `RBX` has held the incoming private pointer since `0x67131178`. The PE export map independently labels `0x671a9d10` as `??_7QObjectPrivate@@6B@`. The precise byte window is raw file offset `0x130598`, length 10, hex `488d05718b0700488903`, SHA-256 `f64d5bb907ab12eadf9b4549a2e4122c93b3a193a1bbb35010d2add052277ec6`. These addresses are preferred-image VAs; the model should store the module digest and RVAs, not a loaded or host pointer. The selected disassembly lies inside an exception-directory unwind region; that region alone does not prove all logical paths.

The next instruction at preferred VA `0x671311a2` is `lea rax,[0x672f0b30]`, and the PE export map labels that target `?shared_null@QString@@0UData@1@A`. Its subsequent private `+0x30` write and atomic increment are outside this cut. The existing `QStringPathOwner` belongs to a separately qualified adapter path and does not by itself establish the one fresh-process QString owner needed for that successor.

## Source seam, guards, and observable state

`TaskFactoryRuntime::initializeNativePrefix` is the reachable continuation. Require one pending Report node and its unchanged `SparseQObjectPrivate` identity at `BeforePrivateFinalAddressPoint`; `qobjectBaseCompleted` must still be false. Require the CP164 `sharedNullReferenceCommitted` marker, the live private `sharedNullBinding`, and pointer identity with the same `WidgetRuntimeState::qlistSharedNull()` in the **expected** session/allocation root. Verify the root symbol's QtCore digest and RVA `0x2f0a50`. If any guard fails, refuse without overwriting the private address point or advancing the cursor. Do not replay the earlier `+1` delta.

On admission, overwrite only `SparseQObjectPrivate::addressPointRva` from `0x1a9a30` to `0x1a9d10` on that same object, with an explicit once-only phase such as `BeforeQStringSharedNull` at RVA `0x1311a2`. Repeated `createTaskBase` calls with either key must retain the pending node, private object, root binding and delta, and the new address point. The outer public QObject has not returned: task `+8` and private `+8` backlinks remain unavailable; TaskInfo, session task, factory maps, thread/QObject counters and later QString/QVector/QList references remain absent. Adapter-scope release is not evidence of native destructor or unwind behavior.

## Focused acceptance checks

1. First repair and test CP164 foreign-root admission. Pair one allocation store with root A, present root B through a factory, and show the private binding, root deltas and cursor stay at the pre-QList cut. Verify the actual `ConversionSession` pair remains admitted.
2. On the qualified CP164 frame, capture pending and private identities, root symbol identity, and delta. Advance once; require the same identities and root delta, private address point RVA `0x1a9d10`, and cursor before `0x1311a2`.
3. Repeat same-key and different-key requests. Check those values after **each** refusal, with one pending prototype, no `SessionTask`, empty factory maps and false QObject completion/backlinks. An absent, expired, mismatched or uncommitted root binding must refuse without the address-point write.
4. Run the focused `doe_task_factory` and root-owner tests, then the normal source-stable checkpoint build/regression gate. This static pass ran no build, test, original-process comparison, GUI control, or live REA session.

## Receipts and limits

The byte slice and binary digests above were rechecked against the local retained PE files. Supporting file SHA-256 receipts (paths relative to this converter checkout):

| File | SHA-256 |
|---|---|
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-prototype-bytes.json` | `b38ce1278e3ef032cb2f1a8e125c22c73357bc176dbc3e46f3a411e320f381c3` |
| `analysis/wave3-closure-next-close-clear-selection-empty-hash-leaves-static.json` (PE export map) | `17fce2e9c1d96c6f9497659be2a4485b161a65ba0efb0bb05f5f467c22739cd6` |
| `analysis/resume164-connector-implementation.md` | `164ae7790e97e5e24e61428a92e8672dc0d8c2d7447904a003e72435519b51a9` |
| `analysis/resume164-connector-source-review.md` | `c01792e2b081ac7b1af308fab90df5a7165e76abaf16c8e32a6bf5606000272e` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `f4a6d985a7f68864348087f7cdeb3dde1407dcde57da859b61df6246e99f47ba` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `b9da4de9994c89abffc5ba7d2565c77fff54d0b60aa1c7ff92c0f6167ab2ed3a` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `88406917fe165126567c84b0f4af586c36940ff65d22ef89f1a4b150f4f6f562` |
| `source/apps/moproj2mst/src/engine/view_manager.cpp` | `d1ac4321eef560232771f175c1d2bbdc8e7d7fa18a51269c4c060d950d92c382` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `3accb5c22e9c4a3aa80613476931629957b8797075846b303db13c6199927321` |

No absolute QList count, actual constructor occurrence, null-allocation outcome, reference release, or TaskInfo owner is established by this cut. This pass changed only this analysis note.
