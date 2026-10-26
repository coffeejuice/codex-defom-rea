# CP130 independent review: SPRING2 `0b12` bounded cut

The new admission matches the retained one-extraction Qt behavior for its declared one-row profile. It returns flag `1` and sets the cursor to the still-unread `2` at `start + 3`; it does not normalize the token into a complete binary number or authorize a second extraction. The static native call flow says the companion helper reads exactly the requested count and ignores QTextStream status. With count one, it exits immediately after storing the first extracted integer, so the remaining `2` is not consumed. This is a PE/caller-flow inference, not original-process differential evidence.

## Cursor and delimiter review

The frozen condition admits only exact lowercase `0b12` with EOF or ASCII whitespace immediately after it, and only when the caller passes the one-row-qualified flag. Its returned cursor is `start + 3`, matching the next unread byte in the retained Qt table. For the tested `0b12\r\n`, that is the `2` at offset 3; CR is at offset 4. `readSprings` requests one companion value and then leaves the companion loop, so neither the cursor nor the later CRLF is reparsed. Both physical inputs are read-only and tests verify they remain unchanged.

The exact token boundary is useful: `0b12x` and `0b13` remain unsupported; EOF and ASCII whitespace are the only accepted following-byte classes. The branch does not broaden to uppercase `0B12`, other binary tokens, signs, or arbitrary numeric grammar. Although Qt’s lower-level parser case-folds the prefix, this candidate cut intentionally admits only the source-qualified literal currently covered by the fixture.

## Broader guards and failure effects

The surrounding reader still clears output first, opens `SPRING.INI`, retains the existing count handling and candidate-only 256-row ceiling, initializes companion flags to zero, and reads no more companion values than the declared count. The one-row gate is passed only for `count == 1 && i == 0`; count-two `0b12` remains guarded, which is appropriate because then the unread `2` would be the start of another requested extraction. The focused tests retain that two-row refusal and verify both source files remain byte-identical when neighboring one-row spellings or two-row tokens are rejected.

The new branch precedes the generic decimal parser and returns only a flag value/cursor. It does not change row parsing, count parsing, numeric overflow handling, ASCII/BOM policy, file-open behavior, or output assignment. On unsupported inputs the parser throws before assigning the parsed vector to `output`; the test checks empty output and unchanged files. Missing companion behavior and zero-default flags are not modified.

## Assessment and limits

No discrepancy found in the narrow implementation against the retained evidence. The acceptance remains deliberately incomplete: no build or runtime validation was performed in this review; native evidence is static; and exact one-token admission does not establish generic Qt stream-state emulation. The report should not claim original runtime differential parity. The most relevant regression guards are the retained `0b13`, `0b12x`, and two-row `0b12` refusals plus the file-preservation checks.
