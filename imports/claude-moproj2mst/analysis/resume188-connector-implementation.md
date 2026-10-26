# CP188 connector implementation

CP188 executes only QtCore RVA `[0x13141c,0x13141e)`, bytes `33 f6`
(`xor esi,esi`), and stops at the caller jump. The qualified static source is
`analysis/resume163-connector-qobject-671313d0.asm`, an unwind-region listing,
with retained PE identity documented in `resume188-connector-next.md` and its
independent plan review. This implementation makes no runtime behavior claim.

## State and transition

`QObjectPrefix::BeforeCallerJump` derives RIP `0x13141e`. `callerRsi` is an
optional 64-bit register observation: unavailable before XOR and exactly zero
after it, including the high 32 bits. `callerArithmeticFlags` records CF=0,
PF=1, ZF=1, SF=0, OF=0, and unavailable AF. It models only these six arithmetic
flags; it does not invent other RFLAGS bits. The older `flags20Eax` remains a
historical CP182 memory expression.

The existing validated flags/epilogue continuation now admits absolute stop
ordinal 10. Ordinals 0 through 9 retain their previous diagnostics and cuts;
11 is the first invalid ordinal. The new default is 10. The qualified CP187
predecessor advances through nonthrowing RSI/flags/cursor assignments only.
RAX/private identity, RBX/task identity, opaque RDI, symbolic RSP=S+8, retained
return-slot lineage, sparse private memory and allocation/reference records
remain unchanged. The caller jump and public/private backlink stores are
outside the cut.

## Refusal, reentry, and rewind

The existing owner/control-block, pending Report ordinal, allocation-bound
private identity, retained module hash/version provenance, exact sparse prefix,
saved RBX/RDI and return-slot checks remain in force at both new cuts. New
state guards require RSI/flags to be unavailable at CP187 and exactly match
XOR at CP188, including unavailable AF. Corruption refuses before mutation.
The phase derives RIP; symbolic RSP and restored identities continue to be
checked against that phase. Reentry at ordinal 10 or any lower diagnostic
budget revalidates and retains the CP188 terminal. Explicit predecessor rewind
must restore phase and unavailable RSI/flags as well as the older contract.

## Focused tests authored

The common unchanged-state snapshot now includes RSI and arithmetic flags.
Older default terminal assertions now expect `BeforeCallerJump`/`0x13141e`;
older explicit stop-9 CP187 tests retain their original boundary. Earlier
rewind helpers restore the new unavailable state.

Added an 80-damage matrix at both CP187 and CP188 (160 fixtures), including
inherited owner/identity/lineage corruption and every modeled flag, unavailable
AF, missing RSI/flags, and invalid phase. Added an exact-effect test that
reverts only phase/RSI/flags and compares the full predecessor snapshot,
checks terminal reentry at all 11 budgets and three keys, and validates an
explicit CP187 rewind/resume. Tests also assert unpublished backlinks and
incomplete QObject state.

No build, executable creation, test run, GUI interaction, REA database access,
deployment, or shared recovery-state write was performed by this worker.
Focused compilation/test execution and independent source review remain
integration gates. The four implementation/test files are released to the
parent worker.
