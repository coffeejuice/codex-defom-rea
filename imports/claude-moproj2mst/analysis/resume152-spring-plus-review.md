# CP152 independent review — explicit-positive SPRING2 narrowing

## Result

No blocking correctness defect or guard leak found in the count-one explicit-positive profile. Current source/test match `analysis/resume152-spring-plus-implementation.md`: reader SHA-256 `09D6FCD852F3D5F2C5942318816A10F734C796E5C107C3125C087A057EFA16B8`; test SHA-256 `52F359644B6E12C7F538252A0A73062AE260420FA180813ED3F05ADA8ECE0541`.

## Sign, range, and low-word behavior

The new arm is reached only after the existing signed-int parse fails, after the exact negative case and CP151 negative branch, and before the unsigned decimal fallback. It requires `allowSingleRowQualifiedToken`, a `+` followed by `1..9`, and EOF or ASCII whitespace after the digit run. The checked accumulation admits magnitude at most `INT64_MAX`; then `uint32_t(magnitude)` is bit-cast to the signed result. This matches the retained positive-sign path: the sign selects decimal, successful positive completion does not negate, and the `int&` operator stores the low dword.

The tests cover the key mappings: `+4294967297\r\n` → 1, `+2147483648\t` → `INT32_MIN`, and `+9223372036854775807` at EOF → -1. They assert both input files remain byte-identical for accepted inputs. The tab/EOF cases exercise both permitted terminator paths. The common post-parse cursor update leaves the delimiter unread, consistent with the parser contract.

## Guards and interactions

`+010` is rejected by the existing leading-zero guard before the new branch. `+9223372036854775808` fails the checked `INT64_MAX` bound. `+4294967297x` cannot enter the plus arm because `x` is not an allowed terminator, and the later unsigned fallback rejects the signed prefix. `++1` fails before numeric conversion. Count-two `+4294967297` and `+2147483648` remain in the existing refusal list because the new arm is one-row gated.

Prior branches remain in order: exact `018`/`019` octal-prefix cases, complete one-row octal, CP151 negative narrowing, and then this positive-sign extension. Unsigned positive low-word handling remains after the sign branches. The tests retain ordinary `09`, octal cases, negative narrowing and signed/radix/malformed guards, so the new arm does not absorb them.

No blocker found. This is a private reader projection backed by static Qt evidence; it does not claim original-process or public Save differential validation. No source edits, build/CTest, GUI, original runtime, or mutable REA operation was performed.
