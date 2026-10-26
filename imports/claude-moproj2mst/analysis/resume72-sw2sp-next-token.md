# CP72: next source-qualified SW2SP token boundary

## Priority: signed leading-zero decimal

The next useful one-value SW2SP token is exact bytes `-010\r\n`. Original QtCore's `QTextStream::getInteger` checks base-0 prefixes only after reading the first nonspace character `0`. A leading sign instead enters the locale sign path at `0x670C545F–54D0`. CP67 establishes that the stream's private constructor uses `QLocale::c()`, whose negative sign is ASCII `-`; the sign match selects decimal base before the decimal parser begins. The decimal loop then reads digits `010`, uses the CR as a delimiter, and applies the negative sign, yielding `-10` with successful extraction. This is the `QTextStream` path at `0x670C5210`, not `QString::toInt`.

The original Extra3D Open fill calls `FUN_1410da690` for exactly one `SW2SP.DAT` integer and formats the result as base-10 text for the control (`analysis/wave2-save-diffs-300-support-140ec8110.c`). Save later serializes the line-edit text (`analysis/wave2-save-diffs-300-support-140ec9040.c`). Thus `-010` is reachable in the same Open→Save path and should display/save as `-10` if unchanged.

Current `readDecimalInt` handles the already-qualified `-0x` fallback, then checks only the unsigned exact octal token `010`, then scans signed decimal. Its leading-zero rejection sees digits `010` after the sign and throws `M2M_E_UNSUPPORTED`. The current test covers unsigned `010` → 8 and ordinary signed `+7`, but not the sign-first decimal case.

## Narrow implementation scope

The source-qualified behavior is a signed decimal token with an ASCII C-locale `+` or `-`, followed by decimal digits beginning with zero, whose value fits the current candidate's supported int32 range. The smallest focused fixture is one-value `SW2SP.DAT` containing `-010\r\n`, expected `-10`. The analogous `+010` is expected to be 10. Keep this separate from unsigned base-0 octal (`010` → 8), preserving its existing branch.

Do not release unrelated parser boundaries as a consequence: leave signed overflow beyond current exact tokens guarded, leave unsigned octal `011` and other octal tokens guarded, and leave suffix-attached forms and grouping comma validation guarded. In particular, the Qt C-locale grouping separator is comma, but the getter has group-size validation logic (`0x670C5706–578B`); the separator alone does not qualify arbitrary grouping. Preserve the one-value SW2SP gate for existing overflow special cases and do not apply them to SPRING counts or multi-value reads.

## Evidence and limits

Original QtCore SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Parser and int-operator captures are `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` and `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`; locale qualification is `analysis/resume67-sw2sp-locale.md`. Original reader count and file binding are in `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` and `analysis/resume54-numeric-path-gap.md`. This is static source behavior, not a fresh original runtime comparison. No source, test, build, GUI, or mutable REA state was changed.
