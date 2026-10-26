# CP170 connector: fourth private zero field at `+0x58`

## Decision and prerequisite

The smallest next owner-bound normal-path instruction cut is **QtCore RVA `[0x1311c3, 0x1311c7)`**, end exclusive. Its one instruction stores the already-zero `RDI` into eight bytes at `+0x58` of the *same* retained QObjectPrivate allocation. Stop at `0x1311c7`, before loading the distinct QVector shared-null address. This cut supplies one known field value and a cursor advance; it does not bind or increment a container, publish a backlink, or complete QObject.

This is conditional source guidance. The inspected source contains CP169's `private50` optional, guarded `BeforePrivateZero50` transition, and `BeforePrivateZero58` stop, but it changed during this read-only pass. The inspected factory tests still contained CP168 `0x1311bf` expectations while the source stopped at `0x1311c3`. **A source-stable CP169 focused/checkpoint/original-comparison acceptance receipt is the prerequisite** before implementing CP170. Begin only with its accepted pending Report/private frame at `BeforePrivateZero58` / RVA `0x1311c3`, with `private40`, `private48`, and `private50` each observed as zero. The static byte proof alone does not accept the predecessor implementation.

## Original bytes and qualification

The local shipped `QtCore4_64.dll` SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred base `0x67000000`. Its PE `.text` section maps RVA `0x1000` to raw offset `0x400`, so this cut is at raw offset `0x1305c3`. The selected application `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The retained constructor chain has TaskBase call public QObject with null parent, the public constructor supply a non-null `0x88` private allocation on this selected path, and private initializer `0x67131160` copy that private pointer from incoming `RCX` into `RBX` at RVA `0x131178`. `xor edi,edi` at RVA `0x1311b5` clears all of `RDI`; the contiguous stores through this cut contain no intervening change to `RBX` or `RDI`.

| Preferred VA / RVA | PE bytes | Effect on selected normal path |
|---|---|---|
| `0x671311b5` / `0x1311b5` | `33 ff` | Full `RDI = 0`. |
| `0x671311bf` / `0x1311bf` | `48 89 7b 50` | CP169 predecessor writes private `+0x50`. |
| `0x671311c3` / `0x1311c3` | `48 89 7b 58` | This cut writes private `+0x58` as a 64-bit zero. |
| `0x671311c7` / `0x1311c7` | `48 8d 05 ea f9 1b 00` | Later load of `QtCore+0x2f0bb8`; outside this cut. |
| `0x671311ce` / `0x1311ce` | `48 89 43 68` | Later private `+0x68` store; outside this cut. |
| `0x671311d2` / `0x1311d2` | `f0 83 05 de f9 1b 00 01` | Later shared reference increment; outside this cut. |

The four selected file bytes are `48897b58`, SHA-256 `3c5054b0b934672c2584fc9bd7a3c9311fbc7117ed45ff53e0074c0429f9222e`. Both the PE mapping and bytes were checked directly. These VAs are preferred-image addresses; the module hash and RVAs qualify persistent observations. The retained disassembly covers an exception-directory unwind region. It does not establish every logical or exceptional path, a live constructor occurrence, or native cleanup.

## Owner-bound source contract

Add `std::optional<std::uint64_t> private58` to `PendingTaskPrototype::SparseQObjectPrivate`, initially disengaged. Disengaged means the allocated native bytes are still unknown. The existing pending Report in `TaskAllocationStore` owns this same private state, and the `BeforePrivateZero58` phase is an instruction cursor, not a completed QObject.

Before mutation, validate the exact CP169 frame and owner: the retained Report identity and private pointee are unchanged; task/private native sizes remain `0xc0`/`0x88`; task/private address points are written and qualified as QtCore RVAs `0x23cac8`/`0x1a9d10`; the factory's live root is the allocation store's associated `WidgetRuntimeState` with both control-block and pointee identity (`ownsWidgetRuntime`). Private `+0x18` QList and `+0x30` QString bindings must lock to that root's pointer-identical qualified records (`QtCore` hash, symbol RVAs `0x2f0a50`/`0x2f0b30`), with both reference-commit markers true. Require `private40 == private48 == private50 == 0`, `private58` disengaged, false QObject/base completion, and unavailable task/private backlinks. These root deltas are already committed observations; do not demand an absolute count or replay an increment. Reject any malformed owner, phase, predecessor, identity, binding, marker, or completion state *before* changing the field or cursor.

On admission, set only `private58 = std::uint64_t{0}` and move once to a distinct `BeforeQVectorSharedNull` phase / RVA `0x1311c7`. Keep the three predecessor zero observations. Leave private `+0x68`, `+0x70`, later second QList, other uninitialized bytes, QVector identity/reference effect, backlinks, thread selection, QObject counter, TaskInfo, task, prototype completion, and factory caches unavailable. Preserve pending/private/root identities, symbolic deltas, counts, and maps. No host register emulator is required for this one zero store because the qualified predecessor established unchanged `RDI = 0` and `RBX = private`.

## Focused acceptance and missing witness

1. From a **source-stable accepted CP169** frame, capture pending/private/root pointees, both root shared-null object identities and deltas, four field optionals, phase, native sizes/address points, counts, maps, and completion/backlink flags. One continuation must yield only `private58 == 0` and `BeforeQVectorSharedNull` / `0x1311c7`; all captured identities, deltas, counts, maps, and flags remain unchanged. No QVector binding or delta is present.
2. Same-key and different-key reentry after the stop must refuse without replaying any zero store or shared increment and without prototype/task/cache growth or QObject completion.
3. Inject absent, expired, foreign, and aliasing-foreign roots; wrong QList/QString binding or symbol qualification; uncommitted markers; wrong address point/private identity; missing or nonzero `private40`, `private48`, or `private50`; prematurely observed `private58`; wrong phase; and prematurely completed/backlinked state. Each refusal must preserve field observations, phase, bindings, deltas, identities, counts, and maps. Include the real `ConversionSession` owner pairing as a positive case.
4. Run focused `doe_task_factory` and root-owner tests, then the established source-stable checkpoint build/regression and original-comparison gates. This read-only pass ran no tests, build, comparison, GUI, or live REA operation.

No additional live witness is needed to interpret this selected normal-path store. The missing **acceptance** witness is CP169's stable source and gates in this checkout. A later QVector binding/atomic, numeric absolute Qt count, full QObject construction, and native failure/unwind require separate evidence and implementation cuts.

## SHA-256 receipts and scope

Paths are relative to this converter checkout. Source hashes identify an inspected, concurrently changing snapshot, not a frozen candidate.

| File | SHA-256 |
|---|---|
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/resume169-connector-next.md` | `f1b89fa2ec43d998b4141c96bf69bd5df074c80548706eb24eb5a2ca87a7ae90` |
| `analysis/resume169-connector-plan-review.md` | `d02e7f7c8ec400ab641d087f2f2bfd197ba54dc80ba07b686929f16a1886f482` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `622e3cae933cc897b5eb24c0a83ed5584de06170ad8dce8752c1503f2e8d9a7a` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `231bf736b2d8842063974aefbdfe8f4818832dd2f0297332be55342bed05494d` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `a33b4b05e059b7313efb196fb34967c20196399b87226c4ff431c1c2106c1d88` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `1ef0cb811cfeb2fe405a3a8688bf3fa7a05d39d51638de348b219e76ff69b3c2` |

This pass changed only this note. It made no source, tests, shared plan/state, build, deployment, original-process, GUI-control, or live REA change. No GUI-control session was acquired.
