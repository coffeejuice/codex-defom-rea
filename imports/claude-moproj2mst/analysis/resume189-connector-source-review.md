# CP189 connector independent source review

## Decision

Pass for the bounded source transition, subject to root's compilation and focused/group test gates. No blocking source defect found. This review compares the CP189 next plan, plan review and implementation report with the current factory, runtime/factory headers and test source; it does not report executed tests or native runtime equivalence.

## Evidence and boundary

Read `resume163-connector-qobject-static.json` before interpreting the retained caller listing. It identifies QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and qualifies the capture as an unwind region, not a complete logical function or runtime proof. The listing at lines 31–37 gives `EB 05` at VA `0x6713141e`, the skipped XOR/MOV at `0x67131420`/`0x67131422`, the public `[rbx+8]=rax` store at `0x67131425`, and the reverse `[rax+8]=rbx` store at `0x67131431`. The jump target is next RVA `0x131420 + 5 = 0x131425`.

`task_factory.cpp:624–730` adds `BeforeTaskPrivateBacklink` at ordinal 11. Ordinal 10 executes an empty switch arm followed only by the common phase assignment. `doe_task_runtime.hpp:190–197` derives the terminal cursor from that phase as `0x131425`; there is no independent mutable RIP or caller-supplied jump predicate. The jump cannot execute the skipped null arm or either backlink store. Neither backlink marker is assigned, and the common qualification rejects either marker already being true. QObject completion, caches and TaskInfo remain outside this cut.

## Admission and preservation

`task_factory.cpp:189–276` admits the new terminal in the existing saved-register/return guards. Both CP188 predecessor and CP189 terminal require RSI zero and the exact retained XOR arithmetic flags, including unavailable AF, RSP `S+8`, restored RDI token identity and CP187 return-slot provenance. Owner pointee and shared control block, Report ordinal, task/private identities, module digest, initializer/call/return RVAs and symbolic stack relations are checked. The instruction loop requalifies the sparse prefix, shared-null bindings, initializer version, flags expression, allocation-bound RAX return identity and restored task RBX before both the jump and terminal refusal.

Consequently, for an admitted CP188 state, the source changes only phase and derived RIP. All represented register/flag observations, sparse memory, ownership/reference observations and return provenance remain unchanged. Unknown native registers, memory and flag bits are not newly measured or established by this model.

## Refusal, reentry and budgets

The factory default becomes 11 and rejects values above 11 before factory/prototype allocation (`task_factory.cpp:129–135`, `doe_task_factory.hpp:105–109`). The loop's `ordinal == 11 || ordinal >= privateFlags20StopOrdinal_` refusal occurs after validation and before the switch or `ordinal + 1` access. Thus every terminal reentry stops at `0x131425`, including smaller diagnostic budgets; no replay, rewind, terminal fallthrough or table overrun is present. A smaller budget at a retained intermediate cut stops at that existing cut. Increasing the budget resumes the qualified prefix. These diagnostic controls do not claim native behavior.

The CP189 corruption test at `test_doe_task_factory.cpp:5988–6100` exercises 81 damage variants at predecessor and terminal, then snapshots the damaged state and verifies refusal without further mutation for three requested task keys. It covers foreign and alias control blocks, module/identity corruption, saved-register and return-slot lineage, wrong/invalid phases, RSI/flags damage, sparse-prefix damage and both prematurely published backlinks. Wrong cursor is represented through phase because cursor is derived.

The preservation test at lines 6105–6138 advances budget 10 to 11, explicitly checks terminal cursor, RSI/flags, RSP, RAX/RBX identities, both false backlinks and incomplete QObject, then temporarily restores only phase to compare the complete modeled predecessor snapshot. The snapshot helper includes provenance/control blocks, sparse state, initializer metadata, allocation/session counts, factory/cache observations and shared-null reference states. Terminal reentry tests cover budgets 0 through 11 and all three keys. Lines 6142–6176 cover every intermediate budget and resumed completion. Existing invalid-budget coverage at lines 5042–5067 tests 12 and maximum size_t before allocation and at a retained prefix.

The preservation test's deliberate phase restoration is an assertion technique confined to test code; production exposes no rewind operation. A coherently reconstructed arbitrary historical frame is not a tested adversarial authentication boundary, and none is claimed.

## Validation limits

Reviewed source and retained evidence only. No source/header/test edits, build, test execution, GUI, REA database, deployment, Git transport or shared coordination-state operation was performed. Only this review artifact was written. Root still owns compilation and focused/group integration validation. The implementation report's narrow scope and source-level behavior are consistent with the inspected files.
