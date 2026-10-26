# CP84: corrected SW2SP base-0 parser consolidation plan

## Decision

Replace the SW2SP one-token literal ladder with one bounded Qt 4 `QTextStream::getInteger` / `operator>>(int&)` model, but only for the already reachable ASCII, default-C-locale, one-value `SW2SP.DAT` path. Keep the caller policy separate from conversion: the parser returns value, conversion outcome, and next unread byte; the original sidecar reader maps a failed extraction to destination zero, ignores QTextStream status, and returns true after a successful QFile open.

This plan supersedes the behavioral assumptions in `resume83-sw2sp-parser-generalization-plan.md`. Do not implement from that earlier plan.

## Corrected source qualifications

The retained image is QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. `analysis/resume83-sw2sp-parser-independent-review.md` independently rechecked the PE section/export mappings and DUMPBIN instruction bytes: zero mismatches across the bounded `getInteger`, `operator>>(int&)`, and `ungetChar` captures. The raw region SHA values and exact bytes are in that review.

Key corrected branches:

- `QChar::toLower() const` is export target `0x6706A650`, called before base selection at `0x670C53BF`, before testing the hex prefix at `0x670C556B`, and before hex-digit classification at `0x670C559D`. Both prefix and hex digits are case-folded. Thus unsigned `0XA` is a hex prefix, and `0xA` accepts uppercase `A` as 10.
- Leading-zero selection examines the next character around `0x670C5417–5448`. If that character is a digit with value 0–7, the parser selects octal. A second digit of 8 or 9 falls through to decimal. `08` is decimal 8; `018` selects octal on its second character and then ungets the later `8`, returning the accumulated prefix 1.
- For signed decimal accumulation, negation is conditional: the completion code tests the 64-bit accumulator and uses signed `jle` before `neg` (`0x670C57BB–57C7`). With an unsigned C++ representation, negate only when the accumulated bit pattern is nonzero and its high bit is clear. This avoids undefined signed overflow and preserves cases such as `-9223372036854775809` -> int 1.
- Prefix failure rollback is radix-specific. Binary invalid-first-bit handling restores the stored `b` and `0` before returning failure (`0x670C5954–59A5`); hexadecimal no-digit handling leaves the `0x` prefix consumed and ungets the first following character. Do not implement one generic rollback rule.
- The default stream is C locale with zero number-options, as established in `resume67-sw2sp-locale.md` and the QLocale constructor captures. A comma after parsed decimal digits is ungot; it is not consumed as grouping.
- `operator>>(int&)` stores the low dword on success (`0x670C6166–616A`). On failure it writes zero and sets stream status based on the current end state (`0x670C6125–6165`). `FUN_1410da690` requests the SW2SP value and ignores stream status after successful open.

## Corrected one-value oracle table

The following rows are the manually derived source oracle from `resume83-sw2sp-parser-independent-review.md`. “Next byte” is the zero-based unread offset from the start of the displayed token. “Operator status” is Qt's fresh-stream internal status, not the adapter return code. Every one-value SW2SP file read returns true after successful open, including failed extractions, and exposes the operator's destination value.

| Exact token bytes | Value | Getter conversion | Next unread byte | Fresh operator status |
|---|---:|---|---:|---:|
| `0xA\r\n` | 10 | success | 3 (`CR`) | 0 |
| `0XA\r\n` | 10 | success | 3 (`CR`) | 0 |
| `0X10\r\n` | 16 | success | 4 (`CR`) | 0 |
| `0B10` | 2 | success | 4 (EOF) | 0 |
| `08\r\n` | 8 | success | 2 (`CR`) | 0 |
| `09\r\n` | 9 | success | 2 (`CR`) | 0 |
| `018\r\n` | 1 | success | 2 (`8`) | 0 |
| `0b12\r\n` | 1 | success | 3 (`2`) | 0 |
| `0b2\r\n` | 0 | failure | 0 (prefix restored) | 2 |
| `0b` | 0 | failure | 0 (prefix restored) | 2 |
| `0xG\r\n` | 0 | failure | 2 (`G`) | 2 |
| `0x` | 0 | failure | 2 (EOF) | 1 |
| `+0x10` | 0 | success | 2 (`x`) | 0 |
| `-010` | -10 | success | 4 (EOF) | 0 |
| `0` | 0 | success | 1 (EOF) | 0 |
| `+` | 0 | failure | 1 (EOF) | 1 |
| `1,234` | 1 | success | 1 (comma) | 0 |
| `18446744073709551616` | 0 | success | 20 (EOF) | 0 |
| `-9223372036854775809` | 1 | success | 20 (EOF) | 0 |
| `-18446744073709551615` | -1 | success | 21 (EOF) | 0 |

The table is intentionally finite. It supports behavior assertions for the listed bytes and helps derive a parser; it is not an original-process execution record.

## Candidate inventory and intended replacement

`readDecimalInt` currently mixes a generic lowercase-hex branch, C-locale x/X fallback, and strict decimal conversion with exact SW2SP literals for uppercase `0xA`, binary tokens, octal tokens, decimal overflow, and comma prefixes. Some generic branches predate the SW2SP gate; they must not be silently broadened to other filenames during consolidation.

Implement an internal helper that accepts the already-decoded ASCII bytes, starting offset, and one-value profile. Return a result such as `{uint32_t lowBits, bool getterSucceeded, size_t nextByte}`. Keep stream status as a separate mapping in the adapter layer because the one-value caller currently ignores it and the public API does not expose it.

Parsing sequence for the qualified profile:

