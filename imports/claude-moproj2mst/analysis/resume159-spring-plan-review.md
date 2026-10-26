# CP159 SPRING2 z-marker plan review

## Decision

The original `z` outcomes are statically evidence-bound, but `z` is not a distinct parser branch. It exercises the exact same generic Qt digit-stop / unget / failed-extraction behavior already implemented and tested for `q` and `?`. Do not admit `z` as another source literal under CP159 unless the selected objective is explicitly to broaden the candidate's malformed-character policy beyond its current bounded marker set.

The current candidate intentionally refuses `1z`; this is narrower than original Qt behavior, not a parser-mechanics gap in the admitted q/? path. Adding `z` to a predicate or exact witness list would duplicate that path and create a third ad hoc spelling. If broader parity is the intended goal, make a deliberate bounded category decision (e.g. selected ASCII invalid characters excluding comma/grouping and existing radix-specific routes) and test that category, rather than promote a single letter as a new algorithm.

## Evidence for the three outcomes

Retained original Qt evidence establishes:

- After decimal digits, a non-digit other than the consumed C-locale grouping comma is ungot and the successful numeric prefix is returned (`getInteger` `0x670C5780–5790`). Thus `1z` yields 1 and leaves z unread.
- With no digits, the invalid character is ungot and the getter fails; `operator>>(int&)` writes zero (`0x670C6125–6133`). Thus initial z yields zero without consuming z.
- `atEnd()` observes unread buffered/device bytes, not conversion status. The original SPRING2 loop increments its requested-row index after each attempted extraction while `!atEnd()` and `i < count`.

Because ASCII z is not a digit, sign, whitespace, or C-locale comma, the static predictions follow: count-one `1z` -> `[1]`; count-two `1z` -> `[1,0]`; count-two `z 2` -> `[0,0]`, with the trailing 2 unreachable. The same source branch already supports those transitions for q/?; `z` adds no new Qt evidence or cursor mechanism. This remains static analysis, not an original-process differential.

## If a broader admission is later approved

At minimum, test the three table outcomes above and assert both input files remain byte-identical. Add `1z?` as a tail case if the contract is defined by the first unread byte; the second request should remain stuck at z, so the trailing question mark is never reached. Keep count-three refusal if the candidate gate remains 1/2 only, and retain controls for a neighboring non-admitted letter, comma (separate grouping path), high-byte input, and existing x/radix witnesses. Tests should demonstrate `z` is covered by the chosen category, not merely a duplicate literal path.

## Source boundary

Current reader hash: `A90ACFA898C7B0AAAE8915D0131FD1ECACC8C53EB27E38771796AFBD1B17BBB3`. Current focused test hash: `EB1312CAE15E2870C01E320605EC78D18627F73DC720F67A1E23B4CF80381807`. CP158's q/? cursor branch and the CP157 BOM/negative paths are present in this snapshot. No source/test/build/GUI/REA state changed for this review.
