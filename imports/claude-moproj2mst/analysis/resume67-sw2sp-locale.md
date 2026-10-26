# CP67: Qt 4 QTextStream locale for reachable SW2SP integer extraction

## Finding

The retained shipped QtCore constructs the relevant default stream with a C `QLocale`; the observed process locale is not needed to predict this `QTextStream` integer path unless some other code calls `setLocale` (none appears in the original reader). This resolves CP66's separator/sign dependency for the original `SW2SP.DAT` extraction.

Original QtCore identity is SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Bounded captures retained for this result:

- `resume66-qtextstream-constructor.asm`: public `QTextStream(QIODevice*)` at `0x670C4B00` calls its private initializer.
- `resume67-qtextstream-private-ctor.asm`: `QTextStreamPrivate` at `0x670C2B60` calls the two-argument `QLocale` constructor at `0x6704D5A0` with `language=1`, `country=0`, for its locale member at `private+0xD0`.
- `resume67-qlocale-c-factory.asm`: exported `QLocale::c()` at `0x67001130` calls the same constructor with the same `(1,0)` pair. This ties the private constructor's explicit enum pair to `QLocale::c()` without relying only on a remembered enum value.
- `resume67-qlocale-language-country-ctor.asm`: constructor lookup logic maps the pair to locale data row index zero.
- `resume67-qlocale-sign-separators.asm`: `QLocale::groupSeparator()` reads locale record offset `+8`; `negativeSign()` reads `+0x10`; `positiveSign()` reads `+0x12`.

The QtCore `.rdata` section maps RVA `0x1B2040` to file offset `0x1B0640`. The first locale record at that offset has words:

`language 0001, country 0000, decimal point 002E (.), group separator 002C (,), negative sign 002D (-), positive sign 002B (+)`.

The language lookup table at RVA `0x1B1E50` maps language value 1 to row index 0. Consequently the stream's locale-specific grouping separator and signs are not host ACP or process-locale values. `FUN_1410da690` contains no `QTextStream::setLocale` call; it constructs the stream and extracts values directly.

## Result for the reachable one-integer sidecar

Original `FUN_1410da690` is the QFile/QTextStream integer reader bound to `SW2SP.DAT` by the Extra3D fill (`analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json`, `analysis/resume54-numeric-path-gap.md`). It reads one integer for this call. QtCore's retained default base-0 `QTextStream::getInteger` and `operator>>(int&)` captures are `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` and `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`.

For each ASCII input below, parsing is decimal fallback, not hexadecimal:

| SW2SP.DAT token | Fallback path | Extracted int | Conversion status |
|---|---|---:|---|
| `0X10` | `X` is not the recognized lowercase `x` prefix. Decimal parse reads `0`; `X` is not C-locale group separator `,`, so it is ungot. | 0 | Success; one digit was parsed |
| `+0x10` | C-locale `+` sign is recognized, selecting decimal. Decimal parse reads `0`; `x` is not `,`, so it is ungot. | 0 | Success |
| `-0x10` | C-locale `-` sign is recognized, selecting decimal. Decimal parse reads `0`; `x` is not `,`, so it is ungot; applying negative sign leaves zero. | 0 | Success |
| `+0X10` / `-0X10` | Same sign/decimal path; `X` is ungot after decimal zero. | 0 | Success |

In `getInteger`, lowercase `0x` recognition occurs at `0x670C53D3–53E5`; the uppercase fallback selects decimal at `0x670C5448`; the leading sign path selects decimal at `0x670C54D0`. Decimal digit accumulation and delimiter/group handling are in `0x670C56D8–578B`; because C locale's group separator is comma, the `x`/`X` delimiter is ungot and the already parsed zero reaches the success path. `operator>>(int&)` stores zero on the success path (`0x670C6125–6127`, `0x670C6166–616A`) without setting conversion error. The reader asks for one integer, so the ungot suffix is not consumed by a second extraction.

These ASCII bytes are invariant under the stream's codec for the listed characters; the result does not require ACP qualification. The result is specifically for this QtCore image, default stream construction, no caller locale override, and one requested integer. It does not qualify malformed tokens, overflow, additional fields, or different QString conversion call sites.

## Candidate implication

The current Extra3D candidate parser's signed and uppercase guards are stricter than this original one-integer SW2SP reader. A candidate-only focused fixture can supply exact bytes `0X10\r\n`, `+0x10\r\n`, and `-0x10\r\n` and call `readInts` with a one-int span; the original-static expected value for each is zero. This report performs no source change and does not validate the fixture by running a build or application.
