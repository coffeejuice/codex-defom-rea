# CP189 connector jump plan review

## Decision

**Pass for the proposed bounded transition and acceptance contract.** The retained caller bytes support an unconditional jump from QtCore RVA `0x13141e` to `0x131425`. The proposed terminal boundary is immediately before the first public backlink store. Implementation and focused test execution remain separate gates; this is a read-only plan review.

## Evidence and transition

`analysis/resume163-connector-qobject-671313d0.asm` shows `EB 05` at VA `0x6713141e` (RVA `0x13141e`). Short-jump displacement is relative to the next instruction at `0x131420`, so `0x131420 + 5 = 0x131425`. The following bytes are `33 F6` and `48 8B C6` at `0x131420` and `0x131422`, respectively; they are skipped. Those form the null-allocation arm reached from the earlier `test rax,rax` / `je 0x131420`. CP188 ends after the normal non-null return path's `xor esi,esi` at `0x13141c`, so the jump is unconditional and no flag or register predicate is required.

CP188's source and test contract establish phase `BeforeCallerJump`, cursor/RIP `0x13141e`, RSI=0, XOR arithmetic flags (with AF unavailable), RSP=`S+8`, and qualified task/private identities and return-slot lineage. CP189 should admit exactly this validated state and advance only the control-flow cursor to `0x131425`. Since `JMP rel8` changes RIP only, retaining RSI, flags, other register observations, RSP, memory, allocation/reference records and backlink observations is correct.

The next instruction at the target is `48 89 43 08` (`mov [rbx+8],rax`), which publishes the public task's private pointer. The later `48 89 58 08` at `0x131431` is the reverse link. The strict stop at the target is therefore before either store; no public/private backlink or QObject completion can be claimed. Static capture metadata qualifies these bytes as an unwind-region listing and explicitly does not establish the complete logical function or runtime behavior.

## Guards and focused acceptance

The plan's requirement to bind the pending Report ordinal, owner/control block, module digest, CP187 lineage, qualified RBX/RAX identities, and exact CP188 phase/cursor is consistent with the existing CP188 source guards. Wrong, missing, foreign, or corrupt predecessor state should refuse before mutation. The plan also correctly requires atomic terminal cursor recording and terminal reentry checks so the jump cannot replay.

Acceptance should compare a full predecessor snapshot with the terminal state, allowing only the derived cursor/RIP change. It should check the preserved RSI and flags, RSP and lineage, sparse memory and allocation/reference observations; assert both backlink markers remain false; exercise wrong phase/cursor, foreign ownership/module/token and corrupted-lineage refusals without mutation; and verify terminal reentry does not replay. The retained listing provides direct evidence for the `EB 05` target and both store boundaries.

No plan blocker found. No source, build, test execution, GUI, REA database, or deployment operation was performed.
