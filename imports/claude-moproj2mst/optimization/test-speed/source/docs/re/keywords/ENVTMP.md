# ENVTMP

Environment temperature: constant, table against time, or region list (code 10).

* registry id 75 (`0x4b`), group 1, flag 5; handler columns 0x1402f7590 / 0x1403d6c90 / 0x1403d6ec0.
* Read: `FUN_14039aa90` case 0x35 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i %Flf` → t, v (failure → 2). Table `T = M+0xb18` reset to template 2, region flag byte `M+0xb10` = 0.
* t = 0: `T.ndim` = 0, `T.const` (`M+0xb20`) = v; table reader (reads nothing).
* t = 1: `T.ndim` = 1, `T.var` (`M+0xb28`) = 1, `T.cnt` (`M+0xb34`) = (int)v; table reader: (int)v pairs.
* t = 10: flag `M+0xb10` = 1; R = (int)v; the region list object at `M+0xb90` is rebuilt by `FUN_140375600` with max(21, R) tables (templates 0x8e..0xa2, then 0xa3). For i = 1..R the i-th table is read: entry 21 (`FUN_14039d2a0` returns 0x15) with `FUN_14041b060`, the others with `FUN_14041ace0` (below). Their return values are ignored; the status stays that of line 0.
* other t: only the reset happens; the status is 0 and no data lines are consumed.

`FUN_14041ace0` (numeric region entry): header line `%i %i %i %Flf %Flf` → (ignored index, ndim, a, d1, d2). ndim 0: `ncomp` = a; if `ncomp < 2` const = d1, else `ncomp` values follow (`%Flf`). ndim 1: var = a, cnt = (int)d1; if cnt = 0: const = d2, ncomp = 0 (clamped to 1); else ncomp = (int)d2; then cnt rows of `x` + ncomp values. Other ndim: returns 1 (ignored). If the total value count > 0, one `fgets`.
`FUN_14041b060` (entry 21, text): same header; ndim 1: var = a, cnt = (int)d1, ncomp = (int)d2, then cnt text lines (`FUN_140413c60`, 64 bytes) copied into 64-byte slots of the value array (`FUN_140323bf0`: strncpy then pad with spaces to 64). ndim 0: nothing more.

## Model fields

Table `M+0xb18` (`S+0x3c8`), flag byte `M+0xb10`, region list `M+0xb90` (`S+0x440`; its first 64 bytes are the FRNNAM name, the table list head is at `+0x40`).

## Default

Constant 20.0 (UNIT ≠ 2) or 68.0 (UNIT = 2), flag 0; the region list is built with 22 entries and filled by `FUN_140375e90` from the table at `0x140550ec0` (only reaches the KEY file if code 10 is later read, which rebuilds it).

## KEY writer

`FUN_1403d6c90`, after BLZMAN. Flag clear: ndim 0 → `"%-6s %7i %20.10E\n"` (0, const); ndim 1 with var 1 → `"%-6s %7i %7i\n"` (1, count) then the rows (`FUN_1402f3130`); anything else → return 3 (KEY write aborted). Flag set: `"%-6s %7i %7i\n"` (10, list length), then for i = 1..length: entry 21 → `FUN_1402f3840` (only if ndim 1: `" %7i %7i %7i %7i %7i\n"` i, 1, var, cnt, ncomp, then each 64-byte slot trimmed of trailing blanks with `"%s\n"`); other entries → `FUN_1402f34f0`: ndim 0 → `" %7i %7i %7i %20.10E\n"` (i, 0, ncomp, const) when ncomp < 2, else `" %7i %7i %7i\n"` (i, 0, ncomp) + values 5 per line; ndim 1 → cnt 0: `" %7i %7i %7i %7i %20.10E\n"` (i, 1, var, 0, const), else `" %7i %7i %7i %7i %7i\n"` (i, 1, var, cnt, ncomp) and per row the abscissa (`" %7i"` row number if var = 0, else `" %20.10E"`) followed by the values (5 per line).

## Notes

With code 10 and fewer than 21 regions the list still holds 21 tables, so the writer emits 21 entries; entries not read keep template defaults (`ndim 1`, `cnt 0`).

## Helpers

`FUN_1404154c0`, `FUN_14012fc70`, `FUN_14041a800`, `FUN_140375600`, `FUN_140375980`, `FUN_14041ace0`, `FUN_14041b060`, `FUN_1404165e0`, `FUN_1403d6c90`, `FUN_1402f3130`, `FUN_1402f34f0`, `FUN_1402f3840`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 486 occurrences in 484 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
