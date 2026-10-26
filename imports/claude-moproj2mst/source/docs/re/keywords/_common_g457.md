# Groups 4, 5, 7: shared reader/writer machinery

Groups 1 and 3 have their own `_common.md` (branch `claude/project-thread-qj8op8`), which covers
the same line helpers, DYNARY object and error codes; this file is the group 4/5/7 view and adds
the material and phase-pair objects, the extra helpers and the KEY writer order for materials.
Where both describe the same function they were derived independently from the same code.

Target `DEF_PRE_64.exe` v11.0.2 (image base 0x140000000). Everything here is recovered from the
decompiled code (headless Ghidra 12.1.4, project copy `deform-rea-work/ghidra_kw457`, outputs in
`deform-rea-work/out/kw457/`). Per-keyword specs in this folder refer to the names defined here.

## 1. Where the handlers really are

The registry (`docs/re/keyword_registry.tsv`) columns are **not** "read, write":

| Column | Group 4/5/7 content | Meaning |
|---|---|---|
| `handler1` | always `0x1402f7590` | no-op stub (destroys its `vector<int>` argument, returns 0) |
| `handler2` | per keyword, or stub `0x1402f75d0` | **KEY write** handler, `int f(FILE*, Model*, vector<int>{id...})` |
| `handler3` | per keyword, or stub `0x1402f7610` (returns -1) | **data accessor**: `int f(Model*, const char* label, vector<int>, void** out)` returning a pointer to the keyword's data inside the object (used by the interactive edit menus) |

The **KEY read** code is not in the registry: each group has one parser with a `switch` on the
keyword id, called from the KEY interpreter `FUN_1401b6520`:

| Group | Parser | Case index | Object key read first |
|---|---|---|---|
| 4 material | `FUN_140384a50` | `id - 0x5c` (92 MTNAME … 133 NUCSIZ) | `fscanf("%i")` material number |
| 5 phase transformation | `FUN_140383120` | `id - 0x86` (134 PHASTF … 143 VARSEL) | `fscanf("%i %i")` from-phase, to-phase |
| 7 dimension | `FUN_14037e530` | only id 0x120 (DIM) | none |

`FUN_1401b6520` passes the parsers of groups 1–6 `FUN_1401d81b0(model)`: normally the model's
data object, but the model's companion of the other dimension (`+0x33e8`) once `DIM` has set the
flag at `+0x33f0` (see DIM.md); group 7 gets the model itself.

The KEY writer `FUN_1401c5810` → `FUN_1401c5af0` writes material data in `FUN_1403cdca0` (one
call per material, in list order) and inter-material (phase) data in `FUN_1403cbf00` (one call per
phase pair, in list order). Those two functions call the handler2 functions in a fixed order
(section 6) and print a few keywords inline. Handler2 is never called through the registry
by the KEY writer.

## 2. Interpreter behaviour around a keyword

`FUN_1401b6520` (see the plan, section 2) reads the keyword with `fscanf("%39s")`; the stream is
then positioned right after the keyword name, **on the keyword line**. The group parser's first
`fscanf` reads the object number(s) from the same line; the following helpers read the rest of
that line with `fgets`.

Return codes of the group parsers: `0` ok, `2` "Reading Error", `3` "Data Error". In
`FUN_1401b6520`, both 2 and 3 print their message to stdout's FILE (`FUN_14047903c()+0x30` is
`__iob[1]`, i.e. **stdout**) and **reading continues** with the next token:

```
ERROR: Keyword %s Reading Error. \n
ERROR: Keyword %s Data Error. \n
```

Any other non-zero value would stop the loop (none of groups 4, 5, 7 produce one: every failing
path returns 2, except the explicit `return 3`s noted per keyword). Note that the interpreter does
not resynchronise after an error: the next `%39s` token is whatever follows the point where the
handler stopped, which may be a number that then yields "WARNING: Keyword %s not found. Skip %s".

## 3. Line helpers (reader side)

All of these read **one line** with `fgets(buf, 0x400, fp)` (at most 1023 chars, including the
`\n`), replace every `,` with a space (`FUN_140323970(buf, ',', ' ')`), zero all outputs, then
`sscanf` the buffer. They return 1 when `fgets` returns NULL (EOF) or `sscanf` returns -1 (the
line holds only whitespace), else 0. A short line (fewer numbers than the format) is **not** an
error: missing outputs stay 0. Text that does not convert stops the scan, again without error.

