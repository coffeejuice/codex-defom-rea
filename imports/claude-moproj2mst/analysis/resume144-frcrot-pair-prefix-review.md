# CP144 review — FRCROT pair-prefix plan

## Review result

`analysis/resume144-frcrot-pair-prefix-plan.md` is consistent with the retained parser-order evidence on the critical control-flow distinction:

- Forward `(master, slave)` lookup/append and ID stores happen before the mode/value reader at `0x140af20b0`.
- Its early failure returns before the common reverse-pair tail. The resulting prefix can contain the forward pair, but no reverse lookup/append or mirror-copy call.
- After mode success, the parser resets/sets the forward table fields and invokes `0x140af73f0`. A table-reader failure is saved while control still reaches the common tail; the reverse pair and mirror-copy boundary are therefore reached before the final failure status is returned.
- Self-pairs skip reverse work. Non-self reverse identity is the ordered tuple `(slave, master)`, and the retained copy call is `0x1408e5d60(reverse, forward)`.

The plan's refusal to claim exact table rows, mirror payload bytes, or public conversion is correct. `resume142-frcrot-input-provenance.md` only source-qualifies nominal/upper/lower inputs and the later handler3 selector; it does not certify a current public producer for the three text values. The private prepared-value seam must remain disconnected from the public reader/conversion path.

## Narrow amendment before implementation

The planned output type should explicitly preserve the forward mode-table prefix on later table-reader failure. The current sequence says the successful mode handler mutates `+0x240` before `0x140af73f0`, but the proposed output summary names pair identities/effect observations and an error category without explicitly retaining the mode reset and recognized mode fields. A table-reader failure must not roll those fields back in the candidate prepared value. At minimum, represent a bounded forward table projection (reset/template marker, dimension, recognized code/value fields where source-qualified) or make the API return a prepared state that retains those mutations; tests should assert that this state survives a failed table-reader outcome while reverse/mirror order still occurs.

The corresponding early mode-line failure must retain only the forward pair identity/order prefix and must not claim that mode reset, mode fields, table reading, reverse work, or mirror copying occurred. Existing pair payload/default initialization should remain opaque. This is an admission clarification, not a reason to add table-row parsing or mirror-copy emulation.

## Suggested test refinement

Keep the seven planned cases, with these assertions made explicit:

1. Early mode failure after a missing forward pair: forward identity appended; no table reset/mode-field observation, reader call, reverse append, or mirror call.
2. Mode success plus later table-reader failure: recognized mode's source-qualified forward table fields remain in prepared state; table failure is the final status; for a non-self pair, reverse ensure and mirror-call observation still follow it in original order.
3. Unrecognized mode: retain the reset/template state with no recognized mode-specific writes, then represent the table-reader and common-tail stages as usual.

These observations must stay logical/value-level. They do not certify native allocation, exception, model-list mutation, table parser partial rows, `0x1408e5d60` payload outcome, handler3 selector bounds, or `0x140898f40` copy lifetime.

## Scope and qualification

The existing plan already sets the right public boundary: no `DB_STEP_DATA` mutation, file parsing, active Type3/DOE conversion, or guard removal. Keep that boundary. Evidence reviewed: `analysis/resume144-frcrot-pair-prefix-plan.md`, `analysis/resume143-frcrot-parser-order.md`, and `analysis/resume142-frcrot-input-provenance.md`, all grounded in original image SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. Static review only; no source/build/GUI/REA action occurred.
