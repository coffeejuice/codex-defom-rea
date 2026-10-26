# CP192 connector: caller spill before reverse publication

## Exact cuts

The next source-ready cut after CP191 is the caller stack spill, not the private backlink. Starting from the validated CP191 terminal at RVA `0x13142c`, execute exactly `[0x13142c,0x131431)`, bytes `48 89 44 24 58`, `mov qword ptr [rsp+0x58],rax`. Stop at `0x131431` before reverse publication. A subsequent separately admitted cut can execute `[0x131431,0x131435)`, bytes `48 89 58 08`, `mov qword ptr [rax+8],rbx`, and stop at `0x131435` before `test rdi,rdi`.

| RVA | Bytes / instruction | Result |
|---|---|---|
| `0x131429` | `4c 8b e0`, `mov r12,rax` | CP191 terminal follows at `0x13142c` |
| `0x13142c` | `48 89 44 24 58`, `mov [rsp+0x58],rax` | Private token spills to caller slot; successor `0x131431` |
| `0x131431` | `48 89 58 08`, `mov [rax+8],rbx` | Public task token stored in private+8; successor `0x131435` |
| `0x131435` | `48 85 ff`, `test rdi,rdi` | Outside both proposed cuts |

The evidence is the retained `analysis/resume163-connector-qobject-671313d0.asm` listing, qualified by `analysis/resume163-connector-qobject-static.json`: QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, capture SHA-256 `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`. The listing uses preferred virtual addresses `0x67131429` through `0x67131435`; the table uses module RVAs. This is an unwind-region disassembly, not a complete logical-function or runtime proof.

## Spill state and admission

S remains initializer-entry RSP. CP191 has live RSP `S+8`, so the spill destination `[RSP+0x58]` is `[S+0x60]`. Record this symbolic slot relation with a value-bearing private-allocation token and provenance tying it to this Report allocation, caller instruction, module and initializer CALL/RET lineage. Do not treat the token as native pointer bits. This is a distinct slot from saved task RBX `[S+0x10]`, saved RDI `[S-8]`, and return lineage `[S]`; none of those records is overwritten.

A phase such as `BeforePrivateTaskBacklink` should derive cursor `0x131431`. After complete admission, commit the spill observation and phase together using nonthrowing operations. The spill changes no register, flags, live RSP, public task+8 publication, private sparse field, allocation count or reference count. In particular, `private08` remains qualified zero, `privateTaskBacklinkAvailable` remains false, and `complete` and `qobjectBaseCompleted` remain false.

Retain the complete CP191 admission: live allocation owner with both pointer and shared-control-block equality; Report ordinal and task/private allocation sizes; stable public/private identities and address points; matching widget and shared-null roots, ownership and committed references; sparse zeros and flags-expression provenance; initializer version and module; RAX private token, restored RBX task token, restored RDI token, R12 private token; zero RSI and retained XOR flags; RSP `S+8`; saved slots and CALL/RET lineage; and exact task+8 publication value/marker. Qualify the new spill record's owner, ordinal, identities, instruction RVA, module and both slot offsets. Reject missing, expired, foreign or inconsistent records before any mutation.

Make publication, R12, returned/restored-register, zero-prefix and cursor admission phase-sensitive for the successor. Earlier phases must reject premature spill observations; the spill terminal must require the exact observation. Reentry at the new terminal validates it even for smaller diagnostic budgets and never repeats the store. A deliberate rewind clears the spill observation and restores the CP191 phase consistently; a phase-only rewind must refuse.

## Later reverse backlink cut

At `0x131431`, qualified RAX names the private allocation and qualified RBX names the public task. The store therefore replaces the current private+8 zero with the task identity. Public task+8 already holds the private identity from CP190; R12 and the new spill continue to hold that private identity. These are opposite directions and must remain distinct value-bearing observations.

Before implementing this later cut, represent private+8's value as a symbolic public-task token rather than coercing a host pointer into the current `optional<uint64_t>` zero representation. Preserve the historical zero only as prior-phase evidence; the current slot cannot simultaneously be asserted zero and published. Use a tagged slot or an explicit qualified transition whose phase rules make zero and symbolic publication mutually exclusive. Add a successor deriving cursor `0x131435`; commit the value, reverse-publication marker and phase after full admission. Existing zero-prefix admission must accept the transitioned slot only at that successor, and must reject premature reverse publication elsewhere. No completion, parenting, RDI branch outcome, later reference update or cache/TaskInfo result follows from this single store. The TEST has not executed, so XOR flags remain the latest represented arithmetic flags.

## Focused acceptance

For the spill cut, extend full-state snapshots with the slot observation and derived cursor. Prove a valid CP191 advance writes exactly the allocation-bound private token to symbolic `[S+0x60]`, stops at `0x131431`, and preserves all other state including private+8 zero and reverse-publication absence. Cover all admitted budget/task-key terminal reentries, a consistent rewind/resume, historical cut behavior, and maximum-plus-one/maximum-size invalid budgets without mutation.

Add refusal snapshots for missing/foreign spill values, owner expiry, equal stored owner pointer with foreign control block, wrong ordinal/module/instruction/slot offsets, wrong phase/cursor, premature spill at every earlier admitted phase, phase-only rewind, and every inherited CP191 identity/register/flags/provenance/shared-null/stack/return-lineage/publication corruption. Confirm allocation and reference counts and all retained state survive each refusal unchanged.

For the later backlink cut, separately prove private+8's zero-to-task transition, exact cursor `0x131435`, and preservation of task+8, R12, spill, registers, flags and counts. Test absent/wrong/foreign task token, zero plus publication contradictions, marker/value disagreement, premature or missing reverse publication, phase-only rewind and terminal reentry. Keep completion flags false through both cuts.

No source edits, builds, executable tests, GUI operations, REA database changes or deployment were performed for this note. Instruction effects above are static source boundaries; native fault outcomes, concurrency visibility, concrete pointer values and successful full QObject/task construction remain unestablished.
