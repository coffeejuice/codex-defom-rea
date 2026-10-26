# WINTMP (id 227, group 2, case 0x53)

Window-defined thermal boundary condition (heat exchange with the environment inside a 2D polygon
or 3D box/cylinder window). Records are `LLWINDEF_DATA` entries of list `obj+0x2ee0`, reset to
BC kind 3. Record layout, list, shape grammar, shape writer and attachment mode: see WINPRS.md.

## KEY grammar (read)

The header is read by the **tokenizer** `FUN_140413900(f, &cnt, 7)` (separators space, comma,
tab, CR, LF; tokens converted with `strtol(..,10)`, missing tokens = 0):

    WINTMP <objNo:%i> <win> <n|s> <t1> <t2> <t3> <mode> [<t4>]
    <ox:%Flf> <oy:%Flf> [<oz:%Flf>]               FUN_140414c20 (3 doubles)
    shape body                                    2D: n lines "x y" (FUN_14041b6a0(f,w,n,0))
                                                  3D: s = 1 box / 2 cylinder (FUN_14041b7d0(f,w,1))
    <v1:%Flf>              [table 1 body]         FUN_140414990
    <v2:%Flf> [<v2b:%Flf>] [table 2 body]         FUN_140414a40 (2 doubles)
    <v3:%Flf> [<v3b:%Flf>] [table 3 body]         FUN_140414a40
    only if the header has >= 7 tokens:
    <v4:%Flf>              [table 4 body]         FUN_140414990

"2D" here is `FUN_1400015e0`; anything else (3D or unset dimension) takes the 3D path.
The tokenizer stores at most 7 tokens; `cnt == 7` (7 or more tokens) enables table 4.

Function codes:

| code | table 1 (`+0x380`), table 4 (`+0x4e8`) | tables 2 (`+0x3f8`) and 3 (`+0x470`) |
|---|---|---|
| 0 | const = v | const = v |
| 1 | `ndim 1`, var 1, `cnt = (int)v` | `ndim 1`, var 1, `cnt = (int)v` |
| 2 | — (untouched) | `ndim 1`, var **3**, `cnt = (int)v` |
| 3 | — | `ndim 2`, var (1, 3), `cnt = ((int)v, (int)v2b)` |
| other | untouched (reset state, value discarded) | untouched |

Table bodies are read by `FUN_14041a800` (`_common.md` §3): `ndim 1` → `cnt` lines `x y`;
`ndim 2` → all time values, all temperature values, then `cnt0 × cnt1` values, free format.
Variable 1 = time, 3 = temperature (corpus: table 2 with code 2 has abscissae 68…2192 °F).
Table 1 is the environment temperature (corpus constant 68 °F / 20 °C), table 2 the heat
transfer coefficient (h(T) table in the corpus); tables 3 and 4 are 0 in every corpus record.

## Model storage

Window `w = FUN_140269bc0(obj+0x2ee0, win)` (get-or-create by position, see WINPRS.md), then
`FUN_14025d1a0(w, 3)`. Origin → `+0x350/+0x358/+0x360`. 2D: `+0x178 = n`, points at `+0x180`
(`+0x210` stays 1). 3D: `+0x210 = s`, body as in WINPRS.md, then `FUN_140267760` (derived points).
Tables: 1 `+0x380` (ndim `+0x384`, const `+0x388`, var `+0x390`, cnt `+0x39c`), 2 `+0x3f8`
(`+0x3fc`, `+0x400`, var `+0x408/+0x40c`, cnt `+0x414/+0x418`), 3 `+0x470` (`+0x474`, `+0x478`,
`+0x480/+0x484`, `+0x48c/+0x490`), 4 `+0x4e8` (`+0x4ec`, `+0x4f0`, `+0x4f8`, `+0x504`).
Attachment mode → `+0x368/+0x36c` (WINPRS.md table), stored after all tables were read, even
when table 4's body failed.

## Read errors and validation

Return **2** on: tokenizer EOF; origin line EOF/blank; 3D with `s` not 1/2 (window already
reset, origin stored); shape body failure; any value line EOF/blank; any table body EOF.
An empty header line gives `win = 0` (see WINPRS.md: overwrites entry 1, or NULL dereference
when the list is empty). No other validation; 2D `n <= 0` reads no point lines.

