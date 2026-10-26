# CP156 explicit-positive decimal wrap plan review

## Conclusion

The plan is supported by the retained immutable Qt evidence, with an important timing note: the current reader and test already contain the proposed direct `uint32_t` implementation and its principal acceptance cases. The CP156 plan's cited pre-change reader/test hashes (`0870C...` / `577994...`) no longer match the current files. Current hashes are reader `89FABC0920BD9B972684F88CABB749C2A91230BF68092CEC01E0E864AC3DB7B9` and test `ADBFF916D98668FEA9A2FBD1F72CCF1C40AD585A2C284C0F3774F3D575A272B1`. This review therefore checks the proposed contract against the current source boundary; it does not claim a build/test result.

## Evidence and arithmetic

The retained `resume83` parser audit ties the Qt getter bytes to the shipped Qt DLL and confirms decimal accumulation wraps in 64-bit `RDI`; completion negates only for a negative sign under its signed-positive check; `operator>>(int&)` stores the low dword. The CP156 proposal correctly composes the explicit-positive route: a recognized `+` selects decimal, the positive path skips negative-sign transformation, and low 32 bits of repeated decimal accumulation modulo 2^64 equal repeated accumulation modulo 2^32. Thus `+9223372036854775808` -> 0 and the `+2^64+1` / `+2*2^64+1` witnesses -> 1 are justified. This is static machine-code reasoning, not a fresh original-process differential.

## Current reader boundary

The current fallback within the `from_chars(int)` error branch requires `allowSingleRowQualifiedToken`, a `+` followed by `[1-9]`, and EOF/ASCII whitespace at the end of the digit run. It accumulates the digits after the sign in `uint32_t`, then reinterprets the low dword as `int32_t`. Therefore it does not widen signed leading-zero tokens (`+010` has second character `0`), signed hex/binary, attached suffixes, or multi-row calls. For over-range plus values with an attached character, the branch's terminator condition fails; the fallback decimal path rejects the leading `+`. Negative overflow remains in its separate capped magnitude route, so the Qt conditional-negation distinction is not incorrectly generalized.

## Current focused test evidence

The test now includes the three above-`INT64_MAX` examples, multiple-wrap `+36893488147419103233`, and byte-preservation checks on both sidecars. Existing `+4294967297` and `+9223372036854775807` expectations remain alongside them. The one-row unsupported list still checks `+4294967297x`; the count-two refusal set now includes `+18446744073709551617`. A specific one-row suffix assertion for an above-`INT64_MAX` magnitude (such as `+18446744073709551617x`) would sharpen coverage of the exact newly admitted region, but the current guard ordering rejects it by inspection. This is a test-strengthening suggestion, not a source blocker.

## Limits

No source was edited, no build or CTest was run, and no GUI/original-process interaction was performed. The contract remains limited to canonical ASCII positive decimal tokens with complete whitespace/EOF termination on the one-row caller path; locale-specific signs/digits, commas, malformed-status parity, BOMs/codecs, and broader stream reuse remain outside it.
