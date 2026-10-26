# CP83: consolidate SW2SP integer special cases into a bounded Qt parser

## Recommendation

Yes, the exact-token ladder can be replaced with one small source-faithful integer parser for the reachable **ASCII, default-C-locale, one-value `SW2SP.DAT` profile**. Retained QtCore disassembly exposes the important base-selection, digit-loop, delimiter, result-narrowing, and failure branches. The helper can therefore model the original `QTextStream::getInteger` and `operator>>(int&)` behavior without pretending to implement all Qt stream codecs or all callers.

Keep the profile boundary explicit: route only `values.size() == 1 && path.u16() == u"SW2SP.DAT"` through the new parser and caller mapping. Preserve the existing conservative path for multi-value requests, `SPRING.INI` counts, and unrelated integer files until their caller-specific effects are separately covered. The original helper initializes the destination, ignores QTextStream status, and returns true after a successful QFile open; that caller policy is distinct from the conversion helper's result/status.

## Current integer special-case inventory

`readDecimalInt(bytes, cursor, allowSw2spOneIntProfile)` currently interleaves a general lowercase-hex parser, a generic uppercase/signed `x` decimal fallback, strict `qt::toInt` decimal parsing, and one-value SW2SP literals. The current hardcoded additions are:

| Current behavior | Scope in candidate | Qt mechanism that can replace the literal |
|---|---|---|
| `0x...` lowercase hex scans digits, wraps `uint64_t`, then narrows low 32 bits | Currently before the SW2SP gate; can affect other callers | Base-16 digit loop, first nonhex unget, 64-bit accumulation, then int operator narrowing |
| Exact `0xA\r\n` returns zero | One-value SW2SP only | Recognized `0x`, zero accepted hex digits because uppercase A is not in the lowercase `a`–`f` test; operator writes zero and records failure; caller ignores status |
| Exact `0b10` -> 2 | One-value SW2SP only | Base-2 loop with `2 * value + bit` |
| Exact `0b2\r\n` -> 0 | One-value SW2SP only | Base-2 prefix, invalid first bit, zero accepted digits -> extraction failure -> output zero |
| Exact `0b12\r\n` -> 1 | One-value SW2SP only | Accept bit 1, unget invalid bit 2; nonzero digit count succeeds with accumulated prefix |
| Exact `08\r\n` -> 0 | One-value SW2SP only | Leading-zero base-8 branch, first digit 8 invalid, zero accepted digits -> failure -> output zero |
| Exact `018\r\n` -> 1 | One-value SW2SP only | Accept octal digit 1, unget invalid 8, succeed with parsed prefix |
| Exact `02` -> 2 and `010` -> 8 | `02` one-value gate; `010` currently lacks that gate | Generic base-8 loop, `8 * value + digit` |
| Exact `040000000000\r\n` -> 0 | One-value SW2SP only | Octal `4×8^10 = 2^32`, 64-bit parse, low-dword int assignment |
| Exact decimal `18446744073709551616` -> 0, `2147483648` -> INT_MIN, `-2147483649` -> INT_MAX | One-value SW2SP only | Unsigned 64-bit decimal accumulator wrap, sign application, then low-dword assignment |
| Exact decimal prefixes `1,234` -> 1 and `2,345` -> 2 | One-value SW2SP only | C-locale number-options comparison skips grouping consumption; comma is ungot as any other non-digit after a nonempty decimal prefix |
| `0X...`, `+0x...`, `-0x...` -> decimal prefix zero | Generic current branch | Prefix recognition is lowercase `x`; sign chooses decimal; zero is parsed and x/X is ungot |
| Signed leading-zero decimal, e.g. `-010` -> -10 | One-value SW2SP only in final decimal scanner | Sign is checked before base selection; signed forms use decimal, unsigned leading-zero forms use octal |

This inventory exposes two maintenance risks. First, each literal is a growing fragment of one base-0 state machine. Second, some branches (notably generic lowercase hex and the x/X fallback) are not restricted to the one-value SW2SP gate, even though other surrounding parser additions are.

## Source-qualified parser contract

The relevant image is shipped QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The primary bounded captures are `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` (`QTextStream::getInteger`, `0x670C5210`) and `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` (`operator>>(int&)`, `0x670C60E0`). `analysis/resume67-sw2sp-locale.md` establishes `QTextStreamPrivate` constructs `QLocale::c()` and the reader does not override it. `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` binds the original helper to one SW2SP integer and shows it ignores stream status after open.

For the bounded ASCII/default-C profile, a replacement should model these parts:

