# GENSUB (id 20, group 6, `case 0x14` of `FUN_140378e00`)

Source: `DEF_PRE_64.exe` v11.0.2, static analysis only (`out/bm_cmd.c` lines 1760-1802, dispatcher
listing `0x14037dcc5..0x14037dee7`, closure `out/p67/gensub.c`, listing of `FUN_14037e5c0`
`0x14037e5c0..0x14038231c` for every `fscanf`/`sscanf`/`fprintf` argument list). Shared
mechanics: [_common.md](_common.md). Facts are marked **[fact]**, deductions **[inferred]**.

## 1. Summary

Prepares the input files of an external sub-meshing step for one **brick-meshed** object:

* `MESH.DAT`: a reduced KEY-like dump of the object's mesh and state (RZ, ELMCON, NDTMP, STRAIN,
  STNCMP, DAMAGE, GRAIN, USRELM, USRNOD, BCCTMP, ECCTMP, BCCDEF, MESHNO, MATAXI), produced by
  writing the object with the KEY writer to `AX_BIL.KEY` and converting that file;
* `DEF_SUB.INI`: two lines (object, node and element counts; one connectivity offset and the user
  value).

It also deletes the files of a previous run (`AX_BIL.KEY` after use, `NEWBRICK.KEY`).
**No external program is started** by this case **[fact]** (only file helpers are called). The
names `AX_BIL.KEY`, `MESH.DAT`, `DEF_SUB.INI`, `NEWBRICK.KEY` also occur in `DEF_DOE.exe` and
`auto_test23.exe` of the install, and `NEWBRICK.KEY` in `DEF_GUI_MO_64.exe`; the consumer of
MESH.DAT / DEF_SUB.INI and producer of NEWBRICK.KEY is one of those flows **[inferred]**.
`GENSUB 0` only deletes `DEF_SUB.INI`. Same code in 2D and 3D, but the converter is written for
3D bricks: a tet mesh (`npe = 4`) gives no MESH.DAT, and a 2D object is mis-parsed (section 4.3)
**[fact]**.
Not used in the corpus.

```
GENSUB <objNo> <value>
```

## 2. Argument grammar

| Line | Reader | Content |
|---|---|---|
| rest of the GENSUB line | `FUN_1404154c0(f, &objNo, &value)` | outputs zeroed, `fgets(0x400)`, `,` → space, `sscanf("%i %Flf")` |

Quirk **[fact]**: the reader result is discarded (test of the dispatcher status `[RSP+0x12d4]`,
0 at entry). EOF / blank → `objNo = 0` → the delete branch. One line consumed.

## 3. Dispatcher part (listing `0x14037dcc5..0x14037dee7`) **[fact]**

1. `one = 1` (`[RSP+0x65e0]`), read the arguments.
2. `objNo == 0`: `FUN_1401b85d0("DEF_SUB.INI")` (delete if it exists) → status 0.
3. Else `ini = fopen("DEF_SUB.INI", "wt")` (`FUN_14047b934`, mode `0x1405546e4`). NULL → status 0,
   nothing else. **The file is created before the object check.**
4. `obj = FUN_1401d6ea0(model, objNo)`; NULL → **return 2** ("Reading Error"); `DEF_SUB.INI`
   stays open (never closed, empty) **[fact]**.
5. `meshNo = 1` (`[RSP+0x6608]`). `k = fopen("AX_BIL.KEY", "wt")`;
   `FUN_14029e250(model, k, obj, 0, 0)` (the per-object KEY writer of KFWRIT/WRTOBJ, see
   [WRTOBJ.md](WRTOBJ.md); not NULL-checked here); `fclose(k)`.
6. `FUN_14037e5c0(model, "AX_BIL.KEY", "MESH.DAT", &outObj, &outNodes, &outElems, &meshNo)`
   (section 4). Its return value and the three outputs are not used **[fact]**.
7. `FUN_1401b85d0("AX_BIL.KEY")`.
8. `c = FUN_140183cb0(obj+0x9e0)` (connectivity array `obj+0x9e8`, after the accessor's sync
   `FUN_1401d5820`), twice; `off = c[4] - c[0]` (for bricks: node 5 minus node 1 of element 1;
   for 4-node elements `c[4]` is node 1 of element 2). No NULL test: an object **without a mesh**
   (NULL connectivity) crashes here **[fact]**.
