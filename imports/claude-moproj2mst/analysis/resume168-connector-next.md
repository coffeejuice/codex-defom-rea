# CP168 connector: next private zero store at `+0x48`

## Decision and predecessor

The smallest next owner-bound normal-path instruction cut is QtCore RVA **`[0x1311bb, 0x1311bf)`**, end exclusive. Its single instruction stores the already-zero `RDI` into eight bytes of the retained QObjectPrivate allocation at `+0x48`. Stop before the independent store to `+0x50` at `0x1311bf`. This cut has no reference-count, container, backlink, or QObject-completion effect.

This is **conditional source guidance**, not a claim that the current source has reached the cut. The inspected `doe_task_runtime.hpp` has no `+0x40` observation or `BeforePrivateZero48` phase, and `task_factory.cpp` still stops at `BeforePrivateZeroFields` / `0x1311b5`. CP167 must first publish one known zero at `+0x40`, retain the same pending Report/private owner, reach `BeforePrivateZero48` / `0x1311bb`, and pass its focused and checkpoint gates. The CP167 plan/review are the predecessor contract; their static byte interpretation alone is not implementation acceptance.

## Original bytes and qualification

The shipped `QtCore4_64.dll` SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred image base `0x67000000`. Its `.text` maps RVA `0x1000` to raw offset `0x400`. The retained disassembly shows `RBX` loaded from incoming `RCX` at RVA `0x131178`; CP167's `xor edi,edi` at `0x1311b5` makes full `RDI` zero, and no intervening instruction changes either register before this store. Thus the selected normal path writes to that same private allocation. This displayed range is an exception-directory unwind region, not proof of every logical path, a live occurrence, or release behavior.

| Preferred VA / RVA | PE bytes | Selected effect |
|---|---|---|
| `0x671311b5` / `0x1311b5` | `33 ff` | CP167 establishes `RDI = 0`. |
| `0x671311b7` / `0x1311b7` | `48 89 7b 40` | CP167 writes private `+0x40`; required predecessor. |
| `0x671311bb` / `0x1311bb` | `48 89 7b 48` | This cut: write eight zero bytes to private `+0x48`. |
| `0x671311bf` / `0x1311bf` | `48 89 7b 50` | Next independent write; outside this cut. |

The four cut bytes at raw PE offset `0x1305bb` are `48897b48`, SHA-256 `04bacf6dbf771d30ffee085344c3c84502d469218c7355ac1e1c0e7f1de450c0`. I checked the local PE section table and bytes directly. VAs above are preferred-image addresses; persistent source qualifications should use the DLL hash and RVAs. The earlier selected TaskBase/public QObject path and non-null `0x88`-byte private allocation are qualified in the cited CP167 evidence; this pass did not observe another constructor occurrence.

## Owner-bound source contract

Add an optional `std::uint64_t` observation for native private `+0x48` to `PendingTaskPrototype::SparseQObjectPrivate`. Disengaged means allocated bytes remain unknown; do not default this field to known zero. The pending Report retained by `TaskAllocationStore` owns the same sparse private object throughout. On entry, require the exact `BeforePrivateZero48` phase, pending Report and unchanged private identity, native allocation sizes `0xc0`/`0x88`, task/private address-point RVAs `0x23cac8`/`0x1a9d10`, written address-point markers, `+0x40` observed as zero, and `+0x48` unobserved. Require false QObject/base completion and unavailable task/private backlinks.

Retain CP167's same-root ownership guard: the factory's live `WidgetRuntimeState` must be the allocation store's exact associated pointee (`ownsWidgetRuntime` checks both control block and `.get()`), with live private `+0x18` QList and `+0x30` QString bindings pointer-identical to that root's qualified records, and both reference-commit markers already true. Check symbol RVAs `0x2f0a50` and `0x2f0b30` and the QtCore module hash. The deltas are prior committed observations, not events to repeat here; their absolute values need not equal one if the same process root has other qualified increments. Refuse malformed phase/owner/binding/marker/field state before mutation.

After admission, set only private `+0x48 = std::uint64_t{0}` and advance once to a distinct `BeforePrivateZero50` phase / RVA `0x1311bf`. Keep `+0x40` known zero; leave `+0x50`, `+0x58`, later QVector/second QList fields, and all other unknown bytes unobserved. No host register object is needed: the qualified predecessor established `RDI=0`, and the retained instruction sequence has no intervening write to it. Do not modify root deltas, identities, factory maps, task count, or completion flags.

## Focused acceptance and missing witness

1. Start from an accepted CP167 frame. Capture pending/private/root pointers, both symbolic deltas, zero-field optionals, phase, and counts. Advance once and require only `+0x48` to become known zero, phase `BeforePrivateZero50`, and every captured identity/delta/count unchanged. `+0x50` and `+0x58` remain unknown.
2. Try same-key and different-key reentry. After each refusal inspect the retained frame: no replayed write or delta, no prototype/task/cache growth, and no QObject completion or backlinks.
3. Before the store, inject absent/expired/foreign or aliasing-foreign root, wrong QList/QString binding or symbol qualification, uncommitted markers, wrong address point, missing/nonzero `+0x40`, prematurely observed `+0x48`, and wrong phase. Each refusal must preserve field observations, phase, bindings, and deltas. Include the real `ConversionSession` pairing as a positive owner case.
4. Run focused `doe_task_factory` and root-owner tests, then the source-stable checkpoint build/regression and original-comparison gates. This read-only pass ran no tests, build, or comparison.

No additional live witness is needed for this one selected normal-path write: local instruction bytes and the CP167 register/private-owner predecessor establish it. **The missing acceptance witness is CP167 implementation and its gates in this checkout snapshot.** A live constructor occurrence, exceptional path, numeric absolute Qt shared-null count, full QObject initialization, and native unwind/release remain outside this cut.

## SHA-256 receipts and scope

Paths are relative to this converter checkout. These receipts identify the files inspected, not a frozen candidate.

| File | SHA-256 |
|---|---|
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/resume167-connector-next.md` | `2effa7a43b542ada45f4a650d3bd2d3f25cff622eeba8322a70f050c636b2daa` |
| `analysis/resume167-connector-plan-review.md` | `d033593d4bc7dea3312e76af1026296f7befbea925fa6ec722f27bae4763f5e5` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `885590d3fed518c3599a9d38d1212e99c1868aa89d0684b1517fb51d9725d900` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `b8d56166ae963390213d8348dcc01f9085500416378bd802da198c4e8f0fd37a` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `a33b4b05e059b7313efb196fb34967c20196399b87226c4ff431c1c2106c1d88` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `9a0c963de03209f070aafc4e3c5b5bcc66b231be889c6f574a6f3d11f9c10bd7` |

This pass changed only this analysis note. It made no source, test, shared plan/state, build, deployment, GUI-control, original-process, or live REA change.
