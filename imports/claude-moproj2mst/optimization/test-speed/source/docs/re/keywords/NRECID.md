# NRECID

Node record table (rows × values).

* registry id 86 (`0x56`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x40 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i %i` → n, m (failure → 2). Table `T = M+0x1298` reset to template 0xb2 (`ndim` 1, var 0). If n < 1 or m < 1 → **return 3 (Data Error)**, no data consumed (the data lines then reach the main loop as unknown keywords). Else `T.cnt` (`M+0x12b4`) = n, `T.ncomp` (`M+0x12d8`) = m, allocate, and the table reader reads n rows of `x` + m values; its status is the handler status.

## Model fields

Table `M+0x1298` (`S+0xb48`).

## Default

template 0xb2: ndim 1, cnt 0, ncomp 1.

## KEY writer

After ACVCOF/previous record, only if `cnt > 0`: `"%-6s %7i %7i\n"` (cnt, ncomp) then `FUN_1402f3130`: per row `" %20.10E"` abscissa and the m values 5 per line (continuation lines start with 8 spaces).

## Helpers

`FUN_140413ee0`, `FUN_14012fc70`, `FUN_14012fad0`, `FUN_14041a800`, `FUN_1402f3130`. Shared mechanics: [_common.md](_common.md).

## Corpus

No corpus KEY file contains this keyword; grammar is from code only.
