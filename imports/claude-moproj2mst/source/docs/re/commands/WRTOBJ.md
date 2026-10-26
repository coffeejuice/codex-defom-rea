# WRTOBJ (id 9, group 6, `case 9` of `FUN_140378e00`)

Source: `DEF_PRE_64.exe` v11.0.2, static analysis only (`out/bm_cmd.c` lines 1442-1464, the
dispatcher listing at `0x14037c4e2..0x14037c5df`, closure `out/p67/wrtobj.c`). Shared mechanics:
[_common.md](_common.md). Facts are marked **[fact]**, deductions **[inferred]**.

## 1. Summary

Writes the KEY data block of **one object** to a separate text file, using the same per-object
writer as KFWRIT (`FUN_14029e250`), optionally renumbering the object in the written block.
No file header, no simulation-control, material or inter-object data is written. Same code in
2D and 3D (the writer itself branches on the dimension) **[fact]**.

```
WRTOBJ <objNo> [<outNo>]
<file name>
```

## 2. Argument grammar

| Line | Reader | Content |
|---|---|---|
| rest of the WRTOBJ line | `FUN_140413ee0(f, &objNo, &outNo)` | `fgets(1024)`, `,` → space (`FUN_140323970`), `sscanf("%i %i")`; both zeroed first, missing values stay 0; `%i` accepts `0x`/octal prefixes |
| next line | `fgets(buf, 0x400, f)` | file name; trailing spaces / `\r` / `\n` removed by `FUN_140323c50` (scan from the end while the char is `' '`, `'\r'` or `'\n'`; leading blanks and tabs are kept) |

Quirk **[fact]**: the return value of `FUN_140413ee0` is discarded (the call at `0x14037c4fa` is
followed by `CMP [RSP+0x12d4],0`, the dispatcher status `local_56d8._4_4_`, which was set to 0 at
function entry `0x140378e47` and is not written in this case). So an EOF / blank first line is
**not** reported: `objNo = outNo = 0` and the case goes on to read the file-name line.

Lines consumed: always 2 (the keyword line and the file-name line), unless `fgets` of the name
hits EOF → **return 2** ("Reading Error").

## 3. Algorithm

1. `FUN_140413ee0` (status ignored, see above).
2. `fgets(name, 1024)`; NULL → return 2 **[fact]**.
3. `FUN_140323c50(name)` (right trim) **[fact]**.
4. `obj = FUN_14017aaf0(model+0x1380, objNo)` — the object list lookup (body of
   `FUN_1401d6ea0`, used by every command): `for (p = *(model+0x1380); p && *(int*)(p+8) != n;
   p = *(p+0x3940));` returns the first object whose number `+8` equals `n`, else NULL. No
   creation, no type test (a type-7 "empty" object that is in the list is found) **[fact]**.
5. `obj == NULL` → nothing happens, **return 0**, no message, no file **[fact]**.
6. `fp = FUN_14047b934(name, "wt")` = `_fsopen(name, "wt", _SH_DENYNO=0x40)` (mode string at
   `DAT_1405545ec` = `77 74 00` "wt"): text mode, truncates/creates; relative names are relative
   to the process working directory **[fact]**. NULL → **return 1** (the KEY loop stops: 1 is
   neither 0, 2 nor 3, see _common.md §1) **[fact]**.
7. `FUN_14029e250(model, fp, obj, outNo, 0)` (5th argument byte 0 = no file header, listing
   `MOV byte ptr [RSP+0x20],0`); its status is **ignored** **[fact]**.
8. `fclose(fp)` (`0x14047b784`); return 0 **[fact]**.

So the command returns 0 on every path except: EOF on the name line (2), open failure (1).

## 4. The object writer `FUN_14029e250(model, f, obj, outNo, withHeader)`

Fully documented in `docs/re/keywords/group2-writer-map.md` (branch
`claude/project-thread-j4c7t9`); WRTOBJ adds nothing to it. Summary of what matters here:

