# CP188 connector independent source review

**PASS — no source blocker found.** The current factory, runtime header, diagnostic-control header and focused test source implement the accepted single-instruction cut at QtCore RVA `[0x13141c,0x13141e)`. Compilation and focused/full regression execution remain integration gates; this review did not run them.

The review compared `resume188-connector-next.md`, `resume188-connector-plan-review.md` and `resume188-connector-implementation.md` with `source/apps/moproj2mst/src/doe/task_factory.cpp`, `include/m2m/doe_task_runtime.hpp`, `include/m2m/doe_task_factory.hpp` and `tests/mdt/test_doe_task_factory.cpp` (the three latter paths share `source/apps/moproj2mst/`). It also read `resume163-connector-qobject-static.json` and the retained caller listing. The evidence identifies QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; its unwind-region qualification supports bounded instruction bytes, not complete function semantics or runtime behavior.

## Instruction result and boundary

`CallerArithmeticFlags` contains exactly the six arithmetic flag observations: CF=false, PF=true, ZF=true, SF=false, OF=false, and an optional AF that defaults to unavailable. It does not fabricate other RFLAGS bits. `callerRsi` is a 64-bit optional and is assigned zero, correctly representing ESI zero extension. Both observations remain unavailable before XOR; `flags20Eax` remains the historical CP182 expression.

The flags continuation's ordinal 9 assigns only RSI and these flags, followed by `BeforeCallerJump`. `returnBoundaryRva()` derives `0x13141e` from that phase. These scalar/optional publications are nonthrowing and follow predecessor validation. No subsequent instruction is admitted: ordinal 10 always raises the terminal diagnostic before reaching the switch. The listing independently has `33 F6` at `0x6713141c`, `EB 05` at `0x6713141e`, public `[rbx+8]=rax` at `0x67131425`, and private `[rax+8]=rbx` at `0x67131431`. Neither jump nor backlink store is implemented in CP188.

## Predecessor continuity and refusals

CP188 retains CP187's qualified saved RBX, saved incoming-RDX-origin RDI token and return-slot records. Guards check allocation owner pointer and control-block identity, pending Report ordinal/task identity, allocation-time private identity, module hash, initializer/call/return RVAs and stack-slot relations. Returned phases require RSP=`S+8`; restored RBX/private RAX identities and opaque RDI token remain bound to their qualified records. RIP is phase-derived rather than caller-supplied. The flags loop additionally revalidates the sparse zero/symbolic flags prefix, initializer version argument and provenance, shared-null bindings and ownership, incomplete QObject state and unpublished backlinks.

The new phase-dependent guard rejects present RSI/flags at CP187 and rejects any CP188 result except RSI=0 and the exact defined XOR flags with AF unavailable. All these guards execute before a CP187-to-CP188 mutation. The normal non-null call/return path is the only admitted predecessor; no null-allocation branch or parent interpretation is introduced.

## Budgets, reentry, rewind and test coverage

The default absolute stop is 10; 0 through 9 retain their earlier instruction boundaries. The early maximum check makes 11 the first invalid ordinal, before factory allocation. Terminal reentry with any accepted lower budget revalidates and raises the CP188 terminal diagnostic without replay. Explicit rewind remains a fixture-controlled restoration of the full validated predecessor: it must reset phase and unavailable RSI/flags together. Changing phase alone is rejected.

The common unchanged-state snapshot includes RSI and arithmetic flags along with cursor, identities, stack, provenance, sparse memory, allocation/reference observations and factory caches. The new source contains 80 corruption cases at each of CP187 and CP188, including foreign ownership/control blocks, wrong provenance, return-slot damage, wrong phase, every defined flag, guessed AF, and missing RSI/flags. Each checks unchanged state across three task keys. The exact-effect case restores only phase/RSI/flags to compare against the full CP187 snapshot, checks RSP/identity/publication boundaries, exercises all 11 terminal reentry budgets and three keys, and validates explicit predecessor rewind/resume. Earlier CP187 explicit-stop and rewind tests remain present. Invalid-budget tests now cover 11 and the maximum unsigned ordinal.

Only this review document was written. No source edits, build, executable creation, test execution, GUI interaction, deployment or REA database access occurred.
