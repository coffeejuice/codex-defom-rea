# CP95 independent review: absent varId setter skip

## Result

The reader now distinguishes an absent varId from a present-but-unqualified value. For an absent attribute, a newly constructed record retains constructor number 0 until the existing outer InputTable ID finalizer; a duplicate-name row skips the setter and preserves the last record's current number. This matches the retained branch. Present invalid values remain guarded. No correction indicated. Read-only review; no build or tests were run.

## Ordering and wrap behavior

The helper still requires the bounded mode-0 header/Description profile, mandatory name/numSamples/dimension, canonical positive sample count, exact dimension 3, empty/absent LinkTo, coherent concrete owner aliases, and no mixed payload. It checks varId presence separately and applies canonical-positive validation only when present. The setter is guarded by the parsed optional, so absence does not write zero or synthesize an early ID.

For a new record, constructor number 0 survives creation and child processing. Variable reindex occurs when the full DoeVariables traversal returns; the separate outer ID pass occurs only after normal completion of all top-level children. The existing CP84 function performs that repair in source list order with a recomputed signed candidate and explicit 32-bit wrap. The tests exercise [0,0,5] -> [6,7,5] and both order-sensitive INT_MAX cases. A matched-row or later sibling refusal preserves number 0 and prevents the outer pass; completed variable traversal followed by a later guard retains index 0 but leaves number 0.

For an existing duplicate name, helper-side selection/invalidation remains skipped and the reader-target last record is used. Missing varId leaves its current number unchanged while a nonempty Description can still update it. This does not claim duplicate-row or Qt temporary/ref allocation parity beyond the already qualified value effects.

## Guards and public boundary

Present empty, zero, negative, leading-zero, whitespace and overflow varId values remain preappend refusals. This is a reconstruction admission rule; it does not claim general QString conversion behavior. Name/sample/dimension/link/owner/Keyword/Geometry guards remain. The actual Session-owned fixture retains the same Study and aliases, continues ordered LHS/FF/USR plus current PAR effects, and records no notification attempt.

Save remains guarded at nonempty-variable dimension reconciliation before XML open. The fixture preserves the old SUB DOM and owner alias with no XML/text/project write. No output or public Save behavior is enabled.

## Disposition

No correction indicated. Keep the distinction explicit: absent varId skips the record setter and may leave constructor 0 until existing outer finalization; present invalid varId remains outside the bounded profile.
