# CP148 implementation: bounded SPRING2 decimal low-dword wrap

## Change

The private SPRING2 integer reader now handles a bounded nonnegative decimal token beginning with `1`–`9`, followed only by ASCII whitespace or EOF, using unsigned low-32 accumulation. This reproduces the retained Qt result for arbitrary-length digits in that profile: the getter's 64-bit multiply/add arithmetic wraps, and the `int&` operator stores the low 32 bits. The candidate rejects signs, multi-digit leading-zero spellings, non-whitespace suffixes and comma-delimited overflow tokens before this fallback; existing comma-prefix handling for in-range parsed integers is unchanged.

Focused one-row fixtures assert:

- `4294967297\r\n` → flag `1`;
- `18446744073709551616\r\n` → flag `0`;
- `18446744073709551617\r\n` → flag `1`.

Each case checks that the primary and companion file bytes remain unchanged. The formerly unsupported `4294967297` expectation was removed. Negative tests retain `+4294967297`, `-4294967297`, leading-zero `04294967297`, and comma-suffixed `4294967297,4` as guarded inputs, along with existing radix/malformed negatives.

## Source qualification

The original QtCore4_64.dll SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. `analysis/resume83-sw2sp-parser-independent-review.md` retains PE-byte-verified parser and operator captures, including the `18446744073709551616` → 0 witness. `analysis/resume123-spring2-unsigned-review.md` independently establishes that positive `4294967296` is accepted well below signed-64 range and narrows to zero. The source-backed `FUN_1410da690` reader ignores conversion status and copies the resulting low dword into the requested flag slot. This is static behavior recovery; no original-process differential was run.

The bounded branch intentionally excludes signed overflow, signs, radix/leading-zero forms, comma/grouping after an overflowing prefix, and malformed suffixes. It does not generalize parser locale, filename, companion count, or codec scope. The downstream public Save behavior remains separately qualified: recovered flags become decimal text in row order under `FUN_1410db030`; writer failure behavior was not changed.

## Static preflight

- Direct `<array>` and `<utility>` includes were added for the new table-driven test fixture.
- Checked the parser still has one `readSpringFlagInt` callsite and the fallback requires a first digit `1`–`9` plus EOF/ASCII-whitespace terminator.
- Checked the test covers both 2^64 wrap and adjacent low-dword result, removes only the newly supported positive overflow expectation, and retains sign/leading-zero/comma guards.
- Checked no trailing whitespace in either owned source/test file.
- No build/CTest, GUI, original runtime, remesh, shared API, or mutable REA state was changed. Root owns integration and validation.

## Frozen hashes

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` — `50BC8B56431F31E2B2D8EA005A7D6C55FF7B0D6DDFF161004E455E8CC22E3B58`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` — `1C62FD8A32D718EBE9CBB89BFF6EEE42CE7E116F6A7805FE566023821A795A0D`
