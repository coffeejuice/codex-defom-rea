# CP189 connector: direct caller jump after XOR

## Scope and decision

Continue only the qualified normal non-null private-allocation path from CP188 terminal QtCore RVA `0x13141e`. Execute the unconditional short jump at `[0x13141e,0x131420)`, bytes `EB 05`, and stop at target RVA `0x131425`, before the public task-field store. This jump is unconditional: there is no flag/register predicate. It is reachable after CP188's `xor esi,esi` at `0x13141c`; it skips the separate null-allocation block at `0x131420..0x131425`.

## Evidence

Retained listing: `analysis/resume163-connector-qobject-671313d0.asm`, qualified by `analysis/resume163-connector-qobject-static.json` as an unwind-region listing (not complete logical-function or runtime proof). The retained QtCore identity is SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, image base `0x67000000`.

| VA | RVA | Bytes | Instruction / effect |
|---|---:|---|---|
| `0x6713141e` | `0x13141e` | `eb 05` | `jmp 0x67131425` / `jmp 0x131425`; signed rel8 `+5` from next RVA `0x131420` |
| `0x67131420` | `0x131420` | `33 f6` | skipped null-allocation arm: `xor esi,esi` |
| `0x67131422` | `0x131422` | `48 8b c6` | skipped null-arm `mov rax,rsi` |
| `0x67131425` | `0x131425` | `48 89 43 08` | first subsequent instruction: `mov qword ptr [rbx+8],rax` |
| `0x67131431` | `0x131431` | `48 89 58 08` | later private-to-public store `[rax+8]=rbx` |

## Transition contract

Admit only an existing validated CP188 terminal with the same owner/control-block binding, pending Report ordinal, QtCore digest, CP187 return-slot lineage, qualified public-task RBX and private-allocation RAX identities, exact phase and cursor. CP188's zero RSI and XOR flag state must remain intact. Reject missing, foreign, inconsistent, or wrong-cursor state without mutation; do not synthesize a branch or infer the selected path from caller-supplied bits.

The sole instruction effect is control flow: RIP advances from `0x13141e` to `0x131425`. All registers, flags, memory, RSP (`S+8`), allocation/reference records, and backlink state are unchanged. Record the terminal cursor/stage atomically so reentry at `0x131425` cannot replay the jump. The null arm is not taken or modeled by this cut; its earlier allocation-null condition is outside this predecessor.

## Boundary and focused acceptance evidence

Stop at `0x131425` before executing `48 89 43 08`. The public `task+8=private` link is still unpublished. The later private `private+8=task` store at `0x131431` also remains untouched. No QObject parent behavior, constructor completion, TLS/thread selection, or runtime behavior is established.

Acceptance should show one valid CP188 advance to `0x131425` with every register/flag, RSP, memory, allocation/reference observation, and both backlink states unchanged; no execution of the skipped null-arm instructions; and refusal without mutation for wrong phase/cursor, foreign tokens/module, or corrupt predecessor lineage. Reentry must not replay. Explicitly assert that neither backlink store has occurred. This is a static, bounded continuation plan; no source/build/GUI/REA database operation was performed.