| Address | Format | Outputs | Notes |
|---|---|---|---|
| `FUN_140413de0(fp, int*, char skipBlank)` | `%i` | 1 int | if `skipBlank` and the line is exactly `"\n"`, reads one more line |
| `FUN_140413ee0(fp, int*, int*)` | `%i %i` | 2 int | |
| `FUN_140413fb0(fp, int*, int*, int*, int* nOut)` | `%i %i %i` | 3 int | if `nOut` given, stores the `sscanf` count |
| `FUN_140414100(fp, int*×4)` | `%i %i %i %i` | 4 int | |
| `FUN_140414990(fp, double*)` | `%Flf` | 1 double | |
| `FUN_140414a40(fp, double*×2)` | `%Flf %Flf` | 2 double | |
| `FUN_140414e40(fp, double*×4)` | `%Flf`×4 | 4 double | |
| `FUN_140414f50(fp, double*×5)` | `%Flf`×5 | 5 double | |
| `FUN_140415080(fp, double*×6)` | `%Flf`×6 | 6 double | |
| `FUN_1404151c0(fp, double*×7)` | `%Flf`×7 | 7 double | |
| `FUN_140415320(fp, double*×9)` | `%Flf`×9 | 9 double | |
| `FUN_1404154c0(fp, int*, double*)` | `%i %Flf` | | |
| `FUN_140415680(fp, int*, double*, double*)` | `%i %Flf %Flf` | | the usual "type n1 n2" header |
| `FUN_140415990(fp, int*, double*×4)` | `%i %Flf`×4 | | |
| `FUN_140415ac0(fp, int*, double*×5)` | `%i %Flf`×5 | | |
| `FUN_140415ff0(fp, int*, int*, double*, double*)` | `%i %i %Flf %Flf` | | |
| `FUN_140413ab0(fp, char* dst, int size)` | — | string | reads and **discards** the rest of the current line, then reads the next line, cuts it at the first `\n`, `strncpy(dst, line, size-1)`, then trims trailing `' '`, `'\n'`, `'\r'` and NULs, not tabs (the string is always NUL-terminated) |

`%i` is C's base-detecting integer: `0x1F` is hex and a leading `0` means **octal** (`08` reads
as 0 and stops). `%Flf` is MSVC's `%lf` with the obsolete `F` (far pointer) size prefix; it is
plain `strtod` syntax. Doubles that the code converts to int use C truncation `(int)d`.

## 4. DYNARY: the tabulated-function object

Most material and phase properties are a 0x78-byte "dynamic array" (RTTI family
`DYNARY_DATA`). Layout (byte offsets):

| Off | Type | Field |
|---|---|---|
| +0x00 | int | `type` (property id, picks defaults in `FUN_14012d2a0`) |
| +0x04 | int | `ndim`: 0 constant, 1–3 tabulated in that many variables |
| +0x08 | double | constant value (ndim 0) |
| +0x10 | int[3] | `var[k]`: independent-variable code of dimension k |
| +0x1c | int[3] | `n[k]`: points along dimension k |
| +0x28 | double*[3] | `axis[k]`: n[k] abscissae |
| +0x40 | int | `ncomp`: values per table point (default 1) |
| +0x48 | double* | `values`: `ncomp * Π n[k]` doubles (see `FUN_14012ff20`) |
| +0x50 | int | (0x14) cleared on init; FSTRES 2/3 and 7/8 store a per-keyword flag here after re-init |
| +0x54 | int | preserved by `FUN_14012fc70` |
| +0x58 | int | (0x16) cleared on init |
| +0x60 | 8 bytes | preserved by `FUN_14012fc70` |

Functions:

* `FUN_14012d050` constructor: zero, then `FUN_14012d2a0(this, 0)`.
* `FUN_14012d2a0(this, type)` (re)initialise: frees arrays, sets `type`, `ndim=0`, value 0,
  `var[]=n[]=0`, `ncomp=1`, then per-type defaults (table below), then allocates
  (`FUN_14012fad0`) unless type is 0x51, 0x65, 0x66 or 0x67.
* `FUN_14012fc70(this, type)`: same as `FUN_14012d2a0` but keeps +0x54 and +0x60.
* `FUN_14012fad0` allocate: frees, clamps negative `n[k]` to 0 and `ncomp<1` to 1, `calloc`s each
  axis and the value array.
