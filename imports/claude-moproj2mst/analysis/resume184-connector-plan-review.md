# CP184 connector epilogue plan independent review

## Verdict

**PASS — no factual blocker found for the proposed one-instruction cut.** The plan correctly scopes CP184 to restoring the saved caller RBX and stopping at RVA `0x13123a`. It keeps the private identity in RAX, preserves CP183's historical EAX expression as provenance, and leaves stack adjustment, RDI restore, RET, caller continuation, and backlink publication unavailable. This is a static review only; no source, build, tests, GUI, or REA database were changed or run.

## Static bytes and symbolic stack derivation

The selected QtCore DLL independently hashes to `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; both retained assembly listings match the hashes recorded by the plan. The initializer listing confirms the prologue sequence: `push rdi` at `0x131165`, `sub rsp,0x30` at `0x131166`, save RBX at `[rsp+0x48]` at `0x131173`, and load the private pointer into RBX at `0x131178`. With entry RSP denoted S, the save slot is `[S+0x10]` and current RSP before the selected restore is `S-0x38`, so `[rsp+0x48]` resolves to that same symbolic slot.

The caller listing confirms `mov rdi,rdx` at `0x1313eb`, `mov rbx,rcx` at `0x1313ee`, the equal-version argument at `0x13140f`, RCX set to the private allocation at `0x131414`, and the call at `0x131417`. Therefore, under the plan's qualified caller/prologue provenance, the saved RBX denotes the pending task identity; the evidence does not justify a numeric address or arbitrary caller-supplied stack value.

At `[0x131235,0x13123a)`, bytes `48 8b 5c 24 48` decode as `mov rbx,[rsp+0x48]`. The following instruction bytes and boundaries also match: add RSP at `0x13123a`, pop RDI at `0x13123e`, and RET at `0x13123f`. Thus the new terminal cursor is exactly `0x13123a`, before stack adjustment. The plan's half-open intervals and cursor progression are correct.

## CP183 identity and historical EAX provenance

The CP183 implementation and source review establish that `returnPrivateIdentity` is the opaque allocation identity copied by `mov rax,rbx`, while the stored `flags20Eax` / `private20` expression records an earlier dword read/transform/store. Once RAX contains a pointer identity, treating its EAX subregister as the old expression would be inconsistent. CP184 correctly retains that expression and `private20` unchanged and separately models restored RBX as the owner-qualified pending task identity. The specified transition mutates only the restored-RBX result and phase; the MOV changes no flags and does not consume or alter RAX.

## Guards, refusal, reentry, and regression coverage

The plan preserves CP183's terminal diagnostic stop and adds an explicit guard exception for the new legitimate successor so the private return identity remains forbidden at earlier cursors. It requires the saved-slot relation and owner/caller/frame provenance, rejects arbitrary stack values, validates terminal state before refusing reentry, and requires refusal to be mutation-free for malformed or foreign state. Earlier rewinds clear the new stage-dependent result while preserving established rewind behavior; corrupted retained provenance must refuse rather than be normalized. These rules are consistent with CP183's current terminal guard and its snapshot/reentry/rewind test structure.

The acceptance cases cover success and full predecessor-state preservation, distinction between task and private identities, malformed predecessor and slot provenance, both same-key and different-key reentry, preservation of CP182/CP183 stops and rewinds, and explicit non-availability of later epilogue and caller effects. They also avoid inventing unrepresented mutable fields merely to create rejection cases. No acceptance gap is apparent for this cut.

## Qualification

The evidence is static disassembly from the identified binary and source/test history from CP183. It establishes the selected instruction sequence and symbolic relationships, not live stack contents, runtime constructor occurrences, exception behavior, or complete QObject behavior. Integration build, regression gates, and candidate-freeze chronology remain pending and are not established by this review.
