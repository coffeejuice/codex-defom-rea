# CP148 next RFORMAT admission: nonnegative decimal low-32 wrap

## Recommended cut

The largest cleanly source-qualified numeric gap in the reachable SPRING2 reader is a nonnegative, non-leading-zero decimal digit run whose value exceeds the candidate's current `2^32` cap. For the bounded profile `[1-9][0-9]*` followed only by ASCII whitespace or EOF, original Qt's base-zero integer getter selects decimal, accumulates with 64-bit multiply/add, and does not reject unsigned-positive overflow; `QTextStream >> int` stores the low 32 bits of the getter result. Thus the externally relevant flag is the input decimal value modulo `2^32`, including after the intermediate 64-bit accumulator wraps.

This matters to public Open/Save: flags become `SpringRecord::flag`; the retained `FUN_1410db030` writer scans and serializes those flags. For example, a source row with companion flag token `4294967297` loads as flag `1`, and an unchanged Save serializes the normalized decimal flag `1` in `SPRING2.INI`. Candidate currently throws for `4294967297` and larger positive values, so it refuses before that reachable Open state is created.

## Original source basis

The governing QtCore binary is original `QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. PE-byte-checked regions and corrected behavior are documented in `analysis/resume83-sw2sp-parser-independent-review.md` and `analysis/resume123-spring2-unsigned-review.md`.

- Base selection chooses decimal for a first digit 1–9; the octal ambiguity exists only for a leading zero, which this proposal excludes.
- Decimal digits accumulate by 64-bit LEA multiply/add at `0x670c56f6–5704`. The operations wrap naturally and the successful digit loop does not reject arithmetic overflow.
- For a nonnegative sign, getter completion does not apply the negative-sign transform. The Qt `int&` operator stores the low dword at `0x670c6166–616a`.
- The corrected Qt table already records `18446744073709551616` (2^64) → getter success, next position EOF, flag 0. The immediately adjacent value 2^64+1 therefore returns low dword 1 by the same no-overflow-check arithmetic. These are static deductions from verified machine code, not original-process differential results.
- The reached caller is `FUN_1410da690`: destinations are pre-zeroed; on successful companion open it loops while not at end and below row count, ignores QTextStream status, and returns success after open. For a one-row complete primary file, a complete decimal companion token is reached and copied into that row's flag.

The `SPRING2` writer contract in `analysis/resume120-spring2-writer-order.md` confirms that these values reach observable persisted output: any nonzero flag causes a companion write, and flags are emitted as decimal values in row order. Write-failure continuation remains separate.

## Candidate difference and exact implementation boundary

`readSpringFlagInt` in `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` currently parses through `from_chars(int)`, then has a `uint64_t` fallback capped at `4294967296`. It therefore refuses `4294967297` even though that token succeeds natively as integer flag 1. The focused test's unsupported-token list also currently expects `4294967297\r\n` to refuse.

A source-ready next implementation can replace that cap only for a narrow decimal class:

- Gate on the existing `SPRING2.INI` path and count-one row profile for the first cut, or on the established generic count range if root chooses to extend it.
- Accept a token beginning with `1`–`9`, containing ASCII decimal digits only through whitespace/EOF, with no sign, leading zero, prefix, or comma.
- Accumulate into unsigned 32-bit arithmetic (or retain 64-bit wrap then narrow); unsigned modulo arithmetic gives the same low dword as Qt's 64-bit wrap for these nonnegative digits.
- Preserve existing comma/unread suffix logic and all sign, base-zero, malformed, codec and filename guards.
- Keep the original bytes unchanged on read. Candidate Save can later format the recovered signed low-dword flag as decimal.

Focused evidence matrix: one-row exact ASCII SPRING with companion `4294967297\r\n` → flag 1; `18446744073709551616\r\n` → 0; `18446744073709551617\r\n` → 1. Add a short ordinary value control (`2147483648` → INT_MIN) to prove existing behavior stays stable. Keep signs (`+4294967297`, `-4294967297`), leading-zero tokens, hex/binary prefixes, comma suffixes and malformed tokens guarded. Since `4294967297\r\n` is already in the candidate refusal loop, remove only that unsupported expectation when assigned; do not broadly release the rest of the matrix.

## Competing remesh failure edge

`analysis/resume124-remesh-handler-frontier.md` still establishes only the pre-handler ordered effects and a conditional default-handler path for a null `fopen` result. It lacks same-run handler slot, replacement filter, debugger/error-mode and observed termination data; the exact missing witness is specified there. That terminal branch is not a safe source-only implementation increment and should remain guarded. The decimal low-dword path above is a more useful next cut because its value, cursor, caller loop, and downstream flag serialization are all bounded by retained primary evidence.

## Qualification and bindings

This proposal covers ASCII digits in the default-C QTextStream branch only. No original runtime comparison, source edit, test execution, build, GUI action, or mutable REA operation occurred. It does not claim signed overflow, prefix/radix, locale grouping, arbitrary malformed suffixes, every possible process codec, or writer failure parity.

- Qt behavioral table: `analysis/resume83-sw2sp-parser-independent-review.md` — `012507E626AA458EEC257D263CF1FF04BBBDF55CE2F334856BBCE3B33D539457`
- Independent unsigned review: `analysis/resume123-spring2-unsigned-review.md` — `9BC58BB3444149CE6BCE99AE684B7634C0CE5272B09CE27351476704DBFC96F4`
- Candidate reader: `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` — `35B209095822E0FCC4692AC9D7B8E484813311473790B451FD439106777C2536`
- Candidate focused test: `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` — `E7ED56CA27CD3D80ABAF4DBC65FBDC1CA0E3AB4C8A3EB397D00C4976C9E947F4`
- Writer order: `analysis/resume120-spring2-writer-order.md` — `AA90D258C00350BCF1DBDF57B37C7EF4A696D16CAFBFC4BB02A2BB5E88B5C896`
- Remesh blocker: `analysis/resume124-remesh-handler-frontier.md` — `9405B7CDF239B6FB97AADE46B8A8603D03CD6E9666751F305C8D3539E1A0CD6A`

Root owns any source assignment and validation.