## KEY writer

`FUN_14029e250` step 49/50 (not for object types 5, 7).

Step 49 (cleanup, modifies the model): `k = 0; while (k < count) { e = nth(k+1); if
(!FUN_1402632d0(e)) { FUN_1402696f0(list, e); k = 0; } k++; }`. `FUN_1402632d0` is "has a shape":
window not 3D → `+0x180 != NULL`; 3D → type 3: mesh node data non-NULL, else `+0x180 != NULL`.
`FUN_1402696f0` unlinks and deletes the entry (count − 1). Quirk: after a removal the scan
restarts at entry **2**, so entry 1 is only tested once (an invalid entry that becomes entry 1
after a removal survives). Window numbers `+0x170` are not renumbered.

Step 50, for each remaining entry. Codes written: `c1 = (t1.ndim != 0)`;
`c2`/`c3` = 0 if `ndim == 0`, 1 if `ndim == 1` and `var[0] == 1`, 2 if `ndim == 1` otherwise,
3 for any other `ndim`; `c4 = (t4.ndim != 0)`; `M` as in WINPRS.md.

Not 2D (3D or unset):

    "%-6s %7i %7i %7i %7i %7i %7i %7i %7i\n"  "WINTMP", N, w+0x170, w+0x210, c1, c2, c3, M, c4
    " %20.10E %20.10E %20.10E\n"              origin x, y, z
    FUN_1402f4890(f, w, 0)

2D:

    "%-6s %7i %7i %7i %7i %7i %7i %7i %7i\n"  "WINTMP", N, w+0x170, w+0x178, c1, c2, c3, M, c4
    " %20.10E %20.10E\n"                      origin x, y
    FUN_1402f4890(f, w, 1)                    (one "x y" line per point)

then always `FUN_1402f3130(f, w+0x380, 1)`, `(.., w+0x3f8, 1)`, `(.., w+0x470, 1)`,
`(.., w+0x4e8, 1)` (mode 1 layout in WINPRS.md). The 7-token header is always written, so table
4 is always written and re-read. Return values ignored.

Round-trip: as WINPRS.md (code from `ndim`/`var`; a table-1 `ndim 1` with var ≠ 1 is written as
code 1). Windows without points are dropped by step 49 (so placeholders do not break the 3D
reread here).

## Unit conversion

None (EN files carry °F, SI files °C, unchanged).

## Helpers

`FUN_140413900`, `FUN_140413a50`, `FUN_14047b6c0` (header tokens); `FUN_140414c20`,
`FUN_140414990`, `FUN_140414a40`, `FUN_14041a800`; shape/list helpers in WINPRS.md;
`FUN_1402632d0` (has shape), `FUN_1402696f0` (remove entry; fixes `LLRANGEDEF_DATA` links at
`+0x920/+0x928` of neighbours via `__RTDynamicCast`), `FUN_1402f4890`, `FUN_1402f3130`.

## Corpus check

All 8 files from corpus_kw.tsv (6 × 2D, 2 × 3D; 18 records) were parsed with this grammar by a
script (`AppData\Local\Temp\claude\F_chk\chk.py`); every record ends exactly before the next
keyword. 12 records (EN files and 2D SI induction files) are byte-identical to the writer layout
regenerated from the parsed values (MSVC 3-digit exponents); `2D/SI/HeatTreat/HT_CRACK_SI.KEY`
(6-integer header, no table 4, `%15.7E`-style numbers) and `3D/SI/.../3D_Induction_Hardening_Example.KEY`
come from another writer but parse. Excerpt (3D EN, cylinder, mode −3):

    WINTMP       1       1       2       0       0       0      -3       0
        0.0000000000E+000    0.0000000000E+000    0.0000000000E+000
        0.0000000000E+000    3.3150700000E-001

(the last line is the radii line after the two axis points).

## Open questions

* Physical meaning of tables 3 (`+0x470`, emissivity?) and 4 (`+0x4e8`, heat flux?); no corpus
  record has non-zero values. Settle from the GUI/solver use of these offsets.
