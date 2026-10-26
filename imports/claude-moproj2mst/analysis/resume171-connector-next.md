# CP171 connector: QVector shared-null address binding at private `+0x68`

## Decision and predecessor

The smallest **owner-bound persistent-state cut** after accepted CP170 is QtCore RVA **`[0x1311c7, 0x1311d2)`**, end exclusive. The seven-byte `lea` computes the address of the exported `QVectorData::shared_null` object, and the four-byte `mov` stores it in the *same* retained QObjectPrivate allocation at offset `+0x68`. Stop at `0x1311d2`, **before** the distinct `lock add` to the shared object's reference DWORD. The `lea` alone changes only a transient register, so `[0x1311c7,0x1311ce)` is not an owner-bound persistent cut. This cut establishes a pointer binding and cursor advance only; it records no QVector reference increment or absolute count.

This is conditional source guidance. The inspected source contains CP170's `private58` observation and `BeforeQVectorSharedNull` stop, but concurrent edits and this read-only pass do not establish a source-stable CP170 focused/checkpoint/original-comparison receipt. Start from that accepted pending Report/private frame at RVA `0x1311c7`, with `private40`, `private48`, `private50`, and `private58` each observed as zero. A retained PE export and direct export-table parse close the loaded **module/global identity**; the converter's actual process-root QVector record does not exist yet and must be added as part of this cut. The isolated brush-gradient shared-null helper cannot be treated as this root's identity without an explicit owner reconciliation.

## Native bytes and identity qualification

