# CP182 connector independent plan review

## Verdict

**Source-ready as a symbolic four-step cut from accepted CP181; concrete-only flags reconstruction remains unsupported.** The plan retains the dependency on one unknown input B, exact per-instruction state and separate memory publication. No blocking evidence defect found. Parent supplies accepted frozen CP181; current source receipts match its repaired snapshot. This review reruns no acceptance gate or original comparison.

## Independent bytes and expression proof

Direct PE reads confirm the four instruction boundaries and hashes:

| RVA interval | Bytes | SHA-256 |
|---|---|---|
| 0x131224..0x131227 | `8b4320` | `387e4a44f5b080dde8628135fc944390bbf950c68ee197363a3b7e8d1abd3806` |
| 0x131227..0x13122c | `25f0fcffff` | `e0c8c1ee91c5a89edf4e82e9c633ce1b9900b8fbe64851bbc541cc6666ae9ede` |
| 0x13122c..0x13122f | `83c860` | `96dccd5b45324dbda00c012f571ae71dcee68b1e9a7185e68469b6e613b9b889` |
| 0x13122f..0x131232 | `894320` | `34e978765ad856d760a3bb25ae4d4aa6e4506bc582675562bda8667940350fc4` |

Full interval SHA-256 is `841574798a97e10cba0dcfe2aa6864bfb1735a61a94c13d4ba6c13b21ef6fb45`. Successor `488bc3` is the excluded private-return-pointer setup. No branch/call/RBX write occurs inside the selected interval. Native relation is `(B & 0xfffffcf0) | 0x60`; unknown-input known-one/zero/unknown masks are 0x60/0x30f/0xfffffc90. The AND stage zero mask 0x30f and unknown mask 0xfffffcf0 are correct. The plan's generalized disjoint known-mask formulas are correct under uint32 truncation. Private24 zero supplies no bytes of private20.

Retained tail does not consume AND/OR flags before return, supporting omission of published EFLAGS state for this bounded path. Static unwind-region evidence does not establish concrete allocator contents, live occurrence, all callers, exceptions/cleanup or full QObject completion.

## Symbol provenance and mutation order

B must have a stable origin tied to the exact allocation-time private identity, offset 0x20, width 32 and read RVA 0x131224 under canonical module/caller qualification. A small expression record with original origin plus stage is sufficient; masks alone are not. Keep the same origin across read/AND/OR/write and reentry. Distinct allocations must not compare as the same origin. Do not regenerate B at an intermediate phase, or infer zero from absent observation.

Use separate EAX expression and memory slots. Read publishes EAX=B with memory still unobserved; AND and OR update only the staged register expression; write publishes its exact expression in memory and leaves EAX unchanged. A uint32 expression means the written EAX value is zero-extended in RAX; this is separate from the later RAX pointer assignment. Terminal phase BeforePrivateReturnSetup records the write, not return completion.

Before every step validate retained Report/private/store/root ownership, sizes/written address points, all bindings/control blocks/markers, canonical version evidence, every prior zero including private28, incomplete base/backlinks, exact cursor/expression stage/origin and memory-write status. Reject forged masks as inconsistent with the origin/stage, not as a new qualified input. Production B remains unknown; test-injected input masks belong to pure expression tests unless a separate qualified runtime-observation contract is supplied.

Accepted expression/cursor pairs should be nonthrowing after validation; construct any needed state before mutation. A later refusal preserves prior accepted register stages and exact cursor. At terminal require matching EAX and stored expression where validation is performed, with no delta/binding/observation/map/count/backlink effects. There is no root reference event or concrete count prerequisite.

## Hook and test requirements

CP181 currently defaults to absolute store stop 6 and terminates at 0x131224. Preserve stops 1..6 and add an explicit internal four-step diagnostic control, or extend absolute ordinals to 7..10 with default10. Either is viable if unchanged target refuses reentry and an explicit raised target resumes without rereading. Avoid making the existing stop6 silently run CP182. Validate hook range before allocation and retained-frame mutation, without C ABI exposure.

Tests must inspect each of four successor cursors, staged expression/origin/masks, memory unknown through OR and published only after write; resume each intermediate stage and compare complete terminal state with uninterrupted execution. Same/different-key calls at unchanged targets must not create another read token. Check unique-allocation origin separation. Pure tests for 0, all-ones and patterns plus partial/disjoint known masks must evaluate the exact formula, preserve unknown-bit dependencies and reject overlapping masks. Private24 must stay zero.

At entry and each intermediate cursor, exercise owner/binding/control-block/marker/version/identity/size/address/written flag/predecessor/completion faults plus missing/foreign origin, wrong offset/width/read RVA, wrong stage, forged masks, missing EAX, premature memory and mismatched stored expression. Snapshot all symbolic slots, phase, private observations, weak ownership, three deltas/available baselines and maps/counts; no rollback or auto-repair. Terminal mismatches should be covered if terminal validation is promised.

Migrate ordinary stop expectations and **all literal reentry diagnostics** to 0x131232, preserving intentional stop1..6 and earlier-seam diagnostics. Earlier rewinds must clear both new symbolic slots/stage publication; CP180/181 complete-frame snapshots must include them. Tests reconstructing CP181 should explicitly start with absent EAX and unwritten private20. Do not add return, QObject completion or public backlinks to satisfy the new stop. Run focused/root/Session, source-stable full regression/comparison and freeze gates afterward.

## Snapshot SHA-256

| File | SHA-256 |
|---|---|
| `analysis/resume182-connector-next.md` | `940ad7fce4720f83be14beec333d52071c7343f1e80705dead44808f29c720ca` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `51a7beacefacd9001d45bbc5d42617ed300a5c53e32803938bbba8b42f6d60d6` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `61e3183bd1e58b6dd08b0e5c283c19414175e7ba841365eb179c6e5b77553a75` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `6ddddc4ef8a69903c3eaa9fe0cf3218c5bb050171137ccd0e7ceb5575e5b64db` |

Only this report was written. No source/test edit, build, executed tests, GUI control, live REA, deployment or shared recovery-state change occurred.
