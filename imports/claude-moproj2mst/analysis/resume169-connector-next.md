# CP169 connector: third private zero field at `+0x50`

## Decision and predecessor

The smallest next owner-bound normal-path instruction cut is **QtCore RVA `[0x1311bf, 0x1311c3)`**, end exclusive. Its sole instruction writes the already-zero `RDI` to eight bytes of the retained QObjectPrivate allocation at offset `+0x50`. Stop before the independent `+0x58` store at `0x1311c3`. This cut changes one field observation and the cursor only; it has no shared-reference, container, backlink, or QObject-completion effect.

This is conditional source guidance. During this read-only pass, CP168's `private48` optional, `BeforePrivateZero50` phase, and guarded store appeared in the checkout after the initial source read. That supplies the source seam, but I did not verify a source-stable CP168 build, focused tests, checkpoint regression, or original comparison. CP168 acceptance is the implementation prerequisite. Begin CP169 only from its accepted pending Report/private frame at `BeforePrivateZero50` / RVA `0x1311bf`, with `+0x40` and `+0x48` each observed as zero.

## Original bytes and qualification

The local shipped `evidence/original-binaries/QtCore4_64.dll` has SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and preferred base `0x67000000`. Its `.text` maps RVA `0x1000` to raw offset `0x400`. The retained disassembly sets `RBX` from incoming `RCX` at RVA `0x131178` and zeros full `RDI` with `xor edi,edi` at `0x1311b5`; none of the intervening instructions changes these registers. The earlier selected TaskBase/public QObject, non-null `0x88`-byte private allocation, and same pending owner are qualified in the CP167/CP168 notes. This range is a displayed exception-directory unwind region; it does not prove all logical paths or a live occurrence.

| Preferred VA / RVA | PE bytes | Selected effect |
|---|---|---|
| `0x671311b5` / `0x1311b5` | `33 ff` | Establish `RDI = 0`. |
| `0x671311bb` / `0x1311bb` | `48 89 7b 48` | CP168 writes private `+0x48`; required predecessor. |
| `0x671311bf` / `0x1311bf` | `48 89 7b 50` | This cut: write eight zero bytes to private `+0x50`. |
| `0x671311c3` / `0x1311c3` | `48 89 7b 58` | Next independent write; outside this cut. |
| `0x671311c7` / `0x1311c7` | `48 8d 05 ea f9 1b 00` | Later QVector shared-null load; outside this cut. |

The four selected PE bytes are `48897b50` at raw offset `0x1305bf`; their SHA-256 is `cf552273f68b8999bd6e337d0904f9350f04f877e2b99701ab3f5394fa5deff2`. I checked those bytes directly in the local DLL. Use module SHA-256 and RVAs as persistent qualifications; the VAs are preferred-image addresses. The static bytes suffice for this one selected normal-path store, without asserting a numeric Qt shared-null count, exceptional behavior, or native release.

## Owner-bound source contract

Add `std::optional<std::uint64_t> private50` to `PendingTaskPrototype::SparseQObjectPrivate`, initially disengaged. A disengaged optional means native allocated bytes are still unknown; do not default them to an observed zero. The pending Report retained by `TaskAllocationStore` continues to own this same sparse private object.

At exact phase `BeforePrivateZero50`, require the retained pending Report and unchanged private identity, native allocation sizes `0xc0`/`0x88`, written task/private address points `0x23cac8`/`0x1a9d10`, `private40 == 0`, `private48 == 0`, and `private50` unobserved. Require incomplete public QObject/base state and unavailable task/private backlinks. Revalidate the live factory root as the allocation store's exact associated `WidgetRuntimeState` pointee and control block through `ownsWidgetRuntime`. Private `+0x18` QList and `+0x30` QString nonowning bindings must resolve to that root's pointer-identical qualified records, with QtCore hash, symbol RVAs `0x2f0a50`/`0x2f0b30`, and both reference-commit markers true. The existing symbolic deltas are retained observations, not new events; do not require absolute counts or replay either increment. Refuse malformed phase, owner, identity, bindings, marker, predecessor fields, or completion state before mutation.

On admission, set only `private50 = std::uint64_t{0}` and advance once to a distinct `BeforePrivateZero58` phase at RVA `0x1311c3`. Keep `private40`/`private48` zero, and leave `+0x58`, QVector and second QList fields, and all other unknown bytes unobserved. Preserve pending/private/root identities, deltas, maps, task/prototype counts, and completion flags. A host register model is unnecessary: the qualified predecessor established `RDI=0`, and no intervening instruction changes it.

## Focused acceptance and missing witness

1. From an accepted CP168 frame, capture pending/private/root identities, both shared-null identities and symbolic deltas, zero-field optionals, phase, counts, and completion/backlink flags. One call must produce only `private50 == 0` and `BeforePrivateZero58`; `+0x58` remains unknown and every captured identity/delta/count/flag stays unchanged.
2. Same-key and different-key reentry must each refuse at `0x1311c3` without replaying a zero store or prior reference increment, growing prototype/task/cache state, or completing QObject.
3. Before the store, inject absent/expired/foreign or aliasing-foreign root, wrong QList/QString binding or symbol qualification, uncommitted markers, wrong address point or private identity, missing/nonzero `private40` or `private48`, prematurely observed `private50`, and wrong phase. Each refusal must leave the retained field observations, phase, bindings, deltas, and counts unchanged. Include the real `ConversionSession` owner pairing as a positive case.
4. Run focused `doe_task_factory` and root-owner tests, then the source-stable checkpoint build/regression and original-comparison gates. This read-only pass ran no tests, build, or comparison.

No additional live witness is needed to identify this one selected store. **The missing acceptance witness is CP168's source-stable gate result in this checkout**, not an additional byte interpretation. A live constructor occurrence, exceptional branch, numeric absolute Qt count, full QObject initialization, native unwind, and release remain outside this cut.

## SHA-256 receipts and scope

Paths are relative to this converter checkout. Source receipts identify the inspected, concurrently moving snapshot, not a candidate freeze.

| File | SHA-256 |
|---|---|
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/resume168-connector-next.md` | `f94c4ca9ce70b2c34358956640a28e311c157e66512c9e15adc9c3306beac9b8` |
| `analysis/resume168-connector-plan-review.md` | `bc8cd6c1ac17ca322561e157772626f024e0f1e0a6066853753849ead47bc37d` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `156a52a6f4d14f92497400a10f9448f143adb1b99a4b0904ed1cc3da96a04f1f` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `1878f797d87810bcb347eb93eded5db4ba6bc1655211329a7b059b2576e6aa28` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `a33b4b05e059b7313efb196fb34967c20196399b87226c4ff431c1c2106c1d88` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `05ae1570fd55c65464fdc378c2597b1fe915b0101c971414dd9af858ee3cb48b` |

This pass changed only this analysis note. It made no source, tests, shared plan/state, build, deployment, GUI-control, original-process, or live REA change. No GUI-control session was acquired.
