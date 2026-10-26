# SEPRES

Separation criterion and value.

* registry id 274 (`0x112`), group 3, flag 5; handler columns 0x1402f7590 / 0x1403cae30 / 0x1402f7610.
* Read: `FUN_140388fb0` case 0x2 (`id - 0x110`). Write: `FUN_1403c9ad0` → `FUN_1403c8560` (per pair).

## Read grammar

Every group 3 keyword except CNTACT starts with `fscanf(f, "%i %i")` on the keyword line → master m, slave s (fscanf -1 → 2). The pair `(m,s)` is found (`FUN_1401facf0`) or appended (`FUN_1401faaa0`, with `+0x168` = m, `+0x16c` = s). The handler then `fgets` **the rest of the same line**. After the handler, if m ≠ s the pair is mirrored into `(s,m)` (see [CNTACT.md](CNTACT.md#mirroring)).

Rest of line `%i %Flf` → c, v (failure → 2). c ∈ {0,1,2} → `pair+0x178` = c (else unchanged); `pair+0x180` = v always. Status 0.

## Model fields

`pair+0x178` int, `pair+0x180` double.

## Default

0, 0.

## KEY writer

`FUN_1403cad60`, after the windows: c outside 0..2 → return 3; else `"%-6s %7i %7i %7i %20.10E\n"` (m, s, c, v). Always written for a pair with a direction.

## Helpers

`FUN_1404154c0`, `FUN_1403cad60`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 927 occurrences in 374 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/DieStress/DieStress_Example2_EN.KEY`.
