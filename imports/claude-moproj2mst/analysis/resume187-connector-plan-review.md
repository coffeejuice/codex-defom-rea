# CP187 connector independent plan review

**PASS — no blocking defect found in the bounded RET plan.** The reviewed contract admits exactly `[0x13123f,0x131240)`, `c3 ret`, and stops at caller RVA `0x13141c` before executing its XOR. This is plan acceptance only; implementation, source review, build, focused/full regression, original comparison and source-stable candidate receipts remain integration gates.

## Evidence checked

Read-only PE parsing independently reconfirmed QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, image base `0x67000000`, and bytes: `0x13123f=c3`, `0x131417=e844fdffff`, `0x13141c=33f6`, `0x131425=48894308`, `0x131431=48895808`. The relative CALL resolves to `0x131160` and its next instruction is `0x13141c`.

Both retained listing hashes match the qualified capture receipt in `resume163-connector-qobject-static.json`: initializer `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad`; caller `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`. The receipt qualifies unwind regions rather than complete logical functions. These checks establish bounded static bytes and symbolic derivation, not captured stack contents or runtime constructor/lifecycle success.

## Contract and frozen-source compatibility

The CP186 header and `task_factory.cpp` retain independent private RAX identity, restored task RBX identity, saved-entry/restored RDI token, live symbolic RSP, and saved/return-slot displacements. At `BeforePrivateReturn`, RSP is `S`, RDI identifies its qualified saved-entry record, RAX remains allocation-bound private identity, and RBX remains the qualified task identity. Saved RDI derives from caller incoming RDX through `0x1313eb`, not a proven concrete parent.

The proposed RET correctly consumes the symbolic `[S] = ReturnToCaller(0x13141c)` target and sets live RSP to `S+8`. It does not fall through into `0x131240`, erase the return slot, alter GPRs/flags/private stores, or change identity/ownership/reference state. Frozen CP186 has return-slot displacement and call/return provenance in the saved-register records; it has no separately materialized return-slot token or explicit RIP field. Integration must introduce or deterministically derive the smallest qualified slot/cursor representation at frame creation, retain it across stops/rewinds, and reject corrupt persisted state rather than repair it. The plan's full-provenance, validate-before-mutation and nonthrowing-transition requirements support that implementation.

The strict successor boundary is correct: `0x13141c` XOR, `0x131425` public `[RBX+8]` publication and `0x131431` private `[RAX+8]` backlink are excluded. Preserving the private zero at `+8` does not establish the later backlink. QObject/base completion and parent inference remain unavailable.

## Guards and acceptance gates

The plan requires genuine CP186 predecessor state, exact qualified target/slot, module/call/initializer provenance, allocation-time identity, owner/control block, ordinal and restored registers before mutation. Its terminal requires cursor `0x13141c`, displacement `+8` and unchanged register results. Reentry including reduced budgets must validate that terminal before refusal without replaying RET; rewind to CP186 restores cursor/RSP and clears only CP187 stage state while retaining slot provenance.

Focused acceptance covers exact snapshot differences, predecessor/terminal corruption without mutation, return-target and displacement corruption, reentry, budget retention, rewind/resume and negative caller/publication/completion assertions. During integration extend all predecessor result guards to allow the new successor, retain explicit earlier stops, and update invalid-budget fixtures if ordinal 9 becomes the terminal stop (10 then first invalid, with maximum-size rejection retained). Include same-key, different-key and unknown-key terminal reentry as inherited CP186 behavior.

`wave3-checkpoint186-validation.json` records 361 registered tests, zero failures, 354 passed, six passed with logged gaps and one with logged differences; its historical comparison retains 93 SAME / five DIFF. These are retained CP186 receipts, not tests rerun by this review or fresh CP187 acceptance.

Minor wording precision, nonblocking: the plan calls the vptr store at `0x1313f8` "later" even though it precedes the allocation/call, and its caller source range `..0x671314a0` is an inclusive disassembly endpoint (receipt end-exclusive `0x671314a1`). Neither affects the cut or proof.

Only this review document was written. No source edit, build, test execution, GUI, runtime, deployment or REA database operation was performed.
