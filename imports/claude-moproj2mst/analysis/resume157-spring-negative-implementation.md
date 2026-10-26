# CP157 SPRING2 negative decimal modulo completion

## Change

The count-one canonical negative decimal route now accumulates magnitude digits into `uint64_t` modulo `2^64`. It applies two's-complement negation only when the wrapped value is nonzero and its high bit is clear, matching Qt's signed-positive completion branch; it then stores the low 32 bits as the flag. This replaces the former `INT64_MAX` magnitude rejection. Existing int32 parsing, exact `-2147483649`, positive-wrap, signed-leading-zero, BOM, and other radix profiles are preserved.

The route remains bounded by the existing count-one gate, canonical `-[1-9][0-9]*` token prefix, and whitespace/EOF terminator. Attached suffixes and count-two forms remain guarded. No general negative leading-zero or malformed-token behavior was added.

## Focused tests

Accepted one-row cases include:

- `-9223372036854775808\r\n` -> 0 (wrapped high bit set, no negation);
- `-9223372036854775809\r\n` -> 1 (same conditional branch, PE-table witness);
- `-18446744073709551615\r\n` -> -1 (wrapped all-ones, PE-table witness);
- `-18446744073709551616` -> 0 at EOF (full accumulator wrap);
- `-18446744073709551617\t` -> -1 (wrap to one then conditional negation).

The tests also retain the preexisting negative controls and assert both `SPRING.INI` and `SPRING2.INI` remain byte-identical after accepted reads. The count-two refusal matrix now includes `-9223372036854775809` and `-18446744073709551617`; the one-row suffix refusal includes `-18446744073709551617x`.

## Static preflight and evidence

- The arithmetic follows the independent plan review `analysis/resume157-spring-negative-overflow-review.md` and PE captures `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` (`0x670c56f6–5704`, `0x670c57bb–57c7`) and `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` (`0x670c6166–616a`).
- The exact two negative overflow examples in the corrected original table are recorded in `analysis/resume83-sw2sp-parser-independent-review.md`.
- Reviewed source gate/order and test matrix; checked for trailing whitespace. No build or CTest was run, per assignment. No GUI or mutable REA state was used.
- This supersedes the CP151 and CP157-after-BOM statements that signed-64 overflow remains refused. It does not claim a fresh original-process differential.

## Frozen hashes

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `5FBE75CD40647CD063AE27AB0128592BC6AA564D0B8CD499082C9B9B8F88BC43`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `348CF582BBE0BF6CD6915DC3FC187A3521A638AF88B027D6A24C7A380B49C154`
