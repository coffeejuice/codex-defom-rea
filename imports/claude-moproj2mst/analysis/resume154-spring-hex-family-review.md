# CP155 complete unsigned SPRING2 hexadecimal review

## Result

No blocking defect found. The frozen route correctly covers complete unsigned ASCII `0[xX][0-9A-Fa-f]+` tokens for the count-one profile while preserving earlier bounded prefix cases and rejecting attached suffixes and broader count-two use.

## Frozen evidence

Reader and focused test hashes match the CP155 implementation receipt:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `62F972D295798FA02E84EA84FE09D1E6243D382903C4BD57AAF0DB36B91B17D4`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `A528A831FEE8F024F6C9EA52D0A1A819C1CD3B721C9680268A70B676E8854912`

## Findings

The route order preserves prior semantics: the bounded bare-prefix handlers and exact `0xA`/`0XA`/`0xA0` cases come first, followed by the existing `0xG` invalid-first-digit approximation and its count-two retry profile. The new route then requires the one-row gate and a `0x` or `0X` prefix with at least one following byte. Its digit loop accepts ASCII `0-9`, `a-f`, and `A-F`, accumulates in `uint32_t` using shift-left-four plus digit, and accepts only EOF or ASCII whitespace after the run. The unsigned arithmetic gives defined modulo-2^32 narrowing; `bit_cast<int32_t>` preserves the low-dword bit pattern. If the prefix has no hex digit, or a non-whitespace suffix follows the digits, the path throws.

This ordering leaves `0x`/`0X` empty-prefix and `0xG` cases to their earlier handlers, while `0xA0G` is refused as an attached suffix. Signs do not match the unsigned prefix gate and remain subject to the distinct signed decimal handling/guard. Count-two `0x1` and `0xA1` are asserted unsupported. Existing exact-prefix behavior is kept as before; the new general admission remains count-one-only.

Tests cover `0x1`, `0xA1`, `0XA0`, mixed-case `0XfF` with tab, high-bit `0x80000000`, EOF `0xFFFFFFFF`, wraparound `0x100000001`, and `0xA00`; accepted cases check byte-for-byte preservation of both primary and companion inputs. The refusal list retains `0xA0G`, and count-two refusals include `0x1` and `0xA1`. Existing bare-prefix, invalid-first-digit, octal, and leading-zero decimal tests remain in the frozen test file.

The static Qt evidence in the proposal supports these boundaries: case-folded `x` selection, case-folded hex digit decoding, repeated base-16 accumulation, success only after at least one digit, and low-dword storage into `int`. The candidate keeps its language explicitly ASCII-bounded.

## Limits

This was a source/test review only; no build or CTest was run. There is no fresh original-process differential. Multi-row cursor/status behavior, Unicode digit handling, BOM/codec behavior, locale signs, and malformed valid-prefix recovery beyond the exercised suffix refusal remain outside the claim.
