# CP83 independent review: correct the numeric oracle before parser generalization

The proposed consolidation is sound only after correcting its native oracle. CP84 must not implement the plan as written: it misidentifies case-folded hexadecimal input, leading-zero decimal fallback, signed overflow completion and zero-digit prefix rollback. Three existing accepted numeric witnesses have wrong expected values. Retain the one-value SW2SP/ASCII/default-C profile while repairing those values, then generalize from the corrected state machine.

No source, test, build, GUI, original execution or mutable REA changed. Only this report was written. Paths are relative to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst`.

## Independently bound evidence

Reviewed plan SHA-256: `b20842aabf3585330ba20d6e1802a1b150a0e63eb4acd0d0145175dc9699e70f`. Retained getInteger capture SHA-256: `f7d4f60a403016cd75528362c1f314307a8472e2e63ce71cc4772d1815e177c7`; int-extraction capture SHA-256: `b8a9fcb3e9fd8bdcdd615248008f0df1713c6e72eb7200c903bb73458e92f73b`.

The actual QtCore4_64.dll rehash is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Python read the PE section/export tables directly. Existing DUMPBIN `/disasm:bytes` independently recaptured these bounded regions in memory; every emitted instruction byte was compared to the PE mapping:

| Region [start,end) | Instruction records / bytes | Mismatches | Raw region SHA-256 |
|---|---:|---:|---|
| getInteger [0x670c5210,0x670c59aa) | 440 / 1,946 | 0 | 4342e280b98efd16708ccedfb5e0f885107695b69c455f05cde4a5ecd707139e |
| operator>>(int&) [0x670c60e0,0x670c617a) | 46 / 154 | 0 | 8024a126fa8c8878a35799b521c7bc0a4242df618fd9cf1f61f7405a8dea0430 |
| ungetChar [0x670c20f0,0x670c21c3) | 62 / 211 | 0 | 022224a5575b1069325bf5ba3b1a14ff254bd00301c0af5ea2a950fe820170b4 |

This is offline static verification, not an original-process differential test. The available reverse-engineering skill had already been read; no live analysis session was used.

## Blocking corrections

**1. Hex prefix and digits are case-folded.** Actual Qt exports resolve 0x6706a650 to `QChar::toLower() const`, 0x67069e70 to isDigit, 0x67069f00 to digitValue and 0x67069c80 to isSpace. getInteger calls toLower at 0x670c53bf before prefix selection, 0x670c556b before validating the hex prefix, and 0x670c559d before classifying each hex digit. The latter then tests the normalized character against 0–9/a–f. Therefore unsigned 0X10 is hexadecimal 16 and 0xA is hexadecimal 10. Uppercase B is also accepted in the unsigned binary prefix. The plan's lowercase-only and uppercase-A failure assertions are contradicted by the named call targets and their positions.

**2. A leading zero does not always select octal.** After peeking the next character, 0x670c5417–5446 tests isDigit/digitValue and uses octal only for a second digit in 0–7. A digit above 7 falls through 0x670c5448, `mov ebx,0ah`, to decimal. Thus 08 and 09 parse as decimal 8 and 9. In 018 the second character is 1, selecting octal; the later 8 is a delimiter and the result is 1. The alleged zero-digit octal failure for 08 is not reachable in this base-0 selection path.

**3. Decimal sign application is conditional on signed 64-bit magnitude.** Decimal accumulation uses 64-bit LEA multiply/add, so modulo-2^64 arithmetic is appropriate. However completion is `test rdi,rdi` at 0x670c57bb, signed `jle` at 0x670c57be and `neg rdi` at 0x670c57c4. For a negative sign, negation happens only when the wrapped accumulator is signed-positive. It is not unconditional unsigned `0-value`. For example, -9223372036854775809 leaves bits 0x8000000000000001 unnegated and yields int +1; -18446744073709551615 leaves all-one bits and yields int -1. The already accepted -2147483649 → INT_MAX is unaffected. Implement this condition by testing the unsigned magnitude's high bit and nonzero value, avoiding C++ signed overflow/undefined negation.

**4. Prefix rollback differs by radix on zero-digit failure.** Binary invalid first digit is ungot at 0x670c595f; zero accepted digits then enter 0x670c5980, unget the stored b at 0x670c598b and the stored leading 0 at 0x670c599b. Therefore 0b2 and bare 0b roll back to the start of the prefix, after any already-consumed whitespace. Hex failure leaves the 0x prefix consumed; only an invalid following digit is ungot. The unget implementation decrements the buffer position and writes the supplied QChar into the buffer, so a future multi-read model may need normalized-character pushback as well as an offset. A one-value parser can report its next offset, but must not claim a complete reusable Qt stream state.

## Corrected manually derived one-value table

Offsets are zero-based next unread ASCII byte positions in the displayed token, with no leading whitespace. A fresh successful int extraction leaves stream status zero. For getter result 1 or 2, operator writes destination zero and selects status 1 if atEnd is true, otherwise 2; those numeric status values are internal observations, not adapter result codes. The file helper returns true after a successful open even for extraction failure.

| Bytes (escaped) | Extracted int | Getter success | Next byte offset | Fresh operator status |
|---|---:|---|---:|---:|
| 0xA\r\n | 10 | yes | 3 (CR) | 0 |
| 0XA\r\n | 10 | yes | 3 (CR) | 0 |
| 0X10\r\n | 16 | yes | 4 (CR) | 0 |
| 0B10 | 2 | yes | 4 (EOF) | 0 |
| 08\r\n | 8 | yes | 2 (CR) | 0 |
| 09\r\n | 9 | yes | 2 (CR) | 0 |
| 018\r\n | 1 | yes | 2 (8) | 0 |
| 0b12\r\n | 1 | yes | 3 (2) | 0 |
| 0b2\r\n | 0 | no | 0 (0, prefix restored) | 2 |
| 0b | 0 | no | 0 (0, prefix restored) | 2 |
| 0xG\r\n | 0 | no | 2 (G) | 2 |
| 0x | 0 | no | 2 (EOF) | 1 |
| +0x10 | 0 | yes | 2 (x) | 0 |
| -010 | -10 | yes | 4 (EOF) | 0 |
| 0 | 0 | yes | 1 (EOF) | 0 |
| + | 0 | no | 1 (EOF) | 1 |
| 1,234 | 1 | yes | 1 (comma) | 0 |
| 18446744073709551616 | 0 | yes | 20 (EOF) | 0 |
| -9223372036854775809 | 1 | yes | 20 (EOF) | 0 |
| -18446744073709551615 | -1 | yes | 21 (EOF) | 0 |

The special single-zero success is explicit at 0x670c539d–53aa when the second probe reaches EOF. Signed 0x forms remain decimal because the sign chooses decimal before automatic prefix selection. These signed cases must not be changed to signed-hex support. The default-C comma delimiter conclusion is consistent with the locale/options comparison at 0x670c5706–578b; it remains distinct from locale-agnostic grouping support.

## Exact affected source and tests

Current inspected snapshot: `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` SHA-256 `d091405c36aa0da0cf13c72e6efe16e9b47f0cc2f4e19fd9b4ac4868ae4dfdb5`; `tests/mdt/test_simcontrol300_native_io.cpp` SHA-256 `f02b6bb68147229b4412cb562a40c3b461c244000e8e11b2849fe1c25664fe7f`. They are review bindings, not a source/build freeze.

| Existing location | Required smallest CP83 repair |
|---|---|
| readDecimalInt near lines 100–138, exact sw2spUpperHexFailure block | Exact admitted one-value `0xA\r\n` must yield 10, not 0; repair the toLower/lowercase-only comments. Do not promote unrelated legacy caller behavior during this repair. |
| readDecimalInt generic x/X fallback near lines 199–215 | Unsigned admitted `0X10\r\n` needs 16 in the one-value SW2SP profile. Signed +0x/-0x/+0X/-0X forms keep zero. Preserve other path/count guards. |
| readDecimalInt exact sw2spInvalidOctalFirstDigit block near lines 175–184 | Exact admitted `08\r\n` must yield 8; it is decimal fallback, not extraction failure. |
| native_extra3d_sw2sp_reads_the_qualified_lowercase_base0_hex_profile, lines 110–150 | Replace the `0xA\r\n` zero assertion/comment with 10. Other previously guarded spellings may remain deliberate scope refusals for CP83, but are not native invalidity assertions. |
| native_extra3d_sw2sp_c_locale_prefix_fallback_reads_one_sidecar_integer, lines 152–180 | Split unsigned `0X10` from the signed fallback rows: expected 16 versus zero. Its two-value legacy assertion is outside the recovered one-value profile and must not be described as original first-value zero behavior. |
| native_extra3d_sw2sp_invalid_octal_first_digit_is_zero_in_one_value_profile, lines 267–281 | Expect 8 and rename to describe leading-zero decimal fallback. Keep other-path/multi-value refusals. |

The binary-invalid-zero result, 018 prefix result, 2^32 octal narrowing, signed-leading-zero decimal and existing 32-bit overflow results remain consistent. Their cursor/status mechanisms still need the corrected helper tests before broad generalization. Some current generic lowercase-hex/x-X branches predate the SW2SP guard; moving their behavior is separate scope and should not accompany the immediate three-witness repair without a distinct caller contract.

The conflicting historical prose is explicitly superseded in the affected claims: `resume67-sw2sp-locale.md` and its implementation report (unsigned uppercase fallback), `resume74-sw2sp-uppercase-hex-failure.md` (uppercase digit failure), `resume80-sw2sp-octal-invalid-next.md` and implementation report (08 octal failure), and the reviewed CP83 generalization plan. Preserve those receipts as history; do not use their wrong expectations as an independent oracle.

## CP84 go/no-go and guard requirements

Before implementing the general parser, freeze a corrected manually sourced table including at least uppercase prefix/digit, 08 decimal versus 018 octal, signed prefix fallback, binary/hex asymmetric rollback and the signed-high-bit overflow examples above. The pure helper should return value, getter outcome and cursor separately; model operator status separately with initially-zero status and logical atEnd. The operator checks prior status at private+0xcc before assigning a new nonzero status, and stores low dword on success at 0x670c6166–616a. Do not collapse getter failure into a claimed public error: the one-int caller initializes destinations, calls extraction only when not atEnd, ignores stream status and returns true on open success.

Retain the exact `values.size()==1 && path==SW2SP.DAT`, ASCII, default-C-locale and ordinary file-open boundaries. Do not infer another extraction after a failed prefix, general SPRING allocation-count support, other filenames, BOM/codecs/non-ASCII or caller-specific multi-value behavior. Empty-file caller pre-zero/atEnd skipping is distinct from a pure helper called on empty input. Exclude unsupported decoder inputs before parsing as today. Use uint64 arithmetic, explicit low-32-bit reinterpretation and the signed-positive-only negation rule; do not use strtol/from_chars or C base-0 assumptions as a substitute for these branches.

Once those corrections are accepted, one bounded parser is preferable to adding literals, but a broad rewrite preserving today's numeric expectations would encode three established defects and introduce overflow/cursor errors. Root should complete the narrow CP83 source/test correction and its focused/full freeze before CP84 consolidation. No new test execution, parity acceptance or completion is claimed here.
