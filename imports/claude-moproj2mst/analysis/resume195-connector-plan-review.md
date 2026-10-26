# CP195 connector plan review

## Decision

**PASS for the proposed bounded JE instruction model, subject to the CP194 source-stable freeze gate before integration.** The plan selects exactly `[0x131438,0x13143a)`, records the two distinct successors, and stops before either successor's effects. The known-null/known-nonnull refinements remain explicit model inputs; unknown nullness is retained at the pre-JE guard. No plan correction is needed. CP194 has been built and tested as reported by the task owner, but it is not yet frozen; CP195 must not be integrated or its source owners released until the required source-stable checkpoint archive and receipt exist.

## Original static evidence and qualification

Read `analysis/resume163-connector-qobject-static.json` before interpreting `analysis/resume163-connector-qobject-671313d0.asm`. The qualification describes a read-only original and an unwind-region disassembly; it explicitly does not prove a complete logical function or native runtime behavior. I independently recomputed the SHA-256 values from the retained files:

- Original `evidence/original-binaries/QtCore4_64.dll`: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` (matches qualification).
- Listing `analysis/resume163-connector-qobject-671313d0.asm`: `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9` (matches qualification).

The listing bytes are consistent with the proposed boundary and branch mapping:

- Preferred VA `0x67131438`: `74 0f`, `je 0x67131449`.
- Taken target `0x67131449` maps to RVA `0x131449` using image base `0x67000000`.
- Fallthrough VA `0x6713143a` maps to RVA `0x13143a`.
- The fallthrough begins `mov rcx,qword ptr [rdi+8]` at `0x6713143a`; the taken target begins a call at `0x67131449`.

Thus `[0x131438,0x13143a)` contains only JE and correctly excludes the parent load and call. This evidence establishes bytes and boundaries only; it does not establish runtime branch execution, pointer values, or surrounding function semantics.

## Contract review

- **Three-valued admission:** The plan correctly preserves parent nullness as known-null, known-nonnull, or unknown. The CP194 default Session route must retain the unknown symbolic parent, its admitted prototype, and zero tasks. CP195 must validate the CP194 terminal and then refuse at `0x131438` without selecting a successor when nullness is unknown. It must not turn `parentIsNull == false` into known-nonnull.
- **Branch result:** With the existing qualification seam bound to the exact restored RDI token and `[S-8]` provenance, known-null gives ZF=1 and selects `0x131449`; known-nonnull gives ZF=0 and selects `0x13143a`. Both paths should retain the prior TEST flag outcome because JE does not modify flags. Keep distinct branch/path metadata and phase-derived cursors so the two successor terminals cannot alias.
- **Effects and stopping:** The plan correctly limits JE to control-flow state. Preserve registers, flags, stack, sparse memory, QObject/task links, owner/reference data, counts, and both completion flags. No parent `+8` load, subsequent indirect load, call, TLS/current-thread action, task publication, or completion is in this cut. Reentry at each selected successor must validate the complete terminal and stop without replay, including at a smaller admitted diagnostic budget.
- **Rewind and refusals:** The obligations cover atomic rewind of branch metadata, phase, and cursor with the CP194 TEST state, and refusal of partial rewinds. The listed corruption cases appropriately cover provenance, `[S-8]`, flags/ZF, unknown-with-forged-successor, wrong marker/cursor, parent-load/call effects, and premature completion. Require unchanged allocation/reference counts on refusals, retain inherited CP193/CP194 task-key protections, and keep invalid-budget behavior nonmutating.
- **Focused assertions:** The plan requires both explicit refinement transitions, full-state equality except control-flow fields, preserved zero counts and incomplete flags, plus the default Session integration test for unknown admission and no successor prediction. These assertions directly exercise the selected contract without implying later native behavior.

## Limits and integration gate

This is a plan/static-evidence review, not implementation or execution evidence. The disassembly qualification limits conclusions to the captured unwind region. CP194's reported build/focused/full tests do not substitute for the source-stable archive/freeze required by the converter instructions. Freeze the tested CP194 source and bind its build/test hashes first; then CP195 may be integrated and tested as its own bounded instruction increment. Do not claim that either successor's native effects or complete connector behavior is recovered.

No source, test, build, GUI, REA database, parent/DMO, or Claude project was modified. Only this review report was written.
