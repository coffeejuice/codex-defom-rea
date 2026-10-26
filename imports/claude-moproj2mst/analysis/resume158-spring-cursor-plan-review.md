# CP158 SPRING2 `q`/`?` cursor plan review

## Conclusion

The proposed `q`/`?` slice is source-ready as a bounded value/cursor model for one- and two-row primary profiles. Qt's retained getter evidence supports both needed transitions: valid decimal digits return their prefix while ungetting the first non-digit; when a subsequent extraction begins on `q` or `?`, the failed getter ungets that same byte and the int operator writes zero. The outer SPRING2 loop advances its requested-row index regardless of conversion status and checks `atEnd()`, which remains false while that ungot character is available.

The inspected reader/test are still the CP157 negative-overflow freeze cited by the plan: `simcontrol300_native_io.cpp` SHA-256 `5FBE75CD40647CD063AE27AB0128592BC6AA564D0B8CD499082C9B9B8F88BC43`; `test_simcontrol300_spring_reader.cpp` SHA-256 `348CF582BBE0BF6CD6915DC3FC187A3521A638AF88B027D6A24C7A380B49C154`. No files were changed.

## Cursor/result mapping

The plan's four outcome rows follow from the original static path:

- One row, `1q\r\n`: extraction consumes `1`, returns 1, and leaves `q`; the count cap prevents another attempt, so output is `[1]`.
- Two rows, `1q\r\n` or `1?\r\n`: first extraction returns 1 and leaves the invalid byte; second extraction fails at that same byte, writes zero, and leaves it unread, giving `[1,0]`.
- Two rows, `q 2\r\n`: both requested extractions encounter the same invalid first byte and write zero; `2` is never reached, giving `[0,0]`.

This does not require claiming sticky status parity or a reusable full QTextStream state; it reproduces the row flags and cursor stasis for the bounded requested count. The retained `resume120-spring2-malformed-flags.md` source chain distinguishes `atEnd()` from conversion status and supports these outcomes. The C-locale grouping comma is a separate path, correctly excluded.

## Candidate implementation boundaries

The current integer helper already keeps `cursor` at the first nonspace byte when the no-digit extraction fails, and after a successful prefix its final suffix branch leaves `cursor` at the first unconsumed byte for existing x/comma witnesses. A narrow `q`/`?` route can use that same cursor discipline:

1. After a canonical supported decimal prefix, return the parsed value and leave the cursor at the first `q`/`?` byte rather than rejecting it.
2. If a later requested extraction starts at `q`/`?`, return zero without advancing cursor.
3. Enable that route only for the supported count-one/count-two SPRING profiles and those exact next-byte classes. Do not generalize to `+q`, `-?`, base prefixes, comma, whitespace, high bytes, or arbitrary initial invalid characters.

For a count-two case such as `1q`, ensure the prefix path is enabled even though the existing `allowSingleRowQualifiedToken` flag is false. Keep any new count-two authorization local to these `q`/`?` witnesses, not a general relaxation of the other count-two refusal gates. A focused neighboring rejection such as `1z` or `1qx` would document the implementation's exact admission boundary if the selected grammar requires a byte boundary after the marker; otherwise the claim should be explicitly limited to the marker as the first unread character and not to the ignored tail.

## Test sufficiency and evidence limits

The proposed one-row/two-row cases assert the outcomes that distinguish the count cap, prefix parse, no-digit zero, and repeated no-advance failure. Keep byte-preservation checks for both source files, and retain x/comma behavior. Add a direct initial-invalid `? 2` witness only if it is intended to validate both invalid-byte classes in the no-digit branch; the current table uses `q` for that role and `?` after a valid prefix. Tests should verify the second row is zero rather than treating malformed extraction as EOF. No fresh original-process comparison, build, or CTest is included here.

The bounded claim excludes full status-bit parity, malformed Unicode/codecs, arbitrary punctuation, comma grouping, sign-only tokens, radix prefixes, and cursor behavior beyond the requested row count.
