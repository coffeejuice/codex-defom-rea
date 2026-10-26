# CP194 connector plan review

## Decision

Block pending one correction to the next-cut description. The proposed CP194 cut itself is correctly bounded to TEST RDI,RDI at RVA `0x131435` and stops at `0x131438` before JE. However, the final sentence of the plan says that a separately modeled JE's taken path ends at `0x13143a`. The retained bytes show that the taken target is `0x131449`; `0x13143a` is the not-taken fallthrough. Correct that sentence before implementation so branch-path state cannot be wired to the wrong successor.

## Evidence checked

Read the qualification before the retained disassembly. `resume163-connector-qobject-static.json` qualifies the capture as an unwind-region disassembly of a read-only original, not a complete logical-function or runtime proof. Its QtCore and listing digests are `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`.

The listing bytes at preferred VA `0x67131435` are `48 85 ff` (`test rdi,rdi`); at `0x67131438` they are `74 0f` (`je 0x67131449`); fallthrough starts at `0x6713143a`. With image base `0x67000000`, these map to RVAs `0x131435`, `0x131438`, `0x131449`, and `0x13143a` respectively. The selected half-open range `[0x131435,0x131438)` therefore contains exactly TEST and excludes JE and both successor paths.

## Contract review

The plan correctly treats restored RDI as the existing symbolic parent pointer `P`, bound to CP193's saved/restored RDI and `[S-8]` provenance, rather than inferring pointer bits or object contents. TEST computes flags from `P & P` without changing RDI or memory: ZF is set iff `P` is null; CF and OF clear; SF/PF follow the result; AF becomes undefined. Requiring branch-ready state to agree with the qualified RDI/ZF is sound. CP193's prior XOR-derived AF must not be carried through TEST.

The CP193 prefix is a suitable gate: its frozen implementation report and source review place the reverse private+8 task backlink, owner/control-block qualification, task/private token identities, caller spill, restored registers, stack/call-return lineage, sparse state and counts at the pre-TEST terminal. CP194 appropriately requires that full predecessor state and exact backlink, then preserves every non-flag observation and both completion flags. Its null/non-null cases are branch predictions only; neither JE nor the parent loads/call may execute in this cut.

The proposed wrong/missing/foreign RDI and `[S-8]` provenance refusals, stale flags, cursor/phase and forged branch-outcome cases are appropriate. Retain CP193's owner/backlink guards and refusal snapshots at both predecessor and successor, including no count/reference changes, and test consistent rewind/resume plus terminal reentry under smaller budgets. No additional blocker found in these obligations.

## Limits

This is a static-boundary and model-contract review, not implementation or execution evidence. It does not establish native pointer values, faults, concurrency, parent-load meaning, later calls, or lifecycle behavior. No source, build, GUI, REA database, or deployment action was performed. Only this review report was written.
