# CP67 implementation: SW2SP C-locale prefix fallback

Implemented the source-qualified `QTextStream` fallback in the existing Extra3D native reader. The original `QTextStreamPrivate` initializes its locale using `(language=1,country=0)`, matching the shipped QtCore `QLocale::c()` factory; locale record zero supplies comma grouping and ASCII signs. See `resume67-sw2sp-locale.md` and bounded captures `resume67-qtextstream-private-ctor.asm`, `resume67-qlocale-c-factory.asm`, `resume67-qlocale-language-country-ctor.asm`, and `resume67-qlocale-sign-separators.asm`.

## Change

`readDecimalInt` still handles the previously qualified unsigned lowercase hex parser first. A new narrow fallback recognizes only an uppercase `0X` prefix or ASCII sign followed immediately by `0x`/`0X`. It returns integer zero and advances the cursor only past the optional sign and the decimal zero, leaving `x`/`X` unread as the original decimal fallback does. It does not parse the suffix as hex. Zero-digit lowercase hex, uppercase A–F digits, other signed numeric forms, unrelated radix forms, overflow, and non-ASCII input remain guarded or follow their earlier behavior.

Tests use the actual native adapter and `SW2SP.DAT` sidecar path. They cover missing-file/open behavior and the one-integer inputs `0X10`, `+0x10`, `-0x10`, `+0X10`, and `-0X10`, each expecting successful read and value zero. A two-value request with `0X10 7` deliberately remains guarded at the unread `X`, checking the cursor boundary without broadening continuation semantics. Existing decimal `+7`, lowercase hex, octal, and malformed guards remain in place.

## Validation and freeze

No build or CTest was run; root validation remains pending. The only owned source/test files changed are:

- `src/engine/simcontrol300_native_io.cpp`: SHA-256 `8040314C0364FE39D5B100DB76DF2981A1E09D5D0230E7636FA62846BF5F940B`
- `tests/mdt/test_simcontrol300_native_io.cpp`: SHA-256 `8453A93297A181B3337016DDAB9DE054B31AB09B4100D3075EABC568129A71F7`

No GUI, mutable REA, or project fixture changes were made.
