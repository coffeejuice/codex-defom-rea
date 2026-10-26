# CP188–189 caller batching review

## Decision

**A two-instruction batch is a defensible candidate, conditional on CP187 being accepted and on retaining both CP188's intermediate state and CP189's terminal state as independently addressable stops.** Batch only the normal-path `xor esi,esi` at QtCore RVA `0x13141c` and unconditional `jmp 0x131425` at `0x13141e`. Stop ordinal 1 must be the exact CP188 terminal (`RIP=0x13141e`); ordinal 2 must be CP189's target (`RIP=0x131425`). Do not extend the batch across the next instruction, `mov [rbx+8],rax` at `0x131425`: that is the first public-link store and a separate publication boundary.

This is a batching recommendation, not implementation or acceptance. Preserve CP188 and CP189's exact-predecessor, refusal-without-mutation and no-replay contracts. A single uninterrupted batch that exposes only `0x131425` would lose the already specified CP188 cursor/refusal point and is not acceptable.

## Retained caller evidence

The retained listing `imports/claude-moproj2mst/analysis/resume163-connector-qobject-671313d0.asm` shows:

| RVA interval | Bytes | Instruction / result |
|---|---|---|
| `[0x13141c,0x13141e)` | `33 f6` | `xor esi,esi`; RSI becomes zero and XOR flags are set (AF undefined). |
| `[0x13141e,0x131420)` | `eb 05` | unconditional jump to `0x131425`; registers, flags and memory are unchanged. |
| `[0x131420,0x131425)` | `33 f6 48 8b c6` | skipped null-allocation arm; excluded by the qualified CP187 non-null predecessor. |
| `[0x131425,0x131429)` | `48 89 43 08` | next instruction, `[RBX+8]=RAX`; do not execute in this batch. |
| `0x131431` | `48 89 58 08` | later `[RAX+8]=RBX` backlink; remains untouched. |

The module qualification and unwind-region limitation are recorded in `resume188-connector-next.md` and `resume189-connector-next.md`: QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, image base `0x67000000`. This evidence supports the bounded bytes and selected static path, not a complete logical function or runtime claim.

## Batch contract and risks

Use the accepted CP187 terminal as the only entry. The first transition must atomically set RSI to zero, record ZF=1/SF=0/PF=1/CF=0/OF=0 with AF unavailable, and advance to `0x13141e`; preserve RAX/RBX/RDI, memory, RSP (`S+8`), allocation/reference observations and backlink state. The second transition must require that exact CP188 terminal and advance only RIP to `0x131425`; preserve every other register/flag, memory, RSP and observation. Keep the same owner/control-block, Report ordinal, module, return-slot lineage and qualified task/private identity guards throughout.

At the intermediate stop, same-key or foreign-key reentry without an explicit continuation must refuse without mutation; a deliberate continuation may advance exactly once. At the final stop, reentry must refuse without replay. A single internal absolute stop ordinal 1 or 2 (default 2), or an equivalent test-only API, can preserve the two cursors; specify whether unchanged-stop reentry refuses and how an explicit raised stop resumes. Do not let a positive per-call budget ambiguously mean both “resume” and “refuse at this cursor.” The stop control is implementation/test machinery, not a native behavior claim.

Primary risk is state-model coupling: CP188 introduces RSI and partially defined flags, while CP189 is a pure control-flow step. Ensure CP189 retains CP188's flag validity/undefined-AF representation and validates it rather than reconstructing or normalizing it. Both transitions must commit cursor and architectural state together; malformed later state must be rejected before any mutation. Keep the null arm excluded, and assert both backlink stores have not occurred. No QObject parent interpretation, constructor completion, TLS/thread selection, or broader runtime behavior is established.

## Gate and command-time proxy

CP180's accepted batching method preserves each intermediate sparse prefix/cursor and validates a prefix before its next effect; it also demonstrates that batching may consolidate source-stable build/full-suite gates only when the intermediate states remain inspectable. CP188–189 have no intervening memory effect, but their separate cursor, flags, and refusal semantics still require dedicated focused acceptance.

For a **proxy only**, the retained CP176–178 receipts report mean build-workflow command time of 47.990 s and full-suite launcher command time of 33.240 s per checkpoint. If two separate checkpoints would otherwise each require that same gate, one two-step checkpoint would avoid one repeated pair: approximately **81.2 seconds of recorded command time** (`47.990 + 33.240`). This is not measured savings for CP188–189 and excludes batch implementation/review cost, lane overlap, and other elapsed work. CP180's precedent also records that its 361-test historical suite had six logged-gap suites and one diagnostic-difference suite; those qualifications remain relevant and the suite is not fresh original-process acceptance evidence. Do not claim end-to-end, token, or realized speed savings from this proxy.

## Acceptance needed before calling the batch safe

1. Show accepted CP187 entry advances to the CP188 stop with exact XOR semantics, AF unavailable, and all non-target state unchanged.
2. At CP188, verify wrong phase/cursor, foreign identities/module and corrupted predecessor lineage refuse with byte-for-byte/frame equality; verify reentry does not replay.
3. Explicit continuation from CP188 advances only to `0x131425`, preserving all state including CP188 flags; verify final reentry refuses.
4. Assert skipped null-arm instructions and both backlink stores are untouched, specifically `[RBX+8]` and `[RAX+8]`.
5. Run the required focused, source-stable full build/regression and freeze once for the combined change, retaining its qualifications and exact receipts. A failure or changed source invalidates the corresponding gate; batching does not waive it.

This review used retained static disassembly and plans plus prior command receipts. It made no source/build/test/GUI/runtime/deployment/REA-database changes and acquired no GUI-control session.
