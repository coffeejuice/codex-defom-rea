# Group 2 (object) keywords: shared reader helpers

Source: `DEF_PRE_64.exe` v11.0.2, Ghidra decompile of `FUN_14038b8a0` and its callees
(`deform-rea-work/out/g2/parser2.c`, depth 2). Every per-keyword spec in this folder names
helpers by address and refers to this file for their exact behaviour.

## How a group 2 keyword line is consumed

The KEY interpreter `FUN_1401b6520` reads the keyword with `fscanf("%39s")` and calls
`FUN_14038b8a0(model + ..., FILE*, keywordId)` (`model` from `FUN_1401d81b0`). The file position
is then just after the keyword token, still on the keyword's line.

1. **Object number.** Unless `keywordId == 0x11e` (286, not a group 2 id, so this branch never
   runs for group 2), the parser does `fscanf(f, "%i", &objNo)`; EOF returns **2**.
   `%i` is the C `%i` conversion: it accepts a sign, `0x…` hex and leading-`0` octal
   (`010` is 8). It skips any whitespace first, including newlines.
2. **Object lookup.** `obj = FUN_14019ccc0(model, objNo)`: get-or-create object `objNo` in the
   object list at `model+0x1380`:
   * `objNo < 1` → NULL → the keyword returns **2** ("Reading Error").
   * missing object: appends placeholder objects until the list holds `objNo-1` entries, each
     with `+8 = its 1-based number` and `+0xc = 7` (type 7, "empty"); then appends the object,
     sets `+8 = objNo` and `model+0x1378 = list count`. A new object's `+0xc` is set to 0 and its name at `+0x828`
   is cleared by `FUN_14015f0b0` (see OBJTYP.md).