1. `readInts` continues to zero the requested span, open the file, apply the existing `asciiBytes` guard, and determine the one-value `SW2SP.DAT` profile. Only that profile is routed through the new parser.
2. Skip source-supported ASCII whitespace. Empty file behavior stays at the existing `atEnd`/count gate: the original reader skips extraction and keeps the initialized zero. Do not conflate this with calling the pure parser directly on empty input.
3. Inspect sign before auto-base selection. `+` or `-` selects decimal, including signed leading-zero strings. Signed `0x`/`0X` is still decimal-prefix zero with `x`/`X` unread; it does not become signed hexadecimal.
4. For unsigned input, case-fold the initial candidate prefix with the recovered `QChar::toLower` behavior. Recognize both `0x` and `0X` as base 16, and both `0b` and `0B` as base 2. Otherwise, when the second character is a digit 0–7, use octal; when it is 8 or 9, use decimal. Preserve the special single-zero success branch.
5. Accumulate accepted digits in `uint64_t` with defined modulo-2^64 arithmetic, using each selected base. Hex digit classification case-folds A–F. When a non-digit arrives, apply the correct branch-specific unget/rollback. A nonempty valid digit sequence succeeds even if the next character is invalid for the selected radix; a zero-digit recognized prefix fails.
6. Apply negative sign only when the wrapped accumulator is signed-positive and nonzero: `if (negative && value != 0 && (value & (1ULL << 63)) == 0) value = 0 - value;`. Then keep the low 32 bits and reinterpret as signed int without implementation-defined narrowing.
7. On getter failure, model `operator>>(int&)` writing zero. Model fresh operator status separately from the current logical `atEnd`: end means status 1, otherwise status 2. The SW2SP adapter ignores that status and returns its existing file-open success decision.

The parser should operate incrementally over bytes and retain `nextByte`; do not build a temporary integer string or delegate to `strtol`, `from_chars`, or host C base-0 parsing. Those APIs do not reproduce Qt's case folding, radix selection, rollback, 64-bit wrap, or conditional sign completion.

## Staged migration

1. **Freeze CP83 corrections.** Keep exact witness tests for `0xA`, `0X10`, and `08` at the corrected values. This plan assumes the candidate SHA values in `resume83-sw2sp-fidelity-repair.md`; root must verify build/tests before replacing more code.
2. **Add parser result and helper privately.** Keep it in the engine's internal implementation boundary. Do not change public headers or caller signatures. Add direct table tests for value, conversion outcome, cursor, and status mapping.
3. **Route only one-value SW2SP.** Preserve the existing strict/general branch for other paths and multi-value spans. Keep `SPRING.INI` count parsing separate because it controls allocation and iteration.
4. **Replace literals in groups.** First replace case-folded hex, base selection, octal/binary digit loops and delimiter behavior. Then replace decimal overflow/sign completion. Finally replace comma-prefix literals with the ordinary C-locale decimal non-digit/unget behavior. At every stage compare against the corrected oracle table.
5. **Remove only redundant one-value literals.** Keep test rows as table entries, even when multiple rows exercise the same loop, until the parser-level behavior has been validated by root's release-active build/test run.
6. **Review pre-gate behavior separately.** After one-value SW2SP is stable, determine whether generic lowercase-hex and x/X fallback branches outside the gate should remain as legacy behavior or receive another source-qualified caller scope. Do not move them as incidental cleanup.

## Differential and regression test plan

Build a table-driven unit test over the corrected 20-row oracle above. For each row, assert parser low bits/value, getter success/failure, exact `nextByte`, and fresh operator status. Then assert the adapter mapping: successful file open returns true; destination contains the parsed value on success and zero on failure. This oracle table must remain separate from implementation code so expected values and cursor positions do not call the parser under test.

Additional focused checks:

- Base selection: uppercase/lowercase X and B; `08`/`09` decimal; `018` octal prefix; signed `-010` decimal; signed `+0x10` decimal-prefix zero.
- Hex and binary character folding: `0xA`, `0XA`, and `0B10`; invalid binary first bit rolls back to the start, while invalid hex digit preserves the consumed prefix.
- Delimiter position: CR, LF, EOF, comma, and one invalid radix character after at least one valid digit. Assert the unread offset, not only the resulting integer.
- Overflow/sign: all int32 edges, decimal 2^64 wrap, and both signed-high-bit cases in the table. Add any further large values only after tracing their exact accumulation/completion path.
- Adapter isolation: representative success and failure rows with a different filename and a two-value span must keep their existing candidate guard behavior. Add a SPRING count guard to ensure it is not routed through this parser.
- Release-active test configuration: do not rely on debug-only `assert`; use the existing test framework checks.

Multi-value tests should initially verify only route/gate isolation, not original output equivalence. After the parser models sticky status, failed-read prefix rollback, `atEnd`, and subsequent extraction, a separate qualified multi-value matrix can be proposed. No such behavior follows from this one-value plan.

## Scope and unresolved behavior

- No original-process dynamic comparison is currently available. All expected values and PE facts here are static/offline.
- Parser input is ASCII bytes after the existing candidate decoder. UTF-8/ANSI/ACP conversion, BOMs, malformed byte sequences, Unicode whitespace/digits, or other QTextStream codec branches remain out of scope.
- The original callsite's default C locale and no `setLocale` are established. Custom stream locale, number options, group-size validation, and any locale setter elsewhere are not modeled.
- Multi-value extraction after success or sticky failure, full `QTextStream::status`, `atEnd` after partial reads, and file-device read packet/fault timing are out of scope.
- `SPRING.INI` integer counts, other sidecars, and any `QString::toInt` callsite are separate contracts.
- Preserve path/count/open gates. Do not make a source-faithful parser globally reachable merely because its inner conversion logic is now better understood.
