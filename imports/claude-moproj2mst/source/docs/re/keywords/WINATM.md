# WINATM (id 228, group 2, case 0x54)

Window-defined atmosphere boundary condition (diffusion/carburizing analogue of WINTMP: an
atmosphere value and a transfer coefficient inside a 2D polygon or 3D box/cylinder window).
Records are `LLWINDEF_DATA` entries of list `obj+0x2f10`, reset to BC kind 4. Record layout,
list, shape grammar/writer and attachment mode: see WINPRS.md.

## KEY grammar (read)

    WINATM <objNo:%i> <win:%i> <n|s:%i> <t1:%i> <t2:%i> <mode:%i>   FUN_140414200 (5 ints, no blank skip)
    <ox:%Flf> <oy:%Flf> [<oz:%Flf>]                                FUN_140414c20
    shape body          2D (FUN_1400015e0): n lines "x y" (FUN_14041b6a0(f,w,n,0))
                        otherwise (3D or unset): s = 1 box / 2 cylinder (FUN_14041b7d0(f,w,1))
    <v1:%Flf>               [table 1 body]                         FUN_140414990
    <v2:%Flf> [<v2b:%Flf>]  [table 2 body]                         FUN_140414a40

| code | table 1 (`+0x5e0`) | table 2 (`+0x658`) |
|---|---|---|
| 0 | const = v | const = v |
| 1 | `ndim 1`, var 1, `cnt = (int)v` | `ndim 1`, var 1, `cnt = (int)v` |
| 2 | untouched | `ndim 1`, var **0xc**, `cnt = (int)v` |
| 3 | untouched | `ndim 2`, var (1, 0xc), `cnt = ((int)v, (int)v2b)` |
| other | untouched (value discarded) | untouched |

Bodies: `FUN_14041a800` (`_common.md` §3). Variable 1 = time; 0xc is the atmosphere analogue of
WINTMP's temperature code 3 (presumably carbon/atom concentration).

## Model storage

`w = FUN_140269bc0(obj+0x2f10, win)`, `FUN_14025d1a0(w, 4)`. Origin `+0x350/+0x358/+0x360`;
2D: `+0x178 = n`, points `+0x180`; 3D: `+0x210 = s`, then `FUN_140267760`. Table 1 `+0x5e0`
(ndim `+0x5e4`, const `+0x5e8`, var `+0x5f0`, cnt `+0x5fc`); table 2 `+0x658` (ndim `+0x65c`,
const `+0x660`, var `+0x668/+0x66c`, cnt `+0x674/+0x678`). Mode → `+0x368/+0x36c` (WINPRS.md),
stored after table 2's body was read, even if that read failed.

## Read errors and validation

Return **2** on: header EOF/blank; origin EOF/blank; 3D `s` not 1/2 (after reset and origin);
shape body failure; value line EOF/blank; table body EOF. `win <= 0`: see WINPRS.md.

## KEY writer

`FUN_14029e250` step 51 (not object types 5/7), every entry of `obj+0x2f10`, no cleanup pass.
`c1 = (t1.ndim != 0)`; `c2` = 0 / 1 (`ndim 1`, `var[0] == 1`) / 2 (`ndim 1`, other var) / 3
(other ndim); `M` as in WINPRS.md.

Not 2D (3D or unset):

    "%-6s %7i %7i %7i %7i %7i %7i\n"   "WINATM", N, w+0x170, w+0x210, c1, c2, M
    " %20.10E %20.10E %20.10E\n"       origin x, y, z
    FUN_1402f4890(f, w, 0)

2D:

    "%-6s %7i %7i %7i %7i %7i %7i\n"   "WINATM", N, w+0x170, w+0x178, c1, c2, M
    " %20.10E %20.10E\n"               origin x, y
    FUN_1402f4890(f, w, 1)

then `FUN_1402f3130(f, w+0x5e0, 1)` and `FUN_1402f3130(f, w+0x658, 1)` (mode-1 layout in
WINPRS.md). Return values ignored.

Round-trip: as WINPRS.md; in 3D, placeholder windows (gaps below `win`) are written with type 1
and no corner lines and do not re-read.

## Unit conversion

None.

## Helpers

`FUN_140414200`, `FUN_140414c20`, `FUN_140414990`, `FUN_140414a40`, `FUN_14041a800`; shape/list
helpers and writers in WINPRS.md.

## Corpus check

No corpus file uses WINATM. Shape grammar shared with WINTMP, which is corpus-checked.

## Open questions

* Meaning of variable code 0xc and of the two tables (atmosphere value vs. transfer
  coefficient); settle from the variable-code name table or GUI strings.