3. **Dispatch** on `keywordId - 0x90` (`switch(local_68)`, see `group2-parser-map.md`).
   An id with no case (only 204 `OBJPHS` among the registry's group 2 ids) hits
   `default: return 2`.
4. **Result.** Every case leaves a helper status in `iVar2`; nonzero → the parser returns **2**,
   zero → **0**. A few cases return **2** or **3** directly (noted per keyword). The interpreter
   prints `ERROR: Keyword %s Reading Error. 
` (2) or `ERROR: Keyword %s Data Error. 
` (3) to
   stdout and **continues with the next token** (see `_common.md` section 1); whatever the
   handler left unread on the line is then scanned as keyword tokens.

Almost every helper starts with `fgets(buf, 1024, f)`, which reads **the rest of the current
line** (the part after the object number) or the next line if the object number was the last
token of the line and its newline has not been consumed yet. Since `fscanf("%i")` stops right
after the digits, the newline is still there: a helper's `fgets` after `KEY 1\n` returns `"\n"`.

## Rest-of-line scalar readers

Common behaviour of all helpers in this table:

* every output is set to 0 first;
* `fgets(buf, 0x400, f)`; NULL (EOF) → return **1**;
* `FUN_140323970(buf, ',', ' ')` replaces every comma by a space (commas are separators);
* one `sscanf(buf, FORMAT, ...)`; result `-1` (EOF before the first conversion, i.e. the line is
  empty or only whitespace) → return **1**; otherwise return **0**.
* Partial lines are accepted: `sscanf` stops at the first token that does not convert and
  the remaining outputs keep 0. A non-numeric first token gives `sscanf == 0` → success, all 0.
* `%Flf` is MSVC syntax: the `F` (far pointer) size prefix is ignored, so it reads a `double`
  like `%lf`. `%i` is as above (hex/octal aware).
* Text after the last conversion is ignored, the whole line is consumed.

| Address | FORMAT | Outputs |
|---|---|---|
| `FUN_140414990` | `%Flf` | d |
| `FUN_140414a40` | `%Flf %Flf` | d d |
| `FUN_140414c20` | `%Flf %Flf %Flf` | d d d |
| `FUN_140414e40` | `%Flf %Flf %Flf %Flf` | 4 d |
| `FUN_140414f50` | `%Flf` ×5 | 5 d |
| `FUN_140415080` | `%Flf` ×6 | 6 d |
| `FUN_1404151c0` | `%Flf` ×7 | 7 d |
| `FUN_1404154c0` | `%i %Flf` | i d |
| `FUN_140415590` | `%i %Flf %i` | i d i |
| `FUN_140415680` | `%i %Flf %Flf` | i d d |
| `FUN_140415880` | `%i %Flf %Flf %Flf` | i 3 d |
| `FUN_140415990` | `%i %Flf %Flf %Flf %Flf` | i 4 d |
| `FUN_140415ac0` | `%i %Flf %Flf %Flf %Flf %Flf` | i 5 d |
| `FUN_140415d70` | `%i %Flf %Flf %Flf %Flf %Flf %Flf %Flf` | i 7 d |
| `FUN_140415f00` | `%i %i %Flf` | i i d |
| `FUN_140415ff0` | `%i %i %Flf %Flf` | i i d d |
| `FUN_140416220` | `%i %i %Flf %Flf %Flf %Flf` | i i 4 d |
| `FUN_140416700` | `%i %i %i %Flf %i` | i i i d i |
| `FUN_140413ee0` | `%i %i` | i i |
| `FUN_140414100` | `%i %i %i %i` | 4 i |
| `FUN_1404144b0` | `%i` ×7 | 7 i |

Variants:

* `FUN_140413de0(f, int *v, char skipBlank)`: `%i`. If `skipBlank` and the first line read is
  exactly `"\n"`, one more line is read (EOF there → 1). Only one blank line is skipped.
* `FUN_140413fb0(f, a, b, c, int *nconv)`: `%i %i %i`; if `nconv` is not NULL it receives the
  `sscanf` result (return 1 only when that is -1).
* `FUN_140414200(f, 5 × int*, char skipBlanks)`: `%i %i %i %i %i`; if `skipBlanks`, keeps
  reading while the line is exactly `"\n"`. Returns `true` (1) on EOF or `sscanf == -1`.
* `FUN_140414600(f, 9 × int*, char skipBlanks)`: same with `%i` ×9.

## Line tokenizer

* `FUN_140413900(f, int *count, int maxTok)`: `fgets` into the global buffer `DAT_14070d060`
  (1024 bytes); EOF → 1. Clears token slots `0..min(maxTok,128)-1` of the global table
  `DAT_14070cc60` (128 `char*`), then `strtok(buf, " ,\n\t\r")` for slot 0 and
  `strtok(NULL, …)` for slots 1..maxTok-1, stopping at the first NULL. `*count` = number of
  tokens, **except** that slot 0 is always counted, so an empty line gives `*count = 1` with
  slot 0 = NULL. Returns 0.
* `FUN_140413a50(i)`: token `i`; returns `""` if `i` is outside 0..127 or the slot is NULL.
* Conversions used on tokens: `FUN_14047b6c0(s)` = `strtol(s, NULL, 10)` (decimal only, unlike
  `%i`), `FUN_14048754c(s)` = `atof(s)` (`_atof_l(s, NULL)`). Missing tokens convert from `""`
  to 0.

## String reader

`FUN_140413ab0(f, char *dst, int n)`:

1. `fgets` once and **discard** it (the rest of the keyword line);
2. `fgets` the next line; EOF on either → 1;
3. cut the line at the first `'\n'` (`FUN_14047dd0d` = `strchr`);
4. `strncpy(dst, line, n-1)` (pads with NULs when shorter; not terminated when longer);
5. scan `dst[n-2]` down to `dst[0]` for the last char that is not NUL, space, `\n` or `\r`;
   `dst[last+1] = 0` (all blank → `dst[0] = 0`). Leading spaces are kept; tabs are kept.

So the value sits alone on the line after the keyword, at most `n-1` characters, trailing
blanks removed.

## Table object and its body reader

The table ("curve") object, its templates `FUN_14012d2a0`/`FUN_14012fc70`, the body reader
`FUN_14041a800` and the table writer `FUN_1402f3130` are documented once for all groups in
`docs/re/keywords/_common.md` section 3 (groups 1+3 thread, branch
`claude/project-thread-qj8op8`). Key points for group 2:

* 0x78 bytes: `+0` template id, `+4` ndim, `+8` double constant (ndim 0, ncomp ≤ 1),
  `+0x10..+0x18` abscissa variable codes, `+0x1c..+0x24` point counts, `+0x28..+0x38` abscissa
  arrays, `+0x40` ncomp, `+0x48` values; `+0x54`/`+0x60` survive `FUN_14012fc70`.
* Group 2 readers usually call `FUN_14012fc70(t, id)` and then overwrite ndim, the variable
  code (`+0x10`) and the point count (`+0x1c`) from the keyword line before calling
  `FUN_14041a800(f, t, 0)` (the third argument is unused).
* `FUN_14041a800` reads with `fscanf("%Flf")`, free format across lines, commas **not**
  accepted; EOF → 1. A non-numeric token makes `fscanf` return 0, which is not detected: the
  value stays 0 and the token stays in the stream. After the data (if any) one `fgets`
  swallows the rest of the last line.
* The table writer `FUN_1402f3130(f, t, mode)` is in `_common.md` §3 for mode 0. Mode 1 (used by
  the window keywords) also writes the `" %7i
"` point-count line (and the size line for
  ndim 2–3) and the constant for ndim 0, and has no `cnt < 1` early exit; see WINPRS.md.

## Cases that read nothing in one dimension

Some cases do nothing in one dimension and return 0 after the object number: ZSTR and MOVANG in
3D; POLYTP, DIEPNT, DIEEDG and MGSIZM in 2D; ROTSYM when not 3D. The rest of the keyword line (and any data lines) stays in
the stream, and the KEY loop then reads those tokens as keywords
(`WARNING: Keyword %s not found. Skip %s`, one per line).

## Per-node and per-element value readers

* **`FUN_140416c90(f, double *dst, int n, int *ncomp, int mode)`** reads a header line then
  `index value…` records:
  * `mode 6` or `2` with `*ncomp > 10` → 1.
  * header by mode: 0 `"%i %Flf"` (count, default), 6 `"%i"`+10×`%Flf`, 1 `"%i %i %Flf"`
    (count, k, default), 2 `"%i %i"`+10×`%Flf` (count, k, defaults; sets `*ncomp = k`),
    3 `"%i %Flf %i"` (count, default, k); other modes → 1. No comma replacement.
  * `sscanf < 1` → 1. Let `nd` = conversions − 1 (modes 0, 6) or − 2 (modes 1, 2, 3).
    If `nd > 0`: when `nd == *ncomp` every node gets the defaults vector, else every node and
    component gets the first default.
  * then records: mode 0 → `FUN_1404199a0(f, dst, count, n, *ncomp)`; other modes →
    `FUN_140419b90(f, dst, count, n, *ncomp, k)`.
  Mode 6 never sets `k`, so its records are read as in mode 0. Mode 2 overwrites `*ncomp` with
  `k`, and its `> 10` check runs on the old `*ncomp`.
* **`FUN_1404199a0(f, dst, count, n, m)`**: `count ≤ 0` → 0. `count` times: `%i` index then
  `m` × `%Flf`; stored at `dst[(index-1)*m + j]` only if `1 ≤ index ≤ n` (others are read and
  dropped). Then one `fgets` (EOF → 1). `count ≤ 0` returns before that `fgets`. A
  non-numeric token is not detected: the buffer keeps the previous record's values, which are
  stored again.
* **`FUN_140419b90(f, dst, count, n, m, k)`**: `k == 0` → as `FUN_1404199a0`. Else `count`
  times: `%i` index then `k` values; stores the first `min(m, k)` at `dst[(index-1)*m + j]`
  when `1 ≤ index ≤ n`. **No** final `fgets`.
* **`FUN_140417c80(f, obj, code, n, &W, …)`**: same record grammar but allocates the field
  itself; header `"%i %i %i %Flf"` = count, flag, W, one default; flag 1 keeps the old values
  and overwrites the components selected by a mask line (see NRECVL.md). `FUN_1404176a0` is its
  twin without the flag handling (second int ignored). `FUN_1404192d0(…)`: `"%i %i"` + up to
  20 defaults (GRAIN.md). In mode 1 of `FUN_140416c90` only one default is read and the header's
  `k` becomes `FUN_140419b90`'s `k`.
* **`FUN_140419d40(f, int *dst, int n, int m)`**: header `"%i %i"` (count, default; no comma
  replacement). 2 conversions → fill all `n×m` with the default. Only `sscanf == -1` is rejected; a
  non-numeric count leaves an uninitialised local. Then
  `FUN_140419ea0(f, dst, count, n, m)`: integer version of `FUN_1404199a0` (`%i` values,
  final `fgets`).
* **`FUN_14041a090(f, double *dst, obj, m)`** and **`FUN_14041a440(f, int *dst, obj, m)`**:
  edge/face records (EC keywords; full detail in [ECCDEF.md](ECCDEF.md)). The third argument is
  the object; the 2D test is on the object's model (`obj+0x3a98`). Header `"%i %Flf"` /
  `"%i %i"` (count, default; no comma replacement); with 2 conversions the default fills all
  `NS × m` entries, `NS = FUN_140165170(obj)` (slot count). `count < 1` → 0. Then `count`
  free-format records: 2D `idx n1 n2` + `m` values, 3D `idx n1 n2 n3 n4` + `m` values (`idx`
  ignored, only `fscanf == -1` checked); slot = `FUN_140200c90(obj, n1, n2)` (2D) or
  `FUN_140200ff0(obj, n1..n4)` (3D). The double reader stores when slot ≥ 0 and drops others
  silently; the int reader stores when `0 ≤ slot < NS`, else prints
  `"ERROR in readEdgeFaceVarInt, iECCArrayAdr
"` to stdout for that record. Final `fgets`.

## Object defaults

A new object is reset by `FUN_14015f0b0`, which calls `FUN_140186200` (sub-structure at
`obj+0x1ea0`: properties, REFPOS, OTPRNG, ELPSOL, FRC*, CSFREQ, VOLCRG, ROTSYM, TRGVOL, USRSUB,
OPSTOP list at `+0x20b8`), `FUN_14018b500` (`obj+0x2160`: PRSNAM, movement, OBJSTP, PRSTRE) and
`FUN_140189790` (`obj+0x2d10`: mesh generation and remesh). Each spec gives its defaults.

## Linked lists

Object lists are `SfLinkedList` (`+8` head, `+0x10` tail, `+0x18` count; vtable slot 1 creates
an entry); entries start as `SfLinkedData` (`+8` byte flag, initially 1; `+0xc` int; `+0x10` next).
Get-or-create, append and n-th are `FUN_14025a990`, `FUN_14025a7d0`, `FUN_14025a9e0` (OPSTOP.md,
DYNARY.md). The n-th lookup (`FUN_14025a9e0`, and `FUN_140269900` for window lists) returns the
**head** for n ≤ 1, including 0 and negative n; window and DYNARY entries are addressed by list
position, not by a stored number.

## Dimension tests

`FUN_1400015e0(model)` is `model+0x1c == 2` (2D) and `FUN_1400015f0(model)` is
`model+0x1c == 3` (3D). The KEY reader sets the dimension from a `DEFORM 2D/3D…` comment.
