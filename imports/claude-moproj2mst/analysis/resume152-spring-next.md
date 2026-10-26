# CP152 next SPRING2 numeric family: explicit-positive decimal narrowing

## Recommended bounded family

The next useful gap is a complete, explicitly positive decimal token in the count-one SPRING2 path: `+[1-9][0-9]*`, magnitude greater than `INT32_MAX` and at most `INT64_MAX`, followed by ASCII whitespace or EOF. This is the positive-sign counterpart to CP151's bounded negative decimal family. Original Qt consumes the plus sign, selects decimal, accumulates the digits, does not apply a negative transform, and then `QTextStream >> int` stores the low 32 bits. The externally visible flag is the signed 32-bit interpretation of `magnitude mod 2^32`.

The primary discriminator is `+4294967297\r\n` -> flag `1`; it has a nonzero flag so an unchanged Save takes the companion writer branch and serializes the normalized decimal flag `1`. A useful high-range boundary is `+9223372036854775807` -> `-1`, with EOF after the token. Keep values above `INT64_MAX`, signed-leading-zero cases such as `+010`, malformed suffixes, and multi-row use guarded.

## Source qualification

The immutable `QtCore4_64.dll` identity is SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; original `DEF_GUI_MO_64.exe` is SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The byte-matched `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` shows:

- Leading ASCII whitespace is consumed at `0x670c5280–52b9` via `QChar::isSpace` called at `0x670c52a0`, before sign parsing.
- The initial sign is compared against the active locale's positive and negative sign characters around `0x670c545f–54d0`. A recognized sign selects base 10, and the sign character is consumed by the common get-char call at `0x670c54d5` before decimal digit parsing.
- Decimal digits accumulate in 64-bit `RDI` with multiply/add at `0x670c56f6–5704`.
- Completion checks whether the consumed sign equals the locale's negative sign; only the negative branch runs signed-positive `test/jle/neg` at `0x670c57bb–57c7`. A positive sign therefore leaves the successfully accumulated magnitude positive.
- `QTextStream::operator>>(int&)` stores the low dword on getter success at `0x670c6166–616a`.

These operations source-qualify the bounded positive-sign mapping without requiring an accumulator wrap. Restrict candidate bytes to ASCII and the Qt default-C stream profile; this does not claim that locale-specific sign characters or non-ASCII digits are modeled. For a complete token, decimal digit consumption leaves the first following non-digit delimiter unread. The original SPRING2 helper requests only one flag for a one-record primary file, copies the integer destination to the record, ignores stream status, and returns success after the companion file opens (`analysis/wave2-save-diffs-300-support-1410da690.c`).

The downstream effect is public: flags are scanned by `FUN_1410db030`, and any nonzero flag causes the companion writer to serialize row flags in decimal (`analysis/resume120-spring2-writer-order.md`). The value above therefore yields a flag of 1 and Save emits `1`, not the original plus-prefixed or large spelling. This is a static prediction, not an original-process run.

## Candidate difference and acceptance proposal

Current frozen reader: `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`, SHA-256 `3D39D2234423D326139EBE3B8FBD2D8782ED8F93B6D6FD247589BB8C21A26FB3`. Focused test: `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`, SHA-256 `EC11894DD863BA9E53E237ABC5EB5CA1108FB3337994FBD7DAA63D2B422F2524`.

Candidate parsing strips `+` only for the bounded `from_chars(int)` path. When a positive magnitude exceeds `INT32_MAX`, its low-dword fallback requires the original prefix to begin with `1`–`9`, so `+4294967297` is rejected. It appears explicitly in the one-row `unsupportedOneRow` fixture list and in a count-two refusal list. The next source cut can add a count-one positive-sign branch after the magnitude-int conversion fails, scanning digits after `+`, requiring a complete ASCII digit run and magnitude <= `INT64_MAX`, then storing the low dword. It must remain gated to `allowSingleRowQualifiedToken` and not alter the separate multi-row tests.

Suggested tests with a complete one-row ASCII primary fixture:

| Companion bytes | Expected flag | Purpose |
|---|---:|---|
| `+4294967297\r\n` | 1 | First mismatch; low-dword and public Save effect |
| `+2147483648\t` | -2147483648 | Intermediate value, tab delimiter |
| `+9223372036854775807` | -1 | Upper admitted magnitude at EOF |
| `+4294967297x\r\n` | guarded | Attached suffix |
| `+010\r\n` | guarded | Leading-zero signed decimal is a separate behavior |
| `+9223372036854775808\r\n` | guarded | Above selected signed-positive interval |
| `++4294967297\r\n` | guarded | Malformed sign sequence |
| two-row `+4294967297\r\n` | guarded | Preserve count-one gate |

Assert exact preservation of both input files for accepted and guarded cases. Existing CP151 negative behavior, CP150 octal behavior, unsigned positive family, comma delimiter and malformed-prefix profiles should stay untouched. No broad parser, locale/sign conversion, BOM, non-ASCII, other filename, or multi-row status/cursor claim follows from this one branch.

## Qualification limit

The machine code strongly source-qualifies the bounded positive-sign arithmetic and reader caller path. It does not qualify positive accumulation beyond `INT64_MAX`, explicit plus combined with radix prefixes, non-C locale signs, or the failed-getter/status consequences of malformed plus tokens. Keep those branches guarded. No source, tests, build, GUI, or mutable REA state changed for this read-only report.
