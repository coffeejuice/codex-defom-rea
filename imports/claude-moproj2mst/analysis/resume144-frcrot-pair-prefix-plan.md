# CP144 FRCROT pair-prefix implementation plan

## Purpose and boundary

Model the source-qualified pair-list prefix of the FRCROT group-3 parser as a private prepared-value operation. The seam should answer which ordered pair records exist at each parser boundary and whether the parser reached the common mirror tail. It must not parse the native text, allocate native pair objects, or claim a public import result.

The native parser is `FUN_140b667a0`; the FRCROT branch is ID `0x114`, jump-table index 4, target `0x140b67701`. CP143 and CP142 document the original-PE addresses, caller, and selection callback. Public input strings remain unowned/unqualified in the active path.

## Prepared inputs and state

Proposed pure input:

- ordered existing pair identities `(master, slave)` for one scratch model;
- parsed signed `master` and `slave` IDs;
- mode/value-line outcome: success or early failure;
- later table-reader outcome: success or failure (status only; no invented row data).

Proposed output:

- ordered pair identities after the reachable prefix;
- ordered effect observations: `ForwardFound` or `ForwardAppended`, `ModeLineFailed` or `ModeLineAccepted`, `ReverseFound` or `ReverseAppended`, `MirrorCopyReached`, and final parser status category;
- explicit stage boundary, so a forward-only error result cannot be confused with successful completion.

Pair equality is the exact ordered integer tuple. Do not normalize, sort, or deduplicate by unordered identity. Appending a pair in the model is a logical observation only; native constructor defaults, allocator behavior, and unwind are not represented.

## Native-derived sequence

1. The parser reads master/slave with the `%i %i` format (`0x141472ac8`, format RVA `0x027e6b80`). It looks up `(master, slave)` in `model+0x13c8` through `0x1408e6bd0`.
2. If missing, it appends the forward pair through `0x1408e6980` and stores IDs at `+0x168/+0x16c`. This happens before the FRCROT mode/value parser call.
3. `0x140af20b0` reads mode/value. On its early failure path, the group-3 parser returns before the common reverse-pair tail. Therefore the prepared model retains a found/appended forward pair and does not perform reverse lookup or mirror-copy observation.
4. After a successful mode/value parse, the mode handler resets/sets the forward table, then calls table reader `0x140af73f0`. The common parser tail follows this work even if the table reader reports failure; the final saved status is checked only after the tail.
5. A self-pair (`master == slave`) skips reverse work. Otherwise the parser looks up `(slave, master)`, appends it if absent, stores reversed IDs, updates its direction flag at `+0x170`, and calls `0x1408e5d60(reverse, forward)`.
6. Only after this mirror tail is parser status returned. Thus a table-reader failure still reaches reverse lookup/copy; a mode-line failure does not.

The exact internals and partial mutation of `0x140af73f0` and `0x1408e5d60` are not established. Represent only that the call boundary is reached. Do not fabricate mirrored payload bytes or say that a failed copy leaves the reverse row in any particular payload state.

## Suggested minimal API and tests

Keep the helper in a new private FRCROT-specific header/source pair, with a test under `tests/doe/`. The function should accept the existing ordered identities and prepared outcomes by value/reference and return an immutable result. Avoid modifying `DB_STEP_DATA`, active Study parsing, shared public headers, or the public Type3/DOE guards.

Focused cases:

1. Missing forward pair plus early mode-line failure: append forward first; no reverse lookup/append/mirror observation; error boundary is before common tail.
2. Existing forward pair plus early mode-line failure: preserve list order and identities; no new row or mirror observation.
3. Successful self-pair: forward ensure occurs, then common tail is reached but reverse work is skipped.
4. Successful non-self pair with missing reverse: forward ensure precedes reverse append and mirror observation.
5. Successful non-self pair with pre-existing reverse: no duplicate reverse append; mirror observation follows lookup.
6. Table-reader failure after mode success: common reverse/mirror boundary is still reached, and final status remains failure.
7. Existing identities in unrelated order: preserve their order and append only the missing exact ordered tuple.

These tests establish the prepared-value order contract only. They do not establish parser tokenization, pair constructor initialization, allocator exception behavior, table rows, mirror payload state, selector vector validity, or all-three-scratch-model output values.

## Integration gate

No public integration in this cut. CP142 shows `FUN_140916530` builds nominal, upper, and lower scratch models from three keyword text values, then invokes handler3 `0x140b51140`; its selector vector is ordered `KwOperations` data. The active public path still does not supply those three qualified values to the native-equivalent parser seam. Later work must additionally close the real keyword owner, file/text parse input, table-row reader, mirror-copy state, and copied-value lifetime before claiming FRCROT results.

## Evidence

Basis: `analysis/resume143-frcrot-parser-order.md` and `analysis/resume142-frcrot-input-provenance.md`, qualified against original GUI PE SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. This is design-only; no source, build, GUI, or mutable REA operation occurred.
