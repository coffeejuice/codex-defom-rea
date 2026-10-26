# CP124 next `SPRING2.INI` numeric stream branch

## Selected branch: signed negative magnitude outside int32

The next distinct source-closed flag case is the ASCII token `-2147483649`. Qt's `QTextStream >> int` path accepts the minus sign, accumulates the positive magnitude in its 64-bit integer register, applies the negative sign in the signed completion branch, then stores the low 32 bits into the `int` destination. The resulting flag is `2147483647`, with successful conversion status. This exercises sign completion and is distinct from CP123's positive low-dword narrowing.

The current SPRING2 candidate parser rejects this token: its signed decimal `from_chars` int32 conversion fails and the positive-only fallback does not apply to the leading minus. This is a conservative guard, but source evidence supports one bounded ASCII witness.

## Original source path and evidence

Original executable: `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Shipped QtCore: `QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

`FUN_1410da690` (`analysis/wave2-save-diffs-300-support-1410da690.c`) creates a QTextStream and calls the same Qt `operator>>(int&)` for each companion integer. It does not inspect conversion status and increments its loop index after each request. The default stream locale is C (`analysis/resume67-sw2sp-locale.md`), so the ASCII minus sign and digits follow the retained signs/decimal branch without process ACP dependence.

The QtCore getter capture `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` shows sign parsing and digit accumulation at `0x670C5614–5704`, followed by sign completion at `0x670C5799–57C7`. Decimal accumulation is performed in 64-bit `RDI`; the completed negative value is `-2147483649`, still representable in signed 64 bits, so this is a successful parse rather than a getter overflow failure. The operator capture `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` shows the successful path storing the low dword at `0x670C6166–616A`; low 32 bits of `-2147483649` represent `2147483647`. This exact token was independently exercised as a statically qualified one-value SW2SP case in `analysis/resume69-sw2sp-overflow-implementation.md`; the present SPRING2 caller uses the same shipped QtCore extraction operator.

This deduction is static PE evidence, not a fresh original-process run.

## Minimal focused test proposal

Use a valid complete one-record `SPRING.INI`, present ASCII `SPRING2.INI` containing `-2147483649\r\n`, and call `readSprings`. Expected result: success, one record, flag `2147483647`; both input files remain byte-for-byte unchanged. A count-two follow-on fixture `-2147483649 1` should be considered only if the worker wants a second-token cursor regression; the one-value case is the smallest closed test.

Keep other signed overflow magnitudes, signed-plus overflow, leading-zero/radix spellings, malformed attached suffixes, wider accumulator overflow, non-ASCII, and BOMs guarded. The current negative guard accurately contains those unimplemented cases.

No source, test, build, GUI, original-process, or mutable REA state changed for this read-only selection.