* `FUN_14012ff20` total value count: if `ndim==0`, `ncomp` when `ncomp>=2` or type is one of
  0xb5, 0xa7, 0xa8, 0xaa, 0xab, else 0; otherwise `ncomp * Π n[k]` (negative → 0).
* `FUN_14012f610` "is complete": true when every `n[k]` (k < ndim) is ≥ 1 (true for ndim 0).

Per-type defaults of `FUN_14012d2a0` used by groups 4/5 (`ndim`, `var[]`, `ncomp`; other fields 0;
"–" = unchanged default):

| type | ndim | var | ncomp | | type | ndim | var | ncomp |
|---|---|---|---|---|---|---|---|---|
| 0x1b | – | – | 1 | | 0x52 | 2 | 13, 12 | 1 |
| 0x1c | 1 | 31 | 1 | | 0x53 | 1 | 1 | 1 |
| 0x1f, 0x20, 0x23 | – | – | 1 | | 0x54 | 1 | 3 | 6 |
| 0x24, 0x25, 0x26 | – | – | 1 | | 0x55 | 1 | 3 | 4 |
| 0x27 | 1 | 3 | 2 | | 0x56 | 1 | 3 | 7 |
| 0x28 | 1 | 12 | 2 | | 0x57, 0x59, 0x5b | 1 | 3 | 6 |
| 0x29 | 2 | 3, 12 | 1 | | 0x58 | 1 | 3 | 7 |
| 0x2c, 0x2e | 3 | 12, 6, 3 | 2 | | 0x5a | 1 | 3 | 9 |
| 0x2d, 0x2f | 3 | 7, 14, 3 | 2 | | 0x5c | 1 | 3 | 3 |
| 0x30 | 3 | 12, 15, 16 | 2 | | 0x5d | 0 | – | 1 |
| 0x32, 0x36, 0x38 | 1 | 3 | 1 | | 0x65 | – | – | 6 (no alloc) |
| 0x43 | 3 | 1, 6, 3 | 1 | | 0x67 | – | – | 1 (no alloc) |
| 0x44 | 3 | 7, 6, 3 | 1 | | 0x68 | 1 | 0 | 6 |
| 0x45, 0x46 | 1 | 3 | 2 | | 0x69 | 1 | 0 | 3 |
| 0x47 | 1 | 3 | 5 | | 0x6c, 0x6d, 0x6e | – | – | 1 |
| 0x48 | 1 | 3 | 3 | | 0xa6, 0xa9 | 1 | 0 (n0=1) | 2 |
| 0x37 | 3 | 12, 6, 3 | 2 | | | | | |
| 0x4f, 0x51 | 3 | 7, 8, 3 | 1 | | 0xa7, 0xa8, 0xaa, 0xab | 0 | – | 1 |
| 0x50 | 3 | 7, 12, 3 | 1 | | 0xae | 0 | value 1.0 | 1 |
| | | | | | 0xaf | 0 | – | 2 |
| | | | | | 0xb4 | 1 | 32 | 1 |
| | | | | | 0xb3 | 0 | – | 1 |
| | | | | | 0xb6 | 0 | – | 1 |
| | | | | | 0xac | 2 | 3, 8 | 1 |
| | | | | | 0x5e, 0x5f, 0x60 | 0 | – | 1 |

Independent-variable codes seen in groups 4/5: 1, 2, 3, 4, 6, 7, 8, 9, 12, 13, 14, 15, 16, 20,
23, 24, 30, 31, 32, 33. Meanings confirmed by the corpus: in FSTRES type 2 (DYNARY type 0x4f:
var 7, 8, 3) the three axes of `IN718_6um…_s000006.KEY` are 0…0.6, 0.001…100 and 900…1200, i.e.
**7 = strain, 8 = strain rate, 3 = temperature**. Also from corpus data: **6 = stress, 1 = time**
(CREEP type 6), **12 = carbon content** (FSTRES type 6, second axis 0.2 / 1.17). Other codes are
named only where a spec has evidence.

### Reading a DYNARY body: `FUN_14041a800(fp, dyn)`

Called after the header has set `ndim`, `var[]`, `n[]` (and `ncomp` via the type). Callers pass a third argument; the function ignores it. Does nothing
(returns 0) when `ndim==0 && ncomp<2`. Otherwise allocates (`FUN_14012fad0`) and reads with
`fscanf("%Flf")` (whitespace-separated, any line breaks):

