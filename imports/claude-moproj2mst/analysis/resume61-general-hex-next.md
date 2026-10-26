# Checkpoint 61: general lowercase hexadecimal boundary from QtCore

## Qualified source behavior

The retained QtCore image is SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` is its bounded `QTextStream` integer getter at `0x670C5210`; `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` is `operator>>(int&)` at `0x670C60E0`. Original Extra3D's `SW2SP.DAT` read is bound to this path in `analysis/resume54-numeric-path-gap.md` and `analysis/wave2-save-diffs-300-support-140ec8110.c`.

For default base 0, the getter recognizes a leading ASCII `0` followed by lowercase `x` and chooses radix 16 (`0x670C53D3–0x670C53E5`). It then consumes one or more digit characters in this exact set: `0`–`9` and lowercase `a`–`f` (`0x670C5590–0x670C55E2`). Each step computes `value = value * 16 + digit` in the 64-bit `rdi` accumulator. Uppercase `A`–`F` are not in the shown digit branch.

When the next character is not in that set, the getter calls its unget path (`0x670C55E6–0x670C55F1`) and, after at least one digit, writes the accumulated value and returns the valid-conversion code (`0x670C55F6–0x670C597B`). Thus the delimiter is the first non-hex character or EOF, not necessarily whitespace. For a one-integer `SW2SP.DAT` read, a fixture such as `0x10x` yields the first integer 16; the trailing `x` is left unread after the requested one value. What a subsequent extraction does with that `x` is a different invalid-token/status case.

If `0x` is not followed by a recognized digit (including `0x` at EOF or `0xg`), the getter takes its no-digit result path (`0x670C55F6–0x670C5604`), not the valid parsed-number path. `operator>>(int&)` treats this nonzero result as conversion failure, writes zero, and updates stream status (`0x670C6125–0x670C6165`). The Extra3D helper's caller-visible return on a malformed value is not equivalent to the reader's status and is not needed to classify the getter boundary.

Sign before prefix is not part of this auto-base hex grammar. Prefix detection begins by requiring the first non-space character to be `0` (`0x670C5350–0x670C537A`). A leading `+` or `-` bypasses this branch; its eventual decimal interpretation and remaining cursor state are not asserted here.

### Overflow consequence

The hex loop visibly shifts the 64-bit accumulator and adds digits without a carry/range check. After parsing, `getInteger` writes the 64-bit accumulated value through its output pointer (`0x670C5969–0x670C5979`). The `int&` operator copies the low 32 bits to the destination (`0x670C6166–0x670C616A`). Therefore, for this getter/operator path, values in the hex branch are accumulated modulo 2^64 and the public `int` result is the low 32 bits. This is a static arithmetic consequence, not an independent runtime fixture result. Examples predicted by the instructions are `0x80000000` → `INT_MIN`, `0xffffffff` → -1, and `0x100000000` → 0. These boundary outputs should be isolated in the next focused test before a candidate implementation adopts the overflow rule.

## Candidate discrepancy and finite matrix

The candidate currently has three exact special cases in `readDecimalInt`: `0x2`, `0x10`, and octal `010`. The original source justifies a single lowercase hexadecimal parser instead of further exact-token exceptions. A finite candidate-only matrix for `SW2SP.DAT` (one integer requested per file) should be:

| Input bytes | Original getter result | Purpose |
|---|---:|---|
| `0x0` | 0 | zero digit |
| `0xa` | 10 | first lowercase alphabetic digit |
| `0xf` | 15 | last accepted lowercase digit |
| `0x10` | 16 | multi-digit accumulation; already implemented |
| `0xdeadbeef` | -559038737 | mixed lowercase digits and low-32-bit conversion |
| `0x80000000` | -2147483648 | signed `int` boundary |
| `0xffffffff` | -1 | high unsigned value narrows to low dword |
| `0x100000000` | 0 | 64-bit accumulator exceeds 32-bit output width |
| `0x10x` | 16; leaves `x` unread | non-whitespace unget boundary in the one-value caller |
| `0x` or `0xg` | conversion failure, output zero | first no-digit boundary; retain caller/status guard |
| `0xA` | 0x prefix branch, then no lowercase hex digit | uppercase digits remain outside the qualified hex set |

The required change should be staged in order: first replace exact `0x2`/`0x10` branches with a bounded lowercase parser for `0x[0-9a-f]+`, allowing the first non-hex suffix to remain unconsumed for the one-value SW2SP reader; next validate overflow examples separately against the existing getter/operator static path and candidate test seam. Keep signed prefixes, uppercase `0X`, uppercase `A`–`F`, binary, other octal forms, and malformed/partial multi-value continuation outside the implementation until each behavior is explicitly covered.

The table predicts behavior from disassembly. It does not claim that arbitrary trailing data is accepted by every multi-value `readInts` use, nor does it resolve sticky status/next-field behavior after a failed extraction. No source changes, build, GUI, or mutable REA action was performed for this audit.
