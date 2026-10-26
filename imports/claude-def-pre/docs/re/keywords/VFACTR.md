# VFACTR

Velocity factor control; in 3D optionally a list of values.

* registry id 57 (`0x39`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x23 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

3D: line 0 `%i %i` → mode (`M+0x1128`), n. If mode == 1 and n > 0: `M+0x112c` = n, table `M+0x1130`.ncomp (`M+0x1170`) = n and `FUN_14041ac10` reads n `%Flf` numbers (free format, **no** trailing `fgets`; its status is ignored); otherwise `M+0x112c` = 0. 2D: line 0 `%i` → `M+0x1128`.

## Model fields

`M+0x1128` mode, `M+0x112c` count, table `M+0x1130` (template 0xb5, values at `M+0x1178`).

## Default

mode 0, count 0, table template 0xb5 (3D models).

## KEY writer

3D: mode 0 → `"%-6s %7i\n"` 0; mode 1 with count > 0 → `"%-6s %7i %7i\n"` 1, `M+0x1170`, then one line of `" %7i"` per value (each double truncated to int) and `"\n"`; otherwise `"%-6s %7i %7i\n"` mode, `M+0x112c`. 2D: `"%-6s %7i\n"` mode (after CNTERR).

## Notes

Values are written as integers although stored as doubles: a fractional factor is truncated on KEY write.

## Helpers

`FUN_140413ee0`, `FUN_14041ac10`, `FUN_140413de0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 357 occurrences in 357 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
