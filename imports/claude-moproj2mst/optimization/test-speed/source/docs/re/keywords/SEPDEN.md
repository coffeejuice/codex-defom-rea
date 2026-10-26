# SEPDEN

Separation density value (2D only).

* registry id 277 (`0x115`), group 3, flag 5; handler columns 0x1402f7590 / 0x1403cafb0 / 0x1402f7610.
* Read: `FUN_140388fb0` case 0x5 (`id - 0x110`). Write: `FUN_1403c9ad0` → `FUN_1403c8560` (per pair).

## Read grammar

Every group 3 keyword except CNTACT starts with `fscanf(f, "%i %i")` on the keyword line → master m, slave s (fscanf -1 → 2). The pair `(m,s)` is found (`FUN_1401facf0`) or appended (`FUN_1401faaa0`, with `+0x168` = m, `+0x16c` = s). The handler then `fgets` **the rest of the same line**. After the handler, if m ≠ s the pair is mirrored into `(s,m)` (see [CNTACT.md](CNTACT.md#mirroring)).

2D only: rest of line `%Flf` → `pair+0x238`. In 3D the rest of the line is not read (status 0; the remaining tokens reach the main loop as unknown keywords). The pair is still created and mirrored.

## Model fields

`pair+0x238` double.

## Default

0.

## KEY writer

2D only, after SEPRES: `FUN_1403caf20` `"%-6s %7i %7i %20.10E\n"`.

## Helpers

`FUN_140414990`, `FUN_1403caf20`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 629 occurrences in 247 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/DieStress/DieStress_Example2_EN.KEY`.
