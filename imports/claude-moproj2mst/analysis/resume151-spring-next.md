# CP151 next SPRING2 numeric family: canonical negative decimal narrowing

## Recommended bounded family

After CP150's one-row unsigned octal family and CP148's nonnegative decimal low-dword family, the next source-qualified gap is a complete, canonical negative decimal integer whose magnitude is greater than `INT32_MAX` but no greater than `INT64_MAX`. Restrict the first cut to count-one `SPRING2.INI`, ASCII bytes, token grammar `-[1-9][0-9]*`, and a terminator of ASCII whitespace or EOF. The candidate already handles ordinary signed 32-bit values and has one exact `-2147483649` extension; it refuses the rest of this useful negative narrowing family.

For the recommended range, Qt accumulates the unsigned magnitude in its 64-bit register, sees a signed-positive accumulator, applies the minus sign, and succeeds. `QTextStream >> int` then stores the low 32 bits. Thus the result is the signed 32-bit interpretation of `0 - (magnitude mod 2^32)`. The first distinguishing fixture is `-4294967297\r\n` -> flag `-1`; it exercises the reachable signed completion and narrowing path and is not another exact adjacent overflow value. A useful upper boundary is `-9223372036854775807` -> flag `1`; keep the signed-64 edge and beyond out of this first cut.

## Evidence and caller path

The bound QtCore is `evidence/gui_mo/QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; original application identity is `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The PE-byte-checked capture `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` binds decimal digit accumulation to 64-bit multiply/add (`0x670c56f6–5704`) and the negative-sign completion decision to `test rdi,rdi`, signed `jle`, and conditional `neg` (`0x670c57bb–57c7`). For magnitudes in `[1, INT64_MAX]`, `jle` is not taken and Qt negates the positive magnitude. The paired capture `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` shows successful `int&` extraction storing the low dword (`0x670c6166–616a`). These machine-code facts and the selected interval establish the value mapping without depending on signed-overflow behavior.

The corrected manual table in `analysis/resume83-sw2sp-parser-independent-review.md` independently gives `-2147483649` -> flag `2147483647`, `-9223372036854775809` -> flag `1`, and `-18446744073709551615` -> flag `-1`. Only the first is within the proposed safe interval and is already implemented; the latter two are documented as sign/64-bit-wrap edges and explicitly remain outside this cut. The same Qt getter/operator are called by the original SPRING2 companion loop in `analysis/wave2-save-diffs-300-support-1410da690.c`; that caller stores each extracted int into the row flag, ignores stream status, and returns success after companion open. A nonzero flag reaches `FUN_1410db030`'s companion write branch and is emitted as decimal, as qualified by `analysis/resume120-spring2-writer-order.md`.

The getter consumes all accepted decimal digits and ungets the first following non-digit; for a complete token ending in CR/LF or EOF, the first delimiter remains unread. With exactly one requested flag there is no retry on a suffix. Recommend byte-preservation checks for both `SPRING.INI` and `SPRING2.INI`; a public Save roundtrip would normalize `-4294967297` to `-1`, but original runtime comparison is not part of this static contract.

## Candidate gap and exact guards

`readSpringFlagInt` currently has the generic `from_chars(int)` path and a source-backed exact `-2147483649` repair. For other out-of-int32 values, its modulo-32 fallback is intentionally restricted to an unsigned non-leading-zero token; the negative family above falls through to `M2M_E_UNSUPPORTED`. Current frozen reader SHA-256: `C4340EA223AB9B6D701EB770328E5C888C7F0609BD07FEE6F17BA5A762FC968C`. Focused test SHA-256: `55F724E24288ABF8CBDBEFE6FB83AEF2953442D0F4099FD1F3A651E4B85B4261`.

Suggested acceptance matrix for a complete one-row ASCII primary fixture:

| Companion bytes | Expected flag | Purpose |
|---|---:|---|
| `-2147483649\r\n` | 2147483647 | Existing exact behavior remains intact |
| `-4294967297\r\n` | -1 | New negative modulo-low32 family witness |
| `-9223372036854775807` | 1 | Largest admitted magnitude and EOF delimiter |
| `-4294967297x\r\n` | guarded | Attached nonnumeric suffix is not complete-token input |
| `-010\r\n` | guarded | Signed leading-zero decimal is a separate candidate branch |
| `+4294967297\r\n` | guarded | Positive-sign completion is outside this family |
| `-9223372036854775808\r\n` | guarded | Signed-64 minimum boundary has distinct completion condition |
| `-9223372036854775809\r\n` | guarded | Magnitude wraps signed-positive and exercises conditional-negation edge |
| `-18446744073709551615\r\n` | guarded | 64-bit accumulation wrap is outside the selected interval |
| two-row `-4294967297\r\n` | guarded | Preserve the count-one admission boundary |

On accepted and rejected reads, assert both original files remain byte-identical. Keep BOM/non-ASCII decoding, locale-sensitive numerals, grouping, radix, malformed prefix/suffix, other filenames, multi-row retry/status, and all magnitude/sign cases outside the interval guarded. In particular, do not generalize negative leading zero based solely on this family: the table records `-010` -> `-10` because the sign selects decimal, but it is a separate behavior from low-dword narrowing and should be qualified independently.

## Limits and qualification

This is a static prediction from byte-matched Qt machine code and the retained caller, not a fresh original-process differential run. The proposed bound avoids every 64-bit overflow or sign-high-bit case. It does not claim that all larger negative text follows mathematical modulo semantics: the corrected table demonstrates conditional sign application for wrapped accumulators. No source, tests, build, GUI, or mutable REA state changed for this report.
