# CP123 bounded `SPRING2.INI` low-32-bit narrowing

## Change

The companion flag parser now admits canonical unsigned decimal tokens from `2147483648` through `4294967296`. The original Qt integer getter accumulates these values successfully in its wider integer register; `QTextStream::operator>>(int&)` stores the low 32 bits. Candidate flags therefore map through the same two's-complement bit pattern, so `2147483648` becomes `INT_MIN`, `4294967295` becomes `-1`, and `4294967296` becomes `0`.

The extension is limited to positive canonical decimal tokens at or below the source-closed boundary `2^32`. Existing signed int32 parsing remains unchanged. Values above `4294967296`, signed overflow forms, radix/leading-zero spellings, malformed tokens outside CP121/CP122, BOMs, and non-ASCII bytes remain guarded.

Focused tests cover `2147483648`, `4294967295`, and `4294967296` across count-one and count-two sidecars, while confirming the main and companion input files remain unchanged. The refusal matrix checks `4294967297` and `-2147483649` outside this branch.

## Source qualification

The original target is `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; shipped QtCore is SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

In `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`, decimal digit accumulation uses 64-bit multiply/add operations (`0x670C56F6–5704`) and succeeds once digits were read, provided no unsupported syntax/delimiter branch intervenes. In `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`, the successful operator branch stores the low 32 bits of the getter result (`0x670C6166–616A`); it writes no conversion error on that branch. The reachable `FUN_1410da690` loop in `analysis/wave2-save-diffs-300-support-1410da690.c` writes each extraction destination and increments the requested index without inspecting stream status. Thus these successful wide positive values become the flag values listed above.

The `4294967296` witness is below the signed 64-bit accumulator limit, so it does not require an accumulator wrap assumption. The next value, `4294967297`, remains guarded. CP71 separately documents a `2^64` wrap witness, but this change does not generalize to arbitrary 64-bit decimal overflow. Signed overflow is also kept outside this branch.

## Static preflight and limits

`git diff --check` passed for the two owned files. No build or tests were run; root owns integration. No page, header, public Session, GUI, or REA state changed. This is a source-qualified arithmetic consequence, not a fresh original-process comparison.

## Frozen hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `CABEF3FA006B68C442EF7BECC0FC76E319C1D2C56645FB05640A670F2A0ABCAE` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `EEE515B327BBC7D09A59CD1B60383E8D45BA8E119D97504541B9565C6FDA489A` |

This receipt supersedes the pre-review CP123 source/test hashes.

No original-process runtime fixture was performed.
