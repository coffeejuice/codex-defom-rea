# CP182 connector: symbolic private `+0x20` flags read/modify/write

## Decision

The selected QtCore straight-line interval **`[0x131224,0x131232)`**, end exclusive, is source-ready as a **symbolic four-instruction cut** after accepted CP181. The first instruction loads an unknown 32-bit value from private `+0x20`; the next two transform the same EAX value; the fourth writes it back. Represent that one read as a stable symbol `B` tied to the retained private allocation and RVA `0x131224`. The final memory relation is **`private20 = (B & 0xfffffcf0) | 0x60`**. No exact numeric `B` or full numeric result is established. Stop at RVA **`0x131232`**, before `mov rax,rbx` and initializer return handling.

Implementation is justified only with the symbolic origin and expression, exact per-instruction cursor and guard described below. A plain `optional<uint32_t>` set to `0x60`, or a model that keeps only known-bit masks and loses the dependency on `B`, would invent data. A concrete-only design remains blocked until an independently qualified input value exists.

The inspected CP181 source stops at `BeforePrivateFlags20Read` / RVA `0x131224` and contains no `private20` value. `analysis/resume181-dual-build.json` recorded an earlier focused failure; the later `analysis/resume181-reentryfix-build.json` records zero exits for configure, repeated focused tests and build at source-tree SHA-256 `ddfbb8a08f0a9299ed05aaae442c6b9bbd1f2872a40c9cd3adcaae2b14a4eaf8`. The parent reports CP181 source-stable full-suite acceptance. This pass reran no gate or original comparison.

## Exact PE bytes and register/memory meaning

Shipped `QtCore4_64.dll` SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; preferred base `0x67000000`. Its `.text` RVA `0x1000` maps to raw offset `0x400`, so the interval begins at PE raw `0x130624`. Full bytes are **`8b432025f0fcffff83c860894320`**, SHA-256 **`841574798a97e10cba0dcfe2aa6864bfb1735a61a94c13d4ba6c13b21ef6fb45`**.

| Preferred VA / RVA interval | Bytes | State after instruction |
|---|---|---|
| `0x67131224..26` / `[0x131224,0x131227)` | `8b 43 20` | `EAX=B`, one 32-bit read from private `+0x20`; memory still unknown. The 32-bit destination zero-extends into RAX. |
| `0x67131227..2b` / `[0x131227,0x13122c)` | `25 f0 fc ff ff` | `EAX=B & 0xfffffcf0`; memory unchanged. |
| `0x6713122c..2e` / `[0x13122c,0x13122f)` | `83 c8 60` | `EAX=(B & 0xfffffcf0) | 0x60`; memory unchanged. |
| `0x6713122f..31` / `[0x13122f,0x131232)` | `89 43 20` | Store that EAX expression as a dword at private `+0x20`; EAX retains it. |
| `0x67131232..34` / `[0x131232,0x131235)` | `48 8b c3` | Later `RAX=RBX` private pointer; outside CP182. |

The four individual byte SHA-256 values, in order, are `387e4a44f5b080dde8628135fc944390bbf950c68ee197363a3b7e8d1abd3806`, `e0c8c1ee91c5a89edf4e82e9c633ce1b9900b8fbe64851bbc541cc6666ae9ede`, `96dccd5b45324dbda00c012f571ae71dcee68b1e9a7185e68469b6e613b9b889`, and `34e978765ad856d760a3bb25ae4d4aa6e4506bc582675562bda8667940350fc4`. `RBX` has held the same private pointer since RVA `0x131178` on the qualified equal-version path. There is no branch, call or write to RBX inside this interval. The preceding CP180 zero at private `+0x24` is the adjacent dword; it does not supply any of the four read bytes at `+0x20`. The retained initializer has no earlier `+0x20` store and the public constructor calls it immediately after private allocation. Allocator contents are not established.

## Symbolic value and known-bit contract

Use a value with a fixed origin such as `{private allocation identity, offset 0x20, width 32, readRva 0x131224}` and a transform stage `Read`, `And`, `Or`. It can be a small POD expression/enum rather than a general symbolic engine. The origin must be retained across continuations; do not generate a new unknown on reentry. Keep the original `B` dependency in both the EAX expression and the final private memory expression. A separate known-bit summary is useful for assertions, but it is insufficient as the sole state.

For completely unknown `B`, the stage summaries are:

| Cursor after step | EAX expression | Known one mask | Known zero mask | Unknown mask | Private `+0x20` |
|---|---|---:|---:|---:|---|
| `0x131227` | `B` | `0` | `0` | `0xffffffff` | Unknown, unmodified |
| `0x13122c` | `B & 0xfffffcf0` | `0` | `0x0000030f` | `0xfffffcf0` | Unknown, unmodified |
| `0x13122f` | `(B & 0xfffffcf0) | 0x60` | `0x00000060` | `0x0000030f` | `0xfffffc90` | Unknown, unmodified |
| `0x131232` | Same | `0x00000060` | `0x0000030f` | `0xfffffc90` | Same expression stored |

