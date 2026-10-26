# CP128 next bounded SPRING2 token: binary-prefix integer

## Selected one-row branch

The next distinct branch after the bounded `018` witness is the automatic-base binary token `0B10\r\n`. Qt's base-0 getter case-folds characters before prefix selection, recognizes the binary prefix, consumes the two binary digits, and succeeds with integer 2. It leaves the CR at byte offset 4 unread; a fresh integer extraction has status zero. The SPRING2 helper copies the value, ignores status, reaches its requested count of one and returns true, so the statically predicted row flag is 2. The lowercase spelling `0b10` follows from the same case-fold-before-prefix branch, but recommend the table's exact `0B10` witness to keep the source mapping direct.

The current candidate reader rejects this token at the base-prefix/leading-zero boundary. A minimal next test is one complete ASCII row in `SPRING.INI` and `0B10\r\n` in `SPRING2.INI`: assert successful read, flag 2, and byte-for-byte preservation of both files. Keep malformed `0B2`, bare `0B`, signed-prefixed variants, other binary strings, overflow, and multi-row retry behavior guarded. These have distinct rollback, status, sign-selection, arithmetic, or loop effects and are not inferred by this successful token.

## Retained Qt and caller evidence

`analysis/resume83-sw2sp-parser-independent-review.md` binds independently recaptured, PE-byte-verified Qt 4 `QTextStream::getInteger` and `operator>>(int&)` code to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

The review locates `QChar::toLower()` before base-prefix selection at `0x670c53bf`; its corrected one-value table explicitly records `0B10` -> 2, successful getter, next offset 4 at EOF, and fresh status 0. Appending CRLF leaves the CR at offset 4 with the same converted value and successful status. The same review describes the binary digit loop and distinguishes failed binary-prefix rollback, so this successful `10` binary digit sequence does not rely on the invalid-prefix rules.

The original SPRING2 helper `FUN_1410da690` invokes `operator>>(int&)`, copies the destination without testing stream status, and stops after the single requested flag; retained caller evidence is summarized in `analysis/resume119-spring2-normal-path.md`. The one-row output prediction is static PE/control-flow inference, not an original-process differential. Input is ASCII and does not require a host ACP claim. Conversion to int 2 is within range; no wider accumulation, overflow or narrowing behavior is claimed.

## Candidate boundary

This is a single successful binary-prefix witness only. Do not release a general binary parser, binary overflow/narrowing, signed prefix handling, failed-prefix cursor/status cases, additional-value loop behavior, non-ASCII/codec behavior, or other filename/count paths from it. Existing candidate refusals for those cases are coverage limits rather than claims about what the original rejects.

No source/test edits, build, GUI, original process, or mutable REA database activity occurred for this read-only report.
