# UNNAME

Names of user node variables.

* registry id 89 (`0x59`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x43 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i %i` → flag, n (failure → 2). If flag < 2: `FUN_1401d21c0(M, n)` resizes the name array to n (frees surplus, reallocs, `calloc(0x40)` new slots; then `FUN_1401d0fc0(M,0)`; nothing happens when n equals the current count). Then, for every slot of the **current** count `M+0x3398`: `fgets(buf, 64)` (EOF → 2); if flag < 2, trim trailing blanks and, when non-empty, `strcpy` into the slot. With flag ≥ 2 the lines are consumed and ignored.

## Model fields

`M+0x3398` count, `M+0x33a0` → array of 64-byte strings.

## Default

count 0, no array.

## KEY writer

Only if the array pointer is non-null: `"%-6s %7i %7i\n"` with 1 and the count from `FUN_1401cbbc0(M)` (which returns `M+0x3398`; its rebuild branch is dead because `FUN_1401d2b70` always returns 1), then every slot `"%s\n"`. UNNAME is written before UENAME.

## Notes

`fgets` with 64 bytes: a name line of 63+ characters is split and the remainder is consumed as the next name.

## Helpers

`FUN_140413ee0`, `FUN_1401d21c0`, `FUN_140323c50`, `FUN_1401cbbc0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 442 occurrences in 442 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
