# CP148 independent review — SPRING2 positive decimal low-dword

## Result

No correctness defect or guard leak found in the reviewed branch. Current source and test match the CP148 freeze hashes: `simcontrol300_native_io.cpp` `50BC8B56431F31E2B2D8EA005A7D6C55FF7B0D6DDFF161004E455E8CC22E3B58`; `test_simcontrol300_spring_reader.cpp` `1C62FD8A32D718EBE9CBB89BFF6EEE42CE7E116F6A7805FE566023821A795A0D`.

## Implementation boundary

The fallback is reached only after `from_chars(int)` fails. The code first rejects multi-digit leading-zero forms. Its new arm then requires the parsed prefix to begin with ASCII `1`–`9` and its terminator to be EOF or `asciiSpace`; it accumulates only the digit prefix in `uint32_t` and bit-casts the low word to `int32_t`. For the reviewed nonnegative decimal class, this is the low 32 bits of Qt's wrapping 64-bit getter result. A run of arbitrarily many digits does not overflow signed arithmetic in the candidate.

Signs do not enter the new arm: `+` and `-` remain in `prefix`, which fails the first-digit check. Leading-zero runs are rejected before it (with the prior narrow `09`/`018` handling left earlier in the function). A comma or other non-whitespace suffix fails the new-arm terminator condition before the existing suffix/cursor logic can accept it. Hex/binary special cases remain earlier and separately bounded. The low-dword branch therefore does not broaden those profiles or swallow overflow-prefix delimiters.

## Test coverage and remaining limits

The focused one-row test verifies `4294967297` → `1`, `2^64` → `0`, and `2^64+1` → `1`, with complete primary-row data and byte-preservation checks for both files. It retains `2147483648` → `INT_MIN` and multi-row ordinary positive values. Negative cases still reject the positive-plus and negative overflow spellings, a leading-zero token, comma-suffixed overflow, radix tokens, malformed delimiters, non-ASCII bytes, and a UTF-8 BOM. The newly supported positive token was removed from the old refusal table.

One nonblocking coverage omission: the new huge-value cases use CRLF terminators; the implementation also admits the source-qualified EOF form, but the CP148 test additions do not exercise a huge digit run ending exactly at EOF. This is a test-only opportunity, not a source defect or evidence that the EOF path is wrong. The report's bounded contract also does not establish arbitrary process codecs or a native-process differential, and this review makes no such claim.

No source edits, build/CTest, GUI, original-process execution, or mutable REA operation were performed.