9. `fprintf(ini, "%6d %6d %6d\n", objNo, obj+0x880 (nodes), obj+0x884 (elements))`;
   `fprintf(ini, "%6d %6d %20.10E\n", 1, off, value)` (arguments from the listing: `R8`, `R9`,
   `[RSP+0x20]`); `fclose(ini)`.
10. `FUN_1401b85d0("NEWBRICK.KEY")`. Status 0.

`FUN_1401b85d0(name)`: opens the file to test existence, closes it, then deletes it; nothing
happens when it does not exist **[fact]**.

## 4. Converter `FUN_14037e5c0(model, inName, outName, &o1, &o2, &o3, &meshNo)`

### 4.1 Frame

* `in = fopen(inName, "rt")`; NULL → return 1 (no MESH.DAT) **[fact]**.
* Main loop: `line = FUN_1401b83d0(buf, 500, in)` (getc loop up to 499 chars incl. `\n`; returns
  0 only at EOF with nothing read). On EOF → write MESH.DAT (4.4), return 0.
* Per line, a fixed sequence of keyword blocks: RZ, BCCTMP, ELMCON, BCCDEF, NDTMP, STRAIN, STNCMP,
  DAMAGE, GRAIN, USRELM, USRNOD, ECCTMP, MATAXI. Block `K` runs only if `status == 0` and
  `strstr(line, "K")` (substring anywhere: `URZ`, `FRZ`, `DRZ`, `PRZ` lines enter the RZ block)
  **[fact]**. Each block `sscanf`s the header (`%s` token first) and then, except ELMCON, checks
  `strcmp(token, "K") == 0`. **The header `sscanf` runs before the `strcmp`**, so matching-substring
  lines also overwrite the header variables (notably `obj` and `C`, below) **[fact]**.
* Data rows are read with `fscanf` straight from `in` (rows are not line-checked).
* After each block, `status == 1` → append `"\tERROR : <K> could not be read\n"` (`<K>` = `RZ
  co-ordinates` for RZ, else the keyword) to `DEF_CTRL.OUT` (`fopen(.., "a")`), close it, and
  **return 1 without closing `in` and without MESH.DAT** **[fact]**. The open handle then makes
  the dispatcher's `AX_BIL.KEY` delete fail on Windows, so the file is left **[oracle:
  `GENSUB/brick2` (ECCTMP error) and `GENSUB/tet3d` (return 4) both leave AX_BIL.KEY]**.
* All values read with `%f` are **float** (single precision) and widened to double
  (`CVTPS2PD`) when stored **[fact]**. So MESH.DAT carries float-rounded copies of the KEY values
  **[oracle]**: all 999 RZ values of `GENSUB/quad2d` equal `(double)(float)` of the scanned text
  (e.g. `.5400000000E+001` → `5.4000000954E+000`), reproduced with a correctly rounded
  decimal → double → float conversion.
* Limit `L = 1.0e7` (`0x14051f1a8`): counts must satisfy `1 <= n` and `(double)n < L`.

Shared variables: `obj` (`[RSP+0x118]`, written by every header except ELMCON), `NN` (node count),
`NE` (element count), `C` (`[RSP+0x6fc]`: the RZ count, overwritten by the second count of NDTMP,
STRAIN, DAMAGE and USRNOD headers — and by any substring line). Arrays are `calloc` (zeroed).

### 4.2 Readers **[fact]**

