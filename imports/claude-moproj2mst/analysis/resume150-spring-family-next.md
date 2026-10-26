# CP150 next SPRING2 numeric family: one-row unsigned octal tokens

## Recommended bounded class

Beyond CP148's non-leading-zero decimal digits and CP149's exact `019` prefix, the next source-proved family is a one-row, unsigned, complete octal token: exact bytes matching `0[0-7]+` followed by EOF or ASCII whitespace, with no leading whitespace or sign. The initial zero selects base zero; the second octal digit selects base 8; the remaining octal digits accumulate in order. Candidate currently rejects general multi-digit leading-zero tokens, while handling selected one-off witnesses such as `018`/`019` separately.

Small acceptance matrix for a count-one primary row:

| Companion bytes | Original getter result | Next unread position | Fresh status | Public row flag |
|---|---:|---|---:|---:|
| `02\r\n` | 2 | offset 2, CR remains | success (0) | 2 |
| `010\r\n` | 8 | offset 3, CR remains | success (0) | 8 |
| `017\r\n` | 15 | offset 3, CR remains | success (0) | 15 |

The primary `SPRING.INI` remains a complete one-row ASCII fixture. The reader copies the flag into the one row and returns true after successful open. A nonzero flag reaches the retained writer's companion-file branch; an unchanged Save serializes the integer as decimal (`02` input becomes `2`, `010` becomes `8`).

## Immutable Qt evidence

Original identities: `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; `QtCore4_64.dll` SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

`analysis/resume83-sw2sp-parser-independent-review.md` rehashes the Qt PE and documents the leading-zero base decision: a second digit in 0–7 selects octal, while 8/9 selects decimal. The retained byte-matched `getInteger` capture `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` shows:

- base-zero dispatch through octal at `0x670c57cc` after the second digit's 0–7 decision;
- digit conversion and the octal range check `(digit - '0') <= 7` at `0x670c5832–5843`;
- accepted-digit accumulation `rdi = rdi * 8 + digit` at `0x670c5845–5852`;
- invalid-character unget at `0x670c585d–5860` and success when at least one octal digit was accepted at `0x670c5865–5869`.

`analysis/resume83-sw2sp-parser-independent-review.md`'s `018\r\n` table entry already proves the successful octal path can leave its first invalid following byte unread and still return value 1/status 0. The proposed full-token class avoids that malformed-prefix continuation branch: it has only accepted 0–7 digits followed by whitespace/EOF. The int operator stores the low 32 bits on successful extraction (`0x670c6166–616a`). Unsigned octal overflow can consequently be implemented as modulo-32 accumulation, but if root prefers a narrower first increment, constrain total tokens to a proven low-range subset and keep overflow guarded.

`analysis/wave2-save-diffs-300-support-1410da690.c` establishes the reachable companion loop and caller-visible flag effect. `analysis/resume120-spring2-writer-order.md` binds nonzero flags to companion write and decimal serialization. These establish a public Open/Save value difference, not a matched runtime experiment.

## Candidate gap and safe bounds

Candidate `readSpringFlagInt` has a general leading-zero guard after the exact witnesses. Thus `02`, `010`, and `017` are currently refused. The CP149 exact `019\r\n` rule and existing `018` rules must remain behaviorally intact; the proposed family only handles complete valid octal digits and can be limited to `allowSingleRowQualifiedToken`.

Keep signed octal forms, whitespace before the leading zero, no-digit prefixes, trailing 8/9 partial forms, binary/hex prefixes, comma/grouping, non-ASCII/codec profiles, other filenames, and multi-row octal parsing guarded. In particular, do not generalize the current count-two exact `018` continuation profile while adding the count-one complete-token family. A focused test should check `02`, `010`, and `017` values, raw input preservation, and refusals for count-two `010`, `-010`, `+010`, ` 010`, `0180`, and `010x`.

## Source bindings

- Current reader: `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` — `E8CEF08D79CC152026C432BDB012D0017BCDB10FDCF53859F382AF58046A7759`
- Current focused test: `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` — `4E8D3A49F1392B4C2BCC2A5AED8B01E1448DE4ABEE60F027CAD2991206757811`
- CP149 source receipt: `analysis/resume149-spring-019-implementation.md` — `D892C8548EC3FF368A99A854288CD04F802A48124C441775D6CE362731F6A35E`
- CP148 positive decimal implementation: `analysis/resume148-spring-decimal-implementation.md` — `5C24F9795562D050F65B9919E19739364914A4800872D7EB04FD8E4689E87993`
- Writer behavior: `analysis/resume120-spring2-writer-order.md` — `AA90D258C00350BCF1DBDF57B37C7EF4A696D16CAFBFC4BB02A2BB5E88B5C896`

No source, test, build, GUI, original runtime, or mutable REA state changed for this read-only report. It preserves CP149 files frozen under root's current validation. Root owns assignment and validation.
