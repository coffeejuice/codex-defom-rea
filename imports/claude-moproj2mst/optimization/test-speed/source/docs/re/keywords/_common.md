# Shared mechanics for keyword groups 1 and 3

Target: `DEF_PRE_64.exe` v11.0.2 (2014-09-29). Everything here was recovered from the code
(headless Ghidra decompiles plus disassembly where the decompiler hid argument order). Corpus KEY
files were used only to check the grammar afterwards (`tools/check_group13_grammar.py`).

## 1. How a keyword reaches its handler

KEY interpreter loop `FUN_1401b6520`:

1. `fscanf(f, "%39s", name)`. Empty or `*`-prefixed tokens: the rest of the line is read with
   `fgets(1024)` (comment; the first comment may switch dimension, see the plan).
2. `FUN_1402f76c0(name, &group)` looks the name up in the 304-entry table at `0x140544400`
   (48-byte records: `+0` id, `+8` name pointer, `+0x10` group). Unknown → `fgets` the rest of the
   line and print `WARNING: Keyword %s not found. Skip %s\n` (suppressed for `SRFNRG`).
3. `obj = FUN_1401d81b0(model)` (the model itself, or its sub-model at `model+0x33e8` when the byte
   at `model+0x33f0` is set), then dispatch: group 1 → `FUN_14039aa90(obj, f, id)`, group 3 →
   `FUN_140388fb0(obj, f, id)`.
4. Return codes: `0` continue; `2` → print `ERROR: Keyword %s Reading Error. \n` and **continue**;
   `3` → print `ERROR: Keyword %s Data Error. \n` and **continue**; any other non-zero value
   stops the loop and is returned. Both group parsers only ever return 0, 2 or 3.
   All these messages go to `stdout` (`FUN_14047903c()+0x30` is `&_iob[1]`).

Important consequence: the keyword token is consumed by `fscanf`, so the *rest of the keyword's
own line* is what the handler's first `fgets` sees. "Line 0" below means that remainder.

## 2. Line helpers (file `FUN_1404xxxxx` family)

All of them `fgets(buf, 1024, f)` one line, replace every `,` with a space (`FUN_140323970`),
zero their outputs first, then `sscanf` with a fixed format. They return 1 when `fgets` hits EOF
or `sscanf` returns -1 (no conversion at all), else 0. Fields missing from the line therefore
keep the value 0 written before the scan. The group parsers map any non-zero helper result to
return code 2 unless noted.

| Helper | Format | Notes |
|---|---|---|
| `FUN_140413de0(f,&i,skipBlank)` | `%i` | if `skipBlank` and the line is exactly `"\n"`, reads one more line (never set by groups 1/3) |
| `FUN_140413ee0` | `%i %i` | |
| `FUN_140413fb0(f,a,b,c,&n)` | `%i %i %i` | optional count of converted fields |
| `FUN_140414100` | `%i %i %i %i` | |
| `FUN_140414200(...,skip)` | `%i %i %i %i %i` | `skip` = skip empty lines first (0 here) |
| `FUN_140414600(...,skip)` | `%i` ×9 | |
| `FUN_1404147e0(f,dst,&n)` | `%i` ×10 | copies `min(n,10)` ints to `dst`; returns 1 only on EOF |
| `FUN_140414990` | `%Flf` | `%Flf` is MSVC `%lf` (double) |
| `FUN_140414a40` | `%Flf %Flf` | |
| `FUN_140414c20` | `%Flf %Flf %Flf` | |
| `FUN_140414d10` | `%Flf %Flf %Flf` | if exactly 1 field converted, all three outputs are reset to 0 |
| `FUN_140414e40` | `%Flf` ×4 | |
| `FUN_140414f50` | `%Flf` ×5 | |
| `FUN_1404154c0` | `%i %Flf` | |
| `FUN_140415680` | `%i %Flf %Flf` | |
| `FUN_140415880` | `%i %Flf %Flf %Flf` | |
| `FUN_140415f00` | `%i %i %Flf` | |
| `FUN_140416100` | `%i %i %Flf %Flf %Flf` | |
| `FUN_1404164e0` | `%i %i %i %Flf` | |
| `FUN_1404165e0` | `%i %i %i %Flf %Flf` | |
| `FUN_140416aa0` | `%Flf %i` | |
| `FUN_140413ab0(f,dst,n)` | string | reads **two** lines: line 0 is discarded, the next line is the value. Cuts at the first `\n` (`strchr`), `strncpy(dst, line, n-1)`, then trims trailing NUL/space/`\n`/`\r` (scan from `n-2` down) |
| `FUN_140413c60(f,dst,n)` | string | same as above but on a single line (no discarded line) |
| `FUN_140413900(f,&cnt,max)` | tokens | `fgets` into the global buffer `0x14070d060`, `strtok` with `" ,\n\t\r"`, keeps up to `max` token pointers in `0x14070cc60[]`; `FUN_140413a50(i)` returns token `i` or `""`. Values are converted with `strtol(...,10)` (`FUN_14047b6c0`) or `atof` (`FUN_14048754c`) |

