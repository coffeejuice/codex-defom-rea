# CP182 connector implementation: symbolic private +0x20 flags

Implemented the qualified QtCore interval `[0x131224,0x131232)`, stopping before `mov rax,rbx` at RVA `0x131232`. The retained unknown dword B is identified by the allocation-time private pointee, offset `0x20`, width 32 and read RVA `0x131224`. Its Read/And/Or stage and original known-bit pair remain in a small trivially copyable expression. Production reads create wholly unknown B; production guards reject test-injected knowledge as well as forged origins, stages or masks.

The four nonthrowing publications preserve the exact cursors `BeforeFlags20And` / `0x131227`, `BeforeFlags20Or` / `0x13122c`, `BeforeFlags20Write` / `0x13122f`, and `BeforePrivateReturnSetup` / `0x131232`. The first three publish EAX expressions only; private20 remains disengaged. Only the fourth writes the matching symbolic expression into private20. Final EAX and memory retain `(B & 0xfffffcf0) | 0x60`, with known-one `0x60`, known-zero `0x30f`, unknown `0xfffffc90`. No full numeric B or flags result is supplied. The adjacent private24 zero cannot seed B. The expression width records the EAX dword operation and native zero extension; no EFLAGS semantic state is published.

Before every instruction and terminal reentry, admission revalidates the retained Report ordinal/pointee, allocation-time private identity, task/private sizes and written address points, factory/allocation root pointee and control-block pairing, committed canonical first QList and QString, canonical committed QVector and optional qualifier, second same-root QList and commit marker, qualified module/version/load/call provenance, all thirteen predecessor sparse zeros through private28, incomplete QObject/base and unavailable public backlinks. Exact current EAX and memory state is compared against the allocation-bound unknown expression at the current stage. Bad later state refuses without rolling back or replaying earlier accepted steps. Before CP180/181 batch writes, premature symbolic slots also refuse.

The existing CP180/181 absolute stop budgets 1..6, default 6, remain intact. A separate private friend diagnostic instruction stop accepts 0..4, default 4. Zero retains the CP181 read-entry prefix and existing diagnostic; each higher value admits one instruction. Invalid stop values refuse before initialization. The hook is internal and does not alter the C ABI or assert that native code throws between these instructions. No backlink, return setup, initializer return, QObject completion, TaskInfo/cache/map, thread or counter effect was added.

## Test source and static preflight

Default factory assertions in the owned test file now use RVA `0x131232` and `BeforePrivateReturnSetup`; deliberate predecessor rewinds clear both symbolic slots. Full frame/private snapshots include EAX and stored symbolic state. The dedicated CP181 test explicitly uses instruction stop 0 and still checks only its single qword zero and phase change.

Eight new Testkit cases cover actual ConversionSession default progression and equality to the prior full frame after removing only CP182 effects; all five entry/intermediate/final stops; same/different/custom-key reentry; stepwise resume to the same uninterrupted frame and allocation-bound symbol; full and partial test-injected known-bit formulas; origin/width/offset/read-RVA/stage/mask/source-mask corruption, missing or premature slots, invalid phases/ordinal, overlapping masks, all predecessor zero missing/nonzero variants at every cursor, refusal retaining intermediate state followed by repair/resume, invalid internal stops, and null private state. The established owner/root/binding/version/completion refusal matrix now includes all five CP182 cursors; the unpaired/foreign/alias/expired-root matrix likewise includes those cursors. Complete frame/construction/private snapshots and all three root reference deltas are checked around refusals.

Static preflight confirms the existing Testkit `main`, supported `CHECK`/`REQUIRE` assertions, no C assert or unsupported REQUIRE_EQ, every quoted include resolves against the existing target include roots, the testkit header/include root exists, and the converter requests C++20. Header static assertions specify symbolic optional types, 32-bit masks, trivial expression copying and nonthrowing expression publications/transforms. These assertions have not been compiled by this worker.

## Independent static PE audit

Read source qualifications in `analysis/resume182-connector-next.md` and the retained initializer/public-constructor disassembly. Read-only PE section parsing confirms `.text` RVA `0x1000` maps to raw `0x400`; selected RVA `0x131224` maps to raw `0x130624`. Canonical QtCore4_64.dll SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Selected 14 bytes `8b432025f0fcffff83c860894320` hash to `841574798a97e10cba0dcfe2aa6864bfb1735a61a94c13d4ba6c13b21ef6fb45`; excluded following bytes are `488bc3`.

| Instruction RVA | Bytes | SHA-256 |
|---|---|---|
| `0x131224` | `8b4320` | `387e4a44f5b080dde8628135fc944390bbf950c68ee197363a3b7e8d1abd3806` |
| `0x131227` | `25f0fcffff` | `e0c8c1ee91c5a89edf4e82e9c633ce1b9900b8fbe64851bbc541cc6666ae9ede` |
| `0x13122c` | `83c860` | `96dccd5b45324dbda00c012f571ae71dcee68b1e9a7185e68469b6e613b9b889` |
| `0x13122f` | `894320` | `34e978765ad856d760a3bb25ae4d4aa6e4506bc582675562bda8667940350fc4` |

Retained assembly identifies RBX as the same private pointee from `0x131178` through the four straight-line instructions and EAX as the dword load/transform/store register. Offset `0x20..0x23` fits within private size `0x88`, does not overlap private24, and has no retained earlier initializer store. Read the REA skill guidance and used the already retained qualified disassembly plus direct read-only PE parsing; no REA target/session or provider was opened or changed.

This is static evidence for the selected non-null equal-version path in the retained unwind region. It establishes no live occurrence, concrete allocator contents, mismatch/exception/cleanup behavior, initializer return or complete public QObject construction. No configure, build, executable generation, executed focused/full test, freeze, original comparison, GUI, live mutable REA or shared plan/state change was performed. Parent owns compiled validation and the source-stable gates. No GUI-control session was acquired.

## Released source SHA-256 (implementation handoff, not a freeze)

| Path | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `c89fd9e4087b0959e3ea003d16339febd41fd1d9421aac4120eb913e71c6424e` |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `c37d45580af418035251f945b9b9a432b8a9ce180273438d66b6de37ed828410` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `fe6d7ba01361a13272e7bb311b279c346acb06ada0891589d81037e547055756` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `c808c395768ca0771992655ff8c392e7c99e9222e64baade32b81a0034e70fa1` |

Decision report SHA-256: `940ad7fce4720f83be14beec333d52071c7343f1e80705dead44808f29c720ca`. File ownership is released to the parent with this report.

## Independent-review origin-inequality follow-up

Extended the existing shared-root, two-factory test with two genuine retained private allocations. It now checks distinct allocation-bound B origins and unequal symbolic EAX/memory expressions despite identical read RVA and known masks, plus equality between each allocation's own EAX and stored expression. This distinguishes per-allocation unknowns from the intentionally shared QList/QString roots. No runtime source changed. Static Testkit/include preflight remains valid; this worker ran no build or tests. The test-file hash in the release table reflects this change. Ownership is released again to the parent.
