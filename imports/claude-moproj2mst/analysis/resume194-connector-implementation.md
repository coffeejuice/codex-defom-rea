# CP194 connector implementation

The converter now models exactly TEST RDI,RDI at QtCore RVAs `[0x131435,0x131438)` and retains the successor cursor at `0x131438` before JE. No branch, parent load, call, completion or publication beyond CP193 is implemented.

The corrected next-cut note and plan review were read with `resume163-connector-qobject-static.json`. That source qualifies the retained disassembly as an unwind-region view of a read-only original, not a full logical function or runtime proof. The module digest is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; the caller listing digest is `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`.

## State and admission

`SavedEntryRdi::parentIsNull` is an optional refinement of the existing symbolic incoming-parent operand P. Its absence means unknown nullness. POP retains that refinement with the restored token, and admission requires saved/restored observations to agree. Unknown nullness remains admitted on the default Session path; no concrete parent value is invented.

`ParentTestOutcome` replaces the live XOR arithmetic-flags representation only after TEST. Its operand remains the exact saved/restored RDI token. CF and OF clear; AF is explicitly Undefined. Unknown ZF is ParentIsZero; unknown SF and PF are ParentSign and ParentLowByteEvenParity expressions over that same P. A known-null refinement resolves ZF/PF to Set and SF to Clear. A known-nonnull refinement resolves only ZF to Clear; SF/PF remain expressions rather than invented pointer-bit values.

The derived prediction is absent for unknown nullness. A null operand predicts JE taken to `0x131449`; a nonnull operand predicts fallthrough to `0x13143a`. The branch itself has not executed, and the modeled cursor remains `0x131438` in both cases.

All CP193 reverse/public backlinks, private RAX/R12 and spill, saved/restored registers, symbolic stack and CALL/RET lineage, allocation/control-block ownership, root bindings, sparse fields and count guards remain required at the CP194 terminal. Terminal reentry validates flags against the retained operand and refinement before stopping, including smaller valid diagnostic budgets. Invalid budgets fail before mutation. The absolute instruction budget extends from 0..15 to 0..16, with production default 16; historical cuts remain available at their former budgets.

## Focused test changes and verification limits

Full-state snapshots now include the optional parent refinement, restored refinement, TEST flag expressions/outcome, phase and derived cursor. New tests cover unknown, null and nonnull operands; exact predecessor rewind/resume; all valid terminal budgets and same/different task keys; invalid budgets; and refusal snapshots for saved/restored provenance, owner/control-block, slot/lineage, flags, premature outcome, phase-only and flag-only rewind, backlink/spill/register corruption and completion state. The existing 118-case inherited CP192-corruption matrix now also checks the CP194 terminal under both smaller and full budgets. The expired-widget-root test also includes CP194. CP193 default-session coverage was pinned to budget 15, and its invalid-budget check now uses 17.

Static preflight confirmed the existing Testkit main, supported CHECK/REQUIRE macros, C++20 configuration, and no new quoted includes or C assert. No build, executable test, full suite, GUI interaction, original execution, REA database mutation, shared-state bookkeeping or deployment was performed in this worker lane. Runtime pass/fail and source-stable freeze remain integration responsibilities.

Files released: `source/apps/moproj2mst/src/doe/task_factory.cpp`, `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp`, `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp`, `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp`, and this report.