Note `%i` accepts octal/hex prefixes (`010` = 8, `0x10` = 16); the `strtol` paths are base 10.

## 3. The table object ("curve", 0x78 bytes)

Used for every value that may be a constant or a function. Layout (int index / byte offset):

| Off | Field |
|---|---|
| `+0x00` | template id |
| `+0x04` | `ndim`: 0 = constant (or plain vector), 1 = one abscissa, 2–3 = multi-dimensional |
| `+0x08` | double constant (used when `ndim == 0` and `ncomp <= 1`) |
| `+0x10..+0x18` | abscissa variable code per dimension (`var[0..2]`) |
| `+0x1c..+0x24` | point count per dimension (`cnt[0..2]`) |
| `+0x28..+0x38` | abscissa arrays per dimension (double) |
| `+0x40` | `ncomp`: values per point |
| `+0x48` | value array (double, `ncomp × Πcnt`) |
| `+0x54`, `+0x60` | preserved across re-initialisation (list link etc.) |

* `FUN_14012d2a0(t, id)` frees the arrays, resets the fields, applies the per-id template
  (big switch; ids used here: `0x15`, `0x16`, `0x17`, `0x4e`, `0x80`, `0xb0`–`0xb2`, `0xb5`, and
  1, 2, 0x1d, 0x1e which fall to the default `ndim=0, ncomp=1`), then allocates (`FUN_14012fad0`).
  `FUN_14012fc70` does the same but keeps `+0x54` and `+0x60`.
* `FUN_14012fad0` clamps (`cnt<0→0`, `ncomp<1→1`, `FUN_14012fc00`) and callocs the arrays.
* `FUN_14012ff20` (total values): `ndim==0` → `ncomp` if `ncomp>=2` (or template ids
  `0xb5,0xa7,0xa8,0xaa,0xab`), else 0; `ndim>0` → `ncomp × Π cnt` (negative → 0).

### Table reader `FUN_14041a800(f, t)`

Does nothing unless `ndim != 0` or `ncomp > 1`. Then re-allocates and reads with
`fscanf("%Flf")` (free format: numbers may span lines):

* `ndim == 0`: `ncomp` values (only if `ncomp >= 2`).
* `ndim == 1`: for each of `cnt[0]` points: abscissa, then `ncomp` values.
* `ndim == 2..3`: all abscissae of dimension 0, then of dimension 1 (, 2), then all
  `ncomp × Π cnt` values in storage order.
* If the total (above) is `> 0`, one `fgets` swallows the rest of the last data line.

Any `fscanf` returning -1 → returns 1. **The group parsers usually store this result as the
handler status, so a short table gives "Reading Error".**

### Table writer `FUN_1402f3130(f, t, mode)` (mode is always 0 in groups 1/3)

* Returns 0 without output when some `cnt[i] < 1` (`FUN_14012f610`).
* `ndim == 0`: mode 0 writes nothing.
* `ndim == 1`: per point: `" %20.10E"` abscissa, then the `ncomp` values through
  `FUN_1402f2e80(f, ncomp, vals, 5, 0)`.
* `ndim == 2..3`: abscissae per dimension then all values, each through `FUN_1402f2e80` with
  `perLine = max(cnt)`.

`FUN_1402f2e80(f, n, v, perLine, alt)`: for `i` in `0..n-1`: before item `i>0` with
`i % perLine == 0` print `"\n"` then 8 spaces; print `" %20.10E"`; finally `"\n"`.
`FUN_1402f2fe0` is the same for ints with `" %7i"`.

So a 1-D table with `ncomp = 1` is written as one line per point: `" x v\n"`.

## 4. Simulation-control object and model offsets

The group 1 parser receives the model `M`; the simulation-control object is embedded at
`S = M + 0x750` (its methods take `S`; specs give both `M+` and `S+` offsets where useful).
`S+0xbc0` points back to the model. Dimension tests: `FUN_1400015e0(M)` is `M+0x1c == 2` (2D),
`FUN_1400015f0(M)` is `M+0x1c == 3` (3D).