* `obj+0x3f24 = outNo`; the printed object number `N = outNo`, or `obj+8` when `outNo == 0`.
  Every registry write handler prints `obj+0x3f24` instead of the stored number when it is `> 0`,
  so **`outNo > 0` renumbers the whole block**. `obj+0x3f24` is reset to 0 on every exit
  **[fact]**. A **negative** `outNo` is printed by the inline lines (header comment, PENVOL,
  USRSUB, TRGVOL, ...) as that negative number, while handlers that test `> 0` print the stored
  number: a mixed block **[inferred from the rule above; not traced per handler]**.
* `withHeader = 0`: the file starts directly with `"*\n*  Data for Object #     N\n*\n"`
  (`sprintf("Data for Object #     %i")`, then `fprintf("*\n*  %s\n*\n")`) **[fact]**.
* Write options `model+0x1744` (set by the last KFWRIT/`FUN_1401c5810`; KFWRIT `case 10` passes 0)
  are honoured exactly as in KFWRIT: bit 0x1000 STNOUT and 0x2000 ELMNOD are written **before**
  the object comment, 0x40000 suppresses the movement block, 0x10000 the remesh block, 0x20000
  OTPRNG, 0x1000000 WINPRS for type-5 objects **[fact]**. The value before any KFWRIT is the model
  constructor's default **[open, see §8]**.
* Object type `obj+0xc == 7` (empty): comment + `OBJTYP` only; type 5: comment, OBJTYP, movement
  block, remesh block (if byte `obj+0x2e05 == 0`), WINPRS (option bit) **[fact]**.
* Side effect: the writer **deletes WINTMP windows without a shape** from `obj+0x2ee0`
  (`FUN_1402696f0`, step 49 of the writer map), so WRTOBJ can modify the model **[fact]**.

Keyword order and the handler that writes each (all in group2-writer-map.md; the line format of
each keyword is in its `docs/re/keywords/<KW>.md`):

| Steps | Keywords | Writer |
|---|---|---|
| 1-2 | STNOUT, ELMNOD (option bits only) | `FUN_1403d6580`, `FUN_1403d6620` |
| 3-4 | object comment, OBJTYP | inline, `FUN_1402a25a0` (status ≠ 0 → writer returns 3, ignored by WRTOBJ; file then holds only the comment) |
| 6-17 | OBJNAM, AVGSTR, LMTSTR, PENVOL (3D inline / 2D ZSTR+PENVOL), USRSUB, REFTMP, TMPLMT, ROTSYM (3D), REFPOS, TRGVOL, ELPSOL, OTPRNG | `FUN_1402a2530`, `FUN_1402af420`, `FUN_1402af4f0`, inline / `FUN_1402af5c0`+`FUN_1402b4400`, inline, `FUN_1402af350`, `FUN_1402af6a0`, inline, inline, inline, inline, `FUN_1402af770` |
| 18 | PRSNAM, MOVCTL, SPDLMT, PRSTRE, STROKE, FORCE, ANGMOV, ANGMVY, CNTRAX, ANGMO2, CNTRA2, OBJUPD, OBJSTP | `FUN_1402a7c80` (MOVCTL.md, STROKE.md, OBJSTP.md, ...) |
| 19-22 | DIEGEO, geometry (TPLOGY, CSPMTX, POLYTP, DIEPNT, DIEEDG), FLDGEO, FLWNET | `FUN_1402aaa70`, `FUN_1402a26c0`, `FUN_1402a9f90`, `FUN_1402a2b70` |
| 23-35 | RZ, DRZ, DRMESH, BCCFNC, BCCDEF, BCCDFN, URZ, FRZ, PRZ, BCCTMP, BCCTFN, NDTMP, NDHEAT, NDFLUX, TIMERC, NRECVL, USRNOD | `FUN_1402a2db0`, registry handlers, state-variable writer `FUN_1402a4a80` (group2-statevar-writers.md) |
| 36-38 | ELMCON, MTLGRP, STRAIN, STNCMP, DENSTY, DAMAGE, STRESS, YLDS, USRELM | `FUN_1402a30c0`, `FUN_1402a4a80`, handlers `FUN_1402b4340` … `FUN_1402b5880` |
| 39 | RMDPTH … MGUSER (remesh block, order in MGNELM.md) | `FUN_1402a9180` |
| 40-47 | FRCSTP, FRCNEL, FRCMTH, CRPTIM, VOLFC, VOLFS, DVOLF, GRAIN/GRAIN2, SIZESH, TXTODF, ERECVL, TICF, DATOM, HDNEST, HDNOBJ, BCCCRB, CRBFLX, VOTAGE, BCCRHT, RHTFLX, CSFREQ, VOLCRG, ZEFI, ZMFI, CURRNT | inline + handlers (writer map steps 40-47) |
| 48-54 | WINPRS, WINTMP, WINATM, WINRSE, LOCTMP, LOCATM, LOCDEF, ECCDEF … ECRFLX | inline, `FUN_1402ac560`, `FUN_1402abf20` |
| 55-61 | MATAXI, NWEAR, SYMSUF (3D), WPAXIS, OPSTOP, DYNARY, RSEDEF, custom variables | `FUN_1402a4a80`, `FUN_1402b4d40`, `FUN_1402a3880`, `FUN_1402a39e0`, `FUN_1402a4020`, `FUN_1402a41c0`, `FUN_1402b7140`, `FUN_1402b7310` |

