# CP120 `SPRING2.INI` malformed integer and trailing-token audit

## Result

The candidate's guard for malformed companion tokens is conservative, but it does not reproduce all original outcomes. The original `FUN_1410da690` ignores QTextStream conversion status and increments its count index after each attempted extraction. Qt's integer getter can consume a valid decimal prefix and unget a nonnumeric suffix. When the count limit is reached immediately after that successful prefix, the suffix is never inspected. If another extraction is requested while the suffix remains, it fails, writes zero to that destination, and the reader continues its loop. Therefore an attached suffix can produce valid-prefix flags followed by zero flags rather than a read failure.

By contrast, candidate `readSpringInt` requires the whole whitespace-delimited token to match the canonical signed decimal int32 grammar. Inputs such as `1x` are refused when they fall within the requested count. This is an explicit unsupported-input boundary, not a claim of full Qt error/status parity. The candidate does match the ordinary canonical profile: complete tokens map in order, EOF-short suffixes remain zero, and extra tokens after `count` are ignored.

## Source-qualified reader behavior

Original application identity: `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

`analysis/wave2-save-diffs-300-support-1410da690.c` shows the companion helper zeroes every destination first; on successful QFile open, its loop tests `!atEnd()` and `i < count`, calls `QTextStream >> int`, increments `i` without examining the extraction result, then returns true. It has no final EOF check. `analysis/wave2-save-diffs-300-support-1410dab40.c` shows the parent consumes the array as the record flags in row order. QtCore identity: SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

For the default C-locale stream used here, the retained integer getter in `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` has these relevant branches:

- Leading whitespace is skipped before each extraction (`0x670C5250–52B9`). Decimal fallback processes digits and tests C-locale sign/group-separator rules (`0x670C5609–5790`). If a character stops parsing after at least one digit and it is not consumed as a recognized group separator, `ungetChar` restores it (`0x670C5780–578B`) and the getter returns success with the accumulated value (`0x670C5790–5969`). Thus a spelling such as `1x` can yield integer `1`, leaving `x` for the next extraction.
- When no digit was accumulated, the getter ungets the failed character and returns a nonzero conversion result (for ordinary invalid ASCII `x`, decimal fallback returns through the error path at `0x670C54AA–54BF`).
- `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` shows `operator>>(int&)` calls `getInteger` when stream buffer/device input is available (`0x670C60F4–6120`). On conversion failure it stores zero (`0x670C6125–6133`), computes the internal error status from end state, and writes that status only if no prior status is set (`0x670C6139–6152`). It does not gate an attempted extraction on the existing status; the input-availability branch is based on the buffer/device pointers. On success it stores the getter result's low 32 bits (`0x670C6166–616A`).
- `analysis/wave2-save-diffs-qt-textstream-atend-complete.asm` shows `atEnd()` uses buffer position/length or the device's end state (`0x670C2660–26CD`); it does not inspect QTextStream conversion status. An ungot character remains available, so `atEnd()` is false while that character remains buffered.

These captures source-qualify cursor and status decisions. They are static PE evidence, not an original-process experiment.

## Consequences for bounded examples

The examples below are deductions from the getter, operator, `atEnd`, and `FUN_1410da690` control flow. They assume successful QFile open and ASCII bytes. They do not claim malformed behavior outside the cited branches.

| Companion bytes | Requested flags | Original-static result | Candidate result |
|---|---:|---|---|
| `1\t-2\t\r\n99\r\n` | 2 | `[1, -2]`; `99` is not extracted | `[1, -2]`; suffix ignored |
| `1\r\n` | 2 | `[1, 0]`; next loop sees EOF and leaves pre-zeroed slot | `[1, 0]` |
| empty readable file | 2 | `[0, 0]`; first loop check sees end | `[0, 0]` |
| `1x\r\n` | 1 | `[1]`; first getter parses `1`, leaves `x`, then count loop ends | Refused; candidate requires full token |
| `1x\r\n` | 2 | `[1, 0]`; second getter fails at ungot `x`, stores zero; helper still returns true | Refused; candidate requires full token |
| `x 2\r\n` | 2 | `[0, 0]`; failure ungets `x`; next iteration retries it and stores zero again | Refused |
| `1 2x\r\n` | 2 | `[1, 2]`; second getter parses `2`, leaves `x`; count ends | Refused |

When an invalid suffix is present and the row count leaves another extraction, status becomes sticky after the first failure, but `atEnd()` remains governed by the unread buffered character and the integer operator still attempts later getter calls. Each failed destination is pre-zeroed by the companion helper and the operator also stores zero on failure. The helper nevertheless returns true because QFile opened successfully and it does not inspect status.

The table demonstrates why malformed-token status behavior must not be collapsed into EOF-short zero fill. The candidate's present-companion rule rejects malformed tokens and leaves the public output empty. It is intentionally narrower than the original and should keep that refusal explicit until this cursor/status behavior is implemented and differentially checked.

## Remaining scope boundaries

- `1,234` follows a distinct C-locale group-separator branch; do not infer its full repeated-extraction result from the plain `1x` case. Existing SW2SP evidence shows the comma can remain unread after the parsed prefix, but SPRING2 multi-read behavior has not been separately exercised.
- Base-0 prefixes, leading-zero octal, signed overflow, and 64-bit accumulation/narrowing are implemented by Qt's getter, but the candidate companion contract intentionally admits only canonical writer-style decimal int32 tokens. No broader numeric parity is claimed here.
- The original default QTextStream codec is process-dependent. All examples above use ASCII bytes; no ACP or alternate codec profile is claimed.
- The writer emits flags using decimal `QString::setNum(..., 10)`, so its own canonical values do not require malformed-token support. Save ordering is separately documented in `analysis/resume119-spring2-normal-path.md`.

No source, tests, build, GUI, original process, or mutable REA state changed for this audit.
