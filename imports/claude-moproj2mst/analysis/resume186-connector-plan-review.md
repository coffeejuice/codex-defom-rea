# CP186 connector saved-RDI plan review

## Verdict

**PASS — no plan blocker found.** The proposed CP186 cut is exactly one instruction, `[0x13123e,0x13123f)`, and its symbolic state change follows the retained CP185 predecessor contract. This review is limited to static byte/provenance evidence and source/test-plan consistency; CP185's prior acceptance limitations and pending integration gates remain in force.

## Byte, cursor, and stack checks

The retained QtCore4_64.dll SHA-256 is `bcf0074c7e9fba3bcaFDAA38742931F309889CBDBACFFBCA32AF83DB237FC160` (case-insensitive); the initializer unwind-region listing hash is `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad`. The listing has `5f pop rdi` at RVA `0x13123e`, then `c3 ret` at `0x13123f`. The cut byte hash recorded by the plan matches the retained CP184 ledger. These artifacts establish static instruction bytes only.

From the frame's entry origin `S`, CP185 leaves live RSP at `S-8`, which addresses the saved entry-RDI slot `[S-8]`. POP loads that token into the restored-RDI abstraction and increments RSP by eight to `S`; the next cursor is `0x13123f`, before RET. POP changes no arithmetic flags or memory. The later RET would consume `[S]`, return to caller RVA `0x13141c`, and leave RSP at `S+8`; it is correctly excluded.

## Provenance and identities

The qualified caller listing shows `mov rdi,rdx` at `0x1313eb`, `mov rcx,rax` at `0x131414`, and the call at `0x131417`, returning to `0x13141c`. Together with the initializer prologue's `push rdi` at `0x131165`, this supports treating `[S-8]` as an opaque saved entry-RDI token derived from the caller's incoming RDX. Its concrete value remains unknown. The plan correctly rejects arbitrary caller-supplied tokens, binds symbolic provenance to frame/owner, ordinal, allocation and module/call-site facts, and distinguishes invalid/absent token state from a potentially null concrete value.

The plan keeps restored RDI separate from CP184's restored task RBX and CP183's private RAX identity. It also preserves the historical saved-RBX MOV stack relation rather than shifting that relation with live RSP, and leaves historical CP182 flag expressions untouched. Nothing in this POP authorizes parent dereference/publication, RET, caller continuation, backlink stores, or QObject/base completion.

## Guards, stop behavior, and acceptance

The proposed guards require a genuine CP185 terminal with live displacement `-8`, saved-RDI slot `-8`, return slot `0`, consistent saved-token provenance, and all retained predecessor identities and ownership facts. Terminal validation before same-key, different-key, or unknown-key refusal avoids treating malformed state as valid. The proposed atomic transition avoids partial POP state if token construction can allocate. The plan also preserves prior explicit stops, makes ordinal 8 the CP186 stop and 9 invalid, requires CP186-to-CP185 rewind/resume to clear only stage-dependent restored RDI, and retains lower-budget terminal state.

Acceptance cases cover the exact token and RSP transition, full snapshot preservation, absent/foreign/corrupt token and result states, predecessor/terminal reentry, earlier-stop refusal, rewind/resume, invalid budgets, and explicit non-effects for RET/publication/completion. These are appropriate for the cut, provided implementation snapshots include the new provenance/result fields and tests use represented fields rather than adding artificial corruption knobs.

## Qualification

No source or test file was changed or executed, and no build, GUI, REA database, original/candidate execution, or deployment was performed. The plan itself correctly leaves original comparison, focused execution, full regression, candidate freeze and retained receipts as integration requirements. The static unwind-region qualification does not establish a complete logical function or runtime constructor behavior.