| Block | Header `sscanf` | Error (status 1) when | Rows (`fscanf`), storage |
|---|---|---|---|
| RZ | `"%s %d %d"` → token, obj, C | `C < 1`, `C >= L`, or coordinates already read | `C × 3` doubles; loop until `NN == C`: `"%*d %f %f %f\n"`, stored and `NN++` only when the result ≠ -1. **At EOF this loop never ends** (hang) |
| BCCTMP | `"%s %d %d %f"` → token, obj, cT, (float, unused) | only if `cT > 0`: `cT >= L`, `NN < cT`, or already read | `cT` rows `"%d %d\n"` → pairs (node, code) by row |
| ELMCON | `"%s %d %d %d"` → token (**no strcmp**), objE, n, npe | `n < 1`, `n >= L` or already read → **status = npe** | array `n × npe` ints allocated, `NE = 0`; if `npe != 8` → status = npe; else loop until `NE == n`: `"%*d %d %d %d %d %d %d %d %d\n"` (8 ints), `NE++` when ≠ -1 (EOF hang as RZ) |
| BCCDEF | `"%s %d %d %f"` → token, obj, cD, float | only if `cD > 0`: `cD >= L`, `NN < cD`, or already read | `cD` rows `"%d %d %d %d\n"` → 4 ints by row |
| NDTMP | `"%s %d %d %f"` → token, obj, C, T0 | needs `NN > 0`; only if `C > 0`: `C >= L`, `NN < C`, or already read | `NN` doubles; `C` rows `"%d %f\n"` (node, t), stored at `node-1` if `1 <= node <= NN` |
| STRAIN | `"%s %d %d %f %d %f"` → token, obj, cE, sE, C, sN | needs `NE > 0`; element part: no error (skipped if invalid); node part: only if `C > 0`: `C >= L`, `NN < C`, already read | element part (if `1 <= cE < L`, `cE <= NE`, not read): `NE` doubles, `cE` rows `"%d %f\n"` by element id; node part: `NN` doubles, `C` rows by node id |
| STNCMP | `"%s %d %d %d %f"` → token, obj, c6, (int), (float) | needs `NE > 0`; `c6 < 1`, `>= L`, `NE < c6` or already read, **and `C > 0` (stale C)** | `NE × 6` doubles + list `idx[c6]`; rows `"%d %f %f %f %f %f %f\n"`: if `1 <= e <= NE`: `idx[i] = e`, 6 values at `(e-1)*6` |
| DAMAGE | `"%s %d %d %f %d %f"` → token, obj, cE, dE, C, dN | as STRAIN | as STRAIN |
| GRAIN | `"%s %d %d %d"` → token, obj, cG, (int) | needs `NE > 0`; `cG` invalid **and stale `C > 0`** | `NE × 16` doubles; rows `"%d %f ×16 \n"` by element id |
| USRELM | `"%s %d %d %f %d"` → token, obj, cU, uE, nvE | needs `NE > 0`; only if `cU > 0`: `cU >= L`, `NE < cU`, already read | `nvE × cU` doubles; per row `"%d "` id, then if `1 <= id <= NE`: `nvE` × `"%f"` stored at `[nvE*i + k]` (**row position i**, not id); invalid id → values not consumed. Sets `hasUE = 1` |
| USRNOD | `"%s %d %d %f %d"` → token, obj, C, uN, nvN | needs `NN > 0`; only if `C > 0`: `C >= L`, `NN < C`, already read | `nvN × C` doubles; as USRELM with ids checked against `NN`. Sets `hasUN = 1` |
| ECCTMP | `"%s %d %d %d"` → token, obj, cX, dX; sets `seenX = 1` even if the token differs | needs `NE > 0`; only if `cX > 0`: `cX >= L`, `NE < cX`, already read | `cX × 5` ints; rows `"%d %d %d %d %d %d\n"`: if `1 <= id <= NE` store the 5 values after `id` by row (the 6th value goes to the same variable as `dX`) |
| MATAXI | `"%s %d %d %d"` → token, obj, cM, (int) | needs `NE > 0`; `cM` invalid **and stale `C > 0`** | `NE × 9` doubles + `idx[cM]`; rows `"%d %f %f %f %f %f %f  %f %f %f\n"` as STNCMP with 9 values |

"Already read" = the block's array pointer is non-NULL. Rows whose `fscanf` returns -1 are skipped
but still counted (except in RZ / ELMCON). After the loop's `while (status != 1)`, an ELMCON
status other than 0, 1, 4 (e.g. 8 after an invalid count, or 3) silently disables all later blocks
until EOF; MESH.DAT is then still written **[fact]**.

### 4.3 ELMCON special returns **[fact]**

After the ELMCON block: `status == 4` → **return 4** at once (no message, `in` not closed, no
MESH.DAT): 3D tets (`ELMCON n e 4`). `status == 1` (npe 1) → the error message path.

