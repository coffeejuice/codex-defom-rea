# CP194 connector source review

## Decision

**PASS for the bounded source transition.** The current implementation models TEST RDI,RDI and stops before JE. No source-level blocker was found against `resume194-connector-next.md`, its corrected plan review, the CP193 frozen archive comparison, and the implementation report. This is a read-only source review; it does not establish that CP194 has built or executed successfully.

## Evidence and byte-level CP193 comparison

Compared the current four assigned files directly with entries in `build/wave3-checkpoint193-verified/source.zip` (8,130,864 bytes). The archive stores paths without the leading `source/`; all four expected entries were present. The hashes below are SHA-256 of the raw archived/current file bytes. Line delta counts normalize line endings before comparing.

| File (archive path) | CP193 SHA-256 | Current SHA-256 | Normalized line delta (added/deleted) |
|---|---|---|---:|
| `apps/moproj2mst/src/doe/task_factory.cpp` | `661fe6a283709e5a6233bf0867db0fc5dde1fe60cbf48f9293e5b4fdda818aad` | `316782fc675f263fef33bb19996ddb40eba5ab0f9f5d3f0fb35fe43b249b228a` | 47 / 18 |
| `apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `5e7f32688d16e0c9497e84831670f10affe8d08d26ef86f53d425c76a14ebe5b` | `083b47d27f2c6201c92ce6d03307a6d8733db4f0a8226d861bc183612c5c0c89` | 43 / 2 |
| `apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `0662118377861f9e3ecccfea8f42273cb2199a9d274ecf433021ca569d9b6544` | `e6c2ee22e487686b31b548375d6490ff03557a9ac1dbc8e5019c243b4a4f8d9b` | 3 / 3 |
| `apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `c4bbf2a17f3430da72aa44e9ea62c78a3ab516df93e0f02f6bec5b607f72c808` | `9d4cddcdb569a97795305701d1e8b1f4f81c3adb9086782657725493382244f8` | 193 / 15 |

The `task_factory.cpp` changes are scoped to admitting the new post-TEST phase in inherited prefix guards, enforcing saved/restored RDI nullness agreement, recomputing and validating TEST flags at the successor, extending the phase/stop table, and adding ordinal 15's TEST-only update. The runtime header adds the `BeforeCallerParentJump` phase/cursor, optional parent-null refinement, symbolic TEST outcome and prediction. The factory header changes the stop range/default from 0..15/15 to 0..16/16. Test deltas add CP194 flag, preserved-state, nullness, refusal, rewind/reentry and unknown default-Session coverage, and pin historical CP193 Session coverage at its former budget. I found no unrelated behavior delta in these four files during normalized diff inspection. This directly compares against the CP193 archive; `wave3-checkpoint193-validation.json` remains the checkpoint validation record.

The corrected CP194 note and plan review establish the selected half-open range `[0x131435,0x131438)`: TEST bytes `48 85 ff`, followed by JE at `0x131438`, taken target `0x131449`, and fallthrough `0x13143a`. The source phase table adds ordinal 15 for TEST and ordinal 16 for the JE stop; the instruction switch handles ordinal 15 only. It does not execute JE, either successor, a parent load, or a call.

## Contract checks

- **Admission/provenance:** CP194 terminal state is admitted only at `BeforeCallerParentJump` with a TEST outcome exactly recomputed from `savedEntryRdi`, no live XOR flags, and restored RDI token/refinement agreeing with the saved token. The inherited saved-RDI allocation owner/control block, task/report/private identities, save and move RVAs, CALL/RET lineage, stack slots and displacement, CP193 publication/backlink, R12, spill, and retained-prefix checks remain in the path before resuming or terminal reentry. Unknown nullness is represented by `std::nullopt`, not invented pointer bits, and is admitted for the default Session route.
- **TEST semantics:** The outcome is bound to the same saved/restored RDI token. CF/OF are Clear and AF is Undefined. Unknown ZF/SF/PF remain symbolic expressions; known-null resolves ZF and SF/PF correctly; known-nonnull resolves only ZF and leaves sign/parity symbolic. `predictedJeSuccessorRva()` returns no prediction for unknown nullness, `0x131449` for null, and `0x13143a` for nonnull. These are predictions only; the phase remains at the pre-JE cursor.
- **State effects:** Ordinal 15 assigns the flag outcome, clears the prior XOR flag model, and advances the phase. It does not write registers, stack, memory, links, counts, or completion fields. The source keeps both completion flags false at the retained stop. CP193 prefix guards and terminal revalidation still run before the diagnostic stop, including smaller valid budgets.
- **Refusal/reentry coverage in source:** Focused tests assert unknown/null/non-null behavior and compare a full predecessor snapshot across TEST; they exercise rewind/resume, valid and invalid budgets, terminal reentry across task keys, and a corruption matrix for saved/restored token and refinement, slot/lineage, flags/outcome, publication/owner, phase, spill, registers, and completion. The Session test checks the unknown-parent default path. These are inspected test assertions, not runtime results.
- **Diagnostic order:** The CP194-specific admission check is phase-qualified, while CP193 and earlier phases retain their predecessor-state expectation. The ordinal and diagnostic table preserve the historical stop messages through CP193 and add the separate CP194 pre-JE stop. No source-level diagnostic-order regression was evident in the inspected paths.

## Limits

No build or executable tests were run. Source inspection cannot establish runtime pass/fail, actual native pointer values, fault/concurrency behavior, branch effects, parent loads, later calls, or public lifecycle behavior. The direct archive comparison confirms source deltas only; runtime pass/fail and source-stable CP194 freeze remain integration checks. No source files, tests, build outputs, GUI state, or REA data were modified; only this report was written.
