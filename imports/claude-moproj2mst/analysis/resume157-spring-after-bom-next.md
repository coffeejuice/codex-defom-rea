# CP157 SPRING2 frontier after BOM support

## Recommended next source-ready family

The largest remaining ASCII numeric gap is canonical **negative decimal magnitudes that cross the signed-64 boundary**, count-one `SPRING2.INI`. This is a separate Qt completion branch from the already admitted negative values through `INT64_MAX` and from the positive-wrap decimal path. Candidate `readSpringFlagInt` still throws when the negative magnitude exceeds `INT64_MAX` (source lines 433–445), while Qt's digit accumulator wraps modulo `2^64` and conditionally applies the negative sign based on the wrapped accumulator's signed value.

Bound the first extension to one primary row, the existing ASCII/BOM-decoded companion profile, token grammar `-[1-9][0-9]*`, and a following Qt whitespace character or EOF. Keep leading-zero signed forms, suffixes, malformed/radix tokens, multi-row retries, non-ASCII codec payloads, and other paths outside this change. This is a source-qualified numeric branch; it does not require a same-run locale witness for ASCII bytes.

## Original Qt arithmetic and caller contract

The evidence is bound to original QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

`analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` shows decimal accumulation through 64-bit multiply/add operations at `0x670c56f6–5704`; those instructions wrap modulo `2^64` and do not branch on overflow. At negative completion, `test rdi,rdi` (`0x670c57bb`) and signed `jle` (`0x670c57be`) skip `neg rdi` (`0x670c57c4`) whenever the wrapped accumulator is zero or has its high bit set. Otherwise Qt negates it modulo `2^64`. `QTextStream::operator>>(int&)` stores the low dword on getter success (`analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`, `0x670c6166–616a`). Therefore the source-faithful candidate transform is:

1. accumulate each decimal digit into `uint64_t` with defined modulo-`2^64` arithmetic;
2. if the accumulator is nonzero and its high bit is clear, replace it with `0 - accumulator`; otherwise leave it unchanged;
3. reinterpret the low 32 bits as the resulting signed flag.

The corrected Qt table in `analysis/resume83-sw2sp-parser-independent-review.md` directly records `-9223372036854775809` → `1` and `-18446744073709551615` → `-1`. Those examples distinguish the conditional sign rule from unconditional mathematical negation. Existing source-qualified `-2147483649` → `2147483647` and canonical negative values through `INT64_MAX` remain unchanged (`analysis/resume151-spring-negative-implementation.md`).

The original `FUN_1410da690` reader fills one row's companion flag, ignores QTextStream status, and returns success after the file opens (`analysis/resume119-spring2-normal-path.md`). A nonzero result proceeds to ordinary decimal companion serialization; a zero flag follows the existing all-zero readable-delete behavior (`analysis/resume120-spring2-writer-order.md`). This is static PE composition, not an original-process comparison.

## Focused candidate matrix

Use the existing valid one-row `SPRING.INI` fixture, assert exact primary/companion byte preservation on read, and keep count-two companions refused for this new family.

| `SPRING2.INI` bytes | Expected flag | Evidence purpose |
|---|---:|---|
| `-9223372036854775808\r\n` | `0` | Wrapped accumulator is `INT64_MIN`; signed `jle` skips negation |
| `-9223372036854775809\r\n` | `1` | Existing corrected-table witness; high bit set, no negation |
| `-18446744073709551615\r\n` | `-1` | Existing corrected-table witness; accumulator remains all ones |
| `-18446744073709551616` | `0` | Full modulo-`2^64` wrap to zero; EOF terminator |
| `-18446744073709551617\t` | `-1` | Wrap to positive one, then conditional negation; tab delimiter |
| `-4294967297\r\n` | `-1` | Existing admitted negative narrowing control |

Negative controls: attached suffix `-18446744073709551617x`, signed leading zero `-010` outside its existing separate narrow behavior, a two-row primary with the same companion bytes, and UTF-8/16/32 BOM payloads containing non-ASCII or malformed numeric text. Keep BOM-less high bytes refused because `QTextCodec::codecForLocale()` is process/application configured; `analysis/resume117-spring-qtstream-locale-encoding.md` says a same-process codec witness is needed before admitting those bytes. This numeric extension does not remove that codec gate.

For an Open→Save check, nonzero `-9223372036854775809` becomes flag `1` and the writer emits ASCII `1`; the zero cases follow the existing zero-flag companion cleanup path. Do not claim candidate Save behavior until root's integration test covers it.

## Evidence limits

The arithmetic, sign-completion condition, low-dword store, one-value caller, and Save branch are source-qualified. The result table combines direct corrected-table rows with transparent modulo arithmetic for the additional boundary tests. It is not a runtime comparison. Locale-specific signs, malformed extraction status beyond the existing bounded cases, and BOM-less non-ASCII decoding remain outside this proposal.

No source, test, build, GUI, or mutable REA state changed in this analysis pass. The preceding CP157 BOM implementation remains frozen in `analysis/resume157-spring-bom-implementation.md`.
