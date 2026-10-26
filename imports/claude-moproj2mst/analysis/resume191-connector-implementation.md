# CP191 connector implementation

Implemented the bounded caller register transfer at QtCore RVA `[0x131429,0x13142c)`, bytes `4c 8b e0`, `mov r12,rax`. The successor phase is `BeforeCallerPrivateSpill`, with derived cursor `0x13142c`. `r12PrivateIdentity` carries the qualified private token; null means that this register observation has not been made, without establishing incoming native R12 contents. The instruction commits only this pointer token and phase through nonthrowing assignments.

## Admission and preserved boundary

Both publication admission paths now permit the CP190 task+8 publication at CP190 and CP191 only. An early guard rejects premature R12, missing terminal R12, and inconsistent R12/private tokens before prefix mutation. The complete existing late-prefix qualification remains active at CP191, including Report/allocation/module/address-point identity, saved register and CALL/RET provenance, shared-null pointees and ownership control blocks, sparse zero fields, symbolic flags, restored registers and RSP=S+8. Phase-sensitive return/RBX/XOR admission includes CP191.

The instruction loop admits absolute budget 13 and uses it as the production default; budgets 0 through 12 retain their earlier instruction stops. CP191 terminal reentry validates all state before refusing at `0x13142c` and cannot replay under a smaller admitted budget. The public task+8 value/marker persist. No caller stack-spill observation is introduced. Private+8 remains zero, reverse-backlink availability and both completion flags remain false, and no allocation/reference/cache effects are added.

## Focused tests prepared

The shared full-state snapshot now includes R12 alongside phase and derived cursor. Added six CP191 tests covering:

- Exact CP190-to-CP191 transfer, full snapshot preservation after consistently undoing only R12/phase, and explicit consistent rewind/resume.
- Terminal reentry for budgets 0..13 and all three existing Report/DOE/Custom task keys; unmodified production default and every intermediate budget.
- A 118-case corruption matrix at predecessor and terminal, including missing/foreign/premature R12, phase-only rewinds, task+8 value/marker, allocation identities, module/provenance, registers/flags/stack/return lineage, missing/foreign/expired/control-block owners, sparse fields, reverse backlink and completion.
- Premature R12 at every earlier instruction budget and before the shared-null prefix; expired widget roots at CP190 and CP191; invalid maximum-plus-one and size_t maximum at the terminal.

Historical CP190 tests explicitly pin their old default-dependent scenario to budget 12. Existing invalid-stop coverage now rejects 14 before allocation and at retained earlier prefixes. Type/nonthrowing-assignment static assertions document the register observation representation.

Static preflight verified the existing Testkit entry point, six resolvable quoted includes using the target include directory, supported CHECK/REQUIRE macros, no C assert, and no duplicate test names. No build, C++ test execution, full suite, GUI, REA database or shared-state change was performed. Integration must run the focused test target before the complete source-stable suite and freeze.

## Evidence qualification

Read `analysis/resume163-connector-qobject-static.json` and the selected retained caller listing. The source is qualified as unwind-region disassembly, not a complete logical function or runtime proof. The listing establishes the local register copy and ordering: next spill at `0x13142c`, later reverse store at `0x131431`. This implementation makes no claim about concrete native pointers, old R12, faults, concurrency, QObject parenting, later counters or final task behavior.

## Released files

- `source/apps/moproj2mst/src/doe/task_factory.cpp`
- `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp`
- `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp`
- `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp`

All four source files and this report are released for independent review and integration.
