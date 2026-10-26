# CP160 SPRING2 malformed sign category: consume the sign, retry the next character

The next distinct private-reader increment is the **recognized C-locale sign with zero decimal digits**, rather than another ordinary invalid marker. Its lexical category is exactly ASCII `+`/`-` (bytes `2B`/`2D`, Unicode U+002B/U+002D). This is a parser category, not a claim that every Unicode punctuation/sign character behaves alike. The original consumes a recognized sign even when conversion fails. A second requested extraction can therefore advance to another sign and then succeed, while the original error status remains set. CP158's q/? failure instead leaves the first invalid character unread and repeats zero forever within the requested count.

This is source-ready from retained original function evidence. No original-process experiment or full QTextStream reconstruction is claimed. CP155 binary, CP156 positive wrap, CP157 BOM/negative wrap, CP158 repaired cursor, and CP159's independent rejection of the extra z literal were read. This proposal does not repeat their implemented numeric families or add z.

## Original decision chain

Original application SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; original QtCore4_64.dll SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

`analysis/resume83-sw2sp-parser-independent-review.md` binds the retained getInteger/operator captures to independently checked PE bytes. It establishes C-locale signs, the unsigned base-selection versus signed-decimal distinction, and fresh failure status 1 at EOF versus 2 while input remains. The same Qt methods are used by the SPRING2 caller; caller qualification is `analysis/resume119-spring2-normal-path.md` and `analysis/resume120-spring2-malformed-flags.md`.

In `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`:

1. `0x670C545F–5497` recognizes the first C-locale negative/positive sign and selects decimal at `0x670C54D0`; `0x670C54D5–54E0` pushes that first character back before entering the decimal extractor. Thus the sign does not start an unsigned radix branch.
2. `0x670C5609–5667` consumes that sign, compares it with the negative/positive sign accessors, and jumps to `0x670C56AA` without counting a digit. `r13d` remains zero. The sign is not subsequently restored.
3. `0x670C56C0–56E7` reads the next character and tests QChar::isDigit. A second ASCII sign is not a digit. In the default C-locale branch `0x670C5706–575C`, it reaches `0x670C5780–578B`, which ungets that second sign. `0x670C5790–5793` sees zero digits and returns failure 1 through `0x670C59A0`. If the sign was the final byte, the getChar failure at `0x670C56D2` reaches the same zero-digit completion with cursor at EOF.
4. `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`, `0x670C6125–6152`, writes destination zero on failure, derives status from atEnd(), and sets it only if the previous status is zero. It does not refuse a later getter call because status is already nonzero (`0x670C60F4–6120`). Success writes the low dword at `0x670C6166–616A` without clearing status.
5. `analysis/wave2-save-diffs-qt-textstream-atend-complete.asm`, `0x670C2660–26CD`, uses unread-buffer/device state. `analysis/wave2-save-diffs-300-support-1410da690.c` pre-zeroes all flags, checks !atEnd() and the requested row bound, extracts into the next row, then increments the row index regardless of status. Successful file open makes the helper return true even after these failures.

No new codec witness is needed for these ASCII bytes. CP157's already-admitted BOM decoding may represent these same ASCII code units; arbitrary Unicode signs (U+2212, U+FF0B/U+FF0D), BOM-less high bytes and Unicode digit categories stay outside this increment.

## Exact cursor, status and row effects

Offsets below are zero-based decoded ASCII byte/code-unit offsets. Status starts at zero; 1 is ReadPastEnd and 2 is ReadCorruptData. Rows refer to requested SPRING records, not lines in SPRING2.INI. Assume successful file open.

| Companion bytes | Requested rows | Flags | Native extraction trace |
|---|---:|---|---|
| `++2` | 1 | `[0]` | Consume first +; unget second +; cursor 1, status 2; row cap ends before 2 is reached. |
| `++2` | 2 | `[0,2]` | First result as above; second extraction consumes +2 to cursor 3/EOF; status remains 2. |
| `+-2` | 2 | `[0,-2]` | First + consumed, - unread at 1; second parses -2; EOF at 3, sticky status 2. |
| `-+2` | 2 | `[0,2]` | First - consumed, + unread at 1; second parses +2; EOF at 3, sticky status 2. |
| `--2` | 2 | `[0,-2]` | First - consumed, second - unread at 1; second parses -2; EOF at 3, sticky status 2. |
| `+\t2` or `-\t2` | 2 | `[0,2]` | Sign consumed, tab ungot at 1, status 2; second extraction skips tab and reads 2; EOF at 3, status 2. |
| `+` or `-` | 2 | `[0,0]` | Exactly one extraction: sign consumed to offset 1/EOF; failure status 1; second slot stays pre-zeroed because next atEnd() is true. |
| `+\t` or `-\t` | 2 | `[0,0]` | First failure leaves tab at 1/status 2. Native attempts the second extraction, consumes whitespace to EOF and fails; sticky status remains 2. |
| `+++2` | 2 | `[0,0]` | Two failed attempts consume one sign each; cursor 2, third + unread; status 2. The 2 is not reached before the row cap. |