For an independently qualified or test-injected input known-bit pair `(ones, zeros)`, `AND` yields `(ones & 0xfffffcf0, zeros | 0x30f)`; `OR` then yields `((ones & 0xfffffcf0) | 0x60, (zeros | 0x30f) & ~0x60)`, constrained to 32 bits. Known-one and known-zero masks must be disjoint. These formulas preserve any known input bits without treating unknown bits as zero. The store overwrites the dword at `+0x20` only; it does not alter the adjacent accepted `+0x24=0` observation. EFLAGS changes from AND/OR need no published semantic state for the retained tail because no following instruction in this initializer consumes them before return; a future proof must recheck any broader path.

## Owner-bound source state and phases

Starting from CP181's accepted `BeforePrivateFlags20Read` / `0x131224`, retain a symbolic EAX slot and a private `+0x20` memory slot with explicit `unknown/unwritten` versus `symbolic written` states. The read can create `B` in EAX without changing memory. Use distinct phases/cursors: `BeforeFlags20And` / `0x131227`, `BeforeFlags20Or` / `0x13122c`, `BeforeFlags20Write` / `0x13122f`, and `BeforePrivateReturnSetup` / `0x131232`. One normal continuation may traverse all four, with a narrow internal test stop after each instruction. The read and each register transform must retain their result with the cursor; the final write alone changes private memory. Validate the entire current prefix before every next step. If a later prefix refuses, retain earlier accepted steps and their exact cursor; do not roll them back or replay the read.

Before entry or resumed transition, revalidate the same pending Report ordinal and allocation-time private pointee; `0xc0`/`0x88` sizes and written task/private address points `0x23cac8`/`0x1a9d10`; exact factory/allocation root pointee and control-block pairing; first and second same-root QList bindings plus markers at RVA `0x2f0a50`, QString at `0x2f0b30`, QVector at `0x2f0bb8`; qualified QtCore version `{digest, EDX 0x40804, load RVA 0x13140f, call RVA 0x131417}` and equal branch; every predecessor sparse zero through `private28=0`; and incomplete QObject/base with both backlinks unavailable. At entry, private `+0x20` must be unobserved and EAX symbolic slot absent. At each intermediate phase require its exact EAX origin/stage and private memory still unobserved; at `0x131232` require the matching stored expression. Refuse foreign origin, forged masks, premature stored value, bad phase, owner or predecessor before mutation. Preserve root reference deltas/baselines, maps, counts, bindings and all other private observations. The source's unsupported stop is a reconstruction boundary, not a claim that native code throws between these instructions.

## Focused acceptance

1. Default continuation from a genuine CP181/`ConversionSession` frame reaches `0x131232` with only the symbolic read/transform/write state and phase changed. `B` remains unknown; final masks are one `0x60`, zero `0x30f`, unknown `0xfffffc90`. The same symbol is used in EAX and stored memory. No public backlink, QObject completion, task/cache, thread or counter effect appears.
2. Diagnostic stops after each of the four steps verify the exact cursor, EAX expression, masks and unchanged versus written memory. Resume each intermediate state to the same terminal symbolic expression as an uninterrupted run. Same-key and different-key reentry at every stop must not create a second read token or replay a write.
3. Pure expression tests with test-supplied inputs `0`, `0xffffffff` and patterned bits verify `((input & 0xfffffcf0) | 0x60)` and the known-mask formulas. They do not seed production allocator bytes. Check that private `+0x24` remains zero and cannot stand in for `+0x20`.
4. At entry and each intermediate cursor, damage owner/root control block or pointee, Report ordinal/private identity, sizes/address points, each binding/marker, version provenance, predecessor zero, EAX origin/stage/masks, private20 write state or completion/backlink flag. Snapshot full frame and three root deltas before/after each refusal. Preserve earlier partial state. Then run focused factory/root/Session tests and the established source-stable checkpoint and original-comparison gates.

## Qualification and SHA-256 receipts

This is static selected-path evidence from a retained exception-directory unwind region, not proof of a live occurrence, concrete allocation contents, mismatch caller, exception recovery, cleanup, full QObject construction or public task/private backlinks. Preferred VAs are display values; digest and RVAs are the source qualification. The hashes below identify the inspected CP181 snapshot, not a CP182 freeze. Paths are relative to this converter checkout.

| File | SHA-256 |
|---|---|
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-671313d0.asm` | `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9` |
| `analysis/resume181-connector-next.md` | `0f467499a94064401a5ee832c5740550d7459b9413bee8dc78ba495c0ca18af0` |
| `analysis/resume181-connector-implementation.md` | `287cc301584a3c444237204b3856660aa13f700af122db4b33493eb1b7dbe9b3` |
| `analysis/resume181-connector-source-review.md` | `c5c4e64a7b7c5994351174f1df89d6e2dfef8ca894ed47b88883568e7200dcdf` |
| `analysis/resume181-reentryfix-build.json` | `ec1a2eedabf547ce2b988a05c967d29a32e140d10b88cbc1e6e94a0e0c328aff` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `61e3183bd1e58b6dd08b0e5c283c19414175e7ba841365eb179c6e5b77553a75` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `51a7beacefacd9001d45bbc5d42617ed300a5c53e32803938bbba8b42f6d60d6` |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `9c9cbbdcc335705921f5a584c0612c4dc332f2ad3f2aaa1d2c520f0194bd0bd7` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `6ddddc4ef8a69903c3eaa9fe0cf3218c5bb050171137ccd0e7ceb5575e5b64db` |

This pass changed only this report. It made no source, tests, shared plan/state, build, deployment, GUI-control, original-process or live REA change. No GUI-control session was acquired.
