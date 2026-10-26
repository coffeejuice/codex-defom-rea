# CP156 complete unsigned SPRING2 binary review

## Result

No blocking defect found. The frozen reader admits complete unsigned ASCII `0[bB][01]+` only for the one-row-qualified path, while the earlier exact cases retain their distinct outcomes.

## Frozen source and test

The reader and test hashes match the CP155 implementation receipt:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `0870C34A0C18DCBA3A2026BA562AC6A2E3C8AE575A100BEE67A3919B6396745D`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `5779941FCFE83FBA867E3655E8D98021203DD29BE1CCEEDAC23FE72860FF768F`

## Findings

In `readSpringFlagInt`, the prior exact `0B10`, invalid-first-bit `0B2`, and partial valid-prefix `0b12` routes occur before `oneRowCompleteBinaryPrefix`. The general route requires `allowSingleRowQualifiedToken`, a `0` plus either-case `b`, and at least one byte after the prefix. It accepts only ASCII `0`/`1` bits, folds them into `uint32_t` with shift-left-one/add, then requires EOF or ASCII whitespace. No bits or an attached non-whitespace character produce an unsupported error. The final `bit_cast<int32_t>` preserves the low-dword value, including high-bit and wraparound cases.

The preserved behavior is tested directly: `0B10` -> 2; `0B2` -> 0; `0b12` -> 1. The new set covers lowercase `0b1`, lowercase `0b10`, uppercase `0B1010` with tab, the bit-31 value at EOF, 32 one-bits -> -1, and a 33-bit value wrapping to 1. The accepted tests assert byte-for-byte preservation of primary and companion files. The one-row refusal matrix retains `0B3`, `0b2`, `0b13`, `0b10x`, and `0b12x`; a two-row refusal list includes complete `0b10` and `0B1010`. Exact `0b12` two-row continuation remains in its separate profile.

The inspected ordering also keeps binary disjoint from the preceding complete hexadecimal parser (`0x`/`0X`) and from later decimal/octal parsing. The static original evidence cited in the implementation receipt supports case-insensitive base-prefix selection, binary digit accumulation, the first nonbinary character being left unread, success only after at least one bit, and low-dword storage into `int`.

## Limits

This was a static review only; no build or CTest was run. No fresh original-process comparison is claimed. Multi-row complete-token extraction, Unicode characters, codec/BOM behavior, and malformed-prefix retry/status effects outside the exact tested witnesses remain out of scope.
