# CP84 independent review: bounded SW2SP parser source matches the corrected oracle

No integration blocker was found in the declared one-value SW2SP profile. The frozen helper and its 20-row test table implement the corrected PE-derived value/getter/cursor/fresh-status contract, including all four CP83 corrections. This is a static source review; root still owns compilation, focused/full execution and archive binding. It does not establish original-process parity or an exhaustive Qt stream implementation.

Only this report was written. No source/test edit, compiler, executable test, GUI or mutable REA action occurred. All implementation paths below are relative to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst/source/apps/moproj2mst`.

## Reviewed binding

The active file hashes match the source hashes released in `analysis/resume84-sw2sp-parser-implementation.md`:

| Source | Lines | SHA-256 |
|---|---:|---|
| src/engine/simcontrol300_qt_int_parser.hpp | 151 | 7784b177f2df89c2ea8ba413a40b51b5a985a2d40d7f5cae0e21a976ed05a8d7 |
| src/engine/simcontrol300_native_io.cpp | 412 | 40b7accdb7a2a9d63d4b7788755f7ad1f29831c99d42e3aa58276109d0562598 |
| tests/mdt/test_simcontrol300_native_io.cpp | 527 | 1a865df23f8e5e68ef07fc67eeb0f966ad850fb5aba1c7df681ce2e31fbee224 |

The controlling oracle is `analysis/resume83-sw2sp-parser-independent-review.md` and the corrected CP84 plan, qualified to original QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. This review reuses that independently PE-verified evidence; no fresh native execution is implied.

A source comparison against the CP83 archive shows only the new private include and the one-value readInts route in the adapter. Existing open/zero/count/ASCII/path handling, readDecimalInt implementation and other consumers remain unchanged. The existing test target has 19 cases versus 18 in CP83: one new table case with exactly 20 literal rows. No new CTest registration is required.

## Mechanism comparison

| Mechanism | Source conclusion |
|---|---|
| Case folding | qt4AsciiLower is used for both unsigned x/b prefix selection and hex digit classification. 0xA/0XA yield 10; 0X10 yields 16; 0B10 yields 2. It does not apply signed hexadecimal parsing. |
| Base selection | Unsigned second character 0–7 selects octal; 8/9 selects decimal. Other lookahead returns decimal zero with the lookahead unread. A lone zero succeeds. 08/09 and 018 take their distinct corrected paths. |
| Accumulation | Unsigned uint64 multiply/add defines modulo-2^64 wrap. No signed arithmetic overflow is used. Decimal sign handling runs after accumulation. |
| Negative completion | The guard `negative && value != 0 && (value & (uint64_t{1} << 63)) == 0` exactly represents native signed-positive-only negation. High-bit magnitudes remain unchanged; the two critical negative-overflow rows map to +1 and -1. |
| Low-dword result | uint32 truncation followed by bit_cast<int32_t> preserves native low bits without an out-of-range signed conversion. The final cast to this Windows target's int is representable. |
| Successful prefix | Digit loops stop before the first invalid character, preserving its offset. Thus 018 stops at byte 2 and 0b12 at byte 3. Comma is retained as a decimal delimiter. |
| Zero-digit radix failure | Binary restores prefixStart; hex retains digitStart after its two-character prefix. 0b2 and 0b return cursor 0; 0xG and 0x retain cursor 2. Consumed leading whitespace remains consumed. |
| Getter versus operator | Parser retains getterSucceeded separately from value/cursor. The fresh operator maps failure to int zero and status 1 at logical end or 2 otherwise; success stores low bits with fresh status zero. The outer file helper ignores status and returns its existing open-success result. |

I walked each of the 20 manually sourced rows against the helper's actual branches and compared the literals to the independent table. Values, getter outcomes, offsets and fresh statuses agree. The direct test checks sourceQualified, getterSucceeded, nextByte, mapped int and fresh status. The same rows are separately passed through readInts and require true/open-success plus the expected destination value. Expected values are literal table data, not values obtained from the parser under test.

The test checks the extracted low-dword value rather than separately asserting every raw uint64 return pattern. Source inspection confirms the relevant register-width/sign mechanism, and the signed-high-bit rows distinguish the formerly incorrect unconditional-negation implementation. No raw-64 runtime assertion is claimed.

## Outer profile isolation

`readInts` still zeros the span first, opens through the existing reader, returns false on failed open and returns early for an empty requested span. For nonempty spans it applies asciiBytes before routing, so BOM/high-byte/undecoded input does not enter the new helper. The route is exactly `values.size()==1 && path.u16()==u"SW2SP.DAT"`. Other filenames, alternate spellings/path shapes, multiple requested values and SPRING.INI stay on the existing parser; SPRING's allocation-sensitive continuation remains guarded.

The new helper is an internal header. Its production include `engine/simcontrol300_qt_int_parser.hpp` resolves through m2m_core's existing private src include directory in CMakeLists.txt, and the test's relative private include resolves directly. All inspected quoted includes resolve under their actual source/target directories. The new test is Release-active Testkit; no raw C assert, skip or OK-or-unsupported alternative is introduced in the 20 oracle rows.

Legacy literal branches remain in readDecimalInt but are bypassed for the one-value SW2SP route. They are not silently moved into additional consumers. Removing redundant branches is a later cleanup after root validation and must retain other callers' source-qualified boundaries.

## Explicit remaining limits

Initial nonnumeric text and a sign followed by a nonnumeric character are deliberately refused by sourceQualified=false, and the adapter tests explicitly require unsupported for `word\r\n` and `-x10\r\n`. These are conservative candidate scope restrictions, not assertions that original Qt throws or returns an adapter error for those inputs. The original integer operator's zero/status behavior must remain distinct in acceptance wording. The implementation receipt already discloses these refusals; they are not a hidden passing fallback or a blocker for its finite declared profile.

`applyQt4IntOperatorToFreshStream` models an initially zero status. It does not preserve an existing nonzero Qt status or the exact mutable QChar pushback buffer for a subsequent extraction. Its name and documentation correctly bound that claim. One-value readInts never performs a second extraction, and tests do not infer original multi-value equivalence from legacy isolation assertions.

The pure helper also relies on ASCII input and `offset <= bytes.size()` as private preconditions. The active adapter supplies decoded ASCII and offset zero, satisfying them. A future direct caller with an out-of-range offset could index outside the string; document/enforce that precondition before widening reuse. Likewise, helper success on a byte prefix is not a codec qualification; the outer decoder remains required.

Empty file callers skip extraction after atEnd and keep pre-zeroed destinations; whitespace-only nonempty input can enter the helper and fail at logical end. The helper's modeled status is not surfaced in Result or the C ABI. Native file packet/fault timing, Unicode whitespace/digits/codecs, custom locale/options, mutable stream status and original runtime parity remain outside this acceptance.

Root can proceed to the authorized focused build/tests with these hashes. No source correction is required by this review for the admitted contract; only actual source-bound results and full freeze can establish that the authored tests compile and pass.
