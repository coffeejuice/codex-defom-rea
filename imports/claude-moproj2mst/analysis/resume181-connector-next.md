# CP181 connector: private `+0x28` zero; separate the flags read

## Decision

The smallest source-ready cut after CP180's accepted five-store batch is QtCore RVA **`[0x131220,0x131224)`**, end exclusive: `mov qword ptr [rbx+28h],rdi`. On the selected non-null, equal-version QObject path, `RBX` still identifies the same retained `0x88`-byte private allocation and `RDI=0`. Record an eight-byte zero at private `+0x28..+0x2f`, then stop at **RVA `0x131224`** before `mov eax,[rbx+20h]`.

Do **not** batch this store with the following flags read/modify/write under the current sparse state. The four following instructions read the prior dword at private `+0x20`, transform it, and write it back at `+0x20`; the retained constructor path provides no prior value for that dword, and CP180 source has no `private20` observation. A concrete `uint32_t` result cannot be derived from the binary alone. A later separately reviewed cut may represent the original value symbolically and preserve the exact transformation. Do not substitute zero or infer allocator initialization.

The inspected CP180 source stops at `BeforePrivateZero28` / RVA `0x131220`. `analysis/resume180-batch-build.json` records successful configure, focused tests and build for source-tree SHA-256 `83d71f4d36d7e5c5bd98e9da57fa5176f67d08b195865c4e5ffaa6ee2a3e7ea3`; the parent reports a source-stable full-suite pass. This read-only pass did not run the gates or an original comparison.

## Exact PE sequence and register basis

Shipped `QtCore4_64.dll` SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred base `0x67000000`. PE `.text` maps RVA `0x1000` to raw offset `0x400`, hence raw offset is RVA minus `0xc00` here. The selected four bytes at raw `0x130620` are **`48 89 7b 28`**, SHA-256 **`004d4c4c5e01ae7e597c0f9e23d125455cb71368395152f5095c20414a587d17`**.

| Preferred VA / RVA interval | PE bytes | Effect |
|---|---|---|
| `0x67131178` / `0x131178` | `48 8b d9` | Earlier `RBX =` incoming private pointer. |
| `0x671311b5` / `0x1311b5` | `33 ff` | Earlier `xor edi,edi` sets full `RDI=0`. |
| `0x67131220..0x67131223` / `[0x131220,0x131224)` | `48 89 7b 28` | **This cut:** private `+0x28` qword zero. |
| `0x67131224..0x67131226` / `[0x131224,0x131227)` | `8b 43 20` | Reads unknown prior private `+0x20` dword into EAX. |
| `0x67131227..0x6713122b` / `[0x131227,0x13122c)` | `25 f0 fc ff ff` | `EAX &= 0xfffffcf0`. |
| `0x6713122c..0x6713122e` / `[0x13122c,0x13122f)` | `83 c8 60` | `EAX |= 0x60`. |
| `0x6713122f..0x67131231` / `[0x13122f,0x131232)` | `89 43 20` | Writes transformed dword at private `+0x20`. |
| `0x67131232` / `0x131232` | `48 8b c3` | Later private pointer return preparation; outside this decision. |

The full following RMW bytes `[0x131224,0x131232)` are `8b432025f0fcffff83c860894320`, SHA-256 `841574798a97e10cba0dcfe2aa6864bfb1735a61a94c13d4ba6c13b21ef6fb45`. The combined store-plus-RMW bytes `[0x131220,0x131232)` are `48897b288b432025f0fcffff83c860894320`, SHA-256 `3baf3f074492ac37c6a20e48343e385426e52fd52f020a76d06ad696476b396c`. These hashes identify bytes, not an accepted combined source cut.

The retained private-initializer disassembly writes no `RBX` or `RDI` after their listed definitions on the selected equal branch, and contains no intervening call. CP178's version comparison jumps over the only mismatch-path call. All CP180 stores and this `+0x28` destination fit the `0x88` allocation. The public QObject constructor's task `+0x08` pointer publication and private backlink occur only after initializer return; neither is in this cut.

## Owner-bound source contract for the `+0x28` store

Add `std::optional<std::uint64_t> private28` to the retained `SparseQObjectPrivate`, initially disengaged. At `BeforePrivateZero28` / `0x131220`, require the same retained pending Report ordinal, exact allocation-time private pointee, native task/private sizes `0xc0`/`0x88`, and written address points `0x23cac8`/`0x1a9d10`. Revalidate the live factory/root association by both pointee and control-block identity; first/second QList bindings to the same qualified root record at RVA `0x2f0a50` with both markers, and QString/QVector bindings to qualified root records at RVAs `0x2f0b30`/`0x2f0bb8` with markers. Require the qualified QtCore version argument `{0x40804, load RVA 0x13140f, call RVA 0x131417}` and the accepted equal-branch phase. Require all prior zero observations: qword `+0x08,+0x10,+0x38,+0x40,+0x48,+0x50,+0x58,+0x78,+0x80`, dword `+0x24,+0x60,+0x64`; require `private28` unobserved. QObject/base must remain incomplete and task/private backlinks unavailable. Refuse any malformed guard before mutation.

