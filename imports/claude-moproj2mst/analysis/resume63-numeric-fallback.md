# CP63: signed and uppercase hexadecimal fallback

## Scope and evidence

This is a static audit of the retained original QtCore image, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The bounded captures are `resume63-getinteger-autobase-fallback.asm`, `resume63-getinteger-decimal.asm`, `resume63-qchar-classifier.asm`, and `resume63-int-operator.asm`. The relevant getter is `QTextStream::getInteger` at `0x670C5210`; `operator>>(int&)` begins at `0x670C60E0`. Original Extra3D's integer extraction is statically bound to this path by `analysis/resume54-numeric-path-gap.md` and the retained standalone decompilation.

No runtime locale observation or candidate/source change was made.

## What the original getter proves

The base-0 prefix dispatch first checks for leading `0`, then recognizes lowercase `x` (and separately lowercase `b`). An uppercase `X` does not take the hex branch. It falls through to QChar classification; `QChar::isDigit` and `digitValue` are called at `0x670C5417` and `0x670C5428`. A non-digit/negative digit value selects base 10 at `0x670C5448`. The second character is ungot at `0x670C5458`, then the first character is ungot at `0x670C54D5`, before the decimal parser starts. The static flow therefore establishes that `0X...` is not recognized as an uppercase hexadecimal prefix.

A sign as the first non-space character bypasses the leading-zero prefix decision. At `0x670C545F–0x670C54A8`, the getter compares it against QLocale's positive and negative sign characters; a matching sign selects base 10 at `0x670C54D0`. It does not restart base-0 prefix detection after consuming a sign. Thus `+0x10` and `-0x10` are not signed forms of the recognized lowercase-hex branch.

In decimal mode, the getter accepts the locale sign at `0x670C5639–0x670C5667`, accumulates decimal digits (`value * 10 + digit`, `0x670C56D8–0x670C5704`), and then checks a locale grouping separator. If a non-digit is not accepted as a grouping separator, it is ungot (`0x670C576A` onward). When at least one digit was collected, the path continues to successful result storage (`0x670C5790` and later). This indicates that under a locale where ASCII `X`/`x` are neither grouping separators nor signs, `0X10` and `+0x10`/`-0x10` parse the decimal prefix `0`, leave the `X`/`x` unread, and return numeric zero. The exact locale-conditioned result is not asserted as universal.

`operator>>(int&)` checks `getInteger`'s return code at `0x670C6125`. A successful conversion copies the low dword to the requested `int` (`0x670C6166–0x670C616A`). A failed conversion writes zero and sets stream status (`0x670C6129–0x670C6165`). Because the leading decimal zero is already a digit, the uppercase/signed examples above do not inherently take the no-digit error path when the delimiter/grouping checks permit the prefix conversion.

## Candidate boundary and next action

The CP61 candidate deliberately implements the source-qualified lowercase `0x[0-9a-f]+` form. Keep signed and uppercase forms outside that parser until original process locale behavior is qualified. The smallest next evidence step is a read-only static capture of the original application/Qt initialization path that establishes the locale used by these `QTextStream` instances (including positive/negative signs and group separator), or a controlled original-process fixture observation for exactly `0X10`, `+0x10`, and `-0x10` in the one-integer SW2SP reader. Do not add parser behavior from C-locale assumptions alone.

Static evidence is sufficient to reject uppercase `0X` as a hex-prefix spelling and to show that a leading sign selects decimal rather than signed hex. Exact output/cursor/status remains conditional on locale details and, for a full reader result, caller handling. No source/build/GUI/mutable REA action was performed.