* `ndim 0`: `ncomp` values → `values[]`.
* `ndim 1`: for each i < n0: `axis0[i]`, then `ncomp` values `values[i*ncomp + c]`.
  (rows of "x  v1 … vncomp")
* `ndim 2, 3`: all of `axis0`, then all of `axis1` (then `axis2`), then `ncomp·Π n` values in
  file order into `values[]` (read into a temporary buffer, then `memcpy`).

Any `fscanf` returning -1 (EOF) → return 1. A conversion failure (returns 0) is not detected: the
value stays 0 and the stream does not advance. After the values, if the total count
(`FUN_14012ff20`) is > 0, one `fgets` discards the rest of the last line.

### Generic "type n1 n2" property header: `FUN_14041b270(fp, dyn, wantExtra, double* extra)`

Reads the rest of the keyword line as `"%i %Flf %Flf %Flf"` → `t, a, b, c` (zeroed first);
EOF/-1 → return 1. Re-inits the DYNARY with type 0 (`FUN_14012fc70(dyn, 0)`: ncomp 1), then:

| t | ndim | var | n | KEY meaning |
|---|---|---|---|---|
| 0 | 0 | – | – | constant = `a` |
| 1 | 1 | 3 | (int)a | f(temperature) |
| 2 | 1 | 9 | (int)a | f(var 9) |
| 3 | 1 | 12 | (int)a | f(var 12) |
| 4 | 2 | 3, 12 | (int)a, (int)b | |
| 5 | 2 | 3, 8 | (int)a, (int)b | f(temperature, strain rate) |
| 6 | 1 | 8 | (int)a | f(strain rate) |
| 7 | 2 | 3, 4 | (int)a, (int)b | |
| other | 0 | – | – | constant 0 (type ignored) |

Extra value: if `wantExtra >= 0`, `extra != NULL` and `t >= 0`: for `t < 4` and at least 3 fields
converted, `*extra = b`; for `t == 4` and at least 4 converted, `*extra = c`. (Callers pass
`wantExtra = 0` or `1`; 0 also qualifies since the test is `>= 0`, but every caller with
`wantExtra = 0` passes `extra = NULL`.) Then `FUN_14041a800` reads the body.

### Sub-function line used by GRNDAT: `FUN_14041c070(fp, dyn)`