Defaults are set when the model is constructed (`FUN_1401c8950` → `FUN_1401c8c00` →
`FUN_1401e7c50(S)` = `FUN_1401e7e90`, `FUN_1401e8490`, `FUN_1401e8650`, `FUN_1401e8fb0`,
`FUN_1401e96b0`, `FUN_1401ea6b0`, `FUN_1401ead60`). Defaults that depend on 2D/3D use the
dimension the model has at construction.

`S+0x240` (`M+0x990`) is the "step definition by time" flag: `FUN_1401ee580(S)` reads it,
`FUN_1401ee5a0(S,&v)` sets it, and `FUN_1401ee510(S)` recomputes it when no `DTTYPE` table is
active (`M+0x93c == 0`): flag = 1 if `DSMAX (M+0x930) <= 0`, else 0.

Write options: `FUN_1401c5810(model, path, &opt)` stores `opt` at `M+0x1744`; bit 1 suppresses
`NSTART`, bit 4 suppresses `MESHNO`, bit 8 replaces the first two `TNOW` values. Read options at
`M+0x1740`: bit 2 sends `MESHNO`, bit 4 sends `NSTART` to a dummy.

## 5. KEY writer order for group 1

`FUN_1401c5af0` → `FUN_1401c5a20` (banner) → **`FUN_1403d4990(M, f)`**, which writes, in order
(section banners are `"*\n*  %s\n*\n"`):

`Process Definition`: TITLE, CURSIM, OPRNAM, SIMNAM, (recompute SMODE), SMODE, STYPE, GEOTYP (2D),
MESHNO (opt bit 4 clear), UNIT, TNOW, TRANS, HTMTHD, STNOUT, ELMNOD.
`Stopping & Step Controls`: NSTART (opt bit 1 clear), STPINC, PDIE, OSCTRL (2D), NSTEP, EMAX, TMAX,
SMAX, VMIN, LMAX, STPDEF, DEMAX, then DSMAX/DTMAX/`DTTYPE 0 0` or DTTYPE-table, SLDERR, DTPMAX,
DVMAX, MDSOBJ, VFACTR (3D) or CNTERR + VFACTR (2D), DTSUB, DPLEN (3D), ODMAX (3D).
`Iteration Controls`: CVGERR, ITRMTH, SOLMTD, ITRMXD, DEFBWD, SOLMTT, ITRMXT, TMPBWD, LATENT, FRQNCY.
`Processing Conditions`: GEOERR, PENINF, UNTE2H, TINTGF, BLZMAN, ENVTMP, FRNNAM, CNVCOF, ENVMPR,
ENVMPT, EHRATE, ENVATM, ACVCOF, TIMEID, ERECID, NRECID.
`User Defined Variables`: USRDEF, UNNAME, UENAME, CVNAME.

Not written: DSMAX_NSTEP (folded into DSMAX), INIGES (read and discarded).
The first non-zero return from a per-keyword writer aborts the whole KEY write.

Every keyword name is printed with `"%-6s"` from `FUN_1402f7720(id)` (registry lookup by id; a
missing id would print `ERROR: KEYWORD NOT EXISTS!`).

## 6. Registry handler columns

Column 1 (`0x1402f7590`) and the column-2/3 default (`0x1402f75d0`, `0x1402f7610`) are stubs that
return 0 / 0 / -1. Group 1 column 2 entries (`FUN_1403d6c90`…`FUN_1403d8420`) are exactly the
per-keyword writers that `FUN_1403d4990` calls directly (signature `(FILE*, M, vec)`); column 3
(`FUN_1403d6ec0`, `…7050`, `…71e0`, `…7530`, `…7e80`) returns a pointer to that keyword's table
(`*out = M + off`, skipped when `M == -0x750`, i.e. `S == NULL`).
Group 3 column 2 entries (`FUN_1403cab90`, …) take an int vector `{pairIndex, master, slave}`:
if `vec[0] >= 1` the pair is the `vec[0]`-th list element (`FUN_1401fac90`), else it is found by
`(vec[1], vec[2])` (`FUN_1401facf0`); then they call the same per-pair writer the KEY writer uses.
They return 3 when the vector has fewer than 2 (FRCFAC: 3) elements. Column 3 returns the
address of the pair's table. The KEY writer itself does not go through the registry.

## 7. Inter-object pair list (group 3)

Pairs live in a singly linked list at `M+0x13c8` (`{head, tail, count}`; next pointer at
pair `+0x390`), element size `0x3a0`, constructor `FUN_1401f46e0` (via `FUN_1401faaa0`, which
appends). `FUN_1401facf0(list, m, s)` finds a pair by `(+0x168 master, +0x16c slave)`.
Group 3 keyword lines start with the two object numbers; see `CNTACT.md` for the mirroring rule
that copies every pair `(m,s)` into `(s,m)` after each keyword.
