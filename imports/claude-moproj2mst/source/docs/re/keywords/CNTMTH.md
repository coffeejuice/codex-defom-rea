# CNTMTH

Contact method.

* registry id 279 (`0x117`), group 3, flag 0; handler columns 0x1402f7590 / 0x1403cbdf0 / 0x1402f7610.
* Read: `FUN_140388fb0` case 0x7 (`id - 0x110`). Write: `FUN_1403c9ad0` → `FUN_1403c8560` (per pair).

## Read grammar

Every group 3 keyword except CNTACT starts with `fscanf(f, "%i %i")` on the keyword line → master m, slave s (fscanf -1 → 2). The pair `(m,s)` is found (`FUN_1401facf0`) or appended (`FUN_1401faaa0`, with `+0x168` = m, `+0x16c` = s). The handler then `fgets` **the rest of the same line**. After the handler, if m ≠ s the pair is mirrored into `(s,m)` (see [CNTACT.md](CNTACT.md#mirroring)).

Rest of line `%i %i %Flf` (`FUN_140415f00`) → a, b, d. a = 0 → `pair+0x330` = 0; else `+0x330` = 1, `+0x334` = b, `+0x338` = d. Status = helper.

## Model fields

`pair+0x330` method flag, `+0x334` int, `+0x338` double.

## Default

0, 1, 0.

## KEY writer

3D only, after INTRST: `FUN_1403cbce0`: flag 0 → `"%-6s %7i %7i %7i\n"` (m, s, 0); flag 1 → `"%-6s %7i %7i %7i %7i %20.10E\n"` (m, s, 1, b, d); else return 3.

## Helpers

`FUN_140415f00`, `FUN_1403cbce0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 298 occurrences in 127 corpus files consumed exactly up to the next keyword/comment; e.g. `key/3D/EN/Cogging/Cogging_Multiple_Ops_Lab1/Operations/Task00002/InterObject.KEY`.
