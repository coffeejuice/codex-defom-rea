# CP136 independent review: one-row SPRING2 `09` fallback

## Result

No blocking defect found. The candidate admits `09` only when the caller's permission is the one-row/first-flag profile and the token has ASCII whitespace or EOF immediately after its two digits. The branch returns 9 and sets the next cursor position to offset 2, leaving CR unread for `09\r\n`. This matches the retained Qt numeric table and base-selection instructions. It remains a static source/caller inference, not a fresh original-process comparison.

The gate is token-level after the parser's ordinary leading-whitespace skip; it is not a whole-file byte equality check. This is consistent with the rest of the bounded reader design and should be described as exact token recognition, not exact full-file matching. The count-one caller stops after its requested extraction, so this branch does not claim validation of later content.

## Frozen source identity

The current files match the CP136 receipt:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `4e09d88fbb75201c29e4766f56336b83ee4e1393acb5dcc760aa51928fea57ce` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `30f7a0d79dbb96198403bb2599af97aa49a6a28cd06814cea84e8527b1ef8fa7` |

## Findings

- `exactDecimal09` is gated by `allowSingleRowQualifiedToken`, which is supplied as `count == 1 && i == 0`; the two-row loop does not get this permission. The branch checks `09` plus whitespace/EOF, returns value 9 and advances from the token's start to its end. With `09\r\n`, the next unread byte is CR at absolute offset 2 after no leading whitespace.
- The retained Qt evidence states the second digit 9 causes base 10 selection, successful value 9, and next unread offset 2. The one-value helper copies the result and ignores status; no overflow/encoding claim is needed for this ASCII in-range token.
- The one-row test asserts success, one row, flag 9, and unchanged SPRING/SPRING2 bytes. Neighboring one-row refusal coverage retains `08`, `019`, and `0180` (along with unrelated existing guarded forms). The count-two refusal list explicitly contains `09`, so the new rule does not silently expand to multi-row continuation. The one-row branch also rejects an attached non-whitespace suffix because its special predicate fails and the broader leading-zero parser guard remains in force.
- The ordinary leading-whitespace skip precedes this exact token branch. A leading-whitespace `09` therefore reaches the same qualified token; the report's “exact” wording should be read as exact token bytes after that skip. Likewise, trailing bytes after a consumed delimiter are outside the one-request helper's validation, consistent with its count-limited caller; they are not certified by this test.

## Validation boundary

The original binary and Qt hashes, `getInteger` selection offsets, and SPRING2 one-row caller behavior are identified in the implementation receipt and retained CP83 evidence. No source/test edits, build/test execution, GUI/original app interaction, or mutable REA activity occurred in this review. Root owns integrated validation.
