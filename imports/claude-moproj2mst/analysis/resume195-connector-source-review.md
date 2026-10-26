# CP195 connector source review

## Decision

**PASS for the bounded JE-only model, with no source-level blocker found.** The implementation maps known-null to the taken successor `0x131449`, known-nonnull to fallthrough `0x13143a`, and retains unknown nullness at the existing `0x131438` guard. It stops before either successor instruction. This review covers source and test assertions only; it does not establish build or runtime test results.

## CP194 archive comparison

Compared the four released files byte-for-byte against `build/wave3-checkpoint194-verified/source.zip` (which stores entries without the leading `source/`). All four match the reported CP194 frozen baseline hashes; current hashes show the bounded CP195 delta.

| File (archive path) | CP194 SHA-256 | Current SHA-256 | Bytes CP194 / current |
|---|---|---|---:|
| `apps/moproj2mst/src/doe/task_factory.cpp` | `316782fc675f263fef33bb19996ddb40eba5ab0f9f5d3f0fb35fe43b249b228a` | `b54aae798dfbc12b0d9d784ca7bebb10a40c820a04ca8f6991b855d05638cc32` | 72271 / 76059 |
| `apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `083b47d27f2c6201c92ce6d03307a6d8733db4f0a8226d861bc183612c5c0c89` | `4eee51e0381ede96f89e68ebc8270b064d6f9ae59b478c5924b1fca83c7d026f` | 22606 / 23245 |
| `apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `e6c2ee22e487686b31b548375d6490ff03557a9ac1dbc8e5019c243b4a4f8d9b` | `cd40aa83b4b92057b56a106afca83ef1da098ede40bf530375377a7a1928c0d5` | 5909 / 5951 |
| `apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `9d4cddcdb569a97795305701d1e8b1f4f81c3adb9086782657725493382244f8` | `4f6ee039e80f2b2a1f109d9aa50388d3b08a1cf9c16407720283002e4e1471bd` | 573587 / 583580 |

The deltas are localized to successor phases/path metadata and phase validation, admitting the JE instruction and its two distinct terminals, extending the diagnostic budget/default, adding branch state to snapshots, and CP195-focused tests. I found no unrelated change in the four-file comparison.

## Contract review

- **JE mapping and stop boundary:** the instruction model at `[0x131438,0x13143a)` refuses unknown ZF without state change. Known-null writes `TakenNullParent` and `BeforeCallerNullParentCall` (derived cursor `0x131449`); known-nonnull writes `FallthroughNonnullParent` and `BeforeCallerParentLoad` (cursor `0x13143a`). The phases are separate, and neither successor's call or load is executed.
- **Unknown default:** parent refinement stays optional. The CP195 branch gate refuses when `parentTestOutcome.parentIsNull` is unknown. The default Session test checks the established guarded refusal at `0x131438`, unchanged complete snapshot, no branch marker or predicted successor, one retained prototype, and zero tasks. This preserves admission without guessing.
- **Provenance and inherited guards:** branch terminals require the saved parent refinement to match the branch, path marker to match the terminal phase, TEST outcome to recompute from the saved RDI provenance, and no live XOR flag model. CP193/CP194 task/private backlink, owner, spill, saved/restored register, stack lineage, counts and prefix checks continue before terminal validation or resumption.
- **Effects:** the JE case changes only branch path and phase. The runtime cursor derives from phase. Flags/TEST outcome and register, stack, sparse memory, QObject links, ownership and counts remain untouched; completion flags remain false. No parent load, subsequent indirect load, call, TLS/current-thread action, task publication or completion was added.
- **Reentry, rewind, refusals:** focused assertions exercise both successor terminals, exact path/cursor mapping, snapshot restoration when rewinding phase and marker together, reentry across keys and admitted budgets, and invalid-budget nonmutation. The added corruption matrix includes wrong/missing marker, wrong phase/cursor, unknown forged successor, partial rewind, provenance/refinement and stack/flag inconsistencies, inherited corruption and premature completion.

## Limits

The reviewed static qualification describes the retained bytes as an unwind-region disassembly of a read-only original, not complete function or runtime proof. This source review does not run tests/builds and cannot establish native pointer values, branch execution, successor effects, fault/concurrency behavior or lifecycle completion. No source, tests, build outputs, GUI, REA database, parent/DMO/Claude files or deployment were modified; only this report was written.