The selected application `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The local shipped `QtCore4_64.dll` SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred base `0x67000000`. Its PE `.text` section maps RVA `0x1000` to file offset `0x400`, placing this cut at raw offset `0x1305c7`. Direct PE export-directory parsing found ordinal 4514, `?shared_null@QVectorData@@2U1@A`, at RVA `0x2f0bb8` (preferred VA `0x672f0bb8`); the retained export map independently agrees. The static initialized 16 bytes there are `01000000000000000000000001000000`, but their first DWORD is **not** a live reference count. Model identity with module hash and RVA, not a loaded preferred VA or a host pointer.

| Preferred VA / RVA | PE bytes | Selected effect |
|---|---|---|
| `0x67131178` / `0x131178` | `48 8b d9` | Earlier private pointer copied from incoming `RCX` to `RBX`. |
| `0x671311c3` / `0x1311c3` | `48 89 7b 58` | Accepted CP170 predecessor wrote private `+0x58`. |
| `0x671311c7` / `0x1311c7` | `48 8d 05 ea f9 1b 00` | `RAX = QtCore+0x2f0bb8`; RIP after instruction is RVA `0x1311ce`, plus displacement `0x1bf9ea`. |
| `0x671311ce` / `0x1311ce` | `48 89 43 68` | Store that address at `[RBX+0x68]`, within the `0x88`-byte private allocation. |
| `0x671311d2` / `0x1311d2` | `f0 83 05 de f9 1b 00 01` | Later atomic `+1` to the same symbol's first DWORD; outside this cut. |
| `0x671311da` / `0x1311da` | `48 89 4b 70` | Later private `+0x70` write; outside this cut. |

The exact 11 bytes are `488d05eaf91b0048894368`, SHA-256 `49ea93d2b8e2eee2ce091ad1564de6d82747ac2053da15f456cc80eaa8bd7c38`. The retained private-initializer disassembly is an exception-directory unwind region, which qualifies this selected normal, non-null allocation path but not all logical paths, a live occurrence, native failure cleanup, or destructor release.

## Source contract

Add one stable, qualified QVector shared-null record to the **allocation store's associated `WidgetRuntimeState`**: QtCore digest above, symbol RVA `0x2f0bb8`, unknown absolute reference count, and a `uint32_t` symbolic delta initially zero. Keep the record pointer-identical for this root's constructor frames. `PendingTaskPrototype::SparseQObjectPrivate` should hold only a nonowning binding representing observed native private `+0x68` and a false reference-committed marker for the later atomic cut. Do not copy the global into the factory or infer a live count from PE data. The existing `BrushGradientStopDataState::makeSharedNullSymbol()` is an isolated helper with a stated runtime-interning prerequisite; if both paths enter one actual process scope later, reconcile them with one canonical symbol owner.

Before any mutation, require the exact pending Report retained by `TaskAllocationStore`, unchanged private identity and native sizes `0xc0`/`0x88`, written task/private address-point RVAs `0x23cac8`/`0x1a9d10`, exact `BeforeQVectorSharedNull` phase, all four predecessor private zero optionals equal to zero, incomplete QObject/base, and unavailable backlinks. Require a live factory root `WidgetRuntimeState` owned by the allocation store by both control-block and pointee identity (`ownsWidgetRuntime`). The private `+0x18` QList and `+0x30` QString bindings must lock to pointer-identical qualified records on that root, at RVAs `0x2f0a50` and `0x2f0b30`, with both prior reference-committed markers true. Require the root's new QVector record to be qualified by the same QtCore hash and `0x2f0bb8`; require private `+0x68` unbound and its reference marker false. Reject malformed owner, identities, bindings, phase, predecessor fields, or completion state before writing either binding or cursor.

On admission, bind private `+0x68` to that exact root QVector record and advance once to a distinct `BeforeQVectorSharedNullIncrement` phase / RVA `0x1311d2`. Preserve all three root symbolic deltas (QVector remains zero), any observed counts, the four zero fields, pending/private/root identities, prototype/task/cache counts and maps, and completion/backlink flags. The subsequent atomic increment is a separate cut. The `lea` and store do not call or change `RCX`, so the later `+0x70` value is not established by this cut.

## Focused acceptance and limits

1. From a **source-stable accepted CP170** frame, capture pending/private/root identities, QList/QString qualified bindings and deltas, the new QVector root identity and zero delta, four zero fields, phase, sizes/address points, counts/maps, and completion/backlink flags. One continuation must bind private `+0x68` to that root QVector object, move to RVA `0x1311d2`, and change nothing else. In particular, its reference marker remains false and no atomic delta is committed.
2. Same-key and different-key reentry after the stop must refuse without rebinding, reference increment, prototype/task/cache growth, or QObject completion. Recheck the retained frame after each refusal.
3. Inject absent, expired, foreign, and aliasing-foreign roots; missing/wrong/malformed QVector record; wrong QList/QString binding or qualification; uncommitted predecessor markers; wrong address point/private identity or allocation size; missing/nonzero predecessor zeros; prematurely bound `+0x68` or committed QVector marker; wrong phase; and prematurely completed/backlinked state. Each refusal must preserve identities, fields, phase, all deltas/counts, and maps. Include the actual `ConversionSession` owner pairing as a positive case and keep isolated brush-gradient symbols unable to satisfy the factory guard.
4. Run focused `doe_task_factory` and root-owner tests, then the normal source-stable checkpoint build/regression and original-comparison gates. This read-only qualification ran none of those. A native absolute count requires a same-call live pre/post observation around RVA `0x1311d2`; the PE initial value and existing caller-supplied ledger examples do not supply it.

## SHA-256 receipts and scope

Paths below are relative to this converter checkout. Source hashes identify an inspected, concurrently changing snapshot, not an implementation freeze.

| File | SHA-256 |
|---|---|
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/wave3-closure-next-close-clear-selection-empty-hash-leaves-static.json` | `17fce2e9c1d96c6f9497659be2a4485b161a65ba0efb0bb05f5f467c22739cd6` |
| `analysis/resume170-connector-next.md` | `fadc6a16fa67cc204dc8f272470ccd99d0d2679a408eb4ff964d5f0d11807958` |
| `analysis/resume170-connector-plan-review.md` | `6e9078a801c82e6cbf246dd46b4ac0921709915f4df53c3d9c58c48dab4af728` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `2aa6dfaeb664511d5987c02e35837ec542f3141d377829a0c2a827e55a09cf9c` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `a3e61c964c57e6733497fd3c395f303081455aa213ca2dc47d3e54d7fb307f77` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `a33b4b05e059b7313efb196fb34967c20196399b87226c4ff431c1c2106c1d88` |
| `source/apps/moproj2mst/src/engine/view_manager.cpp` | `d1ac4321eef560232771f175c1d2bBDC8e7d7fa18a51269c4c060d950d92c382` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `b7fbe5510f0a29fdaa6646dbb202f643341eb85e8865d89e8068d2a62ea46b8f` |
| `source/apps/moproj2mst/include/m2m/engine/brush_gradient_stops.hpp` | `adc6f78674c0706586a8d9dd442f6c42a41ca8adc4f4eb5e0dff3b3e3524c8f2` |
| `source/apps/moproj2mst/src/engine/brush_gradient_stops.cpp` | `f469297380f2bafb762313f9c419b3587ad4593043aae135c6c9ccfb465e1958` |
| `source/apps/moproj2mst/src/engine/report_qobject_ledger.cpp` | `d95b70c3c93585997a81aa4492d2f51e6bf8d26ce4a7880f07fadc9e6851e897` |

This pass changed only this note. It made no source, tests, shared plan/state, build, deployment, GUI-control, original-process, or live REA change. No GUI-control session was acquired.
