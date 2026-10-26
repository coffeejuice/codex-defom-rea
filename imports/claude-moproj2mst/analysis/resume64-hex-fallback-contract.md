# CP64: `QString::toInt(base 0)` signed and uppercase fallback

## Source and capture qualification

The retained original `QtCore4_64.dll` is SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Its export table identifies `?toInt@QString@@QEBAHPEA_NH@Z` at RVA `0x0006C280` (preferred VA `0x6706C280`). Bounded DUMPBIN captures are retained as:

- `resume64-qstring-toint.asm` (`0x6706C280–0x6706C450`)
- `resume64-qstring-toint-integer-core.asm` (`0x6706C120–0x6706C280`)
- `resume64-qt-integer-parser.asm` (`0x6704C460–0x6704C620`)
- `resume64-qt-base0-parser.asm` (`0x67053320–0x670534C0`)
- `resume64-qt-base0-parser-tail.asm` (`0x670534B0–0x67053540`)

These are static captures of the original Qt 4.8 binary. They qualify the `QString::toInt` path; they do not change the separately retained `QTextStream::getInteger` analysis.

## Qualified base-0 behavior

The `QString::toInt` wrapper at `0x6706C280` calls the integer conversion helper at `0x6706C120`, forwarding its `base` argument. That helper calls the byte-oriented conversion core at `0x6704C460`. The core converts the QString to a temporary byte representation and invokes the integer parser at `0x67053320`; the parser's return value and success flag are then checked before the public `int` narrowing/range check at `0x6706C280`.

The parser handles an optional ASCII sign before selecting the radix: `-` sets the negative flag (`0x67053362–337B`), and `+` is also consumed (`0x67053386–338D`). With base 0, an initial `0` selects octal unless followed by `x` or `X`; both cases are explicitly accepted at `0x6705339B–33AB`, and the parser sets radix 16. Therefore the smallest locale-independent fallback discriminator is the QString `0X10` with base 0: it parses as hexadecimal 16. The same static flow establishes `+0X10` → 16 and `-0X10` → -16, assuming these ASCII characters survive QString-to-byte conversion unchanged; that is directly true for these ASCII inputs and does not depend on ACP or locale.

The loop accepts decimal digits and alphabetic digits through its character classification calls (`0x67053410–344D`), rejects values outside the selected radix, and accumulates against a sign-dependent bound. Overflow marks the parse flag negative (`0x67053455–347C`). On completion, the parser applies negation for a negative sign and writes the optional success byte based on parse status (`0x670534F5–34FA`). The wrapper rejects values outside signed 32-bit range and clears `ok`; otherwise it returns the parsed int. For `0X10`, the path consumes a valid digit sequence, has no overflow, reports success, and returns 16.

## Important boundary: this does not settle the stream extraction result

This `QString::toInt(base 0)` behavior must not be applied to the SW2SP `QTextStream >> int` path. That getter has a distinct implementation at `0x670C5210` and its retained capture recognizes lowercase `0x` but not uppercase `0X`; it also uses QLocale sign/group-separator handling. The CP63 result about uppercase/sign decimal fallback pertains only to that separate getter. For an actual `QString::toInt` caller, uppercase prefix and sign are now statically qualified. No candidate parser change is proposed here; the next implementation decision must first establish which original call site is being reconstructed.

No source/build/GUI/mutable REA action was performed.
