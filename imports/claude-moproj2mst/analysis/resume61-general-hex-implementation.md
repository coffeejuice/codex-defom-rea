# Checkpoint 61: general lowercase hex Extra3D reader

## Implementation

Replaced exact `0x2` and `0x10` exceptions with one lowercase hexadecimal parser in `readDecimalInt`. After the `0x` prefix it consumes one or more `[0-9a-f]` bytes, accumulates with unsigned 64-bit shift/add (defined modulo-2^64 wrap), and leaves the first non-hex byte at the cursor. It then reinterprets the low 32 bits as signed `int32_t`, matching the retained Qt getter's 64-bit result followed by `operator>>(int&)` copying the low dword.

No digit after `0x` remains unsupported, as do uppercase prefix/digits, signed prefixes, binary syntax, other octal tokens, and malformed or partial multi-value continuation. The already qualified exact octal `010` branch remains unchanged. No shared or CMake files were edited.

## Finite candidate test matrix

The candidate-only SW2SP one-integer test covers:

- digit mapping: `0x0` → 0, `0x2` → 2, `0xa` → 10, `0xf` → 15, `0x1` → 1 at end-of-input
- multi-digit lowercase accumulation: `0x10` → 16, `0xdeadbeef` → -559038737
- low-32-bit narrowing: `0x80000000` → INT_MIN, `0xffffffff` → -1, `0x100000000` → 0
- first non-hex delimiter behavior: `0x10x` → 16 for the one-value caller, leaving the suffix outside the requested result
- retained guards: `0x`, `0xg`, `0xA`, `0X10`, `+0x10`, and `-0x10`

The guard cases intentionally remain errors in this adapter instead of claiming the original caller-visible status/partial-row behavior. The finite matrix is source-shaped; no test binary was executed here because root owns builds and validation.

## Source qualification

`analysis/resume61-general-hex-next.md` records the bounded original QtCore disassembly and the original Extra3D `SW2SP.DAT` binding. The original getter selects radix 16 for lowercase `0x`, recognizes only digits `0`–`9`/`a`–`f` in that branch, ungets the first non-hex character after at least one digit, writes its 64-bit accumulator, and has no visible overflow check. `operator>>(int&)` copies the low dword. The implementation follows that contract within the one-integer sidecar profile; it does not generalize the parser to signed, uppercase, or malformed multi-value input.

## Freeze hashes

SHA-256 after these edits:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `733D5DB9F3D8C86D2E5DDEA331C236B1838B014D9512B38274BB44090C7CDD63`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `F1F0A13150939C5DE30FB3E8EE1075BAFABD25B373E429BF2B6D0A41D8E0825D`