The whitespace-only tail row distinguishes native attempted extraction count from final flags. The candidate currently pre-skips whitespace in its caller and may skip that last native-equivalent failed attempt; both projections leave the pre-zeroed flag at zero. Do not claim exact attempt-count/status parity merely from a flag test. The `++2` family is the decisive observable regression: treating sign failure like q/? cursor stasis incorrectly returns `[0,0]`.

## Proposed category-level policy

Admit the **zero-digit sign transition** locally under the existing supported one/two-row private SPRING2 profile. When the current decoded character is + or - and the immediately following character is EOF or a non-digit, return zero and advance cursor past exactly the recognized sign. Leave any following character unread. Do not skip whitespace inside this failure transition: Qt ungets that whitespace; the next requested extraction handles it. This is a predicate over the current sign/no-digit state, not a whole-file spelling whitelist or an added invalid-marker literal.

Subsequent requested extractions retain their existing admission rules. In particular, ordinary int32 +2/-2 are already parseable when reached; q/? retain their existing no-advance behavior. An unadmitted next character can still cause the entire private read to refuse. Do not broaden count-three, arbitrary signed magnitude/overflow, radix, comma, high-byte or Unicode routes to support this transition. A sign followed by a digit continues through existing numeric paths; it must not be intercepted by this zero-digit policy. A larger semantic parser consolidation remains separate work.

The policy can also safely accept count-one `+z`/`-z` by the recognized-sign transition: the following z is unread and the row cap ends. That does not admit z as an initial invalid marker, and count-two `+z` still refuses when the second request reaches unsupported z. Such a test would document reachability, not constitute another z-marker implementation. It is optional; the sign-pair category gives the stronger independent behavior.

## Focused tests and controls

Use the private `NativeExtra3DIo::readSprings` seam in `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`; retain byte-for-byte preservation checks for both primary and companion files on every accepted/refused case.

- Parameterize all four ordered sign pairs with every final digit 1..9, both EOF and CRLF termination. For count one, expect `[0]`; for count two, expect `[0,signedDigit]`, with sign taken from the second sign. This is a 2x2x9 category matrix, not four handpicked spellings. Keep result assertions after the full read, which exercises the ignored-status continuation.
- Parameterize both signs at EOF under count one/two and both signs followed by tab then 2 under count two. Include `+\t`/`-\t` for zero-fill value projection without claiming native attempted-read parity.
- Add `+++2`/`---2` under count two to prove only one recognized sign is consumed per failed extraction. An internal cursor seam, if used, should verify offsets 1 then 2 and sticky status 2; no public status field is needed.
- Retain ordinary `+2`/`-2` and CP156/157 overflow tests, malformed signed-radix guards, comma controls, q/? stasis and unadmitted initial z. Retain count-three refusal. Guard U+2212 and U+FF0B/U+FF0D even under BOM because they are not this exact lexical category.
- Existing `++1\r\n` in the one-row refusal matrix must deliberately move to accepted `[0]` under the category; leaving it as a refusal would contradict the new policy. Other unchanged guards remain explicit coverage boundaries, not claims that original Qt rejects those inputs.

Candidate source currently throws in the zero-digit branch of `readSpringFlagInt` after the bounded q/?/x/comma cases. This is the implementation seam; no source/test edit was made here. Current reader SHA-256 is `657685968B5296D519D87A7014735FF2BC76368C846FC25DEE9A4FEBA21DD7DA`; focused test SHA-256 is `A84E6C758688782352ADCFE5954B7EE038119411B513FD98CAAA9AC7181AF752`.

## Immutable evidence binding and limits

Rehashed retained files:

| File beneath analysis/ | SHA-256 |
|---|---|
| wave2-save-diffs-qt-textstream-getinteger-complete.asm | `F7D4F60A403016CD75528362C1F314307A8472E2E63CE71CC4772D1815E177C7` |
| wave2-save-diffs-qt-textstream-read-int-complete.asm | `B8A9FCB3E9FD8BDCDD615248008F0DF1713C6E72EB7200C903BB73458E92F73B` |
| wave2-save-diffs-qt-textstream-atend-complete.asm | `2332BA77C525952C8C73DA391359F3542650A79C030D18047DF3A134A558CBAF` |
| wave2-save-diffs-300-support-1410da690.c | `C973D7D1DB35EE26D84F5E8D24759C558B3D8463D4BB95E45E25AC5D62EE6352` |
| resume159-spring-plan-review.md | `E29FF36AD938112E99191D83D2949C8C97017B90AB605166EBC8AE44DEE3065A` |

The smallest optional original-runtime corroboration would be a fresh standalone Qt int-extraction receipt for ASCII `++2`: two int results, next-character positions and status after each attempt, bound to the original QtCore hash. It is not a missing immutable function witness for this branch: all sign selection, zero-digit failure, unget, sticky-status and caller-loop decisions are retained. No GUI, live app control, original executable/DLL execution, mutable REA, source/test modification, build or CTest occurred for this analysis. Only this report was added.