2D objects: the 2D KEY writer's ELMCON header has no third value, so `npe` keeps its initial 0
→ status 0, `NE = 0`, no element array; every element block is then skipped (`NE > 0` fails).
The 2D RZ rows have two coordinates, but the row format `"%*d %f %f %f\n"` takes three, so the
node rows are read misaligned (the next row's number becomes z) until `C` conversions succeeded,
and the following lines are then scanned from the middle of the RZ data. MESH.DAT is still
written (`ELMCON obj 0 0`, misaligned coordinates). Deterministic, but meaningless **[fact]**.
**[oracle]** `GENSUB/quad2d`: the 999 MESH.DAT RZ values match a simulation of this scan
(`%*d` stops at the `.` of the next number, so `2.5400000000E+001` gives x = `.54...E+001` = 5.4f);
header `ELMCON         1      0      0`, DEF_SUB.INI `1 333 289` / `1 45 1.0`.

### 4.4 Writer (EOF) **[fact]**

`in` is closed; `*o1 = obj`, `*o2 = NN`, `*o3 = NE`; `out = fopen(outName, "wt")` (no NULL test).
Lines in this order (`obj` = last header value, `D` = double, floats widened):

| # | Condition | Format and arguments |
|---|---|---|
| 1 | always | `"RZ        %3d %6d\n"` obj, NN; per node `"%6d %20.10E %20.10E %20.10E\n"` i+1, x, y, z |
| 2 | always | `"ELMCON    %6d %6d %6d\n"` obj, NE, npe; per element `"%6d %6d %6d %6d %6d %6d %6d %6d %6d\n"` i+1, n1..n8 |
| 3 | NDTMP array | `"NDTMP     %3d %6d\n"` obj, NN; per node `"%6d %20.10E \n"` i+1, t[i] |
| 3' | no NDTMP array | same header; per node `"%6d %20.10E \n"` i+1, (D)T0 (header value, 0 if no NDTMP) |
| 4 | STRAIN element array and node array | `"STRAIN    %3d %6d %10.5f %6d %10.5f\n"` obj, NE, (D)sE, NN, (D)sN; `NE` rows `"%6d %20.10E \n"`; `NN` rows the same |
| 4' | element array only | `sN := 0`; header obj, NE, (D)sE, 0, 0.0; `NE` rows |
| 4'' | no element array | header obj, 0, (D)sE, 0, (D)sN; no rows (a node part read alone is dropped) |
| 5 | STNCMP read | `"STNCMP    %3d %6d   6   0.0000\n"` obj, c6; for `i < c6`: `"%6d %20.10E ×6\n"` idx[i], 6 values of element idx[i] (an unused row has idx 0 → values read from before the array: out of bounds) |
| 5' | none | `"STNCMP    %3d       0     6      0.0000\n"` obj |
| 6 | DAMAGE | as STRAIN with `"DAMAGE    %3d %6d %10.5f %6d %10.5f\n"` and dE / dN |
| 7 | GRAIN read | `"GRAIN    %3d %6d  16\n"` obj, NE; per element `"%6d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n"` i+1, 16 values |
| 7' | none | `"GRAIN    %3d %6d  0\n"` obj, NE |
| 8 | USRELM array and hasUE | `"USRELM   %3d %6d %f %3d\n"` obj, cU, (D)uE, nvE; for `k < nvE`, for `i < cU`: `"%6d  %20.10E\n"` i+1, v[nvE*i+k] |
| 8' | else | `"USRELM   %3d %6d  0.0   0\n"` obj, NE |
| 9 | USRNOD array and hasUN | `"USRNOD   %3d %6d %f %3d\n"` obj, **NN**, (D)uN, nvN; for `k < nvN`, for `i < NN`: `"%3d  %20.10E\n"` i+1, v[nvN*i+k] (array has only `nvN × C` entries: reads past it when `C < NN`) |
| 9' | else | `"USRNOD   %3d %6d  0.0   0\n"` obj, NN |
| 10 | `cX == 0` and `cT > 0` | `"BCCTMP    %3d %6d\n"` obj, cT; rows `"%6d %6d \n"` node, code |
| 11 | `seenX` and `cX >= 1` | `"ECCTMP   %3d %6d  0\n"` obj, cX; rows `"%6d %6d %6d %6d %6d %3d \n"` i+1, 5 stored ints |
| 11' | `seenX` and `cX < 1` | `"ECCTMP    %3d  0  %3d\n"` obj, dX |
| 12 | `cD != 0` | `"BCCDEF    %3d %6d\n"` obj, cD; rows `"%6d %6d %6d %6d\n"` 4 ints |
| 12' | `cD == 0` | `"BCCDEF    %3d %6d\n"` obj, 1; one row `"%3d %6d %6d %6d\n"` 1, 0, 0, 0 |
| 13 | always | `"MESHNO    %3d\n"` *meshNo (= 1 from GENSUB) |
| 14 | MATAXI read | `"MATAXI    %3d %6d   9   \n"` obj, cM; rows `"%6d %20.10E ×9\n"` idx[i], 9 values |
| 14' | none | `"MATAXI    %3d       0     9      \n"` obj |

Then `fclose(out)`, free all arrays, return 0. BCCTMP is written only when the ECCTMP count is 0
(normally the KEY writer emits ECCTMP with a count, so BCCTMP rows are dropped) **[fact]**.
Printing uses MSVC `printf` (`%E` with a 3-digit exponent, e.g. `1.0000000000E+002`).

## 5. Outputs

| Case | Files | Status |
|---|---|---|
| `objNo == 0` | `DEF_SUB.INI` deleted | 0 |
| object missing | `DEF_SUB.INI` created empty (left open) | 2 ("Reading Error") |
| 8-node mesh, every block valid | `MESH.DAT`, `DEF_SUB.INI` written; `AX_BIL.KEY`, `NEWBRICK.KEY` deleted | 0 |
| 3D tet mesh | no MESH.DAT; `DEF_SUB.INI` written; `AX_BIL.KEY` left (open handle; oracle) | 0 |
| 2D object | MESH.DAT with misaligned RZ and no elements (4.3); `DEF_SUB.INI` written | 0 |
| converter read error | `DEF_CTRL.OUT` appended, no MESH.DAT, `AX_BIL.KEY` left; `DEF_SUB.INI` written | 0 |
| object without mesh | converter error (`RZ co-ordinates`, if the KEY block has no valid RZ), then access violation in step 8 | crash |

No stdout messages.

## 6. Helpers

| Address | Role | Size | Status |
|---|---|---:|---|
| `FUN_1404154c0` | `%i %Flf` reader | | group2-helpers.md |
| `FUN_14047b934` | `fopen` wrapper (`_fsopen`) | 11 | read |
| `FUN_1401b85d0` | delete file if it exists | 91 | read fully |
| `FUN_14029e250` | per-object KEY writer | | WRTOBJ.md (other worker) |
| `FUN_14037e5c0` | AX_BIL.KEY → MESH.DAT converter | 15708 | read fully (decompile + listing of all I/O calls) |
| `FUN_1401b83d0`, `FUN_1401b8320` | line reader (getc) | 61 / 165 | read fully |
| `FUN_140257150`, `FUN_140256dc0` | calloc doubles / ints (NULL for 0) | 88 / 88 | read fully |
| `FUN_14009e3c0` | `strstr` import thunk (5 bytes; inside the licence address range, but not licence code) | 5 | read |
| `FUN_140183cb0` | connectivity accessor (`+8` after `FUN_1401d5820` sync) | 68 | summarised; `FUN_1401d5820` not followed |

## 7. Open items

* MSVC `scanf("%f")` conversion of the decimal text to float (`_fassign` / `_atoflt`) must be
  reproduced; a correctly rounded `strtof` may differ in rare double-rounding cases. Not checked.
* ~~Whether `remove("AX_BIL.KEY")` fails while the converter left it open~~: it fails, the file
  stays (oracle `brick2`, `tet3d`).
* `FUN_1401d5820` (sync in the connectivity accessor) not followed.
* The external consumer of MESH.DAT / DEF_SUB.INI is not identified.

## 8. Tests

`tests/commands/GENSUB/` (3D brick seed `UPSET_Brick.KEY`, 2D seed `SPIKE_SI.KEY`):
`brick2.KEY` (object 2, value 2.5: its KEY block has `ECCTMP 2 36 0` with only 27 elements,
`NE < cX` → `\tERROR : ECCTMP could not be read` in DEF_CTRL.OUT, no MESH.DAT, AX_BIL.KEY left,
DEF_SUB.INI `2 64 27` / `1 4 2.5` — corrected after oracle run; the first prediction expected
MESH.DAT), `brick1.KEY` (object 1: MESH.DAT, BCCTMP dropped because ECCTMP count 27 > 0, `BCCDEF 1 1` /
`1 0 0 0` default row),
`obj0.KEY` (an earlier GENSUB file then `GENSUB 0` → DEF_SUB.INI deleted), `missing.KEY`
(object 9 → Reading Error, empty DEF_SUB.INI), `quad2d.KEY` (2D object → misaligned MESH.DAT),
`tet3d.KEY` (`UPSET_Tet.KEY`, 4-node → no MESH.DAT), `empty.KEY` (no arguments → delete branch).