A port should call its KFWRIT per-object writer with `(obj, outNo, withHeader=false)`; the output
of `WRTOBJ n 0` must be byte-identical to object `n`'s block in a KFWRIT file written with the
same option word **[inferred]**.

## 5. Floating-point

None: the command only formats existing values (`%20.10E`, MSVC 3-digit exponent; see
group2-writer-map.md).

## 6. Outputs and errors

| Situation | Result |
|---|---|
| object found, file opened | file written, return 0 (no stdout line from WRTOBJ itself) |
| object not found (incl. `objNo` 0 from a blank line) | nothing, return 0 |
| first line blank / EOF | not detected; continues with objNo = outNo = 0 |
| EOF on the file-name line | return 2 → `ERROR: Keyword WRTOBJ Reading Error. ` |
| `_fsopen` fails (bad directory, empty name, locked file) | return 1 → the KEY loop **stops** |
| writer status 3 (OBJTYP / REFPOS error) | ignored, partial file, return 0 |

## 7. Helpers

| Address | Role | Size | Status |
|---|---|---:|---|
| `FUN_140413ee0` | `%i %i` rest-of-line reader (fgets, `,`→` `, sscanf; returns 1 on EOF / sscanf −1) | 189 | read fully |
| `FUN_140323970` | replace every char `a` by `b` in a string | 111 | read fully |
| `FUN_140323c50` | right-trim `' '`, `\r`, `\n` | 128 | read fully |
| `FUN_14017aaf0` | object list find by number (next at `+0x3940`) | 85 | read fully |
| `FUN_1401d6ea0` | `FUN_14017aaf0(model+0x1380, n)` | 39 | read fully |
| `FUN_14047b934` | `_fsopen(name, mode, 0x40)` | 11 | read fully |
| `fclose` `0x14047b784` | CRT | 118 | library |
| `FUN_14029e250` | per-object KEY writer (248-function closure) | 15247 | summarised from group2-writer-map.md; entry/renumbering code re-read |

## 8. Open items

* Default of the write-option word `model+0x1744` before any KFWRIT (model constructor not
  followed here). With KFWRIT first (as the test scripts do) it is 0.
* Negative `outNo`: per-handler behaviour not traced (see §4).

## 9. Oracle check (2026-10-21)

All 6 cases agree (`docs/re/commands/oracle/ov-misc.md`): `WRTOBJ n 0` output is byte-identical
to object n's block of OUT.KEY (from its comment block `*  Data for Object #` up to the next object's comment;
2D and 3D); `outNo` 5 / 3 renumbers every keyword line and the comment, and the model is
unchanged (OUT2.KEY = OUT.KEY); trailing blanks of the name are trimmed; a missing object writes
nothing; an unopenable path stops the KEY loop (no OUT.KEY), with no message.
