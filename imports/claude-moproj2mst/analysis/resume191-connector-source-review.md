# CP191 connector independent source review

Pass for the bounded source change. No blocking source defect found. Build and test execution remain integration requirements; this review does not claim they passed.

Compared `resume191-connector-next.md`, `resume191-connector-plan-review.md`, and `resume191-connector-implementation.md` with the current factory implementation, runtime/factory headers, snapshot helpers, and six CP191 test cases. Also inspected the existing widget ownership qualifier in `task_runtime.cpp`.

## Instruction and state boundary

Read `resume163-connector-qobject-static.json` before interpreting the retained listing. Its qualification is an unwind-region disassembly, not proof of a complete logical function or runtime behavior. The caller listing hash rechecks as `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`. Lines 34–37 retain the public store at `0x131425`, bytes `4c 8b e0` / `mov r12,rax` at `0x131429`, the next spill at `0x13142c`, and reverse store at `0x131431`.

`task_factory.cpp:764–769` copies `returnPrivateIdentity` into `r12PrivateIdentity` at instruction ordinal 12, then advances only the phase. The pointer assignment and phase assignment cannot throw. The late-prefix admission has already required RAX/return identity to match the allocation-bound private token. `doe_task_runtime.hpp:198` derives the successor cursor `0x13142c` from `BeforeCallerPrivateSpill`; there is no independently mutable cursor to disagree with that phase. Null R12 denotes an unavailable observation and makes no claim about incoming native register bits.

The instruction adds no allocation, reference increment, memory store, spill observation, flag mutation, saved-slot mutation, or completion. The qualified terminal retains public task+8 identity/marker, zero private+8, false reverse backlink and completion flags, RSP=S+8, restored RBX/RDI, RAX, zero RSI, and caller XOR flags.

## Admission, budgets, and tests

Both publication admission paths allow the task+8 marker/value only at CP190/CP191. The early R12 guard rejects premature, missing, or foreign observations before prefix writes. CP191 participates in restored-register, returned-stack, and XOR-state qualification. The per-instruction late qualifier retains Report ordinal/allocation sizes, address points, module and initializer provenance, sparse zeros, shared-null identities and ownership control blocks, and CALL/RET/saved-register lineage. `ownsWidgetRuntime` checks both pointer and ownership control block. Reverse publication/completion remains refused.

The maximum and production default are 13; values above 13 are rejected before allocation. The phase array preserves budgets 0–12. Terminal ordinal 13 validates the complete prefix and stops regardless of a smaller admitted budget, so it cannot replay. A consistent rewind clears R12 and restores CP190 phase; phase-only rewind fails the early guard.

The shared snapshot includes R12, phase, derived cursor, register/stack observations, sparse state, provenance, weak-owner identity, allocation/cache counts, and shared-null records. The positive CP191 test compares the complete predecessor snapshot after undoing only R12/phase, then exercises explicit rewind/resume. Other tests cover every budget 0–13, three task keys, production default, 118 corruption cases at predecessor and terminal, premature R12 at earlier cuts, expired widget roots, and invalid 14/size_t maximum. This is source inspection of prepared assertions, not evidence of execution.

## Reviewed source identity

SHA-256 at review:

| File under `source/apps/moproj2mst/` | SHA-256 |
|---|---|
| `src/doe/task_factory.cpp` | `af23d4be58db2cfae70d66d107616d9ed947f8779eba394273a4062658d830c1` |
| `include/m2m/doe_task_runtime.hpp` | `1ce33ca42fb6086ff27cf5d61e8d2f085dc0bc65c6824f981e3aa6e8580d7188` |
| `include/m2m/doe_task_factory.hpp` | `a100132b29ac87295d0453de5f97bd8b496b644fc28e8829efed241c14149fd0` |
| `tests/mdt/test_doe_task_factory.cpp` | `f2b003af8b57b575cf6c115abab0da62eac217b485a19ede92543a1898dca02c` |

Only this review note was written. No source edits, builds, test execution, GUI interaction, REA database work, or deployment was performed. No concrete pointer values, native faults/concurrency, later QObject behavior, or incoming R12 contents are established.
