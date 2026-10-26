# RGDCNT

Rigid contact parameters.

* registry id 281 (`0x119`), group 3, flag 0; handler columns 0x1402f7590 / 0x1403cbbd0 / 0x1402f7610.
* Read: `FUN_140388fb0` case 0x9 (`id - 0x110`). Write: `FUN_1403c9ad0` → `FUN_1403c8560` (per pair).

## Read grammar

Every group 3 keyword except CNTACT starts with `fscanf(f, "%i %i")` on the keyword line → master m, slave s (fscanf -1 → 2). The pair `(m,s)` is found (`FUN_1401facf0`) or appended (`FUN_1401faaa0`, with `+0x168` = m, `+0x16c` = s). The handler then `fgets` **the rest of the same line**. After the handler, if m ≠ s the pair is mirrored into `(s,m)` (see [CNTACT.md](CNTACT.md#mirroring)).

Rest of line `%i %i %i %Flf` (`FUN_1404164e0`) → a, b, c, d (failure → 2). If 0 ≤ a < 5: `pair+0x37c` = a, `+0x380` = b, `+0x384` = c, `+0x388` = d. Status 0.

## Model fields

`pair+0x37c..+0x384` int, `+0x388` double.

## Default

0.

## KEY writer

Last pair record: `FUN_1403cbb10`, only if a > 0: `"%-6s %7i %7i %7i %7i %7i %20.10E\n"`.

## Helpers

`FUN_1404164e0`, `FUN_1403cbb10`. Shared mechanics: [_common.md](_common.md).

## Corpus

No corpus KEY file contains this keyword; grammar is from code only.
