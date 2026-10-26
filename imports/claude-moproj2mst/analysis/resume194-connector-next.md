# CP194 connector: test restored RDI before parent selection

## Selected boundary and retained evidence

Starting from a fully qualified CP193 terminal at QtCore RVA `0x131435`, execute exactly `[0x131435,0x131438)`, bytes `48 85 ff`, `test rdi,rdi`. The successor cursor is `0x131438`, at `74 0f`, `je 0x67131449` (RVA `0x131449`). The conditional branch is the next instruction and is outside this cut. The fall-through begins at RVA `0x13143a`; neither path executes here.

The qualification in `analysis/resume163-connector-qobject-static.json` describes `analysis/resume163-connector-qobject-671313d0.asm` as an unwind-region disassembly of the read-only original, not a complete logical-function or runtime proof. The listing and binary hashes are recorded there. In the retained bytes, preferred VA `0x67131435` is `test rdi,rdi`, `0x67131438` is `je 0x67131449`, and `0x6713143a` begins `mov rcx,[rdi+8]`. Preferred addresses map to RVAs by subtracting image base `0x67000000`.

## Symbolic input and flag result

CP193 preserves the restored RDI value and its qualification from earlier initializer state; this instruction neither reloads RDI nor establishes a new identity. Model it as the existing symbolic parent pointer `P` (including its qualified null/non-null state), and require the CP193 saved/restored-RDI provenance, stack slot `[S-8]`, initializer lineage and register observation to agree. Do not infer concrete pointer bits or parent object contents from this listing.

`TEST RDI,RDI` computes `P & P` for flags without storing a result. It sets ZF iff `P == 0`, clears CF and OF, and sets SF/PF from the result; AF is undefined. Other general registers, memory, stack, links, ownership and counts are unchanged. Preserve the exact modeled flag semantics, including representing AF as undefined rather than carrying CP193's XOR-derived value forward. At cursor `0x131438`, retain the test outcome as branch-ready state (or derive it from the qualified RDI); the `JE` must consume this outcome consistently.

The listing gives the branch destinations: taken `JE` goes to `0x131449`; not-taken execution continues at `0x13143a`. The first fall-through effects are the parent `+8` load at `0x13143a`, then a load through that value at `0x13143e`; these are out of scope. The taken destination begins with a call at `0x131449`; it is also out of scope. Do not claim what those loads/call mean or model either path's effects in CP194.

## Admission and transition contract

Require the complete CP193 prefix and exact reverse backlink value/marker, public task+8, R12 private token, caller spill, restored RBX/RDI, RSP `S+8`, saved slots and CALL/RET lineage, allocation/ownership qualifications, sparse private fields, reference/allocation counts, and phase/cursor at `0x131435`. RDI must be the same qualified parent observation previously restored from `[S-8]`; reject missing, foreign, expired, inconsistent, or substituted provenance. Before execution, require the pre-TEST flags from the admitted predecessor state, not a fabricated branch result.

On success, advance only the modeled flags/test outcome and phase/cursor to `0x131438`. Preserve RDI and every other register, stack slot, memory observation, QObject link, owner/reference record, and count. Keep `complete` and `qobjectBaseCompleted` false. This is a deterministic instruction-model transition, not evidence about native faults, concurrency or branch-path effects.

At terminal reentry, validate the full CP194 state and stop at `0x131438` without replay, including with a smaller admitted diagnostic budget. Rewind to CP193 must restore predecessor flags and phase/cursor together; phase-only or flag-only rewind must refuse. Any saved branch-ready result must be consistent with `P == 0` and ZF.

## Focused obligations and stopping point

Extend full-state snapshots with the TEST flag outcome, undefined-AF representation, derived cursor and phase. Verify one CP193-to-CP194 step for both symbolic cases: null RDI gives ZF=1 and predicts taken `JE` to `0x131449`; non-null RDI gives ZF=0 and predicts fall-through `0x13143a`. These are branch predictions only; do not execute or admit the branch in this cut. In both cases prove all non-flag state and counts are identical, then test consistent rewind/resume and terminal reentry.

Use refusal snapshots for wrong/missing/foreign RDI provenance, inconsistent saved `[S-8]`, stale CP193 flags, wrong phase/cursor, forged branch outcome, and any premature parent-load/call or completion state. Retain prior task-key behavior and all CP193 refusals; invalid budgets must leave state unchanged. The next independent cut, if selected, starts at `0x131438` and must model `JE` separately, ending at `0x131449` if taken or `0x13143a` if not taken after recording distinct path state; no parent load or call belongs to CP194.

This note records static instruction boundaries and reconstruction/test obligations only. It does not establish concrete parent identity, native pointer values, parent thread-data loads, TLS/current-thread behavior, later calls, cache/task publication, or successful public lifecycle behavior. No source edits, builds, executable tests, GUI control, original execution, REA database changes or deployment were performed.
