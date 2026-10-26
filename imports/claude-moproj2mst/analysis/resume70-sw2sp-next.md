# CP70: next SW2SP integer narrowing case

## Selected next case

The smallest positive decimal value above `INT_MAX` is the best next exact SW2SP case: file bytes `2147483648\r\n`, one requested integer. Original QtCore's `QTextStream::getInteger` accumulates decimal digits into a 64-bit value using `value = 10 * value + digit` (`analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`, `0x670C56F6–5704`) without a signed-32 range check. The value `2147483648` fits the 64-bit accumulator. After successful extraction, `QTextStream::operator>>(int&)` copies the low dword (`analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`, `0x670C6166–616A`), yielding signed `int -2147483648`.

The stream is constructed with `QLocale::c()` and no locale override in the original reader (`analysis/resume67-sw2sp-locale.md`). This token has no sign, grouping mark, or radix prefix, so the result does not depend on host locale, ACP, or grouping. `FUN_1410da690` requests one SW2SP integer and does not inspect QTextStream status after extraction (`analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json`). Original fill formats the resulting int in base 10 into the line edit (`analysis/wave2-save-diffs-300-support-140ec8110.c`); the Save-side writer consumes that text (`analysis/wave2-save-diffs-300-support-140ec9040.c`). Thus this is a reachable Open→Save distinction, not an isolated QString parser claim.

The current candidate special case is limited to `-2147483649` for a one-value SW2SP read. Its ordinary decimal path delegates to `qt::toInt`, which rejects `2147483648` as out of signed-int range. A future candidate-only one-int fixture should therefore expect `-2147483648` for this exact input, but keep the implementation bounded until integrated validation.

## Overflow extent already established

For decimal magnitudes that remain representable in the getter's signed 64-bit accumulator, the static data flow is: digit accumulation without an `int32` check → optional negation for C-locale `-` → 64-bit output storage → low-32-bit `int&` assignment. Therefore other exact predictions include `4294967295` → `-1`, `4294967296` → `0`, and `-4294967296` → `0`. These are static arithmetic consequences of the same branch, not separately observed runtime fixtures.

Do not extend this rule to decimal magnitudes that overflow the 64-bit accumulator. The accumulator has no saturation check in the digit loop, so beyond-64-bit behavior follows machine wrap and the later negative-sign handling; a safe general model and relevant edge tests have not been completed. Keep signs before base-0 prefixes as the separately qualified decimal fallback: `-0x...` does not enter the recognized lowercase hex loop.

## Multi-value and remaining guards

`FUN_1410da690` initializes the requested `int` destinations to zero, then loops while the stream is not at end and the requested count is not reached. It does not test stream status or conversion success after `operator>>(int&)`; the count index advances after each extraction. A valid whitespace-delimited sequence such as `2147483648 7` therefore follows the same two successful extractions at the parser level: first low32 result `INT_MIN`, then 7. The current CP69 exception intentionally does not cover a two-element span; no multi-value low-dword implementation is proposed here.

Keep the following branches separate:

- **Grouping:** C-locale group separator is comma, but the getter consults group-size metadata before accepting it (`0x670C5706–578B`). Do not accept comma-separated forms without a tested placement matrix.
- **Uppercase hex digits:** `0xA` enters lowercase-hex prefix dispatch but does not increment the digit count. `getInteger` returns conversion failure; `operator>>(int&)` writes zero and updates status. The one-int caller does not inspect status, so its destination remains zero, but that failure state is not equivalent to a successful decimal parse.
- **Multi-value malformed continuation:** after a failed extraction the outer helper increments its count index and asks `atEnd` again. Exact subsequent status/output behavior was not needed for the selected one-value case and remains unqualified for candidate parity.
- **SPRING.INI:** never reuse the SW2SP low-dword fixture as a count test. Its count controls allocation and record iteration; positive narrowed values can have large resource effects. Retain current guards pending a separate spring-count contract.

No source, tests, build, GUI, or mutable REA state was changed. This analysis uses the retained original QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and the cited original PE decompilations; no original runtime observation is claimed.
