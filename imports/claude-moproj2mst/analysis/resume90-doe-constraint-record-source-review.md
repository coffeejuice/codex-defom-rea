# CP90 independent review: matched Constraint owner

## Result

The implementation stays within the evidence-qualified admission boundary. Accepted rows are owned by the actual Study through unique_ptr-backed concrete values; the legacy constraint count follows committed records. The uninitialized/copied native +0x10 is represented as unknown and never used as a value. Matched rows outside the bounded profile still refuse. I found no correction to recommend. This was read-only; no build or tests were run.

Reviewed the corrected ownership qualification (SHA-256 2B5E463DA9E6B4332BC6E6657CEB40CD7F7D11948EA651FD2FB92520332D5B7E), CP90 implementation receipt, and the five frozen source/test files listed there.

## Field and owner checks

- The admitted profile requires all four attributes, canonical positive int32 Id/ObjectNo/RegionNo and exact Value="0". This is explicitly a reconstruction bound, not native validation or general QString conversion parity.
- Category mapping matches the retained rules: 1010/1012/1080 -> 0, 1081 -> 1, all other admitted IDs -> 2. Duplicate rows remain separate, ordered record identities.
- The owner stores final known fields and explicit non-null empty text. sourceField10Bits is nullopt and const; copies preserve unknownness. No host default zero, parsed-value alias, or computed-double interpretation is assigned to native +0x10. The named native 0x28 allocation-size fact is not asserted as host struct size.
- Study owns records directly; no unrelated-base delete or Session cycle is introduced. On clear, model records are removed in reverse order and the count projection resets. Each append commits the record before updating count; an unqualified later row leaves earlier accepted records intact. Host RAII behavior is not presented as Qt QList/QString refcount, COW, allocation-failure, or native unwind parity.

## Ordering and external effects

The reader clears at the Constraints encounter before row dispatch. Unknown direct children are skipped; the first exact Constraint that does not satisfy the bounded profile refuses after clear and any prior appends. Later top-level child readers and the outer variable-ID tail are not reached on that throw. Normal completion reaches the existing outer ID pass. Repeated lists replace the prior model owner at each clear.

The actual ConversionSession fixture checks the same active Type2 Study and doe alias, one retained record, ordered sample writes followed by current DEF_DOE.PAR read, and no notification attempts. It establishes candidate lazy-read continuation on this owner, not native runtime parity.

The Save fixture correctly records an immediate XML open/truncation effect before the positive-constraint serialization guard. It asserts empty recorded bytes, no normal writer return or refresh boundary, preserved old SUB DOM and retained record, and no later replayable project write. This is a candidate partial journal effect, not completed Save or durable-file parity. Existing InputTable writer and DOE output positive-constraint guards remain in place; no record serialization/output use or refresh receiver was added.

## Disposition

No correction indicated. Keep the cut's limits explicit: no native +0x10 value, generic numeric conversion, native container/refcount replay, matched-row locale parity, output serialization, or public Save completion.