Reads one line (skipping one empty `"\n"` line; in GRNDAT type 2 this skip is what consumes the
end of the preceding `fscanf`'d `"<i> <d>"` line), `"%i %Flf %Flf %Flf"` → `t, a, b, c` with `t`
pre-set to **-1** and the `sscanf` result ignored (a blank line gives t = -1 and returns 0), and
does **not** re-init the DYNARY: `t=0` const `a`; `1`: ndim 1, var 3, n=(int)a; `2`: ndim 1,
var 23; `3`: ndim 2, var 3, 23, n=(int)a,(int)b; other t: nothing. Body via `FUN_14041a800` only
when `t > 0` (so t ≥ 4 reads a body with the DYNARY's previous shape).

### Writing a DYNARY body: `FUN_1402f3130(fp, dyn, mode)`

`mode` 0 is used by groups 4/5 (ANISO passes 1). Returns 2 if `fp` is NULL, 0 if `dyn` is NULL.
For modes 0 and 6 the body is written only when every `n[k] >= 1` (`FUN_14012f610`); modes 1/2
skip that check and write the n[] line with perLine = ndim (all-zero n → perLine 0 →
`FUN_1402f2e80` returns 2, which the TTTD writer ignores).

* `ndim 0`: only for mode 1, 2 or 6: `ncomp==1` → `" %20.10E\n"` const value; `ncomp>=2` → the
  `values` via `FUN_1402f2e80(…, 5)`. Mode 0 writes nothing.
* `ndim 1`: mode 1/2 first write `" %7i\n"` n0. Then for each row i: `" %20.10E"` axis0[i]
  followed by `FUN_1402f2e80(fp, ncomp, &values[i*ncomp], 5, 0)`.
* `ndim 2, 3`: mode 1/2 first write the n[] via `FUN_1402f2fe0` (`" %7i"` each). Then each axis
  k via `FUN_1402f2e80(fp, n[k], axis[k], maxN, mode==2)`, then all values via
  `FUN_1402f2e80(fp, total, values, maxN, mode==2)`, where `maxN = max(n[k])`.

`FUN_1402f2e80(fp, count, double* v, perLine, alt)`: returns 2 if `perLine < 1`, 0 without output
if `count < 1`; writes `" %20.10E"` per value (or, if `alt`, the format built from `" %20.10E"`
plus the string at `DAT_1405417fc`), and before every value whose index is a non-zero multiple of
`perLine` writes `"\n"` then 8 spaces; ends with `"\n"`. `FUN_1402f2fe0` is the same with
`" %7i"` for int arrays.

When `maxN` is 0 (an empty default 2-D/3-D table), the axis write returns 2 without output and
callers ignore it, so only the header / n line appears.

So a 1-D table row is `" x v1 v2 v3 v4 v5\n        v6 …\n"`, and in 2-D/3-D tables each axis
fills one line and the values wrap every `max(n)` values (one row per inner-axis sweep when the
largest dimension is the first one, as in FSTRES).

The KEY file is opened in text mode, so every `"
"` is written as CR LF (seen in every oracle
output); a port must do the same.

All `%E` output comes from the MSVC CRT of the binary: **three-digit exponents**
(`1.0000000000E+000`). A port must reproduce that (e.g. `_set_output_format` is not available in
new CRTs; format the exponent by hand).

### Generic header writers

`name` is the keyword name from `FUN_1402f7720(id)` (registry table at `DAT_140544400`, 0x30-byte
records; unknown id gives `"ERROR: KEYWORD NOT EXISTS!"`), `%-6s` left-justified in 6 columns.

**`FUN_1402f3aa0(fp, name, mat, dyn, hasExtra, double* extra)`** (YOUNG-style), then body
`FUN_1402f3130(fp, dyn, 0)`:

| ndim / var | hasExtra < 1 | hasExtra == 1 && extra |
|---|---|---|
| 0 | `"%-6s %7i %7i %20.10E\n"` name, mat, 0, const | `"%-6s %7i %7i %20.10E %20.10E\n"` …, *extra |
| 1 | `"%-6s %7i %7i %7i\n"` name, mat, t, n0 with t = 1 (var 3), 2 (var 9), 6 (var 8), else 3 | `"%-6s %7i %7i %7i %20.10E\n"` …, *extra |
| 2, var (3,12) | `"%-6s %7i %7i %7i %7i\n"` name, mat, 4, n0, n1 | same + `" %20.10E"` *extra |
| 2, var (3,4) | same with t = 7 | |
| 2, var (3,20) | same with t = 4 | |
| 2, other (e.g. (3,8) from t 5) | **no header line** (body still written) | |
| 3 | no header line | |

Special case for `name == "COARSE"` (see COARSE.md). `hasExtra` values other than 0/1 or a NULL
`extra` with `hasExtra == 1` print nothing for the header.

Note the asymmetry with the reader: t = 5 (var 3, 8) and t = 6 (var 8, ndim 1) read fine, but the
writer maps ndim-1 var 8 to t = 6 correctly while ndim-2 (3, 8) has no case. That is a bug in the
original that the port must reproduce.

**`FUN_1402f40f0(fp, name, mat, dyn, hasExtra, int* extra)`** (HEATCP): like `FUN_1402f3aa0` but the
extra is an int (`" %7i"`; ndim 0 with extra: `"%-6s %7i %7i %20.10E %7i
"`), the ndim-1 map is var 3 → 1, var 9 → 2, else 3, and every ndim-2
table gets t = 4.

**`FUN_1402f43a0(fp, name, a, b, dyn)`** / **`FUN_1402f4530(fp, name, a, b, c, dyn)`** /
**`FUN_1402f4690(fp, name, a, b, d1, d2, dyn)`**: variants with extra leading fields, used by
group 5 handlers; formats listed in the specs that use them.

## 5. The objects

### Material (`FUN_14029c060` allocates 0x2218 bytes, constructor `FUN_14028c6f0`)

Materials live in a singly linked list at model-data `+0x1328` (head `+0x1328`, tail `+0x1330`,
count `+0x1338`; next pointer at material `+0x2200`; `+0x2208` back-pointer to the model;
`+0x2210` byte "skip in KEY output", tested by `FUN_1403d4970`). `FUN_14029c2c0(list, n)` finds
by material number (`+0x0`).

On the first keyword that names a material number not yet in the list, `FUN_140384a50`:
1. `FUN_14029ca10(list, 0)`: rebuilds the number→index arrays at list `+0x18`/`+0x20`;
2. appends a new material, sets `+0x0 = n`;
3. stores the new count into model-data `+0x1320` (number of materials);
4. `FUN_1401d0fc0(modelData, 1)`: rebuilds the per-object state-variable layout for the new
   material set (it copies the 0x4f-int block at model-data `+0x15f8`, re-derives each object's
   state-variable tables and remaps the per-material columns of the arrays at object `+0xa58` /
   `+0xa60` using the old number→index arrays); see GRNDAT.md, which calls it with 0;
5. `FUN_14029ca10(list, 1)`: frees those arrays.

Material constructor defaults (beyond zero / empty DYNARY): name `""` (+0xC, 0x41 chars);
FRAE2H value 0.9 (+0x1ec8); several DYNARYs pre-typed (0x3e2·4=+0xf88 type 0x53,
+0x1000 type 0x52, +0x2160 type 0xaf, +0x1ef0 / +0x1f68 type 0, +0x17f0 type 0x5d,
+0x1878 type 0xb6), and the sub-objects at +0x50 (flow stress, `FUN_140288ae0`), +0x950
(`FUN_140289930`), +0xac8 (`FUN_140289b80`), +0x1078 (`FUN_14028b850`), +0x18f0
(`FUN_14028bcc0`), +0x1ad0 (`FUN_14028c040`), +0x1e40 (`FUN_140289fe0`), +0x2154
(`FUN_14028c500`: 0, 1, 1). The per-keyword specs give the defaults of the fields they use.

### Phase pair (`FUN_140339df0` allocates 0x780 bytes, constructor `FUN_140333f60`)

Inter-material records live in a list at model-data `+0x1358` (count via `FUN_1401b05f0`, next
pointer at `+0x770`), keyed by (`+0x0` from-material, `+0x4` to-material), found by
`FUN_14033a040`. A missing pair is appended without the material-side bookkeeping.

## 6. KEY writer order

Per material (`FUN_1403cdca0(modelData, fp, material, nameOverride)`; skipped if
`material+0x2210` is set):

```
*\n*  Property Data of Material     %i\n*\n        ("%s" built with sprintf, %i = number)
MTNAME (inline: "%-6s %7i\n%s\n", name = override if non-empty else material+0xC;
       the KEY writer passes no override: R9 = 0 at 0x1401c5baa)
FRAE2H FPERV FRCMOD FSTRES YOUNG POISON EXPAND THRCND HEATCP MASDEN COARSE DIFBND EMSVTY HDNPHA
HDNTIM JOMINY
MSTMTR (inline)  CREEP DIFCOE ELRST UTSDAT
HDNRUL (inline)  PMEAB PMITT
MATDEN (inline, 3D only: modelData+0x1c == 3, `FUN_1400015f0`)
ANISO GRNDAT BURGRS ALPHA NDISFM RECVRY
SIZEMD (inline) TXTURE (inline)
SINTER GBENGY GBMOBI NUCSIZ
```

If the model has no materials, `FUN_1401c5af0` writes `*\n*  Property Data of Material\n*\n`
once. Then `*\n*  Inter-Material Data\n*\n` and, per phase pair
(`FUN_1403cbf00(modelData, fp, pair, index)`, 1-based index in R9; only when `PHASTF` is on,
`pair+0xC != 0`; PHEQVF only when the PHASTF value is 2, COARSN only when its flag at `+0x660` is
set). The material-library writer `FUN_1401c5ea0` writes the same pair blocks (filtered by
`FUN_140339ba0`) after the phase materials of a mixture material (`+0x1cc8 == 1`):

```
*\n*  Inter-Material Data for     %i and %i\n*\n
TTTN   (inline: "%-6s %7i %7i\n" then "Transformation from %d -> %d\n")
PHASTF (inline: "%-6s %7i %7i %7i\n")
TTTD PHASLH PHASVL TRNSFP PHEQVF COARSN IPSNRG
VARSEL (inline: "%-6s %7i %7i %7i\n")
```

The first error return of any handler aborts the writer with that code.

Checked against the original: the KFWRIT round trips of the IN718 and JIS-S45C library KEYs
(oracle cases from `tools/oracle_cases457.py`) print the keywords in exactly this order, with
keywords whose handler writes nothing for the data at hand (HDNTIM, JOMINY, ANISO, SINTER,
PHEQVF, COARSN in those files) simply absent.

Keywords never written by the KEY writer: LTHEAT, TMPSOL, TMPLIQ, GRNMDL (registry write
handler is the stub and no inline write exists), DIM.