On admission, set only `private28 = uint64_t{0}` and advance to a phase such as `BeforePrivateFlags20Read` at RVA `0x131224`. Preserve all earlier sparse fields, private/root identities, bindings and markers, three symbolic root reference deltas and baselines, factory/task/cache maps and counts, and completion/backlink flags. Leave private `+0x20` **unknown**. A stopped source path is a reconstruction boundary, not a native throw between these two adjacent instructions.

## Why the flags operation is separate

The exact native relation is `out20 = (in20 & 0xfffffcf0) | 0x60`, where `in20` is the pre-read 32-bit memory value. The mask clears bits `0..3` and `8..9` (`0x0000030f`), the OR sets bits `5..6` (`0x00000060`), and all remaining bits (`0xfffffc90`) retain their unknown input values. Thus the output's known-one mask is `0x60` and known-zero mask is `0x30f`, but the full output value is not known.

The selected initializer region contains no earlier store to private `+0x20`; the public constructor calls the private initializer immediately after allocation on this path. CP180's zero at `+0x24` is the adjacent dword and supplies none of the four bytes read at `+0x20`. The retained evidence does not establish what bytes the allocator returned at `+0x20`. The current sparse model also does not retain an input symbol, known-bit mask or observed value there. A later cut can be source-ready if it introduces an explicit symbolic `in20` identity (or independently qualifies an observed input), records the derived output relation without manufacturing a concrete value, and defines atomic versus intermediate read/EAX phases and refusal behavior. Its review should cover fresh unknown input and reentry. The present proposal stops before the read because no such representation or tests are yet established.

## Short focused tests

1. From accepted CP180 final phase, capture the full retained frame and root counters. One continuation observes `private28=0` and `BeforePrivateFlags20Read` / `0x131224`; all earlier fields and qualified identities remain fixed, and private `+0x20` is still unknown. Include actual `ConversionSession` owner pairing.
2. Same-key and different-key reentry at `0x131224` refuse without replaying the store or changing deltas, maps, counts, completion or backlinks.
3. At entry, corrupt each owner/ordinal/private identity or allocation size, address point, version provenance, root/binding/marker, prior zero or phase; pre-observe `private28` as both zero and nonzero; set completion/backlink early. Each refusal preserves every field, phase, binding and root counter. Exercise the CP180 intermediate-prefix test hook separately so this cut cannot run before all five predecessor stores.
4. Assert no `private20` value, flags result, EAX state, QObject completion or public backlink is published. Run focused `doe_task_factory`, root-owner and Session tests, then source-stable checkpoint and original-comparison gates. This report ran none of them.

## Qualifications and SHA-256 receipts

The retained disassembly is an exception-directory unwind region, not a proven full logical constructor. This establishes the selected non-null, equal-version static path only; no live occurrence, allocator contents, mismatch caller, cleanup or completed QObject is inferred. Preferred VAs are display coordinates; module digest and RVAs qualify source. Paths below are relative to this converter checkout. Source hashes mark the inspected CP180 snapshot, not a CP181 freeze.

| File | SHA-256 |
|---|---|
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-671313d0.asm` | `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9` |
| `analysis/resume180-connector-batching-review.md` | `deac21d81293a4c2e927931ab9aa7cdf932d3344d594b9a35377711ab5c5a1cf` |
| `analysis/resume180-connector-implementation.md` | `b1bd1515a1082551ba70a461a0b120c6f2adb7d3b76591b365f206bcb315f8b4` |
| `analysis/resume180-connector-source-review.md` | `03e0ab69af6f576596e7f427117cb5a7feae4d2ab0683e9000f5089e9a446a9c` |
| `analysis/resume180-batch-build.json` | `5c859aeb39c788cf98e345681cdb75f2d9e1462dd28e78f936763f25e0216a91` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `2b5750b79bac2883c6d8cded5c3661b50e009c8639f78f2eb81fc8cd9ea1cd6a` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `f507339e99751ceb2cbc2d1bb79e33927ba5e1f3ccc223fe38b67db488fc368d` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `921ad7e08473df867ca20dd3540b665cab78342159d169a62bb27edbcb0614fe` |

This pass changed only this report. It made no source, tests, shared plan/state, build, deployment, GUI-control, original-process or live REA change. No GUI-control session was acquired.
