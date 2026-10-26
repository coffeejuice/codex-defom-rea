# DTTYPE

Step-size schedule: table of step size against time or stroke.

* registry id 51 (`0x33`), group 1, flag 7; handler columns 0x1402f7590 / 0x1403d7ce0 / 0x1403d7e80.
* Read: `FUN_14039aa90` case 0x1d (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i %i` → t, n (failure → 2). The table `T = M+0x938` is reset to template 0x80. t = 0: `T.ndim` (`M+0x93c`) = 0, `T.const` (`M+0x940`) = 0, `FUN_1401ee510(S)`. t ≠ 0: `T.ndim` = 1, `T.cnt[0]` (`M+0x954`) = n and: t=1 → `T.var` (`M+0x948`) = 1, time flag `M+0x990` = 0; t=2 → var 2, flag 0; t=3 → var 1, flag 1; t=4 → var 2, flag 1; any other t → var 1, flag 0. Then the table reader always runs: for t ≠ 0 it reads n pairs `x value` (free format) plus the rest of the last line; the handler status is the table-reader status.

## Model fields

Table at `M+0x938` (`S+0x1e8`): `+4` ndim, `+0x10` var code, `+0x1c` count, `+0x28` abscissae, `+0x48` values; flag `M+0x990` (`S+0x240`).

## Default

Template 0x80 (empty constant), flag computed by `FUN_1401ee510`.

## KEY writer

If `M+0x93c == 0`: the writer emits DSMAX/DTMAX (see those) and then `DTTYPE` with `"%-6s %7i %7i\n"` 0, 0. If `M+0x93c == 1`: `FUN_1403d7ce0`: header `"%-6s %7i %7i\n"` with code and count, where code = 1/2 for var 1/2 when the flag is 0, and 3/4 when the flag is 1 (other var codes: no header line, data still written); then `FUN_1402f3130(f, T, 0)`: one line `" %20.10E %20.10E\n"` per point. Any other `M+0x93c` makes the group writer return 3 (KEY write aborted).

## Helpers

`FUN_140413ee0`, `FUN_14012fc70`, `FUN_14041a800`, `FUN_1401ee510`, `FUN_1401ee5a0`, `FUN_1403d7ce0`, `FUN_1402f3130`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 444 occurrences in 443 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