1. **Whitespace and base selection.** Skip the source-supported ASCII whitespace subset. An initial sign selects decimal. Otherwise leading `0` selects the automatic-prefix decision: lowercase `0x` selects hexadecimal, lowercase `0b` selects binary, and other leading-zero numbers use octal. Nonzero-leading values use decimal. Uppercase prefix spellings do not select the radix path.
2. **Digit classes and accumulation.** Decimal accepts `0`–`9`; octal accepts `0`–`7`; binary accepts `0`–`1`; the recovered hex loop accepts `0`–`9` and lowercase `a`–`f`. The getters accumulate in a 64-bit register using base multiply/add; implement that arithmetic with `uint64_t` so wrap is defined. The sign is applied in the recovered decimal path. The `int&` operator stores the low 32 bits on success.
3. **Delimiter and partial-prefix behavior.** The first non-digit is ungot. For the default stream C number-options value, comma is not consumed as a group separator. Any recognized token with at least one accepted digit succeeds with its accumulated value, even when the next byte is a non-digit such as `8` in octal `018` or `2` in binary `0b12`. A recognized radix prefix with zero accepted digits follows the getter failure path; the `int&` operator writes zero and updates status. The one-value sidecar helper ignores status, so its observable result remains success/value zero after a successful open.
4. **Caller policy and cursor.** Return parser outcome separately from the integer, at least as `{value, conversionSucceeded, nextByte}`. That allows one-value SW2SP to preserve its ignore-status/value-zero behavior while preventing the parser itself from disguising conversion failure. It also makes unget position testable. Do not expose or flatten Qt stream status into the public adapter result.

This is not a full Qt decoder. Keep non-ASCII bytes, BOM/codec conversion, non-C locale/options, malformed multibyte input, and other streams outside this profile. Qt’s actual input is QChar-based; this candidate path has an ASCII byte reader. The existing input gate must continue to reject undecoded bytes rather than silently claim codec fidelity.

## Safe migration sequence

1. Add an internal pure parser helper with no QFile, GUI, or public API dependency. Route only one-value SW2SP through it. Keep the existing parser for every other path/shape during the first change.
2. Have the helper return both the low-dword integer and conversion/cursor result. In the SW2SP adapter, map a parser failure to output zero and continue returning open success, matching `FUN_1410da690`; preserve thrown/guarded behavior elsewhere.
3. Replace only the SW2SP exact-token blocks with the state machine. Do not move generic lower-hex behavior into more callers as part of cleanup. After the SW2SP route is proven, review the current pre-gate lower-hex and x/X branches separately; they may need to stay as legacy behavior or move under a distinct qualified caller profile.
4. Keep the existing scalar exact fixtures as transition coverage temporarily, then reduce them to table-driven cases against parser state and caller mapping. Do not delete path/count guards as part of helper refactoring.

## Broad differential test matrix

Tests should be release-active and table-driven. Each row should assert input bytes, result value, parser success/failure, cursor offset after unget, and adapter return. The expected table should be manually derived from the disassembly branches above; do not use the implementation itself as its oracle. Include:

- **Radix selection:** decimal `17`; signed decimal `-17`, `+17`, and signed leading zero `-010`; unsigned octal `02`, `010`, and one multi-digit valid octal; lowercase hex `0x2`, `0xa`; lowercase binary `0b10`.
- **Distinct invalid and partial paths:** zero-digit failure `0xA`, `0b2`, and `08`; accepted-prefix success `018`, `0b12`, and valid digits followed by CR or another ungot delimiter. Assert that caller-facing one-value SW2SP returns success/zero on failure while parser outcome remains failure.
- **Delimiters:** CRLF, LF, EOF, and comma/non-digit unget after at least one valid digit. Check `nextByte` points at the exact unconsumed delimiter. Do not treat comma as grouping.
- **Narrowing/wrap:** signed 32-bit limits, `2147483648`, `-2147483649`; exact modulo-2^64 decimal boundary and one additional decimal digit; octal `040000000000`; binary low-dword boundary; hex low-dword and wrap boundary. The current evidence supports the arithmetic mechanism, but each chosen large literal should be independently checked in the same disassembly path before converting it into a fixed expectation.
- **Guard preservation:** repeat representative success and failure inputs on another filename and on a two-value request. Confirm `SPRING.INI` count parsing remains on its separate guarded path. Add a mixed whitespace/multi-token adapter test only after sticky QTextStream status and next extraction are modeled; it is not necessary for the initial one-value parser.
- **Input boundary:** retain unsupported tests for high bytes, BOMs, and non-ASCII whitespace until QFile/QTextStream byte decoding is separately implemented. ASCII-only evidence does not establish ANSI/ACP or UTF-8 decoding.

## Evidence boundary and open items

The parser state machine and one-value caller behavior are source-qualified by shipped QtCore and original app decompilation, but there has been no original-process differential run. Before calling this “Qt-equivalent,” verify any proposed overflow boundary against the exact register-width and signed-completion instructions; test the parser's status mapping without pretending the public adapter exposes QTextStream status. Multi-value requests remain out of scope because a sticky stream error and `atEnd()`/loop interaction can affect subsequent destination values. `SPRING.INI` also remains separate because its parsed integer controls allocation.

No source or test changes were made in this planning pass.
