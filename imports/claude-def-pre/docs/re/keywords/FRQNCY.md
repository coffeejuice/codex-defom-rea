# FRQNCY

Integer matrix (rows × columns) of frequencies.

* registry id 91 (`0x5b`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x45 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i %i` → r, c. If both > 0: `M+0xa78` = r, `M+0xa7c` = c, table `M+0xa80`.ncomp (`M+0xac0`) = r·c, and the table reader reads r·c numbers (free format) when r·c ≥ 2, plus one `fgets`; status = reader. Otherwise nothing more, status = line 0.

## Model fields

`M+0xa78` rows, `M+0xa7c` cols, table `M+0xa80` (template 0xb5, values `M+0xac8`).

## Default

r = 6, c = 1, values 1, 0, 100, 20, 1, 1 (`FUN_1401e9850`).

## KEY writer

Last record of *Iteration Controls*, only if r > 0, c > 0 and r·c == `M+0xac0`: `"%-6s %7i %7i\n"` then r lines of c × `" %7i"` (doubles truncated) each ended by `"\n"`. The defaults satisfy the condition, so FRQNCY is normally always written.

## Helpers

`FUN_140413ee0`, `FUN_14041a800`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 91 occurrences in 91 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/HeatTreat/MRX_SRX_2D_EN.KEY`.
