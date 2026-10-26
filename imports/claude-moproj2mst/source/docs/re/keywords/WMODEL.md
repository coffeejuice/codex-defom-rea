# WMODEL

Wear model with parameters on the next line.

* registry id 280 (`0x118`), group 3, flag 0; handler columns 0x1402f7590 / 0x1403cba00 / 0x1402f7610.
* Read: `FUN_140388fb0` case 0x8 (`id - 0x110`). Write: `FUN_1403c9ad0` → `FUN_1403c8560` (per pair).

## Read grammar

Every group 3 keyword except CNTACT starts with `fscanf(f, "%i %i")` on the keyword line → master m, slave s (fscanf -1 → 2). The pair `(m,s)` is found (`FUN_1401facf0`) or appended (`FUN_1401faaa0`, with `+0x168` = m, `+0x16c` = s). The handler then `fgets` **the rest of the same line**. After the handler, if m ≠ s the pair is mirrored into `(s,m)` (see [CNTACT.md](CNTACT.md#mirroring)).

Rest of line `%i %i` → w, x (x unused; failure → 2). w = 0 → `pair+0x340` = 0. w = 1 → `+0x340` = 1, next line `%Flf` ×4 → `+0x348..+0x360`. w = 2 → `+0x340` = 2, next line `%Flf %Flf` → `+0x368`, `+0x370`. w < 0 → `+0x340` = 3, `+0x378` = −w. w > 2 → nothing (status 0).

## Model fields

`pair+0x340` model, `+0x348..+0x370` parameters, `+0x378` id.

## Default

model 0.

## KEY writer

`FUN_1403cb840`, after CNTMTH: model 1 → `"%-6s %7i %7i %7i %7i\n"` (m, s, 1, 4) + `" %20.10E %20.10E %20.10E %20.10E\n"`; model 2 → (m, s, 2, 2) + `" %20.10E %20.10E\n"`; model 3 → `"%-6s %7i %7i %7i\n"` (m, s, −id); model 0 → nothing.

## Helpers

`FUN_140413ee0`, `FUN_140414e40`, `FUN_140414a40`, `FUN_1403cb840`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 24 occurrences in 16 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Machining/MACHINING_Example1_EN.KEY`.
