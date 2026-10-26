# CP133 next SPRING2 frontier: two-row hexadecimal failure continuation

## Selected bounded profile

The next useful continuation after the exact two-row binary retry is a two-row primary record with the entire companion byte string exactly `0xG\r\n`. Qt's hexadecimal failure advances differently from binary failure:

1. The first extraction consumes the recognized `0x` prefix, rejects `G` as a hex digit, ungets only `G`, writes zero and sets conversion status.
2. Unread `G` keeps `atEnd()` false. The second extraction is attempted even though status is already nonzero. Decimal fallback rejects and ungets that same `G`, writes zero, and preserves the original nonzero status.
3. The SPRING2 helper increments the index after each extraction, reaches count two, ignores status and returns true.

Thus the statically predicted flags are `[0, 0]`; the first failed getter's next unread position is offset 2, and the second leaves it at offset 2. This differs from the `0B2` rollback case, which restores the cursor to offset 0. The current candidate admits `0xG` only for one row and rejects this exact two-row continuation.

Proposed positive fixture: a valid two-row ASCII `SPRING.INI` plus exact `0xG\r\n` companion; assert read success, both flags zero, and byte-for-byte unchanged inputs. Keep count three and count-two mixed/space-varied companion strings guarded; this slice covers only two repeated failed reads at the exact cursor sequence above.

## Retained original evidence

`analysis/resume83-sw2sp-parser-independent-review.md` records PE-byte-verified Qt 4 `getInteger`, `operator>>(int&)`, `ungetChar`, and `atEnd()` regions, tied to original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Its witness table states `0xG\r\n` fails with value zero, next offset 2 at `G`, and fresh status 2. The disassembly analysis identifies the hex-prefix path and explains that hex failure leaves the `0x` consumed while ungetting only the invalid following digit.

The same report establishes that `operator>>(int&)` bases the getter call on input availability, not prior conversion status, and only writes a new nonzero status when none was already set. Its `atEnd()` evidence is based on unread buffer/device state, so ungot `G` prevents loop exit. `analysis/resume119-spring2-normal-path.md` and `analysis/wave2-save-diffs-300-support-1410da690.c` document that `FUN_1410da690` prezeros flags, loops on `!atEnd()` and count, extracts and increments each time, then returns true after successful open without inspecting status. These static call and machine-code facts imply two zeros for this exact count-two fixture. No original-process differential was performed.

## Guard boundary

Implement only the complete exact two-row profile with companion bytes `0xG\r\n`. Do not admit arbitrary mixed tokens, count-three retry, other malformed hex characters, broader valid hex, signed prefixes, codecs/non-ASCII or other paths. No source/build/runtime work occurred for this read-only selection.
